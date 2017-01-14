#include "header.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../platform/android/jni/JniHelper.h"
#include "jni.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "FMLayerWebView.h"
#endif


enum eControlID
{
    ebtLogon=100,
    ebtRegister,
    ebtSwitchLogon,
    ebtQuickLogon,
    ebtSharedTag,
    ebtSettingTag,
    ebtServiceTag,
	eMenuBottomTag,
    eRegRefereeTag,
    eAnySDKMenuTag,
    eAnySDKLogonTag,
    eSelfLogonTag,
    eAnySDKLogonSuccessTag,
};

CCScene* CLogonLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CLogonLayer *layer = CLogonLayer::create();
    
    layer->setTag(eLogonLayerTag);
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CLogonLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
#if defined(ANYSDK_BD_110000)
    static int nLogon = 0;
    if (nLogon == 0)
    {
        nLogon++;
        CCSprite* pBdLogo = CCSprite::create("bdLogo.png");
        pBdLogo->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
        this->addChild(pBdLogo, 1000);
        pBdLogo->runAction(CCSequence::create(CCDelayTime::create(1.5f), CCFadeOut::create(0), NULL));
    }
#endif
    m_bLogon = false;
    m_bLogonStatus = false;
    
    m_nDaojishi = 60;
    
    CMainLogic::sharedMainLogic()->m_nClickBank = 0;
    CMainLogic::sharedMainLogic()->m_nGameType = 0;
    CMainLogic::sharedMainLogic()->m_bFirstChargeUIStatus = false;
    
    m_spBg = CCSprite::create("h2all/buyubg.jpg");                   //登录背景图
    m_spBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(m_spBg);
    
    //游戏logo@@@@@@@@
    m_nodeSelected = CCNode::create();
    this -> addChild(m_nodeSelected);
    CCSprite* pLogoBg = CCSprite::create("l2ogon/logonsprite.png");
    pLogoBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+120));
    m_nodeSelected -> addChild(pLogoBg);
    pLogoBg->setScale(1.1f);
	//@@@@@@@@
    //反馈按钮
    CCMenuItemImage* reportItem = CCMenuItemImage::create("l2ogon/reportItem.png", "l2ogon/reportItem.png", this, menu_selector(CLogonLayer::clickReportMenu));
    reportItem -> setPosition(ccp(reportItem->getContentSize().width/2, CLIENT_HEIGHT-reportItem->getContentSize().height/2));
    CCMenu* pReportMenu = CCMenu::create(reportItem, NULL);
    pReportMenu -> setPosition(CCPointZero);
    pReportMenu -> setAnchorPoint(CCPointZero);
    this -> addChild(pReportMenu, 10);
    
    loadUIBg();
    loadNodeLogon();
    loadNodeRegister();
    
    float fSound = CCUserDefault::sharedUserDefault()->getFloatForKey("sound", 1);
    float fEffect = CCUserDefault::sharedUserDefault()->getFloatForKey("effect", 1);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(fSound);
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(fEffect);
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_labelVersion = CCLabelTTF::create("版本号：", "Arial", 40);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    m_labelVersion = CCLabelTTF::create("版本号：", "Arial", 40);
#endif
    m_labelVersion->setPosition(ccp(15,30));
    m_labelVersion->setAnchorPoint(ccp(0,0.5));
    this->addChild(m_labelVersion,12);
    m_labelVersion -> setVisible(false);
    m_bClickMenu = false;
    
    CMainLogic::sharedMainLogic()->m_bGameExitHall = false;
    
    return true;
}

void CLogonLayer::checkSelfLogon()
{
    //检测自定义登陆
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/Lelesdkversion.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(CLogonLayer::onSelfLogonHttpComplete));
    CCHttpClient::getInstance()->send(request);
    CC_SAFE_RELEASE(request);
    request = NULL;
}

void CLogonLayer::onSelfLogonHttpComplete(CCHttpClient* sender, CCHttpResponse* response)
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
   
    //解析
    TiXmlDocument* document = new TiXmlDocument();
    int len = strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("pinfo"))
        {
            return;
        }
        TiXmlElement * pCurrentElement=rootElement->FirstChildElement();
        while (pCurrentElement != NULL)
        {
            const char * pPlatform=pCurrentElement->Attribute("n");//渠道名称
            const char * str=pCurrentElement->Attribute("b");//是否显示隐藏项
            if (pPlatform != NULL && str != NULL)
            {
#if defined(ANYSDK_QH360_000023)
                if(strcmp(pPlatform, "360")==0)
                {
                    CCUserDefault::sharedUserDefault()->setBoolForKey("ANYSDK_QH360_000023", atoi(str));
                    CMainLogic::sharedMainLogic()->m_bAnySDKPassed = CCUserDefault::sharedUserDefault()->getBoolForKey("ANYSDK_QH360_000023", false);
                    setSelfLogonVisible(CMainLogic::sharedMainLogic()->m_bAnySDKPassed);
                }
#elif defined(ANYSDK_BD_110000)
                if(strcmp(pPlatform, "bd")==0)
                {
                    CCUserDefault::sharedUserDefault()->setBoolForKey("ANYSDK_BD_110000", atoi(str));
                    CMainLogic::sharedMainLogic()->m_bAnySDKPassed = CCUserDefault::sharedUserDefault()->getBoolForKey("ANYSDK_BD_110000", false);
                    setSelfLogonVisible(CMainLogic::sharedMainLogic()->m_bAnySDKPassed);
                }
#elif defined(ANYSDK_YYB_000550)
                if(strcmp(pPlatform, "yyb")==0)
                {
                    CCUserDefault::sharedUserDefault()->setBoolForKey("ANYSDK_YYB_000550", atoi(str));
                    CMainLogic::sharedMainLogic()->m_bAnySDKPassed = CCUserDefault::sharedUserDefault()->getBoolForKey("ANYSDK_YYB_000550", false);
                    setSelfLogonVisible(CMainLogic::sharedMainLogic()->m_bAnySDKPassed);
                }
#endif
            }
            pCurrentElement=pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
}

void CLogonLayer::setSelfLogonVisible(bool bVisible)
{
    CCMenu* pMenu = (CCMenu*)m_nodeSelected->getChildByTag(eAnySDKMenuTag);
    if (pMenu != NULL)
    {
        CCMenuItemImage* pSelfLogonItem = (CCMenuItemImage*)pMenu->getChildByTag(eSelfLogonTag);
        if (pSelfLogonItem != NULL)
        {
            pSelfLogonItem->setVisible(bVisible);
        }
    }
}

