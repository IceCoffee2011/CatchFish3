#include "header.h"

enum H3MenuTag
{
    eH3BackItemTag=101,
    eH3ChargeItemTag,
    eH3RankItemTag,
    eH3ReapetBeteTag,
    eH3HighBeteTag,
    eH3MiddleBeteTag,
    eH3SmallBeteTag,
    eH3JiangchiTag,
    eH3RedTypeTag,
    eH3BlackTypeTag,
};

const CCPoint SendLiangCardPoint[6] = {ccp(229, 626), ccp(296, 626), ccp(363, 626), ccp(775, 626), ccp(842, 626), ccp(909, 626)};
const CCPoint SendCardPoint[6] = {ccp(229, 626), ccp(909, 626), ccp(296, 626), ccp(842, 626), ccp(363, 626), ccp(775, 626)};

const CCPoint SendBetPoint[10] = {ccp(987, 470), ccp(150, 470), ccp(150, 347), ccp(150, 247), ccp(150, 147), ccp(987, 347), ccp(987, 247), ccp(987, 147), ccp(210, 46), ccp(128, 46)};
const CCPoint PlayerBgPoint[8] = {ccp(893, 485), ccp(253, 485), ccp(150, 347), ccp(150, 247), ccp(150, 147), ccp(987, 347), ccp(987, 247), ccp(987, 147)};

const CCPoint PlayerPoint[10] = {ccp(987, 470), ccp(150, 470), ccp(150, 347), ccp(150, 247), ccp(150, 147), ccp(987, 347), ccp(987, 247), ccp(987, 147), ccp(230, 46), ccp(128, 46)};

const CCPoint ZoushiPoint1[7] = {ccp(114, 22), ccp(194, 22), ccp(114+80*2, 22), ccp(114+80*3, 22), ccp(114+80*4, 22), ccp(114+80*5, 22), ccp(114+80*6, 22)};//114+80*i, 22));90+38*i, 58

const CCPoint ZoushiPoint2[15] = {ccp(90, 58), ccp(90+38*1, 58), ccp(90+38*2, 58), ccp(90+38*3, 58), ccp(90+38*4, 58), ccp(90+38*5, 58), ccp(90+38*6, 58), ccp(90+38*7, 58), ccp(90+38*8, 58), ccp(90+38*9, 58), ccp(90+38*10, 58), ccp(90+38*11, 58), ccp(90+38*12, 58), ccp(90+38*13, 58), ccp(90+38*14, 58)};

const CCPoint txPoint[8] = {ccp(212, 16), ccp(37, 16), ccp(42, 42), ccp(42, 42), ccp(42, 42), ccp(42, 42), ccp(42, 42), ccp(42, 42)};

const char* H3CardPaiXing[7] = {"h3card_chip_235.png", "h3card_chip_danzhang.png", "h3card_chip_duizi.png", "h3card_chip_shunzi.png", "h3card_chip_jinhua.png", "h3card_chip_shunjin.png", "h3card_chip_baozi.png"};


extern char* txName[10] ;
extern char* PailuType[2];
extern char* PailuDiType[2];
extern char* PailuMideleType[2];
extern char* PailuName[7];


H3CardBeteLayer* H3CardBeteLayer::_instance = NULL;

H3CardBeteLayer::H3CardBeteLayer()
{
    
}

H3CardBeteLayer::~H3CardBeteLayer()
{
    m_arrCardBack -> release();
    m_arrBetBlackSprite -> release();
    m_arrBetRedSprite -> release();
    m_arrBetSpecialSprite -> release();
    m_arrRicherBg -> release();
    m_arrCardLiang -> release();
    m_arrPailu1 -> release();
    m_arrPailu2 -> release();
    
    delete m_pClientKernel;
    m_pClientKernel = NULL;
    _instance = NULL;
}

