//
//  FreeCoinLayer.cpp
//  CatchFish
//
//  Created by xiaosong1531 on 15/10/8.
//
//

#include "header.h"

#if defined(IOS_ANDROID)

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
extern int WeiXinCharge;
#endif

#endif

FreeCoinLayer::~FreeCoinLayer()
{
}

FreeCoinLayer* FreeCoinLayer::create(int nOpenType)
{
    FreeCoinLayer* pRet = new FreeCoinLayer();
    if(pRet && pRet->init(nOpenType))
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

bool FreeCoinLayer::init(int nOpenType)
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_nOpenType = nOpenType;
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    m_bReceiveGive = false;
    m_bCanGet = false;
    m_nTotal = CMainLogic::sharedMainLogic()->m_nGiveTotal;
    m_nLeft = CMainLogic::sharedMainLogic()->m_nGiveTotal-CMainLogic::sharedMainLogic()->m_nBankruptcyCount;
    m_nGold = 0;
    m_llCurrentScore = 0;
    m_nConditionScore = 0;
    m_sFailInfo = "";
    m_nDaojishi = 60;
    loadUI();
    loadUIMenu();
    loadUIGave();
    loadUILogon();
    loadBangDingUI();
    if(m_nOpenType == 0)
    {
        sendGiveInfo();
    }
    
    this -> setTouchEnabled(true);
    
    return true;
}

void FreeCoinLayer::loadUI()
{
    m_nodeFreeCoin = CCSprite::create();
    m_nodeFreeCoin->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_nodeFreeCoin);
    
    m_spFreeCoinBg = CCSprite::create("freecoin/freecoinbg.png");
    m_spFreeCoinBg -> setPosition(ccp(0, 0));
    m_nodeFreeCoin->addChild(m_spFreeCoinBg);
    
    CCMenuItemImage* closeButton = CCMenuItemImage::create("freecoin/freecoincloseitem1.png","freecoin/freecoincloseitem1.png",this,menu_selector(FreeCoinLayer::clickMenu));
    closeButton->setTag(104);
    closeButton->setPosition(ccp(m_spFreeCoinBg->getContentSize().width-20, m_spFreeCoinBg->getContentSize().height-100));
    closeButton -> setScale(1.2f);
    m_menuClose = CCMenu::create(closeButton, NULL);
    m_menuClose->setPosition(CCPointZero);
    m_menuClose->setAnchorPoint(CCPointZero);
    m_spFreeCoinBg->addChild(m_menuClose, 2);
    m_menuClose->setTouchPriority(-140);
    
    m_nodeFreeCoin->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void FreeCoinLayer::loadUIMenu()
{
    CCMenuItemImage* pLogonItem = CCMenuItemImage::create("freecoin/freecoinlogonitem.png", "freecoin/freecoinlogonitem.png", this, menu_selector(FreeCoinLayer::clickMenu));
    pLogonItem->setTag(101);
    pLogonItem->setPosition(ccp(m_spFreeCoinBg->getContentSize().width/5*3+16-9, m_spFreeCoinBg->getContentSize().height/2-100));
    CCMenuItemImage* pGaveItem = CCMenuItemImage::create("freecoin/freecoingiveitem.png", "freecoin/freecoingiveitem.png", this, menu_selector(FreeCoinLayer::clickMenu));
    pGaveItem->setTag(102);
    pGaveItem->setPosition(ccp(m_spFreeCoinBg->getContentSize().width/5-30+3, m_spFreeCoinBg->getContentSize().height/2-100));
    CCMenuItemImage* pShareItem = CCMenuItemImage::create("freecoin/freecoinshareitem.png", "freecoin/freecoinshareitem.png", this, menu_selector(FreeCoinLayer::clickMenu));
    pShareItem->setTag(107);
    pShareItem->setPosition(ccp(m_spFreeCoinBg->getContentSize().width/5*2-7-4, m_spFreeCoinBg->getContentSize().height/2-100));
    CCMenuItemImage* pBandingItem = CCMenuItemImage::create("freecoin/freecoinbandingitem.png", "freecoin/freecoinbandingitem.png", this, menu_selector(FreeCoinLayer::clickMenu));
    pBandingItem -> setTag(108);
    pBandingItem -> setPosition(ccp(m_spFreeCoinBg->getContentSize().width/5*4+39-12, m_spFreeCoinBg->getContentSize().height/2-100));
    m_menuFree = CCMenu::create(pLogonItem, pGaveItem, pShareItem, pBandingItem, NULL);
    m_menuFree->setPosition(CCPointZero);
    m_menuFree->setAnchorPoint(CCPointZero);
    m_spFreeCoinBg->addChild(m_menuFree, 4);
    m_menuFree -> setTouchPriority(-140);
    CCLabelTTF* pSpLogonInfo = CCLabelTTF::create("还未签到", "Arial", 23);
    pSpLogonInfo->setColor(ccc3(207, 19, 1));
    pSpLogonInfo->setPosition(ccp(pLogonItem->getContentSize().width/2-30, 220));
    pLogonItem->addChild(pSpLogonInfo, 4, 101);
    char tempLast[32];
    sprintf(tempLast, "今天仅剩%d次", m_nTotal-CMainLogic::sharedMainLogic()->m_nBankruptcyCount);
    m_lGaveTimes = CCLabelTTF::create(tempLast, "Arial", 23);
    m_lGaveTimes->setColor(ccc3(207, 19, 1));
    m_lGaveTimes -> setPosition(ccp(pGaveItem->getContentSize().width/2-30, 222));
    pGaveItem->addChild(m_lGaveTimes, 4);
    char sShareGift[16];
    sprintf(sShareGift, "%d金币", CMainLogic::sharedMainLogic()->m_nShareReward);
    m_lShareTimes = CCLabelTTF::create(sShareGift, "Arial", 23);
    m_lShareTimes->setColor(ccc3(207, 19, 1));
    m_lShareTimes -> setPosition(ccp(pShareItem->getContentSize().width/2-35, 222));
    pShareItem -> addChild(m_lShareTimes, 4);
    char sBandingContent[16];
    sprintf(sBandingContent, "%d金币",CMainLogic::sharedMainLogic()->m_nBindReward);
    CCLabelTTF* pLabelBanding1 = CCLabelTTF::create(sBandingContent, "Arial", 23);
    pLabelBanding1->setColor(ccc3(207, 19, 1));
    pLabelBanding1 -> setPosition(ccp(pBandingItem->getContentSize().width/2-35, 222));
    pBandingItem -> addChild(pLabelBanding1, 4, 101);
   
    CCLabelTTF* pLabelBanding2 = CCLabelTTF::create("已绑定", "Arial", 23);
    pLabelBanding2 -> setColor(ccc3(207, 19, 1));
    pLabelBanding2 -> setPosition(ccp(pBandingItem->getContentSize().width/2-35, 222));
    pBandingItem -> addChild(pLabelBanding2, 4, 102);
    if(CMainLogic::sharedMainLogic()->m_bCanBandingTel)
    {
        pLabelBanding2->setVisible(false);
        pBandingItem->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 0.9f), CCScaleTo::create(0.5f, 1.0f), NULL)));
    }
    else
    {
        pLabelBanding1->setVisible(false);
    }
    
    //动画效果
    if(CMainLogic::sharedMainLogic()->m_bCanShare)
    {
        pShareItem->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 0.9f), CCScaleTo::create(0.5f, 1.0f), NULL)));
    }
    if(CMainLogic::sharedMainLogic()->m_nLogonGift != 0 || !CMainLogic::sharedMainLogic()->m_bReceiveVipLogon)
    {
        pLogonItem->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 0.9f), CCScaleTo::create(0.5f, 1.0f), NULL)));
    }
    else
    {
        pSpLogonInfo->setString("已签到");
    }
    if(CMainLogic::sharedMainLogic()->m_cbCanGetBankruptcy > 0)
    {
        pGaveItem->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 0.9f), CCScaleTo::create(0.5f, 1.0f), NULL)));
    }
    if(CMainLogic::sharedMainLogic()->m_lUserScore >= CMainLogic::sharedMainLogic()->m_nGiveScore)
    {
        CMainLogic::sharedMainLogic()->m_cbCanGetBankruptcy = 0;
        pGaveItem->stopAllActions();
        pGaveItem->setScale(1.0f);
    }
    if(CMainLogic::sharedMainLogic()->m_nGiveScore > CMainLogic::sharedMainLogic()->m_lUserScore && CMainLogic::sharedMainLogic()->m_nBankruptcyCount < CMainLogic::sharedMainLogic()->m_nGiveTotal)//分数满足条件，并且剩余次数也满足条件
    {
        pGaveItem->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 0.9f), CCScaleTo::create(0.5f, 1.0f), NULL)));
    }
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        pShareItem->setVisible(false);
        pLogonItem->setVisible(false);
        //pLogonItem->setPosition(ccp(m_spFreeCoinBg->getContentSize().width/4-20, m_spFreeCoinBg->getContentSize().height/2-100));
