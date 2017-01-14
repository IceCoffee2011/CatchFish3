#include "header.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../platform/android/jni/JniHelper.h"
#include "jni.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "FMLayerWebView.h"
#endif

FirstChargeLayer* FirstChargeLayer::create(int nOpenType, bool bShowGetMenu)
{
    FirstChargeLayer* pRet = new FirstChargeLayer;
    if(pRet && pRet->init(nOpenType, bShowGetMenu))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool FirstChargeLayer::init(int nOpenType, bool bShowGetMenu)
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    m_nOpenType = nOpenType;
    m_nLabaTimes = 0;
    
    if(m_nOpenType != 7)
    {
        CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
        this->addChild(pLayerColor);
    }
    else
    {
        CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 100), CLIENT_WIDTH, CLIENT_HEIGHT);
        this->addChild(pLayerColor);
    }
    
    if(m_nOpenType == 0)//首充
    {
        loadUI();
    }
    else if(m_nOpenType == 1)//客服
    {
        loadServiceUI();
    }
    else if(m_nOpenType == 2)//兑换码
    {
        loadDuiHuanUI();
    }
    else if(m_nOpenType == 3)//喇叭
    {
        loadLabaUI();
    }
    else if(m_nOpenType == 6)//第一次打中话费界面
    {
        loadFirstMobileMoney(bShowGetMenu);
    }
    else if(m_nOpenType == 7)
    {
        loadDantouLayer();
    }
    else if(m_nOpenType == 8)//游戏内分不够充值提示
    {
        loadRemindCharge();
    }
    else if(m_nOpenType == 9 || m_nOpenType == 10)//大厅千炮房或者万炮房分不够充值提示
    {
        loadHallRemindCharge(m_nOpenType);
    }
    else if(m_nOpenType == 11)//大厅分数太多提示
    {
        loadHallScoreHighRemind();
    }
    else if(m_nOpenType == 12)
    {
        loadPaiPaiLeBg();
    }
    this->setTouchEnabled(true);
    
    return true;
}

void FirstChargeLayer::loadPaiPaiLeBg()
{
    int nTime = 60;
    if(GameMainScene::_instance != NULL)
    {
        nTime = GameMainScene::_instance->getShowMenuTime();
    }
    CCSprite* pPaiBg = CCSprite::create("CatchFish2006/remindpaibg.png");
    pPaiBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(pPaiBg);
    CCMenuItemImage* startItem = CCMenuItemImage::create("CatchFish2006/remindpaiitem.png", "CatchFish2006/remindpaiitem.png", this, menu_selector(FirstChargeLayer::clickMenu));
    startItem -> setTag(110);
    startItem -> setPosition(ccp(pPaiBg->getContentSize().width/2, 90));
    CCMenu* startMenu = CCMenu::create(startItem, NULL);
    startMenu -> setPosition(CCPointZero);
    startMenu -> setAnchorPoint(CCPointZero);
    pPaiBg -> addChild(startMenu);
    startMenu -> setTouchPriority(-202);
    char temp[8];
    sprintf(temp, "%d秒", nTime);
    m_PPLLabel = CCLabelTTF::create(temp, "Arial-BoldMT", 32);
    m_PPLLabel -> setPosition(ccp(pPaiBg->getContentSize().width-200, 90));
    pPaiBg -> addChild(m_PPLLabel);
    m_PPLLabel -> setColor(ccc3(255, 0, 0));
    this->schedule(schedule_selector(FirstChargeLayer::showPPLTime), 1.0f);
}

void FirstChargeLayer::showPPLTime()
{
    if(GameMainScene::_instance != NULL)
    {
        int nTime = GameMainScene::_instance->getShowMenuTime() - 1;
        GameMainScene::_instance->setShowMenuTime(nTime);
        if(nTime<1)
        {
            this -> unschedule(schedule_selector(FirstChargeLayer::showPPLTime));
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, ePPLZhunbei);
            this -> removeFromParent();
            GameMainScene::_instance -> setPaiItemEnable(true);
            SimpleAudioEngine::sharedEngine()->playEffect("Music/paistartgame.wav");
            return ;
        }
        char temp[16];
        sprintf(temp, "%d秒", nTime);
        m_PPLLabel -> setString(temp);
    }
    
}

void FirstChargeLayer::loadHallScoreHighRemind()
{
    CCSprite* pSpBg = CCSprite::create("hall/hallscorehigh.png");
    pSpBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(pSpBg);
    CCMenuItemImage* OKItem = CCMenuItemImage::create("hall/quedingitem.png", "hall/quedingitem.png", this, menu_selector(FirstChargeLayer::clickRemindCharge));
    OKItem -> setTag(103);
    OKItem -> setPosition(ccp(pSpBg->getContentSize().width/2, 120));
    CCMenu* pMenu = CCMenu::create(OKItem, NULL);
    pMenu -> setPosition(CCPointZero);
    pMenu -> setAnchorPoint(CCPointZero);
    pSpBg -> addChild(pMenu);
    pMenu -> setTouchPriority(-202);
}

