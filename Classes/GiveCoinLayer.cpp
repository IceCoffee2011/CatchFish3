//
//  GiveCoinLayer.cpp
//  CatchFish
//
//  Created by xiaosong1531 on 15/10/20.
//
//

#include "header.h"

GiveCoinLayer* GiveCoinLayer::create(int nOpenType, bool bShowFirstCharge,std::string sDescription, int nScore)
{
    GiveCoinLayer* pRet = new GiveCoinLayer();
    if(pRet && pRet->init(nOpenType, bShowFirstCharge, sDescription, nScore))
    {
        pRet -> autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool GiveCoinLayer::init(int nOpenType, bool bShowFirstCharge,std::string sDescription, int nScore)
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_nOpenType = nOpenType;
    m_bShowFirstCharge = bShowFirstCharge;
    m_nChargeType = 1;
    m_sDescription = sDescription;
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    m_nodeGiveCoin = CCSprite::create();
    m_nodeGiveCoin -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(m_nodeGiveCoin);
    
    if(bShowFirstCharge)
    {
        loadFirstChargeUI();
    }
    else
    {
        if(CMainLogic::sharedMainLogic()->m_nVipLevel == 0)
        {
            loadUI();
        }
        else
        {
            loadVIPUI(nScore);
        }
    }
    
    
    this->setTouchEnabled(true);
    
    return true;
}

void GiveCoinLayer::loadVIPUI(int nScore)
{
    CCSprite* pVipBg = CCSprite::create("hall/givecoinvipbg.png");
    pVipBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(pVipBg);
    
    CCMenuItemImage* closeButton = CCMenuItemImage::create("mail/mailcloseItem0.png","mail/mailcloseItem1.png",this,menu_selector(GiveCoinLayer::closeMenu));
    closeButton->setTag(111);
    closeButton->setPosition(ccp(pVipBg->getContentSize().width-25, pVipBg->getContentSize().height-25));
    closeButton -> setScale(1.1f);
    CCMenu* pMenuClose = CCMenu::create(closeButton, NULL);
    pMenuClose->setPosition(CCPointZero);
    pMenuClose->setAnchorPoint(CCPointZero);
    pVipBg->addChild(pMenuClose, 2);
    pMenuClose->setTouchPriority(-140);
    
    char temp[16];
    sprintf(temp, "%d", CMainLogic::sharedMainLogic()->m_nVipLevel);
    CCLabelAtlas* vipLabel = CCLabelAtlas::create(temp, "hall/givecoinnum.png", 28, 36, '0');
    vipLabel -> setPosition(ccp(236, 312));
    pVipBg -> addChild(vipLabel);
    CCLabelAtlas* vipLabel2 = CCLabelAtlas::create(temp, "hall/givecoinnum.png", 28, 36, '0');
    vipLabel2 -> setPosition(ccp(573, 312));
    pVipBg -> addChild(vipLabel2);
    CCMenuItemImage* getItem = CCMenuItemImage::create("hall/givecoinitem.png", "hall/givecoinitem.png", this, menu_selector(GiveCoinLayer::clickMenu));
    getItem->setTag(115);
    getItem -> setPosition(ccp(pVipBg->getContentSize().width/2, 110));
    CCMenu* getMenu = CCMenu::create(getItem, NULL);
    getMenu -> setPosition(CCPointZero);
    getMenu -> setAnchorPoint(CCPointZero);
    pVipBg -> addChild(getMenu);
    getMenu -> setTouchPriority(-140);
    
    sprintf(temp, "%d", nScore);
    m_getCoinAtlas = CCLabelAtlas::create(temp, "hall/givecoinnum.png", 28, 36, '0');
    m_getCoinAtlas -> setAnchorPoint(ccp(0.5f, 0.5f));
    m_getCoinAtlas -> setPosition(ccp(170, pVipBg->getContentSize().height/2+70));
    pVipBg -> addChild(m_getCoinAtlas);
}

void GiveCoinLayer::loadFirstChargeUI()
{
    CCSprite* pBg = CCSprite::create("c2harge/firstchargebg2.png");
    pBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(pBg);
    CCMenuItemImage* closeButton = CCMenuItemImage::create("mail/mailcloseItem0.png","mail/mailcloseItem1.png",this,menu_selector(GiveCoinLayer::closeMenu));
    closeButton->setTag(110);
    closeButton->setPosition(ccp(pBg->getContentSize().width-25, pBg->getContentSize().height-25));
    closeButton -> setScale(1.2f);
    CCMenu* pMenuClose = CCMenu::create(closeButton, NULL);
    pMenuClose->setPosition(CCPointZero);
    pMenuClose->setAnchorPoint(CCPointZero);
    pBg->addChild(pMenuClose, 2);
    pMenuClose->setTouchPriority(-140);
    //alipay wechat
    CCMenuItemImage* alipayItem = CCMenuItemImage::create("c2harge/alipayItem.png", "c2harge/alipayItem.png", this, menu_selector(GiveCoinLayer::clickMenu));
    alipayItem -> setTag(108);
    alipayItem -> setPosition(ccp(pBg->getContentSize().width/4+20, 135));
    CCMenuItemImage* wechatItem = CCMenuItemImage::create("c2harge/wechatItem.png", "c2harge/wechatItem.png", this, menu_selector(GiveCoinLayer::clickMenu));
    wechatItem -> setTag(109);
    wechatItem -> setPosition(ccp(pBg->getContentSize().width/4*3-20, 135));
    CCMenu* pChargeMenu = CCMenu::create(alipayItem, wechatItem, NULL);
    pChargeMenu -> setPosition(CCPointZero);
    pChargeMenu -> setAnchorPoint(CCPointZero);
    pBg -> addChild(pChargeMenu);
    pChargeMenu -> setTouchPriority(-140);
}

void GiveCoinLayer::loadUI()
{
    CCSprite* pSpDi = CCSprite::create("messagebox/messageboxbg.png");
    pSpDi -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    addChild(pSpDi);
    CCMenuItemImage* closeButton = CCMenuItemImage::create("h2all/dakacloseitem.png","h2all/dakacloseitem.png",this,menu_selector(GiveCoinLayer::closeMenu));
    closeButton->setTag(101);
    closeButton->setPosition(ccp(pSpDi->getContentSize().width-40, pSpDi->getContentSize().height-34));
    CCMenu* pMenuClose = CCMenu::create(closeButton, NULL);
    pMenuClose->setPosition(CCPointZero);
    pMenuClose->setAnchorPoint(CCPointZero);
    pSpDi->addChild(pMenuClose, 2);
    pMenuClose->setTouchPriority(-140);
    
    CCSize size(600,300);//您还有免费金币没有领取，快点来领取吧！
    m_pLabelTTF = CCLabelTTF::create(m_sDescription.c_str(), "Arial", 40,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    m_pLabelTTF->setColor(ccc3(188, 1.3, 21));
    m_pLabelTTF -> setPosition(ccp(pSpDi->getContentSize().width/2, pSpDi->getContentSize().height/2+20));
    pSpDi->addChild(m_pLabelTTF, 5);
    
    CCMenuItemImage* getItem = CCMenuItemImage::create("freecoin/freecoingetItem3.png", "freecoin/freecoingetItem3.png", this, menu_selector(GiveCoinLayer::clickMenu));
    getItem->setTag(106);
    CCMenuItemImage* chargeItem = CCMenuItemImage::create("freecoin/freecoingetItem4.png", "freecoin/freecoingetItem4.png", this, menu_selector(GiveCoinLayer::clickMenu));
    chargeItem->setPosition(ccp(pSpDi->getContentSize().width/4*3, 120));
    chargeItem->setTag(107);
    CCMenu* pMenuGet = CCMenu::create(getItem, chargeItem, NULL);
    pMenuGet -> setPosition(CCPointZero);
    pMenuGet -> setAnchorPoint(CCPointZero);
    pSpDi -> addChild(pMenuGet, 4);
    pMenuGet -> setTouchPriority(-140);
    if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        chargeItem -> setVisible(false);
        getItem->setPosition(ccp(pSpDi->getContentSize().width/2, 120));
    }
    else
    {
        getItem->setPosition(ccp(pSpDi->getContentSize().width/4, 120));
    }
    
    
    m_nodeGiveCoin->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void GiveCoinLayer::closeMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    if(item->getTag() == 101)
    {
        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(GiveCoinLayer::removeWnd));
        m_nodeGiveCoin->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
    }
    else if(item->getTag() == 110)
    {
        this->removeFromParent();
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameNoScoreClose);
        //先弹出充值界面，然后获取是否有救济金
        GameMainScene::GetInstance()->setHaveNoScore(true);
        ChargeLayer* pChargeLayer = ChargeLayer::create();
        CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeLayer, 11, 20000);
        
        return ;
        //查看有没有免费金币可以领取
