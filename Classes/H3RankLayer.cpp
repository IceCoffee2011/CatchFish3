#include "header.h"

const char* H3RankName[3] = {"h3rank_icon1.png", "h3rank_icon2.png", "h3rank_icon3.png"};

extern char* txName[10];

H3RankLayer::H3RankLayer()
{
    
}

H3RankLayer::~H3RankLayer()
{
    
}

bool H3RankLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_bClickInfoMenu = false;
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    m_vctTodayEarn.clear();
    m_vctWeekEarn.clear();
    m_vctEstEarn.clear();
    m_vctFoodEarn.clear();
    
    
    m_foodEarn.llScore = 0;
    m_foodEarn.nFaceID = 0;
    m_foodEarn.sName = "";
    m_foodEarn.sInfo = "";
    m_foodEarn.llTotalScore = 0;
    
    loadUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void H3RankLayer::loadUI()
{
    m_spBg = CCSprite::create("h3card/nopack_window_frame.png");
    m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-20));
    this -> addChild(m_spBg);
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.55f), CCScaleTo::create(0.07f, 1.45f), NULL));
    CCSprite* pTitle = CCSprite::createWithSpriteFrameName("h3rank_title.png");
    pTitle -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height-3));
    m_spBg -> addChild(pTitle);
    pTitle -> setScale(0.68f);
    
    m_pRankBg = CCSprite::create("h3card/unpack_rank_bg.png");
    m_pRankBg -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2+20));
    m_spBg -> addChild(m_pRankBg);
    
    m_spInfoBg = CCScale9Sprite::create("h3card/nopack_caichi_msgbg.png");//createWithSpriteFrameName("h3rank_tip_bg.png");//
    m_spInfoBg -> setContentSize(CCSize(200, 100));
    m_spInfoBg -> setPosition(ccp(m_pRankBg->getContentSize().width/2, m_pRankBg->getContentSize().height/2+20));
    m_pRankBg -> addChild(m_spInfoBg, 10);
    CCSize size(190,95);
    int nSize = 18;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    nSize = 16;
#endif
    m_labelInfo = CCLabelTTF::create("每天下注盈利累计最多的玩家，每天0:00重置", "Arial", nSize,size,kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    m_labelInfo->setColor(ccc3(255, 255, 255));
    m_labelInfo->setPosition(ccp(5, 95));
    m_labelInfo -> setAnchorPoint(ccp(0, 1));
    m_spInfoBg->addChild(m_labelInfo);
    m_spInfoBg -> setVisible(false);
    
    //bottom menu
    CCSprite* spToday = CCSprite::createWithSpriteFrameName("h3rank_tab_n.png");
    CCMenuItemSprite* pItemTodayEarn = CCMenuItemSprite::create(spToday, spToday, this, menu_selector(H3RankLayer::clickMenu));
    pItemTodayEarn -> setTag(101);
    pItemTodayEarn -> setPosition(ccp(m_spBg->getContentSize().width/5-30, 40));
    pItemTodayEarn -> setScale(0.68f);
    m_menuTodayEarn = CCMenu::create(pItemTodayEarn, NULL);
    m_menuTodayEarn -> setPosition(CCPointZero);
    m_menuTodayEarn -> setAnchorPoint(CCPointZero);
    m_spBg -> addChild(m_menuTodayEarn);
    m_menuTodayEarn -> setTouchPriority(-200);
    CCLabelTTF* pLabelToday1 = CCLabelTTF::create("今日盈利榜", "Arial", 26);
    pLabelToday1 -> setPosition(ccp(pItemTodayEarn->getContentSize().width/2, pItemTodayEarn->getContentSize().height/2));
    pItemTodayEarn -> addChild(pLabelToday1);
    m_spTodayEarn = CCSprite::createWithSpriteFrameName("h3rank_tab_d.png");
    m_spTodayEarn -> setPosition(ccp(m_spBg->getContentSize().width/5-30, 40));
    m_spBg -> addChild(m_spTodayEarn);
    m_spTodayEarn -> setScale(0.68f);
    CCLabelTTF* pLabelToday2 = CCLabelTTF::create("今日盈利榜", "Arial", 26);
    pLabelToday2 -> setPosition(ccp(m_spTodayEarn->getContentSize().width/2, m_spTodayEarn->getContentSize().height/2));
    m_spTodayEarn -> addChild(pLabelToday2);
    pLabelToday2 -> setColor(ccc3(245, 221, 150));
    
    CCSprite* spWeek = CCSprite::createWithSpriteFrameName("h3rank_tab_n.png");
    CCMenuItemSprite* pItemWeekEarn = CCMenuItemSprite::create(spWeek, spWeek, this, menu_selector(H3RankLayer::clickMenu));
    pItemWeekEarn -> setTag(103);
    pItemWeekEarn -> setPosition(ccp(m_spBg->getContentSize().width/5*2-10, 40));
    pItemWeekEarn -> setScale(0.68f);
    m_menuWeekEarn = CCMenu::create(pItemWeekEarn, NULL);
    m_menuWeekEarn -> setPosition(CCPointZero);
    m_menuWeekEarn -> setAnchorPoint(CCPointZero);
    m_spBg -> addChild(m_menuWeekEarn);
    m_menuWeekEarn -> setTouchPriority(-200);
    CCLabelTTF* pLabelWeek1 = CCLabelTTF::create("本周盈利榜", "Arial", 26);
    pLabelWeek1 -> setPosition(ccp(pItemWeekEarn->getContentSize().width/2, pItemWeekEarn->getContentSize().height/2));
    pItemWeekEarn -> addChild(pLabelWeek1);
    m_spWeekEarn = CCSprite::createWithSpriteFrameName("h3rank_tab_d.png");
    m_spWeekEarn -> setPosition(ccp(m_spBg->getContentSize().width/5*2-10, 40));
    m_spBg -> addChild(m_spWeekEarn);
    m_spWeekEarn -> setScale(0.68f);
    CCLabelTTF* pLabelWeek2 = CCLabelTTF::create("本周盈利榜", "Arial", 26);
    pLabelWeek2 -> setPosition(ccp(m_spWeekEarn->getContentSize().width/2, m_spWeekEarn->getContentSize().height/2));
    m_spWeekEarn -> addChild(pLabelWeek2);
    pLabelWeek2 -> setColor(ccc3(245, 221, 150));
    
    CCSprite* spEst = CCSprite::createWithSpriteFrameName("h3rank_tab_n.png");
    CCMenuItemSprite* pItemEstEarn = CCMenuItemSprite::create(spEst, spEst, this, menu_selector(H3RankLayer::clickMenu));
    pItemEstEarn -> setTag(105);
    pItemEstEarn -> setPosition(ccp(m_spBg->getContentSize().width/5*3+10, 40));
    pItemEstEarn -> setScale(0.68f);
    m_menuEstEarn = CCMenu::create(pItemEstEarn, NULL);
    m_menuEstEarn -> setPosition(CCPointZero);
    m_menuEstEarn -> setAnchorPoint(CCPointZero);
    m_spBg -> addChild(m_menuEstEarn);
    m_menuEstEarn -> setTouchPriority(-200);
    CCLabelTTF* pLabelEst1 = CCLabelTTF::create("至尊盈利榜", "Arial", 26);
    pLabelEst1 -> setPosition(ccp(pItemEstEarn->getContentSize().width/2, pItemEstEarn->getContentSize().height/2));
    pItemEstEarn -> addChild(pLabelEst1);
    m_spEstEarn = CCSprite::createWithSpriteFrameName("h3rank_tab_d.png");
    m_spEstEarn -> setPosition(ccp(m_spBg->getContentSize().width/5*3+10, 40));
    m_spBg -> addChild(m_spEstEarn);
    m_spEstEarn -> setScale(0.68f);
    CCLabelTTF* pLabelEst2 = CCLabelTTF::create("至尊盈利榜", "Arial", 26);
    pLabelEst2 -> setPosition(ccp(m_spEstEarn->getContentSize().width/2, m_spEstEarn->getContentSize().height/2));
    m_spEstEarn -> addChild(pLabelEst2);
    pLabelEst2 -> setColor(ccc3(245, 221, 150));
    
    CCSprite* spFood = CCSprite::createWithSpriteFrameName("h3rank_tab_n.png");
    CCMenuItemSprite* pItemFoodEarn = CCMenuItemSprite::create(spFood, spFood, this, menu_selector(H3RankLayer::clickMenu));
    pItemFoodEarn -> setTag(107);
    pItemFoodEarn -> setPosition(ccp(m_spBg->getContentSize().width/5*4+30, 40));
    pItemFoodEarn -> setScale(0.68f);
    m_menuFoodEarn = CCMenu::create(pItemFoodEarn, NULL);
    m_menuFoodEarn -> setPosition(CCPointZero);
    m_menuFoodEarn -> setAnchorPoint(CCPointZero);
    m_spBg -> addChild(m_menuFoodEarn);
    m_menuFoodEarn -> setTouchPriority(-200);
    CCLabelTTF* pLabelFood1 = CCLabelTTF::create("今日富豪榜", "Arial", 26);
    pLabelFood1 -> setPosition(ccp(pItemFoodEarn->getContentSize().width/2, pItemFoodEarn->getContentSize().height/2));
    pItemFoodEarn -> addChild(pLabelFood1);
    m_spFoodEarn = CCSprite::createWithSpriteFrameName("h3rank_tab_d.png");
    m_spFoodEarn -> setPosition(ccp(m_spBg->getContentSize().width/5*4+30, 40));
    m_spBg -> addChild(m_spFoodEarn);
    m_spFoodEarn -> setScale(0.68f);
    CCLabelTTF* pLabelFood2 = CCLabelTTF::create("今日富豪榜", "Arial", 26);
    pLabelFood2 -> setPosition(ccp(m_spFoodEarn->getContentSize().width/2, m_spFoodEarn->getContentSize().height/2));
    m_spFoodEarn -> addChild(pLabelFood2);
    pLabelFood2 -> setColor(ccc3(245, 221, 150));
    m_spFoodEarn -> setVisible(false);
    m_spWeekEarn -> setVisible(false);
    m_spEstEarn -> setVisible(false);
    m_menuTodayEarn -> setVisible(false);
    
    CCSprite* pCloseNormal = CCSprite::createWithSpriteFrameName("h3card_common_closenormal.png");
    CCSprite* pCloseSelected = CCSprite::createWithSpriteFrameName("h3card_common_closeselected.png");
    CCMenuItemSprite* pCloseItem = CCMenuItemSprite::create(pCloseNormal,pCloseSelected, this, menu_selector(H3RankLayer::clickClose));
    pCloseItem->setPosition(ccp(m_spBg->getContentSize().width-10, m_spBg->getContentSize().height-10));
    pCloseItem -> setScale(0.75f);
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    pMenu->setAnchorPoint(CCPointZero);
    m_spBg->addChild(pMenu);
    pMenu->setTouchPriority(-200);
    
    loadTodayEarn();
    loadWeekEarn();
    loadEstEarn();
    loadFoodEarn();
}