H3CardBeteLayer* H3CardBeteLayer::getInstance()
{
    if(_instance == NULL)
    {
        _instance=new H3CardBeteLayer();
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

CCScene* H3CardBeteLayer::scene()
{
    CCScene *scene = CCScene::create();
    if (_instance == NULL)
    {
        _instance=new H3CardBeteLayer();
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



bool H3CardBeteLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    this -> setScale(1.125f);
    srand((unsigned)time(NULL));
    m_bXuanZhong = false;
    m_nBetArea = 0;
    m_nSendCardIndex = 0;
    m_nCurrentStatus = 0;
    m_llCurrentSelectBet = 0;//默认选择最高的下注范围
    m_llOwnUserScore = 0;
    m_llLowCoin = 0;
    m_llMiddleCoin = 0;
    m_llHighCoin = 0;
    m_llBlackBetCoin = 0;
    m_llRedBetCoin = 0;
    m_llSpecialBetCoin = 0;
    m_nClickPlayerPos = 0;
    m_nBetTimes = BET_TIME;
    m_bSpecialCardType = false;
    m_nCardResult = 0;
    m_llRepeatBlackBet = 0;
    m_llRepeatRedBet = 0;
    m_llRepeatSpecialBet = 0;
    m_dwBigWinUserID = 0;
    m_llBigWinScore = 0;
    m_llJackpot = 149458943;
    m_llNewJackpot = 149458943;
    m_nCurrentOnlineNum = 0;
    
    for(int i = 0; i < 20; i++)
    {
        m_nPailu1[0] = rand()%2;
        m_nPailu2[0] = rand()%2;
        m_nPailu3[0] = rand()%7;
    }
    for(int i = 0; i < 8; i++)
    {
        m_dwPlayerUserID[i] = 0;
    }
    m_dwPlayerUserID[8] = CMainLogic::sharedMainLogic()->m_nUserID;
    //CCUserDefault::sharedUserDefault()->setBoolForKey("RepeatBet", false);
    
    m_arrCardBack = CCArray::create();
    m_arrCardBack -> retain();
    m_arrBetBlackSprite = CCArray::create();
    m_arrBetBlackSprite -> retain();
    m_arrBetRedSprite = CCArray::create();
    m_arrBetRedSprite -> retain();
    m_arrBetSpecialSprite = CCArray::create();
    m_arrBetSpecialSprite -> retain();
    m_arrRicherBg = CCArray::create();
    m_arrRicherBg -> retain();
    m_arrCardLiang = CCArray::create();
    m_arrCardLiang -> retain();
    m_arrPailu1 = CCArray::create();
    m_arrPailu1 -> retain();
    m_arrPailu2 = CCArray::create();
    m_arrPailu2 -> retain();
    
    m_nBlackType = 0;
    m_nRedType = 0;
    
    loadRes();
    
    CClientKernel * pClientKernel = new CClientKernel();
    pClientKernel->m_pIClientKernelSink = this;
    m_pClientKernel=pClientKernel;
    
    CMainLogic::sharedMainLogic()->BeforeGameStart();
    CMainLogic::sharedMainLogic()->SendGameOption();
    
    return true;
}

void H3CardBeteLayer::loadRes()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3card/h3card_common.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3card/h3card_jiangchi.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3card/h3card_player.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3card/h3card_pailu.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3card/h3card_big_hongbao.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3card/h3card_poker.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3card/h3card_chip.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3card/h3card_vs.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3card/h3card_donghua.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3card/h3card_dayingjia.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3card/h3card_emo_btn.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3card/h3card_rank.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3card/h3card_kaijiang.plist");
    
    loadBgUI();
    
    this -> setTouchEnabled(true);
}

void H3CardBeteLayer::loadBgUI()
{
    m_pCardBeteBg = CCSprite::create("h3card/nopack_bairen.jpg");
    m_pCardBeteBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_pCardBeteBg);
    
    m_pContainBg = CCSprite::create("h3card/nopack_contentpanel.png");
    m_pContainBg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/2, m_pCardBeteBg->getContentSize().height/2));
    m_pCardBeteBg -> addChild(m_pContainBg, 50);
    CCLabelTTF* pLabel = CCLabelTTF::create("不能同时在红黑方下注", "Arial", 30);
    pLabel -> setPosition(ccp(m_pContainBg->getContentSize().width/2, m_pContainBg->getContentSize().height/2));
    m_pContainBg -> addChild(pLabel);
    m_pContainBg -> setVisible(false);
    
    CCSprite* pTopBg = CCSprite::createWithSpriteFrameName("h3card_common_topcardbg.png");
    pTopBg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/2, m_pCardBeteBg->getContentSize().height-pTopBg->getContentSize().height/2+32));
    m_pCardBeteBg -> addChild(pTopBg);
    CCSprite* pBottomBg = CCSprite::createWithSpriteFrameName("h3card_common_dibian.png");
    pBottomBg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/2, pBottomBg->getContentSize().height/2));
    m_pCardBeteBg -> addChild(pBottomBg);
    
    m_pCardYingBg = CCSprite::create("h3card/h3card_common_fanpai_huoshengpaixing.png");
    m_pCardYingBg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/4+10, 580));
    m_pCardBeteBg -> addChild(m_pCardYingBg);
    m_pCardYingBg -> setColor(ccc3(224, 205, 176));
    m_pCardYingBg -> setOpacity(0);
    m_pCardYingBg -> setVisible(false);

    
    m_pBetSpecialBg = CCSprite::createWithSpriteFrameName("h3card_kaijiang01.png");
    m_pBetSpecialBg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/2, m_pCardBeteBg->getContentSize().height/3+37));
    m_pCardBeteBg -> addChild(m_pBetSpecialBg, 30);
    m_pBetSpecialBg -> setColor(ccYELLOW);
    m_pBetSpecialBg -> setOpacity(0);
    
    m_pBetBlackBg = CCSprite::createWithSpriteFrameName("h3card_kaijiang012.png");
    m_pBetBlackBg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/3+5, m_pCardBeteBg->getContentSize().height/2-25));
    m_pCardBeteBg -> addChild(m_pBetBlackBg, 30);
    m_pBetBlackBg -> setFlipX(true);
    m_pBetBlackBg -> setColor(ccYELLOW);
    m_pBetBlackBg -> setOpacity(0);
    
    m_pBetBlack2Bg = CCSprite::create("h3card/h3card_kaijiang013.png");
    m_pBetBlack2Bg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/3+5, m_pCardBeteBg->getContentSize().height/2-25));
    m_pCardBeteBg -> addChild(m_pBetBlack2Bg, 30);
    m_pBetBlack2Bg -> setColor(ccYELLOW);
    m_pBetBlack2Bg -> setOpacity(0);
    
    m_pBetRedBg = CCSprite::createWithSpriteFrameName("h3card_kaijiang012.png");
    m_pBetRedBg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/3*2-5, m_pCardBeteBg->getContentSize().height/2-25));
    m_pCardBeteBg -> addChild(m_pBetRedBg, 30);
    m_pBetRedBg -> setColor(ccYELLOW);
    m_pBetRedBg -> setOpacity(0);
    
    m_pXiazhuBlackDi = CCSprite::createWithSpriteFrameName("h3card_player_zijixiazhu.png");
    m_pXiazhuBlackDi -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/4, m_pCardBeteBg->getContentSize().height/2-60));
    m_pCardBeteBg -> addChild(m_pXiazhuBlackDi, 50);
    m_pXiazhuRedDi = CCSprite::createWithSpriteFrameName("h3card_player_zijixiazhu.png");
    m_pXiazhuRedDi -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/4*3, m_pCardBeteBg->getContentSize().height/2-60));
    m_pCardBeteBg -> addChild(m_pXiazhuRedDi, 50);
    m_pXiazhuSpecialDi = CCSprite::createWithSpriteFrameName("h3card_player_zijixiazhu.png");
    m_pXiazhuSpecialDi -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/2, m_pCardBeteBg->getContentSize().height/3+25));
    m_pCardBeteBg -> addChild(m_pXiazhuSpecialDi, 50);
    m_pXiazhuBlackDi -> setVisible(false);
    m_pXiazhuRedDi -> setVisible(false);
    m_pXiazhuSpecialDi -> setVisible(false);
    //自己下注label
    m_labelOwnBetBlack = CCLabelTTF::create("", "Arial", 22);
    m_labelOwnBetBlack -> setPosition(ccp(m_pXiazhuBlackDi->getContentSize().width/2, m_pXiazhuBlackDi->getContentSize().height/2));
    m_pXiazhuBlackDi -> addChild(m_labelOwnBetBlack);
    m_labelOwnBetRed = CCLabelTTF::create("", "Arial", 22);
    m_labelOwnBetRed -> setPosition(ccp(m_pXiazhuRedDi->getContentSize().width/2, m_pXiazhuRedDi->getContentSize().height/2));
    m_pXiazhuRedDi -> addChild(m_labelOwnBetRed);
    m_labelOwnBetSpecial = CCLabelTTF::create("", "Arial", 22);
    m_labelOwnBetSpecial -> setPosition(ccp(m_pXiazhuSpecialDi->getContentSize().width/2, m_pXiazhuSpecialDi->getContentSize().height/2));
    m_pXiazhuSpecialDi -> addChild(m_labelOwnBetSpecial);
    m_labelOwnBetBlack -> setColor(ccc3(199, 174, 109));
    m_labelOwnBetRed -> setColor(ccc3(199, 174, 109));
    m_labelOwnBetSpecial -> setColor(ccc3(199, 174, 109));
    //三个区域总下注label
    m_labelBetBlack = CCLabelTTF::create("", "Arial", 26);
    m_labelBetBlack -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/3-40, 410));
    m_pCardBeteBg -> addChild(m_labelBetBlack, 30);
    m_labelBetRed = CCLabelTTF::create("", "Arial", 26);
    m_labelBetRed -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/3*2+25, 410));
    m_pCardBeteBg -> addChild(m_labelBetRed, 50);
    m_labelBetSpecial = CCLabelTTF::create("", "Arial", 26);
    m_labelBetSpecial -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/2, 326));
    m_pCardBeteBg -> addChild(m_labelBetSpecial);
    m_labelBetBlack -> setColor(ccc3(236, 230, 136));
    m_labelBetRed -> setColor(ccc3(236, 230, 136));
    m_labelBetSpecial -> setColor(ccc3(236, 230, 136));
    
    
    m_rtBetBlackBg = CCRenderTexture::create(m_pBetBlackBg->getContentSize().width, m_pBetBlackBg->getContentSize().height, kCCTexture2DPixelFormat_RGBA8888);
    m_rtBetBlackBg -> ignoreAnchorPointForPosition(true);
    m_rtBetBlackBg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/3+5, m_pCardBeteBg->getContentSize().height/2-25));
    m_rtBetBlackBg -> setAnchorPoint(CCPointZero);
    m_pCardBeteBg -> addChild(m_rtBetBlackBg, 0, 1);
    
    m_rtBetRedBg = CCRenderTexture::create(m_pBetRedBg->getContentSize().width, m_pBetRedBg->getContentSize().height, kCCTexture2DPixelFormat_RGBA8888);
    m_rtBetRedBg -> ignoreAnchorPointForPosition(true);
    m_rtBetRedBg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/3*2-5, m_pCardBeteBg->getContentSize().height/2-25));
    m_rtBetRedBg -> setAnchorPoint(CCPointZero);
    m_pCardBeteBg -> addChild(m_rtBetRedBg, 0, 1);
    
    m_rtBetSpecialBg = CCRenderTexture::create(m_pBetSpecialBg->getContentSize().width, m_pBetSpecialBg->getContentSize().height, kCCTexture2DPixelFormat_RGBA8888);
    m_rtBetSpecialBg -> ignoreAnchorPointForPosition(true);
    m_rtBetSpecialBg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/2, m_pCardBeteBg->getContentSize().height/3+40));
    m_rtBetSpecialBg -> setAnchorPoint(CCPointZero);
    m_pCardBeteBg -> addChild(m_rtBetSpecialBg, 0, 1);
    
    //退出按钮
    CCSprite* spBack = CCSprite::createWithSpriteFrameName("h3card_common_btn_return.png");
    CCMenuItemSprite* pBackItem = CCMenuItemSprite::create(spBack, spBack, this, menu_selector(H3CardBeteLayer::clickMenu));
    pBackItem -> setTag(eH3BackItemTag);
    pBackItem -> setPosition(ccp(pBackItem->getContentSize().width-5, m_pCardBeteBg->getContentSize().height-pBackItem->getContentSize().height+10));
    CCMenu* pBackMenu = CCMenu::create(pBackItem, NULL);
    pBackMenu -> setPosition(CCPointZero);
    pBackMenu -> setAnchorPoint(CCPointZero);
    m_pCardBeteBg -> addChild(pBackMenu, 20);
    //充值按钮
    CCSprite* spCharge = CCSprite::createWithSpriteFrameName("h3card_common_add.png");
    CCMenuItemSprite* pChargeItem = CCMenuItemSprite::create(spCharge, spCharge, this, menu_selector(H3CardBeteLayer::clickMenu));
    pChargeItem -> setTag(eH3ChargeItemTag);
    pChargeItem -> setPosition(ccp(m_pCardBeteBg->getContentSize().width-pChargeItem->getContentSize().width+5, m_pCardBeteBg->getContentSize().height-pChargeItem->getContentSize().height+10));
    CCMenu* pChargeMenu = CCMenu::create(pChargeItem, NULL);
    pChargeMenu -> setPosition(CCPointZero);
    pChargeMenu -> setAnchorPoint(CCPointZero);
    m_pCardBeteBg -> addChild(pChargeMenu, 20);
    //排行榜按钮
    CCSprite* spRank = CCSprite::createWithSpriteFrameName("h3card_common_rank.png");
    CCMenuItemSprite* pRankItem = CCMenuItemSprite::create(spRank, spRank, this, menu_selector(H3CardBeteLayer::clickMenu));
    pRankItem -> setTag(eH3RankItemTag);
    pRankItem -> setPosition(ccp(pBottomBg->getContentSize().width-140, pBottomBg->getContentSize().height/2));
    CCMenu* pRankMenu = CCMenu::create(pRankItem, NULL);
    pRankMenu -> setPosition(CCPointZero);
    pRankMenu -> setAnchorPoint(CCPointZero);
    pBottomBg -> addChild(pRankMenu);
    //重复投按钮
    CCSprite* spReapetBete = CCSprite::createWithSpriteFrameName("h3card_common_btn.png");
    CCSprite* spReapetBete2 = CCSprite::createWithSpriteFrameName("h3card_common_btn.png");
    CCSprite* spNoBete = CCSprite::createWithSpriteFrameName("h3card_common_btnbukexuan.png");
    CCMenuItemSprite* pReapetBeteItem = CCMenuItemSprite::create(spReapetBete2, spReapetBete2, this, menu_selector(H3CardBeteLayer::clickBeteMenu));
    pReapetBeteItem -> setTag(eH3ReapetBeteTag);
    pReapetBeteItem -> setPosition(ccp(896, 40));
    CCSprite* spReapetKuang = CCSprite::createWithSpriteFrameName("h3card_common_kongxuankuang.png");
    spReapetKuang -> setPosition(ccp(spReapetKuang->getContentSize().width/2+10, spReapetKuang->getContentSize().height/2+13));
    spReapetBete2 -> addChild(spReapetKuang);
    m_spXuanZhong = CCSprite::createWithSpriteFrameName("h3card_common_xuanzhong.png");
    m_spXuanZhong -> setPosition(ccp(spReapetKuang->getContentSize().width/2, spReapetKuang->getContentSize().height/2));
    spReapetKuang -> addChild(m_spXuanZhong);
    if(!m_bXuanZhong)
    {
        m_spXuanZhong -> setVisible(false);
    }
    
    CCLabelTTF* pLabelReapet = CCLabelTTF::create("重复投", "Arial", 20);
    pLabelReapet -> setPosition(ccp(80, 30));
    spReapetBete2 -> addChild(pLabelReapet);
    CCMenuItemSprite* pHighItem = CCMenuItemSprite::create(spReapetBete, spReapetBete, spNoBete, this, menu_selector(H3CardBeteLayer::clickBeteMenu));
    pHighItem -> setTag(eH3HighBeteTag);
    pHighItem -> setPosition(ccp(771, 40));
    CCMenuItemSprite* pMiddleItem = CCMenuItemSprite::create(spReapetBete, spReapetBete, spNoBete, this, menu_selector(H3CardBeteLayer::clickBeteMenu));
    pMiddleItem -> setTag(eH3MiddleBeteTag);
    pMiddleItem -> setPosition(ccp(646, 40));
    CCMenuItemSprite* pSmallItem = CCMenuItemSprite::create(spReapetBete, spReapetBete, spNoBete, this, menu_selector(H3CardBeteLayer::clickBeteMenu));
    pSmallItem -> setTag(eH3SmallBeteTag);
    pSmallItem -> setPosition(ccp(521, 40));
    //下注按钮
    CCMenu* pBeteMenu = CCMenu::create(pReapetBeteItem, pHighItem, pMiddleItem, pSmallItem, NULL);
    pBeteMenu -> setPosition(CCPointZero);
    pBeteMenu -> setAnchorPoint(CCPointZero);
    pBottomBg -> addChild(pBeteMenu);
    pReapetBeteItem -> setVisible(false);
    m_pSeleteBete = CCSprite::createWithSpriteFrameName("h3card_common_btnxuanzhong.png");
    m_pSeleteBete -> setPosition(ccp(771, 40));
    pBottomBg -> addChild(m_pSeleteBete);
    m_labelLowCoin = CCLabelTTF::create("", "Arial", 24);
    m_labelLowCoin -> setPosition(ccp(521, 40));
    pBottomBg -> addChild(m_labelLowCoin);
    m_labelMiddleCoin = CCLabelTTF::create("", "Arial", 24);
    m_labelMiddleCoin -> setPosition(ccp(646, 40));
    pBottomBg -> addChild(m_labelMiddleCoin);
    m_labelHighCoin = CCLabelTTF::create("", "Arial", 24);
    m_labelHighCoin -> setPosition(ccp(771, 40));
    pBottomBg -> addChild(m_labelHighCoin);
    
    //自己的信息框（昵称、金币数量）
    CCSprite* spNickNameBg = CCSprite::createWithSpriteFrameName("h3card_player_dibianxinxi.png");
    spNickNameBg -> setPosition(ccp(360, 68));
    pBottomBg -> addChild(spNickNameBg);
    CCLabelTTF* pLabelNickName = CCLabelTTF::create(CMainLogic::sharedMainLogic()->m_sNickName.c_str(), "Arial-BoldMT", 20);
    pLabelNickName -> setPosition(ccp(spNickNameBg->getContentSize().width/2, spNickNameBg->getContentSize().height/2));
    spNickNameBg -> addChild(pLabelNickName);
    CCSprite* spCoinBg = CCSprite::createWithSpriteFrameName("h3card_player_dibianxinxi.png");
    CCSprite* spCoin2Bg = CCSprite::createWithSpriteFrameName("h3card_player_dibianxinxi.png");
    spCoinBg -> setPosition(ccp(360, 32));
    pBottomBg -> addChild(spCoinBg);
    CCMenuItemSprite* pCoinItem = CCMenuItemSprite::create(spCoin2Bg, spCoin2Bg, this, menu_selector(H3CardBeteLayer::clickMenu));
    pCoinItem -> setTag(eH3ChargeItemTag);
    pCoinItem -> setOpacity(0);
    pCoinItem -> setPosition(ccp(spCoinBg->getContentSize().width/2, spCoinBg->getContentSize().height/2));
    CCMenu* pCoinMenu = CCMenu::create(pCoinItem, NULL);
    pCoinMenu -> setPosition(CCPointZero);
    pCoinMenu -> setAnchorPoint(CCPointZero);
    spCoinBg -> addChild(pCoinMenu);
    CCSprite* spCoinLogo = CCSprite::createWithSpriteFrameName("h3card_common_icongold.png");
    spCoinLogo -> setPosition(ccp(spCoinLogo->getContentSize().width/2, spCoinBg->getContentSize().height/2));
    spCoinBg -> addChild(spCoinLogo);
    char temp[16];
    sprintf(temp, "%lld", CMainLogic::sharedMainLogic()->m_lUserScore);
    m_pLabelOwnScore = CCLabelBMFont::create(temp, "h3card/h3card_golden.fnt");
    m_pLabelOwnScore -> setPosition(ccp(spCoinBg->getContentSize().width/2+8, spCoinBg->getContentSize().height/2));
    spCoinBg -> addChild(m_pLabelOwnScore);
    m_pLabelOwnScore -> setScale(0.7f);
    //自己头像框
    CCSprite* spTouxiangKuang = CCSprite::createWithSpriteFrameName("h3card_common_head_bg.png");
    spTouxiangKuang -> setPosition(ccp(213, pBottomBg->getContentSize().height/2));
    pBottomBg -> addChild(spTouxiangKuang);
    spTouxiangKuang -> setScale(0.5f);
    CCSprite* spTouxiang = NULL;
    if(CMainLogic::sharedMainLogic()->m_nTouxiangID >= 1 && CMainLogic::sharedMainLogic()->m_nTouxiangID <= 10)
    {
        spTouxiang = CCSprite::create(txName[CMainLogic::sharedMainLogic()->m_nTouxiangID-1]);
    }
    else
    {
        spTouxiang = CCSprite::createWithSpriteFrameName("h3card_common_head.png");
    }
    CCMenuItemSprite* pTouxiangItem = CCMenuItemSprite::create(spTouxiang, spTouxiang, this, menu_selector(H3CardBeteLayer::clickTouxiang));
    pTouxiangItem -> setTag(8);//下标为8，表示自己
    pTouxiangItem -> setPosition(ccp(213, pBottomBg->getContentSize().height/2));
    pTouxiangItem -> setScale(0.6f);
    //玩家列表按钮
    CCSprite* spWanjia = CCSprite::createWithSpriteFrameName("h3card_player_qitawanjia.png");
    CCMenuItemSprite* pWanjiaItem = CCMenuItemSprite::create(spWanjia, spWanjia, this, menu_selector(H3CardBeteLayer::clickWanjiaList));
    pWanjiaItem -> setPosition(ccp(128, 46));
    CCMenu* pTouxiangMenu = CCMenu::create(pTouxiangItem, pWanjiaItem, NULL);
    pTouxiangMenu -> setPosition(CCPointZero);
    pTouxiangMenu -> setAnchorPoint(CCPointZero);
    m_pCardBeteBg -> addChild(pTouxiangMenu, 20);
    //奖池
    CCSprite* spJiangchi = CCSprite::createWithSpriteFrameName("h3card_jiangchi.png");
    CCMenuItemSprite* pJiangchiItem = CCMenuItemSprite::create(spJiangchi, spJiangchi, this, menu_selector(H3CardBeteLayer::openJiangchi));
    pJiangchiItem -> setTag(eH3JiangchiTag);
    pJiangchiItem -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/2, m_pCardBeteBg->getContentSize().height-130));
    CCMenu* pJiangchiMenu = CCMenu::create(pJiangchiItem, NULL);
    pJiangchiMenu -> setPosition(CCPointZero);
    pJiangchiMenu -> setAnchorPoint(CCPointZero);
    m_pCardBeteBg -> addChild(pJiangchiMenu);
    m_labelJackpot = CCLabelBMFont::create("101770933", "h3card/h3card_lotterypool_btn_pot.fnt");
    m_labelJackpot -> setPosition(ccp(pJiangchiItem->getContentSize().width/2, pJiangchiItem->getContentSize().height/3));
    pJiangchiItem -> addChild(m_labelJackpot);
    
    //玩家列表（VIP或者大富豪、幸运星）
    CCSprite* spXingDi1 = CCSprite::createWithSpriteFrameName("h3card_player_bg.png");
    spXingDi1 -> setPosition(ccp(893, m_pCardBeteBg->getContentSize().height-155));
    m_pCardBeteBg -> addChild(spXingDi1, 20);
    CCLabelTTF* pXingName1 = CCLabelTTF::create("w111111", "Arial", 19);
    pXingName1 -> setPosition(ccp(85, 20));
    spXingDi1 -> addChild(pXingName1);
    pXingName1 -> setTag(98);
    CCSprite* spXingDi12 = CCSprite::createWithSpriteFrameName("h3card_player_luckkuang.png");
    spXingDi12 -> setPosition(ccp(spXingDi1->getContentSize().width-37, spXingDi1->getContentSize().height/2-17));
    spXingDi1 -> addChild(spXingDi12, 2);
    CCSprite* spXingID = CCSprite::createWithSpriteFrameName("h3card_player_luck.png");
    spXingID -> setPosition(ccp(85, 52));
    spXingDi1 -> addChild(spXingID, 2);
    
    CCSprite* spFuhaoDi1 = CCSprite::createWithSpriteFrameName("h3card_player_bg.png");
    spFuhaoDi1 -> setPosition(ccp(253, m_pCardBeteBg->getContentSize().height-155));
    m_pCardBeteBg -> addChild(spFuhaoDi1, 20);
    spFuhaoDi1 -> setFlipX(true);
    CCLabelTTF* pFuhaoName1 = CCLabelTTF::create("", "Arial", 19);
    pFuhaoName1 -> setPosition(ccp(172, 20));
    spFuhaoDi1 -> addChild(pFuhaoName1);
    pFuhaoName1 -> setTag(98);
    CCSprite* spFuhaoDi12 = CCSprite::createWithSpriteFrameName("h3card_player_fuhaokuang.png");
    spFuhaoDi12 -> setPosition(ccp(37, spFuhaoDi1->getContentSize().height/2-17));
    spFuhaoDi1 -> addChild(spFuhaoDi12, 2);
    CCSprite* spFuhaoID1 = CCSprite::createWithSpriteFrameName("h3card_player_fuhao1.png");
    spFuhaoID1 -> setPosition(ccp(172, 52));
    spFuhaoDi1 -> addChild(spFuhaoID1, 3);
    
    CCSprite* spFuhaoDi2 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    spFuhaoDi2 -> setPosition(ccp(150, 347));
    m_pCardBeteBg -> addChild(spFuhaoDi2, 20);
    CCSprite* spFuhaoID2 = CCSprite::createWithSpriteFrameName("h3card_player_jiaobiao2.png");
    spFuhaoID2 -> setPosition(ccp(spFuhaoID2->getContentSize().width/2, 60));
    spFuhaoDi2 -> addChild(spFuhaoID2, 3);
    CCLabelTTF* pFuhaoName2 = CCLabelTTF::create("", "Arial", 19);
    pFuhaoName2 -> setPosition(ccp(spFuhaoDi2->getContentSize().width/2, spFuhaoDi2->getContentSize().height-10));
    spFuhaoDi2 -> addChild(pFuhaoName2);
    pFuhaoName2 -> setTag(98);
    CCSprite* spFuhaoDi3 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    spFuhaoDi3 -> setPosition(ccp(150, 247));
    m_pCardBeteBg -> addChild(spFuhaoDi3, 20);
    CCSprite* spFuhaoID3 = CCSprite::createWithSpriteFrameName("h3card_player_jiaobiao3.png");
    spFuhaoID3 -> setPosition(ccp(spFuhaoID3->getContentSize().width/2, 60));
    spFuhaoDi3 -> addChild(spFuhaoID3, 3);
    CCLabelTTF* pFuhaoName3 = CCLabelTTF::create("w111111", "Arial", 19);
    pFuhaoName3 -> setPosition(ccp(spFuhaoDi3->getContentSize().width/2, spFuhaoDi3->getContentSize().height-10));
    spFuhaoDi3 -> addChild(pFuhaoName3);
    pFuhaoName3 -> setTag(98);
    CCSprite* spFuhaoDi4 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    spFuhaoDi4 -> setPosition(ccp(150, 147));
    m_pCardBeteBg -> addChild(spFuhaoDi4, 20);
    CCSprite* spFuhaoID4 = CCSprite::createWithSpriteFrameName("h3card_player_jiaobiao4.png");
    spFuhaoID4 -> setPosition(ccp(spFuhaoID4->getContentSize().width/2, 60));
    spFuhaoDi4 -> addChild(spFuhaoID4, 3);
    CCLabelTTF* pFuhaoName4 = CCLabelTTF::create("w111111", "Arial", 19);
    pFuhaoName4 -> setPosition(ccp(spFuhaoDi4->getContentSize().width/2, spFuhaoDi4->getContentSize().height-10));
    spFuhaoDi4 -> addChild(pFuhaoName4);
    pFuhaoName4 -> setTag(98);
    CCSprite* spFuhaoDi5 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    spFuhaoDi5 -> setPosition(ccp(987, 347));
    m_pCardBeteBg -> addChild(spFuhaoDi5, 20);
    CCSprite* spFuhaoID5 = CCSprite::createWithSpriteFrameName("h3card_player_jiaobiao5.png");
    spFuhaoID5 -> setPosition(ccp(spFuhaoID5->getContentSize().width/2, 60));
    spFuhaoDi5 -> addChild(spFuhaoID5, 3);
    CCLabelTTF* pFuhaoName5 = CCLabelTTF::create("w111111", "Arial", 19);
    pFuhaoName5 -> setPosition(ccp(spFuhaoDi5->getContentSize().width/2, spFuhaoDi5->getContentSize().height-10));
    spFuhaoDi5 -> addChild(pFuhaoName5);
    pFuhaoName5 -> setTag(98);
    CCSprite* spFuhaoDi6 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    spFuhaoDi6 -> setPosition(ccp(987, 247));
    m_pCardBeteBg -> addChild(spFuhaoDi6, 20);
    CCSprite* spFuhaoID6 = CCSprite::createWithSpriteFrameName("h3card_player_jiaobiao6.png");
    spFuhaoID6 -> setPosition(ccp(spFuhaoID6->getContentSize().width/2, 60));
    spFuhaoDi6 -> addChild(spFuhaoID6, 3);
    CCLabelTTF* pFuhaoName6 = CCLabelTTF::create("w111111", "Arial", 19);
    pFuhaoName6 -> setPosition(ccp(spFuhaoDi6->getContentSize().width/2, spFuhaoDi6->getContentSize().height-10));
    spFuhaoDi6 -> addChild(pFuhaoName6);
    pFuhaoName6 -> setTag(98);
    CCSprite* spFuhaoDi7 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    spFuhaoDi7 -> setPosition(ccp(987, 147));
    m_pCardBeteBg -> addChild(spFuhaoDi7, 20);

    CCSprite* spFuhaoID7 = CCSprite::createWithSpriteFrameName("h3card_player_jiaobiao7.png");
    spFuhaoID7 -> setPosition(ccp(spFuhaoID7->getContentSize().width/2, 60));
    spFuhaoDi7 -> addChild(spFuhaoID7, 3);
    CCLabelTTF* pFuhaoName7 = CCLabelTTF::create("w111111", "Arial", 19);
    pFuhaoName7 -> setPosition(ccp(spFuhaoDi7->getContentSize().width/2, spFuhaoDi7->getContentSize().height-10));
    spFuhaoDi7 -> addChild(pFuhaoName7);
    pFuhaoName7 -> setTag(98);
    pXingName1 -> setColor(ccc3(247, 243, 241));
    pFuhaoName1 -> setColor(ccc3(247, 243, 241));
    pFuhaoName2 -> setColor(ccc3(247, 243, 241));
    pFuhaoName3 -> setColor(ccc3(247, 243, 241));
    pFuhaoName4 -> setColor(ccc3(247, 243, 241));
    pFuhaoName5 -> setColor(ccc3(247, 243, 241));
    pFuhaoName6 -> setColor(ccc3(247, 243, 241));
    pFuhaoName7 -> setColor(ccc3(247, 243, 241));
    m_arrRicherBg -> addObject(spXingDi1);
    m_arrRicherBg -> addObject(spFuhaoDi1);
    m_arrRicherBg -> addObject(spFuhaoDi2);
    m_arrRicherBg -> addObject(spFuhaoDi3);
    m_arrRicherBg -> addObject(spFuhaoDi4);
    m_arrRicherBg -> addObject(spFuhaoDi5);
    m_arrRicherBg -> addObject(spFuhaoDi6);
    m_arrRicherBg -> addObject(spFuhaoDi7);
    
    CCMenuItemImage* txItem0 = CCMenuItemImage::create(txName[0], txName[0], this, menu_selector(H3CardBeteLayer::clickTouxiang));
    txItem0 -> setTag(0);
    txItem0 -> setPosition(ccp(spXingDi1->getContentSize().width-37, spXingDi1->getContentSize().height/2-17));
    txItem0 -> setScale(0.56f);
    CCMenu* txMenu0 = CCMenu::create(txItem0, NULL);
    txMenu0 -> setPosition(CCPointZero);
    txMenu0 -> setAnchorPoint(CCPointZero);
    spXingDi1 -> addChild(txMenu0);
    CCMenuItemImage* txItem1 = CCMenuItemImage::create(txName[0], txName[0], this, menu_selector(H3CardBeteLayer::clickTouxiang));
    txItem1 -> setPosition(ccp(37, spFuhaoDi1->getContentSize().height/2-17));
    txItem1 -> setScale(0.56f);
    txItem1 -> setTag(1);
    CCMenu* txMenu1 = CCMenu::create(txItem1, NULL);
    txMenu1 -> setPosition(CCPointZero);
    txMenu1 -> setAnchorPoint(CCPointZero);
    spFuhaoDi1 -> addChild(txMenu1);
    CCMenuItemImage* txItem2 = CCMenuItemImage::create(txName[0], txName[0], this, menu_selector(H3CardBeteLayer::clickTouxiang));
    txItem2 -> setPosition(ccp(spFuhaoDi2->getContentSize().width/2, spFuhaoDi2->getContentSize().height/2-8));
    txItem2 -> setScale(0.56f);
    txItem2 -> setTag(2);
    CCMenu* txMenu2 = CCMenu::create(txItem2, NULL);
    txMenu2 -> setPosition(CCPointZero);
    txMenu2 -> setAnchorPoint(CCPointZero);
    spFuhaoDi2 -> addChild(txMenu2);
    CCMenuItemImage* txItem3 = CCMenuItemImage::create(txName[0], txName[0], this, menu_selector(H3CardBeteLayer::clickTouxiang));
    txItem3 -> setPosition(ccp(spFuhaoDi3->getContentSize().width/2, spFuhaoDi3->getContentSize().height/2-8));
    txItem3 -> setScale(0.56f);
    txItem3 -> setTag(3);
    CCMenu* txMenu3 = CCMenu::create(txItem3, NULL);
    txMenu3 -> setPosition(CCPointZero);
    txMenu3 -> setAnchorPoint(CCPointZero);
    spFuhaoDi3 -> addChild(txMenu3);
    CCMenuItemImage* txItem4 = CCMenuItemImage::create(txName[0], txName[0], this, menu_selector(H3CardBeteLayer::clickTouxiang));
    txItem4 -> setPosition(ccp(spFuhaoDi4->getContentSize().width/2, spFuhaoDi4->getContentSize().height/2-8));
    txItem4 -> setScale(0.56f);
    txItem4 -> setTag(4);
    CCMenu* txMenu4 = CCMenu::create(txItem4, NULL);
    txMenu4 -> setPosition(CCPointZero);
    txMenu4 -> setAnchorPoint(CCPointZero);
    spFuhaoDi4 -> addChild(txMenu4);
    CCMenuItemImage* txItem5 = CCMenuItemImage::create(txName[0], txName[0], this, menu_selector(H3CardBeteLayer::clickTouxiang));
    txItem5 -> setPosition(ccp(spFuhaoDi5->getContentSize().width/2, spFuhaoDi5->getContentSize().height/2-8));
    txItem5 -> setScale(0.56f);
    txItem5 -> setTag(5);
    CCMenu* txMenu5 = CCMenu::create(txItem5, NULL);
    txMenu5 -> setPosition(CCPointZero);
    txMenu5 -> setAnchorPoint(CCPointZero);
    spFuhaoDi5 -> addChild(txMenu5);
    CCMenuItemImage* txItem6 = CCMenuItemImage::create(txName[0], txName[0], this, menu_selector(H3CardBeteLayer::clickTouxiang));
    txItem6 -> setPosition(ccp(spFuhaoDi6->getContentSize().width/2, spFuhaoDi6->getContentSize().height/2-8));
    txItem6 -> setScale(0.56f);
    txItem6 -> setTag(6);
    CCMenu* txMenu6 = CCMenu::create(txItem6, NULL);
    txMenu6 -> setPosition(CCPointZero);
    txMenu6 -> setAnchorPoint(CCPointZero);
    spFuhaoDi6 -> addChild(txMenu6);
    CCMenuItemImage* txItem7 = CCMenuItemImage::create(txName[0], txName[0], this, menu_selector(H3CardBeteLayer::clickTouxiang));
    txItem7 -> setPosition(ccp(spFuhaoDi7->getContentSize().width/2, spFuhaoDi7->getContentSize().height/2-8));
    txItem7 -> setScale(0.56f);
    txItem7 -> setTag(7);
    CCMenu* txMenu7 = CCMenu::create(txItem7, NULL);
    txMenu7 -> setPosition(CCPointZero);
    txMenu7 -> setAnchorPoint(CCPointZero);
    spFuhaoDi7 -> addChild(txMenu7);
    txMenu0 -> setTag(99);
    txMenu1 -> setTag(99);
    txMenu2 -> setTag(99);
    txMenu3 -> setTag(99);
    txMenu4 -> setTag(99);
    txMenu5 -> setTag(99);
    txMenu6 -> setTag(99);
    txMenu7 -> setTag(99);
    
    //文字描述说明
    m_labelCurrentTable = CCLabelTTF::create("1号桌:当前共252位玩家", "Arial", 20);
    m_labelCurrentTable -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/2, m_pCardBeteBg->getContentSize().height-32));
    m_pCardBeteBg -> addChild(m_labelCurrentTable);
    m_labelCurrentTable -> setColor(ccc3(189, 177, 129));
    m_labelBeteInfo = CCLabelTTF::create("", "Arial", 22);
    m_labelBeteInfo -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/2, m_pCardBeteBg->getContentSize().height-200));
    m_pCardBeteBg -> addChild(m_labelBeteInfo);
    
    //走势框
    CCSprite* spZoushi = CCSprite::createWithSpriteFrameName("h3card_pailu_zoushi.png");
    CCMenuItemSprite* pZoushiItem = CCMenuItemSprite::create(spZoushi, spZoushi, this, menu_selector(H3CardBeteLayer::clickZoushi));
    pZoushiItem -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/2, 128));
    m_pZoushiMenu = CCMenu::create(pZoushiItem, NULL);
    m_pZoushiMenu -> setPosition(CCPointZero);
    m_pZoushiMenu -> setAnchorPoint(CCPointZero);
    m_pCardBeteBg -> addChild(m_pZoushiMenu, 20);
