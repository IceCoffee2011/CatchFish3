#include "header.h"

OneMoneyLayer::OneMoneyLayer()
{
    
}

OneMoneyLayer::~OneMoneyLayer()
{
    
}

bool OneMoneyLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    loadUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void OneMoneyLayer::loadUI()
{
    m_spOneBg = CCSprite::create("c2harge/onebg.png");
    m_spOneBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spOneBg);
    m_spOneBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
    char temp[16];
    sprintf(temp, ":%d", CMainLogic::sharedMainLogic()->m_nOneCoinCount);
    CCLabelAtlas* pLabelCoin = CCLabelAtlas::create(temp, "c2harge/onenum.png", 27, 30, '0');
    pLabelCoin -> setAnchorPoint(ccp(0.5f, 0.5f));
    pLabelCoin -> setPosition(ccp(268, 170));
    m_spOneBg -> addChild(pLabelCoin);
    sprintf(temp, ":%d", CMainLogic::sharedMainLogic()->m_cbOneHuafei);
    CCLabelAtlas* pLabelHuafei = CCLabelAtlas::create(temp, "c2harge/onenum.png", 27, 30, '0');
    pLabelHuafei -> setAnchorPoint(ccp(0.5f, 0.5f));
    pLabelHuafei -> setPosition(ccp(482, 170));
    m_spOneBg -> addChild(pLabelHuafei);
    sprintf(temp, ":%d", CMainLogic::sharedMainLogic()->m_cbOneSpeedCount);
    CCLabelAtlas* pLabelSpeed = CCLabelAtlas::create(temp, "c2harge/onenum.png", 27, 30, '0');
    pLabelSpeed -> setAnchorPoint(ccp(0.5f, 0.5f));
    pLabelSpeed -> setPosition(ccp(695, 170));
    m_spOneBg -> addChild(pLabelSpeed);
    sprintf(temp, ":%d", CMainLogic::sharedMainLogic()->m_cbOneSuodingCount);
    CCLabelAtlas* pLabelSuoding = CCLabelAtlas::create(temp, "c2harge/onenum.png", 27, 30, '0');
    pLabelSuoding -> setAnchorPoint(ccp(0.5f, 0.5f));
    pLabelSuoding -> setPosition(ccp(908, 170));
    m_spOneBg -> addChild(pLabelSuoding);
    
    CCMenuItemImage* getItem = CCMenuItemImage::create("c2harge/oneitem.png", "c2harge/oneitem.png", this, menu_selector(OneMoneyLayer::clickMenu));
    getItem -> setPosition(ccp(m_spOneBg->getContentSize().width/2+70, 60));
    getItem -> setTag(101);
    getItem -> setScale(1.2f);
    CCMenuItemImage* closeButton = CCMenuItemImage::create("h2all/dakacloseitem.png","h2all/dakacloseitem.png",this,menu_selector(OneMoneyLayer::clickMenu));
    closeButton->setTag(102);
    closeButton->setPosition(ccp(m_spOneBg->getContentSize().width-72, m_spOneBg->getContentSize().height-95));
    CCMenu* closeMenu = CCMenu::create(getItem, closeButton, NULL);
    closeMenu->setPosition(CCPointZero);
    closeMenu->setAnchorPoint(CCPointZero);
    m_spOneBg->addChild(closeMenu, 2);
    closeMenu->setTouchPriority(-202);
}

void OneMoneyLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    if(item->getTag() == 101)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eOneMoneyGetTag);
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        ChargeUI* pChargeUI = ChargeUI::create(1, 3);//第1个参数表示充值金额，第二个参数表示1元购
        CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeUI, 13, 20003);
    }
    else if(item->getTag() == 102)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eOneMoneyClsoeTag);
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(OneMoneyLayer::removeWnd));
        m_spOneBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
    }
}

void OneMoneyLayer::removeWnd()
{
    this -> removeFromParent();
    //如果是在游戏内没有金币后自动弹出的，那么关闭此框就要跳转到充值界面
    if(GameMainScene::_instance != NULL)
    {
        if(GameMainScene::_instance->getAutoOneCharge())
        {
            GameMainScene::_instance->setAutoOneCharge(false);
            GameMainScene::GetInstance()->setHaveNoScore(true);
            //打开充值界面
            ChargeLayer* pChargeLayer = ChargeLayer::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeLayer, 11, 20000);
        }
    }
}

void OneMoneyLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -201, true);
}

bool OneMoneyLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}
