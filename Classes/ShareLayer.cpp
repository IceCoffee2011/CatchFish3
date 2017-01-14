#include "header.h"
extern int WeiXinCharge;

const CCPoint CardPoint[2] = {ccp(200, 150), ccp(550, 150)};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
    //方法名与java类中的包名+方法名，以下划线连接
    void Java_cn_sharesdk_hellocpp_HelloCpp_getPasteContent(JNIEnv*  env, jobject thiz, jstring str)
    {
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                ShareLayer* pShareLayer = (ShareLayer*)pHallLayer->getChildByTag(19982);
                if(pShareLayer != NULL)
                {
                    std::string txtStr = JniHelper::jstring2string(str);
                    pShareLayer->m_sPasteContent = txtStr;
                    pShareLayer->scheduleOnce(schedule_selector(ShareLayer::scheduleDelayShowPaste), 0.0f);
                }
            }
        }
    }
}
#endif

MyMenu * MyMenu::create(CCMenuItem* item, ...)
{
    va_list args;
    va_start(args,item);
    
    MyMenu *pRet = MyMenu::createWithItems(item, args);
    va_end(args);
    
    
    return pRet;
}

MyMenu* MyMenu::createWithArray(CCArray* pArrayOfItems)
{
    MyMenu *pRet = new MyMenu();
    if (pRet && pRet->initWithArray(pArrayOfItems))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    
    
    return pRet;
}

MyMenu* MyMenu::createWithItem(CCMenuItem* item)
{
    return MyMenu::create(item, NULL);
}

MyMenu* MyMenu::createWithItems(CCMenuItem* item, va_list args)
{
    CCArray* pArray = NULL;
    if( item )
    {
        pArray = CCArray::create(item, NULL);
        CCMenuItem *i = va_arg(args, CCMenuItem*);
        while(i)
        {
            pArray->addObject(i);
            i = va_arg(args, CCMenuItem*);
        }
    }
    
    return MyMenu::createWithArray(pArray);
}


bool MyMenu::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint point = pTouch->getLocation();
    CCRect rect(265, 220, 750, 300);
    if(!rect.containsPoint(point))
    {
        return false;
    }
    m_fStart = pTouch->getLocation();
    m_fEnd = pTouch->getLocation();
    return CCMenu::ccTouchBegan(pTouch,pEvent);
}

void MyMenu::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
    
}

void MyMenu::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    m_fEnd = pTouch->getLocation();
    if(abs((int)m_fEnd.x-(int)m_fStart.x) < 5 || abs((int)m_fEnd.y-(int)m_fStart.y)< 5)
    {
        CCMenu::ccTouchEnded(pTouch,pEvent);
    }
    else
    {
        m_eState = kCCMenuStateWaiting;
        if(m_pSelectedItem)
        {
            m_pSelectedItem->unselected();
        }
    }
}

ShareLayer::ShareLayer()
{
    
}

ShareLayer::~ShareLayer()
{
    for(int i = 0; i < m_vctExchangeInfo.size(); i++)
    {
        delete m_vctExchangeInfo[i];
    }
    m_vctExchangeInfo.clear();
    m_arrShareMenu->release();
}

ShareLayer* ShareLayer::create(int nOpenType)
{
    ShareLayer* pRet = new ShareLayer();
    if(pRet && pRet->init(nOpenType))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return pRet;
    }
}

bool ShareLayer::init(int nOpenType)
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    m_nOpenType = nOpenType;
    m_bShowExchangeCard = false;
    m_bShowGuideGift = true;
    m_sShareUrl = "";
    m_sShareContent = "";
    m_nSelectCardType = -1;
    
    m_vctInfo.clear();
    m_vctExchangeInfo.clear();
    m_arrShareMenu = CCArray::create();
    m_arrShareMenu -> retain();
    
    loadUI();
    loadFriendUI();
    loadCodeUI();
    loadInfoUI();
    loadExchangeUI();
    
    requestShareData();
    
    requestShareInfo();
    
    this ->  setTouchEnabled(true);
    
    
    
    return true;
}

void ShareLayer::loadUI()
{
    m_spShareBg = CCSprite::create("share/sharebg.png");
    m_spShareBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spShareBg);
    if(m_nOpenType == 1)
    {
        m_spShareBg->setVisible(false);
    }
    else
    {
        m_spShareBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
    }
    
    
    CCSprite* pDelayRank = CCSprite::create("h2all/loading1.png");
    pDelayRank -> setPosition(ccp(m_spShareBg->getContentSize().width/2, m_spShareBg->getContentSize().height/2-15));
    m_spShareBg -> addChild(pDelayRank, 1, 99);
    pDelayRank -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
    
    CCMenuItemImage* shareItem1 = CCMenuItemImage::create("share/shareitem1.png", "share/shareitem1.png", this, menu_selector(ShareLayer::clickMenu));
    shareItem1 -> setPosition(ccp(m_spShareBg->getContentSize().width-shareItem1->getContentSize().width/2-12, m_spShareBg->getContentSize().height/2+130));
    shareItem1 -> setTag(101);
    CCMenuItemImage* shareItem2 = CCMenuItemImage::create("share/shareitem2.png", "share/shareitem2.png", this, menu_selector(ShareLayer::clickMenu));
    shareItem2 -> setPosition(ccp(m_spShareBg->getContentSize().width-shareItem2->getContentSize().width/2-12, m_spShareBg->getContentSize().height/2+30));
    shareItem2 -> setTag(102);
    CCMenuItemImage* shareItem3 = CCMenuItemImage::create("share/shareitem3.png", "share/shareitem3.png", this, menu_selector(ShareLayer::clickMenu));
    shareItem3 -> setTag(103);
    shareItem3 -> setPosition(ccp(m_spShareBg->getContentSize().width-shareItem3->getContentSize().width/2-12, m_spShareBg->getContentSize().height/2-70));
    
    CCMenuItemImage* shareItem4 = CCMenuItemImage::create("share/shareitem4.png", "share/shareitem4.png", this, menu_selector(ShareLayer::clickMenu));
    shareItem4 -> setPosition(ccp(m_spShareBg->getContentSize().width-shareItem4->getContentSize().width/2-12, 120));
    shareItem4 -> setTag(104);
    //白光遮罩
    CCSprite* pQuickSprite = CCSprite::create("share/shareheidi.png");
    CCSprite* pQuickSprite2 = CCSprite::create("share/shareheidi.png");
    CCClippingNode* pCliper = CCClippingNode::create();
    pCliper->setStencil(pQuickSprite);
    pCliper->setAlphaThreshold(0);
    shareItem4->addChild(pCliper, 3);
    pQuickSprite2->setOpacity(0);
    pCliper->setPosition(ccp(pQuickSprite->getContentSize().width/2+8, pQuickSprite->getContentSize().height/2+9));
    pCliper->addChild(pQuickSprite2);
    CCSprite* pBaiGuang = CCSprite::create("share/shareguang.png");
    pBaiGuang -> setPosition(ccp(-80, 0));
    pCliper -> addChild(pBaiGuang);
    pBaiGuang -> runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(1.5f, ccp(250, 0)), CCMoveBy::create(0, ccp(-250, 0)), NULL)));
    
    
    CCMenuItemImage* closeItem = CCMenuItemImage::create("share/sharecloseitem.png", "share/sharecloseitem.png", this, menu_selector(ShareLayer::clickClose));
    closeItem -> setPosition(ccp(m_spShareBg->getContentSize().width, m_spShareBg->getContentSize().height-50));
    closeItem -> setTag(201);
    CCMenu* pCloseMenu = CCMenu::create(closeItem, shareItem1, shareItem2, shareItem4, NULL);
    pCloseMenu -> setPosition(CCPointZero);
    pCloseMenu -> setTouchPriority(-130);
    pCloseMenu -> setAnchorPoint(CCPointZero);
    m_spShareBg -> addChild(pCloseMenu);
    
    m_menuGuideGift = CCMenu::create(shareItem3, NULL);
    m_menuGuideGift -> setPosition(CCPointZero);
    m_menuGuideGift -> setAnchorPoint(CCPointZero);
    m_spShareBg -> addChild(m_menuGuideGift);
    m_menuGuideGift -> setTouchPriority(-130);
    m_menuGuideGift -> setVisible(false);
    
    m_labelTotal = CCLabelTTF::create("", "Arial-BoldMT", 24);
    m_labelTotal -> setAnchorPoint(ccp(0, 0.5f));
    m_labelTotal -> setPosition(ccp(165, 455));
    m_labelTotal -> setColor(ccc3(146, 80, 0));
    m_spShareBg -> addChild(m_labelTotal);
    m_labelFriendNum = CCLabelTTF::create("", "Arial-BoldMT", 24);
    m_labelFriendNum -> setAnchorPoint(ccp(0, 0.5f));
    m_labelFriendNum -> setPosition(ccp(165, 397));
    m_labelFriendNum -> setColor(ccc3(146, 80, 0));
    m_spShareBg -> addChild(m_labelFriendNum);
    m_labelCurrent = CCLabelTTF::create("", "Arial-BoldMT", 24);
    m_labelCurrent -> setAnchorPoint(ccp(0, 0.5f));
    m_labelCurrent -> setPosition(ccp(165, 344));
    m_labelCurrent -> setColor(ccc3(146, 80, 0));
    m_spShareBg -> addChild(m_labelCurrent);
    
    m_labelOne = CCLabelTTF::create("", "Arial-BoldMT", 22);
    m_labelOne -> setAnchorPoint(ccp(0, 0.5f));
    m_labelOne -> setPosition(ccp(492, 376));
    m_spShareBg -> addChild(m_labelOne);
    m_labelOne -> setColor(ccc3(146, 80, 0));
    m_labelOneCount = CCLabelTTF::create("", "Arial-BoldMT", 22);
    m_labelOneCount -> setAnchorPoint(ccp(0, 0.5f));
    m_labelOneCount -> setPosition(ccp(524, 346));
    m_spShareBg -> addChild(m_labelOneCount);
    m_labelOneCount -> setColor(ccc3(146, 80, 0));
    m_labelTwo = CCLabelTTF::create("", "Arial-BoldMT", 22);
    m_labelTwo -> setAnchorPoint(ccp(0, 0.5f));
    m_labelTwo -> setPosition(ccp(492, 255));
    m_spShareBg -> addChild(m_labelTwo);
    m_labelTwo -> setColor(ccc3(146, 80, 0));
    m_labelTwoCount = CCLabelTTF::create("", "Arial-BoldMT", 22);
    m_labelTwoCount -> setAnchorPoint(ccp(0, 0.5f));
    m_labelTwoCount -> setPosition(ccp(492, 221));
    m_spShareBg -> addChild(m_labelTwoCount);
    m_labelTwoCount -> setColor(ccc3(146, 80, 0));
    m_labelThree = CCLabelTTF::create("", "Arial-BoldMT", 22);
    m_labelThree -> setAnchorPoint(ccp(0, 0.5f));
    m_labelThree -> setPosition(ccp(492, 130));
    m_spShareBg -> addChild(m_labelThree);
    m_labelThree -> setColor(ccc3(146, 80, 0));
    m_labelThreeCount = CCLabelTTF::create("", "Arial-BoldMT", 22);
    m_labelThreeCount -> setAnchorPoint(ccp(0, 0.5f));
    m_labelThreeCount -> setPosition(ccp(492, 98));
    m_spShareBg -> addChild(m_labelThreeCount);
    m_labelThreeCount -> setColor(ccc3(146, 80, 0));
}

