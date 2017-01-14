#include "header.h"

const char* WufuCardAn[5] = {"h2all/wufuan1.png", "h2all/wufuan2.png", "h2all/wufuan3.png", "h2all/wufuan4.png", "h2all/wufuan5.png"};
const char* WufuCardLiang[5] = {"h2all/wufuliang1.png", "h2all/wufuliang2.png", "h2all/wufuliang3.png", "h2all/wufuliang4.png", "h2all/wufuliang5.png"};

WufuLayer::WufuLayer()
{
    
}

WufuLayer::~WufuLayer()
{
    m_arrAnCard->release();
    m_arrLiangCard->release();
}

WufuLayer* WufuLayer::create(int nCard[5])
{
    WufuLayer* pRet = new WufuLayer();
    if(pRet && pRet->init(nCard))
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

bool WufuLayer::init(int nCard[5])
{
    if(!CCLayer::init())
    {
        return false;
    }
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    m_arrLiangCard = CCArray::create();
    m_arrLiangCard -> retain();
    m_arrAnCard = CCArray::create();
    m_arrAnCard -> retain();
    for(int i = 0; i < 5; i++)
    {
        m_nCard[i] = nCard[i];
    }
    m_nCurrentPage = 1;
    m_nType = 0;
    m_vctWufuRule.clear();
    m_vctVIPNotice.clear();
    
    loadUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void WufuLayer::loadUI()
{
    m_pWufuBg = CCSprite::create("h2all/wufubg.png");
    m_pWufuBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_pWufuBg);
    
    //五福卡
    char temp[8];
    for(int i = 0; i < 5; i++)
    {
        CCSprite* sprite1 = CCSprite::create(WufuCardAn[i]);
        sprite1 -> setTag(201+i);
        sprite1 -> setPosition(ccp(125+187*i, 356));
        m_pWufuBg -> addChild(sprite1);
        CCSprite* sprite2 = CCSprite::create(WufuCardLiang[i]);
        sprite2 -> setTag(301+i);
        sprite2 -> setPosition(ccp(125+187*i, 356));
        m_pWufuBg -> addChild(sprite2);
        m_arrAnCard->addObject(sprite1);
        m_arrLiangCard->addObject(sprite2);
        sprite2 -> setVisible(false);
        sprintf(temp, "%d", m_nCard[i]);
        CCLabelTTF* pLabel1 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
        pLabel1 -> setPosition(ccp(sprite1->getContentSize().width/2+10, 30));
        sprite1->addChild(pLabel1);
        pLabel1 -> setTag(101+i);
        CCLabelTTF* pLabel2 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
        pLabel2 -> setPosition(ccp(sprite2->getContentSize().width/2+10, 30));
        sprite2 -> addChild(pLabel2);
        pLabel2 -> setTag(101+i);
        if(m_nCard[i] > 0)
        {
            sprite2 -> setVisible(true);
            sprite1 -> setVisible(false);
        }
//        CCSprite* firstSp = CCSprite::create();
//        firstSp -> setPosition(ccp(sprite2->getContentSize().width/2, sprite2->getContentSize().height-30));
//        sprite2 -> addChild(firstSp);
//        CCArray* firstChargeArr = CCArray::create();
//        char temp2[32];
//        for(int i = 1;i <= 13; i++)
//        {
//            sprintf(temp2, "hallfirstcharge%d.png", i);
//            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp2);
//            firstChargeArr -> addObject(frame);
//        }
//        CCAnimation* firstChargeAnimation = CCAnimation::createWithSpriteFrames(firstChargeArr, 0.11f);
//        CCAnimate* firstChargeAnimate = CCAnimate::create(firstChargeAnimation);
//        firstSp -> runAction(CCRepeatForever::create(firstChargeAnimate));
    }
    
    
    //按钮
    CCMenuItemImage* closeItem = CCMenuItemImage::create("h2all/dakacloseitem.png", "h2all/dakacloseitem.png", this, menu_selector(WufuLayer::clickMenu));
    closeItem -> setTag(101);
    closeItem -> setPosition(ccp(m_pWufuBg->getContentSize().width-closeItem->getContentSize().width/2+20, m_pWufuBg->getContentSize().height-closeItem->getContentSize().height/2+20));
    CCMenuItemImage* pJing1Item = CCMenuItemImage::create("h2all/jingdong50.png", "h2all/jingdong50.png", this, menu_selector(WufuLayer::clickMenu));
    pJing1Item -> setTag(102);
    pJing1Item -> setPosition(ccp(m_pWufuBg->getContentSize().width/4+30, 90));
    CCMenuItemImage* pJing2Item  = CCMenuItemImage::create("h2all/jingdong100.png", "h2all/jingdong100.png", this, menu_selector(WufuLayer::clickMenu));
    pJing2Item -> setPosition(ccp(m_pWufuBg->getContentSize().width/4*3-30, 90));
    pJing2Item -> setTag(103);
    CCMenuItemImage* pHelpItem = CCMenuItemImage::create("h2all/wufuhelpItem.png", "h2all/wufuhelpItem.png", this, menu_selector(WufuLayer::clickMenu));
    pHelpItem -> setTag(104);
    pHelpItem -> setPosition(ccp(m_pWufuBg->getContentSize().width/2, 90));
    CCMenu* menu = CCMenu::create(closeItem, pJing1Item, pJing2Item, pHelpItem, NULL);
    menu -> setPosition(CCPointZero);
    menu -> setAnchorPoint(CCPointZero);
    m_pWufuBg -> addChild(menu);
    menu -> setTouchPriority(-130);
    
    m_pWufuBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
    
    
    m_layerWufuHelp = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(m_layerWufuHelp);
    m_layerWufuHelp->setVisible(false);
    m_spInfoBg = CCSprite::create("h2all/wufuhelpbg.png");
    m_spInfoBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spInfoBg);
    m_spInfoBg -> setVisible(false);
    //请求帮助的版本号
    requestHelpVersion();
}