//        pGaveItem->setPosition(ccp(m_spFreeCoinBg->getContentSize().width/2, m_spFreeCoinBg->getContentSize().height/2-100));
//        pBandingItem->setPosition(ccp(m_spFreeCoinBg->getContentSize().width/4*3+20, m_spFreeCoinBg->getContentSize().height/2-100));
        pGaveItem->setPosition(ccp(m_spFreeCoinBg->getContentSize().width/3, m_spFreeCoinBg->getContentSize().height/2-100));
        pBandingItem->setPosition(ccp(m_spFreeCoinBg->getContentSize().width/3*2, m_spFreeCoinBg->getContentSize().height/2-100));
    }
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
    pShareItem->setVisible(false);
    pLogonItem->setPosition(ccp(m_spFreeCoinBg->getContentSize().width/4-20, m_spFreeCoinBg->getContentSize().height/2-100));
    pGaveItem->setPosition(ccp(m_spFreeCoinBg->getContentSize().width/2, m_spFreeCoinBg->getContentSize().height/2-100));
    pBandingItem->setPosition(ccp(m_spFreeCoinBg->getContentSize().width/4*3+20, m_spFreeCoinBg->getContentSize().height/2-100));
#endif
}

void FreeCoinLayer::loadUIGave()
{
    m_spGave = CCSprite::create("freecoin/freecoinbg.png");
    m_spGave -> setPosition(ccp(0, 0));
    m_nodeFreeCoin->addChild(m_spGave, 2);
    m_spGave -> setVisible(false);
   
    CCMenuItemImage* closeButton = CCMenuItemImage::create("freecoin/freecoincloseitem1.png","freecoin/freecoincloseitem1.png",this,menu_selector(FreeCoinLayer::clickMenu));
    closeButton->setTag(105);
    closeButton->setPosition(ccp(m_spGave->getContentSize().width-20, m_spGave->getContentSize().height-90));
    CCMenu* pMenuClose = CCMenu::create(closeButton, NULL);
    pMenuClose->setPosition(CCPointZero);
    pMenuClose->setAnchorPoint(CCPointZero);
    m_spGave->addChild(pMenuClose, 2);
    pMenuClose->setTouchPriority(-140);
    
//    m_nodeFreeCoin->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
    
    CCSprite* pSpGave = CCSprite::create("freecoin/freecoingiveitem.png");
    pSpGave->setPosition(ccp(m_spGave->getContentSize().width/4-60, m_spGave->getContentSize().height/2-95));
    pSpGave -> setScale(1.0f);
    m_spGave->addChild(pSpGave, 4);
    CCSprite* pSpGaveInfo = CCSprite::create("freecoin/freecoingaveinfo2.png");
    pSpGaveInfo->setPosition(ccp(m_spGave->getContentSize().width/3*2, m_spGave->getContentSize().height/3*2-170));
    m_spGave->addChild(pSpGaveInfo);
    CCMenuItemImage* getItem = CCMenuItemImage::create("freecoin/freecoingetItem1.png", "freecoin/freecoingetItem1.png", this, menu_selector(FreeCoinLayer::clickMenu));
    getItem->setPosition(ccp(m_spGave->getContentSize().width/2+150, 115));
    getItem->setTag(106);
    m_menuReceive1 = CCMenu::create(getItem, NULL);
    m_menuReceive1 -> setPosition(CCPointZero);
    m_menuReceive1 -> setAnchorPoint(CCPointZero);
    m_spGave -> addChild(m_menuReceive1, 4);
    m_menuReceive1 -> setTouchPriority(-140);
    
    CCMenuItemImage * receiveItem = CCMenuItemImage::create("freecoin/alreceive.png", "freecoin/alreceive.png");
    m_menuReceive2 = CCMenu::create(receiveItem,NULL);
    m_menuReceive2->setPosition(ccp(m_spGave->getContentSize().width/2+150, 115));
    m_spGave->addChild(m_menuReceive2);
    m_menuReceive2 -> setVisible(false);
    m_menuReceive2 -> setTouchEnabled(false);
    
    m_lGaveCoin = CCLabelTTF::create("10000金币", "Arial", 36);
    m_lGaveCoin -> setColor(ccc3(255, 22, 0));
    m_lGaveCoin -> setPosition(ccp(pSpGaveInfo->getContentSize().width/2+30, pSpGaveInfo->getContentSize().height/2));
    pSpGaveInfo->addChild(m_lGaveCoin);
    m_lGaveLastTimes = CCLabelTTF::create("", "Arial", 23);
    m_lGaveLastTimes->setPosition(ccp(pSpGave->getContentSize().width/2-30, 220));
    m_lGaveLastTimes->setColor(ccc3(207, 19, 1));
    pSpGave->addChild(m_lGaveLastTimes);
    
}