void CLogonLayer::loadUIBg()
{
    m_nodeSelected = CCNode::create();
    this -> addChild(m_nodeSelected);
    CCSprite* pLogoBg = CCSprite::create("l2ogon/logonsprite.png");
    pLogoBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+120));
    m_nodeSelected -> addChild(pLogoBg);
    pLogoBg->setScale(1.1f);
    CCMenuItemImage* quickItem = CCMenuItemImage::create("l2ogon/quickLogonItem.png", "l2ogon/quickLogonItem.png", this, menu_selector(CLogonLayer::menuCallFunc));
    quickItem -> setTag(ebtQuickLogon);
    quickItem -> setPosition(ccp(CLIENT_WIDTH/3, 130));
    //白光遮罩
    CCSprite* pQuickSprite = CCSprite::create("l2ogon/quickLogonItem2.png");
    CCSprite* pQuickSprite2 = CCSprite::create("l2ogon/quickLogonItem2.png");
    CCClippingNode* pCliper = CCClippingNode::create();
    pCliper->setStencil(pQuickSprite);
    pCliper->setAlphaThreshold(0);
    m_nodeSelected->addChild(pCliper, 3);
    pQuickSprite2->setOpacity(0);
    pCliper->setPosition(ccp(CLIENT_WIDTH/3, 130));
    pCliper->addChild(pQuickSprite2);
    CCSprite* pBaiGuang = CCSprite::create("hall/baiguang.png");
    pBaiGuang -> setPosition(ccp(-200, quickItem->getContentSize().height/2));
    pCliper -> addChild(pBaiGuang);
    pBaiGuang -> runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(2.0f, ccp(500, 0)), CCDelayTime::create(2.0f), CCMoveBy::create(0, ccp(-500, 0)), NULL)));
    
    
    CCMenuItemImage* accountItem = CCMenuItemImage::create("l2ogon/accountLogonItem.png", "l2ogon/accountLogonItem.png", this, menu_selector(CLogonLayer::enterAccountNode));
    accountItem -> setPosition(ccp(CLIENT_WIDTH/3*2, 130));
    //白光遮罩
    CCSprite* pQuickSprite3 = CCSprite::create("l2ogon/quickLogonItem2.png");
    CCSprite* pQuickSprite4 = CCSprite::create("l2ogon/quickLogonItem2.png");
    CCClippingNode* pCliper2 = CCClippingNode::create();
    pCliper2->setStencil(pQuickSprite3);
    pCliper2->setAlphaThreshold(0);
    m_nodeSelected->addChild(pCliper2, 3);
    pQuickSprite4->setOpacity(0);
    pCliper2->setPosition(ccp(CLIENT_WIDTH/3*2, 130));
    pCliper2->addChild(pQuickSprite4);
    CCSprite* pBaiGuang2 = CCSprite::create("hall/baiguang.png");
    pBaiGuang2 -> setPosition(ccp(-200, accountItem->getContentSize().height/2));
    pCliper2 -> addChild(pBaiGuang2);
    pBaiGuang2 -> runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(2.0f), CCMoveBy::create(2.0f, ccp(500, 0)), CCMoveBy::create(0, ccp(-500, 0)), NULL)));
    
    
    CCMenu* pMenu = CCMenu::create(quickItem, accountItem, NULL);
    pMenu -> setPosition(CCPointZero);
    pMenu -> setAnchorPoint(CCPointZero);
    m_nodeSelected -> addChild(pMenu);
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
    pMenu->setVisible(false);
    CCMenuItemImage* pAnySDKLogonItem = CCMenuItemImage::create("l2ogon/loginBtn.png", "l2ogon/loginBtn2.png", this, menu_selector(CLogonLayer::menuCallFunc));
    pAnySDKLogonItem->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/6));
    pAnySDKLogonItem->setTag(eAnySDKLogonTag);
    CCMenuItemImage* pSelfLogonItem = CCMenuItemImage::create("l2ogon/login2Btn.png", "l2ogon/login2Btn2.png", this, menu_selector(CLogonLayer::menuCallFunc));
    pSelfLogonItem->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.38f));
    pSelfLogonItem->setTag(eSelfLogonTag);
    pSelfLogonItem->setVisible(false);
    CCMenu* pAnySDKMenu = CCMenu::create(pAnySDKLogonItem, pSelfLogonItem, NULL);
    pAnySDKMenu->setPosition(CCPointZero);
    pAnySDKMenu->setAnchorPoint(CCPointZero);
    m_nodeSelected->addChild(pAnySDKMenu, 0, eAnySDKMenuTag);
    checkSelfLogon();
#endif
}

void CLogonLayer::loadNodeLogon()
{
    m_nodeLogon = CCNode::create();
    this -> addChild(m_nodeLogon);
    m_nodeLogon -> setVisible(false);
    CCSprite* pSpLogon = CCSprite::create("h2all/buyubg.jpg");
    pSpLogon -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    m_nodeLogon -> addChild(pSpLogon);
//    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 40), 1280, 720);
//    pSpLogon -> addChild(pLayerColor);
    CCSprite* pSpLogo = CCSprite::create("l2ogon/logonsprite.png");
    pSpLogo -> setPosition(ccp(pSpLogon->getContentSize().width/2, pSpLogon->getContentSize().height-pSpLogo->getContentSize().height/2-20));
    pSpLogon -> addChild(pSpLogo);
    CCMenuItemImage* logonItem = CCMenuItemImage::create("l2ogon/logonitem.png", "l2ogon/logonitem.png", this, menu_selector(CLogonLayer::menuCallFunc));
    logonItem -> setPosition(ccp(pSpLogon->getContentSize().width/3*2-50, 90));
    logonItem -> setTag(ebtLogon);
    CCMenuItemImage* registerItem = CCMenuItemImage::create("l2ogon/registeritem.png", "l2ogon/registeritem.png", this, menu_selector(CLogonLayer::enterRegisterNode));
    registerItem -> setPosition(ccp(pSpLogon->getContentSize().width/3+50, 90));
    CCMenuItemImage* backItem = CCMenuItemImage::create("l2ogon/registerbackitem.png", "l2ogon/registerbackitem.png", this, menu_selector(CLogonLayer::clickBackGuestUI));
    backItem -> setPosition(ccp(CLIENT_WIDTH-backItem->getContentSize().width/2, CLIENT_HEIGHT-backItem->getContentSize().height/2));
    CCMenu* pMenu = CCMenu::create(logonItem, registerItem, backItem, NULL);
    pMenu -> setPosition(CCPointZero);
    pMenu -> setAnchorPoint(CCPointZero);
    pSpLogon -> addChild(pMenu);
    
    CCSprite* pSpLabel1 = CCSprite::create("l2ogon/logonlabel1.png");
    pSpLabel1 -> setPosition(ccp(pSpLogon->getContentSize().width/2-210, pSpLogon->getContentSize().height/2-40));
    pSpLogon -> addChild(pSpLabel1);
    CCSprite* pSpLabel2 = CCSprite::create("l2ogon/logonlabel2.png");
    pSpLabel2 -> setPosition(ccp(pSpLogon->getContentSize().width/2-210, pSpLogon->getContentSize().height/2-140));
    pSpLogon -> addChild(pSpLabel2);
    
    m_pUserNameBox = CCEditBox::create(CCSize(418,70), CCScale9Sprite::create("l2ogon/logonedit.png"));
    m_pUserNameBox->setMaxLength(20);//限制最长字符
    m_pUserNameBox->setReturnType(kKeyboardReturnTypeDone);
    m_pUserNameBox->setInputFlag(kEditBoxInputFlagSensitive);
    m_pUserNameBox->setInputMode(kEditBoxInputModeEmailAddr);
    m_pUserNameBox->setPosition(ccp(pSpLogon->getContentSize().width/2+60, pSpLogon->getContentSize().height/2-40));
    pSpLogon->addChild(m_pUserNameBox);
    m_pPasswordBox = CCEditBox::create(CCSize(418,70), CCScale9Sprite::create("l2ogon/logonedit.png"));//密码输入框
    m_pPasswordBox->setMaxLength(20);
    m_pPasswordBox->setReturnType(kKeyboardReturnTypeDone);
    m_pPasswordBox->setInputFlag(kEditBoxInputFlagPassword);
    m_pPasswordBox->setPosition(ccp(pSpLogon->getContentSize().width/2+60, pSpLogon->getContentSize().height/2-140));
    pSpLogon->addChild(m_pPasswordBox);
    if(CCUserDefault::sharedUserDefault()->getBoolForKey("NewGuestBank"))
    {
        m_pUserNameBox->setText(CCUserDefault::sharedUserDefault()->getStringForKey("NewGuestAccount", "").c_str());
        m_pPasswordBox->setText(CCUserDefault::sharedUserDefault()->getStringForKey("NewGuestPassword", "").c_str());
    }
    else
    {
        m_pUserNameBox->setText(CCUserDefault::sharedUserDefault()->getStringForKey("NewAccount", "").c_str());
        m_pPasswordBox->setText(CCUserDefault::sharedUserDefault()->getStringForKey("NewPassword", "").c_str());
    }
}