//    CCSprite* pXiangqing = CCSprite::createWithSpriteFrameName("h3card_pailu_xiangqing.png");
//    pXiangqing -> setPosition(ccp(pZoushiItem->getContentSize().width-pXiangqing->getContentSize().width/2-12, pZoushiItem->getContentSize().height/2-4));
//    pZoushiItem -> addChild(pXiangqing);
//    CCSprite* spHongbao = CCSprite::createWithSpriteFrameName("h3card_lihua.png");
//    CCMenuItemSprite* pHongbaoItem = CCMenuItemSprite::create(spHongbao, spHongbao, this, menu_selector(H3CardBeteLayer::clickHongbao));
//    pHongbaoItem -> setPosition(ccp(pHongbaoItem->getContentSize().width/2+14, pZoushiItem->getContentSize().height/2-4));
//    CCMenu* pHongbaoMenu = CCMenu::create(pHongbaoItem, NULL);
//    pHongbaoMenu -> setPosition(CCPointZero);
//    pHongbaoMenu -> setAnchorPoint(CCPointZero);
//    pZoushiItem -> addChild(pHongbaoMenu);
    
    
    //规则信息
    CCLabelTTF* pLabel1 = CCLabelTTF::create("豹子", "Arial", 18);
    pLabel1 -> setPosition(ccp(400, 210));
    m_pCardBeteBg -> addChild(pLabel1, 20);
    CCLabelTTF* pLabel2 = CCLabelTTF::create("15倍", "Arial", 18);
    pLabel2 -> setPosition(ccp(400, 190));
    m_pCardBeteBg -> addChild(pLabel2, 20);
    CCLabelTTF* pLabel3 = CCLabelTTF::create("顺金", "Arial", 18);
    pLabel3 -> setPosition(ccp(480, 210));
    m_pCardBeteBg -> addChild(pLabel3, 20);
    CCLabelTTF* pLabel4 = CCLabelTTF::create("10倍", "Arial", 18);
    pLabel4 -> setPosition(ccp(480, 190));
    m_pCardBeteBg -> addChild(pLabel4, 20);
    CCLabelTTF* pLabel5 = CCLabelTTF::create("金花", "Arial", 18);
    pLabel5 -> setPosition(ccp(560, 210));
    m_pCardBeteBg -> addChild(pLabel5, 20);
    CCLabelTTF* pLabel6 = CCLabelTTF::create("4倍", "Arial", 18);
    pLabel6 -> setPosition(ccp(560, 190));
    m_pCardBeteBg -> addChild(pLabel6, 20);
    CCLabelTTF* pLabel7 = CCLabelTTF::create("顺子", "Arial", 18);
    pLabel7 -> setPosition(ccp(640, 210));
    m_pCardBeteBg -> addChild(pLabel7, 20);
    CCLabelTTF* pLabel8 = CCLabelTTF::create("3倍", "Arial", 18);
    pLabel8 -> setPosition(ccp(640, 190));
    m_pCardBeteBg -> addChild(pLabel8, 20);
    CCLabelTTF* pLabel9 = CCLabelTTF::create("对子9-A", "Arial", 18);
    pLabel9 -> setPosition(ccp(720, 210));
    m_pCardBeteBg -> addChild(pLabel9, 20);
    CCLabelTTF* pLabel10 = CCLabelTTF::create("2倍", "Arial", 18);
    pLabel10 -> setPosition(ccp(720, 190));
    m_pCardBeteBg -> addChild(pLabel10, 20);
}

void H3CardBeteLayer::startSendBackCard()
{
    for(int i = 0; i < 6; i++)
    {
        CCSprite* spPoker = CCSprite::createWithSpriteFrameName("poker_back.png");
        spPoker -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/2, 626));
        m_pCardBeteBg -> addChild(spPoker, 10);
        m_arrCardBack -> addObject(spPoker);
        spPoker -> setAnchorPoint(ccp(0.5f, 1.0f));
        spPoker -> setScale(0.67f);
    }
    this -> schedule(schedule_selector(H3CardBeteLayer::scheduleSendClientCard), 0.5f);
}

void H3CardBeteLayer::scheduleSendClientCard()
{
    if(m_nSendCardIndex >= m_arrCardBack->count())
    {
        this -> unschedule(schedule_selector(H3CardBeteLayer::scheduleSendClientCard));
        m_nSendCardIndex = 0;
        return ;
    }
    CCSprite* spPoker = (CCSprite*)m_arrCardBack->objectAtIndex(m_nSendCardIndex);
    if(spPoker != NULL)
    {
        spPoker -> runAction(CCMoveTo::create(0.5f, SendCardPoint[m_nSendCardIndex]));
    }
    m_nSendCardIndex++;
}

void H3CardBeteLayer::showReceiveLiangCard()//收到从服务端返回的亮牌消息
{
    for(int i = 0; i < m_arrCardBack->count(); i++)
    {
        CardSprite* pCardSprite = (CardSprite*)m_arrCardBack->objectAtIndex(i);
        pCardSprite->removeFromParent();
    }
    m_arrCardBack->removeAllObjects();
    for(int i = 0; i < 3; i++)
    {
        CardSprite* spPoker = CardSprite::create("bg.png", "poker_back.png", m_cbBlackCard[i], 0.5);
        spPoker -> setPosition(SendLiangCardPoint[i]);
        m_pCardBeteBg -> addChild(spPoker, 10);
        spPoker -> setAnchorPoint(ccp(0.5f, 1.0f));
        spPoker -> setScale(0.67f);
        m_arrCardLiang -> addObject(spPoker);//0、1、2为黑方底牌，3、4、5为红方底牌
    }
    for(int i = 0; i < 3; i++)
    {
        CardSprite* spPoker = CardSprite::create("bg.png", "poker_back.png", m_cbRedCard[i], 0.5);
        spPoker -> setPosition(SendLiangCardPoint[i+3]);
        m_pCardBeteBg -> addChild(spPoker, 10);
        spPoker -> setAnchorPoint(ccp(0.5f, 1.0f));
        spPoker -> setScale(0.67f);
        m_arrCardLiang -> addObject(spPoker);//0、1、2为黑方底牌，3、4、5为红方底牌
    }
    showCardAnimate();
}

void H3CardBeteLayer::showCardAnimate()//亮牌定时器
{
    this -> scheduleOnce(schedule_selector(H3CardBeteLayer::scheduleShowCard1Animate), 1.0f);
    this -> scheduleOnce(schedule_selector(H3CardBeteLayer::scheduleShowCard2Animate), 1.2f);
    this -> scheduleOnce(schedule_selector(H3CardBeteLayer::scheduleShowCard3Animate), 1.8f);
    this -> scheduleOnce(schedule_selector(H3CardBeteLayer::scheduleShowCard4Animate), 2.1f);
    this -> scheduleOnce(schedule_selector(H3CardBeteLayer::scheduleShowCard5Animate), 2.7f);
    this -> scheduleOnce(schedule_selector(H3CardBeteLayer::scheduleShowCard6Animate), 4.2f);
}

void H3CardBeteLayer::scheduleShowCard1Animate()
{
    CardSprite* spPoker = (CardSprite*)m_arrCardLiang->objectAtIndex(0);
    if(spPoker != NULL)
    {
        spPoker -> openCard();
    }
}

void H3CardBeteLayer::scheduleShowCard2Animate()
{
    CardSprite* spPoker = (CardSprite*)m_arrCardLiang->objectAtIndex(1);
    if(spPoker != NULL)
    {
        spPoker -> openCard();
    }
}

void H3CardBeteLayer::scheduleShowCard3Animate()
{
    CardSprite* spPoker = (CardSprite*)m_arrCardLiang->objectAtIndex(3);
    if(spPoker != NULL)
    {
        spPoker -> openCard();
    }
}

void H3CardBeteLayer::scheduleShowCard4Animate()
{
    CardSprite* spPoker = (CardSprite*)m_arrCardLiang->objectAtIndex(4);
    if(spPoker != NULL)
    {
        spPoker -> openCard();
    }
}

void H3CardBeteLayer::scheduleShowCard5Animate()
{
    CardSprite* spPoker = (CardSprite*)m_arrCardLiang->objectAtIndex(2);
    if(spPoker != NULL)
    {
        spPoker -> runAction(CCSequence::create(CCScaleTo::create(0.3f, 1.0f), CCDelayTime::create(0.3f), CCCallFuncN::create(this, callfuncN_selector(H3CardBeteLayer::showCard2Animate)), CCDelayTime::create(1.0f), CCScaleTo::create(0.2f, 0.6f), CCScaleTo::create(0.1f, 0.67f), CCCallFuncN::create(this, callfuncN_selector(H3CardBeteLayer::showBlackType)), NULL));
    }
}

void H3CardBeteLayer::scheduleShowCard6Animate()
{
    CardSprite* spPoker = (CardSprite*)m_arrCardLiang->objectAtIndex(5);
    if(spPoker != NULL)
    {
        spPoker -> runAction(CCSequence::create(CCScaleTo::create(0.3f, 1.0f), CCDelayTime::create(0.3f), CCCallFuncN::create(this, callfuncN_selector(H3CardBeteLayer::showCard2Animate)), CCDelayTime::create(1.0f), CCScaleTo::create(0.2f, 0.6f), CCScaleTo::create(0.1f, 0.67f), CCCallFuncN::create(this, callfuncN_selector(H3CardBeteLayer::showRedType)), NULL));
    }
}

void H3CardBeteLayer::showCard2Animate(CCNode* pNode)
{
    CardSprite* spPoker = (CardSprite*)pNode;
    if(spPoker != NULL)
    {
        spPoker -> openCard();
    }
}

void H3CardBeteLayer::showBlackType(CCNode* pNode)
{
    if(m_nBlackType > 6 && m_nBlackType < 0)
        return ;
    CCSprite* pCardType = CCSprite::createWithSpriteFrameName(H3CardPaiXing[m_nBlackType]);
    pCardType -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/4+10, 555));
    m_pCardBeteBg -> addChild(pCardType, 11, eH3BlackTypeTag);
    pCardType -> setScale(0.6f);
}

void H3CardBeteLayer::showRedType(CCNode* pNode)
{
    if(m_nRedType > 6 && m_nRedType < 0)
        return ;
    CCSprite* pCardType = CCSprite::createWithSpriteFrameName(H3CardPaiXing[m_nRedType]);
    pCardType -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/4*3-10, 555));
    m_pCardBeteBg -> addChild(pCardType, 11, eH3RedTypeTag);
    pCardType -> setScale(0.6f);
    
    //更新状态为正在结算
    m_labelBeteInfo -> setString("正在结算");
}

void H3CardBeteLayer::clickBeteMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case eH3ReapetBeteTag://重复投按钮
        {
//            if(m_bXuanZhong)
//            {
//                m_bXuanZhong = false;
//                m_spXuanZhong -> setVisible(false);
//                CCUserDefault::sharedUserDefault()->setBoolForKey("RepeatBet", false);
//            }
//            else
//            {
//                m_bXuanZhong = true;
//                m_spXuanZhong -> setVisible(true);
//                CCUserDefault::sharedUserDefault()->setBoolForKey("RepeatBet", true);
//            }
            break;
        }
        case eH3HighBeteTag:
        {
            m_pSeleteBete -> setPosition(ccp(771, 40));
            m_llCurrentSelectBet = m_llHighCoin;
            break;
        }
        case eH3MiddleBeteTag:
        {
            m_pSeleteBete -> setPosition(ccp(646, 40));
            m_llCurrentSelectBet = m_llMiddleCoin;
            break;
        }
        case eH3SmallBeteTag:
        {
            m_pSeleteBete -> setPosition(ccp(521, 40));
            m_llCurrentSelectBet = m_llLowCoin;
            break;
        }
        default:
            break;
    }
}

void H3CardBeteLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    item -> setScale(1.0f);
    item->runAction(CCSequence::create(CCScaleTo::create(0.1f, 0.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
    int nTag = item->getTag();
    if(nTag != eH3BackItemTag)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    }
    else
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    }
    switch (item->getTag()) {
        case eH3BackItemTag://退出游戏
        {
            tagUserInfo * pUserInfo = m_pClientKernel->GetMeUserInfo();
            CMainLogic::sharedMainLogic()->SendStandUpPacket(pUserInfo->wTableID, pUserInfo->wChairID, 1);
            //@@切换到房间列表，临时不能切换到桌子界面
            CMainLogic::sharedMainLogic()->ShowMessage("正在返回到房间选择界面", eMBExitGame);
            //@@关闭音效
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
            SimpleAudioEngine::sharedEngine()->stopAllEffects();
            break;
        }
        case eH3ChargeItemTag://充值按钮
        {
            ChargeLayer* pChargeLayer = ChargeLayer::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeLayer, 11, 20000);
            break;
        }
        case eH3RankItemTag://排行榜按钮
        {
            H3RankLayer* pRankLayer = H3RankLayer::create();
            this -> addChild(pRankLayer, 60);
            break;
        }
        default:
            break;
    }
}

void H3CardBeteLayer::openJiangchi(CCObject* object)
{
    //SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
}

void H3CardBeteLayer::scheduleShowJackpot()
{
    static int nTimes = 0;
    nTimes++;
    SCORE llDif = m_llNewJackpot-m_llJackpot;
    SCORE nTempMoney = llDif/50*nTimes;
    char sMoney[32];
    sprintf(sMoney, "%lld", m_llJackpot+nTempMoney);
    m_labelJackpot->setString(sMoney);
    if (nTimes == 50)
    {
        m_llJackpot = m_llNewJackpot;
        sprintf(sMoney, "%lld", m_llJackpot);
        m_labelJackpot -> setString(sMoney);
        this->unschedule(schedule_selector(H3CardBeteLayer::scheduleShowJackpot));
        nTimes = 0;
        return;
    }
}

void H3CardBeteLayer::removeNode(CCNode* pNode)
{
    pNode -> removeFromParent();
}

void H3CardBeteLayer::clickTouxiang(CCObject* object)//弹出玩家信息框，共9个位置
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    int nTag = item->getTag();
    m_nClickPlayerPos = nTag;
    PlayerInfoLayer* pLayer = PlayerInfoLayer::create(m_dwPlayerUserID[m_nClickPlayerPos]);
    this -> addChild(pLayer, 60, 998);
    
    CMD_C_USE_INFO UseInfo = {0};
    UseInfo.dwUserId = m_dwPlayerUserID[nTag];
    printf("click touxiang userid is %d\n", UseInfo.dwUserId);
    m_pClientKernel->SendSocketData(MDM_GF_GAME,ACTION_C_USER_INFO,&UseInfo,sizeof(UseInfo));
    
}

void H3CardBeteLayer::clickWanjiaList(CCObject* object)//玩家列表
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    PlayerListLayer* pLayer = PlayerListLayer::create();
    this -> addChild(pLayer, 60);
}

void H3CardBeteLayer::clickZoushi(CCObject* object)//走势图
{
    JackActionLayer* pLayer1 = JackActionLayer::create();
    this -> addChild(pLayer1, 60);
    return ;
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    PailuLayer* pLayer = PailuLayer::create();
    this -> addChild(pLayer, 60, 999);
}

void H3CardBeteLayer::clickHongbao(CCObject* object)//红包按钮
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    printf("发红包，燃放礼花\n");
}

void H3CardBeteLayer::onekeyBet()
{
    if(m_llRepeatSpecialBet > 0)
    {
        CMD_C_BET CBet = {0};
        CBet.nPos = 2;
        CBet.llBet = m_llRepeatSpecialBet;
        m_pClientKernel->SendSocketData(MDM_GF_GAME,ACTION_C_BET,&CBet,sizeof(CBet));
    }
    if(m_llRepeatRedBet > 0)
    {
        CMD_C_BET CBet = {0};
        CBet.nPos = 1;
        CBet.llBet = m_llRepeatRedBet;
        m_pClientKernel->SendSocketData(MDM_GF_GAME,ACTION_C_BET,&CBet,sizeof(CBet));
    }
    if(m_llRepeatBlackBet > 0)
    {
        CMD_C_BET CBet = {0};
        CBet.nPos = 0;
        CBet.llBet = m_llRepeatBlackBet;
        m_pClientKernel->SendSocketData(MDM_GF_GAME,ACTION_C_BET,&CBet,sizeof(CBet));
    }
}

void H3CardBeteLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
}