void ShareLayer::requestShareData()//请求分享数据
{
    char temp[128];
    sprintf(temp, "http://%s/GetFriendInfo.aspx?uid=%d", NORMAL_URL, CMainLogic::sharedMainLogic()->m_nUserID);//1807158
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(temp);
    printf("temp url is %s\n", temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(ShareLayer::onHttpShareComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void ShareLayer::onHttpShareComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    printf("str content is %s\n", str.c_str());
    CCSprite* pDelay = (CCSprite*)m_spShareBg->getChildByTag(99);
    if(pDelay != NULL)
    {
        pDelay->removeFromParent();
    }
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement == NULL) return;
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("friend"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL) {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "data"))//相同
            {
                std::string sSumValue = pCurrentElement->Attribute("sumvalue");
                m_labelTotal->setString(sSumValue.c_str());
                std::string sSumCount = pCurrentElement->Attribute("sumcount");
                m_labelFriendNum->setString(sSumCount.c_str());
                std::string sNowValue = pCurrentElement->Attribute("nowvalue");
                m_labelCurrent->setString(sNowValue.c_str());
                m_labelMyFriend->setString(sNowValue.c_str());
                std::string sOnep = pCurrentElement->Attribute("onep");
                m_labelOne->setString(sOnep.c_str());
                std::string sTwop = pCurrentElement->Attribute("twop");
                m_labelTwo->setString(sTwop.c_str());
                std::string sThreep = pCurrentElement->Attribute("threep");
                m_labelThree->setString(sThreep.c_str());
                std::string sOnev = pCurrentElement->Attribute("onev");
                SCORE nOneV = atoll(sOnev.c_str());
                if(nOneV >= 10000000)
                {
                    char temp[16];
                    sprintf(temp, "%lld万", nOneV/10000);
                    m_labelOneCount -> setString(temp);
                }
                else if(nOneV >= 10000)
                {
                    char temp[16];
                    sprintf(temp, "%lld.%lld万", nOneV/10000, nOneV%10000/1000);
                    m_labelOneCount -> setString(temp);
                }
                else
                {
                    m_labelOneCount->setString(sOnev.c_str());
                }
                
                std::string sTwov = pCurrentElement->Attribute("twov");
                SCORE nTwov = atoll(sTwov.c_str());
                if(nTwov >= 10000000)
                {
                    char temp[16];
                    sprintf(temp, "%lld万", nTwov/10000);
                    m_labelTwoCount -> setString(temp);
                }
                else if(nTwov >= 10000)
                {
                    char temp[16];
                    sprintf(temp, "%lld.%lld万", nTwov/10000, nTwov%10000/1000);
                    m_labelTwoCount -> setString(temp);
                }
                else
                {
                    m_labelTwoCount->setString(sTwov.c_str());
                }
                std::string sThreev = pCurrentElement->Attribute("threev");
                SCORE nThreeV = atoll(sThreev.c_str());
                if(nThreeV >= 10000000)
                {
                    char temp[16];
                    sprintf(temp, "%lld万", nThreeV/10000);
                    m_labelThreeCount -> setString(temp);
                }
                else if(nThreeV >= 10000)
                {
                    char temp[16];
                    sprintf(temp, "%lld.%lld万", nThreeV/10000, nThreeV%10000/1000);
                    m_labelThreeCount -> setString(temp);
                }
                else
                {
                    m_labelThreeCount->setString(sThreev.c_str());
                }
                
                std::string sShareCode = pCurrentElement->Attribute("sharecode");
                m_labelCode->setString(sShareCode.c_str());
                std::string sState = pCurrentElement->Attribute("state");
                m_bShowGuideGift = atoi(sState.c_str());
                m_sShareUrl = pCurrentElement->Attribute("shareurl");
                m_sShareContent = pCurrentElement->Attribute("remark");
                std::string sScore = pCurrentElement->Attribute("score");
                m_labelScore->setString(sScore.c_str());
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
    if(!m_bShowGuideGift)
    {
        m_menuGuideGift -> setVisible(true);
    }
    if(CMainLogic::sharedMainLogic()->m_sAndroidMachine != "fenxiang")
    {
        m_menuGuideGift -> setVisible(false);
    }
}

void ShareLayer::loadFriendUI()
{
    m_spFriendBg = CCSprite::create("share/sharecodebg.png");
    m_spFriendBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spFriendBg, 2);
    m_spFriendBg -> setVisible(false);
    CCSprite* pTitle = CCSprite::create("share/sharetitle3.png");
    pTitle -> setPosition(ccp(m_spFriendBg->getContentSize().width/2, m_spFriendBg->getContentSize().height-pTitle->getContentSize().height/2-10));
    m_spFriendBg -> addChild(pTitle);
    
    CCSprite* pFriendBg = CCSprite::create("share/sharefriendbg.png");
    pFriendBg -> setPosition(ccp(m_spFriendBg->getContentSize().width/2, m_spFriendBg->getContentSize().height/2-46));
    m_spFriendBg -> addChild(pFriendBg);
    
    CCMenuItemImage* wxItem = CCMenuItemImage::create("share/shareitem5.png", "share/shareitem5.png", this, menu_selector(ShareLayer::clickMenu));
    wxItem -> setPosition(ccp(m_spFriendBg->getContentSize().width/5+17, m_spFriendBg->getContentSize().height/2-100));
    wxItem -> setTag(105);
    CCMenuItemImage* wxchatItem = CCMenuItemImage::create("share/shareitem5.png", "share/shareitem5.png", this, menu_selector(ShareLayer::clickMenu));
    wxchatItem -> setPosition(ccp(m_spFriendBg->getContentSize().width/2+2, m_spFriendBg->getContentSize().height/2-100));
    wxchatItem -> setTag(106);
    CCMenuItemImage* QQItem = CCMenuItemImage::create("share/shareitem5.png", "share/shareitem5.png", this, menu_selector(ShareLayer::clickMenu));
    QQItem -> setTag(107);
    QQItem -> setPosition(ccp(m_spFriendBg->getContentSize().width/5*4-15, m_spFriendBg->getContentSize().height/2-100));
    
    CCMenuItemImage* copyItem = CCMenuItemImage::create("share/shareitem6.png", "share/shareitem6.png", this, menu_selector(ShareLayer::clickMenu));
    copyItem -> setTag(108);
    copyItem -> setPosition(ccp(m_spFriendBg->getContentSize().width-copyItem->getContentSize().width/2-28, 84));
    
    CCMenuItemImage* pCloseFriendItem = CCMenuItemImage::create("share/sharecloseitem.png", "share/sharecloseitem.png", this, menu_selector(ShareLayer::clickClose));
    pCloseFriendItem -> setPosition(ccp(m_spFriendBg->getContentSize().width, m_spFriendBg->getContentSize().height));
    pCloseFriendItem -> setTag(202);
    CCMenu* pCloseMenu = CCMenu::create(pCloseFriendItem, wxItem, wxchatItem, QQItem, copyItem, NULL);
    pCloseMenu -> setPosition(CCPointZero);
    pCloseMenu -> setTouchPriority(-130);
    pCloseMenu -> setAnchorPoint(CCPointZero);
    m_spFriendBg -> addChild(pCloseMenu);
    
    //邀请码
    m_labelCode = CCLabelTTF::create("", "Arial-BoldMT", 26);
    m_labelCode -> setPosition(ccp(422, 79));
    m_labelCode -> setAnchorPoint(ccp(0, 0.5f));
    pFriendBg -> addChild(m_labelCode);
}

