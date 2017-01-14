#include "header.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "FMLayerWebView.h"
#include "IOSiAP_Bridge.h"

const char* ChargeTypeIOSCMoney1[12] = {"1", "6", "12", "30", "50", "98", "188", "648", "18", "18", "18", "18"};

const char* ProductID1s[12] = {"com.youkuss.daka2coin1", "com.youkuss.daka2coin2", "com.youkuss.daka2coin3", "com.youkuss.daka2coin4",  "com.youkuss.daka2coin5",  "com.youkuss.daka2coin6", "com.youkuss.daka2coin7", "com.youkuss.daka2coin8", "com.youkuss.dakaa2", "com.youkuss.dakaa3", "com.youkuss.dakaa4", "com.youkuss.dakaa6"};

extern std::string orderIDStr;
extern std::string chargeTypeStr;
extern int nchargeRMB;
#endif

ChargeUI* ChargeUI::create(int nChargeCount, int nChargeType)
{
    ChargeUI* pRet = new ChargeUI();
    if(pRet && pRet->init(nChargeCount, nChargeType))
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

ChargeUI::~ChargeUI()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    delete bridge;
    bridge = NULL;
#endif
}

bool ChargeUI::init(int nChargeCount, int nChargeType)
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    m_nChargeCount = nChargeCount;
    m_nChargeType = nChargeType;
    m_nChargeOpenType = 0;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    for(int i = 0; i < 12; i++)
    {
        CMD_GPR_IAPProduct IAPProduct = {0};
        IAPProduct.wRMB = atoi(ChargeTypeIOSCMoney1[i]);
        IAPProduct.wType = 0;
        sprintf(IAPProduct.szProductID, "%s", ProductID1s[i]);
        m_IAPProductList.product[i] = IAPProduct;
    }
    bridge = new IOSiAP_Bridge(m_IAPProductList);
