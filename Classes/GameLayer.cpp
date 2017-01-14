#include "header.h"

enum
{
    eSuperMinusTag=200,
    eSuperPlusTag,
    eSuperAutoTag,
    eSuperCancelAutoTag,
    eSuperStartTag,
    eSuperItemStartTag,
    eSuperStopTag,
    eSuperBackTag,
    eSuperTwoTag,
    eSuperThreeTag,
    eSuperFiveTag,
    eSuperTwoCancleTag,
    eSuperThreeCancleTag,
    eSuperFiveCancleTag,
    eSuperCalcTag,
    eSuperNanTag,
    eSuperNvTag,
    eSuperChargeTag,
    eSuperRankTag,
    eSuperHelpTag,
    eSuperVoiceOpenTag,
    eSuperVoiceCloseTag,
    eSuperRankCloseTag,
    eSuperShouJinAnimationTag,
    eUpSp,
    eDownSp,
    eLeftPoiner,
    eRightPoiner,
    eRollLeftFire1,
    eRollLeftFire2,
    eRollRightFire1,
    eRollRightFire2,
    ePrizeUp,
    ePrizeDown,
    ePrizeBet,
    eGameStar,
    eGameSameLight,
    eAddMoneyNumTag,
    eDragon,
};

#define RED_LIGHT_BG_TAG    70
#define RED_LIGHT_TAG       80
#define BIG_PRIZE_D_TAG       90

std::string GameLayer::m_sCmdLine;
GameLayer* GameLayer::_instance = NULL;

CCScene* GameLayer::scene()
{
    CCScene *scene = CCScene::create();
    if (_instance == NULL)
    {
        _instance=new GameLayer();
        if (_instance && _instance->init())
        {
            _instance->autorelease();
        }
        else
        {
            delete _instance;
        }
    }
    scene->addChild(_instance);
    
    return scene;
    
}

GameLayer::~GameLayer()
{
    m_vctNotice.clear();
    m_vctRank.clear();
    m_noticeArray->release();
	delete m_pClientKernel;
	m_pClientKernel = NULL;
    _instance = NULL;
}

GameLayer::GameLayer()
{
	
}

GameLayer* GameLayer::GetInstance()
{
    if(_instance == NULL)
    {
        _instance=new GameLayer();
        if (_instance && _instance->init())
        {
            _instance->autorelease();
        }
        else
        {
            delete _instance;
        }
    }
    return _instance;
}

bool GameLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
    m_vctNotice.clear();
    m_vctRank.clear();
    m_lUserScore = 0;
    m_fSound = 1;
    m_fEffect = 1;
    m_bRolling = false;
    m_bAuto = false;
    m_bReceiveID = false;
    m_bPrizeRoll = false;
    m_bLackToGuess = true;
    m_bSame = false;
    m_bInitUIComplete = false;
    m_bInitNETComplete = false;
    m_bShowExitGame = false;
    m_bLackChip = true;
    m_bReceiveCoinCant = false;
    m_bReceiveGameScene = false;
    m_sCoinCantDes = "";
    m_bEnterGuided = !CMainLogic::sharedMainLogic()->m_cbSuperPirateGuided;
    m_lChipScore = 0;
    m_lDefultScore = 0;
    m_nTimesOnline = 6;
    m_nGameRollingEffectID = 0;
    m_nPrizeID = 0;
    m_nSelectID = 0;
    m_lRollPrize = 0;
    m_lNativePrize = 0;
    m_lTotalScore = 0;
    m_nMenuIndex = 1;
    m_nItemNum = 0;
    m_nFiveStarExtra = 0;
    m_nSixStarExtra = 0;
    m_nSevenStarExtra = 0;
    m_nEightStarExtra = 0;
    m_lGetPrizeExtra = 0;
    m_nCalTime = 0;
    m_nStarCount = 0;
    m_noticeArray = CCArray::create();
    m_noticeArray->retain();
    for (int i=0; i<SHOW_NUM; i++)
    {
        m_nRollerID[i] = -1;
    }

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SuperPirate/shoujin1.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SuperPirate/shoujin2.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SuperPirate/fireAnimate.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SuperPirate/stars.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SuperPirate/dragon1.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SuperPirate/dragon2.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SuperPirate/SuperPirate1.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SuperPirate/SuperPirate2.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SuperPirate/superguidejian.plist");
    
	CClientKernel * pClientKernel = new CClientKernel();
    pClientKernel->m_pIClientKernelSink = this;
	m_pClientKernel=pClientKernel;
    pBackground = CCSprite::create("SuperPirate/SuperPirateBg.png");
    pBackground->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(pBackground);
    CCSprite* pFireLeft = CCSprite::createWithSpriteFrameName("firesmall_1.png");
    pFireLeft -> setPosition(ccp(522, 472));
    pBackground -> addChild(pFireLeft);
    CCSprite* pFireRight = CCSprite::createWithSpriteFrameName("firesmall_1.png");
    pFireRight -> setPosition(ccp(933, 472));
    pFireRight -> setFlipX(true);
    pBackground -> addChild(pFireRight);
    pFireLeft -> runAction(CCRepeatForever::create(getAnimate(21, "firesmall", 0.05)));
    pFireRight -> runAction(CCRepeatForever::create(getAnimate(21, "firesmall", 0.05)));
    CCSprite* pFireBig = CCSprite::createWithSpriteFrameName("firebig_1.png");
    pFireBig -> setPosition(ccp(CLIENT_WIDTH/2+70, 545));
    pBackground -> addChild(pFireBig);
    pFireBig -> runAction(CCRepeatForever::create(getAnimate(25, "firebig", 0.05)));
    CCSprite* pShandian = CCSprite::createWithSpriteFrameName("shandian_1.png");
    pShandian -> setPosition(ccp(CLIENT_WIDTH/2+70, 470));
    pBackground -> addChild(pShandian);
    pShandian -> runAction(CCRepeatForever::create(CCSequence::create(getAnimate(22, "shandian", 0.07), CCHide::create(), CCDelayTime::create(3.0f), CCShow::create(), NULL)));

    //返回按钮
    CCSprite* pBack1 = CCSprite::createWithSpriteFrameName("SuperPirateClose.png");
    CCSprite* pBack2 = CCSprite::createWithSpriteFrameName("SuperPirateClose2.png");
    CCMenuItemSprite* pBackItem = CCMenuItemSprite::create(pBack1, pBack2, this, menu_selector(GameLayer::clickMenu));
    pBackItem -> setTag(eSuperBackTag);
    pBackItem -> setPosition(ccp(CLIENT_WIDTH*0.9f+50, 650));
    CCSprite* pCharge = CCSprite::createWithSpriteFrameName("superPirateChargeBtn.png");
    CCMenuItemSprite* pChargeItem = CCMenuItemSprite::create(pCharge, pCharge, this, menu_selector(GameLayer::clickMenu));
    pChargeItem -> setTag(eSuperChargeTag);
    pChargeItem -> setPosition(ccp(CLIENT_WIDTH*0.9f+60, 520));
    CCSprite* pRank = CCSprite::createWithSpriteFrameName("rank.png");
    CCMenuItemSprite* pRankItem = CCMenuItemSprite::create(pRank, pRank, this, menu_selector(GameLayer::clickMenu));
    pRankItem -> setTag(eSuperRankTag);
    pRankItem -> setPosition(ccp(CLIENT_WIDTH*0.9f+60, 440));
    CCSprite* pHelp = CCSprite::createWithSpriteFrameName("help.png");
    CCMenuItemSprite* pHelpItem = CCMenuItemSprite::create(pHelp, pHelp, this, menu_selector(GameLayer::clickMenu));
    pHelpItem -> setTag(eSuperHelpTag);
    pHelpItem -> setPosition(ccp(pRankItem->getPositionX(), pRankItem->getPositionY()-80));
    CCSprite* pVoiceOpen = CCSprite::createWithSpriteFrameName("voiceOpen.png");
    m_pVoiceOpenItem = CCMenuItemSprite::create(pVoiceOpen, pVoiceOpen, this, menu_selector(GameLayer::clickMenu));
    m_pVoiceOpenItem -> setTag(eSuperVoiceOpenTag);
    m_pVoiceOpenItem -> setPosition(ccp(pRankItem->getPositionX(), pRankItem->getPositionY()-160));
    CCSprite* pVoiceClose = CCSprite::createWithSpriteFrameName("voiceClose.png");
    m_pVoiceCloseItem = CCMenuItemSprite::create(pVoiceClose, pVoiceClose, this, menu_selector(GameLayer::clickMenu));
    m_pVoiceCloseItem -> setTag(eSuperVoiceCloseTag);
    m_pVoiceCloseItem -> setPosition(m_pVoiceOpenItem->getPosition());
    m_pVoiceCloseItem->setVisible(false);
    CCMenu* pBackMenu = CCMenu::create(pBackItem, pChargeItem, pRankItem, pHelpItem, m_pVoiceOpenItem, m_pVoiceCloseItem, NULL);
    pBackMenu -> setPosition(CCPointZero);
    pBackMenu -> setAnchorPoint(CCPointZero);
    pBackground -> addChild(pBackMenu, 2);
    
    CCLabelTTF* cclNickName = CCLabelTTF::create(CMainLogic::sharedMainLogic()->m_sNickName.c_str(), "Arial-BoldMT", 26);
    cclNickName->setColor(ccc3(255,255,255));
    cclNickName->setAnchorPoint(ccp(0, 0.5f));
    cclNickName->setPosition(ccp(CLIENT_WIDTH/2-166, CLIENT_HEIGHT/3*2+64));
    pBackground->addChild(cclNickName,2);
    //label kuang
    m_labelUserScore = CCLabelTTF::create("0", "Arial-BoldMT", 26);
    m_labelUserScore -> setPosition(ccp(CLIENT_WIDTH/2+117, CLIENT_HEIGHT/3*2+64));
    m_labelUserScore -> setAnchorPoint(ccp(0, 0.5f));
    pBackground -> addChild(m_labelUserScore, 2);
    
    m_node1 = CCNode::create();
    pBackground -> addChild(m_node1, 2);
    m_node2 = CCNode::create();
    pBackground -> addChild(m_node2, 2);
    m_node2 -> setVisible(false);
    
    loadUINode1();
    loadUINode2();
    
    //红绿灯、绿光、红光
    ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE };//效果叠加
    float fTime = 0.8f;
    for (int i = 0 ; i < 3; i++)
    {
        //三个间隔小绿灯
        CCSprite* pBlueLight = CCSprite::createWithSpriteFrameName("light_blue.png");
        pBlueLight->setPosition(ccp(513+170*i, 590));
        pBackground->addChild(pBlueLight);
        pBlueLight->setBlendFunc(cbl);
        CCFadeOut* pBlueFadeOut = CCFadeOut::create(fTime/2);
        CCScaleTo* scaleToBlueNormal = CCScaleTo::create(fTime/2, 1.0f);
        CCSpawn* spawnBuleNormal = CCSpawn::create(pBlueFadeOut, scaleToBlueNormal, NULL);
        CCDelayTime* pBlueDelay = CCDelayTime::create(fTime);
        CCFadeIn* pBlueFadeIn = CCFadeIn::create(fTime/2);
        CCScaleTo* scaleToBuleBig = CCScaleTo::create(fTime/2, 1.2f);
        CCSpawn* spawnBlueBig = CCSpawn::create(pBlueFadeIn, scaleToBuleBig, NULL);
        CCSequence* pBlueSeq = CCSequence::create(spawnBuleNormal, pBlueDelay, spawnBlueBig, NULL);
        pBlueLight->runAction(CCRepeatForever::create(pBlueSeq));
        //三个间隔小红灯
        CCSprite* pRedLight = CCSprite::createWithSpriteFrameName("light_red.png");
        pRedLight->setPosition(ccp(pBlueLight->getPositionX()+85, pBlueLight->getPositionY()));
        pBackground->addChild(pRedLight);
        pBlueLight->setBlendFunc(cbl);
        pRedLight->setOpacity(0);
        CCDelayTime* pRedDelay = CCDelayTime::create(fTime/2);
        CCFadeIn* pRedRedFadeIn = CCFadeIn::create(fTime/2);
        CCScaleTo* scaleToRedBig = CCScaleTo::create(fTime/2, 1.2f);
        CCSpawn* spawnRedNormal = CCSpawn::create(pRedRedFadeIn, scaleToRedBig, NULL);
        CCFadeOut* pRedFadeOut = CCFadeOut::create(fTime/2);
        CCScaleTo* scaleToRedNormal = CCScaleTo::create(fTime/2, 1.0f);
        CCSpawn* spawnRedBig = CCSpawn::create(pRedFadeOut, scaleToRedNormal, NULL);
        CCDelayTime* pRedDelay2 = CCDelayTime::create(fTime/2);
        CCSequence* pRedSeq = CCSequence::create(pRedDelay, spawnRedNormal, spawnRedBig, pRedDelay2, NULL);
        pRedLight->runAction(CCRepeatForever::create(pRedSeq));
        //右边三个红翅膀
        CCSprite* pRightRedLight = CCSprite::createWithSpriteFrameName("light_red_big.png");
        pRightRedLight->setPosition(ccp(1120, 340-84*i));
        pBackground->addChild(pRightRedLight);
        pRightRedLight->setBlendFunc(cbl);
        CCFadeOut* pFadeOut = CCFadeOut::create(1);
        CCScaleTo* scaleToRightRedNormal = CCScaleTo::create(1, 1.0f);
        CCSpawn* spawnRihgtRedNormal = CCSpawn::create(pFadeOut, scaleToRightRedNormal,NULL);
        CCFadeIn* pFadeIn = CCFadeIn::create(1);
        CCScaleTo* scaleToRightBig = CCScaleTo::create(1, 1.2f);
        CCSpawn* spawnRightBig = CCSpawn::create(pFadeIn, scaleToRightBig, NULL);
        CCSequence* pSeq = CCSequence::create(spawnRihgtRedNormal, spawnRightBig, NULL);
        pRightRedLight->runAction(CCRepeatForever::create(pSeq));

        //左边三个红翅膀
        CCSprite* pLeftRedLight = CCSprite::createWithSpriteFrameName("light_red_big.png");
        pLeftRedLight->setPosition(ccp(340, pRightRedLight->getPositionY()));
        pBackground->addChild(pLeftRedLight);
        pLeftRedLight->setFlipX(true);
        pLeftRedLight->setBlendFunc(cbl);
        CCFadeOut* pFadeOut2 = CCFadeOut::create(1);
        CCScaleTo* scaleToLeftRedNormal = CCScaleTo::create(1, 1.0f);
        CCSpawn* spawnLeftNormal = CCSpawn::create(pFadeOut2, scaleToLeftRedNormal, NULL);
        CCFadeIn* pFadeIn2 = CCFadeIn::create(1);
        CCScaleTo* scaleToLeftRedBig = CCScaleTo::create(1, 1.2f);
        CCSpawn* spawnLeftRedBig = CCSpawn::create(pFadeIn2, scaleToLeftRedBig, NULL);
        CCSequence* pSeq2 = CCSequence::create(spawnLeftNormal, spawnLeftRedBig, NULL);
        pLeftRedLight->runAction(CCRepeatForever::create(pSeq2));
    }
    for(int i = 0; i < 2; i++)
    {
        CCSprite* pBlueBigLiht = CCSprite::createWithSpriteFrameName("light_blue_big.png");
        pBlueBigLiht->setPosition(ccp(360+750*i, 558));
        pBackground->addChild(pBlueBigLiht);
        pBlueBigLiht->setBlendFunc(cbl);
        CCFadeOut* pFadeOut = CCFadeOut::create(1);
        CCFadeIn* pFadeIn = CCFadeIn::create(1);
        CCSequence* pSeq = CCSequence::create(pFadeOut, pFadeIn, NULL);
        pBlueBigLiht->runAction(CCRepeatForever::create(pSeq));
    }
    for (int i = 0 ; i < 8; i++)
    {
        CCSprite* pRedBlink = CCSprite::createWithSpriteFrameName("light_red_blink.png");
        if (i == 0)
        {
            pRedBlink->setPosition(ccp(518, 660));
        }
        else if(i == 5)
        {
            pRedBlink->setPosition(ccp(488+68*i, 704));
        }
        else if(i == 6)
        {
            pRedBlink->setPosition(ccp(488+68*i, 701));
        }
        else if(i == 7)
        {
            pRedBlink->setPosition(ccp(928, 660));
        }
        else
        {
            pRedBlink->setPosition(ccp(486+68*i, 702));
        }
        pBackground->addChild(pRedBlink, 0, RED_LIGHT_BG_TAG+i);
        pRedBlink->setOpacity(0);
        redLightBlink();
    }
    for (int i = 0 ; i < 8; i++)
    {
        CCSprite* pRedBlink = CCSprite::createWithSpriteFrameName("light_red_blink.png");
        if (i == 0)
        {
            pRedBlink->setPosition(ccp(518, 660));
        }
        else if(i == 5)
        {
            pRedBlink->setPosition(ccp(488+68*i, 704));
        }
        else if(i == 6)
        {
            pRedBlink->setPosition(ccp(488+68*i, 701));
        }
        else if(i == 7)
        {
            pRedBlink->setPosition(ccp(928, 660));
        }
        else
        {
            pRedBlink->setPosition(ccp(486+68*i, 702));
        }
        pBackground->addChild(pRedBlink, 0, RED_LIGHT_TAG+i);
        pRedBlink->setBlendFunc(cbl);
        pRedBlink->setOpacity(0);
        redLightBlink();
    }
    //排行
    createRankInfo();
    m_pLevelRankTTF = CCLabelTTF::create("0", "", 24);
    m_pLevelRankTTF->setAnchorPoint(ccp(0, 0.5));
    m_pLevelRankTTF->setPosition(ccp(62, 38));
    m_pRankSp->addChild(m_pLevelRankTTF);
    for (int i = 0; i < 11; i++)
    {
        m_pNickNameTTF[i] = CCLabelTTF::create("0", "", 24);
        m_pNickNameTTF[i]->setPosition(ccp(m_pRankSp->getContentSize().width*0.43f, m_pRankSp->getContentSize().height*0.77f-45*i));
        m_pRankSp->addChild(m_pNickNameTTF[i]);
        m_pScoreTTF[i] = CCLabelTTF::create("0", "", 24);
        m_pScoreTTF[i]->setPosition(ccp(m_pRankSp->getContentSize().width*0.82f, m_pNickNameTTF[i]->getPositionY()));
        m_pRankSp->addChild(m_pScoreTTF[i]);
        if (i < 3)
        {
            m_pNickNameTTF[i]->setColor(ccc3(220, 200, 89));
            m_pScoreTTF[i]->setColor(ccc3(249, 224, 86));
        }
        else
        {
            m_pNickNameTTF[i]->setColor(ccc3(247, 240, 189));
            m_pScoreTTF[i]->setColor(ccc3(255, 238, 225));
        }
        if (i == 10)
        {
            m_pNickNameTTF[i]->setPosition(ccp(m_pRankSp->getContentSize().width*0.43f, m_pRankSp->getContentSize().height*0.755f-45*i));
            m_pScoreTTF[i]->setPosition(ccp(m_pRankSp->getContentSize().width*0.82f, m_pNickNameTTF[i]->getPositionY()));
        }
    }
    //初始化巨奖显示数量
    CCScrollView * pBigPrizeScroll = CCScrollView::create(CCSizeMake(500, 54));
    CCLayer* containLayer = CCLayer::create();
    pBigPrizeScroll->setContainer(containLayer);
    pBigPrizeScroll->setTouchEnabled(false);
    pBigPrizeScroll->setPosition(ccp(580, 443));
    pBackground->addChild(pBigPrizeScroll, 1);
    for (int i = 0; i < 10; i++)
    {
        m_pBigPrize[i] = CCSprite::createWithSpriteFrameName("bigPrize0.png");
        for (int j = 1; j < 10; j++)
        {
            char sSp[32];
            sprintf(sSp, "bigPrize%d.png", j);
            CCSprite*  pSp = CCSprite::createWithSpriteFrameName(sSp);
            pSp->setPosition(ccp(m_pBigPrize[i]->getContentSize().width/2, m_pBigPrize[i]->getContentSize().height/2-m_pBigPrize[i]->getContentSize().height*j));
            m_pBigPrize[i]->addChild(pSp);
        }
        containLayer->addChild(m_pBigPrize[i], 2);
        if (i == 0)
        {
            m_pBigPrize[i]->setPosition(ccp(20, m_pBigPrize[i]->getContentSize().height/2));
        }
        else if(i > 0 && i <= 3)
        {
            m_pBigPrize[i]->setPosition(ccp(30+38*i, m_pBigPrize[i]->getContentSize().height/2));
        }
        else if(i > 3 && i < 7)
        {
            m_pBigPrize[i]->setPosition(ccp(40+38*i, m_pBigPrize[i]->getContentSize().height/2));
        }
        else
        {
            m_pBigPrize[i]->setPosition(ccp(50+38*i, m_pBigPrize[i]->getContentSize().height/2));
        }
    }
    for (int i = 0 ; i < 3; i++)
    {
        CCSprite* pBigPrizeD = CCSprite::createWithSpriteFrameName("bigPrize10.png");
        pBigPrizeD->setPosition(ccp(624+124*i, 448));
        pBackground->addChild(pBigPrizeD, 0, BIG_PRIZE_D_TAG+i);
    }
    
    //创建相同图案后显示加金币效果
    CCLabelAtlas* pAddMoneyAtLas = CCLabelAtlas::create("0", "SuperPirate/addMoneyNum.png", 35, 46, '0');
    pAddMoneyAtLas->setScale(1.5f);
    pAddMoneyAtLas->setAnchorPoint(ccp(0.5, 0.5));
    pAddMoneyAtLas->setPosition(ccp(CLIENT_WIDTH*0.58f, CLIENT_HEIGHT*0.38f-25));
    pBackground->addChild(pAddMoneyAtLas, 2, eAddMoneyNumTag);
    CCSprite* pAddMoneySp = CCSprite::createWithSpriteFrameName("addMoney.png");
    pAddMoneyAtLas->addChild(pAddMoneySp, 0, 100);
    pAddMoneyAtLas->setVisible(false);
    
    //创建公告滚动层
    createNoticeScrollView();
    
    this -> schedule(schedule_selector(GameLayer::checkGameStatusOnline), 1.0f);
    
    CMainLogic::sharedMainLogic()->BeforeGameStart();
    CMainLogic::sharedMainLogic()->SendGameOption();
    

    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    changeBigPrize(CMainLogic::sharedMainLogic()->m_lSuperPirateWard);
    
    
	return true;
}

