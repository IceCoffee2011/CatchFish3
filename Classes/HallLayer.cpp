#include "Header.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../platform/android/jni/JniHelper.h"
#include "jni.h"
extern bool bUploadCompleted;
extern std::string meTouXiangPathStr;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "FMLayerWebView.h"
extern int WeiXinCharge;
#endif

extern char* txName[10] ;


enum eControlButtonTag
{
    eSettingTag=200,
    eBackTag,
    eServiceTag,
    eRankTag,
    eTopChargeTag,
    eEnterJCBY=205,
    eEnterNIUNIU,
    eEnterNZNH=207,
    eEnter2NIUNIU,
    eFirstChargeTag,
    eVipTag,
    eJiangQuanTag,
    eMailTag,
    eFreeCoinTag,
    eReceiveChargeGift,
    eDaojuTag,
    eQuickStartGame,
    eUserInfoTag,
    eBeibaoTag,
    eDuihuanTag,
    eLabaTag,
    eEnterFB,
    eGonggaoTag,
    eMatchTag,
    eEnterLKPY,
    ePacketTag,
    eDakasaiTag,
    eWufuTag,
    eShareTag,
    eOneChargeTag,
    eEnterSmallGame,
    eHaidaoTag,
    eGoupaoTag,
};

std::string UserIDStr = "";
extern std::string TouXiangName;
extern bool bTouXiangUpdate;
extern bool bDownLoadOwnUserCompleted;
extern int OwnUserID;

const char* FishZhaYan[4] = {"hallmenu1.png", "hallmenu3.png", "hallmenu5.png", "hallmenu7.png"};
const char* FBRoom[3] = {"h2all/jcfuben1.png", "h2all/jcfuben2.png", "h2all/jcfuben3.png"};

const char* ZhaYanRoom[8] = {"zhayanroom1.png", "zhayanroom2.png", "zhayanroom3.png", "zhayanroom4.png", "zhayanroom5.png", "zhayanroom6.png", "zhayanroom7.png", "zhayanroom8.png"};
const char* GameRoom[4] = {"hall/gameroom1.png", "hall/gameroom2.png", "hall/gameroom3.png", "hall/gameroom4.png"};
const char* MatchScore[4] = {"hall/matchscore1.png", "hall/matchscore2.png", "hall/matchscore3.png", "hall/matchscore4.png"};

CCScene* CHallLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CHallLayer *layer = CHallLayer::create();
    
    layer->setTag(eHallLayerTag);
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CHallLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    srand((unsigned)time(NULL));
    m_bInitCompleted = false;
    m_bCanClickMenu = false;
    m_ptStart = CCPointZero;
    m_sNickName = "";
    m_sMoreGamesInfo = "";
    m_sAdVersionOfRequest = "";
    m_sHallNotice = "";
    m_nUserScore = 0;
    m_nMenuIndex = 0;
    m_nZhayanIndex = 0;
    m_nNoticeIndex = 0;
    m_nServerPort = 0;
    m_bInitNetComplete = false;
    m_arrGameRoom = CCArray::create();
    m_arrGameRoom -> retain();
    m_arrGameType = CCArray::create();
    m_arrGameType ->  retain();
    m_bShowZhayan = false;
    m_bFirstChargeAnimate = false;
    m_bEnterHall = false;
    m_bShowNiuGame = false;
    m_bReceiveHallNotice = false;
    m_bRequestAdvert = false;
    m_bExistRoom = true;
    m_bOpenGonggao = false;
    m_nIsSelectedGameType = 1;
    m_bFBAndPriate = false;
    m_vMoreGamesInfo.clear();
    m_vctStringNotice.clear();
    m_vctLabelNotice.clear();
    m_nRequsetStatus = eUnUse;
    m_nDamageCount = 0;
    m_nMoreGamesCount = 0;
    m_nOwnVersion = 0;
    m_bRequestDamage = false;
    m_nInitNetComplete = false;
    m_vctDefaultNotice.clear();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_vChargeWebView.clear();
#endif
//    m_pFishBowl = NULL;
    
    
    m_spBg = CCSprite::create("h2all/buyubg.jpg");
    m_spBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(m_spBg);
    
    m_nodeGameType = CCNode::create();
    this -> addChild(m_nodeGameType, 10);
    
    m_pVIPBg = CCSprite::create("h3all/userInfoBg.png");
    m_pVIPBg -> setPosition(ccp(m_pVIPBg->getContentSize().width/2+10, CLIENT_HEIGHT-m_pVIPBg->getContentSize().height/2-10));
    this -> addChild(m_pVIPBg, 1);
    m_pVIPBg -> setVisible(false);
    m_pVIPSlider = CCScale9Sprite::create("h2all/vipslider.png");
    m_pVIPSlider->setAnchorPoint(ccp(0, 0.5));
    m_pVIPSlider->setPosition(ccp(107, 16));
    m_pVIPBg->addChild(m_pVIPSlider);
//    m_pVIPHead = CCSprite::create("h2all/vipslider2.png");
//    m_pVIPHead->setPosition(ccp(56, 58));
//    m_pVIPBg->addChild(m_pVIPHead);
    m_pVIPSlider->setContentSize(CCSizeMake(10, m_pVIPSlider->getContentSize().height));
    
//    CCSprite* pUserHeadCircle = CCSprite::create("h3all/userHead.png");
//    pUserHeadCircle->setPosition(ccp(57, 57));
//    m_pVIPBg->addChild(pUserHeadCircle, 2);
    
    m_pSpUserScoreBg = CCSprite::create("h3all/userInfoBg4.png");
    m_pSpUserScoreBg -> setPosition(ccp(m_pVIPBg->getContentSize().width+m_pSpUserScoreBg->getContentSize().width/2+43, CLIENT_HEIGHT-m_pSpUserScoreBg->getContentSize().height/2-12));
    this -> addChild(m_pSpUserScoreBg, 2);
    m_pSpUserMatchBg = CCSprite::create("h3all/userInfoBg6.png");
    m_pSpUserMatchBg -> setPosition(ccp(m_pVIPBg->getContentSize().width+m_pSpUserMatchBg->getContentSize().width/2+43, CLIENT_HEIGHT-m_pSpUserScoreBg->getContentSize().height/2*3-19));
    this -> addChild(m_pSpUserMatchBg, 2);
    CCMenuItemImage* pMatchNum2 = CCMenuItemImage::create("h3all/userInfoBg6.png", "h3all/userInfoBg6.png", this, menu_selector(CHallLayer::buttonCallBack));
    pMatchNum2 -> setPosition(ccp(m_pSpUserMatchBg->getContentSize().width/2, m_pSpUserMatchBg->getContentSize().height/2));
    pMatchNum2 -> setTag(eDakasaiTag);
    CCMenu* pMenuWeekSai = CCMenu::create(pMatchNum2, NULL);
    pMenuWeekSai -> setPosition(CCPointZero);
    pMenuWeekSai -> setAnchorPoint(CCPointZero);
    m_pSpUserMatchBg -> addChild(pMenuWeekSai);
    pMenuWeekSai -> setOpacity(0);
    CCSprite* pSpMatch = CCSprite::create("h2all/matchbei.png");
    pSpMatch -> setPosition(ccp(15, m_pSpUserMatchBg->getContentSize().height/2-1));
    pSpMatch -> setScale(1.0f);
    m_pSpUserMatchBg -> addChild(pSpMatch);
    m_labelMatchNum = CCLabelTTF::create("", "Arial-BoldMT", 28);
    m_labelMatchNum -> setPosition(ccp(420, 627));
    m_labelMatchNum -> setColor(ccc3(255,255,255));
    m_labelMatchNum -> setAnchorPoint(ccp(0, 0.5f));
    this -> addChild(m_labelMatchNum, 3);
    
    //
    CCMenuItemImage* coinItem2 = CCMenuItemImage::create("h3all/userInfoBg4.png", "h3all/userInfoBg4.png", this, menu_selector(CHallLayer::buttonCallBack));
    coinItem2 -> setPosition(ccp(m_pVIPBg->getContentSize().width+m_pSpUserScoreBg->getContentSize().width/2+43, CLIENT_HEIGHT-m_pSpUserScoreBg->getContentSize().height/2-15));
    coinItem2 -> setTag(eTopChargeTag);
    
    CCMenu* coinMenu2 = CCMenu::create(coinItem2, NULL);
    coinMenu2 -> setPosition(CCPointZero);
    coinMenu2 -> setAnchorPoint(CCPointZero);
    this -> addChild(coinMenu2, 1);
    coinMenu2->setOpacity(0);
    
    CCSprite* pSpCoin = CCSprite::create("h2all/coinsprite3.png");
    pSpCoin -> setPosition(ccp(15, m_pSpUserScoreBg->getContentSize().height/2+2));
    pSpCoin -> setScale(0.8f);
    m_pSpUserScoreBg -> addChild(pSpCoin);
    CCMenuItemImage* coinItem = CCMenuItemImage::create("h3all/addItem.png", "h3all/addItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    coinItem -> setPosition(ccp(m_pSpUserScoreBg->getContentSize().width-30, m_pSpUserScoreBg->getContentSize().height/2));
    coinItem -> setTag(eTopChargeTag);
    
    m_menuCoin = CCMenu::create(coinItem, NULL);
    m_menuCoin -> setPosition(CCPointZero);
    m_menuCoin -> setAnchorPoint(CCPointZero);
    m_pSpUserScoreBg -> addChild(m_menuCoin);
    m_pSpUserScoreBg -> setVisible(false);
    m_pSpUserMatchBg -> setVisible(false);
    m_labelMatchNum -> setVisible(false);
    
    //vip
    m_pSpVIP = CCSprite::create("h2all/vipSprite.png");
    m_pSpVIP -> setPosition(ccp(170, 619));
    this -> addChild(m_pSpVIP, 2);
    m_pSpVIP->setScale(1.1f);
    m_pSpVIP->setVisible(false);
    m_labelVIP = CCLabelAtlas::create("", "h2all/vipnumber.png", 19, 23, '0');
    m_labelVIP -> setPosition(ccp(210, 619));
    m_labelVIP -> setAnchorPoint(ccp(0, 0.5f));
    this -> addChild(m_labelVIP, 2);
    m_labelVIP -> setScale(1.2f);
    CCMenuItemImage* vipItem = CCMenuItemImage::create("h2all/vipItem2.png", "h2all/vipItem2.png", this, menu_selector(CHallLayer::buttonCallBack));
    vipItem -> setPosition(ccp(330, 614));
    vipItem -> setTag(eVipTag);
    m_pVipMenu = CCMenu::create(vipItem, NULL);
    m_pVipMenu -> setPosition(CCPointZero);
    m_pVipMenu -> setAnchorPoint(CCPointZero);
    this -> addChild(m_pVipMenu, 2);
    m_pVipMenu->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.7f, ccp(0, 10)), CCMoveBy::create(0.7f, ccp(0, -10)), NULL)));
    m_pVipMenu -> setVisible(false);
    
    
    //昵称
    m_cclNickName = CCLabelTTF::create("", "Arial-BoldMT", 28);
    m_cclNickName->setColor(ccc3(255,255,255));
    m_cclNickName->setPosition(ccp(222, 691));
    //m_cclNickName->setAnchorPoint(ccp(0,0.5));
    this->addChild(m_cclNickName,2);
    
    //UserID
    m_cclUserID = CCLabelTTF::create("", "Arial-BoldMT", 28);
    m_cclUserID->setColor(ccc3(255,255,255));
    m_cclUserID->setPosition(ccp(222,654));
    this->addChild(m_cclUserID,2);
    //m_cclUserID->setVisible(false);
    
    //金币
    m_cclUserScore = CCLabelTTF::create("", "Arial-BoldMT", 28);
    //m_cclUserScore = CCLabelAtlas::create("","CatchFish2001/catchfishnum.png",14,19,'0');
    m_cclUserScore->setColor(ccc3(255,255,255));
    m_cclUserScore->setPosition(ccp(425, 683));
    m_cclUserScore->setAnchorPoint(ccp(0,0.5));
    //m_cclUserScore->setScale(1.3f);
    this->addChild(m_cclUserScore,3);

    
    this->scheduleOnce(schedule_selector(CHallLayer::scheduleBubble), 1.0f);
    this->schedule(schedule_selector(CHallLayer::scheduleBubble), 13);

#if defined(ANYSDK_BD_110000)
    PluginChannel::getInstance()->showToolBar(kToolBarMidLeft);
#endif
	
    return true;
}

void CHallLayer::loadGameType()
{
    //quick start menu animate
    m_quickSprite = CCSprite::create();
    char temp[32];
    CCArray* quickArray = CCArray::create();
    for(int i = 1; i <= 18; i++)
    {
        sprintf(temp, "pirateitem%d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
        quickArray -> addObject(frame);
    }
    CCAnimation* quickAnimation = CCAnimation::createWithSpriteFrames(quickArray, 0.1f);
    CCAnimate* quickAnimate = CCAnimate::create(quickAnimation);
    m_quickSprite -> runAction(CCRepeatForever::create(quickAnimate));
    m_quickSprite -> setPosition(ccp(CLIENT_WIDTH*0.8f-50, 100));
    this -> addChild(m_quickSprite, 3);
    m_quickSprite->setVisible(false);
}

CCAnimate* CHallLayer::getMenuAnimate(int nIndex)
{
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCArray* pArrNiu2 = CCArray::create();
    for(int i = nIndex; i < nIndex+2; i++)
    {
        char temp[16];
        sprintf(temp, "hallmenu%d.png", i+1);
        CCSpriteFrame* frame = cache->spriteFrameByName(temp);
        pArrNiu2->addObject(frame);
    }
    CCAnimation* animation2 = CCAnimation::createWithSpriteFrames(pArrNiu2, 0.1f);
    CCAnimate* animate2 = CCAnimate::create(animation2);
    return animate2;
}

CCAnimate* CHallLayer::getNiuAnimate()
{
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCArray* array = CCArray::create();
    for(int i = 2; i < 4; i++)
    {
        char temp[16];
        sprintf(temp, "hall2menu%d.png", i+1);
        CCSpriteFrame* frame = cache->spriteFrameByName(temp);
        array->addObject(frame);
    }
    CCAnimation* animation1 = CCAnimation::createWithSpriteFrames(array, 0.3f);
    CCAnimate* animate1 = CCAnimate::create(animation1);
    return animate1;
}

void CHallLayer::setMailNumber(int num)
{
    CCMenu* pMenu = (CCMenu*)this->getChildByTag(192);
    if(pMenu != NULL)
    {
        CCArray* array = pMenu->getChildren();
        for(unsigned int i = 0; i < array->count(); i++)
        {
            CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
            if(pItem->getTag() == eMailTag)
            {
                if(num != 0)
                {
                    CCSprite* pMailSprite = (CCSprite*)pItem->getChildByTag(10);
                    if (pMailSprite != NULL)
                    {
//                        m_spMailGuangBg->stopAllActions();
//                        m_sp2MailGuangBg->stopAllActions();
//                        m_spMailGuangBg -> setVisible(true);
//                        m_sp2MailGuangBg -> setVisible(false);
//                        m_spMailGuangBg -> runAction(CCRepeatForever::create(CCRotateBy::create(10.0f, 180)));
//                        m_spMailGuangBg->runAction(CCSequence::create(CCFadeTo::create(0, 150), CCScaleTo::create(0.3f, 0.4f), CCFadeTo::create(0.2f, 255), CCScaleTo::create(0, 0.35f), NULL));
                        pMailSprite->setVisible(true);
                        pMailSprite->stopAllActions();
                        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(CHallLayer::showMailAction2));
                        pMailSprite->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 1.2f),CCSpawn::create(func, CCScaleTo::create(0.5f, 0.9f), NULL), NULL)));
                    }
                }
                else
                {
                    CCSprite* pMailSprite = (CCSprite*)pItem->getChildByTag(10);
                    CCSprite* pMailSprite2 = (CCSprite*)pItem->getChildByTag(11);
                    if (pMailSprite != NULL && pMailSprite2 != NULL)
                    {
//                        m_spMailGuangBg -> setVisible(false);
//                        m_spMailGuangBg->stopAllActions();
//                        m_sp2MailGuangBg -> setVisible(false);
//                        m_sp2MailGuangBg->stopAllActions();
                        pMailSprite->setVisible(false);
                        pMailSprite->stopAllActions();
                        pMailSprite2->setVisible(false);
                        pMailSprite2->stopAllActions();
                    }
                }
            }
        }
    }
}

void CHallLayer::showMailAction2()
{
    CCMenu* pMenu = (CCMenu*)this->getChildByTag(192);
    if(pMenu != NULL)
    {
        CCArray* array = pMenu->getChildren();
        for(unsigned int i = 0; i < array->count(); i++)
        {
            CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
            if(pItem->getTag() == eMailTag)
            {
                CCSprite* pMailSprite2 = (CCSprite*)pItem->getChildByTag(11);
                if (pMailSprite2 != NULL)
                {
                    pMailSprite2->setVisible(true);
                    pMailSprite2->stopAllActions();
                    pMailSprite2->runAction(CCSequence::create(CCFadeTo::create(0, 150), CCScaleTo::create(0.3f, 1.5f), CCFadeTo::create(0.2f, 0), CCScaleTo::create(0, 1.3f), NULL));
//                    m_sp2MailGuangBg->stopAllActions();
//                    m_sp2MailGuangBg -> setVisible(true);
////                    m_sp2MailGuangBg -> runAction(CCRepeatForever::create(CCRotateBy::create(10.0f, 180)));
//                    m_sp2MailGuangBg->runAction(CCSequence::create(CCFadeTo::create(0, 150), CCScaleTo::create(0.3f, 1.2f), CCFadeTo::create(0.2f, 0), CCScaleTo::create(0, 1.0f), NULL));
                }
            }
        }
    }
}

void CHallLayer::showfreecoinAction(bool bShow)
{
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        bShow = false;
    }
    CCMenu* pMenu = (CCMenu*)this->getChildByTag(192);
    if(pMenu != NULL)
    {
        CCArray* array = pMenu->getChildren();
        for(unsigned int i = 0; i < array->count(); i++)
        {
            CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
            if(pItem->getTag() == eFreeCoinTag)
            {
                if(bShow)
                {
                    CCSprite* pMailSprite = (CCSprite*)pItem->getChildByTag(10);
                    if (pMailSprite != NULL)
                    {
                        pMailSprite->setVisible(true);
                        pMailSprite->stopAllActions();
                        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(CHallLayer::showFreecoinAction2));
                        pMailSprite->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 1.2f),CCSpawn::create(func, CCScaleTo::create(0.5f, 0.9f), NULL), NULL)));
                    }
                }
                else
                {
                    CCSprite* pMailSprite = (CCSprite*)pItem->getChildByTag(10);
                    CCSprite* pMailSprite2 = (CCSprite*)pItem->getChildByTag(11);
                    if (pMailSprite != NULL && pMailSprite2 != NULL)
                    {
                        pMailSprite->setVisible(false);
                        pMailSprite->stopAllActions();
                        pMailSprite2->setVisible(false);
                        pMailSprite2->stopAllActions();
                    }
                }
            }
        }
    }
}

void CHallLayer::showFreecoinAction2()
{
    CCMenu* pMenu = (CCMenu*)this->getChildByTag(192);
    if(pMenu != NULL)
    {
        CCArray* array = pMenu->getChildren();
        for(unsigned int i = 0; i < array->count(); i++)
        {
            CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
            if(pItem->getTag() == eFreeCoinTag)
            {
                CCSprite* pMailSprite2 = (CCSprite*)pItem->getChildByTag(11);
                if (pMailSprite2 != NULL)
                {
                    pMailSprite2->setVisible(true);
                    pMailSprite2->stopAllActions();
                    pMailSprite2->runAction(CCSequence::create(CCFadeTo::create(0, 150), CCScaleTo::create(0.3f, 1.5f), CCFadeTo::create(0.2f, 0), CCScaleTo::create(0, 1.3f), NULL));
                }
            }
        }
    }
    
}

void CHallLayer::showVIPAction2()
{
    CCMenu* pMenu = (CCMenu*)this->getChildByTag(192);
    if(pMenu != NULL)
    {
        CCArray* array = pMenu->getChildren();
        for(unsigned int i = 0; i < array->count(); i++)
        {
            CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
            if(pItem->getTag() == eVipTag)
            {
                CCSprite* pMailSprite2 = (CCSprite*)pItem->getChildByTag(11);
                if (pMailSprite2 != NULL)
                {
                    pMailSprite2->setVisible(true);
                    pMailSprite2->stopAllActions();
                    pMailSprite2->runAction(CCSequence::create(CCFadeTo::create(0, 150), CCScaleTo::create(0.3f, 1.5f), CCFadeTo::create(0.2f, 0), CCScaleTo::create(0, 1.3f), NULL));
                }
            }
        }
    }
}

void CHallLayer::showVIPAction(bool bShow)
{
    return;
    CCMenu* pMenu = (CCMenu*)this->getChildByTag(192);
    if(pMenu != NULL)
    {
        CCArray* array = pMenu->getChildren();
        for(unsigned int i = 0; i < array->count(); i++)
        {
            CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
            if(pItem->getTag() == eVipTag)
            {
                if(bShow)
                {
                    CCSprite* pMailSprite = (CCSprite*)pItem->getChildByTag(10);
                    if (pMailSprite != NULL)
                    {
                        pMailSprite->setVisible(true);
                        pMailSprite->stopAllActions();
                        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(CHallLayer::showVIPAction2));
                        pMailSprite->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 1.2f),CCSpawn::create(func, CCScaleTo::create(0.5f, 0.9f), NULL), NULL)));
                    }
                }
                else
                {
                    CCSprite* pMailSprite = (CCSprite*)pItem->getChildByTag(10);
                    CCSprite* pMailSprite2 = (CCSprite*)pItem->getChildByTag(11);
                    if (pMailSprite != NULL && pMailSprite2 != NULL)
                    {
                        pMailSprite->setVisible(false);
                        pMailSprite->stopAllActions();
                        pMailSprite2->setVisible(false);
                        pMailSprite2->stopAllActions();
                    }
                }
            }
        }
    }
}