#endif
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this -> addChild(pLayerColor);
    
    loadUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void ChargeUI::loadUI()
{
    CCSprite* pChargeBg = CCSprite::create("charge/chargetypebg.png");
    pChargeBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(pChargeBg);
    CCMenuItemImage* closeItem = CCMenuItemImage::create("charge/closetypeItem.png", "charge/closetypeItem.png", this, menu_selector(ChargeUI::closeMenu));
    closeItem -> setPosition(ccp(pChargeBg->getContentSize().width-45, pChargeBg->getContentSize().height-35));
    closeItem -> setScale(1.2f);
    CCMenu* pCloseMenu = CCMenu::create(closeItem, NULL);
    pCloseMenu -> setPosition(CCPointZero);
    pCloseMenu -> setAnchorPoint(CCPointZero);
    pChargeBg -> addChild(pCloseMenu);
    pCloseMenu -> setTouchPriority(-205);
    
    CCMenuItemImage* wechatItem = CCMenuItemImage::create("charge/wechatitem.png", "charge/wechatitem.png", this, menu_selector(ChargeUI::clickMenu));
    wechatItem -> setTag(101);
    wechatItem -> setPosition(ccp(pChargeBg->getContentSize().width/3-45, pChargeBg->getContentSize().height/3*2-30));
    CCMenuItemImage* alipayItem = CCMenuItemImage::create("charge/alipayitem.png","charge/alipayitem.png", this, menu_selector(ChargeUI::clickMenu));
    alipayItem -> setTag(102);
    alipayItem -> setPosition(ccp(pChargeBg->getContentSize().width/3*2+45, pChargeBg->getContentSize().height/3*2-30));
    CCMenuItemImage* bankItem = CCMenuItemImage::create("charge/bankitem.png", "charge/bankitem.png", this, menu_selector(ChargeUI::clickMenu));
    bankItem -> setTag(103);
    bankItem -> setPosition(ccp(pChargeBg->getContentSize().width/3-45, pChargeBg->getContentSize().height/3-40));
    CCMenu* pMenu = CCMenu::create(wechatItem, alipayItem, bankItem, NULL);
    pMenu -> setPosition(CCPointZero);
    pMenu -> setAnchorPoint(CCPointZero);
    pChargeBg -> addChild(pMenu);
    pMenu -> setTouchPriority(-205);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(m_nChargeType == 0)//VIP升级和首充都不包含苹果内购
    {
        CCMenuItemImage* appleItem = CCMenuItemImage::create("charge/appleitem.png", "charge/appleitem.png", this, menu_selector(ChargeUI::clickMenu));
        appleItem -> setTag(104);
        appleItem -> setPosition(ccp(pChargeBg->getContentSize().width/3*2+45, pChargeBg->getContentSize().height/3-40));
        CCMenu* pAppleMenu = CCMenu::create(appleItem, NULL);
        pAppleMenu -> setPosition(CCPointZero);
        pAppleMenu -> setAnchorPoint(CCPointZero);
        pChargeBg -> addChild(pAppleMenu);
        pAppleMenu -> setTouchPriority(-205);
    }
#endif
    if(m_nChargeType == 3 || m_nChargeType == 2)//1元购，只显示微信和支付宝，10元限购
    {
        bankItem->setVisible(false);
    }
    
    pChargeBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void ChargeUI::clickMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case 101://微信
        {
            m_nChargeOpenType = 1;
            if(GameMainScene::_instance != NULL || GameLayer::_instance != NULL)
            {
                ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
                if(pChargeLayer != NULL)
                {
                    if(m_nChargeType == 3)//1元购
                    {
                        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eOneGameWXChargeTag);
                        pChargeLayer -> m_nOpenType = 2;
                    }
                    else if(m_nChargeType == 2)//10元限购
                    {
                        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eTenGameWXChargeTag);
                        pChargeLayer -> m_nOpenType = 10;
                    }
                    else
                    {
                        pChargeLayer -> m_nOpenType = 0;
                    }
                    CMainLogic::sharedMainLogic()->m_bClickMessageCharge = false;
                    pChargeLayer -> setCurrentSelectedType(101);
                    pChargeLayer -> selectedChargeTypeAndPrise(101, 0);
                    
                    
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameFirstChargeAlipay);
                }
                else//游戏中主动弹出（1元购或者10元特惠）
                {
                    CMainLogic::sharedMainLogic()->m_nBuyGameType = 1;//购买金币
                    if(GameMainScene::_instance != NULL)//在游戏中
                    {
                        CMainLogic::sharedMainLogic()->m_bGameChargeStatus = true;
                        if(GameMainScene::GetInstance()->getShowExitGame())
                        {
                            if(m_nChargeType == 3)//1元购
                            {
                                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eOneGameWXChargeTag);
                                CMainLogic::sharedMainLogic()->wxCharge(m_nChargeCount, 3, 0);
                            }
                            else if(m_nChargeType == 2)//10元限购
                            {
                                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eTenGameWXChargeTag);
                                CMainLogic::sharedMainLogic()->wxCharge(m_nChargeCount, 1, 0);
                            }
                            else
                            {
                                CMainLogic::sharedMainLogic()->wxCharge(m_nChargeCount, 0, m_nChargeType);
                            }
                        }
                        else
                        {
                            GameMainScene::_instance->exitGame();//离开座位
                        }
                        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameNoScoreAlipay);
                    }
                }
                
            }
            else
            {
                CMainLogic::sharedMainLogic()->m_bFirstChargeUIStatus = true;
                if(m_nChargeType == 3)//1元购
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eOneHallWXChargeTag);
                    CMainLogic::sharedMainLogic()->wxCharge(m_nChargeCount, 3, 0);
                }
                else if(m_nChargeType == 2)//10元限购
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eTenHallWXChargeTag);
                    CMainLogic::sharedMainLogic()->wxCharge(m_nChargeCount, 1, 0);
                }
                else
                {
                    CMainLogic::sharedMainLogic()->wxCharge(m_nChargeCount, 0, m_nChargeType);
                }
            }
            break;
        }
        case 102://支付宝
        {
            m_nChargeOpenType = 2;
            if(GameMainScene::_instance != NULL || GameLayer::_instance != NULL)
            {
                ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
                if(pChargeLayer != NULL)
                {
                    if(m_nChargeType == 3)//1元购
                    {
                        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eOneGameAlipayChargeTag);
                        pChargeLayer -> m_nOpenType = 2;
                    }
                    else if(m_nChargeType == 2)//10元限购
                    {
                        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eTenGameAlipayChargeTag);
                        pChargeLayer -> m_nOpenType = 10;
                    }
                    else
                    {
                        pChargeLayer -> m_nOpenType = 0;
                    }
                    CMainLogic::sharedMainLogic()->m_bClickMessageCharge = false;
                    pChargeLayer -> setCurrentSelectedType(100);
                    pChargeLayer -> selectedChargeTypeAndPrise(100, 0);
                    
                    
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameFirstChargeAlipay);
                }
                else//游戏中主动弹出（1元购或者10元特惠）
                {
                    CMainLogic::sharedMainLogic()->m_nBuyGameType = 1;//购买金币
                    if(GameMainScene::_instance != NULL)//在游戏中
                    {
                        CMainLogic::sharedMainLogic()->m_bGameChargeStatus = true;
                        if(GameMainScene::GetInstance()->getShowExitGame())
                        {
                            if(m_nChargeType == 3)//1元购
                            {
                                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eOneGameAlipayChargeTag);
                                CMainLogic::sharedMainLogic()->alipayCharge(m_nChargeCount, 3, 0);
                            }
                            else if(m_nChargeType == 2)//10元限购
                            {
                                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eTenGameAlipayChargeTag);
                                CMainLogic::sharedMainLogic()->alipayCharge(m_nChargeCount, 1, 0);
                            }
                            else
                            {
                                CMainLogic::sharedMainLogic()->alipayCharge(m_nChargeCount, 0, m_nChargeType);
                            }
                        }
                        else
                        {
                            GameMainScene::_instance->exitGame();//离开座位
                        }
                        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameNoScoreAlipay);
                    }
                }
            }
            else
            {
                CMainLogic::sharedMainLogic()->m_bFirstChargeUIStatus = true;
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallFirstChargeAlipay);
                if(m_nChargeType == 3)//1元购
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eOneHallAlipayChargeTag);
                    CMainLogic::sharedMainLogic()->alipayCharge(m_nChargeCount, 3, 0);
                }
                else if(m_nChargeType == 2)//10元限购
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eTenHallAlipayChargeTag);
                    CMainLogic::sharedMainLogic()->alipayCharge(m_nChargeCount, 1, 0);
                }
                else
                {
                    CMainLogic::sharedMainLogic()->alipayCharge(m_nChargeCount, 0, m_nChargeType);
                }
            }
            break;
        }
        case 103://银行卡
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            CMainLogic::sharedMainLogic()->m_bEnterService = true;
            FMLayerWebView* pLayerWebView = FMLayerWebView::create(0, CMainLogic::sharedMainLogic()->m_nUserID, CMainLogic::sharedMainLogic()->m_sAccount.c_str(), m_nChargeCount);
            pLayerWebView -> setPosition(CCPointZero);
            this -> addChild(pLayerWebView, 300);
            SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            CMainLogic::sharedMainLogic()->AndroidYeepay(m_nChargeCount, m_nChargeType);