void GameLayer::createRankInfo()
{
    m_pRankSp = CCSprite::createWithSpriteFrameName("rankBg.png");
    m_pRankSp->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    pBackground->addChild(m_pRankSp, 10);
    
    m_rankLayer = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    pBackground->addChild(m_rankLayer, 9);
    m_rankLayer->setVisible(false);
    CCSprite* pClose1 = CCSprite::createWithSpriteFrameName("rankClose1.png");
    CCSprite* pClose2 = CCSprite::createWithSpriteFrameName("rankClose1.png");
    CCMenuItemSprite* pCloseItem = CCMenuItemSprite::create(pClose1, pClose2, this, menu_selector(GameLayer::clickMenu));
    pCloseItem->setPosition(ccp(m_pRankSp->getContentSize().width-40, m_pRankSp->getContentSize().height-40));
    pCloseItem->setTag(eSuperRankCloseTag);
    pCloseItem -> setScale(1.2f);
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    pMenu->setTouchPriority(-160);
    m_pRankSp->addChild(pMenu);
    m_pRankSp->setVisible(false);
    CCMenuItem* layerItem = CCMenuItem::create();
    layerItem->setContentSize(CCSizeMake(CLIENT_WIDTH, CLIENT_HEIGHT));
    layerItem->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    CCMenu* pLayerMenu = CCMenu::createWithItem(layerItem);
    pLayerMenu->setTouchPriority(-159);
    pLayerMenu->setPosition(ccp(0, 0));
    m_pRankSp->addChild(pLayerMenu);
}

void GameLayer::onHttpRankComplete(CCHttpClient* sender, CCHttpResponse* response)
{
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
        
        m_vctRank.clear();
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
                    m_pLevelRankTTF->setString(pValue);
                }
                pValue=pCurrentElement->Attribute("nickname");
                if (NULL!=pValue)
                {
                    m_pNickNameTTF[10]->setString(CMainLogic::sharedMainLogic()->m_sNickName.c_str());
                }
                pValue=pCurrentElement->Attribute("count");
                if (NULL!=pValue)
                {
                    m_pScoreTTF[10]->setString(pValue);
                }
            }
            else if(0==nodename.compare("SuInfo"))
            {
                tagRankInfo* pRankInfo = new tagRankInfo;
                m_vctRank.push_back(pRankInfo);
                TiXmlElement* pElment = pCurrentElement->FirstChildElement();
                TiXmlElement* pElment2 = pElment->NextSiblingElement();
                const char * pValue2=NULL;
                pValue2=pElment2->Value();
                if (!strcmp(pValue2, "nickname"))
                {
                    pRankInfo->sNickName = pElment2->GetText();
                }
                TiXmlElement* pElment3 = pElment2->NextSiblingElement();
                pValue2=pElment3->Value();
                if (!strcmp(pValue2, "count"))
                {
                    pRankInfo->sScore = pElment3->GetText();
                }
            }
            pChildElement=pChildElement->NextSiblingElement();
        }
        for (int i = 0; i < m_vctRank.size(); i++)
        {
            m_pNickNameTTF[i]->setString((m_vctRank[i]->sNickName).c_str());
            m_pScoreTTF[i]->setString((m_vctRank[i]->sScore).c_str());
        }
    }
    delete document;
}

void GameLayer::scrollViewDidScroll(CCScrollView* view)
{
}

void GameLayer::scrollViewDidZoom(CCScrollView* view)
{
}

void GameLayer::createNoticeScrollView()
{
    m_pNoticeScrollView = CCScrollView::create();
    m_pNoticeScrollView->setAnchorPoint(ccp(0, 0));
    m_pNoticeScrollView->setPosition(ccp(32, 90));
    m_pNoticeScrollView->setContentSize(CCSizeMake(248, 452));
    m_pNoticeScrollView->setViewSize(CCSizeMake(248, 452));
    
    CCLayer* pContainerLayer = CCLayer::create();//创建一个层，作为滚动的内容
    pContainerLayer->setPosition(ccp(0,0));
    m_pNoticeScrollView->setContainer(pContainerLayer);
    m_pNoticeScrollView->setBounceable(false);
    m_pNoticeScrollView->setDelegate(this);
    m_pNoticeScrollView->setTouchPriority(-157);
    m_pNoticeScrollView->setDirection(kCCScrollViewDirectionVertical);
    pBackground->addChild(m_pNoticeScrollView);
}

void GameLayer::showNotice(std::string sNotice)
{
    m_nTimesOnline = 5;
    //计算公告的位置
    float height = 0;
    float fWidth = 232.0f;
    if (m_vctNotice.size() > 20)
    {
        CCLabelTTF* pLabel = (CCLabelTTF*)m_noticeArray->objectAtIndex(0);
        m_noticeArray -> removeObject(pLabel);
        pLabel -> removeFromParent();
        m_vctNotice.erase(m_vctNotice.begin());
    }
    for(unsigned int i = 0; i < m_vctNotice.size(); i++)
    {
        tagNoticeInfo NoticeInfo = (tagNoticeInfo)m_vctNotice[i];
        float width = NoticeInfo.fWidth;
        height = height + width/fWidth*40+40;//每一个公告的高度
    }
    tagNoticeInfo NoticeInfo;
    NoticeInfo.labelNotice= CCLabelTTF::create(sNotice.c_str(), "Arial-BoldMT", 32);
    NoticeInfo.fWidth = NoticeInfo.labelNotice->getContentSize().width;
    NoticeInfo.labelNotice = HorizontalSpacingANDVerticalSpacing(NoticeInfo.labelNotice->getString(), "Arial-BoldMT", 32, 1.5, 1, fWidth);
    
    CCLayer* pContainerLayer = (CCLayer*)m_pNoticeScrollView->getContainer();
    
    float fY = height + NoticeInfo.fWidth/fWidth*40+40;
    if(fY < 452)
    {
        m_pNoticeScrollView->setTouchEnabled(false);
        NoticeInfo.labelNotice -> setPosition(ccp(10, 432 - height));
    }
    else
    {
        m_pNoticeScrollView->setTouchEnabled(true);
        pContainerLayer->setContentSize(CCSizeMake(248, fY));
        pContainerLayer->setPosition(ccp(0,0));
        m_pNoticeScrollView->setContentSize(CCSizeMake(248, fY));
        NoticeInfo.labelNotice -> setPosition(ccp(10, NoticeInfo.fWidth/fWidth*40+20));
        
        float width = 0;
        for(unsigned int i = 0; i < m_vctNotice.size(); i++)
        {
            tagNoticeInfo NoticeInfo = (tagNoticeInfo)m_vctNotice[i];
            width = width + NoticeInfo.fWidth/fWidth*40+40;
            NoticeInfo.labelNotice -> setPosition(ccp(10, fY-width+NoticeInfo.fWidth/fWidth*40+10));
        }
        
    }
    NoticeInfo.labelNotice->setAnchorPoint(ccp(0, 0.5f));
    pContainerLayer->addChild(NoticeInfo.labelNotice);
    m_noticeArray->addObject(NoticeInfo.labelNotice);
    
    m_vctNotice.push_back(NoticeInfo);
    
}

CCLabelTTF* GameLayer::HorizontalSpacingANDVerticalSpacing(std::string _string, const char *fontName, float fontSize, float horizontalSpacing, float verticalSpacing, float lineWidth)
{
    CCArray* labelTTF_arr = CCArray::create();
    int index = 0;
    int index_max = (int)strlen(_string.c_str());
    bool is_end = true;
    while (is_end) {
        if (_string[index] >= 0 && _string[index] <= 127) {
            std::string englishStr = _string.substr(index,1).c_str();
            const char* englishChar = englishStr.c_str();
            labelTTF_arr->addObject(CCLabelTTF::create(englishChar, fontName, fontSize));
            index+= 1;
        }
        else{
            std::string chineseStr = _string.substr(index,3).c_str();
            const char* chineseChar =  chineseStr.c_str();
            labelTTF_arr->addObject(CCLabelTTF::create(chineseChar, fontName, fontSize));
            index+= 3;
        }
        if (index>=index_max) {
            is_end=false;
        }
    }
    //以上步骤是根据ASCII码找出中英文字符，并创建成一个CCLabelTTF对象存入labelTTF_arr数组中。
    //下面创建的原理是在CCLabelTTF对象上添加子对象CCLabelTTF，以此组合成一句话，以左上角第一个字为锚点。。
    CCLabelTTF* returnTTF = (CCLabelTTF*)labelTTF_arr->objectAtIndex(0);
    returnTTF->setColor(ccc3(178, 144, 116));
    float nowWidth = returnTTF->getContentSize().width;
    CCLabelTTF* dangqiangTTF = returnTTF;
    CCLabelTTF* lineBeginTTF = returnTTF;
    
    int arr_count = labelTTF_arr->count();
    for (int i=1; i < arr_count; i++) {
        CCLabelTTF* beforeTTF = (CCLabelTTF*)labelTTF_arr->objectAtIndex(i);
        beforeTTF->setColor(ccc3(178, 144, 116));
        beforeTTF->setAnchorPoint(ccp(0, 0.5));
        nowWidth+=beforeTTF->getContentSize().width;
        if (nowWidth >= lineWidth) {
            nowWidth = returnTTF->getContentSize().width;
            dangqiangTTF = lineBeginTTF;
            beforeTTF->setPosition(ccp(0, -dangqiangTTF->getContentSize().height*0.5-verticalSpacing));
            lineBeginTTF = beforeTTF;
        }else{
            beforeTTF->setPosition(ccp(dangqiangTTF->getContentSize().width+horizontalSpacing, dangqiangTTF->getContentSize().height*0.5));
        }
        dangqiangTTF->addChild(beforeTTF);
        dangqiangTTF = beforeTTF;
    }
    
    return returnTTF;
}

void GameLayer::changeBigPrize(LONGLONG lBigPrize)
{
    int a[10];
    memset(a, 0, sizeof(a));
    for (int i=0; i<10; i++)
    {
        int p = (int)pow(10, 9-i);
        a[i] = (int)((lBigPrize/p)%10);
    }
    for (int i=0; i<10; i++)
    {
        if (m_pBigPrize[i] == NULL)
        {
            return;
        }
        m_pBigPrize[i]->setVisible(true);
        if (i < 3)
        {
            CCSprite* pBigPrizeD1 = (CCSprite*)pBackground->getChildByTag(BIG_PRIZE_D_TAG+i);
            if (pBigPrizeD1 != NULL)
            {
                pBigPrizeD1->setVisible(true);
            }
        }
        m_pBigPrize[i]->stopAllActions();
        int yPosDest = 27+54*a[i];
        CCMoveTo* moveTo = CCMoveTo::create(1, ccp(m_pBigPrize[i]->getPositionX(), yPosDest));
        m_pBigPrize[i]->runAction(moveTo);
    }
    //隐藏未用到的数字以及逗号
    int nCount = 0;//用来计算位数
    while(lBigPrize)
    {
        lBigPrize = lBigPrize/10;
        nCount++;
    }
    if (nCount < 10 && nCount >= 7)
    {
        CCSprite* pBigPrizeD1 = (CCSprite*)pBackground->getChildByTag(BIG_PRIZE_D_TAG);
        if (pBigPrizeD1 != NULL)
        {
            pBigPrizeD1->setVisible(false);
        }
    }
    else if(nCount < 7 && nCount >=4)
    {
        CCSprite* pBigPrizeD1 = (CCSprite*)pBackground->getChildByTag(BIG_PRIZE_D_TAG);
        if (pBigPrizeD1 != NULL)
        {
            pBigPrizeD1->setVisible(false);
        }
        CCSprite* pBigPrizeD2 = (CCSprite*)pBackground->getChildByTag(BIG_PRIZE_D_TAG+1);
        if (pBigPrizeD2 != NULL)
        {
            pBigPrizeD2->setVisible(false);
        }
    }
    else if(nCount < 4 && nCount >= 0)
    {
        CCSprite* pBigPrizeD1 = (CCSprite*)pBackground->getChildByTag(BIG_PRIZE_D_TAG);
        if (pBigPrizeD1 != NULL)
        {
            pBigPrizeD1->setVisible(false);
        }
        CCSprite* pBigPrizeD2 = (CCSprite*)pBackground->getChildByTag(BIG_PRIZE_D_TAG+1);
        if (pBigPrizeD2 != NULL)
        {
            pBigPrizeD2->setVisible(false);
        }
        CCSprite* pBigPrizeD3 = (CCSprite*)pBackground->getChildByTag(BIG_PRIZE_D_TAG+2);
        if (pBigPrizeD3 != NULL)
        {
            pBigPrizeD3->setVisible(false);
        }
    }
    else
    {
        for(int i = 0; i < 3; i++)
        {
            CCSprite* pBigPrizeD1 = (CCSprite*)pBackground->getChildByTag(BIG_PRIZE_D_TAG+i);
            if (pBigPrizeD1 != NULL)
            {
                pBigPrizeD1->setVisible(true);
            }
        }
    }
    for (int i = 0 ; i < 10-nCount; i++)
    {
        m_pBigPrize[i]->setVisible(false);
    }
}

void GameLayer::lightRedCall(CCNode* pNode)
{
    float fRedTime = 1.0f;
    CCFadeIn* pFadeIn = CCFadeIn::create(fRedTime/2);
    CCScaleTo* scaleToNormal = CCScaleTo::create(fRedTime/2, 1.0f);
    CCSpawn* spawnNormal = CCSpawn::create(pFadeIn, scaleToNormal, NULL);
    CCFadeOut* pFadeOut = CCFadeOut::create(fRedTime/2);
    CCScaleTo* scaleToBig = CCScaleTo::create(fRedTime/2, 1.5f);
    CCSpawn* spawnBig = CCSpawn::create(pFadeOut, scaleToBig ,NULL);
    CCSequence* pSeq = CCSequence::create(spawnNormal, spawnBig, NULL);
    pNode->runAction(CCRepeatForever::create(pSeq));
}

void GameLayer::redLightBlink()
{
    float fRedTime = 1.0f;
    for (int i = 0; i < 8; i++)
    {
        CCSprite* pRedLightBg = (CCSprite*)pBackground->getChildByTag(RED_LIGHT_BG_TAG+i);
        if (pRedLightBg != NULL)
        {
            pRedLightBg->stopAllActions();
            CCDelayTime* pDelay = CCDelayTime::create(fRedTime*i/8);
            CCCallFuncN* pCallFuncN = CCCallFuncN::create(this, callfuncN_selector(GameLayer::lightRedCall));
            CCSequence* pSeq = CCSequence::create(pDelay, pCallFuncN, NULL);
            pRedLightBg->runAction(pSeq);
        }
        CCSprite* pRedLight = (CCSprite*)pBackground->getChildByTag(RED_LIGHT_TAG+i);
        if (pRedLight != NULL)
        {
            pRedLight->stopAllActions();
            CCDelayTime* pDelay = CCDelayTime::create(fRedTime*i/8);
            CCCallFuncN* pCallFuncN = CCCallFuncN::create(this, callfuncN_selector(GameLayer::lightRedCall));
            CCSequence* pSeq = CCSequence::create(pDelay, pCallFuncN, NULL);
            pRedLight->runAction(pSeq);
        }
    }
}

void GameLayer::redLightScroll()
{
    float fRedTime = 0.3f;
    for (int i = 0; i < 8; i++)
    {
        CCSprite* pRedLightBg = (CCSprite*)pBackground->getChildByTag(RED_LIGHT_BG_TAG+i);
        if (pRedLightBg != NULL)
        {
            pRedLightBg->stopAllActions();
            CCDelayTime* pDelay = CCDelayTime::create(fRedTime/8);
            CCFadeTo* pFadeTo1 = CCFadeTo::create(fRedTime/8, 0);
            if (i%2 == 0)
            {
                pFadeTo1 = CCFadeTo::create(fRedTime/8, 180);
            }
            CCDelayTime* pDelay1 = CCDelayTime::create(fRedTime/8);
            CCFadeTo* pFadeTo2 = CCFadeTo::create(fRedTime/8, 0);
            if (i%2 == 0)
            {
                pFadeTo2 = CCFadeTo::create(fRedTime/8, 255);
            }
            else
            {
                pFadeTo2 = CCFadeTo::create(fRedTime/8, 180);
            }
            CCFadeTo* pFadeTo3 = CCFadeTo::create(fRedTime/8, 0);
            if (i%2 == 0)
            {
                pFadeTo3 = CCFadeTo::create(fRedTime/8, 180);
            }
            else
            {
                pFadeTo3 = CCFadeTo::create(fRedTime/8, 255);
            }
            CCDelayTime* pDelay2 = CCDelayTime::create(fRedTime/8);
            CCFadeTo* pFadeTo4 = CCFadeTo::create(fRedTime/8, 0);
            if (i%2 == 0)
            {
                pFadeTo4 = CCFadeTo::create(fRedTime/8, 0);
            }
            else
            {
                pFadeTo4 = CCFadeTo::create(fRedTime/8, 180);
            }
            CCFadeTo* pFadeTo5 = CCFadeTo::create(fRedTime/8, 0);
            if (i%2 == 1)
            {
                pFadeTo5 = CCFadeTo::create(fRedTime/8, 0);
            }
            CCSequence* pSeq = CCSequence::create(pDelay, pFadeTo1, pDelay1, pFadeTo2, pFadeTo3, pDelay2, pFadeTo4, pFadeTo5, NULL);
            pRedLightBg->runAction(CCRepeatForever::create(pSeq));
        }
        CCSprite* pRedLight = (CCSprite*)pBackground->getChildByTag(RED_LIGHT_TAG+i);
        if (pRedLight != NULL)
        {
            pRedLight->stopAllActions();
            CCDelayTime* pDelay = CCDelayTime::create(fRedTime/8);
            CCFadeTo* pFadeTo1 = CCFadeTo::create(fRedTime/8, 0);
            if (i%2 == 0)
            {
                pFadeTo1 = CCFadeTo::create(fRedTime/8, 180);
            }
            CCDelayTime* pDelay1 = CCDelayTime::create(fRedTime/8);
            CCFadeTo* pFadeTo2 = CCFadeTo::create(fRedTime/8, 0);
            if (i%2 == 0)
            {
                pFadeTo2 = CCFadeTo::create(fRedTime/8, 255);
            }
            else
            {
                pFadeTo2 = CCFadeTo::create(fRedTime/8, 180);
            }
            CCFadeTo* pFadeTo3 = CCFadeTo::create(fRedTime/8, 0);
            if (i%2 == 0)
            {
                pFadeTo3 = CCFadeTo::create(fRedTime/8, 180);
            }
            else
            {
                pFadeTo3 = CCFadeTo::create(fRedTime/8, 255);
            }
            CCDelayTime* pDelay2 = CCDelayTime::create(fRedTime/8);
            CCFadeTo* pFadeTo4 = CCFadeTo::create(fRedTime/8, 0);
            if (i%2 == 0)
            {
                pFadeTo4 = CCFadeTo::create(fRedTime/8, 0);
            }
            else
            {
                pFadeTo4 = CCFadeTo::create(fRedTime/8, 180);
            }
            CCFadeTo* pFadeTo5 = CCFadeTo::create(fRedTime/8, 0);
            if (i%2 == 1)
            {
                pFadeTo5 = CCFadeTo::create(fRedTime/8, 0);
            }
            CCSequence* pSeq = CCSequence::create(pDelay, pFadeTo1, pDelay1, pFadeTo2, pFadeTo3, pDelay2, pFadeTo4, pFadeTo5, NULL);
            pRedLight->runAction(CCRepeatForever::create(pSeq));
        }
    }
}

