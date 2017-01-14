#include "header.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "FMLayerWebView.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#endif

const char* MenuNormalName[2] = {"ranknormal1.png", "ranknormal2.png"};
const char* MenuSelectName[2] = {"rankselect1.png", "rankselect2.png"};

const char* RankSpriteInfo[4] = {"vipdengji.png", "leijijinbi.png", "jinrijifen.png", "benzhoujifen.png"};
const char* RankSpriteInfo2[3] = {"vipdengji.png", "jinrijifen.png", "benzhoujifen.png"};

const CCPoint CardNormalPoint[4] = {ccp(357/2, 720-107/2), ccp(357+357/2-3, 720-107/2), ccp(357*2+356/2-6, 720-107/2), ccp(357*2+356+356/2-11, 720-107/2)};

const CCPoint CardGamePoint[4] = {ccp(357/2, 720-107/2), ccp(0, 0), ccp(357+356/2-6, 720-107/2), ccp(357+356+356/2-11, 720-107/2)};

const ccColor3B DayPriseInfo[4] = {ccc3(255, 0, 0), ccc3(255, 0, 255), ccc3(0, 0, 255), ccc3(32, 66, 117)};

enum Rank_Tag
{
    nickName_tag = 200,
    jinbi_Tag,
    honor_Tag,
};

RankLayer::~RankLayer()
{
    m_arrNormal->release();
    m_arrSelect->release();
    m_arrVipCell->release();
    m_arrRankInfo->release();
    m_arrMoneyCell->release();
    m_arrDayCell->release();
    m_arrWeekCell->release();
    
    m_vctVipRank.clear();
    m_vctMoneyRank.clear();
    m_vctMatch.clear();
    m_vctWeekMatch.clear();
    for (int i = 0; i < m_vctDayRankTimes.size(); i++) {
        delete m_vctDayRankTimes[i];
    }
    m_vctDayRankTimes.clear();
    for (int i = 0; i < m_vctWeekRankTimes.size(); i++) {
        delete m_vctWeekRankTimes[i];
    }
    m_vctWeekRankTimes.clear();
    for (int i = 0; i < m_vctVipRankTimes.size(); i++) {
        delete m_vctVipRankTimes[i];
    }
    m_vctVipRankTimes.clear();
    for (int i = 0; i < m_vctMoneyRankTimes.size(); i++) {
        delete m_vctMoneyRankTimes[i];
    }
    m_vctMoneyRankTimes.clear();
}

bool RankLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("rank/bigrank.plist");
    
    m_sVip1Title="";
    m_sVip2Title="";
    m_sMoney1Title="";
    m_sMoney2Title="";
    m_sDayMatchTitle="";
    m_sWeekMatchTitle="";
    m_arrNormal = CCArray::create();
    m_arrNormal -> retain();
    m_arrSelect = CCArray::create();
    m_arrSelect -> retain();
    m_arrVipCell = CCArray::create();
    m_arrVipCell -> retain();
    m_arrRankInfo = CCArray::create();
    m_arrRankInfo -> retain();
    m_arrMoneyCell = CCArray::create();
    m_arrMoneyCell -> retain();
    m_arrDayCell = CCArray::create();
    m_arrDayCell -> retain();
    m_arrWeekCell = CCArray::create();
    m_arrWeekCell -> retain();
    if(GameMainScene::_instance != NULL)
    {
        m_nCurrentIndex = 3;
    }
    else
    {
        m_nCurrentIndex = 1;
    }
    
    loadUI();
    loadMatchXML();
    loadUIVip();
    loadUIMoney();
    //loadUIDayRank();
    //loadUIWeekRank();
    loadMatchUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}


void RankLayer::loadUI()
{
    m_spRankBg = CCSprite::createWithSpriteFrameName("rankbg.png");
    m_spRankBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spRankBg);
    m_spRankBg -> setScale(2.0f);
    CCMenuItemImage* closeItem = CCMenuItemImage::create("barrelUI/lotterycloseitem.png", "barrelUI/lotterycloseitem.png", this, menu_selector(RankLayer::closeMenu));
    closeItem -> setScale(2.0f);
    closeItem -> setPosition(ccp(CLIENT_WIDTH-closeItem->boundingBox().size.width/2+5, CLIENT_HEIGHT-closeItem->boundingBox().size.height/2+5));
    CCMenu* pMenuClose = CCMenu::create(closeItem, NULL);
    pMenuClose -> setPosition(CCPointZero);
    pMenuClose -> setAnchorPoint(CCPointZero);
    this -> addChild(pMenuClose);
    pMenuClose -> setTouchPriority(-200);
    
    CCSprite* pTitleBg = CCSprite::createWithSpriteFrameName("ranktitlebg.png");
    pTitleBg -> setPosition(ccp(28+pTitleBg->getContentSize().width/2, 578));
    this -> addChild(pTitleBg);
    
    //bottom lable
    CCSprite* pSpriteRule = CCSprite::createWithSpriteFrameName("rankruleitem.png");
    CCMenuItemSprite* ruleItem = CCMenuItemSprite::create(pSpriteRule, pSpriteRule, this, menu_selector(RankLayer::clickMenu));
    ruleItem -> setPosition(ccp(ruleItem->getContentSize().width/2+10, ruleItem->getContentSize().height/2+5));
    ruleItem -> setTag(105);
    CCMenu* pRuleMenu = CCMenu::create(ruleItem, NULL);
    pRuleMenu -> setPosition(CCPointZero);
    pRuleMenu -> setAnchorPoint(CCPointZero);
    this -> addChild(pRuleMenu);
    pRuleMenu -> setTouchPriority(-200);
    CCSprite* pNickNameLabel = CCSprite::createWithSpriteFrameName("ranknichenglabel.png");
    pNickNameLabel -> setPosition(ccp(370, 30));
    this -> addChild(pNickNameLabel);
    CCSprite* pNickNamekuang = CCSprite::createWithSpriteFrameName("rankkuangbg.png");
    pNickNamekuang -> setPosition(ccp(505, 30));
    this -> addChild(pNickNamekuang);
    CCLabelTTF* pNickName = CCLabelTTF::create(CMainLogic::sharedMainLogic()->m_sNickName.c_str(), "Arial-BoldMT", 28);
    pNickName -> setPosition(ccp(15,pNickNamekuang->getContentSize().height/2));
    pNickName -> setAnchorPoint(ccp(0, 0.5f));
    pNickName -> setColor(ccc3(253, 196, 57));
    pNickNamekuang -> addChild(pNickName);
    if(CMainLogic::sharedMainLogic()->m_nVipLevel >= 1)
    {
        pNickName -> setColor(ccc3(255, 0, 0));
    }
    
    CCSprite* pJifenKuang = CCSprite::createWithSpriteFrameName("rankkuangbg.png");
    pJifenKuang -> setPosition(ccp(840, 30));
    this -> addChild(pJifenKuang);
    CCSprite* pPaimingLabel = CCSprite::createWithSpriteFrameName("rankpaiminglabel.png");
    pPaimingLabel -> setPosition(ccp(1010, 30));
    this -> addChild(pPaimingLabel);
    CCSprite* pPaimingKuang = CCSprite::createWithSpriteFrameName("rankkuangbg.png");
    pPaimingKuang -> setPosition(ccp(1170, 30));
    this -> addChild(pPaimingKuang);
    //top menu
    if(GameMainScene::_instance != NULL)
    {
        for(int i = 0; i < 2; i++)
        {
            if(i == 1)
                continue;
            CCSprite* pSpDi = CCSprite::createWithSpriteFrameName(MenuNormalName[i]);
            pSpDi -> setPosition(CardGamePoint[i]);
            this -> addChild(pSpDi);
            m_arrNormal -> addObject(pSpDi);
            pSpDi -> setTag(101+i);
            CCSprite* pSpDi2 = CCSprite::createWithSpriteFrameName(MenuSelectName[i]);
            pSpDi2 -> setPosition(CardGamePoint[i]);
            this -> addChild(pSpDi2);
            m_arrSelect -> addObject(pSpDi2);
            pSpDi2 -> setTag(101+i);
            if(i != 2)
            {
                pSpDi2->setVisible(false);
            }
            
            CCSprite* pSpInfo = CCSprite::createWithSpriteFrameName(RankSpriteInfo[i]);
            pSpInfo -> setPosition(ccp(pTitleBg->getContentSize().width/4*3+60, pTitleBg->getContentSize().height/2));
            pSpInfo -> setTag(101+i);
            pTitleBg -> addChild(pSpInfo);
            m_arrRankInfo -> addObject(pSpInfo);
            if(i != 2)
            {
                pSpInfo -> setVisible(false);
            }
        }
    }
    else
    {
        for(int i = 0; i < 2; i++)
        {
            CCSprite* pSpDi = CCSprite::createWithSpriteFrameName(MenuNormalName[i]);
            pSpDi -> setPosition(CardNormalPoint[i]);
            this -> addChild(pSpDi);
            m_arrNormal -> addObject(pSpDi);
            pSpDi -> setTag(101+i);
            CCSprite* pSpDi2 = CCSprite::createWithSpriteFrameName(MenuSelectName[i]);
            pSpDi2 -> setPosition(CardNormalPoint[i]);
            this -> addChild(pSpDi2);
            m_arrSelect -> addObject(pSpDi2);
            pSpDi2 -> setTag(101+i);
            CCSprite* pSpInfo = CCSprite::createWithSpriteFrameName(RankSpriteInfo[i]);
            pSpInfo -> setPosition(ccp(pTitleBg->getContentSize().width/4*3+60, pTitleBg->getContentSize().height/2));
            pTitleBg -> addChild(pSpInfo);
            pSpInfo -> setTag(101+i);
            m_arrRankInfo -> addObject(pSpInfo);
            if(i != 0)
            {
                pSpDi2->setVisible(false);
                pSpInfo -> setVisible(false);
            }
        }
    }
}