void CLogonLayer::loadNodeRegister()
{
    m_nodeRegister = CCSprite::create();
    m_nodeRegister->setPosition(CCPointZero);
    this -> addChild(m_nodeRegister);
    m_nodeRegister -> setVisible(false);
    
    CCSprite* pSpRegisterBg2 = CCSprite::create("h2all/buyubg.jpg");
    pSpRegisterBg2 -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    m_nodeRegister -> addChild(pSpRegisterBg2, 2);
    m_accSprite = CCSprite::create();
    m_accSprite -> setPosition(ccp(0, 0));
    m_nodeRegister -> addChild(m_accSprite, 2);
    m_telSprite = CCSprite::create();
    m_telSprite -> setPosition(CCPointZero);
    m_nodeRegister -> addChild(m_telSprite, 2);
    m_accSprite -> setVisible(false);
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 40), 1280, 720);
    pSpRegisterBg2 -> addChild(pLayerColor);
    
    CCMenuItemImage* telItem = CCMenuItemImage::create("l2ogon/normalitem.png", "l2ogon/normalitem.png", this, menu_selector(CLogonLayer::clickRegister));
    telItem -> setTag(100);
    telItem -> setPosition(ccp(CLIENT_WIDTH/3+16, CLIENT_HEIGHT-110));
    m_menutel = CCMenu::create(telItem, NULL);
    m_menutel -> setPosition(CCPointZero);
    m_menutel -> setAnchorPoint(CCPointZero);
    m_nodeRegister -> addChild(m_menutel, 3);
    CCMenuItemImage* accItem = CCMenuItemImage::create("l2ogon/normalitem.png", "l2ogon/normalitem.png", this, menu_selector(CLogonLayer::clickRegister));
    accItem -> setTag(101);
    accItem -> setPosition(ccp(CLIENT_WIDTH/3*2-16, CLIENT_HEIGHT-110));
    m_menuacc = CCMenu::create(accItem, NULL);
    m_menuacc -> setPosition(CCPointZero);
    m_menuacc -> setAnchorPoint(CCPointZero);
    m_nodeRegister -> addChild(m_menuacc, 3);
    m_spriteTel = CCSprite::create("l2ogon/selectitem.png");
    m_spriteTel -> setPosition(ccp(CLIENT_WIDTH/3+16, CLIENT_HEIGHT-114));
    m_nodeRegister -> addChild(m_spriteTel, 3);
    CCSprite* spSelecttel = CCSprite::create("l2ogon/selecttel.png");
    spSelecttel -> setPosition(ccp(m_spriteTel->getContentSize().width/2, m_spriteTel->getContentSize().height/2+3));
    m_spriteTel -> addChild(spSelecttel);
    m_spriteAcc = CCSprite::create("l2ogon/selectitem.png");
    m_spriteAcc -> setPosition(ccp(CLIENT_WIDTH/3*2-16, CLIENT_HEIGHT-114));
    m_nodeRegister -> addChild(m_spriteAcc, 3);
    CCSprite* spSelectacc = CCSprite::create("l2ogon/selectacc.png");
    spSelectacc -> setPosition(ccp(m_spriteAcc->getContentSize().width/2, m_spriteAcc->getContentSize().height/2+3));
    m_spriteAcc -> addChild(spSelectacc);
    m_menutel -> setTouchEnabled(false);
    m_spriteAcc -> setVisible(false);
    CCSprite* pTelTitle = CCSprite::create("l2ogon/teltitle.png");
    pTelTitle -> setPosition(ccp(m_spriteTel->getContentSize().width/2, m_spriteTel->getContentSize().height/2));
    pTelTitle -> setOpacity(92);
    telItem -> addChild(pTelTitle);
    CCSprite* pAccTitle = CCSprite::create("l2ogon/acctitle.png");
    pAccTitle -> setPosition(ccp(m_spriteAcc->getContentSize().width/2, m_spriteAcc->getContentSize().height/2));
    pAccTitle -> setOpacity(92);
    accItem -> addChild(pAccTitle);
    m_nRegisterType = 1;//1代表手机号注册，2代表账号注册
    
    CCMenuItemImage* backItem = CCMenuItemImage::create("l2ogon/registerbackitem.png", "l2ogon/registerbackitem.png", this, menu_selector(CLogonLayer::clickBackMenu));
    backItem -> setPosition(ccp(CLIENT_WIDTH-backItem->getContentSize().width/2, CLIENT_HEIGHT-backItem->getContentSize().height/2));
    CCMenuItemImage* registerItem = CCMenuItemImage::create("l2ogon/registeritem2.png", "l2ogon/registeritem2.png", this, menu_selector(CLogonLayer::menuCallFunc));
    registerItem -> setPosition(ccp(CLIENT_WIDTH/2, 70));
    registerItem -> setTag(ebtRegister);
    CCMenu* pRegisterMenu = CCMenu::create(backItem, registerItem, NULL);
    pRegisterMenu -> setPosition(CCPointZero);
    pRegisterMenu -> setAnchorPoint(CCPointZero);
    pSpRegisterBg2 -> addChild(pRegisterMenu, 3);
    
    CCSprite* pLabel3 = CCSprite::create("l2ogon/logonlabel3.png");
    pLabel3 -> setPosition(ccp(490, 490));
    pLabel3 -> setAnchorPoint(ccp(1, 0.5f));
    m_accSprite -> addChild(pLabel3);
    CCSprite* pLabel4 = CCSprite::create("l2ogon/logonlabel4.png");
    pLabel4 -> setPosition(ccp(490, 405));
    pLabel4 -> setAnchorPoint(ccp(1, 0.5f));
    m_accSprite -> addChild(pLabel4);
    CCSprite* pLabel6 = CCSprite::create("l2ogon/logonlabel6.png");
    pLabel6 -> setPosition(ccp(490, 235));
    pLabel6 -> setAnchorPoint(ccp(1, 0.5f));
    m_accSprite -> addChild(pLabel6);
    CCSprite* pLabel7 = CCSprite::create("l2ogon/logonlabel7.png");
    pLabel7 -> setPosition(ccp(490, 490));
    pLabel7 -> setAnchorPoint(ccp(1, 0.5f));
    m_telSprite -> addChild(pLabel7);
    CCSprite* pLabel8 = CCSprite::create("l2ogon/messprite.png");
    pLabel8 -> setPosition(ccp(490, 410));
    pLabel8 -> setAnchorPoint(ccp(1, 0.5f));
    m_telSprite -> addChild(pLabel8);
    CCSprite* pLabel9 = CCSprite::create("l2ogon/logonlabel4.png");
    pLabel9 -> setPosition(ccp(490, 330));
    pLabel9 -> setAnchorPoint(ccp(1, 0.5f));
    m_telSprite -> addChild(pLabel9);
    
    CCSprite* pXing1 = CCSprite::create("l2ogon/logonxing.png");
    pXing1 -> setPosition(ccp(330, 490));
    m_accSprite -> addChild(pXing1);
    CCSprite* pXing2 = CCSprite::create("l2ogon/logonxing.png");
    pXing2 -> setPosition(ccp(330, 405));
    m_accSprite -> addChild(pXing2);
    CCSprite* pXing3 = CCSprite::create("l2ogon/logonxing.png");
    pXing3 -> setPosition(ccp(330, 320));
    m_accSprite -> addChild(pXing3);
    CCSprite* pLabel5 = CCSprite::create("l2ogon/logonlabel5.png");
    pLabel5 -> setPosition(ccp(490, 320));
    pLabel5 -> setAnchorPoint(ccp(1, 0.5f));
    m_accSprite -> addChild(pLabel5);
    CCSprite* pXing4 = CCSprite::create("l2ogon/logonxing.png");
    pXing4 -> setPosition(ccp(330, 235));
    m_accSprite -> addChild(pXing4);
    
    CCSprite* pXing5 = CCSprite::create("l2ogon/logonxing.png");
    pXing5 -> setPosition(ccp(330, 490));
    m_telSprite -> addChild(pXing5);
    CCSprite* pXing6 = CCSprite::create("l2ogon/logonxing.png");
    pXing6 -> setPosition(ccp(330, 410));
    m_telSprite -> addChild(pXing6);
    CCSprite* pXing7 = CCSprite::create("l2ogon/logonxing.png");
    pXing7 -> setPosition(ccp(330, 330));
    m_telSprite -> addChild(pXing7);
    CCSprite* pLabel15 = CCSprite::create("l2ogon/logonlabel5.png");
    pLabel15 -> setPosition(ccp(490, 250));
    pLabel15 -> setAnchorPoint(ccp(1, 0.5f));
    m_telSprite -> addChild(pLabel15);
    CCSprite* pXing8 = CCSprite::create("l2ogon/logonxing.png");
    pXing8 -> setPosition(ccp(330, 250));
    m_telSprite -> addChild(pXing8);
    CCSprite* pXing9 = CCSprite::create("l2ogon/logonxing.png");
    pXing9 -> setPosition(ccp(330, 170));
    m_telSprite -> addChild(pXing9);
    CCSprite* pLabel16 = CCSprite::create("l2ogon/logonlabel6.png");
    pLabel16 -> setPosition(ccp(490, 170));
    pLabel16 -> setAnchorPoint(ccp(1, 0.5f));
    m_telSprite -> addChild(pLabel16);
    
    //验证码按钮
    CCMenuItemImage* sendItem = CCMenuItemImage::create("l2ogon/yanzhengmaitem.png", "l2ogon/yanzhengmaitem.png", this, menu_selector(CLogonLayer::clickMessageMenu));
    sendItem -> setPosition(ccp(1000, 490));
    m_sendMenu = CCMenu::create(sendItem, NULL);
    m_sendMenu -> setPosition(CCPointZero);
    m_sendMenu -> setAnchorPoint(CCPointZero);
    m_telSprite -> addChild(m_sendMenu);
    CCMenuItemImage* resendItem = CCMenuItemImage::create("l2ogon/resenditem.png", "l2ogon/resenditem.png", this, menu_selector(CLogonLayer::clickMessageMenu));
    resendItem -> setPosition(ccp(1000, 490));
    m_resendMenu = CCMenu::create(resendItem, NULL);
    m_resendMenu -> setPosition(CCPointZero);
    m_resendMenu -> setAnchorPoint(CCPointZero);
    m_telSprite -> addChild(m_resendMenu);
    m_resendMenu -> setVisible(false);
    m_labelDaojishi = CCLabelTTF::create("60秒", "Arial", 36);
    //m_labelDaojishi -> setColor(ccc3(255, 255, 255));
    m_labelDaojishi -> setPosition(ccp(950, 490));
    m_telSprite -> addChild(m_labelDaojishi);
    m_labelDaojishi -> setVisible(false);
    
    m_pEditBoxPassword = CCEditBox::create(CCSize(373,55), CCScale9Sprite::create("l2ogon/registeredit.png"));
    m_pEditBoxPassword->setMaxLength(20);//限制最长字符
    m_pEditBoxPassword->setReturnType(kKeyboardReturnTypeDone);
    m_pEditBoxPassword->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditBoxPassword->setInputMode(kEditBoxInputModeEmailAddr);
    m_pEditBoxPassword->setPosition(ccp(700, 250));
    m_pEditBoxPassword->setPlaceHolder("请输入密码");
    pSpRegisterBg2->addChild(m_pEditBoxPassword, 10);
    m_pEditBoxEnsurePassword = CCEditBox::create(CCSize(373,55), CCScale9Sprite::create("l2ogon/registeredit.png"));
    m_pEditBoxEnsurePassword->setMaxLength(20);//限制最长字符
    m_pEditBoxEnsurePassword->setReturnType(kKeyboardReturnTypeDone);
    m_pEditBoxEnsurePassword->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditBoxEnsurePassword->setInputMode(kEditBoxInputModeEmailAddr);
    m_pEditBoxEnsurePassword->setPosition(ccp(700, 170));
    m_pEditBoxEnsurePassword->setPlaceHolder("请再次输入密码");
    pSpRegisterBg2->addChild(m_pEditBoxEnsurePassword, 10);
    //m_pEditBoxPassword->setText("111111");
    //m_pEditBoxEnsurePassword->setText("111111");

    m_pEditBoxAcc = CCEditBox::create(CCSize(373,55), CCScale9Sprite::create("l2ogon/registeredit.png"));
    m_pEditBoxAcc->setMaxLength(16);//限制最长字符
    m_pEditBoxAcc->setReturnType(kKeyboardReturnTypeDone);
    m_pEditBoxAcc->setInputFlag(kEditBoxInputFlagSensitive);
    m_pEditBoxAcc->setInputMode(kEditBoxInputModeEmailAddr);
    m_pEditBoxAcc->setPosition(ccp(700, 490));
    m_pEditBoxAcc->setPlaceHolder("请输入账号");
    m_accSprite->addChild(m_pEditBoxAcc);
    m_pEditBoxNickName= CCEditBox::create(CCSize(373,55), CCScale9Sprite::create("l2ogon/registeredit.png"));
    m_pEditBoxNickName->setMaxLength(13);//限制最长字符
    m_pEditBoxNickName->setReturnType(kKeyboardReturnTypeDone);
    m_pEditBoxNickName->setInputFlag(kEditBoxInputFlagSensitive);
    m_pEditBoxNickName->setInputMode(kEditBoxInputModeEmailAddr);
    m_pEditBoxNickName->setPosition(ccp(700, 330));
    m_pEditBoxNickName->setPlaceHolder("请输入昵称");
    pSpRegisterBg2->addChild(m_pEditBoxNickName);
    m_pTelBox= CCEditBox::create(CCSize(373,55), CCScale9Sprite::create("l2ogon/registeredit.png"));
    m_pTelBox->setMaxLength(11);//限制最长字符
    m_pTelBox->setReturnType(kKeyboardReturnTypeDone);
    m_pTelBox->setInputFlag(kEditBoxInputFlagSensitive);
    m_pTelBox->setInputMode(kEditBoxInputModePhoneNumber);
    m_pTelBox->setPosition(ccp(700, 490));
    m_pTelBox->setPlaceHolder("请输入手机号");
    m_telSprite->addChild(m_pTelBox);
    //m_pTelBox->setText("15633297597");
    m_pEditBoxMessage= CCEditBox::create(CCSize(373,55), CCScale9Sprite::create("l2ogon/registeredit.png"));
    m_pEditBoxMessage->setMaxLength(16);//限制最长字符
    m_pEditBoxMessage->setReturnType(kKeyboardReturnTypeDone);
    m_pEditBoxMessage->setInputFlag(kEditBoxInputFlagSensitive);
    m_pEditBoxMessage->setInputMode(kEditBoxInputModePhoneNumber);
    m_pEditBoxMessage->setPosition(ccp(700, 410));
    m_pEditBoxMessage->setPlaceHolder("请输入验证码");
    m_telSprite->addChild(m_pEditBoxMessage);
    
}

