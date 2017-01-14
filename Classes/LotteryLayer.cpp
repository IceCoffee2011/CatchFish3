#include "header.h"

LotteryLayer::~LotteryLayer()
{
    
}

LotteryLayer* LotteryLayer::create(bool bLocalOpen, int nLotteryNum)
{
    LotteryLayer* pRet = new LotteryLayer();
    if(pRet && pRet->init(bLocalOpen, nLotteryNum))
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

bool LotteryLayer::init(bool bLocalOpen, int nLotteryNum)
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_bLocalOpen = bLocalOpen;
    m_bRequestStatus = false;
    m_nCurrentMobileCount = 0;
    
    
    loadUI();
    if(m_bLocalOpen)
    {
        readLotteryConfig();
    }
    
    this -> setTouchEnabled(true);
    
    return true;
}

void LotteryLayer::loadUI()
{
    CCSprite* pBg = CCSprite::create("lottery/lotterybg.png");
    pBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(pBg);
    CCMenuItemImage* closeItem = CCMenuItemImage::create("lottery/closeItem.png", "lottery/closeItem.png", this, menu_selector(LotteryLayer::closeMenu));
    closeItem -> setPosition(ccp(CLIENT_WIDTH-closeItem->getContentSize().width/2, CLIENT_HEIGHT-closeItem->getContentSize().height/2));
    CCMenu* pCloseMenu = CCMenu::create(closeItem, NULL);
    pCloseMenu -> setPosition(CCPointZero);
    pCloseMenu -> setAnchorPoint(CCPointZero);
    pBg -> addChild(pCloseMenu);
    pCloseMenu -> setTouchPriority(-130);
    
    CCMenuItemImage* yidongItem = CCMenuItemImage::create("lottery/yidongItem.png", "lottery/yidongItem.png", this, menu_selector(LotteryLayer::clickExchangeType));
    yidongItem -> setTag(101);
    yidongItem -> setPosition(ccp(CLIENT_WIDTH/4-5, CLIENT_HEIGHT/4*3+45));
    CCMenuItemImage* liantongItem = CCMenuItemImage::create("lottery/liantongItem.png", "lottery/liantongItem.png", this, menu_selector(LotteryLayer::clickExchangeType));
    liantongItem -> setTag(102);
    liantongItem -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/4*3+45));
    CCMenuItemImage* dianxinItem = CCMenuItemImage::create("lottery/dianxinItem.png", "lottery/dianxinItem.png", this, menu_selector(LotteryLayer::clickExchangeType));
    dianxinItem -> setTag(103);
    dianxinItem -> setPosition(ccp(CLIENT_WIDTH/4*3+5, CLIENT_HEIGHT/4*3+45));
    CCMenu* pMenuType = CCMenu::create(yidongItem, liantongItem, dianxinItem, NULL);
    pMenuType -> setPosition(CCPointZero);
    pMenuType -> setAnchorPoint(CCPointZero);
    pBg -> addChild(pMenuType);
    pMenuType -> setTouchPriority(-130);
    m_spYidong = CCSprite::create("lottery/yidongSp.png");
    m_spYidong -> setPosition(ccp(CLIENT_WIDTH/4-5, CLIENT_HEIGHT/4*3+45));
    pBg -> addChild(m_spYidong);
    m_spLiantong = CCSprite::create("lottery/liantongSp.png");
    m_spLiantong -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/4*3+45));
    pBg -> addChild(m_spLiantong);
    m_spDianxin = CCSprite::create("lottery/dianxinSp.png");
    m_spDianxin -> setPosition(ccp(CLIENT_WIDTH/4*3+5, CLIENT_HEIGHT/4*3+45));
    pBg -> addChild(m_spDianxin);
    m_spLiantong -> setVisible(false);
    m_spDianxin -> setVisible(false);
    m_nSelectID = 1;
    m_nPriseName = 0;
    
    m_labelMobile = CCLabelAtlas::create("", "lottery/lotterymobilenum.png", 30, 54, '0');
    m_labelMobile -> setAnchorPoint(ccp(0, 0.5f));
    m_labelMobile -> setPosition(ccp(CLIENT_WIDTH/4*3-70, CLIENT_HEIGHT/3*2-8));
    pBg -> addChild(m_labelMobile);
    m_labelMobileInfo = CCLabelTTF::create("正在查询", "Arial", 32);
    m_labelMobileInfo -> setPosition(ccp(CLIENT_WIDTH/4*3-70, CLIENT_HEIGHT/3*2-8));
    m_labelMobileInfo -> setAnchorPoint(ccp(0, 0.5f));
    pBg -> addChild(m_labelMobileInfo);
    
    m_nodeYidong = CCNode::create();
    this -> addChild(m_nodeYidong);
    m_nodeLiantong = CCNode::create();
    this -> addChild(m_nodeLiantong);
    m_nodeLiantong -> setVisible(false);
    m_nodeDianxin = CCNode::create();
    this -> addChild(m_nodeDianxin);
    m_nodeDianxin -> setVisible(false);
    
    m_spSelect = CCSprite::create("lottery/lotterySelect.png");
    m_spSelect -> setPosition(ccp(CLIENT_WIDTH/4-5, CLIENT_HEIGHT/4*3+45));
    pBg -> addChild(m_spSelect);
    
    
    
    CMD_GPR_MobileCount MobileCount = {0};
    MobileCount.dwUserId = CMainLogic::sharedMainLogic()->m_nUserID;
    CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_MOBILE_COUNT, &MobileCount,sizeof(MobileCount));
}