void H3RankLayer::loadTodayEarn()
{
    m_nodeTodayEarn = CCNode::create();
    m_pRankBg -> addChild(m_nodeTodayEarn);
    CCLabelTTF* pTodayTitle = CCLabelTTF::create("今日盈利余额", "Arial", 16);
    pTodayTitle -> setPosition(ccp(m_pRankBg->getContentSize().width/2+30, m_pRankBg->getContentSize().height-14));
    m_nodeTodayEarn -> addChild(pTodayTitle);
    pTodayTitle -> setColor(ccc3(155, 95, 42));
    CCSprite* spInfo = CCSprite::createWithSpriteFrameName("h3rank_declare.png");
    CCMenuItemSprite* pInfoItem = CCMenuItemSprite::create(spInfo, spInfo, this, menu_selector(H3RankLayer::clickMenu));
    pInfoItem -> setTag(102);
    pInfoItem -> setPosition(ccp(pInfoItem->getContentSize().width/2-8, m_pRankBg->getContentSize().height-14));
    pInfoItem -> setScale(0.6f);
    CCMenu* pMenu = CCMenu::create(pInfoItem, NULL);
    pMenu -> setAnchorPoint(CCPointZero);
    pMenu -> setPosition(CCPointZero);
    m_nodeTodayEarn -> addChild(pMenu);
    pMenu -> setTouchPriority(-200);
    
    m_scrollTodayEarn = CCScrollView::create();
    m_layerTodayEarn = CCLayer::create();
    m_layerTodayEarn -> setContentSize(CCSizeMake(400, 285));
    m_layerTodayEarn -> setAnchorPoint(CCPointZero);
    m_layerTodayEarn -> setPosition(ccp(0,0));
    m_scrollTodayEarn->setPosition(ccp(4,2));
    m_scrollTodayEarn->setViewSize(CCSizeMake(400, 285));
    m_scrollTodayEarn->setContentSize(CCSizeMake(400, 285));
    m_scrollTodayEarn->setContainer(m_layerTodayEarn);
    m_scrollTodayEarn->setDirection(kCCScrollViewDirectionVertical);
    m_scrollTodayEarn->setBounceable(true);
    m_nodeTodayEarn->addChild(m_scrollTodayEarn);
    m_scrollTodayEarn -> setTouchEnabled(true);
    m_scrollTodayEarn -> setTouchPriority(-130);
    
    CCSprite* pDelayRank = CCSprite::create("h2all/loading1.png");
    pDelayRank -> setPosition(ccp(m_pRankBg->getContentSize().width/2, m_pRankBg->getContentSize().height/2-15));
    m_nodeTodayEarn -> addChild(pDelayRank, 1, 99);
    pDelayRank -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
    
    requestTodayEarn();
}