void WufuLayer::requestHelpVersion()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/LeleconfigVersion.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(WufuLayer::onHttpHelpVersionComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
    CCSprite* pDelayRank = CCSprite::create("h2all/loading1.png");
    pDelayRank -> setPosition(ccp(m_spInfoBg->getContentSize().width/2, m_spInfoBg->getContentSize().height/2-15));
    m_spInfoBg -> addChild(pDelayRank, 1, 101);
    pDelayRank -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
}

void WufuLayer::onHttpHelpVersionComplete(CCHttpClient* pSender, CCHttpResponse* response)
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
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement == NULL) return;
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("version"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL) {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "wufurule"))//相同
            {
                std::string sWufuVersion = pCurrentElement->GetText();
                if(CCUserDefault::sharedUserDefault()->getStringForKey("WufuVersion", "") == "")
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("NewWufuVersion", sWufuVersion);
                    sendWufuData();
                }
                else if(CCUserDefault::sharedUserDefault()->getStringForKey("WufuVersion") != sWufuVersion)//本地保存的版本号跟新的不一致，说明有更新
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("NewWufuVersion", sWufuVersion);
                    sendWufuData();
                }
                else//本地已经保存且没有更新内容，则此时直接打开即可
                {
                    readWufuRuleData();
                }
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
}

int WufuLayer::getWufuType()
{
    return m_nType;
}

void WufuLayer::sendWufuData()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/Lelewufurule.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(WufuLayer::onHttpHelpDataComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void WufuLayer::onHttpHelpDataComplete(CCHttpClient* pSender, CCHttpResponse* response)
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
    std::string sVIPNewVersion = CCUserDefault::sharedUserDefault()->getStringForKey("NewWufuVersion");
    CCUserDefault::sharedUserDefault()->setStringForKey("WufuVersion", sVIPNewVersion);
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement == NULL) return;
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("wufurule"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL)
        {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "rule"))//相同
            {
                std::string sData = pCurrentElement->Attribute("data");
                m_vctWufuRule.push_back(sData);
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
        
    }
    delete document;
    
    
    saveFile(str);
    showWufuHelpUI();
    
}

