//
//  IOSiAP_Bridge.cpp
//  LongChengDaRen
//
//  Created by 白白 on 14-11-11.
//
//

//#include "IOSiAP_Bridge.h"
//#include "CommonInclude.h"
//#include "UpdateCoins.h"
#include "header.h"
USING_NS_CC;

extern std::string orderIDStr;

std::string transactionReceiptStr="";
unsigned long transactionReceiptLength = 0;

IOSiAP_Bridge::IOSiAP_Bridge(CMD_GPR_IAPProductList IAPProductList)
{
    iap = new IOSiAP();
    iap->delegate = this;
    memcpy(&m_IAPProductList, &IAPProductList, sizeof(CMD_GPR_IAPProductList));
}

IOSiAP_Bridge::~IOSiAP_Bridge()
{
    delete iap;
}

void IOSiAP_Bridge:: requestProducts(std::string id)
{
    productID = id;
    std::vector<std::string> product;
    for(int i = 0; i < 12; i++)
    {
        product.push_back(m_IAPProductList.product[i].szProductID);
    }
     
    iap->requestProducts(product);
}

void IOSiAP_Bridge::onRequestProductsFinish(void)
{
    std::string identifier = "";
    
    for(int i = 0; i < 12; i++)
    {
        if(m_IAPProductList.product[i].szProductID == productID)
        {
            identifier = m_IAPProductList.product[i].szProductID;
            break;
        }
    }
    
    //必须在onRequestProductsFinish后才能去请求iAP产品数据。
    IOSProduct *product = iap->iOSProductByIdentifier(identifier);
    // 然后可以发起付款请求。,第一个参数是由iOSProductByIdentifier获取的IOSProduct实例，第二个参数是购买数量
    iap->paymentWithProduct(product, 1);
}

void IOSiAP_Bridge::onRequestProductsError(int code)
{
    //这里requestProducts出错了，不能进行后面的所有操作。
    CCLOG("付款失败");
}

void IOSiAP_Bridge::onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity)
{
    if (event == IOSIAP_PAYMENT_PURCHAED) {
        //付款成功了，可以吧金币发给玩家了。
		//根据传入的参数就能知道购买的是哪种类型的金币
        char temp[128];
        if(CMainLogic::sharedMainLogic()->m_nBuyCannonIndex == 0)//金币
        {
            printf("2222productID is %s\n", productID.c_str());
            for(int i = 0; i < 8; i++)
            {
                if(productID == m_IAPProductList.product[i].szProductID)
                {
                    if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
                    {
                        sprintf(temp, "http://lelepay.66y.com/Apple/AppNotify.aspx?ordID=%s", orderIDStr.c_str());
                    }
                    else
                    {
                        sprintf(temp, "%s/Apple/AppNotify.aspx?ordID=%s", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str(), orderIDStr.c_str());
                    }
                    break;
                }
                
            }
        }
        else//武器
        {
            printf("1111productID is %s\n", productID.c_str());
            if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
            {
                sprintf(temp, "http://lelepay.66y.com/skpay/Apple/AppNotify.aspx?ordID=%s", orderIDStr.c_str());
            }
            else
            {
                sprintf(temp, "%s/skpay/Apple/AppNotify.aspx?ordID=%s", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str(), orderIDStr.c_str());
            }
        }
        char pBuffer[4096];
        sprintf(pBuffer, "%s", transactionReceiptStr.c_str());
        CCHttpRequest* request = new CCHttpRequest();
        request -> setUrl(temp);
        request -> setRequestType(CCHttpRequest::kHttpPost);
        request -> setResponseCallback(this, httpresponse_selector(IOSiAP_Bridge::onHttpChargeComplete));
        request -> setRequestData(pBuffer, (int)strlen(pBuffer));
        CCHttpClient::getInstance()->send(request);
        CC_SAFE_RELEASE(request);
        request = NULL;
        CMainLogic::sharedMainLogic()->ShowMessage("正在验证充值结果", eMBNull);
        orderIDStr = "";
        
    }
    else if(event == IOSIAP_PAYMENT_FAILED)
    {
        orderIDStr = "";
        CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
        if(GameMainScene::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->m_bGameChargeStatus = false;
            CMainLogic::sharedMainLogic()->m_nBuyCannon2Index = 0;
            CMainLogic::sharedMainLogic()->m_nBuyCannonIndex = 0;
            CMainLogic::sharedMainLogic()->switchLayer(eHallLayer);
        }
    }
}