CHallLayer::~CHallLayer()
{
    m_arrGameRoom -> release();
    m_arrGameType -> release();
//    delete m_pMagicDevice;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    for(int i = 0; i < m_vChargeWebView.size(); i++)
    {
        delete m_vChargeWebView[i];
    }
    m_vChargeWebView.clear();
#endif
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

//加载UI
void CHallLayer::loadUI()
{
    m_pVIPBg -> setVisible(true);
    m_pSpVIP -> setVisible(true);
    m_pVipMenu -> setVisible(true);
    if(CMainLogic::sharedMainLogic()->m_nVipLevel == 9)
    {
        m_pVipMenu -> setVisible(false);
    }
    m_pSpUserScoreBg -> setVisible(true);
    if(CMainLogic::sharedMainLogic()->m_bShowZhouMatch)
    {
        m_labelMatchNum -> setVisible(true);
        m_pSpUserMatchBg -> setVisible(true);
    }
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        m_labelMatchNum -> setVisible(false);
        m_pSpUserMatchBg -> setVisible(false);
    }
    
    float fHeightY = 10;
    int nWidth = 10;
    float fWidthTop = 60;
    
    //推广返现
    CCMenuItemImage* pShareItem = CCMenuItemImage::create("h3all/shareitem.png", "h3all/shareitem.png", this, menu_selector(CHallLayer::buttonCallBack));
    pShareItem -> setPosition(ccp(CLIENT_WIDTH*0.1f-fWidthTop, 525));
    pShareItem -> setTag(eShareTag);
    
    //公告
    CCMenuItemImage* gonggaoItem = CCMenuItemImage::create("h3all/gonggaoItem.png", "h3all/gonggaoItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    gonggaoItem -> setTag(eGonggaoTag);
    gonggaoItem -> setPosition(ccp(CLIENT_WIDTH*0.1f-fWidthTop, 410));//
    CCSprite* pMailSprite7 = CCSprite::create("h2all/mailSprite.png");
    pMailSprite7 -> setPosition(ccp(gonggaoItem->getContentSize().width-30, gonggaoItem->getContentSize().height-15));
    gonggaoItem->addChild(pMailSprite7, 1, 10);
    pMailSprite7->setVisible(false);
    CCSprite* pMailSprite8 = CCSprite::create("h2all/mailSprite.png");
    pMailSprite8 -> setPosition(ccp(gonggaoItem->getContentSize().width-30, gonggaoItem->getContentSize().height-15));
    gonggaoItem->addChild(pMailSprite8, 1, 11);
    pMailSprite8->setVisible(false);
    pMailSprite8 -> setOpacity(150);
    pMailSprite8 -> setScale(1.3f);
    
    //邮件
    CCMenuItemImage* pMailItem = CCMenuItemImage::create("h3all/mailItem.png", "h3all/mailItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    pMailItem -> setPosition(ccp(CLIENT_WIDTH*0.1f-fWidthTop, 295));//
    pMailItem -> setTag(eMailTag);
    CCSprite* pMailSprite = CCSprite::create("h2all/mailSprite.png");
    pMailSprite -> setPosition(ccp(pMailItem->getContentSize().width-30, pMailItem->getContentSize().height-15));
    pMailItem->addChild(pMailSprite, 1, 10);
    pMailSprite->setVisible(false);
    CCSprite* pMailSprite2 = CCSprite::create("h2all/mailSprite.png");
    pMailSprite2 -> setPosition(ccp(pMailItem->getContentSize().width-30, pMailItem->getContentSize().height-15));
    pMailItem->addChild(pMailSprite2, 1, 11);
    pMailSprite2->setVisible(false);
    pMailSprite2 -> setOpacity(150);
    pMailSprite2 -> setScale(1.3f);
    
    //发红包
    CCMenuItemImage* packetItem = CCMenuItemImage::create("h3all/packetItem.png", "h3all/packetItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    packetItem -> setTag(ePacketTag);
    packetItem -> setPosition(ccp(CLIENT_WIDTH*0.1f-fWidthTop, 180));
    
    //客服
    CCMenuItemImage* pServiceItem = CCMenuItemImage::create("h3all/serviceItem.png", "h3all/serviceItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    pServiceItem -> setPosition(ccp(CLIENT_WIDTH*0.1f-fWidthTop, fHeightY));//CLIENT_WIDTH*0.9f+60, 525
    pServiceItem -> setTag(eServiceTag);
    
    //排行
    CCMenuItemImage* pRankItem = CCMenuItemImage::create("h3all/rankItem.png", "h3all/rankItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    pRankItem -> setPosition(ccp(CLIENT_WIDTH*0.2f-fWidthTop-nWidth*1, fHeightY));
    pRankItem -> setTag(eRankTag);
    
    //设置
    CCMenuItemImage* pSettingItem = CCMenuItemImage::create("h3all/settingItem.png", "h3all/settingItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    pSettingItem -> setPosition(ccp(CLIENT_WIDTH*0.3f-fWidthTop-nWidth*2, fHeightY));//
    pSettingItem -> setTag(eSettingTag);
    
    //免费金币
    CCMenuItemImage* pFreeCoinItem = CCMenuItemImage::create("h3all/freeCoinItem.png", "h3all/freeCoinItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    pFreeCoinItem -> setPosition(ccp(CLIENT_WIDTH*0.4f-fWidthTop-nWidth*3, fHeightY));//
    pFreeCoinItem -> setTag(eFreeCoinTag);
    CCSprite* pMailSprite3 = CCSprite::create("h2all/mailSprite.png");
    pMailSprite3 -> setPosition(ccp(pFreeCoinItem->getContentSize().width-30, pFreeCoinItem->getContentSize().height-15));
    pFreeCoinItem->addChild(pMailSprite3, 1, 10);
    pMailSprite3->setVisible(false);
    CCSprite* pMailSprite4 = CCSprite::create("h2all/mailSprite.png");
    pMailSprite4 -> setPosition(ccp(pFreeCoinItem->getContentSize().width-30, pFreeCoinItem->getContentSize().height-15));
    pFreeCoinItem->addChild(pMailSprite4, 1, 11);
    pMailSprite4->setVisible(false);
    pMailSprite4 -> setOpacity(150);
    pMailSprite4 -> setScale(1.3f);
    
    //集五福
    CCMenuItemImage* pWufuItem = CCMenuItemImage::create("h3all/wufuitem.png", "h3all/wufuitem.png", this, menu_selector(CHallLayer::buttonCallBack));
    pWufuItem -> setPosition(ccp(CLIENT_WIDTH*0.5f-fWidthTop-nWidth*4, fHeightY));
    pWufuItem -> setTag(eWufuTag);
    addMenuParticle(pWufuItem);
    
    //背包按钮
    CCMenuItemImage* pDaojuItem = CCMenuItemImage::create("h3all/beibaoItem.png", "h3all/beibaoItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    pDaojuItem->setPosition(ccp(CLIENT_WIDTH*0.6f-fWidthTop-nWidth*5, fHeightY));
    pDaojuItem->setTag(eDaojuTag);
    
    //返回按钮
    CCMenuItemImage* pBackItem = CCMenuItemImage::create("h3all/backItem.png", "h3all/backItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    pBackItem -> setTag(eBackTag);
    pBackItem -> setPosition(ccp(CLIENT_WIDTH*0.9f+60, 650));
    
    //周赛按钮
    CCMenuItemImage* dakasaiItem = CCMenuItemImage::create("h3all/dakasaiItem.png", "h3all/dakasaiItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    dakasaiItem -> setPosition(ccp(CLIENT_WIDTH*0.9f+60, 525));
    dakasaiItem -> setTag(eDakasaiTag);
    addMenuParticle(dakasaiItem);
    
    //VIP
    CCMenuItemImage* pVipItem = CCMenuItemImage::create("h3all/vipItem.png", "h3all/vipItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    pVipItem -> setPosition(ccp(CLIENT_WIDTH*0.9f+60, 410));//
    pVipItem -> setTag(eVipTag);
    CCSprite* pMailSprite5 = CCSprite::create("h2all/mailSprite.png");
    pMailSprite5 -> setPosition(ccp(pVipItem->getContentSize().width-30, pVipItem->getContentSize().height-15));
    pVipItem->addChild(pMailSprite5, 1, 10);
    pMailSprite5->setVisible(false);
    CCSprite* pMailSprite6 = CCSprite::create("h2all/mailSprite.png");
    pMailSprite6 -> setPosition(ccp(pVipItem->getContentSize().width-30, pVipItem->getContentSize().height-15));
    pVipItem->addChild(pMailSprite6, 1, 11);
    pMailSprite6->setVisible(false);
    pMailSprite6 -> setOpacity(150);
    pMailSprite6 -> setScale(1.3f);
    
    //兑换按钮
    CCMenuItemImage* pLotteryItem = CCMenuItemImage::create("h3all/lotteryItem.png", "h3all/lotteryItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    pLotteryItem -> setPosition(ccp(CLIENT_WIDTH*0.9f+60, 295));
    pLotteryItem -> setTag(eJiangQuanTag);
    
    //充值按钮
    CCMenuItemImage* pChargeItem = CCMenuItemImage::create("charge/chargeBg.png", "charge/chargeBg.png", this, menu_selector(CHallLayer::buttonCallBack));
    pChargeItem -> setPosition(ccp(CLIENT_WIDTH*0.8f+60, 650));
    pChargeItem -> setTag(eTopChargeTag);
    addMenuParticle(pChargeItem);
    
    //副本
    CCMenuItemImage* fbItem = CCMenuItemImage::create("h3all/fubenItem.png", "h3all/fubenItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    fbItem -> setPosition(ccp(CLIENT_WIDTH*0.7f-fWidthTop-nWidth*6, fHeightY));
    fbItem -> setTag(eEnterFB);
    
    
    pDaojuItem -> setAnchorPoint(ccp(0.5f, 0));
    pRankItem->setAnchorPoint(ccp(0.5, 0));
    pSettingItem->setAnchorPoint(ccp(0.5, 0));
    pFreeCoinItem->setAnchorPoint(ccp(0.5, 0));
    fbItem->setAnchorPoint(ccp(0.5, 0));
    pServiceItem->setAnchorPoint(ccp(0.5f, 0));
    pWufuItem->setAnchorPoint(ccp(0.5f, 0));
    
    //快速开始按钮
    CCMenuItemImage* pQuickItem = CCMenuItemImage::create("h3all/quickstartby1.png", "h3all/quickstartby1.png", this, menu_selector(CHallLayer::buttonCallBack));
    pQuickItem -> setTag(eQuickStartGame);
    pQuickItem -> setPosition(ccp(CLIENT_WIDTH*0.9f-20, 77));
    //白光遮罩
    CCSprite* pQuickSprite = CCSprite::create("h3all/quickstartby4.png");
    CCSprite* pQuickSprite2 = CCSprite::create("h3all/quickstartby4.png");
    CCClippingNode* pCliper = CCClippingNode::create();
    pCliper->setStencil(pQuickSprite);
    pCliper->setAlphaThreshold(0);
    pQuickItem->addChild(pCliper, 3);
    pQuickSprite2->setOpacity(0);
    pCliper->setPosition(ccp(pQuickSprite->getContentSize().width/2+14, pQuickSprite->getContentSize().height/2+26));
    pCliper->addChild(pQuickSprite2);
    CCSprite* pBaiGuang = CCSprite::create("hall/baiguang.png");
    pBaiGuang -> setPosition(ccp(-200, pQuickSprite->getContentSize().height/2));
    pCliper -> addChild(pBaiGuang);
    pBaiGuang -> runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(4.5f),CCMoveBy::create(1.5f, ccp(400, 0)), CCMoveBy::create(0, ccp(-400, 0)), NULL)));
    
    
    CCMenu* pMenu = CCMenu::create(pBackItem, pSettingItem, pServiceItem, pMailItem, pChargeItem, pLotteryItem, pVipItem, pFreeCoinItem, pRankItem, pDaojuItem, fbItem,gonggaoItem, pQuickItem,packetItem, dakasaiItem, pWufuItem, pShareItem,NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1, 192);
    
    //1元购按钮
    CCMenuItemImage* pOneItem = CCMenuItemImage::create("c2harge/onechargeitem.png", "c2harge/onechargeitem.png", this, menu_selector(CHallLayer::buttonCallBack));
    pOneItem->setTag(eOneChargeTag);
    pOneItem->setPosition(ccp(CLIENT_WIDTH*0.6f+60, 650));
    m_menuOneCharge = CCMenu::create(pOneItem, NULL);
    m_menuOneCharge->setPosition(CCPointZero);
    m_menuOneCharge->setAnchorPoint(CCPointZero);
    m_nodeGameType->addChild(m_menuOneCharge);
    m_menuOneCharge->setVisible(false);

    //首充礼包
    CCMenuItemImage* pFirstItem = CCMenuItemImage::create("h3all/firstchargeItem.png", "h3all/firstchargeItem.png", this, menu_selector(CHallLayer::buttonCallBack));
    pFirstItem->setTag(eFirstChargeTag);
    pFirstItem->setPosition(ccp(CLIENT_WIDTH*0.7f+60, 650));
    CCSprite* pChargeGuang2 = CCSprite::create("h3all/chargeguang.png");
    pChargeGuang2 -> setPosition(ccp(pFirstItem->getContentSize().width/2, pFirstItem->getContentSize().height/2));
    pFirstItem -> addChild(pChargeGuang2, -1);
    pChargeGuang2 -> runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(1.2f, 1.25f), CCScaleTo::create(1.6, 0.5f), NULL)));
    m_menuFirst = CCMenu::create(pFirstItem, NULL);
    m_menuFirst->setPosition(CCPointZero);
    m_menuFirst->setAnchorPoint(CCPointZero);
    m_nodeGameType->addChild(m_menuFirst);
    
    CCMenuItemImage* pFirstReceiveItem = CCMenuItemImage::create("h2all/firstcharge4.png", "h2all/firstcharge4.png", this, menu_selector(CHallLayer::buttonCallBack));
    pFirstReceiveItem->setTag(eReceiveChargeGift);
    pFirstReceiveItem->setPosition(ccp(CLIENT_WIDTH*0.7f+60, 650));
    m_menuReceive = CCMenu::create(pFirstReceiveItem, NULL);
    m_menuReceive -> setPosition(CCPointZero);
    m_menuReceive -> setAnchorPoint(CCPointZero);
    m_nodeGameType -> addChild(m_menuReceive);
    m_labelReceive = CCLabelTTF::create("", "Arial", 20);
    m_labelReceive -> setPosition(ccp(pFirstReceiveItem->getContentSize().width/2, 15));
    pFirstReceiveItem->addChild(m_labelReceive, 2);
    CCSprite* pChargeGuang3 = CCSprite::create("h3all/chargeguang.png");
    pChargeGuang3 -> setPosition(ccp(pFirstReceiveItem->getContentSize().width/2, pFirstReceiveItem->getContentSize().height/2));
    pFirstReceiveItem -> addChild(pChargeGuang3, -1, 101);
    pChargeGuang3 -> runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(1.2f, 1.25f), CCScaleTo::create(1.6, 0.5f), NULL)));
    
    if(CMainLogic::sharedMainLogic()->m_bIOSPassed && CMainLogic::sharedMainLogic()->m_bShowOneCharge && CMainLogic::sharedMainLogic()->m_bCanOneCharge)
    {
        m_menuOneCharge->setVisible(true);
    }
    
    if(CMainLogic::sharedMainLogic()->m_cbCountTimes == 0)
    {
        m_menuReceive->setVisible(false);
        if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
        {
            m_menuFirst -> setVisible(false);
        }
    }
    else if(CMainLogic::sharedMainLogic()->m_cbCountTimes < 11)
    {
        m_menuFirst->setVisible(false);
        if(!CMainLogic::sharedMainLogic()->m_bFirstChargeCanGet)
        {
            m_labelReceive->setString("今日已领取");
            pChargeGuang3->setVisible(false);
        }
        else
        {
            char temp[16];
            sprintf(temp, "可领取%d/10", CMainLogic::sharedMainLogic()->m_cbCountTimes);
            m_labelReceive->setString(temp);
        }
    }
    else
    {
        m_menuFirst->setVisible(false);
        m_menuReceive->setVisible(false);
    }
    
    m_bInitNetComplete = true;
    showQuickStartAnimate();
    fbItem->setVisible(false);
    if(CMainLogic::sharedMainLogic()->m_bShowFuben)
    {
        fbItem -> setVisible(true);
    }
    if(!CMainLogic::sharedMainLogic()->m_bShowShareInfo)
    {
        pShareItem -> setVisible(false);
    }
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        pDaojuItem->setVisible(false);
        pWufuItem -> setVisible(false);
        pLotteryItem->setVisible(false);
        pRankItem->setVisible(false);
        pFirstReceiveItem->setVisible(false);
        fbItem->setVisible(false);
        packetItem->setVisible(false);
        gonggaoItem->setVisible(false);
        dakasaiItem->setVisible(false);
        pShareItem -> setVisible(false);
        pVipItem->setPosition(ccp(CLIENT_WIDTH*0.7f+60, 650));
        pFreeCoinItem->setPosition(ccp(CLIENT_WIDTH*0.6f+60, 650));
        pFreeCoinItem->setAnchorPoint(ccp(0.5, 0.5));
        pServiceItem->setAnchorPoint(ccp(0.5f, 0));
        pMailItem->setAnchorPoint(ccp(0.5f, 0));
        pServiceItem->setPosition(ccp(CLIENT_WIDTH*0.2f-fWidthTop-nWidth, fHeightY));
        pMailItem->setPosition(ccp(CLIENT_WIDTH*0.1f-fWidthTop, fHeightY));
        pSettingItem->setPosition(ccp(CLIENT_WIDTH*0.3f-fWidthTop-nWidth*2, fHeightY));
    }
    else
    {
        if(!CMainLogic::sharedMainLogic()->m_bShowBeibao)//背包没有打开
        {
            pDaojuItem->setVisible(false);
        }
    }
    if(!CMainLogic::sharedMainLogic()->m_bShowZhouMatch)
    {
        dakasaiItem->setVisible(false);
    }
}

void CHallLayer::addMenuParticle(CCMenuItem* pChargeItem)
{
    CCSprite* pStarMiddle = CCSprite::create("charge/chargegamebg1.png");
    pStarMiddle->setPosition(ccp(pChargeItem->getContentSize().width/2-3.5, pChargeItem->getContentSize().height/2+5));
    pStarMiddle->setOpacity(0);
    pStarMiddle->setScale(0.7f);
    pChargeItem->addChild(pStarMiddle, 1);
    CCSequence* seqMiddle =CCSequence::create(CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL), CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(1.2f),NULL);
    pStarMiddle->runAction(CCRepeatForever::create(seqMiddle));
    CCSprite* pStarDownR = CCSprite::create("charge/chargegamebg1.png");
    pStarDownR->setPosition(ccp(pChargeItem->getContentSize().width/2+7, pChargeItem->getContentSize().height/2-20));
    pChargeItem->addChild(pStarDownR, 1);
    pStarDownR->setOpacity(0);
    pStarDownR->setScale(0.7f);
    CCSequence* seqDR = CCSequence::create(CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(1.0f),NULL);
    pStarDownR->runAction(CCRepeatForever::create(seqDR));
    CCSprite* pStarDownL = CCSprite::create("charge/chargegamebg1.png");
    pStarDownL->setPosition(ccp(pChargeItem->getContentSize().width/2-40, pChargeItem->getContentSize().height/2-10));
    pChargeItem->addChild(pStarDownL, 1);
    pStarDownL->setOpacity(0);
    pStarDownL->setScale(0.5f);
    CCSequence* seqDL = CCSequence::create(CCDelayTime::create(0.4f),CCSpawn::create(CCScaleTo::create(0.2f, 1.3),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 0.8),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.8f),NULL);
    pStarDownL->runAction(CCRepeatForever::create(seqDL));
    CCSprite* pStarUpL = CCSprite::create("charge/chargegamebg1.png");
    pStarUpL->setPosition(ccp(pChargeItem->getContentSize().width/2-23, pChargeItem->getContentSize().height/2+45));
    pChargeItem->addChild(pStarUpL, 1);
    pStarUpL->setOpacity(0);
    pStarUpL->setScale(0.7f);
    CCSequence* seqUL = CCSequence::create(CCDelayTime::create(0.8f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.4f),NULL);
    pStarUpL->runAction(CCRepeatForever::create(seqUL));
    CCSprite* pStarUpR = CCSprite::create("charge/chargegamebg1.png");
    pStarUpR->setPosition(ccp(pChargeItem->getContentSize().width/2+31, pChargeItem->getContentSize().height/2-10));
    pChargeItem->addChild(pStarUpR, 1);
    pStarUpR->setOpacity(0);
    pStarUpR->setScale(0.3f);
    CCSequence* seqUR = CCSequence::create(CCDelayTime::create(1.0f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 0.6),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.2f),NULL);
    pStarUpR->runAction(CCRepeatForever::create(seqUR));
    CCSprite* pSan = CCSprite::create("charge/chargegamebg3.png");
    pSan->setPosition(ccp(pChargeItem->getContentSize().width/2, pChargeItem->getContentSize().height/2+10));
    pChargeItem->addChild(pSan, 1);
    pSan->setOpacity(0);
    pSan->setScale(0.5f);
    ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE };
    pSan->setBlendFunc(cbl);
    CCSequence* seqScale = CCSequence::create(CCScaleTo::create(0.56f, 1.06),CCScaleTo::create(0.6f, 1.0),CCScaleTo::create(0.76f, 0.5),CCDelayTime::create(0.08f),NULL);
    CCSequence* seqRotation = CCSequence::create(CCRotateBy::create(1.92f, 0),CCDelayTime::create(0.08f),NULL);
    CCSequence* seqOp = CCSequence::create(CCFadeTo::create(0.48f, 76.5),CCDelayTime::create(0.88f),CCFadeTo::create(0.56f, 0),CCDelayTime::create(0.08f),NULL);
    CCSpawn* spa = CCSpawn::create(seqScale, seqRotation, seqOp, NULL);
    pSan->runAction(CCRepeatForever::create(spa));
    CCSprite* pSan2 = CCSprite::create("charge/chargegamebg3.png");
    pSan2->setPosition(ccp(pChargeItem->getContentSize().width/2, pChargeItem->getContentSize().height/2+10));
    pChargeItem->addChild(pSan2, 1);
    ccBlendFunc cbl2 = {GL_SRC_ALPHA, GL_ONE };
    pSan2->setBlendFunc(cbl2);
    pSan2->setScale(0.5f);
    pSan2->setOpacity(0);
    CCSequence* seqScale2 = CCSequence::create(CCDelayTime::create(0.12f),CCScaleTo::create(0.64f, 1.07),CCScaleTo::create(1.12, 0.5),CCDelayTime::create(0.12f),NULL);
    CCSequence* seqRotation2 = CCSequence::create(CCRotateBy::create(1.88f, 0),CCDelayTime::create(0.12f),NULL);
    CCSequence* seqOp2 = CCSequence::create(CCDelayTime::create(0.2f),CCFadeIn::create(0.56),CCDelayTime::create(0.68),CCFadeOut::create(0.44f),CCDelayTime::create(0.12),NULL);
    CCSpawn* spa2 = CCSpawn::create(seqScale2,seqRotation2, seqOp2, NULL);
    pSan2->runAction(CCRepeatForever::create(spa2));
}

void CHallLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void CHallLayer::loadSelectedGameTypeNode(bool bGameTypeSelected, std::vector<tagGameKind>& vGameKind)
{
    m_nodeSelectedGameType = CCNode::create();
    this -> addChild(m_nodeSelectedGameType,10);
    unsigned long count = vGameKind.size();
    
    CCLayer* containerLayer = CCLayer::create();
    containerLayer -> setContentSize(CCSizeMake(430*count, 400));
    containerLayer -> setAnchorPoint(CCPointZero);
    containerLayer -> setPosition(ccp(0,0));
    m_pScrollViewGameType = CCScrollView::create();
    m_pScrollViewGameType->setPosition(ccp(0,150));     //根据cocostudio调整的位置
    m_pScrollViewGameType->setViewSize(CCSizeMake(CLIENT_WIDTH, 400));//设置view的大小
    m_pScrollViewGameType->setContentOffset(ccp(0,0));
    m_pScrollViewGameType->setContentSize(CCSizeMake(430*count, 400));//设置scrollview区域的大小
    m_pScrollViewGameType->setContainer(containerLayer);
    m_pScrollViewGameType->setDirection(kCCScrollViewDirectionHorizontal);
    m_pScrollViewGameType->setBounceable(false);
    m_nodeSelectedGameType->addChild(m_pScrollViewGameType);
    m_pScrollViewGameType->setTouchEnabled(false);
    //创建游戏类型
    for(unsigned int i = 0; i < 3; i++)
    {
        if(vGameKind[i].wKindID == JCBY_KIND_ID)
        {
            CCSprite* pSpJCBY = CCSprite::create("hall/jinchanbuyu.png");
            if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                pSpJCBY -> setPosition(ccp(CLIENT_WIDTH/2, containerLayer->getContentSize().height/2));
            }
            else
            {
                pSpJCBY -> setPosition(ccp(CLIENT_WIDTH/3*2, containerLayer->getContentSize().height/2));
            }
            
            containerLayer -> addChild(pSpJCBY);
            pSpJCBY -> setTag(eEnterJCBY);
            m_arrGameType -> addObject(pSpJCBY);
            CCSprite* pMatchRoomLabel = CCSprite::create("hall/roommatchlabel.png");
            pMatchRoomLabel -> setPosition(ccp(pSpJCBY->getContentSize().width-pMatchRoomLabel->getContentSize().width/2-12, pSpJCBY->getContentSize().height-pMatchRoomLabel->getContentSize().height/2-15));
            pSpJCBY -> addChild(pMatchRoomLabel);
            if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                pMatchRoomLabel->setVisible(false);
            }
            //白光遮罩
            CCSprite* pQuickSprite = CCSprite::create("hall/jinchanguang.png");
            CCSprite* pQuickSprite2 = CCSprite::create("hall/jinchanguang.png");
            CCClippingNode* pCliper = CCClippingNode::create();
            pCliper->setStencil(pQuickSprite);
            pCliper->setAlphaThreshold(0);
            pSpJCBY->addChild(pCliper, 3);
            pQuickSprite2->setOpacity(0);
            pCliper->setPosition(ccp(pQuickSprite->getContentSize().width/2+11, pQuickSprite->getContentSize().height/2+15));
            pCliper->addChild(pQuickSprite2);
            CCSprite* pBaiGuang = CCSprite::create("hall/baiguang.png");
            pBaiGuang -> setPosition(ccp(-150, pQuickSprite2->getContentSize().height/2));
            pCliper -> addChild(pBaiGuang);
            pBaiGuang -> runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(1.5f), CCMoveBy::create(1.5f, ccp(400, 0)), CCDelayTime::create(3.0f), CCMoveBy::create(0, ccp(-400, 0)), NULL)));
        }
        else if(vGameKind[i].wKindID == NZNH_KIND_ID)
        {
            CCSprite* pSpJCBY = CCSprite::create("hall/nezhanaohai.png");
            if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                pSpJCBY -> setPosition(ccp(CLIENT_WIDTH/4, containerLayer->getContentSize().height/2+5));
            }
            else
            {
                pSpJCBY -> setPosition(ccp(CLIENT_WIDTH/3, containerLayer->getContentSize().height/2+5));
            }
            
            containerLayer -> addChild(pSpJCBY);
            pSpJCBY -> setTag(eEnterNZNH);
            m_arrGameType -> addObject(pSpJCBY);
            CCSprite* pMatchRoomLabel = CCSprite::create("hall/roommatchlabel.png");
            pMatchRoomLabel -> setPosition(ccp(pSpJCBY->getContentSize().width-pMatchRoomLabel->getContentSize().width+23, pSpJCBY->getContentSize().height-pMatchRoomLabel->getContentSize().height/2-33));
            pSpJCBY -> addChild(pMatchRoomLabel);
            if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                pMatchRoomLabel->setVisible(false);
            }
            //白光遮罩
            CCSprite* pQuickSprite = CCSprite::create("hall/jinchanguang.png");
            CCSprite* pQuickSprite2 = CCSprite::create("hall/jinchanguang.png");
            CCClippingNode* pCliper = CCClippingNode::create();
            pCliper->setStencil(pQuickSprite);
            pCliper->setAlphaThreshold(0);
            pSpJCBY->addChild(pCliper, 3);
            pQuickSprite2->setOpacity(0);
            pCliper->setPosition(ccp(pQuickSprite->getContentSize().width/2+11, pQuickSprite->getContentSize().height/2+15));
            pCliper->addChild(pQuickSprite2);
            CCSprite* pBaiGuang = CCSprite::create("hall/baiguang.png");
            pBaiGuang -> setPosition(ccp(-150, pQuickSprite->getContentSize().height/2));
            pCliper -> addChild(pBaiGuang);
            pBaiGuang -> runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(1.5f, ccp(400, 0)), CCDelayTime::create(4.5f),CCMoveBy::create(0, ccp(-400, 0)), NULL)));
        }
        else if(vGameKind[i].wKindID == CJHD_KIND_ID)
        {
            CCSprite* pSmallGame = CCSprite::create("hall/xiaoyouxi.png");
            pSmallGame -> setPosition(ccp(CLIENT_WIDTH/4*3, containerLayer->getContentSize().height/2));
            containerLayer -> addChild(pSmallGame);
            pSmallGame -> setTag(eEnterSmallGame);
            m_arrGameType -> addObject(pSmallGame);
//            CCSprite* pMatchRoomLabel = CCSprite::create("hall/roommatchlabel.png");
//            pMatchRoomLabel -> setPosition(ccp(pSpLKPY->getContentSize().width-pMatchRoomLabel->getContentSize().width/2-12, pSpLKPY->getContentSize().height-pMatchRoomLabel->getContentSize().height/2-13));
//            pSpLKPY -> addChild(pMatchRoomLabel);
//            if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
//            {
//                pMatchRoomLabel->setVisible(false);
//            }
            //白光遮罩
            CCSprite* pQuickSprite = CCSprite::create("hall/jinchanguang.png");
            CCSprite* pQuickSprite2 = CCSprite::create("hall/jinchanguang.png");
            CCClippingNode* pCliper = CCClippingNode::create();
            pCliper->setStencil(pQuickSprite);
            pCliper->setAlphaThreshold(0);
            pSmallGame->addChild(pCliper, 3);
            pQuickSprite2->setOpacity(0);
            pCliper->setPosition(ccp(pQuickSprite->getContentSize().width/2+20, pQuickSprite->getContentSize().height/2+12));
            pCliper->addChild(pQuickSprite2);
            CCSprite* pBaiGuang = CCSprite::create("hall/baiguang.png");
            pBaiGuang -> setPosition(ccp(-150, pQuickSprite2->getContentSize().height/2));
            pCliper -> addChild(pBaiGuang);
            pBaiGuang -> runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(3.0f), CCMoveBy::create(1.5f, ccp(400, 0)), CCDelayTime::create(1.5f), CCMoveBy::create(0, ccp(-400, 0)), NULL)));
        }
        else if(vGameKind[i].wKindID == YQS_KIND_ID)
        {
            CCSprite* pSpLKPY = CCSprite::create("hall/yitiantulong.png");
            pSpLKPY -> setPosition(ccp(CLIENT_WIDTH/4*3, containerLayer->getContentSize().height/2));
            containerLayer -> addChild(pSpLKPY);
            pSpLKPY -> setTag(eEnterLKPY);
            m_arrGameType -> addObject(pSpLKPY);
            CCSprite* pMatchRoomLabel = CCSprite::create("hall/roommatchlabel.png");
            pMatchRoomLabel -> setPosition(ccp(pSpLKPY->getContentSize().width-pMatchRoomLabel->getContentSize().width/2-12, pSpLKPY->getContentSize().height-pMatchRoomLabel->getContentSize().height/2-13));
            pSpLKPY -> addChild(pMatchRoomLabel);
            if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                pMatchRoomLabel->setVisible(false);
            }
            //白光遮罩
            CCSprite* pQuickSprite = CCSprite::create("hall/jinchanguang.png");
            CCSprite* pQuickSprite2 = CCSprite::create("hall/jinchanguang.png");
            CCClippingNode* pCliper = CCClippingNode::create();
            pCliper->setStencil(pQuickSprite);
            pCliper->setAlphaThreshold(0);
            pSpLKPY->addChild(pCliper, 3);
            pQuickSprite2->setOpacity(0);
            pCliper->setPosition(ccp(pQuickSprite->getContentSize().width/2+11, pQuickSprite->getContentSize().height/2+15));
            pCliper->addChild(pQuickSprite2);
            CCSprite* pBaiGuang = CCSprite::create("hall/baiguang.png");
            pBaiGuang -> setPosition(ccp(-150, pQuickSprite2->getContentSize().height/2));
            pCliper -> addChild(pBaiGuang);
            pBaiGuang -> runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(3.0f), CCMoveBy::create(1.5f, ccp(400, 0)), CCDelayTime::create(1.5f), CCMoveBy::create(0, ccp(-400, 0)), NULL)));
        }
    }
    //创建小游戏
    m_nodeSmallGame = CCNode::create();
    this -> addChild(m_nodeSmallGame, 10);
    m_nodeSmallGame -> setVisible(false);
    CCMenuItemImage* haidaoItem = CCMenuItemImage::create("hall/haidaoitem.png", "hall/haidaoitem.png", this, menu_selector(CHallLayer::buttonCallBack));
    haidaoItem -> setTag(eHaidaoTag);
    haidaoItem -> setPosition(ccp(CLIENT_WIDTH/2, 350));
//    CCMenuItemImage* goupaoItem = CCMenuItemImage::create("hall/goupaoitem.png", "hall/goupaoitem.png", this, menu_selector(CHallLayer::buttonCallBack));
//    goupaoItem -> setTag(eGoupaoTag);
//    goupaoItem -> setPosition(ccp(CLIENT_WIDTH/2, 350));
    CCMenu* pSmallGameMenu = CCMenu::create(haidaoItem, NULL);
    pSmallGameMenu -> setPosition(CCPointZero);
    pSmallGameMenu -> setAnchorPoint(CCPointZero);
    m_nodeSmallGame -> addChild(pSmallGameMenu);
    //如果是从海盗或者副本退出游戏的话，那么直接回到房间类型选择界面
    if(CMainLogic::sharedMainLogic()->m_nGameType == 2 || CMainLogic::sharedMainLogic()->m_nGameType == 3 || CMainLogic::sharedMainLogic()->m_nGameType == 6 || CMainLogic::sharedMainLogic()->m_nGameType == 7)
    {
        m_nodeSelectedGameType->setVisible(false);
        m_nIsSelectedGameType = 3;
        m_pScrollViewGameType -> setTouchEnabled(false);
        m_nodeSmallGame -> setVisible(true);
        return ;
    }
    
    if(!bGameTypeSelected)//游戏房间选择界面
    {
        m_nodeSelectedGameType->setVisible(false);
        m_nIsSelectedGameType = 2;
        m_pScrollViewGameType -> setTouchEnabled(false);
        bool bkindID = false;
        for(unsigned int i = 0;i < m_pMainLogic->m_vRoomJCBY.size(); i++)
        {
            if(m_pMainLogic->m_CurrentServer.wKindID == m_pMainLogic->m_vRoomJCBY[i].wKindID)
            {
                m_arrGameRoom -> removeAllObjects();
                setRoomInfo(CMainLogic::sharedMainLogic()->m_vRoomJCBY);
                bkindID = true;
                break;
            }
        }
        if(!bkindID)
        {
            for(unsigned int i = 0;i < m_pMainLogic->m_vRoomNZNH.size(); i++)
            {
                if(m_pMainLogic->m_CurrentServer.wKindID == m_pMainLogic->m_vRoomNZNH[i].wKindID)
                {
                    m_arrGameRoom -> removeAllObjects();
                    setRoomInfo(CMainLogic::sharedMainLogic()->m_vRoomNZNH);
                    bkindID = true;
                    break;
                }
            }
        }
        if(!bkindID)
        {
            for(unsigned int i = 0;i < m_pMainLogic->m_vRoomLKPY.size(); i++)
            {
                if(m_pMainLogic->m_CurrentServer.wKindID == m_pMainLogic->m_vRoomLKPY[i].wKindID)
                {
                    m_arrGameRoom -> removeAllObjects();
                    setRoomInfo(CMainLogic::sharedMainLogic()->m_vRoomLKPY);
                    bkindID = true;
                    break;
                }
            }
        }
    }
}

void CHallLayer::clickEnterGame(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    m_nodeSelectedGameType->setVisible(false);
    m_pScrollViewGameType -> setTouchEnabled(false);
    m_arrGameRoom -> removeAllObjects();
    CCMenuItemImage* item = (CCMenuItemImage*)object;
    switch (item->getTag()) {
        case eEnterJCBY:
        {
            m_nIsSelectedGameType = 2;
            setRoomInfo(CMainLogic::sharedMainLogic()->m_vRoomJCBY);
            break;
        }
        case eEnterNZNH:
        {
            m_nIsSelectedGameType = 2;
            setRoomInfo(CMainLogic::sharedMainLogic()->m_vRoomNZNH);
            break;
        }
        case eEnterLKPY:
        {
            m_nIsSelectedGameType = 2;
            setRoomInfo(CMainLogic::sharedMainLogic()->m_vRoomLKPY);
            break;
        }
        case eEnterSmallGame:
        {
            m_nIsSelectedGameType = 3;
            m_nodeSmallGame -> setVisible(true);
            break;
        }
        default:
            break;
    }
}

bool compare(tagGameServer obj1, tagGameServer obj2)
{
    if (obj1.wServerPort < obj2.wServerPort)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CHallLayer::setRoomInfo(std::vector<tagGameServer> & vGameServer)
{
    m_nInitNetComplete = true;
    if(vGameServer.size() == 0)
    {
        m_bExistRoom = false;
        return;
    }
    std::sort(vGameServer.begin(), vGameServer.end(), compare);
    unsigned int count = (unsigned int)vGameServer.size();
    CCLayer* pContainerLayer = CCLayer::create();
    pContainerLayer->setContentSize(CCSizeMake(320*count, 390));
    m_pScrollView = CCScrollView::create();
    m_pScrollView->setPosition(ccp(0,150));     //根据cocostudio调整的位置
    m_pScrollView->setViewSize(CCSizeMake(CLIENT_WIDTH, 390));//设置view的大小
    m_pScrollView->setContentOffset(ccp(0,0));
    m_pScrollView->setContentSize(CCSizeMake(320*count, 390));//设置scrollview区域的大小
    m_pScrollView->setContainer(pContainerLayer);
    m_pScrollView->setDirection(kCCScrollViewDirectionHorizontal);
    this->addChild(m_pScrollView);
    m_pScrollView->setTouchEnabled(false);
    for(unsigned int i = 0; i < count; i++)
    {
        char roomTypeName[32];
        sprintf(roomTypeName,"hall/gamebottom2.png");
        if(vGameServer[i].wKindID == JCBY_KIND_ID)
        {
            sprintf(roomTypeName,"hall/gamebottom1.png");
        }
        else if(vGameServer[i].wKindID == NZNH_KIND_ID)
        {
            sprintf(roomTypeName,"hall/gamebottom2.png");
        }
        else if(vGameServer[i].wKindID == YQS_KIND_ID)
        {
            sprintf(roomTypeName,"hall/gamebottom3.png");
        }
        CCSprite* pBgSprite = CCSprite::create(roomTypeName);
        if(count < 4)
        {
            pBgSprite->setPosition(ccp(CLIENT_WIDTH/(count+1)*(i+1), 195));//-10*(1-i)
        }
        else
        {
            //pBgSprite->setPosition(ccp(160+320*i, 195));
            if(i<=1)
            {
                pBgSprite->setPosition(ccp(CLIENT_WIDTH/(count+1)*(i+1)+5*(2-i), 195));//+10*(1-i)
            }
            else
            {
                pBgSprite->setPosition(ccp(CLIENT_WIDTH/(count+1)*(i+1)-5*(i-1), 195));//+10*(1-i)
            }
            
        }
        pContainerLayer->addChild(pBgSprite);
        pBgSprite->setTag(vGameServer[i].wServerPort);
        m_arrGameRoom -> addObject(pBgSprite);
        //房间可能为三个，也可能为四个，但是不变的是端口号1万多到4万多
        //显示级别
        int num = vGameServer[i].wServerPort/10000-1;
        if(vGameServer[i].wKindID == JCBY_KIND_ID)
        {
            if(num >= 0 && num <= 3)
            {
                CCSprite* pSpGameJibie = CCSprite::create(GameRoom[vGameServer[i].wServerPort/10000-1]);
                pSpGameJibie->setPosition(ccp(pBgSprite->getContentSize().width/2, 40));
                pBgSprite->addChild(pSpGameJibie);
                CCSprite* pSpMatchScore = CCSprite::create(MatchScore[vGameServer[i].wServerPort/10000-1]);
                pSpMatchScore->setPosition(ccp(pBgSprite->getContentSize().width-pSpMatchScore->getContentSize().width/2-14, pBgSprite->getContentSize().height-pSpMatchScore->getContentSize().height/2-14));
                pBgSprite->addChild(pSpMatchScore);
                if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
                {
                    pSpMatchScore->setVisible(false);
                }
                char temp[16];
                sprintf(temp, "%lld", vGameServer[i].lMinEnterScore);
                CCLabelTTF* pLabelScore = CCLabelTTF::create(temp, "Arial-BoldMT", 24);
                pLabelScore -> setPosition(ccp(155, 85));
                pBgSprite -> addChild(pLabelScore);
            }
        }
        else if(vGameServer[i].wKindID == NZNH_KIND_ID)
        {
            if(num >= 0 && num <= 3)
            {
                CCSprite* pSpGameJibie = CCSprite::create(GameRoom[vGameServer[i].wServerPort/10000-1]);
                pSpGameJibie->setPosition(ccp(pBgSprite->getContentSize().width/2-12, 40));
                pBgSprite->addChild(pSpGameJibie);
                CCSprite* pSpMatchScore = CCSprite::create(MatchScore[vGameServer[i].wServerPort/10000-1]);
                pSpMatchScore->setPosition(ccp(pBgSprite->getContentSize().width-pSpMatchScore->getContentSize().width/2-34, pBgSprite->getContentSize().height-pSpMatchScore->getContentSize().height/2-30));
                pBgSprite->addChild(pSpMatchScore);
                if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
                {
                    pSpMatchScore->setVisible(false);
                }
                char temp[16];
                sprintf(temp, "%lld", vGameServer[i].lMinEnterScore);
                CCLabelTTF* pLabelScore = CCLabelTTF::create(temp, "Arial-BoldMT", 24);
                pLabelScore -> setPosition(ccp(155, 85));
                pBgSprite -> addChild(pLabelScore);
            }
        }
        else if(vGameServer[i].wKindID == YQS_KIND_ID)
        {
            if(num >= 0 && num <= 3)
            {
                CCSprite* pSpGameJibie = CCSprite::create(GameRoom[vGameServer[i].wServerPort/10000-1]);
                pSpGameJibie->setPosition(ccp(pBgSprite->getContentSize().width/2, 40));
                pBgSprite->addChild(pSpGameJibie);
                CCSprite* pSpMatchScore = CCSprite::create(MatchScore[vGameServer[i].wServerPort/10000-1]);
                pSpMatchScore->setPosition(ccp(pBgSprite->getContentSize().width-pSpMatchScore->getContentSize().width/2-12, pBgSprite->getContentSize().height-pSpMatchScore->getContentSize().height/2-13));
                pBgSprite->addChild(pSpMatchScore);
                if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
                {
                    pSpMatchScore->setVisible(false);
                }
                char temp[16];
                sprintf(temp, "%lld", vGameServer[i].lMinEnterScore);
                CCLabelTTF* pLabelScore = CCLabelTTF::create(temp, "Arial-BoldMT", 24);
                pLabelScore -> setPosition(ccp(155, 85));
                pBgSprite -> addChild(pLabelScore);
            }
        }
    }
    
    
    if(CMainLogic::sharedMainLogic()->m_bEnterGiveCoin)
    {
        FreeCoinLayer* pFreeCoinLayer = FreeCoinLayer::create();
        this->addChild(pFreeCoinLayer, 200, 19996);
    }
    if(CMainLogic::sharedMainLogic()->m_bGameToVIP)
    {
        //打开VIP
        CMainLogic::sharedMainLogic()->ShowMessage("正在打开VIP,请稍候", eMBNull);
        sendVIPVersion("VIP");
    }
}

void CHallLayer::setWeekNum(int nWeekNum)
{
    char temp[16];
    sprintf(temp, "本赛季x%d", nWeekNum);
    m_labelMatchNum->setString(temp);
}

void CHallLayer::notice()
{
    m_spNoticeBg = CCSprite::create("h2all/noticeBg.png");
    m_spNoticeBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.77f+10));
    this->addChild(m_spNoticeBg, 1);
    m_labelNoticeStr = CCLabelTTF::create("","Arial",32);//创建通知
    //m_labelNoticeStr -> setColor(ccc3(255,215,0));
    m_labelNoticeStr -> setAnchorPoint(ccp(0, 0.5));
    m_labelNoticeStr -> setPosition(ccp(m_spNoticeBg->getPositionX()+m_spNoticeBg->getContentSize().width/2, CLIENT_HEIGHT*0.77f+10));
    CCDrawNode* pNoticeZone = CCDrawNode::create();//设置滚动通知的显示区域
    CCPoint point[4] = {ccp(CLIENT_WIDTH/2-m_spNoticeBg->getContentSize().width/2+26+30,CLIENT_HEIGHT*0.77f-26+10),ccp(CLIENT_WIDTH/2+m_spNoticeBg->getContentSize().width/2-15,CLIENT_HEIGHT*0.77f-26+10), ccp(CLIENT_WIDTH/2+m_spNoticeBg->getContentSize().width/2-15, CLIENT_HEIGHT*0.77f+26+10),ccp(CLIENT_WIDTH/2-m_spNoticeBg->getContentSize().width/2+26+30,CLIENT_HEIGHT*0.77f+26+10)};
    pNoticeZone->drawPolygon(point,4,ccc4f(255,255,255,255),2,ccc4f(255,255,255,255));
    CCClippingNode* pCliper = CCClippingNode::create();
    pCliper->setStencil(pNoticeZone);
    pCliper->setAnchorPoint(CCPointZero);
    pCliper->addChild(m_labelNoticeStr);
    this->addChild(pCliper, 1);
     
    setNotice(1.0f);
    this -> schedule(schedule_selector(CHallLayer::setNotice), 4.0f);
}