bool H3CardBeteLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if(m_nCurrentStatus != 1)//下注已经结束
    {
        return true;
    }
    bool bBetArea = false;
    CCPoint point = pTouch->getLocation();
    point = m_pCardBeteBg->convertToNodeSpace(point);
    CCRect rectRed = m_pBetRedBg->boundingBox();
    CCRect rectBlack = m_pBetBlackBg->boundingBox();
    CCRect rectSpecial = m_pBetSpecialBg->boundingBox();
    if(rectRed.containsPoint(point))
    {
        bBetArea = getOwnBetArea(pTouch, m_pBetRedBg, m_rtBetRedBg);
        if(bBetArea)
        {
            if(checkBetCoinOrOwnCoin())
            {
                if(m_nBetArea == 1)
                {
                    showNoBetBg();
                    return true;
                }
                int nPos = 8;
                for(int i = 0; i < 8; i++)
                {
                    if(CMainLogic::sharedMainLogic()->m_nUserID == m_dwPlayerUserID[i])
                    {
                        nPos = i;
                        break;
                    }
                }
                setPlayerBet(nPos, POS_RED, m_llCurrentSelectBet, true);//自己下注、红色区域
                if(nPos < 8)//底图运动
                {
                    CCSprite* pFuhaoDi = (CCSprite*)m_arrRicherBg->objectAtIndex(nPos);
                    if(pFuhaoDi != NULL)
                    {
                        pFuhaoDi -> stopAllActions();
                        pFuhaoDi -> setPosition(PlayerBgPoint[nPos]);
                        int nMove1 = 15;
                        int nMove2 = -15;
                        if(nPos == 0 || nPos == 5 || nPos == 6 || nPos == 7)
                        {
                            nMove1 = -15;
                            nMove2 = 15;
                        }
                        pFuhaoDi -> runAction(CCSequence::create(CCMoveBy::create(0.3f, ccp(nMove1, 0)), CCMoveBy::create(0.15f, ccp(nMove2, 0)), NULL));
                    }
                }
                sendOwnBet(POS_RED);
                m_nBetArea = 2;
            }
        }
    }
    if(!bBetArea)
    {
        if(rectBlack.containsPoint(point))
        {
            bBetArea = getOwnBetArea(pTouch, m_pBetBlack2Bg, m_rtBetBlackBg);
            if(bBetArea)
            {
                if(checkBetCoinOrOwnCoin())
                {
                    if(m_nBetArea == 2)
                    {
                        showNoBetBg();
                        return true;
                    }
                    int nPos = 8;
                    for(int i = 0; i < 8; i++)
                    {
                        if(CMainLogic::sharedMainLogic()->m_nUserID == m_dwPlayerUserID[i])
                        {
                            nPos = i;
                            break;
                        }
                    }
                    setPlayerBet(nPos, POS_BLACK, m_llCurrentSelectBet, true);//自己下注、黑色区域
                    if(nPos < 8)//底图运动
                    {
                        CCSprite* pFuhaoDi = (CCSprite*)m_arrRicherBg->objectAtIndex(nPos);
                        if(pFuhaoDi != NULL)
                        {
                            pFuhaoDi -> stopAllActions();
                            pFuhaoDi -> setPosition(PlayerBgPoint[nPos]);
                            int nMove1 = 15;
                            int nMove2 = -15;
                            if(nPos == 0 || nPos == 5 || nPos == 6 || nPos == 7)
                            {
                                nMove1 = -15;
                                nMove2 = 15;
                            }
                            pFuhaoDi -> runAction(CCSequence::create(CCMoveBy::create(0.3f, ccp(nMove1, 0)), CCMoveBy::create(0.15f, ccp(nMove2, 0)), NULL));
                        }
                    }
                    sendOwnBet(POS_BLACK);
                    m_nBetArea = 1;
                }
            }
        }
    }
    if(!bBetArea)
    {
        if(rectSpecial.containsPoint(point))
        {
            bBetArea = getOwnBetArea(pTouch, m_pBetSpecialBg, m_rtBetSpecialBg);
            if(bBetArea)
            {
                if(checkBetCoinOrOwnCoin())
                {
                    int nPos = 8;
                    for(int i = 0; i < 8; i++)
                    {
                        if(CMainLogic::sharedMainLogic()->m_nUserID == m_dwPlayerUserID[i])
                        {
                            nPos = i;
                            break;
                        }
                    }
                    setPlayerBet(nPos, POS_SPECIAL, m_llCurrentSelectBet, true);
                    if(nPos < 8)//底图运动
                    {
                        CCSprite* pFuhaoDi = (CCSprite*)m_arrRicherBg->objectAtIndex(nPos);
                        if(pFuhaoDi != NULL)
                        {
                            pFuhaoDi -> stopAllActions();
                            pFuhaoDi -> setPosition(PlayerBgPoint[nPos]);
                            int nMove1 = 15;
                            int nMove2 = -15;
                            if(nPos == 0 || nPos == 5 || nPos == 6 || nPos == 7)
                            {
                                nMove1 = -15;
                                nMove2 = 15;
                            }
                            pFuhaoDi -> runAction(CCSequence::create(CCMoveBy::create(0.3f, ccp(nMove1, 0)), CCMoveBy::create(0.15f, ccp(nMove2, 0)), NULL));
                        }
                    }
                    sendOwnBet(POS_SPECIAL);
                }
            }
        }
    }
    if(!bBetArea)
    {
        printf("contain not point\n");
    }
    
    return true;
}

void H3CardBeteLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
}

std::string H3CardBeteLayer::getNickName(std::string strName)
{
    if(strlen(strName.c_str()) > 9)
    {
        std::string str = strName.substr(0, 9) + "..";
        return str;
    }
    return strName;
}

std::string H3CardBeteLayer::getString2FromNumber(SCORE llScore)
{
    char temp[32];
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

std::string H3CardBeteLayer::getString3FromNumber(SCORE llScore)
{
    char temp[32];
    if(llScore >= 10000)
    {
        if(llScore%10000 == 0)
        {
            sprintf(temp, "+%lld万", llScore/10000);
        }
        else
        {
            if(llScore%1000 == 0)
            {
                sprintf(temp, "+%.1f万", llScore/10000.0);
            }
            else
            {
                sprintf(temp, "+%.2f万", llScore/10000.0);
            }
        }
    }
    else
    {
        sprintf(temp, "+%lld", llScore);
    }
    return temp;
}

void H3CardBeteLayer::showNoBetBg()
{
    m_pContainBg -> setVisible(true);
    m_pContainBg -> stopAllActions();
    m_pContainBg -> runAction(CCSequence::create(CCDelayTime::create(2.0f), CCHide::create(), NULL));
}

bool H3CardBeteLayer::checkBetCoinOrOwnCoin()
{
    bool bCanBet = true;
    if(m_llOwnUserScore < m_llCurrentSelectBet)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("您当前拥有金币数量不够", eMBOK);
        bCanBet = false;
    }
    return bCanBet;
}

bool H3CardBeteLayer::getOwnBetArea(CCTouch* pTouch, CCSprite* pBetSprite, CCRenderTexture* rtRender)
{
    bool bBetArea = false;
    ccColor4B color4B = {0, 0, 0, 0};
    
    CCPoint nodePos = pBetSprite->convertTouchToNodeSpace(pTouch);
    unsigned int x = nodePos.x;
    unsigned int y = pBetSprite->getContentSize().height - nodePos.y;
    
    CCPoint point = pBetSprite->getPosition();
    //开始准备绘制
    rtRender->begin();
    //绘制使用的临时精灵，与原图是同一图片
    CCSprite* pTempSpr = CCSprite::createWithSpriteFrame(pBetSprite->displayFrame());
    pTempSpr->setPosition(ccp(pTempSpr->getContentSize().width / 2, pTempSpr->getContentSize().height / 2));
    //绘制
    pTempSpr->visit();
    //结束绘制
    rtRender->end();
    //通过画布拿到这张画布上每个像素点的信息，封装到CCImage中
    CCImage* pImage = rtRender->newCCImage();
    //获取像素数据
    unsigned char* data_ = pImage->getData();
    unsigned int *pixel = (unsigned int *)data_;
    pixel = pixel + (y * (int)pTempSpr->getContentSize().width) * 1 + x * 1;
    //R通道
    color4B.r = *pixel & 0xff;
    //G通道
    color4B.g = (*pixel >> 8) & 0xff;
    //B通过
    color4B.b = (*pixel >> 16) & 0xff;
    //Alpha通道，我们有用的就是Alpha
    color4B.a = (*pixel >> 24) & 0xff;
    
    //printf("当前点击的点的: alpha = %d\n", color4B.a);
    
    if (color4B.a > 10) {
        bBetArea = true;
    } else {
    }
    
    //绘制完成后清理画布的内容
    rtRender->clear(0, 0, 0, 0);
    pImage -> release();
    
    return bBetArea;
}

/*第一个参数意思：0为幸运星位置、1为富豪1位置。。。7为富豪7位置，8为自己下注位置，9为玩家列表位置
  第二个参数意思：0代表黑色下注区域、1代表红色下注区域、2代表特殊牌型下注区域
*/
void H3CardBeteLayer::setPlayerBet(int nPosIndex, int nBetIndex, SCORE llBetScore, bool bIsMy)//玩家座位位置（10个）、下注位置（3个）
{
    if(nPosIndex < 0 || nPosIndex >= 10)
        return ;
    int nPox = 0;
    int nPoy = 0;
    int nIndex = 0;
    if(nBetIndex == POS_BLACK)//黑色区域
    {
        nIndex = rand()%2;
        if(nIndex == 0)
        {
            nPox = 250+rand()%170;
            nPoy = 345+rand()%25;
        }
        else
        {
            nPox = 250+rand()%80;
            nPoy = 240+rand()%110;
        }
    }
    else if(nBetIndex == POS_RED)//红色区域
    {
        nIndex = rand()%2;
        if(nIndex == 0)
        {
            nPox = 710+rand()%180;
            nPoy = 345+rand()%25;
        }
        else
        {
            nPox = 790+rand()%100;
            nPoy = 240+rand()%110;
        }
    }
    else//特殊牌型区域
    {
        nPox = 435+rand()%235;
        nPoy = 255+rand()%30;
    }
    
    
    float fDis = sqrt((nPox-SendBetPoint[nPosIndex].x)*(nPox-SendBetPoint[nPosIndex].x) + (nPoy-SendBetPoint[nPosIndex].y)*(nPoy-SendBetPoint[nPosIndex].y));
    float fTime = fDis/1000;
    BetSprite* pBetSprite = BetSprite::create(llBetScore, nPosIndex);
    pBetSprite -> setPosition(SendBetPoint[nPosIndex]);
    m_pCardBeteBg -> addChild(pBetSprite, 21);
    CCMoveTo* moveTo = CCMoveTo::create(fTime, ccp(nPox, nPoy));
    int nRotate = rand()%60-30+360;
    CCRotateBy* rotateBy = CCRotateBy::create(fTime, nRotate);
    CCEaseSineOut* pOut = CCEaseSineOut::create(moveTo);
    CCSequence* seq = CCSequence::create(CCDelayTime::create(0.1f), CCSpawn::create(pOut, rotateBy, NULL), NULL);
    pBetSprite -> runAction(seq);
    if(nBetIndex == POS_BLACK)
    {
        m_arrBetBlackSprite -> addObject(pBetSprite);
    }
    else if(nBetIndex == POS_RED)
    {
        m_arrBetRedSprite -> addObject(pBetSprite);
    }
    else
    {
        m_arrBetSpecialSprite -> addObject(pBetSprite);
    }
    
    if(bIsMy)//显示自己下注多少
    {
        showMyBetNum(nBetIndex);
    }
    
}

void H3CardBeteLayer::setBetReturn()
{
    m_labelBetBlack -> setString("");//黑方下注总数
    m_labelBetRed -> setString("");
    m_labelBetSpecial -> setString("");
    m_labelOwnBetBlack -> setString("");//自己下注黑方数量
    m_labelOwnBetRed -> setString("");
    m_labelOwnBetSpecial -> setString("");
    m_pXiazhuBlackDi -> setVisible(false);//自己下注底板
    m_pXiazhuRedDi -> setVisible(false);
    m_pXiazhuSpecialDi -> setVisible(false);
    if(m_bSpecialCardType)
    {
        for(int i = 0; i < m_arrBetSpecialSprite->count(); i++)
        {
            BetSprite* pBetSprite = (BetSprite*)m_arrBetSpecialSprite->objectAtIndex(i);
            if(pBetSprite != NULL)
            {
                int nPosIndex = pBetSprite->getChairID();
                if(nPosIndex < 0 || nPosIndex >= 10)
                    continue;
                float fTime = (rand()%5)/10.0f;
                float nPox = pBetSprite->getPositionX();
                float nPoy = pBetSprite->getPositionY();
                float fDis = sqrt((nPox-SendBetPoint[nPosIndex].x)*(nPox-SendBetPoint[nPosIndex].x) + (nPoy-SendBetPoint[nPosIndex].y)*(nPoy-SendBetPoint[nPosIndex].y));
                float fMoveTime = fDis/1000;
                CCMoveTo* moveTo = CCMoveTo::create(fMoveTime, SendBetPoint[nPosIndex]);
                pBetSprite -> runAction(CCSequence::create(CCDelayTime::create(fTime), moveTo, CCCallFuncN::create(this, callfuncN_selector(H3CardBeteLayer::removeThis)), NULL));
            }
        }
    }
    else
    {
        for(int i = 0; i < m_arrBetSpecialSprite->count(); i++)
        {
            BetSprite* pBetSprite = (BetSprite*)m_arrBetSpecialSprite->objectAtIndex(i);
            if(pBetSprite != NULL)
            {
                pBetSprite -> removeFromParent();
            }
        }
        m_arrBetSpecialSprite -> removeAllObjects();
    }
    
    if(m_nCardResult == 0)//黑方胜
    {
        for(int i = 0; i < m_arrBetBlackSprite->count(); i++)
        {
            BetSprite* pBetSprite = (BetSprite*)m_arrBetBlackSprite->objectAtIndex(i);
            if(pBetSprite != NULL)
            {
                int nPosIndex = pBetSprite->getChairID();
                if(nPosIndex < 0 || nPosIndex >= 10)
                    continue;
                float fTime = (rand()%5)/10.0f;
                float nPox = pBetSprite->getPositionX();
                float nPoy = pBetSprite->getPositionY();
                float fDis = sqrt((nPox-SendBetPoint[nPosIndex].x)*(nPox-SendBetPoint[nPosIndex].x) + (nPoy-SendBetPoint[nPosIndex].y)*(nPoy-SendBetPoint[nPosIndex].y));
                float fMoveTime = fDis/1000;
                CCMoveTo* moveTo = CCMoveTo::create(fMoveTime, SendBetPoint[nPosIndex]);
                pBetSprite -> runAction(CCSequence::create(CCDelayTime::create(fTime), moveTo, CCCallFuncN::create(this, callfuncN_selector(H3CardBeteLayer::removeThis)), NULL));
            }
        }
        for(int i = 0; i < m_arrBetRedSprite->count(); i++)
        {
            BetSprite* pBetSprite = (BetSprite*)m_arrBetRedSprite->objectAtIndex(i);
            if(pBetSprite != NULL)
            {
                pBetSprite -> removeFromParent();
            }
        }
        m_arrBetRedSprite -> removeAllObjects();
    }
    else if(m_nCardResult == 1)//红方胜
    {
        for(int i = 0; i < m_arrBetRedSprite->count(); i++)
        {
            BetSprite* pBetSprite = (BetSprite*)m_arrBetRedSprite->objectAtIndex(i);
            if(pBetSprite != NULL)
            {
                int nPosIndex = pBetSprite->getChairID();
                if(nPosIndex < 0 || nPosIndex >= 10)
                    continue;
                float fTime = (rand()%5)/10.0f;
                float nPox = pBetSprite->getPositionX();
                float nPoy = pBetSprite->getPositionY();
                float fDis = sqrt((nPox-SendBetPoint[nPosIndex].x)*(nPox-SendBetPoint[nPosIndex].x) + (nPoy-SendBetPoint[nPosIndex].y)*(nPoy-SendBetPoint[nPosIndex].y));
                float fMoveTime = fDis/1000;
                CCMoveTo* moveTo = CCMoveTo::create(fMoveTime, SendBetPoint[nPosIndex]);
                pBetSprite -> runAction(CCSequence::create(CCDelayTime::create(fTime), moveTo, CCCallFuncN::create(this, callfuncN_selector(H3CardBeteLayer::removeThis)), NULL));
            }
        }
        for(int i = 0; i < m_arrBetBlackSprite->count(); i++)
        {
            BetSprite* pBetSprite = (BetSprite*)m_arrBetBlackSprite->objectAtIndex(i);
            if(pBetSprite != NULL)
            {
                pBetSprite -> removeFromParent();
            }
        }
        m_arrBetBlackSprite -> removeAllObjects();
    }
    
}

void H3CardBeteLayer::sendOwnBet(int nBetIndex)
{
    CMD_C_BET CBet = {0};
    CBet.nPos = nBetIndex;
    CBet.llBet = m_llCurrentSelectBet;
    m_pClientKernel->SendSocketData(MDM_GF_GAME,ACTION_C_BET,&CBet,sizeof(CBet));
}

void H3CardBeteLayer::showMyBetNum(int nArea)//0代表黑色区域、1代表红色区域、2代表特殊区域
{
    if(nArea == POS_BLACK)
    {
        m_pXiazhuBlackDi -> setVisible(true);
        char temp[32];
        m_llBlackBetCoin += m_llCurrentSelectBet;
        if(m_llBlackBetCoin >= 10000)
        {
            sprintf(temp, "%lld万", m_llBlackBetCoin/10000);
        }
        else
        {
            sprintf(temp, "%lld", m_llBlackBetCoin);
        }
        m_labelOwnBetBlack -> setString(temp);
        setOwnScore(m_llOwnUserScore-m_llCurrentSelectBet);
    }
    else if(nArea == POS_RED)
    {
        m_pXiazhuRedDi -> setVisible(true);
        char temp[32];
        m_llRedBetCoin += m_llCurrentSelectBet;
        if(m_llRedBetCoin >= 10000)
        {
            sprintf(temp, "%lld万", m_llRedBetCoin/10000);
        }
        else
        {
            sprintf(temp, "%lld", m_llRedBetCoin);
        }
        m_labelOwnBetRed -> setString(temp);
        setOwnScore(m_llOwnUserScore-m_llCurrentSelectBet);
    }
    else
    {
        m_pXiazhuSpecialDi -> setVisible(true);
        char temp[32];
        m_llSpecialBetCoin += m_llCurrentSelectBet;
        if(m_llSpecialBetCoin >= 10000)
        {
            sprintf(temp, "%lld万", m_llSpecialBetCoin/10000);
        }
        else
        {
            sprintf(temp, "%lld", m_llSpecialBetCoin);
        }
        m_labelOwnBetSpecial -> setString(temp);
        setOwnScore(m_llOwnUserScore-m_llCurrentSelectBet);
    }
}

