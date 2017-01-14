//
//  RankNewLayer.cpp
//  CatchFish3
//
//  Created by Guang on 16/5/29.
//
//

#include "header.h"


enum matchTag{
    eBackTag = 1001,
    eOnGoingTag,
    eMyEnrolTag,
    eMyMatchTag,
    eRankTag,
    eJoinFreeTag,
    eJoinCoinTag,
    eMtachAwardLingQu,
    eMatchInstructionTag,
    eMatchNodeRule1Tag,
    eMatchNodeRule2Tag,
    eMatchNodeRule3Tag,
    eMatchNodePersonNumTag,
    eMatchNodeJoinScoreTag,
    eMatchNodeJoinGroupFont,
    eMatchNodeJoinScoreFont,
    eMatchNodeJoinNumFont,
    eMatchNodeJoinPercentTag,
    eMatchNodeJoinRoom,
    eMatchNodeJoin2Room,
};

enum RankTag{
    eRankMeIndex=100,
    eRankMePaimingSp,
    eRankMeIndexTTF,
    eRankMeVipSP,
    eRankMeVipLevel,
    eRankMeNichengSp,
    eRankMeNickname,
    eRankMeScoreSp,
    eRankMeScore,
};

#if defined(IOS_ANDROID)

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
extern int WeiXinCharge;
#endif

#endif

CRankNewLayer::~CRankNewLayer()
{
    m_MatchingVct.clear();
    m_MatchJoinedVct.clear();
    m_MatchEndVct.clear();
    m_MatchRankVct.clear();
    
    m_pContainTouchItemArray -> release();
    m_pMatchNodeAwardArray -> release();
}

bool CRankNewLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("rank/SelectItemAct.plist");
    
    m_bHistoryRequst=false;
    m_bRankRequest=false;
    m_bJoinAct=false;
    m_bJoinedFirstRequest=true;
    m_bJoinStatus = false;
    m_bJoinRefresh = false;
    m_bClickTouch = false;
    m_nIndexPage = 1;
    m_nShowItem = 0;
    m_nGuangPageID = 0;
    m_nGuangItemID = 0;
    m_nLingQuMatchID = -1;
    m_nJoinMatchID = -1;
    m_MatchingVct.clear();
    m_MatchJoinedVct.clear();
    m_MatchEndVct.clear();
    m_MatchRankVct.clear();
    
    m_pContainTouchItemArray = CCArray::create();
    m_pContainTouchItemArray -> retain();
    m_pMatchNodeAwardArray = CCArray::create();
    m_pMatchNodeAwardArray -> retain();
    
    CCLayerColor* BgLayer = CCLayerColor::create(ccc4(0, 0, 0, 100));
    this->addChild(BgLayer, 1);
    
    CCUserDefault::sharedUserDefault()->setBoolForKey("ShowMatchGuide", false);
    
    loadUI();
    
    this->setTouchEnabled(true);
    
    return true;
}

void CRankNewLayer::loadUI()
{
    m_pSpMatchBg = CCSprite::create();
    m_pSpMatchBg->setAnchorPoint(ccp(0, 0));
    m_pSpMatchBg->setPosition(ccp(0, 0));
    this->addChild(m_pSpMatchBg, 1);
    
    CCSprite* pBg = CCSprite::create("rank/matchBg.png");
    pBg->setScale(2.0f);
    pBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    m_pSpMatchBg->addChild(pBg, 1);
    
    m_nodeMenuStatus = CCNode::create();
    this -> addChild(m_nodeMenuStatus, 100);
    CCMenuItemImage* openItem = CCMenuItemImage::create("competition/bisaikai.png", "competition/bisaikai.png", this, menu_selector(CRankNewLayer::setBisaiOpenStatus));
    openItem -> setTag(101);
    openItem -> setPosition(ccp(CLIENT_WIDTH-openItem->getContentSize().width+10, 70));
    CCMenuItemImage* bisaiCloseItem = CCMenuItemImage::create("competition/bisaiguan.png", "competition/bisaiguan.png", this, menu_selector(CRankNewLayer::setBisaiOpenStatus));
    bisaiCloseItem -> setTag(102);
    bisaiCloseItem -> setPosition(openItem->getPosition());
    m_menuOpenStatus = CCMenu::create(openItem, NULL);
    m_menuOpenStatus -> setPosition(CCPointZero);
    m_menuOpenStatus -> setAnchorPoint(CCPointZero);
    m_nodeMenuStatus -> addChild(m_menuOpenStatus, 100);
    m_menuCloseStatus = CCMenu::create(bisaiCloseItem, NULL);
    m_menuCloseStatus -> setPosition(CCPointZero);
    m_menuCloseStatus -> setAnchorPoint(CCPointZero);
    m_nodeMenuStatus -> addChild(m_menuCloseStatus, 100);
    m_menuOpenStatus -> setTouchPriority(-200);
    m_menuCloseStatus -> setTouchPriority(-200);
    if(CMainLogic::sharedMainLogic()->m_bShowNotifyInfo)
    {
        m_menuCloseStatus -> setVisible(false);
    }
    else
    {
        m_menuOpenStatus -> setVisible(false);
    }
    
    CCMenuItemImage* onGoing = CCMenuItemImage::create("rank/matchHeadSelect.png", "rank/matchHeadSelect.png",this,menu_selector(CRankNewLayer::callBack));
    CCMenuItemImage* myEnrol = CCMenuItemImage::create("rank/matchHeadSelect.png", "rank/matchHeadSelect.png",this,menu_selector(CRankNewLayer::callBack));
    CCMenuItemImage* myMatch = CCMenuItemImage::create("rank/matchHeadSelect.png", "rank/matchHeadSelect.png",this,menu_selector(CRankNewLayer::callBack));
    CCMenuItemImage* rank = CCMenuItemImage::create("rank/matchHeadSelect.png", "rank/matchHeadSelect.png",this,menu_selector(CRankNewLayer::callBack));
    CCMenuItemImage* back = CCMenuItemImage::create("v2ip/vipcloseitem.png", "v2ip/vipcloseitem.png",this,menu_selector(CRankNewLayer::callBack));
    back->setScale(0.7f);
    onGoing->setTag(eOnGoingTag);
    myEnrol->setTag(eMyEnrolTag);
    myMatch->setTag(eMyMatchTag);
    rank->setTag(eRankTag);
    back->setTag(eBackTag);
    onGoing->setPosition(ccp(151.5, 675));
    myEnrol->setPosition(ccp(onGoing->getPositionX()+onGoing->getContentSize().width-1,onGoing->getPositionY()));
    myMatch->setPosition(ccp(myEnrol->getPositionX()+onGoing->getContentSize().width-1,onGoing->getPositionY()));
    rank->setPosition(ccp(myMatch->getPositionX()+onGoing->getContentSize().width-1,onGoing->getPositionY()));
    back->setPosition(ccp(1237.5, 677.5));
    m_systemMenu = CCMenu::create(onGoing, myEnrol, myMatch, rank, back, NULL);
    m_systemMenu->setPosition(ccp(0, 0));
    m_systemMenu->setTouchPriority(-200);
    this->addChild(m_systemMenu, 10);
    m_pHeadShade = CCSprite::create("rank/matchHeadSelected.png");
    m_pHeadShade->setScaleX(1.04f);
    m_pHeadShade->setPosition(ccp(onGoing->getPositionX(), onGoing->getPositionY()-8));
    this->addChild(m_pHeadShade, 11, 200);
    CCLabelTTF* onGoTTF = CCLabelTTF::create("进行中", "Arial", 34);//Arial-BoldMT
    CCLabelTTF* enrolTTF = CCLabelTTF::create("我的报名", "Arial", 34);
    CCLabelTTF* matchTTF = CCLabelTTF::create("我的赛况", "Arial", 34);
    CCLabelTTF* rankTTF = CCLabelTTF::create("名人堂", "Arial", 34);
    onGoTTF->setColor(ccBLACK);
    enrolTTF->setColor(ccBLACK);
    matchTTF->setColor(ccBLACK);
    rankTTF->setColor(ccBLACK);
    onGoTTF->setPosition(onGoing->getPosition());
    enrolTTF->setPosition(myEnrol->getPosition());
    matchTTF->setPosition(myMatch->getPosition());
    rankTTF->setPosition(rank->getPosition());
    this->addChild(onGoTTF, 12);
    this->addChild(enrolTTF, 12);
    this->addChild(matchTTF, 12);
    this->addChild(rankTTF, 12);
    
    m_pNoLingQu = CCSprite::create("CatchFish2001/jiaobiaodi.png");
    m_pNoLingQu->setScale(1.5f);
    m_pNoLingQu -> setPosition(ccp(matchTTF->getContentSize().width+30, matchTTF->getContentSize().height-5));
    matchTTF -> addChild(m_pNoLingQu);
    m_pNoLingQu->setVisible(false);
    m_pNumNoLingQuTTF = CCLabelTTF::create("", "Arial", 20);
    m_pNumNoLingQuTTF->setColor(ccWHITE);
    m_pNumNoLingQuTTF->setPosition(ccp(m_pNoLingQu->getContentSize().width/2, m_pNoLingQu->getContentSize().height/2));
    m_pNoLingQu->addChild(m_pNumNoLingQuTTF);
    
    loadMatchUI();
    loadRankUI();
}

void CRankNewLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -161, true);
}

bool CRankNewLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    m_startPoint=pTouch->getLocation();
    return true;
}

void CRankNewLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(!m_bClickTouch)
    {
        return ;
    }
    CCPoint endPoint = pTouch->getLocation();
    
    if(abs((int)m_startPoint.x-(int)endPoint.x)>15 || abs((int)m_startPoint.y-(int)endPoint.y)>15)
    {
        return ;
    }
    else
    {
        if (m_pMatchContainLayer == NULL)
        {
            return ;
        }
        CCRect containLayerRect = m_pMatchContainLayer->boundingBox();
        if (containLayerRect.containsPoint(endPoint))
        {
            CCPoint point = m_pMatchContainLayer->convertToNodeSpace(endPoint);
            for(int i = 0; i < m_pContainTouchItemArray->count(); i++)
            {
                CCSprite* pSpDi = (CCSprite*)m_pContainTouchItemArray->objectAtIndex(i);
                CCRect rect = pSpDi->boundingBox();
                if(rect.containsPoint(point))
                {
                    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                    pSpDi->setVisible(false);
                    m_pItemWhiteShade->setPosition(pSpDi->getPosition());
                    m_pItemWhiteShade->setVisible(true);
                    
                    if (m_nIndexPage == 1 || m_nIndexPage == 2 || m_nIndexPage == 3)
                    {
                        setLeftMatchNode(i);
                        playSelectItemAnimate(pSpDi->getPosition(), m_nIndexPage, i);
                    }
                    
                    if (m_nIndexPage == 3)//分享的界面，要判断当前是否已经分享
                    {
                        if (m_MatchEndVct.size() <= 0)
                        {
                            return;
                        }
                        if (m_MatchEndVct[i]->nGet == 0)
                        {
                            setMenuItemVisible(eMtachAwardLingQu, true, m_matchMenu);
                            m_pSpLingQued->setVisible(false);
                        }
                        else if (m_MatchEndVct[i]->nGet == 1)
                        {
                            setMenuItemVisible(eMtachAwardLingQu, false, m_matchMenu);
                            m_pSpLingQued->setVisible(true);
                        }
                        else if (m_MatchEndVct[i]->nGet == 2)
                        {
                            setMenuItemVisible(eMtachAwardLingQu, false, m_matchMenu);
                            m_pSpLingQued->setVisible(false);
                        }
                    }
                }
                else
                {
                    pSpDi->setVisible(true);
                }
            }
        }
    }
}

void CRankNewLayer::requestCatchFishMatchingHttp()
{
    char sMatchingUrl[256];
    sprintf(sMatchingUrl, "http://%s/GetMacthInfo.aspx?uid=%d", NORMAL_URL, CMainLogic::sharedMainLogic()->m_nUserID);
    CCHttpRequest* request = new CCHttpRequest();
    printf("url ii is %s\n", sMatchingUrl);
    request -> setUrl(sMatchingUrl);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(CRankNewLayer::onHttpCatchFishMatchingComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CRankNewLayer::onHttpCatchFishMatchingComplete(CCHttpClient* sender, CCHttpResponse* response)
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
        TiXmlElement* rootElement = document->RootElement();//Root
        if (rootElement == NULL)
        {
            return;
        }
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("mach"))
            return;
        TiXmlElement* pChildElement=rootElement->FirstChildElement();
        
        //对之前的信息重置（10分钟更新一次）
        m_MatchEndVct.clear();
        
        while (pChildElement!=NULL)
        {
            TiXmlElement * pCurrentElement=pChildElement;
            const char * pValue=NULL;
            nodename=pCurrentElement->Value();
            if (0==nodename.compare("machInfo"))
            {
                pValue=pCurrentElement->Attribute("value");
                if (!strcmp("true",pValue))
                {
                    NewMatchingVct* pNewMatchingVct= new NewMatchingVct;
                    pNewMatchingVct->nMatchType = atoi(pCurrentElement->Attribute("type"));
                    TiXmlElement* pNodeElement=pCurrentElement->FirstChildElement();
                    while (pNodeElement != NULL)
                    {
                        pValue=pNodeElement->Value();
                        if (!strcmp(pValue, "name"))
                        {
                            pNewMatchingVct->sMatchName = pNodeElement->GetText();
//                            pNewMatchingVct->nHuaFeiNum = selectHuaFeiNum(pNewMatchingVct->sMatchName);
                        }
                        else if (!strcmp(pValue, "num"))
                        {
                            pNewMatchingVct->sMatchNum = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "machnum"))
                        {
                            pNewMatchingVct->sMatchIndex = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "time"))
                        {
                            pNewMatchingVct->sMatchTime = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "cost"))
                        {
                            pNewMatchingVct->sMatchCost = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "count"))
                        {
                            pNewMatchingVct->sMatchGroupNum = pNodeElement->GetText();
                            pNewMatchingVct->nMatchGroupNum = atoi(pNodeElement->Attribute("value"));
                        }
                        else if (!strcmp(pValue, "signup"))
                        {
                            pValue = pNodeElement->GetText();
                            if (!strcmp(pValue, "True"))
                            {
                                pNewMatchingVct->bSignup = true;
                            }
                            else{
                                pNewMatchingVct->bSignup = false;
                            }
                        }
                        else if (!strcmp(pValue, "joinednum"))
                        {
                            pNewMatchingVct->sMatchJoinedNum = pNodeElement->GetText();
                            pNewMatchingVct->nMatchJoinedNum = atoi(pNodeElement->Attribute("value"));
                        }
                        else if (!strcmp(pValue, "joinroom"))
                        {
                            pNewMatchingVct->sJoinRoomName = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "takeremainder"))
                        {
                            pNewMatchingVct->sMatchTakeremainder = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "second"))
                        {
                            pNewMatchingVct->nSecond = (atoi)(pNodeElement->GetText());
                        }
                        else if (!strcmp(pValue, "remark1"))
                        {
                            pNewMatchingVct->sMatchRule1 = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "remark2"))
                        {
                            pNewMatchingVct->sMatchRule2 = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "remark3"))
                        {
                            pNewMatchingVct->sMatchRule3 = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "machaward"))
                        {
                            TiXmlElement * pNodeElement1=pNodeElement->FirstChildElement();
                            nodename = pNodeElement1->Value();
                            if (!strcmp(nodename.c_str(), "num"))
                            {
                                pNewMatchingVct->sFirst=pNodeElement1->Attribute("rank");
                                pNewMatchingVct->sAwardFirst=pNodeElement1->GetText();
                            }
                            TiXmlElement * pNodeElement2=pNodeElement1->NextSiblingElement();
                            nodename = pNodeElement2->Value();
                            if (!strcmp(nodename.c_str(), "num"))
                            {
                                pNewMatchingVct->sSeconde=pNodeElement2->Attribute("rank");
                                pNewMatchingVct->sAwardSecond=pNodeElement2->GetText();
                            }
                            TiXmlElement * pNodeElement3=pNodeElement2->NextSiblingElement();
                            nodename = pNodeElement3->Value();
                            
                            if (!strcmp(nodename.c_str(), "num"))
                            {
                                pNewMatchingVct->sThird=pNodeElement3->Attribute("rank");
                                pNewMatchingVct->sAwardThird=pNodeElement3->GetText();
                            }
                            TiXmlElement * pNodeElement4=pNodeElement3->NextSiblingElement();
                            nodename = pNodeElement4->Value();
                            if (!strcmp(nodename.c_str(), "num"))
                            {
                                pNewMatchingVct->sFourth=pNodeElement4->Attribute("rank");
                                pNewMatchingVct->sAwardFourth=pNodeElement4->GetText();
                            }
                        }
                        pNodeElement=pNodeElement->NextSiblingElement();
                    }
                    m_MatchingVct.push_back(pNewMatchingVct);
                }
                pChildElement=pChildElement->NextSiblingElement();
            }
            else if (0==nodename.compare("userInfo"))
            {
                pValue = pCurrentElement->Attribute("num");
                if (pValue != NULL) {
                    if (m_pNumNoLingQuTTF != NULL) {
                        m_pNumNoLingQuTTF->setString(pValue);
                        if (atoi(pValue) > 0) {
                            m_pNoLingQu->setVisible(true);
                        }
                        else
                        {
                            m_pNoLingQu->setVisible(false);
                        }
                    }
                    
                }
                pChildElement=pChildElement->NextSiblingElement();
            }
            else
            {
                pChildElement=pChildElement->NextSiblingElement();
            }
        }
    }
    delete document;