void FirstChargeLayer::loadHallRemindCharge(int nType)
{
    CCSprite* pSpBg = NULL;
    if(nType == 9)
    {
        pSpBg = CCSprite::create("hall/qianpaoremind.png");
    }
    else
    {
        pSpBg = CCSprite::create("hall/wanpaoremind.png");
    }
    pSpBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(pSpBg);
    CCMenuItemImage* chargeItem = CCMenuItemImage::create("hall/getchargeitem.png", "hall/getchargeitem.png", this, menu_selector(FirstChargeLayer::clickRemindCharge));
    if(nType == 9)
    {
        chargeItem -> setTag(104);
    }
    else
    {
        chargeItem -> setTag(105);
    }
    chargeItem -> setPosition(ccp(pSpBg->getContentSize().width/3*2, 193));
    CCMenuItemImage * closeItem = CCMenuItemImage::create("Dragon/dragoncloseitem.png","Dragon/dragoncloseitem.png",this,menu_selector(FirstChargeLayer::clickRemindCharge));
    closeItem->setPosition(ccp(pSpBg->getContentSize().width-20, pSpBg->getContentSize().height-20));
    closeItem -> setTag(102);
    CCMenu* pMenu = CCMenu::create(chargeItem, closeItem, NULL);
    pMenu -> setPosition(CCPointZero);
    pMenu -> setAnchorPoint(CCPointZero);
    pMenu -> setTouchPriority(-202);
    pSpBg -> addChild(pMenu);
    
    pSpBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void FirstChargeLayer::loadRemindCharge()
{
    m_spRemindCharge = CCSprite::create("Dragon/remindchargebg.png");
    m_spRemindCharge -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spRemindCharge);
    CCMenuItemImage* chargeItem = CCMenuItemImage::create("Dragon/remindchargeitem.png", "Dragon/remindchargeitem.png", this, menu_selector(FirstChargeLayer::clickRemindCharge));
    chargeItem -> setTag(101);
    chargeItem -> setPosition(ccp(m_spRemindCharge->getContentSize().width/2, 120));
    CCMenuItemImage * closeItem = CCMenuItemImage::create("Dragon/dragoncloseitem.png","Dragon/dragoncloseitem.png",this,menu_selector(FirstChargeLayer::clickRemindCharge));
    closeItem->setPosition(ccp(m_spRemindCharge->getContentSize().width-20, m_spRemindCharge->getContentSize().height-20));
    closeItem -> setTag(102);
    CCMenu* pMenu = CCMenu::create(chargeItem, closeItem, NULL);
    pMenu->setTouchPriority(-202);
    pMenu -> setPosition(CCPointZero);
    pMenu -> setAnchorPoint(CCPointZero);
    m_spRemindCharge->addChild(pMenu);
}

void FirstChargeLayer::clickRemindCharge(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case 101://charge
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            this -> removeFromParent();
            ChargeLayer* pChargeLayer = ChargeLayer::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeLayer, 11, 20000);
            break;
        }
        case 102://close
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
            this -> removeFromParent();
            //提醒免费金币
            if(GameMainScene::_instance != NULL)
            {
                if(GameMainScene::GetInstance()->getCellScore()<1000)
                {
                    if(CMainLogic::sharedMainLogic()->m_nBankruptcyCount < CMainLogic::sharedMainLogic()->m_nGiveTotal && GameMainScene::_instance->getUserCurrentScore() < CMainLogic::sharedMainLogic()->m_nGiveScore)//有领取次数，并且玩家分数小于领取限制分数，才能领取救济金
                    {
                        CMainLogic::sharedMainLogic()->m_cbCanGetBankruptcy = 1;
                        //发送是否可以领取救济金的消息
                        GameMainScene::_instance->m_pClientKernel->SendSocketData(MDM_GR_USER, SUB_GRR_QUERYBANKCUPTINFO);
                    }
                }
            }
            break;
        }
        case 103://大厅分数过高提醒
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            this -> removeFromParent();
            break;
        }
        case 104://千炮房98元充值
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            ChargeUI* pChargeUI = ChargeUI::create(98, 0);//0为普通类型充值，1为VIP升级, 3为1元购，2为10元限购
            CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeUI, 13, 20003);
            this -> removeFromParent();
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallQianpao);
            break;
        }
        case 105://万炮房188元充值
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            ChargeUI* pChargeUI = ChargeUI::create(188, 0);//0为普通类型充值，1为VIP升级, 3为1元购，2为10元限购
            CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeUI, 13, 20003);
            this -> removeFromParent();
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallWanpao);
            break;
        }
        default:
            break;
    }
}