void H3RankLayer::requestTodayEarn()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://lelewap.66y.com/RankDayProfit.aspx");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(H3RankLayer::onHttpTodayEarnComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void H3RankLayer::onHttpTodayEarnComplete(CCHttpClient* pSender, CCHttpResponse* response)
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
        if (0!=nodename.compare("data"))
        {
            return;
        }
        TiXmlElement* pFirstElement = rootElement->FirstChildElement();
        if(pFirstElement != NULL)
        {
            const char * pValue = pFirstElement->Value();
            if(!std::strcmp(pValue, "lastday"))//相同
            {
                TodayEarn todayEarn = {0};
                todayEarn.nFaceID = atoi(pFirstElement->Attribute("face"));
                todayEarn.nRankID = 0;
                todayEarn.llScore = atoll(pFirstElement->Attribute("score"));
                todayEarn.sName = pFirstElement->Attribute("name");
                m_vctTodayEarn.push_back(todayEarn);
            }
            TiXmlElement* pCurrentElement = pFirstElement->NextSiblingElement();
            while (pCurrentElement != NULL) {
                const char * pValue = pCurrentElement->Value();
                if(!std::strcmp(pValue, "todayearn"))//相同
                {
                    TodayEarn todayEarn = {0};
                    todayEarn.nFaceID = atoi(pCurrentElement->Attribute("face"));
                    todayEarn.nRankID = atoi(pCurrentElement->Attribute("rank"));
                    todayEarn.llScore = atoll(pCurrentElement->Attribute("score"));
                    todayEarn.sName = pCurrentElement->Attribute("name");
                    m_vctTodayEarn.push_back(todayEarn);
                }
                pCurrentElement = pCurrentElement->NextSiblingElement();
            }
        }
    }
    delete document;
    
    showTodayEarn();
}

void H3RankLayer::showTodayEarn()
{
    CCSprite* pDelayRank = (CCSprite*)m_nodeTodayEarn->getChildByTag(99);
    if(pDelayRank != NULL)
    {
        pDelayRank->stopAllActions();
        pDelayRank->removeFromParent();
    }
    int nCount = (int)m_vctTodayEarn.size()-1;
    printf("nCount is %d\n", nCount);
    if(nCount < 0)
    {
        return ;
    }
    m_layerTodayEarn -> setContentSize(CCSizeMake(400, 56*nCount));
    m_layerTodayEarn -> setPosition(ccp(0,285-56*nCount));
    m_scrollTodayEarn->setContentSize(CCSizeMake(400, 56*nCount));
    char temp[16];
    for(int i = 0; i < nCount; i++)
    {
        CCSprite* spdi = CCSprite::createWithSpriteFrameName("h3rank_line_bottom.png");
        spdi -> setPosition(ccp(200, 56*nCount-56*i-56));
        m_layerTodayEarn -> addChild(spdi);
        spdi -> setScale(0.68f);
        
        if(i <= 2)
        {
            CCSprite* spRankID = CCSprite::createWithSpriteFrameName(H3RankName[i]);
            spRankID -> setPosition(ccp(30, 56*nCount-56*i-28));
            m_layerTodayEarn -> addChild(spRankID);
            spRankID -> setScale(0.6f);
            if(m_vctTodayEarn[i+1].nFaceID >= 1 && m_vctTodayEarn[i+1].nFaceID <= 10)
            {
                CCSprite* pTx = CCSprite::create(txName[m_vctTodayEarn[i+1].nFaceID-1]);
                pTx -> setPosition(ccp(90, 56*nCount-56*i-28));
                pTx -> setScale(0.35f);
                m_layerTodayEarn -> addChild(pTx);
            }
            CCLabelTTF* pName = CCLabelTTF::create(m_vctTodayEarn[i+1].sName.c_str(), "Arial", 18);
            pName -> setPosition(ccp(120, 56*nCount-56*i-28));
            pName -> setColor(ccc3(204, 209, 121));
            pName -> setAnchorPoint(ccp(0, 0.5f));
            m_layerTodayEarn -> addChild(pName);
        }
        else
        {
            sprintf(temp, "%d", i+1);
            CCLabelBMFont* spRankID = CCLabelBMFont::create(temp, "h3card/h3card_golden.fnt");
            spRankID -> setPosition(ccp(30, 56*nCount-56*i-28));
            m_layerTodayEarn -> addChild(spRankID);
            spRankID -> setScale(0.5f);
            CCLabelTTF* pName = CCLabelTTF::create(m_vctTodayEarn[i+1].sName.c_str(), "Arial", 18);
            pName -> setPosition(ccp(80, 56*nCount-56*i-28));
            pName -> setColor(ccc3(204, 209, 121));
            pName -> setAnchorPoint(ccp(0, 0.5f));
            m_layerTodayEarn -> addChild(pName);
        }
        
        CCSprite* pChouma = CCSprite::createWithSpriteFrameName("h3card_common_chouma.png");
        pChouma -> setPosition(ccp(280, 56*nCount-56*i-28));
        m_layerTodayEarn -> addChild(pChouma);
        pChouma -> setScale(0.8f);
        CCLabelBMFont* pLabelScore = CCLabelBMFont::create(getStringScore(m_vctTodayEarn[i+1].llScore).c_str(), "h3card/h3card_golden.fnt");
        pLabelScore -> setPosition(ccp(300, 56*nCount-56*i-28));
        m_layerTodayEarn -> addChild(pLabelScore);
        pLabelScore -> setAnchorPoint(ccp(0, 0.5f));
        pLabelScore -> setScale(0.5f);
    }
    
    CCSprite* pDi = CCSprite::create("h3card/unpack_rank_di.png");
    pDi -> setPosition(ccp(m_pRankBg->getContentSize().width-pDi->getContentSize().width/2-3, m_pRankBg->getContentSize().height/2-11));
    m_nodeTodayEarn -> addChild(pDi);
    CCSprite* pRed = CCSprite::createWithSpriteFrameName("h3rank_geren_show_bg.png");
    pRed -> setPosition(ccp(pDi->getContentSize().width/2, pDi->getContentSize().height/2-10));
    pDi -> addChild(pRed);
    pRed -> setScale(0.68f);
    CCSprite* pChampin = CCSprite::createWithSpriteFrameName("h3rank_zuoriguanjun.png");
    pChampin -> setPosition(ccp(pRed->getContentSize().width/2, pRed->getContentSize().height-pChampin->getContentSize().height/2-15));
    pRed -> addChild(pChampin);
    CCSprite* pTouxiangBg = CCSprite::create(txName[m_vctTodayEarn[0].nFaceID-1]);
    pTouxiangBg -> setPosition(ccp(pRed->getContentSize().width/2, pRed->getContentSize().height/3*2));
    pRed -> addChild(pTouxiangBg);
    pTouxiangBg -> setScale(0.68f);
    CCLabelTTF* pLabel = CCLabelTTF::create(m_vctTodayEarn[0].sName.c_str(), "Arial", 26);
    pLabel -> setPosition(ccp(pRed->getContentSize().width/2, pRed->getContentSize().height/2-10));
    pRed -> addChild(pLabel);
    CCLabelTTF* pLabel2 = CCLabelTTF::create("昨日盈利", "Arial", 22);
    pLabel2 -> setPosition(ccp(pRed->getContentSize().width/2, 160));
    pRed -> addChild(pLabel2);
    CCLabelBMFont* pScore = CCLabelBMFont::create(getStringScore(m_vctTodayEarn[0].llScore).c_str(), "h3card/h3card_golden.fnt");
    pScore -> setPosition(ccp(pRed->getContentSize().width/2, 130));
    pScore -> setScale(0.68f);
    pRed -> addChild(pScore);
}