void ShareLayer::loadCodeUI()
{
    m_spCodeBg = CCSprite::create("share/sharecodebg.png");
    m_spCodeBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spCodeBg, 3);
    if(m_nOpenType == 1)
    {
        m_spShareBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
    }
    else
    {
        m_spCodeBg -> setVisible(false);
    }
    
    CCSprite* pTitle = CCSprite::create("share/sharetitle1.png");
    pTitle -> setPosition(ccp(m_spCodeBg->getContentSize().width/2, m_spCodeBg->getContentSize().height-pTitle->getContentSize().height/2-10));
    m_spCodeBg -> addChild(pTitle);
    
    CCSprite* pInfo1 = CCSprite::create("share/shareinfo1.png");
    pInfo1 -> setPosition(ccp(m_spCodeBg->getContentSize().width/2, m_spCodeBg->getContentSize().height-140));
    m_spCodeBg -> addChild(pInfo1);
    CCSprite* pInfo2 = CCSprite::create("share/shareinfo2.png");
    pInfo2 -> setPosition(ccp(m_spCodeBg->getContentSize().width/2-60, 110));
    m_spCodeBg -> addChild(pInfo2);
    
    m_labelScore = CCLabelTTF::create("", "Arial-BoldMT", 32);
    m_labelScore -> setPosition(ccp(405, 42));
    m_labelScore -> setAnchorPoint(ccp(0, 0.5f));
    pInfo1 -> addChild(m_labelScore);
    m_labelScore -> setColor(ccc3(84, 0, 0));
    
    //验证码输入框
    m_editBoxCode = CCEditBox::create(CCSize(415, 59), CCScale9Sprite::create("share/sharebox.png"));
    m_editBoxCode->setMaxLength(15);//限制最长字符
    m_editBoxCode->setReturnType(kKeyboardReturnTypeDone);
    m_editBoxCode->setInputFlag(kEditBoxInputFlagSensitive);
    m_editBoxCode->setInputMode(kEditBoxInputModeEmailAddr);
    m_editBoxCode->setPosition(ccp(m_spCodeBg->getContentSize().width/2-42, m_spCodeBg->getContentSize().height/2-30));
    m_editBoxCode->setPlaceHolder("请在此输入邀请码");
    m_editBoxCode->setPlaceholderFontName("Arial-BoldMT");
    m_editBoxCode->setPlaceholderFontSize(10);
    m_spCodeBg->addChild(m_editBoxCode, 10);
    m_editBoxCode -> setTouchPriority(-130);
    m_editBoxCode -> setFontName("Arial-BoldMT");
    
    CCMenuItemImage* pasteItem = CCMenuItemImage::create("share/shareitem7.png", "share/shareitem7.png", this, menu_selector(ShareLayer::clickMenu));
    pasteItem -> setTag(109);
    pasteItem -> setPosition(ccp(645, m_spCodeBg->getContentSize().height/2-30));
    
    CCMenuItemImage* getItem = CCMenuItemImage::create("share/shareitem8.png", "share/shareitem8.png", this, menu_selector(ShareLayer::clickMenu));
    getItem -> setPosition(ccp(m_spCodeBg->getContentSize().width-getItem->getContentSize().width/2-40, 110));
    getItem -> setTag(110);
    
    CCMenuItemImage* pCloseItem = CCMenuItemImage::create("share/sharecloseitem.png", "share/sharecloseitem.png", this, menu_selector(ShareLayer::clickClose));
    pCloseItem -> setPosition(ccp(m_spCodeBg->getContentSize().width, m_spCodeBg->getContentSize().height));
    pCloseItem -> setTag(203);
    CCMenu* pCloseMenu = CCMenu::create(pCloseItem, pasteItem, getItem, NULL);
    pCloseMenu -> setPosition(CCPointZero);
    pCloseMenu -> setTouchPriority(-130);
    pCloseMenu -> setAnchorPoint(CCPointZero);
    m_spCodeBg -> addChild(pCloseMenu);
}

void ShareLayer::loadInfoUI()
{
    m_spInfoBg = CCSprite::create("share/sharecodebg.png");
    m_spInfoBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spInfoBg, 4);
    m_spInfoBg -> setVisible(false);
    CCSprite* pTitle = CCSprite::create("share/sharetitle2.png");
    pTitle -> setPosition(ccp(m_spInfoBg->getContentSize().width/2, m_spInfoBg->getContentSize().height-pTitle->getContentSize().height/2-10));
    m_spInfoBg -> addChild(pTitle);
    
    CCMenuItemImage* pCloseItem = CCMenuItemImage::create("share/sharecloseitem.png", "share/sharecloseitem.png", this, menu_selector(ShareLayer::clickClose));
    pCloseItem -> setPosition(ccp(m_spInfoBg->getContentSize().width, m_spInfoBg->getContentSize().height));
    pCloseItem -> setTag(204);
    CCMenu* pCloseMenu = CCMenu::create(pCloseItem, NULL);
    pCloseMenu -> setPosition(CCPointZero);
    pCloseMenu -> setTouchPriority(-130);
    pCloseMenu -> setAnchorPoint(CCPointZero);
    m_spInfoBg -> addChild(pCloseMenu);
    
    m_scrollInfo = CCScrollView::create();
    m_layerInfo = CCLayer::create();
    m_layerInfo -> setContentSize(CCSizeMake(750, 365));
    m_layerInfo -> setAnchorPoint(CCPointZero);
    m_layerInfo -> setPosition(ccp(0,0));
    m_scrollInfo->setPosition(ccp(30,65));     //根据cocostudio调整的位置
    m_scrollInfo->setViewSize(CCSizeMake(750, 365));//设置view的大小
    m_scrollInfo->setContentSize(CCSizeMake(750, 365));//设置scrollview区域的大小
    m_scrollInfo->setContainer(m_layerInfo); //设置需要滚动的内容
    m_scrollInfo->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_scrollInfo->setDelegate(this);
    m_scrollInfo->setBounceable(true);
    m_spInfoBg->addChild(m_scrollInfo);
    m_scrollInfo -> setTouchEnabled(true);
    m_scrollInfo -> setTouchPriority(-130);
}