void CHallLayer::setNotice(float dt)
{
    if(m_bReceiveHallNotice)
    {
        this -> unschedule(schedule_selector(CHallLayer::setNotice));
        return;
    }
    CCHttpRequest* request = new CCHttpRequest();
    char temp[128];
    sprintf(temp, "http://%s/GetMobAnn.aspx?id=1", NORMAL_URL);
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setTag("http test");
    request -> setResponseCallback(this, httpresponse_selector(CHallLayer::onHttpComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CHallLayer::onHttpComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if(m_bReceiveHallNotice)
    {
        return;
    }
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        int size = (int)str.find("*");
        while (size != -1) {
            std::string str1 = str.substr(0, size);
            m_vctDefaultNotice.push_back(str1);
            str = str.substr(size+1);
            size = (int)str.find("*");
        }
        int index = (int)m_vctDefaultNotice.size();
        if(index != 0)
        {
            m_sHallNotice = m_vctDefaultNotice[m_nNoticeIndex];
        }
        else
        {
            m_sHallNotice = "";
        }
        m_labelNoticeStr -> setString(m_sHallNotice.c_str());
    }
    else
    {
        m_sHallNotice = "《乐乐捕鱼》是一款新概念的捕鱼游戏，延续了经典的街机电玩捕鱼玩法并提升改进，足不出户让你有比游戏厅还酷爽的体验！";
        m_labelNoticeStr -> setString(m_sHallNotice.c_str());
    }
    
    m_bReceiveHallNotice = true;
    
    checkNoticeCount();
}

void CHallLayer::checkNoticeCount()
{
    if(m_vctStringNotice.size() == 0 || !CMainLogic::sharedMainLogic()->m_bIOSPassed)//公告数量为0
    {
        int index = (int)m_vctDefaultNotice.size();
        if(index != 0)
        {
            m_sHallNotice = m_vctDefaultNotice[m_nNoticeIndex];
            if(m_nNoticeIndex<index-1)
            {
                m_nNoticeIndex++;
            }
            else
            {
                m_nNoticeIndex = 0;
            }
        }
        else
        {
            if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                m_sHallNotice = "";
            }
            else
            {
                m_sHallNotice = "《乐乐捕鱼》是一款新概念的捕鱼游戏，延续了经典的街机电玩捕鱼玩法并提升改进，足不出户让你有比游戏厅还酷爽的体验！";
            }
        }
        m_labelNoticeStr -> setString(m_sHallNotice.c_str());

        float fWidth = m_labelNoticeStr->getContentSize().width;
        CCMoveTo* pMoveTo0 = CCMoveTo::create((fWidth+m_spNoticeBg->getContentSize().width-71)/CMainLogic::sharedMainLogic()->m_fNoticeSpeed, ccp(m_spNoticeBg->getPositionX()-m_spNoticeBg->getContentSize().width/2-m_labelNoticeStr->getContentSize().width, CLIENT_HEIGHT*0.77f+10));
        CCMoveTo* pMoveTo1 = CCMoveTo::create(0, ccp(m_spNoticeBg->getPositionX()+m_spNoticeBg->getContentSize().width/2, CLIENT_HEIGHT*0.77f+10));
        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(CHallLayer::checkNoticeCount));
        CCSequence* pSeq = CCSequence::create(pMoveTo0, pMoveTo1,func, NULL);
        m_labelNoticeStr->runAction(pSeq);
    }
    else//公告数量不为0，那么就播放其他的公告
    {
        showHallNotice();
    }
}

//公告展现的四种字符串
/*
 1、玩家dddeee打中了金蟾
 2、恭喜玩家【#ff0000dddeee1#ffffff】升级到【#ff0000VIP1#ffffff】
 3、#ffffff恭喜玩家【#ff0000dddeee1#ffffff】升级到【#ff0000VIP1#ffffff】
 4、玩家dddeee打中了金蟾#ff0000
 */
void CHallLayer::showHallNotice()
{
    std::string str = m_vctStringNotice[0].sNotice;
    m_vctStringNotice[0].bHave = false;
    float fWidth=0;
    int size = (int)str.find("#");
    if(size == -1)//说明整个字符串都不包含#
    {
        m_labelNoticeStr->setString(str.c_str());
        fWidth = m_labelNoticeStr->getContentSize().width;
        CCMoveTo* pMoveTo0 = CCMoveTo::create((fWidth+m_spNoticeBg->getContentSize().width-71)/CMainLogic::sharedMainLogic()->m_fNoticeSpeed, ccp(m_spNoticeBg->getPositionX()-m_spNoticeBg->getContentSize().width/2-fWidth, CLIENT_HEIGHT*0.77f+10));
        CCMoveTo* pMoveTo1 = CCMoveTo::create(0, ccp(m_spNoticeBg->getPositionX()+m_spNoticeBg->getContentSize().width/2, CLIENT_HEIGHT*0.77f+10));
        CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(CHallLayer::resumeNoticeCount));
        CCSequence* pSeq = CCSequence::create(pMoveTo0, pMoveTo1,func2, NULL);
        m_labelNoticeStr->runAction(pSeq);
    }
    else if(size != 0)//说明第一个字符不是#
    {
        VIPNotice VipNotice0;
        VipNotice0.sColor = "";
        VipNotice0.sNotice = "";
        VipNotice0.sTag = "";
        std::string str0 = "";
        str0 = str.substr(0, size);
        std::string strColor0 = str0.substr(1, 6);
        VipNotice0.sColor = "FFFFFF";//默认白色
        VipNotice0.sNotice = str0;
        m_vctLabelNotice.push_back(VipNotice0);
        str = str.substr(size);
        size = (int)str.find("#");
        std::string str1 = "";
        std::string str2 = "";
        while (size != -1)
        {
            if(size == 0)
            {
                VIPNotice VipNotice;
                VipNotice.sColor = "";
                VipNotice.sNotice = "";
                VipNotice.sTag = "";
                //判断是否为L
                if (str.substr(1, 1) == "L")
                {
                    VipNotice.sTag = str.substr(2, 2);
                    VipNotice.sColor = "7CBC3D";
                    str = str.substr(4);
                    size = (int)str.find("#");
                }
                else
                {
                    str2 = str.substr(1, 6);
                    if (str2.length() == 6)//有颜色
                    {
                        VipNotice.sColor = str2;
                    }
                    else   //默认颜色
                    {
                        VipNotice.sColor = "FFFF00";
                    }
                    
                    str = str.substr(7);
                    size = (int)str.find("#");
                }
                
                if(size == -1)
                {
                    VipNotice.sNotice = str;
                }
                else if(size != 0)
                {
                    str1 = str.substr(0, size);
                    VipNotice.sNotice = str1;
                    str = str.substr(size);
                    size = (int)str.find("#");
                }
                m_vctLabelNotice.push_back(VipNotice);
            }
        }
        m_labelNoticeStr->setString(m_vctLabelNotice[0].sNotice.c_str());
        int r, g, b = 0;
        std::string s1, s2, s3 = "";
        s1 = m_vctLabelNotice[0].sColor.substr(0, 2);
        s2 = m_vctLabelNotice[0].sColor.substr(2, 2);
        s3 = m_vctLabelNotice[0].sColor.substr(4, 2);
        sscanf(s1.c_str(), "%x", &r);
        sscanf(s2.c_str(), "%x", &g);
        sscanf(s3.c_str(), "%x", &b);
        m_labelNoticeStr->setColor(ccc3(r, g, b));
        fWidth = m_labelNoticeStr->getContentSize().width;
        for(int i = 1; i < m_vctLabelNotice.size(); i++)
        {
            std::string sColor = m_vctLabelNotice[i].sColor;
            std::string sNotice = m_vctLabelNotice[i].sNotice;
            //获取颜色
            int r, g, b = 0;
            std::string str1, str2, str3 = "";
            str1 = sColor.substr(0,2);
            str2 = sColor.substr(2, 2);
            str3 = sColor.substr(4, 2);
            sscanf(str1.c_str(), "%x", &r);
            sscanf(str2.c_str(), "%x", &g);
            sscanf(str3.c_str(), "%x", &b);
            
            CCLabelTTF* pLabel = CCLabelTTF::create(sNotice.c_str(), "Arial", 28);
            pLabel->setColor(ccc3(r, g, b));
            pLabel -> setAnchorPoint(ccp(0, 0.5f));
            pLabel -> setPosition(ccp(fWidth, m_labelNoticeStr->getContentSize().height/2));
            pLabel -> setTag(100+i);
            
            m_labelNoticeStr -> addChild(pLabel);
            fWidth+=pLabel->getContentSize().width;
            if ("" != m_vctLabelNotice[i].sTag)//L
            {
                CCSprite * pLinkline = CCSprite::create("h3all/linkline.png");
                pLinkline->setScaleX(pLabel->getContentSize().width/pLinkline->getContentSize().width);
                pLinkline->setAnchorPoint(ccp(0, 0));
                pLinkline->setPosition(ccp(0, -2));
                pLabel->addChild(pLinkline);
            }
        }
        CCMoveTo* pMoveTo0 = CCMoveTo::create((fWidth+m_spNoticeBg->getContentSize().width-71)/CMainLogic::sharedMainLogic()->m_fNoticeSpeed, ccp(m_spNoticeBg->getPositionX()-m_spNoticeBg->getContentSize().width/2-fWidth, CLIENT_HEIGHT*0.77f+10));
        CCMoveTo* pMoveTo1 = CCMoveTo::create(0, ccp(m_spNoticeBg->getPositionX()+m_spNoticeBg->getContentSize().width/2, CLIENT_HEIGHT*0.77f+10));
        CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(CHallLayer::resumeNoticeCount));
        CCSequence* pSeq = CCSequence::create(pMoveTo0, pMoveTo1, func2, NULL);
        m_labelNoticeStr->runAction(pSeq);
    }
    else//说明第一个字符是#
    {
        std::string str1 = "";
        std::string str2 = "";
        while (size != -1)
        {
            if(size == 0)
            {
                VIPNotice VipNotice;
                VipNotice.sColor = "";
                VipNotice.sNotice = "";
                VipNotice.sTag = "";
                //判断是否为L
                if (str.substr(1, 1) == "L")
                {
                    VipNotice.sTag = str.substr(2, 2);
                    VipNotice.sColor = "7CBC3D";
                    str = str.substr(4);
                    size = (int)str.find("#");
                }
                else
                {
                    str2 = str.substr(1, 6);
                    if (str2.length() == 6)//有颜色
                    {
                        VipNotice.sColor = str2;
                    }
                    else   //默认颜色
                    {
                        VipNotice.sColor = "FFFFFF";
                    }
                    
                    str = str.substr(7);
                    size = (int)str.find("#");
                }
                
                if(size == -1)
                {
                    VipNotice.sNotice = str;
                }
                else if(size != 0)
                {
                    str1 = str.substr(0, size);
                    VipNotice.sNotice = str1;
                    str = str.substr(size);
                    size = (int)str.find("#");
                }
                m_vctLabelNotice.push_back(VipNotice);
            }
        }
        
        m_labelNoticeStr->setString(m_vctLabelNotice[0].sNotice.c_str());
        int r, g, b = 0;
        std::string s1, s2, s3 = "";
        s1 = m_vctLabelNotice[0].sColor.substr(0, 2);
        s2 = m_vctLabelNotice[0].sColor.substr(2, 2);
        s3 = m_vctLabelNotice[0].sColor.substr(4, 2);
        sscanf(s1.c_str(), "%x", &r);
        sscanf(s2.c_str(), "%x", &g);
        sscanf(s3.c_str(), "%x", &b);
        m_labelNoticeStr->setColor(ccc3(r, g, b));
        fWidth = m_labelNoticeStr->getContentSize().width;
        for(int i = 1; i < m_vctLabelNotice.size(); i++)
        {
            std::string sColor = m_vctLabelNotice[i].sColor;
            std::string sNotice = m_vctLabelNotice[i].sNotice;
            //获取颜色
            int r, g, b = 0;
            std::string str1, str2, str3 = "";
            str1 = sColor.substr(0,2);
            str2 = sColor.substr(2, 2);
            str3 = sColor.substr(4, 2);
            sscanf(str1.c_str(), "%x", &r);
            sscanf(str2.c_str(), "%x", &g);
            sscanf(str3.c_str(), "%x", &b);
            
            CCLabelTTF* pLabel = CCLabelTTF::create(sNotice.c_str(), "Arial", 28);
            pLabel->setColor(ccc3(r, g, b));
            pLabel -> setAnchorPoint(ccp(0, 0.5f));
            pLabel -> setPosition(ccp(fWidth, m_labelNoticeStr->getContentSize().height/2));
            pLabel -> setTag(100+i);
            
            m_labelNoticeStr -> addChild(pLabel);
            fWidth+=pLabel->getContentSize().width;
            if ("" != m_vctLabelNotice[i].sTag)//L
            {
                CCSprite * pLinkline = CCSprite::create("h3all/linkline.png");
                pLinkline->setScaleX(pLabel->getContentSize().width/pLinkline->getContentSize().width);
                pLinkline->setScaleY(1.5f);
                pLinkline->setAnchorPoint(ccp(0, 0));
                pLinkline->setPosition(ccp(0, 0));
                pLinkline->setColor(ccc3(255, 0, 0));
                pLabel->addChild(pLinkline);
            }
        }
        CCMoveTo* pMoveTo0 = CCMoveTo::create((fWidth+m_spNoticeBg->getContentSize().width-71)/CMainLogic::sharedMainLogic()->m_fNoticeSpeed, ccp(m_spNoticeBg->getPositionX()-m_spNoticeBg->getContentSize().width/2-fWidth, CLIENT_HEIGHT*0.77f+10));
        CCMoveTo* pMoveTo1 = CCMoveTo::create(0, ccp(m_spNoticeBg->getPositionX()+m_spNoticeBg->getContentSize().width/2, CLIENT_HEIGHT*0.77f+10));
        CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(CHallLayer::resumeNoticeCount));
        CCSequence* pSeq = CCSequence::create(pMoveTo0, pMoveTo1, func2, NULL);
        m_labelNoticeStr->runAction(pSeq);
    }
}

void CHallLayer::resumeNoticeCount()
{
    for(int i = 0; i < m_vctStringNotice.size(); i++)
    {
        if(m_vctStringNotice[i].bHave == false)
        {
            m_vctStringNotice.erase(m_vctStringNotice.begin()+i);
        }
    }
    m_labelNoticeStr->removeAllChildren();
    m_vctLabelNotice.clear();
    checkNoticeCount();
}

void CHallLayer::gameCallBack(CCObject* obj)
{
//    if(m_nIsSelectedGameType==1 && !m_bFBAndPriate)//如果不是点击海盗房间和副本房间
//        return;
    if(!m_bCanClickMenu)
    {
        return;
    }
    CCNode* sceneItem = (CCNode*)obj;
    unsigned short wServerPort = (unsigned short)sceneItem->getTag();
    
    bool bHave = false;
    tagGameServer GameServer = {0};
    for (unsigned int i = 0; i<m_pMainLogic->m_vRoomJCBY.size(); i++)
    {
        GameServer = m_pMainLogic->m_vRoomJCBY[i];
        if (GameServer.wServerPort == wServerPort)
        {
            m_pMainLogic->m_CurrentServer = GameServer;
            CMainLogic::sharedMainLogic()->m_nGameType = 1;
            bHave = true;
            break;
        }
    }
    if(!bHave)
    {
        for (unsigned int i = 0; i<m_pMainLogic->m_vRoomNZNH.size(); i++)
        {
            GameServer = m_pMainLogic->m_vRoomNZNH[i];
            if (GameServer.wServerPort == wServerPort)
            {
                m_pMainLogic->m_CurrentServer = GameServer;
                CMainLogic::sharedMainLogic()->m_nGameType = 5;
                bHave = true;
                break;
            }
        }
    }
    
    if(!bHave)
    {
        for (unsigned int i = 0; i<m_pMainLogic->m_vRoomLKPY.size(); i++)
        {
            GameServer = m_pMainLogic->m_vRoomLKPY[i];
            if (GameServer.wServerPort == wServerPort)
            {
                m_pMainLogic->m_CurrentServer = GameServer;
                CMainLogic::sharedMainLogic()->m_nGameType = 4;
                bHave = true;
                break;
            }
        }
    }
    if(!bHave)
    {
        if(CMainLogic::sharedMainLogic()->m_vRoomPGW.size() != 0)
        {
            if(wServerPort == CMainLogic::sharedMainLogic()->m_vRoomPGW[0].wServerPort)
            {
                GameServer = CMainLogic::sharedMainLogic()->m_vRoomPGW[0];
                m_pMainLogic->m_CurrentServer = GameServer;
                CMainLogic::sharedMainLogic()->m_nGameType = 6;
                bHave = true;
            }
        }
    }
    if(!bHave)
    {
        if(CMainLogic::sharedMainLogic()->m_vRoomRB.size() != 0)
        {
            if(wServerPort == CMainLogic::sharedMainLogic()->m_vRoomRB[0].wServerPort)
            {
                GameServer = CMainLogic::sharedMainLogic()->m_vRoomRB[0];
                m_pMainLogic->m_CurrentServer = GameServer;
                CMainLogic::sharedMainLogic()->m_nGameType = 7;
                bHave = true;
            }
        }
    }
    if(!bHave)
    {
        if(CMainLogic::sharedMainLogic()->m_vRoomFB.size() != 0)
        {
            if(wServerPort == CMainLogic::sharedMainLogic()->m_vRoomFB[0].wServerPort)
            {
                GameServer = CMainLogic::sharedMainLogic()->m_vRoomFB[0];
                m_pMainLogic->m_CurrentServer = GameServer;
                CMainLogic::sharedMainLogic()->m_nGameType = 3;
                bHave = true;
            }
        }
    }
    if(!bHave)
    {
        if(CMainLogic::sharedMainLogic()->m_vRoomPirate.size() != 0)
        {
            if(CMainLogic::sharedMainLogic()->m_vRoomPirate[0].wServerPort == wServerPort)
            {
                GameServer = m_pMainLogic->m_vRoomPirate[0];
                m_pMainLogic->m_CurrentServer = GameServer;
                CMainLogic::sharedMainLogic()->m_nGameType = 2;
                bHave = true;
            }
        }
    }
    if(bHave)
    {
        if(CMainLogic::sharedMainLogic()->m_nGameType == 1)//金蟾
        {
            CMainLogic::sharedMainLogic()->m_nGameRoomType = GameServer.wServerPort/10000-1;
            if(CMainLogic::sharedMainLogic()->m_nGameRoomType == 0)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallRoom1);
            }
            else if(CMainLogic::sharedMainLogic()->m_nGameRoomType == 1)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallRoom2);
            }
            else if(CMainLogic::sharedMainLogic()->m_nGameRoomType == 2)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallRoom3);
            }
            else if(CMainLogic::sharedMainLogic()->m_nGameRoomType == 3)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallRoom4);
            }
        }
        else if(CMainLogic::sharedMainLogic()->m_nGameType == 2)//海盗
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallHaidaoTag);
        }
        else if(CMainLogic::sharedMainLogic()->m_nGameType == 6)//跑狗王
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallPaogouwangTag);
        }
        else if(CMainLogic::sharedMainLogic()->m_nGameType == 3)//副本
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHall3Room1);
        }
        else if(CMainLogic::sharedMainLogic()->m_nGameType == 4)//摇钱树
        {
            CMainLogic::sharedMainLogic()->m_nGameRoomType = GameServer.wServerPort/10000-1;
            if(CMainLogic::sharedMainLogic()->m_nGameRoomType == 0)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHall4Room1);
            }
            else if(CMainLogic::sharedMainLogic()->m_nGameRoomType == 1)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHall4Room2);
            }
            else if(CMainLogic::sharedMainLogic()->m_nGameRoomType == 2)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHall4Room3);
            }
            else if(CMainLogic::sharedMainLogic()->m_nGameRoomType == 3)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHall4Room4);
            }
        }
        else if(CMainLogic::sharedMainLogic()->m_nGameType == 5)//哪吒闹海
        {
            CMainLogic::sharedMainLogic()->m_nGameRoomType = GameServer.wServerPort/10000-1;
            if(CMainLogic::sharedMainLogic()->m_nGameRoomType == 0)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHall5Room1);
            }
            else if(CMainLogic::sharedMainLogic()->m_nGameRoomType == 1)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHall5Room2);
            }
            else if(CMainLogic::sharedMainLogic()->m_nGameRoomType == 2)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHall5Room3);
            }
            else if(CMainLogic::sharedMainLogic()->m_nGameRoomType == 3)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHall5Room4);
            }
        }
        
    }
    if (!bHave)
    {
        m_pMainLogic->ShowMessage("房间信息不存在");
        return;
    }
    
    //这里区分房间的最低分数和最高分数限制
    if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        int nUserScore = atoi(m_cclUserScore->getString());
        if(GameServer.lMaxEnterScore != 0)//说明此房间有最高分限制
        {
            if(nUserScore >= GameServer.lMaxEnterScore)
            {
                FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create(11);
                this -> addChild(pFirstChargeLayer, 200);
                return ;
            }
        }
        
        if(GameServer.wKindID == JCBY_KIND_ID)
        {
            if(GameServer.wServerPort >= 40000)
            {
                if(nUserScore < GameServer.lMinEnterScore)
                {
                    FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create(10);
                    this -> addChild(pFirstChargeLayer, 200);
                    return ;
                }
            }
            else
            {
                if(nUserScore < GameServer.lMinEnterScore)
                {
                    FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create(9);
                    this -> addChild(pFirstChargeLayer, 200);
                    return ;
                }
            }
        }
        else if(GameServer.wKindID == YQS_KIND_ID || GameServer.wKindID == NZNH_KIND_ID)
        {
            if(GameServer.wServerPort >= 40000)
            {
                if(nUserScore < GameServer.lMinEnterScore)
                {
                    FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create(10);
                    this -> addChild(pFirstChargeLayer, 200);
                    return ;
                }
            }
            else
            {
                if(nUserScore < GameServer.lMinEnterScore)
                {
                    FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create(9);
                    this -> addChild(pFirstChargeLayer, 200);
                    return ;
                }
            }
        }
    }
    m_pMainLogic->KIND_ID = GameServer.wKindID;
    if(m_pMainLogic->KIND_ID == JCBY_KIND_ID)
    {
        m_pMainLogic->FishCount = 38;
        m_pMainLogic->DIRPATH1 = "Music/";
        m_pMainLogic->DIRPATH2 = "CatchFish01/";
        m_pMainLogic->DIRPATH3 = "CatchFish01/JCBY/";
        m_pMainLogic->ShowMessage("正在进入游戏房间", eMBNull);
    }
    else if(m_pMainLogic->KIND_ID == NZNH_KIND_ID)
    {
        m_pMainLogic->FishCount = 37;
        m_pMainLogic->DIRPATH1 = "Music/";
        m_pMainLogic->DIRPATH2 = "CatchFish01/";
        m_pMainLogic->DIRPATH3 = "CatchFish01/JCBY/";
        m_pMainLogic->ShowMessage("正在进入游戏房间", eMBNull);
    }
    else if(m_pMainLogic->KIND_ID == FB_KIND_ID)
    {
        m_pMainLogic->FishCount = 38;
        m_pMainLogic->DIRPATH1 = "Music/";
        m_pMainLogic->DIRPATH2 = "CatchFish01/";
        m_pMainLogic->DIRPATH3 = "CatchFish01/JCBY/";
        m_pMainLogic->ShowMessage("正在进入游戏房间", eMBNull);
    }
    else if(m_pMainLogic->KIND_ID == YQS_KIND_ID)
    {
        m_pMainLogic->FishCount = 32;
        m_pMainLogic->DIRPATH1 = "Music/";
        m_pMainLogic->DIRPATH2 = "CatchFish01/";
        m_pMainLogic->DIRPATH3 = "CatchFish01/JCBY/";
        m_pMainLogic->ShowMessage("正在进入游戏房间", eMBNull);
    }
    else
    {
        m_pMainLogic->ShowMessage("正在连接到游戏服务器", eMBNull);
    }
    
    CMainLogic::sharedMainLogic()->m_bEnterGiveCoin = false;
    CMainLogic::sharedMainLogic()->m_bGameToVIP = false;
    CMainLogic::sharedMainLogic()->m_bBYGameToSuper = false;
    CMainLogic::sharedMainLogic()->m_nExitGameToSpecialArm = 0;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CNetInfo * pNetInfo = NULL;
    if (!NATIVE_TEST)
    {
        pNetInfo = m_pMainLogic->m_pNetworkService->Connect(CMainLogic::sharedMainLogic()->m_sNodeIP.c_str(), GameServer.wServerPort, NULL, NULL, true);
    }
    else
    {
        pNetInfo = m_pMainLogic->m_pNetworkService->Connect(PLATFORM_IP, GameServer.wServerPort, NULL, NULL, false);
    }
    //
    //BYTE * pByte = (BYTE *)&GameServer.dwServerAddr;
    //char szServerAddr[32] = {0};
    //sprintf(szServerAddr, "%hhu.%hhu.%hhu.%hhu", pByte[0], pByte[1], pByte[2], pByte[3]);
    //pNetInfo = m_pMainLogic->m_pNetworkService->Connect(szServerAddr, GameServer.wServerPort, NULL, NULL, false);
    
    if(pNetInfo != NULL)
    {
        m_pMainLogic->m_nRoomSocketHandle = m_pMainLogic->m_pNetworkService->GetNetID(pNetInfo);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (!NATIVE_TEST)
    {
        m_pMainLogic->m_nRoomSocketHandle = m_pMainLogic->m_pNetworkService->asynConnect(CMainLogic::sharedMainLogic()->m_sNodeIP.c_str(), GameServer.wServerPort);
    }
    else
    {
        m_pMainLogic->m_nRoomSocketHandle = m_pMainLogic->m_pNetworkService->asynConnect(PLATFORM_IP, GameServer.wServerPort);
    }
#endif
    
    
}