//ClientKernel
bool H3CardBeteLayer::OnEventSocket(const TCP_Command & Command, const void * pBuffer, WORD wDataSize)
{
    //printf("%d, %s, %hu, %hu\n", __LINE__, __FUNCTION__, Command.wMainCmdID, Command.wSubCmdID);
    if (MDM_GF_GAME!=Command.wMainCmdID&&Command.wSubCmdID!=SUB_GF_SYSTEM_MESSAGE)
    {
        return false;
    }
    switch (Command.wSubCmdID)
    {
        case SUB_GF_SYSTEM_MESSAGE://公告
        {
            //printf("红黑大战公告\n");
            return true;
        }
        case SUB_CMD_STATUS:
        {
            CMD_B_GAME_STATUS* pGameStatus = (CMD_B_GAME_STATUS*)pBuffer;
            if(pGameStatus != NULL)
            {
                printf("pGameStatus is %d, time is %d\n", pGameStatus->m_nStatus, pGameStatus->nCountdown);
                if(pGameStatus->m_nStatus == GAME_BET_BEGIN)//开始下注
                {
                    printf("开始下注\n");
                    m_nCurrentStatus = 1;
                    m_nBetTimes = pGameStatus->nCountdown;
                    m_labelBeteInfo -> setString("开始下注");
                    if(pGameStatus->nCountdown > 2)
                    {
                        betStart();//开始下注动画
                    }
                    //重复投功能
//                    if(CCUserDefault::sharedUserDefault()->getBoolForKey("RepeatBet", false))
//                    {
//                        onekeyBet();
//                        setOwnBetCoin(m_llRepeatBlackBet, m_llRepeatRedBet, m_llRepeatSpecialBet);
//                    }
                    
                    if(pGameStatus->nCountdown > 4)
                    {
                        this -> schedule(schedule_selector(H3CardBeteLayer::scheduleShowBet), 1.0f);
                    }
                }
                else if(pGameStatus->m_nStatus == GAME_BET_END)//结束下注
                {
                    printf("结束下注\n");
                    m_nCurrentStatus = 2;
                    m_labelBeteInfo -> setString("正在开牌");
//                    if(CCUserDefault::sharedUserDefault()->getBoolForKey("RepeatBet", false))
//                    {
//                        m_llRepeatSpecialBet = m_llSpecialBetCoin;
//                        m_llRepeatRedBet = m_llRedBetCoin;
//                        m_llRepeatBlackBet = m_llBlackBetCoin;
//                    }
//                    else
//                    {
//                        m_llRepeatSpecialBet = 0;
//                        m_llRepeatRedBet = 0;
//                        m_llRepeatBlackBet = 0;
//                    }
                    betFinish();
                }
                else if(pGameStatus->m_nStatus == GAME_OPEN_CARDS)//亮牌
                {
                    printf("亮牌\n");
                    m_labelBeteInfo -> setString("正在开牌");
                }
                else if(pGameStatus->m_nStatus == GAME_RESULT)//结果
                {
                    m_labelBeteInfo -> setString("正在结算");
                    printf("结果\n");
                }
                else if(pGameStatus->m_nStatus == GAME_NEW_GAME)//新游戏
                {
                    m_labelBeteInfo -> setString("等待开局");
                    printf("新游戏，下一局开始\n");
                    resetData();//重置数据
                    vsAction();
                }
                else if(pGameStatus->m_nStatus == GAME_OPEN_JACKPOT)//奖池开奖
                {
                    printf("奖池开奖");
                }
            }
            break;
        }
        case SUB_CMD_B_BET://下注结果
        {
            printf("下注结果所有人\n");
            CMD_B_BET* pBet = (CMD_B_BET*)pBuffer;
            if(pBet != NULL)
            {
                //printf("dwUserID is %d, nPos is %d, bet is %lld, blackBet is %lld, redBet is %lld, specialBet is %lld\n", pBet->dwUserID, pBet->nPos, pBet->llBet, pBet->llDeskMoney[0], pBet->llDeskMoney[1], pBet->llDeskMoney[2]);
                setBetAreaTotal(pBet->llDeskMoney[0], pBet->llDeskMoney[1], pBet->llDeskMoney[2]);
                if(pBet->dwUserID != CMainLogic::sharedMainLogic()->m_nUserID)
                {
                    int nPos = 9;
                    for(int i = 0; i < 8; i++)
                    {
                        if(m_dwPlayerUserID[i] == pBet->dwUserID)
                        {
                            nPos = i;
                            break;
                        }
                    }
                    if(nPos < 8)
                    {
                        CCSprite* pFuhaoDi = (CCSprite*)m_arrRicherBg->objectAtIndex(nPos);
                        if(pFuhaoDi != NULL)
                        {
                            pFuhaoDi -> stopAllActions();
                            pFuhaoDi -> setPosition(PlayerBgPoint[nPos]);
                            int nMove1 = 15;
                            int nMove2 = -15;
                            if(nPos == 0 || nPos == 5 || nPos == 6 || nPos == 7)
                            {
                                nMove1 = -15;
                                nMove2 = 15;
                            }
                            pFuhaoDi -> runAction(CCSequence::create(CCMoveBy::create(0.3f, ccp(nMove1, 0)), CCMoveBy::create(0.15f, ccp(nMove2, 0)), NULL));
                        }
                    }
                    setPlayerBet(nPos, pBet->nPos, pBet->llBet, false);
                    
                }
            }
            break;
        }
        case SUB_CMD_S_BET:
        {
            //printf("下注自己结果\n");
            CMD_S_BET* pBet = (CMD_S_BET*)pBuffer;
            if(pBet != NULL)
            {
                //printf("bet is %lld, blackBet is %lld, redBet is %lld, specialBet is %lld\n", pBet->llCurrentScore, pBet->llBet[0], pBet->llBet[1], pBet->llBet[2]);
                setOwnScore(pBet->llCurrentScore);//更新自己的当前分数
                setOwnBetCoin(pBet->llBet[0], pBet->llBet[1], pBet->llBet[2]);
            }
            break;
        }
        case SUB_CMD_OPEN_CARDS://亮牌
        {
            CMD_B_OPEN_CARDS* pOpenCards = (CMD_B_OPEN_CARDS*)pBuffer;
            if(pOpenCards != NULL)
            {
                //printf("亮牌\n");
                printf("BlackType is %d, redType is %d, blackvalue is %d, redvalue is %d\n", pOpenCards->nBlackType, pOpenCards->nRedType, pOpenCards->nBlackValue, pOpenCards->nRedValue);
//                printf("black card is %hhu, %hhu, %hhu\n", pOpenCards->cbBlackCards[0], pOpenCards->cbBlackCards[1], pOpenCards->cbBlackCards[2]);
//                printf("red card is %hhu, %hhu, %hhu\n", pOpenCards->cbRedCards[0], pOpenCards->cbRedCards[1], pOpenCards->cbRedCards[2]);
                for(int i = 0; i < 19; i++)
                {
                    m_nPailu3[i] = m_nPailu3[i+1];
                    m_nPailu2[i] = m_nPailu2[i+1];
                    m_nPailu1[i] = m_nPailu1[i+1];
                }
                m_nBlackType = pOpenCards->nBlackType;
                m_nRedType = pOpenCards->nRedType;
                int nMax = pOpenCards->nBlackValue > pOpenCards->nRedValue ? pOpenCards->nBlackValue : pOpenCards->nRedValue;
                if(nMax > 20900)
                {
                    m_bSpecialCardType = true;
                    m_nPailu1[19] = 1;
                }
                else
                {
                    m_nPailu1[19] = 0;
                }
                if(nMax >= 60000)
                {
                    m_nPailu3[19] = 6;//豹子
                }
                else if(nMax >= 50000)
                {
                    m_nPailu3[19] = 5;//顺金
                }
                else if(nMax >= 40000)
                {
                    m_nPailu3[19] = 4;//金花
                }
                else if(nMax >= 30000)
                {
                    m_nPailu3[19] = 3;//顺子
                }
                else if(nMax > 20900)
                {
                    m_nPailu3[19] = 2;//大对子
                }
                else if(nMax >= 20000)
                {
                    m_nPailu3[19] = 1;//小对子
                }
                else
                {
                    m_nPailu3[19] = 0;//单张
                }
                for(int i = 0; i < 3; i++)
                {
                    m_cbRedCard[i] = pOpenCards->cbRedCards[i];
                    m_cbBlackCard[i] = pOpenCards->cbBlackCards[i];
                }
                showReceiveLiangCard();
            }
            break;
        }
        case SUB_CMD_REULT://结果
        {
            CMD_B_RESULT* pResult = (CMD_B_RESULT*)pBuffer;
            if(pResult != NULL)
            {
                printf("结果 result is %d\n", pResult->nWinner);
                m_nCardResult = pResult->nWinner;//0表示黑胜，1表示红胜
                m_nPailu2[19] = pResult->nWinner;
                showWinAnimate();
                showZoushiAnimate();
            }
            break;
        }
        case SUB_CMD_SELF_RESULT://自己结果
        {
            CMD_S_SELF_RESULT* pSelfResult = (CMD_S_SELF_RESULT*)pBuffer;
            if(pSelfResult != NULL)
            {
                printf("自己结果 当前金币：%lld, 输赢金币： %lld\n", pSelfResult->llCurrentScore, pSelfResult->llDeltaScore);
                setOwnScore(pSelfResult->llCurrentScore);
                int nPos = 8;
                for(int i = 0; i < 8; i++)
                {
                    if(CMainLogic::sharedMainLogic()->m_nUserID == m_dwPlayerUserID[i])
                    {
                        nPos = i;
                        break;
                    }
                }
                showPlayerWinScore(nPos, pSelfResult->llDeltaScore);
            }
            
            break;
        }
        case SUB_CMD_DECK_INFO:// 桌面玩家,7个土豪和1个幸运星
        {
            CMD_B_DECK_INFO* pDeckInfo = (CMD_B_DECK_INFO*)pBuffer;
            if(pDeckInfo != NULL)
            {
                printf("桌面玩家信息  dwUserID is %d, %d, %d, %d, %d, %d, %d, %d\n", pDeckInfo->dwUserIDs[0], pDeckInfo->dwUserIDs[1], pDeckInfo->dwUserIDs[2], pDeckInfo->dwUserIDs[3], pDeckInfo->dwUserIDs[4], pDeckInfo->dwUserIDs[5], pDeckInfo->dwUserIDs[6], pDeckInfo->dwUserIDs[7]);
                setPlayerInfo(pDeckInfo->dwUserIDs);
            }
            break;
        }
        case SUB_CMD_DECK_WITH_MONEY://桌面玩家赢家赢的金币
        {
            CMD_B_DECK_WITH_MONEY* pDeckWithMoney = (CMD_B_DECK_WITH_MONEY*)pBuffer;
            if(pDeckWithMoney != NULL)
            {
                printf("桌面玩家输赢情况 dwUserID is %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld\n", pDeckWithMoney->llMoneys[0], pDeckWithMoney->llMoneys[1], pDeckWithMoney->llMoneys[2], pDeckWithMoney->llMoneys[3], pDeckWithMoney->llMoneys[4], pDeckWithMoney->llMoneys[5], pDeckWithMoney->llMoneys[6], pDeckWithMoney->llMoneys[7]);
                for(int i = 0; i < 8; i++)//不显示自己的输赢情况
                {
                    if(CMainLogic::sharedMainLogic()->m_nUserID != m_dwPlayerUserID[i])
                    {
                        showPlayerWinScore(i, pDeckWithMoney->llMoneys[i]);
                    }
                }
            }
            break;
        }
        case SUB_CMD_TOP_WINNER://本轮赢得最多的玩家和赢的金币数
        {
            CMD_B_TOP_WINNER* pTopWinner = (CMD_B_TOP_WINNER*)pBuffer;
            if(pTopWinner != NULL)
            {
                printf("本轮赢取最多的玩家  topWinner is %d, %lld\n", pTopWinner->dwUserID, pTopWinner->llMoney);
                m_dwBigWinUserID = pTopWinner->dwUserID;
                m_llBigWinScore = pTopWinner->llMoney;
            }
            break;
        }
        case SUB_CMD_JACKPOT://大奖池
        {
            CMD_B_JACKPOT* pJackpot = (CMD_B_JACKPOT*)pBuffer;
            if(pJackpot != NULL)
            {
                printf("大奖池   %lld\n", pJackpot->llMoney);
                m_llNewJackpot = pJackpot->llMoney;
                CCSprite* pJiangchiBg = CCSprite::createWithSpriteFrameName("h3card_jiangchi05.png");
                pJiangchiBg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/2, m_pCardBeteBg->getContentSize().height-148));
                m_pCardBeteBg -> addChild(pJiangchiBg);
                pJiangchiBg -> runAction(CCSpawn::create(CCFadeOut::create(2.0f), MyScaleTo::create(2.0f, 1.2f), NULL));
                
                this -> schedule(schedule_selector(H3CardBeteLayer::scheduleShowJackpot), 0.04f);
            }
            break;
        }
        case SUB_CMD_JACKPOT_RESULT:
        {
            printf("jackpot_ result\n");
            break;
        }
        case SUB_CMD_JACKOIT_AWARD:
        {
            printf("jackoit_ award\n");
            CMD_B_JACKPOT_AWARD* pJackpotAward = (CMD_B_JACKPOT_AWARD*)pBuffer;
            if(pJackpotAward != NULL)
            {
                printf("award is %d\n", pJackpotAward->nNum);
                for(int i = 0; i < pJackpotAward->nNum; i++)
                {
                    printf("%d, %lld\n", pJackpotAward->items[i].dwUserID, pJackpotAward->items[i].llMoney);
                }
                printf("\n");
            }
            break;
        }
        case SUB_CMD_ERROR://错误码
        {
            CMD_S_ERROR* pError = (CMD_S_ERROR*)pBuffer;
            if(pError != NULL)
            {
                printf("错误码 等级  %d， ID %d\n", pError->nLevel, pError->nErrorCode);
            }
            break;
        }
        case SUB_CMD_ONLINE_INfO://桌子桌号信息
        {
            CMD_B_ONLINE_INfO* pOnlineInfo = (CMD_B_ONLINE_INfO*)pBuffer;
            if(pBuffer != NULL)
            {
                m_nCurrentOnlineNum = pOnlineInfo->nOnlineNum;
                setTableInfo(pOnlineInfo->nRoomId, pOnlineInfo->nOnlineNum);
            }
            break;
        }
        case SUB_CMD_TREND://走势
        {
            printf("trend\n");
            CMD_B_Trend* pTrend = (CMD_B_Trend*)pBuffer;
            if(pTrend != NULL)
            {
                for(int i = 0; i < SAMPLE_NUM; i++)
                {
                    m_nPailu2[i] = pTrend->items[i].nWinnerSide;
                    m_nPailu3[i] = pTrend->items[i].nCardType;
                    if(m_nPailu3[i] >= 2)
                    {
                        m_nPailu1[i] = 1;
                    }
                    else
                    {
                        m_nPailu1[i] = 0;
                    }
                }
                setZoushi();
            }
            break;
        }
        case SUB_CMD_BET_LEVEL://每局结束后更新下注信息
        {
            printf("bet level\n");
            CMD_S_BET_LEVEL* pBetLevel = (CMD_S_BET_LEVEL*)pBuffer;
            if(pBetLevel != NULL)
            {
                setUserBetInfo(pBetLevel->llLow, pBetLevel->llMid, pBetLevel->llHigh);
            }
            break;
        }
        case SUB_CMD_S_CHAT://表情玩法，暂不开发
        {
            CMD_S_CHAT* pChat = (CMD_S_CHAT*)pBuffer;
            if(pChat != NULL)
            {
                printf("chat money is %lld\n", pChat->llMoney);
            }
            break;
        }
        case SUB_CMD_B_CHAT://表情玩法，暂不开发
        {
            CMD_B_CHAT* pChat = (CMD_B_CHAT*)pBuffer;
            if(pChat != NULL)
            {
                printf("b chat %d, %d, %d\n", pChat->nChatType, pChat->dwFrom, pChat->dwTo);
            }
            break;
        }
        case SUB_CMD_S_USER_INFO://查询玩家信息
        {
            printf("USER_INFO\n");
            CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
            CMD_S_USE_INFO* pUseInfo = (CMD_S_USE_INFO*)pBuffer;
            if(pUseInfo != NULL)
            {
                PlayerInfoLayer* pLayer = (PlayerInfoLayer*)this->getChildByTag(998);
                if(pLayer != NULL)
                {
                    pLayer -> updateUI(pUseInfo->data[0], pUseInfo->data[1], pUseInfo->data[2], pUseInfo->data[3], pUseInfo->data[4]);
                }
                
            }
            break;
        }
            
    }
    return true;
}

bool H3CardBeteLayer::OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
    CMD_S_GAME_SCENE* pGameScene = (CMD_S_GAME_SCENE*)pBuffer;
    if(pGameScene != NULL)
    {
        printf("场景消息:  %lld, %lld, %lld, %lld\n", pGameScene->llMoney, pGameScene->llLow, pGameScene->llMid, pGameScene->llHigh);
        m_llCurrentSelectBet = pGameScene->llHigh;
        setUserBetInfo(pGameScene->llLow, pGameScene->llMid, pGameScene->llHigh);
        setOwnScore(pGameScene->llMoney);
    }
    return true;
}

bool H3CardBeteLayer::OnEventUserLeave(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)
{
    printf("%d, %s\n", __LINE__, __FUNCTION__);
    return true;
}

bool H3CardBeteLayer::OnEventUserEnter(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)
{
    printf("%d, %s\n", __LINE__, __FUNCTION__);
    return true;
}

void H3CardBeteLayer::setUserBetInfo(SCORE llLow, SCORE llMid, SCORE llHigh)
{
    m_llLowCoin = llLow;
    m_llMiddleCoin = llMid;
    m_llHighCoin = llHigh;
    char temp[32];
    if(llLow >= 10000)
    {
        sprintf(temp, "%lld万", llLow/10000);
    }
    else
    {
        sprintf(temp, "%lld", llLow);
    }
    m_labelLowCoin -> setString(temp);
    if(llMid >= 10000)
    {
        sprintf(temp, "%lld万", llMid/10000);
    }
    else
    {
        sprintf(temp, "%lld", llMid);
    }
    m_labelMiddleCoin -> setString(temp);
    if(llHigh >= 10000)
    {
        sprintf(temp, "%lld万", llHigh/10000);
    }
    else
    {
        sprintf(temp, "%lld", llHigh);
    }
    m_labelHighCoin -> setString(temp);
}

void H3CardBeteLayer::setOwnScore(SCORE llScore)
{
    m_llOwnUserScore = llScore;
    char temp[32];
    if(m_llOwnUserScore >= 1000000)
    {
        sprintf(temp, "%lld万", m_llOwnUserScore/10000);
    }
    else
    {
        sprintf(temp, "%lld", m_llOwnUserScore);
    }
    m_pLabelOwnScore -> setString(temp);
}

void H3CardBeteLayer::setOwnBetCoin(SCORE llBlack, SCORE llRed, SCORE llSpecial)
{
    if(llBlack > 0)//黑色区域有下注
    {
        m_pXiazhuBlackDi -> setVisible(true);
        m_labelOwnBetBlack -> setString(getString2FromNumber(llBlack).c_str());
    }
    if(llRed > 0)//红色区域有下注
    {
        m_pXiazhuRedDi -> setVisible(true);
        m_labelOwnBetRed -> setString(getString2FromNumber(llRed).c_str());
    }
    if(llSpecial > 0)//特殊区域有下注
    {
        m_pXiazhuSpecialDi -> setVisible(true);
        m_labelOwnBetSpecial -> setString(getString2FromNumber(llSpecial).c_str());
    }
}

void H3CardBeteLayer::setBetAreaTotal(SCORE llBlack, SCORE llRed, SCORE llSpecial)
{
    if(llBlack > 0)
    {
        m_labelBetBlack->setString(getString2FromNumber(llBlack).c_str());
    }
    if(llRed > 0)
    {
        m_labelBetRed->setString(getString2FromNumber(llRed).c_str());
    }
    if(llSpecial > 0)
    {
        m_labelBetSpecial->setString(getString2FromNumber(llSpecial).c_str());
    }
}

void H3CardBeteLayer::showPlayerWinScore(int nPlayerPos, SCORE llScore)
{
    if(nPlayerPos < 0 || nPlayerPos > 10)
        return ;
    if(llScore > 0)
    {
        CCLabelBMFont* pLabelScore = CCLabelBMFont::create(getString3FromNumber(llScore).c_str(),"h3card/h3card_winner_add.fnt");
        pLabelScore -> setPosition(PlayerPoint[nPlayerPos]);
        m_pCardBeteBg -> addChild(pLabelScore, 50);
        pLabelScore -> setVisible(false);
        pLabelScore -> runAction(CCSequence::create(CCDelayTime::create(1.0f), CCShow::create(), CCMoveBy::create(1.5f, ccp(0, 60)), CCFadeOut::create(0.5f), CCCallFuncN::create(this, callfuncN_selector(H3CardBeteLayer::removeThis)), NULL));
    }
}

void H3CardBeteLayer::setPlayerInfo(DWORD nUserID[8])
{
    for(int i = 0; i < 8; i++)
    {
        m_dwPlayerUserID[i] = nUserID[i];
        tagUserInfo* pUserInfo = CMainLogic::sharedMainLogic()->SearchUserByUserID(nUserID[i]);
        if(pUserInfo == NULL) continue;
        CCSprite* spdi = (CCSprite*)m_arrRicherBg->objectAtIndex(i);
        if(spdi == NULL) continue;
        if(pUserInfo->wFaceID >= 1 && pUserInfo->wFaceID <= 10)
        {
            CCMenu* pMenu = (CCMenu*)spdi->getChildByTag(99);
            if(pMenu != NULL)
            {
                pMenu -> removeFromParent();
                CCMenuItemImage* faceItem = CCMenuItemImage::create(txName[pUserInfo->wFaceID-1], txName[pUserInfo->wFaceID-1], this, menu_selector(H3CardBeteLayer::clickTouxiang));
                faceItem -> setTag(i);
                faceItem -> setPosition(txPoint[i]);
                faceItem -> setScale(0.56f);
                pMenu = CCMenu::create(faceItem, NULL);
                pMenu -> setPosition(CCPointZero);
                pMenu -> setAnchorPoint(CCPointZero);
                spdi -> addChild(pMenu);
                pMenu -> setTag(99);
            }
            CCLabelTTF* pLabelName = (CCLabelTTF*)spdi->getChildByTag(98);
            if(pLabelName != NULL)
            {
                pLabelName -> setString(getNickName(CCharCode::GB2312ToUTF8(pUserInfo->szNickName)).c_str());
            }
        }
    }
}