void ShareLayer::loadExchangeUI()
{
    m_spExchangeBg = CCSprite::create("share/sharecodebg.png");
    m_spExchangeBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spExchangeBg);
    m_spExchangeBg -> setVisible(false);
    CCSprite* pTitle = CCSprite::create("share/sharetitle4.png");
    pTitle -> setPosition(ccp(m_spExchangeBg->getContentSize().width/2, m_spExchangeBg->getContentSize().height-pTitle->getContentSize().height/2-10));
    m_spExchangeBg -> addChild(pTitle);
    
    CCMenuItemImage* pCloseItem = CCMenuItemImage::create("share/sharecloseitem.png", "share/sharecloseitem.png", this, menu_selector(ShareLayer::clickClose));
    pCloseItem -> setPosition(ccp(m_spExchangeBg->getContentSize().width, m_spExchangeBg->getContentSize().height));
    pCloseItem -> setTag(205);
    CCMenu* pCloseMenu = CCMenu::create(pCloseItem, NULL);
    pCloseMenu -> setPosition(CCPointZero);
    pCloseMenu -> setTouchPriority(-130);
    pCloseMenu -> setAnchorPoint(CCPointZero);
    m_spExchangeBg -> addChild(pCloseMenu);
    m_arrShareMenu -> addObject(pCloseMenu);
    
    CCSprite* pInfo3 = CCSprite::create("share/shareinfo3.png");
    pInfo3 -> setPosition(ccp(m_spExchangeBg->getContentSize().width/2, 90));
    m_spExchangeBg -> addChild(pInfo3, 2);
    m_labelMyFriend = CCLabelTTF::create("", "Arial-BoldMT", 30);
    m_labelMyFriend -> setPosition(ccp(192, pInfo3->getContentSize().height/2));
    m_labelMyFriend -> setAnchorPoint(ccp(0, 0.5f));
    pInfo3 -> addChild(m_labelMyFriend);
    
    m_scrollExchange = CCScrollView::create();
    m_layerExchange = CCLayer::create();
    m_layerExchange -> setContentSize(CCSizeMake(750, 300));
    m_layerExchange -> setAnchorPoint(CCPointZero);
    m_layerExchange -> setPosition(ccp(0,0));
    m_scrollExchange->setPosition(ccp(30,120));     //根据cocostudio调整的位置
    m_scrollExchange->setViewSize(CCSizeMake(750, 300));//设置view的大小
    m_scrollExchange->setContentSize(CCSizeMake(750, 300));//设置scrollview区域的大小
    m_scrollExchange->setContainer(m_layerExchange); //设置需要滚动的内容
    m_scrollExchange->setDirection(kCCScrollViewDirectionHorizontal);  //设置滚动的方向，有三种可以选择
    m_scrollExchange->setBounceable(true);
    m_scrollExchange->setDelegate(this);
    m_spExchangeBg->addChild(m_scrollExchange);
    m_scrollExchange -> setTouchEnabled(true);
    m_scrollExchange -> setTouchPriority(-130);
    
    m_spExchange2Bg = CCSprite::create("share/sharecardbg2.png");
    m_spExchange2Bg -> setPosition(ccp(m_spExchangeBg->getContentSize().width/2, m_spExchangeBg->getContentSize().height/2-20));
    m_spExchangeBg -> addChild(m_spExchange2Bg, 10);
    m_spExchange2Bg -> setVisible(false);
    m_layerColor = CCLayerColor::create(ccc4(0, 0, 0, 100), 1280, 720);
    m_spExchangeBg -> addChild(m_layerColor, 9);
    m_layerColor -> setVisible(false);
    m_layerColor -> ignoreAnchorPointForPosition(false);
    m_layerColor -> setPosition(ccp(m_spExchangeBg->getContentSize().width/2, m_spExchangeBg->getContentSize().height/2));
    
    CCMenuItemImage* yidongItem = CCMenuItemImage::create("share/shareitem10.png", "share/shareitem10.png", this, menu_selector(ShareLayer::clickMenu));
    yidongItem -> setTag(111);
    yidongItem -> setPosition(ccp(m_spExchange2Bg->getContentSize().width/5-10, 140));
    CCMenuItemImage* liantongItem = CCMenuItemImage::create("share/shareitem11.png", "share/shareitem11.png", this, menu_selector(ShareLayer::clickMenu));
    liantongItem -> setTag(112);
    liantongItem -> setPosition(ccp(m_spExchange2Bg->getContentSize().width/2, 140));
    CCMenuItemImage* dianxinItem = CCMenuItemImage::create("share/shareitem12.png", "share/shareitem12.png", this, menu_selector(ShareLayer::clickMenu));
    dianxinItem -> setTag(113);
    dianxinItem -> setPosition(ccp(m_spExchange2Bg->getContentSize().width/5*4+10, 140));
    CCMenu* pMobileMenu = CCMenu::create(yidongItem, liantongItem, dianxinItem, NULL);
    pMobileMenu -> setPosition(CCPointZero);
    pMobileMenu -> setAnchorPoint(CCPointZero);
    m_spExchange2Bg -> addChild(pMobileMenu);
    pMobileMenu -> setTouchPriority(-130);
}

void ShareLayer::requestShareInfo()
{
    //请求版本号
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/LeleconfigVersion.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(ShareLayer::onHttpShareVersionComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void ShareLayer::onHttpShareVersionComplete(CCHttpClient* sender, CCHttpResponse* response)
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
            if(!std::strcmp(pValue, "shareinfoversion"))//相同
            {
                std::string sDakasaiVersion = pCurrentElement->GetText();
                if(CCUserDefault::sharedUserDefault()->getStringForKey("ShareVersion", "") == "")
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("NewShareVersion", sDakasaiVersion);
                    sendShareData();
                }
                else if(CCUserDefault::sharedUserDefault()->getStringForKey("ShareVersion") != sDakasaiVersion)//本地保存的版本号跟新的不一致，说明有更新
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("NewShareVersion", sDakasaiVersion);
                    sendShareData();
                }
                else//本地已经保存且没有更新内容，则此时直接打开即可
                {
                    readShareData();
                }
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
}

void ShareLayer::sendShareData()
{
    CCHttpRequest* request = new CCHttpRequest();
    char temp[128];
    sprintf(temp, "http://%s/GetFriendGiftInfo.aspx", NORMAL_URL);
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(ShareLayer::onHttpShareDataComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void ShareLayer::onHttpShareDataComplete(CCHttpClient* pSender, CCHttpResponse* response)
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
    std::string sVIPNewVersion = CCUserDefault::sharedUserDefault()->getStringForKey("NewShareVersion");
    CCUserDefault::sharedUserDefault()->setStringForKey("ShareVersion", sVIPNewVersion);
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement == NULL) return;
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("shareinfo"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL)
        {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "info"))//相同
            {
                std::string sData = pCurrentElement->Attribute("data");
                m_vctInfo.push_back(sData);
            }
            if(!std::strcmp(pValue, "prize"))//相同
            {
                ExchangeInfo* pExchangeInfo = new ExchangeInfo();
                m_vctExchangeInfo.push_back(pExchangeInfo);
                std::string sID = pCurrentElement->Attribute("id");
                pExchangeInfo->nID = atoi(sID.c_str());
                std::string sType = pCurrentElement->Attribute("type");
                pExchangeInfo->nType = atoi(sType.c_str());
                std::string sCount = pCurrentElement->Attribute("count");
                pExchangeInfo->nCount = atoi(sCount.c_str());
                std::string sMoney = pCurrentElement->Attribute("money");
                pExchangeInfo->nMoney = atoi(sMoney.c_str());
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
    
    showShareInfoUI();
    showExchangeUI();
    
    saveFile(str);
}