void IOSiAP_Bridge::onHttpChargeComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    printf("str is %s\n", str.c_str());
    
    //获取邮件
    CMainLogic::sharedMainLogic()->requestIAPMail();
    
    
    bool bVIPUpgrade = false;
    std::string sInfo = "";
    bool bChargeResult = false;
    
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement != NULL)
        {
            TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
            while (pCurrentElement != NULL)
            {
                std::string nodename=pCurrentElement->Value();
                if (nodename == "text")
                {
                    sInfo = pCurrentElement->GetText();
                }
                else if(nodename == "Score")
                {
                    if(CMainLogic::sharedMainLogic()->m_lUserScore != atoll(pCurrentElement->GetText()))
                    {
                        bChargeResult = true;
                    }
                    //更新当前分数
                    CMainLogic::sharedMainLogic()->m_lUserScore = atoll(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setUserScore(CMainLogic::sharedMainLogic()->m_lUserScore);
                        }
                    }
                }
                else if(nodename == "VipLevel")//更新当前VIP等级
                {
                    int nLevel = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_nVipLevel != nLevel)
                    {
                        if(CMainLogic::sharedMainLogic()->m_nVipLevel == 0)//以前VIP等级为0的话，那么现在肯定升级了VIP，所以可以VIP签到
                        {
                            CMainLogic::sharedMainLogic()->m_bReceiveVipLogon = false;
                            //将大厅内的免费金币按钮上的心形展示出来
                            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                            {
                                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                                if(pHallLayer != NULL)
                                {
                                    pHallLayer -> setNickNameColor();
                                    pHallLayer->showfreecoinAction(true);
                                    if(nLevel == 9)
                                    {
                                        pHallLayer->m_pVipMenu->setVisible(false);
                                    }
                                }
                            }
                        }
                        CMainLogic::sharedMainLogic()->m_nVipLevel = nLevel;
                        bVIPUpgrade = true;
                    }
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setVipExp();
                        }
                    }
                }
                else if(nodename == "VipExp")//更新当前经验值
                {
                    CMainLogic::sharedMainLogic()->m_nVipExp = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setVipExp();
                        }
                    }
                }
                else if(nodename == "UserLet")//更新当前奖券
                {
                    CMainLogic::sharedMainLogic()->m_nLottery = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setTicket(CMainLogic::sharedMainLogic()->m_nLottery);
                        }
                    }
                }
                else if(nodename == "ChargeBack")//更新充值返利
                {
                    CMainLogic::sharedMainLogic()->m_nChargeBack = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_nChargeBack!=0)
                    {
                        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                pHallLayer->showVIPAction(true);
                            }
                        }
                    }
                }
                else if(nodename == "Fch")
                {
                    int nFirstCharge = atoi(pCurrentElement->GetText());
                }
                else if(nodename == "Count")//更新当前奖券数量
                {
                    CMainLogic::sharedMainLogic()->m_nLottery = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setTicket(CMainLogic::sharedMainLogic()->m_nLottery);
                        }
                    }
                }
                
                pCurrentElement = pCurrentElement->NextSiblingElement();
            }
        }
    }
    delete document;
    
    if(GameMainScene::_instance != NULL)//如果在游戏中购买，不显示VIP升级界面
    {
        CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK, eExitGameToHall);
    }
    else
    {
        if(!bVIPUpgrade)
        {
            if(CMainLogic::sharedMainLogic()->m_bCanBandingTel)
            {
                if(CCUserDefault::sharedUserDefault()->getStringForKey("BindingTel", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"))
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("BindingTel", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"));
                    CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK, eShowBindingTel);
                }
                else
                {
                    CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK);
                }
            }
            else
            {
                CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK);
            }
        }
        else
        {
            CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK, eShowVipUpgrade);
        }
    }
    if(bChargeResult)
    {
        //播放特效，判断登录奖励是否领取，如果已经领取了，那么2s后就将此框删除
        SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
            }
        }
    }
    ChargeUI* pChargeUi = (ChargeUI*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20003);
    if(pChargeUi != NULL)
    {
        pChargeUi -> removeFromParent();
    }
}

void IOSiAP_Bridge::requestMail()
{
    if(CMainLogic::sharedMainLogic()->m_pNetworkService != NULL)
    {
        CMainLogic::sharedMainLogic()->m_nCheckSocketShut = 15;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
 		CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_pNetworkService->GetNetInfo(CMainLogic::sharedMainLogic()->m_nHallSocketHandle), MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
        
#endif
		//this->scheduleOnce(schedule_selector(IOSiAP_Bridge::receiveHallLink2), 1.0f);
        CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
        pScheduler->scheduleSelector(schedule_selector(IOSiAP_Bridge::receiveHallLink2), this, 1.0f, 0, 0, false);
    }
}

void IOSiAP_Bridge::receiveHallLink2(float dt)
{
    if(CMainLogic::sharedMainLogic()->m_pNetworkService != NULL)
    {
        CMainLogic::sharedMainLogic()->m_nClickBank = 15;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
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
	}
}