//        if(GameMainScene::_instance != NULL)
//        {
//            if(GameMainScene::GetInstance()->getShowExitGame())
//            {
//                CMainLogic::sharedMainLogic()->m_bGameChargeStatus = false;
//                CMainLogic::sharedMainLogic()->switchLayer(eHallLayer);
//            }
//            else if(CMainLogic::sharedMainLogic()->m_nBankruptcyCount < CMainLogic::sharedMainLogic()->m_nGiveTotal && GameMainScene::_instance->getUserCurrentScore() < CMainLogic::sharedMainLogic()->m_nGiveScore)//有领取次数，并且玩家分数小于领取限制分数，才能领取救济金
//            {
//                CMainLogic::sharedMainLogic()->m_cbCanGetBankruptcy = 1;
//                //发送是否可以领取救济金的消息
//                GameMainScene::_instance->m_pClientKernel->SendSocketData(MDM_GR_USER, SUB_GRR_QUERYBANKCUPTINFO);
//            }
//        }
    }
    else if(item->getTag() == 111)
    {
        this -> removeFromParent();
    }
}

void GiveCoinLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    if(item->getTag() == 106)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        //退出游戏，进入到大厅中免费金币界面
        if(GameMainScene::_instance != NULL)
        {
            if(m_nOpenType != 0)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eCanGetFreeCoinTag);
                CMainLogic::sharedMainLogic()->ShowMessage("正在领取，请稍候", eMBExitGame);
                GameMainScene::_instance->m_pClientKernel->SendSocketData(MDM_GR_USER, SUB_GRR_GET_BANKCRUPT_SCORE);
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameNoCoinToFreeCoin);
                CMainLogic::sharedMainLogic()->m_bEnterGiveCoin = true;
                CMainLogic::sharedMainLogic()->m_bGameToVIP = false;
                CMainLogic::sharedMainLogic()->m_bBYGameToSuper = false;
                CMainLogic::sharedMainLogic()->m_nExitGameToSpecialArm = 0;
                GameMainScene::GetInstance()->exitGame();
            }
            
        }
    }
    else if(item->getTag() == 107)
    {
        if(GameMainScene::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameNoCoinToCharge);
            closeMenu(object);
            CCMenuItem* item = CCMenuItem::create();
            item -> setTag(302);
            GameMainScene::_instance->itemCallBack(item);
        }
    }
    else if(item->getTag() == 108)//支付宝
    {
        m_nChargeType = 1;
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        CMainLogic::sharedMainLogic()->m_nBuyGameType = 1;//购买金币
        if(GameMainScene::_instance != NULL)//在游戏中
        {
            CMainLogic::sharedMainLogic()->m_bGameChargeStatus = true;
            if(GameMainScene::GetInstance()->getShowExitGame())
            {
                CMainLogic::sharedMainLogic()->alipayCharge(10, 1, false);
            }
            else
            {
                GameMainScene::_instance->exitGame();//离开座位
            }
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameNoScoreAlipay);
        }
        //打开ChargeLayer类（设置为隐藏），并且下订单（支付宝首充10元）