void ShareLayer::readShareData()
{
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + std::string("ShareInfoData.xml");
    TiXmlDocument* document = new TiXmlDocument(xmlPath.c_str());
    if(document != NULL)
    {
        document -> LoadFile();
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement == NULL) return;
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("shareinfo"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL)
        {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "info"))//相同
            {
                std::string sData = pCurrentElement->Attribute("data");
                m_vctInfo.push_back(sData);
            }
            if(!std::strcmp(pValue, "prize"))//相同
            {
                ExchangeInfo* pExchangeInfo = new ExchangeInfo();
                m_vctExchangeInfo.push_back(pExchangeInfo);
                std::string sID = pCurrentElement->Attribute("id");
                pExchangeInfo->nID = atoi(sID.c_str());
                std::string sType = pCurrentElement->Attribute("type");
                pExchangeInfo->nType = atoi(sType.c_str());
                std::string sCount = pCurrentElement->Attribute("count");
                pExchangeInfo->nCount = atoi(sCount.c_str());
                std::string sMoney = pCurrentElement->Attribute("money");
                pExchangeInfo->nMoney = atoi(sMoney.c_str());
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    document->Clear();
    delete document;
    printf("m_vctExchangeInfo size is %lu\n", m_vctExchangeInfo.size());
    
    showShareInfoUI();
    showExchangeUI();
}