void FirstChargeLayer::loadDantouLayer()
{
    CCMenuItemImage* tongdantouItem = CCMenuItemImage::create("Dragon/dankuang.png", "Dragon/dankuang.png", this, menu_selector(FirstChargeLayer::selectDantou));
    tongdantouItem -> setTag(101);
    tongdantouItem -> setPosition(ccp(CLIENT_WIDTH/4+60, CLIENT_HEIGHT/2));
    CCSprite* tongdantouSp = CCSprite::create("h3all/bbtongdantou.png");
    tongdantouSp -> setPosition(ccp(tongdantouItem->getContentSize().width/2, tongdantouItem->getContentSize().height/2));
    tongdantouItem -> addChild(tongdantouSp);
    CCSprite* tongdantouTitle = CCSprite::create("CatchFish2001/tongdantoutitle.png");
    tongdantouTitle -> setPosition(ccp(tongdantouSp->getContentSize().width/2, tongdantouSp->getContentSize().height+40));
    tongdantouSp -> addChild(tongdantouTitle);
    CCSprite* tongdantouInfo = CCSprite::create("CatchFish2001/tongdantouinfo.png");
    tongdantouInfo -> setPosition(ccp(tongdantouSp->getContentSize().width/2, tongdantouSp->getContentSize().height-170));
    tongdantouSp -> addChild(tongdantouInfo);
    
    CCMenuItemImage* yindantouItem = CCMenuItemImage::create("Dragon/dankuang.png", "Dragon/dankuang.png", this, menu_selector(FirstChargeLayer::selectDantou));
    yindantouItem -> setTag(102);
    yindantouItem -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    CCSprite* yindantouSp = CCSprite::create("h3all/bbyindantou.png");
    yindantouSp -> setPosition(ccp(yindantouItem->getContentSize().width/2, yindantouItem->getContentSize().height/2));
    yindantouItem -> addChild(yindantouSp);
    CCSprite* yindantouTitle = CCSprite::create("CatchFish2001/yindantoutitle.png");
    yindantouTitle -> setPosition(ccp(yindantouSp->getContentSize().width/2, yindantouSp->getContentSize().height+40));
    yindantouSp -> addChild(yindantouTitle);
    CCSprite* yindantouInfo = CCSprite::create("CatchFish2001/yindantouinfo.png");
    yindantouInfo -> setPosition(ccp(yindantouSp->getContentSize().width/2, yindantouSp->getContentSize().height-170));
    yindantouSp -> addChild(yindantouInfo);
    
    CCMenuItemImage* jindantouItem = CCMenuItemImage::create("Dragon/dankuang.png", "Dragon/dankuang.png", this, menu_selector(FirstChargeLayer::selectDantou));
    jindantouItem -> setTag(103);
    jindantouItem -> setPosition(ccp(CLIENT_WIDTH/4*3-60, CLIENT_HEIGHT/2));
    CCSprite* jindantouSp = CCSprite::create("h3all/bbjindantou.png");
    jindantouSp -> setPosition(ccp(jindantouItem->getContentSize().width/2, jindantouItem->getContentSize().height/2));
    jindantouItem -> addChild(jindantouSp);
    CCSprite* jindantouTitle = CCSprite::create("CatchFish2001/jindantoutitle.png");
    jindantouTitle -> setPosition(ccp(jindantouSp->getContentSize().width/2, jindantouSp->getContentSize().height+40));
    jindantouSp -> addChild(jindantouTitle);
    CCSprite* jindantouInfo = CCSprite::create("CatchFish2001/jindantouinfo.png");
    jindantouInfo -> setPosition(ccp(jindantouSp->getContentSize().width/2, jindantouSp->getContentSize().height-170));
    jindantouSp -> addChild(jindantouInfo);
    
    tongdantouSp -> setScale(1.2f);
    yindantouSp -> setScale(1.2f);
    jindantouSp -> setScale(1.2f);
    CCMenu* menu = CCMenu::create(tongdantouItem, yindantouItem, jindantouItem, NULL);
    menu -> setPosition(CCPointZero);
    menu -> setAnchorPoint(CCPointZero);
    this -> addChild(menu);
    menu->setTouchPriority(-202);
    m_labelTong = CCLabelAtlas::create("0", "h3all/zhunzhunum.png", 14, 17, '0');
    m_labelTong -> setAnchorPoint(ccp(1, 0.5f));
    m_labelTong -> setScale(1.5f);
    m_labelTong -> setPosition(ccp(108, 18));
    tongdantouSp -> addChild(m_labelTong, 1, 1);
    m_labelYin = CCLabelAtlas::create("0", "h3all/zhunzhunum.png", 14, 17, '0');
    m_labelYin -> setAnchorPoint(ccp(1, 0.5f));
    m_labelYin -> setScale(1.5f);
    m_labelYin -> setPosition(ccp(108, 18));
    yindantouSp -> addChild(m_labelYin, 1, 1);
    m_labelJin = CCLabelAtlas::create("0", "h3all/zhunzhunum.png", 14, 17, '0');
    m_labelJin -> setAnchorPoint(ccp(1, 0.5f));
    m_labelJin -> setScale(1.5f);
    m_labelJin -> setPosition(ccp(108, 18));
    jindantouSp -> addChild(m_labelJin, 1, 1);
}

void FirstChargeLayer::setDantouNum(int nChujiCount, int nZhongjiCount, int nGaojiCount)
{
    char temp[16];
    sprintf(temp, "%d", nChujiCount);
    m_labelTong -> setString(temp);
    sprintf(temp, "%d", nZhongjiCount);
    m_labelYin -> setString(temp);
    sprintf(temp, "%d", nGaojiCount);
    m_labelJin -> setString(temp);
}