void CLogonLayer::enterAccountNode(CCObject* object)
{
    if(!m_bClickMenu)
        return;
    CMainLogic::sharedMainLogic()->sendUserBehavior(0, eBtUserLogon);
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    m_nodeSelected -> setVisible(false);
    m_nodeLogon -> setVisible(true);
}

void CLogonLayer::clickBackGuestUI(CCObject* object)
{
    CMainLogic::sharedMainLogic()->sendUserBehavior(0, eBtBack);
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    m_nodeLogon -> setVisible(false);
    m_nodeRegister -> setVisible(false);
    m_nodeSelected -> setVisible(true);
}

void CLogonLayer::enterRegisterNode(CCObject* object)
{
    CMainLogic::sharedMainLogic()->sendUserBehavior(0, eBtUserRegister);
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    m_nodeSelected -> setVisible(false);
    m_nodeLogon -> setVisible(false);
    m_nodeRegister -> setVisible(true);
}

void CLogonLayer::clickBackMenu(CCObject* object)
{
    CMainLogic::sharedMainLogic()->sendUserBehavior(0, eBtBack);
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    m_nodeSelected -> setVisible(false);
    m_nodeLogon -> setVisible(true);
    m_nodeRegister -> setVisible(false);
}

void CLogonLayer::clickRegister(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case 100://手机注册
        {
            m_menutel->setTouchEnabled(false);
            m_menuacc->setTouchEnabled(true);
            m_spriteAcc->setVisible(false);
            m_spriteTel->setVisible(true);
            m_accSprite -> setVisible(false);
            m_telSprite -> setVisible(true);
            m_nRegisterType = 1;
            //设置位置
            m_pEditBoxNickName->setPosition(ccp(700, 330));
            m_pEditBoxEnsurePassword->setPosition(ccp(700, 170));
            m_pEditBoxPassword->setPosition(ccp(700, 250));
            break;
        }
        case 101://账号注册
        {
            m_menutel->setTouchEnabled(true);
            m_menuacc->setTouchEnabled(false);
            m_spriteAcc->setVisible(true);
            m_spriteTel->setVisible(false);;
            m_accSprite -> setVisible(true);
            m_telSprite -> setVisible(false);
            m_nRegisterType = 2;
            //设置位置
            m_pEditBoxNickName->setPosition(ccp(700, 405));
            m_pEditBoxEnsurePassword->setPosition(ccp(700, 235));
            m_pEditBoxPassword->setPosition(ccp(700, 320));
            break;
        }
        default:
            break;
    }
}