void FreeCoinLayer::loadUILogon()
{
    
}

void FreeCoinLayer::loadBangDingUI()
{
    //手机号界面
    m_spBangingBg = CCSprite::create("h3all/bangdingbg.png");
    m_spBangingBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(m_spBangingBg, 2);
    m_spBangingBg -> setVisible(false);
    m_labelDaojishi = CCLabelTTF::create("60秒", "Arial", 36);
    m_labelDaojishi -> setPosition(ccp(m_spBangingBg->getContentSize().width-90, m_spBangingBg->getContentSize().height/3*2));
    m_spBangingBg -> addChild(m_labelDaojishi);
    m_labelDaojishi -> setVisible(false);
    CCSprite* pBandingTitle = CCSprite::create("h3all/bangdingtitle.png");
    pBandingTitle -> setPosition(ccp(m_spBangingBg->getContentSize().width/2, m_spBangingBg->getContentSize().height-pBandingTitle->getContentSize().height-20));
    m_spBangingBg -> addChild(pBandingTitle);
    CCSprite* pBandingTel = CCSprite::create("h3all/bangdingtel.png");
    pBandingTel -> setPosition(ccp(pBandingTel->getContentSize().width/2+120, m_spBangingBg->getContentSize().height/3*2));
    m_spBangingBg -> addChild(pBandingTel);
    CCSprite* pBandingCode = CCSprite::create("h3all/bangdingcode.png");
    pBandingCode -> setPosition(ccp(pBandingCode->getContentSize().width/2+120, m_spBangingBg->getContentSize().height/3));
    m_spBangingBg -> addChild(pBandingCode);
    m_sendCodeItem = CCMenuItemImage::create("h3all/sendcodeitem.png", "h3all/sendcodeitem.png", this, menu_selector(FreeCoinLayer::clickMenu));
    m_sendCodeItem -> setTag(109);
    m_sendCodeItem -> setPosition(ccp(m_spBangingBg->getContentSize().width/2, m_spBangingBg->getContentSize().height/2));
    CCMenuItemImage* nextItem = CCMenuItemImage::create("h3all/nextitem.png", "h3all/nextitem.png", this, menu_selector(FreeCoinLayer::clickMenu));
    nextItem -> setTag(110);
    nextItem -> setPosition(ccp(m_spBangingBg->getContentSize().width/2, 100));
    CCMenu* pSendCodeMenu = CCMenu::create(m_sendCodeItem, nextItem, NULL);
    pSendCodeMenu -> setPosition(CCPointZero);
    pSendCodeMenu -> setAnchorPoint(CCPointZero);
    m_spBangingBg -> addChild(pSendCodeMenu);
    pSendCodeMenu -> setTouchPriority(-140);
    m_spDisable = CCSprite::create("h3all/bandingDisable.png");
    m_spDisable -> setPosition(ccp(m_spBangingBg->getContentSize().width/2, m_spBangingBg->getContentSize().height/2));
    m_spBangingBg -> addChild(m_spDisable);
    m_spDisable -> setVisible(false);
    m_pTelBox= CCEditBox::create(CCSize(400,60), CCScale9Sprite::create("h3all/bangdingkuang.png"));
    m_pTelBox->setMaxLength(11);//限制最长字符
    m_pTelBox->setReturnType(kKeyboardReturnTypeDone);
    m_pTelBox->setInputFlag(kEditBoxInputFlagSensitive);
    m_pTelBox->setInputMode(kEditBoxInputModePhoneNumber);
    m_pTelBox->setPosition(ccp(m_spBangingBg->getContentSize().width/3*2-50, m_spBangingBg->getContentSize().height/3*2));
    m_pTelBox->setPlaceHolder(" 请输入手机号");
    m_spBangingBg->addChild(m_pTelBox);
    m_pTelBox -> setTouchPriority(-140);
    m_pTelBox->setFontColor(ccc3(0, 0, 0));
    m_pCodeBox= CCEditBox::create(CCSize(400,60), CCScale9Sprite::create("h3all/bangdingkuang.png"));
    m_pCodeBox->setMaxLength(10);//限制最长字符
    m_pCodeBox->setReturnType(kKeyboardReturnTypeDone);
    m_pCodeBox->setInputFlag(kEditBoxInputFlagSensitive);
    m_pCodeBox->setInputMode(kEditBoxInputModePhoneNumber);
    m_pCodeBox->setPosition(ccp(m_spBangingBg->getContentSize().width/3*2-50, m_spBangingBg->getContentSize().height/3));
    m_pCodeBox->setPlaceHolder(" 请输入验证码");
    m_spBangingBg->addChild(m_pCodeBox);
    m_pCodeBox -> setTouchPriority(-140);
    m_pCodeBox -> setFontColor(ccc3(0, 0, 0));
    CCMenuItemImage * closeTelItem = CCMenuItemImage::create("Dragon/dragoncloseitem.png","Dragon/dragoncloseitem.png",this,menu_selector(FreeCoinLayer::clickMenu));
    closeTelItem -> setTag(111);
    CCMenu* pMenuTelClose = CCMenu::create(closeTelItem, NULL);
    pMenuTelClose->setTouchPriority(-140);
    pMenuTelClose->setPosition(ccp(m_spBangingBg->getContentSize().width-30, m_spBangingBg->getContentSize().height-30));
    m_spBangingBg->addChild(pMenuTelClose,2);
    
    //密码界面
    m_spPasswordBg = CCSprite::create("h3all/bangdingbg.png");
    m_spPasswordBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(m_spPasswordBg, 2);
    m_spPasswordBg -> setVisible(false);
    CCSprite* pBandingTitle2 = CCSprite::create("h3all/bangdingtitle.png");
    pBandingTitle2 -> setPosition(ccp(m_spPasswordBg->getContentSize().width/2, m_spPasswordBg->getContentSize().height-pBandingTitle2->getContentSize().height-20));
    m_spPasswordBg -> addChild(pBandingTitle2);
    CCSprite* pNickName = CCSprite::create("h3all/bangdingnickname.png");
    pNickName -> setPosition(ccp(pNickName->getContentSize().width/2+141, m_spPasswordBg->getContentSize().height/3*2+20));
    m_spPasswordBg -> addChild(pNickName);
    CCSprite* pPassword1 = CCSprite::create("h3all/password1.png");
    pPassword1 -> setPosition(ccp(pPassword1->getContentSize().width/2+141, m_spPasswordBg->getContentSize().height/3*2-75));
    m_spPasswordBg -> addChild(pPassword1);
    CCSprite* pPassword2 = CCSprite::create("h3all/password2.png");
    pPassword2 -> setPosition(ccp(pPassword2->getContentSize().width/2+141, m_spPasswordBg->getContentSize().height/3+20));
    m_spPasswordBg -> addChild(pPassword2);
    
    m_pNickNameBox= CCEditBox::create(CCSize(430,60), CCScale9Sprite::create("h3all/bangdingkuang.png"));
    m_pNickNameBox->setMaxLength(16);//限制最长字符
    m_pNickNameBox->setReturnType(kKeyboardReturnTypeDone);
    m_pNickNameBox->setInputFlag(kEditBoxInputFlagSensitive);
    m_pNickNameBox->setInputMode(kEditBoxInputModeEmailAddr);
    m_pNickNameBox->setPosition(ccp(m_spPasswordBg->getContentSize().width/3*2-50, m_spPasswordBg->getContentSize().height/3*2+20));
    m_pNickNameBox->setPlaceHolder(" 请输入昵称");
    m_spPasswordBg->addChild(m_pNickNameBox);
    m_pNickNameBox -> setTouchPriority(-140);
    m_pNickNameBox->setFontColor(ccc3(0, 0, 0));
    
    m_pPassword1Box= CCEditBox::create(CCSize(430,60), CCScale9Sprite::create("h3all/bangdingkuang.png"));
    m_pPassword1Box->setMaxLength(16);//限制最长字符
    m_pPassword1Box->setReturnType(kKeyboardReturnTypeDone);
    m_pPassword1Box->setInputFlag(kEditBoxInputFlagPassword);
    m_pPassword1Box->setInputMode(kEditBoxInputModeEmailAddr);
    m_pPassword1Box->setPosition(ccp(m_spPasswordBg->getContentSize().width/3*2-50, m_spPasswordBg->getContentSize().height/3*2-75));
    m_pPassword1Box->setPlaceHolder(" 请输入密码");
    m_spPasswordBg->addChild(m_pPassword1Box);
    m_pPassword1Box -> setTouchPriority(-140);
    m_pPassword1Box->setFontColor(ccc3(0, 0, 0));
    CCSprite* pXing1 = CCSprite::create("l2ogon/logonxing.png");//星标
    pXing1->setPosition(ccp(m_spPasswordBg->getContentSize().width*0.14f, m_pPassword1Box->getPositionY()));
    m_spPasswordBg->addChild(pXing1);
    
    m_pPassword2Box= CCEditBox::create(CCSize(430,60), CCScale9Sprite::create("h3all/bangdingkuang.png"));
    m_pPassword2Box->setMaxLength(16);//限制最长字符
    m_pPassword2Box->setReturnType(kKeyboardReturnTypeDone);
    m_pPassword2Box->setInputFlag(kEditBoxInputFlagPassword);
    m_pPassword2Box->setInputMode(kEditBoxInputModeEmailAddr);
    m_pPassword2Box->setPosition(ccp(m_spPasswordBg->getContentSize().width/3*2-50, m_spPasswordBg->getContentSize().height/3+20));
    m_pPassword2Box->setPlaceHolder(" 请再次输入密码");
    m_spPasswordBg->addChild(m_pPassword2Box);
    m_pPassword2Box -> setTouchPriority(-140);
    m_pPassword2Box -> setFontColor(ccc3(0, 0, 0));
    CCSprite* pXing2 = CCSprite::create("l2ogon/logonxing.png");//星标
    pXing2->setPosition(ccp(pXing1->getPositionX(), m_pPassword2Box->getPositionY()));
    m_spPasswordBg->addChild(pXing2);
    
    CCMenuItemImage * closePassItem = CCMenuItemImage::create("Dragon/dragoncloseitem.png","Dragon/dragoncloseitem.png",this,menu_selector(FreeCoinLayer::clickMenu));
    closePassItem -> setTag(112);
    CCMenuItemImage* okItem = CCMenuItemImage::create("h3all/quedingitem.png", "h3all/quedingitem.png", this, menu_selector(FreeCoinLayer::clickMenu));
    okItem -> setTag(113);
    okItem -> setPosition(ccp(m_spPasswordBg->getContentSize().width/2, 100));
    CCMenu* pMenuPassClose = CCMenu::create(okItem, closePassItem, NULL);
    pMenuPassClose->setTouchPriority(-140);
    closePassItem->setPosition(ccp(m_spPasswordBg->getContentSize().width-30, m_spPasswordBg->getContentSize().height-30));
    pMenuPassClose -> setPosition(CCPointZero);
    m_spPasswordBg->addChild(pMenuPassClose,2);
}