void FirstChargeLayer::selectDantou(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    if(item == NULL) return;
    if(item->getTag() == 101)//铜弹头
    {
        int nCount = atoi(m_labelTong->getString());
        if(nCount <= 0)
        {
            CMainLogic::sharedMainLogic()->ShowMessage("对不起，您当前拥有此弹头数量不足");
            return;
        }
        if(GameMainScene::_instance != NULL)
        {
            GameMainScene::_instance->setSelectDantouType(0);
            GameMainScene::_instance->showDantouInfo();
            GameMainScene::GetInstance()->setDantouUsing(true);
        }
    }
    else if(item->getTag() == 102)//银弹头
    {
        int nCount = atoi(m_labelYin->getString());
        if(nCount <= 0)
        {
            CMainLogic::sharedMainLogic()->ShowMessage("对不起，您当前拥有此弹头数量不足");
            return;
        }
        if(GameMainScene::_instance != NULL)
        {
            GameMainScene::_instance->setSelectDantouType(1);
            GameMainScene::_instance->showDantouInfo();
            GameMainScene::GetInstance()->setDantouUsing(true);
        }
    }
    else if(item->getTag() == 103)//金弹头
    {
        int nCount = atoi(m_labelJin->getString());
        if(nCount <= 0)
        {
            CMainLogic::sharedMainLogic()->ShowMessage("对不起，您当前拥有此弹头数量不足");
            return;
        }
        if(GameMainScene::_instance != NULL)
        {
            GameMainScene::_instance->setSelectDantouType(2);
            GameMainScene::_instance->showDantouInfo();
            GameMainScene::GetInstance()->setDantouUsing(true);
        }
    }
    this -> removeFromParent();
    if(GameMainScene::_instance != NULL)
    {
        GameMainScene::GetInstance()->setWndHave(false);
        GameMainScene::GetInstance()->setAutoFire();
        GameMainScene::GetInstance()->setMatchShowStatus(true);
        GameMainScene::GetInstance()->setClickDantouMenu(false);
    }
}

void FirstChargeLayer::loadFirstMobileMoney(bool bShowGetMenu)
{
    if(GameMainScene::_instance != NULL)
    {
        CCSprite* pBg = CCSprite::create("c2harge/mobilefirstbg.png");
        pBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+50));
        this -> addChild(pBg);
        pBg -> setScale(2.0f);
        char temp[16];
        sprintf(temp, "%d", GameMainScene::_instance->getMobileMoney());
        CCLabelAtlas* pLabel = CCLabelAtlas::create(temp, "c2harge/mobilefirstnum.png", 78, 132, '0');
        pLabel -> setAnchorPoint(ccp(0.5f, 0.5f));
        pLabel -> setPosition(ccp(pBg->getContentSize().width/2-45, pBg->getContentSize().height/3-13));
        pBg -> addChild(pLabel);
        pLabel -> setScale(0.5f);
        if(bShowGetMenu)
        {
            CCMenuItemImage* shareItem = CCMenuItemImage::create("c2harge/mobilefirstshareItem.png", "c2harge/mobilefirstshareItem.png", this, menu_selector(FirstChargeLayer::clickMobileFirst));
            shareItem -> setTag(101);
            shareItem -> setScale(0.5f);
            shareItem -> setPosition(ccp(pBg->getContentSize().width/5*4, -45));
            CCMenuItemImage* getItem = CCMenuItemImage::create("c2harge/mobilefirstgetItem.png", "c2harge/mobilefirstgetItem.png", this, menu_selector(FirstChargeLayer::clickMobileFirst));
            getItem -> setTag(102);
            getItem -> setScale(0.5f);
            getItem -> setPosition(ccp(pBg->getContentSize().width/5, -45));
            CCMenu* pMenu = CCMenu::create(shareItem, getItem, NULL);
            pMenu -> setPosition(CCPointZero);
            pMenu -> setAnchorPoint(CCPointZero);
            pBg -> addChild(pMenu);
            pMenu -> setTouchPriority(-202);
            pMenu -> setVisible(false);
            pMenu -> runAction(CCSequence::create(CCDelayTime::create(0.7f), CCShow::create(), NULL));
        }
        else
        {
            CCMenuItemImage* shareItem = NULL;
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
            shareItem = CCMenuItemImage::create("c2harge/mobilefirstgetItem.png", "c2harge/mobilefirstgetItem.png", this, menu_selector(FirstChargeLayer::clickMobileFirst));
#else
            shareItem = CCMenuItemImage::create("c2harge/mobilefirstshareItem.png", "c2harge/mobilefirstshareItem.png", this, menu_selector(FirstChargeLayer::clickMobileFirst));
#endif
            shareItem -> setTag(101);
            shareItem -> setScale(0.5f);
            shareItem -> setPosition(ccp(pBg->getContentSize().width/2, -45));
            CCMenu* pMenu = CCMenu::create(shareItem, NULL);
            pMenu -> setPosition(CCPointZero);
            pMenu -> setAnchorPoint(CCPointZero);
            pBg -> addChild(pMenu);
            pMenu -> setTouchPriority(-202);
            pMenu -> setVisible(false);
            pMenu -> runAction(CCSequence::create(CCDelayTime::create(0.7f), CCShow::create(), NULL));
        }
        pBg -> runAction(CCSequence::create(CCScaleTo::create(0, 0.2f), CCScaleTo::create(0.4f, 2.4f), CCScaleTo::create(0.2f, 2.0f), NULL));
    }
}

void FirstChargeLayer::clickMobileFirst(CCObject* object)
{
    if(GameMainScene::_instance != NULL)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        CCMenuItem* item = (CCMenuItem*)object;
        if(item->getTag() == 101)//分享
        {
            GameMainScene::_instance->sharedMobileMoney(object);
        }
        else if(item->getTag() == 102)//领取
        {
            GameMainScene::_instance->removeSharedUI(object);
        }
    }
}