void WufuLayer::saveFile(std::string str)
{
    //在Documents目录下创建一个可读写的文件
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + std::string("WufuRule.xml");
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

void WufuLayer::readWufuRuleData()
{
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + std::string("WufuRule.xml");
    TiXmlDocument* document = new TiXmlDocument(xmlPath.c_str());
    if(document != NULL)
    {
        document -> LoadFile();
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement == NULL) return;
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("wufurule"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL)
        {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "rule"))//相同
            {
                std::string sData = pCurrentElement->Attribute("data");
                m_vctWufuRule.push_back(sData);
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    document->Clear();
    delete document;
    
    showWufuHelpUI();
}

void WufuLayer::showWufuHelpUI()
{
    CCSprite* pDelayRank = (CCSprite*)m_spInfoBg->getChildByTag(101);
    if(pDelayRank != NULL)
    {
        pDelayRank->removeFromParent();
    }
    int nCount = (int)m_vctWufuRule.size();
    for(int i = 0; i < nCount; i++)
    {
        CCLabelTTF* pLabelRule = CCLabelTTF::create("", "Arial-BoldMT", 24);
        pLabelRule -> setPosition(ccp(36, 324-40*i));
        pLabelRule -> setAnchorPoint(ccp(0, 0.5f));
        m_spInfoBg -> addChild(pLabelRule);
        m_vctVIPNotice.clear();
        float fWidth=0;
        std::string str = m_vctWufuRule[i];
        int size = (int)str.find("#");
        if(size == -1)//说明整个字符串都不包含#
        {
            pLabelRule->setString(str.c_str());
        }
        else if(size != 0)//说明第一个字符不是#
        {
            VIPNotice VipNotice0;
            VipNotice0.sColor = "";
            VipNotice0.sNotice = "";
            VipNotice0.sTag = "";
            std::string str0 = "";
            str0 = str.substr(0, size);
            std::string strColor0 = str0.substr(1, 6);
            VipNotice0.sColor = "FFFFFF";//默认白色
            VipNotice0.sNotice = str0;
            m_vctVIPNotice.push_back(VipNotice0);
            str = str.substr(size);
            size = (int)str.find("#");
            std::string str1 = "";
            std::string str2 = "";
            while (size != -1)
            {
                if(size == 0)
                {
                    VIPNotice VipNotice;
                    VipNotice.sColor = "";
                    VipNotice.sNotice = "";
                    VipNotice.sTag = "";
                    //判断是否为L
                    if (str.substr(1, 1) == "L")
                    {
                        VipNotice.sTag = str.substr(2, 2);
                        VipNotice.sColor = "7CBC3D";
                        str = str.substr(4);
                        size = (int)str.find("#");
                    }
                    else
                    {
                        str2 = str.substr(1, 6);
                        if (str2.length() == 6)//有颜色
                        {
                            VipNotice.sColor = str2;
                        }
                        else   //默认颜色
                        {
                            VipNotice.sColor = "FFFF00";
                        }
                        
                        str = str.substr(7);
                        size = (int)str.find("#");
                    }
                    
                    if(size == -1)
                    {
                        VipNotice.sNotice = str;
                    }
                    else if(size != 0)
                    {
                        str1 = str.substr(0, size);
                        VipNotice.sNotice = str1;
                        str = str.substr(size);
                        size = (int)str.find("#");
                    }
                    m_vctVIPNotice.push_back(VipNotice);
                }
            }
            pLabelRule->setString(m_vctVIPNotice[0].sNotice.c_str());
            int r, g, b = 0;
            std::string s1, s2, s3 = "";
            s1 = m_vctVIPNotice[0].sColor.substr(0, 2);
            s2 = m_vctVIPNotice[0].sColor.substr(2, 2);
            s3 = m_vctVIPNotice[0].sColor.substr(4, 2);
            sscanf(s1.c_str(), "%x", &r);
            sscanf(s2.c_str(), "%x", &g);
            sscanf(s3.c_str(), "%x", &b);
            pLabelRule->setColor(ccc3(r, g, b));
            fWidth = pLabelRule->getContentSize().width;
            for(int i = 1; i < m_vctVIPNotice.size(); i++)
            {
                std::string sColor = m_vctVIPNotice[i].sColor;
                std::string sNotice = m_vctVIPNotice[i].sNotice;
                //获取颜色
                int r, g, b = 0;
                std::string str1, str2, str3 = "";
                str1 = sColor.substr(0,2);
                str2 = sColor.substr(2, 2);
                str3 = sColor.substr(4, 2);
                sscanf(str1.c_str(), "%x", &r);
                sscanf(str2.c_str(), "%x", &g);
                sscanf(str3.c_str(), "%x", &b);
                
                CCLabelTTF* pLabel = CCLabelTTF::create(sNotice.c_str(), "Arial-BoldMT", 24);
                pLabel->setColor(ccc3(r, g, b));
                pLabel -> setAnchorPoint(ccp(0, 0.5f));
                pLabel -> setPosition(ccp(fWidth, pLabelRule->getContentSize().height/2));
                pLabel -> setTag(100+i);
                
                pLabelRule -> addChild(pLabel);
                fWidth+=pLabel->getContentSize().width;
                if ("" != m_vctVIPNotice[i].sTag)//L
                {
                    CCSprite * pLinkline = CCSprite::create("h3all/linkline.png");
                    pLinkline->setScaleX(pLabel->getContentSize().width/pLinkline->getContentSize().width);
                    pLinkline->setAnchorPoint(ccp(0, 0));
                    pLinkline->setPosition(ccp(0, -2));
                    pLabel->addChild(pLinkline);
                }
            }
        }
        else//说明第一个字符是#
        {
            std::string str1 = "";
            std::string str2 = "";
            while (size != -1)
            {
                if(size == 0)
                {
                    VIPNotice VipNotice;
                    VipNotice.sColor = "";
                    VipNotice.sNotice = "";
                    VipNotice.sTag = "";
                    //判断是否为L
                    if (str.substr(1, 1) == "L")
                    {
                        VipNotice.sTag = str.substr(2, 2);
                        VipNotice.sColor = "7CBC3D";
                        str = str.substr(4);
                        size = (int)str.find("#");
                    }
                    else
                    {
                        str2 = str.substr(1, 6);
                        if (str2.length() == 6)//有颜色
                        {
                            VipNotice.sColor = str2;
                        }
                        else   //默认颜色
                        {
                            VipNotice.sColor = "FFFFFF";
                        }
                        
                        str = str.substr(7);
                        size = (int)str.find("#");
                    }
                    
                    if(size == -1)
                    {
                        VipNotice.sNotice = str;
                    }
                    else if(size != 0)
                    {
                        str1 = str.substr(0, size);
                        VipNotice.sNotice = str1;
                        str = str.substr(size);
                        size = (int)str.find("#");
                    }
                    m_vctVIPNotice.push_back(VipNotice);
                }
            }
            pLabelRule->setString(m_vctVIPNotice[0].sNotice.c_str());
            int r, g, b = 0;
            std::string s1, s2, s3 = "";
            s1 = m_vctVIPNotice[0].sColor.substr(0, 2);
            s2 = m_vctVIPNotice[0].sColor.substr(2, 2);
            s3 = m_vctVIPNotice[0].sColor.substr(4, 2);
            sscanf(s1.c_str(), "%x", &r);
            sscanf(s2.c_str(), "%x", &g);
            sscanf(s3.c_str(), "%x", &b);
            pLabelRule->setColor(ccc3(r, g, b));
            fWidth = pLabelRule->getContentSize().width;
            for(int i = 1; i < m_vctVIPNotice.size(); i++)
            {
                std::string sColor = m_vctVIPNotice[i].sColor;
                std::string sNotice = m_vctVIPNotice[i].sNotice;
                //获取颜色
                int r, g, b = 0;
                std::string str1, str2, str3 = "";
                str1 = sColor.substr(0,2);
                str2 = sColor.substr(2, 2);
                str3 = sColor.substr(4, 2);
                sscanf(str1.c_str(), "%x", &r);
                sscanf(str2.c_str(), "%x", &g);
                sscanf(str3.c_str(), "%x", &b);
                
                CCLabelTTF* pLabel = CCLabelTTF::create(sNotice.c_str(), "Arial-BoldMT", 24);
                pLabel->setColor(ccc3(r, g, b));
                pLabel -> setAnchorPoint(ccp(0, 0.5f));
                pLabel -> setPosition(ccp(fWidth, pLabelRule->getContentSize().height/2));
                pLabel -> setTag(100+i);
                pLabelRule -> addChild(pLabel);
                fWidth+=pLabel->getContentSize().width;
                if ("" != m_vctVIPNotice[i].sTag)//L
                {
                    CCSprite * pLinkline = CCSprite::create("h3all/linkline.png");
                    pLinkline->setScaleX(pLabel->getContentSize().width/pLinkline->getContentSize().width);
                    pLinkline->setAnchorPoint(ccp(0, 0));
                    pLinkline->setPosition(ccp(0, -2));
                    pLabel->addChild(pLinkline);
                }
            }
        }
    }
}

void WufuLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case 101:
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
            CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(WufuLayer::removeWnd));
            m_pWufuBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWufuClose);
            break;
        }
        case 102://50元京东卡
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            int num = 0;
            for(int i = 0; i < 5; i++)
            {
                num += m_nCard[i];
            }
            if(num < 3)
            {
                CMainLogic::sharedMainLogic()->ShowMessage("对不起，您的福卡不满足合成条件。", eMBOK);
            }
            else
            {
                //发送兑合成的消息
                m_nType = 1;
                CMainLogic::sharedMainLogic()->ShowMessage("系统将优先帮您使用三张多余福卡合成一张50元京东卡，是否确认？", eMBOKCancel, eDuihuanWufuCard);
            }
            break;
        }
        case 103://100元京东卡
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            bool bCan = true;
            for(int i = 0; i < 5; i++)
            {
                if(m_nCard[i] <= 0)
                {
                    bCan = false;
                    break;
                }
            }
            if(bCan)
            {
                m_nType = 2;
                CMainLogic::sharedMainLogic()->ShowMessage("您即将使用5张福卡合成一张100元京东卡，是否确认？", eMBOKCancel, eDuihuanWufuCard);
                
            }
            else
            {
                CMainLogic::sharedMainLogic()->ShowMessage("对不起，您的福卡不满足合成条件。", eMBOK);
            }
            break;
        }
        case 104://帮助
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            m_nCurrentPage = 2;
            m_spInfoBg -> setVisible(true);
            m_layerWufuHelp -> setVisible(true);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWufuHelp);
            break;
        }
        default:
            break;
    }
}