void LotteryLayer::readLotteryConfig()
{
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + std::string("ExchangeShopConfig.xml");
    TiXmlDocument* document = new TiXmlDocument(xmlPath.c_str());
    if(document != NULL)
    {
        document -> LoadFile();
        
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement != NULL)
        {
            std::string nodename=rootElement->Value();
            if (0!=nodename.compare("exchangeShop"))
            {
                delete document;
                return;
            }
            TiXmlElement* firstElement = rootElement->FirstChildElement();
            std::string firstValue = firstElement->Value();
            if(firstValue == "telCards")
            {
                TiXmlElement* pCurrentElement = firstElement->FirstChildElement();
                while (pCurrentElement != NULL)
                {
                    std::string telCardValue = pCurrentElement->Value();
                    if(telCardValue == "telCard")
                    {
                        TelCardConfig* pTelCardConfig = new TelCardConfig;
                        m_vctTelCard.push_back(pTelCardConfig);
                        const char* prize = pCurrentElement->Attribute("prize");
                        pTelCardConfig->nPrize = atoi(prize);
                        const char* lotteryCount = pCurrentElement->Attribute("lotteryCount");
                        pTelCardConfig->nLotteryCount = atoi(lotteryCount);
                        const char* cardType = pCurrentElement->Attribute("cardType");
                        pTelCardConfig->nCardType = atoi(cardType);
                        const char* sID = pCurrentElement->Attribute("id");
                        pTelCardConfig->nID = atoi(sID);
                    }
                    pCurrentElement = pCurrentElement->NextSiblingElement();
                }
            }
        }
    }
    delete document;
    
    loadUILotteryBg((int)m_vctTelCard.size());
}

void LotteryLayer::parseData(std::string str)
{
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("exchangeShop"))
        {
            delete document;
            return;
        }
        TiXmlElement* firstElement = rootElement->FirstChildElement();
        std::string firstValue = firstElement->Value();
        if(firstValue == "telCards")
        {
            TiXmlElement* pCurrentElement = firstElement->FirstChildElement();
            while (pCurrentElement != NULL)
            {
                std::string telCardValue = pCurrentElement->Value();
                if(telCardValue == "telCard")
                {
                    TelCardConfig* pTelCardConfig = new TelCardConfig;
                    m_vctTelCard.push_back(pTelCardConfig);
                    const char* prize = pCurrentElement->Attribute("prize");
                    pTelCardConfig->nPrize = atoi(prize);
                    const char* lotteryCount = pCurrentElement->Attribute("lotteryCount");
                    pTelCardConfig->nLotteryCount = atoi(lotteryCount);
                    const char* cardType = pCurrentElement->Attribute("cardType");
                    pTelCardConfig->nCardType = atoi(cardType);
                    const char* sID = pCurrentElement->Attribute("id");
                    pTelCardConfig->nID = atoi(sID);
                }
                pCurrentElement = pCurrentElement->NextSiblingElement();
            }
        }
    }
    delete document;
    
    loadUILotteryBg((int)m_vctTelCard.size());
    
    //保存文件
    saveFile(str);
}

void LotteryLayer::saveFile(std::string str)
{
    //在Documents目录下创建一个可读写的文件
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + std::string("ExchangeShopConfig.xml");
    const char* buf = str.c_str();
    FILE* file = fopen(xmlPath.c_str(), "w");
    if(file)
    {
        fputs(buf, file);
        fclose(file);
        CCLog("save succeed");
    }
    else
    {
        CCLog("save file error");
    }
}