printf("---------进行中---------\n");
    for (int i=0; i<(int)m_MatchingVct.size(); i++)
    {
//        printf("%s %s  %d\n",m_MatchingVct[i]->sMatchName.c_str(),m_MatchingVct[i]->sMatchTime.c_str(),m_MatchingVct[i]->nSecond);
    }
    
    m_spAnimate->stopAllActions();
    m_spAnimate->setVisible(false);
    
    
    this->unschedule(schedule_selector(CRankNewLayer::showMyJoinedMatchCountDownTime));
    this->schedule(schedule_selector(CRankNewLayer::showMyJoinedMatchCountDownTime), 1.0f, -1, 0.01f);
    if (m_nIndexPage == 1)
    {
        loadMatchingUI();
    }
}

void CRankNewLayer::requestCatchFishMatchJoinedHttp()
{
    m_bJoinedFirstRequest = false;
    char sMatchJoinedUrl[256];
    sprintf(sMatchJoinedUrl, "http://%s/GetMachApply.aspx?uid=%d", NORMAL_URL, CMainLogic::sharedMainLogic()->m_nUserID);
    printf("url is %s\n", sMatchJoinedUrl);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(sMatchJoinedUrl);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(CRankNewLayer::onHttpCatchFishMatchJoinedComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CRankNewLayer::onHttpCatchFishMatchJoinedComplete(CCHttpClient* sender, CCHttpResponse* response)
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
        TiXmlElement* rootElement = document->RootElement();//Root
        if (rootElement == NULL)
        {
            return;
        }
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("mach"))
            return;
        TiXmlElement* pChildElement=rootElement->FirstChildElement();
        
        //对之前的信息重置（10分钟更新一次）
        m_MatchJoinedVct.clear();
        
        while (pChildElement!=NULL)
        {
            TiXmlElement * pCurrentElement=pChildElement;
            const char * pValue=NULL;
            nodename=pCurrentElement->Value();
            if (0==nodename.compare("machInfo"))
            {
                pValue=pCurrentElement->Attribute("value");
                if (!strcmp("true",pValue))
                {
                    NewMatchJoinedVct* pNewMatchJoinedVct= new NewMatchJoinedVct;
                    pNewMatchJoinedVct->nMatchType = atoi(pCurrentElement->Attribute("type"));
                    TiXmlElement* pNodeElement=pCurrentElement->FirstChildElement();
                    while (pNodeElement != NULL)
                    {
                        pValue=pNodeElement->Value();
                        if (!strcmp(pValue, "name"))
                        {
                            pNewMatchJoinedVct->sMatchName = pNodeElement->GetText();
//                            pNewMatchJoinedVct->nHuaFeiNum = selectHuaFeiNum(pNewMatchJoinedVct->sMatchName);
                        }
                        else if (!strcmp(pValue, "num"))
                        {
                            pNewMatchJoinedVct->sMatchNum = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "machnum"))
                        {
                            pNewMatchJoinedVct->sMatchIndex = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "time"))
                        {
                            pNewMatchJoinedVct->sMatchTime = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "cost"))
                        {
                            pNewMatchJoinedVct->sMatchCost = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "count"))
                        {
                            pNewMatchJoinedVct->sMatchGroupNum = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "joinednum"))
                        {
                            pNewMatchJoinedVct->sMatchJoinedNum = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "second"))
                        {
                            pNewMatchJoinedVct->nSecond = atoi(pNodeElement->GetText());
                        }
                        else if (!strcmp(pValue, "remark1"))
                        {
                            pNewMatchJoinedVct->sMatchRule1 = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "remark2"))
                        {
                            pNewMatchJoinedVct->sMatchRule2 = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "remark3"))
                        {
                            pNewMatchJoinedVct->sMatchRule3 = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "machaward"))
                        {
                            TiXmlElement * pNodeElement1=pNodeElement->FirstChildElement();
                            nodename = pNodeElement1->Value();
                            if (!strcmp(nodename.c_str(), "num"))
                            {
                                pNewMatchJoinedVct->sFirst=pNodeElement1->Attribute("rank");
                                pNewMatchJoinedVct->sAwardFirst=pNodeElement1->GetText();
                            }
                            TiXmlElement * pNodeElement2=pNodeElement1->NextSiblingElement();
                            nodename = pNodeElement2->Value();
                            if (!strcmp(nodename.c_str(), "num"))
                            {
                                pNewMatchJoinedVct->sSeconde=pNodeElement2->Attribute("rank");
                                pNewMatchJoinedVct->sAwardSecond=pNodeElement2->GetText();
                            }
                            TiXmlElement * pNodeElement3=pNodeElement2->NextSiblingElement();
                            nodename = pNodeElement3->Value();
                            if (!strcmp(nodename.c_str(), "num"))
                            {
                                pNewMatchJoinedVct->sThird=pNodeElement3->Attribute("rank");
                                pNewMatchJoinedVct->sAwardThird=pNodeElement3->GetText();
                            }
                            TiXmlElement * pNodeElement4=pNodeElement3->NextSiblingElement();
                            nodename = pNodeElement4->Value();
                            if (!strcmp(nodename.c_str(), "num"))
                            {
                                pNewMatchJoinedVct->sFourth=pNodeElement4->Attribute("rank");
                                pNewMatchJoinedVct->sAwardFourth=pNodeElement4->GetText();
                            }
                        }
                        pNodeElement=pNodeElement->NextSiblingElement();
                    }
                    m_MatchJoinedVct.push_back(pNewMatchJoinedVct);
                }
                pChildElement=pChildElement->NextSiblingElement();
            }
            else
            {
                pChildElement=pChildElement->NextSiblingElement();
            }
        }
    }
    delete document;
    m_bJoinAct = false;
    m_spAnimate->stopAllActions();
    m_spAnimate->setVisible(false);
    
    //this->schedule(schedule_selector(CRankNewLayer::showMyJoinedMatchCountDownTime), 1.0f);
    
    if (m_nIndexPage == 2)
    {
        m_bClickTouch = true;
        loadMatchJoinedUI();
    }
}

void CRankNewLayer::requestCatchFishHistoryMatchHttp()
{
    char sMatchHistoryUrl[256];
    sprintf(sMatchHistoryUrl, "http://%s/GetMathcHisResults.aspx?uid=%d", NORMAL_URL,CMainLogic::sharedMainLogic()->m_nUserID);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(sMatchHistoryUrl);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(::CRankNewLayer::onHttpCatchFishHistoryMatchComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CRankNewLayer::onHttpCatchFishHistoryMatchComplete(CCHttpClient* sender, CCHttpResponse* response)
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
        TiXmlElement* rootElement = document->RootElement();//Root
        if (rootElement == NULL)
        {
            return;
        }
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("mach"))
            return;
        TiXmlElement* pChildElement=rootElement->FirstChildElement();
        
        //对之前的信息重置（10分钟更新一次）
        m_MatchEndVct.clear();
        
        while (pChildElement!=NULL)
        {
            TiXmlElement * pCurrentElement=pChildElement;
            const char * pValue=NULL;
            nodename=pCurrentElement->Value();
            if (0==nodename.compare("machInfo"))
            {
                pValue=pCurrentElement->Attribute("value");
                if (!strcmp("true", pValue))
                {
                    NewMatchEndVct* pNewMatchEndVct= new NewMatchEndVct;
                    pNewMatchEndVct->nMatchType = atoi(pCurrentElement->Attribute("type"));
                    TiXmlElement* pNodeElement=pCurrentElement->FirstChildElement();
                    while (pNodeElement != NULL)
                    {
                        pValue=pNodeElement->Value();
                        if (!strcmp(pValue, "name"))
                        {
                            pNewMatchEndVct->sMatchName = pNodeElement->GetText();
//                            pNewMatchEndVct->nHuaFeiNum = selectHuaFeiNum(pNewMatchEndVct->sMatchName);
                        }
                        else if (!strcmp(pValue, "num"))
                        {
                            pNewMatchEndVct->sMatchNum = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "machnum"))
                        {
                            pNewMatchEndVct->sMatchIndex = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "time"))
                        {
                            pNewMatchEndVct->sMatchTime = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "cost"))
                        {
                            pNewMatchEndVct->sMatchCost = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "count"))
                        {
                            pNewMatchEndVct->sMatchGroupNum = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "machcount"))
                        {
                            pNewMatchEndVct->sMatchJoinedNum = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "isget"))
                        {
                            pNewMatchEndVct->nGet = atoi(pNodeElement->GetText());
                        }
                        else if (!strcmp(pValue, "remark1"))
                        {
                            pNewMatchEndVct->sMatchRule1 = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "remark2"))
                        {
                            pNewMatchEndVct->sMatchRule2 = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "remark3"))
                        {
                            pNewMatchEndVct->sMatchRule3 = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "rank"))
                        {
                            pNewMatchEndVct->sRank = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "score"))
                        {
                            pNewMatchEndVct->sScore = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "award"))
                        {
                            pNewMatchEndVct->sAward = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "isShare"))
                        {
                            pNewMatchEndVct->nShared = atoi(pNodeElement->GetText());
                        }
                        else if (!strcmp(pValue, "machday"))
                        {
                            pNewMatchEndVct->sMatchDay = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "rankn"))
                        {
                            pNewMatchEndVct->nRank = atoi(pNodeElement->GetText());
                        }
                        pNodeElement=pNodeElement->NextSiblingElement();
                    }
                    m_MatchEndVct.push_back(pNewMatchEndVct);
                }
                pChildElement=pChildElement->NextSiblingElement();
            }
            else
            {
                pChildElement=pChildElement->NextSiblingElement();
            }
        }
    }
    delete document;
    
    printf("---------我的赛况---------\n");
    for (int i=0; i<(int)m_MatchEndVct.size(); i++)
    {
//        printf("%s %d\n",m_MatchEndVct[i]->sRank.c_str(),m_MatchEndVct[i]->nShared);
    }
    
    m_spAnimate->stopAllActions();
    m_spAnimate->setVisible(false);
    
    m_bHistoryRequst=true;
    if (m_nIndexPage == 3)
    {
        m_bClickTouch = true;
        loadMatchedUI();
    }
}

void CRankNewLayer::requestCatchFishMatchRankHttp()
{
    char sMatchRankUrl[256];
    sprintf(sMatchRankUrl, "http://%s/GetMrt.aspx?uid=%d", NORMAL_URL, CMainLogic::sharedMainLogic()->m_nUserID);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(sMatchRankUrl);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(CRankNewLayer::onHttpMatchRankComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CRankNewLayer::onHttpMatchRankComplete(CCHttpClient* sender, CCHttpResponse* response)
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
        TiXmlElement* rootElement = document->RootElement();//Root
        if (rootElement == NULL)
        {
            return;
        }
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("ranklist"))
            return;
        TiXmlElement* pChildElement=rootElement->FirstChildElement();
        
        m_MatchRankVct.clear();
        while (pChildElement!=NULL)
        {
            TiXmlElement * pCurrentElement=pChildElement;
            const char * pValue=NULL;
            nodename=pCurrentElement->Value();
            if (0==nodename.compare("userInfo"))
            {
                pValue=pCurrentElement->Attribute("num");
                if (NULL!=pValue)
                {
                    int nRankIndex = atoi(pValue);
                    //排行字体
                    CCLabelTTF* pMyRankIndexTTF = (CCLabelTTF*)m_pSpRankBg->getChildByTag(eRankMeIndexTTF);
                    if (pMyRankIndexTTF != NULL)
                    {
                        if (nRankIndex == 1000)
                        {
                            pMyRankIndexTTF->setString("千里之外");
                        }
                        else
                        {
                            pMyRankIndexTTF->setString(pValue);
                        }
                        pMyRankIndexTTF->setVisible(true);
                    }
                }
                pValue=pCurrentElement->Attribute("nickname");
                if (NULL!=pValue)
                {
                    CCLabelTTF* pMyNickNameTTF = (CCLabelTTF*)m_pSpRankBg->getChildByTag(eRankMeNickname);
                    if (pMyNickNameTTF != NULL)
                    {
                        pMyNickNameTTF->setString(CMainLogic::sharedMainLogic()->m_sNickName.c_str());
                    }
                }
                pValue=pCurrentElement->Attribute("count");
                if (NULL!=pValue)
                {
                    CCLabelTTF* pMyHuaFeiTTF = (CCLabelTTF*)m_pSpRankBg->getChildByTag(eRankMeScore);
                    if (pMyHuaFeiTTF != NULL)
                    {
                        std::string sScore = "元";
                        sScore = pValue+sScore;
                        pMyHuaFeiTTF->setString(sScore.c_str());
                    }
                }
            }
            else if(0==nodename.compare("SuInfo"))
            {
                NewMatchRankVct* pNewMatchRankVct = new NewMatchRankVct;
                TiXmlElement* pElment = pCurrentElement->FirstChildElement();
                TiXmlElement* pElment2 = pElment->NextSiblingElement();
                const char * pValue2=NULL;
                pValue2=pElment->Value();
                if (!strcmp(pValue2, "num"))
                {
                    pNewMatchRankVct->sRank = pElment->GetText();
                }
                pValue2=pElment2->Value();
                if (!strcmp(pValue2, "nickname"))
                {
                    pNewMatchRankVct->sNickName = pElment2->GetText();
                }
                TiXmlElement* pElment3 = pElment2->NextSiblingElement();
                pValue2=pElment3->Value();
                if (!strcmp(pValue2, "count"))
                {
                    pNewMatchRankVct->sScore = pElment3->GetText();
                }
                TiXmlElement* pElment4 = pElment3->NextSiblingElement();
                pValue2=pElment4->Value();
                if (!strcmp(pValue2, "viplevel"))
                {
                    pNewMatchRankVct->sUserVipLevel = pElment4->GetText();
                }
                m_MatchRankVct.push_back(pNewMatchRankVct);
            }
            pChildElement=pChildElement->NextSiblingElement();
        }
    }
    delete document;
    
    m_spAnimate->stopAllActions();
    m_spAnimate->setVisible(false);;
    
    printf("---------名人堂---------\n");
    
    if (m_MatchRankVct.size()<=0)
    {
        m_pNoRankInfoTTF->setVisible(true);
        m_pSpRankChildBg->setVisible(false);
        m_pSpRankBg->setVisible(true);
    }
    else
    {
        for (int i=0; i<m_MatchRankVct.size(); i++)
        {
            int nItemLevel = atoi(m_MatchRankVct[i]->sUserVipLevel.c_str());
            CCLabelAtlas* pVipLevelTTF = (CCLabelAtlas*)m_pRankContainLayer->getChildByTag(100+i);
            if (pVipLevelTTF != NULL)
            {
                pVipLevelTTF->setString(m_MatchRankVct[i]->sUserVipLevel.c_str());
                pVipLevelTTF->setVisible(true);
            }
            
            CCSprite* pSpVip = (CCSprite*)m_pRankContainLayer->getChildByTag(400+i);
            if (pSpVip != NULL)
            {
                pSpVip->setVisible(true);
            }
            
            CCLabelTTF* pNickNameTTF = (CCLabelTTF*)m_pRankContainLayer->getChildByTag(200+i);
            if (pNickNameTTF != NULL)
            {
                if (nItemLevel<=0) {
                    pNickNameTTF->setColor(ccc3(101, 51, 23));
                }
                else
                {
                    pNickNameTTF->setColor(ccRED);
                }
                pNickNameTTF->setString(m_MatchRankVct[i]->sNickName.c_str());
            }
            
            CCLabelTTF* pHuaFeiTTF = (CCLabelTTF*)m_pRankContainLayer->getChildByTag(300+i);
            if (pHuaFeiTTF != NULL)
            {
                m_MatchRankVct[i]->sScore = m_MatchRankVct[i]->sScore+"元";
                pHuaFeiTTF->setString(m_MatchRankVct[i]->sScore.c_str());
            }
        }
        
        if (m_nIndexPage == 4)
        {
            m_pNoRankInfoTTF->setVisible(false);
            m_pSpRankChildBg->setVisible(true);
            m_pSpRankBg->setVisible(true);
        }
    }
    m_bRankRequest=true;
}