void WufuLayer::sendDuihuanCard(int nType)
{
    int type = nType-1;
    if(type == 0)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWufu50yuan);
    }
    else if(type == 1)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWufu100yuan);
    }
    char temp[128];
    sprintf(temp, "http://%s/GetFu.aspx?uid=%d&type=%d", NORMAL_URL,CMainLogic::sharedMainLogic()->m_nUserID, type);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(WufuLayer::onHttpDuihuanComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
    CMainLogic::sharedMainLogic()->ShowMessage("正在合成京东卡，请耐心等待", eMBNull);
}

void WufuLayer::onHttpDuihuanComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    //<wufu><data result="1" card1="12" card2="47" card3="48" card4="48" card5="48" description="恭喜您，成功合成50元京东礼品卡，系统将在30分钟内给您发送充值卡号和密码，请及时查看邮件。"/></wufu>
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement == NULL) return;
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("wufu"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        if (pCurrentElement != NULL)
        {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "data"))//相同
            {
                std::string sResult = pCurrentElement->Attribute("result");
                std::string sDes = pCurrentElement->Attribute("description");
                int nResult = atoi(sResult.c_str());
                if(nResult == 1)//合成成功
                {
                    CMainLogic::sharedMainLogic()->ShowMessage(sDes.c_str(), eMBOK);
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWufuSuccess);
                    
                    //向服务端发送公告
                    CMD_GPR_NoticeWufuCard NoticeWufuCard = {0};
                    NoticeWufuCard.nType = m_nType;
                    strncpy(NoticeWufuCard.szNickName, CCharCode::UTF8ToGB2312(CMainLogic::sharedMainLogic()->m_sNickName.c_str()).c_str(), sizeof(NoticeWufuCard.szNickName));
                    CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_NOTICE, SUB_GPR_NOTICE_WUFU_CARD, &NoticeWufuCard,sizeof(NoticeWufuCard));
                }
                else//合成失败
                {
                    CMainLogic::sharedMainLogic()->ShowMessage(sDes.c_str(), eMBOK);
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWufuFail);
                }
                //更新卡片数量
                m_nCard[0] = atoi(pCurrentElement->Attribute("card1"));
                m_nCard[1] = atoi(pCurrentElement->Attribute("card2"));
                m_nCard[2] = atoi(pCurrentElement->Attribute("card3"));
                m_nCard[3] = atoi(pCurrentElement->Attribute("card4"));
                m_nCard[4] = atoi(pCurrentElement->Attribute("card5"));
                updateCardUI();
            }
        }
    }
    delete document;
}