void CHallLayer::buttonCallBack(CCObject* obj)
{
    if(!m_bCanClickMenu)
    {
        return;
    }
    CCMenuItemSprite* pMenuItem = (CCMenuItemSprite*)obj;
    CCMenu* pMenu = (CCMenu*)this->getChildByTag(192);
    if(pMenu != NULL)
    {
        CCArray* array = pMenu->getChildren();
        for(unsigned int i = 0; i < array->count(); i++)
        {
            CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
            if(pItem->getTag() == pMenuItem->getTag())
            {
                pMenuItem -> setScale(1.0f);
                pMenuItem->runAction(CCSequence::create(CCScaleTo::create(0.1f, 0.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
                SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                break;
            }
        }
    }
    
    switch (pMenuItem->getTag()) {
        case eBackTag:
        {
            if(m_nIsSelectedGameType == 1)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallBack);
                
                CMainLogic::sharedMainLogic()->m_CurrentServer.wKindID = 0;
                CMainLogic::sharedMainLogic()->m_bLinkHallEnter = false;
                CMainLogic::sharedMainLogic()->m_bEnterLoading = false;
                CCScene* pLogonScene = CLogonLayer::scene();
                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3f,pLogonScene));
                CMainLogic::sharedMainLogic()->m_pLogonScene = pLogonScene;
                CMainLogic::sharedMainLogic()->m_pHallScene = NULL;
                CMainLogic::sharedMainLogic()->m_pTableScene = NULL;
                
            }
            else if(m_nIsSelectedGameType == 2)//回到金蟾选择界面
            {
                m_nIsSelectedGameType = 1;
                m_nodeSelectedGameType -> setVisible(true);
                m_pScrollViewGameType -> setTouchEnabled(false);
                //将滚动试图隐藏
                this -> removeChild(m_pScrollView);
                m_pScrollView = NULL;
                m_arrGameRoom->removeAllObjects();
            }
            else if(m_nIsSelectedGameType == 3)//回到金蟾选择界面
            {
                m_nIsSelectedGameType = 1;
                m_nodeSelectedGameType -> setVisible(true);
                m_nodeSmallGame -> setVisible(false);
                
            }
            break;
        }
        case eTopChargeTag:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallCharge);
            CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器...", eMBExitGame);
            CMD_GPR_QuerySkill QuerySkill = {0};
            QuerySkill.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_SKILL, SUB_GPR_QUERY_SKILL, &QuerySkill, sizeof(QuerySkill));
            break;
        }
        case eFirstChargeTag:
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            pMenuItem->runAction(CCSequence::create(CCScaleTo::create(0.1f, 0.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallbtFirstCharge);
            FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create();
            this->addChild(pFirstChargeLayer, 200, 19986);
            
            break;
        }
        case eRankTag:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallRank);
            //排行榜
            CMainLogic::sharedMainLogic()->ShowMessage("正在打开排行榜,请稍候", eMBNull);
            sendVIPVersion("RANK");
            break;
        }
        case eServiceTag://客服反馈
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallService);
            FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create(1);
            this->addChild(pFirstChargeLayer, 300, 19986);
            break;
        }
        case eOneChargeTag:
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            pMenuItem->runAction(CCSequence::create(CCScaleTo::create(0.1f, 0.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
            OneMoneyLayer* pOneMoneyLayer = OneMoneyLayer::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(pOneMoneyLayer, 12, 20004);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eOneMoneyHallTag);
            break;
        }
        case eGonggaoTag:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallGonggao);
            if(CCUserDefault::sharedUserDefault()->getBoolForKey("NoticeDataSuccess", false))
            {
                CNoticeOfHall* pNoticeOfHall = CNoticeOfHall::create(true, 1);
                this->addChild(pNoticeOfHall, 300);
                setGonggaoStatus(false);
            }
            else
            {
                m_bOpenGonggao = true;
                CMainLogic::sharedMainLogic()->sendNoticeVersion("hallnotice");
                CMainLogic::sharedMainLogic()->ShowMessage("正在请求，请稍候", eMBExitGame);
            }
            
            break;
        }
        case eSettingTag:
        {
            if(CMainLogic::sharedMainLogic()->m_vRoomRB.size() == 0)
            {
                CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，当前功能暂未开放。您可以试试其他功能。", eMBOK);
                break;
            }
            CCMenuItem* item = CCMenuItem::create();
            item->setTag(CMainLogic::sharedMainLogic()->m_vRoomRB[0].wServerPort);
            gameCallBack(item);
            break;
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallSetting);
            SettingLayer* pSettingLayer = SettingLayer::create();
            this -> addChild(pSettingLayer,200);
            
//            ChargeWebView::openAppStore();
            //CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_pNetworkService->GetNetInfo(CMainLogic::sharedMainLogic()->m_nHallSocketHandle), MDM_KN_COMMAND, SUB_KN_SHUT_MANUAL);
            break;
        }
        case eHaidaoTag:
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            pMenuItem->runAction(CCSequence::create(CCScaleTo::create(0.1f, 0.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
            if(CMainLogic::sharedMainLogic()->m_vRoomPirate.size() == 0)
            {
                CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，当前功能暂未开放。您可以试试其他功能。", eMBOK);
                break;
            }
            m_bFBAndPriate = true;
            CCMenuItem* item = CCMenuItem::create();
            item->setTag(CMainLogic::sharedMainLogic()->m_vRoomPirate[0].wServerPort);
            gameCallBack(item);
            break;
        }
        case eGoupaoTag://跑狗王
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            pMenuItem->runAction(CCSequence::create(CCScaleTo::create(0.1f, 0.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
            if(CMainLogic::sharedMainLogic()->m_vRoomPGW.size() == 0)
            {
                CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，当前功能暂未开放。您可以试试其他功能。", eMBOK);
                break;
            }
            CCMenuItem* item = CCMenuItem::create();
            item->setTag(CMainLogic::sharedMainLogic()->m_vRoomPGW[0].wServerPort);
            gameCallBack(item);
            break;
        }
        case eMatchTag:
        {
            CCUserDefault::sharedUserDefault()->setBoolForKey("ShowMatchGuide", false);
            //新版本比赛按钮，打开比赛内容
            CRankNewLayer* pRankNewLayer = CRankNewLayer::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(pRankNewLayer, 10, 10200);
            break;
        }
        case eEnterFB:
        {
            if(CMainLogic::sharedMainLogic()->m_vRoomFB.size() == 0)
            {
                CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，当前功能暂未开放。您可以试试其他功能。", eMBOK);
                break;
            }
            m_bFBAndPriate = true;
            CCMenuItem* item = CCMenuItem::create();
            item->setTag(CMainLogic::sharedMainLogic()->m_vRoomFB[0].wServerPort);
            gameCallBack(item);
            break;
        }
        case eDaojuTag:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallBeibao);
            CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器，请稍候", eMBExitGame);
            CMD_GPR_QueryItemCount QueryItemCount;
            QueryItemCount.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_ITEM, SUB_GPR_QUERY_ITEM_COUNT, &QueryItemCount,sizeof(QueryItemCount));
            break;
        }
		case eVipTag:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallVIP);
            CMainLogic::sharedMainLogic()->m_nGameToHallVipLevel = CMainLogic::sharedMainLogic()->m_nVipLevel;
            if(CMainLogic::sharedMainLogic()->m_nVipLevel == 0)
            {
                CMainLogic::sharedMainLogic()->m_nGameToHallVipLevel = 1;
            }
            CMainLogic::sharedMainLogic()->ShowMessage("正在打开VIP,请稍候", eMBNull);
            sendVIPVersion("VIP");
            
            break;
        }
        case eJiangQuanTag:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallLottery);
            CMainLogic::sharedMainLogic()->ShowMessage("正在打开商城,请稍候", eMBNull);
            sendVIPVersion("LOTTERY");
            break;
        }
        case eMailTag:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallMail);
            this -> unschedule(schedule_selector(CMainLogic::requestMail));//取消定时器，等面板关闭时再开启
            MailLayer* pMailLayer = MailLayer::create();
            this -> addChild(pMailLayer, 200, 19998);
            break;
        }
        case eFreeCoinTag:
        {
            CCScene* scene = H3CardBeteLayer::scene();
            CCDirector::sharedDirector()->replaceScene(scene);
            CMainLogic::sharedMainLogic()->m_pHallScene = NULL;
            break;
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallFreecoin);
            FreeCoinLayer* pFreeCoinLayer = FreeCoinLayer::create();
            this->addChild(pFreeCoinLayer, 200, 19996);
            break;
        }
        case eReceiveChargeGift://发送领取充值礼包消息
        {
            pMenuItem->runAction(CCSequence::create(CCScaleTo::create(0.1f, 0.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
            if(!CMainLogic::sharedMainLogic()->m_bFirstChargeCanGet)//不能领取
            {
                CMainLogic::sharedMainLogic()->ShowMessage("您今天已经领取过了，请明天再来领取吧。", eMBOK);
                break;
            }
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallReceiveFirstCharge);
            CMD_GPR_Charge1stGet Charge1stGet;
            Charge1stGet.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_CHARGE1ST_GET,&Charge1stGet,sizeof(Charge1stGet));
            CMainLogic::sharedMainLogic()->ShowMessage("正在领取，请稍候", eMBExitGame);
            
            break;
        }
        case eQuickStartGame://快速开始
        {
            std::sort(CMainLogic::sharedMainLogic()->m_vRoomJCBY.begin(), CMainLogic::sharedMainLogic()->m_vRoomJCBY.end(), compare);
            std::sort(CMainLogic::sharedMainLogic()->m_vRoomNZNH.begin(), CMainLogic::sharedMainLogic()->m_vRoomNZNH.end(), compare);
            //std::sort(CMainLogic::sharedMainLogic()->m_vRoomLKPY.begin(), CMainLogic::sharedMainLogic()->m_vRoomLKPY.end(), compare);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallQuick);
            pMenuItem->runAction(CCSequence::create(CCScaleTo::create(0.1f, 0.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
            //选择房间是根据当前分数进入合适房间，大于等于50*nCellScore
            //分数小于15000进入新手房
            //分数在15000和50000进入百炮房
            //分数在50000和50万进入千炮房
            //分数大于50万进入万炮房
            m_nServerPort = 0;
            int nUserScore = atoi(m_cclUserScore->getString());
            if(nUserScore <= 30000)//进入哪吒新手房
            {
                if(CMainLogic::sharedMainLogic()->m_vRoomNZNH.size() != 0)
                {
                    m_nServerPort = CMainLogic::sharedMainLogic()->m_vRoomNZNH[0].wServerPort;
                }
            }
            else if(nUserScore >= 1000000)//进入李逵万炮或者金蟾万炮或者哪吒万炮
            {
                int num = 3;
                if(CMainLogic::sharedMainLogic()->m_vRoomJCBY.size() == 3)
                {
                    num = 2;
                }
                if(num == 3)
                {
                    int index = rand()%2;
                    if(index == 0)
                    {
                        m_nServerPort = CMainLogic::sharedMainLogic()->m_vRoomJCBY[CMainLogic::sharedMainLogic()->m_vRoomJCBY.size()-1].wServerPort;
                    }
                    else if(index == 1)
                    {
                        if(CMainLogic::sharedMainLogic()->m_vRoomNZNH.size() != 0)
                        {
                            m_nServerPort = CMainLogic::sharedMainLogic()->m_vRoomNZNH[CMainLogic::sharedMainLogic()->m_vRoomNZNH.size()-1].wServerPort;
                        }
                    }
//                    else
//                    {
//                        if(CMainLogic::sharedMainLogic()->m_vRoomLKPY.size() != 0)
//                        {
//                            m_nServerPort = CMainLogic::sharedMainLogic()->m_vRoomLKPY[CMainLogic::sharedMainLogic()->m_vRoomLKPY.size()-1].wServerPort;
//                        }
//                    }
                }
                else
                {
                    m_nServerPort = CMainLogic::sharedMainLogic()->m_vRoomNZNH[CMainLogic::sharedMainLogic()->m_vRoomJCBY.size()-1].wServerPort;
                    /*
                    int index = rand()%2;
                    if(index == 0)
                    {
                        m_nServerPort = CMainLogic::sharedMainLogic()->m_vRoomNZNH[CMainLogic::sharedMainLogic()->m_vRoomJCBY.size()-1].wServerPort;
                    }
                    else if(index == 1)
                    {
                        if(CMainLogic::sharedMainLogic()->m_vRoomLKPY.size() != 0)
                        {
                            m_nServerPort = CMainLogic::sharedMainLogic()->m_vRoomLKPY[CMainLogic::sharedMainLogic()->m_vRoomLKPY.size()-1].wServerPort;
                        }
                    }*/
                }
            }
            else//随机进入金蟾百炮或者李逵百炮或哪吒百炮
            {
                int index = rand()%2;
                if(index == 0)
                {
                    int nCellScore[4] = {0, 2000, 10000, 20000};
                    if(CMainLogic::sharedMainLogic()->m_vRoomJCBY.size() != 0)
                    {
                        for(int i = (int)CMainLogic::sharedMainLogic()->m_vRoomJCBY.size()-1; i >= 0 ; i--)
                        {
                            int num = CMainLogic::sharedMainLogic()->m_vRoomJCBY[i].wServerPort/10000-1;
                            if(num >= 4 && num < 0)
                            {
                                continue;
                            }
                            if(nCellScore[num]*50 <= nUserScore)
                            {
                                m_nServerPort = CMainLogic::sharedMainLogic()->m_vRoomJCBY[i].wServerPort;
                                break;
                            }
                        }
                    }
                }
                else if(index == 1)
                {
                    int nCellScore[4] = {0, 600, 10000, 20000};
                    if(CMainLogic::sharedMainLogic()->m_vRoomNZNH.size() != 0)
                    {
                        for(int i = (int)CMainLogic::sharedMainLogic()->m_vRoomNZNH.size()-1; i >= 0; i--)
                        {
                            int num = CMainLogic::sharedMainLogic()->m_vRoomNZNH[i].wServerPort/10000-1;
                            if(num >= 4 && num < 0)
                            {
                                continue;
                            }
                            if(nCellScore[num]*50 <= nUserScore)
                            {
                                m_nServerPort = CMainLogic::sharedMainLogic()->m_vRoomNZNH[i].wServerPort;
                                break;
                            }
                        }
                    }
                }
//                else
//                {
//                    int nCellScore[4] = {0, 600, 10000, 20000};
//                    if(CMainLogic::sharedMainLogic()->m_vRoomLKPY.size() != 0)
//                    {
//                        for(int i = (int)CMainLogic::sharedMainLogic()->m_vRoomLKPY.size()-1; i >= 0; i--)
//                        {
//                            int num = CMainLogic::sharedMainLogic()->m_vRoomLKPY[i].wServerPort/10000-1;
//                            if(num >= 4 && num < 0)
//                            {
//                                continue;
//                            }
//                            if(nCellScore[num]*50 <= nUserScore)
//                            {
//                                m_nServerPort = CMainLogic::sharedMainLogic()->m_vRoomLKPY[i].wServerPort;
//                                break;
//                            }
//                        }
//                    }
//                }
            }
            m_bFBAndPriate = true;
            CCMenuItem* item = CCMenuItem::create();
            item -> setTag(m_nServerPort);
            gameCallBack(item);
            break;
        }
        case eUserInfoTag:
        {
            CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器，请稍候", eMBExitGame);
            CMD_GPR_QueryItemCount QueryItemCount;
            QueryItemCount.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_ITEM, SUB_GPR_QUERY_ITEM_COUNT, &QueryItemCount,sizeof(QueryItemCount));
            break;
        }
        case eDuihuanTag:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallDuihuan);
            FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create(2);
            this -> addChild(pFirstChargeLayer, 200, 19986);
            break;
        }
        case eLabaTag://喇叭界面
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallLaba);
            CMainLogic::sharedMainLogic()->ShowMessage("正在请求，请稍候", eMBExitGame);
            CMD_GPR_LabaTimes LabaTimes = {0};
            LabaTimes.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_LABA_TIMES, &LabaTimes,sizeof(LabaTimes));
            break;
        }
        case ePacketTag://发红包
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallRedPacket);
            RedPacketLayer* pRedPacket = RedPacketLayer::create(1);
            CCDirector::sharedDirector()->getRunningScene()->addChild(pRedPacket, 21, 29998);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSendRedPacketTag);
            break;
        }
        case eDakasaiTag://大咖赛季榜
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallWeekMatch);
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            DakasaiLayer* pDakasaiLayer = DakasaiLayer::create("");
            this -> addChild(pDakasaiLayer, 200);
            break;
        }
        case eWufuTag:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallWufu);
            CMainLogic::sharedMainLogic()->ShowMessage("正在查询五福卡，请稍候", eMBExitGame);
            CMD_GPR_WufuCard WufuCard = {0};
            WufuCard.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_WUFU_CARD, &WufuCard,sizeof(WufuCard));
            break;
        }
        case eShareTag:
        {
            ShareLayer* pShareLayer = ShareLayer::create();
            this -> addChild(pShareLayer, 200, 19982);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareHallTag);
            break;
        }
    }
}

void CHallLayer::clickLink(std::string sTag)
{
    if(sTag == "01")//跳转到VIP
    {
        CMainLogic::sharedMainLogic()->ShowMessage("正在打开VIP,请稍候", eMBNull);
        sendVIPVersion("VIP");
    }
    else if(sTag == "02")//跳转到奖券商城
    {
        CMainLogic::sharedMainLogic()->ShowMessage("正在打开话费商城,请稍候", eMBNull);
        sendVIPVersion("LOTTERY");
    }
}

bool CHallLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_ptStart = pTouch -> getLocation();
//    if(m_bIsSelectedGameType)
//    {
//        
//        if(m_ptStart.y <= 0 || m_ptStart.y >= CLIENT_HEIGHT)
//        {
//            return false;
//        }
//    }
    CCPoint point = pTouch->getLocation();
    if(m_labelNoticeStr != NULL)
    {
        point = m_labelNoticeStr->convertToNodeSpace(point);
        for (int i=0; i<m_vctLabelNotice.size(); i++)
        {
            CCLabelTTF * label = (CCLabelTTF *)m_labelNoticeStr->getChildByTag(100+i);
            if (NULL != label)
            {
                std::string sNotice = label->getString();
                if (!sNotice.find("查看"))
                {
                    CCRect rect = label->boundingBox();
                    if (rect.containsPoint(point))
                    {
                        for (int k = 0; k<sNotice.length(); k++)
                        {
                            char cbNum = sNotice[k];
                            if (cbNum >= '0' && cbNum <= '9')
                            {
                                int nNum = cbNum-'0';
                                CMainLogic::sharedMainLogic()->m_nGameToHallVipLevel = nNum;
                                break;
                            }
                        }
                        clickLink(m_vctLabelNotice[i].sTag);
                        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallNoticeToVIP);
                    }
                }
            }
        }
    }
    return true;
}

void CHallLayer::ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent)
{
    
}

void CHallLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_nIsSelectedGameType == 1)
        return;
    if(m_arrGameRoom->count() == 0)
        return;
}

void CHallLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_pointEnd = pTouch->getLocation();
    CCPoint point = pTouch -> getLocation();
    CCSprite* pMoreSprite = (CCSprite*)this->getChildByTag(1533);
    if(pMoreSprite != NULL)
    {
        CCRect rect = pMoreSprite->boundingBox();
        if(rect.containsPoint(point))
        {
            if(abs((int)(m_ptStart.x-point.x))<10)
            {
                pMoreSprite->runAction(CCSequence::create(CCScaleTo::create(0.1f, 1.5f), CCScaleTo::create(0.1f, 1.0f), NULL));
                SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                CMainLogic::sharedMainLogic()->m_bIsShowLogonGift = false;
                CMainLogic::sharedMainLogic()->ShowMessage("正在获取,请稍候", eMBNull);
                sendVIPVersion("MOREGAMES_VERSION");
            }
        }
    }
    if(m_nIsSelectedGameType == 1)
    {
        m_bFBAndPriate = false;
        if(abs((int)m_ptStart.x-(int)point.x)>=15)
            return;
        CCLayer* pLayer = (CCLayer*)m_pScrollViewGameType->getContainer();
        if(pLayer != NULL)
        {
            
        }
        CCLayer* pContainLayer = (CCLayer*)m_pScrollViewGameType->getContainer();
        point = pContainLayer->convertToNodeSpace(point);
        for(unsigned int i = 0; i < m_arrGameType->count(); i++)
        {
            CCSprite* pSpBg = (CCSprite*)m_arrGameType -> objectAtIndex(i);
            CCRect rect = pSpBg -> boundingBox();
            if(rect.containsPoint(point))
            {
                CCMenuItem* item = CCMenuItem::create();
                item -> setTag(pSpBg->getTag());
                clickEnterGame(item);
                break;
            }
        }
        
        return;
    }
    else//游戏房间界面
    {
        if(abs((int)m_ptStart.x-(int)point.x)>=15)
            return;
        if(m_arrGameRoom->count() == 0)
            return;
        CCLayer* pContainLayer = (CCLayer*)m_pScrollView->getContainer();
        point = pContainLayer->convertToNodeSpace(point);
        
        for(unsigned int i = 0; i < m_arrGameRoom->count(); i++)
        {
            CCSprite* pSpBg = (CCSprite*)m_arrGameRoom -> objectAtIndex(i);
            CCRect rect = pSpBg -> boundingBox();
            if(rect.containsPoint(point))
            {
                pSpBg -> runAction(CCSequence::create(CCScaleTo::create(0.2f, 0.9f), CCScaleTo::create(0.2f, 1.0f) ,NULL));
                SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                CCMenuItem* item = CCMenuItem::create();
                item -> setTag(pSpBg->getTag());
                gameCallBack(item);
                
                break;
            }
        }
    }
    if(m_arrGameRoom->count() == 0)
        return;
    
    m_pScrollView->unscheduleAllSelectors();
}