void CRankNewLayer::requestCatchFishFinishJoinMatchHttp()
{
    char sFinishJoinMatchUrl[256];
    sprintf(sFinishJoinMatchUrl, "http://%s/GetMacthInfo.aspx?uid=%d", NORMAL_URL, CMainLogic::sharedMainLogic()->m_nUserID);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(sFinishJoinMatchUrl);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(CRankNewLayer::onHttpCatchFishFinishJoinMatchComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CRankNewLayer::onHttpCatchFishFinishJoinMatchComplete(CCHttpClient* sender, CCHttpResponse* response)
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
        TiXmlElement* rootElement = document->RootElement();//Root
        if (rootElement == NULL)
        {
            return;
        }
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("mach"))
            return;
        TiXmlElement* pChildElement=rootElement->FirstChildElement();
        while (pChildElement!=NULL)
        {
            TiXmlElement * pCurrentElement=pChildElement;
            const char * pValue=NULL;
            nodename=pCurrentElement->Value();
            if (0==nodename.compare("machInfo"))
            {
                pValue=pCurrentElement->Attribute("value");
                if (!strcmp("true",pValue))
                {
                    NewMatchingVct* pNewMatchingVct= new NewMatchingVct;
                    TiXmlElement* pNodeElement=pCurrentElement->FirstChildElement();
                    while (pNodeElement != NULL)
                    {
                        pValue=pNodeElement->Value();
                        if (!strcmp(pValue, "name"))
                        {
                            pNewMatchingVct->sMatchName = pNodeElement->GetText();
//                            pNewMatchingVct->nHuaFeiNum = selectHuaFeiNum(pNewMatchingVct->sMatchName);
                        }
                        else if (!strcmp(pValue, "num"))
                        {
                            pNewMatchingVct->sMatchNum = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "machnum"))
                        {
                            pNewMatchingVct->sMatchIndex = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "time"))
                        {
                            pNewMatchingVct->sMatchTime = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "cost"))
                        {
                            pNewMatchingVct->sMatchCost = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "count"))
                        {
                            pNewMatchingVct->sMatchGroupNum = pNodeElement->GetText();
                            pNewMatchingVct->nMatchGroupNum = atoi(pNodeElement->Attribute("value"));
                        }
                        else if (!strcmp(pValue, "signup"))
                        {
                            pValue = pNodeElement->GetText();
                            if (!strcmp(pValue, "True"))
                            {
                                pNewMatchingVct->bSignup = true;
                            }
                            else{
                                pNewMatchingVct->bSignup = false;
                            }
                        }
                        else if (!strcmp(pValue, "joinednum"))
                        {
                            pNewMatchingVct->sMatchJoinedNum = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "joinroom"))
                        {
                            pNewMatchingVct->sJoinRoomName = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "takeremainder"))
                        {
                            pNewMatchingVct->sMatchTakeremainder = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "second"))
                        {
                            pNewMatchingVct->nSecond = (atoi)(pNodeElement->GetText());
                        }
                        else if (!strcmp(pValue, "remark1"))
                        {
                            pNewMatchingVct->sMatchRule1 = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "remark2"))
                        {
                            pNewMatchingVct->sMatchRule2 = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "remark3"))
                        {
                            pNewMatchingVct->sMatchRule3 = pNodeElement->GetText();
                        }
                        else if (!strcmp(pValue, "machaward"))
                        {
                            TiXmlElement * pNodeElement1=pNodeElement->FirstChildElement();
                            nodename = pNodeElement1->Value();
                            if (!strcmp(nodename.c_str(), "num"))
                            {
                                pNewMatchingVct->sFirst=pNodeElement1->Attribute("rank");
                                pNewMatchingVct->sAwardFirst=pNodeElement1->GetText();
                            }
                            TiXmlElement * pNodeElement2=pNodeElement1->NextSiblingElement();
                            nodename = pNodeElement2->Value();
                            if (!strcmp(nodename.c_str(), "num"))
                            {
                                pNewMatchingVct->sSeconde=pNodeElement2->Attribute("rank");
                                pNewMatchingVct->sAwardSecond=pNodeElement2->GetText();
                            }
                            TiXmlElement * pNodeElement3=pNodeElement2->NextSiblingElement();
                            nodename = pNodeElement3->Value();
                            if (!strcmp(nodename.c_str(), "num"))
                            {
                                pNewMatchingVct->sThird=pNodeElement3->Attribute("rank");
                                pNewMatchingVct->sAwardThird=pNodeElement3->GetText();
                            }
                            TiXmlElement * pNodeElement4=pNodeElement3->NextSiblingElement();
                            nodename = pNodeElement4->Value();
                            if (!strcmp(nodename.c_str(), "num"))
                            {
                                pNewMatchingVct->sFourth=pNodeElement4->Attribute("rank");
                                pNewMatchingVct->sAwardFourth=pNodeElement4->GetText();
                            }
                        }
                        pNodeElement=pNodeElement->NextSiblingElement();
                    }
                    
                    int nMatchID = atoi(pNewMatchingVct->sMatchIndex.c_str());
                    if (m_nJoinMatchID != nMatchID)
                    {
                        pChildElement=pChildElement->NextSiblingElement();
                        continue;
                    }
                    int bHas = false;
                    for (int i=0; i<m_MatchingVct.size(); i++)
                    {
                        if (!strcmp(pNewMatchingVct->sMatchIndex.c_str(), m_MatchingVct[i]->sMatchIndex.c_str()))
                        {
                            m_MatchingVct[i] = pNewMatchingVct;
                            CCSprite* pDi = (CCSprite*)m_pMatchContainLayer->getChildByTag(10+i);
                            if (pDi != NULL)
                            {
                                m_bJoinRefresh = false;
                                m_nJoinMatchID = -1;
                                CCLabelTTF* indexTTF = (CCLabelTTF*)m_pMatchContainLayer->getChildByTag(1000+m_nShowItem);
                                if (indexTTF != NULL)
                                {
                                    indexTTF->setString(m_MatchingVct[i]->sMatchJoinedNum.c_str());
                                }
                                
                                CCLabelTTF* pJoinFontTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinNumFont);
                                if (pJoinFontTTF != NULL)
                                {
                                    int nTotalNum = atoi(m_MatchingVct[i]->sMatchJoinedNum.c_str());
                                    int nPerGroupNum = m_MatchingVct[i]->nMatchGroupNum;
                                    char sCurrentGroup[32];
                                    sprintf(sCurrentGroup, "第[%d]组报名人数: ", nTotalNum/nPerGroupNum+1);
                                    pJoinFontTTF->setString(sCurrentGroup);
                                }
                                
                                CCLabelTTF* pJoinPercentTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinPercentTag);
                                if (pJoinPercentTTF != NULL)
                                {
                                    pJoinPercentTTF->setString(m_MatchingVct[i]->sMatchTakeremainder.c_str());
                                    if (pJoinFontTTF != NULL)
                                    {
                                        pJoinPercentTTF->setPosition(ccp(pJoinFontTTF->getPositionX()+pJoinFontTTF->getContentSize().width, pJoinFontTTF->getPositionY()));
                                    }
                                }
                                
                                setMenuItemVisible(eJoinCoinTag, false, m_matchMenu);
                                setMenuItemVisible(eJoinFreeTag, false, m_matchMenu);
                                m_pSpJoined->setVisible(true);
                                
                                bHas = true;
                                break;
                            }
                        }
                    }
                    if (bHas)
                    {
                        break;
                    }
                }
            }
            else if (0==nodename.compare("userInfo"))
            {
                pChildElement=pChildElement->NextSiblingElement();
            }
            else
            {
                pChildElement=pChildElement->NextSiblingElement();
            }
        }
    }
    delete document;
}

void CRankNewLayer::loadMatchUI()
{
    CCSize laySize;
    if (m_MatchingVct.size()<6)
    {
        laySize=CCSizeMake(380, 590);
    }
    else
    {
        laySize=CCSizeMake(380, 660+109*(m_MatchingVct.size()-6));
    }
    m_pMatchScrollView = CCScrollView::create();
    m_pMatchContainLayer = CCLayerColor::create(ccc4(0, 0, 0, 0), laySize.width, laySize.height);
    m_pMatchContainLayer -> setPosition(ccp(0,590-laySize.height));
    m_pMatchScrollView->setPosition(ccp(19, 22));
    m_pMatchScrollView->setViewSize(CCSizeMake(380, 590));//设置view的大小
    m_pMatchScrollView->setContentOffset(ccp(0,0));
    m_pMatchScrollView->setContentSize(laySize);//设置scrollview区域的大小
    m_pMatchScrollView->setContainer(m_pMatchContainLayer); //设置需要滚动的内容
    m_pMatchScrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_pMatchScrollView->setBounceable(false);
    m_pMatchScrollView->setDelegate(this);
    m_pSpMatchBg->addChild(m_pMatchScrollView, 1);
    m_pMatchScrollView -> setTouchEnabled(true);
    m_pMatchScrollView -> setTouchPriority(-200);

    m_pNoMatchesTTF = CCLabelTTF::create("暂无比赛数据", "Arial", 39);
    m_pNoMatchesTTF->setPosition(ccp(190, 560));
    m_pSpMatchBg->addChild(m_pNoMatchesTTF, 1);
    m_pNoMatchesTTF->setVisible(false);
    m_pNoMatchInfoTTF = CCLabelTTF::create("暂无比赛信息", "Arial", 39);
    m_pNoMatchInfoTTF->setPosition(ccp(850, 560));
    m_pSpMatchBg->addChild(m_pNoMatchInfoTTF, 1);
    m_pNoMatchInfoTTF->setVisible(false);
   
    loadMatchNodeUI();
    
    m_nIndexPage = 1;
    m_nShowItem = 0;
    
    //加入一个加载动画，用于展示正在请求
    m_spAnimate = CCSprite::create("h2all/loading1.png");
    m_spAnimate -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spAnimate, 3);
    m_spAnimate -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
    
    requestCatchFishMatchingHttp();
}

void CRankNewLayer::loadMatchingUI()
{
    //清空数组
    for (int i=0; i<m_pMatchNodeAwardArray->count(); i++)
    {
        CCLabelTTF* pTTF = (CCLabelTTF*)m_pMatchNodeAwardArray->objectAtIndex(i);
        if (pTTF != NULL)
        {
            pTTF->removeFromParentAndCleanup(true);
        }
    }
    m_pMatchNodeAwardArray->removeAllObjects();
    for (int i=0; i<m_pContainTouchItemArray->count(); i++)
    {
        CCLabelTTF* pTTF = (CCLabelTTF*)m_pContainTouchItemArray->objectAtIndex(i);
        if (pTTF != NULL)
        {
            pTTF->removeFromParentAndCleanup(true);
        }
    }
    m_pContainTouchItemArray->removeAllObjects();
    m_pMatchContainLayer->removeAllChildrenWithCleanup(true);
    
    CCSize laySize;
    if (m_MatchingVct.size()<6)
    {
        laySize=CCSizeMake(380, 590);
    }
    else
    {
        laySize=CCSizeMake(380, 660+109*(m_MatchingVct.size()-6));
    }
    
    m_pMatchScrollView->setContentSize(laySize);
    m_pMatchContainLayer->setPosition(ccp(0, 590-laySize.height));
    m_pMatchContainLayer->setContentSize(laySize);
    
    if (m_MatchingVct.size()<=0)
    {
        CRankNewGuideLayer* pRankNewGuideLayer = (CRankNewGuideLayer*)this->getChildByTag(9001);
        if (pRankNewGuideLayer != NULL) //比赛指引报名
        {
            pRankNewGuideLayer->removeFromParentAndCleanup(true);
        }
        
        m_bJoinRefresh = false;
        m_pMatchNode->setVisible(false);
        setMenuItemVisible(eMtachAwardLingQu, false, m_matchMenu);
        
        //无数据处理
        m_pNoMatchesTTF->setVisible(true);
        m_pNoMatchInfoTTF->setVisible(true);
        CCUserDefault::sharedUserDefault()->setBoolForKey("bMatchGuide", false);
        return;
    }
    
    m_nShowItem = 0;
    m_pMatchScrollView->setVisible(true);
    m_pMatchNode->setVisible(true);
    m_pNoMatchesTTF->setVisible(false);
    m_pNoMatchInfoTTF->setVisible(false);
    
    bool bMoveContainLayer=false;
    for (int i=0; i<m_MatchingVct.size(); i++)
    {
        CCSprite* pDi;
        if (i==0) {
            pDi = CCSprite::create("rank/matchItem1.png");//红
        }
        else{
            if (m_MatchingVct[i]->nMatchType>1){
                pDi = CCSprite::create("rank/matchItem2.png");//黄>5元
            }
            else{
                pDi = CCSprite::create("rank/matchItem3.png");//蓝
            }
        }
        pDi->setTag(10+i);
        pDi->setPosition(ccp(m_pMatchContainLayer->getContentSize().width/2, m_pMatchContainLayer->getContentSize().height-8-pDi->getContentSize().height/2-(pDi->getContentSize().height+5)*i));
        m_pMatchContainLayer->addChild(pDi, 1, 10+i);
        m_pContainTouchItemArray->addObject(pDi);
        
        CCLabelTTF* typeTTF = CCLabelTTF::create(m_MatchingVct[i]->sMatchName.c_str(), "Arial", 28);
        typeTTF->setAnchorPoint(ccp(0.5f, 0.5f));
        typeTTF->setPosition(ccp(pDi->getPositionX(), pDi->getPositionY()+27));//pDi->getPositionX()+155,
        m_pMatchContainLayer->addChild(typeTTF, 3);
        
        CCLabelTTF* indexTTF = CCLabelTTF::create(m_MatchingVct[i]->sMatchJoinedNum.c_str(), "Arial", 28);
        indexTTF->setAnchorPoint(ccp(0, 0.5f));
        indexTTF->setPosition(ccp(30, pDi->getPositionY()-14));
        m_pMatchContainLayer->addChild(indexTTF, 3, 1000+i);
        
        std::string sSelectStartTime=selectStartMatchTime(m_MatchingVct[i]->sMatchTime.c_str());
        CCLabelTTF* timeTTF = CCLabelTTF::create(sSelectStartTime.c_str(), "Arial", 28);
        timeTTF->setAnchorPoint(ccp(1, 0.5f));
        timeTTF->setPosition(ccp(pDi->getPositionX()+155, pDi->getPositionY()-14));
        m_pMatchContainLayer->addChild(timeTTF, 3);
        
        if (i==0)
        {
            pDi->setVisible(false);
            playSelectItemAnimate(pDi->getPosition(), 1, 0);
            
            m_pItemWhiteShade = CCSprite::create("rank/matchItem4.png");
            m_pItemWhiteShade->setPosition(pDi->getPosition());
            m_pMatchContainLayer->addChild(m_pItemWhiteShade, 2);
//            m_pItemWhiteShade->setVisible(false);
            
            typeTTF->setColor(ccc3(185, 43, 28));
            indexTTF->setColor(ccc3(185, 43, 28));
            timeTTF->setColor(ccc3(185, 43, 28));
            
//            setTime();
            int nCost = atoi(m_MatchingVct[0]->sMatchCost.c_str());
            if (nCost<=0) {
                setMenuItemVisible(eJoinFreeTag, true, m_matchMenu);
                setMenuItemVisible(eJoinCoinTag, false, m_matchMenu);
            }
            else{
                setMenuItemVisible(eJoinFreeTag, false, m_matchMenu);
                setMenuItemVisible(eJoinCoinTag, true, m_matchMenu);
            }
            
        }
        else if (i==1)
        {
            typeTTF->setColor(ccc3(101, 51, 26));
            indexTTF->setColor(ccc3(101, 51, 26));
            timeTTF->setColor(ccc3(101, 51, 26));
        }
        else
        {
            typeTTF->setColor(ccc3(3, 95, 100));
            indexTTF->setColor(ccc3(3, 95, 100));
            timeTTF->setColor(ccc3(3, 95, 100));
        }
        
        bool bMatchGuide = CCUserDefault::sharedUserDefault()->getBoolForKey("bMatchGuide", true);
        if (!bMoveContainLayer && bMatchGuide)
        {
            int nCost = atoi(m_MatchingVct[i]->sMatchCost.c_str());
            if (nCost<=0)
            {
                bMoveContainLayer = true;
                m_pMatchContainLayer->setPositionY(590-pDi->getPositionY()-63);
                setLeftMatchNode(i);
            }
        }
    }
    
    bool bMatchGuide = CCUserDefault::sharedUserDefault()->getBoolForKey("bMatchGuide", true);
    if (bMatchGuide)
    {
        if (this->getChildByTag(9001) == NULL) {
            m_bClickTouch = true;
            CRankNewGuideLayer* pRankNewGuideLayer = CRankNewGuideLayer::create();
            this->addChild(pRankNewGuideLayer, 100, 9001);
        }
    }
    else
    {
        m_bClickTouch = true;
        setLeftMatchNode(0);
    }
}