void H3RankLayer::loadWeekEarn()
{
    m_nodeWeekEarn = CCNode::create();
    m_pRankBg -> addChild(m_nodeWeekEarn);
    m_nodeWeekEarn -> setVisible(false);
    
    CCLabelTTF* pTodayTitle = CCLabelTTF::create("本周盈利余额", "Arial", 16);
    pTodayTitle -> setPosition(ccp(m_pRankBg->getContentSize().width/2+30, m_pRankBg->getContentSize().height-14));
    m_nodeWeekEarn -> addChild(pTodayTitle);
    pTodayTitle -> setColor(ccc3(155, 95, 42));
    CCSprite* spInfo = CCSprite::createWithSpriteFrameName("h3rank_declare.png");
    CCMenuItemSprite* pInfoItem = CCMenuItemSprite::create(spInfo, spInfo, this, menu_selector(H3RankLayer::clickMenu));
    pInfoItem -> setTag(104);
    pInfoItem -> setPosition(ccp(pInfoItem->getContentSize().width/2-8, m_pRankBg->getContentSize().height-14));
    pInfoItem -> setScale(0.6f);
    CCMenu* pMenu = CCMenu::create(pInfoItem, NULL);
    pMenu -> setAnchorPoint(CCPointZero);
    pMenu -> setPosition(CCPointZero);
    m_nodeWeekEarn -> addChild(pMenu);
    pMenu -> setTouchPriority(-200);
    
    m_scrollWeekEarn = CCScrollView::create();
    m_layerWeekEarn = CCLayer::create();
    m_layerWeekEarn -> setContentSize(CCSizeMake(400, 285));
    m_layerWeekEarn -> setAnchorPoint(CCPointZero);
    m_layerWeekEarn -> setPosition(ccp(0,0));
    m_scrollWeekEarn->setPosition(ccp(4,2));
    m_scrollWeekEarn->setViewSize(CCSizeMake(400, 285));
    m_scrollWeekEarn->setContentSize(CCSizeMake(400, 285));
    m_scrollWeekEarn->setContainer(m_layerWeekEarn);
    m_scrollWeekEarn->setDirection(kCCScrollViewDirectionVertical);
    m_scrollWeekEarn->setBounceable(true);
    m_nodeWeekEarn->addChild(m_scrollWeekEarn);
    m_scrollWeekEarn -> setTouchEnabled(false);
    m_scrollWeekEarn -> setTouchPriority(-130);
    
    CCSprite* pDelayRank = CCSprite::create("h2all/loading1.png");
    pDelayRank -> setPosition(ccp(m_pRankBg->getContentSize().width/2, m_pRankBg->getContentSize().height/2-15));
    m_nodeWeekEarn -> addChild(pDelayRank, 1, 99);
    pDelayRank -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
    
    requestWeekEarn();
}

void H3RankLayer::requestWeekEarn()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://lelewap.66y.com/RankWeekProfit.aspx");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(H3RankLayer::onHttpWeekEarnComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void H3RankLayer::onHttpWeekEarnComplete(CCHttpClient* pSender, CCHttpResponse* response)
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
        if (0!=nodename.compare("data"))
        {
            return;
        }
        TiXmlElement* pFirstElement = rootElement->FirstChildElement();
        if(pFirstElement != NULL)
        {
            const char * pValue = pFirstElement->Value();
            if(!std::strcmp(pValue, "lastday"))//相同
            {
                TodayEarn todayEarn = {0};
                todayEarn.nFaceID = atoi(pFirstElement->Attribute("face"));
                todayEarn.nRankID = 0;
                todayEarn.llScore = atoll(pFirstElement->Attribute("score"));
                todayEarn.sName = pFirstElement->Attribute("name");
                m_vctWeekEarn.push_back(todayEarn);
            }
            TiXmlElement* pCurrentElement = pFirstElement->NextSiblingElement();
            while (pCurrentElement != NULL) {
                const char * pValue = pCurrentElement->Value();
                if(!std::strcmp(pValue, "todayearn"))//相同
                {
                    TodayEarn todayEarn = {0};
                    todayEarn.nFaceID = atoi(pCurrentElement->Attribute("face"));
                    todayEarn.nRankID = atoi(pCurrentElement->Attribute("rank"));
                    todayEarn.llScore = atoll(pCurrentElement->Attribute("score"));
                    todayEarn.sName = pCurrentElement->Attribute("name");
                    m_vctWeekEarn.push_back(todayEarn);
                }
                pCurrentElement = pCurrentElement->NextSiblingElement();
            }
        }
    }
    delete document;
    showWeekEarn();
}

