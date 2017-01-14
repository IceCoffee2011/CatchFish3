//
//  CPicturesLayer.cpp
//  CatchFish
//
//  Created by 河北腾游 on 15/6/24.
//
//

#include "CPicturesLayer.h"
#include "CPicturesEditView.h"
#include "ChargeWebView.h"
#include "SimpleAudioEngine.h"
#include "header.h"
using namespace CocosDenshion;

static PicturesEditView * g_PicturesEditView=nil;

extern std::string UserIDStr;
extern bool bTouXiangUpdate;
extern std::string TouXiangName;
extern std::string UserIDStr;
extern long long nOwnUserVersion;

PicturesLayer::PicturesLayer()
{
    
}

PicturesLayer::~PicturesLayer()
{
    [g_PicturesEditView release];
}

bool PicturesLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCLayerColor* pLayer = CCLayerColor::create(ccc4(0, 0, 0, 190));
    this->addChild(pLayer);
    
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    
    m_pSpBg = CCSprite::create("c2harge/userinfobg.png");
    m_pSpBg -> setPosition(ccp(size.width/2, size.height/2));
    this -> addChild(m_pSpBg);
    CCSprite* pFont = CCSprite::create("c2harge/userinfotitle.png");
    pFont->setPosition(ccp(m_pSpBg->getContentSize().width/2, m_pSpBg->getContentSize().height-pFont->getContentSize().height/2-5));
    m_pSpBg->addChild(pFont);
    
    //头像需要通过clippingnode来实现
    
    CCSprite* pHead = CCSprite::create("c2harge/userinfosprite.png");
    pHead->setPosition(ccp(194, m_pSpBg->getContentSize().height/2-10));
    CCSize clipSize = pHead->getContentSize();
    CCSprite* spark1 = NULL;
    if (TouXiangName != "") {
        spark1 = CCSprite::create(TouXiangName.c_str());
    }
    else{
        spark1 = CCSprite::create("h2all/touxiangnv.png");
    }
    spark1->setScale(2.5f);
    spark1->setPosition(pHead->getPosition());
    
    m_pClippingNode = CCClippingNode::create();
    m_pClippingNode->setPosition(ccp(0, 0));
    m_pClippingNode->setAlphaThreshold(0.05f); //设置alpha闸值
    m_pClippingNode->setContentSize(clipSize); //设置尺寸大小
    m_pClippingNode->setStencil(pHead);    //设置模板stencil
    m_pClippingNode->addChild(pHead, 1);   //先添加标题,会完全显示出来,因为跟模板一样大小
    m_pClippingNode->addChild(spark1, 2, 100);      //会被裁减
    m_pSpBg->addChild(m_pClippingNode);

    
    CCSprite* pNickBg = CCSprite::create("c2harge/userinfonamebg.png");
    pNickBg->setPosition(ccp(pHead->getPositionX(), pHead->getPositionY()-pHead->getContentSize().height/2-pNickBg->getContentSize().height/2+10));
    m_pSpBg->addChild(pNickBg);

    CCLabelTTF* pNickTTF = CCLabelTTF::create(CMainLogic::sharedMainLogic()->m_sNickName.c_str(), "Arial", 32);
    pNickTTF->setPosition(pNickBg->getPosition());
    m_pSpBg->addChild(pNickTTF);
    
    CCMenuItemImage* selectedItem = CCMenuItemImage::create("c2harge/userinfoItem1.png", "c2harge/userinfoItem1.png", this, menu_selector(PicturesLayer::selectedMenu));
    selectedItem -> setPosition(ccp(m_pSpBg->getContentSize().width/4*3-30, m_pSpBg->getContentSize().height/2+40));
    CCMenuItemImage* uploadItem = CCMenuItemImage::create("c2harge/userinfoItem2.png", "c2harge/userinfoItem2.png", this, menu_selector(PicturesLayer::uploadMenu));
    uploadItem -> setPosition(ccp(m_pSpBg->getContentSize().width/4*3-30, m_pSpBg->getContentSize().height/2-110));
    CCMenuItemImage* closeItem = CCMenuItemImage::create("freecoin/freecoincloseitem1.png","freecoin/freecoincloseitem1.png",this, menu_selector(PicturesLayer::closeMenu));
    closeItem -> setPosition(ccp(m_pSpBg->getContentSize().width-closeItem->getContentSize().width/2+10, m_pSpBg->getContentSize().height-closeItem->getContentSize().height/2+20));
    m_Menu = CCMenu::create(selectedItem, uploadItem, closeItem, NULL);
    m_Menu -> setPosition(CCPointZero);
    m_Menu -> setAnchorPoint(CCPointZero);
    m_pSpBg -> addChild(m_Menu);
    m_Menu -> setTouchPriority(-200);
    
    pUploadFile = uploadFile::GetInst();
    
    
    g_PicturesEditView = [[PicturesEditView alloc] init];
    [g_PicturesEditView addLocationPicture:this];
    
    this -> setTouchEnabled(true);
    
    return true;
}

void PicturesLayer::selectedMenu(CCObject* object)
{
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallSelectedPicture);
    SimpleAudioEngine::sharedEngine()->playEffect("Music/button.wav");
    if(g_PicturesEditView != nil)
    {
        [g_PicturesEditView EditPictures];
    }
    
}

void PicturesLayer::uploadMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect("Music/button.wav");
    
    m_Menu -> setTouchEnabled(false);
    if(g_PicturesEditView != nil)
    {
        [g_PicturesEditView uploadPictures];
    }
    //上传
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallUploadPicture);
}

void PicturesLayer::uploadImage()
{
    UserInfo userInfo={};
    userInfo.nUserID = atoi(UserIDStr.c_str());
    userInfo.nVersion = nOwnUserVersion;
    char temp[128];
    sprintf(temp, "%s", TouXiangName.c_str());
    pUploadFile->UpLoadFile(temp, &userInfo);
    
    this -> schedule(schedule_selector(PicturesLayer::update2), 0.1f);
}

void PicturesLayer::closeMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect("Music/about.wav");
    if(g_PicturesEditView != nil)
    {
        [g_PicturesEditView backClicked];
    }
    
    this->removeFromParentAndCleanup(true);
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallPictureClose);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void PicturesLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool PicturesLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}


void PicturesLayer::update2(float f)
{
    if(uploadFile::bComplete)
    {
        uploadFile::bComplete = false;
        UIAlertView* alert = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"上传完成" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil] autorelease];
        [alert show];
        this -> unschedule(schedule_selector(PicturesLayer::update2));
        //更新大厅头像
        bTouXiangUpdate = true;
        m_Menu -> setTouchEnabled(true);
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallUploadSuccess);
    }
}