void CRankNewLayer::loadMatchJoinedUI()
{
    //清空数组
    for (int i=0; i<m_pMatchNodeAwardArray->count(); i++)
    {
        CCLabelTTF* pTTF = (CCLabelTTF*)m_pMatchNodeAwardArray->objectAtIndex(i);
        if (pTTF != NULL)
        {
            pTTF->removeFromParentAndCleanup(true);
        }
    }
    m_pMatchNodeAwardArray->removeAllObjects();
    for (int i=0; i<m_pContainTouchItemArray->count(); i++)
    {
        CCLabelTTF* pTTF = (CCLabelTTF*)m_pContainTouchItemArray->objectAtIndex(i);
        if (pTTF != NULL)
        {
            pTTF->removeFromParentAndCleanup(true);
        }
    }
    m_pContainTouchItemArray->removeAllObjects();
    m_pMatchContainLayer->removeAllChildrenWithCleanup(true);
    
    CCSize laySize;
    if (m_MatchJoinedVct.size()<6)
    {
        laySize=CCSizeMake(380, 590);
    }
    else
    {
        laySize=CCSizeMake(380, 660+109*(m_MatchJoinedVct.size()-6));
    }
    
    m_pMatchScrollView->setContentSize(laySize);
    m_pMatchContainLayer->setPosition(ccp(0, 590-laySize.height));
    m_pMatchContainLayer->setContentSize(laySize);
    
    if (m_MatchJoinedVct.size()<=0)
    {
        m_pMatchNode->setVisible(false);
        setMenuItemVisible(eJoinFreeTag, false, m_matchMenu);
        setMenuItemVisible(eJoinCoinTag, false, m_matchMenu);
        setMenuItemVisible(eMtachAwardLingQu, false, m_matchMenu);
        
        //无比赛数据
        m_pNoMatchesTTF->setVisible(true);
        m_pNoMatchInfoTTF->setVisible(true);
        return;
    }
    
    m_nShowItem=0;
    m_pMatchScrollView->setVisible(true);
    m_pMatchNode->setVisible(true);
    m_pNoMatchesTTF->setVisible(false);
    m_pNoMatchInfoTTF->setVisible(false);
    
    for (int i=0; i<m_MatchJoinedVct.size(); i++)
    {
//        if (m_MatchingVct[i]->nMatchType>1){
//            pDi = CCSprite::create("rank/matchItem2.png");//黄>5元
//        }
//        else{
//            pDi = CCSprite::create("rank/matchItem3.png");//蓝
//        }
        CCSprite* pDi = CCSprite::create("rank/matchItem3.png");//蓝
        pDi->setTag(10+i);
        pDi->setPosition(ccp(m_pMatchContainLayer->getContentSize().width/2, m_pMatchContainLayer->getContentSize().height-8-pDi->getContentSize().height/2-(pDi->getContentSize().height+5)*i));
        m_pMatchContainLayer->addChild(pDi, 1, 10+i);
        m_pContainTouchItemArray->addObject(pDi);
        
        CCLabelTTF* typeTTF = CCLabelTTF::create(m_MatchJoinedVct[i]->sMatchName.c_str(), "Arial", 28);
        typeTTF->setAnchorPoint(ccp(0.5f, 0.5f));
        typeTTF->setPosition(ccp(pDi->getPositionX(), pDi->getPositionY()+27));
        m_pMatchContainLayer->addChild(typeTTF, 3);
        
        CCLabelTTF* indexTTF = CCLabelTTF::create(m_MatchJoinedVct[i]->sMatchJoinedNum.c_str(), "Arial", 28);
        indexTTF->setAnchorPoint(ccp(0, 0.5f));
        indexTTF->setPosition(ccp(30, pDi->getPositionY()-14));
        m_pMatchContainLayer->addChild(indexTTF, 3);
        
        std::string sSelectStartTime=selectStartMatchTime(m_MatchJoinedVct[i]->sMatchTime.c_str());
        CCLabelTTF* timeTTF = CCLabelTTF::create(sSelectStartTime.c_str(), "Arial", 28);
        timeTTF->setAnchorPoint(ccp(1, 0.5f));
        timeTTF->setPosition(ccp(pDi->getPositionX()+155, pDi->getPositionY()-14));
        m_pMatchContainLayer->addChild(timeTTF, 3);
        
        if (i==0)
        {
            pDi->setVisible(false);
            playSelectItemAnimate(pDi->getPosition(), 2, 0);
            
            m_pItemWhiteShade = CCSprite::create("rank/matchItem4.png");
            m_pItemWhiteShade->setPosition(pDi->getPosition());
            m_pMatchContainLayer->addChild(m_pItemWhiteShade, 2);
//            m_pItemWhiteShade->setVisible(false);
            
            typeTTF->setColor(ccc3(185, 43, 28));
            indexTTF->setColor(ccc3(185, 43, 28));
            timeTTF->setColor(ccc3(185, 43, 28));
            
//            setTime();
        }
        else if (i==1)
        {
            typeTTF->setColor(ccc3(101, 51, 26));
            indexTTF->setColor(ccc3(101, 51, 26));
            timeTTF->setColor(ccc3(101, 51, 26));
        }
        else{
            typeTTF->setColor(ccc3(3, 95, 100));
            indexTTF->setColor(ccc3(3, 95, 100));
            timeTTF->setColor(ccc3(3, 95, 100));
        }
    }
    setLeftMatchNode(0);
}

void CRankNewLayer::loadMatchedUI()
{
    //清空数组
    for (int i=0; i<m_pMatchNodeAwardArray->count(); i++)
    {
        CCLabelTTF* pTTF = (CCLabelTTF*)m_pMatchNodeAwardArray->objectAtIndex(i);
        if (pTTF != NULL)
        {
            pTTF->removeFromParentAndCleanup(true);
        }
    }
    m_pMatchNodeAwardArray->removeAllObjects();
    for (int i=0; i<m_pContainTouchItemArray->count(); i++)
    {
        CCLabelTTF* pTTF = (CCLabelTTF*)m_pContainTouchItemArray->objectAtIndex(i);
        if (pTTF != NULL)
        {
            pTTF->removeFromParentAndCleanup(true);
        }
    }
    m_pContainTouchItemArray->removeAllObjects();
    m_pMatchContainLayer->removeAllChildrenWithCleanup(true);
    
    CCSize laySize;
    if (m_MatchEndVct.size()<6)
    {
        laySize=CCSizeMake(380, 590);
    }
    else
    {
        laySize=CCSizeMake(380, 660+109*(m_MatchEndVct.size()-6));
    }
    
    m_pMatchScrollView->setContentSize(laySize);
    m_pMatchContainLayer->setPosition(ccp(0, 590-laySize.height));
    m_pMatchContainLayer->setContentSize(laySize);
    
    if (m_MatchEndVct.size()<=0)
    {
        m_pMatchNode->setVisible(false);
        setMenuItemVisible(eJoinFreeTag, false, m_matchMenu);
        setMenuItemVisible(eJoinCoinTag, false, m_matchMenu);
        
        //无比赛数据
        m_pNoMatchesTTF->setVisible(true);
        m_pNoMatchInfoTTF->setVisible(true);
        return;
    }
    
    m_nShowItem=0;
    m_pMatchScrollView->setVisible(true);
    m_pMatchNode->setVisible(true);
    m_pNoMatchesTTF->setVisible(false);
    m_pNoMatchInfoTTF->setVisible(false);
    
    for (int i=0; i<m_MatchEndVct.size(); i++)
    {
//        if (m_MatchingVct[i]->nMatchType>1){
//            pDi = CCSprite::create("rank/matchItem2.png");//黄>5元
//        }
//        else{
//            pDi = CCSprite::create("rank/matchItem3.png");//蓝
//        }
        CCSprite* pDi = CCSprite::create("rank/matchItem3.png");//蓝
        pDi->setTag(10+i);
        pDi->setPosition(ccp(m_pMatchContainLayer->getContentSize().width/2, m_pMatchContainLayer->getContentSize().height-8-pDi->getContentSize().height/2-(pDi->getContentSize().height+5)*i));
        m_pMatchContainLayer->addChild(pDi, 1, 10+i);
        m_pContainTouchItemArray->addObject(pDi);
        
        CCLabelTTF* typeTTF = CCLabelTTF::create(m_MatchEndVct[i]->sMatchName.c_str(), "Arial", 28);
        typeTTF->setAnchorPoint(ccp(0.5, 0.5f));
        typeTTF->setPosition(ccp(pDi->getPositionX(), pDi->getPositionY()+27));
        m_pMatchContainLayer->addChild(typeTTF, 3);
        
//        CCLabelTTF* indexTTF = CCLabelTTF::create(m_MatchEndVct[i]->sMatchJoinedNum.c_str(), "Arial", 28);
//        indexTTF->setAnchorPoint(ccp(0, 0.5f));
//        indexTTF->setPosition(ccp(30, pDi->getPositionY()-14));
//        m_pMatchContainLayer->addChild(indexTTF, 3);
         
        std::string sSelectStartTime=m_MatchEndVct[i]->sMatchDay+"   "+selectStartMatchTime(m_MatchEndVct[i]->sMatchTime.c_str());
        CCLabelTTF* timeTTF = CCLabelTTF::create(sSelectStartTime.c_str(), "Arial", 28);
        timeTTF->setAnchorPoint(ccp(0.5f, 0.5f));
        timeTTF->setPosition(ccp(pDi->getPositionX(), pDi->getPositionY()-14));
        m_pMatchContainLayer->addChild(timeTTF, 3);
        
        if (m_MatchEndVct[i]->nGet == 0)
        {
            CCSprite* pNoLingQu = CCSprite::create("CatchFish2001/jiaobiaodi.png");
            pNoLingQu -> setPosition(ccp(pDi->getPositionX()+pDi->getContentSize().width/2-10, pDi->getPositionY()+pDi->getContentSize().height/2-15));
            m_pMatchContainLayer -> addChild(pNoLingQu, 3, 600+i);
            printf("pNoLingQu=(%0.1f  %0.1f)\n",pNoLingQu->getPositionX(), pNoLingQu->getPositionY());
            
            CCLabelTTF* pNoNumLingQuTTF = CCLabelTTF::create("1", "Arial", 20);
            pNoNumLingQuTTF->setPosition(ccp(pNoLingQu->getContentSize().width/2, pNoLingQu->getContentSize().height/2));
            pNoLingQu->addChild(pNoNumLingQuTTF);
        }

        
        if (i==0)
        {
            pDi->setVisible(false);
            playSelectItemAnimate(pDi->getPosition(), 3, 0);
            
            m_pItemWhiteShade = CCSprite::create("rank/matchItem4.png");
            m_pItemWhiteShade->setPosition(pDi->getPosition());
            m_pMatchContainLayer->addChild(m_pItemWhiteShade, 2);
//            m_pItemWhiteShade->setVisible(false);
            
            typeTTF->setColor(ccc3(185, 43, 28));
//            indexTTF->setColor(ccc3(185, 43, 28));
            timeTTF->setColor(ccc3(185, 43, 28));
            
            CCLabelTTF* pMatchNodeInstructionTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchInstructionTag);
            if (pMatchNodeInstructionTTF != NULL)
            {
                std::string sInstruction;
                int nCost = atoi(m_MatchEndVct[0]->sMatchCost.c_str());
                if (nCost<=0) {
                    sInstruction = m_MatchEndVct[0]->sMatchName+"(免费报名)"+"  比赛时间: "+m_MatchEndVct[0]->sMatchTime;
                }
                else{
                    sInstruction = m_MatchEndVct[0]->sMatchName+"  比赛时间: "+m_MatchEndVct[0]->sMatchTime;
                }
                pMatchNodeInstructionTTF->setString(sInstruction.c_str());
            }
            
            if (m_MatchEndVct[i]->nGet == 0)
            {
                setMenuItemVisible(eMtachAwardLingQu, true, m_matchMenu);
                m_pSpLingQued->setVisible(false);
            }
            else if (m_MatchEndVct[i]->nGet == 1)
            {
                setMenuItemVisible(eMtachAwardLingQu, false, m_matchMenu);
                m_pSpLingQued->setVisible(true);
            }
            else if (m_MatchEndVct[i]->nGet == 2)
            {
                setMenuItemVisible(eMtachAwardLingQu, false, m_matchMenu);
                m_pSpLingQued->setVisible(false);
            }
        }
        else if (i==1)
        {
            typeTTF->setColor(ccc3(101, 51, 26));
//            indexTTF->setColor(ccc3(101, 51, 26));
            timeTTF->setColor(ccc3(101, 51, 26));
        }
        else{
            typeTTF->setColor(ccc3(3, 95, 100));
//            indexTTF->setColor(ccc3(3, 95, 100));
            timeTTF->setColor(ccc3(3, 95, 100));
        }
    }
    setLeftMatchNode(0);
}