void CLogonLayer::clickMessageMenu(CCObject* object)//短信验证
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    //验证手机号是否合法
    std::string telephoneStr = m_pTelBox->getText();
    const char* pChar = telephoneStr.c_str();
    bool bTelephone = true;
     //判断字符串是否符合手机号码
    if(telephoneStr == "")
    {
        CMainLogic::sharedMainLogic()->ShowMessage("请输入您当前使用的手机号码。", eMBOK);
        return;
    }
    else if(telephoneStr.length() != 11)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("请输入有效的手机号", eMBOK);
        return;
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
                return;
            }
        }
        else
        {
            CMainLogic::sharedMainLogic()->ShowMessage("请输入有效的手机号", eMBOK);
            return;
        }
    }
    CMainLogic::sharedMainLogic()->sendUserBehavior(0, eBtGetUserTel);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string str = ChargeWebView::OnInternet();
    if(str == "")
    {
        //没有可用的网络
        CMainLogic::sharedMainLogic()->ShowMessage("当前没有可用的网络，请检查网络设置", eMBOK, eDisConnectNet);
        return;
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string str = CMainLogic::sharedMainLogic()->OnInternet();
    if(str == "2")//当前没有网络
    {
        CMainLogic::sharedMainLogic()->ShowMessage("当前没有可用的网络，请检查网络设置", eMBOK, eDisConnectNet);
        return;
    }
#endif
    {
        //向服务端请求验证码
        CMD_GPR_MBVerificationCode MBVerificationCode;
        strncpy(MBVerificationCode.szMobilePhone,telephoneStr.c_str(),sizeof(MBVerificationCode.szMobilePhone));
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GP_MBCODE_VERIFY,&MBVerificationCode,sizeof(MBVerificationCode));
    }
    
    
    //将验证码按钮设置为不可点击的状态
    m_sendMenu -> setVisible(false);
    m_resendMenu -> setVisible(false);
    m_labelDaojishi->setString("60秒");
    m_labelDaojishi->setVisible(true);
    //开启倒计时定时器
    
    this -> schedule(schedule_selector(CLogonLayer::mesDaojishi), 1.0f);
}

void CLogonLayer::sendTelRegister(float dt)
{
    for(int i = 0; i < 10; i++)
    {
        //向服务端请求验证码
        CMD_GPR_MBVerificationCode MBVerificationCode;
        strncpy(MBVerificationCode.szMobilePhone, "15633297597", sizeof(MBVerificationCode.szMobilePhone));
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GP_MBCODE_VERIFY,&MBVerificationCode,sizeof(MBVerificationCode));
    }
}

//短信倒计时定时器
void CLogonLayer::mesDaojishi()
{
    m_nDaojishi--;
    if(m_nDaojishi<0)
    {
        //停止定时器
        this -> unschedule(schedule_selector(CLogonLayer::mesDaojishi));
        m_nDaojishi = 60;
        m_labelDaojishi -> setVisible(false);
        m_resendMenu -> setVisible(true);
        return ;
    }
    char temp[16];
    sprintf(temp, "%d秒", m_nDaojishi);
    m_labelDaojishi->setString(temp);
}