void FreeCoinLayer::sendGiveInfo()
{
    int nUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    char temp[256];
    sprintf(temp, "http://%s/GetBankruptcy.aspx?uid=%d",NORMAL_URL,nUserID);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setTag("http test");
    request -> setResponseCallback(this, httpresponse_selector(FreeCoinLayer::onHttpGiveInfoComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void FreeCoinLayer::onHttpGiveInfoComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    
    
    //对str进行xml解析，将30条数据保存到m_vctRankTimes中
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("freeGold"))
        {
            delete document;
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL)
        {
            const char* currentValue = pCurrentElement->Value();
            if(!strcmp(currentValue, "bankruptcy"))
            {
                m_bReceiveGive = true;
                const char* value = pCurrentElement->Attribute("canGet");
                m_bCanGet = atoi(value);
                value = pCurrentElement->Attribute("total");
                m_nTotal = atoi(value);
                value = pCurrentElement->Attribute("left");
                m_nLeft = atoi(value);
                CMainLogic::sharedMainLogic()->m_nBankruptcyCount = CMainLogic::sharedMainLogic()->m_nGiveTotal-m_nLeft;
                value = pCurrentElement->Attribute("gold");
                m_nGold = atoi(value);
                value = pCurrentElement->Attribute("currentScore");
                m_llCurrentScore = atoll(value);
                value = pCurrentElement->Attribute("conditionScore");
                m_nConditionScore = atoi(value);
                value = pCurrentElement->Attribute("des");
                m_sFailInfo = value;
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    
    if(m_nTotal != 0)
    {
        char temp[32];
        sprintf(temp, "今天仅剩%d次", m_nLeft);
        m_lGaveTimes->setString(temp);
        m_lGaveLastTimes->setString(temp);
        sprintf(temp, "%d金币", m_nGold);
        m_lGaveCoin -> setString(temp);
    }
    
    delete document;
}

void FreeCoinLayer::openLogonGive()
{
    CMD_GPR_VIPUser VIPUser;
    VIPUser.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_VIP_INFO, &VIPUser,sizeof(VIPUser));
}

void FreeCoinLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case 101:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eBtFreeCoinLogon);
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            m_menuFree->setTouchEnabled(false);
            m_menuClose->setTouchEnabled(false);
            m_spFreeCoinBg->setVisible(false);
            //发送请求签到的消息
            CMainLogic::sharedMainLogic()->m_bFirstLogon = false;
            CMD_GP_QuerySigninInfo SigninInfo;
            SigninInfo.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GP_QUERY_SIGNIN_INFO, &SigninInfo,sizeof(SigninInfo));
            CMainLogic::sharedMainLogic()->ShowMessage("正在查询签到，请稍候", eMBExitGame);
            
            break;
        }
        case 102:
        {
            if(!m_bReceiveGive)//没有收到消息时，不能领取，点击按钮没有反应
            {
                break;
            }
            if(!m_bCanGet)//不能领取，这种情况有一下几种：
                //1、当前分数够2、银行分数够3、当前分数和银行分数之和够4、今日救济金已全部领取
            {
                SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                if(m_nLeft == 0)
                {
                    if(m_nLeft == 0)//今日已经领取完毕
                    {
                        CMainLogic::sharedMainLogic()->ShowMessage(m_sFailInfo.c_str(), eMBOK, eMBFreeCharge);
                    }
                    break;
                }
                else if(m_llCurrentScore >= m_nConditionScore)//身上分数够，不能领取
                {
                    CMainLogic::sharedMainLogic()->ShowMessage(m_sFailInfo.c_str(), eMBOK);
                }
                
                break;
            }
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eBtFreeCoinGive);
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            m_menuFree->setTouchEnabled(false);
            m_menuClose->setTouchEnabled(false);
            m_spGave->setVisible(true);
            m_spGave->runAction(CCSequence::create(CCFadeIn::create(0.01f), CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
            m_spFreeCoinBg->setVisible(false);
            break;
        }
        case 103:
        {
            break;
            
        }
        case 104:
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
            CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(FreeCoinLayer::removeWnd));
            m_nodeFreeCoin->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f),CCScaleTo::create(0.06f, 0.3f), func, NULL));
            break;
        }
        case 105:
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
            if(m_nOpenType == 0)
            {
                m_menuFree->setTouchEnabled(true);
                m_menuClose->setTouchEnabled(true);
                //m_spGave->setVisible(false);
                //m_spFreeCoinBg->setVisible(true);
                CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(FreeCoinLayer::removeWnd2));
                m_spGave->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCSpawn::create(CCScaleTo::create(0.06f, 0.3f), NULL), func, NULL));
            }
            else
            {
                CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(FreeCoinLayer::removeWnd));
                m_spGave->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCSpawn::create(CCScaleTo::create(0.06f, 0.3f), NULL), func, NULL));
            }
            break;
        }
        case 106://领取救济金币
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            //先判断能不能领取
            if(!m_bCanGet)//不能领取，这种情况有一下几种：
                //1、当前分数够2、银行分数够3、当前分数和银行分数之和够4、今日救济金已全部领取
            {
                if(m_nLeft == 0)
                {
                    break;
                }
                
                else if(m_llCurrentScore >= m_nConditionScore)//身上分数够，不能领取
                {
                    CMainLogic::sharedMainLogic()->ShowMessage(m_sFailInfo.c_str(), eMBOK);
                }
                
                break;
            }
            sendGetCoin();
            break;
        }
        case 107:
        {
#if defined(IOS_ANDROID)
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            //请求分享域名
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eBtFreeCoinShare);
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_SHARE_URL);
            CMainLogic::sharedMainLogic()->ShowMessage("正在请求，请稍候", eMBExitGame);