void CRankNewLayer::loadMatchNodeUI()
{
    m_pMatchNode = CCNode::create();
    m_pMatchNode->setPosition(ccp(0, 0));
    m_pSpMatchBg->addChild(m_pMatchNode, 1);
    
    CCSprite* pMatchNodeInfoBg = CCSprite::create("rank/matchNodeInfoBg.png");
    pMatchNodeInfoBg->setPosition(ccp(843, 317));
    m_pMatchNode->addChild(pMatchNodeInfoBg, 1);
    
    CCMenuItemImage* joinFree = CCMenuItemImage::create("rank/matchJoinFree.png", "rank/matchJoinFree.png",this,menu_selector(CRankNewLayer::callBack));
    CCMenuItemImage* joinCoin = CCMenuItemImage::create("rank/matchJoinCoin.png", "rank/matchJoinCoin.png",this,menu_selector(CRankNewLayer::callBack));
    CCMenuItemImage* lingQu = CCMenuItemImage::create("rank/matchBtnLingQu.png", "rank/matchBtnLingQu.png",this,menu_selector(CRankNewLayer::callBack));
    joinFree->setTag(eJoinFreeTag);
    joinCoin->setTag(eJoinCoinTag);
    lingQu->setTag(eMtachAwardLingQu);
    joinFree->setPosition(ccp(CLIENT_WIDTH/2+200, 80));
    joinCoin->setPosition(ccp(CLIENT_WIDTH/2+200, 80));
    lingQu->setPosition(joinFree->getPosition());
    joinFree->setVisible(false);
    joinCoin->setVisible(false);
    lingQu->setVisible(false);
    m_matchMenu = CCMenu::create(joinFree, lingQu, joinCoin, NULL);
    m_matchMenu->setPosition(ccp(0, 0));
    m_matchMenu->setTouchPriority(-200);
    m_pMatchNode->addChild(m_matchMenu, 1);
    
    m_pSpJoined = CCSprite::create("rank/matchBtnAlreadyJoin.png");
    m_pSpJoined->setPosition(ccp(CLIENT_WIDTH/2+200, 80));
    m_pMatchNode->addChild(m_pSpJoined, 1);
    m_pSpJoined->setVisible(false);
    m_pSpLingQued = CCSprite::create("rank/matchBtnLingQu2.png");
    m_pSpLingQued->setPosition(lingQu->getPosition());
    m_pMatchNode->addChild(m_pSpLingQued, 1);
    m_pSpLingQued->setVisible(false);

    CCLabelTTF* pMatchNodeInstructionTTF = CCLabelTTF::create("", "Arial-BoldMT", 28);
    pMatchNodeInstructionTTF->setColor(ccc3(244, 6, 6));
    pMatchNodeInstructionTTF->setPosition(ccp(CLIENT_WIDTH/2+202, CLIENT_HEIGHT/2+210));
    m_pMatchNode->addChild(pMatchNodeInstructionTTF, 1, eMatchInstructionTag);

    CCLabelTTF* pMatchRule1TTF = CCLabelTTF::create("", "Arial", 28);
    pMatchRule1TTF->setColor(ccc3(32, 66, 117));
    pMatchRule1TTF->setAnchorPoint(ccp(0, 0));
    pMatchRule1TTF->setPosition(ccp(597, 499));
    m_pMatchNode->addChild(pMatchRule1TTF, 1, eMatchNodeRule1Tag);
    CCLabelTTF* pMatchRule2TTF = CCLabelTTF::create("", "Arial", 28);
    pMatchRule2TTF->setColor(ccc3(32, 66, 117));
    pMatchRule2TTF->setAnchorPoint(ccp(0, 0));
    pMatchRule2TTF->setPosition(ccp(597, 463));
    m_pMatchNode->addChild(pMatchRule2TTF, 1, eMatchNodeRule2Tag);
    CCLabelTTF* pMatchRule3TTF = CCLabelTTF::create("", "Arial", 28);
    pMatchRule3TTF->setColor(ccc3(32, 66, 117));
    pMatchRule3TTF->setAnchorPoint(ccp(0, 0));
    pMatchRule3TTF->setPosition(ccp(597, 425));
    m_pMatchNode->addChild(pMatchRule3TTF, 1, eMatchNodeRule3Tag);
    
    //参赛条件
    CCLabelTTF* pMatchJoinFont = CCLabelTTF::create("参赛人数: ", "Arial", 32);
    pMatchJoinFont->setColor(ccc3(32, 66, 117));
    pMatchJoinFont->setAnchorPoint(ccp(0, 0.5f));
    pMatchJoinFont->setPosition(ccp(590-100, 340));
    m_pMatchNode->addChild(pMatchJoinFont, 1, eMatchNodeJoinGroupFont);
    CCLabelTTF* pMatchPersonNumTTF = CCLabelTTF::create("8", "Arial-BoldMT", 32);
    pMatchPersonNumTTF->setColor(ccc3(220, 121, 30));
    pMatchPersonNumTTF->setAnchorPoint(ccp(0, 0.5f));
    pMatchPersonNumTTF->setPosition(ccp(pMatchJoinFont->getPositionX()+pMatchJoinFont->getContentSize().width, pMatchJoinFont->getPositionY()));
    m_pMatchNode->addChild(pMatchPersonNumTTF, 1, eMatchNodePersonNumTag);
    
    CCLabelTTF* pMatchJoinScoreFont = CCLabelTTF::create("报名费用: ", "Arial", 32);
    pMatchJoinScoreFont->setColor(ccc3(32, 66, 117));
    pMatchJoinScoreFont->setAnchorPoint(ccp(0, 0.5f));
    pMatchJoinScoreFont->setPosition(ccp(880, 340));
    m_pMatchNode->addChild(pMatchJoinScoreFont, 1, eMatchNodeJoinScoreFont);
    CCLabelTTF* pMatchJoinScoreTTF = CCLabelTTF::create("", "Arial-BoldMT", 32);
    pMatchJoinScoreTTF->setColor(ccc3(220, 121, 30));
    pMatchJoinScoreTTF->setAnchorPoint(ccp(0, 0.5f));
    pMatchJoinScoreTTF->setPosition(ccp(pMatchJoinScoreFont->getPositionX()+pMatchJoinScoreFont->getContentSize().width, pMatchJoinScoreFont->getPositionY()));
    m_pMatchNode->addChild(pMatchJoinScoreTTF, 1, eMatchNodeJoinScoreTag);

    CCLabelTTF* pMatchJoinNumFont = CCLabelTTF::create("第[00]组报名人数: ", "Arial", 32);
    pMatchJoinNumFont->setColor(ccc3(32, 66, 117));
    pMatchJoinNumFont->setAnchorPoint(ccp(0, 0.5f));
    pMatchJoinNumFont->setPosition(ccp(685+195, 295));
    m_pMatchNode->addChild(pMatchJoinNumFont, 1, eMatchNodeJoinNumFont);
    CCLabelTTF* pMatchJoinRoom = CCLabelTTF::create("参赛房间: ", "Arial", 32);
    pMatchJoinRoom->setColor(ccc3(32, 66, 117));
    pMatchJoinRoom->setAnchorPoint(ccp(0, 0.5f));
    pMatchJoinRoom->setPosition(ccp(590-100, 295));
    m_pMatchNode->addChild(pMatchJoinRoom, 1, eMatchNodeJoinRoom);
    CCLabelTTF* pMatchJoinRoomTTF = CCLabelTTF::create("", "Arial-BoldMT", 32);
    pMatchJoinRoomTTF->setColor(ccc3(220, 121, 30));
    pMatchJoinRoomTTF->setAnchorPoint(ccp(0, 0.5f));
    pMatchJoinRoomTTF->setPosition(ccp(pMatchJoinRoom->getPositionX()+pMatchJoinRoom->getContentSize().width, pMatchJoinRoom->getPositionY()));
    m_pMatchNode->addChild(pMatchJoinRoomTTF, 1, eMatchNodeJoin2Room);
    CCLabelTTF* pMatchJoinPercentTTF = CCLabelTTF::create("6", "Arial-BoldMT", 32);
    pMatchJoinPercentTTF->setColor(ccc3(220, 121, 30));
    pMatchJoinPercentTTF->setAnchorPoint(ccp(0, 0.5f));
    pMatchJoinPercentTTF->setPosition(ccp(pMatchJoinNumFont->getPositionX()+pMatchJoinNumFont->getContentSize().width, pMatchJoinNumFont->getPositionY()));
    m_pMatchNode->addChild(pMatchJoinPercentTTF, 1, eMatchNodeJoinPercentTag);
    m_pMatchNode->setVisible(false);
}

void CRankNewLayer::loadRankUI()
{
    m_pSpRankBg = CCSprite::create();
    m_pSpRankBg->setAnchorPoint(ccp(0, 0));
    m_pSpRankBg->setPosition(ccp(0, 0));
    this->addChild(m_pSpRankBg, 1);
    
    CCSprite* pRankBg = CCSprite::create("rank/matchRankbg.png");
    pRankBg->setScale(2.0f);
    pRankBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    m_pSpRankBg->addChild(pRankBg, 1);
    CCSprite* pRankTitle = CCSprite::create("rank/matchRankTitle.png");
    pRankTitle->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT-141));
    m_pSpRankBg->addChild(pRankTitle, 1);
    
    m_pSpRankChildBg = CCSprite::create();
    m_pSpRankChildBg->setPosition(ccp(0, 0));
    m_pSpRankBg->addChild(m_pSpRankChildBg, 1);
    m_pRankScrollView = CCScrollView::create();
    m_pRankContainLayer = CCLayerColor::create(ccc4(0, 0, 0, 0));
    m_pRankContainLayer -> setContentSize(CCSizeMake(1180, 730));//360
    m_pRankContainLayer -> setAnchorPoint(CCPointZero);
    m_pRankContainLayer -> setPosition(ccp(0,460-m_pRankContainLayer->getContentSize().height));
    m_pRankScrollView->setPosition(ccp(50, 81));     //根据cocostudio调整的位置
    m_pRankScrollView->setViewSize(CCSizeMake(1180, 460));//设置view的大小
    m_pRankScrollView->setContentOffset(ccp(0,0));
    m_pRankScrollView->setContentSize(CCSizeMake(1180, 730));//设置scrollview区域的大小
    m_pRankScrollView->setContainer(m_pRankContainLayer); //设置需要滚动的内容
    m_pRankScrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_pRankScrollView->setBounceable(false);
    m_pRankScrollView->setDelegate(this);
    m_pSpRankChildBg->addChild(m_pRankScrollView, 1);
    m_pRankScrollView -> setTouchPriority(-200);
    m_pRankScrollView->setTouchEnabled(false);
    
    m_pNoRankInfoTTF = CCLabelTTF::create("暂无排名数据", "Arial", 39);
    m_pNoRankInfoTTF->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+100));
    m_pSpRankBg->addChild(m_pNoRankInfoTTF, 5);
    m_pNoMatchInfoTTF->setVisible(false);
    
    for(int i = 0; i < 10; i++)
    {
        CCSprite* pRankCellDi;
        if (i == 0) {
            pRankCellDi = CCSprite::create("rank/matchCell1.png");
        }
        else if (i == 1) {
            pRankCellDi = CCSprite::create("rank/matchCell2.png");
        }
        else if (i == 2) {
            pRankCellDi = CCSprite::create("rank/matchCell3.png");
        }
        else {
            pRankCellDi = CCSprite::create("rank/matchCell4.png");
        }
        pRankCellDi->setPosition(ccp(m_pRankContainLayer->getContentSize().width/2, m_pRankContainLayer->getContentSize().height-3-pRankCellDi->getContentSize().height/2-(pRankCellDi->getContentSize().height+7)*i));
        m_pRankContainLayer->addChild(pRankCellDi, 1);
        
        CCSprite* pSpVip = CCSprite::create("rank/matchVip.png");
        pSpVip->setPosition(ccp(195, pRankCellDi->getPositionY()));
        m_pRankContainLayer->addChild(pSpVip, 1, 400+i);
        
        CCLabelAtlas* pVipLevelTTF = CCLabelAtlas::create("0", "rank/matchVipNum.png", 20, 28, '0');
        pVipLevelTTF->setAnchorPoint(ccp(0, 0.5f));
        pVipLevelTTF->setPosition(ccp(pSpVip->getPositionX()+pSpVip->getContentSize().width/2+5, pRankCellDi->getPositionY()));
        m_pRankContainLayer->addChild(pVipLevelTTF, 1, 100+i);
        
        CCLabelTTF* pNickNameTTF = CCLabelTTF::create("", "Arial", 32);
        pNickNameTTF->setColor(ccc3(101, 51, 23));
        pNickNameTTF->setPosition(ccp(CLIENT_WIDTH/2-67, pRankCellDi->getPositionY()));
        m_pRankContainLayer->addChild(pNickNameTTF, 1, 200+i);
        
        CCLabelTTF* pHuaFeiTTF = CCLabelTTF::create("", "Arial", 32);
        pHuaFeiTTF->setColor(ccc3(101, 51, 23));
        pHuaFeiTTF->setPosition(ccp(1010, pRankCellDi->getPositionY()));
        m_pRankContainLayer->addChild(pHuaFeiTTF, 1, 300+i);
        
        if (i == 0) {
            CCSprite* pFirstBg = CCSprite::create("rank/matchIndex1.png");
            pFirstBg->setPosition(ccp(130, pRankCellDi->getPositionY()));
            m_pRankContainLayer->addChild(pFirstBg, 1);
        }
        else if (i == 1) {
            CCSprite* pSecondBg = CCSprite::create("rank/matchIndex2.png");
            pSecondBg->setPosition(ccp(130, pRankCellDi->getPositionY()));
            m_pRankContainLayer->addChild(pSecondBg, 1);
        }
        else if (i == 2) {
            CCSprite* pThirdBg = CCSprite::create("rank/matchIndex3.png");
            pThirdBg->setPosition(ccp(130, pRankCellDi->getPositionY()));
            m_pRankContainLayer->addChild(pThirdBg, 1);
        }
        else
        {
            char sIndex[8];
            sprintf(sIndex, "%d", i+1);
            CCLabelAtlas* pIndexBg = CCLabelAtlas::create(sIndex, "rank/matchIndexNum.png", 29, 38, '0');
            pIndexBg->setAnchorPoint(ccp(0.5f, 0.5f));
            pIndexBg->setPosition(ccp(130, pRankCellDi->getPositionY()));
            m_pRankContainLayer->addChild(pIndexBg, 1);
        }
    }
    
    
    CCSprite* pMeKuang1 = CCSprite::create("rank/rankMeKuang.png");
    pMeKuang1->setPosition(ccp(270, 33));
    m_pSpRankBg->addChild(pMeKuang1, 1);
    CCSprite* pMeKuang2 = CCSprite::create("rank/rankMeKuang.png");
    pMeKuang2->setPosition(ccp(CLIENT_WIDTH/2+60, 33));
    m_pSpRankBg->addChild(pMeKuang2, 1);
    CCSprite* pMeKuang3 = CCSprite::create("rank/rankMeKuang.png");
    pMeKuang3->setPosition(ccp(1120, 33));
    m_pSpRankBg->addChild(pMeKuang3, 1);
    
    //自己昵称
    CCSprite* pMeNickNameSP = CCSprite::create("rank/rankMeNichengSp.png");
    pMeNickNameSP->setPosition(ccp(135, 33));
    m_pSpRankBg->addChild(pMeNickNameSP, 1, eRankMeNichengSp);
    CCLabelTTF* pMeNickName = CCLabelTTF::create("", "Arial", 30);
    pMeNickName->setAnchorPoint(ccp(0, 0.5f));
    pMeNickName->setColor(ccc3(253, 196, 57));
    pMeNickName->setPosition(ccp(pMeNickNameSP->getPositionX()+pMeNickNameSP->getContentSize().width/2+15, 33));
    m_pSpRankBg->addChild(pMeNickName, 1, eRankMeNickname);
    
    //自己排名
    CCSprite* pMeRankSP = CCSprite::create("rank/rankMePaimingSp.png");
    pMeRankSP->setPosition(ccp(CLIENT_WIDTH/2-100, 33));
    m_pSpRankBg->addChild(pMeRankSP, 1, eRankMePaimingSp);
    CCLabelTTF* pMeIndexTTF = CCLabelTTF::create("千里之外", "Arial", 30);
    pMeIndexTTF->setAnchorPoint(ccp(0, 0.5f));
    pMeIndexTTF->setColor(ccc3(253, 196, 57));
    pMeIndexTTF->setPosition(ccp(pMeRankSP->getPositionX()+pMeRankSP->getContentSize().width/2+15, 33));
    m_pSpRankBg->addChild(pMeIndexTTF, 1, eRankMeIndexTTF);
    pMeIndexTTF->setVisible(false);
    
    //自己话费
    CCSprite* pMeScoreSP = CCSprite::create("rank/rankMeHuafeiSp.png");
    pMeScoreSP->setPosition(ccp(960, 33));
    m_pSpRankBg->addChild(pMeScoreSP, 1, eRankMeScoreSp);
    CCLabelTTF* pMeScore = CCLabelTTF::create("", "Arial", 30);
    pMeScore->setAnchorPoint(ccp(0, 0.5f));
    pMeScore->setColor(ccc3(253, 196, 57));
    pMeScore->setPosition(ccp(pMeScoreSP->getPositionX()+pMeScoreSP->getContentSize().width/2+15, 33));
    m_pSpRankBg->addChild(pMeScore, 1, eRankMeScore);
    
    m_pSpRankChildBg->setVisible(false);
    m_pSpRankBg->setVisible(false);
}