void FirstChargeLayer::loadDuiHuanUI()
{
    m_spDuiHuan = CCSprite::create("c2harge/duihuanbg.png");
    m_spDuiHuan -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spDuiHuan);
    
    CCMenuItemImage* closeButton = CCMenuItemImage::create("freecoin/freecoincloseitem1.png","freecoin/freecoincloseitem1.png",this,menu_selector(FirstChargeLayer::closeMenu));
    closeButton->setPosition(ccp(m_spDuiHuan->getContentSize().width-40, m_spDuiHuan->getContentSize().height-40));
    CCMenu* pMenuClose = CCMenu::create(closeButton, NULL);
    pMenuClose->setPosition(CCPointZero);
    pMenuClose->setAnchorPoint(CCPointZero);
    m_spDuiHuan->addChild(pMenuClose, 2);
    closeButton -> setScale(1.3f);
    pMenuClose->setTouchPriority(-202);
    
    CCMenuItemImage* pChargeItem = CCMenuItemImage::create("c2harge/duihuanitem.png", "c2harge/duihuanitem.png", this, menu_selector(FirstChargeLayer::sendDuiHuan));
    pChargeItem->setPosition(ccp(m_spDuiHuan->getContentSize().width/2, 98));
    CCMenu* pClickMenu = CCMenu::create(pChargeItem, NULL);
    pClickMenu->setPosition(CCPointZero);
    pClickMenu->setAnchorPoint(CCPointZero);
    m_spDuiHuan->addChild(pClickMenu, 5);
    pClickMenu->setTouchPriority(-202);
    
    m_pEditBox = CCEditBox::create(CCSize(469,69), CCScale9Sprite::create("c2harge/duihuankuang.png"));
    m_pEditBox->setMaxLength(50);//限制最长字符
    m_pEditBox->setReturnType(kKeyboardReturnTypeDone);
    m_pEditBox->setInputFlag(kEditBoxInputFlagSensitive);
    m_pEditBox->setInputMode(kEditBoxInputModeEmailAddr);
    m_pEditBox->setPosition(ccp(m_spDuiHuan->getContentSize().width/2, m_spDuiHuan->getContentSize().height/2+25));
    m_pEditBox->setPlaceHolder("请在此输入兑换码");
    m_pEditBox->setPlaceholderFontSize(26);
    m_spDuiHuan->addChild(m_pEditBox, 10);
    m_pEditBox -> setTouchPriority(-202);
    
    m_spDuiHuan->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void FirstChargeLayer::loadLabaUI()
{
    m_spLaba = CCSprite::create("c2harge/lababg.png");
    m_spLaba -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spLaba);
    m_spLabaInfo = CCLabelTTF::create("10万金币可发送一条全服广播","Airal", 32);
    m_spLabaInfo -> setPosition(ccp(m_spLaba->getContentSize().width/2, 180));
    m_spLabaInfo -> setColor(ccc3(255, 0, 0));
    m_spLaba -> addChild(m_spLabaInfo);
    m_spLabaInfo -> setVisible(false);
    
    CCMenuItemImage* closeButton = CCMenuItemImage::create("freecoin/freecoincloseitem1.png","freecoin/freecoincloseitem1.png",this,menu_selector(FirstChargeLayer::closeMenu));
    closeButton->setPosition(ccp(m_spLaba->getContentSize().width-40, m_spLaba->getContentSize().height-40));
    CCMenu* pMenuClose = CCMenu::create(closeButton, NULL);
    pMenuClose->setPosition(CCPointZero);
    pMenuClose->setAnchorPoint(CCPointZero);
    m_spLaba->addChild(pMenuClose, 2);
    closeButton -> setScale(1.3f);
    pMenuClose->setTouchPriority(-202);
    
    CCMenuItemImage* pChargeItem = CCMenuItemImage::create("c2harge/labasend.png", "c2harge/labasend.png", this, menu_selector(FirstChargeLayer::sendLaba));
    pChargeItem->setPosition(ccp(m_spLaba->getContentSize().width/2, 98));
    m_labaMenu = CCMenu::create(pChargeItem, NULL);
    m_labaMenu->setPosition(CCPointZero);
    m_labaMenu->setAnchorPoint(CCPointZero);
    m_spLaba->addChild(m_labaMenu, 5);
    m_labaMenu->setTouchPriority(-202);
    m_labaMenu -> setVisible(false);
    
    m_pLabaEditBox = CCEditBox::create(CCSize(680,69), CCScale9Sprite::create("c2harge/duihuankuang.png"));
    m_pLabaEditBox->setMaxLength(25);//限制最长字符
    m_pLabaEditBox->setReturnType(kKeyboardReturnTypeDone);
    m_pLabaEditBox->setInputFlag(kEditBoxInputFlagSensitive);
    m_pLabaEditBox->setInputMode(kEditBoxInputModeEmailAddr);
    m_pLabaEditBox->setPosition(ccp(m_spLaba->getContentSize().width/2, m_spLaba->getContentSize().height/2+25));
    m_pLabaEditBox->setPlaceHolder("请输入内容（禁发表情符）");
    m_pLabaEditBox->setPlaceholderFontSize(18);
    m_spLaba->addChild(m_pLabaEditBox, 10);
    m_pLabaEditBox -> setTouchPriority(-202);
    m_pLabaEditBox->setDelegate(this);
    
    m_spLaba->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void FirstChargeLayer::setLabaLabel()
{
    m_pLabaEditBox -> setText("");
    if(m_nLabaTimes > 0)
    {
        m_nLabaTimes--;
    }
    if(m_nLabaTimes == 0)
    {
        m_labaMenu->setVisible(true);
        m_spLabaInfo -> setVisible(true);
    }
    else
    {
        m_labaMenu->setVisible(false);
        m_spLabaInfo -> setVisible(false);
    }
}

