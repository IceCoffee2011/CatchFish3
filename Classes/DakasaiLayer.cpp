#include "header.h"

DakasaiLayer::DakasaiLayer()
{
    
}

DakasaiLayer::~DakasaiLayer()
{
    for (int i = 0; i < m_vctDakasai.size(); i++) {
        delete m_vctDakasai[i];
    }
    m_vctDakasai.clear();
    for (int i = 0; i < m_vctLastDakasai.size(); i++) {
        delete m_vctLastDakasai[i];
    }
    m_vctLastDakasai.clear();
}

DakasaiLayer* DakasaiLayer::create(std::string str)
{
    DakasaiLayer* pRet = new DakasaiLayer();
    if(pRet && pRet->init(str))
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

bool DakasaiLayer::init(std::string str)
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    m_nCurrentIndex = 1;
    m_vctDakasai.clear();
    m_vctLastDakasai.clear();
    m_vctRule.clear();
    m_vctPrize.clear();
    m_vctVIPNotice.clear();
    m_bRequestSuccess = false;
    
    loadMenuUI();
    loadMoreRankUI(str);
    loadRuleUI();
    loadLastRankUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void DakasaiLayer::loadMenuUI()
{
    m_spBg = CCSprite::create("dakasai/dakasaibg.png");
    m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spBg);
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
    CCSprite* pTitle = CCSprite::create("dakasai/matchtitle.png");
    pTitle -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height-100));
    m_spBg -> addChild(pTitle);
    m_spTitle1 = CCSprite::create("dakasai/matchinfo1.png");
    m_spTitle1->setPosition(ccp(m_spBg->getContentSize().width/2-81, m_spBg->getContentSize().height-160));
    m_spBg -> addChild(m_spTitle1);
    m_spTitle2 = CCSprite::create("dakasai/matchinfo2.png");
    m_spTitle2->setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height-160));
    m_spBg -> addChild(m_spTitle2);
    m_spTitle2 -> setVisible(false);
    
    
    //four menu
    CCMenuItemImage* moreItem = CCMenuItemImage::create("dakasai/morerankitem.png", "dakasai/morerankitem.png", this, menu_selector(DakasaiLayer::clickMenu));
    moreItem -> setTag(101);
    moreItem -> setPosition(ccp(pTitle->getContentSize().width/5-74, pTitle->getContentSize().height/2-4));
    CCMenuItemImage* ruleItem = CCMenuItemImage::create("dakasai/ruleitem.png", "dakasai/ruleitem.png", this, menu_selector(DakasaiLayer::clickMenu));
    ruleItem -> setTag(102);
    ruleItem -> setPosition(ccp(pTitle->getContentSize().width/5*2-24, pTitle->getContentSize().height/2-4));
    CCMenuItemImage* prizeItem = CCMenuItemImage::create("dakasai/prizeitem.png", "dakasai/prizeitem.png", this, menu_selector(DakasaiLayer::clickMenu));
    prizeItem -> setTag(103);
    prizeItem -> setPosition(ccp(pTitle->getContentSize().width/5*3+18, pTitle->getContentSize().height/2-4));
    CCMenuItemImage* lastItem = CCMenuItemImage::create("dakasai/lastrankitem.png", "dakasai/lastrankitem.png", this, menu_selector(DakasaiLayer::clickMenu));
    lastItem -> setTag(105);
    lastItem -> setPosition(ccp(pTitle->getContentSize().width/5*4+70, pTitle->getContentSize().height/2-4));
    
    CCMenuItemImage* closeButton = CCMenuItemImage::create("h2all/dakacloseitem.png","h2all/dakacloseitem.png",this,menu_selector(DakasaiLayer::closeMenu));
    closeButton->setTag(104);
    closeButton->setPosition(ccp(m_spBg->getContentSize().width+80, m_spBg->getContentSize().height-40));
//    closeButton -> setScale(1.2f);
    CCMenu* closeMenu = CCMenu::create(closeButton, NULL);
    closeMenu->setPosition(CCPointZero);
    closeMenu->setAnchorPoint(CCPointZero);
    m_spBg->addChild(closeMenu, 2);
    closeMenu->setTouchPriority(-140);
    
    
    m_moreMenu = CCMenu::create(moreItem, NULL);
    m_moreMenu -> setPosition(CCPointZero);
    m_moreMenu -> setAnchorPoint(CCPointZero);
    pTitle -> addChild(m_moreMenu);
    m_moreMenu -> setTouchPriority(-140);
    m_ruleMenu = CCMenu::create(ruleItem, NULL);
    m_ruleMenu -> setPosition(CCPointZero);
    m_ruleMenu -> setAnchorPoint(CCPointZero);
    pTitle -> addChild(m_ruleMenu);
    m_ruleMenu -> setTouchPriority(-140);
    m_prizeMenu = CCMenu::create(prizeItem, NULL);
    m_prizeMenu -> setPosition(CCPointZero);
    m_prizeMenu -> setAnchorPoint(CCPointZero);
    pTitle -> addChild(m_prizeMenu);
    m_prizeMenu -> setTouchPriority(-140);
    m_lastMenu = CCMenu::create(lastItem, NULL);
    m_lastMenu -> setPosition(CCPointZero);
    m_lastMenu -> setAnchorPoint(CCPointZero);
    pTitle -> addChild(m_lastMenu);
    m_lastMenu -> setTouchPriority(-140);
    
    m_lastMenu->setOpacity(0);
    m_ruleMenu->setOpacity(0);
    m_prizeMenu->setOpacity(0);
}