void GameLayer::checkGameStatusOnline()
{
    m_nTimesOnline--;
    if(m_nTimesOnline<0)
    {
        //发送检测包
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
        this->scheduleOnce(schedule_selector(GameLayer::disConnectGame), 3.0f);
        m_nTimesOnline = 5;
    }
}

void GameLayer::disConnectGame()
{
    this -> unschedule(schedule_selector(GameLayer::checkGameStatusOnline));
    CMainLogic::sharedMainLogic()->ShowMessage("与服务器已经断开连接", eMBSuperOK, eDisConnectNet);
}

CCAnimate* GameLayer::getAnimate(int nCount, std::string sName, float interval)
{
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    char temp[32];
    CCArray* array = CCArray::create();
    for(int i = 1; i < nCount; i++)
    {
        sprintf(temp, "%s_%d.png", sName.c_str(), i);
        CCSpriteFrame* frame = cache->spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, interval);
    CCAnimate* animate = CCAnimate::create(animation);
    return animate;
}

void GameLayer::loadUINode1()
{
    CCSprite* pYafen = CCSprite::createWithSpriteFrameName("SuperPirateYafen.png");
    pYafen -> setPosition(ccp(CLIENT_WIDTH/5*2+10,72));
    m_node1 -> addChild(pYafen);
    CCLabelAtlas* pBetBgAtlas = CCLabelAtlas::create("999999", "SuperPirate/gameBetNum.png", 25, 36, '0');
    pBetBgAtlas -> setOpacity(50);
    pBetBgAtlas -> setAnchorPoint(ccp(0.5, 0.5));
    pBetBgAtlas -> setPosition(ccp(pYafen->getContentSize().width/2, pYafen->getContentSize().height/2+10));
    pYafen -> addChild(pBetBgAtlas, 2);
    m_labelCurrentYa = CCLabelAtlas::create("", "SuperPirate/gameBetNum.png", 25, 36, '0');
    m_labelCurrentYa -> setPosition(ccp(pYafen->getContentSize().width-40, pYafen->getContentSize().height/2+10));
    m_labelCurrentYa -> setAnchorPoint(ccp(1, 0.5f));
    pYafen -> addChild(m_labelCurrentYa, 2);
    //加减押注
    CCSprite* pJian1 = CCSprite::createWithSpriteFrameName("SuperPirateJian.png");
    CCSprite* pJian2 = CCSprite::createWithSpriteFrameName("SuperPirateJian2.png");
    CCMenuItemSprite* jianItem = CCMenuItemSprite::create(pJian1, pJian2, this, menu_selector(GameLayer::gameCallBack));
    jianItem -> setPosition(ccp(CLIENT_WIDTH/5*2-150, 80));
    jianItem -> setTag(eSuperMinusTag);
    CCSprite* pJia1 = CCSprite::createWithSpriteFrameName("SuperPirateJia.png");
    CCSprite* pJia2 = CCSprite::createWithSpriteFrameName("SuperPirateJia2.png");
    CCMenuItemSprite* jiaItem = CCMenuItemSprite::create(pJia1, pJia2, this, menu_selector(GameLayer::gameCallBack));
    jiaItem -> setPosition(ccp(CLIENT_WIDTH/5*3-84, 80));
    jiaItem -> setTag(eSuperPlusTag);
    CCSprite* pAuto1 = CCSprite::createWithSpriteFrameName("SuperPirateAuto.png");
    CCSprite* pAuto2 = CCSprite::createWithSpriteFrameName("SuperPirateAuto2.png");
    CCMenuItemSprite* autoItem = CCMenuItemSprite::create(pAuto1, pAuto2, this, menu_selector(GameLayer::gameCallBack));
    autoItem -> setPosition(ccp(CLIENT_WIDTH/5*3+90, 77));
    autoItem -> setTag(eSuperAutoTag);
    CCSprite* pStart1 = CCSprite::createWithSpriteFrameName("SuperPirateStart.png");
    CCSprite* pStart2 = CCSprite::createWithSpriteFrameName("SuperPirateStart2.png");
    CCMenuItemSprite* startItem = CCMenuItemSprite::create(pStart1, pStart2, this, menu_selector(GameLayer::gameCallBack));
    startItem -> setPosition(ccp(CLIENT_WIDTH/5*4+11, 87));
    startItem -> setTag(eSuperStartTag);
    
    CCSprite* pDaoJuStart = CCSprite::createWithSpriteFrameName("superPirateDaoJuGoBtn.png");
    CCMenuItemSprite* daoJuStartItem = CCMenuItemSprite::create(pDaoJuStart, pDaoJuStart, this, menu_selector(GameLayer::gameCallBack));
    daoJuStartItem -> setPosition(ccp(CLIENT_WIDTH/5*4+11, 85));
    daoJuStartItem -> setTag(eSuperItemStartTag);
    daoJuStartItem -> setVisible(false);
    CCLabelAtlas* pDaoJuNumTTF = CCLabelAtlas::create("", "SuperPirate/DaoJuGoNum.png", 27, 30, '0');
    pDaoJuNumTTF->setPosition(ccp(110, 70));
    daoJuStartItem->addChild(pDaoJuNumTTF, 1, 10);
    
    CCSprite* pCancelAuto1 = CCSprite::createWithSpriteFrameName("SuperPirateCancelAuto.png");
    CCSprite* pCancelAuto2 = CCSprite::createWithSpriteFrameName("SuperPirateCancelAuto2.png");
    CCMenuItemSprite* cancelAutoItem = CCMenuItemSprite::create(pCancelAuto1, pCancelAuto2, this, menu_selector(GameLayer::gameCallBack));
    cancelAutoItem -> setTag(eSuperCancelAutoTag);
    cancelAutoItem -> setPosition(ccp(CLIENT_WIDTH/5*3+90, 77));
    CCSprite* pStop1 = CCSprite::createWithSpriteFrameName("SuperPirateStop.png");
    CCSprite* pStop2 = CCSprite::createWithSpriteFrameName("SuperPirateStop2.png");
    CCMenuItemSprite* stopItem = CCMenuItemSprite::create(pStop1, pStop2, this, menu_selector(GameLayer::gameCallBack));
    stopItem -> setPosition(ccp(CLIENT_WIDTH/5*4+10, 88));
    stopItem -> setTag(eSuperStopTag);
    stopItem -> setVisible(false);
    cancelAutoItem -> setVisible(false);
    m_menuMain1 = CCMenu::create(jiaItem, jianItem, autoItem, startItem, daoJuStartItem, cancelAutoItem, stopItem, NULL);
    m_menuMain1 -> setAnchorPoint(CCPointZero);
    m_menuMain1 -> setPosition(CCPointZero);
    m_node1 -> addChild(m_menuMain1, 100);
    if (m_bReceiveGameScene)
    {
        m_menuMain1->setTouchEnabled(true);
    }
    else{
        m_menuMain1->setTouchEnabled(false);
    }
    
    CCSprite* pNode1bg = CCSprite::createWithSpriteFrameName("SuperPirateNode1Bg.png");
    pNode1bg->setPosition(ccp(CLIENT_WIDTH/2+88, CLIENT_HEIGHT/2-100));
    m_node1->addChild(pNode1bg, 1);
    CCSprite* pNode1Shade1 = CCSprite::createWithSpriteFrameName("SuperPirateshade.png");//prizeWhiteShade.png
    pNode1Shade1->setScaleX(4.7f);
    pNode1Shade1->setPosition(ccp(CLIENT_WIDTH/2+88, CLIENT_HEIGHT/2-32.5));
    m_node1->addChild(pNode1Shade1, 3);
    CCSprite* pNode1Shade2 = CCSprite::createWithSpriteFrameName("SuperPirateshade.png");//prizeWhiteShade.png
    pNode1Shade2->setFlipY(true);
    pNode1Shade2->setScaleX(4.7f);
    pNode1Shade2->setPosition(ccp(CLIENT_WIDTH/2+88, CLIENT_HEIGHT/2-32.5-pNode1Shade2->getContentSize().height));
    m_node1->addChild(pNode1Shade2, 3);
    
    CCSprite* pNode1Vert1 = CCSprite::createWithSpriteFrameName("vert1.png");
    pNode1Vert1->setPosition(ccp(CLIENT_WIDTH/2+88-92, CLIENT_HEIGHT/2-100));
    m_node1->addChild(pNode1Vert1, 1);
    CCSprite* pNode1Vert2 = CCSprite::createWithSpriteFrameName("vert1.png");
    pNode1Vert2->setPosition(ccp(CLIENT_WIDTH/2+88+92, CLIENT_HEIGHT/2-100));
    m_node1->addChild(pNode1Vert2, 1);
    
    CCSprite* pRollWhiteShade = CCSprite::createWithSpriteFrameName("SuperPirateRollShade.png");
    pRollWhiteShade->setScale(1.86f);
    pRollWhiteShade->setPosition(ccp(CLIENT_WIDTH/2+88, CLIENT_HEIGHT/2-100));
    m_node1->addChild(pRollWhiteShade, 3);
    pRollWhiteShade->setVisible(false);
    
    m_GameScrollView = CCScrollView::create(CCSizeMake(563, 260));
    CCLayerColor * containLayer = CCLayerColor::create(ccc4(0, 0, 0, 0), 563, 260);
    m_GameScrollView->setContainer(containLayer);
    m_GameScrollView->setTouchEnabled(false);
    m_GameScrollView->setPosition(ccp(0, 4));
    pNode1bg->addChild(m_GameScrollView, 1);
    
    char sPicName[32];
    int index = 0;
    for (int i=0; i<SHOW_NUM; i++)
    {
        index = 10;
        sprintf(sPicName, "SuperPirate_%d.png", 9);
        m_spNums[i] = CCSprite::createWithSpriteFrameName(sPicName);
        m_spNums[i]->setPosition(ccp(281-188*(i-1), containLayer->getContentSize().height/2-15));
        containLayer->addChild(m_spNums[i], 3);
        
        //上面
        sprintf(sPicName, "SuperPirate_%d.png", 10);//(1+1)>10?1:(1+1)
        CCSprite* pSpUp = CCSprite::createWithSpriteFrameName(sPicName);
        pSpUp->setPosition(ccp(m_spNums[i]->getContentSize().width/2, m_spNums[i]->getContentSize().height+86));
        m_spNums[i]->addChild(pSpUp, 1, eUpSp);
        //下面
        sprintf(sPicName, "SuperPirate_%d.png", 8);//(1-1)<1?10:(index-1)
        CCSprite* pSpDown = CCSprite::createWithSpriteFrameName(sPicName);
        pSpDown->setPosition(ccp(m_spNums[i]->getContentSize().width/2, -90));
        m_spNums[i]->addChild(pSpDown, 1, eDownSp);
        
        CCSprite* pRollAnimateDown = CCSprite::createWithSpriteFrameName("gameRoll.png");
        pRollAnimateDown->setAnchorPoint(ccp(0.5, 0));
        pRollAnimateDown->setPosition(ccp(90+190*i, 0));
        pRollAnimateDown->setScaleX(2.0);
        CCSprite* pRollAnimateUp = CCSprite::createWithSpriteFrameName("gameRoll.png");
        pRollAnimateUp->setAnchorPoint(ccp(0.5, 0));
        pRollAnimateUp->setPosition(ccp(90+190*i, pRollAnimateDown->getContentSize().height));
        pRollAnimateUp->setScaleX(2.0);
        pRollAnimateDown->setVisible(false);
        pRollAnimateUp->setVisible(false);
        if (i==0)
        {
            pRollAnimateDown->setTag(1001);
            pRollAnimateUp->setTag(1002);
        }
        else if (i==1)
        {
            pRollAnimateDown->setTag(1003);
            pRollAnimateUp->setTag(1004);
        }
        if (i==2)
        {
            pRollAnimateDown->setTag(1005);
            pRollAnimateUp->setTag(1006);
        }
        containLayer->addChild(pRollAnimateDown, 3);
        containLayer->addChild(pRollAnimateUp, 3);
    }
    
    //闪光动画
    CCSprite* pLeftCircle = CCSprite::createWithSpriteFrameName("circle.png");
    pLeftCircle->setPosition(ccp(450-20-5, 254));
    m_node1->addChild(pLeftCircle, 3);
    CCSprite* pRightCircle = CCSprite::createWithSpriteFrameName("circle.png");
    pRightCircle->setPosition(ccp(1010+10+3, 254));
    m_node1->addChild(pRightCircle, 3);
    
    CCSprite* pPointerLeft = CCSprite::createWithSpriteFrameName("SuperPiratePointer.png");
    pPointerLeft->setAnchorPoint(ccp(0, 0.5));
    pPointerLeft->setPosition(ccp(CLIENT_WIDTH/2-189-32, CLIENT_HEIGHT/2-109));
    m_node1->addChild(pPointerLeft, 3, eLeftPoiner);
    CCSprite* pPointerRight = CCSprite::createWithSpriteFrameName("SuperPiratePointer.png");
    pPointerRight->setFlipX(true);
    pPointerRight->setAnchorPoint(ccp(1, 0.5));
    pPointerRight->setPosition(ccp(CLIENT_WIDTH/2+365+32, CLIENT_HEIGHT/2-109));
    m_node1->addChild(pPointerRight, 3, eRightPoiner);
    
    CCSprite* pRollLeftFire1 = CCSprite::createWithSpriteFrameName("huo_1.png");
    pRollLeftFire1->setAnchorPoint(ccp(0.5, 1));
    pRollLeftFire1->setPosition(ccp(pPointerLeft->getPositionX()+60+5, pPointerLeft->getPositionY()+2));
    m_node1->addChild(pRollLeftFire1, 2, eRollLeftFire1);
    pRollLeftFire1 -> setVisible(false);
    CCSprite* pRollLeftFire2 = CCSprite::createWithSpriteFrameName("huo_1.png");
    pRollLeftFire2->setAnchorPoint(ccp(0.5, 1));
    pRollLeftFire2->setPosition(ccp(pPointerLeft->getPositionX()+60+5, pPointerLeft->getPositionY()-1));
    m_node1->addChild(pRollLeftFire2, 2, eRollLeftFire2);
    pRollLeftFire2 -> setVisible(false);
    
    CCSprite* pRollRightFire1 = CCSprite::createWithSpriteFrameName("huo_1.png");
    pRollRightFire1->setAnchorPoint(ccp(0.5, 1));
    pRollRightFire1->setPosition(ccp(pPointerRight->getPositionX()-40-5, pPointerRight->getPositionY()+2));
    m_node1->addChild(pRollRightFire1, 2, eRollRightFire1);
    pRollRightFire1->setVisible(false);
    CCSprite* pRollRightFire2 = CCSprite::createWithSpriteFrameName("huo_1.png");
    pRollRightFire2->setAnchorPoint(ccp(0.5, 1));
    pRollRightFire2->setPosition(ccp(pPointerRight->getPositionX()-40-5, pPointerRight->getPositionY()-1));
    m_node1->addChild(pRollRightFire2, 2, eRollRightFire2);
    pRollRightFire2->setVisible(false);
    
    CCSprite* pStar = CCSprite::createWithSpriteFrameName("gameRollWin_1.png");
    pStar->setScale(1.5f);
    pStar->setPosition(ccp(CLIENT_WIDTH/2+135, CLIENT_HEIGHT/2-60));
    m_node1->addChild(pStar, 101, eGameStar);
    pStar->setVisible(false);
    
    CCSprite *pSameLight = CCSprite::createWithSpriteFrameName("sameLingt_1.png");
    pSameLight->setAnchorPoint(ccp(0, 0.5));
    pSameLight->setPosition(ccp(CLIENT_WIDTH/2-166, CLIENT_HEIGHT/2-113));
    m_node1->addChild(pSameLight, 2, eGameSameLight);
    pSameLight->setVisible(false);
}