void CRankNewLayer::setLeftMatchNode(int indexItem)
{
    if (m_nIndexPage == 1)
    {
        if (indexItem>= m_MatchingVct.size())
        {
            return;
        }
    }
    else if (m_nIndexPage == 2)
    {
        if (indexItem>= m_MatchJoinedVct.size())
        {
            return;
        }
    }
    else if (m_nIndexPage == 3)
    {
        if (indexItem>= m_MatchEndVct.size())
        {
            return;
        }
    }
    
    for (int i=0; i<m_pMatchNodeAwardArray->count(); i++)
    {
        CCLabelTTF* pTTF = (CCLabelTTF*)m_pMatchNodeAwardArray->objectAtIndex(i);
        if (pTTF != NULL)
        {
            pTTF->removeFromParentAndCleanup(true);
        }
    }
    m_pMatchNodeAwardArray->removeAllObjects();
    m_nShowItem = indexItem;
    std::string sInstruction;
    std::string sMatchTime = "比赛时间: ";
    std::string sMatchRule1;
    std::string sMatchRule2;
    std::string sMatchRule3;
    std::string sMatchPersonNum;
    std::string sMatchJoinScore;
    std::string sMatchJoinTotal;
    
    if (m_nIndexPage == 1)
    {
        int nCost = atoi(m_MatchingVct[indexItem]->sMatchCost.c_str());
        if (nCost<=0) {
            if (indexItem == 0)
            {
                printf("222\n");
                setTime();
            }
            else
            {
                sInstruction = m_MatchingVct[indexItem]->sMatchName+"(免费报名)"+"  比赛时间: "+m_MatchingVct[indexItem]->sMatchTime;
                CCLabelTTF* pInstructionTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchInstructionTag);
                if (pInstructionTTF != NULL)
                {
                    pInstructionTTF->setString(sInstruction.c_str());
                }
            }
        }
        else{
            if (indexItem == 0) {
                printf("111\n");
                setTime();
            }
            else{
                sInstruction = m_MatchingVct[indexItem]->sMatchName+"  比赛时间: "+m_MatchingVct[indexItem]->sMatchTime;
                CCLabelTTF* pInstructionTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchInstructionTag);
                if (pInstructionTTF != NULL)
                {
                    pInstructionTTF->setString(sInstruction.c_str());
                }
            }
        }
        sMatchTime = sMatchTime+m_MatchingVct[indexItem]->sMatchTime;
        sMatchRule1 = m_MatchingVct[indexItem]->sMatchRule1;
        sMatchRule2 = m_MatchingVct[indexItem]->sMatchRule2;
        sMatchRule3 = m_MatchingVct[indexItem]->sMatchRule3;
        sMatchPersonNum = m_MatchingVct[indexItem]->sMatchGroupNum;
        sMatchJoinScore = m_MatchingVct[indexItem]->sMatchCost;
        
        CCLabelTTF* pFirstTTF = CCLabelTTF::create(m_MatchingVct[indexItem]->sFirst.c_str(), "Arial", 32);
        pFirstTTF->setColor(ccc3(32, 66, 117));
        pFirstTTF->setAnchorPoint(ccp(0, 0.5));
        pFirstTTF->setPosition(ccp(600, 200));
        m_pMatchNodeAwardArray->addObject(pFirstTTF);
        m_pMatchNode->addChild(pFirstTTF, 1);
        CCLabelTTF* pFirstAwardTTF = CCLabelTTF::create(m_MatchingVct[indexItem]->sAwardFirst.c_str(), "Arial-BoldMT", 32);
        pFirstAwardTTF->setColor(ccc3(220, 121, 30));
        pFirstAwardTTF->setAnchorPoint(ccp(0, 0.5));
        pFirstAwardTTF->setPosition(ccp(pFirstTTF->getPositionX()+pFirstTTF->getContentSize().width+5, pFirstTTF->getPositionY()));
        m_pMatchNodeAwardArray->addObject(pFirstAwardTTF);
        m_pMatchNode->addChild(pFirstAwardTTF, 1);
        
        CCLabelTTF* pSecondTTF = CCLabelTTF::create(m_MatchingVct[indexItem]->sSeconde.c_str(), "Arial", 32);
        pSecondTTF->setColor(ccc3(32, 66, 117));
        pSecondTTF->setAnchorPoint(ccp(0, 0.5));
        pSecondTTF->setPosition(ccp(890, 200));
        m_pMatchNodeAwardArray->addObject(pSecondTTF);
        m_pMatchNode->addChild(pSecondTTF, 1);
        CCLabelTTF* pSecondAwardTTF = CCLabelTTF::create(m_MatchingVct[indexItem]->sAwardSecond.c_str(), "Arial-BoldMT", 32);
        pSecondAwardTTF->setColor(ccc3(220, 121, 30));
        pSecondAwardTTF->setAnchorPoint(ccp(0, 0.5));
        pSecondAwardTTF->setPosition(ccp(pSecondTTF->getPositionX()+pSecondTTF->getContentSize().width+5, pSecondTTF->getPositionY()));
        m_pMatchNodeAwardArray->addObject(pSecondAwardTTF);
        m_pMatchNode->addChild(pSecondAwardTTF, 1);
        
        CCLabelTTF* pThirdTTF = CCLabelTTF::create(m_MatchingVct[indexItem]->sThird.c_str(), "Arial", 32);
        pThirdTTF->setColor(ccc3(32, 66, 117));
        pThirdTTF->setAnchorPoint(ccp(0, 0.5));
        pThirdTTF->setPosition(ccp(600, 160));
        m_pMatchNodeAwardArray->addObject(pThirdTTF);
        m_pMatchNode->addChild(pThirdTTF, 1);
        CCLabelTTF* pThirdAwardTTF = CCLabelTTF::create(m_MatchingVct[indexItem]->sAwardThird.c_str(), "Arial-BoldMT", 32);
        pThirdAwardTTF->setColor(ccc3(220, 121, 30));
        pThirdAwardTTF->setAnchorPoint(ccp(0, 0.5));
        pThirdAwardTTF->setPosition(ccp(pThirdTTF->getPositionX()+pThirdTTF->getContentSize().width+5, pThirdTTF->getPositionY()));

        m_pMatchNodeAwardArray->addObject(pThirdAwardTTF);
        m_pMatchNode->addChild(pThirdAwardTTF, 1);
        
        CCLabelTTF* pFourthTTF = CCLabelTTF::create(m_MatchingVct[indexItem]->sFourth.c_str(), "Arial", 32);
        pFourthTTF->setColor(ccc3(32, 66, 117));
        pFourthTTF->setAnchorPoint(ccp(0, 0.5));
        pFourthTTF->setPosition(ccp(890, 160));
        m_pMatchNodeAwardArray->addObject(pFourthTTF);
        m_pMatchNode->addChild(pFourthTTF, 1);
        CCLabelTTF* pFourthAwardTTF = CCLabelTTF::create(m_MatchingVct[indexItem]->sAwardFourth.c_str(), "Arial-BoldMT", 32);
        pFourthAwardTTF->setColor(ccc3(220, 121, 30));
        pFourthAwardTTF->setAnchorPoint(ccp(0, 0.5));
        pFourthAwardTTF->setPosition(ccp(pFourthTTF->getPositionX()+pFourthTTF->getContentSize().width+5, pFourthTTF->getPositionY()));
        m_pMatchNodeAwardArray->addObject(pFourthAwardTTF);
        m_pMatchNode->addChild(pFourthAwardTTF, 1);
        
        CCLabelTTF* pJoinGroupFontTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinGroupFont);
        if (pJoinGroupFontTTF != NULL)
        {
            pJoinGroupFontTTF->setPositionY(340);
        }
        CCLabelTTF* pJoinScoreFontTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinScoreFont);
        if (pJoinScoreFontTTF != NULL)
        {
            pJoinScoreFontTTF->setPositionY(340);
        }
        
        CCLabelTTF* pJoinFontTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinNumFont);
        if (pJoinFontTTF != NULL)
        {
            pJoinFontTTF->setVisible(true);
            int nTotalNum = atoi(m_MatchingVct[indexItem]->sMatchJoinedNum.c_str());
            int nPerGroupNum = m_MatchingVct[indexItem]->nMatchGroupNum;
            char sCurrentGroup[32];
            sprintf(sCurrentGroup, "第[%d]组报名人数: ", nTotalNum/nPerGroupNum+1);
            pJoinFontTTF->setString(sCurrentGroup);
        }
        
        CCLabelTTF* pJoinPercentTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinPercentTag);
        if (pJoinPercentTTF != NULL)
        {
            pJoinPercentTTF->setString(m_MatchingVct[indexItem]->sMatchTakeremainder.c_str());
            if (pJoinFontTTF != NULL)
            {
                pJoinPercentTTF->setPosition(ccp(pJoinFontTTF->getPositionX()+pJoinFontTTF->getContentSize().width, pJoinFontTTF->getPositionY()));
            }
            pJoinPercentTTF->setVisible(true);
        }
        CCLabelTTF* pJoinRoomTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinRoom);
        if (pJoinRoomTTF != NULL)
        {
            pJoinRoomTTF->setVisible(true);
        }
        CCLabelTTF* pJoinRoom2TTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoin2Room);
        if (pJoinRoom2TTF != NULL)
        {
            std::string sMatchJoin2Room = m_MatchingVct[indexItem]->sJoinRoomName;
            pJoinRoom2TTF->setString(sMatchJoin2Room.c_str());
			pJoinRoom2TTF->setVisible(true);
        }
        
        if (m_MatchingVct[indexItem]->bSignup) {
            setMenuItemVisible(eJoinFreeTag, false, m_matchMenu);
            setMenuItemVisible(eJoinCoinTag, false, m_matchMenu);
            m_pSpJoined->setVisible(true);
        }
        else
        {
            int nCost = atoi(m_MatchingVct[indexItem]->sMatchCost.c_str());
            if (nCost<=0) {
                setMenuItemVisible(eJoinFreeTag, true, m_matchMenu);
                setMenuItemVisible(eJoinCoinTag, false, m_matchMenu);
            }
            else{
                setMenuItemVisible(eJoinFreeTag, false, m_matchMenu);
                setMenuItemVisible(eJoinCoinTag, true, m_matchMenu);
            }
            m_pSpJoined->setVisible(false);
        }
    }
    else if (m_nIndexPage == 2)
    {
        printf("333\n");
        setTime();
        sMatchTime = sMatchTime+m_MatchJoinedVct[indexItem]->sMatchTime;
        sMatchRule1 = m_MatchJoinedVct[indexItem]->sMatchRule1;
        sMatchRule2 = m_MatchJoinedVct[indexItem]->sMatchRule2;
        sMatchRule3 = m_MatchJoinedVct[indexItem]->sMatchRule3;
        sMatchPersonNum = m_MatchJoinedVct[indexItem]->sMatchGroupNum;
        sMatchJoinScore = m_MatchJoinedVct[indexItem]->sMatchCost;

        CCLabelTTF* pFirstTTF = CCLabelTTF::create(m_MatchJoinedVct[indexItem]->sFirst.c_str(), "Arial", 32);
        pFirstTTF->setColor(ccc3(32, 66, 117));
        pFirstTTF->setAnchorPoint(ccp(0, 0.5));
        pFirstTTF->setPosition(ccp(600, 200));
        m_pMatchNodeAwardArray->addObject(pFirstTTF);
        m_pMatchNode->addChild(pFirstTTF, 1);
        CCLabelTTF* pFirstAwardTTF = CCLabelTTF::create(m_MatchJoinedVct[indexItem]->sAwardFirst.c_str(), "Arial-BoldMT", 32);
        pFirstAwardTTF->setColor(ccc3(220, 121, 30));
        pFirstAwardTTF->setAnchorPoint(ccp(0, 0.5));
        pFirstAwardTTF->setPosition(ccp(pFirstTTF->getPositionX()+pFirstTTF->getContentSize().width+5, pFirstTTF->getPositionY()));
        m_pMatchNodeAwardArray->addObject(pFirstAwardTTF);
        m_pMatchNode->addChild(pFirstAwardTTF, 1);
        
        CCLabelTTF* pSecondTTF = CCLabelTTF::create(m_MatchJoinedVct[indexItem]->sSeconde.c_str(), "Arial", 32);
        pSecondTTF->setColor(ccc3(32, 66, 117));
        pSecondTTF->setAnchorPoint(ccp(0, 0.5));
        pSecondTTF->setPosition(ccp(890, 200));
        m_pMatchNodeAwardArray->addObject(pSecondTTF);
        m_pMatchNode->addChild(pSecondTTF, 1);
        CCLabelTTF* pSecondAwardTTF = CCLabelTTF::create(m_MatchJoinedVct[indexItem]->sAwardSecond.c_str(), "Arial-BoldMT", 32);
        pSecondAwardTTF->setColor(ccc3(220, 121, 30));
        pSecondAwardTTF->setAnchorPoint(ccp(0, 0.5));
        pSecondAwardTTF->setPosition(ccp(pSecondTTF->getPositionX()+pSecondTTF->getContentSize().width+5, pSecondTTF->getPositionY()));
        m_pMatchNodeAwardArray->addObject(pSecondAwardTTF);
        m_pMatchNode->addChild(pSecondAwardTTF, 1);
        
        CCLabelTTF* pThirdTTF = CCLabelTTF::create(m_MatchJoinedVct[indexItem]->sThird.c_str(), "Arial", 32);
        pThirdTTF->setColor(ccc3(32, 66, 117));
        pThirdTTF->setAnchorPoint(ccp(0, 0.5));
        pThirdTTF->setPosition(ccp(600, 160));
        m_pMatchNodeAwardArray->addObject(pThirdTTF);
        m_pMatchNode->addChild(pThirdTTF, 1);
        CCLabelTTF* pThirdAwardTTF = CCLabelTTF::create(m_MatchJoinedVct[indexItem]->sAwardThird.c_str(), "Arial-BoldMT", 32);
        pThirdAwardTTF->setColor(ccc3(220, 121, 30));
        pThirdAwardTTF->setAnchorPoint(ccp(0, 0.5));
        pThirdAwardTTF->setPosition(ccp(pThirdTTF->getPositionX()+pThirdTTF->getContentSize().width+5, pThirdTTF->getPositionY()));
        
        m_pMatchNodeAwardArray->addObject(pThirdAwardTTF);
        m_pMatchNode->addChild(pThirdAwardTTF, 1);
        
        CCLabelTTF* pFourthTTF = CCLabelTTF::create(m_MatchJoinedVct[indexItem]->sFourth.c_str(), "Arial", 32);
        pFourthTTF->setColor(ccc3(32, 66, 117));
        pFourthTTF->setAnchorPoint(ccp(0, 0.5));
        pFourthTTF->setPosition(ccp(890, 160));
        m_pMatchNodeAwardArray->addObject(pFourthTTF);
        m_pMatchNode->addChild(pFourthTTF, 1);
        CCLabelTTF* pFourthAwardTTF = CCLabelTTF::create(m_MatchJoinedVct[indexItem]->sAwardFourth.c_str(), "Arial-BoldMT", 32);
        pFourthAwardTTF->setColor(ccc3(220, 121, 30));
        pFourthAwardTTF->setAnchorPoint(ccp(0, 0.5));
        pFourthAwardTTF->setPosition(ccp(pFourthTTF->getPositionX()+pFourthTTF->getContentSize().width+5, pFourthTTF->getPositionY()));
        m_pMatchNodeAwardArray->addObject(pFourthAwardTTF);
        m_pMatchNode->addChild(pFourthAwardTTF, 1);
        
        CCLabelTTF* pJoinGroupFontTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinGroupFont);
        if (pJoinGroupFontTTF != NULL)
        {
            pJoinGroupFontTTF->setPositionY(320);
        }
        CCLabelTTF* pJoinScoreFontTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinScoreFont);
        if (pJoinScoreFontTTF != NULL)
        {
            pJoinScoreFontTTF->setPositionY(320);
        }
        
        CCLabelTTF* pJoinPercentTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinPercentTag);
        if (pJoinPercentTTF != NULL)
        {
            pJoinPercentTTF->setVisible(false);
        }
        
        CCLabelTTF* pJoinFontTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinNumFont);
        if (pJoinFontTTF != NULL)
        {
            pJoinFontTTF->setVisible(false);
        }
        CCLabelTTF* pJoinRoomTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinRoom);
        if (pJoinRoomTTF != NULL)
        {
            pJoinRoomTTF->setVisible(false);
        }
        CCLabelTTF* pJoinRoom2TTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoin2Room);
        if (pJoinRoom2TTF != NULL)
        {
            pJoinRoom2TTF->setVisible(false);
        }
    }
    else if (m_nIndexPage == 3)
    {
        int nCost = atoi(m_MatchEndVct[indexItem]->sMatchCost.c_str());
        if (nCost<=0) {
            sInstruction = m_MatchEndVct[indexItem]->sMatchName+"(免费报名)"+"  比赛时间: "+m_MatchEndVct[indexItem]->sMatchTime;
        }
        else{
            sInstruction = m_MatchEndVct[indexItem]->sMatchName+"  比赛时间: "+m_MatchEndVct[indexItem]->sMatchTime;
        }
        sMatchTime = sMatchTime+m_MatchEndVct[indexItem]->sMatchTime;
        sMatchRule1 = m_MatchEndVct[indexItem]->sMatchRule1;
        sMatchRule2 = m_MatchEndVct[indexItem]->sMatchRule2;
        sMatchRule3 = m_MatchEndVct[indexItem]->sMatchRule3;
        sMatchPersonNum = m_MatchEndVct[indexItem]->sMatchGroupNum;
        sMatchJoinScore = m_MatchEndVct[indexItem]->sMatchCost;
        
        
        CCLabelTTF* pInstructionTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchInstructionTag);
        if (pInstructionTTF != NULL)
        {
            pInstructionTTF->setString(sInstruction.c_str());
        }
        
        CCLabelTTF* pJoinGroupFontTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinGroupFont);
        if (pJoinGroupFontTTF != NULL)
        {
            pJoinGroupFontTTF->setPositionY(320);
        }
        CCLabelTTF* pJoinScoreFontTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinScoreFont);
        if (pJoinScoreFontTTF != NULL)
        {
            pJoinScoreFontTTF->setPositionY(320);
        }
        
        CCLabelTTF* pJoinPercentTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinPercentTag);
        if (pJoinPercentTTF != NULL)
        {
            pJoinPercentTTF->setVisible(false);
        }
        
        CCLabelTTF* pJoinFontTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinNumFont);
        if (pJoinFontTTF != NULL)
        {
            pJoinFontTTF->setVisible(false);
        }
        CCLabelTTF* pJoinRoomTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinRoom);
        if (pJoinRoomTTF != NULL)
        {
            pJoinRoomTTF->setVisible(false);
        }
        CCLabelTTF* pJoinRoom2TTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoin2Room);
        if (pJoinRoom2TTF != NULL)
        {
            pJoinRoom2TTF->setVisible(false);
        }
        
        std::string sRankAward = "排名: "+m_MatchEndVct[indexItem]->sRank+"    奖励: "+m_MatchEndVct[indexItem]->sAward+"    积分: "+m_MatchEndVct[indexItem]->sScore;
        CCLabelTTF* pSelfTTF1 = CCLabelTTF::create("排名: ", "Arial", 32);
        pSelfTTF1->setColor(ccc3(32, 66, 117));
        pSelfTTF1->setAnchorPoint(ccp(0.5f, 0.5f));
        pSelfTTF1->setPosition(ccp(845-270, 170));
        m_pMatchNodeAwardArray->addObject(pSelfTTF1);
        m_pMatchNode->addChild(pSelfTTF1, 1);
        CCLabelTTF* pSelfTTF2 = CCLabelTTF::create("奖励: ", "Arial", 32);
        pSelfTTF2->setColor(ccc3(32, 66, 117));
        pSelfTTF2->setAnchorPoint(ccp(0.5f, 0.5f));
        pSelfTTF2->setPosition(ccp(845-70, 170));
        m_pMatchNodeAwardArray->addObject(pSelfTTF2);
        m_pMatchNode->addChild(pSelfTTF2, 1);
        CCLabelTTF* pSelfTTF3 = CCLabelTTF::create("积分: ", "Arial", 32);
        pSelfTTF3->setColor(ccc3(32, 66, 117));
        pSelfTTF3->setAnchorPoint(ccp(0.5f, 0.5f));
        pSelfTTF3->setPosition(ccp(845+175, 170));
        m_pMatchNodeAwardArray->addObject(pSelfTTF3);
        m_pMatchNode->addChild(pSelfTTF3, 1);
        
        CCLabelTTF* pSelfRankTTF = CCLabelTTF::create(m_MatchEndVct[indexItem]->sRank.c_str(), "Arial-BoldMT", 32);
        pSelfRankTTF->setColor(ccc3(220, 121, 30));
        pSelfRankTTF->setAnchorPoint(ccp(0, 0.5f));
        pSelfRankTTF->setPosition(ccp(pSelfTTF1->getPositionX()+40, 170));
        m_pMatchNodeAwardArray->addObject(pSelfRankTTF);
        m_pMatchNode->addChild(pSelfRankTTF, 1);
        
        CCLabelTTF* pSelfRankAwardTTF = CCLabelTTF::create(m_MatchEndVct[indexItem]->sAward.c_str(), "Arial-BoldMT", 32);
        pSelfRankAwardTTF->setColor(ccc3(220, 121, 30));
        pSelfRankAwardTTF->setAnchorPoint(ccp(0, 0.5f));
        pSelfRankAwardTTF->setPosition(ccp(pSelfTTF2->getPositionX()+40, 170));
        m_pMatchNodeAwardArray->addObject(pSelfRankAwardTTF);
        m_pMatchNode->addChild(pSelfRankAwardTTF, 1);
        
        CCLabelTTF* pSelfRankScoreTTF = CCLabelTTF::create(m_MatchEndVct[indexItem]->sScore.c_str(), "Arial-BoldMT", 32);
        pSelfRankScoreTTF->setColor(ccc3(220, 121, 30));
        pSelfRankScoreTTF->setAnchorPoint(ccp(0, 0.5f));
        pSelfRankScoreTTF->setPosition(ccp(pSelfTTF3->getPositionX()+40, 170));
        m_pMatchNodeAwardArray->addObject(pSelfRankScoreTTF);
        m_pMatchNode->addChild(pSelfRankScoreTTF, 1);
    }
    
    CCLabelTTF* pRule1TTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeRule1Tag);
    if (pRule1TTF != NULL)
    {
        pRule1TTF->setString(sMatchRule1.c_str());
    }
    CCLabelTTF* pRule2TTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeRule2Tag);
    if (pRule2TTF != NULL)
    {
        pRule2TTF->setString(sMatchRule2.c_str());
    }
    CCLabelTTF* pRule3TTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeRule3Tag);
    if (pRule3TTF != NULL)
    {
        pRule3TTF->setString(sMatchRule3.c_str());
    }
    CCLabelTTF* pPersonNumTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodePersonNumTag);
    if (pPersonNumTTF != NULL)
    {
        if (m_nIndexPage == 1) {
            pPersonNumTTF->setPositionY(340);
        }
        else if (m_nIndexPage == 2 || m_nIndexPage == 3) {
            pPersonNumTTF->setPositionY(320);
        }
        pPersonNumTTF->setString(sMatchPersonNum.c_str());
    }
    CCLabelTTF* pJoinScoreTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchNodeJoinScoreTag);
    if (pJoinScoreTTF != NULL)
    {
        if (m_nIndexPage == 1) {
            pJoinScoreTTF->setPositionY(340);
        }
        else if (m_nIndexPage == 2 || m_nIndexPage == 3) {
            pJoinScoreTTF->setPositionY(320);
        }
        pJoinScoreTTF->setString(sMatchJoinScore.c_str());
    }

    m_pMatchNode->setVisible(true);
}