#endif
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eChargeUIBank);
            break;
        }
        case 104://苹果内购
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            IOSCharge(m_nChargeCount);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eChargeUIIOSPay);
#endif
            break;
        }
        default:
            break;
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void ChargeUI::IOSCharge(int nChargeCount)
{
    if(orderIDStr != "")
    {
        CMainLogic::sharedMainLogic()->ShowMessage("请等待当前订单完成", eMBOK);
        return;
    }
    
    for(int i = 0; i < 8; i++)
    {
        if(m_IAPProductList.product[i].wRMB == nChargeCount)
        {
            //向后台发送未支付订单
            char tempOrder[32];
            sprintf(tempOrder, "IAP%ld%d", time(0), CMainLogic::sharedMainLogic()->m_nUserID);
            orderIDStr = tempOrder;
            char szMD5Result[128];
            char userID[128];
            sprintf(userID, "uid=%d&otn=%s&tfee=%d&time=%lu", CMainLogic::sharedMainLogic()->m_nUserID,tempOrder,m_IAPProductList.product[i].wRMB, time(0));
            CEncryptMD5::toString32(userID,szMD5Result);
            char temp[256];
            int nType = 1;
            if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
            {
                sprintf(temp, "http://lelepay.66y.com/Apple/AppUrl.aspx?uid=%d&otn=%s&tfee=%d&time=%lu&info=%s&ctype=%d", CMainLogic::sharedMainLogic()->m_nUserID,tempOrder,m_IAPProductList.product[i].wRMB, time(0), szMD5Result, nType);
            }
            else
            {
                sprintf(temp, "%s/Apple/AppUrl.aspx?uid=%d&otn=%s&tfee=%d&time=%lu&info=%s&ctype=%d", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str(), CMainLogic::sharedMainLogic()->m_nUserID,tempOrder,m_IAPProductList.product[i].wRMB, time(0), szMD5Result, nType);
            }
            
            chargeTypeStr = m_IAPProductList.product[i].szProductID;
            nchargeRMB = m_IAPProductList.product[i].wRMB;
            CCHttpRequest* request = new CCHttpRequest();
            request -> setUrl(temp);
            request -> setRequestType(CCHttpRequest::kHttpPost);
            request -> setResponseCallback(this, httpresponse_selector(ChargeUI::onHttpIAPComplete));
            CCHttpClient::getInstance()->send(request);
            CC_SAFE_RELEASE(request);
            request = NULL;
            CMainLogic::sharedMainLogic()->ShowMessage("正在获取订单", eMBNull);
            
            break;
        }
    }
}

void ChargeUI::onHttpIAPComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->HttpErrorHandle(response, eWXOrderErr);
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    if(!strcmp(str.c_str(), "success"))
    {
        CMainLogic::sharedMainLogic()->ShowMessage("正在请求支付", eMBIAPNull);
        char tempProductID[32];
        sprintf(tempProductID, "%s", chargeTypeStr.c_str());
        bridge->requestProducts(tempProductID);
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK, eChargeBackHall);
    }
}
#endif

void ChargeUI::closeMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    this -> removeFromParent();
}

void ChargeUI::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -204, true);
}

bool ChargeUI::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

int ChargeUI::getChargeType()//查看是否是VIP升级
{
    return m_nChargeType;
}

int ChargeUI::getChargeOpenType()
{
    return m_nChargeOpenType;
}
