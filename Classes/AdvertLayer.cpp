#include "header.h"

AdvertLayer::AdvertLayer()
{
    
}

AdvertLayer::~AdvertLayer()
{
    
}

bool AdvertLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    if(CMainLogic::sharedMainLogic()->m_vctAdvertConfig.size() == 0)
    {
        return false;
    }
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    std::string sName = CMainLogic::sharedMainLogic()->m_vctAdvertConfig[0].sName;
    std::string sPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string sPathName = sPath + sName;
    m_spBg = CCSprite::create(sPathName.c_str());
    if(NULL == m_spBg)
    {
        return false;
    }
    else
    {
        m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
        this -> addChild(m_spBg);
        
        CCMenuItemImage* closeItem = CCMenuItemImage::create("h2all/dakacloseitem.png", "h2all/dakacloseitem.png", this, menu_selector(AdvertLayer::clickMenu));
        closeItem -> setPosition(ccp(m_spBg->getContentSize().width-closeItem->getContentSize().width/2+40, m_spBg->getContentSize().height-closeItem->getContentSize().height/2-20));
        CCMenu* pCloseMenu = CCMenu::create(closeItem, NULL);
        pCloseMenu -> setPosition(CCPointZero);
        pCloseMenu -> setAnchorPoint(CCPointZero);
        m_spBg -> addChild(pCloseMenu);
        pCloseMenu -> setTouchPriority(-200);
        
        m_spBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
    }
    
    this -> setTouchEnabled(true);
    
    return true;
}

void AdvertLayer::clickMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(AdvertLayer::removeWnd));
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
}

void AdvertLayer::removeWnd()
{
    this -> removeFromParent();
}

void AdvertLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool AdvertLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}
