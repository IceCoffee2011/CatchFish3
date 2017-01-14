#include "header.h"

#define LABABOTICE_HEIGHT 495

LabaNoticeLayer::LabaNoticeLayer(int nHeight)
{
    m_nHeight = nHeight;
    init();
}

LabaNoticeLayer::~LabaNoticeLayer()
{
    
}

bool LabaNoticeLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_bShowCurrent = false;
    
    return true;
}

void LabaNoticeLayer::loadUI()
{
    this->removeAllChildren();
    CCSprite* sprite2 = CCSprite::createWithSpriteFrameName("labakuang1.png");
    sprite2 -> setPosition(ccp(CLIENT_WIDTH/4*3-5, m_nHeight-6.5));
    this -> addChild(sprite2, 1);
    sprite2 -> setScale(2.0f);
    m_spNoticeBg = CCSprite::createWithSpriteFrameName("labakuang1.png");
    m_spNoticeBg -> setPosition(ccp(CLIENT_WIDTH/4+5, m_nHeight));
    this -> addChild(m_spNoticeBg, 2);
    m_spNoticeBg->setScale(2.0f);
    m_spNoticeBg->setFlipX(true);
    m_spNoticeBg->setFlipY(true);
    CCSprite* pLogo = CCSprite::create("h3all/labalogo.png");
    pLogo -> setPosition(ccp(158-80, m_spNoticeBg->getContentSize().height/2-3));
    m_spNoticeBg -> addChild(pLogo);
    pLogo->setScale(0.35f);
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    char temp[24];
    CCArray* array = CCArray::create();
    for(int i = 1; i <= 14; i++)
    {
        sprintf(temp, "labakuang%d.png", i);
        CCSpriteFrame* frame = cache->spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, 0.07f);
    CCAnimate* animate = CCAnimate::create(animation);
    CCAnimation* animation2 = CCAnimation::createWithSpriteFrames(array, 0.07f);
    CCAnimate* animate2 = CCAnimate::create(animation2);
    m_spNoticeBg -> runAction(CCRepeatForever::create(animate2));
    sprite2 -> runAction(CCRepeatForever::create(animate));
    
    m_labelNoticeStr = CCLabelTTF::create("","Arial-BoldMT",32);//创建通知
    m_labelNoticeStr -> setAnchorPoint(ccp(0, 0.5));
    m_labelNoticeStr -> setPosition(ccp(175-80, m_spNoticeBg->getContentSize().height/2-3));
    m_spNoticeBg -> addChild(m_labelNoticeStr);
    m_labelNoticeStr -> setScale(0.5f);
    
}

void LabaNoticeLayer::showLabaNotice()
{
    std::string str = "";
    std::string str2 = "";
    if(CMainLogic::sharedMainLogic()->m_vctLabaNotice.size() != 0)
    {
        str = CMainLogic::sharedMainLogic()->m_vctLabaNotice[0].sNickName+": ";
        str2 = CMainLogic::sharedMainLogic()->m_vctLabaNotice[0].sContent;
        CMainLogic::sharedMainLogic()->m_vctLabaNotice.erase(CMainLogic::sharedMainLogic()->m_vctLabaNotice.begin());
        m_bShowCurrent = true;
    }
    else
    {
        //隐藏自己
        this->setVisible(false);
        m_bShowCurrent = false;
        return;
    }
    m_labelNoticeStr -> setString(str.c_str());
    m_labelNoticeStr -> setColor(ccc3(255, 0, 0));
    CCLabelTTF* pLabelContent = CCLabelTTF::create(str2.c_str(), "Arial-BoldMT", 32);
    pLabelContent -> setPosition(ccp(m_labelNoticeStr->getContentSize().width, m_labelNoticeStr->getContentSize().height/2));
    pLabelContent -> setAnchorPoint(ccp(0, 0.5f));
    pLabelContent -> setColor(ccc3(236, 255, 186));
    m_labelNoticeStr -> addChild(pLabelContent);
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(2, 31, 46, 255), 960, 50);
    pLayerColor -> setPosition(ccp(160, m_nHeight-22));
    CCDrawNode* pNoticeZone = CCDrawNode::create();//设置滚动通知的显示区域
    CCPoint point[4] = {ccp(160, m_nHeight-35),ccp(1120, m_nHeight-35), ccp(1120, m_nHeight+15),ccp(160, m_nHeight+15)};
    pNoticeZone->drawPolygon(point,4,ccc4f(255,255,255,255),2,ccc4f(255,255,255,255));
    CCClippingNode* pCliper = CCClippingNode::create();
    pCliper->setStencil(pNoticeZone);
    pCliper->setAnchorPoint(CCPointZero);
    pCliper->addChild(pLayerColor);
    this->addChild(pCliper, 2, 101);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(LabaNoticeLayer::removeNotice));
    pLayerColor -> runAction(CCSequence::create(CCMoveBy::create(2.0f, ccp(CLIENT_WIDTH, 0)), CCDelayTime::create(7.0f), func, NULL));
    CCSprite* pGuang = CCSprite::create("h3all/labanoticeguang.png");
    pGuang -> setPosition(ccp(160+pGuang->getContentSize().width/2, m_nHeight-30));
    this -> addChild(pGuang, 3, 102);
    pGuang -> runAction(CCSequence::create(CCMoveBy::create(2.0f, ccp(660, 0)), CCFadeOut::create(0), NULL));
    CCSprite* pGuang2 = CCSprite::create("h3all/labanoticeguang.png");
    pGuang2 -> setPosition(ccp(160+pGuang2->getContentSize().width/2, m_nHeight+22));
    this -> addChild(pGuang2, 3, 103);
    pGuang2 -> setFlipY(true);
    pGuang2 -> runAction(CCSequence::create(CCMoveBy::create(2.0f, ccp(660, 0)), CCFadeOut::create(0), NULL));
}

void LabaNoticeLayer::removeNotice()
{
    m_labelNoticeStr->removeAllChildren();
    
    CCClippingNode* pCliper = (CCClippingNode*)this->getChildByTag(101);
    if(pCliper != NULL)
    {
        pCliper -> removeFromParent();
    }
    CCSprite* sprite = (CCSprite*)this->getChildByTag(102);
    if(sprite != NULL)
    {
        sprite -> removeFromParent();
    }
    sprite = (CCSprite*)this->getChildByTag(103);
    if(sprite != NULL)
    {
        sprite -> removeFromParent();
    }
    showLabaNotice();
}