void GameLayer::loadUINode2()
{
    //2、3、5、结算按钮
    CCSprite* pTwo1 = CCSprite::createWithSpriteFrameName("SuperPirateTwo.png");
    CCSprite* pTwo2 = CCSprite::createWithSpriteFrameName("SuperPirateTwo2.png");
    CCMenuItemSprite* twoItem = CCMenuItemSprite::create(pTwo1, pTwo2, this, menu_selector(GameLayer::gameCallBack));
    twoItem -> setPosition(ccp(CLIENT_WIDTH/5*1+220, 75));
    twoItem -> setTag(eSuperTwoTag);
    CCSprite* pThree1 = CCSprite::createWithSpriteFrameName("SuperPirateThree.png");
    CCSprite* pThree2 = CCSprite::createWithSpriteFrameName("SuperPirateThree2.png");
    CCMenuItemSprite* threeItem = CCMenuItemSprite::create(pThree1, pThree2, this, menu_selector(GameLayer::gameCallBack));
    threeItem -> setPosition(ccp(CLIENT_WIDTH/5*2+130, 75));
    threeItem -> setTag(eSuperThreeTag);
    CCSprite* pFive1 = CCSprite::createWithSpriteFrameName("SuperPirateFive.png");
    CCSprite* pFive2 = CCSprite::createWithSpriteFrameName("SuperPirateFive2.png");
    CCMenuItemSprite* fiveItem = CCMenuItemSprite::create(pFive1, pFive2, this, menu_selector(GameLayer::gameCallBack));
    fiveItem -> setPosition(ccp(CLIENT_WIDTH/5*3+40, 75));
    fiveItem -> setTag(eSuperFiveTag);
    CCSprite* pCancelTwo = CCSprite::createWithSpriteFrameName("SuperPirateTwo2.png");
    CCMenuItemSprite* twoCanleItem = CCMenuItemSprite::create(pCancelTwo, pCancelTwo, this, menu_selector(GameLayer::gameCallBack));
    twoCanleItem -> setPosition(ccp(CLIENT_WIDTH/5*1+220, 75));
    twoCanleItem -> setTag(eSuperTwoCancleTag);
    CCSprite* pCancelThree = CCSprite::createWithSpriteFrameName("SuperPirateThree2.png");
    CCMenuItemSprite* threeCanleItem = CCMenuItemSprite::create(pCancelThree, pCancelThree, this, menu_selector(GameLayer::gameCallBack));
    threeCanleItem -> setPosition(ccp(CLIENT_WIDTH/5*2+130, 75));
    threeCanleItem -> setTag(eSuperThreeCancleTag);
    CCSprite* pCancelFive = CCSprite::createWithSpriteFrameName("SuperPirateFive2.png");
    CCMenuItemSprite* fiveCanleItem = CCMenuItemSprite::create(pCancelFive, pCancelFive, this, menu_selector(GameLayer::gameCallBack));
    fiveCanleItem -> setPosition(ccp(CLIENT_WIDTH/5*3+40, 75));
    fiveCanleItem -> setTag(eSuperFiveCancleTag);
    CCSprite* pCal1 = CCSprite::createWithSpriteFrameName("SuperPirateCal.png");
    CCSprite* pCal2 = CCSprite::createWithSpriteFrameName("SuperPirateCal2.png");
    CCMenuItemSprite* calcItem = CCMenuItemSprite::create(pCal1, pCal2, this, menu_selector(GameLayer::gameCallBack));
    calcItem -> setPosition(ccp(CLIENT_WIDTH/5*4+10, 88));
    calcItem -> setTag(eSuperCalcTag);
    m_pCalTTF = CCLabelTTF::create("", "Arial-BoldMT", 24);
    m_pCalTTF->setColor(ccRED);
    m_pCalTTF->setPosition(ccp(calcItem->getContentSize().width/2, calcItem->getContentSize().height/2-9));
    calcItem->addChild(m_pCalTTF);
    CCSprite* pNan1 = CCSprite::createWithSpriteFrameName("SuperPiratenan1.png");
    CCSprite* pNan2 = CCSprite::createWithSpriteFrameName("SuperPiratenan2.png");
    CCMenuItemSprite* nanItem = CCMenuItemSprite::create(pNan1, pNan2, this, menu_selector(GameLayer::gameCallBack));
    nanItem -> setPosition(ccp(770, CLIENT_HEIGHT/2-75));
    nanItem -> setTag(eSuperNanTag);
    CCSprite* pNv1 = CCSprite::createWithSpriteFrameName("SuperPiratenv1.png");
    CCSprite* pNv2 = CCSprite::createWithSpriteFrameName("SuperPiratenv2.png");
    CCMenuItemSprite* nvItem = CCMenuItemSprite::create(pNv1, pNv2, this, menu_selector(GameLayer::gameCallBack));
    nvItem -> setPosition(ccp(923, CLIENT_HEIGHT/2-75));
    nvItem -> setTag(eSuperNvTag);
    twoCanleItem->setVisible(false);
    threeCanleItem->setVisible(false);
    fiveCanleItem->setVisible(false);
    m_menuMain2 = CCMenu::create(twoItem, threeItem, fiveItem, twoCanleItem, threeCanleItem, fiveCanleItem, calcItem, nanItem, nvItem, NULL);
    m_menuMain2 -> setAnchorPoint(CCPointZero);
    m_menuMain2 -> setPosition(CCPointZero);
    m_node2 -> addChild(m_menuMain2, 100);
    
    m_PrizeScrollView = CCScrollView::create(CCSizeMake(187, 267));
    CCLayerColor * containLayer = CCLayerColor::create(ccc4(0, 0, 0, 0), 187, 267);
    m_PrizeScrollView->setContainer(containLayer);
    m_PrizeScrollView->setTouchEnabled(false);
    m_PrizeScrollView->setPosition(ccp(460, 180-50-3));
    m_node2->addChild(m_PrizeScrollView, 1);
    
    //白底
    CCSprite* pWhiteBg = CCSprite::createWithSpriteFrameName("SuperPirateNode1Bg.png");
    pWhiteBg->setScaleX(0.5f);
    pWhiteBg->setPosition(ccp(containLayer->getContentSize().width/2, containLayer->getContentSize().height/2));
    containLayer->addChild(pWhiteBg, 1);
    //显示结果图片
    m_spPrize = CCSprite::createWithSpriteFrameName("PrizeHero1.png");
    m_spPrize->setPosition(ccp(containLayer->getContentSize().width/2, containLayer->getContentSize().height/2));
    containLayer->addChild(m_spPrize, 1);
    CCSprite* pPrizeUp = CCSprite::createWithSpriteFrameName("PrizeHero2.png");
    pPrizeUp->setAnchorPoint(ccp(0.5, 0));
    pPrizeUp->setPosition(ccp(m_spPrize->getContentSize().width/2, m_spPrize->getContentSize().height+30));
    m_spPrize->addChild(pPrizeUp, 1, ePrizeUp);
    CCSprite* pPrizeDown = CCSprite::createWithSpriteFrameName("PrizeHero2.png");
    pPrizeDown->setAnchorPoint(ccp(0.5, 1));
    pPrizeDown->setPosition(ccp(m_spPrize->getContentSize().width/2, -30));
    m_spPrize->addChild(pPrizeDown, 1, ePrizeDown);
    m_spPrize->setVisible(false);
    
    CCSprite* pPrizeRollAnimateDown = CCSprite::createWithSpriteFrameName("prizeRoll.png");
    pPrizeRollAnimateDown->setAnchorPoint(ccp(0.5, 0));
    pPrizeRollAnimateDown->setPosition(ccp(containLayer->getContentSize().width/2, -100));
    pPrizeRollAnimateDown->setScaleX(1.0f);
    containLayer->addChild(pPrizeRollAnimateDown, 2, 2001);
    CCSprite* pPrizeRollAnimateUp = CCSprite::createWithSpriteFrameName("prizeRoll.png");
    pPrizeRollAnimateUp->setAnchorPoint(ccp(0.5, 0));
    pPrizeRollAnimateUp->setPosition(ccp(containLayer->getContentSize().width/2, pPrizeRollAnimateDown->getContentSize().height-200));
    pPrizeRollAnimateUp->setScaleX(1.0f);
    containLayer->addChild(pPrizeRollAnimateUp, 2, 2002);
    pPrizeRollAnimateDown->setVisible(false);
    pPrizeRollAnimateUp->setVisible(false);
    
    //两条竖线
    CCSprite* pNode2Vert1 = CCSprite::createWithSpriteFrameName("vert1.png");
    pNode2Vert1->setPosition(ccp(450, CLIENT_HEIGHT/2-100));
    m_node2->addChild(pNode2Vert1, 2);
    CCSprite* pNode2Vert2 = CCSprite::createWithSpriteFrameName("vert1.png");
    pNode2Vert2->setPosition(ccp(450+187+20, CLIENT_HEIGHT/2-100));
    m_node2->addChild(pNode2Vert2, 2);
    
    //遮罩
    CCSprite* pNode2Shade1 = CCSprite::createWithSpriteFrameName("SuperPirateshade.png");//prizeWhiteShade.png
    pNode2Shade1->setScaleX(4.7f);
    pNode2Shade1->setPosition(ccp(CLIENT_WIDTH/2+88, CLIENT_HEIGHT/2-32.5));
    m_node2->addChild(pNode2Shade1, 2);
    CCSprite* pNode2Shade2 = CCSprite::createWithSpriteFrameName("SuperPirateshade.png");//prizeWhiteShade.png
    pNode2Shade2->setFlipY(true);
    pNode2Shade2->setScaleX(4.7f);
    pNode2Shade2->setPosition(ccp(CLIENT_WIDTH/2+88, CLIENT_HEIGHT/2-32.5-pNode2Shade2->getContentSize().height));
    m_node2->addChild(pNode2Shade2, 2);
    
    //底注
    CCSprite* pMoneyBg0 = CCSprite::createWithSpriteFrameName("moneyBg0.png");
    pMoneyBg0->setPosition(ccp(846, 170));
    m_node2->addChild(pMoneyBg0, 2, 1000);
    pMoneyBg0->setScaleX(0.85f);
    pMoneyBg0->setScaleY(0.5f);
    CCSprite* pMoneyBg1 = CCSprite::createWithSpriteFrameName("moneyBg1.png");
    pMoneyBg1->setPosition(pMoneyBg0->getPosition());
    m_node2->addChild(pMoneyBg1, 2);
    CCLabelAtlas* pCurrentBetNum = CCLabelAtlas::create("0", "SuperPirate/currentBetNum.png", 35, 60, '0');
    pCurrentBetNum->setAnchorPoint(ccp(0.5f, 0.5f));
    pCurrentBetNum->setPosition(pMoneyBg0->getPosition());
    m_node2->addChild(pCurrentBetNum, 2, ePrizeBet);
    pCurrentBetNum->setScale(0.7f);
    
    //星星
    for (int i=0; i<8; i++)
    {
        CCSprite* grayStar = CCSprite::createWithSpriteFrameName("SuperPirateXing1.png");
        grayStar->setPosition(ccp(CLIENT_WIDTH/2+73*i-168, CLIENT_HEIGHT*0.7-88));
        m_node2->addChild(grayStar, 2);
        
        CCSprite* pBlindStar = CCSprite::createWithSpriteFrameName("prizeStarBlind_7.png");
        pBlindStar->setPosition(ccp(grayStar->getPositionX()+1, grayStar->getPositionY()));
        pBlindStar->setOpacity(50);
        pBlindStar->setScale(2.7f);
        m_node2->addChild(pBlindStar, 2, 120+i);
        
        CCSprite* colorStar = CCSprite::createWithSpriteFrameName("winStar_1.png");
        colorStar->setScale(1.2f);
        colorStar->setPosition(ccp(grayStar->getPositionX()+7, CLIENT_HEIGHT*0.7+45));
        m_node2->addChild(colorStar, 2, 110+i);
        colorStar->setVisible(false);
        
    }
    //5星之后播放金龙动画
    CCSprite* pDragon = CCSprite::createWithSpriteFrameName("dragon_1.png");
    pDragon->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    m_node2->addChild(pDragon, 1000, eDragon);
    pDragon->setVisible(false);
    
    CCSprite* pSpReceiveGuessEndBg = CCSprite::createWithSpriteFrameName("receiveGuessEnd.png");
    pSpReceiveGuessEndBg->setPosition(ccp(386, 262));
    m_node2->addChild(pSpReceiveGuessEndBg, 2);
    
    CCSize contSize =CCSizeMake(pSpReceiveGuessEndBg->getContentSize().width-4,pSpReceiveGuessEndBg->getContentSize().height-4);
    m_GuessScrollView = CCScrollView::create(contSize);
    CCLayerColor* pContainGuessLayer = CCLayerColor::create(ccc4(0, 0, 0, 0),contSize.width, contSize.height);
    m_GuessScrollView->setContainer(pContainGuessLayer);
    m_GuessScrollView->setTouchEnabled(false);
    m_GuessScrollView->setPosition(ccp(2, 2));
    pSpReceiveGuessEndBg->addChild(m_GuessScrollView, 1);
}

void GameLayer::setCmdLine(LPTSTR lpCmdLine)
{
	m_sCmdLine=lpCmdLine;
	wprintf(L"%s\n",lpCmdLine);
}

void GameLayer::ResetGameFrame()
{
}

void GameLayer::CloseGameFrame()
{
}

//接收服务端事件消息
bool GameLayer::OnEventSocket(const TCP_Command & Command, const void * pBuffer, WORD wDataSize)
{
	switch(Command.wSubCmdID)
	{
	case SUB_S_GAME_READY:
		{
			return OnSubReadyGame(pBuffer, wDataSize);
		}
		break;
	case SUB_S_START_GAME:
		{
			return OnSubStartGame(pBuffer, wDataSize);
		}
		break;
	case SUB_S_PICTURES_ROLL:		//转动
		{
			return OnSubPicRoll(pBuffer, wDataSize);
		}
		break;
	case SUB_S_SEND_RESLUT:			//转动结果
		{
			return OnSubReceiveResult(pBuffer, wDataSize);
		}
		break;
    case SUB_S_SEND_USEITEM_RESLUT:	//道具转动结果
        {
            return OnSubReceiveItemResult(pBuffer, wDataSize);
        }
        break;
	case SUB_S_CHANGE_BET:
		{
			return OnSubChangeBet(pBuffer, wDataSize);
		}
		break;
	case SUB_S_STOP_SCROLL:			//停止转动
		{
			return OnSubStopScroll(pBuffer, wDataSize);
		}
		break;
	case SUB_S_CLOWN_BET:
		{
			return OnSubClownBet(pBuffer, wDataSize);
		}
		break;
	case SUB_S_CHANGE_PRIZE:
		{
			return OnSubChangePrize(pBuffer, wDataSize);
		}
		break; 
	case SUB_S_GET_PRIZE:
		{
			return OnSubGetPrize(pBuffer, wDataSize);
		}
		break;
    case SUB_S_USER_CANT_PLAY:
        {
            return OnUserCantPlay(pBuffer, wDataSize);
        }
        break;
	}
	return false;
}

bool GameLayer::OnUserCantPlay(const void * pBuffer, WORD wDataSize)
{
    if (wDataSize != sizeof(CMD_S_CANT_PLAY_REMIND))
    {
        return false;
    }
    CMD_S_CANT_PLAY_REMIND* pCantPlayRemind = (CMD_S_CANT_PLAY_REMIND*)pBuffer;
    if(pCantPlayRemind != NULL)
    {
        m_bReceiveCoinCant = true;
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperScoreMinus);
        m_sCoinCantDes = CCharCode::GB2312ToUTF8(pCantPlayRemind->szDescribeString);
    }
    return true;
}

bool GameLayer::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return false;
}

//接收服务端场景消息
bool GameLayer::OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	switch(cbGameStation)
	{
	case GAME_SCENE_FREE:
		{
			OnSubReadyGame(pBuffer, wDataSize);
            m_bInitNETComplete = true;
            m_bReceiveGameScene = true;
            if(m_bInitUIComplete)
            {
                showGuideLayer();
            }
			return true;
		}
	case GAME_SCENE_GO:
		{
			if (sizeof(CMD_S_GAME_START) != wDataSize ) return false;

			CMD_S_START_STATUS  *pStartChip = (CMD_S_START_STATUS *) pBuffer;
			CMD_S_GAME_START startGame = {0};
			startGame.lGameNeed = pStartChip->lGameNeed;
			startGame.lUserScore = pStartChip->lUserScore;
			startGame.lDefultChip = pStartChip->lDefultChip;
			OnSubStartGame((void*)&startGame, sizeof(startGame));

			return true;
		}
	case GAME_SCENE_PICTURES_ROLL:
		{
			if (sizeof(CMD_S_PICTURES_ROLL) != wDataSize) return false;

			CMD_S_PICROLL_STATUS *picRollStatus = (CMD_S_PICROLL_STATUS*)pBuffer;
			CMD_S_PICTURES_ROLL picsRoll = {0};
			picsRoll.nAvailableTimes = picRollStatus->nAvailableTimes;
			picsRoll.lGameNeed = picRollStatus->lGameNeed;
			picsRoll.lUserScore = picRollStatus->lUserScore;
			picsRoll.lDefultChip = picRollStatus->lDefultChip;
			picsRoll.bIsRolling = picRollStatus->bIsRolling;
			OnSubPicRoll((void*)&picsRoll, sizeof(picsRoll));

			return true;
		}
	case GAME_SCENE_CHANGE_BET:
		{
			if (sizeof(CMD_S_CHANGE_BET) != wDataSize) return false;

			CMD_S_CHANGEBET_STATUS *changeBetStatus = (CMD_S_CHANGEBET_STATUS*)pBuffer;
			CMD_S_CHANGE_BET changeBet = {0};
			changeBet.lGameNeed = changeBetStatus->lGameNeed;
			changeBet.lUserScore = changeBetStatus->lUserScore;
			changeBet.lDefultChip = changeBetStatus->lDefultChip;
			OnSubChangeBet((void*)&changeBet, sizeof(changeBet));

			return true;
		}
	case GAME_SCENE_STOP:
		{
			if (sizeof(CMD_S_STOP_SCROLL) != wDataSize) return false;
			CMD_S_STOP_STATUS* stopStatus = (CMD_S_STOP_STATUS*)pBuffer;
			CMD_S_STOP_SCROLL stopRoll = {0};
			stopRoll.bIsRolling = stopStatus->bIsRolling;
			OnSubStopScroll((void*)&stopRoll, sizeof(stopRoll));

			return true;
		}
	case GAME_SCENE_CLOWN_BET:
		{
			if (sizeof(CMD_S_CLOWN_BET) != wDataSize) return false;

			CMD_S_CLOWNBET_STATUS *clownBetStatus = (CMD_S_CLOWNBET_STATUS*)pBuffer;
			CMD_S_CLOWN_BET clownBet = {0};
			clownBet.bChooseRight = clownBetStatus->bChooseRight;
			clownBet.lChipPrize = clownBetStatus->lChipPrize;
			clownBet.lUserScore = clownBetStatus->lUserScore;
			clownBet.nStarCount = clownBetStatus->nStarCount;
			clownBet.nClownIndex = clownBetStatus->nClownIndex;
			clownBet.nTimes = clownBetStatus->nTimes;
			clownBet.bLackChip = clownBetStatus->bLackChip;
			OnSubClownBet((void*)&clownBet, sizeof(clownBet));
			return true;
		}
	case GAME_SCENE_CHANGE_PRIZE:
		{
			if (sizeof(CMD_S_CHANGE_PRIZE) != wDataSize) return false;

			CMD_S_CHANGEPRIZE_STATUS *changePrizeStatus = (CMD_S_CHANGEPRIZE_STATUS*)pBuffer;
			CMD_S_CHANGE_PRIZE changePrize = {0};
			changePrize.lChipPrize = changePrizeStatus->lGameNeed;
			changePrize.lUserScore = changePrizeStatus->lUserScore;
			changePrize.nTimes = changePrize.nTimes;
			OnSubChangeBet((void*)&changePrize, sizeof(changePrize));

			return true;
		}
	case GAME_SCENE_END:
		{
			if (sizeof(CMD_S_GET_PRIZE) != wDataSize) return false;

			CMD_S_GETPRIZE_STATUS* gameEndStatus = (CMD_S_GETPRIZE_STATUS*)pBuffer;
			CMD_S_GET_PRIZE gameEnd = {0};
			gameEnd.wUserID = gameEndStatus->wUserID;
			gameEnd.lFinalPrize = gameEndStatus->lFinalPrize;
			gameEnd.lChipPrize = gameEndStatus->lChipPrize;
			gameEnd.lUserScore = gameEndStatus->lUserScore;
			gameEnd.lDefultChip = gameEndStatus->lDefultChip;
			gameEnd.lCurrentChip = gameEndStatus->lCurrentChip;
			gameEnd.nStarCount = gameEndStatus->nStarCount;
			gameEnd.nTimes = gameEndStatus->nTimes;
			gameEnd.bLackChip = gameEndStatus->bLackChip;
			gameEnd.nPrizeTimes = gameEndStatus->nPrizeTimes;
			OnSubGetPrize((void*)&gameEnd, sizeof(gameEnd));

			return true;
		}
	}
	return false;
}

//游戏准备
bool GameLayer::OnSubReadyGame(const void * pBuffer, WORD wDataSize)
{
    m_nTimesOnline = 5;
	if (wDataSize != sizeof(CMD_S_GAME_READY))
    {
        return false;
    }
	CMD_S_GAME_READY* pGameReady = (CMD_S_GAME_READY*)pBuffer;
    m_lUserScore = pGameReady->lUserScore;
    m_lChipScore = pGameReady->lGameNeed;
    m_lDefultScore = pGameReady->lDefultChip;
    m_nItemNum = pGameReady->nItemNum;
    m_nFiveStarExtra = pGameReady->nFiveStarExtralReward;
    m_nSixStarExtra = pGameReady->nSixStarExtralReward;
    m_nSevenStarExtra = pGameReady->nSevenStarExtralReward;
    m_nEightStarExtra = pGameReady->nEightStarExtralReward;
    
    char reayScore[32];
    sprintf(reayScore, "%lld", m_lUserScore);
    m_labelUserScore -> setString(reayScore);
    sprintf(reayScore, "%lld", m_lChipScore);
    if(m_bEnterGuided)
    {
        m_labelCurrentYa -> setString(reayScore);
        m_nCurrentYaZhu = m_lChipScore;
    }
    else
    {
        m_labelCurrentYa -> setString(reayScore);
    }
	return true;
}

//开始游戏
bool GameLayer::OnSubStartGame(const void * pBuffer, WORD wDataSize)
{
    m_nTimesOnline = 5;
	return true;
}

bool GameLayer::OnSubReceiveResult(const void * pBuffer, WORD wDataSize)
{
    m_nTimesOnline = 5;
	if (wDataSize != sizeof(CMD_S_SEND_RESULT))
    {
        return false;
    }
	CMD_S_SEND_RESULT* sendResult = (CMD_S_SEND_RESULT*)pBuffer;
    m_bSame = sendResult->bPictureIsAllSame;
    m_bLackChip = sendResult->bLackChip;
    m_lUserScore  = sendResult->lUserScore;
    m_nStarCount = sendResult->nStarCount;
    m_lChipScore = sendResult->lCurrentChip;
    m_lRollPrize = sendResult->lCurrentChip;//假如要相同，是第二个界面的基础赌注
    m_lDefultScore = sendResult->lDefultChip;
    m_nRollerID[0] = sendResult->bResultData[0];//图片ID
    m_nRollerID[1] = sendResult->bResultData[1];
    m_nRollerID[2] = sendResult->bResultData[2];
    m_bReceiveID = true;
    m_lTotalScore = m_lUserScore+m_lChipScore;
	return true;
}
bool GameLayer::OnSubReceiveItemResult(const void * pBuffer, WORD wDataSize)
{
    m_nTimesOnline = 5;
    if (wDataSize != sizeof(CMD_S_USEITEM_SEND_RESULT))
    {
        return false;
    }
    CMD_S_USEITEM_SEND_RESULT* sendResult = (CMD_S_USEITEM_SEND_RESULT*)pBuffer;
    m_bSame = sendResult->bPictureIsAllSame;
    m_bLackChip = sendResult->bLackChip;
    m_lUserScore  = sendResult->lUserScore;
    m_nStarCount = sendResult->nStarCount;
    m_lChipScore = sendResult->lCurrentChip;
    m_lRollPrize = sendResult->lCurrentChip;//假如要相同，是第二个界面的基础赌注
    m_lDefultScore = sendResult->lDefultChip;
    m_nRollerID[0] = sendResult->bResultData[0];//图片ID
    m_nRollerID[1] = sendResult->bResultData[1];
    m_nRollerID[2] = sendResult->bResultData[2];
    m_nItemNum = sendResult->nItemNum;
    m_bReceiveID = true;
    m_lTotalScore = m_lUserScore+m_lChipScore;
    return true;
}
//开始转动
bool GameLayer::OnSubPicRoll(const void * pBuffer, WORD wDataSize)
{
    m_nTimesOnline = 5;
	if (wDataSize != sizeof(CMD_S_PICTURES_ROLL))
    {
        return false;
    }

	return true;
}

//改变赌注(Rolling)
bool GameLayer::OnSubChangeBet(const void * pBuffer, WORD wDataSize)
{
    m_nTimesOnline = 5;
	if (wDataSize != sizeof(CMD_S_CHANGE_BET))
    {
        return false;
    }
	CMD_S_CHANGE_BET* changeBet = (CMD_S_CHANGE_BET*)pBuffer;
    m_lUserScore = changeBet->lUserScore;
    m_lChipScore = changeBet->lGameNeed;
    m_lDefultScore = changeBet->lDefultChip;
    char sChangBet[32];
    sprintf(sChangBet, "%lld", changeBet->lGameNeed);
    m_labelCurrentYa -> setString(sChangBet);
    
    return true;
}

//停止Rolling,分析转动结果(服务器返回stop结果)
bool GameLayer::OnSubStopScroll(const void * pBuffer, WORD wDataSize)
{
    m_nTimesOnline = 5;
	return true;
}