void CRankNewLayer::callBack(CCObject* obj)
{
    CCMenuItem* item = (CCMenuItem*)obj;
    int _Tag= item->getTag();
    if (_Tag == eBackTag)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    }
    else
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    }
    switch (_Tag) {
        case eOnGoingTag:
        {
            if (GameMainScene::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameMatchOnGoing);
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallMatchOnGoing);
            }
            
            m_spAnimate->stopAllActions();
            m_spAnimate->setVisible(false);
            
            setMenuItemVisible(eOnGoingTag, false, m_systemMenu);
            setMenuItemVisible(eMyEnrolTag, true, m_systemMenu);
            setMenuItemVisible(eMyMatchTag, true, m_systemMenu);
            setMenuItemVisible(eRankTag, true, m_systemMenu);
            setMenuItemVisible(eMtachAwardLingQu, false, m_matchMenu);
            m_pMatchScrollView->setVisible(false);
            m_pNoMatchesTTF->setVisible(false);
            m_pNoMatchInfoTTF->setVisible(false);
            m_pMatchNode->setVisible(false);
            m_pSpRankBg->setVisible(false);
            m_pSpJoined->setVisible(false);
            m_pSpLingQued->setVisible(false);
            m_pRankScrollView->setTouchEnabled(false);
            m_pNumNoLingQuTTF->setColor(ccWHITE);
            
            m_pHeadShade->setPosition(ccp(item->getPositionX(), item->getPositionY()-8));
            
            m_nGuangItemID=-1;
            m_nIndexPage = 1;
            m_nShowItem = 0;
            m_nodeMenuStatus -> setVisible(true);
            loadMatchingUI();
            break;
        }
        case eMyEnrolTag:
        {
            if (GameMainScene::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameMatchMyEnrol);
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallMatchMyEnrol);
            }
            m_spAnimate->stopAllActions();
            m_spAnimate->setVisible(false);
            
            setMenuItemVisible(eOnGoingTag, true, m_systemMenu);
            setMenuItemVisible(eMyEnrolTag, false, m_systemMenu);
            setMenuItemVisible(eMyMatchTag, true, m_systemMenu);
            setMenuItemVisible(eRankTag, true, m_systemMenu);
            setMenuItemVisible(eJoinFreeTag, false, m_matchMenu);
            setMenuItemVisible(eJoinCoinTag, false, m_matchMenu);
            setMenuItemVisible(eMtachAwardLingQu, false, m_matchMenu);
            m_pMatchScrollView->setVisible(false);
            m_pNoMatchesTTF->setVisible(false);
            m_pNoMatchInfoTTF->setVisible(false);
            m_pMatchNode->setVisible(false);
            m_pSpRankBg->setVisible(false);
            m_pSpJoined->setVisible(false);
            m_pSpLingQued->setVisible(false);
            m_pRankScrollView->setTouchEnabled(false);
            m_pNumNoLingQuTTF->setColor(ccWHITE);
            
            m_pHeadShade->setPosition(ccp(item->getPositionX(), item->getPositionY()-8));
            
            m_nGuangItemID=-1;
            m_nIndexPage = 2;
            m_nShowItem = 0;
            m_nodeMenuStatus -> setVisible(false);
            if (m_bJoinAct || m_bJoinedFirstRequest)
            {
                //加入一个加载动画，用于展示请求
                m_spAnimate -> setVisible(true);
                m_spAnimate -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
                
                requestCatchFishMatchJoinedHttp();
            }
            else
            {
                loadMatchJoinedUI();
            }
            break;
        }
        case eMyMatchTag:
        {
            if (GameMainScene::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameMatchMyMatch);
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallMatchMyMatch);
            }
            m_spAnimate->stopAllActions();
            m_spAnimate->setVisible(false);
            
            setMenuItemVisible(eOnGoingTag, true, m_systemMenu);
            setMenuItemVisible(eMyEnrolTag, true, m_systemMenu);
            setMenuItemVisible(eMyMatchTag, false, m_systemMenu);
            setMenuItemVisible(eRankTag, true, m_systemMenu);
            setMenuItemVisible(eJoinFreeTag, false, m_matchMenu);
            setMenuItemVisible(eJoinCoinTag, false, m_matchMenu);
            m_pMatchScrollView->setVisible(false);
            m_pNoMatchesTTF->setVisible(false);
            m_pNoMatchInfoTTF->setVisible(false);
            m_pMatchNode->setVisible(false);
            m_pSpRankBg->setVisible(false);
            m_pSpJoined->setVisible(false);
            m_pSpLingQued->setVisible(false);
            m_pRankScrollView->setTouchEnabled(false);
            m_pNumNoLingQuTTF->setColor(ccBLACK);
            
            m_pHeadShade->setPosition(ccp(item->getPositionX(), item->getPositionY()-8));
            
            m_nGuangItemID=-1;
            m_nIndexPage = 3;
            m_nShowItem = 0;
            m_nodeMenuStatus -> setVisible(false);
            if (!m_bHistoryRequst)
            {
                //加入一个加载动画，用于展示请求
                m_spAnimate -> setVisible(true);
                m_spAnimate -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
                
                requestCatchFishHistoryMatchHttp();
            }
            else
            {
                loadMatchedUI();
            }
            
            break;
        }
        case eRankTag:
        {
            if (GameMainScene::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameMatchRank);
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallMatchRank);
            }
            m_spAnimate->stopAllActions();
            m_spAnimate->setVisible(false);
            
            setMenuItemVisible(eOnGoingTag, true, m_systemMenu);
            setMenuItemVisible(eMyEnrolTag, true, m_systemMenu);
            setMenuItemVisible(eMyMatchTag, true, m_systemMenu);
            setMenuItemVisible(eRankTag, false, m_systemMenu);
            setMenuItemVisible(eJoinFreeTag, false, m_matchMenu);
            setMenuItemVisible(eJoinCoinTag, false, m_matchMenu);
            setMenuItemVisible(eMtachAwardLingQu, false, m_matchMenu);
            m_pMatchScrollView->setVisible(false);
            m_pNoMatchesTTF->setVisible(false);
            m_pNoMatchInfoTTF->setVisible(false);
            m_pNoRankInfoTTF->setVisible(false);
            m_pSpJoined->setVisible(false);
            m_pSpLingQued->setVisible(false);
            m_pMatchNode->setVisible(false);
            m_pRankScrollView->setTouchEnabled(true);
            m_pNumNoLingQuTTF->setColor(ccWHITE);
            
            m_pHeadShade->setPosition(ccp(item->getPositionX(), item->getPositionY()-8));
            
            m_nGuangItemID=-1;
            m_nIndexPage = 4;
            m_nShowItem = 0;
            m_nodeMenuStatus -> setVisible(false);
            m_pSpRankBg->setVisible(true);
            
            if (!m_bRankRequest)
            {
                //加入一个加载动画，用于展示请求
                m_spAnimate -> setVisible(true);
                m_spAnimate -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
                
                requestCatchFishMatchRankHttp();
            }
            else
            {
                if (m_MatchRankVct.size()<=0) {
                    m_pNoRankInfoTTF->setVisible(true);
                    m_pSpRankChildBg->setVisible(false);
                }
                else
                {
                    m_pNoRankInfoTTF->setVisible(false);
                    m_pSpRankChildBg->setVisible(true);
                }
            }
            break;
        }
        case eBackTag:
        {
            if (GameMainScene::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameMatchClose);
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallMatchClose);
            }
            this->removeFromParent();
            //如果是比赛指引关闭此界面
            if(CCUserDefault::sharedUserDefault()->getBoolForKey("bMatchGuide",true))
            {
                CCUserDefault::sharedUserDefault()->setBoolForKey("bMatchGuide", false);
                //通知捕鱼还是大厅显示比赛指引结束界面
                if(GameMainScene::_instance != NULL)
                {
                    GameMainScene::_instance->showMatchGuideEnd(m_bJoinStatus);//ture or false
                }
                else if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                {
                    CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                    if(pHallLayer != NULL)
                    {
                        pHallLayer -> showMatchGuideEnd(m_bJoinStatus);//ture or false
                    }
                }
			}
            CEnrollRemind* pEnrollRemindLayer = (CEnrollRemind*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(eCmptNotifyTag);
            if(pEnrollRemindLayer != NULL)
            {
                pEnrollRemindLayer -> setVisible(true);
            }
            //显示比赛中玩家分数排名更新框
            CCmptStatus* pCmptStatusLayer = (CCmptStatus*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(eCmptStatusTag);
            if(pCmptStatusLayer != NULL)
            {
                pCmptStatusLayer -> setVisible(true);
            }
            break;
        }
        case eJoinFreeTag:
        {
            if (GameMainScene::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameMatchJoinFree);
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallMatchJoinFree);
            }
            //发送消息，在接受到消息的地方，成功，将其加入到对应的容器里面，同时对ui进行更新
            CMD_GRR_MatchJoin MatchJoin = {0};
            MatchJoin.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            MatchJoin.nMatchID = atoi(m_MatchingVct[m_nShowItem]->sMatchIndex.c_str());
            printf("游戏内 免费报名\n");
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_USER, SUB_GR_Match_SIGNUP, &MatchJoin, sizeof(MatchJoin));
            
            CMainLogic::sharedMainLogic()->ShowMessage("正在报名，请稍候", eMBGameNull);
            
            break;
        }
        case eJoinCoinTag:
        {
            if (GameMainScene::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameMatchJoinCoin);
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallMatchJoinCoin);
            }
            
            //发送消息，在接受到消息的地方，成功，将其加入到对应的容器里面，同时对ui进行更新
            CMD_GRR_MatchJoin MatchJoin = {0};
            MatchJoin.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            MatchJoin.nMatchID = atoi(m_MatchingVct[m_nShowItem]->sMatchIndex.c_str());
            
            if (GameMainScene::_instance != NULL)
            {
                CCLOG("游戏内 金币报名");
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_USER, SUB_GR_Match_SIGNUP, &MatchJoin, sizeof(MatchJoin));
            }
            else
            {
                CCLOG("大厅内 金币报名");
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GR_USER, SUB_GR_Match_SIGNUP, &MatchJoin, sizeof(MatchJoin));
            }
            CMainLogic::sharedMainLogic()->ShowMessage("正在报名，请稍候", eMBGameNull);
            break;
        }
        case eMtachAwardLingQu:
        {
            int nShare = m_MatchEndVct[m_nShowItem]->nShared;
            if (nShare==0)
            {
                if (GameMainScene::_instance != NULL)
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameMatchLingQuDirect);
                }
                else
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallMatchLingQuDirect);
                }
                
                CMD_GRO_MatchGetMatchPrise MatchGetMatchPrise = {0};//                       //用户领取奖励
                MatchGetMatchPrise.nMatchID = atoi(m_MatchEndVct[m_nShowItem]->sMatchIndex.c_str());
                MatchGetMatchPrise.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                strncpy(MatchGetMatchPrise.szMatchDate, CCharCode::UTF8ToGB2312(m_MatchEndVct[m_nShowItem]->sMatchDay.c_str()).c_str(), sizeof(MatchGetMatchPrise.szMatchDate));
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_USER, SUB_GR_MATCH_GET_MATCH_PRIZE, &MatchGetMatchPrise, sizeof(MatchGetMatchPrise));
                CMainLogic::sharedMainLogic()->ShowMessage("正在请求，请稍候", eMBExitGame);
            }
            else if (nShare==1)//需要分享
            {
                //去服务器请求能不能分享
                CMD_GRR_CanMatchShare CanMatchShare = {0};
                CanMatchShare.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                CanMatchShare.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                CanMatchShare.nMatchID = atoi(m_MatchEndVct[m_nShowItem]->sMatchIndex.c_str());
                CanMatchShare.nRank = m_MatchEndVct[m_nShowItem]->nRank;
                strncpy(CanMatchShare.szMatchDate, CCharCode::UTF8ToGB2312(m_MatchEndVct[m_nShowItem]->sMatchDay.c_str()).c_str(), sizeof(CanMatchShare.szMatchDate));
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_USER, SUB_GRR_CAN_MATCH_SHARE, &CanMatchShare, sizeof(CanMatchShare));
                CMainLogic::sharedMainLogic()->ShowMessage("正在请求，请稍候", eMBExitGame);
            }
            break;
        }
        default:
            break;
    }
}