void DakasaiLayer::loadMoreRankUI(std::string str)
{
    m_nodeRank = CCNode::create();
    m_spBg -> addChild(m_nodeRank);
    CCSprite* pTitleBg = CCSprite::create("dakasai/matchnowcell.png");
    pTitleBg -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height-220));
    m_nodeRank -> addChild(pTitleBg);
    
    m_pRankScrollView = CCScrollView::create();
    m_Ranklayer = CCLayer::create();
    m_Ranklayer -> setContentSize(CCSizeMake(982, 70));//360
    m_Ranklayer -> setAnchorPoint(CCPointZero);
    m_Ranklayer -> setPosition(ccp(0,325-70));
    m_pRankScrollView->setPosition(ccp(5, 108));     //根据cocostudio调整的位置
    m_pRankScrollView->setViewSize(CCSizeMake(982, 325));//设置view的大小
    m_pRankScrollView->setContentOffset(ccp(0,0));
    m_pRankScrollView->setContentSize(CCSizeMake(982, 70));//设置scrollview区域的大小
    m_pRankScrollView->setContainer(m_Ranklayer); //设置需要滚动的内容
    m_pRankScrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_pRankScrollView->setBounceable(true);
    m_pRankScrollView->setDelegate(this);
    m_pRankScrollView->setTouchPriority(-200);
    m_nodeRank->addChild(m_pRankScrollView);
    
    CCSprite* pDelayRank = CCSprite::create("h2all/loading1.png");
    pDelayRank -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2-15));
    m_nodeRank -> addChild(pDelayRank, 1, 101);
    pDelayRank -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
    
    CCHttpRequest* request = new CCHttpRequest();
    char temp[128];
    sprintf(temp, "http://%s/GetMachRank.aspx?type=0&userid=%d", NORMAL_URL, CMainLogic::sharedMainLogic()->m_nUserID);
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(DakasaiLayer::onHttpDakasaiComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void DakasaiLayer::onHttpDakasaiComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    CCSprite* pDelayRank = (CCSprite*)m_nodeRank->getChildByTag(101);
    if(pDelayRank != NULL)
    {
        pDelayRank->stopAllActions();
        pDelayRank->removeFromParent();
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
        if (0!=nodename.compare("dakasai"))
        {
            return;
        }
        std::string sMatchTimes = rootElement->Attribute("n");
        std::string sPrizeName = rootElement->Attribute("a");
        CCLabelTTF* pLabel = CCLabelTTF::create(sMatchTimes.c_str(), "Arial-BoldMT", 32);
        pLabel -> setPosition(ccp(200, m_spTitle1->getContentSize().height/2));
        m_spTitle1 -> addChild(pLabel);
        CCLabelTTF* pLabel2 = CCLabelTTF::create(sPrizeName.c_str(), "Arial-BoldMT", 32);
        pLabel2 -> setPosition(ccp(m_spTitle1->getContentSize().width+20, m_spTitle1->getContentSize().height/2));
        pLabel2->setAnchorPoint(ccp(0, 0.5f));
        m_spTitle1 -> addChild(pLabel2);
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL) {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "item"))//相同
            {
                TagDakasaiRank* pDakaRank = new TagDakasaiRank();
                m_vctDakasai.push_back(pDakaRank);
                std::string sRank = pCurrentElement->Attribute("rank");
                std::string sCup = pCurrentElement->Attribute("cup");
                pDakaRank->nRank = atoi(sRank.c_str());
                pDakaRank->nNum = atoi(sCup.c_str());
                std::string sChangeNum = pCurrentElement->Attribute("num");
                pDakaRank->nChargeNum = atoi(sChangeNum.c_str());
                std::string sLastNum = pCurrentElement->Attribute("lastdaycup");
                pDakaRank->nLastNum = atoi(sLastNum.c_str());
                pDakaRank->sName = pCurrentElement->Attribute("name");
                pDakaRank->sPrize = pCurrentElement->Attribute("reward");
            }
            if(!std::strcmp(pValue, "myitem"))//相同
            {
                TagDakasaiRank* pDakaRank = new TagDakasaiRank();
                m_vctDakasai.push_back(pDakaRank);
                std::string sRank = pCurrentElement->Attribute("rank");
                std::string sCup = pCurrentElement->Attribute("cup");
                pDakaRank->nRank = atoi(sRank.c_str());
                pDakaRank->nNum = atoi(sCup.c_str());
                std::string sChangeNum = pCurrentElement->Attribute("num");
                pDakaRank->nChargeNum = atoi(sChangeNum.c_str());
                std::string sLastNum = pCurrentElement->Attribute("lastdaycup");
                pDakaRank->nLastNum = atoi(sLastNum.c_str());
                pDakaRank->sName = pCurrentElement->Attribute("name");
                pDakaRank->sPrize = pCurrentElement->Attribute("reward");
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
    
    int nCount = (int)m_vctDakasai.size();
    m_Ranklayer -> setContentSize(CCSizeMake(982, 70*(nCount-1)));//360
    m_Ranklayer -> setPosition(ccp(0,325-70*(nCount-1)));
    m_pRankScrollView->setContentSize(CCSizeMake(982, 70*(nCount-1)));//设置scrollview区域的大小
    if(nCount == 0)
        return ;
    if(1)
    {
        //展现自己的数据
        int nHeight = 5;
        CCSprite* pMyCell = CCSprite::create("dakasai/mynowcell.png");
        pMyCell -> setPosition(ccp(m_spBg->getContentSize().width/2, 68));
        m_nodeRank -> addChild(pMyCell);
        CCLabelTTF* pLabelName = CCLabelTTF::create(m_vctDakasai[nCount-1]->sName.c_str(), "Arial-BoldMT", 28);
        pLabelName -> setPosition(ccp(370, pMyCell->getContentSize().height/2+nHeight));
        pMyCell -> addChild(pLabelName);
        char temp[32];
        if(m_vctDakasai[nCount-1]->nRank == 10001)
        {
            sprintf(temp, "千里之外");
            CCLabelTTF* pLabelRank = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
            pLabelRank -> setPosition(ccp(65, pMyCell->getContentSize().height/2+nHeight));
            pMyCell -> addChild(pLabelRank);
            pLabelRank -> setColor(ccc3(123, 47, 17));
        }
        else
        {
            if(m_vctDakasai[nCount-1]->nRank == 1)
            {
                CCSprite* pSpRank = CCSprite::create("h2all/dakasaione.png");
                pSpRank -> setPosition(ccp(65, pMyCell->getContentSize().height/2+nHeight));
                pMyCell -> addChild(pSpRank);
            }
            else if(m_vctDakasai[nCount-1]->nRank == 2)
            {
                CCSprite* pSpRank = CCSprite::create("h2all/dakasaitwo.png");
                pSpRank -> setPosition(ccp(65, pMyCell->getContentSize().height/2+nHeight));
                pMyCell -> addChild(pSpRank);
            }
            else if(m_vctDakasai[nCount-1]->nRank == 3)
            {
                CCSprite* pSpRank = CCSprite::create("h2all/dakasaithree.png");
                pSpRank -> setPosition(ccp(65, pMyCell->getContentSize().height/2+nHeight));
                pMyCell -> addChild(pSpRank);
            }
            else
            {
                sprintf(temp, "%d", m_vctDakasai[nCount-1]->nRank);
                CCLabelTTF* pLabelRank = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabelRank -> setPosition(ccp(65, pMyCell->getContentSize().height/2+nHeight));
                pMyCell -> addChild(pLabelRank);
                pLabelRank -> setColor(ccc3(123, 47, 17));
            }
        }
        
        
        CCSprite* pBei1 = CCSprite::create("h2all/dakasaibei.png");
        pBei1 -> setPosition(ccp(140, pMyCell->getContentSize().height/2+nHeight));
        pMyCell -> addChild(pBei1);
        sprintf(temp, "x%d", m_vctDakasai[nCount-1]->nNum);
        CCLabelTTF* pLabelBei1 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
        pLabelBei1 -> setAnchorPoint(ccp(0, 0.5f));
        pLabelBei1 -> setPosition(ccp(pBei1->getContentSize().width+10, pBei1->getContentSize().height/2));
        pBei1 -> addChild(pLabelBei1);
        pLabelBei1 -> setColor(ccc3(123, 47, 17));
        CCSprite* pBei2 = CCSprite::create("h2all/dakasaibei.png");
        pBei2 -> setPosition(ccp(520, pMyCell->getContentSize().height/2+nHeight));
        pMyCell -> addChild(pBei2);
        sprintf(temp, "x%d", m_vctDakasai[nCount-1]->nLastNum);
        CCLabelTTF* pLabelBei2 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
        pLabelBei2 -> setAnchorPoint(ccp(0, 0.5f));
        pLabelBei2 -> setPosition(ccp(pBei2->getContentSize().width+10, pBei2->getContentSize().height/2));
        pBei2 -> addChild(pLabelBei2);
        if(m_vctDakasai[nCount-1]->nChargeNum == 10001)
        {
            sprintf(temp, "千里之外");
            CCLabelTTF* pLabelChangeNum = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
            pLabelChangeNum -> setPosition(ccp(712, pMyCell->getContentSize().height/2+nHeight));
            pMyCell -> addChild(pLabelChangeNum);
        }
        else
        {
            if(m_vctDakasai[nCount-1]->nChargeNum > 0)
            {
                sprintf(temp, "%d", m_vctDakasai[nCount-1]->nChargeNum);
                CCSprite* pJiantou = CCSprite::create("dakasai/matchshang.png");
                pJiantou -> setPosition(ccp(690, pMyCell->getContentSize().height/2+nHeight));
                pMyCell -> addChild(pJiantou);
                CCLabelTTF* pLabelChangeNum = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabelChangeNum -> setPosition(ccp(pJiantou->getContentSize().width+10, pJiantou->getContentSize().height/2));
                pLabelChangeNum -> setAnchorPoint(ccp(0, 0.5f));
                pJiantou -> addChild(pLabelChangeNum);
            }
            else if(m_vctDakasai[nCount-1]->nChargeNum == 0)
            {
                sprintf(temp, "%d", m_vctDakasai[nCount-1]->nChargeNum);
                CCSprite* pJiantou = CCSprite::create("dakasai/matchzhong.png");
                pJiantou -> setPosition(ccp(690, pMyCell->getContentSize().height/2+nHeight));
                pMyCell -> addChild(pJiantou);
                CCLabelTTF* pLabelChangeNum = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabelChangeNum -> setPosition(ccp(pJiantou->getContentSize().width+10, pJiantou->getContentSize().height/2));
                pLabelChangeNum -> setAnchorPoint(ccp(0, 0.5f));
                pJiantou -> addChild(pLabelChangeNum);
            }
            else
            {
                sprintf(temp, "%d", abs(m_vctDakasai[nCount-1]->nChargeNum));
                CCSprite* pJiantou = CCSprite::create("dakasai/matchxia.png");
                pJiantou -> setPosition(ccp(690, pMyCell->getContentSize().height/2+nHeight));
                pMyCell -> addChild(pJiantou);
                CCLabelTTF* pLabelChangeNum = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabelChangeNum -> setPosition(ccp(pJiantou->getContentSize().width+10, pJiantou->getContentSize().height/2));
                pLabelChangeNum -> setAnchorPoint(ccp(0, 0.5f));
                pJiantou -> addChild(pLabelChangeNum);
            }
            
        }
        CCLabelTTF* pLabelPrize = CCLabelTTF::create(m_vctDakasai[nCount-1]->sPrize.c_str(), "Arial-BoldMT", 28);
        pLabelPrize -> setPosition(ccp(892, pMyCell->getContentSize().height/2+nHeight));
        pMyCell -> addChild(pLabelPrize);
        
    }
    for(int i = 0; i < nCount-1; i++)
    {
        CCSprite* pMyCell = CCSprite::create("dakasai/matchnowcell2.png");
        pMyCell -> setPosition(ccp(m_Ranklayer->getContentSize().width/2, m_Ranklayer->getContentSize().height-70*(i)-35));
        m_Ranklayer -> addChild(pMyCell);
        CCLabelTTF* pLabelName = CCLabelTTF::create(m_vctDakasai[i]->sName.c_str(), "Arial-BoldMT", 28);
        pLabelName -> setPosition(ccp(370, pMyCell->getContentSize().height/2));
        pMyCell -> addChild(pLabelName);
        pLabelName -> setColor(ccc3(123, 47, 17));
        char temp[32];
        if(m_vctDakasai[i]->nRank == 10001)
        {
            sprintf(temp, "千里之外");
            CCLabelTTF* pLabelRank = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
            pLabelRank -> setPosition(ccp(65, pMyCell->getContentSize().height/2));
            pMyCell -> addChild(pLabelRank);
            pLabelRank -> setColor(ccc3(123, 47, 17));
        }
        else
        {
            if(m_vctDakasai[i]->nRank == 1)
            {
                CCSprite* pSpRank = CCSprite::create("h2all/dakasaione.png");
                pSpRank -> setPosition(ccp(65, pMyCell->getContentSize().height/2));
                pMyCell -> addChild(pSpRank);
            }
            else if(m_vctDakasai[i]->nRank == 2)
            {
                CCSprite* pSpRank = CCSprite::create("h2all/dakasaitwo.png");
                pSpRank -> setPosition(ccp(65, pMyCell->getContentSize().height/2));
                pMyCell -> addChild(pSpRank);
            }
            else if(m_vctDakasai[i]->nRank == 3)
            {
                CCSprite* pSpRank = CCSprite::create("h2all/dakasaithree.png");
                pSpRank -> setPosition(ccp(65, pMyCell->getContentSize().height/2));
                pMyCell -> addChild(pSpRank);
            }
            else
            {
                sprintf(temp, "%d", m_vctDakasai[i]->nRank);
                CCLabelTTF* pLabelRank = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabelRank -> setPosition(ccp(65, pMyCell->getContentSize().height/2));
                pMyCell -> addChild(pLabelRank);
                pLabelRank -> setColor(ccc3(123, 47, 17));
            }
        }
        
        CCSprite* pBei1 = CCSprite::create("h2all/dakasaibei.png");
        pBei1 -> setPosition(ccp(140, pMyCell->getContentSize().height/2));
        pMyCell -> addChild(pBei1);
        sprintf(temp, "x%d", m_vctDakasai[i]->nNum);
        CCLabelTTF* pLabelBei1 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
        pLabelBei1 -> setAnchorPoint(ccp(0, 0.5f));
        pLabelBei1 -> setPosition(ccp(pBei1->getContentSize().width+10, pBei1->getContentSize().height/2));
        pBei1 -> addChild(pLabelBei1);
        pLabelBei1 -> setColor(ccc3(123, 47, 17));
        
        CCSprite* pBei2 = CCSprite::create("h2all/dakasaibei.png");
        pBei2 -> setPosition(ccp(520, pMyCell->getContentSize().height/2));
        pMyCell -> addChild(pBei2);
        sprintf(temp, "x%d", m_vctDakasai[i]->nLastNum);
        CCLabelTTF* pLabelBei2 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
        pLabelBei2 -> setAnchorPoint(ccp(0, 0.5f));
        pLabelBei2 -> setPosition(ccp(pBei2->getContentSize().width+10, pBei2->getContentSize().height/2));
        pBei2 -> addChild(pLabelBei2);
        pLabelBei2 -> setColor(ccc3(123, 47, 17));
        if(m_vctDakasai[i]->nChargeNum == 10001)
        {
            sprintf(temp, "千里之外");
            CCLabelTTF* pLabelChangeNum = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
            pLabelChangeNum -> setPosition(ccp(712, pMyCell->getContentSize().height/2));
            pMyCell -> addChild(pLabelChangeNum);
            pLabelChangeNum -> setColor(ccc3(123, 47, 17));
        }
        else
        {
            if(m_vctDakasai[i]->nChargeNum > 0)
            {
                sprintf(temp, "%d", m_vctDakasai[i]->nChargeNum);
                CCSprite* pJiantou = CCSprite::create("dakasai/matchshang.png");
                pJiantou -> setPosition(ccp(690, pMyCell->getContentSize().height/2));
                pMyCell -> addChild(pJiantou);
                CCLabelTTF* pLabelChangeNum = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabelChangeNum -> setPosition(ccp(pJiantou->getContentSize().width+10, pJiantou->getContentSize().height/2));
                pLabelChangeNum -> setAnchorPoint(ccp(0, 0.5f));
                pJiantou -> addChild(pLabelChangeNum);
                pLabelChangeNum -> setColor(ccc3(123, 47, 17));
            }
            else if(m_vctDakasai[i]->nChargeNum == 0)
            {
                sprintf(temp, "%d", m_vctDakasai[i]->nChargeNum);
                CCSprite* pJiantou = CCSprite::create("dakasai/matchzhong.png");
                pJiantou -> setPosition(ccp(690, pMyCell->getContentSize().height/2));
                pMyCell -> addChild(pJiantou);
                CCLabelTTF* pLabelChangeNum = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabelChangeNum -> setPosition(ccp(pJiantou->getContentSize().width+10, pJiantou->getContentSize().height/2));
                pLabelChangeNum -> setAnchorPoint(ccp(0, 0.5f));
                pJiantou -> addChild(pLabelChangeNum);
                pLabelChangeNum -> setColor(ccc3(123, 47, 17));
            }
            else
            {
                sprintf(temp, "%d", abs(m_vctDakasai[i]->nChargeNum));
                CCSprite* pJiantou = CCSprite::create("dakasai/matchxia.png");
                pJiantou -> setPosition(ccp(690, pMyCell->getContentSize().height/2));
                pMyCell -> addChild(pJiantou);
                CCLabelTTF* pLabelChangeNum = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabelChangeNum -> setPosition(ccp(pJiantou->getContentSize().width+10, pJiantou->getContentSize().height/2));
                pLabelChangeNum -> setAnchorPoint(ccp(0, 0.5f));
                pJiantou -> addChild(pLabelChangeNum);
                pLabelChangeNum -> setColor(ccc3(123, 47, 17));
            }
            
        }
        CCLabelTTF* pLabelPrize = CCLabelTTF::create(m_vctDakasai[i]->sPrize.c_str(), "Arial-BoldMT", 28);
        pLabelPrize -> setPosition(ccp(892, pMyCell->getContentSize().height/2));
        pMyCell -> addChild(pLabelPrize);
        pLabelPrize -> setColor(ccc3(123, 47, 17));
        
    }
}