//下注小丑结果
bool GameLayer::OnSubClownBet(const void * pBuffer, WORD wDataSize)
{
    m_nTimesOnline = 5;
	if (wDataSize != sizeof(CMD_S_CLOWN_BET))
    {
        return false;
    }
    this->unschedule(schedule_selector(GameLayer::TimerToAutoCalculate));
    m_nCalTime = 0;
    this->schedule(schedule_selector(GameLayer::TimerToAutoCalculate), 1.0, 15, 0.01f);
    
	CMD_S_CLOWN_BET* clownBet = (CMD_S_CLOWN_BET*)pBuffer;
    m_lTotalScore = clownBet->lUserScore + clownBet->lChipPrize;
    m_lRollPrize = clownBet->lChipPrize;
    if (clownBet->bChooseRight)
    {
        m_nPrizeID = m_nSelectID;
        //m_lRollPrize = clownBet->lChipPrize;
        m_nStarCount = clownBet->nStarCount;
        m_bLackToGuess = false;
        playPrizeBetChangeAnimate(m_lRollPrize);
    
        CCSprite* spStar = (CCSprite*)m_node2->getChildByTag(110+m_nStarCount-1);
        if (spStar != NULL )
        {
            spStar->setVisible(true);
            spStar->runAction(CCSequence::create(CCShow::create(), getAnimate(32, "winStar", 0.07), NULL));
        }
        
        if (m_nStarCount == 5 || m_nStarCount == 6)
        {
            SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/star5Sound.mp3");
        }
        else if (m_nStarCount == 7 || m_nStarCount == 8)
        {
            SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/star7Sound.mp3");
        }
        
        if (m_nStarCount>=5)
        {
            CCSprite* pDragon = (CCSprite*)m_node2->getChildByTag(eDragon);
            if (pDragon != NULL)
            {
                pDragon->runAction(CCSequence::create(CCShow::create(), getAnimate(25, "dragon", 0.05), CCHide::create(), NULL));
                //震屏
                shakeScreen();
            }
        }
        if(m_nStarCount == 1)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult12);
        }
        else if(m_nStarCount == 2)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult13);
        }
        else if(m_nStarCount == 3)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult14);
        }
        else if(m_nStarCount == 4)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult15);
        }
        else if(m_nStarCount == 5)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult16);
        }
        else if(m_nStarCount == 6)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult17);
        }
        else if(m_nStarCount == 7)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult18);
        }
        else if(m_nStarCount == 8)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult19);
        }
        CCSprite* pBlindStar = (CCSprite*)m_node2->getChildByTag(120+m_nStarCount-1);
        if (pBlindStar != NULL)
        {
            pBlindStar->stopAllActions();
            pBlindStar->setOpacity(0);
            pBlindStar->setScale(2.5f);
            pBlindStar->setVisible(true);
            CCSpawn* spawnBig = CCSpawn::create(CCFadeTo::create(0.5f,100), CCScaleTo::create(0.5f, 3.0f), NULL);
            CCSpawn* spawnNormal = CCSpawn::create(CCFadeTo::create(0.5f,0), CCScaleTo::create(0.5f, 2.5f),NULL);
            CCSequence* seqForever = CCSequence::create(spawnBig, spawnNormal, NULL);
            CCRepeatForever* forever = CCRepeatForever::create(seqForever);
            pBlindStar->runAction(forever);
        }
        
        if (m_nStarCount == 8)
        {
            setMenuTouchEnable(false, m_menuMain2);
            this->unschedule(schedule_selector(GameLayer::TimerToAutoCalculate));
            CCNode* pNode = CCNode::create();
            this->addChild(pNode);
            pNode->runAction(CCSequence::create(CCDelayTime::create(1.5f), CCCallFunc::create(this, callfunc_selector(GameLayer::sendAutoCalculate)), CCCallFuncN::create(this, callfuncN_selector(GameLayer::delayRemoveShouJinAnimate)),NULL));
        }
        setMenuItemVisible(eSuperTwoTag, true, m_menuMain2);
        setMenuItemVisible(eSuperThreeTag, true, m_menuMain2);
        setMenuItemVisible(eSuperFiveTag, true, m_menuMain2);
        setMenuItemVisible(eSuperTwoCancleTag, false, m_menuMain2);
        setMenuItemVisible(eSuperThreeCancleTag, false, m_menuMain2);
        setMenuItemVisible(eSuperFiveCancleTag, false, m_menuMain2);
    }
    else
    {
        setMenuItemVisible(eSuperTwoTag, true, m_menuMain2);
        setMenuItemVisible(eSuperThreeTag, true, m_menuMain2);
        setMenuItemVisible(eSuperFiveTag, true, m_menuMain2);
        setMenuItemVisible(eSuperTwoCancleTag, false, m_menuMain2);
        setMenuItemVisible(eSuperThreeCancleTag, false, m_menuMain2);
        setMenuItemVisible(eSuperFiveCancleTag, false, m_menuMain2);
        
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult20);
        m_nPrizeID = 1-m_nSelectID;//取反
        m_lUserScore = clownBet->lUserScore;
        char sUserScore[32];
        sprintf(sUserScore, "%lld", m_lUserScore);
        m_labelUserScore->setString(sUserScore);
        
        float fTime = 0.6f;
        char sPrize[16];
        CCLabelAtlas* pCurrentBetNum = (CCLabelAtlas*)m_node2->getChildByTag(ePrizeBet);
        if (pCurrentBetNum != NULL) {
            sprintf(sPrize, "%s", pCurrentBetNum->getString());
//            sprintf(sPrize, "%lld", m_lChipScore);
        }
        CCLabelAtlas* pLoseScoreTTF = CCLabelAtlas::create(sPrize, "SuperPirate/loseNum.png", 35, 60, '0');
        pLoseScoreTTF->setPosition(ccp(500, 200));//846, 170
        pBackground->addChild(pLoseScoreTTF, 10);
        pLoseScoreTTF->setOpacity(0);
        CCSprite* pLeftWing = CCSprite::createWithSpriteFrameName("loseWing.png");
        pLeftWing->setPosition(ccp(-pLeftWing->getContentSize().width*0.7f, pLoseScoreTTF->getContentSize().height/2));
        pLoseScoreTTF->addChild(pLeftWing);
        CCRotateBy* pLeftRotateBy1 = CCRotateBy::create(fTime/6, 16);
        CCRotateBy* pLeftRotateBy2 = CCRotateBy::create(fTime/6, -16);
        CCSequence* pLeftSeq = CCSequence::create(pLeftRotateBy1, pLeftRotateBy2, NULL);
        CCRepeat* pLeftRepeat = CCRepeat::create(pLeftSeq, 3);
        pLeftWing->runAction(pLeftRepeat);
        
        CCSprite* pRightWing = CCSprite::createWithSpriteFrameName("loseWing.png");
        pRightWing->setFlipX(true);
        pRightWing->setPosition(ccp(pLoseScoreTTF->getContentSize().width+pLeftWing->getContentSize().width*0.7f, pLoseScoreTTF->getContentSize().height/2));
        pLoseScoreTTF->addChild(pRightWing);
        CCRotateBy* pRightRotateBy1 = CCRotateBy::create(fTime/6, -16);
        CCRotateBy* pRightRotateBy2 = CCRotateBy::create(fTime/6, 16);
        CCSequence* pRightSeq = CCSequence::create(pRightRotateBy1, pRightRotateBy2, NULL);
        CCRepeat* pRightRepeat = CCRepeat::create(pRightSeq, 3);
        pRightWing->runAction(pRightRepeat);
        
        CCFadeIn* pFadeIn = CCFadeIn::create(fTime/8);
        ccBezierConfig tr0;
        tr0.endPosition=ccp(-140, 150);
        tr0.controlPoint_1=ccp(-40, 0);
        tr0.controlPoint_2=ccp(-160, 0);
        CCActionInterval* bezierForward = CCBezierBy::create(fTime*7/8, tr0);
        CCDelayTime* pDelay = CCDelayTime::create(fTime*7/16);
        CCFadeOut* pFadeOut = CCFadeOut::create(fTime*7/16);
        CCSequence* pSeq = CCSequence::create(pDelay, pFadeOut, NULL);
        CCSpawn* pSpawn = CCSpawn::create(pSeq, bezierForward, NULL);
        CCCallFuncN* pCallFuncN = CCCallFuncN::create(this, callfuncN_selector(GameLayer::removeOutSide));
        CCSequence* pSeq2 = CCSequence::create(pFadeIn, pSpawn, pCallFuncN, NULL);
        pLoseScoreTTF->runAction(pSeq2);
    }
    
    dealGuessResult();
    
	if (clownBet->bLackChip)//不足下注
    {
        m_lRollPrize = clownBet->lChipPrize;
        m_lChipScore = clownBet->lChipPrize;
        char sChipScore[32];
        sprintf(sChipScore, "%lld", clownBet->lChipPrize);
        m_labelCurrentYa -> setString(sChipScore);
        m_bLackToGuess = true;
        //发送结算消息
        sendAutoCalculate();
	}

	return true;
}

//改变猜小丑赌注
bool GameLayer::OnSubChangePrize(const void * pBuffer, WORD wDataSize)
{
    m_nTimesOnline = 5;
	if (wDataSize != sizeof(CMD_S_CHANGE_PRIZE))
    {
        return false;
    }
	CMD_S_CHANGE_PRIZE* changePrize = (CMD_S_CHANGE_PRIZE*)pBuffer;
    m_lChipScore = changePrize->lChipPrize;
    m_lUserScore = changePrize->lUserScore;
    
    char sUserScore[32];
    sprintf(sUserScore, "%lld", changePrize->lUserScore);
    m_labelUserScore->setString(sUserScore);
    if (m_lChipScore != m_lNativePrize)
    {
        CCLabelAtlas* pCurrentBetNum = (CCLabelAtlas*)m_node2->getChildByTag(ePrizeBet);
        if (pCurrentBetNum != NULL)
        {
            char sChipScore[32];
            sprintf(sChipScore, "%lld", m_lChipScore);
            pCurrentBetNum->setString(sChipScore);
        }
    }
    
	return true;
}

//收金
bool GameLayer::OnSubGetPrize(const void * pBuffer, WORD wDataSize)
{
    m_nTimesOnline = 5;
	if (wDataSize != sizeof(CMD_S_GET_PRIZE))
    {
        return false;
    }
	CMD_S_GET_PRIZE* gameEnd = (CMD_S_GET_PRIZE*)pBuffer;
    m_lUserScore = gameEnd->lUserScore;
    m_lChipScore = gameEnd->lChipPrize;
    m_lDefultScore = gameEnd->lDefultChip;
    m_lGetPrizeExtra = gameEnd->lFinalPrize;
    playGetPrizeAnimate();
    return true;
}

void GameLayer::OnEventUserMemberOrder(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)
{
}

void GameLayer::OnEventUserStatus(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)
{
}

bool GameLayer::OnEventUserLeave(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)
{
	return false;
}

bool GameLayer::OnEventUserEnter(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)
{
	return false;
}

void GameLayer::OnEventUserScore(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)
{
}

void GameLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    if(item == NULL)
        return;
    
    if(item->getTag() != eSuperRankCloseTag)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        item->runAction(CCSequence::create(CCScaleTo::create(0.1f, 0.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
    }
    else
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    }
    
    gameCallBack(item);
}

void GameLayer::exitGame()
{
    //停止心跳包检测
    this -> unschedule(schedule_selector(GameLayer::checkGameStatusOnline));
    this -> unschedule(schedule_selector(GameLayer::disConnectGame));
    if(CMainLogic::sharedMainLogic()->m_bGameChargeStatus)
    {
        m_bShowExitGame = true;
    }
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    CMainLogic::sharedMainLogic()->setPlaying(false);
    CMainLogic::sharedMainLogic()->m_bAutoExitGame = true;
    tagUserInfo * pUserInfo = m_pClientKernel->GetMeUserInfo();
    CMainLogic::sharedMainLogic()->SendStandUpPacket(pUserInfo->wTableID, pUserInfo->wChairID, 1);
}

void GameLayer::gameCallBack(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    if(item == NULL)
        return;
    switch (item->getTag())
    {
        case eSuperBackTag://退出按钮
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateExitTag);
            
            //SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/rankSound.mp3");
            exitGame();
            break;
        }
        case eSuperChargeTag://充值按钮
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateChargeTag);
            
            ChargeLayer* pChargeLayer = ChargeLayer::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeLayer, 11, 20000);
            
            break;
        }
        case eSuperRankTag://排行按钮
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateRankTag);
            
            m_pRankSp->setVisible(true);
            m_rankLayer->setVisible(true);
            m_pRankSp->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
            int nUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            char temp[256];
            sprintf(temp, "http://%s/GetSuperRank.aspx?uid=%d",NORMAL_URL,nUserID);
            CCHttpRequest* request = new CCHttpRequest();
            request -> setUrl(temp);
            request -> setRequestType(CCHttpRequest::kHttpGet);
            request -> setTag("http test");
            request -> setResponseCallback(this, httpresponse_selector(GameLayer::onHttpRankComplete));
            CCHttpClient::getInstance()->send(request);
            request -> release();
            break;
        }
        case eSuperHelpTag:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateHelpTag);
            SuperHelpLayer* pSuperHelpLayer = SuperHelpLayer::create();
            pBackground -> addChild(pSuperHelpLayer, 200);
            break;
        }
        case eSuperVoiceOpenTag://关闭声音
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateSoundOffTag);
            
            m_pVoiceOpenItem->setVisible(false);
            m_pVoiceCloseItem->setVisible(true);
            m_fSound = CCUserDefault::sharedUserDefault()->getFloatForKey("sound", 1);
            m_fEffect = CCUserDefault::sharedUserDefault()->getFloatForKey("effect", 1);
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
            break;
        }
        case eSuperVoiceCloseTag://打开声音
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateSoundOnTag);
            
            m_pVoiceOpenItem->setVisible(true);
            m_pVoiceCloseItem->setVisible(false);
            CCUserDefault::sharedUserDefault()->getFloatForKey("sound", m_fSound);
            CCUserDefault::sharedUserDefault()->getFloatForKey("effect", m_fEffect);
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_fSound);
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_fEffect);
            break;
        }
        case eSuperRankCloseTag://排行关闭按钮
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateRankCloseTag);
            
            CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(GameLayer::hideRankLayerColor));
            m_pRankSp->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, CCHide::create(), NULL));
            break;
        }
        case eSuperMinusTag://减注
        {
            if (m_nItemNum>0)
            {
                char temp[128];
                sprintf(temp, "使用藏宝图时固定底注为%lld,无法更改底注", m_lChipScore);
                CMainLogic::sharedMainLogic()->ShowMessage(temp, eMBSuperOK);
                break;
            }
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateMinusBetTag);
            
            SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/clickSound.mp3");
            if (m_nItemNum>0)
            {
                break;
            }
            sendMinusGameBet();
            break;
        }
        case eSuperPlusTag://加注
        {
            if (m_nItemNum>0)
            {
                char temp[128];
                sprintf(temp, "使用藏宝图时固定底注为%lld,无法更改底注", m_lChipScore);
                CMainLogic::sharedMainLogic()->ShowMessage(temp, eMBSuperOK);
                break;
            }
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPiratePlusBetTag);
            
            SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/clickSound.mp3");
            if (m_nItemNum>0)
            {
                break;
            }
            sendPlusGameBet();
            break;
        }
        case eSuperAutoTag://自动
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateAutoTag);
            
            if (m_bAuto)
            {
                return;
            }
            if(m_bReceiveCoinCant)
            {
                CMainLogic::sharedMainLogic()->ShowMessage(m_sCoinCantDes.c_str(), eMBSuperOK, eExitSuperToHall);
                break;
            }
            if ((m_lUserScore >= m_lChipScore && m_lUserScore>0) || m_nItemNum > 0)
            {
                SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/autoSound.mp3");
                m_bAuto = true;
                setMenuItemVisible(eSuperAutoTag, false, m_menuMain1);
                setMenuItemVisible(eSuperCancelAutoTag, true, m_menuMain1);
                if(!m_bRolling)
                {
                    sendGameRoll();
                }
            }
            else
            {
                CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，金币不足了。您可以充值后继续游戏。", eMBSuperOK, eShowSuperCharge);
            }
            break;
        }
        case eSuperCancelAutoTag://取消自动
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateCancelAutoTag);
            
            m_bAuto = false;
            setMenuItemVisible(eSuperCancelAutoTag, false, m_menuMain1);
            setMenuItemVisible(eSuperAutoTag, true, m_menuMain1);
            if (m_lUserScore<m_lChipScore || m_lUserScore<=0)
            {
                setMenuItemTouchEnable(eSuperStartTag, true, m_menuMain1);
                setMenuItemTouchEnable(eSuperMinusTag, true, m_menuMain1);
                setMenuItemTouchEnable(eSuperPlusTag, true, m_menuMain1);
            }
            break;
        }
        case eSuperStartTag://开始转动
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateGoTag);
            if(m_bReceiveCoinCant)
            {
                CMainLogic::sharedMainLogic()->ShowMessage(m_sCoinCantDes.c_str(), eMBSuperOK, eExitSuperToHall);
                break;
            }
            if (m_lUserScore >= m_lChipScore && m_lUserScore>0)
            {
                if (m_bAuto)
                {
                    break;
                }
                SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/clickSound.mp3");
                sendGameRoll();
            }
            else
            {
                CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，金币不足了。您可以充值后继续游戏。", eMBSuperOK, eShowSuperCharge);
            }
            break;
        }
        case eSuperItemStartTag://开始转动
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateGoTag);
            if (m_bAuto)
            {
                break;
            }
            SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/clickSound.mp3");
            sendGameRoll();
            break;
        }
        case eSuperStopTag:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateStopTag);
            
        	if (!m_bReceiveID)
			{
                break;
            }
            if (m_bAuto)
            {
                break;
            }
            SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/stopRoll.mp3");
            this->unschedule(schedule_selector(GameLayer::TimerToStopRoll));
            TimerToStopRoll(0);
            break;
        }
        case eSuperTwoTag://2倍
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateTwoPrizeTag);
            
            SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/clickSound.mp3");
            sendChangePrizeTimes(2);
            break;
        }
        case eSuperThreeTag://3倍
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateThreePrizeTag);
            
            SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/clickSound.mp3");
           	sendChangePrizeTimes(3);
            break;
        }
        case eSuperFiveTag://5倍
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateFivePrizeTag);
            
            SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/clickSound.mp3");
           	sendChangePrizeTimes(5);
            break;
        }
        case eSuperTwoCancleTag://2倍取消
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateCancelTwoPrizeTag);
            
            sendCancelChangePrizeTimes(2);
            break;
        }
        case eSuperThreeCancleTag://3倍取消
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateCancelThreePrizeTag);
            
            sendCancelChangePrizeTimes(3);
            break;
        }
        case eSuperFiveCancleTag://5倍取消
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateCancelFivePrizeTag);
            
            sendCancelChangePrizeTimes(5);
            break;
        }
        case eSuperNanTag://男
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateNanTag);
            if(m_bReceiveCoinCant)
            {
                CMainLogic::sharedMainLogic()->ShowMessage(m_sCoinCantDes.c_str(), eMBSuperOK, eExitSuperToHall);
                break;
            }
            m_nSelectID = 0;
            CMD_C_CLOWN_BET colorClown = {0};
            m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_CLOWN_BET, &colorClown, sizeof(colorClown));
            this->unschedule(schedule_selector(GameLayer::TimerToAutoCalculate));
            m_nCalTime = 0;
            this->schedule(schedule_selector(GameLayer::TimerToAutoCalculate), 1.0, 15, 0.01f);
            setGuessItemTouchEnable(false);
            break;
        }
        case eSuperNvTag://女
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateNvTag);
            if(m_bReceiveCoinCant)
            {
                CMainLogic::sharedMainLogic()->ShowMessage(m_sCoinCantDes.c_str(), eMBSuperOK, eExitSuperToHall);
                break;
            }
            m_nSelectID = 1;
            CMD_C_CLOWN_BET colorClown = {0};
            m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_CLOWN_BET, &colorClown, sizeof(colorClown));
            this->unschedule(schedule_selector(GameLayer::TimerToAutoCalculate));
            m_nCalTime = 0;
            this->schedule(schedule_selector(GameLayer::TimerToAutoCalculate), 1.0, 15, 0.01f);
            setGuessItemTouchEnable(false);
            break;
        }
        case eSuperCalcTag://结算
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateGetPrizeTag);
            
            if(!m_bEnterGuided)
            {
                this->unschedule(schedule_selector(GameLayer::TimerToAutoCalculate));
                sendAutoCalculate();
            }
            
            break;
        }
        default:
            break;
    }
}

//发送++消息
void GameLayer::sendPlusGameBet()
{
    if (m_lUserScore>0)
    {
        CMD_C_CHANGE_BET changeBet = {0};
        changeBet.bPlusChip = true;
        changeBet.bMinusChip = false;
        m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_CHANGE_BET, &changeBet, sizeof(changeBet));
    }
}

//发送--消息
void GameLayer::sendMinusGameBet()
{
    if (m_lUserScore>0)
    {
        CMD_C_CHANGE_BET changeBet = {0};
        changeBet.bPlusChip = false;
        changeBet.bMinusChip = true;
        m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_CHANGE_BET, &changeBet, sizeof(changeBet));
    }
}