void H3RankLayer::showWeekEarn()
{
    CCSprite* pDelayRank = (CCSprite*)m_nodeWeekEarn->getChildByTag(99);
    if(pDelayRank != NULL)
    {
        pDelayRank->stopAllActions();
        pDelayRank->removeFromParent();
    }
    int nCount = (int)m_vctWeekEarn.size()-1;
    if(nCount < 0)
    {
        return ;
    }
    m_layerWeekEarn -> setContentSize(CCSizeMake(400, 56*nCount));
    m_layerWeekEarn -> setPosition(ccp(0,285-56*nCount));
    m_scrollWeekEarn->setContentSize(CCSizeMake(400, 56*nCount));
    char temp[16];
    for(int i = 0; i < nCount; i++)
    {
        CCSprite* spdi = CCSprite::createWithSpriteFrameName("h3rank_line_bottom.png");
        spdi -> setPosition(ccp(200, 56*nCount-56*i-56));
        m_layerWeekEarn -> addChild(spdi);
        spdi -> setScale(0.68f);
        if(i <= 2)
        {
            CCSprite* spRankID = CCSprite::createWithSpriteFrameName(H3RankName[i]);
            spRankID -> setPosition(ccp(30, 56*nCount-56*i-28));
            m_layerWeekEarn -> addChild(spRankID);
            spRankID -> setScale(0.6f);
            if(m_vctWeekEarn[i+1].nFaceID >= 1 && m_vctWeekEarn[i+1].nFaceID <= 10)
            {
                CCSprite* pTx = CCSprite::create(txName[m_vctWeekEarn[i+1].nFaceID-1]);
                pTx -> setPosition(ccp(90, 56*nCount-56*i-28));
                pTx -> setScale(0.35f);
                m_layerWeekEarn -> addChild(pTx);
            }
            CCLabelTTF* pName = CCLabelTTF::create(m_vctWeekEarn[i+1].sName.c_str(), "Arial", 18);
            pName -> setPosition(ccp(120, 56*nCount-56*i-28));
            pName -> setColor(ccc3(204, 209, 121));
            pName -> setAnchorPoint(ccp(0, 0.5f));
            m_layerWeekEarn -> addChild(pName);
        }
        else
        {
            sprintf(temp, "%d", i+1);
            CCLabelBMFont* spRankID = CCLabelBMFont::create(temp, "h3card/h3card_golden.fnt");
            spRankID -> setPosition(ccp(30, 56*nCount-56*i-28));
            m_layerWeekEarn -> addChild(spRankID);
            spRankID -> setScale(0.5f);
            CCLabelTTF* pName = CCLabelTTF::create(m_vctWeekEarn[i+1].sName.c_str(), "Arial", 18);
            pName -> setPosition(ccp(80, 56*nCount-56*i-28));
            pName -> setColor(ccc3(204, 209, 121));
            pName -> setAnchorPoint(ccp(0, 0.5f));
            m_layerWeekEarn -> addChild(pName);
        }
        
        CCSprite* pChouma = CCSprite::createWithSpriteFrameName("h3card_common_chouma.png");
        pChouma -> setPosition(ccp(280, 56*nCount-56*i-28));
        m_layerWeekEarn -> addChild(pChouma);
        pChouma -> setScale(0.8f);
        CCLabelBMFont* pLabelScore = CCLabelBMFont::create(getStringScore(m_vctWeekEarn[i+1].llScore).c_str(), "h3card/h3card_golden.fnt");
        pLabelScore -> setPosition(ccp(300, 56*nCount-56*i-28));
        m_layerWeekEarn -> addChild(pLabelScore);
        pLabelScore -> setAnchorPoint(ccp(0, 0.5f));
        pLabelScore -> setScale(0.5f);
    }
    
    CCSprite* pDi = CCSprite::create("h3card/unpack_rank_di.png");
    pDi -> setPosition(ccp(m_pRankBg->getContentSize().width-pDi->getContentSize().width/2-3, m_pRankBg->getContentSize().height/2-11));
    m_nodeWeekEarn -> addChild(pDi);
    CCSprite* pRed = CCSprite::createWithSpriteFrameName("h3rank_geren_show_bg.png");
    pRed -> setPosition(ccp(pDi->getContentSize().width/2, pDi->getContentSize().height/2-10));
    pDi -> addChild(pRed);
    pRed -> setScale(0.68f);
    CCSprite* pChampin = CCSprite::createWithSpriteFrameName("h3rank_shangzhouguanjin.png");
    pChampin -> setPosition(ccp(pRed->getContentSize().width/2, pRed->getContentSize().height-pChampin->getContentSize().height/2-15));
    pRed -> addChild(pChampin);
    CCSprite* pTouxiangBg = CCSprite::create(txName[m_vctWeekEarn[0].nFaceID-1]);
    pTouxiangBg -> setPosition(ccp(pRed->getContentSize().width/2, pRed->getContentSize().height/3*2));
    pRed -> addChild(pTouxiangBg);
    pTouxiangBg -> setScale(0.68f);
    CCLabelTTF* pLabel = CCLabelTTF::create(m_vctWeekEarn[0].sName.c_str(), "Arial", 26);
    pLabel -> setPosition(ccp(pRed->getContentSize().width/2, pRed->getContentSize().height/2-10));
    pRed -> addChild(pLabel);
    CCLabelTTF* pLabel2 = CCLabelTTF::create("上周盈利", "Arial", 22);
    pLabel2 -> setPosition(ccp(pRed->getContentSize().width/2, 160));
    pRed -> addChild(pLabel2);
    CCLabelBMFont* pScore = CCLabelBMFont::create(getStringScore(m_vctWeekEarn[0].llScore).c_str(), "h3card/h3card_golden.fnt");
    pScore -> setPosition(ccp(pRed->getContentSize().width/2, 130));
    pScore -> setScale(0.68f);
    pRed -> addChild(pScore);
}

void H3RankLayer::loadEstEarn()
{
    m_nodeEstEarn = CCNode::create();
    m_pRankBg -> addChild(m_nodeEstEarn);
    m_nodeEstEarn -> setVisible(false);
    
    CCLabelTTF* pTodayTitle = CCLabelTTF::create("每日盈利余额", "Arial", 16);
    pTodayTitle -> setPosition(ccp(m_pRankBg->getContentSize().width/2, m_pRankBg->getContentSize().height-14));
    m_nodeEstEarn -> addChild(pTodayTitle);
    pTodayTitle -> setColor(ccc3(155, 95, 42));
    CCLabelTTF* pTodayTime = CCLabelTTF::create("上榜时间", "Arial", 16);
    pTodayTime -> setPosition(ccp(m_pRankBg->getContentSize().width/6*5, m_pRankBg->getContentSize().height-14));
    m_nodeEstEarn -> addChild(pTodayTime);
    pTodayTime -> setColor(ccc3(155, 95, 42));
    CCSprite* spInfo = CCSprite::createWithSpriteFrameName("h3rank_declare.png");
    CCMenuItemSprite* pInfoItem = CCMenuItemSprite::create(spInfo, spInfo, this, menu_selector(H3RankLayer::clickMenu));
    pInfoItem -> setTag(106);
    pInfoItem -> setPosition(ccp(pInfoItem->getContentSize().width/2-8, m_pRankBg->getContentSize().height-14));
    pInfoItem -> setScale(0.6f);
    CCMenu* pMenu = CCMenu::create(pInfoItem, NULL);
    pMenu -> setAnchorPoint(CCPointZero);
    pMenu -> setPosition(CCPointZero);
    m_nodeEstEarn -> addChild(pMenu);
    pMenu -> setTouchPriority(-200);
    
    m_scrollEstEarn = CCScrollView::create();
    m_layerEstEarn = CCLayer::create();
    m_layerEstEarn -> setContentSize(CCSizeMake(574, 285));
    m_layerEstEarn -> setAnchorPoint(CCPointZero);
    m_layerEstEarn -> setPosition(ccp(0,0));
    m_scrollEstEarn->setPosition(ccp(4,2));
    m_scrollEstEarn->setViewSize(CCSizeMake(574, 285));
    m_scrollEstEarn->setContentSize(CCSizeMake(574, 285));
    m_scrollEstEarn->setContainer(m_layerEstEarn);
    m_scrollEstEarn->setDirection(kCCScrollViewDirectionVertical);
    m_scrollEstEarn->setBounceable(true);
    m_nodeEstEarn->addChild(m_scrollEstEarn);
    m_scrollEstEarn -> setTouchEnabled(false);
    m_scrollEstEarn -> setTouchPriority(-130);
    
    CCSprite* pDelayRank = CCSprite::create("h2all/loading1.png");
    pDelayRank -> setPosition(ccp(m_pRankBg->getContentSize().width/2, m_pRankBg->getContentSize().height/2-15));
    m_nodeEstEarn -> addChild(pDelayRank, 1, 99);
    pDelayRank -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
    
    requestEstEarn();
}

void H3RankLayer::requestEstEarn()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://lelewap.66y.com/RankEveryDay.aspx");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(H3RankLayer::onHttpEstEarnComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void H3RankLayer::onHttpEstEarnComplete(CCHttpClient* pSender, CCHttpResponse* response)
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
        if (0!=nodename.compare("data"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL) {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "todayearn"))//相同
            {
                EstEarn estEarn = {0};
                estEarn.nFaceID = atoi(pCurrentElement->Attribute("face"));
                estEarn.nRankID = atoi(pCurrentElement->Attribute("rank"));
                estEarn.llScore = atoll(pCurrentElement->Attribute("score"));
                estEarn.sName = pCurrentElement->Attribute("name");
                estEarn.sTime = pCurrentElement->Attribute("time");
                m_vctEstEarn.push_back(estEarn);
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
    
    showEstEarn();
}