void CLogonLayer::resetStatus()
{
    this -> unschedule(schedule_selector(CLogonLayer::mesDaojishi));
    m_sendMenu->setVisible(true);
    m_resendMenu->setVisible(false);
    m_labelDaojishi->setVisible(false);
    m_nDaojishi = 60;
}

void CLogonLayer::sendCheckHallData()
{
    CMainLogic::sharedMainLogic()->ShowMessage("正在连接网络", eMBExitGame);
    //发送检测包
    CMainLogic::sharedMainLogic()->m_bNeedLogon = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_pNetworkService->GetNetInfo(CMainLogic::sharedMainLogic()->m_nHallSocketHandle), MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#endif
	CMainLogic::sharedMainLogic()->openCheckLink();
}

void CLogonLayer::onEnterTransitionDidFinish()
{
    if(GameMainScene::_instance != NULL)
    {
        GameMainScene::_instance = NULL;
    }
    CMainLogic::sharedMainLogic()->freeMemory();
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3all/labanotice.plist");
    if(CMainLogic::sharedMainLogic()->m_bLinkHallEnter)
    {
        this -> scheduleOnce(schedule_selector(CLogonLayer::sendCheckHallData), 0.0f);
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CMainLogic::sharedMainLogic()->m_bExitGame = false;
#endif
	
    m_bClickMenu = true;
}
//恢复CCEditBox的光标位置
void CLogonLayer::animationFinishCallFunc(CCNode * pNode)
{
    m_pPasswordBox->setPosition(m_pPasswordBox->getPosition());
    m_pUserNameBox->setPosition(m_pUserNameBox->getPosition());
    m_pEditBoxNickName->setPosition(m_pEditBoxNickName->getPosition());
    m_pEditBoxAcc->setPosition(m_pEditBoxAcc->getPosition());
    m_pEditBoxEnsurePassword->setPosition(m_pEditBoxEnsurePassword->getPosition());
    m_pEditBoxPassword->setPosition(m_pEditBoxPassword->getPosition());
    m_pEditBoxTel->setPosition(m_pEditBoxTel->getPosition());
    m_pEditBoxMessage->setPosition(m_pEditBoxMessage->getPosition());
}

CLogonLayer::~CLogonLayer()
{
    CCArmatureDataManager::sharedArmatureDataManager()->purge();
    DictionaryHelper::shareHelper()->purgeDictionaryHelper();
}

void CLogonLayer::delayLogin(float ft)
{
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
    if (PluginChannel::getInstance()->getInitSuccess())
    {
        CMainLogic::sharedMainLogic()->m_nLogonType = eLogon;
        PluginChannel::getInstance()->login(CHANNEL_NUM);
        CCUserDefault::sharedUserDefault()->setBoolForKey("NewGuestBank", false);
        //sendLogonPacket();
        CMainLogic::sharedMainLogic()->ShowMessage("正在努力登入游戏......", eMBExitGame);
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage("初始化失败，请尝试再次登录！");
    }
#endif
}

void CLogonLayer::menuCallFunc(CCObject* obj)
{
    if(!m_bClickMenu)
        return;
    CCMenuItem* pTempItem = (CCMenuItem*)obj;
    if (NULL == pTempItem)
    {
        ERROR_CHECK;
        return;
    }
    CMainLogic::sharedMainLogic()->m_nClickBank = 0;
    CMainLogic::sharedMainLogic()->m_nMailCount = 0;
    CMainLogic::sharedMainLogic()->m_bHaveAdvert = false;
    CMainLogic::sharedMainLogic()->m_bClickIcon = false;
    CMainLogic::sharedMainLogic()->m_bEnterGiveCoin = false;
    CMainLogic::sharedMainLogic()->m_bGameToVIP = false;
    CMainLogic::sharedMainLogic()->m_bBYGameToSuper = false;
    CMainLogic::sharedMainLogic()->m_bGameChargeStatus = false;
    CMainLogic::sharedMainLogic()->m_nExitGameToSpecialArm = 0;
    CMainLogic::sharedMainLogic()->m_nShowChargeType = 0;
    CMainLogic::sharedMainLogic()->m_bBuyTryCannon = false;
    CMainLogic::sharedMainLogic()->m_nShowCoinArmSkill = 0;
    CMainLogic::sharedMainLogic()->m_bShowTryCannon = false;
    m_bLogon = true;
    CMainLogic::sharedMainLogic()->m_bLogonStatus = true;
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    int tempTag = pTempItem->getTag();
    switch (tempTag)
    {
        case eSelfLogonTag:
        {
            enterAccountNode(pTempItem);
            break;
        }
        case eAnySDKLogonTag:
        {
            this->scheduleOnce(schedule_selector(CLogonLayer::delayLogin), 0.1f);
            break;
        }
        case eAnySDKLogonSuccessTag:
        {
            CCUserDefault::sharedUserDefault()->setBoolForKey("NewGuestBank", false);
            sendLogonPacket();
            break;
        }
        case ebtLogon:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(0, eBtLogon);
            pTempItem ->runAction(CCSequence::create(CCScaleTo::create(0.1f, 1.2f), CCScaleTo::create(0.1f, 1.0f), NULL));
            std::string accStr = m_pUserNameBox->getText();
            if(accStr == "")
            {
                CMainLogic::sharedMainLogic()->ShowMessage("请输入账号", eMBOK);
                return;
            }
            std::string passwordStr = m_pPasswordBox->getText();
            if(passwordStr == "")
            {
                CMainLogic::sharedMainLogic()->ShowMessage("请输入密码", eMBOK);
                return;
            }
            
            CMainLogic::sharedMainLogic()->m_sPassword = m_pPasswordBox->getText();
            CMainLogic::sharedMainLogic()->m_sAccount = m_pUserNameBox->getText();
            CMainLogic::sharedMainLogic()->m_nLogonType = eLogon;
            //游客登录状态标记，用于给游客自动填充银行密码
            CCUserDefault::sharedUserDefault()->setBoolForKey("NewGuestBank", false);
            sendLogonPacket();
            break;
        }
        case ebtRegister://确定注册按钮
        {
            CMainLogic::sharedMainLogic()->m_nRegisterType = m_nRegisterType;
            pTempItem ->runAction(CCSequence::create(CCScaleTo::create(0.1f, 1.2f), CCScaleTo::create(0.1f, 1.0f), NULL));
            if(m_nRegisterType == 2)
            {
                std::string nameStr = m_pEditBoxNickName->getText();
                if(nameStr == "")
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("请输入昵称");
                    return;
                }
                std::string accountStr = m_pEditBoxAcc->getText();
                if(accountStr == "")
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("请输入账号");
                    return;
                }
                if(!efficacyNickName(nameStr.c_str()))
                {
                    return;
                }
                if(!efficacyAccounts(accountStr.c_str()))
                {
                    return;
                }
                CMainLogic::sharedMainLogic()->m_sAccount = accountStr;
                //服务端保存的都是GB2312字符
                nameStr = CCharCode::UTF8ToGB2312(nameStr.c_str());
                CMainLogic::sharedMainLogic()->m_sNickName = nameStr;//gb2312
                if(nameStr == "")
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("您输入的昵称不合法", eMBOK);
                    return;
                }

            }
            else if(m_nRegisterType == 1)//手机号注册
            {
                //验证手机号是否合法
                std::string telephoneStr = m_pTelBox->getText();
                const char* pChar = telephoneStr.c_str();
                bool bTelephone = true;
                //判断字符串是否符合手机号码
                if(telephoneStr.length() != 11)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("请输入有效的手机号", eMBOK);
                    return;
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
                            return;
                        }
                    }
                    else
                    {
                        CMainLogic::sharedMainLogic()->ShowMessage("请输入有效的手机号", eMBOK);
                        return;
                    }
                }
                std::string mesStr = m_pEditBoxMessage->getText();
                if(mesStr == "")
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("请您输入手机短信验证码。");
                    return;
                }
                std::string sNickName = m_pEditBoxNickName->getText();
                if(sNickName == "")
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("请输入昵称", eMBOK);
                    return;
                }
                CMainLogic::sharedMainLogic()->m_sAccount = telephoneStr;
                //服务端保存的都是GB2312字符
                telephoneStr = CCharCode::UTF8ToGB2312(sNickName.c_str());
                CMainLogic::sharedMainLogic()->m_sNickName = telephoneStr;//gb2312
                CMainLogic::sharedMainLogic()->m_sVerificationCode = mesStr;
            }
            std::string passwordStr = m_pEditBoxPassword->getText();
            if(passwordStr == "")
            {
                CMainLogic::sharedMainLogic()->ShowMessage("请输入密码");
                return;
            }
            std::string rePasswordStr = m_pEditBoxEnsurePassword->getText();
            if(rePasswordStr == "")
            {
                CMainLogic::sharedMainLogic()->ShowMessage("请再次输入密码");
                return;
            }
            if(strcmp(passwordStr.c_str(), rePasswordStr.c_str()))
            {
                CMainLogic::sharedMainLogic()->ShowMessage("两次输入密码不正确，请检查输入！");
                return;
            }
            
            CMainLogic::sharedMainLogic()->m_sPassword = passwordStr;
            
            CMainLogic::sharedMainLogic()->m_nLogonType = eRegister;
            
            CMainLogic::sharedMainLogic()->m_sTelephone = m_pTelBox->getText();
            
            //游客登录状态标记，用于给游客自动填充银行密码
            CCUserDefault::sharedUserDefault()->setBoolForKey("NewGuestBank", false);
            sendLogonPacket();
            break;
        }
        case ebtQuickLogon:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(0, eBtYoukeLogon);
            if (m_pUserNameBox->getText() != "" && m_pUserNameBox->getText() != CCUserDefault::sharedUserDefault()->getStringForKey("NewGuestAccount"))
            {
                CMainLogic::sharedMainLogic()->ShowMessage("使用游客账号登录会覆盖当前的账号和密码，是否继续？", eMBOKCancel, eGuestLogon);
                return;
            }
            constructGuestPacket();
            sendLogonPacket();
            break;
        }
    }
}