void CHallLayer::setMainLogic(CMainLogic * pMainLogic)
{
    m_pMainLogic = pMainLogic;
}

void CHallLayer::setUserNickName(const char * str)
{
    m_sNickName = str;
    m_cclNickName->setString(str);
    setNameLength(str);
//    if(CMainLogic::sharedMainLogic()->m_nVipLevel >= 1)
//    {
//        m_cclNickName->setColor(ccc3(255, 0, 0));
//    }
}

void CHallLayer::setNickNameColor()
{
//    if(CMainLogic::sharedMainLogic()->m_nVipLevel >= 1)
//    {
//        m_cclNickName->setColor(ccc3(255, 0, 0));
//    }
}

void CHallLayer::setUserScore(int64_t score)
{
    m_nUserScore = score;
    char sUserScore[32];
    sprintf(sUserScore,"%lld",score);
    m_cclUserScore->setString(sUserScore);
}

void CHallLayer::setUserID(unsigned int gameID)
{
    char temp[16];
    sprintf(temp, "ID：%d",gameID);
    m_cclUserID->setString(temp);
}

void CHallLayer::setTicket(int nTicket)
{
}

void CHallLayer::setVipExp()
{
    int nCurrentVipLevel = CMainLogic::sharedMainLogic()->m_nVipLevel;
    int nVIPExp = CMainLogic::sharedMainLogic()->m_nVipExp;
    
    char tempVIPLevel[8];
    sprintf(tempVIPLevel, "%d", nCurrentVipLevel);
    m_labelVIP->setString(tempVIPLevel);
    
    m_pVIPSlider->setContentSize(CCSizeMake(10, m_pVIPSlider->getContentSize().height));
    float fTotalLong = m_pVIPBg->getContentSize().width;
    float fRadius = 52;//m_pVIPHead->getContentSize().width/2;
    float fJYlong = fTotalLong-2*fRadius;
    int a = rand()%100;
    float fPersent = (float)a/100;
    float fPerLong = fPersent*fJYlong;
    //取出玩家当前经验值
    if(nCurrentVipLevel == 0)//当前玩家VIP为0
    {
        if(CMainLogic::sharedMainLogic()->m_vctVipExpConfig.size() != 0)
        {
            //fPerLong = fJYlong*nVIPExp/(CMainLogic::sharedMainLogic()->m_vctVipExpConfig[0].nChargeCount*nExpPercent);
            fPerLong = fJYlong*nVIPExp/CMainLogic::sharedMainLogic()->m_vctVipExpConfig[0].nChargeCount;
        }
    }
    else
    {
        for(int i = 0; i < CMainLogic::sharedMainLogic()->m_vctVipExpConfig.size(); i++)
        {
            //取出玩家当前经验值
            if(CMainLogic::sharedMainLogic()->m_vctVipExpConfig[i].nLevel == nCurrentVipLevel)
            {
                if(i < CMainLogic::sharedMainLogic()->m_vctVipExpConfig.size()-1)
                {
                    //下一级需要的经验
                    int nNextExp = CMainLogic::sharedMainLogic()->m_vctVipExpConfig[i+1].nChargeCount;//*nExpPercent;
                    float f1 = (float)nVIPExp/nNextExp;
                    fPerLong = f1*fJYlong;
                }
                else//玩家已经满级
                {
                    fPerLong = fJYlong;
                }
                break;
            }
        }
    }
    //jJYlong就是总长度，这个函数中要传入的参数就必须是现在已有的经验值和此时vip等级的经验值
    m_pVIPSlider->setContentSize(CCSizeMake(5+fPerLong, m_pVIPSlider->getContentSize().height));
}


void CHallLayer::sendSharedCompleted()
{
    int nGameID = CMainLogic::sharedMainLogic()->m_nGameID;
    char temp[32];
    sprintf(temp, "%dbuyu2015",nGameID);
    char szMD5Result[128];
    CEncryptMD5::toString32(temp,szMD5Result);
    char urlStr[100];
    sprintf(urlStr, "http://%s/Share.aspx?uid=%d&num=%s", NORMAL_URL,nGameID,szMD5Result);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(urlStr);
    request -> setRequestType(CCHttpRequest::kHttpPost);
    request -> setTag("http test");
    request -> setRequestData(szMD5Result, (int)strlen(szMD5Result));
    request -> setResponseCallback(this, httpresponse_selector(CHallLayer::onHttpSharedComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CHallLayer::onHttpSharedComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK);
}

void CHallLayer::sendRank()
{
    RankLayer* pRankLayer = RankLayer::create();
    this -> addChild(pRankLayer,200);
    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
}

void CHallLayer::onHttpRankComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    
    
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
bool CHallLayer::getExitOtherGame()
{
	JniMethodInfo info;
	bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if(bHave)
	{
		jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
		bool bHave1 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","isAvilible2", "()Z");
		if(bHave1)
		{
			bool bExist = (bool)info.env->CallBooleanMethod(jobj, info.methodID);
			return bExist;
		}
	}
	return false;
}

void CHallLayer::openOtherGame()
{
	JniMethodInfo info;
	bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if(bHave)
	{
		jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
		bool bHave1 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","openNiuNiu", "()V");
		if(bHave1)
		{
			info.env->CallVoidMethod(jobj, info.methodID);
		}
	}
}

void CHallLayer::openNiuDownLoad()
{
	JniMethodInfo info;
	bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if(bHave)
	{
		jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
		bool bHave1 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","openNiuDownLoad", "()V");
		if(bHave1)
		{
			info.env->CallVoidMethod(jobj, info.methodID);
		}
	}
}

#endif

void CHallLayer::onEnterTransitionDidFinish()
{
    CMainLogic::sharedMainLogic()->freeMemory();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("CatchFish01/fish19.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3all/quickstart.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3all/labanotice.plist");
    //CMainLogic::sharedMainLogic()->m_pMessageBox->m_bShow = false;
    
    if(CMainLogic::sharedMainLogic()->m_bClickIcon)
    {
        loadFirstMagicPtc();
    }
    loadMagicPtc();

    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CMainLogic::sharedMainLogic()->m_bExitGame = false;
#endif
    
    //loadGameType();
    showFirstChargeAnimate();
    showNiuGameMenu();
    setMailNumber(CMainLogic::sharedMainLogic()->m_nMailCount);
    
    m_bEnterHall = true;
    m_bInitCompleted = true;
    showQuickStartAnimate();
    
    
    if(CMainLogic::sharedMainLogic() != NULL)
    {
        if(CMainLogic::sharedMainLogic()->m_bClickIcon)
        {
            //检查网络连接
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            std::string str = ChargeWebView::OnInternet();
            if(str == "")
            {
                //没有可用的网络
                CMainLogic::sharedMainLogic()->ShowMessage("当前没有可用的网络，请检查网络设置", eMBOK, eDisConnectNet);
                CMainLogic::sharedMainLogic()->m_bNetCanStatus = false;
                return;
            }
            else
            {
                CMainLogic::sharedMainLogic()->initConnect();
                CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器请稍候...", eMBNull);
            }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            std::string str = CMainLogic::sharedMainLogic()->OnInternet();
            if(str == "2")//当前没有网络
            {
                CMainLogic::sharedMainLogic()->ShowMessage("当前没有可用的网络，请检查网络设置", eMBOK, eDisConnectNet);
                CMainLogic::sharedMainLogic()->m_bNetCanStatus = false;
                return;
            }
            else
            {
                CMainLogic::sharedMainLogic()->initConnect();
                CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器请稍候...", eMBNull);
            }
#endif
            
        }
    }
    
    
    //有两个地方在请求定时器，在此一处，在邮件面板关闭的时候有一处，在邮件面板打开的时候将定时器关闭了
    this -> schedule(schedule_selector(CMainLogic::requestMail),300);
    
    float fSound = CCUserDefault::sharedUserDefault()->getFloatForKey("sound", 1);
    float fEffect = CCUserDefault::sharedUserDefault()->getFloatForKey("effect", 1);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(fSound);
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(fEffect);
    if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
    {
        WheelLayer* pWheelLayer = (WheelLayer*)this->getChildByTag(19981);
        if(pWheelLayer == NULL)
        {
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/4.mp3", true);
        }
        
    }
    this->setTouchEnabled(true);
    
    if(!CMainLogic::sharedMainLogic()->m_bBYGameToSuper && !CMainLogic::sharedMainLogic()->m_bSuperToBY && CMainLogic::sharedMainLogic()->m_bShowNoticeLayer && CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        CMainLogic::sharedMainLogic()->m_bShowNoticeLayer = false;
        if(CMainLogic::sharedMainLogic()->m_bReceiveShowWheel)
        {
            WheelLayer* pWheelLayer = WheelLayer::create();
            this -> addChild(pWheelLayer, 199, 19981);
        }
//        if(CCUserDefault::sharedUserDefault()->getBoolForKey("NoticeDataSuccess", false))
//        {
//            CNoticeOfHall* pNoticeOfHall = CNoticeOfHall::create(true, 1);
//            this->addChild(pNoticeOfHall, 300);
//            setGonggaoStatus(false);
//        }
//        else
//        {
//            CMainLogic::sharedMainLogic()->sendNoticeVersion("hallnotice");
//        }
    }
    
    if(CMainLogic::sharedMainLogic()->m_bGameChargeStatus)
    {
        CMainLogic::sharedMainLogic()->m_bGameChargeStatus = false;
		CMainLogic::sharedMainLogic()->m_bNetPageCharge = false;
        this -> scheduleOnce(schedule_selector(CHallLayer::addChargeLayer), 0.0f);
    }
    update(0);
    this -> scheduleUpdate();
    this->scheduleOnce(schedule_selector(CHallLayer::setClickMenu), 0.5f);
}

void CHallLayer::showNoticeLayer()
{
    CNoticeOfHall* pNoticeOfHall = CNoticeOfHall::create(true, 1);
    this->addChild(pNoticeOfHall, 300);
    setGonggaoStatus(false);
}

void CHallLayer::setClickMenu()
{
    m_bCanClickMenu = true;
    if(!m_bExistRoom)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("当前游戏暂没有开放房间，敬请期待！", eMBOK);
    }
}

void CHallLayer::addChargeLayer()
{
    ChargeLayer* pChargeLayer = ChargeLayer::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeLayer, 11, 20000);
    if(CMainLogic::sharedMainLogic()->m_nShowChargeType == 1)//支付宝验证结果
    {
        if(CMainLogic::sharedMainLogic()->m_nShowCoinArmSkill == 1)//购买金币
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            pChargeLayer -> sendAlipayChargeSuccess();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            pChargeLayer -> notifyIOSAlipayCompleted();
#endif
        }
    }
    else if(CMainLogic::sharedMainLogic()->m_nShowChargeType == 2)//微信充值验证结果
    {
        if(CMainLogic::sharedMainLogic()->m_nShowCoinArmSkill == 1)//购买金币
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            pChargeLayer -> notifyIOSWXCompleted();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			pChargeLayer -> sendWXChargeSuccess();
#endif
        }

    }
    else if(CMainLogic::sharedMainLogic()->m_nShowChargeType == 3)//网页充值
    {
        pChargeLayer -> setWebViewCharge(false);
		CMainLogic::sharedMainLogic()->updateCurrentScore();
    }
}

void CHallLayer::requestMail(float dt)
{
    CMD_GPR_MailIDList MailIDList;
    MailIDList.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    CMainLogic::sharedMainLogic() -> SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_QUERY_MAIL_ID_LIST, &MailIDList, sizeof(MailIDList));
}

void CHallLayer::setNameLength(std::string nameStr)
{
    int lMax = 0;
    CCLabelTTF * nameTTF = CCLabelTTF::create(nameStr.c_str(), "Arial", 20);
    float strLength = nameTTF->getContentSize().width;
    if (strLength - 95>5) {
        lMax = (180<strLength)?180:strLength;
    }
    else
    {
        lMax = strLength;
    }
    std::string  string1 = nameStr;
    std::string  string2;
    if (strLength>lMax)
    {
        const char * s = nameStr.c_str();
        int i = 0, j = 0;
        while (s[i])
        {
            if ((s[i] & 0xc0) != 0x80)
            {
                j++;
                if (j>1) {
                    std::string  string3 = string1.substr(0,i).c_str();
                    nameTTF ->setString(string3.c_str());
                    strLength = nameTTF->getContentSize().width;
                    
                    if ((strLength)>lMax) {
                        break;
                    }
                    else
                    {
                        string2 = string1.substr(0,i).c_str();
                    }
                }
            }
            i++;
        }
    }
    else
    {
        string2 = nameStr;
    }
    m_cclNickName -> setString(string2.c_str());
}

void CHallLayer::constructGuestPacket()
{
    if (CCUserDefault::sharedUserDefault()->getStringForKey("NewGuestAccount", "")=="")
    {
        CMainLogic::sharedMainLogic()->m_sAccount = "";
        CMainLogic::sharedMainLogic()->m_sPassword = "";
        CMainLogic::sharedMainLogic()->m_nLogonType = eGuestRegister;
    }
    else
    {
        CMainLogic::sharedMainLogic()->m_sAccount = CCUserDefault::sharedUserDefault()->getStringForKey("NewGuestAccount", "");
        CMainLogic::sharedMainLogic()->m_sPassword = CCUserDefault::sharedUserDefault()->getStringForKey("NewGuestPassword", "");
        CMainLogic::sharedMainLogic()->m_nLogonType = eLogon;
    }
}

void CHallLayer::sendLogonInfo()
{
    //玩家第一次进来时，肯定是构造游客数据
    if(CCUserDefault::sharedUserDefault()->getBoolForKey("NewGuestBank"))
    {
        CMainLogic::sharedMainLogic()->m_sAccount = CCUserDefault::sharedUserDefault()->getStringForKey("NewGuestAccount", "");
        CMainLogic::sharedMainLogic()->m_sPassword = CCUserDefault::sharedUserDefault()->getStringForKey("NewGuestPassword", "");
    }
    else
    {
        CMainLogic::sharedMainLogic()->m_sAccount = CCUserDefault::sharedUserDefault()->getStringForKey("NewAccount", "");
        CMainLogic::sharedMainLogic()->m_sPassword = CCUserDefault::sharedUserDefault()->getStringForKey("NewPassword", "");
        if(CMainLogic::sharedMainLogic()->m_sAccount == "")//玩家第一次登录
        {
            constructGuestPacket();
        }
    }
    //进行登录
    CMainLogic::sharedMainLogic()->OnLinkHall(CMainLogic::sharedMainLogic()->m_nHallSocketHandle);
    
    CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器请稍候...",eMBNull);

    CCUserDefault::sharedUserDefault()->setStringForKey("NewAccount", CMainLogic::sharedMainLogic()->m_sAccount.c_str());
    CCUserDefault::sharedUserDefault()->setStringForKey("NewPassword", CMainLogic::sharedMainLogic()->m_sPassword.c_str());

}

void CHallLayer::sendVIPVersion(std::string sTag)
{
    if(sTag == "ADVERT_VERSION")
    {
        m_bRequestAdvert = true;
    }
    else
    {
        m_bRequestAdvert = false;
    }
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/LeleconfigVersion.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setTag(sTag.c_str());
    request -> setResponseCallback(this, httpresponse_selector(CHallLayer::onHttpVIPVersionComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CHallLayer::onHttpVIPVersionComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        if(!m_bRequestAdvert)
        {
            CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        }
        return;
    }
    if (!response->isSucceed())
    {
        if(!m_bRequestAdvert)
        {
            CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        }
        return;
    }
    
    int nVIP = 0;
    
    std::string tag = response->getHttpRequest()->getTag();
    if(tag == "VIP")
    {
        nVIP = 1;
    }
    else if(tag == "LOTTERY")
    {
        nVIP = 2;
    }
    else if(tag == "MOREGAMES_VERSION")
    {
        nVIP = 3;
    }
    else if(tag == "ADVERT_VERSION")
    {
        nVIP = 4;
    }
    else if(tag == "RANK")
    {
        nVIP = 5;
    }
    
    
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    //对str进行xml解析，将30条数据保存到m_vctRankTimes中
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("version"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL) {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "newvipConfig"))//相同
            {
                std::string sVIPVersion = pCurrentElement->GetText();
                if(CCUserDefault::sharedUserDefault()->getStringForKey("VIPVersion1", "") == "")
                {
                    //当前不存在VIP版本号，保存并点击VIP按钮的时候
                    if(nVIP == 1)
                    {
                        CCUserDefault::sharedUserDefault()->setStringForKey("VIPNewVersion1", sVIPVersion);
                        sendVIP();
                    }
                }
                else if(CCUserDefault::sharedUserDefault()->getStringForKey("VIPVersion1") != sVIPVersion)//本地保存的VIP版本号跟新的不一致，说明VIP有更新
                {
                    if(nVIP == 1)
                    {
                        CCUserDefault::sharedUserDefault()->setStringForKey("VIPNewVersion1", sVIPVersion);
                        sendVIP();
                    }
                }
                else//本地已经保存VIP，并且没有更新内容，则此时直接打开VIP即可
                {
                    if(nVIP == 1)
                    {
                        CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
                        VIPLayer* pVIPLayer = VIPLayer::create(true,CMainLogic::sharedMainLogic()->m_nGameToHallVipLevel);
                        this->addChild(pVIPLayer, 200, 19995);
                    }
                    
                }
            }
            else if(!std::strcmp(pValue, "exchangeNewShopVersion"))
            {
                std::string sLotteryVersion = pCurrentElement->GetText();
                if(CCUserDefault::sharedUserDefault()->getStringForKey("LotteryVersion", "") == "")
                {
                    if(nVIP == 2)
                    {
                        //当前不存在奖券版本号
                        CCUserDefault::sharedUserDefault()->setStringForKey("LotteryNewVersion", sLotteryVersion);
                        sendLottery();
                    }
                }
                else if(CCUserDefault::sharedUserDefault()->getStringForKey("LotteryVersion") != sLotteryVersion)//本地保存的奖券版本号跟新的不一致，说明奖券有更新
                {
                    if(nVIP == 2)
                    {
                        CCUserDefault::sharedUserDefault()->setStringForKey("LotteryNewVersion", sLotteryVersion);
                        sendLottery();
                    }
                }
                else//本地已经保存奖券兑换信息，并且没有更新内容，则此时直接打开商城即可
                {
                    if(nVIP == 2)
                    {
                        CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
                        LotteryLayer* pLotteryLayer = LotteryLayer::create(true);
                        this -> addChild(pLotteryLayer, 200, 19989);
                    }
                }
            }
            else if(!std::strcmp(pValue, "rankVersion"))
            {
                std::string sRankVersion = pCurrentElement->GetText();
                if(CCUserDefault::sharedUserDefault()->getStringForKey("RankVersion", "") == "")
                {
                    if(nVIP == 5)
                    {
                        //当前不存在比赛版本号
                        CCUserDefault::sharedUserDefault()->setStringForKey("RankNewVersion", sRankVersion);
                        sendRankVersion();
                    }
                }
                else if(CCUserDefault::sharedUserDefault()->getStringForKey("RankVersion") != sRankVersion)//本地保存的奖券版本号跟新的不一致，说明奖券有更新
                {
                    if(nVIP == 5)
                    {
                        CCUserDefault::sharedUserDefault()->setStringForKey("RankNewVersion", sRankVersion);
                        sendRankVersion();
                    }
                }
                else//本地已经保存奖券兑换信息，并且没有更新内容，则此时直接打开商城即可
                {
                    if(nVIP == 5)
                    {
                        sendRank();
                    }
                }
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
        
    }
    delete document;
}

//请求版本号，并保存
void CHallLayer::sendRankVersion()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/LeleMatchPriseInfo.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(CHallLayer::onHttpRankVersionComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CHallLayer::onHttpRankVersionComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    
    saveRankVersion(str);
}

void CHallLayer::saveRankVersion(std::string str)
{
    //在Documents目录下创建一个可读写的文件
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + std::string("MatchPriseInfo.xml");
    const char* buf = str.c_str();
    FILE* file = fopen(xmlPath.c_str(), "w");
    if(file)
    {
        fputs(buf, file);
        fclose(file);
        std::string sRankNewVersion = CCUserDefault::sharedUserDefault()->getStringForKey("RankNewVersion");
        CCUserDefault::sharedUserDefault()->setStringForKey("RankVersion", sRankNewVersion);
        sendRank();
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，获取不到比赛了，请稍后再试一次吧。", eMBOK);
    }
}