void LotteryLayer::loadUILotteryBg(int nCount)
{
    for(int i = 0; i < nCount; i++)
    {
        if(m_vctTelCard[i]->nCardType == 1)//移动
        {
            CCSprite* pTypeBg = CCSprite::create("lottery/lotterytypebg.png");
            pTypeBg -> setPosition(ccp(CLIENT_WIDTH/4*(1+i%3)-(i%3-1)*10, CLIENT_HEIGHT/3+20));
            m_nodeYidong -> addChild(pTypeBg);
            CCMenuItemImage* duihuanItem = CCMenuItemImage::create("lottery/duihuanItem.png", "lottery/duihuanItem.png", this, menu_selector(LotteryLayer::sendExchangeData));
            duihuanItem -> setPosition(ccp(pTypeBg->getContentSize().width/2, duihuanItem->getContentSize().height-10));
            CCMenu* pMenu = CCMenu::create(duihuanItem, NULL);
            pMenu -> setPosition(CCPointZero);
            pMenu -> setAnchorPoint(CCPointZero);
            pTypeBg -> addChild(pMenu);
            pMenu -> setTouchPriority(-130);
            duihuanItem -> setTag(201+i);
            char temp[32];
            sprintf(temp, "%d话费券", m_vctTelCard[i]->nLotteryCount);
            CCLabelTTF* pLabelTitle = CCLabelTTF::create(temp, "Arial-BoldMT", 36);
            pLabelTitle -> setPosition(ccp(pTypeBg->getContentSize().width/2, pTypeBg->getContentSize().height-30));
            pTypeBg -> addChild(pLabelTitle);
            sprintf(temp, "%d", m_vctTelCard[i]->nPrize);
            CCLabelTTF* pLabelNum = CCLabelTTF::create(temp, "Arial-BoldMT", 44);
            pLabelNum -> setAnchorPoint(ccp(0.5, 0.5));
            pLabelNum -> setPosition(ccp(pTypeBg->getContentSize().width/2+10, pTypeBg->getContentSize().height/2+8));
            pLabelNum->setRotation(15);
            pTypeBg->addChild(pLabelNum);
            
        }
        else if(m_vctTelCard[i]->nCardType == 2)//联通
        {
            CCSprite* pTypeBg = CCSprite::create("lottery/lotterytypebg.png");
            pTypeBg -> setPosition(ccp(CLIENT_WIDTH/4*(1+i%3)-(i%3-1)*10, CLIENT_HEIGHT/3+20));
            m_nodeLiantong -> addChild(pTypeBg);
            CCMenuItemImage* duihuanItem = CCMenuItemImage::create("lottery/duihuanItem.png", "lottery/duihuanItem.png", this, menu_selector(LotteryLayer::sendExchangeData));
            duihuanItem -> setPosition(ccp(pTypeBg->getContentSize().width/2, duihuanItem->getContentSize().height-10));
            CCMenu* pMenu = CCMenu::create(duihuanItem, NULL);
            pMenu -> setPosition(CCPointZero);
            pMenu -> setAnchorPoint(CCPointZero);
            pTypeBg -> addChild(pMenu);
            pMenu -> setTouchPriority(-130);
            duihuanItem -> setTag(201+i);
            char temp[32];
            sprintf(temp, "%d话费券", m_vctTelCard[i]->nLotteryCount);
            CCLabelTTF* pLabelTitle = CCLabelTTF::create(temp, "Arial-BoldMT", 36);
            pLabelTitle -> setPosition(ccp(pTypeBg->getContentSize().width/2, pTypeBg->getContentSize().height-30));
            pTypeBg -> addChild(pLabelTitle);
            sprintf(temp, "%d", m_vctTelCard[i]->nPrize);
            CCLabelTTF* pLabelNum = CCLabelTTF::create(temp, "Arial-BoldMT", 44);
            pLabelNum -> setAnchorPoint(ccp(0.5, 0.5));
            pLabelNum -> setPosition(ccp(pTypeBg->getContentSize().width/2+10, pTypeBg->getContentSize().height/2+8));
            pLabelNum->setRotation(15);
            pTypeBg->addChild(pLabelNum);
        }
        else if(m_vctTelCard[i]->nCardType == 3)//电信
        {
            CCSprite* pTypeBg = CCSprite::create("lottery/lotterytypebg.png");
            pTypeBg -> setPosition(ccp(CLIENT_WIDTH/4*(1+i%3)-(i%3-1)*10, CLIENT_HEIGHT/3+20));
            m_nodeDianxin -> addChild(pTypeBg);
            CCMenuItemImage* duihuanItem = CCMenuItemImage::create("lottery/duihuanItem.png", "lottery/duihuanItem.png", this, menu_selector(LotteryLayer::sendExchangeData));
            duihuanItem -> setPosition(ccp(pTypeBg->getContentSize().width/2, duihuanItem->getContentSize().height-10));
            CCMenu* pMenu = CCMenu::create(duihuanItem, NULL);
            pMenu -> setPosition(CCPointZero);
            pMenu -> setAnchorPoint(CCPointZero);
            pTypeBg -> addChild(pMenu);
            pMenu -> setTouchPriority(-130);
            duihuanItem -> setTag(201+i);
            char temp[32];
            sprintf(temp, "%d话费券", m_vctTelCard[i]->nLotteryCount);
            CCLabelTTF* pLabelTitle = CCLabelTTF::create(temp, "Arial-BoldMT", 36);
            pLabelTitle -> setPosition(ccp(pTypeBg->getContentSize().width/2, pTypeBg->getContentSize().height-30));
            pTypeBg -> addChild(pLabelTitle);
            sprintf(temp, "%d", m_vctTelCard[i]->nPrize);
            CCLabelTTF* pLabelNum = CCLabelTTF::create(temp, "Arial-BoldMT", 44);
            pLabelNum -> setAnchorPoint(ccp(0.5, 0.5));
            pLabelNum -> setPosition(ccp(pTypeBg->getContentSize().width/2+10, pTypeBg->getContentSize().height/2+8));
            pLabelNum->setRotation(15);
            pTypeBg->addChild(pLabelNum);
        }
    }
}