void ShareLayer::saveFile(std::string str)
{
    //在Documents目录下创建一个可读写的文件
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + std::string("ShareInfoData.xml");
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

void ShareLayer::showShareInfoUI()
{
    int nSize = (int)m_vctInfo.size();
    if(nSize < 10)
    {
        m_scrollInfo -> setTouchEnabled(false);
    }
    else
    {
        m_layerInfo->setContentSize(CCSize(750, 40*nSize));
        m_layerInfo -> setPosition(ccp(0, 365-40*nSize));
    }
    for(int i = 0; i < nSize; i++)
    {
        CCLabelTTF* pLabel = CCLabelTTF::create(m_vctInfo[i].c_str(), "Arial-BoldMT", 24);
        pLabel -> setPosition(ccp(2, m_layerInfo->getContentSize().height-40*i-26));
        pLabel -> setAnchorPoint(ccp(0, 0.5f));
        m_layerInfo -> addChild(pLabel);
    }
}

void ShareLayer::showExchangeUI()
{
    char temp[64];
    int nSize = (int)m_vctExchangeInfo.size();
    if(nSize < 4)
    {
        m_scrollExchange -> setTouchEnabled(false);
        if(nSize == 1)
        {
            if(m_vctExchangeInfo[0]->nType == 0)//京东卡
            {
                CCSprite* pExchangeBg = CCSprite::create("share/sharejingdongbg.png");
                pExchangeBg -> setPosition(ccp(m_layerExchange->getContentSize().width/2, m_layerExchange->getContentSize().height/2));
                m_layerExchange -> addChild(pExchangeBg);
                CCMenuItemImage* pExchangeItem = CCMenuItemImage::create("share/shareitem9.png", "share/shareitem9.png", this, menu_selector(ShareLayer::clickExchange));
                pExchangeItem -> setTag(0);
                pExchangeItem -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 47));
                MyMenu* pMenu = MyMenu::create(pExchangeItem, NULL);
                pMenu -> setPosition(CCPointZero);
                pMenu -> setAnchorPoint(CCPointZero);
                pMenu -> setTouchPriority(-130);
                pExchangeBg -> addChild(pMenu);
                if(m_vctExchangeInfo[0]->nCount >= 10000)
                {
                    sprintf(temp, "消耗%d万友情值", m_vctExchangeInfo[0]->nCount/10000);
                }
                else
                {
                    sprintf(temp, "消耗%d友情值", m_vctExchangeInfo[0]->nCount);
                }
                CCLabelTTF* pLabel1 = CCLabelTTF::create(temp, "Arial-BoldMT", 24);
                pLabel1 -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 242));
                pExchangeBg -> addChild(pLabel1);
                sprintf(temp, "%d元京东卡", m_vctExchangeInfo[0]->nMoney);
                CCLabelTTF* pLabel2 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabel2 -> setPosition(ccp(pExchangeItem->getContentSize().width/2, pExchangeItem->getContentSize().height/2));
                pExchangeItem -> addChild(pLabel2);
                pLabel2 -> setColor(ccc3(151, 39, 17));
                m_arrShareMenu -> addObject(pMenu);
            }
            if(m_vctExchangeInfo[0]->nType == 1)//电话卡
            {
                CCSprite* pExchangeBg = CCSprite::create("share/sharetelcardbg.png");
                pExchangeBg -> setPosition(ccp(m_layerExchange->getContentSize().width/2, m_layerExchange->getContentSize().height/2));
                m_layerExchange -> addChild(pExchangeBg);
                CCMenuItemImage* pExchangeItem = CCMenuItemImage::create("share/shareitem9.png", "share/shareitem9.png", this, menu_selector(ShareLayer::clickExchange));
                pExchangeItem -> setTag(0);
                pExchangeItem -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 50));
                MyMenu* pMenu = MyMenu::create(pExchangeItem, NULL);
                pMenu -> setPosition(CCPointZero);
                pMenu -> setAnchorPoint(CCPointZero);
                pMenu -> setTouchPriority(-130);
                pExchangeBg -> addChild(pMenu);
                if(m_vctExchangeInfo[0]->nCount >= 10000)
                {
                    sprintf(temp, "消耗%d万友情值", m_vctExchangeInfo[0]->nCount/10000);
                }
                else
                {
                    sprintf(temp, "消耗%d友情值", m_vctExchangeInfo[0]->nCount);
                }
                CCLabelTTF* pLabel1 = CCLabelTTF::create(temp, "Arial-BoldMT", 24);
                pLabel1 -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 242));
                pExchangeBg -> addChild(pLabel1);
                sprintf(temp, "%d元话费卡", m_vctExchangeInfo[0]->nMoney);
                CCLabelTTF* pLabel2 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabel2 -> setPosition(ccp(pExchangeItem->getContentSize().width/2, pExchangeItem->getContentSize().height/2));
                pExchangeItem -> addChild(pLabel2);
                pLabel2 -> setColor(ccc3(151, 39, 17));
                m_arrShareMenu -> addObject(pMenu);
            }
        }
        else if(nSize == 2)
        {
            for(int i = 0; i < m_vctExchangeInfo.size(); i++)
            {
                if(m_vctExchangeInfo[i]->nType == 0)//京东卡
                {
                    CCSprite* pExchangeBg = CCSprite::create("share/sharejingdongbg.png");
                    pExchangeBg -> setPosition(CardPoint[i]);
                    m_layerExchange -> addChild(pExchangeBg);
                    CCMenuItemImage* pExchangeItem = CCMenuItemImage::create("share/shareitem9.png", "share/shareitem9.png", this, menu_selector(ShareLayer::clickExchange));
                    pExchangeItem -> setTag(i);
                    pExchangeItem -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 47));
                    MyMenu* pMenu = MyMenu::create(pExchangeItem, NULL);
                    pMenu -> setPosition(CCPointZero);
                    pMenu -> setAnchorPoint(CCPointZero);
                    pMenu -> setTouchPriority(-130);
                    pExchangeBg -> addChild(pMenu);
                    if(m_vctExchangeInfo[i]->nCount >= 10000)
                    {
                        sprintf(temp, "消耗%d万友情值", m_vctExchangeInfo[i]->nCount/10000);
                    }
                    else
                    {
                        sprintf(temp, "消耗%d友情值", m_vctExchangeInfo[i]->nCount);
                    }
                    CCLabelTTF* pLabel1 = CCLabelTTF::create(temp, "Arial-BoldMT", 24);
                    pLabel1 -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 242));
                    pExchangeBg -> addChild(pLabel1);
                    sprintf(temp, "%d元京东卡", m_vctExchangeInfo[i]->nMoney);
                    CCLabelTTF* pLabel2 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                    pLabel2 -> setPosition(ccp(pExchangeItem->getContentSize().width/2, pExchangeItem->getContentSize().height/2));
                    pExchangeItem -> addChild(pLabel2);
                    pLabel2 -> setColor(ccc3(151, 39, 17));
                    m_arrShareMenu -> addObject(pMenu);
                }
                if(m_vctExchangeInfo[i]->nType == 1)//电话卡
                {
                    CCSprite* pExchangeBg = CCSprite::create("share/sharetelcardbg.png");
                    pExchangeBg -> setPosition(CardPoint[i]);
                    m_layerExchange -> addChild(pExchangeBg);
                    CCMenuItemImage* pExchangeItem = CCMenuItemImage::create("share/shareitem9.png", "share/shareitem9.png", this, menu_selector(ShareLayer::clickExchange));
                    pExchangeItem -> setTag(i);
                    pExchangeItem -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 50));
                    MyMenu* pMenu = MyMenu::create(pExchangeItem, NULL);
                    pMenu -> setPosition(CCPointZero);
                    pMenu -> setAnchorPoint(CCPointZero);
                    pMenu -> setTouchPriority(-130);
                    pExchangeBg -> addChild(pMenu);
                    if(m_vctExchangeInfo[i]->nCount >= 10000)
                    {
                        sprintf(temp, "消耗%d万友情值", m_vctExchangeInfo[i]->nCount/10000);
                    }
                    else
                    {
                        sprintf(temp, "消耗%d友情值", m_vctExchangeInfo[i]->nCount);
                    }
                    CCLabelTTF* pLabel1 = CCLabelTTF::create(temp, "Arial-BoldMT", 24);
                    pLabel1 -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 242));
                    pExchangeBg -> addChild(pLabel1);
                    sprintf(temp, "%d元话费卡", m_vctExchangeInfo[i]->nMoney);
                    CCLabelTTF* pLabel2 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                    pLabel2 -> setPosition(ccp(pExchangeItem->getContentSize().width/2, pExchangeItem->getContentSize().height/2));
                    pExchangeItem -> addChild(pLabel2);
                    pLabel2 -> setColor(ccc3(151, 39, 17));
                    m_arrShareMenu -> addObject(pMenu);
                }
            }
        }
        else if(nSize == 3)
        {
            for(int i = 0; i < m_vctExchangeInfo.size(); i++)
            {
                if(m_vctExchangeInfo[i]->nType == 0)//京东卡
                {
                    CCSprite* pExchangeBg = CCSprite::create("share/sharejingdongbg.png");
                    pExchangeBg -> setPosition(ccp(m_layerExchange->getContentSize().width/(nSize+1)*(i+1)+(i-1)*60, m_layerExchange->getContentSize().height/2));
                    m_layerExchange -> addChild(pExchangeBg);
                    CCMenuItemImage* pExchangeItem = CCMenuItemImage::create("share/shareitem9.png", "share/shareitem9.png", this, menu_selector(ShareLayer::clickExchange));
                    pExchangeItem -> setTag(i);
                    pExchangeItem -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 47));
                    MyMenu* pMenu = MyMenu::create(pExchangeItem, NULL);
                    pMenu -> setPosition(CCPointZero);
                    pMenu -> setAnchorPoint(CCPointZero);
                    pMenu -> setTouchPriority(-130);
                    pExchangeBg -> addChild(pMenu);
                    if(m_vctExchangeInfo[i]->nCount >= 10000)
                    {
                        sprintf(temp, "消耗%d万友情值", m_vctExchangeInfo[i]->nCount/10000);
                    }
                    else
                    {
                        sprintf(temp, "消耗%d友情值", m_vctExchangeInfo[i]->nCount);
                    }
                    CCLabelTTF* pLabel1 = CCLabelTTF::create(temp, "Arial-BoldMT", 24);
                    pLabel1 -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 242));
                    pExchangeBg -> addChild(pLabel1);
                    sprintf(temp, "%d元京东卡", m_vctExchangeInfo[i]->nMoney);
                    CCLabelTTF* pLabel2 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                    pLabel2 -> setPosition(ccp(pExchangeItem->getContentSize().width/2, pExchangeItem->getContentSize().height/2));
                    pExchangeItem -> addChild(pLabel2);
                    pLabel2 -> setColor(ccc3(151, 39, 17));
                    m_arrShareMenu -> addObject(pMenu);
                }
                if(m_vctExchangeInfo[i]->nType == 1)//电话卡
                {
                    CCSprite* pExchangeBg = CCSprite::create("share/sharetelcardbg.png");
                    pExchangeBg -> setPosition(ccp(m_layerExchange->getContentSize().width/(nSize+1)*(i+1)+(i-1)*60, m_layerExchange->getContentSize().height/2));
                    m_layerExchange -> addChild(pExchangeBg);
                    CCMenuItemImage* pExchangeItem = CCMenuItemImage::create("share/shareitem9.png", "share/shareitem9.png", this, menu_selector(ShareLayer::clickExchange));
                    pExchangeItem -> setTag(i);
                    pExchangeItem -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 50));
                    MyMenu* pMenu = MyMenu::create(pExchangeItem, NULL);
                    pMenu -> setPosition(CCPointZero);
                    pMenu -> setAnchorPoint(CCPointZero);
                    pMenu -> setTouchPriority(-130);
                    pExchangeBg -> addChild(pMenu);
                    if(m_vctExchangeInfo[i]->nCount >= 10000)
                    {
                        sprintf(temp, "消耗%d万友情值", m_vctExchangeInfo[i]->nCount/10000);
                    }
                    else
                    {
                        sprintf(temp, "消耗%d友情值", m_vctExchangeInfo[i]->nCount);
                    }
                    CCLabelTTF* pLabel1 = CCLabelTTF::create(temp, "Arial-BoldMT", 24);
                    pLabel1 -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 242));
                    pExchangeBg -> addChild(pLabel1);
                    sprintf(temp, "%d元话费卡", m_vctExchangeInfo[i]->nMoney);
                    CCLabelTTF* pLabel2 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                    pLabel2 -> setPosition(ccp(pExchangeItem->getContentSize().width/2, pExchangeItem->getContentSize().height/2));
                    pExchangeItem -> addChild(pLabel2);
                    pLabel2 -> setColor(ccc3(151, 39, 17));
                    m_arrShareMenu -> addObject(pMenu);
                }
            }
        }
    }
    else
    {
        m_layerExchange -> setContentSize(CCSize(230*nSize, 300));
        for(int i = 0; i < m_vctExchangeInfo.size(); i++)
        {
            if(m_vctExchangeInfo[i]->nType == 0)//京东卡
            {
                CCSprite* pExchangeBg = CCSprite::create("share/sharejingdongbg.png");
                pExchangeBg -> setPosition(ccp(115+230*i, m_layerExchange->getContentSize().height/2));
                m_layerExchange -> addChild(pExchangeBg);
                CCMenuItemImage* pExchangeItem = CCMenuItemImage::create("share/shareitem9.png", "share/shareitem9.png", this, menu_selector(ShareLayer::clickExchange));
                pExchangeItem -> setTag(i);
                pExchangeItem -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 47));
                MyMenu* pMenu = MyMenu::create(pExchangeItem, NULL);
                pMenu -> setPosition(CCPointZero);
                pMenu -> setAnchorPoint(CCPointZero);
                pMenu -> setTouchPriority(-132);
                pExchangeBg -> addChild(pMenu);
                if(m_vctExchangeInfo[i]->nCount >= 10000)
                {
                    sprintf(temp, "消耗%d万友情值", m_vctExchangeInfo[i]->nCount/10000);
                }
                else
                {
                    sprintf(temp, "消耗%d友情值", m_vctExchangeInfo[i]->nCount);
                }
                CCLabelTTF* pLabel1 = CCLabelTTF::create(temp, "Arial-BoldMT", 24);
                pLabel1 -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 242));
                pExchangeBg -> addChild(pLabel1);
                sprintf(temp, "%d元京东卡", m_vctExchangeInfo[i]->nMoney);
                CCLabelTTF* pLabel2 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabel2 -> setPosition(ccp(pExchangeItem->getContentSize().width/2, pExchangeItem->getContentSize().height/2));
                pExchangeItem -> addChild(pLabel2);
                pLabel2 -> setColor(ccc3(151, 39, 17));
                m_arrShareMenu -> addObject(pMenu);
            }
            if(m_vctExchangeInfo[i]->nType == 1)//电话卡
            {
                CCSprite* pExchangeBg = CCSprite::create("share/sharetelcardbg.png");
                pExchangeBg -> setPosition(ccp(115+230*i, m_layerExchange->getContentSize().height/2));
                m_layerExchange -> addChild(pExchangeBg);
                CCMenuItemImage* pExchangeItem = CCMenuItemImage::create("share/shareitem9.png", "share/shareitem9.png", this, menu_selector(ShareLayer::clickExchange));
                pExchangeItem -> setTag(i);
                pExchangeItem -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 50));
                MyMenu* pMenu = MyMenu::create(pExchangeItem, NULL);
                pMenu -> setPosition(CCPointZero);
                pMenu -> setAnchorPoint(CCPointZero);
                pMenu -> setTouchPriority(-132);
                pExchangeBg -> addChild(pMenu);
                if(m_vctExchangeInfo[i]->nCount >= 10000)
                {
                    sprintf(temp, "消耗%d万友情值", m_vctExchangeInfo[i]->nCount/10000);
                }
                else
                {
                    sprintf(temp, "消耗%d友情值", m_vctExchangeInfo[i]->nCount);
                }
                CCLabelTTF* pLabel1 = CCLabelTTF::create(temp, "Arial-BoldMT", 24);
                pLabel1 -> setPosition(ccp(pExchangeBg->getContentSize().width/2, 242));
                pExchangeBg -> addChild(pLabel1);
                sprintf(temp, "%d元话费卡", m_vctExchangeInfo[i]->nMoney);
                CCLabelTTF* pLabel2 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabel2 -> setPosition(ccp(pExchangeItem->getContentSize().width/2, pExchangeItem->getContentSize().height/2));
                pExchangeItem -> addChild(pLabel2);
                pLabel2 -> setColor(ccc3(151, 39, 17));
                m_arrShareMenu -> addObject(pMenu);
            }
        }
    }
    
}