void H3RankLayer::showEstEarn()
{
    CCSprite* pDelayRank = (CCSprite*)m_nodeEstEarn->getChildByTag(99);
    if(pDelayRank != NULL)
    {
        pDelayRank->stopAllActions();
        pDelayRank->removeFromParent();
    }
    int nCount = (int)m_vctEstEarn.size();
    if(nCount < 0)
    {
        return ;
    }
    m_layerEstEarn -> setContentSize(CCSizeMake(574, 56*nCount));
    m_layerEstEarn -> setPosition(ccp(0,285-56*nCount));
    m_scrollEstEarn->setContentSize(CCSizeMake(574, 56*nCount));
    for(int i = 0; i < nCount; i++)
    {
        CCSprite* spdi = CCSprite::createWithSpriteFrameName("h3rank_line_bottom.png");
        spdi -> setPosition(ccp(574/2, 56*nCount-56*i-56));
        m_layerEstEarn -> addChild(spdi);
        //spdi -> setScale(0.68f);
        if(i <= 2)
        {
            if(m_vctEstEarn[i].nFaceID >= 1 && m_vctEstEarn[i].nFaceID <= 10)
            {
                CCSprite* pTx = CCSprite::create(txName[m_vctEstEarn[i].nFaceID-1]);
                pTx -> setPosition(ccp(40, 56*nCount-56*i-28));
                pTx -> setScale(0.35f);
                m_layerEstEarn -> addChild(pTx);
            }
            CCLabelTTF* pName = CCLabelTTF::create(m_vctEstEarn[i].sName.c_str(), "Arial", 18);
            pName -> setPosition(ccp(70, 56*nCount-56*i-28));
            pName -> setColor(ccc3(204, 209, 121));
            pName -> setAnchorPoint(ccp(0, 0.5f));
            m_layerEstEarn -> addChild(pName);
        }
        else
        {
            CCLabelTTF* pName = CCLabelTTF::create(m_vctEstEarn[i].sName.c_str(), "Arial", 18);
            pName -> setPosition(ccp(30, 56*nCount-56*i-28));
            pName -> setColor(ccc3(204, 209, 121));
            pName -> setAnchorPoint(ccp(0, 0.5f));
            m_layerEstEarn -> addChild(pName);
        }
        
        CCSprite* pChouma = CCSprite::createWithSpriteFrameName("h3card_common_chouma.png");
        pChouma -> setPosition(ccp(250, 56*nCount-56*i-28));
        m_layerEstEarn -> addChild(pChouma);
        pChouma -> setScale(0.8f);
        CCLabelBMFont* pLabelScore = CCLabelBMFont::create(getStringScore(m_vctEstEarn[i].llScore).c_str(), "h3card/h3card_golden.fnt");
        pLabelScore -> setPosition(ccp(270, 56*nCount-56*i-28));
        m_layerEstEarn -> addChild(pLabelScore);
        pLabelScore -> setAnchorPoint(ccp(0, 0.5f));
        pLabelScore -> setScale(0.5f);
        CCLabelTTF* pTime = CCLabelTTF::create(m_vctEstEarn[i].sTime.c_str(), "Arial", 18);
        pTime -> setPosition(ccp(480, 56*nCount-56*i-28));
        pTime -> setColor(ccc3(204, 209, 121));
        m_layerEstEarn -> addChild(pTime);
    }
}

void H3RankLayer::loadFoodEarn()
{
    m_nodeFoodEarn = CCNode::create();
    m_pRankBg -> addChild(m_nodeFoodEarn);
    m_nodeFoodEarn -> setVisible(false);
    
    CCLabelTTF* pTodayTitle = CCLabelTTF::create("今日累计下注金额", "Arial", 16);
    pTodayTitle -> setPosition(ccp(m_pRankBg->getContentSize().width/2+30, m_pRankBg->getContentSize().height-14));
    m_nodeFoodEarn -> addChild(pTodayTitle);
    pTodayTitle -> setColor(ccc3(155, 95, 42));
    CCSprite* spInfo = CCSprite::createWithSpriteFrameName("h3rank_declare.png");
    CCMenuItemSprite* pInfoItem = CCMenuItemSprite::create(spInfo, spInfo, this, menu_selector(H3RankLayer::clickMenu));
    pInfoItem -> setTag(108);
    pInfoItem -> setPosition(ccp(pInfoItem->getContentSize().width/2-8, m_pRankBg->getContentSize().height-14));
    pInfoItem -> setScale(0.6f);
    CCMenu* pMenu = CCMenu::create(pInfoItem, NULL);
    pMenu -> setAnchorPoint(CCPointZero);
    pMenu -> setPosition(CCPointZero);
    m_nodeFoodEarn -> addChild(pMenu);
    pMenu -> setTouchPriority(-200);
    
    m_scrollFoodEarn = CCScrollView::create();
    m_layerFoodEarn = CCLayer::create();
    m_layerFoodEarn -> setContentSize(CCSizeMake(400, 285));
    m_layerFoodEarn -> setAnchorPoint(CCPointZero);
    m_layerFoodEarn -> setPosition(ccp(0,0));
    m_scrollFoodEarn->setPosition(ccp(4,2));
    m_scrollFoodEarn->setViewSize(CCSizeMake(400, 285));
    m_scrollFoodEarn->setContentSize(CCSizeMake(400, 285));
    m_scrollFoodEarn->setContainer(m_layerFoodEarn);
    m_scrollFoodEarn->setDirection(kCCScrollViewDirectionVertical);
    m_scrollFoodEarn->setBounceable(true);
    m_nodeFoodEarn->addChild(m_scrollFoodEarn);
    m_scrollFoodEarn -> setTouchEnabled(true);
    m_scrollFoodEarn -> setTouchPriority(-130);
    
    CCSprite* pDelayRank = CCSprite::create("h2all/loading1.png");
    pDelayRank -> setPosition(ccp(m_pRankBg->getContentSize().width/2, m_pRankBg->getContentSize().height/2-15));
    m_nodeFoodEarn -> addChild(pDelayRank, 1, 99);
    pDelayRank -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
    
    requestFoodEarn();
}

void H3RankLayer::requestFoodEarn()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://lelewap.66y.com/RankDayRich.aspx");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(H3RankLayer::onHttpFoodEarnComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void H3RankLayer::onHttpFoodEarnComplete(CCHttpClient* pSender, CCHttpResponse* response)
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
        if (0!=nodename.compare("data"))
        {
            return;
        }
        TiXmlElement* pFirstElement = rootElement->FirstChildElement();
        if(pFirstElement != NULL)
        {
            const char * pValue = pFirstElement->Value();
            if(!std::strcmp(pValue, "lastday"))//相同
            {
                m_foodEarn.llScore = atoll(pFirstElement->Attribute("score"));
                m_foodEarn.nFaceID = atoi(pFirstElement->Attribute("face"));
                m_foodEarn.sName = pFirstElement->Attribute("name");
                m_foodEarn.sInfo = pFirstElement->Attribute("info");
                m_foodEarn.llTotalScore = atoll(pFirstElement->Attribute("totalScore"));
            }
            TiXmlElement* pCurrentElement = pFirstElement->NextSiblingElement();
            while (pCurrentElement != NULL) {
                const char * pValue = pCurrentElement->Value();
                if(!std::strcmp(pValue, "todayearn"))//相同
                {
                    TodayEarn todayEarn = {0};
                    todayEarn.nFaceID = atoi(pCurrentElement->Attribute("face"));
                    todayEarn.nRankID = atoi(pCurrentElement->Attribute("rank"));
                    todayEarn.llScore = atoll(pCurrentElement->Attribute("score"));
                    todayEarn.sName = pCurrentElement->Attribute("name");
                    m_vctFoodEarn.push_back(todayEarn);
                }
                pCurrentElement = pCurrentElement->NextSiblingElement();
            }
        }
    }
    delete document;
    
    showFoodEarn();
}