void LotteryLayer::sendExchangeData(CCObject* object)
{
    if(!m_bRequestStatus)
    {
        return ;
    }
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    if(item != NULL)
    {
        int nTag = item->getTag()-201;
        for(int i = 0; i < m_vctTelCard.size(); i++)
        {
            if(nTag == i)
            {
                if(m_vctTelCard[i]->nLotteryCount <= m_nCurrentMobileCount)
                {
                    m_nPriseName = m_vctTelCard[i]->nPrize;
                    sendUserInfo();
                }
                else
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，您当前的话费券数量不足", eMBOK);
                }
            }
        }
    }
}

void LotteryLayer::sendUserInfo()
{
    int nUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    char temp[256];
    sprintf(temp, "http://%s/Getexchange.aspx?type=1&uid=%d&uadress=%s&utel=%s&uitem=%d&uname=%s&cardtype=%d",NORMAL_URL, nUserID, "", "",m_nPriseName, "", m_nSelectID);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(LotteryLayer::OnHttpExchangeLotteryComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
    //正在发送兑换请求，请稍候
    CMainLogic::sharedMainLogic()->ShowMessage("正在发送兑换请求，请稍候", eMBNull);
    if (m_nSelectID == 1)
    {
        if (m_nPriseName == 30)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallLotteryToThirtyCMCC);
        }
        else if (m_nPriseName == 50)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallLotteryToFiftyCMCC);
        }
        else if (m_nPriseName == 100)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallLotteryToHundredCMCC);
        }
    }
    else if (m_nSelectID == 2)
    {
        if (m_nPriseName == 30)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallLotteryToThirtyCUCC);
        }
        else if (m_nPriseName == 50)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallLotteryToFiftyCUCC);
        }
        else if (m_nPriseName == 100)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallLotteryToHundredCUCC);
        }
    }
    else if (m_nSelectID == 3)
    {
        if (m_nPriseName == 30)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallLotteryToThirtyCTCC);
        }
        else if (m_nPriseName == 50)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallLotteryToFiftyCTCC);
        }
        else if (m_nPriseName == 100)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallLotteryToHundredCTCC);
        }
    }
}