void H3CardBeteLayer::setTableInfo(int nTableID, int nNum)
{
    char temp[64];
    sprintf(temp, "%d号桌:当前共%d位玩家", nTableID, nNum);
    m_labelCurrentTable -> setString(temp);
}

void H3CardBeteLayer::setZoushi()
{
    CCArray* array = m_pZoushiMenu->getChildren();
    if(array == NULL || array->count() == 0)
        return ;
    CCMenuItem* pZoushiItem = (CCMenuItem*)array->objectAtIndex(0);
    if(pZoushiItem == NULL)
        return ;
    for(int i = 0; i < 7; i++)
    {
        CCSprite* spZoushi1 = CCSprite::createWithSpriteFrameName(PailuDiType[m_nPailu1[i+13]]);
        spZoushi1 -> setPosition(ZoushiPoint1[i]);
        pZoushiItem -> addChild(spZoushi1);
        m_arrPailu1 -> addObject(spZoushi1);
        CCLabelTTF* pLabelPailu = CCLabelTTF::create(PailuName[m_nPailu3[i+13]], "Arial", 20);
        pLabelPailu -> setPosition(ccp(spZoushi1->getContentSize().width/2, spZoushi1->getContentSize().height/2));
        spZoushi1 -> addChild(pLabelPailu);
        if(m_nPailu3[i+13] > 1)
        {
            pLabelPailu -> setColor(ccc3(125, 78, 13));
        }
        else
        {
            pLabelPailu -> setColor(ccc3(232, 206, 143));
        }
    }
    for(int i = 0; i < 15; i++)
    {
        CCSprite* spZoushi2 = CCSprite::createWithSpriteFrameName(PailuType[m_nPailu2[i+5]]);
        spZoushi2 -> setPosition(ZoushiPoint2[i]);
        pZoushiItem -> addChild(spZoushi2);
        m_arrPailu2 -> addObject(spZoushi2);
        
    }
}

//重置数据
void H3CardBeteLayer::resetData()
{
    m_nBetArea = 0;//玩家下注区域
    m_labelBetBlack -> setString("");//黑方下注总数
    m_labelBetRed -> setString("");
    m_labelBetSpecial -> setString("");
    m_labelOwnBetBlack -> setString("");//自己下注黑方数量
    m_labelOwnBetRed -> setString("");
    m_labelOwnBetSpecial -> setString("");
    m_pXiazhuBlackDi -> setVisible(false);//自己下注底板
    m_pXiazhuRedDi -> setVisible(false);
    m_pXiazhuSpecialDi -> setVisible(false);
    m_llBlackBetCoin = 0;
    m_llRedBetCoin = 0;
    m_llSpecialBetCoin = 0;
    m_nBetTimes = BET_TIME;
    m_bSpecialCardType = false;
    m_nCardResult = 0;
    m_dwBigWinUserID = 0;
    m_llBigWinScore = 0;
    
    //删除现有牌
    for(int i = 0; i < m_arrCardLiang->count(); i++)
    {
        CardSprite* pCardSprite = (CardSprite*)m_arrCardLiang->objectAtIndex(i);
        if(pCardSprite != NULL)
        {
            pCardSprite -> removeFromParent();
        }
    }
    m_arrCardLiang -> removeAllObjects();
    CCSprite* pBlackType = (CCSprite*)m_pCardBeteBg->getChildByTag(eH3BlackTypeTag);
    if(pBlackType != NULL)
    {
        pBlackType -> removeFromParent();
    }
    CCSprite* pRedType = (CCSprite*)m_pCardBeteBg->getChildByTag(eH3RedTypeTag);
    if(pRedType != NULL)
    {
        pRedType -> removeFromParent();
    }
    
    //移除筹码
    for(int i = 0; i < m_arrBetBlackSprite->count(); i++)
    {
        BetSprite* pBetSprite = (BetSprite*)m_arrBetBlackSprite->objectAtIndex(i);
        if(pBetSprite != NULL)
        {
            pBetSprite->removeFromParent();
        }
    }
    m_arrBetBlackSprite->removeAllObjects();
    for(int i = 0; i < m_arrBetRedSprite->count(); i++)
    {
        BetSprite* pBetSprite = (BetSprite*)m_arrBetRedSprite->objectAtIndex(i);
        if(pBetSprite != NULL)
        {
            pBetSprite->removeFromParent();
        }
    }
    m_arrBetRedSprite->removeAllObjects();
    for(int i = 0; i < m_arrBetSpecialSprite->count(); i++)
    {
        BetSprite* pBetSprite = (BetSprite*)m_arrBetSpecialSprite->objectAtIndex(i);
        if(pBetSprite != NULL)
        {
            pBetSprite->removeFromParent();
        }
    }
    m_arrBetSpecialSprite->removeAllObjects();
    
}

void H3CardBeteLayer::showWinAnimate()
{
    if(m_bSpecialCardType)
    {
        m_pBetSpecialBg -> runAction(CCRepeat::create(CCSequence::create(CCFadeIn::create(0.4f), CCFadeOut::create(0.4f), NULL), 3));
    }
    if(m_nCardResult == 0)//黑方胜
    {
        m_pBetBlackBg -> runAction(CCRepeat::create(CCSequence::create(CCFadeIn::create(0.4f), CCFadeOut::create(0.4f), NULL),3));
        m_pCardYingBg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/4+10, 580));
        m_pCardYingBg -> runAction(CCSequence::create(CCShow::create(), CCFadeIn::create(0.8f), CCDelayTime::create(0.6f), CCFadeOut::create(0.8f), NULL));
    }
    else if(m_nCardResult == 1)//红方胜
    {
        m_pBetRedBg -> runAction(CCRepeat::create(CCSequence::create(CCFadeIn::create(0.4f), CCFadeOut::create(0.4f), NULL), 3));
        m_pCardYingBg -> setPosition(ccp(m_pCardBeteBg->getContentSize().width/4*3-10, 580));
        m_pCardYingBg -> runAction(CCSequence::create(CCShow::create(), CCFadeIn::create(0.8f), CCDelayTime::create(0.6f), CCFadeOut::create(0.8f), NULL));
    }
    
    this -> scheduleOnce(schedule_selector(H3CardBeteLayer::setBetReturn), 2.0f);
    this -> scheduleOnce(schedule_selector(H3CardBeteLayer::bigWinnerShow), 3.0f);
}

void H3CardBeteLayer::showZoushiAnimate()
{
    CCSprite* spdi1 = (CCSprite*)m_arrPailu1->objectAtIndex(0);
    if(spdi1 != NULL)
    {
        spdi1 -> removeFromParent();
        m_arrPailu1 -> removeObject(spdi1);
    }
    CCSprite* spdi2 = (CCSprite*)m_arrPailu2->objectAtIndex(0);
    if(spdi2 != NULL)
    {
        spdi2 -> removeFromParent();
        m_arrPailu2 -> removeObject(spdi2);
    }
    for(int i = 0; i < m_arrPailu1->count(); i++)
    {
        CCSprite* spdi = (CCSprite*)m_arrPailu1->objectAtIndex(i);
        if(spdi == NULL) continue;
        spdi -> runAction(CCMoveTo::create(1.0f, ZoushiPoint1[i]));
    }
    for(int i = 0; i < m_arrPailu2->count(); i++)
    {
        CCSprite* spdi = (CCSprite*)m_arrPailu2->objectAtIndex(i);
        if(spdi == NULL) continue;
        spdi -> runAction(CCMoveTo::create(1.0f, ZoushiPoint2[i]));
    }
    this -> scheduleOnce(schedule_selector(H3CardBeteLayer::startNewZoushiAnimate), 1.0f);
}

void H3CardBeteLayer::startNewZoushiAnimate()
{
    CCArray* array = m_pZoushiMenu->getChildren();
    if(array == NULL || array->count() == 0)
        return ;
    CCMenuItem* item = (CCMenuItem*)array->objectAtIndex(0);
    if(item == NULL)
        return ;
    CCSprite* spdi1 = CCSprite::createWithSpriteFrameName(PailuDiType[m_nPailu1[19]]);
    spdi1 -> setPosition(ccp(740, 80));
    item -> addChild(spdi1);
    spdi1 -> runAction(CCSequence::create(CCDelayTime::create(0.1f), CCMoveTo::create(0.5f, ZoushiPoint1[6]), NULL));
    m_arrPailu1 -> addObject(spdi1);
    if(m_nPailu3[19] >= 0 || m_nPailu3[19] <= 6)
    {
        CCLabelTTF* pLabelPailu = CCLabelTTF::create(PailuName[m_nPailu3[19]], "Arial", 20);
        pLabelPailu -> setPosition(ccp(spdi1->getContentSize().width/2, spdi1->getContentSize().height/2));
        spdi1 -> addChild(pLabelPailu);
        if(m_nPailu3[19] > 1)
        {
            pLabelPailu -> setColor(ccc3(125, 78, 13));
        }
        else
        {
            pLabelPailu -> setColor(ccc3(232, 206, 143));
        }
    }
    CCSprite* spdi2 = CCSprite::createWithSpriteFrameName(PailuType[m_nPailu2[19]]);
    spdi2 -> setPosition(ccp(740, 80));
    item -> addChild(spdi2);
    spdi2 -> runAction(CCSequence::create(CCDelayTime::create(0.1f), CCMoveTo::create(0.5f, ZoushiPoint2[14]), NULL));
    m_arrPailu2 -> addObject(spdi2);
    
    PailuLayer* pLayer = (PailuLayer*)this->getChildByTag(999);
    if(pLayer != NULL)
    {
        pLayer -> updateUI();
    }
    else
    {
        int nOpen = CCUserDefault::sharedUserDefault()->getIntegerForKey("FirstOpenH3Pailu", 1);
        if(nOpen == 2)
        {
            CCUserDefault::sharedUserDefault()->setIntegerForKey("FirstOpenH3Pailu", 1);
        }
        else
        {
            CCUserDefault::sharedUserDefault()->setIntegerForKey("FirstOpenH3Pailu", 2);
        }
    }
    
}

void H3CardBeteLayer::scheduleShowBet()
{
    m_nBetTimes--;
    if(m_nBetTimes <= 3)
    {
        this -> unschedule(schedule_selector(H3CardBeteLayer::scheduleShowBet));
        scheduleRemind();
    }
}

void H3CardBeteLayer::betIndexAction(bool bStart)
{
    //背景
    CCSprite* pBg = CCSprite::createWithSpriteFrameName("h3card_donghua_beijing.png");
    pBg->setPosition(ccp(CLIENT_WIDTH/2, 467));
    this->addChild(pBg);
    pBg->setOpacity(0);
    CCFadeIn* pBgFadeIn = CCFadeIn::create(0.3f);
    CCDelayTime* pBgDelay = CCDelayTime::create(0.9f);
    CCFadeOut* pBgFadeOut = CCFadeOut::create(0.18f);
    CCCallFuncN* pBgCallFuncN = CCCallFuncN::create(this, callfuncN_selector(H3CardBeteLayer::removeThis));
    CCSequence* pBgSeq = CCSequence::create(pBgFadeIn, pBgDelay, pBgFadeOut, pBgCallFuncN, NULL);
    pBg->runAction(pBgSeq);
    //开始下注或者下注结束
    CCSprite* pIndexSp = NULL;
    if (bStart)
    {
        pIndexSp = CCSprite::createWithSpriteFrameName("h3card_donghua_kaishi.png");
    }
    else
    {
        pIndexSp = CCSprite::createWithSpriteFrameName("h3card_donghua_jieshu.png");
    }
    pIndexSp->setScale(6.0f);
    pIndexSp->setPosition(ccp(pBg->getContentSize().width/2, pBg->getContentSize().height/2));
    pBg->addChild(pIndexSp);
    CCDelayTime* pIndexDelay0 = CCDelayTime::create(0.15f);
    CCScaleTo* pIndexScaleTo0 = CCScaleTo::create(0.13f, 1);
    CCScaleTo* pIndexScaleTo1 = CCScaleTo::create(0.05f, 1.2f);
    CCDelayTime* pIndexDelay1 = CCDelayTime::create(0.8f);
    CCFadeOut* pIndexFadeOut = CCFadeOut::create(0.18f);
    CCSequence* pIndexSeq = CCSequence::create(pIndexDelay0, pIndexScaleTo0, pIndexScaleTo1, pIndexDelay1, pIndexFadeOut, NULL);
    pIndexSp->runAction(pIndexSeq);
    //上下两道光
    CCSprite* pUpLight = CCSprite::create("h3card/light.png");
    pUpLight->setAnchorPoint(ccp(0.5f, 0));
    pUpLight->setPosition(ccp(-pBg->getContentSize().width*0.15f, pBg->getContentSize().height*1.05f));
    pUpLight->setScaleY(-1);
    pBg->addChild(pUpLight);
    pUpLight->setOpacity(0);
    CCDelayTime* pUpLightDelay = CCDelayTime::create(0.28f);
    CCFadeTo* pUpLightFadeTo0 = CCFadeTo::create(0.01f, 200);
    CCFadeTo* pUpLightFadeTo1 = CCFadeTo::create(0.01F, 255);
    CCMoveBy* pUpLightMoveBy0 = CCMoveBy::create(0.3f, ccp(pBg->getContentSize().width*0.9f, 0));
    CCMoveBy* pUpLightMoveBy1 = CCMoveBy::create(0.05f, ccp(pBg->getContentSize().width*0.1f, 0));
    CCScaleTo* pUpLightScaleTo = CCScaleTo::create(0.05f, 0);
    CCSpawn* pUpLightSpawn = CCSpawn::create(pUpLightMoveBy1, pUpLightScaleTo, NULL);
    CCSequence* pUpLightSeq = CCSequence::create(pUpLightDelay, pUpLightFadeTo0, pUpLightFadeTo1, pUpLightMoveBy0, pUpLightSpawn, NULL);
    pUpLight->runAction(pUpLightSeq);
    CCSprite* pDownLight = CCSprite::create("h3card/light.png");
    pDownLight->setAnchorPoint(ccp(0.5f, 0));
    pDownLight->setPosition(ccp(pBg->getContentSize().width*1.15f, -pBg->getContentSize().height*0.05f));
    pBg->addChild(pDownLight);
    pDownLight->setOpacity(0);
    CCDelayTime* pDownLightDelay = CCDelayTime::create(0.28f);
    CCFadeTo* pDownLightFadeTo0 = CCFadeTo::create(0.01f, 200);
    CCFadeTo* pDownLightFadeTo1 = CCFadeTo::create(0.01F, 255);
    CCMoveBy* pDownLightMoveBy0 = CCMoveBy::create(0.3f, ccp(-pBg->getContentSize().width*0.9f, 0));
    CCMoveBy* pDownLightMoveBy1 = CCMoveBy::create(0.05f, ccp(-pBg->getContentSize().width*0.1f, 0));
    CCScaleTo* pDownLightScaleTo = CCScaleTo::create(0.05f, 0);
    CCSpawn* pDownLightSpawn = CCSpawn::create(pDownLightMoveBy1, pDownLightScaleTo, NULL);
    CCSequence* pDownLightSeq = CCSequence::create(pDownLightDelay, pDownLightFadeTo0, pDownLightFadeTo1, pDownLightMoveBy0, pDownLightSpawn, NULL);
    pDownLight->runAction(pDownLightSeq);
}

void H3CardBeteLayer::betFinish()
{
    betIndexAction(false);
    //查看是否还存在闹钟，如果存在，则删除
    CCSprite* pRemindBg = (CCSprite*)this->getChildByTag(99);
    if(pRemindBg != NULL)
    {
        pRemindBg -> stopAllActions();
        pRemindBg -> removeFromParent();
    }
}

void H3CardBeteLayer::scheduleRemind()
{
    //背景
    CCSprite* pBg = CCSprite::createWithSpriteFrameName("h3card_donghua_beijing.png");
    pBg->setPosition(ccp(CLIENT_WIDTH/2, 467));
    pBg->setTag(99);
    this->addChild(pBg);
    CCDelayTime* pRemindDelay = CCDelayTime::create(3);
    //CCCallFunc* pBetCallFunc = CCCallFunc::create(this, callfunc_selector(H3CardBeteLayer::betFinish));
    CCCallFuncN* pBgCallFuncN = CCCallFuncN::create(this, callfuncN_selector(H3CardBeteLayer::removeThis));
    CCSequence* pBgSeq = CCSequence::create(pRemindDelay, pBgCallFuncN, NULL);
    pBg->runAction(pBgSeq);
    CCLabelTTF* pRemindTTF = CCLabelTTF::create("下注阶段即将结束", "", 36);
    pRemindTTF->setColor(ccc3(219, 211, 212));
    pRemindTTF->setPosition(ccp(pBg->getContentSize().width/2, pBg->getContentSize().height*0.8f));
    pBg->addChild(pRemindTTF);
    //闹铃
    CCSprite* pClockSp = CCSprite::createWithSpriteFrameName("h3card_donghua_colok01.png");
    pClockSp->setPosition(ccp(pBg->getContentSize().width/2, pBg->getContentSize().height*0.32f));
    pBg->addChild(pClockSp);
    CCDelayTime* pClockDelay = CCDelayTime::create(0.017f);
    CCScaleTo* pClockScaleTo0 = CCScaleTo::create(0.1f, 1.3f);
    CCScaleTo* pClockScaleTo1 = CCScaleTo::create(0.1f, 1.0f);
    CCRotateBy* pClockRotateBy0 = CCRotateBy::create(0.1f, -10);
    CCSpawn* pClockSpawn = CCSpawn::create(pClockScaleTo1, pClockRotateBy0, NULL);
    CCRotateBy* pClockRotateBy1 = CCRotateBy::create(0.17f, 20);
    CCRotateBy* pClockRotateBy2 = CCRotateBy::create(0.08f, -10);
    CCDelayTime* pClockDelay1 = CCDelayTime::create(0.543f);
    CCSequence* pClockSeq = CCSequence::create(pClockDelay, pClockScaleTo0, pClockSpawn, pClockRotateBy1, pClockRotateBy2, pClockDelay1, NULL);
    CCRepeat* pClockRepeat = CCRepeat::create(pClockSeq, 3);
    pClockSp->runAction(pClockRepeat);
    
    CCSprite* pClockMiddle = CCSprite::createWithSpriteFrameName("h3card_donghua_colok03.png");
    pClockMiddle->setAnchorPoint(ccp(0.5f, 0));
    pClockMiddle->setPosition(ccp(pClockSp->getContentSize().width*0.5f, pClockSp->getContentSize().height*0.91f));
    pClockSp->addChild(pClockMiddle, -1);
    CCRotateBy* pMiddleRotateBy0 = CCRotateBy::create(0.05f, 30);
    CCRotateBy* pMiddleRotateBy1 = CCRotateBy::create(0.1f, -60);
    CCSequence* pMiddleSeq = CCSequence::create(pMiddleRotateBy0, pMiddleRotateBy1, pMiddleRotateBy0, NULL);
    CCRepeatForever* pMiddleRepeat = CCRepeatForever::create(pMiddleSeq);
    pClockMiddle->runAction(pMiddleRepeat);
    
    CCSprite* pClockRight = CCSprite::createWithSpriteFrameName("h3card_donghua_colok02.png");
    pClockRight->setAnchorPoint(ccp(0.5f, 0));
    pClockRight->setPosition(ccp(pClockSp->getContentSize().width*0.87f, pClockSp->getContentSize().height*0.85f));
    pClockSp->addChild(pClockRight);
    CCRotateBy* pRightRotateBy0 = CCRotateBy::create(0.12f, -30);
    CCRotateBy* pRightRotateBy1 = CCRotateBy::create(0.24f, 60);
    CCSequence* pRightSeq = CCSequence::create(pRightRotateBy0, pRightRotateBy1, pRightRotateBy0, NULL);
    CCRepeatForever* pRightRepeat = CCRepeatForever::create(pRightSeq);
    pClockRight->runAction(pRightRepeat);
    
    CCSprite* pClockLeft = CCSprite::createWithSpriteFrameName("h3card_donghua_colok02.png");
    pClockLeft->setAnchorPoint(ccp(0.5f, 0));
    pClockLeft->setPosition(ccp(pClockSp->getContentSize().width*0.13f, pClockSp->getContentSize().height*0.85f));
    pClockSp->addChild(pClockLeft);
    pClockLeft->setScaleX(-1);
    CCRotateBy* pLeftRotateBy0 = CCRotateBy::create(0.12f, 30);
    CCRotateBy* pLeftRotateBy1 = CCRotateBy::create(0.24f, -60);
    CCSequence* pLeftSeq = CCSequence::create(pLeftRotateBy0, pLeftRotateBy1, pLeftRotateBy0, NULL);
    CCRepeatForever* pLeftRepeat = CCRepeatForever::create(pLeftSeq);
    pClockLeft->runAction(pLeftRepeat);
    //倒计时显示
    CCLabelBMFont* pRemindBMFont = CCLabelBMFont::create("3","h3card/h3card_remind_num.fnt");
    pRemindBMFont->setAnchorPoint(ccp(0.5f,0.5f));
    pRemindBMFont->setPosition(ccp(pClockSp->getContentSize().width*0.58f, pClockSp->getContentSize().height/2));
    pRemindBMFont->setScale(0.5f);
    pClockSp->addChild(pRemindBMFont);
    CCDelayTime* pAtlasDelay = CCDelayTime::create(1);
    CCCallFuncN* pCallFuncN = CCCallFuncN::create(this, callfuncN_selector(H3CardBeteLayer::remindCallFunc));
    CCSequence* pAtlasSeq = CCSequence::create(pAtlasDelay, pCallFuncN, pAtlasDelay, pCallFuncN, NULL);
    pRemindBMFont->runAction(pAtlasSeq);
}