//发送转动消息
void GameLayer::sendGameRoll()
{
    if(m_bReceiveCoinCant)
    {
        CMainLogic::sharedMainLogic()->ShowMessage(m_sCoinCantDes.c_str(), eMBSuperOK, eExitSuperToHall);
        return ;
    }
    SimpleAudioEngine::sharedEngine()->stopEffect(m_nGameRollingEffectID);
    if ((m_lUserScore>=m_lChipScore && m_lChipScore>0) || m_nItemNum>0)
    {
        setMenuItemVisible(eSuperItemStartTag, false, m_menuMain1);
        setMenuItemVisible(eSuperStartTag, false, m_menuMain1);
        setMenuItemVisible(eSuperStopTag, true, m_menuMain1);
        setMenuItemTouchEnable(eSuperMinusTag, false, m_menuMain1);
        setMenuItemTouchEnable(eSuperPlusTag, false, m_menuMain1);
        if (m_bAuto)
        {
            setMenuItemTouchEnable(eSuperStartTag, false, m_menuMain1);
            setMenuItemTouchEnable(eSuperStopTag, false, m_menuMain1);
        }
        else
        {
            setMenuItemTouchEnable(eSuperStartTag, true, m_menuMain1);
            setMenuItemTouchEnable(eSuperStopTag, true, m_menuMain1);
        }
        m_bRolling = true;
        m_bReceiveID = false;
        redLightScroll();
        
        if (m_nItemNum<=0)
        {
            LONGLONG lUserScore = m_lUserScore - m_lChipScore;
            char tmp[32];
            sprintf(tmp, "%lld", lUserScore);
            m_labelUserScore->setString(tmp);
            CMD_C_START_GAME startGame = {0};
            m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_GO_GAME, &startGame, sizeof(startGame));
        }
        else
        {
            setItemNum(m_nItemNum-1);
            CMD_C_START_GAME startGame = {0};
            m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_GO_GAME_ITEM, &startGame, sizeof(startGame));
        }
        
        startGameRoll();
        
        if(m_bAuto)
        {
            this->unschedule(schedule_selector(GameLayer::TimerToStopRoll));
            this->schedule(schedule_selector(GameLayer::TimerToStopRoll), 0.5f);
        }
        else
        {
            this->unschedule(schedule_selector(GameLayer::TimerToStopRoll));
            this->scheduleOnce(schedule_selector(GameLayer::TimerToStopRoll), 4.0f);
        }
    }
    else
    {
        //积分不足等原因
        CMainLogic::sharedMainLogic()->ShowMessage("当前分数不足，请先充值", eMBSuperOK, eShowSuperCharge);
        
        m_bRolling = false;
        m_bAuto = false;
        setMenuItemVisible(eSuperCancelAutoTag, false, m_menuMain1);
        setMenuItemVisible(eSuperAutoTag, true, m_menuMain1);
        setMenuItemTouchEnable(eSuperCancelAutoTag, true, m_menuMain1);
        setMenuItemTouchEnable(eSuperAutoTag, true, m_menuMain1);
        setMenuItemTouchEnable(eSuperStartTag, true, m_menuMain1);
        setMenuItemTouchEnable(eSuperStopTag, true, m_menuMain1);
        setMenuItemTouchEnable(eSuperMinusTag, true, m_menuMain1);
        setMenuItemTouchEnable(eSuperPlusTag, true, m_menuMain1);
    }
}

//发送改变赌注2、3、5
void GameLayer::sendChangePrizeTimes(int nTimes)
{
    LONGLONG totalScore = m_lTotalScore;
    bool bEnought=false;
    if (totalScore >= nTimes*m_lRollPrize)
    {
        bEnought=true;
    }
    if (!bEnought)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("您当前金币不足", eMBSuperOK, eShowSuperCharge);
    }
    else
    {
        if (nTimes == 2)
        {
            setMenuItemVisible(eSuperTwoTag, false, m_menuMain2);
            setMenuItemVisible(eSuperTwoCancleTag, true, m_menuMain2);
            setMenuItemVisible(eSuperFiveTag, true, m_menuMain2);
            setMenuItemVisible(eSuperFiveCancleTag, false, m_menuMain2);
            setMenuItemVisible(eSuperThreeTag, true, m_menuMain2);
            setMenuItemVisible(eSuperThreeCancleTag, false, m_menuMain2);
        }
        else if (nTimes == 3)
        {
            setMenuItemVisible(eSuperThreeTag, false, m_menuMain2);
            setMenuItemVisible(eSuperThreeCancleTag, true, m_menuMain2);
            setMenuItemVisible(eSuperFiveTag, true, m_menuMain2);
            setMenuItemVisible(eSuperFiveCancleTag, false, m_menuMain2);
            setMenuItemVisible(eSuperTwoTag, true, m_menuMain2);
            setMenuItemVisible(eSuperTwoCancleTag, false, m_menuMain2);
        }
        else if (nTimes == 5)
        {
            setMenuItemVisible(eSuperFiveTag, false, m_menuMain2);
            setMenuItemVisible(eSuperFiveCancleTag, true, m_menuMain2);
            setMenuItemVisible(eSuperTwoTag, true, m_menuMain2);
            setMenuItemVisible(eSuperTwoCancleTag, false, m_menuMain2);
            setMenuItemVisible(eSuperThreeTag, true, m_menuMain2);
            setMenuItemVisible(eSuperThreeCancleTag, false, m_menuMain2);
        }
        char temp[32];
        playPrizeBetChangeAnimate(nTimes*m_lRollPrize);
        sprintf(temp, "%lld", totalScore-nTimes*m_lRollPrize);
        m_nMenuIndex = nTimes;
        m_labelUserScore->setString(temp);
        CMD_C_CHANGE_PRIZE changePrize = {0};
        changePrize.lChipPrize = m_lChipScore;
        changePrize.lUserScore = m_lUserScore;
        changePrize.nTimes = nTimes;
        m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_CHANGE_PRIZE, &changePrize, sizeof(changePrize));
    }
}

//发送取消改变2、3、5
void GameLayer::sendCancelChangePrizeTimes(int nTimes)
{
    if (nTimes == 2)
    {
        setMenuItemVisible(eSuperTwoTag, true, m_menuMain2);
        setMenuItemVisible(eSuperTwoCancleTag, false, m_menuMain2);
        setMenuItemVisible(eSuperThreeTag, true, m_menuMain2);
        setMenuItemVisible(eSuperThreeCancleTag, false, m_menuMain2);
        setMenuItemVisible(eSuperFiveTag, true, m_menuMain2);
        setMenuItemVisible(eSuperFiveCancleTag, false, m_menuMain2);
    }
    else if (nTimes == 3)
    {
        setMenuItemVisible(eSuperThreeTag, true, m_menuMain2);
        setMenuItemVisible(eSuperThreeCancleTag, false, m_menuMain2);
        setMenuItemVisible(eSuperTwoTag, true, m_menuMain2);
        setMenuItemVisible(eSuperTwoCancleTag, false, m_menuMain2);
        setMenuItemVisible(eSuperFiveTag, true, m_menuMain2);
        setMenuItemVisible(eSuperFiveCancleTag, false, m_menuMain2);
    }
    else if (nTimes == 5)
    {
        setMenuItemVisible(eSuperFiveTag, true, m_menuMain2);
        setMenuItemVisible(eSuperFiveCancleTag, false, m_menuMain2);
        setMenuItemVisible(eSuperTwoTag, true, m_menuMain2);
        setMenuItemVisible(eSuperTwoCancleTag, false, m_menuMain2);
        setMenuItemVisible(eSuperThreeTag, true, m_menuMain2);
        setMenuItemVisible(eSuperThreeCancleTag, false, m_menuMain2);
    }
    char temp[32];
    playPrizeBetChangeAnimate(m_lRollPrize);
    if(m_nMenuIndex == 2)
    {
        sprintf(temp, "%lld", m_lUserScore+m_lRollPrize);
    }
    else if(m_nMenuIndex == 3)
    {
        sprintf(temp, "%lld", m_lUserScore+2*m_lRollPrize);
    }
    else if(m_nMenuIndex == 5)
    {
        sprintf(temp, "%lld", m_lUserScore+4*m_lRollPrize);
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage("请参考帮助做出正确的操作!", eMBSuperOK);
    }
    m_nMenuIndex = 1;
    m_labelUserScore->setString(temp);
    CMD_C_CHANGE_PRIZE changePrize = {0};
    changePrize.lChipPrize = m_lChipScore;
    changePrize.lUserScore = m_lUserScore;
    changePrize.nTimes = 1;
    
    m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_CHANGE_PRIZE, &changePrize, sizeof(changePrize));
}

void GameLayer::setMenuItemVisible(int nMenuTag, bool bShow, CCMenu* menu)
{
    for(int i = 0; i < menu->getChildren()->count(); i++)
    {
        CCMenuItem* item = (CCMenuItem*)(menu->getChildren()->objectAtIndex(i));
        if(item != NULL)
        {
            if(item->getTag() == nMenuTag)
            {
                item->setVisible(bShow);
                break;
            }
        }
    }
}

void GameLayer::setMenuTouchEnable(bool bEnable, CCMenu* menu)
{
    for (int i=0; i<m_menuMain2->getChildren()->count(); i++)
    {
        CCMenuItem* item = (CCMenuItem*)(m_menuMain2->getChildren()->objectAtIndex(i));
        if (item != NULL)
        {
            item->setEnabled(bEnable);
        }
    }
    
    
}

void GameLayer::setMenuItemTouchEnable(int nItemTag, bool bEnable, CCMenu* menu)
{
    for(int i = 0; i < menu->getChildren()->count(); i++)
    {
        CCMenuItem* item = (CCMenuItem*)(menu->getChildren()->objectAtIndex(i));
        if(item != NULL)
        {
            if(item->getTag() == nItemTag)
            {
                item->setEnabled(bEnable);
                break;
            }
        }
    }
}

void GameLayer::TimerToStopRoll(float dt)
{
    if(m_bEnterGuided)
    {
        m_bReceiveID = true;
        m_bSame = true;
        for(int i =0; i < SHOW_NUM; i++)
        {
            m_nRollerID[i] = 2;
        }
    }
    if (m_bReceiveID)
    {
        this->unschedule(schedule_selector(GameLayer::TimerToStopRoll));
        setMenuItemTouchEnable(eSuperStopTag, false, m_menuMain1);
    }
    else
    {
        return;
    }
    redLightBlink();
    
    stopGameRoll();
    
    char sEndPicName[32];
    for (int i=0; i<SHOW_NUM; i++)
    {
        sprintf(sEndPicName, "SuperPirate_%d.png", m_nRollerID[i]);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()-> spriteFrameByName(sEndPicName);
        m_spNums[i]->setDisplayFrame(frame);
        CCSprite* pSpUp = (CCSprite*)m_spNums[i]->getChildByTag(eUpSp);
        if (pSpUp != NULL)
        {
            sprintf(sEndPicName, "SuperPirate_%d.png", (m_nRollerID[i]+1)>10?1:(m_nRollerID[i]+1));
            frame = CCSpriteFrameCache::sharedSpriteFrameCache()-> spriteFrameByName(sEndPicName);
            pSpUp->setDisplayFrame(frame);
        }
        CCSprite* pSpDown = (CCSprite*)m_spNums[i]->getChildByTag(eDownSp);
        if (pSpDown != NULL)
        {
            sprintf(sEndPicName, "SuperPirate_%d.png", (m_nRollerID[i]-1)<1?10:(m_nRollerID[i]-1));
            frame = CCSpriteFrameCache::sharedSpriteFrameCache()-> spriteFrameByName(sEndPicName);
            pSpDown->setDisplayFrame(frame);
        }
        m_spNums[i]->stopAllActions();
        m_spNums[i]->setPositionY(95);
        m_spNums[i]->setVisible(true);
        if (i == (SHOW_NUM-1))
        {
            CCSequence* seq = CCSequence::create(CCMoveBy::create(0.25f, ccp(0, 20)), CCCallFuncN::create(this, callfuncN_selector(GameLayer::delayToShowRollEnd)), NULL);
            m_spNums[i]->runAction(seq);
        }
        else
        {
            CCSequence* seq = CCSequence::create(CCMoveBy::create(0.25f, ccp(0, 20)), NULL);
            m_spNums[i]->runAction(seq);
        }
        
    }
}

void GameLayer::delayToShowRollEnd(CCNode* node)
{
    if (m_bSame)
    {
        if(m_nRollerID[0] == 1)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult2);
        }
        else if(m_nRollerID[0] == 2)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult3);
        }
        else if(m_nRollerID[0] == 3)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult4);
        }
        else if(m_nRollerID[0] == 4)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult5);
        }
        else if(m_nRollerID[0] == 5)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult6);
        }
        else if(m_nRollerID[0] == 6)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult7);
        }
        else if(m_nRollerID[0] == 7)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult8);
        }
        else if(m_nRollerID[0] == 8)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult9);
        }
        else if(m_nRollerID[0] == 9)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult10);
        }
        else if(m_nRollerID[0] == 10)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult11);
        }
        setMenuItemTouchEnable(eSuperMinusTag, false, m_menuMain1);
        setMenuItemTouchEnable(eSuperPlusTag, false, m_menuMain1);
        setMenuItemTouchEnable(eSuperStartTag, false, m_menuMain1);
        setMenuItemTouchEnable(eSuperStopTag, false, m_menuMain1);
        setMenuItemTouchEnable(eSuperAutoTag, false, m_menuMain1);
        setMenuItemTouchEnable(eSuperAutoTag, false, m_menuMain1);
        SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/lightSound.mp3");
        playGameWinAnimate();
        this->scheduleOnce(schedule_selector(GameLayer::TimerToEnterToPrizeLayer), 1.5f);
    }
    else
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperResult1);
        SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/loseSound.mp3");
        setMenuItemVisible(eSuperStopTag, false, m_menuMain1);
        if (m_nItemNum>0)
        {
            setMenuItemVisible(eSuperItemStartTag, true, m_menuMain1);
        }
        else
        {
            setMenuItemVisible(eSuperStartTag, true, m_menuMain1);
        }
        
        char sGameRollEndChip[32];
        sprintf(sGameRollEndChip, "%lld", m_lChipScore);
        m_labelCurrentYa->setString(sGameRollEndChip);
        sprintf(sGameRollEndChip, "%lld", m_lUserScore);
        m_labelUserScore->setString(sGameRollEndChip);
        
        if (m_bAuto)
        {
            CCNode* pNode = CCNode::create();
            this -> addChild(pNode);
            pNode->runAction(CCSequence::create(CCDelayTime::create(0.5f),
                                                CCCallFunc::create(this, callfunc_selector(GameLayer::sendGameRoll)),CCCallFuncN::create(this, callfuncN_selector(GameLayer::delayRemoveShouJinAnimate)),
                                                NULL));
        }
        else
        {
            m_bRolling = false;
            //失败 第一个界面触摸管理:开启
            setMenuItemTouchEnable(eSuperMinusTag, true, m_menuMain1);
            setMenuItemTouchEnable(eSuperPlusTag, true, m_menuMain1);
            setMenuItemTouchEnable(eSuperStartTag, true, m_menuMain1);
            setMenuItemTouchEnable(eSuperStopTag, true, m_menuMain1);
        }
    }
}

void GameLayer::TimerToEnterToPrizeLayer(float dt)
{
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    this->unschedule(schedule_selector(GameLayer::TimerToStopRoll));
    this->unschedule(schedule_selector(GameLayer::TimerToEnterToPrizeLayer));
    m_node1->setVisible(false);
    m_node2->setVisible(true);
    setMenuTouchEnable(true, m_menuMain2);
    
    startPrizeRoll();
    m_bLackToGuess = false;
    //星星充值
    for (int i=0; i<8; i++)
    {
        CCSprite* colorStar = (CCSprite*)m_node2->getChildByTag(110+i);
        if (colorStar != NULL)
        {
            colorStar->setVisible(false);
        }
		CCSprite* pBlindStar = (CCSprite*)m_node2->getChildByTag(120+i);
        if (pBlindStar != NULL)
        {
            pBlindStar->stopAllActions();
            pBlindStar->setOpacity(50);
            pBlindStar->setScale(2.5f);
            pBlindStar->setVisible(false);
        }
    }
    
    if(m_bEnterGuided)
    {
        SuperGuideLayer* pSupreGuideLayer = (SuperGuideLayer*)this->getChildByTag(9999);
        if(pSupreGuideLayer != NULL)
        {
            playPrizeBetChangeAnimate(2*pSupreGuideLayer->getYaZhu());
        }
    }
    else
    {
        playPrizeBetChangeAnimate(m_lChipScore);
        m_nCalTime = 0;
        this->schedule(schedule_selector(GameLayer::TimerToAutoCalculate), 1.0, 15, 0.01f);
    }
    
    m_bPrizeRoll = true;
}


void GameLayer::TimerToRollLayer(float dt)
{//分不够导致退出node2，不会播放收金动画
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    m_lTotalScore = 0;
    stopPrizeRoll();
    setMenuItemVisible(eSuperTwoTag, true, m_menuMain2);
    setMenuItemVisible(eSuperTwoCancleTag, false, m_menuMain2);
    setMenuItemVisible(eSuperThreeTag, true, m_menuMain2);
    setMenuItemVisible(eSuperThreeCancleTag, false, m_menuMain2);
    setMenuItemVisible(eSuperFiveTag, true, m_menuMain2);
    setMenuItemVisible(eSuperFiveCancleTag, false, m_menuMain2);
    m_node2->setVisible(false);
    
    char sDefultScore[32];
    sprintf(sDefultScore, "%lld", m_lChipScore);
    m_labelCurrentYa -> setString(sDefultScore);
    char sUserScore[32];
    sprintf(sUserScore, "%lld", m_lUserScore);
    m_labelUserScore->setString(sUserScore);
    
    //开启按钮触摸
    m_node1->setVisible(true);
    if (m_nItemNum>0)
    {
        setMenuItemVisible(eSuperItemStartTag, true, m_menuMain1);
        setMenuItemVisible(eSuperStartTag, false, m_menuMain1);
    }
    else
    {
        setMenuItemVisible(eSuperItemStartTag, false, m_menuMain1);
        setMenuItemVisible(eSuperStartTag, true, m_menuMain1);
    }
    setMenuItemVisible(eSuperStopTag, false, m_menuMain1);
    //自动按钮在一个界面只有出现相同图案跳转到第二个界面的时候不能点击，其他时候(在第一个界面)都可以点击
    
  
    if(m_bAuto)
    {
        //自动 关闭触摸
        sendGameRoll();
        setMenuItemTouchEnable(eSuperAutoTag, true, m_menuMain1);
        setMenuItemTouchEnable(eSuperCancelAutoTag, true, m_menuMain1);
    }
    else
    {
        m_bRolling = false;
        //开启触摸
        setMenuItemTouchEnable(eSuperMinusTag, true, m_menuMain1);
        setMenuItemTouchEnable(eSuperPlusTag, true, m_menuMain1);
        setMenuItemTouchEnable(eSuperStopTag, true, m_menuMain1);
        setMenuItemTouchEnable(eSuperAutoTag, true, m_menuMain1);
        setMenuItemTouchEnable(eSuperCancelAutoTag, true, m_menuMain1);
        if (m_nItemNum>0)
        {
            setMenuItemTouchEnable(eSuperItemStartTag, true, m_menuMain1);
        }
        else
        {
            setMenuItemTouchEnable(eSuperStartTag, true, m_menuMain1);
        }
    }
    if(m_bEnterGuided)
    {
        SuperGuideLayer* pSuperGuideLayer = (SuperGuideLayer*)this->getChildByTag(9999);
        if(pSuperGuideLayer != NULL)
        {
            pSuperGuideLayer->setVisible(true);
            pSuperGuideLayer->setShowNode6();
        }
    }
}

void GameLayer::TimerToAutoCalculate(float dt)
{
    if (m_nCalTime>=15)
    {
        m_nCalTime = 0;
        this->unschedule(schedule_selector(GameLayer::TimerToAutoCalculate));
        sendAutoCalculate();
    }
    else
    {
        char sTime[8];
        sprintf(sTime, "%d", 15-m_nCalTime);
        m_pCalTTF->setString(sTime);
        m_pCalTTF->setVisible(true);
        m_nCalTime++;;
    }
}

void GameLayer::sendAutoCalculate()
{
    m_pCalTTF->setVisible(false);
    setMenuTouchEnable(false, m_menuMain2);
    this->unschedule(schedule_selector(GameLayer::TimerToAutoCalculate));
    CMD_C_GET_PRIZE gameEnd = {0};
    gameEnd.wUserID = m_pClientKernel->GetMeChairID();
    m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_GET_PRIZE, &gameEnd, sizeof(gameEnd));
}