//        ChargeLayer* pChargeLayer = ChargeLayer::create(10);
//        CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeLayer, 11, 20000);
//        pChargeLayer -> setVisible(false);
//        CMainLogic::sharedMainLogic()->m_bClickMessageCharge = false;
//        pChargeLayer -> setCurrentSelectedType(100);
//        pChargeLayer -> selectedChargeTypeAndPrise(100, 0);
    }
    else if(item->getTag() == 109)//微信
    {
        m_nChargeType = 2;
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        //下订单之前需要先退出房间，所以先发送离开房间的消息
        CMainLogic::sharedMainLogic()->m_nBuyGameType = 1;//购买金币
        //一旦点击了卡片，那么就要判断在没在游戏中，如果在的话，就要先退出游戏
        if(GameMainScene::_instance != NULL)//在游戏中
        {
            CMainLogic::sharedMainLogic()->m_bGameChargeStatus = true;
            if(GameMainScene::GetInstance()->getShowExitGame())
            {
                CMainLogic::sharedMainLogic()->wxCharge(10, 1, false);
            }
            else
            {
                GameMainScene::_instance->exitGame();//离开座位
            }
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameNoScoreWechat);
        }
        //打开ChargeLayer类（设置为隐藏），并且下订单（微信首充10元）
//        ChargeLayer* pChargeLayer = ChargeLayer::create(10);
//        CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeLayer, 11, 20000);
//        pChargeLayer -> setVisible(false);
//        CMainLogic::sharedMainLogic()->m_bClickMessageCharge = false;
//        pChargeLayer -> setCurrentSelectedType(101);
//        pChargeLayer -> selectedChargeTypeAndPrise(101, 0);
    }
    else if(item->getTag() == 115)//每日领取救济金(VIP)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        this -> removeFromParent();
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eCanGetFreeCoinTag);
        CMainLogic::sharedMainLogic()->ShowMessage("正在领取，请稍候", eMBExitGame);
        GameMainScene::_instance->m_pClientKernel->SendSocketData(MDM_GR_USER, SUB_GRR_GET_BANKCRUPT_SCORE);
    }
}

void GiveCoinLayer::removeWnd()
{
    this->removeFromParent();
}

void GiveCoinLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool GiveCoinLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}