void LotteryLayer::OnHttpExchangeLotteryComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    int nLottery = 0;
    //解析xml文件，显示结果，并且更新大厅界面和奖券界面上的奖券数量
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("userExchange"))
        {
            CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次兑换失败了，请稍后再试一次。", eMBOK);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, ehallLotteryDuiHuanFail);
            return;
        }
        std::string type=rootElement->Attribute("value");
        if(!std::strcmp(type.c_str(), "0"))//兑换失败
        {
            std::string sReason = rootElement->GetText();
            CMainLogic::sharedMainLogic()->ShowMessage(sReason.c_str(), eMBOK);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, ehallLotteryDuiHuanFail);
        }
        else if(!std::strcmp(type.c_str(), "1"))
        {
            TiXmlElement* pFirstElement = rootElement->FirstChildElement();
            if(pFirstElement != NULL)
            {
                std::string firstValue = pFirstElement->Attribute("lotteryCount");
                nLottery = atoi(firstValue.c_str());
                std::string sResult = pFirstElement->GetText();
                
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallLotteryDuiHuanSuccess);
                setLabelMoney(nLottery);
                if(CMainLogic::sharedMainLogic()->m_bCanBandingTel)
                {
                    if(CCUserDefault::sharedUserDefault()->getStringForKey("BindingTel", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"))
                    {
                        CCUserDefault::sharedUserDefault()->setStringForKey("BindingTel", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"));
                        CMainLogic::sharedMainLogic()->ShowMessage(sResult.c_str(), eMBOK, eShowMobileBinding);
                    }
                    else
                    {
                        CMainLogic::sharedMainLogic()->ShowMessage(sResult.c_str(), eMBOK);
                    }
                }
                else
                {
                    CMainLogic::sharedMainLogic()->ShowMessage(sResult.c_str(), eMBOK);
                }
                //向服务端发送公告
                CMD_GPR_ExchangeMobile ExchangeMobile = {0};
                ExchangeMobile.nExchangeType = m_nSelectID;
                ExchangeMobile.nExchangeCount = m_nPriseName;
                strncpy(ExchangeMobile.szNickName, CCharCode::UTF8ToGB2312(CMainLogic::sharedMainLogic()->m_sNickName.c_str()).c_str(), sizeof(ExchangeMobile.szNickName));
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_NOTICE, SUB_GPR_EXCHANGE_MOBILE, &ExchangeMobile,sizeof(ExchangeMobile));
            }
        }
    }
    
    delete document;
}

void LotteryLayer::clickExchangeType(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    if(item != NULL)
    {
        switch (item->getTag()) {
            case 101:
            {
                if(m_nSelectID != 1)
                {
                    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                }
                m_nSelectID = 1;
                m_spDianxin -> setVisible(false);
                m_spLiantong -> setVisible(false);
                m_spYidong -> setVisible(true);
                m_nodeYidong -> setVisible(true);
                m_nodeLiantong -> setVisible(false);
                m_nodeDianxin -> setVisible(false);
                m_spSelect -> setPosition(ccp(CLIENT_WIDTH/4-5, CLIENT_HEIGHT/4*3+45));
                break;
            }
            case 102:
            {
                if(m_nSelectID != 2)
                {
                    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                }
                m_nSelectID = 2;
                m_spLiantong -> setVisible(true);
                m_spYidong -> setVisible(false);
                m_spDianxin -> setVisible(false);
                m_nodeLiantong -> setVisible(true);
                m_nodeYidong -> setVisible(false);
                m_nodeDianxin -> setVisible(false);
                m_spSelect -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/4*3+45));
                break;
            }
            case 103:
            {
                if(m_nSelectID != 3)
                {
                    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                }
                m_nSelectID = 3;
                m_spDianxin -> setVisible(true);
                m_spYidong -> setVisible(false);
                m_spLiantong -> setVisible(false);
                m_nodeDianxin -> setVisible(true);
                m_nodeYidong -> setVisible(false);
                m_nodeLiantong -> setVisible(false);
                m_spSelect -> setPosition(ccp(CLIENT_WIDTH/4*3+5, CLIENT_HEIGHT/4*3+45));
                break;
            }
            default:
                break;
        }
    }
}

void LotteryLayer::setLabelMoney(int nCount)
{
    m_nCurrentMobileCount = nCount;
    m_bRequestStatus = true;
    char temp[16];
    sprintf(temp, "%d", nCount);
    m_labelMobile -> setString(temp);
    m_labelMobileInfo -> setVisible(false);
    
}

void LotteryLayer::closeMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    this -> removeFromParent();
    if(GameMainScene::_instance != NULL)
    {
        GameMainScene::GetInstance()->setWndHave(false);
        GameMainScene::GetInstance()->setAutoFire();
        GameMainScene::GetInstance()->setMatchShowStatus(true);
    }
}

void LotteryLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool LotteryLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}