#else
            CMainLogic::sharedMainLogic()->ShowMessage("此功能暂未开放", eMBOK);
#endif
            break;
        }
        case 108:
        {
            if (!CMainLogic::sharedMainLogic()->m_bCanBandingTel) {
                break;
            }
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eBtFreeCoinBanding);
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            m_nodeFreeCoin->setScale(0);
            m_spBangingBg->setVisible(true);
            m_spBangingBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
            
            break;
        }
        case 109://发送验证码
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            if(!checkTelephone())
                break;
            std::string telephoneStr = m_pTelBox->getText();
            //向服务端请求验证码
            //绑定手机号验证码请求
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eBtGetUserTel);
            CMD_GPR_MB_BIND_REQUEST MBBindRequest;
            strncpy(MBBindRequest.szMobilePhone,telephoneStr.c_str(),sizeof(MBBindRequest.szMobilePhone));
            MBBindRequest.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_MOBILE_BIND,&MBBindRequest,sizeof(MBBindRequest));
            //开启倒计时
            m_labelDaojishi->setString("60秒");
            this -> schedule(schedule_selector(FreeCoinLayer::mesDaojishi), 1.0f);
            m_sendCodeItem -> setVisible(false);
            m_spDisable -> setVisible(true);
            m_labelDaojishi->setVisible(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            CMainLogic::sharedMainLogic()->m_bOpenPhoto = true;
#endif
            break;
        }
        case 110://下一步（密码界面显示,判断手机号和验证码正确性）
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            if(!checkTelephone())
                break;
            std::string codeStr = m_pCodeBox->getText();
            if(codeStr == "")
            {
                CMainLogic::sharedMainLogic()->ShowMessage("请您输入手机短信验证码。", eMBOK);
                break;
            }
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSendTelCodeNext);
            m_spBangingBg -> setVisible(false);
            m_spPasswordBg -> setVisible(true);
            m_spPasswordBg -> runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
            
            break;
        }
        case 111://关闭手机输入界面
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
            m_spBangingBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), CCHide::create(), NULL));
            m_nodeFreeCoin->runAction(CCSequence::create(CCDelayTime::create(0.21),CCShow::create(), CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
            break;
        }
        case 112://关闭密码输入界面
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
            m_spPasswordBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), CCHide::create(), NULL));
            m_spBangingBg -> runAction(CCSequence::create(CCDelayTime::create(0.2f),CCScaleTo::create(0, 0.8f),CCShow::create(), CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
            break;
        }
        case 113://密码确定按钮
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            std::string passwordStr1 = m_pPassword1Box->getText();
            std::string passwordStr2 = m_pPassword2Box->getText();
            if(passwordStr1 == "")
            {
                CMainLogic::sharedMainLogic()->ShowMessage("请输入密码", eMBOK);
                break;
            }
            if(passwordStr2 == "")
            {
                CMainLogic::sharedMainLogic()->ShowMessage("请再次输入密码", eMBOK);
                break;
            }
            if(passwordStr1 != passwordStr2)
            {
                CMainLogic::sharedMainLogic()->ShowMessage("对不起，您两次输入的密码不一致，请重新输入。", eMBOK);
                break;
            }
            if(passwordStr1.length() < 6)
            {
                CMainLogic::sharedMainLogic()->ShowMessage("请输入至少6位的有效密码。", eMBOK);
                break;
            }

            CMainLogic::sharedMainLogic()->m_sModifyPassword = passwordStr1;
            BYTE cbDataBuffer[SOCKET_TCP_PACKET];
            //发送消息
            std::string telephoneStr = m_pTelBox->getText();
            std::string codeStr = m_pCodeBox->getText();
            std::string sNickName = m_pNickNameBox->getText();
            char szMD5Result[128];
            CEncryptMD5::toString32(passwordStr1.c_str(),szMD5Result);
            CMD_GP_MobileCaptchaVerific* pMobileCaptchaVerific = (CMD_GP_MobileCaptchaVerific*)cbDataBuffer;
            pMobileCaptchaVerific->dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            strncpy(pMobileCaptchaVerific->szMobilePhone,telephoneStr.c_str(),sizeof(pMobileCaptchaVerific->szMobilePhone));
            strncpy(pMobileCaptchaVerific->szVerificationCode, codeStr.c_str(), sizeof(pMobileCaptchaVerific->szVerificationCode));
            strncpy(pMobileCaptchaVerific->szLogonPass,szMD5Result,sizeof(pMobileCaptchaVerific->szLogonPass));
            
            WORD wHeadSize = sizeof(CMD_GP_MobileCaptchaVerific);
            CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);
            if(sNickName != "")
            {
                SendPacket.AddPacket(CCharCode::UTF8ToGB2312(sNickName.c_str()).c_str(), DTP_GP_UI_NICKNAME);
            }
            WORD wSendSize = SendPacket.GetDataSize()+sizeof(CMD_GP_MobileCaptchaVerific);
            
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_MOBILE_BIND_VERIFY, cbDataBuffer, wSendSize);
            
            CMainLogic::sharedMainLogic()->ShowMessage("正在验证，请稍候...", eMBExitGame);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSendBandingComplete);
            break;
        }
        case 115://关闭密码输入界面
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
            m_spPasswordBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), CCHide::create(), NULL));
            m_nodeFreeCoin->runAction(CCSequence::create(CCDelayTime::create(0.21),CCShow::create(), CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
            m_spBangingBg->setVisible(false);
            break;
        }
        case 116:
        {
            m_menuFree->setTouchEnabled(false);
            m_menuClose->setTouchEnabled(false);
            m_spGave->setVisible(true);
            m_spGave->runAction(CCSequence::create(CCFadeIn::create(0.01f), CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
            m_spFreeCoinBg->setVisible(false);
            break;
        }
        default:
            break;
    }
}

void FreeCoinLayer::sendShareInfo(std::string str1, std::string str2)
{
	CMainLogic::sharedMainLogic()->m_nShareInterface = 1;
    //只做微信分享
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(ChargeWebView::checkWX())
    {
        CMainLogic::sharedMainLogic()->m_bSharedState = true;
        WeiXinCharge = 2;
        ChargeWebView::openIOSShare(str1, str2, 1);//0代表微信好友，1代表朋友圈
        //ChargeWebView::openQQShare(str1, str2, 0);//0代表QQ好友，1代表QQ空间
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，检测到您还未安装微信客户端，分享失败", eMBOK);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    jobject jobj;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "cn/sharesdk/hellocpp/HelloCpp","getInstance","()Ljava/lang/Object;");
    if (isHave)
    {
        //调用Java静态函数，取得对象。
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        if (jobj != NULL)
        {
            isHave = JniHelper::getMethodInfo(minfo,"cn/sharesdk/hellocpp/HelloCpp","share2weixin","(Ljava/lang/String;Ljava/lang/String;I)V");
            if (isHave)
            {
                jstring jStr1 = minfo.env->NewStringUTF(str1.c_str());
				jstring jStr2 = minfo.env->NewStringUTF(str2.c_str());
                jint nType = 1;
                //调用java非静态函数, 参数1：Java对象，上面已经取得   参数2：方法ID
                minfo.env->CallVoidMethod(jobj, minfo.methodID, jStr1, jStr2, nType);
            }
        }
    }
#endif
}

void FreeCoinLayer::sendGetCoin()
{
    int nUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    char temp[256];
    sprintf(temp, "http://%s/GetBankruptcy.aspx?uid=%d&action=1",NORMAL_URL,nUserID);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setTag("http test");
    request -> setResponseCallback(this, httpresponse_selector(FreeCoinLayer::onHttpGetCoinComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
    CMainLogic::sharedMainLogic()->ShowMessage("正在领取，请稍候", eMBNull);
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eBtGiveLingqu);
}

void FreeCoinLayer::onHttpGetCoinComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
    
    //对str进行xml解析，将30条数据保存到m_vctRankTimes中
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("freeGold"))
        {
            delete document;
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (NULL != pCurrentElement) {
            const char* sValue = pCurrentElement->Value();
            if(!strcmp(sValue, "result"))
            {
                const char* value = pCurrentElement->Attribute("content");
                //领取成功或者失败，显示提示界面
                CMainLogic::sharedMainLogic()->ShowMessage(value, eMBOK);
                std::string sResult = pCurrentElement->Attribute("success");
                if(sResult == "True")
                {
                    //播放金币特效
                    CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                    if(pHallLayer != NULL)
                    {
                        SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                        pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                    }
                    //关闭当前界面
                    CCMenuItem* item = CCMenuItem::create();
                    item -> setTag(105);
                    clickMenu(item);
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eLingquSuccess);
                }
                else
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eLingquFail);
                }
                
            }
            if(!strcmp(sValue, "bankruptcy"))
            {
                const char* value = pCurrentElement->Attribute("canGet");
                m_bCanGet = atoi(value);
                value = pCurrentElement->Attribute("total");
                m_nTotal = atoi(value);
                value = pCurrentElement->Attribute("left");
                m_nLeft = atoi(value);
                CMainLogic::sharedMainLogic()->m_nBankruptcyCount = CMainLogic::sharedMainLogic()->m_nGiveTotal-m_nLeft;
                value = pCurrentElement->Attribute("gold");
                m_nGold = atoi(value);
                value = pCurrentElement->Attribute("currentScore");
                m_llCurrentScore = atoll(value);
                value = pCurrentElement->Attribute("conditionScore");
                m_nConditionScore = atoi(value);
                value = pCurrentElement->Attribute("des");
                m_sFailInfo = value;
                //更新大厅内玩家分数
                CMainLogic::sharedMainLogic()->m_lUserScore = m_llCurrentScore;
                if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                {
                    CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                    if(pHallLayer != NULL)
                    {
                        pHallLayer->setUserScore(m_llCurrentScore);
                    }
                }
                //更新此界面上显示的剩余领取次数
                if(m_nTotal != 0)
                {
                    char temp[32];
                    sprintf(temp, "今天仅剩%d次", m_nLeft);
                    m_lGaveTimes->setString(temp);
                    if(m_nLeft == 0)//剩余领取次数为0，将领取按钮设置为不可点击状态
                    {
                        //sprintf(temp, "%d/%d", m_nTotal-m_nLeft , m_nTotal);
                        m_menuReceive1 -> setVisible(false);
                        m_menuReceive2 -> setVisible(true);
                    }
                    else
                    {
                        //sprintf(temp, "%d/%d", m_nTotal-m_nLeft+1 , m_nTotal);
                    }
                    m_lGaveLastTimes->setString(temp);
                    sprintf(temp, "%d金币", m_nGold);
                    m_lGaveCoin -> setString(temp);
                }
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;//230905
    
    if(CMainLogic::sharedMainLogic()->m_nGiveScore < CMainLogic::sharedMainLogic()->m_lUserScore)
    {
        //停止动作
        CCArray* array = m_menuFree->getChildren();
        for(unsigned int i = 0; i < array->count(); i++)
        {
            CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
            if(pItem->getTag() == 102)
            {
                pItem -> stopAllActions();
                pItem -> setScale(1.0f);
                break;
            }
        }
        CMainLogic::sharedMainLogic()->m_cbCanGetBankruptcy = 0;
        //将大厅心形隐藏
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                if(CMainLogic::sharedMainLogic()->m_nLogonGift != 0 || CMainLogic::sharedMainLogic()->m_bCanShare || CMainLogic::sharedMainLogic()->m_cbCanGetBankruptcy>0 || !CMainLogic::sharedMainLogic()->m_bReceiveVipLogon || CMainLogic::sharedMainLogic()->m_bCanBandingTel)
                {
                    pHallLayer->showfreecoinAction(true);
                }
                else
                {
                    pHallLayer->showfreecoinAction(false);
                }
            }
        }
    }
}