void H3CardBeteLayer::remindCallFunc(cocos2d::CCNode *pNode)
{
    CCLabelBMFont* pRemindBMFont = (CCLabelBMFont*)pNode;
    int nRemind = atoi(pRemindBMFont->getString());
    nRemind--;
    char sRemind[2];
    sprintf(sRemind, "%d", nRemind);
    pRemindBMFont->setString(sRemind);
    if (nRemind == 1)
    {
        pRemindBMFont->runAction(CCMoveBy::create(0, ccp(-5, 0)));
    }
}

void H3CardBeteLayer::bigWinnerShow()
{
    tagUserInfo* pUserInfo = CMainLogic::sharedMainLogic()->SearchUserByUserID(m_dwBigWinUserID);
    if(pUserInfo == NULL) return;
    std::string sNickName = CCharCode::GB2312ToUTF8(pUserInfo->szNickName);
    int nPicIndex = pUserInfo->wFaceID;
//    int nVipLevel = 0;
    CCLayer* pTempLayer = CCLayer::create();
    pTempLayer->setPosition(CCPointZero);
    this->addChild(pTempLayer);
    CCDelayTime* pTempDelay = CCDelayTime::create(2.15f);
    CCScaleTo* pTempScaleTo = CCScaleTo::create(0.23f, 0);
    CCCallFuncN* pBgCallFuncN = CCCallFuncN::create(this, callfuncN_selector(H3CardBeteLayer::removeThis));
    CCSequence* pTempSeq = CCSequence::create(pTempDelay, pTempScaleTo, pBgCallFuncN, NULL);
    pTempLayer->runAction(pTempSeq);
    //发光背景
    CCSprite* pLightBg = CCSprite::create("h3card/nopack_big_huangquan1_.png");
    pLightBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.45f));
    pTempLayer->addChild(pLightBg);
    pLightBg->setScale(1.2f);
    pLightBg->setOpacity(0);
    CCDelayTime* pLightBgDelay = CCDelayTime::create(0.203f);
    CCFadeIn* pLightBgFadeIn = CCFadeIn::create(0.133f);
    CCSequence* pLightBgSeq = CCSequence::create(pLightBgDelay, pLightBgFadeIn, NULL);
    pLightBg->runAction(pLightBgSeq);
    CCSprite* pLight = CCSprite::create("h3card/nopack_big_hongguang_.png");
    pLight->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.45f));
    pTempLayer->addChild(pLight);
    pLight->setScale(0.6f);
    pLight->setOpacity(0);
    CCDelayTime* pLightDelay = CCDelayTime::create(0.203f);
    CCFadeIn* pLightFadeIn = CCFadeIn::create(0.133f);
    CCScaleTo* pLightScaleTo = CCScaleTo::create(0.133f, 1.2f);
    CCSpawn* pLightSpawn0 = CCSpawn::create(pLightFadeIn, pLightScaleTo, NULL);
    CCSequence* pLightSeq = CCSequence::create(pLightDelay, pLightSpawn0, NULL);
    pLight->runAction(pLightSeq);
    //左下金币
    CCSprite* pLeft1Coin = CCSprite::createWithSpriteFrameName("h3card_dayingjia02.png");
    pLeft1Coin->setPosition(ccp(CLIENT_WIDTH*0.43f, CLIENT_HEIGHT*0.46f));
    pTempLayer->addChild(pLeft1Coin);
    pLeft1Coin->setOpacity(0);
    pLeft1Coin->setScale(1.5f);
    CCDelayTime* pLeft1CoinDelay = CCDelayTime::create(0.42f);
    CCFadeIn* pLeft1CoinFadeIn = CCFadeIn::create(0);
    CCMoveBy* pLeft1CoinMoveBy1 = CCMoveBy::create(0.15f, ccp(-CLIENT_WIDTH*0.125f, -CLIENT_HEIGHT*0.1f));
    CCMoveBy* pLeft1CoinMoveBy2 = CCMoveBy::create(0.067f, ccp(-CLIENT_WIDTH*0.125f, -CLIENT_HEIGHT*0.1f));
    CCFadeOut* pLeft1CoinFadeOut = CCFadeOut::create(0.067f);
    CCSpawn* pLeft1CoinSpawn = CCSpawn::create(pLeft1CoinMoveBy2, pLeft1CoinFadeOut, NULL);
    CCSequence* pLeft1CoinSeq = CCSequence::create(pLeft1CoinDelay, pLeft1CoinFadeIn, pLeft1CoinMoveBy1, pLeft1CoinSpawn, NULL);
    pLeft1Coin->runAction(pLeft1CoinSeq);
    //左中金币
    CCSprite* pLeft2Coin = CCSprite::createWithSpriteFrameName("h3card_dayingjia01.png");
    pLeft2Coin->setPosition(ccp(CLIENT_WIDTH*0.35f, CLIENT_HEIGHT*0.595f));
    pTempLayer->addChild(pLeft2Coin);
    pLeft2Coin->setOpacity(0);
    pLeft2Coin->setScale(1.5f);
    CCDelayTime* pLeft2CoinDelay = CCDelayTime::create(0.487f);
    CCFadeIn* pLeft2CoinFadeIn = CCFadeIn::create(0);
    CCMoveBy* pLeft2CoinMoveBy1 = CCMoveBy::create(0.083f, ccp(-CLIENT_WIDTH*0.125f, CLIENT_HEIGHT*0.1f));
    CCMoveBy* pLeft2CoinMoveBy2 = CCMoveBy::create(0.067f, ccp(-CLIENT_WIDTH*0.125f, CLIENT_HEIGHT*0.2f));
    CCFadeOut* pLeft2CoinFadeOut = CCFadeOut::create(0.067f);
    CCSpawn* pLeft2CoinSpawn = CCSpawn::create(pLeft2CoinMoveBy2, pLeft2CoinFadeOut, NULL);
    CCSequence* pLeft2CoinSeq = CCSequence::create(pLeft2CoinDelay, pLeft2CoinFadeIn, pLeft2CoinMoveBy1, pLeft2CoinSpawn, NULL);
    pLeft2Coin->runAction(pLeft2CoinSeq);
    //左上金币
    CCSprite* pLeft3Coin = CCSprite::createWithSpriteFrameName("h3card_dayingjia05.png");
    pLeft3Coin->setPosition(ccp(CLIENT_WIDTH*0.435f, CLIENT_HEIGHT*0.564f));
    pTempLayer->addChild(pLeft3Coin);
    pLeft3Coin->setOpacity(0);
    pLeft3Coin->setScale(1.5f);
    CCDelayTime* pLeft3CoinDelay = CCDelayTime::create(0.537f);
    CCFadeIn* pLeft3CoinFadeIn = CCFadeIn::create(0);
    CCMoveBy* pLeft3CoinMoveBy1 = CCMoveBy::create(0.23f, ccp(-CLIENT_WIDTH*0.07f, CLIENT_HEIGHT*0.14f));
    CCMoveBy* pLeft3CoinMoveBy2 = CCMoveBy::create(0.1f, ccp(-CLIENT_WIDTH*0.07f, CLIENT_HEIGHT*0.14f));
    CCFadeOut* pLeft3CoinFadeOut = CCFadeOut::create(0.1f);
    CCSpawn* pLeft3CoinSpawn = CCSpawn::create(pLeft3CoinMoveBy2, pLeft3CoinFadeOut, NULL);
    CCSequence* pLeft3CoinSeq = CCSequence::create(pLeft3CoinDelay, pLeft3CoinFadeIn, pLeft3CoinMoveBy1, pLeft3CoinSpawn, NULL);
    pLeft3Coin->runAction(pLeft3CoinSeq);
    //右下金币
    CCSprite* pRight1Coin = CCSprite::createWithSpriteFrameName("h3card_dayingjia03.png");
    pRight1Coin->setPosition(ccp(CLIENT_WIDTH*0.555f, CLIENT_HEIGHT*0.46f));
    pTempLayer->addChild(pRight1Coin);
    pRight1Coin->setOpacity(0);
    pRight1Coin->setScale(1.5f);
    CCDelayTime* pRight1CoinDelay = CCDelayTime::create(0.42f);
    CCFadeIn* pRight1CoinFadeIn = CCFadeIn::create(0);
    CCMoveBy* pRight1CoinMoveBy1 = CCMoveBy::create(0.23f, ccp(CLIENT_WIDTH*0.24f, -CLIENT_HEIGHT*0.18f));
    CCFadeOut* pRight1CoinFadeOut = CCFadeOut::create(0.23f);
    CCSequence* pRight1CoinSeq = CCSequence::create(pRight1CoinDelay, pRight1CoinFadeIn, pRight1CoinMoveBy1, pRight1CoinFadeOut, NULL);
    pRight1Coin->runAction(pRight1CoinSeq);
    //右中金币
    CCSprite* pRight2Coin = CCSprite::createWithSpriteFrameName("h3card_dayingjia04.png");
    pRight2Coin->setPosition(ccp(CLIENT_WIDTH*0.562f, CLIENT_HEIGHT*0.5f));
    pTempLayer->addChild(pRight2Coin);
    pRight2Coin->setOpacity(0);
    pRight2Coin->setScale(1.5f);
    CCDelayTime* pRight2CoinDelay = CCDelayTime::create(0.52f);
    CCFadeTo* pRight2CoinFadeTo = CCFadeTo::create(0, 125);
    CCMoveBy* pRight2CoinMoveBy1 = CCMoveBy::create(0.25f, ccp(CLIENT_WIDTH*0.12f, CLIENT_HEIGHT*0.07f));
    CCMoveBy* pRight2CoinMoveBy2 = CCMoveBy::create(0.134f, ccp(CLIENT_WIDTH*0.09f, CLIENT_HEIGHT*0.045f));
    CCFadeOut* pRight2CoinFadeOut = CCFadeOut::create(0.134f);
    CCSpawn* pRight2CoinSpawn = CCSpawn::create(pRight2CoinMoveBy2, pRight2CoinFadeOut, NULL);
    CCSequence* pRight2CoinSeq = CCSequence::create(pRight2CoinDelay, pRight2CoinFadeTo, pRight2CoinMoveBy1, pRight2CoinSpawn, NULL);
    pRight2Coin->runAction(pRight2CoinSeq);
    //右上金币
    CCSprite* pRight3Coin = CCSprite::createWithSpriteFrameName("h3card_dayingjia06.png");
    pRight3Coin->setPosition(ccp(CLIENT_WIDTH*0.555f, CLIENT_HEIGHT*0.56f));
    pTempLayer->addChild(pRight3Coin);
    pRight3Coin->setOpacity(0);
    pRight3Coin->setScale(1.5f);
    CCDelayTime* pRight3CoinDelay = CCDelayTime::create(0.52f);
    CCFadeTo* pRight3CoinFadeTo = CCFadeTo::create(0, 125);
    CCMoveBy* pRight3CoinMoveBy1 = CCMoveBy::create(0.2f, ccp(CLIENT_WIDTH*0.06f, CLIENT_HEIGHT*0.07f));
    CCMoveBy* pRight3CoinMoveBy2 = CCMoveBy::create(0.167f, ccp(CLIENT_WIDTH*0.04f, CLIENT_HEIGHT*0.045f));
    CCFadeOut* pRight3CoinFadeOut = CCFadeOut::create(0.167f);
    CCSpawn* pRight3CoinSpawn = CCSpawn::create(pRight3CoinMoveBy2, pRight3CoinFadeOut, NULL);
    CCSequence* pRight3CoinSeq = CCSequence::create(pRight3CoinDelay, pRight3CoinFadeTo, pRight3CoinMoveBy1, pRight3CoinSpawn, NULL);
    pRight3Coin->runAction(pRight3CoinSeq);
    //左一星星
    for (int i = 0; i < 2; i++)
    {
        CCSprite* pLeft1Star = CCSprite::createWithSpriteFrameName("h3card_dayingjia_xingxing01.png");
        pLeft1Star->setPosition(ccp(CLIENT_WIDTH*0.465f, CLIENT_HEIGHT*0.5f));
        pTempLayer->addChild(pLeft1Star);
        pLeft1Star->setAnchorPoint(ccp(1, 0));
        pLeft1Star->setOpacity(0);
        pLeft1Star->setScale(0.5f);
        if (i == 1)
        {
            ccBlendFunc cbl1Left = {GL_SRC_ALPHA, GL_ONE };
            pLeft1Star->setBlendFunc(cbl1Left);
        }
        CCDelayTime* pLeft1StarDelay = CCDelayTime::create(0.353f);
        CCFadeIn* pLeft1StarFadeIn = CCFadeIn::create(0);
        CCScaleTo* pLeft1ScaleTo1 = CCScaleTo::create(0.117f, 1.6f);
        CCScaleTo* pLeft1ScaleTo2 = CCScaleTo::create(0.1f, 1.4f);
        CCDelayTime* pLeft1Delay2 = CCDelayTime::create(0.017f);
        CCFadeOut* pLeft1FadeOut = NULL;
        if (i == 1)
        {
            pLeft1FadeOut = CCFadeOut::create(0.25f);
        }
        CCSequence* pLeft1StarSeq = CCSequence::create(pLeft1StarDelay, pLeft1StarFadeIn, pLeft1ScaleTo1, pLeft1ScaleTo2, pLeft1Delay2, pLeft1FadeOut, NULL);
        pLeft1Star->runAction(pLeft1StarSeq);
    }
    //右一星星
    for (int i = 0; i < 2; i++)
    {
        CCSprite* pRight1Star = CCSprite::createWithSpriteFrameName("h3card_dayingjia_xingxing01.png");
        pRight1Star->setFlipX(true);
        pRight1Star->setAnchorPoint(ccp(0, 0));
        pRight1Star->setPosition(ccp(CLIENT_WIDTH*0.54f, CLIENT_HEIGHT*0.5f));
        pTempLayer->addChild(pRight1Star);
        pRight1Star->setOpacity(0);
        pRight1Star->setScale(0.6f);
        if (i == 1)
        {
            ccBlendFunc cbl1Right = {GL_SRC_ALPHA, GL_ONE };
            pRight1Star->setBlendFunc(cbl1Right);
        }
        CCDelayTime* pRight1StarDelay = CCDelayTime::create(0.353f);
        CCFadeIn* pRight1StarFadeIn = CCFadeIn::create(0);
        CCScaleTo* pRight1ScaleTo1 = CCScaleTo::create(0.083f, 1.6f);
        CCScaleTo* pRight1ScaleTo2 = CCScaleTo::create(0.067f, 1.4f);
        CCDelayTime* pRight1Delay2 = CCDelayTime::create(0.017f);
        CCFadeOut* pRight1FadeOut = NULL;
        if (i == 1)
        {
            pRight1FadeOut = CCFadeOut::create(0.25f);
        }
        CCSequence* pRight1StarSeq = CCSequence::create(pRight1StarDelay, pRight1StarFadeIn, pRight1ScaleTo1, pRight1ScaleTo2, pRight1Delay2, pRight1FadeOut, NULL);
        pRight1Star->runAction(pRight1StarSeq);
    }
    //右2星星
    CCSprite* pRight2Star = CCSprite::createWithSpriteFrameName("h3card_dayingjia_xingxing.png");
    pRight2Star->setFlipX(true);
    pRight2Star->setPosition(ccp(CLIENT_WIDTH*0.525f, CLIENT_HEIGHT*0.425f));
    pTempLayer->addChild(pRight2Star);
    ccBlendFunc cbl2pRight = {GL_SRC_ALPHA, GL_ONE };
    pRight2Star->setBlendFunc(cbl2pRight);
    pRight2Star->setAnchorPoint(ccp(0, 0));
    pRight2Star->setOpacity(0);
    pRight2Star->setScale(0.5f);
    CCDelayTime* pRight2StarDelay = CCDelayTime::create(0.42f);
    CCFadeIn* pRight2StarFadeIn = CCFadeIn::create(0);
    CCScaleTo* pRight2ScaleTo1 = CCScaleTo::create(0.083f, 1.6f);
    CCScaleTo* pRight2ScaleTo2 = CCScaleTo::create(0.067f, 1.2f);
    CCSequence* pRight2StarSeq = CCSequence::create(pRight2StarDelay, pRight2StarFadeIn, pRight2ScaleTo1, pRight2ScaleTo2, NULL);
    pRight2Star->runAction(pRight2StarSeq);
    //左2星星
    CCSprite* pLeft2Star = CCSprite::createWithSpriteFrameName("h3card_dayingjia_xingxing.png");
    pLeft2Star->setPosition(ccp(CLIENT_WIDTH*0.48f, CLIENT_HEIGHT*0.41f));
    pTempLayer->addChild(pLeft2Star);
    ccBlendFunc cbl2Left = {GL_SRC_ALPHA, GL_ONE };
    pLeft2Star->setBlendFunc(cbl2Left);
    pLeft2Star->setAnchorPoint(ccp(1, 0));
    pLeft2Star->setOpacity(0);
    pLeft2Star->setScale(0.5f);
    CCDelayTime* pLeft2StarDelay = CCDelayTime::create(0.42f);
    CCFadeIn* pLeft2StarFadeIn = CCFadeIn::create(0);
    CCScaleTo* pLeft2ScaleTo1 = CCScaleTo::create(0.083f, 1.6f);
    CCScaleTo* pLeft2ScaleTo2 = CCScaleTo::create(0.067f, 1.3f);
    CCFadeOut* pLeft2FadeOut = CCFadeOut::create(0.25f);
    CCSequence* pLeft2StarSeq = CCSequence::create(pLeft2StarDelay, pLeft2StarFadeIn, pLeft2ScaleTo1, pLeft2ScaleTo2, pLeft2FadeOut, NULL);
    pLeft2Star->runAction(pLeft2StarSeq);
    //左3星星
    CCSprite* pLeft3Star = CCSprite::createWithSpriteFrameName("h3card_dayingjia_xingxing.png");
    pLeft3Star->setPosition(ccp(CLIENT_WIDTH*0.48f, CLIENT_HEIGHT*0.41f));
    pTempLayer->addChild(pLeft3Star);
    ccBlendFunc cbl3Left = {GL_SRC_ALPHA, GL_ONE };
    pLeft3Star->setBlendFunc(cbl3Left);
    pLeft3Star->setAnchorPoint(ccp(1, 0));
    pLeft3Star->setOpacity(0);
    pLeft3Star->setScale(0.6f);
    CCDelayTime* pLeft3StarDelay = CCDelayTime::create(0.437f);
    CCFadeIn* pLeft3StarFadeIn = CCFadeIn::create(0);
    CCScaleTo* pLeft3ScaleTo1 = CCScaleTo::create(0.083f, 1.3f);
    CCSequence* pLeft3StarSeq = CCSequence::create(pLeft3StarDelay, pLeft3StarFadeIn, pLeft3ScaleTo1, NULL);
    pLeft3Star->runAction(pLeft3StarSeq);
    //头像背景
    CCSprite* pHeadBg = CCSprite::create("h3card/nopack_window_frame_1.png");
    pHeadBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.54f));
    pTempLayer->addChild(pHeadBg);
    if(nPicIndex>=1 && nPicIndex<=10)
    {
        CCSprite* pHead = CCSprite::create(txName[nPicIndex-1]);
        pHead -> setPosition(ccp(pHeadBg->getContentSize().width/2, pHeadBg->getContentSize().height/2));
        pHeadBg -> addChild(pHead);
    }
    //大赢家
    CCSprite* pBigWinner = CCSprite::createWithSpriteFrameName("h3card_dayingjia_biaoti.png");
    pBigWinner->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.55f));
    pTempLayer->addChild(pBigWinner);
    pBigWinner->setScale(1.5f);
    CCScaleTo* pBigWinnerScaleTo0 = CCScaleTo::create(0.1f, 5.2f);
    CCMoveBy* pBigWinnerMoveBy0 = CCMoveBy::create(0.1f, ccp(0, CLIENT_HEIGHT*0.15f));
    CCSpawn* pBigWinnerSpawn = CCSpawn::create(pBigWinnerScaleTo0, pBigWinnerMoveBy0, NULL);
    CCScaleTo* pBigWinnerScaleTo1 = CCScaleTo::create(0.12f, 0.8f);
    CCScaleTo* pBigWinnerScaleTo2 = CCScaleTo::create(0.02f, 1.0f);
    CCSequence* pBigWinnerSeq = CCSequence::create(pBigWinnerSpawn, pBigWinnerScaleTo1, pBigWinnerScaleTo2, NULL);
    pBigWinner->runAction(pBigWinnerSeq);
    //大赢家叠加
    CCSprite* pBigWinner2 = CCSprite::createWithSpriteFrameName("h3card_dayingjia_biaoti.png");
    pBigWinner2->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.7f));
    pTempLayer->addChild(pBigWinner2);
    pBigWinner2->setOpacity(0);
    pBigWinner2->setScale(5.2f);
    ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE };
    pBigWinner2->setBlendFunc(cbl);
    CCDelayTime* pWinner2Delay = CCDelayTime::create(0.1f);
    CCFadeIn* pWinner2FadeIn = CCFadeIn::create(0.02f);
    CCScaleTo* pWinner2ScaleTo0 = CCScaleTo::create(0.12f, 0.7f);
    CCScaleTo* pWinner2ScaleTo1 = CCScaleTo::create(0.1f, 1.0f);
    CCScaleTo* pWinner2ScaleTo2 = CCScaleTo::create(0.3f, 2.0f);
    CCFadeOut* pWinner2FadeOut = CCFadeOut::create(0.3f);
    CCSpawn* pWinner2Spawn2 = CCSpawn::create(pWinner2ScaleTo2, pWinner2FadeOut, NULL);
    CCSequence* pWinner2Seq = CCSequence::create(pWinner2Delay, pWinner2FadeIn, pWinner2ScaleTo0, pWinner2ScaleTo1, pWinner2Spawn2, NULL);
    pBigWinner2->runAction(pWinner2Seq);
    //黑底
    CCSprite* pHeiBg = CCSprite::createWithSpriteFrameName("h3card_dayingjia_hei.png");
    pHeiBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.45f));
    pTempLayer->addChild(pHeiBg);
    pHeiBg->setScale(1.5f);
    CCScaleTo* pHeiBgScaleTo0 = CCScaleTo::create(0.1f, 5.2f);
    CCMoveBy* pHeiBgMoveBy0 = CCMoveBy::create(0.1f, ccp(0, -CLIENT_HEIGHT*0.11f));
    CCSpawn* pHeiBgSpawn = CCSpawn::create(pHeiBgScaleTo0, pHeiBgMoveBy0, NULL);
    CCScaleTo* pHeiBgScaleTo1 = CCScaleTo::create(0.12f, 0.8f);
    CCScaleTo* pHeiBgScaleTo2 = CCScaleTo::create(0.05f, 1.0f);
    CCSequence* pHeiBgSeq = CCSequence::create(pHeiBgSpawn, pHeiBgScaleTo1, pHeiBgScaleTo2, NULL);
    pHeiBg->runAction(pHeiBgSeq);
    
    CCLabelBMFont* pScoreBMFont = CCLabelBMFont::create(getString2FromNumber(m_llBigWinScore).c_str(), "h3card/h3card_lotterypool_draw.fnt");
    pScoreBMFont->setPosition(ccp(pHeiBg->getContentSize().width/2, pHeiBg->getContentSize().height/2));
    pHeiBg->addChild(pScoreBMFont);
    pScoreBMFont->setScale(0.35f);
    //昵称背景
    CCSprite* pNickNameBg = CCSprite::createWithSpriteFrameName("h3card_dayingjia_caidi.png");
    pNickNameBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.42f));
    pTempLayer->addChild(pNickNameBg);
    CCLabelTTF* pNickTTF = CCLabelTTF::create(sNickName.c_str(), "", 36);
    pNickTTF->setPosition(ccp(pNickNameBg->getContentSize().width/2, pNickNameBg->getContentSize().height*0.6f));
    pNickNameBg->addChild(pNickTTF);
    pNickTTF->setOpacity(0);
    CCDelayTime* pTTFDelay = CCDelayTime::create(0.1f);
    CCFadeIn* pTTFFadeIn = CCFadeIn::create(0);
    pNickTTF->runAction(CCSequence::create(pTTFDelay, pTTFFadeIn, NULL));
    pNickNameBg->setOpacity(0);
    pNickNameBg->setScale(5.2f);
    CCDelayTime* pNickDelay = CCDelayTime::create(0.1f);
    CCFadeIn* pNickFadeIn = CCFadeIn::create(0);
    CCScaleTo* pNickScaleTo1 = CCScaleTo::create(0.12f, 0.8f);
    CCScaleTo* pNickScaleTo2 = CCScaleTo::create(0.02f, 1.0f);
    CCSequence* pNickSeq = CCSequence::create(pNickDelay, pNickFadeIn, pNickScaleTo1, pNickScaleTo2, NULL);
    pNickNameBg->runAction(pNickSeq);
}