void H3RankLayer::showFoodEarn()
{
    
    CCSprite* pDelayRank = (CCSprite*)m_nodeFoodEarn->getChildByTag(99);
    if(pDelayRank != NULL)
    {
        pDelayRank->stopAllActions();
        pDelayRank->removeFromParent();
    }
    int nCount = (int)m_vctFoodEarn.size();
    if(nCount < 0)
    {
        return ;
    }
    m_layerFoodEarn -> setContentSize(CCSizeMake(400, 56*nCount));
    m_layerFoodEarn -> setPosition(ccp(0,285-56*nCount));
    m_scrollFoodEarn->setContentSize(CCSizeMake(400, 56*nCount));
    
    
    char temp[16];
    for(int i = 0; i < nCount; i++)
    {
        CCSprite* spdi = CCSprite::createWithSpriteFrameName("h3rank_line_bottom.png");
        spdi -> setPosition(ccp(200, 56*nCount-56*i-56));
        m_layerFoodEarn -> addChild(spdi);
        spdi -> setScale(0.68f);
        
        if(i <= 2)
        {
            CCSprite* spRankID = CCSprite::createWithSpriteFrameName(H3RankName[i]);
            spRankID -> setPosition(ccp(30, 56*nCount-56*i-28));
            m_layerFoodEarn -> addChild(spRankID);
            spRankID -> setScale(0.6f);
            if(m_vctFoodEarn[i].nFaceID >= 1 && m_vctFoodEarn[i].nFaceID <= 10)
            {
                CCSprite* pTx = CCSprite::create(txName[m_vctFoodEarn[i].nFaceID-1]);
                pTx -> setPosition(ccp(90, 56*nCount-56*i-28));
                pTx -> setScale(0.35f);
                m_layerFoodEarn -> addChild(pTx);
            }
            CCLabelTTF* pName = CCLabelTTF::create(m_vctFoodEarn[i].sName.c_str(), "Arial", 18);
            pName -> setPosition(ccp(120, 56*nCount-56*i-28));
            pName -> setColor(ccc3(204, 209, 121));
            pName -> setAnchorPoint(ccp(0, 0.5f));
            m_layerFoodEarn -> addChild(pName);
        }
        else
        {
            sprintf(temp, "%d", i+1);
            CCLabelBMFont* spRankID = CCLabelBMFont::create(temp, "h3card/h3card_golden.fnt");
            spRankID -> setPosition(ccp(30, 56*nCount-56*i-28));
            m_layerFoodEarn -> addChild(spRankID);
            spRankID -> setScale(0.5f);
            CCLabelTTF* pName = CCLabelTTF::create(m_vctFoodEarn[i].sName.c_str(), "Arial", 18);
            pName -> setPosition(ccp(80, 56*nCount-56*i-28));
            pName -> setColor(ccc3(204, 209, 121));
            pName -> setAnchorPoint(ccp(0, 0.5f));
            m_layerFoodEarn -> addChild(pName);
        }
        
        CCSprite* pChouma = CCSprite::createWithSpriteFrameName("h3card_common_chouma.png");
        pChouma -> setPosition(ccp(280, 56*nCount-56*i-28));
        m_layerFoodEarn -> addChild(pChouma);
        pChouma -> setScale(0.8f);
        CCLabelBMFont* pLabelScore = CCLabelBMFont::create(getStringScore(m_vctFoodEarn[i].llScore).c_str(), "h3card/h3card_golden.fnt");
        pLabelScore -> setPosition(ccp(300, 56*nCount-56*i-28));
        m_layerFoodEarn -> addChild(pLabelScore);
        pLabelScore -> setAnchorPoint(ccp(0, 0.5f));
        pLabelScore -> setScale(0.5f);
    }
    
    CCSprite* pDi = CCSprite::create("h3card/unpack_rank_di.png");
    pDi -> setPosition(ccp(m_pRankBg->getContentSize().width-pDi->getContentSize().width/2-3, m_pRankBg->getContentSize().height/2-11));
    m_nodeFoodEarn -> addChild(pDi);
    CCSprite* pRed = CCSprite::createWithSpriteFrameName("h3rank_geren_show_bg.png");
    pRed -> setPosition(ccp(pDi->getContentSize().width/2, pDi->getContentSize().height/2-10));
    pDi -> addChild(pRed);
    pRed -> setScale(0.68f);
    CCSprite* pChampin = CCSprite::createWithSpriteFrameName("h3rank_zuorishoufu.png");
    pChampin -> setPosition(ccp(pRed->getContentSize().width/2, pRed->getContentSize().height-pChampin->getContentSize().height/2-15));
    pRed -> addChild(pChampin);
    CCSprite* pTouxiangBg = CCSprite::create(txName[m_foodEarn.nFaceID-1]);
    pTouxiangBg -> setPosition(ccp(pRed->getContentSize().width/4, pRed->getContentSize().height/3*2+18));
    pRed -> addChild(pTouxiangBg);
    pTouxiangBg -> setScale(0.6f);
    CCLabelTTF* pLabel = CCLabelTTF::create(m_foodEarn.sName.c_str(), "Arial", 20);
    pLabel -> setPosition(ccp(pRed->getContentSize().width/4*3-20, pRed->getContentSize().height/3*2+45));
    pRed -> addChild(pLabel);
    pLabel -> setColor(ccc3(240, 220, 216));
    CCLabelTTF* pLabel2 = CCLabelTTF::create("昨日下注", "Arial", 18);
    pLabel2 -> setPosition(ccp(pRed->getContentSize().width/4*3-20, pRed->getContentSize().height/3*2+18));
    pRed -> addChild(pLabel2);
    pLabel2 -> setColor(ccc3(166, 142, 139));
    CCLabelBMFont* pScore = CCLabelBMFont::create(getStringScore(m_foodEarn.llScore).c_str(), "h3card/h3card_golden.fnt");
    pScore -> setPosition(ccp(pRed->getContentSize().width/4*3-20, pRed->getContentSize().height/3*2-10));
    pScore -> setScale(0.68f);
    pRed -> addChild(pScore);
    CCLabelTTF* pTotalInfo = CCLabelTTF::create("奖励总额:", "Arial", 20);
    pTotalInfo -> setPosition(ccp(pRed->getContentSize().width/3-10, pRed->getContentSize().height/2));
    pRed -> addChild(pTotalInfo);
    pTotalInfo -> setColor(ccc3(218, 171, 45));
    CCLabelBMFont* pTotalScore = CCLabelBMFont::create(getStringScore(m_foodEarn.llScore).c_str(), "h3card/h3card_golden.fnt");
    pTotalScore -> setPosition(ccp(pRed->getContentSize().width/2, pRed->getContentSize().height/2));
    pTotalScore -> setAnchorPoint(ccp(0, 0.5f));
    pTotalScore -> setScale(0.68f);
    pRed -> addChild(pTotalScore);
    CCSize size(185,85);
    int nSize = 16;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    nSize = 13;
#endif
    CCLabelTTF* pLabelInfo = CCLabelTTF::create(m_foodEarn.sInfo.c_str(), "Arial", nSize,size,kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    pLabelInfo->setColor(ccc3(255, 255, 255));
    pLabelInfo->setPosition(ccp(30, 190));
    pLabelInfo -> setAnchorPoint(ccp(0, 1));
    pRed->addChild(pLabelInfo);
}

std::string H3RankLayer::getStringScore(SCORE llScore)
{
    char temp[16];
    if(llScore >= 100000000)
    {
        if(llScore%100000000 == 0)
        {
            sprintf(temp, "%lld亿", llScore/100000000);
        }
        else
        {
            if(llScore%10000000 == 0)
            {
                sprintf(temp, "%.1f亿", llScore/100000000.0);
            }
            else
            {
                sprintf(temp, "%.2f亿", llScore/100000000.0);
            }
        }
    }
    else if(llScore >= 10000)
    {
        if(llScore%10000 == 0)
        {
            sprintf(temp, "%lld万", llScore/10000);
        }
        else
        {
            if(llScore%1000 == 0)
            {
                sprintf(temp, "%.1f万", llScore/10000.0);
            }
            else
            {
                sprintf(temp, "%.2f万", llScore/10000.0);
            }
        }
    }
    else
    {
        sprintf(temp, "%lld", llScore);
    }
    return temp;
}

void H3RankLayer::clickMenu(CCObject* object)
{
    if(m_bClickInfoMenu)
    {
        m_bClickInfoMenu = false;
        m_spInfoBg -> setVisible(false);
        return ;
    }
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case 101://今日盈利榜
        {
            m_menuTodayEarn -> setVisible(false);
            m_spTodayEarn -> setVisible(true);
            m_menuEstEarn -> setVisible(true);
            m_spEstEarn -> setVisible(false);
            m_menuFoodEarn -> setVisible(true);
            m_spFoodEarn -> setVisible(false);
            m_menuWeekEarn -> setVisible(true);
            m_spWeekEarn -> setVisible(false);
            m_nodeTodayEarn -> setVisible(true);
            m_nodeWeekEarn -> setVisible(false);
            m_nodeEstEarn -> setVisible(false);
            m_nodeFoodEarn -> setVisible(false);
            m_scrollTodayEarn -> setTouchEnabled(true);
            m_scrollWeekEarn -> setTouchEnabled(false);
            m_scrollEstEarn -> setTouchEnabled(false);
            m_scrollFoodEarn -> setTouchEnabled(false);
            break;
        }
        case 102://榜单说明
        {
            m_bClickInfoMenu = true;
            m_spInfoBg -> setVisible(true);
            m_labelInfo -> setString("每天下注盈利累计最多的玩家，每天0:00重置");
            break;
        }
        case 103://本周盈利榜
        {
            m_menuTodayEarn -> setVisible(true);
            m_spTodayEarn -> setVisible(false);
            m_menuEstEarn -> setVisible(true);
            m_spEstEarn -> setVisible(false);
            m_menuFoodEarn -> setVisible(true);
            m_spFoodEarn -> setVisible(false);
            m_menuWeekEarn -> setVisible(false);
            m_spWeekEarn -> setVisible(true);
            m_nodeTodayEarn -> setVisible(false);
            m_nodeWeekEarn -> setVisible(true);
            m_nodeEstEarn -> setVisible(false);
            m_nodeFoodEarn -> setVisible(false);
            m_scrollTodayEarn -> setTouchEnabled(false);
            m_scrollWeekEarn -> setTouchEnabled(true);
            m_scrollEstEarn -> setTouchEnabled(false);
            m_scrollFoodEarn -> setTouchEnabled(false);
            break;
        }
        case 104://榜单说明
        {
            m_bClickInfoMenu = true;
            m_spInfoBg -> setVisible(true);
            m_labelInfo -> setString("本周下注盈利累计最多的玩家，每周一0:00重置");
            break;
        }
        case 105://至尊盈利榜
        {
            m_menuTodayEarn -> setVisible(true);
            m_spTodayEarn -> setVisible(false);
            m_menuEstEarn -> setVisible(false);
            m_spEstEarn -> setVisible(true);
            m_menuFoodEarn -> setVisible(true);
            m_spFoodEarn -> setVisible(false);
            m_menuWeekEarn -> setVisible(true);
            m_spWeekEarn -> setVisible(false);
            m_nodeTodayEarn -> setVisible(false);
            m_nodeWeekEarn -> setVisible(false);
            m_nodeEstEarn -> setVisible(true);
            m_nodeFoodEarn -> setVisible(false);
            m_scrollTodayEarn -> setTouchEnabled(false);
            m_scrollWeekEarn -> setTouchEnabled(false);
            m_scrollEstEarn -> setTouchEnabled(true);
            m_scrollFoodEarn -> setTouchEnabled(false);
            break;
        }
        case 106://榜单说明
        {
            m_bClickInfoMenu = true;
            m_spInfoBg -> setVisible(true);
            m_labelInfo -> setString("展示过去60天，每天的盈利冠军");
            break;
        }
        case 107://今日富豪榜
        {
            m_menuTodayEarn -> setVisible(true);
            m_spTodayEarn -> setVisible(false);
            m_menuEstEarn -> setVisible(true);
            m_spEstEarn -> setVisible(false);
            m_menuFoodEarn -> setVisible(false);
            m_spFoodEarn -> setVisible(true);
            m_menuWeekEarn -> setVisible(true);
            m_spWeekEarn -> setVisible(false);
            
            m_nodeTodayEarn -> setVisible(false);
            m_nodeWeekEarn -> setVisible(false);
            m_nodeEstEarn -> setVisible(false);
            m_nodeFoodEarn -> setVisible(true);
            
            m_scrollTodayEarn -> setTouchEnabled(false);
            m_scrollWeekEarn -> setTouchEnabled(false);
            m_scrollEstEarn -> setTouchEnabled(false);
            m_scrollFoodEarn -> setTouchEnabled(true);
            break;
        }
        case 108://榜单说明
        {
            m_bClickInfoMenu = true;
            m_spInfoBg -> setVisible(true);
            m_labelInfo -> setString("每天累计下注最多的玩家，每天0:00重置");
            break;
        }
        default:
            break;
    }
}

void H3RankLayer::clickClose(CCObject* object)
{
    if(m_bClickInfoMenu)
    {
        m_bClickInfoMenu = false;
        m_spInfoBg -> setVisible(false);
        return ;
    }
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(H3RankLayer::removeWnd));
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.55f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
}

void H3RankLayer::removeWnd()
{
    this -> removeFromParent();
}

void H3RankLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool H3RankLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if(m_bClickInfoMenu)
    {
        m_bClickInfoMenu = false;
        m_spInfoBg -> setVisible(false);
    }
    return true;
}