void FreeCoinLayer::removeWnd()
{
    this -> removeFromParentAndCleanup(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CMainLogic::sharedMainLogic()->m_bOpenPhoto = false;
#endif
}

void FreeCoinLayer::removeWnd2()
{
    m_spGave->setVisible(false);
    m_spFreeCoinBg->setVisible(true);
    m_nodeFreeCoin->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void FreeCoinLayer::setFreeCoinBg(bool bShow)
{
    if(bShow)
    {
        m_nodeFreeCoin->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
        m_menuFree->setTouchEnabled(true);
        m_menuClose->setTouchEnabled(true);
        m_spFreeCoinBg->setVisible(true);
    }
}

void FreeCoinLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool FreeCoinLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

void FreeCoinLayer::setDelayTimeToSendShareSuccess()
{
    this -> scheduleOnce(schedule_selector(FreeCoinLayer::sendSharedCompleted), 0.1f);
}

void FreeCoinLayer::sendSharedCompleted()
{
    int nGameID = CMainLogic::sharedMainLogic()->m_nGameID;
    char temp[30];
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
    request -> setResponseCallback(this, httpresponse_selector(FreeCoinLayer::onHttpSharedComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
    CMainLogic::sharedMainLogic()->ShowMessage("正在领取，请稍候", eMBExitGame);
}

void FreeCoinLayer::onHttpSharedComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    
    int nResult = 0;
    
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("xml"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while(pCurrentElement != NULL)
        {
            std::string value = pCurrentElement->Value();
            if(value == "text")
            {
                std::string sText = pCurrentElement->GetText();
                CMainLogic::sharedMainLogic()->ShowMessage(sText.c_str(), eMBOK);
                std::string sResult = "已经领取";
                std::string::size_type idx = sText.find(str);
                if(idx != std::string::npos)
                {
                    nResult = 1;
                }
            }
            else if(value == "Score")
            {
                nResult = 1;
                const char* score = pCurrentElement->GetText();
                SCORE llScore = atoll(score);
                if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                {
                    CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                    if(pHallLayer != NULL)
                    {
                        pHallLayer->setUserScore(llScore);
                        CMainLogic::sharedMainLogic()->m_lUserScore = llScore;
                        SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                        pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                    }
                }
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    document->Clear();
    delete document;
    bool bCanShare = CMainLogic::sharedMainLogic()->m_bCanShare;
    if(bCanShare)
    {
        if(nResult == 1)
        {
            CMainLogic::sharedMainLogic()->m_bCanShare = false;
        }
        else
        {
            CMainLogic::sharedMainLogic()->m_bCanShare = true;
        }
    }
    if(!CMainLogic::sharedMainLogic()->m_bCanShare)
    {
        //停止动画
        CCArray* array = m_menuFree->getChildren();
        for(unsigned int i = 0; i < array->count(); i++)
        {
            CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
            if(pItem->getTag() == 107)
            {
                pItem -> stopAllActions();
                pItem -> setScale(1.0f);
                break;
            }
        }
        //停止动画
        if(CMainLogic::sharedMainLogic()->m_nGiveScore < CMainLogic::sharedMainLogic()->m_lUserScore)
        {
            //停止动作
            CCArray* array = m_menuFree->getChildren();
            for(unsigned int i = 0; i < array->count(); i++)
            {
                CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
                if(pItem->getTag() == 102)
                {
                    pItem -> stopAllActions();
                    pItem -> setScale(1.0f);
                    break;
                }
            }
            CMainLogic::sharedMainLogic()->m_cbCanGetBankruptcy = 0;
        }
    }
    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            if(CMainLogic::sharedMainLogic()->m_nLogonGift != 0 || CMainLogic::sharedMainLogic()->m_bCanShare || CMainLogic::sharedMainLogic()->m_cbCanGetBankruptcy>0 || !CMainLogic::sharedMainLogic()->m_bReceiveVipLogon || CMainLogic::sharedMainLogic()->m_bCanBandingTel)
            {
                pHallLayer->showfreecoinAction(true);
            }
            else
            {
                pHallLayer->showfreecoinAction(false);
            }
        }
    }
}

void FreeCoinLayer::stopLogonItem()
{
    //停止动画
    CCArray* array = m_menuFree->getChildren();
    for(unsigned int i = 0; i < array->count(); i++)
    {
        CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
        if(pItem->getTag() == 101)
        {
            pItem -> stopAllActions();
            pItem -> setScale(1.0f);
            CCLabelTTF* pLabel = (CCLabelTTF*)pItem->getChildByTag(101);
            if(pLabel != NULL)
            {
                pLabel -> setString("已签到");
            }
            break;
        }
    }
    if(CMainLogic::sharedMainLogic()->m_lUserScore > CMainLogic::sharedMainLogic()->m_nGiveScore)//如果玩家身上分数大于领取限制，则停止救济金跳动
    {
        m_bCanGet = false;
        //停止动作
        CCArray* array = m_menuFree->getChildren();
        for(unsigned int i = 0; i < array->count(); i++)
        {
            CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
            if(pItem->getTag() == 102)
            {
                pItem -> stopAllActions();
                pItem -> setScale(1.0f);
                break;
            }
        }
    }
}

SCORE FreeCoinLayer::getCurrentScore()
{
    return m_llCurrentScore;
}

bool FreeCoinLayer::checkTelephone()
{
    //验证手机号是否合法
    std::string telephoneStr = m_pTelBox->getText();
    const char* pChar = telephoneStr.c_str();
    bool bTelephone = true;
    //判断字符串是否符合手机号码
    if(telephoneStr == "")
    {
        CMainLogic::sharedMainLogic()->ShowMessage("请输入您当前使用的手机号码。", eMBOK);
        return false;
    }
    else if(telephoneStr.length() != 11)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("请输入有效的手机号", eMBOK);
        return false;
    }
    else
    {
        for(int i = 0; i < 11; i++)
        {
            if(pChar[i]<'0' || pChar[i]>'9')
            {
                bTelephone = false;
                break;
            }
        }
        if(bTelephone)
        {
            if(pChar[0] == '1')
            {
            }
            else
            {
                CMainLogic::sharedMainLogic()->ShowMessage("请输入有效的手机号", eMBOK);
                return false;
            }
        }
        else
        {
            CMainLogic::sharedMainLogic()->ShowMessage("请输入有效的手机号", eMBOK);
            return false;
        }
    }
    return true;
}

//短信倒计时定时器
void FreeCoinLayer::mesDaojishi(float dt)
{
    m_nDaojishi--;
    if(m_nDaojishi<0)
    {
        //停止定时器
        this -> unschedule(schedule_selector(FreeCoinLayer::mesDaojishi));
        m_nDaojishi = 60;
        m_labelDaojishi -> setVisible(false);
        m_sendCodeItem -> setVisible(true);
        m_spDisable -> setVisible(false);
        return ;
    }
    char temp[16];
    sprintf(temp, "%d秒", m_nDaojishi);
    m_labelDaojishi->setString(temp);
}

void FreeCoinLayer::resetStatus()
{
    this -> unschedule(schedule_selector(FreeCoinLayer::mesDaojishi));
    m_labelDaojishi->setVisible(false);
    m_nDaojishi = 60;
    m_sendCodeItem -> setVisible(true);
    m_spDisable -> setVisible(false);
}

void FreeCoinLayer::stopBindingAction()
{
    //停止动作
    CCArray* array = m_menuFree->getChildren();
    for(unsigned int i = 0; i < array->count(); i++)
    {
        CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
        if(pItem->getTag() == 108)
        {
            pItem -> stopAllActions();
            pItem -> setScale(1.0f);
            CCLabelTTF* alReadyBindingTTF = (CCLabelTTF*)pItem->getChildByTag(102);
            if (alReadyBindingTTF != NULL) {
                alReadyBindingTTF->setVisible(true);
            }
            CCLabelTTF* noReadyBindingTTF = (CCLabelTTF*)pItem->getChildByTag(101);
            if (noReadyBindingTTF != NULL) {
                noReadyBindingTTF->setVisible(false);
            }
            break;
        }
    }
}