void GameLayer::setGuessItemTouchEnable(bool bTouch)
{
    CCMenuItem* nanItem = (CCMenuItem*)m_menuMain2->getChildByTag(eSuperNanTag);
    if (nanItem != NULL)
    {
        nanItem->setEnabled(bTouch);
    }
    
    CCMenuItem* nvItem = (CCMenuItem*)m_menuMain2->getChildByTag(eSuperNvTag);
    if (nvItem != NULL)
    {
        nvItem->setEnabled(bTouch);
    }
}


void GameLayer::playGetPrizeAnimate()
{
    CCSprite* pShouJinBefore = (CCSprite*)m_node2->getChildByTag(eSuperShouJinAnimationTag);
    if (pShouJinBefore != NULL)
    {
        pShouJinBefore->removeFromParentAndCleanup(true);
    }
    
    if(m_bEnterGuided)
    {
        m_bLackToGuess = false;
    }
    if (!m_bLackToGuess)
    {
        SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/getPrize.mp3");
        
        CCSprite* pShouJin = CCSprite::createWithSpriteFrameName("shoujin_1.png");
        pShouJin->setAnchorPoint(ccp(0.5, 1));
        pShouJin->setPosition(ccp(CLIENT_WIDTH/2+90, CLIENT_HEIGHT/2+320));
        m_node2->addChild(pShouJin, 200, eSuperShouJinAnimationTag);
        
        CCArray* array = CCArray::create();
        char temp[32];
        
        for(int i = 1; i <= 41; i++)
        {
            sprintf(temp, "shoujin_%d.png", i);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()-> spriteFrameByName(temp);
            array -> addObject(frame);
        }
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, 0.05f);
        animation -> setRestoreOriginalFrame(true);
        CCAnimate* animate = CCAnimate::create(animation);
        CCCallFuncN* pShouJinCallFuncN = CCCallFuncN::create(this, callfuncN_selector(GameLayer::delayRemoveShouJinAnimate));
        CCSequence* seq = CCSequence::create(animate, pShouJinCallFuncN, NULL);
        pShouJin->runAction(seq);
        
        if (m_nStarCount >= 5)
        {
            SuperExtraLayer* pSuperExtraLayer = SuperExtraLayer::create(m_lGetPrizeExtra);
            m_node2->addChild(pSuperExtraLayer, 200, 995);
            this->scheduleOnce(schedule_selector(GameLayer::TimerToRollLayer), 0.035f*41+0.05f);
        }
        else{
            this->scheduleOnce(schedule_selector(GameLayer::TimerToRollLayer), 0.05f*41+0.05f);
        }
        
        
    }
    else
    {
        //失败动画
        this->scheduleOnce(schedule_selector(GameLayer::TimerToRollLayer), 0.5f);
    }
}

void GameLayer::delayRemoveShouJinAnimate(CCNode* node)
{
    node->removeFromParentAndCleanup(true);
}

void GameLayer::playGetPrizeExtra()
{
}

CCSequence* GameLayer::getSeqAnimate()
{
    CCSequence* seq = CCSequence::create(CCSpawn::create(CCFadeIn::create(0.4f),
                                                         CCMoveBy::create(0.4f, ccp(0, 15)),
                                                         NULL),
                                         CCMoveBy::create(0.2, ccp(0, -4)),
                                         CCSpawn::create(CCFadeOut::create(0.4f),
                                                         CCMoveBy::create(0.5f, ccp(0, 23)),
                                                         NULL),
                                         CCCallFuncN::create(this, callfuncN_selector(GameLayer::delayRemoveShouJinAnimate)),
                                         NULL);
    return seq;
}

void GameLayer::playPrizeBetChangeAnimate(LONGLONG lChangedPrizeBet)
{
    m_lNativePrize = lChangedPrizeBet;
    this->unschedule(schedule_selector(GameLayer::runToTargetBet));
    this->schedule(schedule_selector(GameLayer::runToTargetBet), 0.02f);
}

void GameLayer::runToTargetBet(float dt)
{
    static int nTimes = 3;
    if (nTimes == 10)
    {
        this->unschedule(schedule_selector(GameLayer::runToTargetBet));
        nTimes = 3;
        return;
    }
    nTimes++;
    if (nTimes%2==0)
    {
        SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/changePrize2Sound.mp3");
    }
    int nTempMoney = m_lNativePrize*0.1f*nTimes;
    char sMoney[32];
    sprintf(sMoney, "%d", nTempMoney);
    CCLabelAtlas* pCurrentBetNum = (CCLabelAtlas*)m_node2->getChildByTag(ePrizeBet);
    if (pCurrentBetNum != NULL)
    {
        CCSprite* pMoneyBg = (CCSprite*)m_node2->getChildByTag(1000);
        if (pMoneyBg != NULL)
        {
            pMoneyBg->stopAllActions();
            pMoneyBg->setScaleX(0.85f);
            pMoneyBg->setScaleY(0.5f);
        }
        pCurrentBetNum->stopAllActions();
        pCurrentBetNum->setPosition(ccp(846, 180));
        pCurrentBetNum->setScale(0.9f);
        pCurrentBetNum->setString(sMoney);
        if (nTimes == 10)
        {
            float fTime = 0.6f;
            CCScaleTo* pScaleTo1 = CCScaleTo::create(fTime*2/3, 1.1f);
            CCDelayTime* pDelayTime = CCDelayTime::create(fTime/3);
            CCScaleTo* pScaleTo2 = CCScaleTo::create(0, 0.7f);
            CCMoveTo* pMoveBy2 = CCMoveTo::create(0, ccp(846, 170));
            CCSpawn* pSpawn = CCSpawn::create(pScaleTo2, pMoveBy2, NULL);
            CCSequence* pSeq = CCSequence::create(pScaleTo1, pDelayTime, pSpawn, NULL);
            pCurrentBetNum->runAction(pSeq);
            
            CCDelayTime* pDelay = CCDelayTime::create(0.4f);
            CCScaleTo* pScale = CCScaleTo::create(fTime, 1.0f);
            CCEaseSineIn* pEaseInOut = CCEaseSineIn::create(pScale);
            CCScaleTo* pScale1 = CCScaleTo::create(fTime, 0.85f, 0.5f);
            CCSequence* pSeq1 = CCSequence::create(pDelay, pEaseInOut, pScale1, NULL);
            pMoneyBg->runAction(pSeq1);
            CCNode* pNode = CCNode::create();
            this -> addChild(pNode);
            pNode->runAction(CCSequence::create(CCDelayTime::create(0.45f),
                                                CCCallFunc::create(this, callfunc_selector(GameLayer::delayToPlayChangePrizeSound)),
                                                CCCallFuncN::create(this, callfuncN_selector(GameLayer::delayRemoveShouJinAnimate)),
                                                NULL));
        }
    }
}

void GameLayer::delayToPlayChangePrizeSound()
{
    SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/changePrizeSound.mp3");
    if(m_bEnterGuided)
    {
        SuperGuideLayer* pSupreGuideLayer = (SuperGuideLayer*)this->getChildByTag(9999);
        if(pSupreGuideLayer != NULL)
        {
            pSupreGuideLayer->setVisible(true);
            if(pSupreGuideLayer->getCurrentIndex() == 4)
            {
                pSupreGuideLayer->setShowNode4();
            }
            else if(pSupreGuideLayer->getCurrentIndex() == 5)
            {
                pSupreGuideLayer->setShowNode5();
            }
        }
    }
}

void GameLayer::playFireAnimate()
{
    CCSprite* pLeftFire = CCSprite::createWithSpriteFrameName("firesmall_1.png");
    pLeftFire->setPosition(ccp(520, 470));
    pBackground->addChild(pLeftFire, 10);
    CCArray* array = CCArray::create();
    char temp[32];
    for(int i = 1; i <= 21; i++)
    {
        sprintf(temp, "firesmall_%d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()-> spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, 0.07f);
    animation -> setRestoreOriginalFrame(true);
    CCAnimate* animate = CCAnimate::create(animation);
    pLeftFire->runAction(CCRepeatForever::create(animate));

    CCSprite* pRightFire = CCSprite::createWithSpriteFrameName("firesmall_1.png");
    pRightFire->setFlipX(true);
    pRightFire->setPosition(ccp(9, 450));
    pBackground->addChild(pRightFire, 10);
    pRightFire->runAction(CCRepeatForever::create(animate));
}

void GameLayer::startGameRoll()
{
    for (int index=0; index<SHOW_NUM; index++)
    {
        m_spNums[index]->setVisible(false);
    }
    
    CCLayer* pContainLayer = (CCLayer*)m_GameScrollView->getContainer();
    if (pContainLayer != NULL)
    {
        m_nGameRollingEffectID = SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/rollSound.mp3", true);
        float fTimes = 0.1f;
        CCSprite* pRollAnimateDown1 = (CCSprite*)pContainLayer->getChildByTag(1001);
        if (pRollAnimateDown1 != NULL)
        {
            pRollAnimateDown1->stopAllActions();
            pRollAnimateDown1->setPositionY(0);
            pRollAnimateDown1->setVisible(true);
            CCMoveBy* moveBy1 = CCMoveBy::create(fTimes, ccp(0, -pRollAnimateDown1->getContentSize().height));
            CCMoveTo* moveTo = CCMoveTo::create(0, ccp(90, pRollAnimateDown1->getContentSize().height));
            CCMoveBy* moveBy2 = CCMoveBy::create(fTimes, ccp(0, -pRollAnimateDown1->getContentSize().height));
            CCSequence* seq = CCSequence::create(moveBy1, moveTo, moveBy2, NULL);
            pRollAnimateDown1->runAction(CCRepeatForever::create(seq));
        }
        CCSprite* pRollAnimateUp1 = (CCSprite*)pContainLayer->getChildByTag(1002);
        if (pRollAnimateUp1 != NULL)
        {
            pRollAnimateUp1->stopAllActions();
            pRollAnimateUp1->setPositionY(pRollAnimateDown1->getContentSize().height);
            pRollAnimateUp1->setVisible(true);
            CCMoveBy* moveBy = CCMoveBy::create(fTimes*2, ccp(0, -pRollAnimateUp1->getContentSize().height*2));
            CCMoveTo* moveTo = CCMoveTo::create(0, ccp(90, pRollAnimateUp1->getContentSize().height));
            CCSequence* seq = CCSequence::create(moveBy, moveTo, NULL);
            pRollAnimateUp1->runAction(CCRepeatForever::create(seq));
        }
        CCSprite* pRollAnimateDown2 = (CCSprite*)pContainLayer->getChildByTag(1003);
        if (pRollAnimateDown2 != NULL)
        {
            pRollAnimateDown2->stopAllActions();
            pRollAnimateDown2->setPositionY(0);
            pRollAnimateDown2->setVisible(true);
            CCMoveBy* moveBy1 = CCMoveBy::create(fTimes, ccp(0, -pRollAnimateDown2->getContentSize().height));
            CCMoveTo* moveTo = CCMoveTo::create(0, ccp(90+190, pRollAnimateDown2->getContentSize().height));
            CCMoveBy* moveBy2 = CCMoveBy::create(fTimes, ccp(0, -pRollAnimateDown2->getContentSize().height));
            CCSequence* seq = CCSequence::create(moveBy1, moveTo, moveBy2, NULL);
            pRollAnimateDown2->runAction(CCRepeatForever::create(seq));
        }
        CCSprite* pRollAnimateUp2 = (CCSprite*)pContainLayer->getChildByTag(1004);
        if (pRollAnimateUp2 != NULL)
        {
            pRollAnimateUp2->stopAllActions();
            pRollAnimateUp2->setPositionY(pRollAnimateDown2->getContentSize().height);
            pRollAnimateUp2->setVisible(true);
            CCMoveBy* moveBy = CCMoveBy::create(fTimes*2, ccp(0, -pRollAnimateUp2->getContentSize().height*2));
            CCMoveTo* moveTo = CCMoveTo::create(0, ccp(90+190, pRollAnimateUp2->getContentSize().height));
            CCSequence* seq = CCSequence::create(moveBy, moveTo, NULL);
            pRollAnimateUp2->runAction(CCRepeatForever::create(seq));
        }
        CCSprite* pRollAnimateDown3 = (CCSprite*)pContainLayer->getChildByTag(1005);
        if (pRollAnimateDown3 != NULL)
        {
            pRollAnimateDown3->stopAllActions();
            pRollAnimateDown3->setPositionY(0);
            pRollAnimateDown3->setVisible(true);
            CCMoveBy* moveBy1 = CCMoveBy::create(fTimes, ccp(0, -pRollAnimateDown3->getContentSize().height));
            CCMoveTo* moveTo = CCMoveTo::create(0, ccp(90+190*2, pRollAnimateDown3->getContentSize().height));
            CCMoveBy* moveBy2 = CCMoveBy::create(fTimes, ccp(0, -pRollAnimateDown3->getContentSize().height));
            CCSequence* seq = CCSequence::create(moveBy1, moveTo, moveBy2, NULL);
            pRollAnimateDown3->runAction(CCRepeatForever::create(seq));
        }
        CCSprite* pRollAnimateUp3 = (CCSprite*)pContainLayer->getChildByTag(1006);
        if (pRollAnimateUp3 != NULL)
        {
            pRollAnimateUp3->stopAllActions();
            pRollAnimateUp3->setPositionY(pRollAnimateDown3->getContentSize().height);
            pRollAnimateUp3->setVisible(true);
            CCMoveBy* moveBy = CCMoveBy::create(fTimes*2, ccp(0, -pRollAnimateUp3->getContentSize().height*2));
            CCMoveTo* moveTo = CCMoveTo::create(0, ccp(90+190*2, pRollAnimateUp3->getContentSize().height));
            CCSequence* seq = CCSequence::create(moveBy, moveTo, NULL);
            pRollAnimateUp3->runAction(CCRepeatForever::create(seq));
        }
    }
    
    CCSprite* pPointerLeft = (CCSprite*)m_node1->getChildByTag(eLeftPoiner);
    if (pPointerLeft != NULL)
    {
        pPointerLeft->setVisible(true);
        CCSequence* seq1 = CCSequence::create(CCRotateTo::create(0.2, 20), CCRotateTo::create(0.2, 10), NULL);
        pPointerLeft->runAction(CCRepeatForever::create(seq1));
    }
    
    CCSprite* pPointerRight = (CCSprite*)m_node1->getChildByTag(eRightPoiner);
    if (pPointerRight != NULL)
    {
        pPointerRight->setVisible(true);
        CCSequence* seq1 = CCSequence::create(CCRotateTo::create(0.2, -20), CCRotateTo::create(0.2, -10), NULL);
        pPointerRight->runAction(CCRepeatForever::create(seq1));
        
    }
    
    CCSprite* pRollLeftFire1 = (CCSprite*)m_node1->getChildByTag(eRollLeftFire1);
    if (pRollLeftFire1 != NULL)
    {
        pRollLeftFire1->setVisible(true);
        pRollLeftFire1 -> runAction(CCRepeatForever::create(getAnimate(11, "huo", 0.05)));
    }
    CCSprite* pRollLeftFire2 = (CCSprite*)m_node1->getChildByTag(eRollLeftFire2);
    if (pRollLeftFire2 != NULL)
    {
        pRollLeftFire2->setVisible(true);
        pRollLeftFire2 -> runAction(CCRepeatForever::create(getAnimate(11, "huo", 0.05)));
    }
    
    CCSprite* pRolRightFire1 = (CCSprite*)m_node1->getChildByTag(eRollRightFire1);
    if (pRolRightFire1 != NULL)
    {
        pRolRightFire1->setVisible(true);
        pRolRightFire1 -> runAction(CCRepeatForever::create(getAnimate(11, "huo", 0.05)));
    }
    CCSprite* pRolRightFire2 = (CCSprite*)m_node1->getChildByTag(eRollRightFire2);
    if (pRolRightFire2 != NULL)
    {
        pRolRightFire2->setVisible(true);
        pRolRightFire2 -> runAction(CCRepeatForever::create(getAnimate(11, "huo", 0.05)));
    }
}

void GameLayer::stopGameRoll()
{
    CCLayer* pContainLayer = (CCLayer*)m_GameScrollView->getContainer();
    if (pContainLayer != NULL)
    {
        SimpleAudioEngine::sharedEngine()->stopEffect(m_nGameRollingEffectID);
        for (int i=1; i<=6; i++)
        {
            CCSprite* pRollAnimate = (CCSprite*)pContainLayer->getChildByTag(1000+i);
            if (pRollAnimate != NULL)
            {
                pRollAnimate->stopAllActions();
                pRollAnimate->setVisible(false);
                if (i%2==0)
                {
                    pRollAnimate->setPositionY(0);
                }
                else
                {
                    pRollAnimate->setPositionY(pRollAnimate->getContentSize().height);
                }
            }
        }
    }
    
    CCSprite* pPointerLeft = (CCSprite*)m_node1->getChildByTag(eLeftPoiner);
    if (pPointerLeft != NULL)
    {
        pPointerLeft->stopAllActions();
        pPointerLeft->setRotation(0);
    }
    
    CCSprite* pPointerRight = (CCSprite*)m_node1->getChildByTag(eRightPoiner);
    if (pPointerRight != NULL)
    {
        pPointerRight->stopAllActions();
        pPointerRight->setRotation(0);
    }
    
    CCSprite* pRollLeftFire1 = (CCSprite*)m_node1->getChildByTag(eRollLeftFire1);
    if (pRollLeftFire1 != NULL)
    {
        pRollLeftFire1->setVisible(false);
        pRollLeftFire1->stopAllActions();
    }
    CCSprite* pRollLeftFire2 = (CCSprite*)m_node1->getChildByTag(eRollLeftFire2);
    if (pRollLeftFire2 != NULL)
    {
        pRollLeftFire2->setVisible(false);
        pRollLeftFire2->stopAllActions();
    }
    
    CCSprite* pRolRightFire1 = (CCSprite*)m_node1->getChildByTag(eRollRightFire1);
    if (pRolRightFire1 != NULL)
    {
        pRolRightFire1->setVisible(false);
        pRolRightFire1->stopAllActions();
    }
    CCSprite* pRolRightFire2 = (CCSprite*)m_node1->getChildByTag(eRollRightFire2);
    if (pRolRightFire2 != NULL)
    {
        pRolRightFire2->setVisible(false);
        pRolRightFire2->stopAllActions();
    }
}

void GameLayer::startPrizeRoll()
{
    if (!m_node2->isVisible())
    {
        SimpleAudioEngine::sharedEngine()->stopEffect(m_nPrizeRollingEffectID);
        return;
    }
    m_spPrize->setVisible(false);
    if (m_nStarCount < 8)
    {
        if (!m_bLackToGuess)
        {
            setGuessItemTouchEnable(true);
        }
    }
    redLightScroll();
    
    CCLayer* pContainLayer = (CCLayer*)m_PrizeScrollView->getContainer();
    if (pContainLayer != NULL)
    {
        m_nPrizeRollingEffectID = SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/prizerollSound.mp3", true);
        CCSprite* pRollAnimateDown1 = (CCSprite*)pContainLayer->getChildByTag(2001);
        if (pRollAnimateDown1 != NULL)
        {
            pRollAnimateDown1->setVisible(true);
            CCMoveBy* moveBy1 = CCMoveBy::create(0.25, ccp(0, -pRollAnimateDown1->getContentSize().height+100));
            CCMoveTo* moveTo = CCMoveTo::create(0, ccp(187/2, pRollAnimateDown1->getContentSize().height-200));
            CCMoveBy* moveBy2 = CCMoveBy::create(0.25, ccp(0, -pRollAnimateDown1->getContentSize().height+100));
            CCSequence* seq = CCSequence::create(moveBy1, moveTo, moveBy2, NULL);
            pRollAnimateDown1->runAction(CCRepeatForever::create(seq));
        }
        CCSprite* pRollAnimateUp1 = (CCSprite*)pContainLayer->getChildByTag(2002);
        if (pRollAnimateUp1 != NULL)
        {
            pRollAnimateUp1->setVisible(true);
            CCMoveBy* moveBy = CCMoveBy::create(0.25*2, ccp(0, -(pRollAnimateUp1->getContentSize().height-100)*2));
            CCMoveTo* moveTo = CCMoveTo::create(0, ccp(187/2, pRollAnimateUp1->getContentSize().height-200));
            CCSequence* seq = CCSequence::create(moveBy, moveTo, NULL);
            pRollAnimateUp1->runAction(CCRepeatForever::create(seq));
        }
    }
}

void GameLayer::stopPrizeRoll()
{
    redLightBlink();
    
    CCLayer* pContainLayer = (CCLayer*)m_PrizeScrollView->getContainer();
    if (pContainLayer != NULL)
    {
        this->stopAllActions();
        SimpleAudioEngine::sharedEngine()->stopEffect(m_nPrizeRollingEffectID);
        CCSprite* pRollAnimateDown1 = (CCSprite*)pContainLayer->getChildByTag(2001);
        if (pRollAnimateDown1 != NULL)
        {
            pRollAnimateDown1->stopAllActions();
            pRollAnimateDown1->setVisible(false);
            pRollAnimateDown1->setPosition(ccp(pContainLayer->getContentSize().width/2, -100));
        }
        CCSprite* pRollAnimateUp1 = (CCSprite*)pContainLayer->getChildByTag(2002);
        if (pRollAnimateUp1 != NULL)
        {
            pRollAnimateUp1->stopAllActions();
            pRollAnimateUp1->setVisible(false);
            pRollAnimateUp1->setPosition(ccp(pContainLayer->getContentSize().width/2, pRollAnimateDown1->getContentSize().height-200));
        }
    }
}

void GameLayer::dealGuessResult()
{
    stopPrizeRoll();
    setGuessShow();
}

void GameLayer::setGuessShow()
{
    std::string sResultName="";
    std::string sResultOther1Name="";
    std::string sResultOther2Name="";
    std::string sGuessEndName="";
    if (m_nPrizeID == m_nSelectID)
    {
        if (m_nSelectID == 0)
        {
            sResultName = "PrizeHero1.png";
            sResultOther1Name = "PrizeHero2.png";
            sResultOther2Name = "PrizeHero2.png";
            sGuessEndName = "receiveGuessEndnan.png";
        }
        else
        {
            sResultName = "PrizeHero2.png";
            sResultOther1Name = "PrizeHero1.png";
            sResultOther2Name = "PrizeHero1.png";
            sGuessEndName = "receiveGuessEndnv.png";
        }
        SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/winSound.mp3");
    }
    else
    {
        if (m_nSelectID == 0)
        {
            sResultName = "PrizeHero2.png";
            sResultOther1Name = "PrizeHero1.png";
            sResultOther2Name = "PrizeHero1.png";
            sGuessEndName = "receiveGuessEndnv.png";
        }
        else
        {
            sResultName = "PrizeHero1.png";
            sResultOther1Name = "PrizeHero2.png";
            sResultOther2Name = "PrizeHero1.png";
            sGuessEndName = "receiveGuessEndnan.png";
        }
        SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/loseSound.mp3");
    }
    CCSpriteFrame* pPrizeResultFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(sResultName.c_str());
    CCSpriteFrame* pPrizeResultOther1Frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(sResultOther1Name.c_str());
    CCSpriteFrame* pPrizeResultOther2Frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(sResultOther2Name.c_str());
    
    m_spPrize -> setDisplayFrame(pPrizeResultFrame);
    CCSprite* pPrizeUp = (CCSprite*)m_spPrize->getChildByTag(ePrizeUp);
    if (pPrizeUp != NULL)
    {
        pPrizeUp->setDisplayFrame(pPrizeResultOther1Frame);
    }
    CCSprite* pPrizeDown = (CCSprite*)m_spPrize->getChildByTag(ePrizeDown);
    if (pPrizeDown != NULL)
    {
        pPrizeDown->setDisplayFrame(pPrizeResultOther2Frame);
    }
    m_spPrize->setVisible(true);
    
    CCNode* pNode = CCNode::create();
    this -> addChild(pNode);
    pNode->runAction(CCSequence::create(CCDelayTime::create(0.5f),
                                           CCCallFunc::create(this, callfunc_selector(GameLayer::startPrizeRoll)),
                                            CCCallFuncN::create(this, callfuncN_selector(GameLayer::delayRemoveShouJinAnimate)),
                                           NULL));
    
    
    CCLayer* pGuessEndContainLayer = (CCLayer*)m_GuessScrollView->getContainer();
    if (pGuessEndContainLayer != NULL)
    {
        
        CCSprite* pSpGuessEnd = CCSprite::createWithSpriteFrameName(sGuessEndName.c_str());
        pSpGuessEnd->setAnchorPoint(ccp(0.5, 1));
        pGuessEndContainLayer->addChild(pSpGuessEnd, 1);
        pSpGuessEnd->setPosition(ccp(pGuessEndContainLayer->getContentSize().width/2, 0));
        int nGuessEndCount = pGuessEndContainLayer->getChildrenCount();
        CCMoveTo* moveTo=NULL;
        if (nGuessEndCount <= 5)
        {
            moveTo = CCMoveTo::create(0.05f, ccp(pSpGuessEnd->getPositionX(), 264 - 52.5*(nGuessEndCount-1)));
            pSpGuessEnd->runAction(moveTo);
        }
        else
        {
            for(int i=0; i<nGuessEndCount; i++)
            {
                CCSprite* pBeforeEnd = (CCSprite*)(pGuessEndContainLayer->getChildren()->objectAtIndex(i));
                if (pBeforeEnd != NULL)
                {
                    if (i==0)
                    {
                        CCMoveBy* moveBy = CCMoveBy::create(0.05f, ccp(0, 52.5));
                        CCCallFuncN* callFuncN = CCCallFuncN::create(this, callfuncN_selector(GameLayer::removeOutSide));
                        CCSequence* seq = CCSequence::create(moveBy, callFuncN, NULL);
                        pBeforeEnd->runAction(seq);
                    }
                    else
                    {
                        CCMoveBy* moveBy = CCMoveBy::create(0.05f, ccp(0, 52.5));
                        pBeforeEnd->runAction(moveBy);
                    }
                }
            }
        }
    }
}

void GameLayer::playGameWinAnimate()
{
    CCNode* pNode = CCNode::create();
    this -> addChild(pNode);
    pNode->runAction(CCSequence::create(CCDelayTime::create(0.3f), CCCallFunc::create(this, callfunc_selector(GameLayer::delayToPlayStarSound)),  CCCallFuncN::create(this, callfuncN_selector(GameLayer::delayRemoveShouJinAnimate)),NULL));
    CCSprite* pSameLight = (CCSprite*)m_node1->getChildByTag(eGameSameLight);
    if (pSameLight != NULL)
    {
        CCSequence* seq = CCSequence::create(CCShow::create(),
                                             CCRepeat::create(getAnimate(10, "sameLingt", 0.05f), 4),
                                             CCHide::create(),
                                             NULL);
        pSameLight->runAction(seq);
    }
    
    CCSprite* pStar = (CCSprite*)m_node1->getChildByTag(eGameStar);
    if (pStar != NULL)
    {
        pStar->setVisible(true);
        CCSequence* seq = CCSequence::create(CCDelayTime::create(0.1f),
                                             CCShow::create(),
                                             getAnimate(24, "gameRollWin", 0.06f),
                                             CCHide::create(),
                                             NULL);
        pStar->runAction(seq);
    }
    CCLabelAtlas* pAddMoneyNum = (CCLabelAtlas*)pBackground->getChildByTag(eAddMoneyNumTag);
    if (pAddMoneyNum != NULL)
    {
        char sPrize[16];
        sprintf(sPrize, "%lld", m_lRollPrize);
        if(m_bEnterGuided)
        {
            SuperGuideLayer* pSuperGuideLayer = (SuperGuideLayer*)this->getChildByTag(9999);
            if(pSuperGuideLayer != NULL)
            {
                char temp[16];
                sprintf(temp, "%d", 2*pSuperGuideLayer->getYaZhu());
                pAddMoneyNum->setString(temp);
            }
        }
        else
        {
            pAddMoneyNum->setString(sPrize);
        }
        
        pAddMoneyNum->setVisible(false);
        
        pAddMoneyNum->setScale(0);
        CCSequence* seq = CCSequence::create(CCDelayTime::create(0.1f),
                                             CCShow::create(),
                                             CCFadeIn::create(0),
                                             CCScaleTo::create(0.2, 1.3f),
                                             CCSpawn::create(CCMoveBy::create(1.1f, ccp(0, 50)),
                                                             CCSequence::create(CCDelayTime::create(0.5),
                                                             CCFadeOut::create(0.6f),NULL),
                                                             NULL),
                                             CCMoveBy::create(0, ccp(0, -50)),
                                             CCHide::create(),
                                             NULL);
        pAddMoneyNum->runAction(seq);
        CCSprite* pAddMoney = (CCSprite*)pAddMoneyNum->getChildByTag(100);
        if (pAddMoney != NULL)
        {
            pAddMoney->setPosition(ccp(-pAddMoney->getContentSize().width*0.55f, pAddMoneyNum->getContentSize().height/2));
            
            pAddMoney->setScale(0);
            CCSequence* seq = CCSequence::create(CCDelayTime::create(0.1f),
                                                 CCShow::create(),
                                                 CCFadeIn::create(0),
                                                 CCScaleTo::create(0.2, 1.0f),
                                                 CCDelayTime::create(0.5),
                                                 CCFadeOut::create(0.6f),
                                                 CCHide::create(),
                                                 NULL);
            pAddMoney->runAction(seq);
        }
    }
}

void GameLayer::delayToPlayStarSound()
{
    SimpleAudioEngine::sharedEngine()->playEffect("SuperPirate/gameStarSound.mp3");
}

void GameLayer::setGoType()
{
    m_menuMain1->setTouchEnabled(true);
    if (m_nItemNum > 0)
    {
        setMenuItemVisible(eSuperStartTag, false, m_menuMain1);
        setMenuItemVisible(eSuperItemStartTag, true, m_menuMain1);
        setItemNum(m_nItemNum);
        char sItemBet[8];
        sprintf(sItemBet, "%lld", m_lChipScore);
        m_labelCurrentYa->setString(sItemBet);
    }
    else
    {
        setMenuItemVisible(eSuperStartTag, true, m_menuMain1);
        setMenuItemVisible(eSuperItemStartTag, false, m_menuMain1);
    }
}

void GameLayer::setItemNum(int nItemNum)
{
    for (int i=0; i<m_menuMain1->getChildren()->count(); i++)
    {
        CCMenuItem* pMenuItem = (CCMenuItem*)(m_menuMain1->getChildren()->objectAtIndex(i));
        if (pMenuItem != NULL)
        {
            CCLabelAtlas* pItemNumTTF = (CCLabelAtlas*)pMenuItem->getChildByTag(10);
            if (pItemNumTTF !=NULL)
            {
                char sItemNum[16];
                sprintf(sItemNum, "%d", nItemNum);
                pItemNumTTF->setString(sItemNum);
            }
        }
    }
}

void GameLayer::removeOutSide(CCNode* node)
{
    node->removeFromParentAndCleanup(true);
}

void GameLayer::hideRankLayerColor()
{
    m_rankLayer -> setVisible(false);
}

void GameLayer::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
    m_bInitUIComplete = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CMainLogic::sharedMainLogic()->m_bExitGame = false;
#endif
    if(m_bInitNETComplete)
    {
        showGuideLayer();
    }
}

void GameLayer::onExit()
{
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    CCLayer::onExit();
}

void GameLayer::setYaZhuLabel(std::string str)
{
    m_labelCurrentYa->setString(str.c_str());
    m_labelCurrentYa->setVisible(true);
}

void GameLayer::setStatusYaZhuLabel(bool bStatus)
{
    m_labelCurrentYa -> setVisible(bStatus);
}

void GameLayer::dealGuessResultGuide(int nResult)
{
    SuperGuideLayer* pSuperGuideLayer = (SuperGuideLayer*)this->getChildByTag(9999);
    if(pSuperGuideLayer != NULL)
    {
        playPrizeBetChangeAnimate(4*pSuperGuideLayer->getYaZhu());
    }
    CCSprite* spStar = (CCSprite*)m_node2->getChildByTag(110);
    if (spStar != NULL)
    {
        spStar->setVisible(true);
        spStar->runAction(CCSequence::create(CCShow::create(), getAnimate(32, "winStar", 0.07), NULL));
    }
    
    m_nSelectID = nResult;
    m_nPrizeID = m_nSelectID;
    dealGuessResult();
}

void GameLayer::setEnterGuided(bool bEnterGuide)
{
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateGuideFinishedTag);
    
    m_bEnterGuided = bEnterGuide;
    CMainLogic::sharedMainLogic()->m_cbSuperPirateGuided = true;
    CMainLogic::sharedMainLogic()->sendSuperPirateGuideFinished();
    m_labelCurrentYa->setVisible(true);
	char temp[24];
    sprintf(temp, "%lld", m_nCurrentYaZhu);
    m_labelCurrentYa -> setString(temp);
}