void DakasaiLayer::loadRuleUI()
{
    m_nodeRule = CCNode::create();
    m_spBg -> addChild(m_nodeRule);
    m_nodeRule -> setVisible(false);
    CCSprite* pRuleBg = CCSprite::create("dakasai/matchrule.png");
    pRuleBg -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2-75));
    m_nodeRule -> addChild(pRuleBg);
    
    m_nodePrize = CCNode::create();
    m_spBg -> addChild(m_nodePrize);
    m_nodePrize -> setVisible(false);
    CCSprite* pPrizeBg = CCSprite::create("dakasai/matchrule.png");
    pPrizeBg -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2-75));
    m_nodePrize -> addChild(pPrizeBg);
    
    //请求版本号
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/LeleconfigVersion.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(DakasaiLayer::onHttpVersionComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
    
    CCSprite* pDelayRank = CCSprite::create("h2all/loading1.png");
    pDelayRank -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2-15));
    m_nodeRule -> addChild(pDelayRank, 1, 101);
    pDelayRank -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
    
    CCSprite* pDelayPrize = CCSprite::create("h2all/loading1.png");
    pDelayPrize -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2-15));
    m_nodePrize -> addChild(pDelayPrize, 1, 101);
    pDelayPrize -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
}

void DakasaiLayer::onHttpVersionComplete(CCHttpClient* pSender, CCHttpResponse* response)
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
            if(!std::strcmp(pValue, "dakasairule"))//相同
            {
                std::string sDakasaiVersion = pCurrentElement->GetText();
                if(CCUserDefault::sharedUserDefault()->getStringForKey("DakasaiVersion", "") == "")
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("NewDakasaiVersion", sDakasaiVersion);
                    sendDakasaiData();
                }
                else if(CCUserDefault::sharedUserDefault()->getStringForKey("DakasaiVersion") != sDakasaiVersion)//本地保存的版本号跟新的不一致，说明有更新
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("NewDakasaiVersion", sDakasaiVersion);
                    sendDakasaiData();
                }
                else//本地已经保存且没有更新内容，则此时直接打开即可
                {
                    readRuleData();
                }
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
}