void RankLayer::loadMatchXML()
{
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + std::string("MatchPriseInfo.xml");
    TiXmlDocument* document = new TiXmlDocument(xmlPath.c_str());
    if(document != NULL)
    {
        document -> LoadFile();
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("rankInfo"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement!=NULL)
        {
            std::string pValue = pCurrentElement->Value();
            if (pValue == "vipRank")
            {
                m_sVip1Title = pCurrentElement->Attribute("str1");
                m_sVip2Title = pCurrentElement->Attribute("str2");
                
                TiXmlElement* tCurrentElement=pCurrentElement->FirstChildElement();
                while (tCurrentElement != NULL)
                {
                    
                    const char * sValue = tCurrentElement->Value();
                    if(!std::strcmp(sValue, "item"))
                    {
                        tagVipRankInfo pVipRankInfo={};
                        const char * tValue = tCurrentElement->Attribute("level");
                        if(NULL != tValue)
                        {
                            pVipRankInfo.sLevel = (tValue);
                        }
                        tValue = tCurrentElement->Attribute("barrel");
                        if(NULL != tValue)
                        {
                            pVipRankInfo.sBarrel = tValue;
                        }
                        tValue = tCurrentElement->Attribute("logonCoin");
                        if(NULL != tValue)
                        {
                            pVipRankInfo.sLogonCoin = tValue;
                        }
                        tValue = tCurrentElement->Attribute("chargeExtra");
                        if(NULL != tValue)
                        {
                            pVipRankInfo.sChargeExtra = tValue;
                        }
                        m_vctVipRank.push_back(pVipRankInfo);
                        tCurrentElement=tCurrentElement->NextSiblingElement();
                    }
                }
            }
            else if (pValue == "moneyRank")
            {
                m_sMoney1Title = pCurrentElement->Attribute("str1");
                m_sMoney2Title = pCurrentElement->Attribute("str2");
                TiXmlElement* tCurrentElement=pCurrentElement->FirstChildElement();
                while (tCurrentElement != NULL)
                {
                    const char * sValue = tCurrentElement->Value();
                    if(!std::strcmp(sValue, "item"))
                    {
                        tagMoneyRankInfo pMoneyRankInfo={};
                        const char * tValue = tCurrentElement->Attribute("charge");
                        if(NULL != tValue)
                        {
                            pMoneyRankInfo.sCharge = tValue;
                        }
                        tValue = tCurrentElement->Attribute("chargeExtra");
                        if(NULL != tValue)
                        {
                            pMoneyRankInfo.sChargeExtra = tValue;
                        }
                        m_vctMoneyRank.push_back(pMoneyRankInfo);
                        
                        tCurrentElement=tCurrentElement->NextSiblingElement();
                    }
                }
            }
            else if (pValue == "dayRank")
            {
                m_sDayMatchTitle = pCurrentElement->Attribute("str");
                TiXmlElement* tCurrentElement=pCurrentElement->FirstChildElement();
                while (tCurrentElement != NULL)
                {
                    const char * sValue = tCurrentElement->Value();
                    if(!std::strcmp(sValue, "item"))
                    {
                        tagMatchInfo pMatchRankInfo={};
                        const char * tValue = tCurrentElement->Attribute("rank");
                        if(NULL != tValue)
                        {
                            pMatchRankInfo.sLevel = tValue;
                        }
                        tValue = tCurrentElement->Attribute("prizeCoin");
                        if(NULL != tValue)
                        {
                            pMatchRankInfo.sCoin = tValue;
                        }
                        tValue = tCurrentElement->Attribute("zhenzhu");
                        if(NULL != tValue)
                        {
                            pMatchRankInfo.sZhenzhu = tValue;
                        }
                        tValue = tCurrentElement->Attribute("lottery");
                        if(NULL != tValue)
                        {
                            pMatchRankInfo.sLottery = tValue;
                        }
                        m_vctMatch.push_back(pMatchRankInfo);
                        
                        tCurrentElement=tCurrentElement->NextSiblingElement();
                    }
                }
            }
            else if (pValue == "weekRank")
            {
                m_sWeekMatchTitle = pCurrentElement->Attribute("str");
                TiXmlElement* tCurrentElement=pCurrentElement->FirstChildElement();
                while (tCurrentElement != NULL)
                {
                    const char * sValue = tCurrentElement->Value();
                    if(!std::strcmp(sValue, "item"))
                    {
                        tagWeekMatchInfo pWeekMatchInfo={};
                        const char * tValue = tCurrentElement->Attribute("rank");
                        if(NULL != tValue)
                        {
                            pWeekMatchInfo.sLevel = tValue;
                        }
                        tValue = tCurrentElement->Attribute("prize");
                        if(NULL != tValue)
                        {
                            pWeekMatchInfo.sPrize = tValue;
                        }
                        m_vctWeekMatch.push_back(pWeekMatchInfo);
                        tCurrentElement=tCurrentElement->NextSiblingElement();
                    }
                }
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
}

void RankLayer::loadMatchUI()
{
    m_MatchNode = CCNode::create();
    this -> addChild(m_MatchNode);
    if(GameMainScene::_instance == NULL)
    {
        m_MatchNode -> setVisible(false);
    }
    
    CCSprite* pPaimingSp = CCSprite::createWithSpriteFrameName("rankpaimingbg.png");
    pPaimingSp -> setPosition(ccp(CLIENT_WIDTH-pPaimingSp->getContentSize().width/2-25, CLIENT_HEIGHT/3*2+45));
    m_MatchNode -> addChild(pPaimingSp);
    m_labelDayYesRank = CCLabelTTF::create("", "Arial-BoldMT", 30);
    m_labelDayYesRank -> setColor(ccc3(32, 66, 117));
    m_labelDayYesRank -> setPosition(ccp(pPaimingSp->getContentSize().width/2, 110));
    pPaimingSp -> addChild(m_labelDayYesRank);
    m_labelDayRankTitle = CCLabelTTF::create(m_sDayMatchTitle.c_str(), "Arial-BoldMT", 30);
    m_labelDayRankTitle -> setPosition(ccp(pPaimingSp->getContentSize().width/2, 50));
    m_labelDayRankTitle -> setColor(ccc3(211, 18, 18));
    pPaimingSp -> addChild(m_labelDayRankTitle);
    m_pPriseSp = CCSprite::createWithSpriteFrameName("rankprisebg.png");
    m_pPriseSp -> setPosition(ccp(CLIENT_WIDTH-m_pPriseSp->getContentSize().width/2-25, CLIENT_HEIGHT/3+18));
    m_MatchNode -> addChild(m_pPriseSp);
    CCSprite* pPriseTitle = CCSprite::createWithSpriteFrameName("rankpriseinfotitle.png");
    pPriseTitle -> setPosition(ccp(m_pPriseSp->getContentSize().width/2, m_pPriseSp->getContentSize().height-45));
    m_pPriseSp -> addChild(pPriseTitle);
    CCLabelTTF* pLabel1 = CCLabelTTF::create("名次", "Arial-BoldMT", 32);
    pLabel1 -> setPosition(ccp(423/5-30, 244));
    m_pPriseSp -> addChild(pLabel1);
    CCLabelTTF* pLabel2 = CCLabelTTF::create("金币", "Arial-BoldMT", 32);
    pLabel2 -> setPosition(ccp(423/5*2-10, 244));
    m_pPriseSp -> addChild(pLabel2);
    CCLabelTTF* pLabel3 = CCLabelTTF::create("弹头", "Arial-BoldMT", 32);
    pLabel3 -> setPosition(ccp(423/5*3+10, 244));
    m_pPriseSp -> addChild(pLabel3);
    CCLabelTTF* pLabel4 = CCLabelTTF::create("话费券", "Arial-BoldMT", 32);
    pLabel4 -> setPosition(ccp(423/5*4+30, 244));
    m_pPriseSp -> addChild(pLabel4);
    pLabel1 -> setColor(ccc3(32, 66, 117));
    pLabel2 -> setColor(ccc3(32, 66, 117));
    pLabel3 -> setColor(ccc3(32, 66, 117));
    pLabel4 -> setColor(ccc3(32, 66, 117));
    
    
    
    long count = m_vctMatch.size();
    m_scrollMatchView = CCScrollView::create();
    m_containMatchLayer = CCLayer::create();
    m_containMatchLayer -> setContentSize(CCSizeMake(423, 45*count));
    m_containMatchLayer -> setAnchorPoint(CCPointZero);
    m_containMatchLayer -> setPosition(ccp(0,-45*count+200));
    m_scrollMatchView->setPosition(ccp(0, 20));     //根据cocostudio调整的位置
    m_scrollMatchView->setViewSize(CCSizeMake(423, 200));//设置view的大小
    m_scrollMatchView->setContentOffset(ccp(0,0));
    m_scrollMatchView->setContentSize(CCSizeMake(423, 45*count));//设置scrollview区域的大小
    m_scrollMatchView->setContainer(m_containMatchLayer); //设置需要滚动的内容
    m_scrollMatchView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_scrollMatchView->setBounceable(true);
    m_scrollMatchView->setDelegate(this);
    m_pPriseSp->addChild(m_scrollMatchView);
    m_scrollMatchView -> setTouchEnabled(false);
    if(GameMainScene::_instance != NULL)
    {
        m_scrollMatchView -> setTouchEnabled(true);
    }
    m_scrollMatchView -> setTouchPriority(-200);
    for(int i = 0; i < count; i++)
    {
        CCLabelTTF* pLabelLevel = CCLabelTTF::create(m_vctMatch[i].sLevel.c_str(), "Arial-BoldMT", 26);
        pLabelLevel -> setPosition(ccp(m_containMatchLayer->getContentSize().width/5-30, m_containMatchLayer->getContentSize().height-45*i-20));
        m_containMatchLayer -> addChild(pLabelLevel);
        CCLabelTTF* pLabelCoin = CCLabelTTF::create(m_vctMatch[i].sCoin.c_str(), "Arial-BoldMT", 26);
        pLabelCoin -> setPosition(ccp(m_containMatchLayer->getContentSize().width/5*2-10, m_containMatchLayer->getContentSize().height-45*i-20));
        m_containMatchLayer -> addChild(pLabelCoin);
        CCLabelTTF* pLabelZhenzhu = CCLabelTTF::create(m_vctMatch[i].sZhenzhu.c_str(), "Arial-BoldMT", 26);
        pLabelZhenzhu -> setPosition(ccp(m_containMatchLayer->getContentSize().width/5*3+10, m_containMatchLayer->getContentSize().height-45*i-20));
        m_containMatchLayer -> addChild(pLabelZhenzhu);
        CCLabelTTF* pLabelLottery = CCLabelTTF::create(m_vctMatch[i].sLottery.c_str(), "Arial-BoldMT", 26);
        pLabelLottery -> setPosition(ccp(m_containMatchLayer->getContentSize().width/5*4+30, m_containMatchLayer->getContentSize().height-45*i-20));
        m_containMatchLayer -> addChild(pLabelLottery);
        if(i <= 3)
        {
            pLabelLevel -> setColor(DayPriseInfo[i]);
            pLabelCoin -> setColor(DayPriseInfo[i]);
            pLabelLottery -> setColor(DayPriseInfo[i]);
            pLabelZhenzhu -> setColor(DayPriseInfo[i]);
        }
        else
        {
            pLabelLevel->setColor(DayPriseInfo[3]);
            pLabelCoin -> setColor(DayPriseInfo[3]);
            pLabelLottery -> setColor(DayPriseInfo[3]);
            pLabelZhenzhu -> setColor(DayPriseInfo[3]);
        }
        CCSprite* pLine = CCSprite::createWithSpriteFrameName("rankpriseline.png");
        pLine -> setPosition(ccp(m_containMatchLayer->getContentSize().width/2, m_containMatchLayer->getContentSize().height-45*i-40));
        m_containMatchLayer->addChild(pLine);
        if(i == count-1)
        {
            pLine->setVisible(false);
        }
    }
}

void RankLayer::loadUIVip()
{
    m_VipNode = CCNode::create();
    this -> addChild(m_VipNode);
    if(GameMainScene::_instance != NULL)
    {
        m_VipNode->setVisible(false);
    }
    m_lRankVIP = CCLabelTTF::create("", "Arial-BoldMT", 28);
    m_lRankVIP -> setAnchorPoint(ccp(0, 0.5f));
    m_lRankVIP -> setColor(ccc3(253, 196, 57));
    m_lRankVIP -> setPosition(ccp(1070+10, 30));
    m_VipNode -> addChild(m_lRankVIP);
    
    m_spAnimateVIP = CCSprite::create("h2all/loading1.png");
    m_spAnimateVIP -> setPosition(ccp(450, 360));
    m_VipNode -> addChild(m_spAnimateVIP);
    m_spAnimateVIP -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
    
    CCSprite* pPaimingSp = CCSprite::createWithSpriteFrameName("rankpaimingbg.png");
    pPaimingSp -> setPosition(ccp(CLIENT_WIDTH-pPaimingSp->getContentSize().width/2-25, CLIENT_HEIGHT/3*2+45));
    m_VipNode -> addChild(pPaimingSp);
    CCLabelTTF*  pLabelVip1 = CCLabelTTF::create(m_sVip1Title.c_str(), "Arial-BoldMT", 48);
    CCLabelTTF*  pLabelVip2 = CCLabelTTF::create(m_sVip2Title.c_str(), "Arial-BoldMT", 40);
    pLabelVip1->setColor(ccc3(27, 84, 120));
    pLabelVip2->setColor(ccc3(196, 17, 17));
    pLabelVip1->setPosition(ccp(pPaimingSp->getContentSize().width/2, pPaimingSp->getContentSize().height/2+30));
    pLabelVip2->setPosition(ccp(pPaimingSp->getContentSize().width/2, pPaimingSp->getContentSize().height/2-30));
    pPaimingSp->addChild(pLabelVip1);
    pPaimingSp->addChild(pLabelVip2);
    CCSprite* pPriseSp = CCSprite::createWithSpriteFrameName("rankprisebg.png");
    pPriseSp -> setPosition(ccp(CLIENT_WIDTH-pPriseSp->getContentSize().width/2-25, CLIENT_HEIGHT/3+18));
    m_VipNode -> addChild(pPriseSp);
    CCSprite* pPriseTitle = CCSprite::createWithSpriteFrameName("rankvipprisetitle.png");
    pPriseTitle -> setPosition(ccp(pPriseSp->getContentSize().width/2, pPriseSp->getContentSize().height-45));
    pPriseSp -> addChild(pPriseTitle);
    CCLabelTTF* pLabel1 = CCLabelTTF::create("等级", "Arial-BoldMT", 28);
    pLabel1 -> setPosition(ccp(423/3, 244));
    pPriseSp -> addChild(pLabel1);
    CCLabelTTF* pLabel4 = CCLabelTTF::create("充值返利", "Arial-BoldMT", 28);
    pLabel4 -> setPosition(ccp(423/3*2, 244));
    pPriseSp -> addChild(pLabel4);
    pLabel1 -> setColor(ccc3(32, 66, 117));
    pLabel4 -> setColor(ccc3(32, 66, 117));
    
    int count = (int)m_vctVipRank.size();
    float height = 0;
    if (count < 5)
    {
        height = 200;
    }
    else
    {
        height = 200+20+45*(count-5);
    }
    m_scrollVipInfoView = CCScrollView::create();
    CCLayer* pVIpInfoLayer = CCLayer::create();
    pVIpInfoLayer -> setContentSize(CCSizeMake(383, height));//360
    pVIpInfoLayer -> setAnchorPoint(CCPointZero);
    pVIpInfoLayer -> setPosition(ccp(0,200-height));
    m_scrollVipInfoView->setPosition(ccp(20, 20));     //根据cocostudio调整的位置
    m_scrollVipInfoView->setViewSize(CCSizeMake(383, 200));//设置view的大小
    m_scrollVipInfoView->setContentOffset(ccp(0,0));
    m_scrollVipInfoView->setContentSize(CCSizeMake(383, 360));//设置scrollview区域的大小
    m_scrollVipInfoView->setContainer(pVIpInfoLayer); //设置需要滚动的内容
    m_scrollVipInfoView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_scrollVipInfoView->setBounceable(true);
    m_scrollVipInfoView->setDelegate(this);
    pPriseSp->addChild(m_scrollVipInfoView);
    if(GameMainScene::_instance != NULL)
    {
        m_scrollVipInfoView -> setTouchEnabled(false);
    }
    else
    {
        m_scrollVipInfoView -> setTouchEnabled(true);
    }
    m_scrollVipInfoView -> setTouchPriority(-200);

    for(int i = 0; i < count; i++)
    {
        CCLabelTTF* pLabelLevel = CCLabelTTF::create(m_vctVipRank[i].sLevel.c_str(), "Arial-BoldMT", 26);
        pLabelLevel->setColor(ccc3(219, 120, 30));
        pLabelLevel -> setPosition(ccp(pVIpInfoLayer->getContentSize().width/3, pVIpInfoLayer->getContentSize().height-45*i-20));
        pVIpInfoLayer -> addChild(pLabelLevel);
        
        CCLabelTTF* pLabelChargeExtra = CCLabelTTF::create(m_vctVipRank[i].sChargeExtra.c_str(), "Arial-BoldMT", 26);
        pLabelChargeExtra->setColor(ccc3(219, 120, 30));
        pLabelChargeExtra->setAnchorPoint(ccp(0, 0.5));
        pLabelChargeExtra -> setPosition(ccp(pVIpInfoLayer->getContentSize().width/3*2, pVIpInfoLayer->getContentSize().height-45*i-20));
        pVIpInfoLayer -> addChild(pLabelChargeExtra);
    
        CCSprite* pLine = CCSprite::createWithSpriteFrameName("rankpriseline.png");
        pLine -> setPosition(ccp(pVIpInfoLayer->getContentSize().width/2, pVIpInfoLayer->getContentSize().height-45*i-40));
        pVIpInfoLayer->addChild(pLine);
        if(i == count-1)
        {
            pLine->setVisible(false);
        }
    }
 
    
    
    CCSprite* pJifenLabel = CCSprite::createWithSpriteFrameName("rankcurrentvip.png");
    pJifenLabel -> setPosition(ccp(692, 30));
    m_VipNode -> addChild(pJifenLabel);
    char vipTemp[16];
    sprintf(vipTemp, "%d", CMainLogic::sharedMainLogic()->m_nVipLevel);
    m_lVIPJifen = CCLabelTTF::create(vipTemp, "Arial-BoldMT", 28);
    m_lVIPJifen -> setAnchorPoint(ccp(0, 0.5f));
    m_lVIPJifen -> setPosition(ccp(840-101+10, 30));
    m_lVIPJifen -> setColor(ccc3(253, 196, 57));
    m_VipNode -> addChild(m_lVIPJifen);
    //VipScrollView
    m_scrollVipView = CCScrollView::create();
    m_containVipLayer = CCLayer::create();
    m_containVipLayer -> setContentSize(CCSizeMake(800, 8000));
    m_containVipLayer -> setAnchorPoint(CCPointZero);
    m_containVipLayer -> setPosition(ccp(0,-7550));
    m_scrollVipView->setPosition(ccp(14, 90));     //根据cocostudio调整的位置
    m_scrollVipView->setViewSize(CCSizeMake(800, 450));//设置view的大小
    m_scrollVipView->setContentOffset(ccp(0,0));
    m_scrollVipView->setContentSize(CCSizeMake(800, 8000));//设置scrollview区域的大小
    m_scrollVipView->setContainer(m_containVipLayer); //设置需要滚动的内容
    m_scrollVipView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_scrollVipView->setBounceable(true);
    m_scrollVipView->setDelegate(this);
    m_VipNode->addChild(m_scrollVipView);
    m_scrollVipView -> setTouchEnabled(true);
    m_scrollVipView -> setTouchPriority(-200);
    
    for(int i = 0; i < 100; i++)
    {
        CCSprite* pSpCell = NULL;
        CCSprite* pSpBottom = NULL;
        if(i == 0)
        {
            pSpCell = CCSprite::createWithSpriteFrameName("rankfirstcell.png");
            pSpBottom = CCSprite::createWithSpriteFrameName("rankfirst.png");
        }
        else if(i == 1)
        {
            pSpCell = CCSprite::createWithSpriteFrameName("ranksecondcell.png");
            pSpBottom = CCSprite::createWithSpriteFrameName("ranksecond.png");
        }
        else if(i == 2)
        {
            pSpCell = CCSprite::createWithSpriteFrameName("rankthirdcell.png");
            pSpBottom = CCSprite::createWithSpriteFrameName("rankthird.png");
        }
        else
        {
            pSpCell = CCSprite::createWithSpriteFrameName("ranknormalcell.png");
        }
        
        pSpCell -> setPosition(ccp(m_containVipLayer->getContentSize().width/2, m_containVipLayer->getContentSize().height-(i+1)*80+40));
        pSpCell -> setScale(2.0f);
        m_containVipLayer -> addChild(pSpCell);
        m_arrVipCell -> addObject(pSpCell);
        if(i<3)
        {
            pSpBottom -> setPosition(ccp(48, pSpCell->getContentSize().height/2));
            pSpBottom -> setScale(0.5f);
            pSpCell -> addChild(pSpBottom);
        }
        
        if(i >= 3)
        {
            char temp[16];
            sprintf(temp, "%d", i+1);
            CCLabelAtlas* pLabelRank =  CCLabelAtlas::create(temp, "rank/rankvipnum2.png", 32, 38, '0');
            pLabelRank->setScale(0.45f);
            pLabelRank -> setAnchorPoint(ccp(0.5f, 0.5f));
            pLabelRank -> setPosition(ccp(48, pSpCell->getContentSize().height/2));
            pSpCell -> addChild(pLabelRank);
        }
        CCLabelTTF* pLabelName = CCLabelTTF::create("", "Arial-BoldMT", 30);
        pLabelName -> setScale(0.5f);
        pLabelName -> setPosition(ccp(pSpCell->getContentSize().width/2, pSpCell->getContentSize().height/2));
        pLabelName -> setTag(nickName_tag);
        pLabelName -> setColor(ccc3(101, 51, 26));
        pSpCell -> addChild(pLabelName, 2);
        CCLabelAtlas* pLabelJifen = CCLabelAtlas::create("", "rank/rankvipnum.png", 21, 31, '0');
        pLabelJifen -> setScale(0.5f);
        pLabelJifen -> setTag(jinbi_Tag);
        pLabelJifen -> setAnchorPoint(ccp(0,0.5f));
        pLabelJifen -> setPosition(ccp(pSpCell->getContentSize().width/4*3+45, pSpCell->getContentSize().height/2));
        pSpCell -> addChild(pLabelJifen, 2);
    }
    
    sendVipData();
}

void RankLayer::sendVipData()
{
    int nUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    char temp[256];
    sprintf(temp, "http://%s/GetVIPRank.aspx?uid=%d",NORMAL_URL,nUserID);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setTag("http test");
    request -> setResponseCallback(this, httpresponse_selector(RankLayer::onHttpVIPComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void RankLayer::onHttpVIPComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    
    updateVIPData(str);
}

void RankLayer::updateVIPData(std::string str)
{
    //对str进行xml解析，将100条数据保存到m_vctRankTimes中
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("ranklist"))
        {
            return;
        }
        std::string type=rootElement->Attribute("value");
        if(!std::strcmp(type.c_str(), "true"))//相同
        {
            TiXmlElement * pCurrentElement=rootElement->FirstChildElement();
            const char * pValue=pCurrentElement->Value();
            if (!strcmp(pValue,"userInfo"))
            {
                std::string numStr = pCurrentElement->Attribute("num");
                int nNum = atoi(numStr.c_str());
                if(nNum == 1001)//排名千里之外
                {
                    m_lRankVIP -> setString("千里之外");
                }
                else//排名在30名内
                {
                    m_lRankVIP -> setString(numStr.c_str());
                }
            }
            
            pCurrentElement=pCurrentElement->NextSiblingElement();
            while (NULL!=pCurrentElement)
            {
                const char * pValue=pCurrentElement->Value();
                if (!strcmp(pValue,"VIPInfo"))
                {
                    tagRankVIPTimes* RankTimes = new tagRankVIPTimes;
                    m_vctVipRankTimes.push_back(RankTimes);
                    TiXmlElement* firstElement = pCurrentElement -> FirstChildElement();
                    const char * valueId=firstElement->Value();
                    if (!strcmp(valueId,"num"))
                    {
                        int id=atoi(firstElement->GetText());
                        RankTimes->nID = id;
                    }
                    TiXmlElement* secondElement = firstElement -> NextSiblingElement();
                    const char * nickname=secondElement->Value();
                    if (!strcmp(nickname,"nickname"))
                    {
                        RankTimes->sNickName = secondElement->GetText();
                    }
                    TiXmlElement* ThirdElement = secondElement -> NextSiblingElement();
                    const char * sLevel = ThirdElement->Value();
                    if (!strcmp(sLevel,"vipLevel"))
                    {
                        int slevel=atoi(ThirdElement->GetText());
                        RankTimes->nLevel = slevel;
                    }
                    TiXmlElement* FourElement = ThirdElement -> NextSiblingElement();
                    const char * vipExp=FourElement->Value();
                    if (!strcmp(vipExp,"vipExp"))
                    {
                        SCORE vipExp=atoll(FourElement->GetText());
                        RankTimes->llVipExp = vipExp;
                    }
                }
                pCurrentElement=pCurrentElement->NextSiblingElement();
            }
            
        }
        else//没有数据
        {
            m_scrollVipView->setVisible(false);
            m_scrollVipView->setTouchEnabled(false);
            CCLabelTTF* pLabelTTF = CCLabelTTF::create("暂无数据", "Arial", 64);
            pLabelTTF -> setPosition(ccp(360,360));
            m_VipNode -> addChild(pLabelTTF);
        }
    }
    delete document;
    
    m_spAnimateVIP->stopAllActions();
    m_spAnimateVIP->removeFromParent();
    
    //更新控件显示
    for(unsigned int i = 0; i < m_vctVipRankTimes.size(); i++)
    {
        CCSprite* pSpCell = (CCSprite*)m_arrVipCell->objectAtIndex(i);
        if(pSpCell != NULL)
        {
            CCLabelTTF* pLabelName = (CCLabelTTF*)pSpCell->getChildByTag(nickName_tag);
            if(pLabelName != NULL)
            {
                pLabelName -> setString(m_vctVipRankTimes[i]->sNickName.c_str());
                if(m_vctVipRankTimes[i]->nLevel >= 1)
                {
                    pLabelName -> setColor(ccc3(255, 0, 0));
                }
            }
            CCLabelAtlas* pLabelJinbi = (CCLabelAtlas*)pSpCell->getChildByTag(jinbi_Tag);
            if(pLabelJinbi != NULL)
            {
                char temp[32];
                sprintf(temp, "%d", m_vctVipRankTimes[i]->nLevel);
                pLabelJinbi -> setString(temp);
                CCSprite* pVipLogo = CCSprite::createWithSpriteFrameName("rankvipsprite.png");
                pVipLogo -> setPosition(ccp(-pVipLogo->getContentSize().width+10, pLabelJinbi->getContentSize().height/2));
                pLabelJinbi->addChild(pVipLogo);
            }
        }
    }
}

void RankLayer::loadUIMoney()
{
    m_MoneyNode = CCNode::create();
    this -> addChild(m_MoneyNode);
    m_MoneyNode -> setVisible(false);
    m_lRankMoney = CCLabelTTF::create("", "Arial-BoldMT", 28);
    m_lRankMoney -> setAnchorPoint(ccp(0, 0.5f));
    m_lRankMoney -> setColor(ccc3(253, 196, 57));
    m_lRankMoney -> setPosition(ccp(1070+10, 30));
    m_MoneyNode -> addChild(m_lRankMoney);
    
    CCSprite* pPaimingSp = CCSprite::createWithSpriteFrameName("rankpaimingbg.png");
    pPaimingSp -> setPosition(ccp(CLIENT_WIDTH-pPaimingSp->getContentSize().width/2-25, CLIENT_HEIGHT/3*2+45));
    m_MoneyNode -> addChild(pPaimingSp);
    CCLabelTTF*  pLabelMoney1 = CCLabelTTF::create(m_sMoney1Title.c_str(), "Arial-BoldMT", 48);
    CCLabelTTF*  pLabelMoney2 = CCLabelTTF::create(m_sMoney2Title.c_str(), "Arial-BoldMT", 40);
    pLabelMoney1->setColor(ccc3(27, 84, 120));
    pLabelMoney2->setColor(ccc3(200, 17, 17));
    pLabelMoney1->setPosition(ccp(pPaimingSp->getContentSize().width/2, pPaimingSp->getContentSize().height/2+30));
    pLabelMoney2->setPosition(ccp(pPaimingSp->getContentSize().width/2, pPaimingSp->getContentSize().height/2-30));
    pPaimingSp->addChild(pLabelMoney1);
    pPaimingSp->addChild(pLabelMoney2);
    CCSprite* pPriseSp = CCSprite::createWithSpriteFrameName("rankprisebg.png");
    pPriseSp -> setPosition(ccp(CLIENT_WIDTH-pPriseSp->getContentSize().width/2-25, CLIENT_HEIGHT/3+18));
    m_MoneyNode -> addChild(pPriseSp);
    CCSprite* pPriseTitle = CCSprite::createWithSpriteFrameName("rankmoneyprisetitle.png");
    pPriseTitle -> setPosition(ccp(pPriseSp->getContentSize().width/2, pPriseSp->getContentSize().height-45));
    pPriseSp -> addChild(pPriseTitle);
    CCSprite* pPriseTitleInfo = CCSprite::createWithSpriteFrameName("rankmoneypriseinfo2.png");
    pPriseTitleInfo -> setPosition(ccp(pPriseSp->getContentSize().width/2, pPriseSp->getContentSize().height-120));
    pPriseSp -> addChild(pPriseTitleInfo);
    
    int count = (int)m_vctMoneyRank.size();
    float height = 0;
    if (count < 5)
    {
        height = 171;
    }
    else
    {
        height = 171+19+38*(count-5);
    }
    m_scrollMoneyInfoView = CCScrollView::create();
    CCLayer* pMoneyInfoLayer = CCLayer::create();
    pMoneyInfoLayer -> setContentSize(CCSizeMake(334, height));
    pMoneyInfoLayer -> setAnchorPoint(CCPointZero);
    pMoneyInfoLayer -> setPosition(ccp(0,171-height));
    m_scrollMoneyInfoView->setPosition(ccp(40, 20));     //根据cocostudio调整的位置
    m_scrollMoneyInfoView->setViewSize(CCSizeMake(334, 171));//设置view的大小
    m_scrollMoneyInfoView->setContentOffset(ccp(0,0));
    m_scrollMoneyInfoView->setContentSize(CCSizeMake(334, 234));//设置scrollview区域的大小
    m_scrollMoneyInfoView->setContainer(pMoneyInfoLayer); //设置需要滚动的内容
    m_scrollMoneyInfoView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_scrollMoneyInfoView->setBounceable(true);
    m_scrollMoneyInfoView->setDelegate(this);
    pPriseSp->addChild(m_scrollMoneyInfoView);
    m_scrollMoneyInfoView -> setTouchEnabled(false);
    m_scrollMoneyInfoView -> setTouchPriority(-200);
    
    for (int i=0; i<count; i++) {
        CCLabelTTF* pLabelCharge = CCLabelTTF::create(m_vctMoneyRank[i].sCharge.c_str(), "Arial-BoldMT", 26);
        pLabelCharge->setAnchorPoint(ccp(0, 0.5));
        pLabelCharge->setColor(ccc3(205, 27, 32));
        pLabelCharge -> setPosition(ccp(10, pMoneyInfoLayer->getContentSize().height-38*i-20));
        pMoneyInfoLayer -> addChild(pLabelCharge);
        CCLabelTTF* pLabelExtra = CCLabelTTF::create(m_vctMoneyRank[i].sChargeExtra.c_str(), "Arial-BoldMT", 26);
        pLabelExtra->setAnchorPoint(ccp(1, 0.5));
        pLabelExtra->setColor(ccc3(205, 27, 32));
        pLabelExtra -> setPosition(ccp(pMoneyInfoLayer->getContentSize().width-10, pMoneyInfoLayer->getContentSize().height-38*i-20));
        pMoneyInfoLayer -> addChild(pLabelExtra);
    }
    
    CCSprite* pJifenLabel = CCSprite::createWithSpriteFrameName("rankcurrentcoin.png");
    pJifenLabel -> setPosition(ccp(682, 30));
    m_MoneyNode -> addChild(pJifenLabel);
    char scoreTemp[16];
    sprintf(scoreTemp, "%lld", CMainLogic::sharedMainLogic()->m_lUserScore);
    m_lMoneyJifen = CCLabelTTF::create(scoreTemp, "Arial-BoldMT", 28);
    m_lMoneyJifen -> setAnchorPoint(ccp(0, 0.5f));
    m_lMoneyJifen -> setPosition(ccp(840-101+10, 30));
    m_lMoneyJifen -> setColor(ccc3(253, 196, 57));
    m_MoneyNode -> addChild(m_lMoneyJifen);
    m_scrollMoneyView = CCScrollView::create();
    m_containMoneyLayer = CCLayer::create();
    m_containMoneyLayer -> setContentSize(CCSizeMake(800, 8000));
    m_containMoneyLayer -> setAnchorPoint(CCPointZero);
    m_containMoneyLayer -> setPosition(ccp(0,-7550));
    m_scrollMoneyView->setPosition(ccp(14,90));     //根据cocostudio调整的位置
    m_scrollMoneyView->setViewSize(CCSizeMake(800, 450));//设置view的大小
    m_scrollMoneyView->setContentOffset(ccp(0,0));
    m_scrollMoneyView->setContentSize(CCSizeMake(800, 8000));//设置scrollview区域的大小
    m_scrollMoneyView->setContainer(m_containMoneyLayer); //设置需要滚动的内容
    m_scrollMoneyView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_scrollMoneyView->setBounceable(true);
    m_scrollMoneyView->setDelegate(this);
    m_MoneyNode->addChild(m_scrollMoneyView);
    m_scrollMoneyView -> setTouchEnabled(false);
    m_scrollMoneyView -> setTouchPriority(-200);
    
    m_spAnimateMoney = CCSprite::create("h2all/loading1.png");
    m_spAnimateMoney -> setPosition(ccp(450, 360));
    m_MoneyNode -> addChild(m_spAnimateMoney);
    m_spAnimateMoney -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
    
    for(int i = 0; i < 100; i++)
    {
        CCSprite* pSpCell = NULL;
        CCSprite* pSpBottom = NULL;
        if(i == 0)
        {
            pSpCell = CCSprite::createWithSpriteFrameName("rankfirstcell.png");
            pSpBottom = CCSprite::createWithSpriteFrameName("rankfirst.png");
        }
        else if(i == 1)
        {
            pSpCell = CCSprite::createWithSpriteFrameName("ranksecondcell.png");
            pSpBottom = CCSprite::createWithSpriteFrameName("ranksecond.png");
        }
        else if(i == 2)
        {
            pSpCell = CCSprite::createWithSpriteFrameName("rankthirdcell.png");
            pSpBottom = CCSprite::createWithSpriteFrameName("rankthird.png");
        }
        else
        {
            pSpCell = CCSprite::createWithSpriteFrameName("ranknormalcell.png");
        }
        
        pSpCell -> setPosition(ccp(m_containMoneyLayer->getContentSize().width/2, m_containMoneyLayer->getContentSize().height-(i+1)*80+40));
        pSpCell -> setScale(2.0f);
        m_containMoneyLayer -> addChild(pSpCell);
        m_arrMoneyCell -> addObject(pSpCell);
        if(i<3)
        {
            pSpBottom -> setPosition(ccp(48, pSpCell->getContentSize().height/2));
            pSpBottom -> setScale(0.5f);
            pSpCell -> addChild(pSpBottom);
        }
        
        if(i >= 3)
        {
            char temp[16];
            sprintf(temp, "%d", i+1);
            CCLabelAtlas* pLabelRank =  CCLabelAtlas::create(temp, "rank/rankvipnum2.png", 32, 38, '0');
            pLabelRank->setScale(0.45f);
            pLabelRank -> setAnchorPoint(ccp(0.5f, 0.5f));
            pLabelRank -> setPosition(ccp(48, pSpCell->getContentSize().height/2));
            pSpCell -> addChild(pLabelRank);
        }
        CCLabelTTF* pLabelName = CCLabelTTF::create("", "Arial-BoldMT", 30);
        pLabelName -> setScale(0.5f);
        pLabelName -> setPosition(ccp(pSpCell->getContentSize().width/2, pSpCell->getContentSize().height/2));
        pLabelName -> setTag(nickName_tag);
        pLabelName -> setColor(ccc3(101, 51, 26));
        pSpCell -> addChild(pLabelName, 2);
        CCLabelTTF* pLabelJifen = CCLabelTTF::create("", "Arial-BoldMT", 32);
        pLabelJifen -> setScale(0.5f);
        pLabelJifen -> setTag(jinbi_Tag);
        pLabelJifen -> setPosition(ccp(pSpCell->getContentSize().width/4*3+45, pSpCell->getContentSize().height/2));
        pLabelJifen -> setColor(ccc3(101, 51, 26));
        pSpCell -> addChild(pLabelJifen, 2);
    }
    
    sendMoneyData();
}

void RankLayer::sendMoneyData()
{
    int nUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    char temp[256];
    sprintf(temp, "http://%s/GetScoreRank.aspx?uid=%d",NORMAL_URL,nUserID);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setTag("http test");
    request -> setResponseCallback(this, httpresponse_selector(RankLayer::onHttpMoneyComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void RankLayer::onHttpMoneyComplete(CCHttpClient* sender, CCHttpResponse* response)
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
        if (0!=nodename.compare("ranklist"))
        {
            return;
        }
        std::string type=rootElement->Attribute("value");
        if(!std::strcmp(type.c_str(), "true"))//相同
        {
            TiXmlElement * pCurrentElement=rootElement->FirstChildElement();
            const char * pValue=pCurrentElement->Value();
            if (!strcmp(pValue,"userInfo"))
            {
                std::string numStr = pCurrentElement->Attribute("num");
                int nNum = atoi(numStr.c_str());
                if(nNum == 1001)//排名千里之外
                {
                    m_lRankMoney-> setString("千里之外");
                }
                else//排名在30名内
                {
                    m_lRankMoney-> setString(numStr.c_str());
                }
            }
            
            pCurrentElement=pCurrentElement->NextSiblingElement();
            while (NULL!=pCurrentElement)
            {
                const char * pValue=pCurrentElement->Value();
                if (!strcmp(pValue,"ScoreInfo"))
                {
                    tagRankTimes* RankTimes = new tagRankTimes;
                    m_vctMoneyRankTimes.push_back(RankTimes);
                    TiXmlElement* firstElement = pCurrentElement -> FirstChildElement();
                    const char * valueId=firstElement->Value();
                    if (!strcmp(valueId,"num"))
                    {
                        int id=atoi(firstElement->GetText());
                        RankTimes->nID = id;
                    }
                    TiXmlElement* secondElement = firstElement -> NextSiblingElement();
                    const char * nickname=secondElement->Value();
                    if (!strcmp(nickname,"nickname"))
                    {
                        RankTimes->sNickName = secondElement->GetText();
                    }
                    TiXmlElement* thirdElement = secondElement -> NextSiblingElement();
                    const char * score=thirdElement->Value();
                    if (!strcmp(score,"count"))
                    {
                        SCORE score=atoll(thirdElement->GetText());
                        RankTimes->llScore = score;
                    }
                    TiXmlElement* fourElement = thirdElement -> NextSiblingElement();
                    const char * level=fourElement->Value();
                    if (!strcmp(level,"viplevel"))
                    {
                        int nLevel = atoi(fourElement->GetText());
                        RankTimes->nLevel = nLevel;
                    }
                }
                pCurrentElement=pCurrentElement->NextSiblingElement();
            }
            
        }
        else//没有数据
        {
            m_scrollMoneyView->setVisible(false);
            m_scrollMoneyView->setTouchEnabled(false);
            CCLabelTTF* pLabelTTF = CCLabelTTF::create("暂无数据", "Arial", 64);
            pLabelTTF -> setPosition(ccp(360,360));
            m_MoneyNode -> addChild(pLabelTTF);
        }
    }
    delete document;
    
    m_spAnimateMoney->stopAllActions();
    m_spAnimateMoney->removeFromParent();
    
    //更新控件显示
    for(unsigned int i = 0; i < m_vctMoneyRankTimes.size(); i++)
    {
        CCSprite* pSpCell = (CCSprite*)m_arrMoneyCell->objectAtIndex(i);
        if(pSpCell != NULL)
        {
            CCLabelTTF* pLabelName = (CCLabelTTF*)pSpCell->getChildByTag(nickName_tag);
            if(pLabelName != NULL)
            {
                pLabelName -> setString(m_vctMoneyRankTimes[i]->sNickName.c_str());
                if(m_vctMoneyRankTimes[i]->nLevel >= 1)
                {
                    pLabelName -> setColor(ccc3(255, 0, 0));
                }
            }
            CCLabelTTF* pLabelJinbi = (CCLabelTTF*)pSpCell->getChildByTag(jinbi_Tag);
            if(pLabelJinbi != NULL)
            {
                char temp[32];
                sprintf(temp, "%lld", m_vctMoneyRankTimes[i]->llScore);
                pLabelJinbi -> setString(temp);
            }
        }
    }
}

void RankLayer::loadUIDayRank()
{
    m_DayNode = CCNode::create();
    this -> addChild(m_DayNode);
    if(GameMainScene::_instance == NULL)
    {
        m_DayNode -> setVisible(false);
    }
    m_lRankDay = CCLabelTTF::create("", "Arial-BoldMT", 28);
    m_lRankDay -> setAnchorPoint(ccp(0,0.5f));
    m_lRankDay -> setPosition(ccp(1070+10, 30));
    m_lRankDay -> setColor(ccc3(253, 196, 57));
    m_DayNode -> addChild(m_lRankDay);
    CCSprite* pJifenLabel = CCSprite::createWithSpriteFrameName("rankjifenlabel.png");
    pJifenLabel -> setPosition(ccp(682, 30));
    m_DayNode -> addChild(pJifenLabel);
    m_lDayJifen = CCLabelTTF::create("", "Arial-BoldMT", 28);
    m_lDayJifen -> setAnchorPoint(ccp(0, 0.5f));
    m_lDayJifen -> setPosition(ccp(840-101+10, 30));
    m_lDayJifen -> setColor(ccc3(253, 196, 57));
    m_DayNode -> addChild(m_lDayJifen);
    
    //DayScrollView
    m_scrollDayView = CCScrollView::create();
    m_containDayLayer = CCLayer::create();
    m_containDayLayer -> setContentSize(CCSizeMake(800, 8000));
    m_containDayLayer -> setAnchorPoint(CCPointZero);
    m_containDayLayer -> setPosition(ccp(0,-7550));
    m_scrollDayView->setPosition(ccp(14,90));     //根据cocostudio调整的位置
    m_scrollDayView->setViewSize(CCSizeMake(800, 450));//设置view的大小
    m_scrollDayView->setContentOffset(ccp(0,0));
    m_scrollDayView->setContentSize(CCSizeMake(800, 8000));//设置scrollview区域的大小
    m_scrollDayView->setContainer(m_containDayLayer); //设置需要滚动的内容
    m_scrollDayView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_scrollDayView->setBounceable(true);
    m_scrollDayView->setDelegate(this);
    m_DayNode->addChild(m_scrollDayView);
    if(GameMainScene::_instance != NULL)
    {
        m_scrollDayView -> setTouchEnabled(true);
    }
    else
    {
        m_scrollDayView -> setTouchEnabled(false);
    }
    
    m_scrollDayView -> setTouchPriority(-200);
    
    m_spAnimateDay = CCSprite::create("h2all/loading1.png");
    m_spAnimateDay -> setPosition(ccp(450, 360));
    m_DayNode -> addChild(m_spAnimateDay);
    m_spAnimateDay -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
    
    for(int i = 0; i < 100; i++)//
    {
        CCSprite* pSpCell = NULL;
        CCSprite* pSpBottom = NULL;
        if(i == 0)
        {
            pSpCell = CCSprite::createWithSpriteFrameName("rankfirstcell.png");
            pSpBottom = CCSprite::createWithSpriteFrameName("rankfirst.png");
        }
        else if(i == 1)
        {
            pSpCell = CCSprite::createWithSpriteFrameName("ranksecondcell.png");
            pSpBottom = CCSprite::createWithSpriteFrameName("ranksecond.png");
        }
        else if(i == 2)
        {
            pSpCell = CCSprite::createWithSpriteFrameName("rankthirdcell.png");
            pSpBottom = CCSprite::createWithSpriteFrameName("rankthird.png");
        }
        else
        {
            pSpCell = CCSprite::createWithSpriteFrameName("ranknormalcell.png");
        }
        
        pSpCell -> setPosition(ccp(m_containDayLayer->getContentSize().width/2, m_containDayLayer->getContentSize().height-(i+1)*80+40));
        pSpCell -> setScale(2.0f);
        m_containDayLayer -> addChild(pSpCell);
        m_arrDayCell -> addObject(pSpCell);
        if(i<3)
        {
            pSpBottom -> setPosition(ccp(48, pSpCell->getContentSize().height/2));
            pSpBottom -> setScale(0.5f);
            pSpCell -> addChild(pSpBottom);
        }
        
        if(i >= 3)
        {
            char temp[16];
            sprintf(temp, "%d", i+1);
            CCLabelAtlas* pLabelRank =  CCLabelAtlas::create(temp, "rank/rankvipnum2.png", 32, 38, '0');
            pLabelRank->setScale(0.45f);
            pLabelRank -> setAnchorPoint(ccp(0.5f, 0.5f));
            pLabelRank -> setPosition(ccp(48, pSpCell->getContentSize().height/2));
            pSpCell -> addChild(pLabelRank);
        }
        CCLabelTTF* pLabelName = CCLabelTTF::create("", "Arial-BoldMT", 30);
        pLabelName -> setScale(0.5f);
        pLabelName -> setPosition(ccp(pSpCell->getContentSize().width/2, pSpCell->getContentSize().height/2));
        pLabelName -> setTag(nickName_tag);
        pLabelName -> setColor(ccc3(101, 51, 26));
        pSpCell -> addChild(pLabelName, 2);
        CCLabelTTF* pLabelJifen = CCLabelTTF::create("", "Arial-BoldMT", 32);
        pLabelJifen -> setScale(0.5f);
        pLabelJifen -> setTag(jinbi_Tag);
        pLabelJifen -> setPosition(ccp(pSpCell->getContentSize().width/4*3+45, pSpCell->getContentSize().height/2));
        pLabelJifen -> setColor(ccc3(101, 51, 26));
        pSpCell -> addChild(pLabelJifen, 2);
    }
    if(GameMainScene::_instance == NULL)
    {
        sendDayData();
    }
}

void RankLayer::sendDayData()
{
    int nUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    char temp[256];
    sprintf(temp, "http://%s/getranklist.aspx?uid=%d",NORMAL_URL,nUserID);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(RankLayer::onHttpDayComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void RankLayer::onHttpDayComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    
    updateDayData(str);
}

void RankLayer::updateDayData(std::string str)
{
    //对str进行xml解析，将30条数据保存到m_vctRankTimes中
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("ranklist"))
        {
            return;
        }
        std::string type=rootElement->Attribute("value");
        if(!std::strcmp(type.c_str(), "true"))//相同
        {
            TiXmlElement * pCurrentElement=rootElement->FirstChildElement();
            const char * pValue=pCurrentElement->Value();
            if (!strcmp(pValue,"userInfo"))
            {
                std::string numStr = pCurrentElement->Attribute("num");
                int nNum = atoi(numStr.c_str());
                if(nNum == 1001)//排名千里之外
                {
                    m_lRankDay -> setString("千里之外");
                }
                else//排名在30名内
                {
                    m_lRankDay -> setString(numStr.c_str());
                }
                std::string yesNum = pCurrentElement->Attribute("lastnum");
                int nYesNum = atoi(yesNum.c_str());
                if(nYesNum == 1001)
                {
                    char temp[64];
                    sprintf(temp, "您昨日的排名在千里之外");
                    m_labelDayYesRank->setString(temp);
                }
                else
                {
                    char temp[64];
                    sprintf(temp, "您昨日的排名为:%d", nYesNum);
                    m_labelDayYesRank->setString(temp);
                }
                std::string sCount = pCurrentElement->Attribute("count");
                m_lDayJifen -> setString(sCount.c_str());
            }
            
            pCurrentElement=pCurrentElement->NextSiblingElement();
            while (NULL!=pCurrentElement)
            {
                const char * pValue=pCurrentElement->Value();
                if (!strcmp(pValue,"accountsInfo"))
                {
                    tagRankTimes* RankTimes = new tagRankTimes;
                    m_vctDayRankTimes.push_back(RankTimes);
                    TiXmlElement* firstElement = pCurrentElement -> FirstChildElement();
                    const char * valueId=firstElement->Value();
                    if (!strcmp(valueId,"num"))
                    {
                        int id=atoi(firstElement->GetText());
                        RankTimes->nID = id;
                    }
                    TiXmlElement* secondElement = firstElement -> NextSiblingElement();
                    const char * nickname=secondElement->Value();
                    if (!strcmp(nickname,"nickname"))
                    {
                        RankTimes->sNickName = secondElement->GetText();
                    }
                    TiXmlElement* thirdElement = secondElement -> NextSiblingElement();
                    const char * score=thirdElement->Value();
                    if (!strcmp(score,"count"))
                    {
                        SCORE score=atoll(thirdElement->GetText());
                        RankTimes->llScore = score;
                    }
                    TiXmlElement* fourElement = thirdElement -> NextSiblingElement();
                    const char* level = fourElement->Value();
                    if(!strcmp(level, "viplevel"))
                    {
                        int nLevel = atoi(fourElement->GetText());
                        RankTimes->nLevel = nLevel;
                    }
                }
                pCurrentElement=pCurrentElement->NextSiblingElement();
            }
            
        }
        else//没有数据
        {
            m_scrollDayView->setVisible(false);
            m_scrollDayView->setTouchEnabled(false);
            CCLabelTTF* pLabelTTF = CCLabelTTF::create("暂无比赛结果", "Arial", 64);
            pLabelTTF -> setPosition(ccp(360,360));
            m_DayNode -> addChild(pLabelTTF);
        }
    }
    delete document;
    
    m_spAnimateDay->stopAllActions();
    m_spAnimateDay->removeFromParent();
    
    //更新控件显示
    for(unsigned int i = 0; i < m_vctDayRankTimes.size(); i++)
    {
        CCSprite* pSpCell = (CCSprite*)m_arrDayCell->objectAtIndex(i);
        if(pSpCell != NULL)
        {
            CCLabelTTF* pLabelName = (CCLabelTTF*)pSpCell->getChildByTag(nickName_tag);
            if(pLabelName != NULL)
            {
                pLabelName -> setString(m_vctDayRankTimes[i]->sNickName.c_str());
                if(m_vctDayRankTimes[i]->nLevel >= 1)
                {
                    pLabelName -> setColor(ccc3(255, 0, 0));
                }
            }
            CCLabelTTF* pLabelJinbi = (CCLabelTTF*)pSpCell->getChildByTag(jinbi_Tag);
            if(pLabelJinbi != NULL)
            {
                char temp[32];
                sprintf(temp, "%lld", m_vctDayRankTimes[i]->llScore);
                pLabelJinbi -> setString(temp);
            }
        }
    }
}

void RankLayer::loadUIWeekRank()
{
    m_WeekNode = CCNode::create();
    this -> addChild(m_WeekNode);
    m_WeekNode -> setVisible(false);
    m_lRankWeek = CCLabelTTF::create("", "Arial-BoldMT", 28);
    m_lRankWeek -> setAnchorPoint(ccp(0,0.5f));
    m_lRankWeek -> setPosition(ccp(1070+10, 30));
    m_lRankWeek -> setColor(ccc3(253, 196, 57));
    m_WeekNode -> addChild(m_lRankWeek);
    CCSprite* pJifenLabel = CCSprite::createWithSpriteFrameName("rankjifenlabel.png");
    pJifenLabel -> setPosition(ccp(682, 30));
    m_WeekNode -> addChild(pJifenLabel);
    m_lWeekJifen = CCLabelTTF::create("", "Arial-BoldMT", 28);
    m_lWeekJifen -> setAnchorPoint(ccp(0, 0.5f));
    m_lWeekJifen -> setPosition(ccp(840-101+10, 30));
    m_lWeekJifen -> setColor(ccc3(253, 196, 57));
    m_WeekNode -> addChild(m_lWeekJifen);
    
    CCSprite* pPaimingSp = CCSprite::createWithSpriteFrameName("rankpaimingbg.png");
    pPaimingSp -> setPosition(ccp(CLIENT_WIDTH-pPaimingSp->getContentSize().width/2-25, CLIENT_HEIGHT/3*2+45));
    m_WeekNode -> addChild(pPaimingSp);
    m_labelWeekYesRank = CCLabelTTF::create("", "Arial-BoldMT", 30);
    m_labelWeekYesRank -> setColor(ccc3(32, 66, 117));
    m_labelWeekYesRank -> setPosition(ccp(pPaimingSp->getContentSize().width/2, 110));
    pPaimingSp -> addChild(m_labelWeekYesRank);
    m_labelWeekRankTitle = CCLabelTTF::create(m_sWeekMatchTitle.c_str(), "Arial-BoldMT", 30);
    m_labelWeekRankTitle -> setPosition(ccp(pPaimingSp->getContentSize().width/2, 50));
    m_labelWeekRankTitle -> setColor(ccc3(211, 18, 18));
    pPaimingSp -> addChild(m_labelWeekRankTitle);
    CCSprite* pPriseSp = CCSprite::createWithSpriteFrameName("rankprisebg.png");
    pPriseSp -> setPosition(ccp(CLIENT_WIDTH-pPriseSp->getContentSize().width/2-25, CLIENT_HEIGHT/3+18));
    m_WeekNode -> addChild(pPriseSp);
    CCSprite* pPriseTitle = CCSprite::createWithSpriteFrameName("rankpriseinfotitle.png");
    pPriseTitle -> setPosition(ccp(pPriseSp->getContentSize().width/2, pPriseSp->getContentSize().height-45));
    pPriseSp -> addChild(pPriseTitle);
    CCLabelTTF* pLabel1 = CCLabelTTF::create("名次", "Arial-BoldMT", 32);
    pLabel1 -> setPosition(ccp(423/4-30, 244));
    pPriseSp -> addChild(pLabel1);
    CCLabelTTF* pLabel2 = CCLabelTTF::create("奖品", "Arial-BoldMT", 32);
    pLabel2 -> setPosition(ccp(pPriseSp->getContentSize().width/2+50, 244));
    pPriseSp -> addChild(pLabel2);
    pLabel1 -> setColor(ccc3(32, 66, 117));
    pLabel2 -> setColor(ccc3(32, 66, 117));
    
    int count = (int)m_vctWeekMatch.size();
    float height = 0;
    if (count < 5)
    {
        height = 200;
    }
    else
    {
        height = 200+20+45*(count-5);
    }
    m_scrollWeekInfoView = CCScrollView::create();
    CCLayer* pWeekMatchInfoLayer = CCLayer::create();
    pWeekMatchInfoLayer -> setContentSize(CCSizeMake(350, height));
    pWeekMatchInfoLayer -> setAnchorPoint(CCPointZero);
    pWeekMatchInfoLayer -> setPosition(ccp(0,200-height));
    m_scrollWeekInfoView->setPosition(ccp(40, 20));     //根据cocostudio调整的位置
    m_scrollWeekInfoView->setViewSize(CCSizeMake(350, 200));//设置view的大小
    m_scrollWeekInfoView->setContentOffset(ccp(0,0));
    m_scrollWeekInfoView->setContentSize(CCSizeMake(350, 200));//设置scrollview区域的大小
    m_scrollWeekInfoView->setContainer(pWeekMatchInfoLayer); //设置需要滚动的内容
    m_scrollWeekInfoView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_scrollWeekInfoView->setBounceable(true);
    m_scrollWeekInfoView->setDelegate(this);
    pPriseSp->addChild(m_scrollWeekInfoView);
    if (count < 5)
    {
        m_scrollWeekInfoView -> setTouchEnabled(false);
    }
    else
    {
        m_scrollWeekInfoView -> setTouchEnabled(true);
    }
    m_scrollWeekInfoView -> setTouchPriority(-200);
    
    for (int i=0; i<count; i++) {
        CCLabelTTF* pLabelLevel = CCLabelTTF::create(m_vctWeekMatch[i%4].sLevel.c_str(), "Arial-BoldMT", 28);
        pLabelLevel -> setPosition(ccp(pWeekMatchInfoLayer->getContentSize().width/5-30, pWeekMatchInfoLayer->getContentSize().height-45*i-20));
        pWeekMatchInfoLayer -> addChild(pLabelLevel);
        
        CCLabelTTF* pLabelBarrel = CCLabelTTF::create(m_vctWeekMatch[i%4].sPrize.c_str(), "Arial-BoldMT", 28);
        pLabelBarrel -> setPosition(ccp(pWeekMatchInfoLayer->getContentSize().width/3*2-7, pWeekMatchInfoLayer->getContentSize().height-45*i-20));
        pWeekMatchInfoLayer -> addChild(pLabelBarrel);
        
        if (i==0)
        {
            pLabelLevel -> setColor(ccc3(255, 0, 0));
            pLabelBarrel -> setColor(ccc3(255, 0, 0));
        }
        else if (i==1)
        {
            pLabelLevel -> setColor(ccc3(255, 0, 255));
            pLabelBarrel -> setColor(ccc3(255, 0, 255));
        }
        else if (i==1)
        {
            pLabelLevel -> setColor(ccc3(0, 0, 255));
            pLabelBarrel -> setColor(ccc3(0, 0, 255));
        }
        else
        {
            pLabelLevel -> setColor(ccc3(32, 66, 117));
            pLabelBarrel -> setColor(ccc3(32, 66, 117));
        }
    }
    
    //WeekScrollView
    m_scrollWeekView = CCScrollView::create();
    m_containWeekLayer = CCLayer::create();
    m_containWeekLayer -> setContentSize(CCSizeMake(800, 8000));
    m_containWeekLayer -> setAnchorPoint(CCPointZero);
    m_containWeekLayer -> setPosition(ccp(0,-7550));
    m_scrollWeekView->setPosition(ccp(14,90));     //根据cocostudio调整的位置
    m_scrollWeekView->setViewSize(CCSizeMake(800, 450));//设置view的大小
    m_scrollWeekView->setContentOffset(ccp(0,0));
    m_scrollWeekView->setContentSize(CCSizeMake(800, 8000));//设置scrollview区域的大小
    m_scrollWeekView->setContainer(m_containWeekLayer); //设置需要滚动的内容
    m_scrollWeekView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_scrollWeekView->setBounceable(true);
    m_scrollWeekView->setDelegate(this);
    m_WeekNode->addChild(m_scrollWeekView);
    m_scrollWeekView -> setTouchEnabled(false);
    m_scrollWeekView -> setTouchPriority(-200);
    
    m_spAnimateWeek = CCSprite::create("h2all/loading1.png");
    m_spAnimateWeek -> setPosition(ccp(450, 360));
    m_WeekNode -> addChild(m_spAnimateWeek);
    m_spAnimateWeek -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
    
    for(int i = 0; i < 100; i++)
    {
        CCSprite* pSpCell = NULL;
        CCSprite* pSpBottom = NULL;
        if(i == 0)
        {
            pSpCell = CCSprite::createWithSpriteFrameName("rankfirstcell.png");
            pSpBottom = CCSprite::createWithSpriteFrameName("rankfirst.png");
        }
        else if(i == 1)
        {
            pSpCell = CCSprite::createWithSpriteFrameName("ranksecondcell.png");
            pSpBottom = CCSprite::createWithSpriteFrameName("ranksecond.png");
        }
        else if(i == 2)
        {
            pSpCell = CCSprite::createWithSpriteFrameName("rankthirdcell.png");
            pSpBottom = CCSprite::createWithSpriteFrameName("rankthird.png");
        }
        else
        {
            pSpCell = CCSprite::createWithSpriteFrameName("ranknormalcell.png");
        }
        
        pSpCell -> setPosition(ccp(m_containWeekLayer->getContentSize().width/2, m_containWeekLayer->getContentSize().height-(i+1)*80+40));
        pSpCell -> setScale(2.0f);
        m_containWeekLayer -> addChild(pSpCell);
        m_arrWeekCell -> addObject(pSpCell);
        if(i<3)
        {
            pSpBottom -> setPosition(ccp(48, pSpCell->getContentSize().height/2));
            pSpBottom -> setScale(0.5f);
            pSpCell -> addChild(pSpBottom);
        }
        
        if(i >= 3)
        {
            char temp[16];
            sprintf(temp, "%d", i+1);
            CCLabelAtlas* pLabelRank =  CCLabelAtlas::create(temp, "rank/rankvipnum2.png", 32, 38, '0');
            pLabelRank->setScale(0.45f);
            pLabelRank -> setAnchorPoint(ccp(0.5f, 0.5f));
            pLabelRank -> setPosition(ccp(48, pSpCell->getContentSize().height/2));
            pSpCell -> addChild(pLabelRank);
        }
        CCLabelTTF* pLabelName = CCLabelTTF::create("", "Arial-BoldMT", 30);
        pLabelName -> setScale(0.5f);
        pLabelName -> setPosition(ccp(pSpCell->getContentSize().width/2, pSpCell->getContentSize().height/2));
        pLabelName -> setTag(nickName_tag);
        pLabelName -> setColor(ccc3(101, 51, 26));
        pSpCell -> addChild(pLabelName, 2);
        CCLabelTTF* pLabelJifen = CCLabelTTF::create("", "Arial-BoldMT", 32);
        pLabelJifen -> setScale(0.5f);
        pLabelJifen -> setTag(jinbi_Tag);
        pLabelJifen -> setColor(ccc3(101, 51, 26));
        pLabelJifen -> setPosition(ccp(pSpCell->getContentSize().width/4*3+45, pSpCell->getContentSize().height/2));
        pSpCell -> addChild(pLabelJifen, 2);
    }
    
    sendWeekData();
}

void RankLayer::sendWeekData()
{
    int nUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    char temp[256];
    sprintf(temp, "http://%s/getweeklist.aspx?uid=%d",NORMAL_URL,nUserID);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setTag("http test");
    request -> setResponseCallback(this, httpresponse_selector(RankLayer::onHttpWeekComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void RankLayer::onHttpWeekComplete(CCHttpClient* sender, CCHttpResponse* response)
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
        if (0!=nodename.compare("ranklist"))
        {
            return;
        }
        std::string type=rootElement->Attribute("value");
        if(!std::strcmp(type.c_str(), "true"))//相同
        {
            TiXmlElement * pCurrentElement=rootElement->FirstChildElement();
            const char * pValue=pCurrentElement->Value();
            if (!strcmp(pValue,"userInfo"))
            {
                std::string numStr = pCurrentElement->Attribute("num");
                int nNum = atoi(numStr.c_str());
                if(nNum == 1001)//排名千里之外
                {
                    m_lRankWeek -> setString("千里之外");
                }
                else//排名在30名内
                {
                    m_lRankWeek -> setString(numStr.c_str());
                }
                
                std::string yesNum = pCurrentElement->Attribute("lastnum");
                int nYesNum = atoi(yesNum.c_str());
                if(nYesNum == 1001)
                {
                    char temp[64];
                    sprintf(temp, "您上周的排名在千里之外");
                    m_labelWeekYesRank->setString(temp);
                }
                else
                {
                    char temp[64];
                    sprintf(temp, "您上周的排名为:%d", nYesNum);
                    m_labelWeekYesRank->setString(temp);
                }
                std::string sCount = pCurrentElement->Attribute("count");
                m_lWeekJifen -> setString(sCount.c_str());
            }
            
            pCurrentElement=pCurrentElement->NextSiblingElement();
            while (NULL!=pCurrentElement)
            {
                const char * pValue=pCurrentElement->Value();
                if (!strcmp(pValue,"accountsInfo"))
                {
                    tagRankTimes* RankTimes = new tagRankTimes;
                    m_vctWeekRankTimes.push_back(RankTimes);
                    TiXmlElement* firstElement = pCurrentElement -> FirstChildElement();
                    const char * valueId=firstElement->Value();
                    if (!strcmp(valueId,"num"))
                    {
                        int id=atoi(firstElement->GetText());
                        RankTimes->nID = id;
                    }
                    TiXmlElement* secondElement = firstElement -> NextSiblingElement();
                    const char * nickname=secondElement->Value();
                    if (!strcmp(nickname,"nickname"))
                    {
                        RankTimes->sNickName = secondElement->GetText();
                    }
                    TiXmlElement* thirdElement = secondElement -> NextSiblingElement();
                    const char * score=thirdElement->Value();
                    if (!strcmp(score,"count"))
                    {
                        SCORE score=atoll(thirdElement->GetText());
                        RankTimes->llScore = score;
                    }
                    TiXmlElement* fourElement = thirdElement -> NextSiblingElement();
                    const char * level=fourElement->Value();
                    if (!strcmp(level,"viplevel"))
                    {
                        int nLevel = atoi(fourElement->GetText());
                        RankTimes->nLevel = nLevel;
                    }
                }
                pCurrentElement=pCurrentElement->NextSiblingElement();
            }
            
        }
        else//没有数据
        {
            m_scrollWeekView->setVisible(false);
            m_scrollWeekView->setTouchEnabled(false);
            CCLabelTTF* pLabelTTF = CCLabelTTF::create("暂无比赛结果", "Arial", 64);
            pLabelTTF -> setPosition(ccp(360,360));
            m_WeekNode -> addChild(pLabelTTF);
        }
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK);
    }
    delete document;
    
    m_spAnimateWeek->stopAllActions();
    m_spAnimateWeek->removeFromParent();
    
    //更新控件显示
    for(unsigned int i = 0; i < m_vctWeekRankTimes.size(); i++)
    {
        CCSprite* pSpCell = (CCSprite*)m_arrWeekCell->objectAtIndex(i);
        if(pSpCell != NULL)
        {
            CCLabelTTF* pLabelName = (CCLabelTTF*)pSpCell->getChildByTag(nickName_tag);
            if(pLabelName != NULL)
            {
                pLabelName -> setString(m_vctWeekRankTimes[i]->sNickName.c_str());
                if(m_vctWeekRankTimes[i]->nLevel >= 1)
                {
                    pLabelName -> setColor(ccc3(255, 0, 0));
                }
            }
            CCLabelTTF* pLabelJinbi = (CCLabelTTF*)pSpCell->getChildByTag(jinbi_Tag);
            if(pLabelJinbi != NULL)
            {
                char temp[32];
                sprintf(temp, "%lld", m_vctWeekRankTimes[i]->llScore);
                pLabelJinbi -> setString(temp);
            }
        }
    }
}

void RankLayer::showCurrentMenu(int nIndex)
{
    for(int i = 0; i < m_arrNormal->count(); i++)
    {
        CCSprite* pSpDi = (CCSprite*)m_arrNormal->objectAtIndex(i);
        pSpDi->setVisible(true);
    }
    for(int i = 0; i < m_arrSelect->count(); i++)
    {
        CCSprite* pSpDi = (CCSprite*)m_arrSelect->objectAtIndex(i);
        if(pSpDi->getTag() == nIndex)
        {
            pSpDi->setVisible(true);
        }
        else
        {
            pSpDi->setVisible(false);
        }
    }
    for(int i = 0; i < m_arrRankInfo->count(); i++)
    {
        CCSprite* pSpInfo = (CCSprite*)m_arrRankInfo->objectAtIndex(i);
        if(pSpInfo->getTag() == nIndex)
        {
            pSpInfo->setVisible(true);
        }
        else
        {
            pSpInfo->setVisible(false);
        }
    }
}

void RankLayer::showDayRank()
{
    if(m_nCurrentIndex == 3)
        return;
    m_nCurrentIndex = 3;
    showCurrentMenu(100+m_nCurrentIndex);
    m_VipNode -> setVisible(false);
    m_scrollVipView -> setTouchEnabled(false);
    m_scrollVipInfoView -> setTouchEnabled(false);
    m_MoneyNode -> setVisible(false);
    m_scrollMoneyView -> setTouchEnabled(false);
    m_scrollMoneyInfoView -> setTouchEnabled(false);
    m_DayNode -> setVisible(true);
    m_scrollDayView -> setTouchEnabled(true);
    m_WeekNode -> setVisible(false);
    m_scrollWeekView -> setTouchEnabled(false);
    m_MatchNode -> setVisible(true);
    m_scrollMatchView -> setTouchEnabled(true);
}

void RankLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case 101://vip
        {
            if(m_nCurrentIndex == 1)
                break;
            m_nCurrentIndex = 1;
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            showCurrentMenu(100+m_nCurrentIndex);
            
            m_VipNode -> setVisible(true);
            m_scrollVipView -> setTouchEnabled(true);
            m_scrollVipInfoView -> setTouchEnabled(true);
            m_MoneyNode -> setVisible(false);
            m_scrollMoneyView -> setTouchEnabled(false);
            m_scrollMoneyInfoView -> setTouchEnabled(false);
//            m_DayNode -> setVisible(false);
//            m_scrollDayView -> setTouchEnabled(false);
//            m_WeekNode -> setVisible(false);
//            m_scrollWeekView -> setTouchEnabled(false);
            m_MatchNode -> setVisible(false);
            m_scrollMatchView -> setTouchEnabled(false);
            if(GameMainScene::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameRankVIP);
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallRankVIP);
            }
            break;
        }
        case 102://财富
        {
            if(m_nCurrentIndex == 2)
                break;
            m_nCurrentIndex = 2;
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            showCurrentMenu(100+m_nCurrentIndex);
            
            m_VipNode -> setVisible(false);
            m_scrollVipView -> setTouchEnabled(false);
            m_scrollVipInfoView -> setTouchEnabled(false);
            m_MoneyNode -> setVisible(true);
            m_scrollMoneyView -> setTouchEnabled(true);
            m_scrollMoneyInfoView -> setTouchEnabled(true);
//            m_DayNode -> setVisible(false);
//            m_scrollDayView -> setTouchEnabled(false);
//            m_WeekNode -> setVisible(false);
//            m_scrollWeekView -> setTouchEnabled(false);
            m_MatchNode -> setVisible(false);
            m_scrollMatchView -> setTouchEnabled(false);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallRankMoney);
            break;
        }
        case 103://每日比赛
        {
            if(m_nCurrentIndex == 3)
                break;
            m_nCurrentIndex = 3;
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            showCurrentMenu(100+m_nCurrentIndex);
            
            m_VipNode -> setVisible(false);
            m_scrollVipView -> setTouchEnabled(false);
            m_scrollVipInfoView -> setTouchEnabled(false);
            m_MoneyNode -> setVisible(false);
            m_scrollMoneyView -> setTouchEnabled(false);
            m_scrollMoneyInfoView -> setTouchEnabled(false);
            m_DayNode -> setVisible(true);
            m_scrollDayView -> setTouchEnabled(true);
            m_WeekNode -> setVisible(false);
            m_scrollWeekView -> setTouchEnabled(false);
            m_MatchNode -> setVisible(true);
            m_scrollMatchView -> setTouchEnabled(true);
            break;
        }
        case 104://每周比赛
        {
            if(m_nCurrentIndex == 4)
                break;
            m_nCurrentIndex = 4;
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            showCurrentMenu(100+m_nCurrentIndex);
            
            m_VipNode -> setVisible(false);
            m_scrollVipView -> setTouchEnabled(false);
            m_scrollVipInfoView -> setTouchEnabled(false);
            m_MoneyNode -> setVisible(false);
            m_scrollMoneyView -> setTouchEnabled(false);
            m_scrollMoneyInfoView -> setTouchEnabled(false);
            m_DayNode -> setVisible(false);
            m_scrollDayView -> setTouchEnabled(false);
            m_WeekNode -> setVisible(true);
            m_scrollWeekView -> setTouchEnabled(true);
            m_MatchNode -> setVisible(false);
            m_scrollMatchView -> setTouchEnabled(false);
            break;
        }
        case 105://比赛说明
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            CMainLogic::sharedMainLogic()->m_bEnterService = true;
            FMLayerWebView* pFMLayerWebView = FMLayerWebView::create(0, 1);
            pFMLayerWebView -> setPosition(CCPointZero);
            this -> addChild(pFMLayerWebView, 300);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            CMainLogic::sharedMainLogic()->openRankInfo();
#endif
            if(GameMainScene::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameRankRule);
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallRankRule);
            }
            break;
        }
        default:
            break;
    }
}

void RankLayer::closeMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CMainLogic::sharedMainLogic()->m_bEnterService = false;
    this -> removeFromParent();
}

void RankLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool RankLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint point = pTouch->getLocation();
    for(int i = 0; i < m_arrNormal->count(); i++)
    {
        CCSprite* pSpDi = (CCSprite*)m_arrNormal->objectAtIndex(i);
        if(pSpDi != NULL)
        {
            CCRect rect = pSpDi->boundingBox();
            if(rect.containsPoint(point))
            {
                CCMenuItem* item = CCMenuItem::create();
                item->setTag(pSpDi->getTag());
                clickMenu(item);
                break;
            }
        }
    }
    return true;
}