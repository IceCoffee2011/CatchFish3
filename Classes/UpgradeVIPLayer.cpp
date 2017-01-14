#include "header.h"

bool UpgradeVIPLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), 1280, 720);
    this->addChild(pLayerColor);
    
    loadUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void UpgradeVIPLayer::loadUI()
{
    CCSprite* pSpBg = CCSprite::create("v2ip/vipupgradebg.png");
    pSpBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(pSpBg);
    CCSprite* pSpTitle = CCSprite::create("v2ip/vipupgradetitle.png");
    pSpTitle -> setPosition(ccp(pSpBg->getContentSize().width/2, pSpBg->getContentSize().height/3*2+50));
    pSpBg -> addChild(pSpTitle);
    CCSprite* pSpInfo = CCSprite::create("v2ip/vipupgradesprite.png");
    pSpInfo -> setPosition(ccp(pSpBg->getContentSize().width/2, pSpBg->getContentSize().height/2-50));
    pSpBg -> addChild(pSpInfo);
    CCSprite* pSpInfo2 = CCSprite::create("v2ip/vipupgradeinfo.png");
    pSpInfo2 -> setPosition(ccp(pSpBg->getContentSize().width/2, pSpBg->getContentSize().height/2+30));
    pSpBg -> addChild(pSpInfo2);
    char temp[16];
    sprintf(temp, "%d", CMainLogic::sharedMainLogic()->m_nVipLevel);
    CCLabelAtlas* pVIPLevel = CCLabelAtlas::create(temp, "v2ip/vipupgradenum.png", 31, 49, '0');
    pVIPLevel -> setPosition(ccp(pSpInfo2->getContentSize().width/2+65, pSpInfo2->getContentSize().height/2+5));
    pVIPLevel -> setAnchorPoint(ccp(0.5f, 0.5f));
    pSpInfo2 -> addChild(pVIPLevel);
    
    CCMenuItemImage* vipItem = CCMenuItemImage::create("v2ip/vipupgradeitem3.png", "v2ip/vipupgradeitem4.png", this, menu_selector(UpgradeVIPLayer::clickMenu));
    vipItem -> setPosition(ccp(pSpBg->getContentSize().width/3+30, pSpBg->getContentSize().height/3-55));
    CCMenuItemImage* chargeItem = CCMenuItemImage::create("v2ip/vipupgradeitem1.png", "v2ip/vipupgradeitem2.png", this, menu_selector(UpgradeVIPLayer::clickClose));
    chargeItem -> setPosition(ccp(pSpBg->getContentSize().width/3*2-30, pSpBg->getContentSize().height/3-55));
    CCMenu* pMenu = CCMenu::create(vipItem, chargeItem, NULL);
    pMenu -> setPosition(CCPointZero);
    pMenu -> setAnchorPoint(CCPointZero);
    pSpBg -> addChild(pMenu);
    pMenu -> setTouchPriority(-206);
    
}

void UpgradeVIPLayer::clickMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CMainLogic::sharedMainLogic()->m_bChargeState = false;
    CMainLogic::sharedMainLogic()->m_bGameToVIP = false;
    CMainLogic::sharedMainLogic()->m_bBYGameToSuper = false;
    CMainLogic::sharedMainLogic()->m_nExitGameToSpecialArm = 0;
    if(GameMainScene::_instance != NULL)
    {
        GameMainScene::GetInstance()->setWndHave(false);
    }
    this->removeFromParent();
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(CCUserDefault::sharedUserDefault()->getFloatForKey("sound", 1.0f));
    CCNode* pNode = CCDirector::sharedDirector()->getRunningScene();
    if(pNode != NULL)
    {
        ChargeLayer* pChargeLayer = (ChargeLayer*)pNode->getChildByTag(20000);
        if(pChargeLayer != NULL)
        {
            pChargeLayer -> removeFromParent();
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        Charge2Layer* pCharge2Layer = (Charge2Layer*)pNode->getChildByTag(20002);
        if(pCharge2Layer != NULL)
        {
            pCharge2Layer -> removeFromParent();
        }
#endif
    }
    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            CMainLogic::sharedMainLogic()->m_nGameToHallVipLevel = CMainLogic::sharedMainLogic()->m_nVipLevel;
            CMainLogic::sharedMainLogic()->ShowMessage("正在打开VIP，请稍候...", eMBNull);
            pHallLayer->sendVIPVersion("VIP");
        }
    }
}

void UpgradeVIPLayer::clickClose(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    this -> removeFromParent();
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(CCUserDefault::sharedUserDefault()->getFloatForKey("sound", 1.0f));
    ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
    if(pChargeLayer != NULL)
    {
        //弹出绑定手机界面
        if(CMainLogic::sharedMainLogic()->m_bCanBandingTel)
        {
            if(CCUserDefault::sharedUserDefault()->getStringForKey("BindingTel", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"))
            {
                CCUserDefault::sharedUserDefault()->setStringForKey("BindingTel", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"));
                CCNode* pNode = CCDirector::sharedDirector()->getRunningScene();
                if(pNode != NULL)
                {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    Charge2Layer* pCharge2Layer = (Charge2Layer*)pNode->getChildByTag(20002);
                    if(pCharge2Layer != NULL)
                    {
                        BandingLayer* pBandingLayer = BandingLayer::create();
                        pCharge2Layer->addChild(pBandingLayer, 100);
                    }
#endif
                    ChargeLayer* pChargeLayer = (ChargeLayer*)pNode->getChildByTag(20000);
                    if(pChargeLayer != NULL)
                    {
                        BandingLayer* pBandingLayer = BandingLayer::create();
                        pChargeLayer->addChild(pBandingLayer, 100);
                    }
                }
            }
        }
    }
    else
    {
        pChargeLayer = ChargeLayer::create();
        CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeLayer, 11, 20000);
    }
    
    
}

void UpgradeVIPLayer::onEnterTransitionDidFinish()
{
    this->setScale(0.0f);
    this -> runAction(CCSequence::create(CCScaleTo::create(0.2f, 1.2), CCScaleTo::create(0.1f,1.0f), NULL));
    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
            pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
        }
    }
    
}

bool UpgradeVIPLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void UpgradeVIPLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -205, true);
}