void FirstChargeLayer::setLabaLastTime(int nLabaTimes, int nLastCoin)
{
    m_nLabaTimes = nLabaTimes;
    if(nLastCoin >= 0)
    {
        char temp[64];
        if(nLastCoin < 10000)
        {
            sprintf(temp, "%d金币可发送一条全服广播", nLastCoin);
        }
        else
        {
            sprintf(temp, "%d万金币可发送一条全服广播", nLastCoin/10000);
        }
        m_spLabaInfo -> setString(temp);
    }
    
    if(nLabaTimes > 0)
    {
        m_labaMenu->setVisible(false);
        m_spLabaInfo -> setVisible(false);
    }
    else
    {
        m_labaMenu->setVisible(true);
        m_spLabaInfo -> setVisible(true);
    }
}

bool FirstChargeLayer::efficacyNickName(const char* str)
{
    bool bHageIllegal = false;
    int nCharLength = (int)strlen(str);
    
    //int nDataLength = nCharLength*sizeof(TCHAR);
    for(int i = 0; i < nCharLength; i++)
    {
        TCHAR sTemp = str[i];
        //if((sTemp>='!' && sTemp<'0') || (sTemp>'9' && sTemp<'A')|| (sTemp>'Z' && sTemp<'a')|| (sTemp>'z' && sTemp<='~'))
        if((sTemp>'Z' && sTemp<'a')|| (sTemp>'z' && sTemp<='~'))
        {
            bHageIllegal = true;
        }
    }
    if(bHageIllegal)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("您输入的内容包含非法字符，请重新输入!", eMBOK);
        return false;
    }
    //长度判断
    if(nCharLength < LEN_LESS_NICKNAME || nCharLength > LEN_MORE_NICKNAME)
    {
        //变量定义
        WORD nChinessCount=0;
        
        //计算字符
        for (int i=0;i<nCharLength;i++)
        {
            if (str[i]>=0x80)
            {
                nChinessCount++;
            }
        }
    }
    
    return true;
}

void FirstChargeLayer::sendLaba(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    std::string sLaba = m_pLabaEditBox->getText();
    if(sLaba == "")
    {
        CMainLogic::sharedMainLogic()->ShowMessage("请您写下想要发送的内容。", eMBOK);
        return ;
    }
    if(sLaba.length()>56)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，您想要发送的内容有点长，请删减后再试一次。", eMBOK);
        return;
    }
    if(!efficacyNickName(sLaba.c_str()))
    {
        return;
    }
    char temp[64];
    if(CMainLogic::sharedMainLogic()->m_nVipLevel >= 1)
    {
        sprintf(temp, "VIP%d%s", CMainLogic::sharedMainLogic()->m_nVipLevel, CMainLogic::sharedMainLogic()->m_sNickName.c_str());
    }
    else
    {
        sprintf(temp, "%s", CMainLogic::sharedMainLogic()->m_sNickName.c_str());
    }
    if(GameMainScene::_instance != NULL)
    {
        CMD_GR_BroadLaba BroadLaba = {0};
        BroadLaba.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
        strncpy(BroadLaba.szContent,CCharCode::UTF8ToGB2312(sLaba.c_str()).c_str(),sLaba.length());
        strncpy(BroadLaba.szNickName,temp,sizeof(BroadLaba.szNickName));
        BroadLaba.wSize = sLaba.length()+1;
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_USER, SUB_GR_USE_LABA, &BroadLaba,sizeof(BroadLaba));
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameUserLaba);
    }
    else
    {
        CMD_GPR_BroadLaba BroadLaba = {0};
        BroadLaba.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
        strncpy(BroadLaba.szContent,CCharCode::UTF8ToGB2312(sLaba.c_str()).c_str(),sLaba.length());
        strncpy(BroadLaba.szNickName,temp,sizeof(BroadLaba.szNickName));
        BroadLaba.wSize = sLaba.length()+1;
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_BROAD_LABA, &BroadLaba,sizeof(BroadLaba));
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallSendLaba);
    }
    CMainLogic::sharedMainLogic()->ShowMessage("正在发送，请稍候", eMBExitGame);
    
}

void FirstChargeLayer::sendDuiHuan(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    std::string str = m_pEditBox->getText();
    if(str == "")
    {
        CMainLogic::sharedMainLogic()->ShowMessage("请在输入框中输入您的兑换码，谢谢。", eMBOK);
        return ;
    }
    if(str.length()>50)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，您想要发送的内容有点长，请删减后再试一次。", eMBOK);
        return;
    }
    //发送兑换码
    int nUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    char temp[128];
    sprintf(temp, "%d%swxdhm",nUserID, str.c_str());
    char szMD5Result[128];
    CEncryptMD5::toString32(temp,szMD5Result);
    char urlStr[256];
    sprintf(urlStr, "lelewx.66y.com/wxcodekey.aspx?userid=%d&code=%s&sign=%s", nUserID, str.c_str(), szMD5Result);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(urlStr);
    request -> setRequestType(CCHttpRequest::kHttpPost);
    request -> setRequestData(szMD5Result, (int)strlen(szMD5Result));
    request -> setResponseCallback(this, httpresponse_selector(FirstChargeLayer::onHttpDuihuanComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
    CMainLogic::sharedMainLogic()->ShowMessage("正在发送验证码，请稍候", eMBNull);
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallSendDuihuanma);
}

void FirstChargeLayer::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
    std::string str = CCharCode::UTF8ToGB2312(text.c_str());
}