void ShareLayer::clickExchange(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    item -> setScale(1.0f);
    item->runAction(CCSequence::create(CCScaleTo::create(0.1f, 0.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    
    int nTag = item->getTag();
    if(nTag > m_vctExchangeInfo.size()-1)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，兑换失败，请联系客服或者请稍后重试！", eMBOK);
        return ;
    }
    if(m_vctExchangeInfo[nTag]->nType == 0)//兑换京东卡
    {
        //先判断是不是达到可以兑换的条件
        if(m_vctExchangeInfo[nTag]->nCount > atoi(m_labelCurrent->getString()))
        {
            CMainLogic::sharedMainLogic()->ShowMessage("对不起，您当前友情值没有达到兑换奖励的条件", eMBOK);
            return ;
        }
        sendExchangeInfo(m_vctExchangeInfo[nTag]->nID);
    }
    else if(m_vctExchangeInfo[nTag]->nType == 1)//兑换话费卡
    {
        //先判断是不是达到可以兑换的条件
        if(m_vctExchangeInfo[nTag]->nCount > atoi(m_labelCurrent->getString()))
        {
            CMainLogic::sharedMainLogic()->ShowMessage("对不起，您当前友情值没有达到兑换奖励的条件", eMBOK);
            return ;
        }
        m_bShowExchangeCard = true;
        //屏蔽兑换界面的触摸（滚动视图和按钮）
        for(int i = 0; i < m_arrShareMenu->count(); i++)
        {
            CCMenu* pMenu = (CCMenu*)m_arrShareMenu->objectAtIndex(i);
            if(pMenu != NULL)
            {
                pMenu -> setTouchEnabled(false);
            }
        }
        m_scrollExchange -> setTouchEnabled(false);
        //显示话费选项卡界面
        m_spExchange2Bg -> setVisible(true);
        m_layerColor -> setVisible(true);
        m_nSelectCardType = m_vctExchangeInfo[nTag]->nID;
    }
    
}

void ShareLayer::sendExchangeInfo(int nID)
{
    char temp[128];
    sprintf(temp, "http://%s/GetFriendValue.aspx?uid=%d&value=%d", NORMAL_URL, CMainLogic::sharedMainLogic()->m_nUserID, nID);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(ShareLayer::onHttpExchangeComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
    CMainLogic::sharedMainLogic()->ShowMessage("正在兑换，请稍候...", eMBNull);
}

void ShareLayer::onHttpExchangeComplete(CCHttpClient* pSender, CCHttpResponse* response)
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
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement == NULL) return;
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("GiftInfo"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL) {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "data"))//相同
            {
                std::string sResult = pCurrentElement->Attribute("result");
                if(!atoi(sResult.c_str()))//0成功，1失败
                {
                    //更新当前友情值
                    std::string sNowValue = pCurrentElement->Attribute("nowvalue");
                    m_labelCurrent -> setString(sNowValue.c_str());
                    m_labelMyFriend -> setString(sNowValue.c_str());
                }
                else
                {
                    
                }
                std::string sInfo = pCurrentElement->Attribute("Info");
                CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK);
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
}

void ShareLayer::clickMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    item -> setScale(1.0f);
    item->runAction(CCSequence::create(CCScaleTo::create(0.1f, 0.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
    if(item->getTag() == 101)//兑换奖励
    {
        m_spShareBg -> setVisible(false);
        m_spExchangeBg -> setVisible(true);
        m_spExchangeBg -> runAction(CCSequence::create(CCFadeIn::create(0.01f), CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareExchangeTag);
    }
    else if(item->getTag() == 102)//活动说明
    {
        m_spShareBg -> setVisible(false);
        m_spInfoBg -> setVisible(true);
        m_spInfoBg -> runAction(CCSequence::create(CCFadeIn::create(0.01f), CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareInfoTag);
    }
    else if(item->getTag() == 103)//新手礼包
    {
        m_spShareBg -> setVisible(false);
        m_spCodeBg -> setVisible(true);
        m_spCodeBg->runAction(CCSequence::create(CCFadeIn::create(0.01f), CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareNewGiftTag);
    }
    else if(item->getTag() == 104)//邀请好友
    {
        m_spShareBg -> setVisible(false);
        m_spFriendBg -> setVisible(true);
        m_spFriendBg->runAction(CCSequence::create(CCFadeIn::create(0.01f), CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareFriendTag);
    }
    else if(item->getTag() == 105)//微信好友
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareWX1Tag);
        shareWX(0);
    }
    else if(item->getTag() == 106)//微信朋友圈
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareWX2Tag);
        shareWX(1);
    }
    else if(item->getTag() == 107)//QQ好友
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareQQTag);
        shareQQ();
    }
    else if(item->getTag() == 108)//复制功能
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareCopyTag);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChargeWebView::copy(m_labelCode->getString());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo info;
        bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
        jobject jobj;
        if(bHave)
        {
            jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
            bool bHave2 = false;
            bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","copy", "(Ljava/lang/String;)V");
            if(bHave2)
            {
                jstring sGongZhonghao = info.env->NewStringUTF(m_labelCode->getString());
                info.env->CallVoidMethod(jobj, info.methodID, sGongZhonghao);
            }
        }