void CHallLayer::sendVIP()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/LeleVipConfigInfo.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setTag("http test");
    request -> setResponseCallback(this, httpresponse_selector(CHallLayer::onHttpVIPComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CHallLayer::onHttpVIPComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    std::string sVIPNewVersion = CCUserDefault::sharedUserDefault()->getStringForKey("VIPNewVersion1");
    CCUserDefault::sharedUserDefault()->setStringForKey("VIPVersion1", sVIPNewVersion);
    VIPLayer* pVIPLayer = VIPLayer::create(false, CMainLogic::sharedMainLogic()->m_nGameToHallVipLevel);
    this->addChild(pVIPLayer, 200, 19995);
    pVIPLayer -> parseData(str);
    
    
    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
}

void CHallLayer::sendLottery()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/LeleExchangeShop.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setTag("http test");
    request -> setResponseCallback(this, httpresponse_selector(CHallLayer::onHttpLotteryComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CHallLayer::onHttpLotteryComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    std::string sLotteryNewVersion = CCUserDefault::sharedUserDefault()->getStringForKey("LotteryNewVersion");
    CCUserDefault::sharedUserDefault()->setStringForKey("LotteryVersion", sLotteryNewVersion);
    
    LotteryLayer* pLotteryLayer = LotteryLayer::create(false);
    this->addChild(pLotteryLayer, 200, 19989);
    pLotteryLayer -> parseData(str);
    
    
    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
}

//加载特效
void CHallLayer::loadFirstMagicPtc()
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    Utils::preloadFile("CatchFish01.ptc");
#endif
    CCSize size=getContentSize();
    int client_wi=(int)size.width;
    int client_he=(int)size.height;
    m_pMagicDevice=new MP_Device_WRAP(client_wi, client_he);
    m_pMagicDevice->Create();
    MP_Manager& MP=MP_Manager::GetInstance();
    MP_Platform* platform=new MP_Platform_COCOS;
    MP.Initialization(MAGIC_pXnY, platform, MAGIC_INTERPOLATION_ENABLE, MAGIC_CHANGE_EMITTER_DEFAULT, 1024, 1024, 1, 1.f, 0.1f, true);
    unsigned long lSize = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    const char * pParticle = (const char *)CCFileUtils::sharedFileUtils()->getFileData("CatchFish01/CatchFish01.ptc", "rb", &lSize);
    MP.LoadEmittersFromFileInMemory(pParticle);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    MP.LoadAllEmitters();
#endif
    
    
    MP.RefreshAtlas();
    MP.RestoreAtlas();
    MP.CloseFiles();
    MP.Stop();
    m_pMagicDevice->SetScene3d();
    HM_EMITTER hmEmitter=MP.getHmEmitterByName("penjinbi");
    if(hmEmitter)
    {
        Magic_CorrectEmitterPosition(hmEmitter, client_wi, client_he);
        CCMagicEmitter* pMagicEmitter = CCMagicEmitter::create(hmEmitter);
        pMagicEmitter -> setContentSize(CCSize(client_wi, client_he));
        pMagicEmitter -> isIgnoreAnchorPointForPosition();
        pMagicEmitter -> setAnchorPoint(ccp(0.5,0.5));
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CMainLogic::sharedMainLogic()->m_pHallScene->addChild(pMagicEmitter, 31, 20001);
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        pMagicEmitter->setVisible(false);
#endif
    }
}

void CHallLayer::loadMagicPtc()
{
//#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//    Utils::preloadFile("CatchFish01.ptc");
//#endif
    CCSize size=getContentSize();
    int client_wi=(int)size.width;
    int client_he=(int)size.height;
//    m_pMagicDevice=new MP_Device_WRAP(client_wi, client_he);
//    m_pMagicDevice->Create();
    MP_Manager& MP=MP_Manager::GetInstance();
//    MP_Platform* platform=new MP_Platform_COCOS;
//    MP.Initialization(MAGIC_pXnY, platform, MAGIC_INTERPOLATION_ENABLE, MAGIC_CHANGE_EMITTER_DEFAULT, 1024, 1024, 1, 1.f, 0.1f, true);
//    unsigned long lSize = 0;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    const char * pParticle = (const char *)CCFileUtils::sharedFileUtils()->getFileData("CatchFish01/CatchFish01.ptc", "rb", &lSize);
//    MP.LoadEmittersFromFileInMemory(pParticle);
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    MP.LoadAllEmitters();
//#endif
//    
//    
//    MP.RefreshAtlas();
//    MP.RestoreAtlas();
//    MP.CloseFiles();
//    MP.Stop();
//    m_pMagicDevice->SetScene3d();
    HM_EMITTER hmEmitter=MP.getHmEmitterByName("penjinbi");
    if(hmEmitter)
    {
        Magic_CorrectEmitterPosition(hmEmitter, client_wi, client_he);
        CCMagicEmitter* pMagicEmitter = CCMagicEmitter::create(hmEmitter);
        pMagicEmitter -> setContentSize(CCSize(client_wi, client_he));
        pMagicEmitter -> isIgnoreAnchorPointForPosition();
        pMagicEmitter -> setAnchorPoint(ccp(0.5,0.5));
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CMainLogic::sharedMainLogic()->m_pHallScene->addChild(pMagicEmitter, 31, 20001);
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        pMagicEmitter->setVisible(false);
#endif
    }
}

void CHallLayer::playParticle(float x, float y)
{
    MP_Manager & MP=MP_Manager::GetInstance();
    MP_Emitter * emitter=NULL;
    emitter=MP.GetEmitterByName("penjinbi");
    if (emitter!=NULL)
    {
        HM_EMITTER hmEmitter = emitter->GetEmitter();
        if (hmEmitter)
        {
            MP_POSITION pos;
            emitter->GetPosition(pos);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            CCMagicEmitter* pMagicEmitter = (CCMagicEmitter*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20001);
			if(pMagicEmitter != NULL)
			{
				pMagicEmitter->setVisible(true);
			}
#endif
            pos.x = x;
            pos.y = y;
            emitter->Move(pos);
            emitter->SetState(MAGIC_STATE_STOP);
            emitter->SetState(MAGIC_STATE_UPDATE);
        }
    }
}

void CHallLayer::hideOneChargeMenu()
{
    m_menuOneCharge -> setVisible(false);
}

//隐藏首充按钮，显示剩余领取次数界面
void CHallLayer::showLastTimes(int count)
{
    if(count < 11)
    {
        m_menuFirst->setVisible(false);
        m_menuReceive->setVisible(true);
        char temp[16];
        sprintf(temp, "可领取%d/10", count);
        m_labelReceive->setString(temp);
        hideFirstChargeAnimate();
    }
    else//已经领取完了
    {
        m_menuFirst->setVisible(false);
        m_menuReceive->setVisible(false);
        if(CMainLogic::sharedMainLogic()->m_bShowOneCharge && CMainLogic::sharedMainLogic()->m_bCanOneCharge)//移动1元首充的位置
        {
            CCArray* array = m_menuOneCharge->getChildren();
            if(array->count() != 0) return ;
            CCMenuItem* item = (CCMenuItem*)array->objectAtIndex(0);
            if(item != NULL)
            {
                item -> setPosition(ccp(CLIENT_WIDTH*0.6f+60, 650));
            }
        }
    }
}

void CHallLayer::hideFirstChargeAnimate()
{
    if(!CMainLogic::sharedMainLogic()->m_bFirstChargeCanGet)
    {
        m_labelReceive->setString("今日已领取");
        CCArray* array = m_menuReceive->getChildren();
        if(array->count() == 1)
        {
            CCMenuItemImage* item = (CCMenuItemImage*)array->objectAtIndex(0);
            if(item != NULL)
            {
                CCSprite* sprite = (CCSprite*)item->getChildByTag(101);
                if(sprite != NULL)
                {
                    sprite -> setVisible(false);
                }
                sprite = (CCSprite*)item->getChildByTag(102);
                if(sprite != NULL)
                {
                    sprite -> setVisible(false);
                }
            }
        }
    }
    else
    {
        CCArray* array = m_menuReceive->getChildren();
        if(array->count() == 1)
        {
            CCMenuItemImage* item = (CCMenuItemImage*)array->objectAtIndex(0);
            if(item != NULL)
            {
                CCSprite* sprite = (CCSprite*)item->getChildByTag(101);
                if(sprite != NULL)
                {
                    sprite -> setVisible(true);
                }
                sprite = (CCSprite*)item->getChildByTag(102);
                if(sprite != NULL)
                {
                    sprite -> setVisible(true);
                }
            }
        }
    }
}

void CHallLayer::showFirstChargeAnimate(bool bChargeBack)
{
    if(m_bFirstChargeAnimate || bChargeBack)
    {
        if(CMainLogic::sharedMainLogic()->m_cbCountTimes == 0)//可以首充
        {
            if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                m_menuFirst->setVisible(true);
            }
            m_menuReceive->setVisible(false);
        }
        else if(CMainLogic::sharedMainLogic()->m_cbCountTimes == 11)//首充功能已经领取完毕
        {
            m_menuFirst->setVisible(false);
            m_menuReceive->setVisible(false);
        }
        else
        {
            m_menuFirst->setVisible(false);
            if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                m_menuReceive->setVisible(true);
            }
            char temp[16];
            sprintf(temp, "可领取%d/10", CMainLogic::sharedMainLogic()->m_cbCountTimes);
            m_labelReceive->setString(temp);
            hideFirstChargeAnimate();
        }
    }
}

void CHallLayer::showFirstCharge()
{
    m_bFirstChargeAnimate = true;
    if(m_bEnterHall)
    {
        showFirstChargeAnimate();
    }
}

void CHallLayer::showNiuGame()
{
    m_bShowNiuGame = true;
    if(m_bEnterHall)
    {
        showNiuGameMenu();
    }
}

void CHallLayer::showNiuGameMenu()
{
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        return;
    }
    if(m_bShowNiuGame)
    {
        if(CMainLogic::sharedMainLogic()->m_bShowNiuGame)
        {
            CCSprite * pMoreGamesMenu = CCSprite::create("h2all/gdyxBg.png");
            pMoreGamesMenu->setTag(1533);
            pMoreGamesMenu->setPosition(ccp(16, 200));
            this->addChild(pMoreGamesMenu, 100);
            float fWidth = pMoreGamesMenu->getContentSize().width;
            for (int i=0; i<4; i++)
            {
                char str1[32];
                sprintf(str1, "h2all/gdyx0_%d.png", i+1);
                CCSprite * pSp1 = CCSprite::create(str1);
                pSp1->setTag(90+i);
                pSp1->setOpacity(200);
                pSp1->setPosition(ccp(fWidth/2-2, 115-i*27));
                pMoreGamesMenu->addChild(pSp1, 1);
                
                char str2[32];
                sprintf(str2, "h2all/gdyx1_%d.png", i+1);
                CCSprite * pSpAct = CCSprite::create(str2);
                pSpAct->setTag(100+i);
                pSpAct->setPosition(ccp(fWidth/2-2, 115-i*27));
                pMoreGamesMenu->addChild(pSpAct, 2);
                pSpAct->setOpacity(0);
            }
            
            CCScaleTo * scaleTo = CCScaleTo::create(0.2, 1.25);
            CCFadeOut * fadeOut = CCFadeOut::create(0.2);
            CCCallFuncN * callFuncN = CCCallFuncN::create(this, callfuncN_selector(CHallLayer::callBackForMoreGamesMenu));
            CCSpawn * spawn = CCSpawn::create(scaleTo, fadeOut, NULL);
            CCSequence * seq = CCSequence::create(CCFadeIn::create(0.1), spawn, callFuncN, NULL);
            CCSprite * pSpAct1 = (CCSprite *)pMoreGamesMenu->getChildByTag(100);
            pSpAct1->runAction(seq);
        }
    }
}

void CHallLayer::callBackForMoreGamesMenu(CCNode * node)
{
    int nTag = node->getTag();
    if (nTag==103)
    {
        nTag = 100;
    }
    else
    {
        nTag = nTag+1;
    }
    float fDelayTime = 0;
    if (nTag == 103)
    {
        fDelayTime = 1.0;
    }
    else
    {
        fDelayTime = 0.0;
    }
    
    CCScaleTo * scaleTo = CCScaleTo::create(0.2, 1.25);
    CCFadeOut * fadeOut = CCFadeOut::create(0.2);
    CCDelayTime * delayTime = CCDelayTime::create(fDelayTime);
    CCCallFuncN * callFuncN = CCCallFuncN::create(this, callfuncN_selector(CHallLayer::callBackForMoreGamesMenu));
    CCSpawn * spawn = CCSpawn::create(scaleTo, fadeOut, NULL);
    CCSequence * seq = CCSequence::create(CCFadeIn::create(0.1), spawn, delayTime, callFuncN, NULL);
    
    CCSprite * pMoreGamesMenu = (CCSprite *)this->getChildByTag(1533);
    CCSprite * pAct = (CCSprite *)pMoreGamesMenu->getChildByTag(nTag);
    pAct->runAction(seq);
}

void CHallLayer::createBubble0()
{
    CCSprite* pBubble = CCSprite::create("h2all/qipao1.png");
    pBubble->setPosition(ccp(1080+rand()%150, 60+rand()%30));
    this->addChild(pBubble);
    pBubble->setScale(0);
    pBubble->setOpacity(200);
    float fScale = 0.6f+0.1*(rand()%9);
    CCScaleTo* pScaleTo0 = CCScaleTo::create(0.3f, fScale);
    CCMoveBy* pMoveBy0 = NULL;
    int nPosX = 1155;
    if (pBubble->getPositionX() > nPosX)
    {
        pMoveBy0 = CCMoveBy::create(0.3f, ccp(-30, 50));
    }
    else
    {
        pMoveBy0 = CCMoveBy::create(0.3f, ccp(30, 50));
    }
    CCSpawn* pSpawn = CCSpawn::create(pScaleTo0, pMoveBy0, NULL);
    int nIndex = rand()%7;
    if (nIndex == 1)
    {
        pBubble->runAction(CCSequence::create(pSpawn, CCMoveBy::create(1+0.2f*(rand()%6), ccp(0, 200+rand()%150)), CCCallFuncN::create(this, callfuncN_selector(CHallLayer::removeSelf)), NULL));
    }
    else if (nIndex == 2)
    {
        if (pBubble->getPositionX() > nPosX)
        {
            pBubble->runAction(CCSequence::create(pSpawn, CCMoveBy::create(2+0.2f*(rand()%11), ccp(rand()%90, 680)), CCCallFuncN::create(this, callfuncN_selector(CHallLayer::removeSelf)), NULL));
        }
        else
        {
            pBubble->runAction(CCSequence::create(pSpawn, CCMoveBy::create(2+0.2f*(rand()%11), ccp(-rand()%90, 680)), CCCallFuncN::create(this, callfuncN_selector(CHallLayer::removeSelf)), NULL));
        }
    }
    else if(nIndex == 3)
    {
        if (pBubble->getPositionX() > nPosX)
        {
            pBubble->runAction(CCSequence::create(pSpawn, CCMoveBy::create(2, ccp(60, 100)), CCCallFuncN::create(this, callfuncN_selector(CHallLayer::removeSelf)), NULL));
        }
        else
        {
            pBubble->runAction(CCSequence::create(pSpawn, CCMoveBy::create(2, ccp(-60, 100)), CCCallFuncN::create(this, callfuncN_selector(CHallLayer::removeSelf)), NULL));
        }
    }
    else
    {
        CCMoveBy* pMoveBy1 = CCMoveBy::create(3+rand()%4, ccp(-100+rand()%200, 680));
        CCEaseSineIn* pEaseIn = CCEaseSineIn::create(pMoveBy1);
        pBubble->runAction(CCSequence::create(pEaseIn, CCCallFuncN::create(this, callfuncN_selector(CHallLayer::removeSelf)), NULL));
    }
}

void CHallLayer::createBubble1()
{
    CCSprite* pBubble = CCSprite::create("h2all/qipao1.png");
    pBubble->setPosition(ccp(50+rand()%150, 60+rand()%30));
    this->addChild(pBubble);
    pBubble->setScale(0);
    pBubble->setOpacity(200);
    float fScale = 0.6f+0.1*(rand()%9);
    CCScaleTo* pScaleTo0 = CCScaleTo::create(0.3f, fScale);
    CCMoveBy* pMoveBy0 = NULL;
    int nPosX = 125;
    if (pBubble->getPositionX() > nPosX)
    {
        pMoveBy0 = CCMoveBy::create(0.3f, ccp(-30, 50));
    }
    else
    {
        pMoveBy0 = CCMoveBy::create(0.3f, ccp(30, 50));
    }
    CCSpawn* pSpawn = CCSpawn::create(pScaleTo0, pMoveBy0, NULL);
    int nIndex = rand()%7;
    if (nIndex == 1)
    {
        pBubble->runAction(CCSequence::create(pSpawn, CCMoveBy::create(1+0.2f*(rand()%6), ccp(0, 200+rand()%150)), CCCallFuncN::create(this, callfuncN_selector(CHallLayer::removeSelf)), NULL));
    }
    else if (nIndex == 2)
    {
        if (pBubble->getPositionX() > nPosX)
        {
            pBubble->runAction(CCSequence::create(pSpawn, CCMoveBy::create(2+0.2f*(rand()%11), ccp(rand()%90, 680)), CCCallFuncN::create(this, callfuncN_selector(CHallLayer::removeSelf)), NULL));
        }
        else
        {
            pBubble->runAction(CCSequence::create(pSpawn, CCMoveBy::create(2+0.2f*(rand()%11), ccp(-rand()%90, 680)), CCCallFuncN::create(this, callfuncN_selector(CHallLayer::removeSelf)), NULL));
        }
    }
    else if(nIndex == 3)
    {
        if (pBubble->getPositionX() > nPosX)
        {
            pBubble->runAction(CCSequence::create(pSpawn, CCMoveBy::create(2, ccp(60, 100)), CCCallFuncN::create(this, callfuncN_selector(CHallLayer::removeSelf)), NULL));
        }
        else
        {
            pBubble->runAction(CCSequence::create(pSpawn, CCMoveBy::create(2, ccp(-60, 100)), CCCallFuncN::create(this, callfuncN_selector(CHallLayer::removeSelf)), NULL));
        }
    }
    else
    {
        CCMoveBy* pMoveBy1 = CCMoveBy::create(3+rand()%4, ccp(-100+rand()%200, 680));
        CCEaseSineIn* pEaseIn = CCEaseSineIn::create(pMoveBy1);
        pBubble->runAction(CCSequence::create(pEaseIn, CCCallFuncN::create(this, callfuncN_selector(CHallLayer::removeSelf)), NULL));
    }
}

void CHallLayer::removeSelf(CCNode* pNode)
{
    pNode->removeFromParent();
}

void CHallLayer::scheduleBubble(float ft)
{
    for (int i = 0 ; i < 40; i++)
    {
        CCDelayTime* pDelay = CCDelayTime::create(0.13f*i+0.1f*(rand()%5));
        CCCallFunc* pCallFunc0 = CCCallFunc::create(this, callfunc_selector(CHallLayer::createBubble0));
        CCSequence* seq0 = CCSequence::create(pDelay, pCallFunc0, NULL);
        CCCallFunc* pCallFunc1 = CCCallFunc::create(this, callfunc_selector(CHallLayer::createBubble1));
        CCSequence* seq1 = CCSequence::create(pDelay, pCallFunc1, NULL);
        this->runAction(CCSpawn::create(seq0, seq1, NULL));
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//判断Android手机是否存在str游戏
bool CHallLayer::canExitFishGameAndroid(std::string str)
{
    JniMethodInfo info;
    bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if(bHave)
    {
        jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
        
        bool bHave1 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","isAvilible2", "(Ljava/lang/String;)Z");
        if(bHave1)
        {
            jstring appidjStr = info.env->NewStringUTF(str.c_str());
            bool bExist = (bool)info.env->CallBooleanMethod(jobj, info.methodID, appidjStr);
            return bExist;
        }
    }
    return false;
}
#endif

void CHallLayer::TimerToLoadMoreGames(float dt)
{
    this->unschedule(schedule_selector(CHallLayer::TimerToLoadMoreGames));
    CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，本次请求超时了，请稍后再试一次吧。", eMBOK);
}

void CHallLayer::update(float dt)
{
    /*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(bTouXiangUpdate)
    {
        bTouXiangUpdate = false;
        //更新头像
//        CCNode* node = (CCNode*)m_pHeadClipping->getChildByTag(999);
//        if(node != NULL)
//        {
//            m_pHeadClipping -> removeChildByTag(999);
//        }
        
        CCMenuItemImage* sexItem = CCMenuItemImage::create(TouXiangName.c_str(), TouXiangName.c_str(), this, menu_selector(CHallLayer::EditTouXiang));
        printf("%s\n", TouXiangName.c_str());
        sexItem->setScale(1.2f);
        sexItem -> setPosition(ccp(56,57));
        CCMenu* pMenu = CCMenu::create(sexItem, NULL);
        pMenu -> setPosition(CCPointZero);
        pMenu -> setAnchorPoint(CCPointZero);
        m_pHeadClipping -> addChild(pMenu,2,999);
        m_pHeadClipping->setVisible(true);
        
        if (this->getChildByTag(999) != NULL)
        {
            this->getChildByTag(999)->setVisible(false);
        }
    }
    if(bDownLoadOwnUserCompleted)
    {
        //更新头像
        if (m_pHeadClipping != NULL)
        {
            bDownLoadOwnUserCompleted = false;
            CCNode* node = (CCNode*)m_pHeadClipping->getChildByTag(999);
            if(node != NULL)
            {
                m_pHeadClipping -> removeChildByTag(999);
            }
            CCMenuItemImage* sexItem = CCMenuItemImage::create(TouXiangName.c_str(), TouXiangName.c_str(), this, menu_selector(CHallLayer::EditTouXiang));
            printf("%s\n", TouXiangName.c_str());
            sexItem -> setPosition(ccp(56,57));
            CCMenu* pMenu = CCMenu::create(sexItem, NULL);
            pMenu -> setPosition(CCPointZero);
            pMenu -> setAnchorPoint(CCPointZero);
            m_pHeadClipping -> addChild(pMenu,2,999);
            m_pHeadClipping -> setVisible(true);
            m_pHeadClipping->setVisible(true);
            if (this->getChildByTag(999) != NULL)
            {
                this->getChildByTag(999)->setVisible(false);
            }
        }
    }
    if(m_vChargeWebView.size() == 1)
    {
        if(m_vChargeWebView[0]->bDownloadComplete)
        {
            delete m_vChargeWebView[0];
            m_vChargeWebView[0] = NULL;
        }
        m_vChargeWebView.clear();
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(bUploadCompleted)
    {
        bUploadCompleted = false;
        createTouXiang("");
    }
#endif
     */
}

void CHallLayer::showQuickStartAnimate()
{
    if(m_bInitCompleted && m_bInitNetComplete)//大厅初始化完成并且网络初始化完成
    {
//        if(CMainLogic::sharedMainLogic()->m_bIOSPassed && CMainLogic::sharedMainLogic()->m_bAndroidOpenStatus)
//        {
//            m_quickSprite -> setVisible(true);
//        }
        
        //发送是否可以有红包可以领取， 并且是登录完成发送（不是退出发件发送）
        if(CMainLogic::sharedMainLogic()->m_bIOSPassed && CMainLogic::sharedMainLogic()->m_bLogonSuccess)
        {
            CMainLogic::sharedMainLogic()->m_bLogonSuccess = false;
            CMD_GPO_QueryGeRenHongBao QueryGeRenHongBao = {0};
            QueryGeRenHongBao.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GP_QUERY_GE_REN_HONG_BAO, &QueryGeRenHongBao, sizeof(QueryGeRenHongBao));
            
        }
        if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
        {
            //请求奖杯数量
            CMD_GPR_WeekBei WeekBei = {0};
            WeekBei.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_WEEK_BEI, &WeekBei, sizeof(WeekBei));
        }
        
        
        
        CCArray* arrayFirst = m_menuFirst->getChildren();
        CCMenuItem* pItem = (CCMenuItem*)arrayFirst->objectAtIndex(0);
        CCSprite* firstSp = CCSprite::create();
        firstSp -> setPosition(ccp(pItem->getContentSize().width/2, pItem->getContentSize().height/2+10));
        pItem -> addChild(firstSp);
        CCArray* firstChargeArr = CCArray::create();
        char temp[32];
        for(int i = 1;i <= 13; i++)
        {
            sprintf(temp, "hallfirstcharge%d.png", i);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
            firstChargeArr -> addObject(frame);
        }
        CCAnimation* firstChargeAnimation = CCAnimation::createWithSpriteFrames(firstChargeArr, 0.11f);
        CCAnimate* firstChargeAnimate = CCAnimate::create(firstChargeAnimation);
        firstSp -> runAction(CCRepeatForever::create(firstChargeAnimate));
        
        CCArray* receiveArray = m_menuReceive->getChildren();
        CCMenuItem* receiveItem = (CCMenuItem*)receiveArray->objectAtIndex(0);
        CCSprite* firstSp2 = CCSprite::create();
        firstSp2 -> setPosition(ccp(receiveItem->getContentSize().width/2, receiveItem->getContentSize().height/2+10));
        receiveItem -> addChild(firstSp2, 1, 102);
        CCArray* firstChargeArr2 = CCArray::create();
        for(int i = 1;i <= 13; i++)
        {
            sprintf(temp, "hallfirstcharge%d.png", i);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
            firstChargeArr2 -> addObject(frame);
        }
        CCAnimation* firstChargeAnimation2 = CCAnimation::createWithSpriteFrames(firstChargeArr2, 0.11f);
        CCAnimate* firstChargeAnimate2 = CCAnimate::create(firstChargeAnimation2);
        firstSp2 -> runAction(CCRepeatForever::create(firstChargeAnimate2));
        if(!CMainLogic::sharedMainLogic()->m_bFirstChargeCanGet)
        {
            firstSp2 -> setVisible(false);
        }
    }
}