void FirstChargeLayer::onHttpDuihuanComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    std::string str2 = "";
    if(str != "")
    {
        str2 = str.substr(0, 1);
        str = str.substr(1);
    }
    if(str2 == "0")//兑换成功，点击确定关闭兑换框， 同时去请求邮件
    {
        //CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK, eCloseDuihuan);
        CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK);
        CMainLogic::sharedMainLogic()->requestMail();
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallSendDuihuanSuccess);
        this -> removeFromParent();
    }
    else//兑换失败
    {
        CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK);
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallSendDuihuanFail);
    }
}

void FirstChargeLayer::loadUI()
{
    m_nodeFirstCharge = CCSprite::create("c2harge/firstchargebg.png");
    m_nodeFirstCharge -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_nodeFirstCharge);
    
    CCMenuItemImage* closeButton = CCMenuItemImage::create("h2all/dakacloseitem.png","h2all/dakacloseitem.png",this,menu_selector(FirstChargeLayer::closeMenu));
    closeButton->setPosition(ccp(m_nodeFirstCharge->getContentSize().width-10, m_nodeFirstCharge->getContentSize().height-50));
    CCMenu* pMenuClose = CCMenu::create(closeButton, NULL);
    pMenuClose->setPosition(CCPointZero);
    closeButton->setScale(1.3f);
    pMenuClose->setAnchorPoint(CCPointZero);
    m_nodeFirstCharge->addChild(pMenuClose, 2);
    pMenuClose->setTouchPriority(-202);
    
    //获取按钮
    CCMenuItemImage* getItem = CCMenuItemImage::create("c2harge/oneitem.png", "c2harge/oneitem.png", this, menu_selector(FirstChargeLayer::clickMenu));
    getItem -> setTag(111);
    getItem -> setPosition(ccp(m_nodeFirstCharge->getContentSize().width/4*3+115, 80));
    CCMenu* pChargeMenu = CCMenu::create(getItem, NULL);
    pChargeMenu -> setPosition(CCPointZero);
    pChargeMenu -> setAnchorPoint(CCPointZero);
    m_nodeFirstCharge -> addChild(pChargeMenu);
    pChargeMenu -> setTouchPriority(-202);
    
    //alipay wechat
//    CCMenuItemImage* alipayItem = CCMenuItemImage::create("c2harge/alipayItem.png", "c2harge/alipayItem.png", this, menu_selector(FirstChargeLayer::clickMenu));
//    alipayItem -> setTag(108);
//    alipayItem -> setPosition(ccp(m_nodeFirstCharge->getContentSize().width/4*3+80, 80));
//    CCMenuItemImage* wechatItem = CCMenuItemImage::create("c2harge/wechatItem.png", "c2harge/wechatItem.png", this, menu_selector(FirstChargeLayer::clickMenu));
//    wechatItem -> setTag(109);
//    wechatItem -> setPosition(ccp(m_nodeFirstCharge->getContentSize().width/2+70, 80));
//    CCMenu* pChargeMenu = CCMenu::create(alipayItem, wechatItem, NULL);
//    pChargeMenu -> setPosition(CCPointZero);
//    pChargeMenu -> setAnchorPoint(CCPointZero);
//    m_nodeFirstCharge -> addChild(pChargeMenu);
//    pChargeMenu -> setTouchPriority(-202);
    
    m_nodeFirstCharge->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void FirstChargeLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    if(item->getTag() == 108)//支付宝
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        //打开ChargeLayer类（设置为隐藏），并且下订单（支付宝首充10元）
        if(GameMainScene::_instance == NULL)
        {
            CMainLogic::sharedMainLogic()->m_bFirstChargeUIStatus = true;
            CMainLogic::sharedMainLogic()->alipayCharge(10);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallFirstChargeAlipay);
        }
        else
        {
            ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
            if(pChargeLayer != NULL)
            {
                CMainLogic::sharedMainLogic()->m_bClickMessageCharge = false;
                pChargeLayer -> m_nOpenType = 10;
                pChargeLayer -> setCurrentSelectedType(100);
                pChargeLayer -> selectedChargeTypeAndPrise(100, 0);
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameFirstChargeAlipay);
            }
        }
        
        this -> removeFromParent();
    }
    else if(item->getTag() == 109)//微信
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        //打开ChargeLayer类（设置为隐藏），并且下订单（微信首充10元）
        if(GameMainScene::_instance == NULL)
        {
            CMainLogic::sharedMainLogic()->m_bFirstChargeUIStatus = true;
            CMainLogic::sharedMainLogic()->wxCharge(10);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallFirstChargeWechat);
        }
        else
        {
            ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
            if(pChargeLayer != NULL)
            {
                CMainLogic::sharedMainLogic()->m_bClickMessageCharge = false;
                pChargeLayer -> m_nOpenType = 10;
                pChargeLayer -> setCurrentSelectedType(101);
                pChargeLayer -> selectedChargeTypeAndPrise(101, 0);
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameFirstChargeWechat);
            }
        }
        this -> removeFromParent();
    }
    else if(item->getTag() == 110)//拍拍乐进入游戏
    {
        if(GameMainScene::_instance != NULL)
        {
            this -> unschedule(schedule_selector(FirstChargeLayer::showPPLTime));
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, ePPLZhunbei);
            this -> removeFromParent();
            GameMainScene::_instance -> setPaiItemEnable(true);
            SimpleAudioEngine::sharedEngine()->playEffect("Music/paistartgame.wav");
        }
    }
    else if(item->getTag() == 111)//10元充值获取按钮
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eTenGetTag);
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        ChargeUI* pChargeUI = ChargeUI::create(10, 2);//第1个参数表示充值金额，第二个参数表示10元限购
        CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeUI, 13, 20003);
    }
}