void CRankNewLayer::setMatchShare(BYTE cbShare)
{
    if(cbShare)//需要分享，去服务器请求域名，然后进行分享，走网站领取结果
    {
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
        requestShareEndSuccessMatchAward();
#else
        if (GameMainScene::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameMatchLingQuShare);
        }
        else
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallMatchLingQuShare);
        }
        //分享领奖
        CMD_GR_MatchShareInfo MatchShareInfo = {0};
        MatchShareInfo.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
        MatchShareInfo.nMatchID = atoi(m_MatchEndVct[m_nShowItem]->sMatchIndex.c_str());
        MatchShareInfo.nRank = m_MatchEndVct[m_nShowItem]->nRank;
        strncpy(MatchShareInfo.szMatchDate, CCharCode::UTF8ToGB2312(m_MatchEndVct[m_nShowItem]->sMatchDay.c_str()).c_str(), sizeof(MatchShareInfo.szMatchDate));
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_USER, SUB_GR_MATCH_SHAREINFO, &MatchShareInfo, sizeof(MatchShareInfo));
#endif
    }
    else//不需要分享，则直接向服务器发送领取消息
    {
        if (GameMainScene::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameMatchLingQuDirect);
        }
        else
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallMatchLingQuDirect);
        }
        
        CMD_GRO_MatchGetMatchPrise MatchGetMatchPrise = {0};//                       //用户领取奖励
        MatchGetMatchPrise.nMatchID = atoi(m_MatchEndVct[m_nShowItem]->sMatchIndex.c_str());
        MatchGetMatchPrise.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
        strncpy(MatchGetMatchPrise.szMatchDate, CCharCode::UTF8ToGB2312(m_MatchEndVct[m_nShowItem]->sMatchDay.c_str()).c_str(), sizeof(MatchGetMatchPrise.szMatchDate));
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_USER, SUB_GR_MATCH_GET_MATCH_PRIZE, &MatchGetMatchPrise, sizeof(MatchGetMatchPrise));
    }
}

void CRankNewLayer::setMenuItemVisible(int tag, bool bShow, CCMenu* menu)
{
    if (menu == NULL) {
        return ;
    }
    for (int i=0; i<menu->getChildrenCount(); i++)
    {
        CCMenuItem* item = (CCMenuItem*)menu->getChildren()->objectAtIndex(i);
        if (item != NULL && tag==item->getTag())
        {
            if (tag == item->getTag())
            {
                item->setVisible(bShow);
                break;
            }
        }
    }
}

void CRankNewLayer::sendShareInfo(std::string str1, std::string str2)
{
    CMainLogic::sharedMainLogic()->m_nShareInterface = 2;
    //只做微信分享
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(ChargeWebView::checkWX())
    {
        CMainLogic::sharedMainLogic()->m_bSharedState = true;
        WeiXinCharge = 2;
        ChargeWebView::openIOSShare(str1, str2, 1);
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

void CRankNewLayer::requestShareEndSuccessMatchAward()
{
    char szMD5Result[256];
    char userID[256];
    sprintf(userID, "uid=%d&mnum=%s&mday=%s&time=%lubuyudaka2016", CMainLogic::sharedMainLogic()->m_nUserID,m_MatchEndVct[m_nShowItem]->sMatchIndex.c_str(),m_MatchEndVct[m_nShowItem]->sMatchDay.c_str(), time(0));
    m_nLingQuMatchID = atoi(m_MatchEndVct[m_nShowItem]->sMatchIndex.c_str());
    CEncryptMD5::toString32(userID,szMD5Result);
    char sMatchAwardUrl[256];
    sprintf(sMatchAwardUrl, "http://%s/ShareNewed.aspx?uid=%d&mnum=%s&mday=%s&time=%lu&info=%s", NORMAL_URL, CMainLogic::sharedMainLogic()->m_nUserID,m_MatchEndVct[m_nShowItem]->sMatchIndex.c_str(),m_MatchEndVct[m_nShowItem]->sMatchDay.c_str(), time(0), szMD5Result);
    CCLOG("sMatchAwardUrl=%s",sMatchAwardUrl);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(sMatchAwardUrl);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(CRankNewLayer::onHttpMatchEndAwardComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CRankNewLayer::onHttpMatchEndAwardComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    std::string sContent = "";
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
        TiXmlElement* rootElement = document->RootElement();//Root
        if (rootElement == NULL)
        {
            return;
        }
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("ShareInfo"))
            return;
        TiXmlElement* pChildElement=rootElement->FirstChildElement();
        
        while (pChildElement!=NULL)
        {
            nodename=pChildElement->Value();
            if (0==nodename.compare("text"))
            {
                sContent = pChildElement->GetText();
                CMainLogic::sharedMainLogic()->ShowMessage(sContent.c_str(), eMBOK);
            }
            if (0==nodename.compare("bool"))
            {
                std::string sResult = pChildElement->GetText();
                if(sResult == "true")
                {
                    refreshMatchingInfo(3, m_nLingQuMatchID);
                }
            }
            pChildElement = pChildElement->NextSiblingElement();
        }
    }
    delete document;
}

int CRankNewLayer::selectHuaFeiNum(std::string sHuaFei)
{
    int tNum = -1;
    //字符串截取数字
    for (int k = 0; k<sHuaFei.length(); k++)
    {
        char cbNum = sHuaFei[k];
        if (cbNum >= '0' && cbNum <= '9')
        {
            tNum = cbNum-'0';
            return tNum;
        }
    }
    return tNum;
}

std::string CRankNewLayer::selectStartMatchTime(std::string sMatchTime)
{
    std::string sStartStr="";
    int size = (int)sMatchTime.find("-");
    if (size > 0)//包换，并且不是在第一个
    {
        if (m_nIndexPage==1 || m_nIndexPage==2)
        {
            sStartStr = "今日: "+sMatchTime.substr(0, size);
        }
        else
        {
            sStartStr = sMatchTime.substr(0, size);
        }
    }
    return sStartStr;
}

void CRankNewLayer::showMyJoinedMatchCountDownTime(float dt)
{
    for (int i=0; i<m_MatchJoinedVct.size(); i++)
    {
        m_MatchJoinedVct[i]->nSecond--;
    }
    if (m_MatchingVct.size() > 0)
    {
        m_MatchingVct[0]->nSecond--;
    }
    setTime();
}

void CRankNewLayer::setTime()
{
    if (m_nIndexPage == 1 && m_nShowItem == 0)
    {
        CCLabelTTF* pInstructionTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchInstructionTag);
        if (pInstructionTTF == NULL) {
            return;
        }
        if (m_MatchingVct.size()<=0) {
            return;
        }
        if (m_MatchingVct[0]->nSecond < -1)
        {
            return;
        }
        
        int nHour = m_MatchingVct[0]->nSecond/3600;
        int nMinute = (m_MatchingVct[0]->nSecond-nHour*3600)/60;
        int nSecond = (m_MatchingVct[0]->nSecond-nHour*3600-nMinute*60)%60;
        if (nHour==0 && nMinute==0 && nSecond==-1)
        {
            CCLOG("倒计时结束，重新请求");
            //清空进行中数据; 同时scrollview、matchNode不可见；
            //重新请求数进行中的比赛数据，
            //加载动画可见
            //return
            m_MatchingVct[0]->nSecond = -2;
            m_MatchingVct.clear();
            m_pMatchScrollView->setVisible(false);
            m_pMatchNode->setVisible(false);
            
            m_nIndexPage = 1;
            m_nShowItem = 0;
            //加入一个加载动画，用于展示请求
            m_spAnimate -> setVisible(true);
            m_spAnimate -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
            
            requestCatchFishMatchingHttp();
            
            return;
        }
        std::string sTime="";
        if (nHour>0)
        {
            char sHour[8];
            if (nHour<10) {
                sprintf(sHour, "0%d:", nHour);
            }
            else{
                sprintf(sHour, "%d:", nHour);
            }
            sTime = sHour;
        }
        
        char sMinute[8];
        char sSecond[8];
        if (nMinute<10) {
            sprintf(sMinute, "0%d:", nMinute);
        }
        else{
            sprintf(sMinute, "%d:", nMinute);
        }
        if (nSecond<10) {
            sprintf(sSecond, "0%d", nSecond);
        }
        else{
            sprintf(sSecond, "%d", nSecond);
        }
        
        int nCost = atoi(m_MatchingVct[m_nShowItem]->sMatchCost.c_str());
        if (nCost<=0) {
            sTime = m_MatchingVct[m_nShowItem]->sMatchName+"(免费报名)"+" 倒计时: "+sTime+sMinute+sSecond;
        }
        else{
            sTime = m_MatchingVct[m_nShowItem]->sMatchName+" 倒计时: "+sTime+sMinute+sSecond;
        }
        pInstructionTTF->setString(sTime.c_str());
        pInstructionTTF->setVisible(true);
    }
    else if (m_nIndexPage == 2)
    {
        CCLabelTTF* pInstructionTTF = (CCLabelTTF*)m_pMatchNode->getChildByTag(eMatchInstructionTag);
        if (pInstructionTTF == NULL) {
            return;
        }
        if (m_MatchJoinedVct.size()<=0) {
            return;
        }
        if (m_nShowItem==0 && m_MatchJoinedVct[0]->nSecond<=0)
        {
            std::string strTime="";
            int nCost = atoi(m_MatchJoinedVct[0]->sMatchCost.c_str());
            if (nCost<=0) {
                strTime = m_MatchJoinedVct[0]->sMatchName+"(免费报名)"+" 比赛时间: "+m_MatchJoinedVct[0]->sMatchTime;
            }
            else{
                strTime = m_MatchJoinedVct[0]->sMatchName+m_MatchJoinedVct[0]->sMatchTime;
            }
            pInstructionTTF->setString(strTime.c_str());
            return;
        }
        int nHour = m_MatchJoinedVct[m_nShowItem]->nSecond/3600;
        int nMinute = (m_MatchJoinedVct[m_nShowItem]->nSecond-nHour*3600)/60;
        int nSecond = (m_MatchJoinedVct[m_nShowItem]->nSecond-nHour*3600-nMinute*60)%60;
        std::string sTime="";
        if (nHour>0)
        {
            char sHour[8];
            if (nHour<10) {
                sprintf(sHour, "0%d:", nHour);
            }
            else{
                sprintf(sHour, "%d:", nHour);
            }
            sTime = sHour;
        }
        
        char sMinute[8];
        char sSecond[8];
        if (nMinute<10) {
            sprintf(sMinute, "0%d:", nMinute);
        }
        else{
            sprintf(sMinute, "%d:", nMinute);
        }
        if (nSecond<10) {
            sprintf(sSecond, "0%d", nSecond);
        }
        else{
            sprintf(sSecond, "%d", nSecond);
        }
        
        int nCost = atoi(m_MatchJoinedVct[m_nShowItem]->sMatchCost.c_str());
        if (m_MatchJoinedVct[m_nShowItem]->nSecond>0) {
            if (nCost<=0) {
                sTime = m_MatchJoinedVct[m_nShowItem]->sMatchName+"(免费报名)"+" 倒计时: "+sTime+sMinute+sSecond;
            }
            else{
                sTime = m_MatchJoinedVct[m_nShowItem]->sMatchName+" 倒计时: "+sTime+sMinute+sSecond;
            }
        }
        else{
            if (nCost<=0) {
                sTime = m_MatchJoinedVct[m_nShowItem]->sMatchName+"(免费报名)"+" 比赛时间: "+m_MatchJoinedVct[m_nShowItem]->sMatchTime;
            }
            else{
                sTime = m_MatchJoinedVct[m_nShowItem]->sMatchName+" 比赛时间: "+m_MatchJoinedVct[m_nShowItem]->sMatchTime;
            }
        }
        
        pInstructionTTF->setString(sTime.c_str());
    }
}

void CRankNewLayer::playSelectItemAnimate(CCPoint p, int nPageIndex, int nItemIndex)
{
    if ((m_nGuangPageID==nPageIndex) && (m_nGuangItemID==nItemIndex))
    {
        return;
    }
    
    CCSprite* pBefore = (CCSprite*)m_pMatchContainLayer->getChildByTag(9990);
    if (pBefore != NULL) {
        pBefore->stopAllActions();
        pBefore->removeFromParentAndCleanup(true);
    }
    
    char temp[32];
    CCArray* array = CCArray::create();
    for(int i = 1; i < 78; i++)
    {
        sprintf(temp, "selectItemAct%d.png", i+1);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, 0.03f);
    CCAnimate* animate = CCAnimate::create(animation);
    
    CCSprite* pFirstFrame = CCSprite::createWithSpriteFrameName("selectItemAct1.png");
    pFirstFrame->setPosition(p);
    m_pMatchContainLayer->addChild(pFirstFrame, 3, 9990);
    pFirstFrame->runAction(CCRepeatForever::create(animate));
    
    m_nGuangPageID=nPageIndex;
    m_nGuangItemID = nItemIndex;
}

void CRankNewLayer::refreshMatchingInfo(int nMatchType, int nMatchID)
{
    if (nMatchType == 1)//第一个界面
    {
        m_bJoinAct=true;
        int nItemID=-1;
        int nSelectItemID=-1;
        for (int i=0; i<m_MatchingVct.size(); i++)
        {
            nItemID = atoi(m_MatchingVct[i]->sMatchIndex.c_str());
            if (nItemID == nMatchID)
            {
                nSelectItemID=i;
                m_MatchingVct[i]->bSignup = true;
                break;
            }
        }
        if (m_nIndexPage == 1 && m_nShowItem == nSelectItemID)
        {
//            m_MatchingVct.clear();
//            m_pMatchScrollView->setVisible(false);
//            m_pMatchNode->setVisible(false);
//            
//            //加入一个加载动画，用于展示请求
//            m_spAnimate -> setVisible(true);
//            m_spAnimate -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
//
            m_nJoinMatchID = nMatchID;
            
            m_bJoinRefresh = true;
            //requestCatchFishMatchingHttp();
            requestCatchFishFinishJoinMatchHttp();
        }
    }
    else if (nMatchType == 3)//第三个界面
    {
        int nItemID=-1;
        int nSelectItemID=-1;
        for (int i=0; i<m_MatchEndVct.size(); i++)
        {
            nItemID = atoi(m_MatchEndVct[i]->sMatchIndex.c_str());
            if (nItemID == nMatchID)
            {
                nSelectItemID=i;
                m_MatchEndVct[i]->nGet = 1;
                break;
            }
        }
        if (m_nIndexPage == 3 && m_nShowItem == nSelectItemID)
        {
            setMenuItemVisible(eMtachAwardLingQu, false, m_matchMenu);
            m_pSpLingQued->setVisible(true);
            
            int nNewCount = atoi(m_pNumNoLingQuTTF->getString()) - 1;
            nNewCount = nNewCount>=0?nNewCount:0;
            char sNewCount[8];
            sprintf(sNewCount, "%d", nNewCount);
            m_pNumNoLingQuTTF->setString(sNewCount);
            if (nNewCount <=0 ) {
                m_pNoLingQu->setVisible(false);
            }
            else{
                m_pNoLingQu->setVisible(true);
            }
            
            CCSprite* pNoLingQu = (CCSprite*)m_pMatchContainLayer->getChildByTag(600+m_nShowItem);
            if (pNoLingQu != NULL) {
                pNoLingQu->removeFromParentAndCleanup(true);
            }
        }
    }
}

void CRankNewLayer::setAlreadyJoinedVisible()
{
    setMenuItemVisible(eJoinFreeTag, false, m_matchMenu);
    setMenuItemVisible(eJoinCoinTag, false, m_matchMenu);
    m_pSpJoined->setVisible(true);
}

void CRankNewLayer::setBisaiOpenStatus(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    switch(item->getTag())
    {
        case 101:
            m_menuOpenStatus -> setVisible(false);
            m_menuCloseStatus -> setVisible(true);
            CMainLogic::sharedMainLogic()->m_bShowNotifyInfo = false;
            CCUserDefault::sharedUserDefault()->setBoolForKey("ShowBisaiNotifyInfo", false);
            CMainLogic::sharedMainLogic()->ShowMessage("您已关闭报名提醒", eMBOK);
            break;
        case 102:
            m_menuOpenStatus -> setVisible(true);
            m_menuCloseStatus -> setVisible(false);
            CMainLogic::sharedMainLogic()->m_bShowNotifyInfo = true;
            CCUserDefault::sharedUserDefault()->setBoolForKey("ShowBisaiNotifyInfo", true);
            CMainLogic::sharedMainLogic()->ShowMessage("您已开启报名提醒", eMBOK);
            break;
    }
}