//检查合格返回为true
bool CLogonLayer::efficacyAccounts(const char* str)
{
    //变量定义
    int nCharLength=strlen(str);
    
    bool bHaveIllegal=false;
    bool bOnlyNumber=true;
    char temp[100];
    for (int i=0; i<nCharLength; i++)
    {
        TCHAR sTemp=str[i];
        if ((sTemp>='A' && sTemp<='Z')||(sTemp>='a' && sTemp<='z')||(sTemp=='_'))
        {
            bOnlyNumber=false;
        }
        else if (sTemp>='0' && sTemp<='9')
        {
        }
        else
        {
            bHaveIllegal=true;
        }
    }
    if (bHaveIllegal)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("游戏帐号包含非法字符，只能包含大小写字母、数字、下划线。", eMBOK);
        return false;
    }
    if (bOnlyNumber)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("游戏帐号不能是纯数字。", eMBOK);
        return false;
    }
    
    //长度判断
    if (nCharLength<LEN_LESS_ACCOUNTS)
    {
        //变量定义
        int nChinessCount=0;
        
        //计算字符
        for (int i=0;i<nCharLength;i++)
        {
            if (str[i]>=0x80)
            {
                nChinessCount++;
            }
        }
        
        //长度判断
        if ((nChinessCount+nCharLength)<LEN_LESS_ACCOUNTS)
        {
            sprintf(temp, "游戏帐号必须大于 %d 位以上，请重新输入！", LEN_LESS_ACCOUNTS);
            CMainLogic::sharedMainLogic()->ShowMessage(temp, eMBOK);
            return false;
        }
    }
    
    return true;
}

bool CLogonLayer::efficacyNickName(const char* str)
{
    bool bHageIllegal = false;
    int nCharLength = (int)strlen(str);
    
    //int nDataLength = nCharLength*sizeof(TCHAR);
    for(int i = 0; i < nCharLength; i++)
    {
        TCHAR sTemp = str[i];
        if((sTemp>='!' && sTemp<'0') || (sTemp>'9' && sTemp<'A')|| (sTemp>'Z' && sTemp<'a')|| (sTemp>'z' && sTemp<='~'))
        {
            bHageIllegal = true;
        }
    }
    if(bHageIllegal)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("游戏昵称包含非法字符，请重新输入!", eMBOK);
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
    if(strlen(str) < 6)
    {
        char temp[256];
        sprintf(temp, "游戏昵称必须大于%d个字符，请重新输入！", LEN_LESS_NICKNAME);
        CMainLogic::sharedMainLogic()->ShowMessage(temp, eMBOK);
        return false;
    }
    //长度判断
    if (strlen(str) > 21)
    {
        char temp[256];
        sprintf(temp, "您的游戏昵称超长，请重新输入！");
        CMainLogic::sharedMainLogic()->ShowMessage(temp, eMBOK);
        return false;
    }
    
    return true;
}

void CLogonLayer::constructGuestPacket()
{
//    CMainLogic::sharedMainLogic()->m_sAccount = "";
//    CMainLogic::sharedMainLogic()->m_sPassword = "";
//    CMainLogic::sharedMainLogic()->m_nLogonType = eGuestRegister;
    
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
    //游客登录状态标记，用于给游客自动填充银行密码
    CCUserDefault::sharedUserDefault()->setBoolForKey("NewGuestBank", true);
}

void getLogonUrl(DWORD dwBaseData, char szServerName[32])
{
    int nNumber[] = {
        456820, 134661, 789512, 456873, 324184, 652115, 326686, 215897, 123598, 121349
    };
    int nNodeIndex = dwBaseData % 10;
    sprintf(szServerName, "%d", nNumber[nNodeIndex]);
    std::string str = szServerName;
    str += '.';
    str += '6';
    str += '6';
    str += 'y';
    str += '.';
    str += 'c';
    str += 'o';
    str += 'm';
    sprintf(szServerName, "%s", str.c_str());
}