void DakasaiLayer::sendDakasaiData()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/Leledakasairule.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(DakasaiLayer::onHttpDakasaiDataComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void DakasaiLayer::onHttpDakasaiDataComplete(CCHttpClient* pSender, CCHttpResponse* response)
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
    std::string sVIPNewVersion = CCUserDefault::sharedUserDefault()->getStringForKey("NewDakasaiVersion");
    CCUserDefault::sharedUserDefault()->setStringForKey("DakasaiVersion", sVIPNewVersion);
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement == NULL) return;
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("dakasai"))
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
                m_vctRule.push_back(sData);
            }
            else if(!std::strcmp(pValue, "prize"))//相同
            {
                std::string sPrize = pCurrentElement->Attribute("data");
                m_vctPrize.push_back(sPrize);
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
        
    }
    delete document;
    
    //展示界面
    showRuleUI();
    showPrizeUI();
    saveFile(str);
}

void DakasaiLayer::saveFile(std::string str)
{
    //在Documents目录下创建一个可读写的文件
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + std::string("DakasaiData.xml");
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

void DakasaiLayer::readRuleData()
{
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + std::string("DakasaiData.xml");
    TiXmlDocument* document = new TiXmlDocument(xmlPath.c_str());
    if(document != NULL)
    {
        document -> LoadFile();
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement == NULL) return;
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("dakasai"))
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
                m_vctRule.push_back(sData);
            }
            else if(!std::strcmp(pValue, "prize"))//相同
            {
                std::string sPrize = pCurrentElement->Attribute("data");
                m_vctPrize.push_back(sPrize);
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    document->Clear();
    delete document;
    
    showRuleUI();
    showPrizeUI();
}