void H3CardBeteLayer::removeThis(CCNode* pNode)
{
    pNode->removeFromParent();
}

void H3CardBeteLayer::vsAction()
{
    CCLayerColor* pTempLayer = CCLayerColor::create(ccc4(0, 0, 0, 155));
    pTempLayer->setPosition(CCPointZero);
    this->addChild(pTempLayer);
    CCDelayTime* pDelay = CCDelayTime::create(2.0f);//2.55
    //CCCallFunc* pStartCallFunc = CCCallFunc::create(this, callfunc_selector(H3CardBeteLayer::betStart));
    CCCallFuncN* pCallFuncN = CCCallFuncN::create(this, callfuncN_selector(H3CardBeteLayer::removeThis));
    CCSequence* pTempSeq = CCSequence::create(pDelay, pCallFuncN, NULL);
    pTempLayer->runAction(pTempSeq);
    //红方
    CCSprite* pRedSpBg = CCSprite::createWithSpriteFrameName("h3card_vs_VS05.png");
    pRedSpBg->setPosition(ccp(CLIENT_WIDTH+pRedSpBg->getContentSize().width/2, CLIENT_HEIGHT*0.55f));
    pTempLayer->addChild(pRedSpBg);
    CCMoveBy* pRedBgMoveBy0 = CCMoveBy::create(0.1f, ccp(-CLIENT_WIDTH*0.36f, 0));
    CCDelayTime* pRedBgDelay = CCDelayTime::create(0.017f);
    CCMoveBy* pRedBgMoveBy1 = CCMoveBy::create(0.017f, ccp(-CLIENT_WIDTH*0.1f, 0));
    CCDelayTime* pRedBgDelay2 = CCDelayTime::create(0.85f);
    CCMoveBy* pRedBgMoveBy2 = CCMoveBy::create(0.2f, ccp(-CLIENT_WIDTH*0.54f-pRedSpBg->getContentSize().width, 0));
    CCSequence* pRedBgSeq = CCSequence::create(pRedBgMoveBy0, pRedBgDelay, pRedBgMoveBy1, pRedBgDelay2, pRedBgMoveBy2, NULL);
    pRedSpBg->runAction(pRedBgSeq);
    CCSprite* pRedSp = CCSprite::createWithSpriteFrameName("h3card_vs_VS03.png");
    pRedSp->setPosition(ccp(CLIENT_WIDTH+pRedSpBg->getContentSize().width/2, CLIENT_HEIGHT*0.63f));
    pTempLayer->addChild(pRedSp);
    CCMoveBy* pRedMoveBy0 = CCMoveBy::create(0.1f, ccp(-CLIENT_WIDTH*0.46f, 0));
    CCDelayTime* pRedDelay = CCDelayTime::create(0.017f);
    CCMoveBy* pRedMoveBy = CCMoveBy::create(0.83f, ccp(-pRedSpBg->getContentSize().width*0.05f, 0));
    CCMoveBy* pRedMoveBy2 = CCMoveBy::create(0.017f, ccp(-pRedSpBg->getContentSize().width*0.01f, pRedSpBg->getContentSize().height*0.05f));
    CCMoveBy* pRedMoveBy3 = CCMoveBy::create(0.2f, ccp(-CLIENT_WIDTH*0.5f, CLIENT_HEIGHT*0.3f));
    CCFadeOut* pRedFadeOut = CCFadeOut::create(0.2f);
    CCSpawn* pRedSpwan = CCSpawn::create(pRedFadeOut, pRedMoveBy3, NULL);
    pRedSp->runAction(CCSequence::create(pRedMoveBy0, pRedDelay, pRedMoveBy, pRedMoveBy2, pRedSpwan, NULL));
    //VS
    CCSprite* pVS = CCSprite::createWithSpriteFrameName("h3card_vs_VS04.png");
    pVS->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.6f));
    pTempLayer->addChild(pVS);
    pVS->setOpacity(200);
    pVS->setScale(6.0f);
    CCDelayTime* pVSDelay = CCDelayTime::create(0.07f);
    CCScaleTo* pVSScaleTo0 = CCScaleTo::create(0.035f, 4.1f);
    CCDelayTime* pVSDelay1 = CCDelayTime::create(0.017f);
    CCScaleTo* pVSScaleTo2 = CCScaleTo::create(0.017f, 2.0f);
    CCFadeTo* pVSFadeTo = CCFadeTo::create(0.017f, 255);
    CCSpawn* pVSSpawn = CCSpawn::create(pVSScaleTo2, pVSFadeTo, NULL);
    CCScaleTo* pVSScaleTo3 = CCScaleTo::create(0.017f, 1.0f);
    CCMoveBy* pVSMoveBy0 = CCMoveBy::create(0.017f, ccp(-CLIENT_WIDTH*0.008f, CLIENT_HEIGHT*0.04f));
    CCSpawn* pVSSpawn2 = CCSpawn::create(pVSScaleTo3, pVSMoveBy0, NULL);
    CCScaleTo* pVSScaleTo4 = CCScaleTo::create(0.017f, 0.6f);
    CCScaleTo* pVSScaleTo5 = CCScaleTo::create(0.09f, 1.15f);
    CCDelayTime* pVSDelay2 = CCDelayTime::create(0.717f);
    CCScaleTo* pVSScaleTo6 = CCScaleTo::create(0.017f, 1.18f);
    CCScaleTo* pVSScaleTo7 = CCScaleTo::create(0.017f, 1.5f);
    CCFadeTo* pVSFadeTo2 = CCFadeTo::create(0.017f, 200);
    CCSpawn* pVSSpawn3 = CCSpawn::create(pVSScaleTo7, pVSFadeTo2, NULL);
    CCScaleTo* pVSScaleTo8 = CCScaleTo::create(0.183f, 6.0f);
    CCFadeOut* pVSFadeOut = CCFadeOut::create(0.183f);
    CCSpawn* pVSSpawn4 = CCSpawn::create(pVSScaleTo8, pVSFadeOut, NULL);
    CCSequence* pVSSeq = CCSequence::create(pVSDelay, pVSScaleTo0, pVSDelay1, pVSSpawn, pVSSpawn2, pVSScaleTo4, pVSScaleTo5, pVSDelay2, pVSScaleTo6, pVSSpawn3, pVSSpawn4, NULL);
    pVS->runAction(pVSSeq);
    //vs亮光叠加
    CCSprite* pVSLight = CCSprite::createWithSpriteFrameName("h3card_vs_huohuang.png");
    pVSLight->setPosition(ccp(pVS->getContentSize().width/2, pVS->getContentSize().height/2));
    pVS->addChild(pVSLight);
    ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE };
    pVSLight->setBlendFunc(cbl);
    CCDelayTime* pVSLightDelay = CCDelayTime::create(0.156f);
    CCFadeOut* pVSLightFadeOut = CCFadeOut::create(0);
    pVSLight->runAction(CCSequence::create(pVSLightDelay, pVSLightFadeOut, NULL));
    //vs2叠加
    CCSprite* pVS2 = CCSprite::createWithSpriteFrameName("h3card_vs_VS04.png");
    pVS2->setPosition(ccp(CLIENT_WIDTH*0.492f, CLIENT_HEIGHT*0.64f));
    pTempLayer->addChild(pVS2);
    pVS2->setScale(0.6f);
    pVS2->setOpacity(0);
    ccBlendFunc cbl1 = {GL_SRC_ALPHA, GL_ONE };
    pVS2->setBlendFunc(cbl1);
    CCDelayTime* pVS2Delay = CCDelayTime::create(0.156f);
    CCFadeIn* pVS2FadeIn = CCFadeIn::create(0);
    CCScaleTo* pVS2ScaleTo1 = CCScaleTo::create(0.09f, 1.15f);
    CCScaleTo* pVS2ScaleTo2 = CCScaleTo::create(0.37f, 2.2f);
    CCFadeOut* pVS2FadeOut = CCFadeOut::create(0.37f);
    CCSpawn* pVS2Spawn = CCSpawn::create(pVS2ScaleTo2, pVS2FadeOut, NULL);
    CCSequence* pVS2Seq = CCSequence::create(pVS2Delay, pVS2FadeIn, pVS2ScaleTo1, pVS2Spawn, NULL);
    pVS2->runAction(pVS2Seq);
    
    //黑方
    CCSprite* pBlackSpBg = CCSprite::createWithSpriteFrameName("h3card_vs_VS01.png");
    pBlackSpBg->setPosition(ccp(-pBlackSpBg->getContentSize().width/2, CLIENT_HEIGHT*0.45f));
    pTempLayer->addChild(pBlackSpBg);
    CCMoveBy* pBlackBgMoveBy0 = CCMoveBy::create(0.1f, ccp(CLIENT_WIDTH*0.35f, 0));
    CCDelayTime* pBlackBgDelay = CCDelayTime::create(0.017f);
    CCMoveBy* pBlackBgMoveBy1 = CCMoveBy::create(0.017f, ccp(CLIENT_WIDTH*0.1f, 0));
    CCDelayTime* pBlackBgDelay2 = CCDelayTime::create(0.85f);
    CCMoveBy* pBlackBgMoveBy2 = CCMoveBy::create(0.2f, ccp(CLIENT_WIDTH*0.54f+pRedSpBg->getContentSize().width, 0));
    CCSequence* pBlackBgSeq = CCSequence::create(pBlackBgMoveBy0, pBlackBgDelay, pBlackBgMoveBy1, pBlackBgDelay2, pBlackBgMoveBy2, NULL);
    pBlackSpBg->runAction(pBlackBgSeq);
    CCSprite* pBlackSp = CCSprite::createWithSpriteFrameName("h3card_vs_VS02.png");
    pBlackSp->setPosition(ccp(-pBlackSpBg->getContentSize().width/2, CLIENT_HEIGHT*0.535f));
    pTempLayer->addChild(pBlackSp);
    CCMoveBy* pBlackMoveBy0 = CCMoveBy::create(0.1f, ccp(CLIENT_WIDTH*0.46f, 0));
    CCDelayTime* pBlackDelay = CCDelayTime::create(0.017f);
    CCMoveBy* pBlackMoveBy = CCMoveBy::create(0.83f, ccp(pBlackSpBg->getContentSize().width*0.05f, 0));
    CCMoveBy* pBlackMoveBy2 = CCMoveBy::create(0.017f, ccp(pBlackSpBg->getContentSize().width*0.01f, pRedSpBg->getContentSize().height*0.05f));
    CCMoveBy* pBlackMoveBy3 = CCMoveBy::create(0.2f, ccp(CLIENT_WIDTH*0.5f, CLIENT_HEIGHT*0.3f));
    CCFadeOut* pBlackFadeOut = CCFadeOut::create(0.2f);
    CCSpawn* pBlackSpwan = CCSpawn::create(pBlackFadeOut, pBlackMoveBy3, NULL);
    pBlackSp->runAction(CCSequence::create(pBlackMoveBy0, pBlackDelay, pBlackMoveBy, pBlackMoveBy2, pBlackSpwan, NULL));
    //光圈
    CCSprite* pCircleSp = CCSprite::createWithSpriteFrameName("h3card_vs_guangquan.png");
    pCircleSp->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.65f));
    pTempLayer->addChild(pCircleSp);
    pCircleSp->setScale(0.6f);
    pCircleSp->setOpacity(0);
    ccBlendFunc cbl2 = {GL_SRC_ALPHA, GL_ONE };
    pCircleSp->setBlendFunc(cbl2);
    CCDelayTime* pCircleDelay = CCDelayTime::create(0.15f);
    CCFadeIn* pCircleFadeIn = CCFadeIn::create(0);
    CCScaleTo* pCircleScaleTo1 = CCScaleTo::create(0.1f, 1.2f);
    CCScaleTo* pCircleScaleTo2 = CCScaleTo::create(0.25f, 2.2f);
    CCFadeOut* pCircleFadeOut1 = CCFadeOut::create(0.25f);
    CCSpawn* pCircleSpawn = CCSpawn::create(pCircleScaleTo2, pCircleFadeOut1, NULL);
    CCSequence* pCircleSeq = CCSequence::create(pCircleDelay, pCircleFadeIn, pCircleScaleTo1, pCircleSpawn, NULL);
    pCircleSp->runAction(pCircleSeq);
    CCSprite* pCircleSp2 = CCSprite::createWithSpriteFrameName("h3card_vs_guangquan.png");
    pCircleSp2->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.65f));
    pTempLayer->addChild(pCircleSp2);
    pCircleSp2->setScale(0.6f);
    pCircleSp2->setOpacity(0);
    ccBlendFunc cbl3 = {GL_SRC_ALPHA, GL_ONE };
    pCircleSp2->setBlendFunc(cbl3);
    CCDelayTime* pCircle2Delay = CCDelayTime::create(0.2f);
    CCFadeIn* pCircle2FadeIn = CCFadeIn::create(0);
    CCScaleTo* pCircle2ScaleTo1 = CCScaleTo::create(0.15f, 1.2f);
    CCScaleTo* pCircle2ScaleTo2 = CCScaleTo::create(0.25f, 2.2f);
    CCFadeOut* pCircle2FadeOut1 = CCFadeOut::create(0.25f);
    CCSpawn* pCircle2Spawn = CCSpawn::create(pCircle2ScaleTo2, pCircle2FadeOut1, NULL);
    CCSequence* pCircle2Seq = CCSequence::create(pCircle2Delay, pCircle2FadeIn, pCircle2ScaleTo1, pCircle2Spawn, NULL);
    pCircleSp2->runAction(pCircle2Seq);
}

void H3CardBeteLayer::betStart()
{
    betIndexAction(true);//开始下注动画
    startSendBackCard();//开始发牌动画
}