void FirstChargeLayer::closeMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(FirstChargeLayer::removeWnd));
    if(m_nOpenType == 0)
    {
        CMainLogic::sharedMainLogic()->m_bFirstChargeUIStatus = false;
        m_nodeFirstCharge->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallFirstChargeClose);
    }
    else if(m_nOpenType == 1)
    {
        CCMenuItem* item = (CCMenuItem*)object;
        if(item -> getTag() == 101)
        {
            CMainLogic::sharedMainLogic()->m_bEnterService = false;
            m_nodeService->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
        }
        else if(item->getTag() == 102)
        {
            CMainLogic::sharedMainLogic()->m_bEnterService = true;
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallOnlineService);
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            char tempUserID[16];
            sprintf(tempUserID, "%d", CMainLogic::sharedMainLogic()->m_nGameID);
            FMLayerWebView* pFMLayerWebView = FMLayerWebView::create(tempUserID,CMainLogic::sharedMainLogic()->m_sAccount.c_str());
            pFMLayerWebView -> setPosition(CCPointZero);
            this -> addChild(pFMLayerWebView, 300, 10001);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            char chargeURL[256];
            sprintf(chargeURL, "http://%s/Feedback.aspx?gameid=%d&accname=%s", NORMAL_URL,CMainLogic::sharedMainLogic()->m_nGameID, CMainLogic::sharedMainLogic()->m_sAccount.c_str());
            JniMethodInfo minfo;
            jobject jobj;
            bool isHave = JniHelper::getStaticMethodInfo(minfo, "cn/sharesdk/hellocpp/HelloCpp","getInstance","()Ljava/lang/Object;");
            if (isHave)
            {
                //调用Java静态函数，取得对象。
                jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
                if (jobj != NULL)
                {
                    isHave = JniHelper::getMethodInfo(minfo,"cn/sharesdk/hellocpp/HelloCpp","openWebView","(Ljava/lang/String;)V");
                    if (isHave)
                    {
                        jstring urlStr = minfo.env->NewStringUTF(chargeURL);
                        minfo.env->CallVoidMethod(jobj, minfo.methodID, urlStr);
                    }  
                }  
            }
#endif
        }
    }
    else if(m_nOpenType == 2)
    {
        m_spDuiHuan -> runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
    }
    else if(m_nOpenType == 3)
    {
        m_spLaba -> runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
    }
    else if(m_nOpenType == 4)
    {
        removeWnd();
    }
    else if(m_nOpenType == 7 || m_nOpenType == 8)
    {
        this -> removeFromParent();
    }
}

void FirstChargeLayer::removeWnd()
{
    this->removeFromParent();
}

void FirstChargeLayer::loadServiceUI()
{
    m_nodeService = CCSprite::create();
    this->addChild(m_nodeService);
    m_nodeService->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    m_pServiceDi = CCSprite::create("freecoin/servicebg.png");
    m_pServiceDi->setPosition(ccp(0, 0));
    m_nodeService->addChild(m_pServiceDi);
    
    CCMenuItemImage* closeButton = CCMenuItemImage::create("freecoin/serviceItem1.png","freecoin/serviceItem1.png",this,menu_selector(FirstChargeLayer::closeMenu));
    closeButton->setTag(102);
    closeButton->setPosition(ccp(m_pServiceDi->getContentSize().width/2-6, 110));
    CCMenuItemImage * closeItem = CCMenuItemImage::create("Dragon/dragoncloseitem.png","Dragon/dragoncloseitem.png",this,menu_selector(FirstChargeLayer::closeMenu));
    closeItem->setTag(101);
    closeItem->setScale(1.2f);
    closeItem->setPosition(ccp(m_pServiceDi->getContentSize().width-40, m_pServiceDi->getContentSize().height-40));
    CCMenu* pMenuClose = CCMenu::create(closeButton, closeItem, NULL);
    pMenuClose->setPosition(CCPointZero);
    pMenuClose->setAnchorPoint(CCPointZero);
    m_pServiceDi->addChild(pMenuClose, 2);
    pMenuClose->setTouchPriority(-202);
    
    char temp[16];
    sprintf(temp, "%s", CMainLogic::sharedMainLogic()->m_sServiceQQ.c_str());
    CCLabelAtlas* pLabelInfo2 = CCLabelAtlas::create(temp, "freecoin/serviceQQ.png", 37, 67, '0');
    pLabelInfo2->setPosition(ccp(m_pServiceDi->getContentSize().width/2-13, m_pServiceDi->getContentSize().height/3+23));
    m_pServiceDi->addChild(pLabelInfo2);
    
    m_nodeService->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void FirstChargeLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -201, true);
}

bool FirstChargeLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if(m_nOpenType == 7)
    {
        CCPoint point = pTouch->getLocation();
        if(point.y <= 200 || point.y >= 520 || point.x <= 150 || point.x >= 1130)
        {
            this -> removeFromParent();
            if(GameMainScene::_instance != NULL)
            {
                GameMainScene::GetInstance()->setWndHave(false);
                GameMainScene::GetInstance()->setAutoFire();
                GameMainScene::GetInstance()->setMatchShowStatus(true);
                GameMainScene::GetInstance()->setClickDantouMenu(false);
            }
        }
    }
    return true;
}