void WufuLayer::updateCardUI()
{
    char temp[8];
    for(int i = 0; i < 5; i++)
    {
        sprintf(temp, "%d", m_nCard[i]);
        CCSprite* sprite1 = (CCSprite*)m_arrAnCard->objectAtIndex(i);
        if(sprite1 == NULL) continue;
        CCLabelTTF* pLabel1 = (CCLabelTTF*)sprite1->getChildByTag(101+i);
        if(pLabel1 == NULL) continue;
        pLabel1 -> setString(temp);
        CCSprite* sprite2 = (CCSprite*)m_arrLiangCard->objectAtIndex(i);
        if(sprite2 == NULL) continue;
        CCLabelTTF* pLabel2 = (CCLabelTTF*)sprite2->getChildByTag(101+i);
        if(pLabel2 == NULL) continue;
        pLabel2 -> setString(temp);
        if(m_nCard[i] <= 0)
        {
            sprite1->setVisible(true);
            sprite2->setVisible(false);
        }
        else
        {
            sprite1->setVisible(false);
            sprite2 -> setVisible(true);
        }
    }
    if(GameMainScene::_instance != NULL)//更新游戏内五福卡图标上数字
    {
        GameMainScene::_instance->setWufuNum(m_nCard);
    }
}

void WufuLayer::removeWnd()
{
    this -> removeFromParent();
    if(GameMainScene::_instance != NULL)
    {
        GameMainScene::GetInstance()->setWndHave(false);
        GameMainScene::GetInstance()->setAutoFire();
        GameMainScene::GetInstance()->setMatchShowStatus(true);
    }
}

void WufuLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool WufuLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if(m_nCurrentPage == 2)
    {
        CCPoint point = pTouch->getLocation();
//        point = m_spInfoBg->convertToNodeSpace(point);
        CCRect rect = m_spInfoBg->boundingBox();
        if(!rect.containsPoint(point))
        {
            m_spInfoBg->setVisible(false);
            m_layerWufuHelp -> setVisible(false);
            m_nCurrentPage = 1;
        }
    }
    return true;
}