void DakasaiLayer::showRuleUI()
{
    CCSprite* pDelayRank = (CCSprite*)m_nodeRule->getChildByTag(101);
    if(pDelayRank != NULL)
    {
        pDelayRank->stopAllActions();
        pDelayRank->removeFromParent();
    }
    int nCount = (int)m_vctRule.size();
    m_pRuleScrollView = CCScrollView::create();
    CCLayer* layer = CCLayer::create();
    layer -> setContentSize(CCSizeMake(910, 40*(nCount+1)));//360
    layer -> setAnchorPoint(CCPointZero);
    layer -> setPosition(ccp(0,405-40*(nCount+1)));
    m_pRuleScrollView->setPosition(ccp(35, 67));     //根据cocostudio调整的位置
    m_pRuleScrollView->setViewSize(CCSizeMake(910, 405));//设置view的大小
    m_pRuleScrollView->setContentOffset(ccp(0,0));
    m_pRuleScrollView->setContentSize(CCSizeMake(910, 40*(nCount+1)));//设置scrollview区域的大小
    m_pRuleScrollView->setContainer(layer); //设置需要滚动的内容
    m_pRuleScrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_pRuleScrollView->setBounceable(true);
    if(nCount>10)
    {
        m_pRuleScrollView->setTouchPriority(-200);
    }
    
    m_nodeRule->addChild(m_pRuleScrollView);
    for(int i = 0; i < nCount; i++)
    {
        CCLabelTTF* pLabelRule = CCLabelTTF::create("", "Arial", 30);
        pLabelRule -> setPosition(ccp(5, layer->getContentSize().height-30-40*i));
        pLabelRule -> setAnchorPoint(ccp(0, 0.5f));
        layer -> addChild(pLabelRule);
        m_vctVIPNotice.clear();
        float fWidth=0;
        std::string str = m_vctRule[i];
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
                
                CCLabelTTF* pLabel = CCLabelTTF::create(sNotice.c_str(), "Arial", 30);
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
                
                CCLabelTTF* pLabel = CCLabelTTF::create(sNotice.c_str(), "Arial", 30);
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

void DakasaiLayer::showPrizeUI()
{
    CCSprite* pDelayRank = (CCSprite*)m_nodePrize->getChildByTag(101);
    if(pDelayRank != NULL)
    {
        pDelayRank->stopAllActions();
        pDelayRank->removeFromParent();
    }
    int nCount = (int)m_vctPrize.size();
    m_pPrizeScrollView = CCScrollView::create();
    CCLayer* layer = CCLayer::create();
    layer -> setContentSize(CCSizeMake(910, 40*(nCount+1)));//360
    layer -> setAnchorPoint(CCPointZero);
    layer -> setPosition(ccp(0,405-40*(nCount+1)));
    m_pPrizeScrollView->setPosition(ccp(35, 67));     //根据cocostudio调整的位置
    m_pPrizeScrollView->setViewSize(CCSizeMake(910, 405));//设置view的大小
    m_pPrizeScrollView->setContentOffset(ccp(0,0));
    m_pPrizeScrollView->setContentSize(CCSizeMake(910, 40*(nCount+1)));//设置scrollview区域的大小
    m_pPrizeScrollView->setContainer(layer); //设置需要滚动的内容
    m_pPrizeScrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_pPrizeScrollView->setBounceable(true);
    if(nCount>10)
    {
        m_pPrizeScrollView->setTouchPriority(-200);
    }
    
    m_nodePrize->addChild(m_pPrizeScrollView);
    for(int i = 0; i < nCount; i++)
    {
        CCLabelTTF* pLabelRule = CCLabelTTF::create("", "Arial", 30);
        pLabelRule -> setPosition(ccp(5, layer->getContentSize().height-30-40*i));
        pLabelRule -> setAnchorPoint(ccp(0, 0.5f));
        layer -> addChild(pLabelRule);
        m_vctVIPNotice.clear();
        float fWidth=0;
        std::string str = m_vctPrize[i];
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
                
                CCLabelTTF* pLabel = CCLabelTTF::create(sNotice.c_str(), "Arial", 30);
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
                
                CCLabelTTF* pLabel = CCLabelTTF::create(sNotice.c_str(), "Arial", 30);
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

void DakasaiLayer::loadLastRankUI()
{
    m_nodeLastRank = CCNode::create();
    m_spBg -> addChild(m_nodeLastRank);
    m_nodeLastRank -> setVisible(false);
    
    CCSprite* pDelayRank = CCSprite::create("h2all/loading1.png");
    pDelayRank -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2-15));
    m_nodeLastRank -> addChild(pDelayRank, 1, 101);
    pDelayRank -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
    //请求上一次排行榜
    CCHttpRequest* request = new CCHttpRequest();
    char temp[128];
    sprintf(temp, "http://%s/GetMachRank.aspx?type=1&userid=%d", NORMAL_URL, CMainLogic::sharedMainLogic()->m_nUserID);
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(DakasaiLayer::onHttpDakasaiLastComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void DakasaiLayer::onHttpDakasaiLastComplete(CCHttpClient* pSender, CCHttpResponse* response)
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
        if (0!=nodename.compare("dakasai"))
        {
            return;
        }
        std::string sMatchTimes = rootElement->Attribute("n");
        CCLabelTTF* pLabel = CCLabelTTF::create(sMatchTimes.c_str(), "Arial-BoldMT", 32);
        pLabel -> setPosition(ccp(200, m_spTitle2->getContentSize().height/2));
        m_spTitle2 -> addChild(pLabel);
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL) {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "item"))//相同
            {
                TagDakasaiRank* pDakaRank = new TagDakasaiRank();
                m_vctLastDakasai.push_back(pDakaRank);
                std::string sRank = pCurrentElement->Attribute("rank");
                std::string sCup = pCurrentElement->Attribute("cup");
                pDakaRank->nRank = atoi(sRank.c_str());
                pDakaRank->nNum = atoi(sCup.c_str());
                pDakaRank->sName = pCurrentElement->Attribute("name");
                pDakaRank->sPrize = pCurrentElement->Attribute("reward");
            }
            if(!std::strcmp(pValue, "myitem"))//相同
            {
                TagDakasaiRank* pDakaRank = new TagDakasaiRank();
                m_vctLastDakasai.push_back(pDakaRank);
                std::string sRank = pCurrentElement->Attribute("rank");
                std::string sCup = pCurrentElement->Attribute("cup");
                pDakaRank->nRank = atoi(sRank.c_str());
                pDakaRank->nNum = atoi(sCup.c_str());
                pDakaRank->sName = pCurrentElement->Attribute("name");
                pDakaRank->sPrize = pCurrentElement->Attribute("reward");
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
    CCSprite* pDelayRank = (CCSprite*)m_nodeLastRank->getChildByTag(101);
    if(pDelayRank != NULL)
    {
        pDelayRank->stopAllActions();
        pDelayRank->removeFromParent();
    }
    m_bRequestSuccess = true;
    int nCount = (int)m_vctLastDakasai.size();
    m_pLastScrollView = CCScrollView::create();
    CCLayer* layer = CCLayer::create();
    layer -> setContentSize(CCSizeMake(982, 70*(nCount-1)));//360
    layer -> setAnchorPoint(CCPointZero);
    layer -> setPosition(ccp(0,325-70*(nCount-1)));
    m_pLastScrollView->setPosition(ccp(5, 108));     //根据cocostudio调整的位置
    m_pLastScrollView->setViewSize(CCSizeMake(982, 325));//设置view的大小
    m_pLastScrollView->setContentOffset(ccp(0,0));
    m_pLastScrollView->setContentSize(CCSizeMake(982, 70*(nCount-1)));//设置scrollview区域的大小
    m_pLastScrollView->setContainer(layer); //设置需要滚动的内容
    m_pLastScrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_pLastScrollView->setBounceable(true);
    m_pLastScrollView->setDelegate(this);
    m_pLastScrollView->setTouchPriority(-200);
    m_nodeLastRank->addChild(m_pLastScrollView);
    m_pLastScrollView->setTouchEnabled(false);
    
    CCSprite* pTitleBg = CCSprite::create("dakasai/matchlastcell.png");
    pTitleBg -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height-220));
    m_nodeLastRank -> addChild(pTitleBg);
    if(nCount == 0)
        return ;
    
    if(1)
    {
        //展现自己的数据
        int nHeight = 5;
        CCSprite* pMyCell = CCSprite::create("dakasai/mylastcell.png");
        pMyCell -> setPosition(ccp(m_spBg->getContentSize().width/2, 68));
        m_nodeLastRank -> addChild(pMyCell);
        CCLabelTTF* pLabelName = CCLabelTTF::create(m_vctLastDakasai[nCount-1]->sName.c_str(), "Arial-BoldMT", 28);
        pLabelName -> setPosition(ccp(318, pMyCell->getContentSize().height/2+nHeight));
        pMyCell -> addChild(pLabelName);
        char temp[32];
        if(m_vctLastDakasai[nCount-1]->nRank == 10001)
        {
            sprintf(temp, "千里之外");
            CCLabelTTF* pLabelRank = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
            pLabelRank -> setPosition(ccp(76, pMyCell->getContentSize().height/2+nHeight));
            pMyCell -> addChild(pLabelRank);
            pLabelRank -> setColor(ccc3(123, 47, 17));
        }
        else
        {
            if(m_vctLastDakasai[nCount-1]->nRank == 1)
            {
                CCSprite* pSpRank = CCSprite::create("h2all/dakasaione.png");
                pSpRank -> setPosition(ccp(76, pMyCell->getContentSize().height/2+nHeight));
                pMyCell -> addChild(pSpRank);
            }
            else if(m_vctLastDakasai[nCount-1]->nRank == 2)
            {
                CCSprite* pSpRank = CCSprite::create("h2all/dakasaitwo.png");
                pSpRank -> setPosition(ccp(76, pMyCell->getContentSize().height/2+nHeight));
                pMyCell -> addChild(pSpRank);
            }
            else if(m_vctLastDakasai[nCount-1]->nRank == 3)
            {
                CCSprite* pSpRank = CCSprite::create("h2all/dakasaithree.png");
                pSpRank -> setPosition(ccp(76, pMyCell->getContentSize().height/2+nHeight));
                pMyCell -> addChild(pSpRank);
            }
            else
            {
                sprintf(temp, "%d", m_vctLastDakasai[nCount-1]->nRank);
                CCLabelTTF* pLabelRank = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabelRank -> setPosition(ccp(76, pMyCell->getContentSize().height/2+nHeight));
                pMyCell -> addChild(pLabelRank);
                pLabelRank -> setColor(ccc3(123, 47, 17));
            }
        }
        
        
        CCSprite* pBei1 = CCSprite::create("h2all/dakasaibei.png");
        pBei1 -> setPosition(ccp(575, pMyCell->getContentSize().height/2+nHeight));
        pMyCell -> addChild(pBei1);
        sprintf(temp, "x%d", m_vctLastDakasai[nCount-1]->nNum);
        CCLabelTTF* pLabelBei1 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
        pLabelBei1 -> setAnchorPoint(ccp(0, 0.5f));
        pLabelBei1 -> setPosition(ccp(pBei1->getContentSize().width+10, pBei1->getContentSize().height/2));
        pBei1 -> addChild(pLabelBei1);
        pLabelBei1 -> setColor(ccc3(123, 47, 17));
        CCLabelTTF* pLabelPrize = CCLabelTTF::create(m_vctLastDakasai[nCount-1]->sPrize.c_str(), "Arial-BoldMT", 28);
        pLabelPrize -> setPosition(ccp(857, pMyCell->getContentSize().height/2+nHeight));
        pMyCell -> addChild(pLabelPrize);
        
    }
    for(int i = 0; i < nCount-1; i++)
    {
        CCSprite* pMyCell = CCSprite::create("dakasai/matchlastcell2.png");
        pMyCell -> setPosition(ccp(layer->getContentSize().width/2, layer->getContentSize().height-70*(i)-35));
        layer -> addChild(pMyCell);
        CCLabelTTF* pLabelName = CCLabelTTF::create(m_vctLastDakasai[i]->sName.c_str(), "Arial-BoldMT", 28);
        pLabelName -> setPosition(ccp(318, pMyCell->getContentSize().height/2));
        pMyCell -> addChild(pLabelName);
        pLabelName -> setColor(ccc3(123, 47, 17));
        char temp[32];
        if(m_vctLastDakasai[i]->nRank == 10001)
        {
            sprintf(temp, "千里之外");
            CCLabelTTF* pLabelRank = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
            pLabelRank -> setPosition(ccp(76, pMyCell->getContentSize().height/2));
            pMyCell -> addChild(pLabelRank);
            pLabelRank -> setColor(ccc3(123, 47, 17));
        }
        else
        {
            if(m_vctLastDakasai[i]->nRank == 1)
            {
                CCSprite* pSpRank = CCSprite::create("h2all/dakasaione.png");
                pSpRank -> setPosition(ccp(76, pMyCell->getContentSize().height/2));
                pMyCell -> addChild(pSpRank);
            }
            else if(m_vctLastDakasai[i]->nRank == 2)
            {
                CCSprite* pSpRank = CCSprite::create("h2all/dakasaitwo.png");
                pSpRank -> setPosition(ccp(76, pMyCell->getContentSize().height/2));
                pMyCell -> addChild(pSpRank);
            }
            else if(m_vctLastDakasai[i]->nRank == 3)
            {
                CCSprite* pSpRank = CCSprite::create("h2all/dakasaithree.png");
                pSpRank -> setPosition(ccp(76, pMyCell->getContentSize().height/2));
                pMyCell -> addChild(pSpRank);
            }
            else
            {
                sprintf(temp, "%d", m_vctLastDakasai[i]->nRank);
                CCLabelTTF* pLabelRank = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
                pLabelRank -> setPosition(ccp(76, pMyCell->getContentSize().height/2));
                pMyCell -> addChild(pLabelRank);
                pLabelRank -> setColor(ccc3(123, 47, 17));
            }
        }
        
        CCSprite* pBei1 = CCSprite::create("h2all/dakasaibei.png");
        pBei1 -> setPosition(ccp(575, pMyCell->getContentSize().height/2));
        pMyCell -> addChild(pBei1);
        sprintf(temp, "x%d", m_vctLastDakasai[i]->nNum);
        CCLabelTTF* pLabelBei1 = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
        pLabelBei1 -> setAnchorPoint(ccp(0, 0.5f));
        pLabelBei1 -> setPosition(ccp(pBei1->getContentSize().width+10, pBei1->getContentSize().height/2));
        pBei1 -> addChild(pLabelBei1);
        pLabelBei1 -> setColor(ccc3(123, 47, 17));
        
        CCLabelTTF* pLabelPrize = CCLabelTTF::create(m_vctLastDakasai[i]->sPrize.c_str(), "Arial-BoldMT", 28);
        pLabelPrize -> setPosition(ccp(857, pMyCell->getContentSize().height/2));
        pMyCell -> addChild(pLabelPrize);
        pLabelPrize -> setColor(ccc3(123, 47, 17));
        
    }
}

void DakasaiLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case 101:
        {
            if(m_nCurrentIndex != 1)
            {
                SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                m_nCurrentIndex = 1;
                m_nodeRank->setVisible(true);
                m_nodeRule->setVisible(false);
                m_nodePrize->setVisible(false);
                m_nodeLastRank->setVisible(false);
                m_pRankScrollView->setTouchEnabled(true);
                if(m_bRequestSuccess)
                {
                    m_pLastScrollView->setTouchEnabled(false);
                }
                m_moreMenu->setOpacity(255);
                m_ruleMenu->setOpacity(0);
                m_prizeMenu->setOpacity(0);
                m_lastMenu->setOpacity(0);
                m_spTitle1->setVisible(true);
                m_spTitle2->setVisible(false);
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWeekMatch1);
            }
            break;
        }
        case 102:
        {
            if(m_nCurrentIndex != 2)
            {
                SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                m_nCurrentIndex = 2;
                m_nodeRank->setVisible(false);
                m_nodeRule->setVisible(true);
                m_nodePrize->setVisible(false);
                m_nodeLastRank->setVisible(false);
                m_pRankScrollView->setTouchEnabled(false);
                if(m_bRequestSuccess)
                {
                    m_pLastScrollView->setTouchEnabled(false);
                }
                m_moreMenu->setOpacity(0);
                m_ruleMenu->setOpacity(255);
                m_prizeMenu->setOpacity(0);
                m_lastMenu->setOpacity(0);
                m_spTitle1->setVisible(true);
                m_spTitle2->setVisible(false);
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWeekMatch2);
            }
            break;
        }
        case 103:
        {
            if(m_nCurrentIndex != 3)
            {
                SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                m_nCurrentIndex = 3;
                m_nodeRank->setVisible(false);
                m_nodeRule->setVisible(false);
                m_nodePrize->setVisible(true);
                m_nodeLastRank->setVisible(false);
                m_pRankScrollView->setTouchEnabled(false);
                if(m_bRequestSuccess)
                {
                    m_pLastScrollView->setTouchEnabled(false);
                }
                m_moreMenu->setOpacity(0);
                m_ruleMenu->setOpacity(0);
                m_prizeMenu->setOpacity(255);
                m_lastMenu->setOpacity(0);
                m_spTitle1->setVisible(true);
                m_spTitle2->setVisible(false);
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWeekMatch3);
            }
            break;
        }
        case 105:
        {
            if(m_nCurrentIndex != 4)
            {
                SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                m_nCurrentIndex = 4;
                m_nodeRank->setVisible(false);
                m_nodeRule->setVisible(false);
                m_nodePrize->setVisible(false);
                m_nodeLastRank->setVisible(true);
                m_pRankScrollView->setTouchEnabled(false);
                if(m_bRequestSuccess)
                {
                    m_pLastScrollView->setTouchEnabled(true);
                }
                m_moreMenu->setOpacity(0);
                m_ruleMenu->setOpacity(0);
                m_prizeMenu->setOpacity(0);
                m_lastMenu->setOpacity(255);
                m_spTitle1->setVisible(false);
                m_spTitle2->setVisible(true);
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWeekMatch4);
            }
            break;
        }
        default:
            break;
    }
}

void DakasaiLayer::closeMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(DakasaiLayer::removeFunc));
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWeekMatch5);
}

void DakasaiLayer::removeFunc()
{
    this -> removeFromParent();
}

void DakasaiLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool DakasaiLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}