void CHallLayer::EditTouXiang(CCObject* object)
{
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
        return ;
    //return ;
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    //自定义头像
    TouXiangLayer* pLayer = TouXiangLayer::create(1);
    this -> addChild(pLayer, 200);
    return ;
    if(CMainLogic::sharedMainLogic()->m_nVipLevel == 0)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，只有VIP用户才能更换自定义头像", eMBOK);
        return ;
    }
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallTouxiang);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    PicturesLayer* pPicturesLayer = PicturesLayer::create();
    this -> addChild(pPicturesLayer, 400);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    PictureLayer* pPictureLayer = PictureLayer::create();
    this -> addChild(pPictureLayer, 400);
#endif
}

void CHallLayer::setGender(int gender)
{
    createDefaultTouXiang(gender);
    /*
    if (m_pVIPBg->getChildByTag(10) == NULL)
    {
        CCSprite* pHead = CCSprite::create("h2all/vipslider2.png");
        pHead->setPosition(ccp(pHead->getContentSize().width/2, m_pVIPBg->getContentSize().height/2));
        CCSize clipSize = pHead->getContentSize();
        CCSprite* spark1 = CCSprite::create();
        spark1->setPosition(pHead->getPosition());
        
        m_pHeadClipping = CCClippingNode::create();
        m_pHeadClipping->setPosition(ccp(0, 0));
        m_pHeadClipping->setAlphaThreshold(0.05f); //设置alpha闸值
        m_pHeadClipping->setContentSize(clipSize); //设置尺寸大小
        m_pHeadClipping->setStencil(pHead);    //设置模板stencil
        m_pHeadClipping->addChild(pHead, 1);   //先添加标题,会完全显示出来,因为跟模板一样大小
        m_pHeadClipping->addChild(spark1, 2, 100);      //会被裁减
        m_pVIPBg->addChild(m_pHeadClipping, 1, 10);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    OwnUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    char tempUser[16];
    sprintf(tempUser, "%d", CMainLogic::sharedMainLogic()->m_nUserID);
    UserIDStr = tempUser;
    bool bExistTouXiang = ChargeWebView::findUserIDFile(tempUser);
    if (bExistTouXiang)
    {
        CCMenuItemImage* sexItem = CCMenuItemImage::create(TouXiangName.c_str(), TouXiangName.c_str(), this, menu_selector(CHallLayer::EditTouXiang));
        printf("%s\n", TouXiangName.c_str());
        sexItem -> setScale(1.2f);
        sexItem -> setPosition(ccp(56, 57));
        CCMenu* pMenu = CCMenu::create(sexItem, NULL);
        pMenu -> setPosition(CCPointZero);
        pMenu -> setAnchorPoint(CCPointZero);
        m_pHeadClipping -> addChild(pMenu,2,999);
        m_pHeadClipping->setVisible(true);
    }
    else
    {
        createDefaultTouXiang(1);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo info;
    bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if(bHave)
    {
        jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
        bool bHave1 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","findTXTExist", "(Ljava/lang/String;)Ljava/lang/String;");
        if(bHave1)
        {
            char temp[16];
            sprintf(temp, "%d", CMainLogic::sharedMainLogic()->m_nUserID);
            jstring userIDStr = info.env->NewStringUTF(temp);
            jstring strTXT = (jstring)info.env->CallObjectMethod(jobj, info.methodID, userIDStr);
            std::string txtStr = JniHelper::jstring2string(strTXT);
            if(strcmp(txtStr.c_str(), "") == 0)//为空，那么没有这个文件，则使用默认的方式创建头像，同时访问网站，查看有没有自己头像信息
            {
                createDefaultTouXiang(gender);
            }
            else//不为空，则查看文件是否存在，如果文件也存在，那么使用这个文件创建头像，否则先使用默认的方式创建头像
            {
                bool bExistFunc = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","findFileExist", "(Ljava/lang/String;)Ljava/lang/String;");
                if(bExistFunc)
                {
                    char tempFileName[32];
                    sprintf(tempFileName, "%d_%s.png", CMainLogic::sharedMainLogic()->m_nUserID, txtStr.c_str());
                    jstring fileStr = info.env->NewStringUTF(tempFileName);
                    jstring filePathStr = (jstring)info.env->CallObjectMethod(jobj, info.methodID, fileStr);
                    std::string filePathName = JniHelper::jstring2string(filePathStr);
                    if(!strcmp(filePathName.c_str(), ""))//文件不存在，则使用默认的方式创建头像，同时请求网站，查看有没有自己信息，
                    {
                        createDefaultTouXiang(gender);
                    }
                    else//文件中存在头像文件，
                    {
                        char tempPathTouXiang[128];
                        sprintf(tempPathTouXiang, "%s", filePathName.c_str());
                        createTouXiang(filePathName);
                    }
                }
                
            }
        }
    }
#endif
    //首次登录的时候请求网站，从桌子界面或者游戏界面返回的时候就不请求了
    if(1)//CMainLogic::sharedMainLogic()->m_bFirstLogon)
    {
        CMainLogic::sharedMainLogic()->m_bChargeState = false;
        //同时访问网站，查看是否有头像信息
        char urlStr[256];
        sprintf(urlStr, "http://%s/GetUserVsion.aspx?uid1=%d&uid2=%d&uid3=%d&uid4=%d&uid5=%d&uid6=%d&tableid=%d",NORMAL_URL, CMainLogic::sharedMainLogic()->m_nUserID,0,0,0,0,0, 0);
        CCHttpRequest* request = new CCHttpRequest();
        request -> setUrl(urlStr);
        request -> setRequestType(CCHttpRequest::kHttpGet);
        request -> setTag("http test");
        request -> setResponseCallback(this, httpresponse_selector(CHallLayer::onHttpVersionComplete));
        CCHttpClient::getInstance()->send(request);
        CC_SAFE_RELEASE(request);
        request = NULL;
    }*/
}

void CHallLayer::createDefaultTouXiang(int gender)
{
    /*
    if (m_pHeadClipping!=NULL)
    {
        m_pHeadClipping->setVisible(false);
    }*/
    CCNode* node = (CCNode*)this->getChildByTag(999);
    if(node != NULL)
    {
        node -> removeFromParent();
    }
    printf("gender is %d\n", gender);
    if(gender<=0 || gender > 10)
    {
        CCMenuItemImage* sexItem = CCMenuItemImage::create("h2all/touxiangnv.png", "h2all/touxiangnv.png", this, menu_selector(CHallLayer::EditTouXiang));
        sexItem -> setPosition(ccp(sexItem->getContentSize().width/2+9,CLIENT_HEIGHT-sexItem->getContentSize().height/2-9));
        sexItem -> setScale(0.9f);
        CCMenu* pMenu = CCMenu::create(sexItem, NULL);
        pMenu -> setPosition(CCPointZero);
        pMenu -> setAnchorPoint(CCPointZero);
        this -> addChild(pMenu,2,999);
    }
    else
    {
        CCMenuItemImage* sexItem = CCMenuItemImage::create(txName[gender-1], txName[gender-1], this, menu_selector(CHallLayer::EditTouXiang));
        sexItem -> setPosition(ccp(sexItem->getContentSize().width/2-2,CLIENT_HEIGHT-sexItem->getContentSize().height/2+1));
        sexItem -> setScale(0.7f);
        CCMenu* pMenu = CCMenu::create(sexItem, NULL);
        pMenu -> setPosition(CCPointZero);
        pMenu -> setAnchorPoint(CCPointZero);
        this -> addChild(pMenu,2,999);
    }
    /*
    if(1)//女
    {
        //CCMenuItemImage* sexItem = CCMenuItemImage::create("h2all/touxiangnv.png", "h2all/touxiangnv.png", this, menu_selector(CHallLayer::EditTouXiang));
        CCMenuItemImage* sexItem = CCMenuItemImage::create("beibao/tx1.png", "beibao/tx1.png", this, menu_selector(CHallLayer::EditTouXiang));
        sexItem -> setPosition(ccp(sexItem->getContentSize().width/2-3,CLIENT_HEIGHT-sexItem->getContentSize().height/2+1));
        //sexItem -> setScale(0.9f);
        sexItem -> setScale(0.7f);
        CCMenu* pMenu = CCMenu::create(sexItem, NULL);
        pMenu -> setPosition(CCPointZero);
        pMenu -> setAnchorPoint(CCPointZero);
        this -> addChild(pMenu,2,999);
    }
    else//男
    {
        CCMenuItemImage* sexItem = CCMenuItemImage::create("h2all/touxiangnan.png", "h2all/touxiangnan.png", this, menu_selector(CHallLayer::EditTouXiang));
        sexItem -> setPosition(ccp(sexItem->getContentSize().width/2+15,CLIENT_HEIGHT-sexItem->getContentSize().height/2-10));
        CCMenu* pMenu = CCMenu::create(sexItem, NULL);
        pMenu -> setPosition(CCPointZero);
        pMenu -> setAnchorPoint(CCPointZero);
        this -> addChild(pMenu,2,999);
    }
     */
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void CHallLayer::onHttpVersionComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    
    static int index = 0;
    index = 0;
    int nTouxiang[6] = {0};
    int nUserID[6] = {0};
    int nTableID = 0;
    //对str进行xml解析
    TiXmlDocument* document = new TiXmlDocument();
    long len = strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), (int)len))//房间返回大厅崩溃
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("accountsInface"))
        {
            return;
        }
        TiXmlElement * pCurrentElement=rootElement->FirstChildElement();
        if(pCurrentElement != NULL)
        {
            const char * pValue=pCurrentElement->Value();
            if (!strcmp(pValue,"tableid"))
            {
                nTableID = atoi(pCurrentElement->GetText());
            }
            TiXmlElement* firstElement = pCurrentElement -> NextSiblingElement();
            while (NULL!=firstElement)
            {
                const char * pValue=firstElement->Value();
                if (!strcmp(pValue,"facelist"))
                {
                    TiXmlElement* secondElement = firstElement -> FirstChildElement();
                    const char * valueId=secondElement->Value();
                    if (!strcmp(valueId,"uid"))
                    {
                        nUserID[index] = atoi(secondElement->GetText());
                    }
                    TiXmlElement* thirdElement = secondElement -> NextSiblingElement();
                    const char * nickname=thirdElement->Value();
                    if (!strcmp(nickname,"vsion"))
                    {
                        int id=atoi(thirdElement->GetText());
                        nTouxiang[index] = id;
                    }
                    index++;
                }
                firstElement=firstElement->NextSiblingElement();
            }
        }
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK);
    }
    delete document;
    if(nTouxiang[0] != 0)//在网站上有头像，需要下载下来，保存到沙盒中，并且在大厅上展示出来
    {
        ChargeWebView* pChargeWebView = new ChargeWebView;
        m_vChargeWebView.push_back(pChargeWebView);
        pChargeWebView->findUserVersionFile(nUserID[0], nTouxiang[0], 0, 0);
    }
}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)


void CHallLayer::createTouXiang(string pathStr)
{
    /*
    //更新头像
    CCNode* node = (CCNode*)this->getChildByTag(999);
    if(node != NULL)
    {
        this -> removeChildByTag(999);
    }
    std::string touxiangpath = "";
    if(!strcmp(pathStr.c_str(), ""))
    {
        touxiangpath = meTouXiangPathStr;
    }
    else
    {
        touxiangpath = pathStr;
    }
    FILE* fp = fopen(touxiangpath.c_str(), "rb");
    if(!fp)
    {
        return ;
    }
    
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buf = (char*)malloc(len);
    fread(buf, len, 1, fp);
    fclose(fp);
    if(len == 0 || buf == NULL)
    {
        return ;
    }
    CCImage* img = new CCImage;
    img->initWithImageData(buf, len);
    free(buf);
    CCTexture2D* texture = new CCTexture2D();
    texture -> initWithImage(img);
    
    if (m_pHeadClipping != NULL && texture != NULL)
    {
        if (m_pHeadClipping->getChildByTag(100) != NULL)
        {
            m_pHeadClipping->removeChildByTag(100);
        }
        meTouXiangPathStr = touxiangpath;
        CCSprite* tempsprite = CCSprite::createWithTexture(texture);
        texture -> release();
        CCMenuItemSprite* sexItem = CCMenuItemSprite::create(tempsprite, tempsprite, this, menu_selector(CHallLayer::EditTouXiang));
        sexItem -> setScale(1.2f);
        sexItem -> setPosition(ccp(56, 57));
        CCMenu* pMenu = CCMenu::create(sexItem, NULL);
        pMenu -> setPosition(CCPointZero);
        pMenu -> setAnchorPoint(CCPointZero);
        m_pHeadClipping->addChild(pMenu, 2, 100);
        m_pHeadClipping->setVisible(true);
    }
    img -> release();
     */
}

void CHallLayer::onHttpVersionComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if(CCDirector::sharedDirector()->getRunningScene() != CMainLogic::sharedMainLogic()->m_pHallScene)
        return ;
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    
    static int index = 0;
    index = 0;
    int nTouxiang[6];
    int nUserID[6];
    int nTableID = 0;
    //对str进行xml解析
    TiXmlDocument* document = new TiXmlDocument();
    int len = strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("accountsInface"))
        {
            return;
        }
        TiXmlElement * pCurrentElement=rootElement->FirstChildElement();
        if(pCurrentElement != NULL)
        {
            const char * pValue=pCurrentElement->Value();
            if (!strcmp(pValue,"tableid"))
            {
                nTableID = atoi(pCurrentElement->GetText());
            }
            TiXmlElement* firstElement = pCurrentElement -> NextSiblingElement();
            while (NULL!=firstElement)
            {
                const char * pValue=firstElement->Value();
                if (!strcmp(pValue,"facelist"))
                {
                    TiXmlElement* secondElement = firstElement -> FirstChildElement();
                    const char * valueId=secondElement->Value();
                    if (!strcmp(valueId,"uid"))
                    {
                        nUserID[index] = atoi(secondElement->GetText());
                    }
                    TiXmlElement* thirdElement = secondElement -> NextSiblingElement();
                    const char * nickname=thirdElement->Value();
                    if (!strcmp(nickname,"vsion"))
                    {
                        int id=atoi(thirdElement->GetText());
                        nTouxiang[index] = id;
                    }
                    index++;
                }
                firstElement=firstElement->NextSiblingElement();
            }
        }
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK);
    }
    delete document;
    
    if(nTouxiang[0] != 0)//在网站上有头像，需要下载下来，保存到沙盒中，并且在大厅上展示出来
    {
        m_nOwnVersion = nTouxiang[0];
        //先判断在原文件中有没有头像txt文件和png文件，如果有txt文件，那么判断内容是否相同，如果没有那么需要保存txt文件和png文件，同时更新头像显示
        JniMethodInfo info;
        bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
        jobject jobj;
        if(bHave)
        {
            jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
            bool bHave1 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","findTXTExist", "(Ljava/lang/String;)Ljava/lang/String;");
            if(bHave1)
            {
                char temp[16];
                sprintf(temp, "%d", CMainLogic::sharedMainLogic()->m_nUserID);
                jstring userIDStr = info.env->NewStringUTF(temp);
                jstring strTXT = (jstring)info.env->CallObjectMethod(jobj, info.methodID, userIDStr);
                std::string txtStr = JniHelper::jstring2string(strTXT);
                char newVersion[16];
                sprintf(newVersion, "%d", nTouxiang[0]);
                if(strcmp(txtStr.c_str(), newVersion) == 0)//文件里版本号跟网站上版本号相同，则不处理
                {
                    //查看文件里是否有头像文件，如果有那么不处理，如果没有，需要下载
                    bool bExistFunc = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","findFileExist", "(Ljava/lang/String;)Ljava/lang/String;");
                    if(bExistFunc)
                    {
                        char tempFileName[32];
                        sprintf(tempFileName, "%d_%s.png", CMainLogic::sharedMainLogic()->m_nUserID, newVersion);
                        jstring fileStr = info.env->NewStringUTF(tempFileName);
                        jstring filePathStr = (jstring)info.env->CallObjectMethod(jobj, info.methodID, fileStr);
                        std::string filePathName = JniHelper::jstring2string(filePathStr);
                        if(!strcmp(filePathName.c_str(), ""))//文件不存在，则需要从网站上下载数据
                        {
                            downloadPicture(CMainLogic::sharedMainLogic()->m_nUserID);
                        }
                        else//文件中存在头像文件，则不处理
                        {
                        }
                    }
                }
                else//版本号不相同，那么更新版本号和头像
                {
                    //写入版本号，用java实现
                    bool bWriteVersion = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "writeVersion", "(Ljava/lang/String;Ljava/lang/String;)V");
                    if(bWriteVersion)
                    {
                        char tempFileName[32];
                        sprintf(tempFileName, "%d", newVersion);
                        jstring versionStr = info.env->NewStringUTF(tempFileName);
                        sprintf(tempFileName, "%d", CMainLogic::sharedMainLogic()->m_nUserID);
                        jstring useridStr = info.env->NewStringUTF(tempFileName);
                        info.env->CallVoidMethod(jobj, info.methodID, versionStr, useridStr);
                    }
                    downloadPicture(CMainLogic::sharedMainLogic()->m_nUserID);
                }
            }
        }
    }
    
}
//从网站上下载图片
void CHallLayer::downloadPicture(int nUserID)
{
    char temp[256];
    sprintf(temp, "http://%s/GameUpFile/%d.png", NORMAL_URL, nUserID);
    //加载网络图片
    CCHttpClient* httpClient = CCHttpClient::getInstance();
    CCHttpRequest* httpReq =new CCHttpRequest();
    httpReq->setRequestType(CCHttpRequest::kHttpGet);
    httpReq->setUrl(temp);
    httpReq->setResponseCallback(this,httpresponse_selector(CHallLayer::onGetFinished));
    httpReq->setTag("PicGet");
    httpClient->setTimeoutForConnect(30);
    httpClient->send(httpReq);
    CC_SAFE_RELEASE(httpReq);
    httpReq = NULL;
}

void CHallLayer::onGetFinished(CCHttpClient* client, CCHttpResponse* response)
{
    /*
    if (!response->isSucceed())
    {
        return ;
    }
    
    const char* tag = response->getHttpRequest()->getTag();
    if (0 == strcmp("PicGet",tag))
    {
        std::vector<char> *data = response->getResponseData();
        int data_length = data->size();
        std::string res;
        for (int i = 0;i<data_length;++i)
        {
            res+=(*data)[i];
        }
        res+='\0';
    }
    
    // 数据转存
    unsigned char* pBuffer = NULL;
    unsigned long bufferSize = 0;
    std::vector<char> *buffer = response->getResponseData();
    char path[256];
    sprintf(path, "sdcard/airmaintain/picture/%d_%d.png", CMainLogic::sharedMainLogic()->m_nUserID, m_nOwnVersion);
    //std::string path = "sdcard/airmaintain/picture/1510.png";
    pBuffer = CCFileUtils::sharedFileUtils()->getFileData(path, "r", &bufferSize);
    std::string buff(buffer->begin(),buffer->end());
    
    //保存到本地文件
    FILE *fp = fopen(path, "wb+");
    fwrite(buff.c_str(), 1,buffer->size(),  fp);
    fclose(fp);
    
    //显示头像
    createTouXiang(path);
     */
}

#endif

void CHallLayer::showMatchGuideEnd(bool bJoinSuccess)
{
    FirstChargeLayer* pFirstChargeLayer = NULL;
    if(bJoinSuccess)
    {
        pFirstChargeLayer = FirstChargeLayer::create(7);
    }
    else
    {
        pFirstChargeLayer = FirstChargeLayer::create(8);
    }
    this -> addChild(pFirstChargeLayer, 200);
}

void CHallLayer::updateRoomUserCount(WORD wServerID, WORD nUserCount)
{
    for(int i = 0; i < m_arrGameRoom->count(); i++)
    {
        int num = 0;
        if(num == 0)
        {
            CCUserDefault::sharedUserDefault()->setIntegerForKey("xinshoufangnum", nUserCount);
        }
        else if(num == 1)
        {
            CCUserDefault::sharedUserDefault()->setIntegerForKey("baipaofangnum", nUserCount);
        }
        else if(num == 2)
        {
            CCUserDefault::sharedUserDefault()->setIntegerForKey("qianpaofangnum", nUserCount);
        }
        else if(num == 3)
        {
            CCUserDefault::sharedUserDefault()->setIntegerForKey("wanpaofangnum", nUserCount);
        }
    }
}

void CHallLayer::setGonggaoStatus(bool bShow)
{
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        bShow = false;
    }
    CCMenu* pMenu = (CCMenu*)this->getChildByTag(192);
    if(pMenu != NULL)
    {
        CCArray* array = pMenu->getChildren();
        for(unsigned int i = 0; i < array->count(); i++)
        {
            CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
            if(pItem->getTag() == eGonggaoTag)
            {
                if(bShow)
                {
                    CCSprite* pMailSprite = (CCSprite*)pItem->getChildByTag(10);
                    if (pMailSprite != NULL)
                    {
                        pMailSprite->setVisible(true);
                        pMailSprite->stopAllActions();
                        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(CHallLayer::showFreecoinAction2));
                        pMailSprite->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 1.2f),CCSpawn::create(func, CCScaleTo::create(0.5f, 0.9f), NULL), NULL)));
                    }
                }
                else
                {
                    CCSprite* pMailSprite = (CCSprite*)pItem->getChildByTag(10);
                    CCSprite* pMailSprite2 = (CCSprite*)pItem->getChildByTag(11);
                    if (pMailSprite != NULL && pMailSprite2 != NULL)
                    {
                        pMailSprite->setVisible(false);
                        pMailSprite->stopAllActions();
                        pMailSprite2->setVisible(false);
                        pMailSprite2->stopAllActions();
                    }
                }
            }
        }
    }
}

void CHallLayer::setGonggaoStatus2()
{
    CCMenu* pMenu = (CCMenu*)this->getChildByTag(192);
    if(pMenu != NULL)
    {
        CCArray* array = pMenu->getChildren();
        for(unsigned int i = 0; i < array->count(); i++)
        {
            CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
            if(pItem->getTag() == eGonggaoTag)
            {
                CCSprite* pMailSprite2 = (CCSprite*)pItem->getChildByTag(11);
                if (pMailSprite2 != NULL)
                {
                    pMailSprite2->setVisible(true);
                    pMailSprite2->stopAllActions();
                    pMailSprite2->runAction(CCSequence::create(CCFadeTo::create(0, 150), CCScaleTo::create(0.3f, 1.5f), CCFadeTo::create(0.2f, 0), CCScaleTo::create(0, 1.3f), NULL));
                }
            }
        }
    }
}