void GameLayer::shakeScreen()
{
    float ft = 0.02f;
    int nDis = 3;
    CCPoint point[26] = {
        ccp(27+nDis,-23+nDis),ccp(-20+nDis,18-nDis),ccp(-36-nDis,25-nDis),ccp(37-nDis,-32+nDis),
        ccp(-36+nDis,7+nDis),ccp(36+nDis,36-nDis),ccp(-24-nDis,-18-nDis),ccp(-28-nDis,19+nDis),
        ccp(30+nDis,-27+nDis),ccp(-10+nDis,-23-nDis),ccp(-16-nDis,21-nDis),ccp(-14-nDis,-18+nDis),
        ccp(15+nDis,21+nDis),ccp(24+nDis,-18-nDis),ccp(-16-nDis,-12-nDis),ccp(-16-nDis,24+nDis),
        ccp(21+nDis,-17+nDis),ccp(17+nDis,15-nDis),ccp(-16-nDis,-18-nDis),ccp(-20-nDis,19+nDis),
        ccp(20+nDis,-16+nDis),ccp(22+nDis,-12-nDis),ccp(-20-nDis,22-nDis),ccp(17-nDis,8+nDis),
        ccp(-6+nDis,-10+nDis),ccp(12-nDis,9-nDis)};
    
    
    CCMoveBy* moveBy0 = CCMoveBy::create(ft,point[0]);
    CCMoveBy* moveBy1 = CCMoveBy::create(ft,point[1]);
    CCMoveBy* moveBy2 = CCMoveBy::create(ft,point[2]);
    CCMoveBy* moveBy3 = CCMoveBy::create(ft,point[3]);
    CCMoveBy* moveBy4 = CCMoveBy::create(ft,point[4]);
    CCMoveBy* moveBy5 = CCMoveBy::create(ft,point[5]);
    CCMoveBy* moveBy6 = CCMoveBy::create(ft,point[6]);
    CCMoveBy* moveBy7 = CCMoveBy::create(ft,point[7]);
    CCMoveBy* moveBy8 = CCMoveBy::create(ft,point[8]);
    CCMoveBy* moveBy9 = CCMoveBy::create(ft,point[9]);
    CCMoveBy* moveBy10 = CCMoveBy::create(ft,point[10]);
    CCMoveBy* moveBy11 = CCMoveBy::create(ft,point[11]);
    CCMoveBy* moveBy12 = CCMoveBy::create(ft,point[12]);
    CCMoveBy* moveBy13 = CCMoveBy::create(ft,point[13]);
    CCMoveBy* moveBy14 = CCMoveBy::create(ft,point[14]);
    CCMoveBy* moveBy15 = CCMoveBy::create(ft,point[15]);
    CCMoveBy* moveBy16 = CCMoveBy::create(ft,point[16]);
    CCMoveBy* moveBy17 = CCMoveBy::create(ft,point[17]);
    CCMoveBy* moveBy18 = CCMoveBy::create(ft,point[18]);
    CCMoveBy* moveBy19 = CCMoveBy::create(ft,point[19]);
    CCMoveBy* moveBy20 = CCMoveBy::create(ft,point[20]);
    CCMoveBy* moveBy21 = CCMoveBy::create(ft,point[21]);
    CCMoveBy* moveBy22 = CCMoveBy::create(ft,point[22]);
    CCMoveBy* moveBy23 = CCMoveBy::create(ft,point[23]);
    CCMoveBy* moveBy24 = CCMoveBy::create(ft,point[24]);
    CCMoveBy* moveBy25 = CCMoveBy::create(ft,point[25]);
    
    CCSequence* seq1 = CCSequence::create(moveBy0, moveBy1, moveBy2, moveBy3,moveBy4, moveBy5, moveBy6, moveBy7, moveBy8, moveBy9, moveBy10, moveBy11,moveBy12, moveBy13, moveBy14, moveBy15,moveBy16, moveBy17, moveBy18, moveBy19, moveBy20, moveBy21, moveBy22, moveBy23,moveBy24,moveBy25,moveBy0, moveBy1, moveBy2, moveBy3,moveBy4, moveBy5, moveBy6, moveBy7, moveBy8, moveBy9, moveBy10, moveBy11,moveBy12, moveBy13, moveBy14, moveBy15,moveBy16, moveBy17, moveBy18, moveBy19, moveBy20, moveBy21, moveBy22, moveBy23,moveBy24,moveBy25,moveBy0, moveBy1, moveBy2, moveBy3,moveBy4, moveBy5, moveBy6, moveBy7, moveBy8, moveBy9, moveBy10, moveBy11,moveBy12, moveBy13, moveBy14, moveBy15,moveBy16, moveBy17, moveBy18, moveBy19, moveBy20, moveBy21, moveBy22, moveBy23,moveBy24,moveBy25,NULL);
    CCRepeat* pRet = CCRepeat::create(seq1, 1);
    pRet -> setTag(10001);
    pBackground -> stopActionByTag(10001);
    pBackground -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    pBackground -> runAction(pRet);
}

void GameLayer::showGuideLayer()
{
    if (m_bEnterGuided)
    {
        SuperGuideLayer* pGuideLayer = SuperGuideLayer::create();
        this -> addChild(pGuideLayer, 120, 9999);
        m_nCurrentYaZhu = atoll(m_labelCurrentYa->getString());
        m_labelCurrentYa->setString("1000");
    }
    setGoType();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void GameLayer::openReplaceSceneTime()
{
    this -> scheduleOnce(schedule_selector(GameLayer::backHallScene), 0.1f);
}

void GameLayer::backHallScene()
{
    if(CMainLogic::sharedMainLogic()->m_bChargeSuccess)//微信充值成功
    {
        if(CMainLogic::sharedMainLogic()->m_nBuyGameType == 1)//微信购买金币
        {
            CMainLogic::sharedMainLogic()->switchLayer(eHallLayer, false, false, 2, 1);
        }
        else if(CMainLogic::sharedMainLogic()->m_nBuyGameType == 2)//微信购买武器
        {
            CMainLogic::sharedMainLogic()->switchLayer(eHallLayer, false, false, 2, 2);
        }
        else if(CMainLogic::sharedMainLogic()->m_nBuyGameType == 3)//微信购买技能
        {
            CMainLogic::sharedMainLogic()->switchLayer(eHallLayer, false, false, 2, 3);
        }
    }
    else if(CMainLogic::sharedMainLogic()->m_sAlipaySign != "")
    {
        if(CMainLogic::sharedMainLogic()->m_nBuyGameType == 1)//支付宝购买金币
        {
            CMainLogic::sharedMainLogic()->switchLayer(eHallLayer, false, false, 1, 1);
        }
        else if(CMainLogic::sharedMainLogic()->m_nBuyGameType == 2)//支付宝购买武器
        {
            CMainLogic::sharedMainLogic()->switchLayer(eHallLayer, false, false, 1, 2);
        }
        else if(CMainLogic::sharedMainLogic()->m_nBuyGameType == 3)//支付宝购买技能
        {
            CMainLogic::sharedMainLogic()->switchLayer(eHallLayer, false, false, 1, 3);
        }
    }//支付宝充值
    else
    {
        if(CMainLogic::sharedMainLogic()->m_bNetPageCharge)//网页支付
        {
            CMainLogic::sharedMainLogic()->switchLayer(eHallLayer, false, false, 3, 1);
        }
        else
        {
            CMainLogic::sharedMainLogic()->switchLayer(eHallLayer);
        }
    }
    
}
#endif

//海盗中打开VIP
void GameLayer::openVIP()
{
    CMainLogic::sharedMainLogic()->ShowMessage("正在打开VIP", eMBGameNull);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/LeleconfigVersion.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(GameLayer::onHttpVIPVersionComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void GameLayer::onHttpVIPVersionComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    //对str进行xml解析，将30条数据保存到m_vctRankTimes中
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("version"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL) {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "newvipConfig"))//相同
            {
                std::string sVIPVersion = pCurrentElement->GetText();
                if(CCUserDefault::sharedUserDefault()->getStringForKey("VIPVersion1", "") == "")
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("VIPNewVersion1", sVIPVersion);
                    sendVIP();
                }
                else if(CCUserDefault::sharedUserDefault()->getStringForKey("VIPVersion1") != sVIPVersion)//本地保存的VIP版本号跟新的不一致，说明VIP有更新
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("VIPNewVersion1", sVIPVersion);
                    sendVIP();
                }
                else//本地已经保存VIP，并且没有更新内容，则此时直接打开VIP即可
                {
                    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
                    VIPLayer* pVIPLayer = VIPLayer::create(true,CMainLogic::sharedMainLogic()->m_nGameToHallVipLevel);
                    this->addChild(pVIPLayer, 200, 19995);
                }
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
}

void GameLayer::sendVIP()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/LeleVipConfigInfo.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(GameLayer::onHttpVIPComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void GameLayer::onHttpVIPComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    std::string sVIPNewVersion = CCUserDefault::sharedUserDefault()->getStringForKey("VIPNewVersion1");
    CCUserDefault::sharedUserDefault()->setStringForKey("VIPVersion1", sVIPNewVersion);
    
    VIPLayer* pVIPLayer = VIPLayer::create(false, CMainLogic::sharedMainLogic()->m_nGameToHallVipLevel);
    this->addChild(pVIPLayer, 200, 19995);
    pVIPLayer -> parseData(str);
    
    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
    
}