#endif
        CMainLogic::sharedMainLogic()->ShowMessage("邀请码复制成功！请在分享面板中粘贴邀请码方便好友输入", eMBOK);
    }
    else if(item->getTag() == 109)//粘贴功能
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareNewGiftPasteTag);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        std::string str = ChargeWebView::checkPaste();
        m_editBoxCode->setText(str.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo info;
        bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
        jobject jobj;
        if(bHave)
        {
            jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
            bool bHave2 = false;
            bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","paste", "()V");
            if(bHave2)
            {
                info.env->CallVoidMethod(jobj, info.methodID);
            }
        }
#endif
    }
    else if(item->getTag() == 110)//确认领取
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareNewGiftGetTag);
        std::string str = m_editBoxCode->getText();
        if(str == "")
        {
            CMainLogic::sharedMainLogic()->ShowMessage("请您先输入邀请码", eMBOK);
            return ;
        }
        char temp[128];
        sprintf(temp, "http://%s/GetFriendGift.aspx?uid=%d&ShareCode=%s", NORMAL_URL, CMainLogic::sharedMainLogic()->m_nUserID, str.c_str());
        CCHttpRequest* request = new CCHttpRequest();
        request -> setUrl(temp);
        printf("temp url is %s\n", temp);
        request -> setRequestType(CCHttpRequest::kHttpGet);
        request -> setResponseCallback(this, httpresponse_selector(ShareLayer::onHttpGuideGiftComplete));
        CCHttpClient::getInstance()->send(request);
        request -> release();
        CMainLogic::sharedMainLogic()->ShowMessage("正在领取，请稍候...", eMBNull);
    }
    else if(item->getTag() == 111)//兑换移动话费卡
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareExchangeGiftTag);
        sendExchangeInfo(m_nSelectCardType);
    }
    else if(item->getTag() == 112)//兑换联通话费卡
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareExchangeGiftTag);
        sendExchangeInfo(m_nSelectCardType+1);
    }
    else if(item->getTag() == 113)//兑换电信话费卡
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareExchangeGiftTag);
        sendExchangeInfo(m_nSelectCardType+2);
    }
}

void ShareLayer::onHttpGuideGiftComplete(CCHttpClient* pSender, CCHttpResponse* response)
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
    printf("str result is %s\n", str.c_str());
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement == NULL) return;
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("GiftInfo"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL) {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "data"))//相同
            {
                std::string sResult = pCurrentElement->Attribute("IsSucess");
                if(!atoi(sResult.c_str()))//0成功，1失败
                {
                    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
                    GetPriseLayer* pPriseLayer = GetPriseLayer::create(1, atoi(m_labelScore->getString()), "");
                    CCDirector::sharedDirector()->getRunningScene()->addChild(pPriseLayer, 15);
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        std::string sScore = pCurrentElement->Attribute("Score");
                        CMainLogic::sharedMainLogic()->m_lUserScore = atoll(sScore.c_str());
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer -> playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                            SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                            pHallLayer -> setUserScore(CMainLogic::sharedMainLogic()->m_lUserScore);
                            this -> scheduleOnce(schedule_selector(ShareLayer::closeGuideGiftLayer), 3.5f);
                            //隐藏新手引导按钮
                            m_menuGuideGift -> setVisible(false);
                        }
                    }
                }
                else
                {
                    std::string sInfo = pCurrentElement->Attribute("Info");
                    CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK);
                }
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
}

void ShareLayer::closeGuideGiftLayer()
{
    CCMenuItem* item = CCMenuItem::create();
    item -> setTag(206);
    clickClose(item);
}

void ShareLayer::scheduleDelayShowPaste()
{
    setPasteContent(m_sPasteContent);
}

void ShareLayer::setPasteContent(std::string str)
{
    m_editBoxCode->setText(str.c_str());
}

void ShareLayer::shareWX(int nType)
{
    CMainLogic::sharedMainLogic()->m_nShareInterface = 5;
    //只做微信分享
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string str = ChargeWebView::checkPaste();
    if(str == "" || str != m_labelCode->getString())
    {
        CMainLogic::sharedMainLogic()->ShowMessage("请您先复制邀请码", eMBOK);
        return ;
    }
    if(ChargeWebView::checkWX())
    {
        CMainLogic::sharedMainLogic()->m_bSharedState = true;
        WeiXinCharge = 2;
        ChargeWebView::openIOSShare(m_sShareUrl, m_sShareContent, nType);//0代表微信好友，1代表朋友圈
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
                jstring jStr1 = minfo.env->NewStringUTF(m_sShareUrl.c_str());
                jstring jStr2 = minfo.env->NewStringUTF(m_sShareContent.c_str());
                jint type = nType;
                //调用java非静态函数, 参数1：Java对象，上面已经取得   参数2：方法ID
                minfo.env->CallVoidMethod(jobj, minfo.methodID, jStr1, jStr2, type);
            }
        }
    }
#endif
}

void ShareLayer::shareQQ()
{
    CMainLogic::sharedMainLogic()->m_nShareInterface = 5;
    //只做微信分享
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string str = ChargeWebView::checkPaste();
    if(str == "" || str != m_labelCode->getString())
    {
        CMainLogic::sharedMainLogic()->ShowMessage("请您先复制邀请码", eMBOK);
        return ;
    }
    if(ChargeWebView::checkQQ())
    {
        CMainLogic::sharedMainLogic()->m_bSharedState = true;
        WeiXinCharge = 2;
        ChargeWebView::openQQShare(m_sShareUrl, m_sShareContent, 0);//0代表QQ好友，1代表空间
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，检测到您还未安装QQ客户端，分享失败", eMBOK);
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
            isHave = JniHelper::getMethodInfo(minfo,"cn/sharesdk/hellocpp/HelloCpp","onClickShare","(Ljava/lang/String;Ljava/lang/String;)V");
            if (isHave)
            {
                jstring jStr1 = minfo.env->NewStringUTF(m_sShareUrl.c_str());
                jstring jStr2 = minfo.env->NewStringUTF(m_sShareContent.c_str());
//                jint type = nType;
                //调用java非静态函数, 参数1：Java对象，上面已经取得   参数2：方法ID
                minfo.env->CallVoidMethod(jobj, minfo.methodID, jStr1, jStr2);
            }
        }
    }
#endif
}

void ShareLayer::clickClose(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    if(item->getTag() == 201)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareCloseTag);
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(ShareLayer::removeWnd));
        m_spShareBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
    }
    else if(item->getTag() == 202)//关闭邀请好友框，显示主界面
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareFriendCloseTag);
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
        m_spFriendBg->setVisible(false);
        m_spShareBg->setVisible(true);
        m_spShareBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
    }
    else if(item->getTag() == 203)//关闭新手领取奖励界面
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareNewGiftCloseTag);
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
        m_spCodeBg->setVisible(false);
        m_spShareBg->setVisible(true);
        m_spShareBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
        this -> unschedule(schedule_selector(ShareLayer::closeGuideGiftLayer));
    }
    else if(item->getTag() == 204)//关闭活动说明界面
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareInfoCloseTag);
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
        m_spInfoBg->setVisible(false);
        m_spShareBg->setVisible(true);
        m_spShareBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
    }
    else if(item->getTag() == 205)//关闭兑换界面
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareExchangeCloseTag);
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
        m_spExchangeBg->setVisible(false);
        m_spShareBg->setVisible(true);
        m_spShareBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
    }
    else if(item->getTag() == 206)//默认关闭新手奖励框
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eShareNewGiftCloseTag);
        m_spCodeBg->setVisible(false);
        m_spShareBg->setVisible(true);
        m_spShareBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
    }
}

void ShareLayer::removeWnd()
{
    this -> removeFromParent();
}

void ShareLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool ShareLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if(m_bShowExchangeCard)
    {
        CCPoint point = pTouch->getLocation();
        point = m_spExchangeBg->convertToNodeSpace(point);
        CCRect rect = m_spExchange2Bg->boundingBox();
        if(!rect.containsPoint(point))
        {
            m_spExchange2Bg -> setVisible(false);
            m_layerColor -> setVisible(false);
            m_bShowExchangeCard = false;
            //打开兑换界面上滚动和按钮的触摸
            m_scrollExchange -> setTouchEnabled(true);
            for(int i = 0; i < m_arrShareMenu->count(); i++)
            {
                CCMenu* pMenu = (CCMenu*)m_arrShareMenu->objectAtIndex(i);
                if(pMenu != NULL)
                {
                    pMenu -> setTouchEnabled(true);
                }
            }
        }
    }
    return true;
}