void CLogonLayer::sendLogonPacket()
{
    if (CMainLogic::sharedMainLogic()->m_sAccount=="" && CMainLogic::sharedMainLogic()->m_nLogonType!=eGuestRegister)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("帐号不能为空");
        return;
    }
    
    if (CMainLogic::sharedMainLogic()->m_sPassword=="" && CMainLogic::sharedMainLogic()->m_nLogonType!=eGuestRegister)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("密码不能为空");
        return;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string str = ChargeWebView::OnInternet();
    if(str == "")
    {
        //没有可用的网络
        CMainLogic::sharedMainLogic()->ShowMessage("当前没有可用的网络，请检查网络设置", eMBOK, eDisConnectNet);
        return;
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string str = CMainLogic::sharedMainLogic()->OnInternet();
    if(str == "2")//当前没有网络
    {
        CMainLogic::sharedMainLogic()->ShowMessage("当前没有可用的网络，请检查网络设置", eMBOK, eDisConnectNet);
        return;
    }
#endif
    
    if(CMainLogic::sharedMainLogic()->m_bForeGround)//发送检测包，同时启动3秒定时器
    {
        if(CMainLogic::sharedMainLogic()->m_pNetworkService != NULL)
        {
            CMainLogic::sharedMainLogic()->ShowMessage("正在检测服务器，请稍候", eMBExitGame);
            CMainLogic::sharedMainLogic()->m_nCheckSocketShut = 9;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_pNetworkService->GetNetInfo(CMainLogic::sharedMainLogic()->m_nHallSocketHandle), MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#endif
			this -> scheduleOnce(schedule_selector(CLogonLayer::receiveHallLink), 3.0f);
        }
    }
    else
    {
        sendLogonInfo();
    }
}

void CLogonLayer::receiveHallLink()
{
    if(CMainLogic::sharedMainLogic()->m_pNetworkService != NULL)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器，请稍候", eMBExitGame);
        CMainLogic::sharedMainLogic()->m_nClickBank = 9;
#if (CC_TARGET_PLATOFM == CC_PLATFORM_ANDROID)
        CNetInfo * pNetInfo = NULL;
        if (!NATIVE_TEST)
        {
            pNetInfo = CMainLogic::sharedMainLogic()->m_pNetworkService->Connect(CMainLogic::sharedMainLogic()->m_sNodeIP.c_str(), PORT_LOGON, NULL, NULL, true);
        }
        else
        {
            pNetInfo = CMainLogic::sharedMainLogic()->m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON, NULL, NULL, false);
        }
        CMainLogic::sharedMainLogic()->m_nHallSocketHandle = CMainLogic::sharedMainLogic()->m_pNetworkService->GetNetID(pNetInfo);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (!NATIVE_TEST)
        {
            CMainLogic::sharedMainLogic()->m_nHallSocketHandle = CMainLogic::sharedMainLogic()->m_pNetworkService->Connect(CMainLogic::sharedMainLogic()->m_sNodeIP.c_str(), PORT_LOGON);
        }
        else
        {
            CMainLogic::sharedMainLogic()->m_nHallSocketHandle = CMainLogic::sharedMainLogic()->m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON);
        }
#endif
        this -> scheduleOnce(schedule_selector(CLogonLayer::logonHallSchedule), 3.0f);
    }
}

void CLogonLayer::logonHallSchedule()
{
    CMainLogic::sharedMainLogic()->ShowMessage("无法连接服务器，请稍候再试", eMBOK);
}

void CLogonLayer::sendLogonInfo()
{
    CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器请稍候...",eMBExitGame);
    
    CMainLogic::sharedMainLogic()->OnLinkHall(CMainLogic::sharedMainLogic()->m_nHallSocketHandle);
    m_bLogonStatus = true;
    this -> scheduleOnce(schedule_selector(CLogonLayer::mesNetInfo), 5.0f);
}

void CLogonLayer::mesNetInfo()
{
    CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，您当前的网络环境很不稳定，请稍后重试", eMBOK);
}

void CLogonLayer::requestPublicIP()
{
    //检测版本
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://city.ip138.com/ip2city.asp");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setTag("http test");
    request -> setResponseCallback(this, httpresponse_selector(CLogonLayer::onHttpPublicIPComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CLogonLayer::onHttpPublicIPComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    std::string str1 = "";
    std::string str2 = "";
    /*
    int size = str.find("[");
    if(size >= 0)
    {
        str1 = str.substr(size+1);
        int size2 = str1.find("]");
        if(size2 >= 0)
        {
            str2 = str1.substr(0, size2);
            CMainLogic::sharedMainLogic()->m_sPublicIP = str2;
            //printf("CCUserDefault::sharedUserDefault()->getStringForKey is %s", CCUserDefault::sharedUserDefault()->getStringForKey("PUBLICIP").c_str());
            if(CCUserDefault::sharedUserDefault()->getStringForKey("PUBLICIP") == "")
            {
                CCUserDefault::sharedUserDefault()->setStringForKey("PUBLICIP", str2.c_str());
            }
        }
    }*/
}

void CLogonLayer::clickReportMenu(CCObject* object)
{
    CMainLogic::sharedMainLogic()->sendUserBehavior(0, eBtFaceBack);
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FMLayerWebView* pFMLayerWebView = FMLayerWebView::create();
    pFMLayerWebView -> setPosition(CCPointZero);
    this -> addChild(pFMLayerWebView, 300);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    char chargeURL[256];
    sprintf(chargeURL, "http://%s/Feedback.aspx?gameid=0&accname=0", NORMAL_URL);
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

void CLogonLayer::testCallFunc()
{
    //CMainLogic::sharedMainLogic()->ShowMessage(m_strMsg.c_str());
    //对str进行xml解析，将30条数据保存到m_vctRankTimes中
    TiXmlDocument* document = new TiXmlDocument();
    int len = strlen(m_strMsg.c_str());
    if(document->LoadFile2((unsigned char *)m_strMsg.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("uinfo"))
        {
            return;
        }
        TiXmlElement* pCurrentElement=rootElement->FirstChildElement("info");
        while(pCurrentElement != NULL)
        {
            std::string type=pCurrentElement->Attribute("accounts");
            if (type != "")
            {
                CMainLogic::sharedMainLogic()->m_sAccount = type;
                CCUserDefault::sharedUserDefault()->setStringForKey("AnySDKAccount", CMainLogic::sharedMainLogic()->m_sAccount.c_str());
            }
            std::string sLoginPsd=pCurrentElement->Attribute("plogin");
            if (sLoginPsd != "")
            {
                CMainLogic::sharedMainLogic()->m_sPassword = sLoginPsd;
            }
            std::string sBankPsd=pCurrentElement->Attribute("pbank");
            if (sBankPsd != "")
            {
                CCUserDefault::sharedUserDefault()->setStringForKey("NewGuestPassword", sBankPsd.c_str());
                CCUserDefault::sharedUserDefault()->setStringForKey("NewPassword", sBankPsd.c_str());
            }
            pCurrentElement=pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
    
    CCMenuItem* pAnySDKLogonItem = CCMenuItem::create();
    pAnySDKLogonItem->setTag(eAnySDKLogonSuccessTag);
    menuCallFunc(pAnySDKLogonItem);
    
    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
}

void CLogonLayer::showText(const char* sss)
{
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
    m_strMsg = sss;
    int nByte = 0;
    std::string str2 = ZBase64::Decode(m_strMsg.c_str(), (int)strlen(m_strMsg.c_str()), nByte);
    m_strMsg = str2;
    
    if (sss != NULL)
    {
        CCDelayTime* pDelay = CCDelayTime::create(1);
        CCCallFunc* pCallFunc = CCCallFunc::create(this, callfunc_selector(CLogonLayer::testCallFunc));
        this->runAction(CCSequence::create(pDelay, pCallFunc, NULL));
    }
#endif
}


