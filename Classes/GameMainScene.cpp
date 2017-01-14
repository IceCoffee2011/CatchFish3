#include "header.h"

#if defined(IOS_ANDROID)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
extern int WeiXinCharge;
#endif

#endif


GameMainScene* GameMainScene::_instance = NULL;
std::string GameMainScene::m_sCmdLine;

bool getCannonByType1(int m, int n)
{
    return (m & (1 << (n-1)));
}

bool GameMainScene::m_bWndHave = false;
bool GameMainScene::m_bHaveWndCalc = false;

const int JCBYFishID[38] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,30,31,32,33,34,35,36,37,40};
const int DYDFishID[37] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36};
const int LKPYFishID[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
const char* ParticleName[6] = {"p1", "p11", "p7", "p24", "lan2", "yuanhao"};
const char* FishChatData[48] = {
    "福到啦，送福啦",
    "开发商日子不过啦",
    "烫的脚疼",
    "爸比救我！",
    "许愿别找我",
    "我也是一名水友",
    "我还赶着去颁奖呢",
    "马失前蹄啊",
    "轻点！疼~",
    "呼~好险",
    "我快坚持不住啦！",
    "我只是路过打个酱油",
    "击中友军！",
    "别开炮，自己人！",
    "让炮火来的更猛烈些吧！",
    "厉害了",
    "我快不行了",
    "轻点，大兄弟！",
    "干爹救我！",
    "我是来找爸比的",
    "今天不宜出门",
    "我需要医生！",
    "只想被帅的人打中",
    "我可不是随便的鱼",
    "我也是一名水友",
    "只想被清蒸",
    "只想被红烧",
    "打鱼别打脸",
    "打脸伤自尊呐",
    "话费赛是免报名费的哦",
    "集五福可合成京东卡",
    "每晚有大量10元话费赛哦",
    "坚持参赛每周赢实物大奖",
    "关注微信公众号有好礼",
    "绑定手机后账号不怕丢",
    "好羡慕周赛榜上的大神们",
    "5分钟就能赢一元话费",
    "红包手快有，手慢无哦",
    "想做一条安静的美人鱼",
    "高倍房有比赛积分加成哦",
    "抓金龙，交好运",
    "合理使用技能才能打高分哦",
    "升级VIP，捕鱼更容易~",
    "鱼香肉丝为什么没有鱼",
    "偶尔出来卖个萌",
    "出来刷一波存在感",
    "饭后出来溜个弯",
    "今天水下交通有点堵",
};

GameMainScene* GameMainScene::GetInstance()
{
    if(_instance == NULL)
    {
        _instance=new GameMainScene();
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

GameMainScene::GameMainScene()
{
	memset(m_LockLine,0,sizeof(m_LockLine));
	memset(m_bLockFishID, 0,sizeof(m_bLockFishID));
	memset(m_bHaveUser, 0,sizeof(m_bHaveUser));
	memset(m_bAndroid,0, sizeof(m_bAndroid));
	memset(m_bUserSuperPao,0, sizeof(m_bUserSuperPao));
	memset(m_fRotation, 0,sizeof(m_fRotation));
	memset(m_nOwnTotalMoney, 0,sizeof(m_nOwnTotalMoney));
	memset(m_nRealChairID, 0,sizeof(m_nRealChairID));
	memset(m_nUserScore, 0,sizeof(m_nUserScore));
	memset(m_nBeiLv,0, sizeof(m_nBeiLv));
	memset(m_nFishDied, 0,sizeof(m_nFishDied));
	memset(m_nFishType,0, sizeof(m_nFishType));
    KIND_ID = CMainLogic::sharedMainLogic()->KIND_ID;
	for(int i = 0; i < GAME_PLAYER; i++)
	{
		m_nOwnTotalMoney[i] = 0;
		m_bLockFishID[i] = false;
		m_nLockFishID[i] = 0;
		m_bAndroid[i] = true;
		m_fRotation[i] = 0;
		m_nEveryBullet[i] = 0;
        m_bTryBarrelStatus[i] = false;
        m_bBaojiStatus[i] = false;
        m_bSameDie[i] = false;
        m_bPPLChairID[i] = false;
        m_bPPLReceiveCancel[i] = false;
        for(int j = 0; j < 3; j++)
        {
            m_nShijinbiFishID[i][j] = 0;
        }
	}
	for(int i = 0; i < 40; i++)
	{
		m_nFishDied[i] = 0;
	}
    m_nHistoryLockFishID = 0;
	m_arrMoney = CCArray::create();
	m_arrMoney -> retain();
	m_arrBullet = CCArray::create();
	m_arrBullet -> retain();
    m_arrLockFish = CCArray::create();
    m_arrLockFish -> retain();
	m_arrMenu = CCArray::create();
	m_arrMenu -> retain();
    m_arrQipao = CCArray::create();
    m_arrQipao -> retain();
	m_Obb1 = new OBB;
	m_Obb2 = new OBB;
    m_nCurrentCannonIndex = 0;
	m_nMeRealChairID = 2;
	m_nReCount = 9;
    m_nMatchID = 0;
    m_nRemindNum = 0;
    m_nShowMenuTime = 0;
	m_nPreSceneIndex = 0;
	m_nSceneIndex = 0;
	m_nSuoDingFishIndex = 0;
	m_nBgMusicIndex = rand()%3;
    m_nOutTime = 60;
	m_dwBulletID = 0;
	m_nFreezeTime = 0;
	m_nCurrentBulletCount = 0;
    m_nCurrentCannonType = 0;
    m_nTryLastScore = 0;
    m_wMobileMoney = 0;
    m_nChangeSceneIndex = 0;
    m_nSelectDantouType = -1;
    m_nTimesOnline = 10;
    m_nDantouTotalNum = 0;
    m_nOnlineTime = 0;
    m_lUserScore = CMainLogic::sharedMainLogic()->m_lUserScore;
	m_bMenuOpened = false;
    m_nLastSpeedUp = CMainLogic::sharedMainLogic()->m_nSpeedUpTimes;
    m_nLastBaoji = CMainLogic::sharedMainLogic()->m_nBaojiTimes;
    m_nLastSanshe = CMainLogic::sharedMainLogic()->m_nSansheTimes;
    m_bReceiveScene = false;
	m_bFishingOffSeason = false;
    m_bPaipaileStatus = false;
    
    m_bAutoOneCharge = false;
    
    m_bPPLZhuanComplete = false;
    m_bIsPaipaileStatus = false;
	m_bUserLock = false;
	m_bFreeze = false;
    m_bOnline = false;
	m_bAutoFire = false;
	m_bCanShoot = true;
	m_bWaiting = true;
    m_bClickAutoFire = false;
    m_bFirstMobileMoney = false;
    m_bClickDantouMenu = false;
	m_bEffect = true;
    m_bSelectDantouNum = true;
    
    m_bShadow = CCUserDefault::sharedUserDefault()->getBoolForKey("shadow", true);
    m_bParticle = CCUserDefault::sharedUserDefault()->getBoolForKey("particle", true);
	m_bSpeedUp = false;
    m_bClickSpeedUp = false;
	m_bHaveWndCalc = false;
	m_bWndHave = false;
	m_bTouch = false;
    m_bInitCompleted = false;
    m_bHaveWnd = false;
    m_bEnterGameScene = false;
    m_bIsSansheStatus = false;
    m_bIsBaojiStatus = false;
    m_bClickSanshe = false;
    m_bClickBaoji = false;
    m_bCanRequestRank = true;
    m_bClickExitMenu = false;
    m_bShowExitGame = false;
    m_bClickShared = false;
    m_bClickMenu = true;
    m_bDantouUsing = false;
    m_bHaveNoScore = false;
    
    m_bFirstEnter = true;//玩家刚进来
    m_bXinShouRoom = true;
    m_sShareURL = "";
    m_sShareContent = "";
    
    m_sMatchDate = "";
    m_sMatchTitle = "";
    if(KIND_ID == FB_KIND_ID)
    {
        m_bHaveTryCannon = true;
    }
    else
    {
        m_bHaveTryCannon = false;
    }
    m_vctBoxPrize.clear();
	m_vctFishStorg.clear();
	m_fishPathIndex.clear();
	m_vctGameFish.clear();
	m_vctFishFactory.clear();
	m_vctSound.clear();
	m_labelBossBeilv.clear();
    
	m_pMagicDevice=NULL;
    
    for(int i = 0; i < CMainLogic::sharedMainLogic()->m_vctNotice.size(); i++)
    {
        delete CMainLogic::sharedMainLogic()->m_vctNotice[i];
    }
    CMainLogic::sharedMainLogic()->m_vctNotice.clear();
    
    m_nFishCount = CMainLogic::sharedMainLogic()->FishCount;
    

	setFishType();
    
    m_nodeScore = CCNode::create();
    this->addChild(m_nodeScore);

	CClientKernel * pClientKernel = new CClientKernel();
    pClientKernel->m_pIClientKernelSink = this;
	m_pClientKernel=pClientKernel;
}

GameMainScene::~GameMainScene()
{
    m_vctBoxPrize.clear();
	CCSpriteBatchNode* batchNode1 = SpriteBatchManager::sharedSpriteBatchManager()->spriteBatchForKey("CatchFish01/fish9.png");
	for(unsigned int i = 0; i < m_arrMoney->count(); i++)
	{
		GoldSprite* sprite = (GoldSprite*)m_arrMoney->objectAtIndex(i);
		if (batchNode1 != NULL)
		{
			batchNode1->removeChild(sprite, true);
		}
	}
	m_arrMoney -> release();
    m_arrLockFish -> release();
    m_arrQipao -> release();
    CCSpriteBatchNode* batchNode2 = SpriteBatchManager::sharedSpriteBatchManager()->spriteBatchForKey("CatchFish01/fish4.png");
    for(unsigned int i = 0; i < m_arrBullet->count();i++)
	{
		BulletSprite* bulletSprite = (BulletSprite*)m_arrBullet -> objectAtIndex(i);
		if (batchNode2 != NULL)
		{
			batchNode2->removeChild(bulletSprite, true);
		}
	}
	m_arrBullet -> release();
	m_arrMenu -> release();
	for (unsigned int i = 0; i < m_vctGameFish.size(); i++)
	{
		if(m_vctGameFish[i]->getGroupFish())
		{
			for(unsigned int j = 0; j < m_vctGameFish[i]->getGameFishData().size(); j++)
			{
				delete m_vctGameFish[i]->getGameFishData()[j];
			}
			m_vctGameFish[i]->getGameFishData().clear();
		}
		delete m_vctGameFish[i];
	}
	m_vctGameFish.clear();
    for (unsigned int i = 0; i < m_vctFishFactory.size(); i++)
    {
        GameFish* pGameFish = m_vctFishFactory[i];
        if(pGameFish != NULL)
        {
            if(pGameFish->getGroupFish())
            {
                if(pGameFish->m_pFish != NULL)
                {
                    CCArray* fishArray = pGameFish->m_pFish->getChildren();
                    for(int j = 0; j < fishArray->count(); j++)
                    {
                        CCSprite* sprite1 = (CCSprite*)fishArray->objectAtIndex(j);
                        if(sprite1 != NULL)
                        {
                            sprite1->stopAllActions();
                            sprite1->unscheduleAllSelectors();
                        }
                    }
                }
            }
            else
            {
                if(pGameFish->m_pFish != NULL)
                {
                    pGameFish->m_pFish->stopAllActions();
                    pGameFish->m_pFish->unscheduleAllSelectors();
                }
                if(pGameFish->m_pFishShadow != NULL)
                {
                    pGameFish->m_pFishShadow->stopAllActions();
                    pGameFish->m_pFishShadow->unscheduleAllSelectors();
                }
            }
        }
    }
	for (unsigned int i = 0; i < m_vctFishFactory.size(); i++)
	{
		delete m_vctFishFactory[i];
	}
	m_vctFishFactory.clear();
	for(unsigned int i = 0; i < m_fishPathIndex.size(); i++)
	{
		delete m_fishPathIndex[i];
	}
	m_fishPathIndex.clear();
	for(unsigned int i = 0; i < m_vctFishStorg.size(); i++)
	{
		delete m_vctFishStorg[i];
	}
	m_vctFishStorg.clear();
	for(unsigned int i = 0; i < m_vctSound.size(); i++)
	{
		delete m_vctSound[i];
	}
	m_vctSound.clear();
	for(unsigned int i = 0; i < m_vctParticle.size(); i++)
	{
		delete m_vctParticle[i];
	}
	m_vctParticle.clear();
	m_labelBossBeilv.clear();
	
	delete m_pClientKernel;
	m_pClientKernel = NULL;
//	delete m_pMagicDevice;
// 	MP_Manager::GetInstance().Destroy();
    SpriteBatchManager::sharedSpriteBatchManager()->release();
	delete m_Obb1;
	m_Obb1 = NULL;
	delete m_Obb2;
	m_Obb2 = NULL;	
	_instance=NULL;
}

CCScene* GameMainScene::scene()
{
	CCScene *scene = CCScene::create();
    scene -> setTag(eGameMainSceneTag);
	if (_instance == NULL)
	{
		_instance=new GameMainScene();
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

bool GameMainScene::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
	srand((unsigned)time(NULL));
	m_sizeVisible = CCDirector::sharedDirector()->getVisibleSize();
	m_fishLayer = CCLayer::create();
	this -> addChild(m_fishLayer,1);
	addSpriteBatch();
    std::string dirpath = CMainLogic::sharedMainLogic()->DIRPATH3;
    if(CMainLogic::sharedMainLogic()->KIND_ID == YQS_KIND_ID)
    {
        dirpath = dirpath+"GameFishLK.xml";
    }
    else if(CMainLogic::sharedMainLogic()->KIND_ID == NZNH_KIND_ID)
    {
        dirpath = dirpath+"GameFishNZNH.xml";
    }
    else
    {
        dirpath = dirpath+"GameFish.xml";
    }
    
    readFishXML(dirpath.c_str());
	readFishPathXML("CatchFish01/PathIndex.xml");

	initBaseThings();
	loadMagicPtc();
    
	return true;
}

//救济金币框
void GameMainScene::loadUIGiveBg()
{
    //发送是否可以领取救济金的消息
    m_pClientKernel->SendSocketData(MDM_GR_USER, SUB_GRR_QUERYBANKCUPTINFO);
}

//初始化控件
void GameMainScene::initBaseThings()
{
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), 700,70);
    this -> addChild(pLayerColor, 100);
    pLayerColor -> setAnchorPoint(ccp(0.5f, 0.5f));
    pLayerColor -> ignoreAnchorPointForPosition(false);
    pLayerColor -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    m_luckLabel = CCLabelTTF::create("", "Arial-BoldMT", 60);
    m_luckLabel -> setColor(ccc3(255, 0, 0));
    m_luckLabel -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_luckLabel, 100);
    m_luckLabel -> setVisible(false);
    pLayerColor -> setVisible(false);
    
    CCLayerColor* pLayerColor2 = CCLayerColor::create(ccc4(0, 0, 0, 190), 700,70);
    this -> addChild(pLayerColor2, 100);
    pLayerColor2 -> setAnchorPoint(ccp(0.5f, 0.5f));
    pLayerColor2 -> ignoreAnchorPointForPosition(false);
    pLayerColor2 -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-100));
    m_luckLabel2 = CCLabelTTF::create("", "Arial-BoldMT", 60);
    m_luckLabel2 -> setColor(ccc3(255, 0, 0));
    m_luckLabel2 -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-100));
    this -> addChild(m_luckLabel2, 100);
    m_luckLabel2 -> setVisible(false);
    pLayerColor2 -> setVisible(false);
    
    //聊天输入框
    m_spChatBg = CCSprite::create("CatchFish2006/chatbg.png");
    m_spChatBg -> setPosition(ccp(CLIENT_WIDTH/2, 180));
    this -> addChild(m_spChatBg, 40);
    m_spChatBg -> setVisible(false);
    CCMenuItemImage* chatItem = CCMenuItemImage::create("CatchFish2006/chatsenditem.png", "CatchFish2006/chatsenditem.png", this, menu_selector(GameMainScene::itemCallBack));
    chatItem -> setTag(331);
    chatItem -> setPosition(ccp(m_spChatBg->getContentSize().width-chatItem->getContentSize().width/2-7, m_spChatBg->getContentSize().height/2));
    CCMenu* sendChatMenu = CCMenu::create(chatItem, NULL);
    sendChatMenu -> setPosition(CCPointZero);
    sendChatMenu -> setAnchorPoint(CCPointZero);
    m_spChatBg -> addChild(sendChatMenu);
    
    m_chatBox = CCEditBox::create(CCSize(516,67), CCScale9Sprite::create("CatchFish2006/chatkuang.png"));
    m_chatBox->setMaxLength(35);//限制最长字符
    m_chatBox->setReturnType(kKeyboardReturnTypeDone);
    m_chatBox->setInputFlag(kEditBoxInputFlagSensitive);
    m_chatBox->setInputMode(kEditBoxInputModeEmailAddr);
    m_chatBox->setPosition(ccp(m_spChatBg->getContentSize().width/2-56, m_spChatBg->getContentSize().height/2));
    m_chatBox->setPlaceHolder("请输入内容（禁发表情符）");
    m_chatBox->setPlaceholderFontSize(18);
    m_spChatBg->addChild(m_chatBox, 10);
    m_chatBox -> setTouchPriority(-202);
    m_chatBox -> setFontColor(ccc3(0, 0, 0));
    m_chatBox -> setFontName("Arial-BoldMT");
    
    
    //拍拍乐
    m_spRemindBg = CCSprite::create("CatchFish2006/remindbg.png");
    m_spRemindBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spRemindBg, 100);
    m_spRemindBg -> setVisible(false);
    m_labelRemind = CCLabelAtlas::create("", "CatchFish2006/remindnum.png", 33, 40, '0');
    m_labelRemind -> setAnchorPoint(ccp(0.5f, 0.5f));
    m_labelRemind -> setPosition(ccp(300, m_spRemindBg->getContentSize().height/2));
    m_spRemindBg -> addChild(m_labelRemind);
    
    for(int i = 0; i < 200; i++)
	{
		GoldSprite* sprite = GoldSprite::create();
		sprite -> setHave(false);
		sprite->setScale(0.75f);
		m_arrMoney -> addObject(sprite);
		sprite -> setVisible(false);
		addChildToBatch(true, sprite, "CatchFish01/fish9.png");
	}
	//创建子弹
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < MaxBulletCount*GAME_PLAYER; j++)
		{
			BulletSprite* bulletSprite = BulletSprite::create(i*2+1);
			bulletSprite -> setVisible(false);
			bulletSprite -> setHave(false);
			m_arrBullet -> addObject(bulletSprite);
            bulletSprite -> setAnchorPoint(ccp(0.5, 1));
			addChildToBatch(true, bulletSprite, "CatchFish01/fish4.png");
		}
	}
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h2all/byguidejian.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("CatchFish01/skilllabel.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3all/labanotice.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3all/yuchao.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("competition/rankLight.plist");
	//水纹动画
	CCAnimation* animation = CCAnimation::create();
	for (int i = 1; i <= 32; i++)
	{
		CCString* fileNameStr = CCString::createWithFormat("Water_%d.png", i);
		CCSpriteFrame* SpFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileNameStr->getCString());
		animation->addSpriteFrame(SpFrame);
	}
	animation->setDelayPerUnit(0.1f);
	animation->setLoops(-1);
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "Water");
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            CCAnimate* animate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("Water"));
            animate -> setTag(1000);
            CCSprite* waterSp = CCSprite::createWithSpriteFrameName("Water_1.png");
            waterSp->setPosition(ccp(150+300*i, 150+300*j));
            this->addChild(waterSp, 19, 5000+i*3+j);
            waterSp->setOpacity(130);
            waterSp->runAction(animate);
        }
    }
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("CatchFish01/Wave0.png", "CatchFish01/Wave0.plist", "CatchFish01/Wave.ExportJson");
	m_armatureWave = CCArmature::create("Wave");
	m_armatureWave->getAnimation()->playWithIndex(0);
	m_armatureWave->setAnchorPoint(ccp(0, 0.5));
	m_armatureWave->setPosition(ccp(m_sizeVisible.width+10, m_sizeVisible.height/2));
	this->addChild(m_armatureWave);
	
	//倒计时图片
	m_spReCount = CCSprite::create("CatchFish2001/countNum.png");
	m_spReCount->setPosition(ccp(m_sizeVisible.width/2, m_sizeVisible.height/2));
	this->addChild(m_spReCount, 1000);
	m_spReCount->setVisible(false);
	m_labelTTFReCount = CCLabelAtlas::create("","CatchFish01/goldNum6.png",27,34,'0');
	this -> addChild(m_labelTTFReCount,1001);

	loadBarrel();	//初始化炮台层
    //create a menu layer
    CCLayer* menuLayer = CCLayer::create();
    this -> addChild(menuLayer, 40, 503);
    CCLayer* menuLayer2 = CCLayer::create();
    menuLayer2 -> setPosition(CCPointZero);
    this -> addChild(menuLayer2, 40, 504);
    
    //自动开炮按钮
    CCMenuItemImage* autofireItem = CCMenuItemImage::create("CatchFish2001/autofirebg.png", "CatchFish2001/autofirebg.png", this, menu_selector(GameMainScene::itemCallBack));
    autofireItem -> setPosition(ccp(1220,130));//142,autofireItem->getContentSize().height/2+2
    autofireItem -> setTag(308);
    CCSprite* autofireSprite1 = CCSprite::createWithSpriteFrameName("autofire1.png");
    autofireSprite1 -> setPosition(ccp(autofireItem->getContentSize().width/2-5, autofireItem->getContentSize().height/2+9));
    autofireSprite1->setScale(1.1f);
    autofireItem -> addChild(autofireSprite1);
    CCSprite* autofireName = CCSprite::create("CatchFish2001/autofirename.png");
    autofireName -> setPosition(ccp(autofireItem->getContentSize().width/2, autofireName->getContentSize().height-7));
    autofireItem -> addChild(autofireName);
    
    CCMenuItemImage* autofireItem2 = CCMenuItemImage::create("CatchFish2001/autofirebg.png", "CatchFish2001/autofirebg.png", this, menu_selector(GameMainScene::itemCallBack));
    autofireItem2 -> setPosition(ccp(1220,130));
    autofireItem2 -> setTag(309);
    CCSprite* cancelAutoFireSprite = CCSprite::createWithSpriteFrameName("autofire1.png");
    cancelAutoFireSprite -> setPosition(ccp(autofireItem2->getContentSize().width/2-5, autofireItem2->getContentSize().height/2+9));
    autofireItem2 -> addChild(cancelAutoFireSprite);
    cancelAutoFireSprite->setScale(1.1f);
    CCSprite* autofireName2 = CCSprite::create("CatchFish2001/autofirename2.png");
    autofireName2 -> setPosition(ccp(autofireItem2->getContentSize().width/2, autofireName2->getContentSize().height-7));
    autofireItem2 -> addChild(autofireName2);
    CCArray* autofireArray = CCArray::create();
    for(int i = 1; i <= 20; i++)
    {
        char temp[24];
        sprintf(temp, "autofire%d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
        autofireArray -> addObject(frame);
    }
    CCAnimation* autofireAnimation = CCAnimation::createWithSpriteFrames(autofireArray, 0.05f);
    CCAnimate* autofireAnimate = CCAnimate::create(autofireAnimation);
    cancelAutoFireSprite -> runAction(CCRepeatForever::create(autofireAnimate));
    
    CCMenuItemImage* dantouItem = CCMenuItemImage::create("h3all/wufuitem.png", "h3all/wufuitem.png", this, menu_selector(GameMainScene::itemCallBack));
    dantouItem -> setPosition(ccp(1220, 540));
    dantouItem -> setTag(333);
    m_pMenuDantou = CCMenu::create(dantouItem, NULL);
    m_pMenuDantou -> setPosition(CCPointZero);
    m_pMenuDantou -> setAnchorPoint(CCPointZero);
    menuLayer2 -> addChild(m_pMenuDantou, 40, 507);
    m_arrMenu -> addObject(m_pMenuDantou);
    
    m_spDantouDi = CCSprite::create("CatchFish2001/jiaobiaodi.png");
    m_spDantouDi -> setPosition(ccp(dantouItem->getContentSize().width-10, dantouItem->getContentSize().height-10));
    dantouItem -> addChild(m_spDantouDi, 2);
    m_labelDantou = CCLabelAtlas::create("", "CatchFish2001/jiaobiaonum.png", 13, 16, '0');
    m_labelDantou -> setPosition(ccp(m_spDantouDi->getContentSize().width/2, m_spDantouDi->getContentSize().height/2));
    m_labelDantou -> setAnchorPoint(ccp(0.5f, 0.5f));
    m_spDantouDi -> addChild(m_labelDantou);
    m_spDantouDi -> setScale(1.2f);
    m_spDantouDi -> setVisible(false);
    
    m_dtParticle = CCSprite::create("h3all/wufuitem.png");
    dantouItem -> addChild(m_dtParticle);
    m_dtParticle -> setPosition(ccp(dantouItem->getContentSize().width/2, dantouItem->getContentSize().height/2));
    m_dtParticle -> setOpacity(0);
    m_dtParticle -> setVisible(false);
    CCSprite* pStarMiddleDT = CCSprite::create("charge/chargegamebg1.png");
    pStarMiddleDT->setPosition(ccp(m_dtParticle->getContentSize().width/2-3.5, m_dtParticle->getContentSize().height/2+5));
    pStarMiddleDT->setOpacity(0);
    pStarMiddleDT->setScale(0.7f);
    m_dtParticle->addChild(pStarMiddleDT, 1);
    CCSequence* seqMiddleDT =CCSequence::create(CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL), CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(1.2f),NULL);
    pStarMiddleDT->runAction(CCRepeatForever::create(seqMiddleDT));
    CCSprite* pStarDownRDT = CCSprite::create("charge/chargegamebg1.png");
    pStarDownRDT->setPosition(ccp(m_dtParticle->getContentSize().width/2+7, m_dtParticle->getContentSize().height/2-20));
    m_dtParticle->addChild(pStarDownRDT, 1);
    pStarDownRDT->setOpacity(0);
    pStarDownRDT->setScale(0.7f);
    CCSequence* seqDRDT = CCSequence::create(CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(1.0f),NULL);
    pStarDownRDT->runAction(CCRepeatForever::create(seqDRDT));
    
    CCSprite* pStarDownLDT = CCSprite::create("charge/chargegamebg1.png");
    pStarDownLDT->setPosition(ccp(m_dtParticle->getContentSize().width/2-40, m_dtParticle->getContentSize().height/2-10));
    m_dtParticle->addChild(pStarDownLDT, 1);
    pStarDownLDT->setOpacity(0);
    pStarDownLDT->setScale(0.5f);
    CCSequence* seqDLDT = CCSequence::create(CCDelayTime::create(0.4f),CCSpawn::create(CCScaleTo::create(0.2f, 1.3),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 0.8),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.8f),NULL);
    pStarDownLDT->runAction(CCRepeatForever::create(seqDLDT));
    
    CCSprite* pStarUpLDT = CCSprite::create("charge/chargegamebg1.png");
    pStarUpLDT->setPosition(ccp(m_dtParticle->getContentSize().width/2-23, m_dtParticle->getContentSize().height/2+45));
    m_dtParticle->addChild(pStarUpLDT, 1);
    pStarUpLDT->setOpacity(0);
    pStarUpLDT->setScale(0.7f);
    CCSequence* seqULDT = CCSequence::create(CCDelayTime::create(0.8f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.4f),NULL);
    pStarUpLDT->runAction(CCRepeatForever::create(seqULDT));
    
    CCSprite* pStarUpRDT = CCSprite::create("charge/chargegamebg1.png");
    pStarUpRDT->setPosition(ccp(m_dtParticle->getContentSize().width/2+31, m_dtParticle->getContentSize().height/2-10));
    m_dtParticle->addChild(pStarUpRDT, 1);
    pStarUpRDT->setOpacity(0);
    pStarUpRDT->setScale(0.3f);
    CCSequence* seqURDT = CCSequence::create(CCDelayTime::create(1.0f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 0.6),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.2f),NULL);
    pStarUpRDT->runAction(CCRepeatForever::create(seqURDT));
    
    CCSprite* pSanDT = CCSprite::create("charge/chargegamebg3.png");
    pSanDT->setPosition(ccp(m_dtParticle->getContentSize().width/2, m_dtParticle->getContentSize().height/2+10));
    m_dtParticle->addChild(pSanDT, 1);
    pSanDT->setOpacity(0);
    pSanDT->setScale(0.5f);
    ccBlendFunc cblDT = {GL_SRC_ALPHA, GL_ONE };
    pSanDT->setBlendFunc(cblDT);
    CCSequence* seqScaleDT = CCSequence::create(CCScaleTo::create(0.56f, 1.06),CCScaleTo::create(0.6f, 1.0),CCScaleTo::create(0.76f, 0.5),CCDelayTime::create(0.08f),NULL);
    CCSequence* seqRotationDT = CCSequence::create(CCRotateBy::create(1.92f, 0),CCDelayTime::create(0.08f),NULL);
    CCSequence* seqOpDT = CCSequence::create(CCFadeTo::create(0.48f, 76.5),CCDelayTime::create(0.88f),CCFadeTo::create(0.56f, 0),CCDelayTime::create(0.08f),NULL);
    CCSpawn* spaDT = CCSpawn::create(seqScaleDT, seqRotationDT, seqOpDT, NULL);
    pSanDT->runAction(CCRepeatForever::create(spaDT));
    
    CCSprite* pSan2DT = CCSprite::create("charge/chargegamebg3.png");
    pSan2DT->setPosition(ccp(m_dtParticle->getContentSize().width/2, m_dtParticle->getContentSize().height/2+10));
    m_dtParticle->addChild(pSan2DT, 1);
    ccBlendFunc cbl2DT = {GL_SRC_ALPHA, GL_ONE };
    pSan2DT->setBlendFunc(cbl2DT);
    pSan2DT->setScale(0.5f);
    pSan2DT->setOpacity(0);
    CCSequence* seqScale2DT = CCSequence::create(CCDelayTime::create(0.12f),CCScaleTo::create(0.64f, 1.07),CCScaleTo::create(1.12, 0.5),CCDelayTime::create(0.12f),NULL);
    CCSequence* seqRotation2DT = CCSequence::create(CCRotateBy::create(1.88f, 0),CCDelayTime::create(0.12f),NULL);
    CCSequence* seqOp2DT = CCSequence::create(CCDelayTime::create(0.2f),CCFadeIn::create(0.56),CCDelayTime::create(0.68),CCFadeOut::create(0.44f),CCDelayTime::create(0.12),NULL);
    CCSpawn* spa2DT = CCSpawn::create(seqScale2DT,seqRotation2DT, seqOp2DT, NULL);
    pSan2DT->runAction(CCRepeatForever::create(spa2DT));
    
    
    CCMenuItemImage* mobileItem = CCMenuItemImage::create("h2all/mobileItem.png", "h2all/mobileItem.png", this, menu_selector(GameMainScene::itemCallBack));
    mobileItem -> setPosition(ccp(80, CLIENT_HEIGHT/2-120));//1220,510
    mobileItem -> setTag(315);//313
    mobileItem -> setAnchorPoint(ccp(0.5f,0.1f));
    //    CCScaleTo* scale1 = CCScaleTo::create(0.25f, 0.9f, 1.1f);
    //    CCScaleTo* scale2 = CCScaleTo::create(0.25f, 1.1f, 0.9f);
    //    CCSequence* seq1 = CCSequence::create(scale1, scale2, NULL);
    //    mobileItem -> runAction(CCRepeatForever::create(seq1));
    m_pMenuMobile = CCMenu::create(mobileItem,NULL);
    m_pMenuMobile -> setPosition(CCPointZero);
    m_pMenuMobile -> setAnchorPoint(CCPointZero);
    menuLayer2 -> addChild(m_pMenuMobile,40,507);
    m_arrMenu -> addObject(m_pMenuMobile);
    
    CCSprite* pStarMiddleLottery = CCSprite::create("charge/chargegamebg1.png");
    pStarMiddleLottery->setPosition(ccp(mobileItem->getContentSize().width/2-3.5, mobileItem->getContentSize().height/2+5));
    pStarMiddleLottery->setOpacity(0);
    pStarMiddleLottery->setScale(0.7f);
    mobileItem->addChild(pStarMiddleLottery, 10);
    CCSequence* seqMiddleLottery =CCSequence::create(CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL), CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(1.2f),NULL);
    pStarMiddleLottery->runAction(CCRepeatForever::create(seqMiddleLottery));
    CCSprite* pStarDownRLottery = CCSprite::create("charge/chargegamebg1.png");
    pStarDownRLottery->setPosition(ccp(mobileItem->getContentSize().width/2+7, mobileItem->getContentSize().height/2-20));
    mobileItem->addChild(pStarDownRLottery, 10);
    pStarDownRLottery->setOpacity(0);
    pStarDownRLottery->setScale(0.7f);
    CCSequence* seqDRLottery = CCSequence::create(CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(1.0f),NULL);
    pStarDownRLottery->runAction(CCRepeatForever::create(seqDRLottery));
    
    CCSprite* pStarDownLLottery = CCSprite::create("charge/chargegamebg1.png");
    pStarDownLLottery->setPosition(ccp(mobileItem->getContentSize().width/2-40, mobileItem->getContentSize().height/2-10));
    mobileItem->addChild(pStarDownLLottery, 10);
    pStarDownLLottery->setOpacity(0);
    pStarDownLLottery->setScale(0.5f);
    CCSequence* seqDLLottery = CCSequence::create(CCDelayTime::create(0.4f),CCSpawn::create(CCScaleTo::create(0.2f, 1.3),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 0.8),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.8f),NULL);
    pStarDownLLottery->runAction(CCRepeatForever::create(seqDLLottery));
    
    CCSprite* pStarUpLLottery = CCSprite::create("charge/chargegamebg1.png");
    pStarUpLLottery->setPosition(ccp(mobileItem->getContentSize().width/2-23, mobileItem->getContentSize().height/2+45));
    mobileItem->addChild(pStarUpLLottery, 10);
    pStarUpLLottery->setOpacity(0);
    pStarUpLLottery->setScale(0.7f);
    CCSequence* seqULLottery = CCSequence::create(CCDelayTime::create(0.8f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.4f),NULL);
    pStarUpLLottery->runAction(CCRepeatForever::create(seqULLottery));
    
    CCSprite* pStarUpRLottery = CCSprite::create("charge/chargegamebg1.png");
    pStarUpRLottery->setPosition(ccp(mobileItem->getContentSize().width/2+31, mobileItem->getContentSize().height/2-10));
    mobileItem->addChild(pStarUpRLottery, 10);
    pStarUpRLottery->setOpacity(0);
    pStarUpRLottery->setScale(0.3f);
    CCSequence* seqURLottery = CCSequence::create(CCDelayTime::create(1.0f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 0.6),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.2f),NULL);
    pStarUpRLottery->runAction(CCRepeatForever::create(seqURLottery));
    
    CCSprite* pSanLottery = CCSprite::create("charge/chargegamebg3.png");
    pSanLottery->setPosition(ccp(mobileItem->getContentSize().width/2, mobileItem->getContentSize().height/2+10));
    mobileItem->addChild(pSanLottery, 10);
    pSanLottery->setOpacity(0);
    pSanLottery->setScale(0.5f);
    ccBlendFunc cblLottery = {GL_SRC_ALPHA, GL_ONE };
    pSanLottery->setBlendFunc(cblLottery);
    CCSequence* seqScaleLottery = CCSequence::create(CCScaleTo::create(0.56f, 1.06),CCScaleTo::create(0.6f, 1.0),CCScaleTo::create(0.76f, 0.5),CCDelayTime::create(0.08f),NULL);
    CCSequence* seqRotationLottery = CCSequence::create(CCRotateBy::create(1.92f, 0),CCDelayTime::create(0.08f),NULL);
    CCSequence* seqOpLottery = CCSequence::create(CCFadeTo::create(0.48f, 76.5),CCDelayTime::create(0.88f),CCFadeTo::create(0.56f, 0),CCDelayTime::create(0.08f),NULL);
    CCSpawn* spaLottery = CCSpawn::create(seqScaleLottery, seqRotationLottery, seqOpLottery, NULL);
    pSanLottery->runAction(CCRepeatForever::create(spaLottery));
    
    CCSprite* pSan2Lottery = CCSprite::create("charge/chargegamebg3.png");
    pSan2Lottery->setPosition(ccp(mobileItem->getContentSize().width/2, mobileItem->getContentSize().height/2+10));
    mobileItem->addChild(pSan2Lottery, 10);
    ccBlendFunc cbl2Lottery = {GL_SRC_ALPHA, GL_ONE };
    pSan2Lottery->setBlendFunc(cbl2Lottery);
    pSan2Lottery->setScale(0.5f);
    pSan2Lottery->setOpacity(0);
    CCSequence* seqScale2Lottery = CCSequence::create(CCDelayTime::create(0.12f),CCScaleTo::create(0.64f, 1.07),CCScaleTo::create(1.12, 0.5),CCDelayTime::create(0.12f),NULL);
    CCSequence* seqRotation2Lottery = CCSequence::create(CCRotateBy::create(1.88f, 0),CCDelayTime::create(0.12f),NULL);
    CCSequence* seqOp2Lottery = CCSequence::create(CCDelayTime::create(0.2f),CCFadeIn::create(0.56),CCDelayTime::create(0.68),CCFadeOut::create(0.44f),CCDelayTime::create(0.12),NULL);
    CCSpawn* spa2Lottery = CCSpawn::create(seqScale2Lottery,seqRotation2Lottery, seqOp2Lottery, NULL);
    pSan2Lottery->runAction(CCRepeatForever::create(spa2Lottery));
    
    //邮件
    CCMenuItemImage* pMailItem = CCMenuItemImage::create("CatchFish2001/mailItem.png", "CatchFish2001/mailItem.png", this, menu_selector(GameMainScene::itemCallBack));
    pMailItem -> setPosition(ccp(80, CLIENT_HEIGHT/2+30));//1220, 430
    pMailItem -> setAnchorPoint(ccp(0.5f,0.1f));
    pMailItem -> setTag(329);
    m_pMailMenu = CCMenu::create(pMailItem, NULL);
    m_pMailMenu -> setPosition(CCPointZero);
    m_pMailMenu -> setAnchorPoint(CCPointZero);
    menuLayer2 -> addChild(m_pMailMenu, 110);
    m_arrMenu -> addObject(m_pMailMenu);
    CCSprite* pMailSprite = CCSprite::create("h2all/mailSprite.png");
    pMailSprite -> setPosition(ccp(pMailItem->getContentSize().width-30, pMailItem->getContentSize().height-15));
    pMailItem->addChild(pMailSprite, 1, 10);
    pMailSprite->setVisible(false);
    CCSprite* pMailSprite2 = CCSprite::create("h2all/mailSprite.png");
    pMailSprite2 -> setPosition(ccp(pMailItem->getContentSize().width-30, pMailItem->getContentSize().height-15));
    pMailItem->addChild(pMailSprite2, 1, 11);
    pMailSprite2->setVisible(false);
    pMailSprite2 -> setOpacity(150);
    pMailSprite2 -> setScale(1.3f);
    
    //首充礼包
    CCMenuItemImage* pChargeItem = CCMenuItemImage::create("CatchFish2001/chargegamebg.png", "CatchFish2001/chargegamebg.png", this, menu_selector(GameMainScene::itemCallBack));
    pChargeItem -> setPosition(ccp(1220, 430));//80, CLIENT_HEIGHT/2+30
    pChargeItem->setTag(302);
    pChargeItem -> setScale(0.9f);
    pChargeItem -> setAnchorPoint(ccp(0.5f,0.5f));
//    CCSprite* pSpChargeBg2 = CCSprite::create("CatchFish2001/gamechargebg.png");
//    pSpChargeBg2 -> setPosition(ccp(pChargeItem->getContentSize().width/2, 2));
//    pChargeItem -> addChild(pSpChargeBg2, 10);
//    pSpChargeBg2 -> setScale(1.1f);
    CCSprite* pStarMiddle = CCSprite::create("charge/chargegamebg1.png");
    pStarMiddle->setPosition(ccp(pChargeItem->getContentSize().width/2-3.5, pChargeItem->getContentSize().height/2+5));
    pStarMiddle->setOpacity(0);
    pStarMiddle->setScale(0.7f);
    pChargeItem->addChild(pStarMiddle, 1);
    CCSequence* seqMiddle =CCSequence::create(CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL), CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(1.2f),NULL);
    pStarMiddle->runAction(CCRepeatForever::create(seqMiddle));
    CCSprite* pStarDownR = CCSprite::create("charge/chargegamebg1.png");
    pStarDownR->setPosition(ccp(pChargeItem->getContentSize().width/2+7, pChargeItem->getContentSize().height/2-20));
    pChargeItem->addChild(pStarDownR, 1);
    pStarDownR->setOpacity(0);
    pStarDownR->setScale(0.7f);
    CCSequence* seqDR = CCSequence::create(CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(1.0f),NULL);
    pStarDownR->runAction(CCRepeatForever::create(seqDR));
    
    CCSprite* pStarDownL = CCSprite::create("charge/chargegamebg1.png");
    pStarDownL->setPosition(ccp(pChargeItem->getContentSize().width/2-40, pChargeItem->getContentSize().height/2-10));
    pChargeItem->addChild(pStarDownL, 1);
    pStarDownL->setOpacity(0);
    pStarDownL->setScale(0.5f);
    CCSequence* seqDL = CCSequence::create(CCDelayTime::create(0.4f),CCSpawn::create(CCScaleTo::create(0.2f, 1.3),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 0.8),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.8f),NULL);
    pStarDownL->runAction(CCRepeatForever::create(seqDL));
    
    CCSprite* pStarUpL = CCSprite::create("charge/chargegamebg1.png");
    pStarUpL->setPosition(ccp(pChargeItem->getContentSize().width/2-23, pChargeItem->getContentSize().height/2+45));
    pChargeItem->addChild(pStarUpL, 1);
    pStarUpL->setOpacity(0);
    pStarUpL->setScale(0.7f);
    CCSequence* seqUL = CCSequence::create(CCDelayTime::create(0.8f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.4f),NULL);
    pStarUpL->runAction(CCRepeatForever::create(seqUL));
    
    CCSprite* pStarUpR = CCSprite::create("charge/chargegamebg1.png");
    pStarUpR->setPosition(ccp(pChargeItem->getContentSize().width/2+31, pChargeItem->getContentSize().height/2-10));
    pChargeItem->addChild(pStarUpR, 1);
    pStarUpR->setOpacity(0);
    pStarUpR->setScale(0.3f);
    CCSequence* seqUR = CCSequence::create(CCDelayTime::create(1.0f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 0.6),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.2f),NULL);
    pStarUpR->runAction(CCRepeatForever::create(seqUR));
    
    CCSprite* pSan = CCSprite::create("charge/chargegamebg3.png");
    pSan->setPosition(ccp(pChargeItem->getContentSize().width/2, pChargeItem->getContentSize().height/2+10));
    pChargeItem->addChild(pSan, 1);
    pSan->setOpacity(0);
    pSan->setScale(0.5f);
    ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE };
    pSan->setBlendFunc(cbl);
    CCSequence* seqScale = CCSequence::create(CCScaleTo::create(0.56f, 1.06),CCScaleTo::create(0.6f, 1.0),CCScaleTo::create(0.76f, 0.5),CCDelayTime::create(0.08f),NULL);
    CCSequence* seqRotation = CCSequence::create(CCRotateBy::create(1.92f, 0),CCDelayTime::create(0.08f),NULL);
    CCSequence* seqOp = CCSequence::create(CCFadeTo::create(0.48f, 76.5),CCDelayTime::create(0.88f),CCFadeTo::create(0.56f, 0),CCDelayTime::create(0.08f),NULL);
    CCSpawn* spa = CCSpawn::create(seqScale, seqRotation, seqOp, NULL);
    pSan->runAction(CCRepeatForever::create(spa));
    
    CCSprite* pSan2 = CCSprite::create("charge/chargegamebg3.png");
    pSan2->setPosition(ccp(pChargeItem->getContentSize().width/2, pChargeItem->getContentSize().height/2+10));
    pChargeItem->addChild(pSan2, 1);
    ccBlendFunc cbl2 = {GL_SRC_ALPHA, GL_ONE };
    pSan2->setBlendFunc(cbl2);
    pSan2->setScale(0.5f);
    pSan2->setOpacity(0);
    CCSequence* seqScale2 = CCSequence::create(CCDelayTime::create(0.12f),CCScaleTo::create(0.64f, 1.07),CCScaleTo::create(1.12, 0.5),CCDelayTime::create(0.12f),NULL);
    CCSequence* seqRotation2 = CCSequence::create(CCRotateBy::create(1.88f, 0),CCDelayTime::create(0.12f),NULL);
    CCSequence* seqOp2 = CCSequence::create(CCDelayTime::create(0.2f),CCFadeIn::create(0.56),CCDelayTime::create(0.68),CCFadeOut::create(0.44f),CCDelayTime::create(0.12),NULL);
    CCSpawn* spa2 = CCSpawn::create(seqScale2,seqRotation2, seqOp2, NULL);
    pSan2->runAction(CCRepeatForever::create(spa2));
    
    //背包按钮（现在的聊天按钮）
    CCMenuItemImage* pBeibaoItem = CCMenuItemImage::create("CatchFish2001/chatitem.png", "CatchFish2001/chatitem.png", this, menu_selector(GameMainScene::itemCallBack));
    pBeibaoItem -> setTag(330);
    pBeibaoItem -> setPosition(ccp(40,40));
    pBeibaoItem -> setScale(0.8f);
    m_pBeibaoMenu = CCMenu::create(pBeibaoItem, NULL);
    m_pBeibaoMenu -> setPosition(CCPointZero);
    m_pBeibaoMenu -> setAnchorPoint(CCPointZero);
    menuLayer2 -> addChild(m_pBeibaoMenu, 110);
    m_arrMenu -> addObject(m_pBeibaoMenu);
    if(!CMainLogic::sharedMainLogic()->m_bShowChat)
    {
        m_pBeibaoMenu->setVisible(false);
    }
    
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        m_pBeibaoMenu -> setVisible(false);
        
        m_pMenuDantou -> setVisible(false);
        
    }
    m_pMailMenu -> setVisible(false);
    m_pMenuMobile -> setVisible(false);
    //锁定，散射，锁定按钮
    CCMenuItemImage* speedUpItem = CCMenuItemImage::create("CatchFish2001/speeditem.png", "CatchFish2001/speeditem.png", this, menu_selector(GameMainScene::itemCallBack));
    speedUpItem -> setTag(306);
    speedUpItem -> setPosition(ccp(1220, 330));//430
    m_spSpeedUp = CCSprite::create("CatchFish2001/autofirebg2.png");
    m_spSpeedUp -> setPosition(ccp(speedUpItem->getContentSize().width/2, speedUpItem->getContentSize().height/2));
    speedUpItem -> addChild(m_spSpeedUp, 1);
    m_spSpeedUp -> setVisible(false);
    m_spSpeedUp -> setOpacity(100);
    CCSprite *speedupSprite = CCSprite::create("CatchFish2001/speeditem.png");
    speedupCD = CCProgressTimer::create(speedupSprite);
    speedupCD->setType(kCCProgressTimerTypeRadial);
    speedupCD->setPosition(ccp(speedUpItem->getContentSize().width/2, speedUpItem->getContentSize().height/2));
    speedUpItem->addChild(speedupCD, 1);
    speedupCD -> setVisible(false);
    CCSprite* speedUpDi = CCSprite::create("CatchFish2001/jiaobiaodi.png");
    speedUpDi -> setPosition(ccp(speedUpItem->getContentSize().width-10, speedUpItem->getContentSize().height-10));
    speedUpItem -> addChild(speedUpDi, 2);
    m_labelSpeedUp = CCLabelAtlas::create("", "CatchFish2001/jiaobiaonum.png", 13, 16, '0');
    m_labelSpeedUp -> setPosition(ccp(speedUpDi->getContentSize().width/2, speedUpDi->getContentSize().height/2));
    m_labelSpeedUp -> setAnchorPoint(ccp(0.5f, 0.5f));
    speedUpDi -> addChild(m_labelSpeedUp);
    
    //播放转圈特效
    speedSp1 = CCSprite::createWithSpriteFrameName("sanshe1.png");
    speedSp1 -> setPosition(ccp(speedUpItem->getContentSize().width/2, speedUpItem->getContentSize().height/2));
    speedUpItem -> addChild(speedSp1, 3);
    speedSp1->setVisible(false);
    CCArray* speedUpArray = CCArray::create();
    char speedUptemp[24];
    for(int i = 1; i <= 26; i++)
    {
        sprintf(speedUptemp, "sanshe%d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(speedUptemp);
        speedUpArray -> addObject(frame);
    }
    CCAnimation* speedUpAnimation = CCAnimation::createWithSpriteFrames(speedUpArray, 0.05f);
    CCAnimate* speedUpAnimate = CCAnimate::create(speedUpAnimation);
    speedSp1 -> runAction(CCRepeatForever::create(speedUpAnimate));
    
    CCMenuItemImage* sansheItem = CCMenuItemImage::create("CatchFish2001/sansheitem.png", "CatchFish2001/sansheitem.png", this, menu_selector(GameMainScene::itemCallBack));
    sansheItem -> setPosition(ccp(1220, 330));
    sansheItem -> setTag(322);
    m_spSanche = CCSprite::create("CatchFish2001/autofirebg2.png");
    m_spSanche -> setPosition(ccp(sansheItem->getContentSize().width/2, sansheItem->getContentSize().height/2));
    sansheItem -> addChild(m_spSanche, 1);
    m_spSanche -> setVisible(false);
    m_spSanche -> setOpacity(100);
    CCSprite *sansheSprite = CCSprite::create("CatchFish2001/sansheitem.png");
    sansheCD = CCProgressTimer::create(sansheSprite);
    sansheCD->setType(kCCProgressTimerTypeRadial);
    sansheCD->setPosition(ccp(sansheItem->getContentSize().width/2, sansheItem->getContentSize().height/2));
    sansheItem->addChild(sansheCD, 1);
    sansheCD -> setVisible(false);
    CCSprite* sansheDi = CCSprite::create("CatchFish2001/jiaobiaodi.png");
    sansheDi -> setPosition(ccp(sansheItem->getContentSize().width-10, sansheItem->getContentSize().height-10));
    sansheItem -> addChild(sansheDi, 2);
    m_labelSanshe = CCLabelAtlas::create("", "CatchFish2001/jiaobiaonum.png", 13, 16, '0');
    m_labelSanshe -> setPosition(ccp(sansheDi->getContentSize().width/2, sansheDi->getContentSize().height/2));
    m_labelSanshe -> setAnchorPoint(ccp(0.5f, 0.5f));
    sansheDi -> addChild(m_labelSanshe);
    //播放转圈特效
    speedSp2 = CCSprite::createWithSpriteFrameName("sanshe1.png");
    speedSp2 -> setPosition(ccp(sansheItem->getContentSize().width/2, sansheItem->getContentSize().height/2));
    sansheItem -> addChild(speedSp2, 3);
    speedSp2->setVisible(false);
    CCArray* sansheArray = CCArray::create();
    char sanshetemp[24];
    for(int i = 1; i <= 26; i++)
    {
        sprintf(sanshetemp, "sanshe%d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(sanshetemp);
        sansheArray -> addObject(frame);
    }
    CCAnimation* sansheAnimation = CCAnimation::createWithSpriteFrames(sansheArray, 0.05f);
    CCAnimate* sansheAnimate = CCAnimate::create(sansheAnimation);
    speedSp2 -> runAction(CCRepeatForever::create(sansheAnimate));
    
    CCMenuItemImage* baojiItem = CCMenuItemImage::create("CatchFish2001/suodingitem.png", "CatchFish2001/suodingitem.png", this, menu_selector(GameMainScene::itemCallBack));
    baojiItem -> setPosition(ccp(1220, 230));
    baojiItem -> setTag(323);
    m_spBaoji = CCSprite::create("CatchFish2001/autofirebg2.png");
    m_spBaoji -> setPosition(ccp(baojiItem->getContentSize().width/2, baojiItem->getContentSize().height/2));
    baojiItem -> addChild(m_spBaoji, 1);
    m_spBaoji -> setOpacity(100);
    m_spBaoji -> setVisible(false);
    CCSprite *activeSprite = CCSprite::create("CatchFish2001/suodingitem.png");
    baojiCD = CCProgressTimer::create(activeSprite);
    baojiCD->setType(kCCProgressTimerTypeRadial);
    baojiCD->setPosition(ccp(baojiItem->getContentSize().width/2, baojiItem->getContentSize().height/2));
    baojiItem->addChild(baojiCD, 1);
    baojiCD -> setVisible(false);
    CCSprite* baojiDi = CCSprite::create("CatchFish2001/jiaobiaodi.png");
    baojiDi -> setPosition(ccp(baojiItem->getContentSize().width-10, baojiItem->getContentSize().height-10));
    baojiItem->addChild(baojiDi, 2);
    m_labelBaoji = CCLabelAtlas::create("", "CatchFish2001/jiaobiaonum.png", 13, 16, '0');
    m_labelBaoji -> setPosition(ccp(baojiDi->getContentSize().width/2, baojiDi->getContentSize().height/2));
    m_labelBaoji -> setAnchorPoint(ccp(0.5f, 0.5f));
    baojiDi -> addChild(m_labelBaoji);
    //播放转圈特效
    speedSp3 = CCSprite::createWithSpriteFrameName("sanshe1.png");
    speedSp3 -> setPosition(ccp(baojiItem->getContentSize().width/2, baojiItem->getContentSize().height/2));
    baojiItem -> addChild(speedSp3, 3);
    speedSp3->setVisible(false);
    CCArray* baojiArray = CCArray::create();
    char baojitemp[24];
    for(int i = 1; i <= 26; i++)
    {
        sprintf(baojitemp, "sanshe%d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(baojitemp);
        baojiArray -> addObject(frame);
    }
    CCAnimation* baojiAnimation = CCAnimation::createWithSpriteFrames(baojiArray, 0.05f);
    CCAnimate* baojiAnimate = CCAnimate::create(baojiAnimation);
    speedSp3 -> runAction(CCRepeatForever::create(baojiAnimate));
    
    CCMenu* pMenuWnd5 = CCMenu::create(baojiItem, NULL);
    pMenuWnd5 -> setPosition(CCPointZero);
    pMenuWnd5 -> setAnchorPoint(CCPointZero);
    menuLayer2 -> addChild(pMenuWnd5, 108);
    CCMenu* pMenuWnd3 = CCMenu::create(sansheItem, NULL);
    pMenuWnd3 -> setPosition(CCPointZero);
    pMenuWnd3 -> setAnchorPoint(CCPointZero);
    menuLayer2 -> addChild(pMenuWnd3, 109);
    pMenuWnd3 -> setVisible(false);
    CCMenu* pMenuWnd1 = CCMenu::create(speedUpItem, NULL);
    pMenuWnd1 -> setPosition(CCPointZero);
    pMenuWnd1 -> setAnchorPoint(CCPointZero);
    menuLayer2 -> addChild(pMenuWnd1, 102);
    
    CCMenu* pMenuWnd4 = CCMenu::create(autofireItem,NULL);
    pMenuWnd4 -> setPosition(CCPointZero);
    pMenuWnd4 -> setAnchorPoint(CCPointZero);
    menuLayer2 -> addChild(pMenuWnd4,1,103);
    CCMenu* pMenuWnd6 = CCMenu::create(autofireItem2,NULL);
    pMenuWnd6 -> setPosition(CCPointZero);
    pMenuWnd6 -> setAnchorPoint(CCPointZero);
    menuLayer2 -> addChild(pMenuWnd6,1,105);
    pMenuWnd6->setVisible(false);
    CCMenu* pMenuWnd7 = CCMenu::create(pChargeItem, NULL);
    pMenuWnd7 -> setPosition(CCPointZero);
    pMenuWnd7 -> setAnchorPoint(CCPointZero);
    menuLayer2 -> addChild(pMenuWnd7,1,106);
    m_arrMenu -> addObject(pMenuWnd4);
    m_arrMenu -> addObject(pMenuWnd6);
    m_arrMenu -> addObject(pMenuWnd7);
    m_arrMenu -> addObject(pMenuWnd1);
    m_arrMenu -> addObject(pMenuWnd5);
    
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)//未通过审核
    {
        speedUpItem->setVisible(false);
        sansheItem->setVisible(false);
        baojiItem->setVisible(false);
    }
    
    CCSprite* menuSprite1 = CCSprite::create("CatchFish2001/menuItem1.png");
    CCSprite* menuSprite2 = CCSprite::create("CatchFish2001/menuItem1.png");
    CCMenuItemSprite* menuItem1 = CCMenuItemSprite::create(menuSprite1, menuSprite2,this,menu_selector(GameMainScene::itemCallBack));
    menuItem1 -> setTag(300);
    menuItem1 -> setPosition(ccp(CLIENT_WIDTH-60, CLIENT_HEIGHT-60));
    CCMenu* pMenu1 = CCMenu::create(menuItem1,NULL);
    pMenu1 -> setPosition(CCPointZero);
    pMenu1 -> setAnchorPoint(CCPointZero);
    this -> addChild(pMenu1,41,501);
    m_arrMenu -> addObject(pMenu1);
    CCSprite* menuSprite3 = CCSprite::create("CatchFish2001/menuItem1.png");
    menuSprite3 -> setPosition(ccp(menuItem1->getContentSize().width/2, menuItem1->getContentSize().height/2));
    menuItem1 -> addChild(menuSprite3, 1, 100);
    menuSprite3 -> setVisible(false);

    
    int nHeight = 50;
    CCSprite* pSpMenu = CCSprite::create();
    pSpMenu->setAnchorPoint(ccp(0.5f, 1.0f));
    pSpMenu->setPosition(ccp(CLIENT_WIDTH, CLIENT_HEIGHT-nHeight));
    menuLayer->addChild(pSpMenu, 1, 999);
    
    CCSprite* helpSprite1 = CCSprite::create("CatchFish2001/help.png");
    CCSprite* helpSprite2 = CCSprite::create("CatchFish2001/help.png");
    CCMenuItemSprite* helpItem = CCMenuItemSprite::create(helpSprite1, helpSprite2,this,menu_selector(GameMainScene::itemCallBack));
    helpItem -> setPosition(ccp(-CLIENT_WIDTH+(m_sizeVisible.width - 60), -13));
    helpItem -> setTag(303);
    CCSprite* settingSprite1 = CCSprite::create("CatchFish2001/setting.png");
    CCSprite* settingSprite2 = CCSprite::create("CatchFish2001/setting.png");
    CCMenuItemSprite* settingItem = CCMenuItemSprite::create(settingSprite1, settingSprite2,this,menu_selector(GameMainScene::itemCallBack));
    settingItem -> setPosition(helpItem->getPosition());
    settingItem -> setTag(304);
    CCSprite* gameExitSprite1 = CCSprite::create("CatchFish2001/exit.png");
    CCSprite* gameExitSprite2 = CCSprite::create("CatchFish2001/exit.png");
    CCMenuItemSprite* gameExitItem = CCMenuItemSprite::create(gameExitSprite1, gameExitSprite2,this,menu_selector(GameMainScene::itemCallBack));
    gameExitItem -> setPosition(helpItem->getPosition());
    gameExitItem -> setTag(305);
    
    CCMenu* pMenuWnd = CCMenu::create(settingItem,helpItem,gameExitItem,NULL);
    pMenuWnd -> setPosition(CCPointZero);
    pMenuWnd -> setAnchorPoint(CCPointZero);
    pMenuWnd->setTag(100);
    pSpMenu -> addChild(pMenuWnd);
    m_arrMenu -> addObject(pMenuWnd);
    pMenuWnd->setTouchEnabled(false);
    pMenuWnd->setVisible(false);

	//初始化设置层、帮助层、购买金币层
	m_fishWndLayer = FishWndLayer::create();
	addChild(m_fishWndLayer, 99);
    
    m_pSpBlue = CCSprite::create("CatchFish2001/bluebg.png");
    m_pSpBlue -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    m_pSpBlue -> setScaleX(CLIENT_WIDTH/m_pSpBlue->getContentSize().width);
    m_pSpBlue -> setScaleY(CLIENT_HEIGHT/m_pSpBlue->getContentSize().height);
    this -> addChild(m_pSpBlue, 100);
    m_pSpBlue -> setOpacity(0);
    
    m_speedupAnimate = CCSprite::createWithSpriteFrameName("speeduplabel1.png");
    m_speedupAnimate -> setPosition(ccp(CLIENT_WIDTH/2, 400));
    m_speedupAnimate -> setVisible(false);
    m_speedupAnimate -> setScale(2.0f);
    m_baojiAnimate = CCSprite::createWithSpriteFrameName("baojilabel1.png");
    m_baojiAnimate -> setPosition(ccp(CLIENT_WIDTH/2, 400));
    m_baojiAnimate -> setVisible(false);
    m_baojiAnimate -> setScale(2.0f);
    this -> addChild(m_baojiAnimate, 100);
    this -> addChild(m_speedupAnimate , 100);
    
    //在线时间奖励
    CCMenuItemImage* pOnlineItem = CCMenuItemImage::create("olinereward/onlinegame1.png", "olinereward/onlinegame1.png", this, menu_selector(GameMainScene::itemCallBack));
    pOnlineItem -> setTag(334);
    pOnlineItem -> setPosition(ccp(70, CLIENT_HEIGHT/2));
    pOnlineItem -> setOpacity(0);
    m_pOnlineMenu = CCMenu::create(pOnlineItem, NULL);
    m_pOnlineMenu -> setPosition(CCPointZero);
    m_pOnlineMenu -> setAnchorPoint(CCPointZero);
    menuLayer2 -> addChild(m_pOnlineMenu);
    m_arrMenu -> addObject(m_pOnlineMenu);
    m_pOnlineMenu -> setVisible(false);
    m_pOnlineBg = CCSprite::create("olinereward/onlinegame1.png");
    m_pOnlineBg -> setPosition(ccp(pOnlineItem->getContentSize().width/2, pOnlineItem->getContentSize().height/2));
    pOnlineItem -> addChild(m_pOnlineBg);
    CCSprite* pOnlineTimeBg = CCSprite::create("olinereward/onlinegame2.png");
    pOnlineTimeBg -> setPosition(ccp(pOnlineItem->getContentSize().width/2, 0));
    pOnlineItem -> addChild(pOnlineTimeBg);
    m_pOnlineLabel = CCLabelTTF::create("", "Arial-BoldMT", 30);
    m_pOnlineLabel -> setPosition(ccp(pOnlineTimeBg->getContentSize().width/2, pOnlineTimeBg->getContentSize().height/2));
    pOnlineTimeBg -> addChild(m_pOnlineLabel);
    m_pOnlineDaojishi = CCSprite::create("olinereward/onlinegame5.png");
    m_pOnlineDaojishi -> setPosition(ccp(pOnlineTimeBg->getContentSize().width/2, -15));
    pOnlineTimeBg -> addChild(m_pOnlineDaojishi);
    m_pOnlineLingqu = CCSprite::create("olinereward/onlinegame3.png");
    m_pOnlineLingqu -> setPosition(ccp(pOnlineTimeBg->getContentSize().width/2, pOnlineTimeBg->getContentSize().height/2));
    pOnlineTimeBg -> addChild(m_pOnlineLingqu);
    m_pOnlineLingqu -> setVisible(false);
    m_pOnline2Lingqu = CCSprite::create("olinereward/onlinegame4.png");
    m_pOnline2Lingqu -> setPosition(ccp(pOnlineItem->getContentSize().width+30, pOnlineItem->getContentSize().height/2));
    m_pOnline2Lingqu -> setAnchorPoint(ccp(0, 0.5f));
    pOnlineItem -> addChild(m_pOnline2Lingqu);
    m_pOnline2Lingqu -> setVisible(false);
    m_pOnline2Lingqu -> runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f, ccp(-20, 0)), CCMoveBy::create(0.5f, ccp(20, 0)), NULL)));
    
    
    //比赛按钮
    CCMenuItemImage* bisaiItem = CCMenuItemImage::create("competition/cmptBtn.png", "competition/cmptBtn.png", this, menu_selector(GameMainScene::itemCallBack));
    bisaiItem -> setTag(326);
    bisaiItem -> setPosition(ccp(80, 569-117/2+10-40));
    m_pBisaiMenu = CCMenu::create(bisaiItem, NULL);
    m_pBisaiMenu -> setPosition(CCPointZero);
    m_arrMenu -> addObject(m_pBisaiMenu);
    m_pBisaiMenu -> setAnchorPoint(CCPointZero);
    menuLayer2 -> addChild(m_pBisaiMenu);
    bisaiItem -> setAnchorPoint(ccp(0.7f, 0.0f));
    m_spMatchGuide1 = CCSprite::createWithSpriteFrameName("byguidejian2.png");
    m_spMatchGuide1->setPosition(ccp(bisaiItem->getContentSize().width+30, bisaiItem->getContentSize().height/2));
    bisaiItem->addChild(m_spMatchGuide1);
    m_spMatchGuide1->setRotation(90);
    CCRepeatForever* pRep = CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f, ccp(10, 0)), CCMoveBy::create(0.5f, ccp(-10, 0)), NULL));
    m_spMatchGuide1->runAction(pRep);
    m_spMatchGuide2 = CCSprite::create("competition/matchguide2.png");
    m_spMatchGuide2->setPosition(ccp(bisaiItem->getContentSize().width+80+m_spMatchGuide2->getContentSize().width/2, bisaiItem->getContentSize().height/2));
    bisaiItem->addChild(m_spMatchGuide2);
    m_spMatchGuide1 -> setVisible(false);
    m_spMatchGuide2 -> setVisible(false);
    if(!CMainLogic::sharedMainLogic()->m_bShowMatch)
    {
        bisaiItem->setVisible(false);
    }
    //比赛特效
    CCSprite* pStarMiddle3 = CCSprite::create("charge/chargegamebg1.png");
    pStarMiddle3->setPosition(ccp(bisaiItem->getContentSize().width/2-3.5, bisaiItem->getContentSize().height/2+5));
    pStarMiddle3->setOpacity(0);
    pStarMiddle3->setScale(0.7f);
    bisaiItem->addChild(pStarMiddle3, 1);
    CCSequence* seqMiddle3 =CCSequence::create(CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL), CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(1.2f),NULL);
    pStarMiddle3->runAction(CCRepeatForever::create(seqMiddle3));
    CCSprite* pStarDownR3 = CCSprite::create("charge/chargegamebg1.png");
    pStarDownR3->setPosition(ccp(bisaiItem->getContentSize().width/2+7, bisaiItem->getContentSize().height/2-20));
    bisaiItem->addChild(pStarDownR3, 1);
    pStarDownR3->setOpacity(0);
    pStarDownR3->setScale(0.7f);
    CCSequence* seqDR3 = CCSequence::create(CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(1.0f),NULL);
    pStarDownR3->runAction(CCRepeatForever::create(seqDR3));
    
    CCSprite* pStarDownL3 = CCSprite::create("charge/chargegamebg1.png");
    pStarDownL3->setPosition(ccp(bisaiItem->getContentSize().width/2-40, bisaiItem->getContentSize().height/2-10));
    bisaiItem->addChild(pStarDownL3, 1);
    pStarDownL3->setOpacity(0);
    pStarDownL3->setScale(0.5f);
    CCSequence* seqDL3 = CCSequence::create(CCDelayTime::create(0.4f),CCSpawn::create(CCScaleTo::create(0.2f, 1.3),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 0.8),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.8f),NULL);
    pStarDownL3->runAction(CCRepeatForever::create(seqDL3));
    
    CCSprite* pStarUpL3 = CCSprite::create("charge/chargegamebg1.png");
    pStarUpL3->setPosition(ccp(bisaiItem->getContentSize().width/2-23, bisaiItem->getContentSize().height/2+45));
    bisaiItem->addChild(pStarUpL3, 1);
    pStarUpL3->setOpacity(0);
    pStarUpL3->setScale(0.7f);
    CCSequence* seqUL3 = CCSequence::create(CCDelayTime::create(0.8f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.4f),NULL);
    pStarUpL3->runAction(CCRepeatForever::create(seqUL3));
    
    CCSprite* pStarUpR3 = CCSprite::create("charge/chargegamebg1.png");
    pStarUpR3->setPosition(ccp(bisaiItem->getContentSize().width/2+31, bisaiItem->getContentSize().height/2-10));
    bisaiItem->addChild(pStarUpR3, 1);
    pStarUpR3->setOpacity(0);
    pStarUpR3->setScale(0.3f);
    CCSequence* seqUR3 = CCSequence::create(CCDelayTime::create(1.0f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 0.6),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.2f),NULL);
    pStarUpR3->runAction(CCRepeatForever::create(seqUR3));
    
    CCSprite* pSan3 = CCSprite::create("charge/chargegamebg3.png");
    pSan3->setPosition(ccp(bisaiItem->getContentSize().width/2, bisaiItem->getContentSize().height/2+10));
    bisaiItem->addChild(pSan3, 1);
    pSan3->setOpacity(0);
    pSan3->setScale(0.5f);
    ccBlendFunc cbl3 = {GL_SRC_ALPHA, GL_ONE };
    pSan3->setBlendFunc(cbl3);
    CCSequence* seqScale3 = CCSequence::create(CCScaleTo::create(0.56f, 1.06),CCScaleTo::create(0.6f, 1.0),CCScaleTo::create(0.76f, 0.5),CCDelayTime::create(0.08f),NULL);
    CCSequence* seqRotation3 = CCSequence::create(CCRotateBy::create(1.92f, 0),CCDelayTime::create(0.08f),NULL);
    CCSequence* seqOp3 = CCSequence::create(CCFadeTo::create(0.48f, 76.5),CCDelayTime::create(0.88f),CCFadeTo::create(0.56f, 0),CCDelayTime::create(0.08f),NULL);
    CCSpawn* spa3 = CCSpawn::create(seqScale3, seqRotation3, seqOp3, NULL);
    pSan3->runAction(CCRepeatForever::create(spa3));
    
    CCSprite* pSan23 = CCSprite::create("charge/chargegamebg3.png");
    pSan23->setPosition(ccp(bisaiItem->getContentSize().width/2, bisaiItem->getContentSize().height/2+10));
    bisaiItem->addChild(pSan23, 1);
    ccBlendFunc cbl23 = {GL_SRC_ALPHA, GL_ONE };
    pSan23->setBlendFunc(cbl23);
    pSan23->setScale(0.5f);
    pSan23->setOpacity(0);
    CCSequence* seqScale23 = CCSequence::create(CCDelayTime::create(0.12f),CCScaleTo::create(0.64f, 1.07),CCScaleTo::create(1.12, 0.5),CCDelayTime::create(0.12f),NULL);
    CCSequence* seqRotation23 = CCSequence::create(CCRotateBy::create(1.88f, 0),CCDelayTime::create(0.12f),NULL);
    CCSequence* seqOp23 = CCSequence::create(CCDelayTime::create(0.2f),CCFadeIn::create(0.56),CCDelayTime::create(0.68),CCFadeOut::create(0.44f),CCDelayTime::create(0.12),NULL);
    CCSpawn* spa23 = CCSpawn::create(seqScale23,seqRotation23, seqOp23, NULL);
    pSan23->runAction(CCRepeatForever::create(spa23));
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        m_pBisaiMenu->setVisible(false);
    }
    if(CMainLogic::sharedMainLogic()->KIND_ID == FB_KIND_ID)
    {
        m_pBisaiMenu->setVisible(false);
        
        pMenuWnd1->setVisible(false);
        pMenuWnd3->setVisible(false);
        pMenuWnd5->setVisible(false);
        m_pBeibaoMenu -> setVisible(false);
        
        m_pMenuDantou -> setVisible(false);
        pChargeItem->setVisible(false);
    }
    m_pMailMenu->setVisible(false);
    m_pMenuMobile -> setVisible(false);
	schedule(schedule_selector(GameMainScene::update),0.016f);
	this->schedule(schedule_selector(GameMainScene::onFireAndroid), 1.0f);
	this -> schedule(schedule_selector(GameMainScene::createFish),0.01f);
    CMainLogic::sharedMainLogic()->BeforeGameStart();
    CMainLogic::sharedMainLogic()->SendGameOption();
}

CCMenu* GameMainScene::getBisaiMenu()
{
    return m_pBisaiMenu;
}

bool GameMainScene::getInitCompleted()
{
    return m_bInitCompleted;
}

CCSprite* GameMainScene::createSprite(std::string sName, int num)
{
	std::string nameStr = sName + "1.png";
	CCSprite* normalSp = CCSprite::createWithSpriteFrameName(nameStr.c_str());
	CCArray* array = CCArray::create();
	for(int i = 1; i <= num; i++)
	{
		char temp[20];
		sprintf(temp,"%d.png",i);
		std::string name = sName+temp;
		CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		CCSpriteFrame* frame = cache -> spriteFrameByName(name.c_str());
		array-> addObject(frame);
	}
	CCAnimation* animation1 = CCAnimation::createWithSpriteFrames(array,0.2f);
	CCAnimate* animate1 = CCAnimate::create(animation1);
	normalSp->runAction(CCRepeatForever::create(animate1));
	return normalSp;
}

CCMenuItemSprite* GameMainScene::createMenuItem(std::string sName, int num)
{
	std::string nameStr = sName + "1.png";
	CCSprite* normalSp = CCSprite::createWithSpriteFrameName(nameStr.c_str());
	CCArray* array = CCArray::create();
	for(int i = 1; i <= num; i++)
	{
		char temp[20];
		sprintf(temp,"%d.png",i);
		std::string name = sName+temp;
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache() -> spriteFrameByName(name.c_str());
		array-> addObject(frame);
	}
	CCAnimation* animation1 = CCAnimation::createWithSpriteFrames(array,0.2f);
	CCAnimate* animate1 = CCAnimate::create(animation1);
	CCAnimation* animation2 = CCAnimation::createWithSpriteFrames(array,0.2f);
	CCAnimate* animate2 = CCAnimate::create(animation2);
	normalSp->runAction(CCRepeatForever::create(animate1));
	CCSprite* selectSp = CCSprite::createWithSpriteFrameName(nameStr.c_str());
	selectSp->runAction(CCRepeatForever::create(animate2));
	CCMenuItemSprite* menuItem = CCMenuItemSprite::create(normalSp, selectSp);

	return menuItem;
}

void GameMainScene::menuTouchTrue()
{
    CCMenu* pMenu = (CCMenu*)this->getChildByTag(501);
    if (pMenu != NULL)
    {
        pMenu->setTouchEnabled(true);
    }
    if(m_bMenuOpened)
    {
        if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
        {
//            m_pMenuMobile->setVisible(true);//false
        }
        
        m_pMenuDantou -> setVisible(false);
    }
    else
    {
        if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
        {
            if(KIND_ID == NZNH_KIND_ID || KIND_ID == YQS_KIND_ID || KIND_ID == JCBY_KIND_ID)
            {
//                m_pMenuMobile->setVisible(true);
                m_pMenuDantou -> setVisible(true);
            }
            else if(CMainLogic::sharedMainLogic()->KIND_ID == FB_KIND_ID)
            {
                m_pMenuMobile->setVisible(false);
                m_pMenuDantou -> setVisible(false);
            }
        }
        else
        {
            m_pMenuMobile->setVisible(false);
            m_pMenuDantou -> setVisible(false);
        }
        
    }
    m_pMenuMobile->setVisible(false);
}

void GameMainScene::menuTouchFalse()
{
    CCMenu* pMenu = (CCMenu*)this->getChildByTag(501);
    if (pMenu != NULL)
    {
        pMenu->setTouchEnabled(false);
    }
}

void GameMainScene::showMenu()
{
    CCLayer* menuLayer = (CCLayer*)this -> getChildByTag(503);
    if(menuLayer != NULL)
    {
        CCSprite* pSpMenu = (CCSprite*)menuLayer->getChildByTag(999);
        if(pSpMenu != NULL)
        {
            CCMenu* pMenu = (CCMenu*)pSpMenu->getChildByTag(100);
            if(pMenu != NULL)
            {
                pMenu -> setVisible(true);
            }
        }
    }
}

void GameMainScene::menuItemTouchTrue()
{
    CCLayer* menuLayer = (CCLayer*)this -> getChildByTag(503);
    if(menuLayer != NULL)
    {
        CCSprite* pSpMenu = (CCSprite*)menuLayer->getChildByTag(999);
        if(pSpMenu != NULL)
        {
            CCMenu* pMenu = (CCMenu*)pSpMenu->getChildByTag(100);
            if(pMenu != NULL)
            {
                pMenu->setTouchEnabled(true);
            }
        }
    }
    m_bClickMenu = true;
}

void GameMainScene::menuItemTouchFalse()
{
    CCLayer* menuLayer = (CCLayer*)this -> getChildByTag(503);
    if(menuLayer != NULL)
    {
        CCSprite* pSpMenu = (CCSprite*)menuLayer->getChildByTag(999);
        if(pSpMenu != NULL)
        {
            CCMenu* pMenu = (CCMenu*)pSpMenu->getChildByTag(100);
            if(pMenu != NULL)
            {
                pMenu->setTouchEnabled(false);
            }
        }
    }
}

void GameMainScene::showMobileMenu()
{
    CCLayer* menuLayer = (CCLayer*)this -> getChildByTag(503);
    if(menuLayer != NULL)
    {
        CCSprite* pSpMenu = (CCSprite*)menuLayer->getChildByTag(999);
        if(pSpMenu != NULL)
        {
            CCMenu* pMenu = (CCMenu*)pSpMenu->getChildByTag(100);
            if(pMenu != NULL)
            {
                pMenu -> setVisible(false);
            }
        }
    }
}

//各个按钮回调相应方法
void GameMainScene::itemCallBack(CCObject* obj)
{
	CCMenuItem* tempItem = (CCMenuItem*)obj;
	int nItemTag = tempItem->getTag();
    if(nItemTag != 312 && nItemTag != 311 && nItemTag != 323 && nItemTag != 322 && nItemTag != 306 && nItemTag != 310 && nItemTag != 320 && nItemTag != 327 && nItemTag != 332)
    {
        playSoundByID(true, 30);
    }
	
	switch(nItemTag)
	{
    case 300://HideMenu
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameMenu1);
            float fTotalTime = 0.4f;
            menuTouchFalse();
            CCCallFunc* pCallFunc = CCCallFunc::create(this, callfunc_selector(GameMainScene::menuTouchTrue));
            m_bClickMenu = false;
            //m_pMenuMobile -> setVisible(false);
            m_pMenuDantou -> setVisible(false);
            this->runAction(CCSequence::create(CCDelayTime::create(0.4f), pCallFunc, NULL));
            if (m_bMenuOpened)
            {
                CCMenu* pMenu1 = (CCMenu*)this->getChildByTag(501);
                if(pMenu1 != NULL)
                {
                    CCArray* array = pMenu1->getChildren();
                    if(array != NULL && array->count()>=1)
                    {
                        CCMenuItem* item = (CCMenuItem*)array->objectAtIndex(0);
                        if(item != NULL)
                        {
                            item->runAction(CCRotateBy::create(0.15f, 135));
                            CCSprite* sprite = (CCSprite*)item->getChildByTag(100);
                            if(sprite != NULL)
                            {
                                sprite -> setVisible(false);
                            }
                        }
                    }
                }
                m_bMenuOpened = false;
                
                menuItemTouchFalse();
                
                CCLayer* menuLayer = (CCLayer*)this -> getChildByTag(503);
                if(menuLayer != NULL)
                {
                    CCSprite* pSpMenu = (CCSprite*)menuLayer->getChildByTag(999);
                    if(pSpMenu != NULL)
                    {
                        CCMenu* pMenu = (CCMenu*)pSpMenu->getChildByTag(100);
                        if(pMenu != NULL)
                        {
                            CCArray* array = pMenu->getChildren();
                            for(int i = 0; i < array->count(); i++)
                            {
                                CCMenuItem* pMenuItem = (CCMenuItem*)array->objectAtIndex(i);
                                if(pMenuItem != NULL)
                                {
                                    CCPoint ptEnd = ccp(-CLIENT_WIDTH+(m_sizeVisible.width - 50), -10);
                                    CCMoveTo* pMoveTo = CCMoveTo::create(0, ptEnd);
                                    if(pMenuItem->getTag() == 304)//设置
                                    {
                                        pMoveTo = CCMoveTo::create(0.2f, ptEnd);
                                    }
                                    else if(pMenuItem->getTag() == 303)//帮助
                                    {
                                        pMoveTo = CCMoveTo::create(0.3f, ptEnd);
                                    }
                                    else if(pMenuItem->getTag() == 305)//退出
                                    {
                                        pMoveTo = CCMoveTo::create(0.4f, ptEnd);
                                    }
                                    CCCallFunc* pCallFunc1 = CCCallFunc::create(this, callfunc_selector(GameMainScene::menuItemTouchFalse));
                                    CCRotateBy* pRotateBy = CCRotateBy::create(fTotalTime-0.2f, -180);
                                    CCRotateBy* pRotateBy2 = CCRotateBy::create(0.2f, -180);
                                    CCSpawn* pSpawn = CCSpawn::create(pRotateBy2, pMoveTo, NULL);
                                    CCSequence* pSeq = CCSequence::create(pCallFunc1, pRotateBy, pSpawn, NULL);
                                    pMenuItem->runAction(pSeq);
                                }
                            }
                            
                            CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(GameMainScene::showMobileMenu));
                            this->runAction(CCSequence::create(CCDelayTime::create(0.4f), func, NULL));
                        }
                    }
                }
            }
            else
            {
                
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameMenu2);
                m_bMenuOpened = true;
                CCMenu* pMenu1 = (CCMenu*)this->getChildByTag(501);
                if(pMenu1 != NULL)
                {
                    CCArray* array = pMenu1->getChildren();
                    if(array != NULL && array->count()>=1)
                    {
                        CCMenuItem* item = (CCMenuItem*)array->objectAtIndex(0);
                        if(item != NULL)
                        {
                            item->runAction(CCRotateBy::create(0.15f, -135));
                            CCSprite* sprite = (CCSprite*)item->getChildByTag(100);
                            if(sprite != NULL)
                            {
                                sprite -> setVisible(true);
                            }
                        }
                    }
                }
                
                menuItemTouchFalse();
                
                CCLayer* menuLayer = (CCLayer*)this -> getChildByTag(503);
                if(menuLayer != NULL)
                {
                    CCSprite* pSpMenu = (CCSprite*)menuLayer->getChildByTag(999);
                    if(pSpMenu != NULL)
                    {
                        CCMenu* pMenu = (CCMenu*)pSpMenu->getChildByTag(100);
                        if(pMenu != NULL)
                        {
                            float fMoveTime = 0.5f;
                            CCArray* array = pMenu->getChildren();
                            for(int i = 0; i < array->count(); i++)
                            {
                                CCMenuItem* pMenuItem = (CCMenuItem*)array->objectAtIndex(i);
                                if(pMenuItem != NULL)
                                {
                                    CCMoveTo* pMoveTo = CCMoveTo::create(0, CCPointZero);
                                    if(pMenuItem->getTag() == 304)//设置
                                    {
                                        pMoveTo = CCMoveTo::create(fMoveTime, ccp(-CLIENT_WIDTH+(m_sizeVisible.width - 60)-110, -30));
                                    }
                                    else if(pMenuItem->getTag() == 303)//帮助
                                    {
                                        pMoveTo = CCMoveTo::create(fMoveTime, ccp(-CLIENT_WIDTH+(m_sizeVisible.width - 60)-60, -100));
                                    }
                                    else if(pMenuItem->getTag() == 305)//退出
                                    {
                                        pMoveTo = CCMoveTo::create(fMoveTime, ccp(-CLIENT_WIDTH+(m_sizeVisible.width - 60)+20, -125));
                                    }
                                    CCEaseBackOut* pEaseBackOut = CCEaseBackOut::create(pMoveTo);
                                    CCRotateBy* pRotateBy = CCRotateBy::create(fTotalTime, 360);
                                    CCSpawn* pSpawn = CCSpawn::create(pEaseBackOut, pRotateBy, NULL);
                                    CCCallFunc* pCallFun = CCCallFunc::create(this, callfunc_selector(GameMainScene::menuItemTouchTrue));
                                    CCCallFunc* pCallFun2 = CCCallFunc::create(this, callfunc_selector(GameMainScene::showMenu));
                                    CCSequence* pSeq = CCSequence::create(pCallFun2, pSpawn, pCallFun, NULL);
                                    pMenuItem->runAction(pSeq);
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
	case 302://Charge
		{
            if(KIND_ID == FB_KIND_ID)
                break;
            m_bHaveNoScore = false;
            setMatchShowStatus(false);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameCharge);
            if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                ChargeLayer* pChargeLayer = ChargeLayer::create();
                CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeLayer, 11, 20000);
                CMainLogic::sharedMainLogic()->m_nSpeedUpTimes = m_nLastSpeedUp;
                CMainLogic::sharedMainLogic()->m_nBaojiTimes = m_nLastBaoji;
                CMainLogic::sharedMainLogic()->m_nSansheTimes = m_nLastSanshe;
            }
            else//审核未通过
            {
                CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器", eMBExitGame);
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_IAP_PRODUCT_LIST);
            }
            //取消自动开炮
            if(m_bAutoFire)
            {
                this -> unschedule(schedule_selector(GameMainScene::startBullet));
            }
            
			break;
		}
	case 303://点击帮助按钮
		{
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameHelp);
            m_bHaveWnd = true;
			this->setTouchEnabled(false);
			setMenuTouch(false);
            m_fishWndLayer->showHelpSprite();
			break;
		}
	case 304://点击设置按钮
		{
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameSetting);
            SettingLayer* pSettingLayer = SettingLayer::create();
            this -> addChild(pSettingLayer, 200);
            if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER)
            {
                pSettingLayer -> setRotation(180);
            }
            
			break;
		}
	case 305://WndCalc
		{
            if(KIND_ID == FB_KIND_ID)//直接退出，不显示结算框
            {
                CMainLogic::sharedMainLogic()->m_bGameChargeStatus = false;
                CMainLogic::sharedMainLogic()->m_bBuyTryCannon = false;
                exitGame();
                break;
            }
			if(m_bHaveWndCalc)
				return;
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameExit);
            m_bHaveWnd = true;
			m_bHaveWndCalc = true;
			this -> setTouchEnabled(false);
			if(m_bAutoFire)
			{
				this -> unschedule(schedule_selector(GameMainScene::startBullet));
			}
			m_fishWndLayer -> updateCalcWnd(m_nFishType,m_nFishDied,m_nFishCount);
			m_fishWndLayer -> showWndCalcSprite();
			setMenuTouch(false);
			break;
		}
	case 306://加速
		{
            if(m_barrelLayer[m_nMeRealChairID]->getBarrelMult() == 4 || m_barrelLayer[m_nMeRealChairID]->getBarrelMult() == 11)//生化炮
            {
                CMainLogic::sharedMainLogic()->ShowMessage("对不起，当前的炮并不能使用技能。", eMBOK);
                break;
            }
            if(m_bTryBarrelStatus[m_nMeRealChairID])//试用炮期间，不能试用技能
                break;
            if(m_nLastSpeedUp <= 0)
            {
                playSoundByID(true, 30);
                BuyBarrelLayer* pBuyBarrelLayer = BuyBarrelLayer::create(1, 2);
                pBuyBarrelLayer -> setPosition(ccp(0, 0));
                this -> addChild(pBuyBarrelLayer, 100);
                if(m_nMeRealChairID >= GAME_PLAYER/2)
                {
                    pBuyBarrelLayer -> setRotation(180);
                }
                if(m_bAutoFire)
                {
                    this -> unschedule(schedule_selector(GameMainScene::startBullet));
                }
                break;
            }
            if(m_bClickSpeedUp)
            {
                break;
            }
            if(m_bSpeedUp)
            {
                break;
            }
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameSpeedup);
            SimpleAudioEngine::sharedEngine()->playEffect("Music/useskill.wav");
            m_bClickSpeedUp = true;
            CMD_GRR_UseSkill UseSkill = {0};
            UseSkill.cbSkillID = 0;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_SKILL, SUB_GRR_USE_SKILL, &UseSkill, sizeof(UseSkill));
			break;
		}
	case 307:
		{
			break;
		}
	case 308://AutoFire
		{
            if(m_bIsBaojiStatus || m_bClickBaoji)//如果是使用技能期间，则停止对自动开炮的使用
            {
                break;
            }
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameAutoFire);
            m_bClickAutoFire = true;
			CCLayer* menuLayer = (CCLayer*)this -> getChildByTag(504);
			if(menuLayer != NULL)
			{
				CCMenu* pMenuAuto = (CCMenu*)menuLayer -> getChildByTag(103);
				if(pMenuAuto != NULL)
                {
                    CCArray* array = (CCArray*)pMenuAuto->getChildren();
                    for(int i = 0; i < array->count(); i++)
                    {
                        CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
                        if(pItem->getTag() == tempItem->getTag())
                        {
                            CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::menuAction));
                            CCCallFuncN* func2 = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::menuTouchFalse));
                            pItem -> setScale(1.0f);
                            pItem->stopAllActions();
                            pItem->runAction(CCSequence::create(func2, CCScaleTo::create(0.1f, 1.8f), CCScaleTo::create(0.1f, 1.0f), func, NULL));
                            break;
                        }
                    }
                }
			}
			break;
		}
	case 309://CancelAutoFire
		{
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameCancelAutoFire);
            m_bClickAutoFire = false;
			CCLayer* menuLayer = (CCLayer*)this -> getChildByTag(504);
			if(menuLayer != NULL)
			{
				CCMenu* pMenuAuto = (CCMenu*)menuLayer -> getChildByTag(105);
				if(pMenuAuto != NULL)
                {
                    CCArray* array = (CCArray*)pMenuAuto->getChildren();
                    for(int i = 0; i < array->count(); i++)
                    {
                        CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
                        if(pItem->getTag() == tempItem->getTag())
                        {
                            CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::menuAction2));
                            CCCallFuncN* func2 = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::menuTouchFalse2));
                            pItem -> setScale(1.0f);
                            pItem->stopAllActions();
                            pItem->runAction(CCSequence::create(func2, CCScaleTo::create(0.1f, 1.8f), CCScaleTo::create(0.1f, 1.0f), func, NULL));
                            break;
                        }
                    }
                }
			}
			break;
		}
	case 310://Lock
		{
            if(m_bIsSansheStatus)
            {
                break;
            }
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameSuoding);
            playSoundByID(true, 30);
            CCLayer* pMenuLayer = (CCLayer*)this->getChildByTag(504);
            if(pMenuLayer != NULL)
            {
                CCMenu* pMenu = (CCMenu*)pMenuLayer->getChildByTag(106);
                if(pMenu != NULL)
                {
                    CCArray* array = (CCArray*)pMenu->getChildren();
                    for(unsigned int i = 0; i < array->count(); i++)
                    {
                        CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
                        if(pItem->getTag() == tempItem->getTag())
                        {
                            pItem -> setScale(1.0f);
                            pItem->stopAllActions();
                            pItem->runAction(CCSequence::create(CCScaleTo::create(0.1f, 1.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
                        }
                    }
                }
            }
            lockFish2();
			break;
		}
	case 311://加炮		
        {
            if(m_bPaipaileStatus)
                break;
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameJiaMult);
            SimpleAudioEngine::sharedEngine()->playEffect("Music/changebarrel.mp3");
			changeCannonMultiple(1, m_barrelLayer[m_nMeRealChairID]->getBarrelMult()-1);//1加炮，2减炮，0啥也不干（切换炮台）
            
            
			break;
		}
	case 312://减炮
		{
            /*
            CCSprite* pWufuInfo = CCSprite::create("CatchFish2006/wufusprite.png");
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                pWufuInfo -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/3));
                pWufuInfo -> setRotation(180);
            }
            else
            {
                pWufuInfo -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/3*2));
            }
            this -> addChild(pWufuInfo, 98);
            pWufuInfo -> setOpacity(0);
            CCCallFuncN* funcN = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall));
            pWufuInfo -> runAction(CCSequence::create(CCFadeIn::create(1.0f), CCDelayTime::create(2.0f), CCFadeOut::create(1.0f), funcN, NULL));
             */
            if(m_bPaipaileStatus)
                break;
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameJianMult);
            SimpleAudioEngine::sharedEngine()->playEffect("Music/changebarrel.mp3");
			changeCannonMultiple(2, m_barrelLayer[m_nMeRealChairID]->getBarrelMult()-1);
			break;
		}
    case 313://话费卡充值
        {
            break;
        }
    case 314://首充礼包
        {
            
            break;
        }
    case 315://话费兑换
        {
            setMatchShowStatus(false);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameLottery);
            CCHttpRequest* request = new CCHttpRequest();
            request -> setUrl("http://cdn.66y.com/leleclient/LeleExchangeShop.xml");
            request -> setRequestType(CCHttpRequest::kHttpGet);
            request -> setResponseCallback(this, httpresponse_selector(GameMainScene::onHttpLotteryComplete));
            CCHttpClient::getInstance()->send(request);
            CCHttpClient::getInstance()->setTimeoutForConnect(10);
            CCHttpClient::getInstance()->setTimeoutForRead(10);
            request -> release();
            CMainLogic::sharedMainLogic()->ShowMessage("正在打开商城，请稍候...", eMBNull);
            break;
        }
    case 316://喇叭按钮
        {
            break;
        }
    case 321:
        {
        }
    case 322://散射
        {
        }
    case 323://锁定
        {
            if(m_bClickSanshe || m_bIsSansheStatus)//如果点击了散射和使用散射技能期间，那么让锁定不可点击
            {
                CMainLogic::sharedMainLogic()->ShowMessage("对不起，散射与锁定不能同时使用", eMBOK);
                break;
            }
            if(m_bTryBarrelStatus[m_nMeRealChairID])//试用炮期间，不能试用技能
                break;
            if(m_nLastBaoji <= 0)
            {
                playSoundByID(true, 30);
                BuyBarrelLayer* pBuyBarrelLayer = BuyBarrelLayer::create(3, 2);
                pBuyBarrelLayer -> setPosition(ccp(0, 0));
                this -> addChild(pBuyBarrelLayer, 100);
                if(m_nMeRealChairID >= GAME_PLAYER/2)
                {
                    pBuyBarrelLayer -> setRotation(180);
                }
                if(m_bAutoFire)
                {
                    this -> unschedule(schedule_selector(GameMainScene::startBullet));
                }
                break;
            }
            if(m_bClickBaoji)
            {
                break;
            }
            if(m_bIsBaojiStatus)
            {
                break;
            }
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameSuoding);
            SimpleAudioEngine::sharedEngine()->playEffect("Music/useskill.wav");
            m_bClickBaoji = true;
            CMD_GRR_UseSkill UseSkill = {0};
            UseSkill.cbSkillID = 2;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_SKILL, SUB_GRR_USE_SKILL, &UseSkill, sizeof(UseSkill));
            break;
        }
    case 324://试用炮
        {
            break;
        }
    case 326:
        {
            m_spMatchGuide1->setVisible(false);
            m_spMatchGuide2->setVisible(false);
            
            //隐藏5、3、1分钟提示框
            CEnrollRemind* pEnrollRemindLayer = (CEnrollRemind*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(eCmptNotifyTag);
            if(pEnrollRemindLayer != NULL)
            {
                pEnrollRemindLayer -> setVisible(false);
            }
            //隐藏比赛中玩家分数排名更新框
            CCmptStatus* pCmptStatusLayer = (CCmptStatus*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(eCmptStatusTag);
            if(pCmptStatusLayer != NULL)
            {
                pCmptStatusLayer -> setVisible(false);
            }
            
            CCUserDefault::sharedUserDefault()->setBoolForKey("ShowMatchGuide", false);
            //新版本比赛按钮，打开比赛内容
            CRankNewLayer* pRankNewLayer = CRankNewLayer::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(pRankNewLayer, 10, 10200);
            break;
            break;
        }
    case 327://背包按钮
        {
            if(m_bTryBarrelStatus[m_nMeRealChairID])//试用炮期间，不能试用技能
                break;
            if(m_bDantouUsing)
            {
                break;
            }
            m_bSelectDantouNum = false;
            m_bClickDantouMenu = false;
            setMatchShowStatus(false);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameBeibao);
            playSoundByID(true, 30);
            CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器，请稍候", eMBExitGame);
            CMD_GPR_QueryItemCount QueryItemCount;
            QueryItemCount.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_ITEM, SUB_GPR_QUERY_ITEM_COUNT, &QueryItemCount,sizeof(QueryItemCount));
            break;
        }
    case 328://弹头按钮
        {
            m_bSelectDantouNum = false;
            if(m_bTryBarrelStatus[m_nMeRealChairID])//试用炮期间，不能使用弹头
                break;
            m_dtParticle -> setVisible(false);
            setMatchShowStatus(false);
            m_bClickDantouMenu = true;
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameClickDantouTag);
            playSoundByID(true, 30);
            CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器，请稍候", eMBExitGame);
            CMD_GPR_QueryItemCount QueryItemCount;
            QueryItemCount.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_ITEM, SUB_GPR_QUERY_ITEM_COUNT, &QueryItemCount,sizeof(QueryItemCount));
            break;
        }
    case 329://邮件按钮
        {
            MailLayer* pMailLayer = MailLayer::create();
            this -> addChild(pMailLayer, 200, 19998);
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                pMailLayer -> setRotation(180);
            }
            break;
        }
    case 330://聊天按钮
        {
            m_spChatBg -> setVisible(true);
            m_chatBox -> setText("");
            break;
        }
    case 331://发送聊天
        {
            std::string sChat = m_chatBox->getText();
            if(sChat == "")
            {
                CMainLogic::sharedMainLogic()->ShowMessage("请您写下想要发送的内容。", eMBOK);
                break;
            }
            if(sChat.length() > 105)
            {
                CMainLogic::sharedMainLogic()->ShowMessage("对不起，您想要发送的内容有点长，请删减后再试一次。", eMBOK);
                break;
            }
            m_spChatBg -> setVisible(false);
            m_barrelLayer[m_nMeRealChairID]->setShowOneChatBg(true);
            char tempName[64];
            sprintf(tempName, "%s:", CMainLogic::sharedMainLogic()->m_sNickName.c_str());
            char tempData[128];
            sprintf(tempData, "%s %s", tempName, m_chatBox->getText());
            m_barrelLayer[m_nMeRealChairID]->setChatDataName(tempName, tempData);
            //向服务器发送聊天消息
            CMD_C_Chat Chat = {};
            Chat.wChairID = m_nMeRealChairID;
            strncpy(Chat.szContent, CCharCode::UTF8ToGB2312(sChat.c_str()).c_str(), sizeof(Chat.szContent));
            m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_CHAT, &Chat, sizeof(Chat));
            //启动定时器
            m_barrelLayer[m_nMeRealChairID] -> scheduleOnce(schedule_selector(BarrelLayer::hideChatBg), 5.0f);
            break;
        }
    case 332://拍拍乐次数
        {
            m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_PPL_PUSH);
            //隐藏小手图标
            SimpleAudioEngine::sharedEngine()->playEffect("Music/paianxia.wav");
            m_nOutTime = 60;
            m_labelTTFReCount->setVisible(false);
            m_spReCount->setVisible(false);
            break;
        }
    case 333://五福按钮
        {
            m_bSelectDantouNum = false;
            m_dtParticle -> setVisible(false);
            setMatchShowStatus(false);
            CMainLogic::sharedMainLogic()->ShowMessage("正在查询五福卡，请稍候", eMBGameNull);
            CMD_GPR_WufuCard WufuCard = {0};
            WufuCard.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_WUFU_CARD, &WufuCard,sizeof(WufuCard));
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameClickWufu);
            break;
        }
    case 334://在线奖励按钮
        {
            if(m_nOnlineTime == 0)//可以直接领奖
            {
                CMainLogic::sharedMainLogic()->ShowMessage("正在领取，请稍候", eMBGameNull);
                m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_ONLINE_REWARD_GET);
            }
            else
            {
                CMainLogic::sharedMainLogic()->m_bAutoCheckOnlineInfo = false;
                setMatchShowStatus(false);
                CMainLogic::sharedMainLogic()->ShowMessage("正在查询，请稍候", eMBGameNull);
                m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_ONLINE_REWARD_INFO);
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eOnlineRequest);
                
            }
            break;
        }
	default:
		break;
	}
}

void GameMainScene::showPaiRemindBg()
{
    m_nRemindNum--;
    
    if(m_nRemindNum <= 0)
    {
        m_spRemindBg -> setVisible(false);
        this -> unschedule(schedule_selector(GameMainScene::showPaiRemindBg));
        for(int i = 0; i < GAME_PLAYER; i++)
        {
            if(m_bPPLChairID[i])
            {
                startPaiZhuanUI(i);//显示开始界面，几位玩家中奖就显示几个界面
                break;
            }
        }
        return ;
    }
    char temp[8];
    sprintf(temp, "%d", m_nRemindNum);
    m_labelRemind -> setString(temp);
}

void GameMainScene::startPaiZhuanUI(int nChairID)
{
    SimpleAudioEngine::sharedEngine()->playEffect("Music/paizhuan.wav");
    //要保证转到自己的一定要在最上边
    int nOrder = 18;
    if(nChairID == m_nMeRealChairID)
    {
        nOrder = 19;
    }
    CCNode* paiNode = CCNode::create();
    this -> addChild(paiNode, nOrder, ePaiNodeTag+nChairID);
    if(m_nMeRealChairID >= GAME_GENRE/2)
    {
        paiNode -> setRotation(180);
        paiNode -> setPosition(ccp(CLIENT_WIDTH, CLIENT_HEIGHT));
    }
    CCSprite* paipaizi = CCSprite::create("CatchFish2006/paipanzi.png");
    paipaizi -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    paiNode -> addChild(paipaizi, 1, 1);
    CCSprite* paiguang1 = CCSprite::create("CatchFish2006/panguang1.png");
    paiguang1 -> setPosition(ccp(paipaizi->getContentSize().width/2, paipaizi->getContentSize().height/2));
    paipaizi -> addChild(paiguang1);
    CCSprite* paiguang2 = CCSprite::create("CatchFish2006/panguang2.png");
    paiguang2 -> setPosition(ccp(paipaizi->getContentSize().width/2, paipaizi->getContentSize().height/2));
    paipaizi -> addChild(paiguang2);
    paiguang1 -> runAction(CCRepeatForever::create(CCSequence::create(CCHide::create(), CCDelayTime::create(0.5f), CCShow::create(), CCDelayTime::create(0.5f), NULL)));
    paiguang2 -> runAction(CCRepeatForever::create(CCSequence::create(CCShow::create(), CCDelayTime::create(0.5f), CCHide::create(), CCDelayTime::create(0.5f), NULL)));
    CCSprite* paijinchan = CCSprite::create("CatchFish2006/paijinchan2.png");
    paijinchan -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    paiNode -> addChild(paijinchan, 1, 2);
    CCSprite* paijinchan1 = CCSprite::create("CatchFish2006/paijinchan1.png");
    paijinchan1 -> setPosition(ccp(paijinchan->getContentSize().width/2, paijinchan->getContentSize().height/2));
    paijinchan -> addChild(paijinchan1, 1, 2);
    CCSprite* paiguang = CCSprite::createWithSpriteFrameName("h3001.png");
    ccBlendFunc cbl = {GL_ONE_MINUS_DST_COLOR, GL_ONE};
    paiguang -> setBlendFunc(cbl);
    paiguang -> setScaleX(0.8f);
    paiguang -> setScaleY(8.0f);
    CCArray* lineArray = CCArray::create();
    char temp[32];
    for(int i = 1; i <= 10; i++)
    {
        sprintf(temp,"h300%d.png",i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
        lineArray -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(lineArray,0.15f);
    animation->setLoops(-1);
    CCAnimate* animate = CCAnimate::create(animation);
    paiguang -> runAction(CCRepeatForever::create(animate));
    paiguang -> setPosition(ccp(paijinchan->getContentSize().width/2, paijinchan->getContentSize().height/2));
    paiguang -> setAnchorPoint(ccp(0,0.5f));
    paijinchan -> addChild(paiguang, -1);
    CCMenuItemImage* paiItem = CCMenuItemImage::create("CatchFish2006/paiitem.png", "CatchFish2006/paiitem.png", this, menu_selector(GameMainScene::itemCallBack));
    paiItem -> setTag(332);
    paiItem -> setPosition(ccp(CLIENT_WIDTH/2+225, CLIENT_HEIGHT/2));
    CCMenu* paiMenu = CCMenu::create(paiItem, NULL);
    paiMenu -> setPosition(CCPointZero);
    paiMenu -> setAnchorPoint(CCPointZero);
    paiNode -> addChild(paiMenu, 2, 3);
    paiMenu -> setVisible(false);
    CCSprite* huangSprite = CCSprite::create("CatchFish2006/huangguang.png");
    huangSprite -> setPosition(ccp(CLIENT_WIDTH/2+225, CLIENT_HEIGHT/2));
    paiNode -> addChild(huangSprite, 3, 6);
    huangSprite -> setVisible(false);
    huangSprite -> setScale(0.8f);
    CCSprite* shou = CCSprite::createWithSpriteFrameName("jinchanshou1.png");
    shou -> setPosition(ccp(CLIENT_WIDTH/2-10+225, CLIENT_HEIGHT/2+10));
    shou -> setAnchorPoint(ccp(0, 1));
    paiNode -> addChild(shou, 4, 5);
    shou -> setVisible(false);
    CCArray* array1 = CCArray::create();
    for(int i = 1; i <= 2; i++)
    {
        sprintf(temp, "jinchanshou%d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
        array1 -> addObject(frame);
    }
    CCAnimation* animation1 = CCAnimation::createWithSpriteFrames(array1, 0.2f);
    CCAnimate* animate1 = CCAnimate::create(animation1);
    shou -> runAction(CCRepeatForever::create(animate1));
    
    CCLabelAtlas* pDaojishi = CCLabelAtlas::create("3", "CatchFish2006/remindnum.png", 33, 40, '0');
    pDaojishi -> setAnchorPoint(ccp(0.5f, 0.5f));
    pDaojishi -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+170));
    paiNode -> addChild(pDaojishi, 2, 4);
    pDaojishi -> setScale(2.0f);
    pDaojishi -> setVisible(false);
    
    int rotation1 = 0;
    int rotation2 = 0;
    if(m_nMeRealChairID >= GAME_PLAYER/2)
    {
        if(nChairID == 0)
        {
            rotation1 = 152;
            rotation2 = 305;
        }
        else if(nChairID == 1)
        {
            rotation1 = 120;
            rotation2 = 240;
        }
        else if(nChairID == 2)
        {
            rotation1 = 62;
            rotation2 = 125;
        }
        else if(nChairID == 3)
        {
            rotation1 = 30;
            rotation2 = 60;
        }
    }
    else
    {
        if(nChairID == 0)
        {
            rotation1 = 62;
            rotation2 = 125;
        }
        else if(nChairID == 1)
        {
            rotation1 = 30;
            rotation2 = 60;
        }
        else if(nChairID == 2)
        {
            rotation1 = 152;
            rotation2 = 305;
        }
        else if(nChairID == 3)
        {
            rotation1 = 120;
            rotation2 = 240;
        }
    }
    
    CCEaseSineInOut* pRep1 = CCEaseSineInOut::create(CCRotateTo::create(6, -360*5-rotation1));
    pRep1 -> setTag(101);
    paipaizi -> runAction(pRep1);
    CCEaseSineInOut* pRep2 = CCEaseSineInOut::create(CCRotateTo::create(6, 360*10+rotation2));
    pRep2 -> setTag(102);
    paijinchan -> runAction(pRep2);
    
    //检查接下来是否继续转动转盘
    if(nChairID < GAME_PLAYER)
    {
        for(int i = nChairID+1; i < GAME_PLAYER; i++)
        {
            if(m_bPPLChairID[i])
            {
                CCSprite* sprite = CCSprite::create();
                this -> addChild(sprite);
                CCCallFuncND* funcND = CCCallFuncND::create(this, callfuncND_selector(GameMainScene::showPaiPaiLeUI2), (void*)(long)i);
                CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall));
                sprite -> runAction(CCSequence::create(CCDelayTime::create(6.5f), funcND, func, NULL));
                break;
            }
        }
    }
    CCSprite* sprite = CCSprite::create();
    this -> addChild(sprite);
    CCCallFuncND* funcND = CCCallFuncND::create(this, callfuncND_selector(GameMainScene::showPaiPaiLeUI3), (void*)(long)nChairID);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall));
    sprite -> runAction(CCSequence::create(CCDelayTime::create(6.5f), funcND, func, NULL));
}

void GameMainScene::showPaiPaiLeUI2(CCNode* node, void* chairID)
{
    long nChairID = (long)chairID;
    startPaiZhuanUI((int)nChairID);//显示转动
}

void GameMainScene::showPaiPaiLeUI3(CCNode* node, void* chairID)
{
    long nChairID = (long)chairID;
    
    //显示停止
    if(m_nMeRealChairID == nChairID)
    {
        SimpleAudioEngine::sharedEngine()->playEffect("Music/paizhuanstop.mp3");
        FirstChargeLayer* pLayer = FirstChargeLayer::create(12);
        CCDirector::sharedDirector()->getRunningScene()->addChild(pLayer, 19, 29999);
        SimpleAudioEngine::sharedEngine()->playEffect("Music/paizhongjiang.mp3");
    }
    //判断是否是最后一个玩家
    for(int i = GAME_PLAYER-1; i >= 0; i--)
    {
        if(m_bPPLChairID[i])
        {
            if(i == nChairID)
            {
                showPaiPaiLeUI();
            }
        }
    }
}

void GameMainScene::showPaiPaiLeUI()//所有玩家都执行一遍，最后统一执行
{
    m_bPPLZhuanComplete = true;
    for(int i = 0; i < GAME_PLAYER; i++)
    {
        if(i == m_nMeRealChairID)
        {
            if(!m_bPPLChairID[i])//没有中奖
            {
                SimpleAudioEngine::sharedEngine()->playEffect("Music/paishibai.wav");
                SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
                //如果收到了取消消息，那么移除拍拍乐界面
                m_bIsPaipaileStatus = false;
                m_bPaipaileStatus = false;
            }
        }
    }
    for(int i = 0; i < GAME_PLAYER; i++)
    {
        if(m_bPPLChairID[i])
        {
            if(m_bPPLReceiveCancel[i])
            {
                //移除界面
                CCNode* paiNode = (CCNode*)this->getChildByTag(ePaiNodeTag+i);
                if(paiNode != NULL)
                {
                    CCSprite* paiguang = (CCSprite*)paiNode->getChildByTag(6);
                    if(paiguang != NULL)
                    {
                        paiguang -> setVisible(false);
                        paiguang -> stopAllActions();
                    }
                    CCArray* array = (CCArray*)paiNode->getChildren();
                    for(int i = 0; i < array->count(); i++)
                    {
                        CCSprite* sprite = (CCSprite*)array->objectAtIndex(i);
                        if(sprite != NULL)
                        {
                            if(sprite->getTag() == 1 || sprite->getTag() == 2)
                            {
                                sprite -> runAction(CCSpawn::create(CCRotateBy::create(3.0f, 360*6), CCScaleTo::create(3.0f, 0.1f), NULL));
                            }
                            else
                            {
                                sprite->setVisible(false);
                            }
                        }
                    }
                    paiNode -> runAction(CCSequence::create(CCDelayTime::create(3.0f), CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall)), NULL));
                }
            }
        }
    }
}

void GameMainScene::setPaiItemEnable(bool bEnable)//点击准备，开启3秒倒计时
{
    m_bIsPaipaileStatus = true;
    m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_PPL_CONFIRM);
    //开启三秒定时器
    this -> schedule(schedule_selector(GameMainScene::showPaiDaojishi), 1.0f);
    this -> scheduleOnce(schedule_selector(GameMainScene::playPaiDaojishi), 1.0f);
    CCNode* paiNode = (CCNode*)this->getChildByTag(ePaiNodeTag+m_nMeRealChairID);
    if(paiNode != NULL)
    {
        CCLabelAtlas* pLabel = (CCLabelAtlas*)paiNode->getChildByTag(4);
        if(pLabel != NULL)
        {
            pLabel -> setVisible(true);
        }
        CCMenu* paiMenu = (CCMenu*)paiNode->getChildByTag(3);
        if(paiMenu != NULL)
        {
            paiMenu -> setVisible(bEnable);
            paiMenu -> setTouchEnabled(false);
        }
    }
}

void GameMainScene::playPaiDaojishi()//3秒倒计时，结束时，转动
{
    SimpleAudioEngine::sharedEngine()->playEffect("Music/paiselectdaojishi.mp3");
}

void GameMainScene::showPaiDaojishi()//3秒倒计时，结束时，转动
{
    CCNode* paiNode = (CCNode*)this->getChildByTag(ePaiNodeTag+m_nMeRealChairID);
    if(paiNode != NULL)
    {
        CCLabelAtlas* pLabel = (CCLabelAtlas*)paiNode->getChildByTag(4);
        if(pLabel != NULL)
        {
            int num = atoi(pLabel->getString());
            num--;
            if(num <= 0)
            {
                SimpleAudioEngine::sharedEngine()->playEffect("Music/paipai2.mp3");
                pLabel -> setVisible(false);
                CCMenu* paiMenu = (CCMenu*)paiNode->getChildByTag(3);
                if(paiMenu != NULL)
                {
                    paiMenu -> setTouchEnabled(true);
                }
                this -> unschedule(schedule_selector(GameMainScene::showPaiDaojishi));
                
                CCSprite* shou = (CCSprite*)paiNode->getChildByTag(5);
                if(shou != NULL)
                {
                    shou -> setVisible(true);
                }
                CCSprite* huangguang = (CCSprite*)paiNode->getChildByTag(6);
                if(huangguang != NULL)
                {
                    huangguang -> setVisible(true);
                    huangguang -> runAction(CCRepeatForever::create(CCBlink::create(2.0f, 5)));
                }
            }
            else
            {
                char temp[8];
                sprintf(temp, "%d", num);
                pLabel -> setString(temp);
            }
        }
    }
}

void GameMainScene::paipaileGameOver(int nScore)
{
    CCSprite* sprite = CCSprite::create("CatchFish2006/paicalcbg.png");
    sprite -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(sprite, 52);
    if(m_nMeRealChairID >= GAME_PLAYER/2)
    {
        sprite -> setRotation(180);
    }
    char temp[16];
    sprintf(temp, "%d", nScore);
    CCLabelAtlas* pLabel = CCLabelAtlas::create(temp, "CatchFish2006/paicalcnum.png", 40, 57, '0');
    pLabel -> setPosition(ccp(sprite->getContentSize().width/2, 165));
    pLabel -> setAnchorPoint(ccp(0.5f, 0.5f));
    sprite -> addChild(pLabel);
    CCCallFuncND* funcND = CCCallFuncND::create(this, callfuncND_selector(GameMainScene::showBarrelPlate), (void*)(long)nScore);
    CCCallFuncN* func2 = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall));
    sprite -> runAction(CCSequence::create(CCDelayTime::create(3.0f), funcND, func2, NULL));
}

void GameMainScene::showBarrelPlate(CCNode* pNode, void* nScore)
{
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    long nPaiScore = (long)nScore;
    if(nPaiScore != 0)
    {
        setPlate(m_nMeRealChairID, (int)nPaiScore, 12, true);
    }
}

void GameMainScene::onHttpLotteryComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    
    LotteryLayer* pLotteryLayer = LotteryLayer::create(false, CMainLogic::sharedMainLogic()->m_nLottery);
    this->addChild(pLotteryLayer, 200, 19989);
    if(m_nMeRealChairID >= GAME_PLAYER/2)
    {
        pLotteryLayer -> setRotation(180);
    }
    pLotteryLayer -> parseData(str);
    
    if(m_bAutoFire)
    {
        this -> unschedule(schedule_selector(GameMainScene::startBullet));
    }
    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
}

void GameMainScene::cancelSpeedupStatus(CCNode* node)
{
    if(node != NULL)
    {
        node -> removeFromParent();
        cancelSpeedUpFire();
        if(m_bIsBaojiStatus == false && m_bIsSansheStatus == false)
        {
            m_pSpBlue -> runAction(CCFadeTo::create(2.0f, 0));
        }
        if(m_nLastSpeedUp > 0)
        {
            speedSp1 -> setVisible(true);
        }
    }
}

void GameMainScene::cancelSansheStatus(CCNode* node)
{
    if(node != NULL)
    {
        node -> removeFromParent();
        m_bIsSansheStatus = false;
        if(m_bIsBaojiStatus == false && m_bSpeedUp == false)
        {
            m_pSpBlue -> runAction(CCFadeTo::create(2.0f, 0));
        }
        if(m_nLastSanshe > 0)
        {
            speedSp2 -> setVisible(true);
        }
    }
}

void GameMainScene::cancelBaojiStatus(CCNode* node)
{
    if(node != NULL)
    {
        node -> removeFromParent();
        if(m_bIsBaojiStatus)
        {
            m_bIsBaojiStatus = false;
            removeLockLine(m_nMeRealChairID);
            m_bAutoFire = false;
            this -> unschedule(schedule_selector(GameMainScene::startBullet));
            for(int i = 0; i < m_arrLockFish->count(); i++)
            {
                CCSprite* pLockDi = (CCSprite*)m_arrLockFish->objectAtIndex(i);
                if(pLockDi == NULL) continue;
                pLockDi -> setVisible(false);
            }
        }
        
        if(m_bIsSansheStatus == false && m_bSpeedUp == false)
        {
            m_pSpBlue -> runAction(CCFadeTo::create(2.0f, 0));
        }
        
        if(m_nLastBaoji > 0)
        {
            speedSp3 -> setVisible(true);
        }
    }
}

void GameMainScene::hideLabelAnimate(CCNode* node)
{
    node->runAction(CCSequence::create(CCFadeOut::create(1.0f), CCHide::create(), NULL));
}

void GameMainScene::showZengSkillAnimate(int nSkillID)
{
    CCCallFuncN* call = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
    if(nSkillID == 0)//加速
    {
        CCSprite* sprite = CCSprite::create("CatchFish2001/speedup1.png");
        this -> addChild(sprite, 100);
        if(m_nMeRealChairID >= GAME_PLAYER/2)
        {
            sprite -> setRotation(180);
            sprite -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-100));
            sprite -> runAction(CCSequence::create(CCMoveTo::create(0.27f, ccp(60, 720-330)), call, NULL));//430
        }
        else
        {
            sprite -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+100));
            sprite -> runAction(CCSequence::create(CCMoveTo::create(0.27f, ccp(1220, 330)), call, NULL));//430
        }
       
    }
    else if(nSkillID == 2)//锁定
    {
        CCSprite* sprite = CCSprite::create("CatchFish2001/suodingsp2.png");
        this -> addChild(sprite, 100);
        if(m_nMeRealChairID >= GAME_PLAYER/2)
        {
            sprite -> setRotation(180);
            sprite -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-100));
            sprite -> runAction(CCSequence::create(CCMoveTo::create(0.35f, ccp(60, 720-230)), call, NULL));
        }
        else
        {
            sprite -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+100));
            sprite -> runAction(CCSequence::create(CCMoveTo::create(0.35f, ccp(1220, 230)), call, NULL));
        }
    }
}

//系统赠送技能
void GameMainScene::showSkillStatus(int nSkillID, int nCount)
{
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
        return;
    if(nSkillID == 0)//系统赠送加速技能
    {
        m_nLastSpeedUp = nCount;
        char temp[16];
        sprintf(temp, "%d", m_nLastSpeedUp);
        m_labelSpeedUp->setString(temp);
        if(m_nLastSpeedUp>0)
        {
            speedSp1->setVisible(true);
        }
        CCSprite* pParent = (CCSprite*)m_labelSpeedUp->getParent();
        if(pParent != NULL)
        {
            pParent -> runAction(CCRepeat::create(CCSequence::create(CCScaleTo::create(0.5f, 2.0f), CCScaleTo::create(0.5f, 1.0f), NULL), 3));
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        SimpleAudioEngine::sharedEngine()->playEffect("Music/openbox.mp3");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		SimpleAudioEngine::sharedEngine()->playEffect("Music/openbox.ogg");
#endif
        TryBarrelLayer* pBuyBarrelLayer = TryBarrelLayer::create(1);
        pBuyBarrelLayer -> setPosition(ccp(0, 0));
        this -> addChild(pBuyBarrelLayer, 100);
        if(m_nMeRealChairID >= GAME_PLAYER/2)
        {
            pBuyBarrelLayer -> setRotation(180);
        }
    }
    else if(nSkillID == 1)//系统赠送散射技能
    {
    }
    else if(nSkillID == 2)//系统赠送锁定技能
    {
        m_nLastBaoji = nCount;
        char temp[16];
        sprintf(temp, "%d", m_nLastBaoji);
        m_labelBaoji->setString(temp);
        if(m_nLastBaoji>0)
        {
            speedSp3->setVisible(true);
        }
        CCSprite* pParent = (CCSprite*)m_labelBaoji->getParent();
        if(pParent != NULL)
        {
            pParent -> runAction(CCRepeat::create(CCSequence::create(CCScaleTo::create(0.5f, 2.0f), CCScaleTo::create(0.5f, 1.0f), NULL), 3));
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        SimpleAudioEngine::sharedEngine()->playEffect("Music/openbox.mp3");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		SimpleAudioEngine::sharedEngine()->playEffect("Music/openbox.ogg");
#endif
        TryBarrelLayer* pBuyBarrelLayer = TryBarrelLayer::create(3);
        pBuyBarrelLayer -> setPosition(ccp(0, 0));
        this -> addChild(pBuyBarrelLayer, 100);
        if(m_nMeRealChairID >= GAME_PLAYER/2)
        {
            pBuyBarrelLayer -> setRotation(180);
        }
    }
}

void GameMainScene::setSkillStatus(int nSkillID, int nCount)
{
    if(nSkillID == 0)//使用加速技能
    {
        m_nLastSpeedUp = nCount;
        CMainLogic::sharedMainLogic()->m_nSpeedUpTimes = nCount;
        m_bSpeedUp = true;
        m_bClickSpeedUp = false;
        char temp[16];
        sprintf(temp, "%d", nCount);
        m_labelSpeedUp -> setString(temp);
        speedSp1->setVisible(false);
        m_pSpBlue -> stopAllActions();
        m_pSpBlue -> runAction(CCFadeTo::create(1.0f, 127));
        m_spSpeedUp -> setVisible(true);
        speedupCD -> setVisible(true);
        float fDelayTime = 15.0f;
        CCProgressTo *progressToAction = CCProgressTo::create(fDelayTime, 100.0f);
        speedupCD->runAction(progressToAction);
        CCSprite* sprite = CCSprite::create();
        this -> addChild(sprite);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::cancelSpeedupStatus));
        sprite -> runAction(CCSequence::create(CCDelayTime::create(fDelayTime), func, NULL));
        speedUpFire();
        m_speedupAnimate->setOpacity(255);
        m_speedupAnimate->setVisible(true);
        if(m_nMeRealChairID >= GAME_PLAYER/2)
        {
            m_speedupAnimate -> setRotation(180);
            m_speedupAnimate -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT-400));
        }
        char tempSpeedUp[32];
        CCArray* speedupArray = CCArray::create();
        for(int i = 1; i <= 11; i++)
        {
            sprintf(tempSpeedUp, "speeduplabel%d.png", i);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(tempSpeedUp);
            speedupArray -> addObject(frame);
        }
        CCAnimation* speedupAnimation = CCAnimation::createWithSpriteFrames(speedupArray, 0.1f);
        CCAnimate* speedupAnimate = CCAnimate::create(speedupAnimation);
        CCCallFuncN* hideFunc = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::hideLabelAnimate));
        m_speedupAnimate -> runAction(CCSequence::create(speedupAnimate, hideFunc, NULL));
        
    }
    else if(nSkillID == 1)//使用散射技能
    {
    }
    else if(nSkillID == 2)//使用锁定技能
    {
        CMainLogic::sharedMainLogic()->m_nBaojiTimes = nCount;
        m_nLastBaoji = nCount;
        m_bClickBaoji = false;
        char temp[16];
        sprintf(temp, "%d", m_nLastBaoji);
        m_labelBaoji->setString(temp);
        speedSp3->setVisible(false);
        m_bIsBaojiStatus = true;
        m_pSpBlue -> stopAllActions();
        m_pSpBlue -> runAction(CCFadeTo::create(1.0f, 127));
        //m_pSpBlue -> setOpacity(255);
        //m_pSpBlue -> setVisible(true);
        m_spBaoji -> setVisible(true);
        baojiCD -> setVisible(true);
        float fDelayTime = 15.0f;
        CCProgressTo *progressToAction = CCProgressTo::create(fDelayTime, 100.0f);
        baojiCD->runAction(progressToAction);
        CCSprite* sprite = CCSprite::create();
        this -> addChild(sprite);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::cancelBaojiStatus));
        sprite -> runAction(CCSequence::create(CCDelayTime::create(fDelayTime), func, NULL));
        m_baojiAnimate -> setOpacity(255);
        m_baojiAnimate -> setVisible(true);
        if(m_nMeRealChairID >= GAME_PLAYER/2)
        {
            m_baojiAnimate -> setRotation(180);
            m_baojiAnimate -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT-400));
        }
        char tempbaojilabel[32];
        CCArray* baojilabelArray = CCArray::create();
        for(int i = 1; i <= 11; i++)
        {
            sprintf(tempbaojilabel, "baojilabel%d.png", i);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(tempbaojilabel);
            baojilabelArray -> addObject(frame);
        }
        CCAnimation* baojilabelAnimation = CCAnimation::createWithSpriteFrames(baojilabelArray, 0.1f);
        CCAnimate* baojilabelAnimate = CCAnimate::create(baojilabelAnimation);
        CCCallFuncN* hideFunc = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::hideLabelAnimate));
        m_baojiAnimate -> runAction(CCSequence::create(baojilabelAnimate, hideFunc, NULL));
        
        //如果是自动开炮，那么先关闭自动开炮，然后再停止对自动开炮按钮的触摸
        if(m_bAutoFire)
        {
            CCMenuItem* item = CCMenuItem::create();
            item -> setTag(309);
            itemCallBack(item);
        }
        
        for(int i = 0; i < m_arrLockFish->count(); i++)
        {
            CCSprite* pLockDi = (CCSprite*)m_arrLockFish->objectAtIndex(i);
            if(pLockDi == NULL) continue;
            pLockDi -> setVisible(true);
        }
        
        //寻找记忆锁定 寻找锁定ID
        if(m_nHistoryLockFishID != 0)
        {
            for(int i = 0; i < m_vctFishFactory.size(); i++)
            {
                GameFish* pGameFish = m_vctFishFactory[i];
                if(pGameFish == NULL || pGameFish->getDied() || pGameFish->getAppear())
                    continue;
                if(pGameFish->getFishID() == m_nHistoryLockFishID)
                {
                    //发送锁定消息
                    m_barrelLayer[m_nMeRealChairID]->lockRealFish(pGameFish);
                    sendLockFishID(pGameFish->getFishID(),m_nMeRealChairID);
                    m_barrelLayer[m_nMeRealChairID]->setLockFishID(pGameFish->getFishID());
                    m_nLockFishID[m_nMeRealChairID] = pGameFish->getFishID();
                    m_bLockFishID[m_nMeRealChairID] = true;
                    m_bUserLock = true;
                    if(!m_bAutoFire)//锁定到鱼之后，开启自动开炮
                    {
                        m_bAutoFire = true;
                        this->schedule(schedule_selector(GameMainScene::startBullet), SPEED_NORMAL);
                        this -> schedule(schedule_selector(GameMainScene::checkAutoFire), 1.0f);
                    }
                    break;
                }
            }
        }
    }
}

void GameMainScene::setDantouNum(int nTongDantou, int nYinDantou, int nJinDantou)
{
    m_nDantouTotalNum = nTongDantou + nYinDantou + nJinDantou;
    if(m_nDantouTotalNum != 0)
    {
        m_spDantouDi -> setVisible(true);
        char temp[16];
        sprintf(temp, "%d", m_nDantouTotalNum);
        m_labelDantou -> setString(temp);
    }
}

void GameMainScene::setWufuNum(int nCard[5])
{
    m_nDantouTotalNum = nCard[0]+nCard[1]+nCard[2]+nCard[3]+nCard[4];
    if(m_nDantouTotalNum > 0)
    {
        m_spDantouDi -> setVisible(true);
        char temp[16];
        sprintf(temp, "%d", m_nDantouTotalNum);
        m_labelDantou -> setString(temp);
    }
    else
    {
        m_spDantouDi -> setVisible(false);
    }
}

void GameMainScene::menuTouchFalse(CCNode* node)
{
    CCLayer* menuLayer = (CCLayer*)this -> getChildByTag(504);
    if(menuLayer != NULL)
    {
        CCMenu* pMenuAuto = (CCMenu*)menuLayer -> getChildByTag(103);
        if(pMenuAuto != NULL)
        {
            
            pMenuAuto->setTouchEnabled(false);
        }
    }
}

void GameMainScene::menuTouchFalse2(CCNode* node)
{
    CCLayer* menuLayer = (CCLayer*)this -> getChildByTag(504);
    if(menuLayer != NULL)
    {
        CCMenu* pMenuAuto = (CCMenu*)menuLayer -> getChildByTag(105);
        if(pMenuAuto != NULL)
        {
            pMenuAuto->setVisible(true);
            pMenuAuto->setTouchEnabled(false);
        }
    }
}

void GameMainScene::menuAction(CCNode* node)
{
    node->setVisible(false);
    CCLayer* menuLayer = (CCLayer*)this -> getChildByTag(504);
    if(menuLayer != NULL)
    {
        CCMenu* pMenuAuto = (CCMenu*)menuLayer -> getChildByTag(105);
        if(pMenuAuto != NULL)
        {
            CCArray* array = (CCArray*)pMenuAuto->getChildren();
            for(int i = 0; i < array->count(); i++)
            {
                CCMenuItem* item = (CCMenuItem*)array->objectAtIndex(i);
                item->setVisible(true);
            }
            pMenuAuto->setVisible(true);
            pMenuAuto->setTouchEnabled(true);
        }
    }
    if(!m_bAutoFire)
    {
        
        this -> schedule(schedule_selector(GameMainScene::checkAutoFire), 1.0f);
    }
    if(!m_bIsBaojiStatus)
    {
        autoFire();
    }
}

void GameMainScene::menuAction2(CCNode* node)
{
    node->setVisible(false);
    CCLayer* menuLayer = (CCLayer*)this -> getChildByTag(504);
    if(menuLayer != NULL)
    {
        CCMenu* pMenuAuto = (CCMenu*)menuLayer -> getChildByTag(103);
        if(pMenuAuto != NULL)
        {
            CCArray* array = (CCArray*)pMenuAuto->getChildren();
            for(int i = 0; i < array->count(); i++)
            {
                CCMenuItem* item = (CCMenuItem*)array->objectAtIndex(i);
                item->setVisible(true);
            }
            pMenuAuto->setVisible(true);
            pMenuAuto->setTouchEnabled(true);
        }
    }
    if(!m_bIsBaojiStatus)
    {
        autoFire();
    }
}

//加速、取消加速
void GameMainScene::speedUpFire()
{
    m_bSpeedUp = true;
	if(m_bSpeedUp)
	{
		if(m_bAutoFire  || m_bTouch)
		{
			this -> unschedule(schedule_selector(GameMainScene::startBullet));
			this -> schedule(schedule_selector(GameMainScene::startBullet),SPEED_NORMAL);//SPEED_UP
		}
	}
}

void GameMainScene::cancelSpeedUpFire()
{
    m_bSpeedUp = false;
    if(m_bAutoFire || m_bTouch)
    {
        this -> unschedule(schedule_selector(GameMainScene::startBullet));
        this -> schedule(schedule_selector(GameMainScene::startBullet),SPEED_NORMAL);
    }
}

//初始化炮台层
void GameMainScene::loadBarrel()
{
	for(int i = GAME_PLAYER/2; i < GAME_PLAYER; i++)
	{
		m_barrelLayer[i] = BarrelLayer::create(i);
        m_barrelLayer[i] -> setPosition(ccp(220-440*(i-2),0));
		m_barrelLayer[i] -> setRotation(180);
		this -> addChild(m_barrelLayer[i],26-i);
		m_barrelLayer[i]->setVisible(false);
	}
	for(int i = 0; i < GAME_PLAYER/2; i++)
	{
		m_barrelLayer[i] = BarrelLayer::create(i);
		m_barrelLayer[i] -> setPosition(ccp(-220+440*i,0));
		this -> addChild(m_barrelLayer[i],20);
		m_barrelLayer[i]->setVisible(false);
	}
}

void GameMainScene::addSpriteBatch()
{
	CCSpriteBatchNode* batchNode = NULL;
    if(KIND_ID == YQS_KIND_ID)
    {
        batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/yaoqianshu1.png");
        m_fishLayer -> addChild(batchNode,1);
        batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/yaoqianshu2.png");
        m_fishLayer -> addChild(batchNode,1);
        batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/yaoqianshu3.png");
        m_fishLayer -> addChild(batchNode,1);
        batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/yaoqianshu4.png");//死鱼
        this -> addChild(batchNode,16);
    }
    else if(KIND_ID == NZNH_KIND_ID)
    {
        batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/nezha1.png");//通用小鱼以及组合鱼
        m_fishLayer -> addChild(batchNode,1);
        batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/nezha2.png");//大鱼
        m_fishLayer -> addChild(batchNode,1);
        batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/nezha3.png");//大鱼
        m_fishLayer -> addChild(batchNode,1);
        batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/nezha6.png");//死鱼
        this -> addChild(batchNode,16);
    }
    else
    {
        batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/jcby1.png");//通用小鱼以及组合鱼
        m_fishLayer -> addChild(batchNode,1);
        batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/jcby2.png");//大鱼
        m_fishLayer -> addChild(batchNode,1);
        batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/jcby3.png");//大鱼
        m_fishLayer -> addChild(batchNode,1);
        batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/jcby5.png");//大鱼
        m_fishLayer -> addChild(batchNode,1);
        batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/jcby4.png");//死鱼
        this -> addChild(batchNode,16);
    }
    batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/fish4.png");
    this -> addChild(batchNode,1);
    
    batchNode = SpriteBatchManager::sharedSpriteBatchManager()->addSpriteBatch("CatchFish01/fish9.png");
    this -> addChild(batchNode,21);
}

void GameMainScene::addChildToBatch(bool bAdd, CCNode* pNode, std::string filePath, bool bLine, int nZorder)
{
	CCSpriteBatchNode* batchNode = SpriteBatchManager::sharedSpriteBatchManager()->spriteBatchForKey(filePath.c_str());
	if (batchNode != NULL)
	{
		if (bAdd)
		{
            if(bLine)
            {
                batchNode->addChild(pNode, 2);
            }
            else
            {
                batchNode->addChild(pNode, nZorder);
            }
		}
		else
		{
			batchNode->removeChild(pNode, true);
		}
	}
}

void GameMainScene::setMenuTouch(bool bTouch)
{
	for(unsigned int i = 0; i < (int)m_arrMenu -> count(); i++)
	{
		CCMenu* pMenu = (CCMenu*)m_arrMenu -> objectAtIndex(i);
		pMenu -> setTouchEnabled(bTouch);
	}
}

void GameMainScene::boolAppearShadow(bool bAppear)
{
	if (m_bShadow)
	{
		for (unsigned int i = 0; i < m_vctFishFactory.size(); i++)
		{
			GameFish* pGameFish = m_vctFishFactory[i];
			shadowCallFunc(pGameFish, true);
		}
	}
	else
	{
		for (unsigned int i = 0; i < m_vctFishFactory.size(); i++)
		{
			GameFish* pGameFish = m_vctFishFactory[i];
			shadowCallFunc(pGameFish, false);
		}
	}
}

void GameMainScene::shadowCallFunc(GameFish* pGameFish, bool bAppear)
{
	if (pGameFish->getGroupFish())
	{
		CCArray* childFishAry = pGameFish->m_pFish->getChildren();
		if (childFishAry!=NULL)
		{
			for (unsigned int i = 0; i < childFishAry->count(); i++)
			{
				CCSprite* childFish = (CCSprite*)childFishAry->objectAtIndex(i);
				CCArray* childShadowAry = childFish->getChildren();
				if (childShadowAry!=NULL)
				{
					for (unsigned int j = 0; j < childShadowAry->count(); j++)
					{
						CCSprite* childShadow = (CCSprite*)childShadowAry->objectAtIndex(j);
						childShadow->setVisible(bAppear);
					}
				}
			}
		}
	}
    else if(pGameFish->getSpriteID() == 1027 || pGameFish->getSpriteID() == 1028 || pGameFish->getSpriteID() == 1029)
    {
        pGameFish->m_pFishShadow->setVisible(false);
    }
    else
    {
       pGameFish->m_pFishShadow->setVisible(bAppear);
    }
}

//读取配置
void GameMainScene::readFishXML(const char* filePath)
{
	unsigned long size;
	char *pFileContent =(char*)CCFileUtils::sharedFileUtils()->getFileData(filePath, "r", &size);
	TiXmlDocument* myDocument = new TiXmlDocument;
	myDocument->Parse(pFileContent,0,TIXML_ENCODING_UTF8);
	TiXmlElement* rootElement = myDocument->RootElement();//Root
	std::string nodename=rootElement->Value();
	if (0!=nodename.compare("client"))
	{
		return;
	}
	TiXmlElement * pChildElement=rootElement->FirstChildElement();
	while (pChildElement!=NULL)
	{
		TiXmlElement * pCurrentElement=pChildElement;
		const char * pValue=NULL;
		pChildElement=pChildElement->NextSiblingElement();
		nodename=pCurrentElement->Value();
		if (0==nodename.compare("sprite"))
		{
			std::string type=pCurrentElement->Attribute("type");
			if (0==type.compare("fish"))
			{
				int id = 0;
				std::string firstFrameStr = "";
				std::string commonNameStr = "";
				std::string filePath = "";
				std::string particleStr = "";
				std::string particleStr2 = "";
				int frameCount = 0;
				int loopTimes = 0;
				float fScale = 1.0f;
				float duration = 0.0f;
				float width = 0;
				float height = 0;
				float xPos = 0;
				float yPos = 0;
				bool bGroup = false;
				int nFish = 0;
				int nDiedAnimate = 0;
				float fLockScale = 0.0f;
				pValue=pCurrentElement->Attribute("id");
				if (NULL!=pValue)
				{
                    int nSpriteID = 0;
                    nSpriteID = atoi(pValue);
					id = atoi(pValue);
				}
				pValue=pCurrentElement->Attribute("firstName");
				if (NULL!=pValue)
				{
					firstFrameStr = pValue;
				}
				pValue=pCurrentElement->Attribute("commonName");
				if (NULL!=pValue)
				{
					commonNameStr = pValue;
				}
				pValue=pCurrentElement->Attribute("particle");
				if (NULL!=pValue)
				{
					particleStr = pValue;
				}
				pValue=pCurrentElement->Attribute("particle2");
				if (NULL!=pValue)
				{
					particleStr2 = pValue;
				}
				pValue=pCurrentElement->Attribute("frameCount");
				if (NULL!=pValue)
				{
					frameCount = atoi(pValue);
				}
				pValue=pCurrentElement->Attribute("loop");
				if (NULL!=pValue)
				{
					loopTimes = atoi(pValue);
				}
				pValue=pCurrentElement->Attribute("scale");
				if (NULL!=pValue)
				{
					fScale = atof(pValue);
				}
				pValue=pCurrentElement->Attribute("duration");
				if (NULL!=pValue)
				{
					duration = atof(pValue);
				}
				pValue=pCurrentElement->Attribute("rectwidth");
				if (NULL!=pValue)
				{
					width = atof(pValue);
				}
				pValue=pCurrentElement->Attribute("rectheight");
				if (NULL!=pValue)
				{
					height = atof(pValue);
				}
				pValue=pCurrentElement->Attribute("xPos");
				if (NULL!=pValue)
				{
					xPos = atof(pValue);
				}
				pValue=pCurrentElement->Attribute("yPos");
				if (NULL!=pValue)
				{
					yPos = atof(pValue);
				}
				pValue = pCurrentElement->Attribute("included");
				if (NULL != pValue)
				{
					filePath = pValue;
				}
				pValue=pCurrentElement->Attribute("group");
				if (NULL!=pValue)
				{
					bGroup = atoi(pValue)>0;
				}
				pValue=pCurrentElement->Attribute("fish");
				if (NULL!=pValue)
				{
					nFish = atoi(pValue);
				}
				pValue=pCurrentElement->Attribute("diedAnimate");
				if (NULL!=pValue)
				{
					nDiedAnimate = atoi(pValue);
				}
				pValue=pCurrentElement->Attribute("lockScale");
				if (NULL!=pValue)
				{
					fLockScale = atof(pValue);
				}

				std::vector<GameFishData*>vctFishData;
				vctFishData.clear();
				TiXmlElement * pChildElement2=pCurrentElement->FirstChildElement();
				while (pChildElement2!=NULL)
				{
					TiXmlElement * pCurrentElement2=pChildElement2;
					const char * pValue=NULL;
					GameFishData* pGameFishData = new GameFishData;
					pChildElement2=pChildElement2->NextSiblingElement();
					nodename=pCurrentElement2->Value();
					if (0==nodename.compare("data"))
					{
						pValue=pCurrentElement2->Attribute("spriteid");
						if (NULL!=pValue)
						{
							pGameFishData->setSpriteID(atoi(pValue));
						}
						pValue=pCurrentElement2->Attribute("posx");
						if (NULL!=pValue)
						{
							pGameFishData->setPosX(atof(pValue));
						}
						pValue=pCurrentElement2->Attribute("posy");
						if (NULL!=pValue)
						{
							pGameFishData->setPosY(atof(pValue));
						}
						pValue=pCurrentElement2->Attribute("zorder");
						if (NULL!=pValue)
						{
							pGameFishData->setZorder(atoi(pValue));
						}
						pValue=pCurrentElement2->Attribute("rotation");
						if (NULL!=pValue)
						{
							pGameFishData->setRotateBy(atoi(pValue));
						}
						pValue=pCurrentElement2->Attribute("scale");
						if (NULL!=pValue)
						{
							pGameFishData->setSubScale(atof(pValue));
						}
						vctFishData.push_back(pGameFishData);
					}
				}
                GameFish* pGameFish = new GameFish(firstFrameStr, commonNameStr, frameCount, loopTimes, duration, width, height, xPos, yPos, bGroup,vctFishData, filePath,nFish,nDiedAnimate,id,fLockScale,true, 0);
                pGameFish->setSpriteID(id);
                pGameFish->setScale(fScale);
				pGameFish->setMagParticle(particleStr);
				pGameFish->setMagParticle2(particleStr2);
				m_vctGameFish.push_back(pGameFish);
			}
		}
		else if (0==nodename.compare("particle"))
		{
			tagParticleCfg * pParticleCfg=new tagParticleCfg;
			if (NULL==pParticleCfg)
			{
				ERROR_LOG;
				continue;
			}
			pParticleCfg->magicEmitter = NULL;
			pValue=pCurrentElement->Attribute("filepath");
			if (NULL!=pValue)
			{
				pParticleCfg->sPath=pValue;
			}
			m_vctParticle.push_back(pParticleCfg);
		}
		else if (0==nodename.compare("sound"))
		{
			tagSoundCfg * pSoundCfg=new tagSoundCfg;
			if (NULL==pSoundCfg)
			{
				ERROR_LOG;
				continue;
			}
			pValue=pCurrentElement->Attribute("id");
			if (NULL!=pValue)
			{
				pSoundCfg->nSoundID=atoi(pValue);
			}
			pValue=pCurrentElement->Attribute("filepath");
			if (NULL!=pValue)
			{
				pSoundCfg->sPath=pValue;
			}
			pValue=pCurrentElement->Attribute("loop");
			if (NULL!=pValue)
			{
				pSoundCfg->bLoop=(atoi(pValue)>0);
			}
			m_vctSound.push_back(pSoundCfg);
		}
	}
    myDocument->Clear();
	delete myDocument;
}

GameFish* GameMainScene::getGameFishByID(int nID)
{
	GameFish* pGameFish = NULL;
	for (unsigned int i=0; i < m_vctGameFish.size(); i++)
	{
		if (nID==m_vctGameFish[i]->getSpriteID())
		{
			pGameFish=m_vctGameFish[i];
			break;
		}
	}
	return pGameFish;
}

GameFish* GameMainScene::createFishByID(int nID)
{
	GameFish* newFish = NULL;
	GameFish* pGameFish = getGameFishByID(nID);

    if (pGameFish!=NULL)
    {
        newFish = new GameFish(pGameFish->getFirstName(), pGameFish->getCommonName(), pGameFish->getFrameCount(), pGameFish->getLoopTimes(), pGameFish->getDuration(),pGameFish->getWidth(),pGameFish->getHeight(),pGameFish->getXPos(),pGameFish->getYPos(), pGameFish->getGroupFish(), pGameFish->vctGameFishData, pGameFish->getFilePath(),pGameFish->getFish(),pGameFish->getDiedAnimate(),pGameFish->getSpriteID(),pGameFish->getLockScale(), pGameFish->getScale(), pGameFish->getFishScore());
        if(newFish)
        {
            newFish->setMagParticle(pGameFish->getMagParticle());
            newFish->setMagParticle2(pGameFish->getMagParticle2());
            newFish->setDied(false);
            if (!newFish->getGroupFish())
            {
                //newFish->m_pFish->setScale(pGameFish->m_pFish->getScale());
            }
            if (!getFishShadow())
                shadowCallFunc(newFish, false);
        }
    }

	return newFish;
}

//播放声音
void GameMainScene::playSoundByID(bool bEffect, int nID)
{
	tagSoundCfg* pSoundCfg = NULL;
	for(unsigned int i = 0; i < m_vctSound.size(); i++)
	{
		if (nID == m_vctSound[i]->nSoundID)
		{
			pSoundCfg = m_vctSound[i];
			break;
		}
	}
	if (pSoundCfg!=NULL)
	{
        std::string dirpath = CMainLogic::sharedMainLogic()->DIRPATH1;
		std::string musicStr = dirpath+pSoundCfg->sPath;
		if (m_bEffect && bEffect)
			SimpleAudioEngine::sharedEngine()->playEffect(musicStr.c_str(), pSoundCfg->bLoop);
		else if(!bEffect)
		{
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic(musicStr.c_str(), pSoundCfg->bLoop);
		}
	}
}

//机器人锁定
void GameMainScene::onFireAndroid(float ft)
{
    int m_SmallChairID = 0;
    m_bAndroid[m_nMeRealChairID] = false;
    for(int i = 0; i < GAME_PLAYER; i++)
    {
        if(!m_bHaveUser[i])  continue;
        if(!m_bAndroid[i])
        {
            m_SmallChairID = m_nRealChairID[i];
            break;
        }
    }
    if(m_nMeRealChairID == m_SmallChairID)
    {
        for(int i = 0; i < GAME_PLAYER; i++)
        {
            if(!m_bHaveUser[i]) continue;
            if(m_nLockFishID[i] != 0) continue;
            if(i == m_nMeRealChairID)  continue;
            for(unsigned int j = 0; j < m_vctFishFactory.size(); j++)
            {
                if(m_nSuoDingFishIndex >= (int)m_vctFishFactory.size())
                {
                    m_nSuoDingFishIndex = 0;
                }
                GameFish* pGameFish = m_vctFishFactory[m_nSuoDingFishIndex++];
                if(pGameFish == NULL || pGameFish->getDied())
                    continue;
                if(pGameFish->getMultiple()>=25 || pGameFish->isProperty(enBoss) || pGameFish->isProperty(enBomb))
                {
                    CCPoint point = pGameFish->m_pFish->getPosition();
                    if(point.x >0 && point.y > 0 && point.x < m_sizeVisible.width && point.y < m_sizeVisible.height)
                    {
                        sendLockFishID(pGameFish->getFishID(),m_nRealChairID[i]);
                        break;
                    }
                }
            }
        }
    }
}

//退出倒计时提醒
void GameMainScene::outGameCount(float ft)
{
	m_nOutTime--;
	if (m_nOutTime == 9)
	{
		playSoundByID(true, 21);
		m_spReCount->setVisible(true);
		m_labelTTFReCount->setVisible(true);
		m_labelTTFReCount -> setString("9");
		m_labelTTFReCount -> setScale(1.5f);
		if (m_nMeRealChairID >= 0 && m_nMeRealChairID < GAME_PLAYER/2)
		{
			m_labelTTFReCount->setPosition(ccp(m_sizeVisible.width*0.36 - 20, m_sizeVisible.height*0.465 - 32));
		}
		else
		{
			m_spReCount->setRotation(180.0f);
			m_labelTTFReCount->setRotation(180.0f);
			m_labelTTFReCount->setPosition(ccp(m_sizeVisible.width*0.64 + 20, m_sizeVisible.height*0.535 + 32));
		}
	}
	if(m_nOutTime < 10)
	{
		char cCountNum[10];
		sprintf(cCountNum, "%d", m_nOutTime);
		m_labelTTFReCount->setString(cCountNum);
		if (m_nOutTime == 0)
			exitGame();
	}
}

//切换场景时鱼透明度降低然后消失
void GameMainScene::changeSceneScehdule(float ft)
{
	static float fRectWidth = m_sizeVisible.width;
    static float fBgWidth = m_sizeVisible.width;
    fBgWidth-=(m_sizeVisible.width+m_armatureWave->getContentSize().width)/56;
    fRectWidth-=(m_sizeVisible.width+m_armatureWave->getContentSize().width)/50;
	if (fRectWidth>=0)
	{
		CCMoveTo* moveTo = CCMoveTo::create(0.1f, ccp(fRectWidth-(m_sizeVisible.width+m_armatureWave->getContentSize().width)/47, m_sizeVisible.height/2));
        m_armatureWave->runAction(CCSequence::create(CCShow::create(), moveTo, NULL));

        if (m_nMeRealChairID>=GAME_PLAYER/2)
        {
            m_spBkgrnd2->setZOrder(-1);
            m_spBkgrnd2->setTextureRect(CCRectMake(0, 0, (m_sizeVisible.width-fBgWidth)+30, m_spBkgrnd->getContentSize().height));
        }
        else
        {
            m_spBkgrnd->setTextureRect(CCRectMake(0, 0, fBgWidth-75, m_spBkgrnd->getContentSize().height));
        }
	}
	else
	{
        if (m_nMeRealChairID>=GAME_PLAYER/2)
        {
            m_spBkgrnd2->setZOrder(-1);
            m_spBkgrnd2->setTextureRect(CCRectMake(0, 0, m_sizeVisible.width, m_spBkgrnd->getContentSize().height));
        }
        m_nBgMusicIndex += 1;
        m_nBgMusicIndex %= 3;
        playSoundByID(false, m_nBgMusicIndex%3);
		
        CCMoveTo* moveTo = CCMoveTo::create(0, ccp(m_sizeVisible.width, m_sizeVisible.height/2));
        m_armatureWave->runAction(CCSequence::create(moveTo, CCHide::create(), NULL));

		unschedule(schedule_selector(GameMainScene::changeSceneScehdule));
		fRectWidth = m_sizeVisible.width;
        fBgWidth = m_sizeVisible.width;
		this->removeChild(m_spBkgrnd);
		m_spBkgrnd = m_spBkgrnd2;
		m_spBkgrnd->setZOrder(-1);
		for (unsigned int i = 0; i < m_vctFishFactory.size(); i++)
		{
			GameFish* pGameFish = m_vctFishFactory[i];
			if (pGameFish == NULL || pGameFish->getDied())
				continue;
			addChildToBatch(false, pGameFish->m_pFish, pGameFish->getFilePath());
			delete pGameFish;
			pGameFish = NULL;
		}
		m_vctFishFactory.clear();
	}
	for (unsigned int i = 0; i < m_vctFishFactory.size(); i++)
	{
		GameFish* pFish = m_vctFishFactory[i];
		if (pFish == NULL || pFish->getDied() || pFish->getAppear())
			continue;
		if (pFish->m_pFish->getPositionX()+pFish->m_pFish->getContentSize().width >= fRectWidth)
		{
			CCCallFuncN* callFunc = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::fishOpacityCall));
			CCDelayTime* delay = CCDelayTime::create(0.1f);
			CCSequence* seq = CCSequence::create(callFunc, delay, NULL);
			CCRepeatForever* repeat = CCRepeatForever::create(seq);
			pFish->m_pFish->runAction(repeat);
			pFish->setHave(false);
			pFish -> setAppear(true);
			if (pFish->isProperty(enBoss))
			{
				for(unsigned int k = 0; k < m_labelBossBeilv.size(); k++)
				{
					if(m_labelBossBeilv[k]->getBossTag() == pFish->getFishID())
					{
						m_labelBossBeilv[k]->removeFromParentAndCleanup(true);
						m_labelBossBeilv.erase(m_labelBossBeilv.begin()+k);
						break;
					}
				}
			}
            if(KIND_ID == YQS_KIND_ID)
            {
                if(pFish->getSpriteID()>=1016 || pFish->isProperty(enLine))
                {
                    for(int k = 0; k < m_arrLockFish->count(); k++)
                    {
                        CCSprite* pLockDi = (CCSprite*)m_arrLockFish->objectAtIndex(k);
                        if(pLockDi == NULL) continue;
                        if(pLockDi->getTag() == pFish->getFishID())
                        {
                            m_arrLockFish -> removeObject(pLockDi);
                            pLockDi -> removeFromParentAndCleanup(true);
                            break;
                        }
                    }
                }
            }
            else
            {
                if(pFish->getSpriteID()>=1017 || pFish->isProperty(enLine))
                {
                    for(int j = 0; j < m_arrLockFish->count(); j++)
                    {
                        CCSprite* pLockDi = (CCSprite*)m_arrLockFish->objectAtIndex(j);
                        if(pLockDi == NULL) continue;
                        if(pLockDi->getTag() == pFish->getFishID())
                        {
                            m_arrLockFish -> removeObject(pLockDi);
                            pLockDi -> removeFromParentAndCleanup(true);
                            break;
                        }
                    }
                }
            }
            for(int j = 0; j < m_arrQipao->count(); j++)
            {
                CCSprite* pQipao = (CCSprite*)m_arrQipao->objectAtIndex(j);
                if(pQipao == NULL) continue;
                if(pQipao->getTag() == pFish->getFishID())
                {
                    pQipao -> setVisible(false);
//                    m_arrQipao -> removeObject(pQipao);
//                    pQipao -> removeFromParentAndCleanup(true);
                    break;
                }
                
            }
        }
    }
}

void GameMainScene::fishOpacityCall(CCNode* pNode)
{
	CCSprite* tempFish = (CCSprite*)pNode;
	int m = tempFish->getOpacity();
	if (m > 0)
	{
		m -= 12;
		CCArray* shadowAry = tempFish->getChildren();
		if (shadowAry!=NULL)
		{
			for (unsigned int i = 0; i < shadowAry->count(); i++)
			{
				CCSprite* shadow = (CCSprite*)shadowAry->objectAtIndex(i);
                shadow->stopAllActions();
                if (m<=100)
                    shadow->setOpacity(0);
                else
                    shadow->setOpacity(m-100);
			}
		}
		if (m<0)
			tempFish->setOpacity(0);
		else
			tempFish->setOpacity(m);
	}
	else
	{
		CCArray* tempAry = tempFish->getChildren();
		if (tempAry!=NULL)
		{
			for (unsigned int i = 0; i < tempAry->count(); i++)
			{
				CCSprite* sprite = (CCSprite*)tempAry->objectAtIndex(i);
                sprite -> stopAllActions();
                m = sprite->getOpacity();
				m -= 8;
				if (m<0)
					sprite->setOpacity(0);
				else
					sprite->setOpacity(m);
				CCArray* childAry = sprite->getChildren();
				if (childAry!=NULL)
				{
					for(unsigned int j = 0; j < childAry->count(); j++)
					{
						CCSprite* childSp = (CCSprite*)childAry->objectAtIndex(j);
                        childSp->stopAllActions();
						if (m<100)
							childSp->setOpacity(0);
						else
							childSp->setOpacity(m-100);
					}
				}
			}
		}
	}
}

//开启触摸
void GameMainScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);
}

bool GameMainScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    m_spChatBg -> setVisible(false);
    if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        if(m_bMenuOpened && m_bClickMenu)
        {
            m_bClickMenu = false;
            CCMenuItem* item = CCMenuItem::create();
            item->setTag(300);
            itemCallBack(item);
        }
    }
    
    if(m_bIsPaipaileStatus)//拍拍乐期间不能开炮
        return true;
    
	CCPoint startPoint = pTouch -> getLocation();
	CCPoint lockPoint = pTouch -> getLocation();
    
    if(m_bFirstEnter)//还没有上分
        return false;
	if((startPoint.y>m_sizeVisible.height-25) || (startPoint.y<48))
	{
		return false;
	}
	{
        if(m_nSelectDantouType != -1)
        {
            m_bDantouUsing = false;
            CCSprite* spInfo = (CCSprite*)this->getChildByTag(eDantouInfoTag);
            if(spInfo != NULL)
            {
                spInfo -> stopAllActions();
                spInfo -> runAction(CCSequence::create(CCFadeTo::create(1.0f, 0), CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall)), NULL));
            }
            //发送发射子弹的消息
            char bulletTemp[32];
            if(m_nSelectDantouType == 0)
            {
                sprintf(bulletTemp, "%s", "CatchFish2001/bullettong.png");
            }
            else if(m_nSelectDantouType == 1)
            {
                sprintf(bulletTemp, "%s", "CatchFish2001/bulletyin.png");
            }
            else if(m_nSelectDantouType == 2)
            {
                sprintf(bulletTemp, "%s", "CatchFish2001/bulletjin.png");
            }
            else
            {
                return true;
            }
            m_pointDantou = startPoint;
            CCSprite* bulletSprite = CCSprite::create(bulletTemp);
            float fDix = 0;
            if(m_nMeRealChairID == 0)
            {
                fDix = CLIENT_WIDTH/3;
                bulletSprite -> setPosition(ccp(fDix, bulletSprite->getContentSize().height/2));
            }
            else if(m_nMeRealChairID == 1)
            {
                fDix = CLIENT_WIDTH/3*2;
                bulletSprite -> setPosition(ccp(fDix, bulletSprite->getContentSize().height/2));
            }
            else if(m_nMeRealChairID == 2)
            {
                fDix = CLIENT_WIDTH/3;
            }
            else if(m_nMeRealChairID == 3)
            {
                fDix = CLIENT_WIDTH/3*2;
            }
            
            this -> addChild(bulletSprite, 1);
            float fRotation = getBarrelRotation(ccp(fDix, bulletSprite->getContentSize().height/2), startPoint);
            float dis = sqrt(abs((int)(startPoint.x-fDix))*abs((int)(startPoint.x-fDix))+abs((int)(startPoint.y-bulletSprite->getContentSize().height/2))*abs((int)(startPoint.y-bulletSprite->getContentSize().height/2)));
            bulletSprite->setRotation(fRotation);
            float fSpeed = 600.0f;
            if(m_nMeRealChairID == 2)
            {
                fRotation = getBarrelRotation(ccp(CLIENT_WIDTH/3, bulletSprite->getContentSize().height/2), startPoint);
                bulletSprite->setRotation(fRotation+180);
                bulletSprite -> setPosition(ccp(CLIENT_WIDTH/3*2, CLIENT_HEIGHT-bulletSprite->getContentSize().height/2));
            }
            else if(m_nMeRealChairID == 3)
            {
                fRotation = getBarrelRotation(ccp(CLIENT_WIDTH/3*2, bulletSprite->getContentSize().height/2), startPoint);
                bulletSprite->setRotation(fRotation+180);
                bulletSprite -> setPosition(ccp(CLIENT_WIDTH/3, CLIENT_HEIGHT-bulletSprite->getContentSize().height/2));
            }
            CCMoveTo* moveTo;
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                moveTo = CCMoveTo::create(dis/fSpeed, ccp(CLIENT_WIDTH-startPoint.x, CLIENT_HEIGHT-startPoint.y));
                m_pointDantou = ccp(CLIENT_WIDTH-startPoint.x, CLIENT_HEIGHT-startPoint.y);
            }
            else
            {
                moveTo = CCMoveTo::create(dis/fSpeed, startPoint);
            }
            SimpleAudioEngine::sharedEngine()->playEffect("Music/fashepaodan.mp3");
            CCCallFuncND* func = CCCallFuncND::create(this, callfuncND_selector(GameMainScene::removeBulletCall),(void*)(long)m_nSelectDantouType);
            bulletSprite->runAction(CCSequence::create(moveTo, func, NULL));
            m_nSelectDantouType = -1;
            return true;
        }
        if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER)
		{
			lockPoint.x = m_sizeVisible.width - startPoint.x;
			lockPoint.y = m_sizeVisible.height - startPoint.y;
		}
        
		CCPoint point = ccp(m_barrelLayer[m_nMeRealChairID]->getCannonBg()->boundingBox().origin.x + m_barrelLayer[m_nMeRealChairID]->getCannonBg()->boundingBox().size.width/2,m_barrelLayer[m_nMeRealChairID]->getCannonBg()->boundingBox().origin.y + m_barrelLayer[m_nMeRealChairID]->getCannonBg()->boundingBox().size.height/2-15);
		m_ptStart = m_barrelLayer[m_nMeRealChairID] ->convertToWorldSpace(point);
		m_ptEnd = pTouch -> getLocation();

		if(!m_bUserLock && m_nLockFishID[m_nMeRealChairID] == 0)
		{
			m_fRotation[m_nMeRealChairID] = getBarrelRotation(m_ptStart, m_ptEnd);
			m_barrelLayer[m_nMeRealChairID]->getBarrel() ->setRotation(m_fRotation[m_nMeRealChairID]);
		}
		else if(m_bUserLock)
		{
			//m_fRotation[m_nMeRealChairID] = getBarrelRotation(m_ptStart, m_ptEnd);
			m_barrelLayer[m_nMeRealChairID]->getBarrel() ->setRotation(m_fRotation[m_nMeRealChairID]);
		}
		if(m_nCurrentBulletCount >= MaxBulletCount)  return true;
        
        //判断是否可以领取救济金币（条件是当前分数不够开最低一炮，并且有领取次数）
        if(!m_bTryBarrelStatus[m_nMeRealChairID])
        {
            if(m_nUserScore[m_nMeRealChairID] < m_nBeiLv[m_nMeRealChairID] && !m_bClickAutoFire)//当前分数符合领取条件
            {
                if(m_nUserScore[m_nMeRealChairID] < m_nCellScore)//小于最低分
                {
                    if(m_nCurrentBulletCount == 0)
                    {
                        if(KIND_ID == NZNH_KIND_ID || KIND_ID == YQS_KIND_ID || KIND_ID == JCBY_KIND_ID)
                        {
                            if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
                            {
                                bool bShowFirstCharge = false;
                                bool bShowCharge = false;
                                bool bShowOneCharge = false;
                                //如果玩家在新手房，并且有1元购
                                if(m_nCellScore < 100)//新手房
                                {
                                    if(CMainLogic::sharedMainLogic()->m_bShowOneCharge && CMainLogic::sharedMainLogic()->m_bCanOneCharge)//有1元购
                                    {
                                        bShowOneCharge = true;
                                    }
                                    else if(CMainLogic::sharedMainLogic()->m_cbCountTimes == 0)
                                    {
                                        bShowFirstCharge = true;
                                    }
                                    else
                                    {
                                        bShowCharge = true;
                                    }
                                }
                                else if(m_nCellScore < 1000)//百炮房
                                {
                                    if(CMainLogic::sharedMainLogic()->m_cbCountTimes == 0)
                                    {
                                        bShowFirstCharge = true;
                                    }
                                    else
                                    {
                                        bShowCharge = true;
                                    }
                                }
                                else
                                {
                                    bShowCharge = true;
                                }
                                if(bShowOneCharge)
                                {
                                    m_bAutoOneCharge = true;
                                    OneMoneyLayer* pOneMoneyLayer = OneMoneyLayer::create();
                                    CCDirector::sharedDirector()->getRunningScene()->addChild(pOneMoneyLayer, 12, 20004);
                                }
                                else if(bShowFirstCharge)
                                {
                                    GiveCoinLayer* pGiveCoinLayer = GiveCoinLayer::create(0, true);
                                    this -> addChild(pGiveCoinLayer, 200, eGiveCoinLayerChargeTag);
                                    if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID <GAME_PLAYER)
                                    {
                                        pGiveCoinLayer->setRotation(180);
                                    }
                                }
                                else if(bShowCharge)
                                {
                                    //弹出提示充值的界面
                                    FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create(8);
                                    CCDirector::sharedDirector()->getRunningScene()->addChild(pFirstChargeLayer, 19);
                                    //弹出充值界面
                                    if(m_nCellScore < 1000)//如果在新手房或者百炮房，那么在充值界面关闭的时候，要弹出救济金
                                    {
                                        m_bHaveNoScore = true;
                                    }
                                }
                            }
                            else//苹果审核期间
                            {
                                if((CMainLogic::sharedMainLogic()->m_nBankruptcyCount < CMainLogic::sharedMainLogic()->m_nGiveTotal && m_nUserScore[m_nMeRealChairID] < CMainLogic::sharedMainLogic()->m_nGiveScore) || CMainLogic::sharedMainLogic()->m_nLogonGift != 0 || CMainLogic::sharedMainLogic()->m_bCanShare || !CMainLogic::sharedMainLogic()->m_bReceiveVipLogon || CMainLogic::sharedMainLogic()->m_bCanBandingTel)//有领取次数，并且玩家分数小于领取限制分数，才能领取救济金
                                {
                                    CMainLogic::sharedMainLogic()->m_cbCanGetBankruptcy = 1;
                                    loadUIGiveBg();
                                }
                                else//提示玩家充值
                                {
                                    CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，金币不足了。您可以充值后继续游戏。",eMBOKCancel,eChargeGameMoney);
                                }
                            }
                        }
                        else
                        {
                            CMainLogic::sharedMainLogic()->ShowMessage("很遗憾，本次捕鱼之旅没有获得美人鱼的垂青，祝您下次好运。",eMBOK,eExitFBToHall);
                        }
                    }
                    return true;
                }
                else//切换到最低分@@@@0718
                {
                    changeCannonMultiple(3, m_barrelLayer[m_nMeRealChairID]->getBarrelMult()-1);
                    setBeiLv(m_nMeRealChairID, m_nCellScore);
                }
            }
        }
        
		if(m_bAutoFire)
		{
			return true;
		}
		if (m_bCanShoot)
		{
            /*
            int nAttribute = (m_bIsBaojiStatus == true) ? 1 : 0;
            if(nAttribute == 1)
            {
                nAttribute = rand()%2;
            }
            if(m_bTryBarrelStatus[m_nMeRealChairID])
                nAttribute = 0;
			if((m_nUserScore[m_nMeRealChairID] >= m_nBeiLv[m_nMeRealChairID] || m_bTryBarrelStatus[m_nMeRealChairID]) && !m_bFishingOffSeason)//如果是试用炮状态就不用判断当前分数够不够开炮的
			{
                if(m_bIsSansheStatus && !m_bTryBarrelStatus[m_nMeRealChairID])
                {
                    int nIndex = 0;
                    nIndex = m_nUserScore[m_nMeRealChairID]/m_nBeiLv[m_nMeRealChairID] >= 5 ? 5 : m_nUserScore[m_nMeRealChairID]/m_nBeiLv[m_nMeRealChairID];
                    nIndex = MaxBulletCount - m_nCurrentBulletCount < 5 ? MaxBulletCount-m_nCurrentBulletCount : nIndex;
                    
                    DWORD dwBulletID=getNewBulletID();
                    onShoot(m_fRotation[m_nMeRealChairID],dwBulletID, nIndex, nAttribute);
                    userShoot(m_nMeRealChairID,m_fRotation[m_nMeRealChairID],m_nMeRealChairID,false,dwBulletID, nIndex, nAttribute);
                    for(int i = 0; i < 4; i++)
                    {
                        getNewBulletID();
                    }
                }
                else
                {
                    DWORD dwBulletID=getNewBulletID();
                    onShoot(m_fRotation[m_nMeRealChairID],dwBulletID, 1, nAttribute);
                    userShoot(m_nMeRealChairID,m_fRotation[m_nMeRealChairID],m_nMeRealChairID,false,dwBulletID, 1, nAttribute);
                }
				
			}*/
            if(m_bSpeedUp)
            {
                this -> schedule(schedule_selector(GameMainScene::start2Bullet), 0.05f, 4, 0);
            }
            else
            {
                start2Bullet();
            }
            
            
		}
		m_bTouch = true;
		if (m_bSpeedUp)
			this -> schedule(schedule_selector(GameMainScene::startBullet),SPEED_NORMAL);//SPEED_UP
		else
			this -> schedule(schedule_selector(GameMainScene::startBullet),SPEED_NORMAL);
	}
	return true;
}

void GameMainScene::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
    m_ptEnd = pTouch -> getLocation();
    CCPoint point = ccp(m_barrelLayer[m_nMeRealChairID]->getCannonBg()->boundingBox().origin.x + m_barrelLayer[m_nMeRealChairID]->getCannonBg()->boundingBox().size.width/2,m_barrelLayer[m_nMeRealChairID]->getCannonBg()->boundingBox().origin.y + m_barrelLayer[m_nMeRealChairID]->getCannonBg()->boundingBox().size.height/2-15);
    m_ptStart = m_barrelLayer[m_nMeRealChairID] ->convertToWorldSpace(point);
    if(!m_bUserLock)
    {
        m_fRotation[m_nMeRealChairID] = getBarrelRotation(m_ptStart, m_ptEnd);
        m_barrelLayer[m_nMeRealChairID]->getBarrel() ->setRotation(m_fRotation[m_nMeRealChairID]);
    }
}

void GameMainScene::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint point = pTouch->getLocation();
    if(m_nMeRealChairID >= GAME_PLAYER/2)
    {
        point.x = CLIENT_WIDTH-point.x;
        point.y = CLIENT_HEIGHT-point.y;
    }
	m_bTouch = false;
	if(!m_bAutoFire)
	{
		this -> unschedule(schedule_selector(GameMainScene::startBullet));
	}
    if(m_bFishingOffSeason)
    {
        return ;
    }
    if(m_bIsBaojiStatus)
    {
        for(int i = 0; i < m_arrLockFish->count(); i++)
        {
            CCSprite* pLockDi = (CCSprite*)m_arrLockFish->objectAtIndex(i);
            if(pLockDi == NULL) continue;
            CCRect rect = pLockDi->boundingBox();
            if(rect.containsPoint(point))
            {
                int nLockTag = pLockDi->getTag();
                for(int j = 0; j < m_vctFishFactory.size(); j++)
                {
                    GameFish* pGameFish = m_vctFishFactory[j];
                    if(pGameFish == NULL || pGameFish->getDied() || pGameFish->getAppear())
                        continue;
                    if(pGameFish->getFishID() == nLockTag)
                    {
                        //发送锁定信息
                        m_barrelLayer[m_nMeRealChairID]->lockRealFish(pGameFish);
                        sendLockFishID(pGameFish->getFishID(),m_nMeRealChairID);
                        m_barrelLayer[m_nMeRealChairID]->setLockFishID(pGameFish->getFishID());
                        m_nLockFishID[m_nMeRealChairID] = pGameFish->getFishID();
                        m_bLockFishID[m_nMeRealChairID] = true;
                        m_bUserLock = true;
                        m_nHistoryLockFishID = pGameFish->getFishID();
                        if(!m_bAutoFire)//锁定到鱼之后，开启自动开炮
                        {
                            m_bAutoFire = true;
                            this->schedule(schedule_selector(GameMainScene::startBullet), SPEED_NORMAL);
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
}

void GameMainScene::setWndHave(bool have)
{
	m_bWndHave = have;
    m_bHaveWnd = have;
}

bool GameMainScene::getWndHave()
{
	return m_bWndHave;
}

void GameMainScene::setWndCalcHave(bool have)
{
	m_bHaveWndCalc = have;
}

bool GameMainScene::getWndCalcHave()
{
	return m_bHaveWndCalc;
}

//删除锁定
void GameMainScene::removeLockLine(int nID)
{
    m_barrelLayer[nID]->getLockBlockSprite() -> setVisible(false);
    m_barrelLayer[nID]->getLockBlockSprite() -> removeAllChildren();
    m_nLockFishID[nID] = 0;
    m_bLockFishID[nID] = false;
    m_barrelLayer[nID]->getLockNumSprite() -> setVisible(false);
    m_barrelLayer[nID]->getLockLine()->setVisible(false);
    if(nID == m_nMeRealChairID)
    {
        haveSuoDingAnimate(false);
        m_bUserLock = false;
        //锁定线取消之后，去掉玩家的自动开炮
        if(m_bIsBaojiStatus)
        {
            m_bAutoFire = false;
            if(!m_bTouch)
            {
                this -> unschedule(schedule_selector(GameMainScene::startBullet));
            }
            
        }
    }
}

//炮筒旋转
float GameMainScene::getBarrelRotation(CCPoint ptStart, CCPoint ptEnd)
{
	float xPos, yPos;
	float rotation;
	if((int)ptStart.x != (int)ptEnd.x)
	{
		xPos = ptEnd.x - ptStart.x;
		yPos = ptEnd.y - ptStart.y;
		if(ptEnd.y < ptStart.y)
		{
			yPos = 0;
			if(xPos > 0)
			{
				rotation = 90;
			}
			else
			{
				rotation = 270;
			}
		}
		else
		{
			rotation = atan2(xPos,yPos)*(180/M_PI);
		}
	}
	else
	{
		rotation = 0;
	}
	return rotation;
}

//震屏
void GameMainScene::shakeScreen()
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
    
    m_spBkgrnd->stopAllActions();
    if(m_nMeRealChairID>=GAME_PLAYER/2)
    {
        m_spBkgrnd->setRotation(180);
        m_spBkgrnd->setPosition(ccp(m_sizeVisible.width+45,m_sizeVisible.height+45));
    }
    else
    {
        m_spBkgrnd->setPosition(ccp(-45,-45));
    }
    m_spBkgrnd->runAction(CCRepeat::create(seq1,1));
    
    
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			CCSprite* spWater = (CCSprite*)this -> getChildByTag(5000+i*3+j);
			if(spWater != NULL)
			{
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
				CCSequence* seq2 = CCSequence::create(moveBy0, moveBy1, moveBy2, moveBy3,moveBy4, moveBy5, moveBy6, moveBy7, moveBy8, moveBy9, moveBy10, moveBy11,moveBy12, moveBy13, moveBy14, moveBy15,moveBy16, moveBy17, moveBy18, moveBy19, moveBy20, moveBy21, moveBy22, moveBy23,moveBy24,moveBy24,moveBy25,moveBy0, moveBy1, moveBy2, moveBy3,moveBy4, moveBy5, moveBy6, moveBy7, moveBy8, moveBy9, moveBy10, moveBy11,moveBy12, moveBy13, moveBy14, moveBy15,moveBy16, moveBy17, moveBy18, moveBy19, moveBy20, moveBy21, moveBy22, moveBy23,moveBy24,moveBy25,moveBy0, moveBy1, moveBy2, moveBy3,moveBy4, moveBy5, moveBy6, moveBy7, moveBy8, moveBy9, moveBy10, moveBy11,moveBy12, moveBy13, moveBy14, moveBy15,moveBy16, moveBy17, moveBy18, moveBy19, moveBy20, moveBy21, moveBy22, moveBy23,moveBy24,moveBy25,NULL);
                spWater->stopAllActions();
                spWater->setPosition(ccp(150+300*i, 150+300*j));
				spWater->runAction(CCRepeat::create(seq2,1));
                CCAnimate* animate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("Water"));
                spWater->runAction(animate);
			}
		}
	}
    
    m_fishLayer->stopAllActions();
	m_fishLayer->setPosition(CCPointZero);
	moveBy0 = CCMoveBy::create(ft,point[0]);
	moveBy1 = CCMoveBy::create(ft,point[1]);
	moveBy2 = CCMoveBy::create(ft,point[2]);
	moveBy3 = CCMoveBy::create(ft,point[3]);
	moveBy4 = CCMoveBy::create(ft,point[4]);
	moveBy5 = CCMoveBy::create(ft,point[5]);
	moveBy6 = CCMoveBy::create(ft,point[6]);
	moveBy7 = CCMoveBy::create(ft,point[7]);
	moveBy8 = CCMoveBy::create(ft,point[8]);
	moveBy9 = CCMoveBy::create(ft,point[9]);
	moveBy10 = CCMoveBy::create(ft,point[10]);
	moveBy11 = CCMoveBy::create(ft,point[11]);
	moveBy12 = CCMoveBy::create(ft,point[12]);
	moveBy13 = CCMoveBy::create(ft,point[13]);
	moveBy14 = CCMoveBy::create(ft,point[14]);
	moveBy15 = CCMoveBy::create(ft,point[15]);
	moveBy16 = CCMoveBy::create(ft,point[16]);
	moveBy17 = CCMoveBy::create(ft,point[17]);
	moveBy18 = CCMoveBy::create(ft,point[18]);
	moveBy19 = CCMoveBy::create(ft,point[19]);
	moveBy20 = CCMoveBy::create(ft,point[20]);
	moveBy21 = CCMoveBy::create(ft,point[21]);
	moveBy22 = CCMoveBy::create(ft,point[22]);
	moveBy23 = CCMoveBy::create(ft,point[23]);
	moveBy24 = CCMoveBy::create(ft,point[24]);
	moveBy25 = CCMoveBy::create(ft,point[25]);
	seq1 = CCSequence::create(moveBy0, moveBy1, moveBy2, moveBy3,moveBy4, moveBy5, moveBy6, moveBy7, moveBy8, moveBy9, moveBy10, moveBy11,moveBy12, moveBy13, moveBy14, moveBy15,moveBy16, moveBy17, moveBy18, moveBy19, moveBy20, moveBy21, moveBy22, moveBy23,moveBy24,moveBy24,moveBy25,moveBy0, moveBy1, moveBy2, moveBy3,moveBy4, moveBy5, moveBy6, moveBy7, moveBy8, moveBy9, moveBy10, moveBy11,moveBy12, moveBy13, moveBy14, moveBy15,moveBy16, moveBy17, moveBy18, moveBy19, moveBy20, moveBy21, moveBy22, moveBy23,moveBy24,moveBy25,moveBy0, moveBy1, moveBy2, moveBy3,moveBy4, moveBy5, moveBy6, moveBy7, moveBy8, moveBy9, moveBy10, moveBy11,moveBy12, moveBy13, moveBy14, moveBy15,moveBy16, moveBy17, moveBy18, moveBy19, moveBy20, moveBy21, moveBy22, moveBy23,moveBy24,moveBy25,NULL);
	seq1 -> setTag(102);
	m_fishLayer->runAction(CCRepeat::create(seq1,1));
}

//钱堆运动
void GameMainScene::createMoney(int wChairID, CCPoint fishPoint, int fishMult,float rotation, int nFishScore, bool bTryCannon)
{
	if (fishMult == 0)return;
    if (nFishScore == 0) return;
//	int execute = 1;
	CCPoint endPoint;
	CCPoint jumpPoint;
	float height = 0;

    if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER)
    {
        CCPoint point1 = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
        endPoint = CCPointMake(m_sizeVisible.width-point1.x,m_sizeVisible.height-point1.y);
        endPoint = ccp(endPoint.x,endPoint.y);
        jumpPoint = ccp(0,-20);
        height = -180;
    }
    else
    {
        endPoint = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
        endPoint = ccp(endPoint.x,endPoint.y);
        jumpPoint = ccp(0,20);
        height = 180;
    }
	if(m_nMeRealChairID < GAME_PLAYER/2 && m_nMeRealChairID >= 0 && wChairID >= GAME_PLAYER/2 && wChairID < GAME_PLAYER)
	{
		endPoint = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
		endPoint = ccp(endPoint.x,endPoint.y+14);
	}
	if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER && wChairID >= 0 && wChairID < GAME_PLAYER/2)
	{
		CCPoint point1 = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
		endPoint = CCPointMake(m_sizeVisible.width-point1.x,m_sizeVisible.height-point1.y);
		endPoint = ccp(endPoint.x,endPoint.y+14);
    }
	
	int goldNum1 = 0;
	if(fishMult<=2)
		goldNum1 = 2;
    else if(fishMult<=4)
        goldNum1 = 3;
    else if(fishMult<=6)
        goldNum1 = 4;
    else if(fishMult<=8)
        goldNum1 = 5;
	else if(fishMult<=10)
		goldNum1 = 6;
	else if(fishMult<=15)
		goldNum1 = 7;
    else if(fishMult<=20)
        goldNum1 = 8;
    else if(fishMult<=30)
        goldNum1 = 6;
    else if(fishMult<=60)
        goldNum1 = 7;
	else if(fishMult<=100)
		goldNum1 = 8;
    else if(fishMult<=200)
        goldNum1 = 9;
	else
		goldNum1 = 10;
	int nHalf = goldNum1/2;
	for(int i = 0; i < goldNum1; i++)
	{
		GoldSprite* sprite = NULL;
		for(unsigned int j = 0; j < m_arrMoney->count(); j++)
		{
			sprite = (GoldSprite*)m_arrMoney -> objectAtIndex(j);
			if(sprite -> getHave())
			{
				continue;
			}
			sprite -> setHave(true);
			sprite -> setChairID(wChairID);
			sprite -> setMoneyRotation(rotation);
			if(wChairID >= GAME_PLAYER/2 && wChairID < GAME_PLAYER)
			{
				sprite -> setPosition(ccp(fishPoint.x-50*nHalf,fishPoint.y - 60));
				sprite -> setRotation(180);
			}
			else if(wChairID >= 0 && wChairID < GAME_PLAYER/2)
			{
				sprite -> setPosition(ccp(fishPoint.x-50*nHalf,fishPoint.y + 60));
			}
			
			sprite -> setScale(0.5f);
			sprite->setVisible(false);
            
            int nGodeType = 0;
            if(fishMult<=20)
            {
//                CCAnimate* animate = CCAnimate::create(animation2);
//                sprite -> runAction(animate);
                sprite -> setScale(0.7f);
                nGodeType = 0;
            }
            else
            {
//                CCAnimate* animate = CCAnimate::create(animation);
//                sprite -> runAction(animate);
                sprite -> setScale(0.8f);
                nGodeType = 1;
            }
            
//			if(goldNum2 > 0 && goldNum2-- != 0)
//			{
//				CCAnimate* animate = CCAnimate::create(animation2);
//				sprite -> runAction(animate);
//				sprite -> setScale(0.8f);
//			}
//			else
//			{
//				CCAnimate* animate = CCAnimate::create(animation2);
//				sprite -> runAction(animate);
//				sprite -> setScale(0.7f);
//			}
			nHalf--;
			int a = 0, b = 0, c = 0,d = 0;
			if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER)
			{
				a = -150;
				b = 200;
				c = -35;
				d = -25;
			}
			else if(m_nMeRealChairID >= 0 && m_nMeRealChairID < GAME_PLAYER/2)
			{
				a = 150;
				b = -200;
				c = 35;
				d = 25;
			}
			CCMoveBy* moveBy1 = CCMoveBy::create(0.18f, ccp(0,a));
			CCMoveBy* moveBy2 = CCMoveBy::create(0.23f,ccp(0,b));
			CCJumpBy* jumpBy2 = CCJumpBy::create(0.15f,ccp(0,0),c,1);
			CCJumpBy* jumpBy3 = CCJumpBy::create(0.15f,ccp(0,0),d,1);
			float distance = sqrt((sprite->getPositionX() - endPoint.x)*(sprite->getPositionX() - endPoint.x) + (sprite->getPositionY() - endPoint.y)*(sprite->getPositionY() - endPoint.y));
			CCMoveTo* moveTo = CCMoveTo::create(distance/sprite->getSpeed(),endPoint);
			CCCallFuncND* funcND = CCCallFuncND::create(this, callfuncND_selector(GameMainScene::removeGold), (void*)(long)nGodeType);
			CCCallFuncN* func1 = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::hideGold));
			//CCSequence* seq = CCSequence::create(moveBy1, moveBy2,CCDelayTime::create(0.05f),jumpBy2, jumpBy3,CCDelayTime::create(0.1f),moveTo,CCHide::create(),func1,NULL);
            CCSequence* seq = CCSequence::create(CCDelayTime::create(0.1f),moveTo,CCHide::create(),func1,NULL);
			sprite->runAction(CCSequence::create(CCDelayTime::create(0.05f*i),funcND,seq, NULL));
//			if(execute == 1)	//积分版上的精灵只执行一次动画
//			{
//				CCCallFuncND* func2 = CCCallFuncND::create(this, callfuncND_selector(GameMainScene::moneyAnimation), (void*)(long)bTryCannon);
//				sprite -> runAction(CCSequence::create(CCDelayTime::create(distance/sprite->getSpeed()+0.7),func2,NULL));
//                if(wChairID == m_nMeRealChairID)
//                {
//                    if(!bTryCannon)//试用炮期间，不显示得分
//                    {
//                        CCCallFuncND* func3 = CCCallFuncND::create(this, callfuncND_selector(GameMainScene::showMeScore), (int*)(long)nFishScore);
//                        m_nodeScore->runAction(CCSequence::create(CCDelayTime::create(distance/sprite->getSpeed()+0.7f), func3, NULL));
//                    }
//                }
//			}
//			execute++;
			break;
		}
	}
}

void GameMainScene::createMoney3(int wChairID, CCPoint fishPoint, int fishMult,float rotation, int nFishScore)//拍拍乐金币
{
    if (fishMult == 0)return;
    int index = 0;
    int count = m_arrMoney -> count();
    if(fishMult >= 30)
        fishMult = 30;
    int nHalf = fishMult/2;
    CCPoint endPoint;
    if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER)
    {
        CCPoint point1 = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
        endPoint = CCPointMake(m_sizeVisible.width-point1.x,m_sizeVisible.height-point1.y);
        endPoint = ccp(endPoint.x,endPoint.y);
    }
    else
    {
        endPoint = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
        endPoint = ccp(endPoint.x,endPoint.y);
    }
    if(m_nMeRealChairID < GAME_PLAYER/2 && m_nMeRealChairID >= 0 && wChairID >= GAME_PLAYER/2 && wChairID < GAME_PLAYER)
    {
        endPoint = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
        endPoint = ccp(endPoint.x,endPoint.y+14);
    }
    if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER && wChairID >= 0 && wChairID < GAME_PLAYER/2)
    {
        CCPoint point1 = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
        endPoint = CCPointMake(m_sizeVisible.width-point1.x,m_sizeVisible.height-point1.y);
        endPoint = ccp(endPoint.x,endPoint.y+14);
    }
    int nFirst = 0;
    for(int i = 0; i < count; i++)
    {
        GoldSprite* sprite = (GoldSprite*)m_arrMoney -> objectAtIndex(i);
        if(sprite -> getHave())
        {
            continue;
        }
        sprite -> setHave(true);
        sprite -> setChairID(wChairID);
        sprite -> setMoneyRotation(rotation);
        sprite -> setPosition(ccp(fishPoint.x-40*nHalf*sin(-(rotation)/(180/M_PI)),fishPoint.y+nHalf*40*cos(-(rotation)/(180/M_PI))));
        sprite -> setVisible(true);
        sprite -> setScale(0.7f);
        index++;
        addMoney2(wChairID, sprite,endPoint);
        if(index == fishMult)
        {
            break;
        }
        nHalf--;
        
        float distance = sqrt((sprite->getPositionX() - endPoint.x)*(sprite->getPositionX() - endPoint.x) + (sprite->getPositionY() - endPoint.y)*(sprite->getPositionY() - endPoint.y));
        if(nFirst == 0 && wChairID == m_nMeRealChairID)
        {
            CCCallFuncND* func3 = CCCallFuncND::create(this, callfuncND_selector(GameMainScene::showMeScore), (int*)(long)nFishScore);
            CCCallFuncN* func4 = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall));
            CCSprite* coinSprite = CCSprite::create();
            this -> addChild(coinSprite);
            coinSprite -> setTag(wChairID);
            coinSprite->runAction(CCSequence::create(CCDelayTime::create(distance/sprite->getSpeed()+0.7f), func3, func4, NULL));
        }
        nFirst++;
    }
    
}

void GameMainScene::moneyAnimation(CCNode* node, void* bTryCannon)
{
    GoldSprite* sprite = (GoldSprite*)node;
    CCSprite* moneySprite = NULL;
    CCSprite* pSpBg = NULL;
    moneySprite = m_barrelLayer[sprite->getChairID()]->getMoneySprite();
    pSpBg = (CCSprite*)m_barrelLayer[sprite->getChairID()]->getMoneySprite()->getParent();
    
    moneySprite -> runAction(CCSequence::create(CCScaleTo::create(0.2f,1.5f),CCScaleTo::create(0.2f,1.0f),NULL));
    //播放序列帧动画
    CCSprite* money = CCSprite::createWithSpriteFrameName("guangItem1_1.png");
    if(pSpBg != NULL)
    {
        pSpBg -> addChild(money ,10);
        money -> setPosition(m_barrelLayer[sprite->getChairID()]->getMoneySprite()->getPosition());
    }
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCArray* array = CCArray::create();
    int moneyCount = 13;
    for(int i = 1; i <= moneyCount; i++)
	{
		char temp[20];
		sprintf(temp,"guangItem1_%d.png",i);
		CCSpriteFrame* frame = cache->spriteFrameByName(temp);
		array -> addObject(frame);
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(array,0.02f);
	CCAnimate* animate = CCAnimate::create(animation);
	CCCallFuncN* call = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
	money ->runAction(CCSequence::create(animate,call,NULL));//播放完之后从屏幕中删除
}

//产生金币
void GameMainScene::createMoney2(int wChairID, CCPoint fishPoint, int fishMult,float rotation)
{
	if (fishMult == 0)return;
	int index = 0;
	int count = m_arrMoney -> count();
	if(fishMult >= 30)
		fishMult = 30;
	int nHalf = fishMult/2;
	CCPoint endPoint;
	if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER)
	{
		CCPoint point1 = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
		endPoint = CCPointMake(m_sizeVisible.width-point1.x,m_sizeVisible.height-point1.y);
		endPoint = ccp(endPoint.x-130,endPoint.y);
	}
	else
	{
		endPoint = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
		endPoint = ccp(endPoint.x+130,endPoint.y);
	}
	if(m_nMeRealChairID < GAME_PLAYER/2 && m_nMeRealChairID >= 0 && wChairID >= GAME_PLAYER/2 && wChairID < GAME_PLAYER)
	{
		endPoint = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
		endPoint = ccp(endPoint.x - 240,endPoint.y+14);
	}
	if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER && wChairID >= 0 && wChairID < GAME_PLAYER/2)
	{
		CCPoint point1 = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
		endPoint = CCPointMake(m_sizeVisible.width-point1.x,m_sizeVisible.height-point1.y);
		endPoint = ccp(endPoint.x + 240,endPoint.y+14);
	}
	for(int i = 0; i < count; i++)
	{
		GoldSprite* sprite = (GoldSprite*)m_arrMoney -> objectAtIndex(i);
		if(sprite -> getHave())
		{
			continue;
		}
		sprite -> setHave(true);
		sprite -> setChairID(wChairID);
		sprite -> setMoneyRotation(rotation);
		sprite -> setPosition(ccp(fishPoint.x-40*nHalf*sin(-(rotation)/(180/M_PI)),fishPoint.y+nHalf*80*cos(-(rotation)/(180/M_PI))));
		sprite -> setVisible(true);
		sprite -> setScale(1.1f);
		index++;
		addMoney2(wChairID, sprite,endPoint);
		if(nHalf == 0)//积分版上的精灵只执行一次动画
		{
			CCPoint startPoint = sprite -> getPosition();
			float distance = sqrt((startPoint.x - endPoint.x)*(startPoint.x - endPoint.x) + (startPoint.y - endPoint.y)*(startPoint.y - endPoint.y));
			CCCallFuncND* func2 = CCCallFuncND::create(this, callfuncND_selector(GameMainScene::moneyAnimation), (void*)0);
			sprite -> runAction(CCSequence::create(CCDelayTime::create(distance/sprite->getSpeed()+0.8),func2,NULL));
		}
		if(index == fishMult)
		{
			break;
		}
		nHalf--;
	}
}

void GameMainScene::addMoney2(int wChairID, GoldSprite* sprite, CCPoint endPoint)
{
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCArray* array = CCArray::create();
	char temp[20];
	for(int i = 1; i <= 12; i++)
	{
		sprintf(temp, "gold_0_0%i.png", i);
		CCSpriteFrame* frame = cache -> spriteFrameByName(temp);
		array -> addObject(frame);
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(array,0.08f);
	animation -> setRestoreOriginalFrame(true);
	CCAnimate* animate = CCAnimate::create(animation);
	CCPoint startPoint = sprite -> getPosition();
	
	float distance = sqrt((startPoint.x - endPoint.x)*(startPoint.x - endPoint.x) + (startPoint.y - endPoint.y)*(startPoint.y - endPoint.y));
	CCMoveTo* moveTo = CCMoveTo::create(distance/sprite->getSpeed(),endPoint);
	CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::hideGold));
	sprite -> runAction(CCRepeatForever::create(animate));
	sprite -> runAction(CCSequence::create(CCDelayTime::create(0.5f),moveTo,CCHide::create(),func,NULL));
}

void GameMainScene::removeGold(CCNode* node, void * nGoldType)
{
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    char temp[20];
    CCArray* array = CCArray::create();
    for(int i = 1; i < 13; i++)
    {
        sprintf(temp,"gold_0_0%d.png",i);
        CCSpriteFrame* frame = cache -> spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array,0.05f);
    animation->setLoops(-1);
    CCArray* array2 = CCArray::create();
    for(int i = 1; i < 13; i++)
    {
        sprintf(temp,"gold_1_0%d.png",i);
        CCSpriteFrame* frame = cache -> spriteFrameByName(temp);
        array2 -> addObject(frame);
    }
    CCAnimation* animation2 = CCAnimation::createWithSpriteFrames(array2,0.05f);
    animation2->setLoops(-1);
    long lVal = (long)nGoldType;
    if(lVal == 0)
    {
        CCAnimate* animate = CCAnimate::create(animation2);
        node -> runAction(animate);
    }
    else
    {
        CCAnimate* animate = CCAnimate::create(animation);
        node -> runAction(animate);
    }
    node -> setVisible(true);
}

//隐藏金币
void GameMainScene::hideGold(CCNode* node)
{
	GoldSprite* sprite = (GoldSprite*)node;
	sprite -> setHave(false);
	sprite -> stopAllActions();
	sprite -> setVisible(false);
}

//自动开炮
void GameMainScene::autoFire()
{
	m_bAutoFire = !m_bAutoFire;
	if(m_bAutoFire)
	{
		if (m_bSpeedUp)
			this -> schedule(schedule_selector(GameMainScene::startBullet),SPEED_NORMAL);//SPEED_UP
		else
			this -> schedule(schedule_selector(GameMainScene::startBullet),SPEED_NORMAL);
	}
	else
	{
		this -> unschedule(schedule_selector(GameMainScene::startBullet));
	}
}

//子弹ID+1
unsigned int GameMainScene::getNewBulletID()
{
	m_dwBulletID++;
	if (0==m_dwBulletID) m_dwBulletID=1;
	return m_dwBulletID;
}

void GameMainScene::cancelAutoFire()
{
    CCMenuItem* item = CCMenuItem::create();
    item -> setTag(309);
    itemCallBack(item);
}

void GameMainScene::checkAutoFire()//检测玩家分数，如果不足时，弹出充值界面或者救济金界面
{
    if(m_nCurrentBulletCount >= MaxBulletCount)  return;
    if(m_bAutoFire)
    {
        if(m_nCurrentBulletCount == 0 && !m_bTryBarrelStatus[m_nMeRealChairID])
        {
            if(m_nUserScore[m_nMeRealChairID] < m_nBeiLv[m_nMeRealChairID])
            {
                if(m_nUserScore[m_nMeRealChairID] < m_nCellScore)
                {
                    if(KIND_ID == NZNH_KIND_ID || KIND_ID == YQS_KIND_ID || KIND_ID == JCBY_KIND_ID)
                    {
                        if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
                        {
                            bool bShowFirstCharge = false;
                            bool bShowCharge = false;
                            bool bShowOneCharge = false;
                            //如果玩家在新手房，并且有1元购
                            if(m_nCellScore < 100)//新手房
                            {
                                if(CMainLogic::sharedMainLogic()->m_bShowOneCharge && CMainLogic::sharedMainLogic()->m_bCanOneCharge)//有1元购
                                {
                                    bShowOneCharge = true;
                                }
                                else if(CMainLogic::sharedMainLogic()->m_cbCountTimes == 0)
                                {
                                    bShowFirstCharge = true;
                                }
                                else
                                {
                                    bShowCharge = true;
                                }
                            }
                            else if(m_nCellScore < 1000)//百炮房
                            {
                                if(CMainLogic::sharedMainLogic()->m_cbCountTimes == 0)
                                {
                                    bShowFirstCharge = true;
                                }
                                else
                                {
                                    bShowCharge = true;
                                }
                            }
                            else
                            {
                                bShowCharge = true;
                            }
                            if(bShowOneCharge)
                            {
                                m_bAutoOneCharge = true;
                                OneMoneyLayer* pOneMoneyLayer = OneMoneyLayer::create();
                                CCDirector::sharedDirector()->getRunningScene()->addChild(pOneMoneyLayer, 12, 20004);
                            }
                            else if(bShowFirstCharge)
                            {
                                GiveCoinLayer* pGiveCoinLayer = GiveCoinLayer::create(0, true);
                                this -> addChild(pGiveCoinLayer, 200, eGiveCoinLayerChargeTag);
                                if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID <GAME_PLAYER)
                                {
                                    pGiveCoinLayer->setRotation(180);
                                }
                            }
                            else if(bShowCharge)
                            {
                                //弹出提示充值的界面
                                FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create(8);
                                CCDirector::sharedDirector()->getRunningScene()->addChild(pFirstChargeLayer, 19);
                                //弹出充值界面
                                if(m_nCellScore < 1000)//如果在新手房或者百炮房，那么在充值界面关闭的时候，要弹出救济金
                                {
                                    m_bHaveNoScore = true;
                                }
                            }
//                            //如果玩家还有首充，那么先提示首充
//                            if(CMainLogic::sharedMainLogic()->m_cbCountTimes == 0 && m_nCellScore < 1000)
//                            {
//                                GiveCoinLayer* pGiveCoinLayer = GiveCoinLayer::create(0, true);
//                                this -> addChild(pGiveCoinLayer, 200, eGiveCoinLayerChargeTag);
//                                if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID <GAME_PLAYER)
//                                {
//                                    pGiveCoinLayer->setRotation(180);
//                                }
//                            }
//                            else
//                            {
//                                //弹出充值界面
//                                m_bHaveNoScore = true;
//                                CCMenuItem* item = CCMenuItem::create();
//                                item -> setTag(302);
//                                itemCallBack(item);
//                            }
                        }
                        else//苹果审核期间
                        {
                            if((CMainLogic::sharedMainLogic()->m_nBankruptcyCount < CMainLogic::sharedMainLogic()->m_nGiveTotal && m_nUserScore[m_nMeRealChairID] < CMainLogic::sharedMainLogic()->m_nGiveScore) || CMainLogic::sharedMainLogic()->m_nLogonGift != 0 || CMainLogic::sharedMainLogic()->m_bCanShare || !CMainLogic::sharedMainLogic()->m_bReceiveVipLogon || CMainLogic::sharedMainLogic()->m_bCanBandingTel)//有领取次数，并且玩家分数小于领取限制分数，才能领取救济金
                            {
                                CMainLogic::sharedMainLogic()->m_cbCanGetBankruptcy = 1;
                                loadUIGiveBg();
                            }
                            else
                            {
                                CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，金币不足了。您可以充值后继续游戏。",eMBOKCancel,eChargeGameMoney);
                            }
                        }
                        CCMenuItem* item = CCMenuItem::create();//取消自动开炮
                        item -> setTag(309);
                        itemCallBack(item);
                        this -> unschedule(schedule_selector(GameMainScene::checkAutoFire));
                        return;
                    }
                    else
                    {
                        CMainLogic::sharedMainLogic()->ShowMessage("很遗憾，本次捕鱼之旅没有获得美人鱼的垂青，祝您下次好运。",eMBOK,eExitFBToHall);
                    }
                }
                else//@@@@0718
                {
                    changeCannonMultiple(3, m_barrelLayer[m_nMeRealChairID]->getBarrelMult()-1);
                    setBeiLv(m_nMeRealChairID, m_nCellScore);
                }
                
                
                
            }
        }
    }
}

void GameMainScene::start2Bullet()//发射子弹
{
    if(m_nUserScore[m_nMeRealChairID] >= 0 && !m_bFishingOffSeason)
    {
        if(m_nCurrentBulletCount >= MaxBulletCount)  return;
        int nAttribute = (m_bIsBaojiStatus == true) ? 1 : 0;
        if(nAttribute == 1)
        {
            nAttribute = rand()%2;
        }
        if(m_bTryBarrelStatus[m_nMeRealChairID])
            nAttribute = 0;
        if(m_nUserScore[m_nMeRealChairID] < m_nBeiLv[m_nMeRealChairID])
        {
            changeCannonMultiple(3, m_barrelLayer[m_nMeRealChairID]->getBarrelMult()-1);
            setBeiLv(m_nMeRealChairID, m_nCellScore);
        }
        if((m_nUserScore[m_nMeRealChairID] >= m_nBeiLv[m_nMeRealChairID] || m_bTryBarrelStatus[m_nMeRealChairID]) && !m_bFishingOffSeason)
        {
            if(m_bIsSansheStatus && !m_bTryBarrelStatus[m_nMeRealChairID])
            {
                int nIndex = 0;
                nIndex = m_nUserScore[m_nMeRealChairID]/m_nBeiLv[m_nMeRealChairID] >= 5 ? 5 : m_nUserScore[m_nMeRealChairID]/m_nBeiLv[m_nMeRealChairID];
                nIndex = MaxBulletCount - m_nCurrentBulletCount < 5 ? MaxBulletCount-m_nCurrentBulletCount : nIndex;
                
                DWORD dwBulletID=getNewBulletID();
                onShoot(m_fRotation[m_nMeRealChairID],dwBulletID, nIndex, nAttribute);
                userShoot(m_nMeRealChairID,m_fRotation[m_nMeRealChairID],m_nMeRealChairID,false,dwBulletID, nIndex, nAttribute);
                for(int i = 0; i < 4; i++)
                {
                    getNewBulletID();
                }
            }
            else
            {
                DWORD dwBulletID=getNewBulletID();
                onShoot(m_fRotation[m_nMeRealChairID],dwBulletID, 1, nAttribute);
                userShoot(m_nMeRealChairID,m_fRotation[m_nMeRealChairID],m_nMeRealChairID,false,dwBulletID, 1, nAttribute);
            }
        }
    }
}

//定时器发射子弹
void GameMainScene::startBullet(float dt)
{
    if(m_bSpeedUp)
    {
        this -> schedule(schedule_selector(GameMainScene::start2Bullet), 0.05f, 4, 0);
    }
    else
    {
        start2Bullet();
    }
    
}

void GameMainScene::bulletStatus()
{
	m_bCanShoot = true;
}

//渔网
void GameMainScene::createFishNet(CCPoint point,float rotation,int type, int nMultiple, WORD wChairID, BYTE cbAttribute, int nIndex)
{
    if(type == TwoFishNet)
    {
        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        CCSprite* fishNetSprite1 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w01_01.png"));
        fishNetSprite1->setScale(1.2f);//
        float fishWidth = fishNetSprite1 -> boundingBox().size.width/5-25;
        fishNetSprite1 -> setPosition(ccp(point.x - fishWidth*sin((90-rotation)/(180/M_PI)),point.y+fishWidth*cos((90-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite1, "CatchFish01/fish4.png");
        CCSprite* fishNetSprite2 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w01_01.png"));
        fishNetSprite2->setScale(1.2f);//
        fishNetSprite2 -> setPosition(ccp(point.x + fishWidth*sin((90-rotation)/(180/M_PI)),point.y-fishWidth*cos((90-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite2, "CatchFish01/fish4.png");
        CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
        fishNetSprite1 -> runAction(CCSequence::create(fishNetAnimate(9, 1),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite2 -> runAction(CCSequence::create(fishNetAnimate(9, 1),CCDelayTime::create(0.1f),func,NULL));
    }
    else if(type == ThreeFishNet)
    {
        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        CCSprite* fishNetSprite1 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w01_01.png"));
        float fishWidth = 50;
        fishNetSprite1 -> setPosition(ccp(point.x + fishWidth*sin((60 - rotation)/(180/M_PI)),point.y-fishWidth*cos((60 - rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite1, "CatchFish01/fish4.png");
        CCSprite* fishNetSprite2 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w01_01.png"));
        fishNetSprite2 -> setPosition(ccp(point.x + fishWidth*sin((-60-rotation)/(180/M_PI)),point.y-fishWidth*cos((-60-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite2, "CatchFish01/fish4.png");
        CCSprite* fishNetSprite3 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w01_01.png"));
        fishNetSprite3 -> setPosition(ccp(point.x + fishWidth*sin((-180-rotation)/(180/M_PI)),point.y-fishWidth*cos((-180-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite3, "CatchFish01/fish4.png");
        CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
        fishNetSprite1 -> runAction(CCSequence::create(fishNetAnimate(9, 1),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite2 -> runAction(CCSequence::create(fishNetAnimate(9, 1),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite3 -> runAction(CCSequence::create(fishNetAnimate(9, 1),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite1->setScale(1.2f);
        fishNetSprite2->setScale(1.2f);
        fishNetSprite3->setScale(1.2f);
    }
    else if(type == FourFishNet)
    {
        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        CCSprite* fishNetSprite1 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w01_01.png"));
        float fishWidth = 80;
        fishNetSprite1 -> setPosition(ccp(point.x - fishWidth*sin((45-rotation)/(180/M_PI)),point.y+fishWidth*cos((45-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite1, "CatchFish01/fish4.png");
        CCSprite* fishNetSprite2 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w01_01.png"));
        fishNetSprite2 -> setPosition(ccp(point.x + fishWidth*sin((45-rotation)/(180/M_PI)),point.y-fishWidth*cos((45-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite2, "CatchFish01/fish4.png");
        CCSprite* fishNetSprite3 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w01_01.png"));
        fishNetSprite3 -> setPosition(ccp(point.x + fishWidth*sin((-45-rotation)/(180/M_PI)),point.y-fishWidth*cos((-45-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite3, "CatchFish01/fish4.png");
        CCSprite* fishNetSprite4 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w01_01.png"));
        fishNetSprite4 -> setPosition(ccp(point.x - fishWidth*sin((-45-rotation)/(180/M_PI)),point.y+fishWidth*cos((-45-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite4, "CatchFish01/fish4.png");
        CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
        fishNetSprite1 -> runAction(CCSequence::create(fishNetAnimate(9, 1),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite2 -> runAction(CCSequence::create(fishNetAnimate(9, 1),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite3 -> runAction(CCSequence::create(fishNetAnimate(9, 1),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite4 -> runAction(CCSequence::create(fishNetAnimate(9, 1),CCDelayTime::create(0.1f),func,NULL));
    }
    else if(type == TwoSuperFishNet)
    {
//        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
//        CCSprite* fishNetSprite1 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w02_01.png"));
//        fishNetSprite1 -> setPosition(point);
//        fishNetSprite1 -> setScale(2.0f);
//        addChildToBatch(true, fishNetSprite1, "CatchFish01/fish4.png");
//        CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
//        fishNetSprite1 -> runAction(CCSequence::create(fishNetAnimate(14, 2),CCDelayTime::create(0.1f),func,NULL));
        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        CCSprite* fishNetSprite1 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w02_01.png"));
        fishNetSprite1->setScale(1.2f);
        float fishWidth = fishNetSprite1 -> boundingBox().size.width/5-25;
        fishNetSprite1 -> setPosition(ccp(point.x - fishWidth*sin((90-rotation)/(180/M_PI)),point.y+fishWidth*cos((90-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite1, "CatchFish01/fish4.png");
        CCSprite* fishNetSprite2 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w02_01.png"));
        fishNetSprite2->setScale(1.2f);
        fishNetSprite2 -> setPosition(ccp(point.x + fishWidth*sin((90-rotation)/(180/M_PI)),point.y-fishWidth*cos((90-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite2, "CatchFish01/fish4.png");
        CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
        fishNetSprite1 -> runAction(CCSequence::create(fishNetAnimate(9, 2),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite2 -> runAction(CCSequence::create(fishNetAnimate(9, 2),CCDelayTime::create(0.1f),func,NULL));
    }
    else if(type == ThreeSuperFishNet)
    {
//        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
//        CCSprite* fishNetSprite1 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w03_01.png"));
//        fishNetSprite1 -> setPosition(point);
//        fishNetSprite1 -> setScale(2.0f);
//        addChildToBatch(true, fishNetSprite1, "CatchFish01/fish4.png");
//        CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
//        fishNetSprite1 -> runAction(CCSequence::create(fishNetAnimate(14, 3),CCDelayTime::create(0.1f),func,NULL));
        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        CCSprite* fishNetSprite1 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w02_01.png"));
        float fishWidth = 50;
        fishNetSprite1 -> setPosition(ccp(point.x + fishWidth*sin((60 - rotation)/(180/M_PI)),point.y-fishWidth*cos((60 - rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite1, "CatchFish01/fish4.png");
        CCSprite* fishNetSprite2 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w02_01.png"));
        fishNetSprite2 -> setPosition(ccp(point.x + fishWidth*sin((-60-rotation)/(180/M_PI)),point.y-fishWidth*cos((-60-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite2, "CatchFish01/fish4.png");
        CCSprite* fishNetSprite3 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w02_01.png"));
        fishNetSprite3 -> setPosition(ccp(point.x + fishWidth*sin((-180-rotation)/(180/M_PI)),point.y-fishWidth*cos((-180-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite3, "CatchFish01/fish4.png");
        CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
        fishNetSprite1 -> runAction(CCSequence::create(fishNetAnimate(9, 2),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite2 -> runAction(CCSequence::create(fishNetAnimate(9, 2),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite3 -> runAction(CCSequence::create(fishNetAnimate(9, 2),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite1->setScale(1.2f);
        fishNetSprite2->setScale(1.2f);
        fishNetSprite3->setScale(1.2f);
    }
    else if(type == FourSuperFishNet)
    {
//        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
//        CCSprite* fishNetSprite1 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w04_01.png"));
//        fishNetSprite1 -> setPosition(point);
//        fishNetSprite1 -> setScale(2.0f);
//        addChildToBatch(true, fishNetSprite1, "CatchFish01/fish4.png");
//        CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
//        fishNetSprite1 -> runAction(CCSequence::create(fishNetAnimate(14, 4),CCDelayTime::create(0.1f),func,NULL));
        CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        CCSprite* fishNetSprite1 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w02_01.png"));
        float fishWidth = 50;
        fishNetSprite1 -> setPosition(ccp(point.x + fishWidth*sin((60 - rotation)/(180/M_PI)),point.y-fishWidth*cos((60 - rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite1, "CatchFish01/fish4.png");
        CCSprite* fishNetSprite2 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w02_01.png"));
        fishNetSprite2 -> setPosition(ccp(point.x + fishWidth*sin((-60-rotation)/(180/M_PI)),point.y-fishWidth*cos((-60-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite2, "CatchFish01/fish4.png");
        CCSprite* fishNetSprite3 = CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("w02_01.png"));
        fishNetSprite3 -> setPosition(ccp(point.x + fishWidth*sin((-180-rotation)/(180/M_PI)),point.y-fishWidth*cos((-180-rotation)/(180/M_PI))));
        addChildToBatch(true, fishNetSprite3, "CatchFish01/fish4.png");
        CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
        fishNetSprite1 -> runAction(CCSequence::create(fishNetAnimate(9, 2),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite2 -> runAction(CCSequence::create(fishNetAnimate(9, 2),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite3 -> runAction(CCSequence::create(fishNetAnimate(9, 2),CCDelayTime::create(0.1f),func,NULL));
        fishNetSprite1->setScale(1.2f);
        fishNetSprite2->setScale(1.2f);
        fishNetSprite3->setScale(1.2f);
    }
}

CCAnimate* GameMainScene::fishNetAnimate(int nNum, int nType)
{
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCArray* array = CCArray::create();
    char temp[20];
    for(int i = 0; i < nNum; i++)
    {
        sprintf(temp,"w0%d_0%i.png", nType, i + 1);
        CCSpriteFrame* frame = cache -> spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array,0.02f);
    CCAnimate* animate = CCAnimate::create(animation);
    return animate;
}

CCSequence* GameMainScene::shandianAction(int nIndex)
{
    float fTime = 0.4f;
    CCArray* array = CCArray::create();
    char temp[20];
    for(int i = 0; i < 5; i++)
    {
        sprintf(temp, "shandianline%d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()-> spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, fTime/5);
    animation -> setRestoreOriginalFrame(true);
    CCAnimate* animate = CCAnimate::create(animation);
    CCCallFuncN* pCallFuncN = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall));
    CCSequence* pSeq = CCSequence::create(CCDelayTime::create(0.15f*nIndex),CCShow::create(),animate,  pCallFuncN, NULL);
    return pSeq;
}

void GameMainScene::getDantouType(int nType)
{
    /*
    if (nType == 0)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameGetTongDantou);
    }
    else if (nType == 1)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameGetYinDantou);
    }
    else if (nType == 2)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameGetJinDantou);
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    SimpleAudioEngine::sharedEngine()->playEffect("Music/openbox.mp3");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->playEffect("Music/openbox.ogg");
#endif
    
    GetPriseLayer* pPriseLayer = GetPriseLayer::create(nType+4, 1, "");
    CCDirector::sharedDirector()->getRunningScene()->addChild(pPriseLayer, 15);
    
    int nDantouType = nType;
    char dantouTemp[32];
    if(nDantouType == 0)
    {
        sprintf(dantouTemp, "CatchFish2001/tongdantou1.png");
    }
    else if(nDantouType == 1)
    {
        sprintf(dantouTemp, "CatchFish2001/yindantou1.png");
    }
    else if(nDantouType == 2)
    {
        sprintf(dantouTemp, "CatchFish2001/jindantou1.png");
    }
    CCSprite* dantouSprite = CCSprite::create(dantouTemp);
    
    this -> addChild(dantouSprite, 50);
    dantouSprite -> setAnchorPoint(ccp(0.5, 0.2));
    dantouSprite -> setScale(0.2f);
    dantouSprite -> setVisible(false);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall));
    CCEaseExponentialIn* easeIn;
    if(m_nMeRealChairID >= GAME_PLAYER/2)
    {
        dantouSprite -> setRotation(180);
        dantouSprite -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+80));
        CCMoveTo* moveTo = CCMoveTo::create(1.0f, ccp(CLIENT_WIDTH-1220, CLIENT_HEIGHT-500));
        easeIn = CCEaseExponentialIn::create(moveTo);
    }
    else
    {
        dantouSprite -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-80));
        CCMoveTo* moveTo = CCMoveTo::create(1.0f, ccp(1220, 500));
        easeIn = CCEaseExponentialIn::create(moveTo);
    }
    dantouSprite -> runAction(CCSequence::create(CCDelayTime::create(3.2f), CCShow::create(), CCSpawn::create(CCScaleTo::create(0.5f, 1.2f), CCFadeIn::create(0.3f), NULL), CCScaleTo::create(0.2f, 1.0f), CCDelayTime::create(1.0f), CCSpawn::create(easeIn, CCScaleTo::create(1.0f, 0.6f), NULL), CCCallFunc::create(this, callfunc_selector(GameMainScene::playDantouSound)), func, NULL));
    */
}

void GameMainScene::playDantouSound(CCNode* pNode, void* nCount)
{
    long nWufuCount = (long)nCount;
    m_dtParticle -> setVisible(true);
    m_spDantouDi -> setVisible(true);
    m_spDantouDi -> runAction(CCRepeat::create(CCSequence::create(CCScaleTo::create(0.5f, 2.2f), CCScaleTo::create(0.5f, 1.2f), NULL), 3));
    m_nDantouTotalNum += nWufuCount;
    if(m_nDantouTotalNum != 0)
    {
        char temp[16];
        sprintf(temp, "%d", m_nDantouTotalNum);
        m_labelDantou -> setString(temp);
    }
}

void GameMainScene::showDantouInfo()
{
    CCSprite* spInfo1 = (CCSprite*)this->getChildByTag(eDantouInfoTag);
    if(spInfo1 != NULL)
    {
        spInfo1 -> stopAllActions();
        spInfo1 -> runAction(CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall)));
    }
    char temp[32];
    if(m_nSelectDantouType == 0)
    {
        sprintf(temp, "CatchFish2001/tongdantousp.png");
    }
    else if(m_nSelectDantouType == 1)
    {
        sprintf(temp, "CatchFish2001/yindantousp.png");
    }
    else if(m_nSelectDantouType == 2)
    {
        sprintf(temp, "CatchFish2001/jindantousp.png");
    }
    CCSprite* spInfo = CCSprite::create(temp);
    spInfo -> setPosition(ccp(CLIENT_WIDTH/2, 150));
    this -> addChild(spInfo, 20, eDantouInfoTag);
    if(m_nMeRealChairID >= GAME_PLAYER/2)
    {
        spInfo -> setRotation(180);
        spInfo -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT-150));
    }
    spInfo->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(1.0f, 150), CCFadeTo::create(1.0f, 255), NULL)));
}

void GameMainScene::removeBulletCall(CCNode* pNode, void* nType)
{
    long lType = (long)nType;
    pNode->removeFromParent();
    //播放特效和检索鱼
    playParticle("zibao", m_pointDantou.x, m_pointDantou.y);
    SimpleAudioEngine::sharedEngine()->playEffect("Music/dantoubao.wav");
    int nFishID = 0;
    for(int i = 0; i < m_vctFishFactory.size(); i++)
    {
        GameFish* pGameFish = m_vctFishFactory[i];
        if(pGameFish == NULL || pGameFish->getDied() || pGameFish->getAppear())
            continue;
        CCRect rect = pGameFish->m_pFish->boundingBox();
        if(rect.containsPoint(m_pointDantou))
        {
            //向服务端发送这条鱼消息
            nFishID = pGameFish->getFishID();
            break;
        }
    }
    CMD_C_UsePearl UsePearl;
    UsePearl.cbType = lType;
    UsePearl.nFishID = nFishID;
    m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_USE_PEARL,&UsePearl,sizeof(UsePearl));
    if(lType == 0)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameClickTongDantouTag);
    }
    else if(lType == 1)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameClickYinDantouTag);
    }
    else if(lType == 2)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameClickJinDantouTag);
    }
}

void GameMainScene::removeNodeCall(CCNode* pNode)
{
	pNode->removeFromParent();
}

//绘制锁定线
void GameMainScene::drawLockLine(int nChairID, GameFish* pGameFish)
{
    if(pGameFish != NULL)
    {
        CCPoint point = m_barrelLayer[nChairID] -> convertToWorldSpace(m_barrelLayer[nChairID] -> getCannonBg() -> getPosition());
        CCPoint endPoint1 = m_barrelLayer[nChairID]->getCannonBg() ->convertToNodeSpace(pGameFish->m_pFish->getPosition());
        CCPoint fishPoint = pGameFish->m_pFish->getPosition();
        float f = sqrt((point.x-fishPoint.x)*(point.x - fishPoint.x) + (point.y - fishPoint.y)*(point.y - fishPoint.y));
        if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER)
        {
            f = sqrt((point.x-(m_sizeVisible.width-fishPoint.x))*(point.x - (m_sizeVisible.width-fishPoint.x)) + (point.y - (m_sizeVisible.height -fishPoint.y))*(point.y - (m_sizeVisible.height -fishPoint.y)));
        }
        
        m_barrelLayer[nChairID]->getLockNumSprite() ->setVisible(true);
        if(nChairID == m_nMeRealChairID)
        {
            //m_barrelLayer[nChairID]->getLockNumSprite() -> setColor(ccGREEN);
        }
        else
        {
            //m_barrelLayer[nChairID]->getLockNumSprite() -> setColor(ccc3(24,201,255));
        }
        m_barrelLayer[nChairID]->getLockNumSprite() -> setPosition(endPoint1);
        if(nChairID >= GAME_PLAYER/2 && nChairID < GAME_PLAYER && m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER)
        {
            CCPoint numPoint =CCPointMake(m_sizeVisible.width - fishPoint.x,m_sizeVisible.height - fishPoint.y);
            m_barrelLayer[nChairID]->getLockNumSprite() -> setPosition(m_barrelLayer[nChairID]->getCannonBg()->convertToNodeSpace(numPoint));
        }
        else if(nChairID >= 0 && nChairID < GAME_PLAYER/2 && m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER)
        {
            CCPoint numPoint =CCPointMake(m_sizeVisible.width - fishPoint.x,m_sizeVisible.height - fishPoint.y);
            m_barrelLayer[nChairID]->getLockNumSprite() -> setPosition(m_barrelLayer[nChairID]->getCannonBg()->convertToNodeSpace(numPoint));
            m_barrelLayer[nChairID]->getLockNumSprite() -> setRotation(180);
        }
        else if(nChairID >= 0 && nChairID <GAME_PLAYER/2 && m_nMeRealChairID >= 0 && m_nMeRealChairID <GAME_PLAYER/2)
        {
            m_barrelLayer[nChairID]->getLockNumSprite() -> setPosition(endPoint1);
        }
        else if(nChairID >= GAME_PLAYER/2 && nChairID <GAME_PLAYER && m_nMeRealChairID >= 0 && m_nMeRealChairID <GAME_PLAYER/2)
        {
            m_barrelLayer[nChairID]->getLockNumSprite() -> setPosition(endPoint1);
            m_barrelLayer[nChairID]->getLockNumSprite() -> setRotation(180);
        }
        //f是鱼到炮台之间的距离，计算锁定线缩放长度
        float fScaleX = f/m_barrelLayer[nChairID]->getLockLine()->getContentSize().width;
        m_barrelLayer[nChairID]->getLockLine() -> setScaleX(fScaleX);
        m_barrelLayer[nChairID]->getLockLine() -> setScaleY(4+fScaleX);
        m_barrelLayer[nChairID]->getLockLine() -> setVisible(true);
        m_barrelLayer[nChairID]->getLockLine() -> setRotation(m_fRotation[nChairID]-90);
        //m_barrelLayer[nChairID]->getLockLine() -> setColor(ccc3(100,100,100));
    }
    //    if(pGameFish != NULL)
    //    {
    //        CCPoint point = m_barrelLayer[nChairID] -> convertToWorldSpace(m_barrelLayer[nChairID] -> getCannonBg() -> getPosition());
//        CCPoint endPoint1 = m_barrelLayer[nChairID]->getCannonBg() ->convertToNodeSpace(pGameFish->m_pFish->getPosition());
//        CCPoint fishPoint = pGameFish->m_pFish->getPosition();
//        float f = sqrt((point.x-fishPoint.x)*(point.x - fishPoint.x) + (point.y - fishPoint.y)*(point.y - fishPoint.y));
//        if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER)
//        {
//            f = sqrt((point.x-(m_sizeVisible.width-fishPoint.x))*(point.x - (m_sizeVisible.width-fishPoint.x)) + (point.y - (m_sizeVisible.height -fishPoint.y))*(point.y - (m_sizeVisible.height -fishPoint.y)));
//        }
//        
//        m_barrelLayer[nChairID]->getLockNumSprite() ->setVisible(true);
//        m_barrelLayer[nChairID]->getLockNumSprite() -> setPosition(endPoint1);
//        if(nChairID >= GAME_PLAYER/2 && nChairID < GAME_PLAYER && m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER)
//        {
//            CCPoint numPoint =CCPointMake(m_sizeVisible.width - fishPoint.x,m_sizeVisible.height - fishPoint.y);
//            m_barrelLayer[nChairID]->getLockNumSprite() -> setPosition(m_barrelLayer[nChairID]->getCannonBg()->convertToNodeSpace(numPoint));
//        }
//        else if(nChairID >= 0 && nChairID < GAME_PLAYER/2 && m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER)
//        {
//            CCPoint numPoint =CCPointMake(m_sizeVisible.width - fishPoint.x,m_sizeVisible.height - fishPoint.y);
//            m_barrelLayer[nChairID]->getLockNumSprite() -> setPosition(m_barrelLayer[nChairID]->getCannonBg()->convertToNodeSpace(numPoint));
//            m_barrelLayer[nChairID]->getLockNumSprite() -> setRotation(180);
//        }
//        else if(nChairID >= 0 && nChairID <GAME_PLAYER/2 && m_nMeRealChairID >= 0 && m_nMeRealChairID <GAME_PLAYER/2)
//        {
//        }
//        else if(nChairID >= GAME_PLAYER/2 && nChairID <GAME_PLAYER && m_nMeRealChairID >= 0 && m_nMeRealChairID <GAME_PLAYER/2)
//        {
//            m_barrelLayer[nChairID]->getLockNumSprite() -> setRotation(180);
//        }
//        //f是鱼到炮台之间的距离，计算锁定线缩放长度
//        
//        float fScaleX = 0;
//        float fScale = 0;
//        /*if(m_barrelLayer[nChairID]->getVipLevel() == 0)
//        {
//            fScaleX = f/(m_barrelLayer[nChairID]->getLockLine()->getContentSize().width);
//            fScale = 4+fScaleX;
//        }
//        else if(m_barrelLayer[nChairID]->getVipLevel()>=1 && m_barrelLayer[nChairID]->getVipLevel()<=3)
//        {
//            fScaleX = f/(abs)((int)(m_barrelLayer[nChairID]->getLockLine()->getContentSize().width-50));
//            fScale = 1.0;
//        }
//        else if(m_barrelLayer[nChairID]->getVipLevel()>=4 && m_barrelLayer[nChairID]->getVipLevel()<=6)
//        {
//            fScaleX = f/(abs)((int)(m_barrelLayer[nChairID]->getLockLine()->getContentSize().width-140));
//            fScale = 1.0;
//        }
//        else if(m_barrelLayer[nChairID]->getVipLevel()>=7 && m_barrelLayer[nChairID]->getVipLevel()<=9)
//        {
//            fScaleX = f/(abs)((int)(m_barrelLayer[nChairID]->getLockLine()->getContentSize().width-100));
//            fScale = 1.0;
//        }*/
//        fScaleX = f/(abs)((int)(m_barrelLayer[nChairID]->getLockLine()->getContentSize().width-140));
//        fScale = 1.0;
//        m_barrelLayer[nChairID]->getLockLine() -> setScaleX(fScaleX);
//        m_barrelLayer[nChairID]->getLockLine() -> setScaleY(fScale);
//        m_barrelLayer[nChairID]->setLockLinePoint();
//        m_barrelLayer[nChairID]->getLockLine() -> setVisible(true);
//        m_barrelLayer[nChairID]->getLockLine() -> setRotation(m_fRotation[nChairID]-90);//锁定线的角度
//    }
}

int GameMainScene::getMeRealChairID()
{
	return m_nMeRealChairID;
}

BarrelLayer* GameMainScene::getBarrelLayer()
{
	return m_barrelLayer[m_nMeRealChairID];
}

int GameMainScene::getOwnTotalMoney()
{
	return m_nOwnTotalMoney[m_nMeRealChairID];
}

FishWndLayer* GameMainScene::getFishWndLayer()
{
	return m_fishWndLayer;
}

void GameMainScene::readFishPathXML(const char* filePath)
{
	unsigned long size;
	char *pFileContent =(char*)CCFileUtils::sharedFileUtils()->getFileData(filePath, "r", &size);
	TiXmlDocument* myDocument = new TiXmlDocument;
	myDocument->Parse(pFileContent,0,TIXML_ENCODING_UTF8);
	TiXmlElement * rootElement=myDocument->RootElement();
	if (NULL!=rootElement)
	{
		std::string nodename=rootElement->Value();
		if (0!=nodename.compare("Data"))
		{
			return;
		}
		TiXmlElement * pCurrentElement=rootElement->FirstChildElement();
		while (NULL!=pCurrentElement)
		{
			const char * pValue=pCurrentElement->Value();
			if (!strcmp(pValue,"PathIndex"))
			{
				tagPathIndex* pFishPathIndex = new tagPathIndex;
				m_fishPathIndex.push_back(pFishPathIndex);
				TiXmlElement* firstElement = pCurrentElement -> FirstChildElement();
				const char * valueId=firstElement->Value();
				if (!strcmp(valueId,"id"))
				{
					int id=atoi(firstElement->GetText());
					pFishPathIndex->pPathIndexId = id;
				}
				TiXmlElement* secondElement = firstElement -> NextSiblingElement();
				int i = 0;
				while(NULL != secondElement)
				{
					const char* secondValue = secondElement -> Value();
					if(!strcmp(secondValue,"Information"))
					{
						pValue = secondElement->Attribute("xPox");
						if(NULL != pValue)
						{
							pFishPathIndex->pPathIndex[i].xPos = atoi(pValue);	
						}
						pValue = secondElement->Attribute("yPos");
						if(NULL != pValue)
						{
							pFishPathIndex->pPathIndex[i].yPos = atoi(pValue);
						}
						pValue = secondElement->Attribute("rotation");
						if(NULL != pValue)
						{
							pFishPathIndex->pPathIndex[i].rotation = atoi(pValue);
						}
						pValue = secondElement->Attribute("moveTime");
						if(NULL != pValue)
						{
							pFishPathIndex->pPathIndex[i].moveTime = atoi(pValue);
						}
						pValue = secondElement->Attribute("speed");
						if(NULL != pValue)
						{
							pFishPathIndex->pPathIndex[i].speed = atoi(pValue);
						}
					}
					secondElement = secondElement -> NextSiblingElement();
					i++;
				}
			}
			pCurrentElement=pCurrentElement->NextSiblingElement();
		}
	}
	delete myDocument;
}

tagPathIndex* GameMainScene::getFishPathIndex(int id)
{
	tagPathIndex* pPathIndex = NULL;
	for(int i = 0; i < (int)m_fishPathIndex.size(); i++)
	{
		if(id == m_fishPathIndex[i]->pPathIndexId)
		{
			pPathIndex = m_fishPathIndex[i];
			break;
		}
	}
	return pPathIndex;
}

void GameMainScene::setHitFish(int bulletID,int bulletRealID,bool bAndroid, GameFish* fish)
{
	if(fish == NULL) 
		return;
    
//    //宝箱抖动
//    if(fish->getSpriteID() == 1026)
//    {
//        float fRotation = fish->m_pFish->getRotation();
//        CCRotateBy* rotateBy1 = CCRotateBy::create(0.1f, 20);
//        CCRotateBy* rotateBy2 = CCRotateBy::create(0.2f, -40);
//        CCRotateBy* rotateBy3 = CCRotateBy::create(0.1f, 20);
//        CCRepeat* pRepeat = CCRepeat::create(CCSequence::create(rotateBy1, rotateBy2, rotateBy3, NULL), 1);
//        pRepeat -> setTag(301);
//        fish->m_pFish->stopActionByTag(301);
//        fish->m_pFish->setRotation(fRotation);
//        fish->m_pFish->runAction(pRepeat);
//    }
    
	float fXPos = fish->m_pFish->getPositionX();
	float fYPos = fish->m_pFish->getPositionY();
	float tx = fXPos;
	float ty = fYPos;
	int nOtherCount = 0;
	int nOtherFishID[100];
	bool bBomb = false;
	bool bSameDie = false;
	bool bLineFish = false;
	bool bRangeBomb = false;
	memset(nOtherFishID,0,sizeof(nOtherFishID));
	if(fish->isProperty(enBomb)) bBomb = true;
	if(fish->isProperty(enSameDie)) bSameDie = true;
	if(fish->isProperty(enLine)) bLineFish = true;
	if(fish->isProperty(enRangeBomb)) bRangeBomb = true;
	for(unsigned int k = 0; k < m_vctFishFactory.size(); k++)
	{
		GameFish* pGameFish = m_vctFishFactory[k];
		if(NULL == pGameFish || pGameFish->getDied()) continue;
		if(fish->getFishID() == pGameFish->getFishID())  continue;
		bool bFishInScreen = false;
		CCPoint pt = pGameFish->m_pFish->getPosition();
		if(pt.x > 0 && pt.x < m_sizeVisible.width && pt.y > 0 && pt.y < m_sizeVisible.height)
			bFishInScreen = true;
        int nMaxNum = 99;
		if(bBomb && bFishInScreen)
		{
			if(nOtherCount>nMaxNum)
				break;
			nOtherFishID[nOtherCount++] = pGameFish->getFishID();
		}
		if(bSameDie && bFishInScreen)
		{
			if(nOtherCount>nMaxNum)
				break;
			nOtherFishID[nOtherCount++] = pGameFish->getFishID();
		}
		if (bRangeBomb && bFishInScreen)
		{
			float fX = pGameFish->m_pFish->getPositionX() - fXPos;
			float fY = pGameFish->m_pFish->getPositionY() - fYPos;
			if (fX*fX + fY*fY <= 350*350)
			{
				if(nOtherCount>nMaxNum)
					break;
				nOtherFishID[nOtherCount++] = pGameFish->getFishID();
			}
		}
		if(bLineFish && bFishInScreen && fish->getSpriteID() == pGameFish->getSpriteID())
		{
			if(nOtherCount>nMaxNum)
				break;
			nOtherFishID[nOtherCount++] = pGameFish->getFishID();
		}
        if (nOtherCount > 1)
        {
            m_bSameDie[m_nMeRealChairID] = true;
        }
        else
        {
            m_bSameDie[m_nMeRealChairID] = false;
        }
        //鱼变红
        if(!(fish->getPropertyOfFish()&enLine))
        {
            CCTintTo* pTintTo1 = CCTintTo::create(0.0f, 255, 80, 80);
            CCDelayTime* pDelay = CCDelayTime::create(0.35f);
            CCTintTo* pTintTo2 = CCTintTo::create(0.0f, 255, 255, 255);
            CCSequence* pSeq = CCSequence::create(pTintTo1, pDelay, pTintTo2, NULL);
            fish->m_pFish->runAction(pSeq);
        }
        if(fish->getGroupFish())
        {
            CCArray* childFishAry = fish->m_pFish->getChildren();
            if (childFishAry!=NULL)
            {
                for (unsigned int i = 0; i < childFishAry->count(); i++)
                {
                    CCSprite* pChildFish = (CCSprite*)childFishAry->objectAtIndex(i);
                    if (pChildFish != NULL)
                    {
                        CCTintTo* pTintTo1 = CCTintTo::create(0.0f, 255, 80, 80);
                        CCDelayTime* pDelay = CCDelayTime::create(0.35f);
                        CCTintTo* pTintTo2 = CCTintTo::create(0.0f, 255, 255, 255);
                        CCSequence* pSeq = CCSequence::create(pTintTo1, pDelay, pTintTo2, NULL);
                        pChildFish->runAction(pSeq);
                    }
                }
            }
        }
    }
	hitFish(fish->getFishID(),bulletID,bulletRealID,bAndroid,nOtherCount,(int)tx,(int)m_sizeVisible.height-ty,nOtherFishID);
}

//碰撞检测
void GameMainScene::checkCollision(float ft)
{
    for(int i = 0; i < (int)m_arrBullet->count(); i++)
    {
        BulletSprite* bulletSprite = (BulletSprite*)m_arrBullet->objectAtIndex(i);
        if(!bulletSprite->getHave())
            continue;
        float xBulletPos = 0;
        float yBulletPos = 0;
        float widthBullet = 96;
        float heightBullet = 54;
        if(bulletSprite->getType() == TwoFishNet || bulletSprite->getType() == TwoSuperFishNet)
        {
            xBulletPos = 46;//26
            widthBullet = 50;//70
        }
        if(bulletSprite->getType() == ThreeFishNet || bulletSprite->getType() == ThreeSuperFishNet)
        {
            xBulletPos = 46;//11
            widthBullet = 50;//83
        }
        if(bulletSprite->getType() == FourFishNet || bulletSprite->getType() == FourSuperFishNet)
        {
            xBulletPos = 0;
            widthBullet = 96;
        }
        

		if(m_pClientKernel->GetUserInfo(bulletSprite->getRealChairID()) == NULL)
		{
			for(unsigned int k = 0; k < m_vctFishFactory.size(); k++)
			{
				GameFish* fish = m_vctFishFactory[k];
				if(fish == NULL || fish->getDied() || fish->getAppear())
					continue;
                if(!fish->m_pFish->boundingBox().intersectsRect(bulletSprite->boundingBox()))
                {
                    continue;
                }
                CCPoint pt = bulletSprite->convertToWorldSpace(ccp(xBulletPos,yBulletPos));
                m_Obb1->setVertex(0, pt.x, pt.y);
                pt = bulletSprite->convertToWorldSpace(ccp(widthBullet,yBulletPos));
                m_Obb1->setVertex(1, pt.x, pt.y);
                
                pt = bulletSprite->convertToWorldSpace(ccp(widthBullet,heightBullet));
                m_Obb1->setVertex(2, pt.x, pt.y);
                pt = bulletSprite->convertToWorldSpace(ccp(xBulletPos,heightBullet));
                m_Obb1->setVertex(3, pt.x, pt.y);
                float width = fish->getWidth();
				float height = fish->getHeight();
				float xPos = fish->getXPos();
				float yPos = fish->getYPos();
				pt = fish->m_pFish->convertToWorldSpace(ccp(xPos,yPos));
				m_Obb2->setVertex(0, pt.x, pt.y);
				pt = fish->m_pFish->convertToWorldSpace(ccp(xPos,height));
				m_Obb2->setVertex(1,pt.x, pt.y);
				pt = fish->m_pFish->convertToWorldSpace(ccp(width,height));
				m_Obb2->setVertex(2, pt.x, pt.y);
				pt = fish->m_pFish->convertToWorldSpace(ccp(width,yPos));
				m_Obb2->setVertex(3, pt.x, pt.y);

                if(m_Obb1->isCollidWithOBB(m_Obb2))
                {
                    if(!bulletSprite -> getHave())
                        continue;
                    setHitFish(bulletSprite->getBulletID(),bulletSprite->getRealChairID(),bulletSprite->getAndroid(),fish);
                    
                    createFishNet(bulletSprite->getPosition(),bulletSprite -> getBulletRotation(),bulletSprite -> getType(), bulletSprite->getMultiple(), bulletSprite->getRealChairID(), bulletSprite->getAttribute());
                    
                    bulletSprite -> setVisible(false);
                    bulletSprite -> setIsStop(true);
                    bulletSprite -> setHave(false);
                    m_nEveryBullet[bulletSprite->getRealChairID()]--;
                    if(bulletSprite->getRealChairID() == m_nMeRealChairID)
                    {
                        if (m_bUserSuperPao[m_nMeRealChairID])
                            playSoundByID(true, 39);
                        else
                            playSoundByID(true, 12);
                        m_nCurrentBulletCount--;
                    }
                    break;
                }
			}
		}
		else
		{
			if(m_bLockFishID[bulletSprite->getRealChairID()])//如果有玩家是锁定状态
            {
				bool bFishExist = false;
				for(unsigned int k = 0; k < m_vctFishFactory.size(); k++)
				{
					GameFish* fish = m_vctFishFactory[k];
					if (fish==NULL || fish->getDied() || fish->getAppear())
						continue;
					if(m_nLockFishID[bulletSprite->getRealChairID()] == fish->getFishID())
					{
						bFishExist = true;
						break;
					}
				}
				if (bFishExist)
				{
					for(unsigned int k = 0; k < m_vctFishFactory.size(); k++)
					{
						GameFish* fish = m_vctFishFactory[k];
						if(fish == NULL)
						{
							m_bLockFishID[bulletSprite->getRealChairID()] = false;
							m_nLockFishID[bulletSprite->getRealChairID()] = 0;
							removeLockLine(bulletSprite->getRealChairID());
							continue;
						}
						if(fish->getDied())
							continue;
						if(m_nLockFishID[bulletSprite->getRealChairID()] == fish->getFishID())
						{
                            if(!fish->m_pFish->boundingBox().intersectsRect(bulletSprite->boundingBox()))
                            {
                                continue;
                            }
                            CCPoint pt = bulletSprite->convertToWorldSpace(ccp(xBulletPos,yBulletPos));
                            m_Obb1->setVertex(0, pt.x, pt.y);
                            pt = bulletSprite->convertToWorldSpace(ccp(widthBullet,yBulletPos));
                            m_Obb1->setVertex(1, pt.x, pt.y);
                            
                            pt = bulletSprite->convertToWorldSpace(ccp(widthBullet,heightBullet));
                            m_Obb1->setVertex(2, pt.x, pt.y);
                            pt = bulletSprite->convertToWorldSpace(ccp(xBulletPos,heightBullet));
                            m_Obb1->setVertex(3, pt.x, pt.y);
                            float width = fish->getWidth();
							float height = fish->getHeight();
							float xPos = fish->getXPos();
							float yPos = fish->getYPos();
							pt = fish->m_pFish->convertToWorldSpace(ccp(xPos,yPos));
							m_Obb2->setVertex(0, pt.x, pt.y);
							pt = fish->m_pFish->convertToWorldSpace(ccp(xPos,height));
							m_Obb2->setVertex(1,pt.x, pt.y);
							pt = fish->m_pFish->convertToWorldSpace(ccp(width,height));
							m_Obb2->setVertex(2, pt.x, pt.y);
							pt = fish->m_pFish->convertToWorldSpace(ccp(width,yPos));
							m_Obb2->setVertex(3, pt.x, pt.y);
                            if(m_Obb1->isCollidWithOBB(m_Obb2))
                            {
                                if(!bulletSprite -> getHave())
                                    continue;
                                setHitFish(bulletSprite->getBulletID(),bulletSprite->getRealChairID(),bulletSprite->getAndroid(),fish);
                                //bulletSprite->getPosition()
                                createFishNet(fish->m_pFish->getPosition(),bulletSprite -> getBulletRotation(),bulletSprite -> getType(), bulletSprite->getMultiple(), bulletSprite->getRealChairID(), bulletSprite->getAttribute());
                                bulletSprite -> setVisible(false);
                                bulletSprite -> setIsStop(true);
                                bulletSprite -> setHave(false);
                                m_nEveryBullet[bulletSprite->getRealChairID()]--;
                                if(bulletSprite->getRealChairID() == m_nMeRealChairID)
                                {
                                    if (m_bUserSuperPao[m_nMeRealChairID])
                                        playSoundByID(true, 39);
                                    else
                                        playSoundByID(true, 12);
                                    m_nCurrentBulletCount--;
                                }
                                break;
                            }
						}
					}
				}
				else
				{
					removeLockLine(bulletSprite->getRealChairID());
				}
			}
			else
			{
				for(unsigned int k = 0; k < m_vctFishFactory.size(); k++)
				{
					GameFish* fish = m_vctFishFactory[k];
					if(fish == NULL || fish->getDied() || fish->getAppear())
						continue;
                    if(!fish->m_pFish->boundingBox().intersectsRect(bulletSprite->boundingBox()))
                    {
                        continue;
                    }
                    CCPoint pt = bulletSprite->convertToWorldSpace(ccp(xBulletPos,yBulletPos));
                    m_Obb1->setVertex(0, pt.x, pt.y);
                    pt = bulletSprite->convertToWorldSpace(ccp(widthBullet,yBulletPos));
                    m_Obb1->setVertex(1, pt.x, pt.y);
                    
                    pt = bulletSprite->convertToWorldSpace(ccp(widthBullet,heightBullet));
                    m_Obb1->setVertex(2, pt.x, pt.y);
                    pt = bulletSprite->convertToWorldSpace(ccp(xBulletPos,heightBullet));
                    m_Obb1->setVertex(3, pt.x, pt.y);
                    float width = fish->getWidth();
					float height = fish->getHeight();
					float xPos = fish->getXPos();
					float yPos = fish->getYPos();
					pt = fish->m_pFish->convertToWorldSpace(ccp(xPos,yPos));
					m_Obb2->setVertex(0, pt.x, pt.y);
					pt = fish->m_pFish->convertToWorldSpace(ccp(xPos,height));
					m_Obb2->setVertex(1,pt.x, pt.y);
					pt = fish->m_pFish->convertToWorldSpace(ccp(width,height));
					m_Obb2->setVertex(2, pt.x, pt.y);
					pt = fish->m_pFish->convertToWorldSpace(ccp(width,yPos));
					m_Obb2->setVertex(3, pt.x, pt.y);
                    
					if(m_Obb1->isCollidWithOBB(m_Obb2))
					{
						if(!bulletSprite -> getHave())
							continue;
						setHitFish(bulletSprite->getBulletID(),bulletSprite->getRealChairID(),bulletSprite->getAndroid(),fish);
                        createFishNet(bulletSprite->getPosition(),bulletSprite -> getBulletRotation(),bulletSprite -> getType(), bulletSprite->getMultiple(), bulletSprite->getRealChairID(), bulletSprite->getAttribute());
                        bulletSprite -> setVisible(false);
                        bulletSprite -> setIsStop(true);
                        bulletSprite -> setHave(false);
                        m_nEveryBullet[bulletSprite->getRealChairID()]--;
                        if(bulletSprite->getRealChairID() == m_nMeRealChairID)
                        {
                            if (m_bUserSuperPao[m_nMeRealChairID])
                                playSoundByID(true, 39);
                            else
                                playSoundByID(true, 12);
                            m_nCurrentBulletCount--;
                        }
						break;
					}
				}
			}
		}
	}
}

void GameMainScene::update(float dt)
{
    //子弹运动
    bool bBullet = false;
    for(int i = 0; i < (int)m_arrBullet -> count(); i++)
    {
        BulletSprite* bulletSprite = (BulletSprite*)m_arrBullet -> objectAtIndex(i);
        if(bulletSprite->getHave())
        {
            CCPoint point = convertToWorldSpace(bulletSprite -> getPosition());
            if(point.x > m_sizeVisible.width || point.x < 0)
            {
                float rotation = (360 - bulletSprite -> getBulletRotation());
                bulletSprite -> setBulletRotation(rotation);
                bulletSprite -> setRotation(rotation);
            }
            if(point.y > m_sizeVisible.height || point.y < 0)
            {
                float rotation = (180 - bulletSprite -> getBulletRotation());
                bulletSprite -> setBulletRotation(rotation);
                bulletSprite -> setRotation(rotation);
            }
            float m_x = bulletSprite -> getBulletSpeed()*sin(bulletSprite ->getBulletRotation()/(180/M_PI));
            float m_y = bulletSprite -> getBulletSpeed()*cos(bulletSprite ->getBulletRotation()/(180/M_PI));
            bulletSprite -> setPosition(ccp(bulletSprite -> getPositionX()+m_x,bulletSprite -> getPositionY()+m_y));
            bBullet = true;
        }
    }
    if(bBullet)
        checkCollision(0);
    
    for(int j = 0; j < GAME_PLAYER; j++)
	{	
		if(m_nLockFishID[j] == 0)  continue;
		for(unsigned int i = 0; i < m_vctFishFactory.size(); i++)
		{
			GameFish* pGameFish = m_vctFishFactory[i];
			if(pGameFish == NULL || pGameFish->getDied() || pGameFish->getAppear())
			continue;
			if(m_nLockFishID[j] == pGameFish->getFishID())
			{
				CCPoint point = ccp(m_barrelLayer[j]->getCannonBg()->boundingBox().origin.x + m_barrelLayer[j]->getCannonBg()->boundingBox().size.width/2,m_barrelLayer[j]->getCannonBg()->boundingBox().origin.y + m_barrelLayer[j]->getCannonBg()->boundingBox().size.height/2);
				CCPoint m_ptStart1 = m_barrelLayer[j] ->convertToWorldSpace(point);
				CCPoint m_ptEnd1 = pGameFish->m_pFish->getPosition();
				if(j >= 0 && j <GAME_PLAYER/2 && m_nMeRealChairID >=0 && m_nMeRealChairID <GAME_PLAYER/2)
				{
					m_ptEnd1 = pGameFish->m_pFish->getPosition();
				}
				else if(j >= GAME_PLAYER/2 && j <GAME_PLAYER && m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID <GAME_PLAYER)
				{
					m_ptEnd1 = CCPointMake(m_sizeVisible.width - m_ptEnd1.x,m_sizeVisible.height - m_ptEnd1.y);
				}
				else if(j >= 0 && j <GAME_PLAYER/2 && m_nMeRealChairID >=GAME_PLAYER/2 && m_nMeRealChairID <GAME_PLAYER)
				{
					m_ptEnd1 = pGameFish->m_pFish->getPosition();
					m_ptStart1 = CCPointMake(m_sizeVisible.width - m_ptStart1.x,m_sizeVisible.height - m_ptStart1.y);
				}
				else if(j >= GAME_PLAYER/2 && j <GAME_PLAYER && m_nMeRealChairID >= 0 && m_nMeRealChairID <GAME_PLAYER/2)
				{
					m_ptEnd1 = CCPointMake(m_sizeVisible.width - m_ptEnd1.x,m_sizeVisible.height - m_ptEnd1.y);
					m_ptStart1 = CCPointMake(m_sizeVisible.width - m_ptStart1.x,m_sizeVisible.height - m_ptStart1.y);
				}
				m_fRotation[j] = getBarrelRotation(m_ptStart1, m_ptEnd1);
				m_barrelLayer[j]->getBarrel() ->setRotation(m_fRotation[j]);

				for(int k = 0; k < (int)m_arrBullet->count(); k++)
				{
					BulletSprite* bullet = (BulletSprite*)m_arrBullet->objectAtIndex(k);
					if(!bullet->getHave())
						continue;
					if(bullet->getRealChairID() == j)
					{
						float xPos,yPos,rotation;
						if(bullet->getPositionX() != pGameFish->m_pFish->getPositionX())
						{
							xPos = pGameFish->m_pFish->getPositionX() - bullet->getPositionX();
							yPos = pGameFish->m_pFish->getPositionY() - bullet->getPositionY();
							rotation = atan2(xPos,yPos)*(180/M_PI);
						}
						else
						{
							if(bullet->getPositionY() < pGameFish->m_pFish->getPositionY())
							{
								rotation = 0;
							}
							else
							{
								rotation = 180;
							}
						}
						bullet -> setRotation(rotation);
						bullet->setBulletRotation(rotation);
					}
				}
				drawLockLine(j,pGameFish);
				if(pGameFish->m_pFish->getPositionX()>m_sizeVisible.width || pGameFish->m_pFish->getPositionX()<0 ||
					pGameFish->m_pFish->getPositionY()>m_sizeVisible.height|| pGameFish->m_pFish->getPositionY()<0 || m_bFishingOffSeason == true)
				{
					removeLockLine(j);
				}
			}
		}
	}
	for(int i = 0; i < GAME_PLAYER; i++)
	{
		if(!m_bLockFishID[i])
		{
			removeLockLine(i);
		}
	}
	for(unsigned int i = 0; i < m_vctFishFactory.size(); i++)
	{
		GameFish* gameFish = m_vctFishFactory[i];
		if(gameFish == NULL || gameFish->getDied() || gameFish->getAppear()) continue;
		if(gameFish->m_pFish->getPositionX() < m_sizeVisible.width+gameFish->m_pFish->getContentSize().width/2 && gameFish->m_pFish->getPositionX() > -gameFish->m_pFish->getContentSize().width/2 && 
			gameFish->m_pFish->getPositionY() < m_sizeVisible.height+gameFish->m_pFish->getContentSize().height/2 && gameFish->m_pFish->getPositionY() > -gameFish->m_pFish->getContentSize().height/2)
		{
			gameFish->setHave(true);
		}
	}
	for(unsigned int i = 0; i < m_vctFishFactory.size(); i++)
	{
		GameFish* gameFish = m_vctFishFactory[i];
		if(gameFish == NULL || gameFish->getDied()|| gameFish->getAppear()) continue;
		if(gameFish->getHave())
		{
			if(gameFish->m_pFish->getPositionX() > m_sizeVisible.width+gameFish->m_pFish->getContentSize().width/2 || gameFish->m_pFish->getPositionX() < -gameFish->m_pFish->getContentSize().width/2 || 
				gameFish->m_pFish->getPositionY() > m_sizeVisible.height+gameFish->m_pFish->getContentSize().height/2|| gameFish->m_pFish->getPositionY() < -gameFish->m_pFish->getContentSize().height/2)
			{
                if(KIND_ID == YQS_KIND_ID)
                {
                    if(gameFish->getSpriteID()>=1016 || gameFish->isProperty(enLine))
                    {
                        for(int k = 0; k < m_arrLockFish->count(); k++)
                        {
                            CCSprite* pLockDi = (CCSprite*)m_arrLockFish->objectAtIndex(k);
                            if(pLockDi == NULL) continue;
                            if(pLockDi->getTag() == gameFish->getFishID())
                            {
                                m_arrLockFish -> removeObject(pLockDi);
                                pLockDi -> removeFromParentAndCleanup(true);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if(gameFish->getSpriteID()>=1017 || gameFish->isProperty(enLine))
                    {
                        for(int j = 0; j < m_arrLockFish->count(); j++)
                        {
                            CCSprite* pLockDi = (CCSprite*)m_arrLockFish->objectAtIndex(j);
                            if(pLockDi == NULL) continue;
                            if(pLockDi->getTag() == gameFish->getFishID())
                            {
                                m_arrLockFish -> removeObject(pLockDi);
                                pLockDi -> removeFromParentAndCleanup(true);
                                break;
                            }
                        }
                    }
                }
                for(int j = 0; j < m_arrQipao->count(); j++)
                {
                    CCSprite* pQipao = (CCSprite*)m_arrQipao->objectAtIndex(j);
                    if(pQipao == NULL) continue;
                    if(pQipao->getTag() == gameFish->getFishID())
                    {
                        pQipao -> setVisible(false);
//                        m_arrQipao -> removeObject(pQipao);
//                        pQipao -> removeFromParentAndCleanup(true);
                        break;
                    }
                    
                }
                
				for (std::vector<GameFish*>::iterator it = m_vctFishFactory.begin(); it != m_vctFishFactory.end(); )
				{
					if (*it == gameFish)
					{
						gameFish->setHave(false);
						it = m_vctFishFactory.erase(it);
						addChildToBatch(false, gameFish->m_pFish, gameFish->getFilePath());
						delete gameFish;
						gameFish = NULL;
						break;
					}
					else
						it++;
				}
                
			}
		}
	}
	for(unsigned int i = 0; i < m_vctFishFactory.size(); i++)
	{
		if(m_bFreeze)	
			break;
		GameFish* gameFish = m_vctFishFactory[i];
		if(gameFish == NULL)
			continue;
		if(!gameFish->getDied())
		{
			if (gameFish->isProperty(enBoss))
			{
				if (gameFish->getBossScore()>0)
				{
					char temp[10];
					sprintf(temp,"%d",gameFish->getBossScore());
					for(unsigned int k = 0; k < m_labelBossBeilv.size(); k++)
					{
						if(m_labelBossBeilv[k]->getBossTag() == gameFish->getFishID())
						{
							m_labelBossBeilv[k] -> setString(temp);
							m_labelBossBeilv[k] -> setPosition(gameFish->m_pFish->getPosition());
							m_labelBossBeilv[k] -> setRotation(gameFish->m_pFish->getRotation());
							break;
						}
					}
				}
			}
            if(KIND_ID == YQS_KIND_ID)
            {
                if(gameFish->getSpriteID()>=1016 || gameFish->isProperty(enLine))
                {
                    for(int k = 0; k < m_arrLockFish->count(); k++)
                    {
                        CCSprite* pLockDi = (CCSprite*)m_arrLockFish->objectAtIndex(k);
                        if(pLockDi == NULL) continue;
                        if(pLockDi->getTag() == gameFish->getFishID())
                        {
                            pLockDi -> setPosition(gameFish->m_pFish->getPosition());
                            pLockDi -> setRotation(gameFish->m_pFish->getRotation());
                            break;
                        }
                    }
                }
            }
            else
            {
                if(gameFish->getSpriteID()>=1017 || gameFish->isProperty(enLine))
                {
                    for(int k = 0; k < m_arrLockFish->count(); k++)
                    {
                        CCSprite* pLockDi = (CCSprite*)m_arrLockFish->objectAtIndex(k);
                        if(pLockDi == NULL) continue;
                        if(pLockDi->getTag() == gameFish->getFishID())
                        {
                            pLockDi -> setPosition(gameFish->m_pFish->getPosition());
                            pLockDi -> setRotation(gameFish->m_pFish->getRotation());
                            break;
                        }
                    }
                }
            }
            for(int k = 0; k < m_arrQipao->count(); k++)
            {
                CCSprite* pQipao = (CCSprite*)m_arrQipao->objectAtIndex(k);
                if(pQipao == NULL) continue;
                if(pQipao->getTag() == gameFish->getFishID())
                {
                    pQipao -> setPosition(gameFish->m_pFish->getPosition());
                    break;
                }
            }
            
			gameFish->moveFish();
			gameFish->changePath();
		}
	}
}

//机器人锁定
void GameMainScene::androidLock(float dt)
{
	int m_SmallChairID = 0;
	m_bAndroid[m_nMeRealChairID] = false;
	for(int i = 0; i < GAME_PLAYER; i++)
	{
		if(!m_bHaveUser[i])  continue;
		if(!m_bAndroid[i])
		{
			m_SmallChairID = m_nRealChairID[i];
			break;
		}
	}
	if(m_nMeRealChairID == m_SmallChairID)
	{
		for(int i = 0; i < GAME_PLAYER; i++)
		{
			if(!m_bHaveUser[i]) continue;
			if(m_nLockFishID[i] != 0) continue;
			if(i == m_nMeRealChairID)  continue;
			for(unsigned int j = 0; j < m_vctFishFactory.size(); j++)
			{
				if(m_nSuoDingFishIndex >= (int)m_vctFishFactory.size())
				{
					m_nSuoDingFishIndex = 0;
				}
				GameFish* pGameFish = m_vctFishFactory[m_nSuoDingFishIndex++];
				if(pGameFish == NULL || pGameFish->getDied())
					continue;
				if(pGameFish->getMultiple()>=10)
				{
					CCPoint point = pGameFish->m_pFish->getPosition();
					if(point.x >0 && point.y > 0 && point.x < m_sizeVisible.width && point.y < m_sizeVisible.height)
					{
						sendLockFishID(pGameFish->getFishID(),m_nRealChairID[i]);
						break;
					}
				}
			}
		}
	}
}

//延迟鱼
void GameMainScene::createFish(float dt)
{
	for(int i = 0; i < (int)m_vctFishStorg.size(); i++)
	{
		tagFishPack* pFishPack = m_vctFishStorg[i];
		if(NULL == pFishPack)
			continue;
		if(pFishPack->nCreateDelayTime>0)
			pFishPack->nCreateDelayTime--;
		if(pFishPack->nCreateDelayTime == 0)
		{
			addFish(pFishPack);
			pFishPack->nCreateDelayTime = -1;
		}
	}
	for(int i = 0; i < (int)m_vctFishStorg.size();)
	{
		tagFishPack* pFishPack = m_vctFishStorg[i];
		if(NULL == pFishPack)//修改死循环问题
        {
            i++;
			continue;
        }
		if(pFishPack->nCreateDelayTime<0)
		{
			m_vctFishStorg.erase(m_vctFishStorg.begin()+i);
			delete pFishPack;
			pFishPack = NULL;
		}
        else
        {
            i++;
        }
	}
	if(m_bFishingOffSeason)
	{
		for(unsigned int i = 0; i < m_vctFishStorg.size(); i++)
		{
			delete m_vctFishStorg[i];
			m_vctFishStorg[i] = NULL;
		}
		m_vctFishStorg.clear();
	}
}

//增加鱼
void GameMainScene::addFish(tagFishPack* pFishPack)
{
    if (pFishPack == NULL) return;
    if (m_bWaiting && m_spWait!=NULL)
    {
        m_bWaiting = false;
        m_spWait->setVisible(false);
        m_spWait->removeFromParent();
    }
    GameFish* pGameFish = createFishByID(pFishPack->nSpriteID+1000);
    if (pGameFish!=NULL)
    {
        tagPathIndex* fishPath = getFishPathIndex(pFishPack->nPathIndex1);
        float fXPos,fYPos,fRotation;
        int nMoveTime,nSpeed;
        if(pFishPack->nPathIndex1>=0)
        {
            fXPos = (float)fishPath->pPathIndex[0].xPos+pFishPack->nXPos;
            fYPos = (float)fishPath->pPathIndex[0].yPos+pFishPack->nYPos;
            fRotation = (float)fishPath->pPathIndex[0].rotation;
            nMoveTime = fishPath->pPathIndex[0].moveTime;
            nSpeed = fishPath->pPathIndex[0].speed;
        }
        else
        {
            fXPos=(float)pFishPack->nXPos;
            fYPos=(float)pFishPack->nYPos;
            fRotation=pFishPack->fRotation[0];
            nMoveTime=pFishPack->nMoveTime[0];
            nSpeed=pFishPack->nSpeed[0];
            for (int i=0;i<5;i++)
            {
                pGameFish->setRotationArray(i,pFishPack->fRotation[i]);
                pGameFish->setMoveTimeArray(i,pFishPack->nMoveTime[i]);
                pGameFish->setSpeedArray(i,pFishPack->nSpeed[i]);
            }
        }
        
        pGameFish->initData(fishPath);
        pGameFish->setPropertyOfFish(pFishPack->nProperty);
        pGameFish->setSpriteID(pFishPack->nSpriteID+1000);
        pGameFish->setFishID(pFishPack->nFishID);
        pGameFish->setMultiple(pFishPack->nMultiple);
        pGameFish->m_pFish->setPosition(ccp(fXPos, m_sizeVisible.height-fYPos));
        addChildToBatch(true, pGameFish->m_pFish, pGameFish->getFilePath());
        m_vctFishFactory.push_back(pGameFish);
        
        //红鱼和BOSS倍率显示
        if(pGameFish->isProperty(enLine))
        {
            pGameFish->m_pFish->setColor(ccc3(255,0,0));
        }
        else if (pGameFish->isProperty(enBoss))//boss
        {
            //倍率标签添加到了this上，如果要获取这个标签与BOSS相对应的话，使用tag值
            BossBeilv* label = BossBeilv::create("","CatchFish01/goldNum4.png",54,54,'0');
            label -> setAnchorPoint(ccp(0.5f,0.5f));
            label -> setBossTag(pGameFish->getFishID());
            this->addChild(label, 2);
            label -> setPosition(ccp(720,450));
            if (m_nMeRealChairID>=GAME_PLAYER/2)
                label->setRotation(180.0f);
            m_labelBossBeilv.push_back(label);
        }
        
        if(KIND_ID == YQS_KIND_ID)
        {
            //添加锁定光圈
            if(pGameFish->getSpriteID() == 1022 || pGameFish->getSpriteID() == 1023)//凤凰、四不像
            {
                CCSprite* pLockDi = CCSprite::createWithSpriteFrameName("suodingfish3.png");
                pLockDi->setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
                m_fishLayer->addChild(pLockDi, 2, pGameFish->getFishID());
                m_arrLockFish -> addObject(pLockDi);
                if(!m_bIsBaojiStatus)
                {
                    pLockDi -> setVisible(false);
                }
            }
            else if(pGameFish->getSpriteID() == 1018 || pGameFish->getSpriteID() == 1019 || pGameFish->getSpriteID() == 1020 || pGameFish->getSpriteID() == 1021 || pGameFish->getSpriteID() == 1025 || pGameFish->getSpriteID() == 1026 || pGameFish->getSpriteID() == 1027)//绿鳖、宝箱、金龙、金马、倚天剑、全屏炸弹、金鳖
            {
                CCSprite* pLockDi = CCSprite::createWithSpriteFrameName("suodingfish2.png");
                pLockDi->setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
                m_fishLayer->addChild(pLockDi, 2, pGameFish->getFishID());
                m_arrLockFish -> addObject(pLockDi);
                if(!m_bIsBaojiStatus)
                {
                    pLockDi -> setVisible(false);
                }
            }
            else if(pGameFish->getSpriteID() == 1017 || pGameFish->getSpriteID() == 1024 || (pGameFish->getSpriteID()>=1028 && pGameFish->getSpriteID()<=1036) || pGameFish->isProperty(enLine) || pGameFish->getSpriteID() == 1037)//银鳖、屠龙刀、组合鱼、红鱼、五福鱼
            {
                CCSprite* pLockDi = CCSprite::createWithSpriteFrameName("suodingfish1.png");
                pLockDi->setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
                m_fishLayer->addChild(pLockDi, 2, pGameFish->getFishID());
                m_arrLockFish -> addObject(pLockDi);
                if(!m_bIsBaojiStatus)
                {
                    pLockDi -> setVisible(false);
                }
            }
        }
        else if(KIND_ID == NZNH_KIND_ID)
        {
            if(pGameFish->getSpriteID() == 1022 || pGameFish->getSpriteID() == 1024 || pGameFish->getSpriteID() == 1021 || pGameFish->getSpriteID() == 1026)//青龙、乾坤袋、三太子、龙王
            {
                CCSprite* pLockDi = CCSprite::createWithSpriteFrameName("suodingfish4.png");
                pLockDi->setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
                m_fishLayer->addChild(pLockDi, 2, pGameFish->getFishID());
                m_arrLockFish -> addObject(pLockDi);
                if(!m_bIsBaojiStatus)
                {
                    pLockDi -> setVisible(false);
                }
            }
            else if((pGameFish->getSpriteID()>=1030 && pGameFish->getSpriteID()<=1041) || pGameFish->isProperty(enLine) || pGameFish->getSpriteID() == 1042)//组合鱼、红鱼、五福鱼
            {
                CCSprite* pLockDi = CCSprite::createWithSpriteFrameName("suodingfish1.png");
                pLockDi->setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
                m_fishLayer->addChild(pLockDi, 2, pGameFish->getFishID());
                m_arrLockFish -> addObject(pLockDi);
                if(!m_bIsBaojiStatus)
                {
                    pLockDi -> setVisible(false);
                }
            }
            else if(pGameFish->getSpriteID() == 1017 || pGameFish->getSpriteID() == 1018 || pGameFish->getSpriteID() == 1019 || pGameFish->getSpriteID() == 1020 || pGameFish->getSpriteID() == 1025 || pGameFish->getSpriteID() == 1027 || pGameFish->getSpriteID() == 1028 || pGameFish->getSpriteID() == 1023 || pGameFish->getSpriteID() == 1029)//虾兵、龟丞相、蟹将、莲花、水草、全屏炸弹、半屏炸弹、哪吒、定屏
            {
                CCSprite* pLockDi = CCSprite::createWithSpriteFrameName("suodingfish2.png");
                pLockDi->setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
                m_fishLayer->addChild(pLockDi, 2, pGameFish->getFishID());
                m_arrLockFish -> addObject(pLockDi);
                if(!m_bIsBaojiStatus)
                {
                    pLockDi -> setVisible(false);
                }
            }
        }
        else
        {
            //添加锁定光圈
            if(pGameFish->isProperty(enBoss) || pGameFish->getSpriteID() == 1024 || pGameFish->getSpriteID() == 1025 || pGameFish->getSpriteID() == 1021)//BOSS、半屏炸弹、全屏炸弹、白虎
            {
                CCSprite* pLockDi = CCSprite::createWithSpriteFrameName("suodingfish3.png");
                pLockDi->setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
                this->addChild(pLockDi, 2, pGameFish->getFishID());
                m_arrLockFish -> addObject(pLockDi);
                if(!m_bIsBaojiStatus)
                {
                    pLockDi -> setVisible(false);
                }
            }
            if(pGameFish->getSpriteID() == 1020)//麒麟
            {
                CCSprite* pLockDi = CCSprite::createWithSpriteFrameName("suodingfish4.png");
                pLockDi->setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
                this->addChild(pLockDi, 2, pGameFish->getFishID());
                m_arrLockFish -> addObject(pLockDi);
                if(!m_bIsBaojiStatus)
                {
                    pLockDi -> setVisible(false);
                }
            }
            else if((pGameFish->getSpriteID()>=1040 && pGameFish->getSpriteID()<=1045) || pGameFish->isProperty(enLine) || (pGameFish->getSpriteID()>=1030 && pGameFish->getSpriteID()<=1037) || pGameFish->getSpriteID() == 1023)//组合鱼、红鱼、定屏
            {
                CCSprite* pLockDi = CCSprite::createWithSpriteFrameName("suodingfish1.png");
                pLockDi->setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
                this->addChild(pLockDi, 2, pGameFish->getFishID());
                m_arrLockFish -> addObject(pLockDi);
                if(!m_bIsBaojiStatus)
                {
                    pLockDi -> setVisible(false);
                }
            }
            else if(pGameFish->getSpriteID() == 1017 || pGameFish->getSpriteID() == 1018 || pGameFish->getSpriteID() == 1019 || pGameFish->getSpriteID() == 1026 || pGameFish->getSpriteID() == 1027)//金蟾绿蟾、黄金鲨、银龙、金龙
            {
                CCSprite* pLockDi = CCSprite::createWithSpriteFrameName("suodingfish2.png");
                pLockDi->setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
                this->addChild(pLockDi, 2, pGameFish->getFishID());
                m_arrLockFish -> addObject(pLockDi);
                if(!m_bIsBaojiStatus)
                {
                    pLockDi -> setVisible(false);
                }
            }
        }
    }
}

void GameMainScene::addStorgeFish(tagFishPack* pFishPack)
{
	if (pFishPack == NULL) return;
	if (m_bWaiting && m_spWait!=NULL)
	{
		m_bWaiting = false;
		m_spWait->setVisible(false);
		m_spWait->removeFromParent();
	}
	tagFishPack* pFishPack2 = new tagFishPack;
	memcpy(pFishPack2,pFishPack,sizeof(tagFishPack));
	m_vctFishStorg.push_back(pFishPack2);
}

void GameMainScene::setAutoFire()
{
	if((m_bAutoFire) && m_bSpeedUp)
	{
		this -> schedule(schedule_selector(GameMainScene::startBullet),SPEED_NORMAL);//SPEED_UP
	}
	else if(m_bAutoFire)
	{
		this -> schedule(schedule_selector(GameMainScene::startBullet),SPEED_NORMAL);
	}
}

CCArray* GameMainScene::getMenuArray()
{
	return m_arrMenu;
}

void GameMainScene::setCellScore(int nCellScore)
{
	m_nCellScore = nCellScore;
}

int GameMainScene::getCellScore()
{
    return m_nCellScore;
}

void GameMainScene::setMaxBullet(int nMaxBullet)
{
	m_nMaxBullet = nMaxBullet;
}

void GameMainScene::setMeInformation(int nPlayerID, int nMeRealID)
{
	m_nMeRealChairID = nPlayerID;
	m_nMeRealChairID = nMeRealID;
}

void GameMainScene::setBeiLv(WORD wChairID, int nScore)
{
    int beilv = m_nBeiLv[wChairID];
    int nExchangeTimes = 0;
    if (m_nBeiLv[wChairID] == 0)
    {
        if (nScore != m_nCellScore)
        {
            if (nScore>=m_nCellScore && nScore <= m_nExchargeTimes/2*m_nCellScore)
            {
                m_barrelLayer[wChairID]->changeBarrel(2);
            }
            else if (nScore > nExchangeTimes*m_nCellScore && nScore <= m_nExchargeTimes*m_nCellScore)
            {
                m_barrelLayer[wChairID]->changeBarrel(3);
            }
//            else if (nScore >= 6*m_nCellScore && nScore <= 10*m_nCellScore)
//            {
//                m_barrelLayer[wChairID]->changeBarrel(4);
//            }
        }
        
    }
    m_nBeiLv[wChairID]=nScore;
    char blv[20];
    sprintf(blv,"%d",nScore);
    m_barrelLayer[wChairID]->getMultAtlas()->setString(blv);
    //切换炮筒
    int nIndex=0;
    if (m_nBeiLv[wChairID]<=m_nExchargeTimes/2*m_nCellScore)
    {
        nIndex=2;
        if(beilv >= m_nCellScore && beilv <= m_nExchargeTimes/2*m_nCellScore)
        {
            
        }
        else
        {
            m_barrelLayer[wChairID]->changeBarrel(nIndex);
        }
    }
    else if(m_nBeiLv[wChairID]>m_nExchargeTimes/2*m_nCellScore&&m_nBeiLv[wChairID]<=m_nExchargeTimes*m_nCellScore)
    {
        nIndex=3;
        if(beilv > m_nExchargeTimes/2*m_nCellScore && beilv <= m_nExchargeTimes*m_nCellScore)
        {
            
        }
        else
        {
            m_barrelLayer[wChairID]->changeBarrel(nIndex);
        }
    }
//    else
//    {
//        nIndex=4;
//        if(beilv == 6*m_nCellScore || beilv == 7*m_nCellScore || beilv == 8*m_nCellScore || beilv == 9*m_nCellScore || beilv == 10*m_nCellScore || beilv == (10*m_nCellScore-1))
//        {
//            
//        }
//        else
//        {
//            m_barrelLayer[wChairID]->changeBarrel(nIndex);
//        }
//    }
    m_barrelLayer[wChairID]->setbarrelRotation(m_barrelLayer[wChairID]->getBarrel()->getRotation());
//    if (wChairID == m_nMeRealChairID)
//    {
//        if (nScore == m_nCellScore)
//            playSoundByID(true, 9);
//    }
}

void GameMainScene::setUserScore(WORD wChairID, int nScore)
{
	m_nUserScore[wChairID]=nScore;
	char score[20];
	sprintf(score,"%d",nScore);
	m_barrelLayer[wChairID]->getGamelastAtlas()->setString(score);
    
    if(wChairID == m_nMeRealChairID)
    {
        if(nScore >= 0)//玩家子弹不为0
        {
            m_bFirstEnter = false;//玩家已经开过炮
            m_bOnline = true;
        }
        
    }
}

void GameMainScene::userComeInorLeave(int PlayerID,bool ComeOrLeave,int RealUserid)
{
    if (PlayerID >= GAME_PLAYER)
    {
        ERROR_CHECK;
        return;
    }
	m_bAndroid[PlayerID]=true;
	m_bHaveUser[PlayerID] = ComeOrLeave;
	m_nRealChairID[PlayerID]=RealUserid;
	m_barrelLayer[PlayerID]->setLockFishID(0);
	m_nLockFishID[PlayerID]=0;
	m_bLockFishID[PlayerID] = false;
	m_nBeiLv[PlayerID] = m_nCellScore;
	memset(&m_LockLine[PlayerID],0,sizeof(tagLockLine));
	if (ComeOrLeave)
	{
		m_nBeiLv[PlayerID] = m_nCellScore;
		m_nUserScore[PlayerID] = 0;
        //m_barrelLayer[PlayerID]->setBarrel(2);
        m_barrelLayer[PlayerID]->superMenu(2, false);
        m_barrelLayer[PlayerID]->showBaoji(false);
        m_barrelLayer[PlayerID]->getSuperPaoSprite()->setVisible(false);
	}
}

void GameMainScene::setSceneIndex(int nIndex)
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
	m_nBgMusicIndex += 1;
    m_nBgMusicIndex %= 3;
	playSoundByID(false, m_nBgMusicIndex%3);
    m_nSceneIndex = nIndex%2;
	
	char cBgSp[64];

    std::string dirpath = CMainLogic::sharedMainLogic()->DIRPATH3;
    if (KIND_ID == YQS_KIND_ID)
    {
        sprintf(cBgSp, "bg0%d.png", m_nSceneIndex);
    }
    else if (KIND_ID == NZNH_KIND_ID)
    {
        sprintf(cBgSp, "bg1%d.png", m_nSceneIndex);
    }
    else
    {
        sprintf(cBgSp, "bg%d.png", m_nSceneIndex);
    }
    dirpath = dirpath + cBgSp;

	m_spBkgrnd = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(dirpath.c_str()));

    m_spBkgrnd->setScale(1.1f);//2.7
//    m_spBkgrnd->setScale(1.4f);
	m_spBkgrnd->setAnchorPoint(CCPointZero);

	if (m_nMeRealChairID >= GAME_PLAYER/2)
	{
		m_spBkgrnd->setRotation(180.0f);
		m_spBkgrnd->setPosition(ccp(m_sizeVisible.width+45, m_sizeVisible.height+45));
	}
	else
	{
		m_spBkgrnd->setPosition(ccp(-45,-45));
	}

	m_spBkgrnd->setTag(0);
	this->addChild(m_spBkgrnd, -1);
}

void GameMainScene::changeFishScene(bool bYuchao)//true为鱼潮来临false为宝箱来袭
{
    CCSprite* sprite = CCSprite::createWithSpriteFrameName("yuchao1.png");
    sprite -> setPosition(ccp(CLIENT_WIDTH+sprite->getContentSize().width/2, CLIENT_HEIGHT/2));
    this -> addChild(sprite, 39);
    sprite -> setScale(2.0f);
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCArray* array = CCArray::create();
    char temp[16];
    for(int i = 1; i <= 13; i++)
    {
        sprintf(temp, "yuchao%d.png", i);
        CCSpriteFrame* frame = cache->spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, 0.12f);
    CCAnimate* animate = CCAnimate::create(animation);
    sprite -> runAction(CCRepeatForever::create(animate));
    
    if(m_nMeRealChairID >= GAME_PLAYER/2)
    {
        sprite -> setRotation(180);
    }
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall));
    sprite -> runAction(CCSequence::create(CCMoveTo::create(0.5f, ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2)), CCDelayTime::create(4.2f), CCMoveTo::create(3.05f, ccp(-sprite->getContentSize().width/2, CLIENT_HEIGHT/2)), func, NULL));
    
    this -> scheduleOnce(schedule_selector(GameMainScene::changeScene), 3.0f);
}

//切换场景
void GameMainScene::changeScene()
{
    int nIndex = m_nChangeSceneIndex;
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	if (m_nBgMusicIndex==nIndex)
    {
		m_nBgMusicIndex+=1;
    }
	else
    {
		m_nBgMusicIndex=nIndex%4;
    }
	playSoundByID(true, 31);
	playSoundByID(true, 32);
	m_bFishingOffSeason = true;
	m_nPreSceneIndex=m_nSceneIndex;
    m_nSceneIndex=nIndex%2;
    if (m_nPreSceneIndex==m_nSceneIndex)
        m_nSceneIndex=(m_nSceneIndex+1)%2;
	
	char cBgSp[64];

    std::string dirpath = CMainLogic::sharedMainLogic()->DIRPATH3;
    if (KIND_ID == YQS_KIND_ID)
    {
        sprintf(cBgSp, "bg0%d.png", m_nSceneIndex);
    }
    else if (KIND_ID == NZNH_KIND_ID)
    {
        sprintf(cBgSp, "bg1%d.png", m_nSceneIndex);
    }
    else
    {
        sprintf(cBgSp, "bg%d.png", m_nSceneIndex);
    }
    dirpath = dirpath+cBgSp;

	m_spBkgrnd2 = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(dirpath.c_str()));

    m_spBkgrnd2->setScale(1.1f);//2.7
//    m_spBkgrnd2->setScale(1.4f);
	m_spBkgrnd2->setAnchorPoint(CCPointZero);

	if (m_nMeRealChairID >= GAME_PLAYER/2)
	{
		m_spBkgrnd2->setRotation(180.0f);
		m_spBkgrnd2->setPosition(ccp(m_sizeVisible.width+45, m_sizeVisible.height+45));
	}
	else
		m_spBkgrnd2->setPosition(ccp(-45,-45));
	this->addChild(m_spBkgrnd2, -2);

    //修改波浪剧情
	schedule(schedule_selector(GameMainScene::changeSceneScehdule), 0.1f);
}

void GameMainScene::setSuperPao(WORD wChairID)
{
	m_bUserSuperPao[wChairID]=true;
}

void GameMainScene::cancelSuperPao(WORD wChairID)
{
	m_bUserSuperPao[wChairID]=false;
}

int GameMainScene::getBulletType(int nMult)
{
    int nMaxMultiple = 0;
    if(m_GameScene.lMaxMultiple == 99)
    {
        nMaxMultiple = 100;
    }
    else
    {
        nMaxMultiple = m_GameScene.lMaxMultiple;
    }
    float fIndex = (float)nMult/nMaxMultiple;
    int nIndex=0;
    if(fIndex <= 0.3f)
    {
        nIndex=1;
    }
    else if(fIndex > 0.3f && fIndex <= 0.6f)
    {
        nIndex=2;
    }
    else
    {
        nIndex=3;
    }
    return nIndex;
}

void GameMainScene::userShoot2(WORD wChairID, float fRotation1, int RealChairID, bool bAndroid, DWORD dwBulletID,int nCount, BYTE cbAttribute)
{
    int type = m_barrelLayer[wChairID]->getBarrelMult();
    int bulletType = 0;
    int distanceBullet = 0;
    int distance = 0;
    if(type == 2)
    {
        if(m_barrelLayer[wChairID]->getSuperBarrel())
        {
            bulletType = 7;
        }
        else
        {
            bulletType = 1;
        }
        distanceBullet = 3;
    }
    else if(type == 3)
    {
        if(m_barrelLayer[wChairID]->getSuperBarrel())
        {
            bulletType = 9;
        }
        else
        {
            bulletType = 3;
        }
        distanceBullet = 5;
    }
    else if(type == 4)
    {
        if(m_barrelLayer[wChairID]->getSuperBarrel())
        {
            bulletType = 11;
        }
        else
        {
            bulletType = 5;
        }
        distanceBullet = 11;
    }
    
    if(m_nEveryBullet[wChairID] >= MaxBulletCount)
        return;
    m_nEveryBullet[wChairID]++;
    for(unsigned int i = 0; i < MaxBulletCount*GAME_PLAYER; i++)
    {
        BulletSprite* bulletSprite = (BulletSprite*)m_arrBullet->objectAtIndex(((bulletType+1)/2-1)*MaxBulletCount*GAME_PLAYER+i);
        if(bulletSprite->getHave())  continue;
        if(wChairID == m_nMeRealChairID)
        {
            m_nCurrentBulletCount++;
        }
        bulletSprite->m_vctFishID.clear();
        bulletSprite->setAttribute(cbAttribute);
        bulletSprite->setHave(true);
        bulletSprite->setVisible(true);
        bulletSprite->setScale(0.90f);
        bulletSprite->setRealChairID(wChairID);
        bulletSprite->setBulletID(dwBulletID);
        bulletSprite->setMultiple(getBulletType(m_nBeiLv[wChairID]));
        if(wChairID == m_nMeRealChairID)
        {
            bulletSprite -> setAndroid(false);
        }
        else
        {
            bulletSprite -> setAndroid(bAndroid);
        }
        bulletSprite->setIsStop(false);
        bulletSprite -> setAnchorPoint(ccp(0.5f,0.5f));
        if(m_bLockFishID[wChairID])
        {
            fRotation1 = m_fRotation[wChairID];
        }
        m_barrelLayer[wChairID]->getBarrel()->setRotation(fRotation1);
        m_barrelLayer[wChairID]->setbarrelRotation(fRotation1);
        if(nCount>1)
        {
            m_barrelLayer[wChairID]->getBarrel()->setRotation(m_fRotation[wChairID]);
            m_barrelLayer[wChairID]->setbarrelRotation(m_fRotation[wChairID]);
        }
        CCPoint point1 = m_barrelLayer[wChairID] -> getCannonBg()->convertToWorldSpace(m_barrelLayer[wChairID]->getBarrel()->getPosition());
        if(wChairID >= 0 && wChairID <GAME_PLAYER/2 && m_nMeRealChairID <GAME_PLAYER/2 && m_nMeRealChairID >= 0)
        {
            bulletSprite->setPosition(ccp(point1.x+((m_barrelLayer[wChairID]->getBarrel()->getContentSize().width/2+distance)*sin(fRotation1/(180/M_PI))),point1.y+(m_barrelLayer[wChairID]->getBarrel()->getContentSize().height/2+distanceBullet+distance)*cos(fRotation1/(180/M_PI))));
            bulletSprite -> setRotation(fRotation1);
            bulletSprite -> setRotation(fRotation1);
            bulletSprite -> setBulletRotation(fRotation1);
        }
        else if(wChairID >= GAME_PLAYER/2 && wChairID <GAME_PLAYER && m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID <GAME_PLAYER)
        {
            bulletSprite->setPosition(ccp(m_sizeVisible.width -point1.x-((m_barrelLayer[wChairID]->getBarrel()->getContentSize().width/2+distance)*sin(fRotation1/(180/M_PI))),m_sizeVisible.height -point1.y-(m_barrelLayer[wChairID]->getBarrel()->getContentSize().height/2+distanceBullet+distance)*cos(fRotation1/(180/M_PI))));
            bulletSprite -> setRotation(180+fRotation1);
            bulletSprite -> setBulletRotation(180+fRotation1);
        }
        else if(wChairID >= 0 && wChairID <GAME_PLAYER/2 && m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID <GAME_PLAYER)
        {
            bulletSprite->setPosition(ccp(m_sizeVisible.width -point1.x+((m_barrelLayer[wChairID]->getBarrel()->getContentSize().width/2+distance)*sin(fRotation1/(180/M_PI))),m_sizeVisible.height -point1.y+(m_barrelLayer[wChairID]->getBarrel()->getContentSize().height/2+distanceBullet+distance)*cos(fRotation1/(180/M_PI))));
            bulletSprite -> setRotation(fRotation1);
            bulletSprite -> setBulletRotation(fRotation1);
        }
        else if(wChairID >= GAME_PLAYER/2 && wChairID <GAME_PLAYER && m_nMeRealChairID >= 0 && m_nMeRealChairID <GAME_PLAYER/2)
        {
            bulletSprite->setPosition(ccp(point1.x-((m_barrelLayer[wChairID]->getBarrel()->getContentSize().width/2+distance)*sin(fRotation1/(180/M_PI))),point1.y-(m_barrelLayer[wChairID]->getBarrel()->getContentSize().height/2+distanceBullet+distance)*cos(fRotation1/(180/M_PI))));
            bulletSprite -> setRotation(180+fRotation1);
            bulletSprite -> setBulletRotation(180+fRotation1);
        }
        m_barrelLayer[wChairID]->playBarrelAnimate1();
        break;
    }
}

//发射子弹
void GameMainScene::userShoot(WORD wChairID, float fRotation1, int RealChairID, bool bAndroid, DWORD dwBulletID, int nCount, BYTE cbAttribute)
{
    if(m_bBaojiStatus[m_nMeRealChairID] && m_nMeRealChairID == wChairID)//如果自己是爆机状态
    {
        return ;
    }
	if (m_bFishingOffSeason) return;
	m_bAndroid[wChairID]=bAndroid;
	if (wChairID==m_nMeRealChairID && m_nUserScore[m_nMeRealChairID]-m_nBeiLv[m_nMeRealChairID]<0)
    {
        if(!m_bTryBarrelStatus[m_nMeRealChairID])
        {
            return;
        }
        
    }

    if(nCount>1)
    {
        removeLockLine(wChairID);
    }
	if(wChairID == m_nMeRealChairID)//审核期间不弹出试用炮
	{
        if(m_nUserScore[m_nMeRealChairID] >= nCount*m_nBeiLv[m_nMeRealChairID])
        {
            if(!m_bTryBarrelStatus[m_nMeRealChairID])//如果不是试用炮状态，那么不消耗子弹
            {
                m_nUserScore[m_nMeRealChairID] -= nCount*m_nBeiLv[m_nMeRealChairID];//玩家自己发射子弹不经过服务
            }
        }
        else
        {
            if(!m_bTryBarrelStatus[m_nMeRealChairID])
            {
                m_nUserScore[m_nMeRealChairID] -= m_nBeiLv[m_nMeRealChairID];//玩家自己发射子弹不经过服务
            }
        }
		setUserScore(m_nMeRealChairID, m_nUserScore[m_nMeRealChairID]);
		if (m_bUserSuperPao[wChairID])
			playSoundByID(true, 38);
		else
        {
			playSoundByID(true, 11);
        }
        m_nOutTime = 60;
		m_labelTTFReCount->setVisible(false);
		m_spReCount->setVisible(false);
	}
    if(!m_bLockFishID[wChairID])
    {
        m_fRotation[wChairID] = fRotation1;
    }
    if(nCount>1)//nCount 代表子弹数量，最大为5（意味散射）
    {
        for(int i = 0; i < nCount; i++)
        {
            userShoot2(wChairID, fRotation1-30*i+60, RealChairID, bAndroid, dwBulletID+i,nCount, cbAttribute);
        }
    }
    else
    {
        userShoot2(wChairID, fRotation1, RealChairID, bAndroid, dwBulletID, nCount, cbAttribute);
    }
}

void GameMainScene::lockFish()
{
	m_barrelLayer[m_nMeRealChairID]->setUserLockFish(true);
	m_bUserLock = true;
}

void GameMainScene::lockFishID(WORD wChairID, int nFishID)
{
	if (wChairID==m_nMeRealChairID)
	{
		return;
	}
	GameFish* gameFish = NULL;
	for(unsigned int i = 0; i < m_vctFishFactory.size(); i++)
	{
		gameFish = m_vctFishFactory[i];
		if(gameFish == NULL || gameFish->getDied())
			continue;
		if(gameFish->getFishID() != nFishID)
		{
			gameFish = NULL;
		}
		else
		{
			break;
		}
	}
	if(gameFish == NULL)  return;
	m_nLockFishID[wChairID] = nFishID;
	m_bLockFishID[wChairID] = true;
	if(m_nMeRealChairID >= 0 && m_nMeRealChairID < GAME_PLAYER/2 && wChairID >= GAME_PLAYER/2 && wChairID < GAME_PLAYER)
	{
		m_barrelLayer[wChairID]->getLockBlockSprite()->setRotation(180);
	}
	else if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER && wChairID >= 0 && wChairID < GAME_PLAYER/2)
	{
		m_barrelLayer[wChairID]->getLockBlockSprite()->setRotation(180);
	}
	m_barrelLayer[wChairID]->lockRealFish(gameFish);
}

//捕获鱼
void GameMainScene::setCapture(WORD wChairID, CMD_S_CaptureFish * pCaptureFish)
{
	//红鱼同类炸弹
	if(pCaptureFish->nProperty&enLine || pCaptureFish->nProperty&enSameDie || pCaptureFish->nProperty&enBomb || pCaptureFish->nProperty&enRangeBomb)
	{
		CCPoint ptBase,ptOther[100];
		int nFishCount = 0;
		GameFish* pFishBase = NULL;
		for(unsigned int i = 0; i < m_vctFishFactory.size(); i++)
		{
			pFishBase = m_vctFishFactory[i];
			if(pFishBase == NULL || pFishBase->getDied())  continue;
			
			if(pFishBase->getFishID() == pCaptureFish->nFishID[0])
			{
				break;
			}
		}
		if(pFishBase == NULL)  return;
		ptBase = CCPointMake((int)pFishBase->m_pFish->getPositionX(),(int)pFishBase->m_pFish->getPositionY());
		for(int i = 0; i < pCaptureFish->nCaptureCount; i++)
		{
			GameFish* pFishOther = NULL;
			for(unsigned int j = 0; j < m_vctFishFactory.size(); j++)
			{
				pFishOther = m_vctFishFactory[j];
				if(NULL == pFishOther || pFishOther->getDied())  continue;
				if(pFishOther -> getFishID() == pCaptureFish->nFishID[i])
				{
					if(nFishCount < pCaptureFish->nCaptureCount)
					{
						ptOther[nFishCount].x = pFishOther->m_pFish->getPositionX();
						ptOther[nFishCount].y = pFishOther->m_pFish->getPositionY();
						nFishCount++;
						pFishOther->setSpeed(0);
					}
					break;
				}
			}
		}
		GameLine* pGameLine = GameLine::create();
		if(NULL != pGameLine)
		{
			this -> addChild(pGameLine,0);
			pGameLine->setLine(wChairID,ptBase,nFishCount,ptOther,pCaptureFish);
			playSoundByID(true, 22);
		}
		return;
	}
	showScoreInfo(wChairID,pCaptureFish);
}

//设置得分圆盘
void GameMainScene::setPlate(WORD wChairID, int nScore, int nFishID, bool bShowName)
{
	if(m_nMeRealChairID >= 0 && m_nMeRealChairID < GAME_PLAYER/2 && wChairID >= GAME_PLAYER/2 && wChairID < GAME_PLAYER)
	{
		m_barrelLayer[wChairID]->getSpritePlate()->setRotation(180);
	}
	else if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER && wChairID >= 0 && wChairID < GAME_PLAYER/2)
	{
		m_barrelLayer[wChairID]->getSpritePlate()->setRotation(180);
	}
	m_barrelLayer[wChairID]->clickYuanPan(nScore, nFishID, bShowName);
	if (wChairID == m_nMeRealChairID)
		playSoundByID(true, 16);
}

//设置BOSS倍率
void GameMainScene::setBossScore(int nScore)
{
	GameFish * pGameFish=NULL;
	for(unsigned int i = 0; i < m_vctFishFactory.size(); i++)
	{
		pGameFish = m_vctFishFactory[i];
		if (NULL==pGameFish || pGameFish->getDied()) continue;
		if (pGameFish->isProperty(enBoss))
		{
			pGameFish->setBossScore(nScore);
		}
	}
}

void GameMainScene::showScoreInfo(WORD wChairID, CMD_S_CaptureFish * pCaptureFish)
{
	int mult = pCaptureFish->nTotalMultiple;
    int nPlateMult = mult;
    int nLotteryCount = pCaptureFish->nLottteryCount;
	if (mult != 0)
	{
		if(mult >= 40)
		{
			mult = 40;
		}
        m_barrelLayer[wChairID]->fishDiedScore(wChairID,pCaptureFish->nTotalScore,mult);
        int nFishID = pCaptureFish->nFishID[0];
        GameFish* pGameFish = NULL;
        for(int i = 0; i < m_vctFishFactory.size(); i++)
        {
            pGameFish = m_vctFishFactory[i];
            if(pGameFish == NULL || pGameFish->getDied() || pGameFish->getAppear())
                continue;
            if(pGameFish->getFishID() == nFishID)
            {
                break;
            }
            else
            {
                pGameFish = NULL;
            }
        }
        if(1)
        {
            if(nPlateMult>=60)
            {
                setPlate(wChairID,pCaptureFish->nTotalScore, 0);
            }
        }
        else//大鱼得分圆盘
        {
            if(pGameFish != NULL)
            {
//                int nSpriteID = pGameFish->getSpriteID();
                if(pCaptureFish->nProperty&enBoss)
                {
                    setPlate(wChairID,pCaptureFish->nTotalScore, 6);//美人鱼
                }
                else if(pCaptureFish->nTotalMultiple >= 60)
                {
                    setPlate(wChairID,pCaptureFish->nTotalScore, 11);//jianyu
                }
                
//                else if(nSpriteID == 1015)
//                {
//                    if(m_nBeiLv[wChairID]*40 == pCaptureFish->nTotalScore)
//                    {
//                        setPlate(wChairID,pCaptureFish->nTotalScore, 9);//黄金鲨
//                    }
//                }
//                else if(nSpriteID == 1017)
//                {
//                    if(m_nBeiLv[wChairID]*100 == pCaptureFish->nTotalScore)
//                    {
//                        setPlate(wChairID,pCaptureFish->nTotalScore, 7);//金龙鱼
//                    }
//                }
//                else if(nSpriteID == 1016)
//                {
//                    if(m_nBeiLv[wChairID]*80 == pCaptureFish->nTotalScore)
//                    {
//                        setPlate(wChairID,pCaptureFish->nTotalScore, 8);//电鳗
//                    }
//                }
//                else if(nSpriteID == 1019)
//                {
//                    if(m_nBeiLv[wChairID]*250 == pCaptureFish->nTotalScore)
//                    {
//                        setPlate(wChairID,pCaptureFish->nTotalScore, 5);//绿蟾
//                    }
//                }
//                else if(nSpriteID == 1020)
//                {
//                    if(m_nBeiLv[wChairID]*300 == pCaptureFish->nTotalScore)
//                    {
//                        setPlate(wChairID,pCaptureFish->nTotalScore, 4);//金蟾
//                    }
//                }
//                else if(nSpriteID == 1023)
//                {
//                    if(m_nBeiLv[wChairID]*55 == pCaptureFish->nTotalScore)
//                    {
//                        setPlate(wChairID,pCaptureFish->nTotalScore, 1);//浅水炸弹
//                    }
//                }
//                else if(nSpriteID == 1024)
//                {
//                    if(m_nBeiLv[wChairID]*110 == pCaptureFish->nTotalScore)
//                        setPlate(wChairID,pCaptureFish->nTotalScore, 0);//深水炸弹
//                }
//                else if(nSpriteID == 1029)
//                {
//                    if(m_nBeiLv[wChairID]*40 == pCaptureFish->nTotalScore)
//                    {
//                        setPlate(wChairID,pCaptureFish->nTotalScore, 10);//食人鱼
//                    }
//                }
//                else if(nSpriteID == 1030)
//                {
//                    if(m_nBeiLv[wChairID]*200 == pCaptureFish->nTotalScore)
//                    {
//                        setPlate(wChairID,pCaptureFish->nTotalScore, 11);//jianyu
//                    }
//                }
            }
        }
        
    }
    int nShowTryCannon = 0;
    if(m_bTryBarrelStatus[m_nMeRealChairID])
    {
        if(m_nTryLastScore != pCaptureFish->nTryScore)
        {
            nShowTryCannon = 1;
            m_nTryLastScore = pCaptureFish->nTryScore;
        }
    }
	for (int i=0; i<pCaptureFish->nCaptureCount; i++)
	{
        if(i == 0)
        {
            userAddMoney(wChairID,pCaptureFish->nFishID[i],pCaptureFish->nFishScore[i],pCaptureFish->nFishMultiple[i], nLotteryCount, pCaptureFish->bPlayParticle, pCaptureFish->nTryScore, nShowTryCannon);
        }
        else
        {
            userAddMoney(wChairID,pCaptureFish->nFishID[i],pCaptureFish->nFishScore[i],pCaptureFish->nFishMultiple[i], 0, false, pCaptureFish->nTryScore, nShowTryCannon);
        }
	}
}

//捕获鱼得分显示以及死鱼动画
void GameMainScene::userAddMoney(WORD wChairID, int nFishID, int nFishScore, int nFishMultiple, int nLotteryCount, bool bPlayParticle, int nTryScore, int nShowTryCannon)//nShowTryCannon=1为试用期间打死的鱼，=0为正常打死鱼
{
	GameFish* gameFish = NULL;
	for(unsigned int i = 0; i < m_vctFishFactory.size(); i++)
	{
		gameFish = m_vctFishFactory[i];
		if(gameFish == NULL)
			continue;
		if(gameFish->getFishID() != nFishID)
		{
			gameFish = NULL;
		}
		else
		{
			break;
		}
	}
	if(gameFish == NULL || gameFish->getAppear())
	{
		return;
	}
	gameFish->setHitUser(wChairID);
	gameFish -> setDied(true);
	gameFish->m_pFish->setVisible(false);
	gameFish->m_pFish->stopAllActions();
    gameFish->setFishScore(nFishScore);
    if(1)//if(nFishScore != 0)
    {
        if (gameFish->getMagParticle() != "")
        {
            playParticle(gameFish->getMagParticle().c_str(), gameFish->m_pFish->getPositionX(), gameFish->m_pFish->getPositionY());
        }
        if (gameFish->getMagParticle2() != "")
            playParticle(gameFish->getMagParticle2().c_str(), gameFish->m_pFish->getPositionX(), gameFish->m_pFish->getPositionY());
    }
    
    
    for(int i = 0; i < GAME_PLAYER/2; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(m_nShijinbiFishID[i][j] == gameFish->getFishID())
            {
                m_nShijinbiFishID[i][j] = 0;
            }
        }
    }
    
    //五福卡片飞向玩家炮台
    bool bShakeScreen = false;//五福鱼是否震屏
    if(nLotteryCount > 0 && CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        bShakeScreen  = true;
        playParticle("wufu", gameFish->m_pFish->getPositionX(), gameFish->m_pFish->getPositionY());
        if(wChairID != m_nMeRealChairID)
        {
            CCPoint endPoint;
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                CCPoint point1 = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
                endPoint = CCPointMake(m_sizeVisible.width-point1.x,m_sizeVisible.height-point1.y);
                endPoint = ccp(endPoint.x,endPoint.y);
            }
            else
            {
                endPoint = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
                endPoint = ccp(endPoint.x,endPoint.y);
            }
            if(m_nMeRealChairID < GAME_PLAYER/2 && wChairID >= GAME_PLAYER/2)
            {
                endPoint = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
                endPoint = ccp(endPoint.x,endPoint.y+14);
            }
            if(m_nMeRealChairID >= GAME_PLAYER/2 && wChairID < GAME_PLAYER/2)
            {
                CCPoint point1 = m_barrelLayer[wChairID]->convertToWorldSpace(m_barrelLayer[wChairID] -> getCannonBg() -> getPosition());
                endPoint = CCPointMake(m_sizeVisible.width-point1.x,m_sizeVisible.height-point1.y);
                endPoint = ccp(endPoint.x,endPoint.y+14);
            }
            CCSprite* pWufuFish2 = CCSprite::create("CatchFish2006/wufufish.png");
            pWufuFish2 -> setPosition(gameFish->m_pFish->getPosition());
            pWufuFish2 -> setRotation(gameFish->m_pFish->getRotation());
            this -> addChild(pWufuFish2, 80);
            float fDix = pWufuFish2->getPositionX();
            float fDiy = pWufuFish2->getPositionY();
            float fDis = sqrt((fDix-endPoint.x)*(fDix-endPoint.x) + (fDiy-endPoint.y)*(fDiy-endPoint.y));
            float dt = fDis/500;
            CCCallFuncN* funcN = CCCallFuncN::create(this, callfuncN_selector( GameMainScene::removeNodeCall));
            pWufuFish2 -> runAction(CCSequence::create(CCScaleTo::create(0.5f, 2.0f), CCSpawn::create(CCMoveTo::create(dt, endPoint), CCScaleTo::create(dt, 1.0f), NULL), CCFadeOut::create(1.0f), funcN, NULL));
        }
        else
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWufuGet);
            CCSprite* pWufuFish2 = CCSprite::create("CatchFish2006/wufufish.png");
            pWufuFish2 -> setPosition(gameFish->m_pFish->getPosition());
            pWufuFish2 -> setRotation(gameFish->m_pFish->getRotation());
            this -> addChild(pWufuFish2, 80);
            CCPoint endPoint;
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                endPoint = CCPointMake(CLIENT_WIDTH-1220, CLIENT_HEIGHT-540);
            }
            else
            {
                endPoint = CCPointMake(1220, 540);
            }
            float fDix = pWufuFish2->getPositionX();
            float fDiy = pWufuFish2->getPositionY();
            float fDis = sqrt((fDix-endPoint.x)*(fDix-endPoint.x) + (fDiy-endPoint.y)*(fDiy-endPoint.y));
            float dt = fDis/500;
            CCCallFuncN* funcN = CCCallFuncN::create(this, callfuncN_selector( GameMainScene::removeNodeCall));
            pWufuFish2 -> runAction(CCSequence::create(CCScaleTo::create(0.5f, 2.0f), CCSpawn::create(CCMoveTo::create(dt, endPoint), CCScaleTo::create(dt, 1.0f), NULL), CCCallFuncND::create(this, callfuncND_selector(GameMainScene::playDantouSound), (void*)(long)nLotteryCount), CCFadeOut::create(1.0f), funcN, NULL));
        }
    }
    
    if(wChairID == m_nMeRealChairID)
    {
        if(!nShowTryCannon)
        {
            if(KIND_ID == YQS_KIND_ID)
            {
                if(gameFish->getSpriteID()<=1030 && gameFish->getSpriteID()>=1000)
                {
                    for(unsigned int i = 0; i < 31; i++)
                    {
                        if(gameFish->getSpriteID() == m_nFishType[i])
                        {
                            m_nFishDied[i]++;
                            break;
                        }
                    }
                }
                else if(gameFish->getSpriteID()<=1036 && gameFish->getSpriteID()>=1031)
                {
                    m_nFishDied[m_nFishCount-1]++;
                }
            }
            else if(KIND_ID == NZNH_KIND_ID)
            {
                if(gameFish->getSpriteID()<=1035 && gameFish->getSpriteID()>=1000)
                {
                    for(unsigned int i = 0; i < 36; i++)
                    {
                        if(gameFish->getSpriteID() == m_nFishType[i])
                        {
                            m_nFishDied[i]++;
                            break;
                        }
                    }
                }
                else if(gameFish->getSpriteID()<=1041 && gameFish->getSpriteID()>=1036)
                {
                    m_nFishDied[m_nFishCount-1]++;
                }
            }
            else
            {
                if(gameFish->getSpriteID()<=1036 && gameFish->getSpriteID()>=1000)
                {
                    for(unsigned int i = 0; i < 37; i++)
                    {
                        if(gameFish->getSpriteID() == m_nFishType[i])
                        {
                            m_nFishDied[i]++;
                            break;
                        }
                    }
                }
                else if(gameFish->getSpriteID()<=1045 && gameFish->getSpriteID()>=1040)
                {
                    m_nFishDied[m_nFishCount-1]++;
                }
                if(KIND_ID != FB_KIND_ID)
                {
                    //如果结算框存在
                    if(m_bHaveWndCalc)
                    {
                        m_fishWndLayer->updateCalcWnd(m_nFishType, m_nFishDied, m_nFishCount);
                    }
                }
            }
            
        }
        
	}
	//定屏
	if (gameFish->isProperty(enFreeze))
	{
        if(nFishScore != 0)
        {
            if(m_bFreeze)
            {
                m_nFreezeTime = 10;
            }
            else
            {
                m_bFreeze=true;
                m_nFreezeTime = 10;
                schedule(schedule_selector(GameMainScene::freezedScreen),1.0f);
            }
        }
        playSoundByID(true, 17);
	}
	else if(gameFish->isProperty(enJumpBomb))
	{
        if(nFishScore != 0)
        {
            if(m_bFreeze)
            {
                if(m_nFreezeTime <= 6)
                {
                    m_nFreezeTime = 6;
                }
            }
            else
            {
                m_bFreeze=true;
                m_nFreezeTime = 6;
                schedule(schedule_selector(GameMainScene::freezedScreen),1.0f);
                playSoundByID(true, 23);
            }
        }
	}
	else if (gameFish->isProperty(enBoss))
	{
		for(unsigned int k = 0; k < m_labelBossBeilv.size(); k++)
		{
			if(m_labelBossBeilv[k]->getBossTag() == gameFish->getFishID())
			{
				m_labelBossBeilv[k]->removeFromParentAndCleanup(true);
				m_labelBossBeilv.erase(m_labelBossBeilv.begin()+k);
				break;
			}
		}
	}
    else if (gameFish->isProperty(enLine))
    {
        playParticle("j3", gameFish->m_pFish->getPositionX(), gameFish->m_pFish->getPositionY());
    }
    if(KIND_ID == YQS_KIND_ID)
    {
        if(gameFish->getSpriteID()>=1016 || gameFish->isProperty(enLine))
        {
            for(int k = 0; k < m_arrLockFish->count(); k++)
            {
                CCSprite* pLockDi = (CCSprite*)m_arrLockFish->objectAtIndex(k);
                if(pLockDi == NULL) continue;
                if(pLockDi->getTag() == gameFish->getFishID())
                {
                    m_arrLockFish -> removeObject(pLockDi);
                    pLockDi -> removeFromParentAndCleanup(true);
                    break;
                }
            }
        }
    }
    else
    {
        if(gameFish->getSpriteID()>=1017 || gameFish->isProperty(enLine))
        {
            for(int j = 0; j < m_arrLockFish->count(); j++)
            {
                CCSprite* pLockDi = (CCSprite*)m_arrLockFish->objectAtIndex(j);
                if(pLockDi == NULL) continue;
                if(pLockDi->getTag() == gameFish->getFishID())
                {
                    m_arrLockFish -> removeObject(pLockDi);
                    pLockDi -> removeFromParentAndCleanup(true);
                    break;
                }
            }
        }
    }
    
    if (gameFish->getMultiple() >= 60 || gameFish->isProperty(enBoss) || gameFish->isProperty(enRangeBomb) || gameFish->isProperty(enBomb) || gameFish->isProperty(enLine) || bShakeScreen==true)
    {
        int nSoundIndex = (rand()%2+1)*100+rand()%8;
        playSoundByID(true, nSoundIndex);//随机播放大鱼音效
        SimpleAudioEngine::sharedEngine()->playEffect("Music/bombnew.mp3");
        shakeScreen();//大于N倍震屏
    }
    else
    {
        if (wChairID == m_nMeRealChairID)
        {
            if (gameFish->getMultiple() >= 20)
            {
                playSoundByID(true, 26);
            }
            else
            {
                if (!m_bSameDie[m_nMeRealChairID])
                {
                    playSoundByID(true, 19);
                }
            }
        }
    }
	//删除锁定
	for(int i = 0; i < GAME_PLAYER; i++)
	{
		if(m_nLockFishID[i] == gameFish->getFishID())
		{
			removeLockLine(i);
		}
	}
	//创建死鱼
    GameFish* pGameFish = createFishByID(gameFish->getSpriteID()+1000);
    char temp[16];
    sprintf(temp, "%d%d", gameFish->getFishID(), nShowTryCannon);
    CCString* str = CCString::create(temp);
    int n = atoi(str->getCString());
    CCCallFuncND* callFunND = CCCallFuncND::create(this, callfuncND_selector(GameMainScene::createMoneyCall), (void*)(long)n);
    if (pGameFish!=NULL)
    {
        pGameFish -> setSpriteID(gameFish->getSpriteID()+1000);
        pGameFish->m_pFish->setRotation(gameFish->getFishRotation()-90);
        pGameFish->m_pFish->setPosition(gameFish->m_pFish->getPosition());
        //美人鱼死亡动画
        //捕获boss显示特殊动画
//        if (pGameFish->isProperty(enBoss) && wChairID == m_nMeRealChairID && CMainLogic::sharedMainLogic()->KIND_ID == JCBY_KIND_ID)
//        {
//            if(nFishScore != 0)
//            {
//                CBeautfFishDeath* pBeautfFishDeath = CBeautfFishDeath::create(nFishScore);
//                this->addChild(pBeautfFishDeath, 200);
//                if(m_nMeRealChairID>=GAME_PLAYER/2)
//                {
//                    pBeautfFishDeath->setRotation(180);
//                }
//            }
//        }
        if(pGameFish->getGroupFish())//组合鱼没有阴影
        {
            CCArray* childFishAry = pGameFish->m_pFish->getChildren();
            if (childFishAry!=NULL)
            {
                for (unsigned int i = 0; i < childFishAry->count(); i++)
                {
                    CCSprite* childFish = (CCSprite*)childFishAry->objectAtIndex(i);
                    CCArray* childShadowAry = childFish->getChildren();
                    if (childShadowAry!=NULL)
                    {
                        for (unsigned int j = 0; j < childShadowAry->count(); j++)
                        {
                            CCSprite* childShadow = (CCSprite*)childShadowAry->objectAtIndex(j);
                            childShadow->setVisible(false);
                        }
                    }
                }
            }
        }
        if (KIND_ID == NZNH_KIND_ID)
        {
            if(pGameFish->getSpriteID()>=2030 && pGameFish->getSpriteID()<=2035)
            {
                pGameFish->m_pFish->setScale(1.5f);
                pGameFish->m_pFish->runAction(CCSequence::create(CCRotateBy::create(0.1f, 30), CCRotateBy::create(0.1f, -60), CCRotateBy::create(0.1f, 60), CCRotateBy::create(0.1f, -60), CCRotateBy::create(0.1f, 60), CCRotateBy::create(0.1f, -60), CCRotateBy::create(0.1f, 60), CCRotateBy::create(0.1f, -60), CCRotateBy::create(0.1f, 60), CCRotateBy::create(0.1f, -60), CCRotateBy::create(0.1f, 60), CCRotateBy::create(0.1f, -60), CCRotateBy::create(0.1f, 60), NULL));
                CCArray* shadowAry = pGameFish->m_pFish->getChildren();
                if (shadowAry!=NULL)
                {
                    CCSprite* shadow = NULL;
                    for (unsigned int i = 0; i < shadowAry->count(); i++)
                    {
                        shadow = (CCSprite*)shadowAry->objectAtIndex(i);
                        if(shadow != NULL)
                        {
                            if(shadow->getTag() == 100)//是鱼
                            {
                                //shadow->runAction(CCRepeatForever::create(fishDieRandAct(0.1f)));
                            }
                            else//是圆盘
                            {
                                //shadow->runAction(CCSequence::create(CCRotateTo::create(1.5f,1080),NULL));
                            }
                        }
                    }
                }
                addChildToBatch(true, pGameFish->m_pFish, pGameFish->getFilePath());
                float fTime = pGameFish->getLoopTimes()*pGameFish->getFrameCount()*pGameFish->getDuration();
                CCSequence* seq = CCSequence::create(CCDelayTime::create(fTime), callFunND, NULL);
                this->runAction(seq);
            }
            else if(pGameFish->getSpriteID()>=2036 && pGameFish->getSpriteID()<=2041)
            {
                pGameFish->m_pFish->setScale(1.5f);
                CCArray* shadowAry = pGameFish->m_pFish->getChildren();
                if (shadowAry!=NULL)
                {
                    CCSprite* shadow = NULL;
                    for (unsigned int i = 0; i < shadowAry->count(); i++)
                    {
                        shadow = (CCSprite*)shadowAry->objectAtIndex(i);
                        if(shadow != NULL)
                        {
                            if(shadow->getTag() == 100)//是鱼
                            {
                            }
                            else//是圆盘
                            {
                                //shadow->runAction(CCSequence::create(CCRotateTo::create(1.5f,1080),NULL));
                            }
                        }
                    }
                }
                addChildToBatch(true, pGameFish->m_pFish, pGameFish->getFilePath());
                float fTime = pGameFish->getLoopTimes()*pGameFish->getFrameCount()*pGameFish->getDuration();
                CCSequence* seq = CCSequence::create(CCDelayTime::create(fTime), callFunND, NULL);
                this->runAction(seq);
            }
            else
            {
                if(pGameFish->getDiedAnimate())
                {
                    //pGameFish->m_pFish->runAction(CCRepeatForever::create(fishDieRandAct(0.1f)));
                }
                else
                {
                    pGameFish->m_pFish->runAction(CCRepeatForever::create(fishDieRandAct(0.1f)));
                }
                addChildToBatch(true, pGameFish->m_pFish, pGameFish->getFilePath());
                float fTime = pGameFish->getLoopTimes()*pGameFish->getFrameCount()*pGameFish->getDuration();
                CCSequence* seq = CCSequence::create(CCDelayTime::create(fTime), callFunND, NULL);
                this->runAction(seq);
            }
        }
        else if (KIND_ID == YQS_KIND_ID)
        {
            if(pGameFish->getSpriteID()>=2017 && pGameFish->getSpriteID()<=2030)
            {
                float fRotation = 90.0f;
                if (pGameFish->getSpriteID()>=2028 && pGameFish->getSpriteID()<=2030)
                {
                    pGameFish->m_pFish->setScale(2.0f);
                    if (pGameFish->getSpriteID()==2029) {
                        fRotation = 30.0f;
                    }
                }
                if(!pGameFish->getDiedAnimate())
                {
                    CCDelayTime* pDelay = CCDelayTime::create(0.1f);
                    CCRotateBy* pRotateBy1 = CCRotateBy::create(0, fRotation);
                    CCSequence* pSeq = CCSequence::create(pRotateBy1, pDelay, NULL);
                    pGameFish->m_pFish->runAction(CCRepeatForever::create(pSeq));
                    addChildToBatch(true, pGameFish->m_pFish, pGameFish->getFilePath());
                    float fTime = pGameFish->getLoopTimes()*pGameFish->getFrameCount()*pGameFish->getDuration();
                    CCSequence* seq = CCSequence::create(CCDelayTime::create(fTime), callFunND, NULL);
                    this->runAction(seq);
                }
            }
            else
            {
                pGameFish->m_pFish->setScale(1.5f);
                if(!pGameFish->getDiedAnimate())
                {
                    pGameFish->m_pFish->runAction(CCRepeatForever::create(fishDieRandAct(0.1f)));
                }
                addChildToBatch(true, pGameFish->m_pFish, pGameFish->getFilePath());
                float fTime = pGameFish->getLoopTimes()*pGameFish->getFrameCount()*pGameFish->getDuration();
                CCSequence* seq = CCSequence::create(CCDelayTime::create(fTime), callFunND, NULL);
                this->runAction(seq);
            }
        }
        else
        {
            if(pGameFish->getSpriteID()>=2040 && pGameFish->getSpriteID()<=2045)
            {
                pGameFish->m_pFish->setScale(1.5f);
                pGameFish->m_pFish->runAction(CCRepeatForever::create(fishDieRandAct2(0.1f)));
                CCArray* shadowAry = pGameFish->m_pFish->getChildren();
                if (shadowAry!=NULL)
                {
                    CCSprite* shadow = NULL;
                    for (unsigned int i = 0; i < shadowAry->count(); i++)
                    {
                        shadow = (CCSprite*)shadowAry->objectAtIndex(i);
                        if(shadow != NULL)
                        {
                            if(shadow->getTag() == 100)//是鱼
                            {
                                shadow->runAction(CCRepeatForever::create(fishDieRandAct2(0.1f)));
                            }
                            else//是圆盘
                            {
                                shadow->runAction(CCSequence::create(CCRotateTo::create(1.5f,1080),NULL));
                            }
                        }
                    }
                }
                addChildToBatch(true, pGameFish->m_pFish, pGameFish->getFilePath());
                float fTime = pGameFish->getLoopTimes()*pGameFish->getFrameCount()*pGameFish->getDuration();
                CCSequence* seq = CCSequence::create(CCDelayTime::create(fTime), callFunND, NULL);
                this->runAction(seq);
            }
            else if(pGameFish->getSpriteID()>=2034 && pGameFish->getSpriteID()<=2037)
            {
                pGameFish->m_pFish->setScale(1.5f);
                CCArray* shadowAry = pGameFish->m_pFish->getChildren();
                if (shadowAry!=NULL)
                {
                    CCSprite* shadow = NULL;
                    for (unsigned int i = 0; i < shadowAry->count(); i++)
                    {
                        shadow = (CCSprite*)shadowAry->objectAtIndex(i);
                        if(shadow != NULL)
                        {
                            if(shadow->getTag() == 100)//是鱼
                            {
                            }
                            else//是圆盘
                            {
                                shadow->runAction(CCSequence::create(CCRotateTo::create(1.5f,1080),NULL));
                            }
                        }
                    }
                }
                addChildToBatch(true, pGameFish->m_pFish, pGameFish->getFilePath());
                float fTime = pGameFish->getLoopTimes()*pGameFish->getFrameCount()*pGameFish->getDuration();
                CCSequence* seq = CCSequence::create(CCDelayTime::create(fTime), callFunND, NULL);
                this->runAction(seq);
            }
            else
            {
                if (pGameFish -> getSpriteID() >= 2015 && pGameFish->getSpriteID() <= 2017)
                {
                    pGameFish->m_pFish->runAction(CCRepeatForever::create(fishDieRandAct2(0.1f)));
                }
                else if(pGameFish->getSpriteID() == 2018)//美人鱼
                {
                    CCDelayTime* pDelay = CCDelayTime::create(0.1f);
                    CCRotateBy* pRotateBy1 = CCRotateBy::create(0, 90);
                    CCScaleTo* pScale = CCScaleTo::create(0, 1.3f);
                    CCSequence* pSeq = CCSequence::create(pScale, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, pRotateBy1, pDelay, NULL);
                    CCScaleTo* pScaleTo = CCScaleTo::create(1.5f, 0);
                    CCSpawn* pSpawn = CCSpawn::create(pSeq, pScaleTo, NULL);
                    pGameFish->m_pFish->runAction(pSpawn);
                }
                else
                {
                    if(pGameFish->getDiedAnimate())
                    {
                        pGameFish->m_pFish->runAction(CCRepeatForever::create(fishDieRandAct2(0.1f)));
                    }
                    else
                    {
                        CCSequence* seq0 = fishDieRandAct2(0.1f);
                        CCRepeat* repeat = CCRepeat::create(seq0, 4);
                        pGameFish->m_pFish->runAction(repeat);
                    }
                }
                addChildToBatch(true, pGameFish->m_pFish, pGameFish->getFilePath());
                float fTime = pGameFish->getLoopTimes()*pGameFish->getFrameCount()*pGameFish->getDuration();
                CCSequence* seq = CCSequence::create(CCDelayTime::create(fTime), callFunND, NULL);
                this->runAction(seq);
            }
        }
        delete pGameFish;
        pGameFish = NULL;
    }
    else
    {
        this->runAction(callFunND);
    }
	
	//得分数字显示
	if(KIND_ID == NZNH_KIND_ID || KIND_ID == FB_KIND_ID || KIND_ID == YQS_KIND_ID || KIND_ID == JCBY_KIND_ID)
	{
		if (nFishScore != 0)
        {
			char temp[20];
			sprintf(temp,"%d",nFishScore);
			CCLabelAtlas* fishScore = CCLabelAtlas::create(temp,"CatchFish01/goldNum5.png",55,61,'0');
			fishScore -> setAnchorPoint(ccp(0.5,0.5));
			fishScore -> setScale(0.5f);
			fishScore -> setPosition(gameFish->m_pFish->getPosition());
			this -> addChild(fishScore, 17);
			CCMoveBy* moveBy0 = CCMoveBy::create(0,ccp(0,0));
			CCMoveBy* moveBy1 = CCMoveBy::create(0,ccp(0,0));
            CCSpawn* spawn1 = NULL;
			CCSpawn* spawn2 = NULL;
			if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID <GAME_PLAYER)
			{
				fishScore -> setRotation(180);
				moveBy0 = CCMoveBy::create(0.1f, ccp(0, -130));
				moveBy1 = CCMoveBy::create(0.15f, ccp(0, 30));
				spawn1 = CCSpawn::create(CCScaleTo::create(0.1f,1),moveBy0,NULL);
                if (gameFish->getMultiple() >= 60)
                {
                    spawn2 = CCSpawn::create(CCScaleTo::create(0.15f,1.4f),moveBy1,NULL);
                }
                else
                {
                    spawn2 = CCSpawn::create(CCScaleTo::create(0.15f,0.7f),moveBy1,NULL);
                }
			}
			else
			{
				moveBy0 = CCMoveBy::create(0.1f, ccp(0, 130));
				moveBy1 = CCMoveBy::create(0.15f, ccp(0, -30));
				spawn1 = CCSpawn::create(CCScaleTo::create(0.1f,1),moveBy0,NULL);
                if (gameFish->getMultiple() >= 60)
                {
                    spawn2 = CCSpawn::create(CCScaleTo::create(0.15f,1.4f),moveBy1,NULL);
                }
                else
                {
                    spawn2 = CCSpawn::create(CCScaleTo::create(0.15f,0.7f),moveBy1,NULL);
                }
            }
            CCDelayTime* pDelay = CCDelayTime::create(0.6f);
            if (gameFish->getMultiple() >= 60)
            {
                pDelay = CCDelayTime::create(1.2f);
            }
			CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
			fishScore->runAction(CCSequence::create(spawn1, spawn2, pDelay, CCSpawn::create(CCScaleTo::create(0.2f,0.1f),CCFadeOut::create(0.2f),NULL),func, NULL));
		}
	}
	if(KIND_ID == 2000)
	{
		if (nFishScore != 0)
		{
			char temp[20];
			sprintf(temp,"%d",nFishScore);
			CCLabelAtlas* fishScore = CCLabelAtlas::create(temp,"CatchFish01/goldNum5.png",55,61,'0');
			fishScore -> setAnchorPoint(ccp(0.5,0.5));
			fishScore -> setPosition(gameFish->m_pFish->getPosition());
			this -> addChild(fishScore, 17);
			CCMoveBy* moveBy0 = NULL;
			CCMoveBy* moveBy1 = NULL;
			if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID <GAME_PLAYER)
			{
				fishScore -> setRotation(180);
				moveBy0 = CCMoveBy::create(0.2f, ccp(0, -70));
				moveBy1 = CCMoveBy::create(0.2f, ccp(0, 30));
			}
			else
			{
				moveBy0 = CCMoveBy::create(0.2f, ccp(0, 70));
				moveBy1 = CCMoveBy::create(0.2f, ccp(0, -30));
			}
			CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
			fishScore->runAction(CCSequence::create(moveBy0, moveBy1, CCDelayTime::create(0.7f),func, NULL));
		}
	}
    
    
}

void GameMainScene::showBossScore(CCNode* node, void* nScore)
{
    long score = (long)nScore;
    CCLabelAtlas* pLabel = (CCLabelAtlas*)node;
    if(pLabel != NULL)
    {
        long nCurrentScore = atol(pLabel->getString());
        char temp[16];
        sprintf(temp, "%ld", nCurrentScore+score);
        pLabel -> setString(temp);
    }
}

void GameMainScene::showMeScore(CCNode* pNode, void* fishScore)
{
    int nTag = pNode->getTag();
    if(nTag >= 0 && nTag <= 3)
    {
        long nFishScore = (long)fishScore;
        m_barrelLayer[nTag]->setLabelScore(nFishScore);
    }
    
}

//鱼死随机跳动动画
CCSequence* GameMainScene::fishDieRandAct(float fDuration)
{
    CCDelayTime* pDelay = CCDelayTime::create(fDuration);
    CCRotateBy* pRotateBy1 = CCRotateBy::create(0, 15);
    CCSequence* pSeq = CCSequence::create(pRotateBy1, pDelay, NULL);
    
	return pSeq;
}

CCSequence* GameMainScene::fishDieRandAct2(float fDuration)
{
    CCDelayTime* pDelay = CCDelayTime::create(0.1f);
    CCRotateBy* pRotateBy1 = CCRotateBy::create(0, 90);
    CCSequence* pSeq = CCSequence::create(pRotateBy1, pDelay, NULL);
    
    return pSeq;
}

void GameMainScene::createMoneyCall(CCNode* pNode, void* v)
{
    long lVal = (long)v;
    bool bTryCannon = lVal%10;
    long nFishID = 0;
    if(bTryCannon)
    {
        nFishID = (lVal-1)/10;
    }
    else
    {
        nFishID = lVal/10;
    }
	GameFish* pGameFish = NULL;
	for(unsigned int i = 0; i < m_vctFishFactory.size(); i++)
	{
		pGameFish = m_vctFishFactory[i];
		if(pGameFish == NULL)
			continue;
		if(pGameFish->getFishID() != nFishID)
			pGameFish = NULL;
		else
			break;
	}
	if(pGameFish == NULL || pGameFish->getAppear())
		return;
	//创建金币
	if(KIND_ID == NZNH_KIND_ID || KIND_ID == FB_KIND_ID || KIND_ID == YQS_KIND_ID || KIND_ID == JCBY_KIND_ID)
	{
		createMoney(pGameFish->getHitUser(), pGameFish->m_pFish->getPosition(),pGameFish->getMultiple(),(float)pGameFish->m_pFish->getRotation()+90, pGameFish->getFishScore(), bTryCannon);
        //createMoney(pGameFish->getHitUser(), ccp(pGameFish->m_pFish->getPositionX(), pGameFish->m_pFish->getPositionY()+80), pGameFish->getMultiple(),(float)pGameFish->m_pFish->getRotation()+90, pGameFish->getFishScore(), bTryCannon);
	}
	if(KIND_ID == 2000)
	{
		createMoney2(pGameFish->getHitUser(), pGameFish->m_pFish->getPosition(),pGameFish->getMultiple(),(float)pGameFish->m_pFish->getRotation()+90);
	}
	
	//活鱼消失
	for (std::vector<GameFish*>::iterator it = m_vctFishFactory.begin(); it != m_vctFishFactory.end();)
	{
		if (*it == NULL)
		{
            *it++;//修改死循环问题
			ERROR_LOG;
			continue;
		}
        if(KIND_ID == YQS_KIND_ID)
        {
            if(pGameFish->getSpriteID()>=1016 || pGameFish->isProperty(enLine))
            {
                for(int k = 0; k < m_arrLockFish->count(); k++)
                {
                    CCSprite* pLockDi = (CCSprite*)m_arrLockFish->objectAtIndex(k);
                    if(pLockDi == NULL) continue;
                    if(pLockDi->getTag() == pGameFish->getFishID())
                    {
                        m_arrLockFish -> removeObject(pLockDi);
                        pLockDi -> removeFromParentAndCleanup(true);
                        break;
                    }
                }
            }
        }
        else
        {
            if(pGameFish->getSpriteID()>=1017 || pGameFish->isProperty(enLine))
            {
                for(int j = 0; j < m_arrLockFish->count(); j++)
                {
                    CCSprite* pLockDi = (CCSprite*)m_arrLockFish->objectAtIndex(j);
                    if(pLockDi == NULL) continue;
                    if(pLockDi->getTag() == pGameFish->getFishID())
                    {
                        m_arrLockFish -> removeObject(pLockDi);
                        pLockDi -> removeFromParentAndCleanup(true);
                        break;
                    }
                }
            }
        }
        for(int j = 0; j < m_arrQipao->count(); j++)
        {
            CCSprite* pQipao = (CCSprite*)m_arrQipao->objectAtIndex(j);
            if(pQipao == NULL) continue;
            if(pQipao->getTag() == pGameFish->getFishID())
            {
                pQipao -> setVisible(false);
//                m_arrQipao -> removeObject(pQipao);
//                pQipao -> removeFromParentAndCleanup(true);
                break;
            }
            
        }
		if (*it == pGameFish)
		{
			it = m_vctFishFactory.erase(it);
			addChildToBatch(false, pGameFish->m_pFish, pGameFish->getFilePath());
			delete pGameFish;
			pGameFish = NULL;
			break;
		}
		else
			it++;
	}
}

//定屏
void GameMainScene::freezedScreen(float dt)
{
	m_nFreezeTime--;
	if(m_nFreezeTime == 0)
	{
		m_bFreeze = false;
		unschedule(schedule_selector(GameMainScene::freezedScreen));
	}
}

void GameMainScene::setCmdLine(LPTSTR lpCmdLine)
{
	m_sCmdLine=lpCmdLine;
	wprintf(L"%s\n",lpCmdLine);
}

bool GameMainScene::OnEventUserEnter(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)
{
    if (wChairID >= GAME_PLAYER)
    {
        ERROR_CHECK;
        return true;
    }
	//检测其他座位上有没有人，没有人则将炮台隐藏
	for(int i = 0; i < GAME_PLAYER; i++)
	{
		if(m_pClientKernel->GetUserInfo(i) == NULL)
		{
			m_barrelLayer[i]->setVisible(false);
			userComeInorLeave(i,false,i);
		}
	}
	m_nEveryBullet[wChairID] = 0;
	userComeInorLeave(wChairID,true,wChairID);
	setMeInformation(m_pClientKernel->GetMeChairID(), m_pClientKernel->GetMeChairID());
	m_barrelLayer[wChairID]->setVisible(true);
	std::string str = m_pClientKernel->GetUserInfo(wChairID)->szNickName;
    str = CCharCode::GB2312ToUTF8(m_pClientKernel->GetUserInfo(wChairID)->szNickName);
    setNameLength(wChairID, str);
	//m_barrelLayer[wChairID]->getLabelName()->setString(str.c_str());
	char temp[20];
	sprintf(temp,"%d",m_nOwnTotalMoney[wChairID]);
	m_barrelLayer[wChairID]->getGamelastAtlas()->setString(temp);
    
	if((wChairID < GAME_PLAYER/2 && m_nMeRealChairID >= GAME_PLAYER/2)||(wChairID >= GAME_PLAYER/2 && m_nMeRealChairID <GAME_PLAYER/2))
	{
        m_barrelLayer[wChairID]->getMultAtlas()->setRotation(180);
        m_barrelLayer[wChairID]->getScoreBlockSprite()->setRotation(180);
        m_barrelLayer[wChairID]->getScoreBlockSprite()->setFlipX(true);
        m_barrelLayer[wChairID]->getScoreBlockSprite()->setFlipY(true);
        m_barrelLayer[wChairID]->getScoreBlockSprite()->setPosition(ccp(m_barrelLayer[wChairID]->getCannonBg()->getContentSize().width/2+20, m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/3*2+19));
        
	}
    m_barrelLayer[wChairID]->setMoneyPoint(m_nMeRealChairID, wChairID);
    m_barrelLayer[wChairID]->setChatSpriteBg(m_nMeRealChairID, wChairID);
    
    if(m_nMeRealChairID == 0 || m_nMeRealChairID == 1)
    {
        if(wChairID == 0)
        {
            m_barrelLayer[wChairID]->getScoreBlockSprite()->setPosition(ccp(-m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().width+90, m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/3*2+25));
            m_barrelLayer[wChairID]->getLabelName()->setAnchorPoint(ccp(1.0f, 0.5f));
            m_barrelLayer[wChairID]->getLabelName()->setPosition(ccp(-m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().width+200, m_barrelLayer[wChairID]->getCannonBg()->getContentSize().height/3-10));
        }
        else if(wChairID == 1)
        {
            m_barrelLayer[wChairID]->getScoreBlockSprite() -> setPosition(ccp(m_barrelLayer[wChairID]->getCannonBg()->boundingBox().size.width + m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().width/2+20,m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/3*2+25));
            m_barrelLayer[wChairID]->getGamelastAtlas()->setAnchorPoint(ccp(0, 0.5f));
            m_barrelLayer[wChairID]->getGamelastAtlas()->setPosition(ccp(15, m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/2));
            m_barrelLayer[wChairID]->getLabelName()->setAnchorPoint(ccp(0, 0.5f));
            m_barrelLayer[wChairID]->getLabelName() -> setPosition(ccp(m_barrelLayer[wChairID]->getCannonBg()->getContentSize().width+20, m_barrelLayer[wChairID]->getCannonBg()->getContentSize().height/3-10));
        }
        else if(wChairID == 2)
        {
            m_barrelLayer[wChairID]->getScoreBlockSprite() -> setPosition(ccp(-m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().width+90,m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/3*2+25));
            m_barrelLayer[wChairID]->getLabelName()->setRotation(180);
            m_barrelLayer[wChairID]->getLabelName()->setAnchorPoint(ccp(0, 0.5f));
            m_barrelLayer[wChairID]->getLabelName()->setPosition(ccp(-30, m_barrelLayer[wChairID]->getCannonBg()->getContentSize().height/3-10));
            m_barrelLayer[wChairID]->getGamelastAtlas()->setAnchorPoint(ccp(0, 0.5f));
            m_barrelLayer[wChairID]->getGamelastAtlas()->setPosition(ccp(15, m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/2));
            
        }
        else if(wChairID == 3)
        {
            m_barrelLayer[wChairID]->getScoreBlockSprite() -> setPosition(ccp(m_barrelLayer[wChairID]->getCannonBg()->boundingBox().size.width + m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().width/2+20,m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/3*2+25));
            m_barrelLayer[wChairID]->getLabelName()->setRotation(180);
            m_barrelLayer[wChairID]->getLabelName()->setAnchorPoint(ccp(1, 0.5f));
            m_barrelLayer[wChairID]->getLabelName()->setPosition(ccp(200, m_barrelLayer[wChairID]->getCannonBg()->getContentSize().height/3-10));
            m_barrelLayer[wChairID]->getGamelastAtlas()->setAnchorPoint(ccp(1, 0.5f));
            m_barrelLayer[wChairID]->getGamelastAtlas()->setPosition(ccp(m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().width-15, m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/2));
        }
    }
    else if(m_nMeRealChairID == 2 || m_nMeRealChairID == 3)
    {
        if(wChairID == 0)
        {
            m_barrelLayer[wChairID]->getLabelName()->setAnchorPoint(ccp(0, 0.5f));
            m_barrelLayer[wChairID]->getLabelName()->setRotation(180);
            m_barrelLayer[wChairID]->getLabelName()->setPosition(ccp(-30, m_barrelLayer[wChairID]->getCannonBg()->getContentSize().height/3-10));
            m_barrelLayer[wChairID]->getGamelastAtlas()->setAnchorPoint(ccp(0, 0.5f));
            m_barrelLayer[wChairID]->getGamelastAtlas()->setPosition(ccp(15, m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/2));
            m_barrelLayer[wChairID]->getScoreBlockSprite()->setPosition(ccp(-m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().width+90, m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/3*2+25));
        }
        else if(wChairID == 1)
        {
            m_barrelLayer[wChairID]->getLabelName()->setRotation(180);
            m_barrelLayer[wChairID]->getLabelName()->setAnchorPoint(ccp(1.0f, 0.5f));
            m_barrelLayer[wChairID]->getLabelName()->setPosition(ccp(210, m_barrelLayer[wChairID]->getCannonBg()->getContentSize().height/3-10));
            m_barrelLayer[wChairID]->getGamelastAtlas()->setAnchorPoint(ccp(1, 0.5f));
            m_barrelLayer[wChairID]->getGamelastAtlas()->setPosition(ccp(m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().width-15, m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/2));
            m_barrelLayer[wChairID]->getScoreBlockSprite()->setPosition(ccp(m_barrelLayer[wChairID]->getCannonBg()->boundingBox().size.width + m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().width/2+30, m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/3*2+25));
        }
        else if(wChairID == 2)
        {
            m_barrelLayer[wChairID]->getScoreBlockSprite()->setPosition(ccp(-m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().width+90, m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/3*2+25));
            m_barrelLayer[wChairID]->getLabelName()->setAnchorPoint(ccp(1.0f, 0.5f));
            m_barrelLayer[wChairID]->getLabelName()->setPosition(ccp(-m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().width+200, m_barrelLayer[wChairID]->getCannonBg()->getContentSize().height/3-10));
        }
        else if(wChairID == 3)
        {
            m_barrelLayer[wChairID]->getGamelastAtlas()->setAnchorPoint(ccp(0, 0.5f));
            m_barrelLayer[wChairID]->getGamelastAtlas()->setPosition(ccp(15, m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/2));
            m_barrelLayer[wChairID]->getLabelName()->setAnchorPoint(ccp(1.0f, 0.5f));
            m_barrelLayer[wChairID]->getLabelName()->setPosition(ccp(370, m_barrelLayer[wChairID]->getCannonBg()->getContentSize().height/3-10));
            m_barrelLayer[wChairID]->getScoreBlockSprite()->setPosition(ccp(m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().width+80, m_barrelLayer[wChairID]->getScoreBlockSprite()->getContentSize().height/3*2+25));
        }
    }
    
	return true;
}

bool GameMainScene::OnEventSocket(const TCP_Command & Command, const void * pData, WORD wDataSize)
{
    m_nTimesOnline = 10;
	if (MDM_GF_GAME!=Command.wMainCmdID&&Command.wSubCmdID!=SUB_GF_SYSTEM_MESSAGE)
	{
		//ERROR_LOG;
		return false;
	}
    if(m_bClickExitMenu && Command.wSubCmdID!=SUB_S_BULLET_COUNT)
        return true;
	switch (Command.wSubCmdID)
	{
	case SUB_GF_SYSTEM_MESSAGE://公告
		{
			CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
			if (wDataSize<=(sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString))) return false;
			if (pSystemMessage->wType&0x0020)
			{
			}

			return true;
		}
	case SUB_S_BULLET_COUNT://设置分数
		{
			if (!(wDataSize==sizeof(CMD_S_BulletCount))) return false;
			CMD_S_BulletCount * BulletCount=(CMD_S_BulletCount *)pData;
			if (BulletCount->bAdd)
			{
				setUserScore(BulletCount->wChairID,BulletCount->lScore);
			}
			else
			{
				setUserScore(BulletCount->wChairID,0);
			}
            if(!this->isTouchEnabled() && BulletCount->wChairID == m_nMeRealChairID)
            {
                this->setTouchEnabled(true);
            }
            if(BulletCount->wChairID == m_nMeRealChairID)
            {
                if(!BulletCount->bAdd)//下分
                {
                    exitMainGame();
                }
            }
			return true;
		}
	case SUB_S_BONUS_INFO://设置倍率
		{
			if (!(wDataSize==sizeof(CMD_S_CannonMultiple))) return false;
			CMD_S_CannonMultiple * pCannonMultiple=(CMD_S_CannonMultiple *)pData;
			setBeiLv(pCannonMultiple->wChairID,pCannonMultiple->nCannonMultiple);
			return true;
		}
	case SUB_S_TRACE_POINT://增加鱼
		{
            if(!m_bReceiveScene)
                break;
            if(m_bInitCompleted == false)
            {
                m_bInitCompleted = true;
                //播放背景音乐
                m_nBgMusicIndex += 1;
                m_nBgMusicIndex %= 3;
                playSoundByID(false, m_nBgMusicIndex%3);
                //播放炮台音效
                
            }
			WORD wTraceCount=wDataSize/sizeof(tagFishPack);
			tagFishPack * pFishPack=(tagFishPack *)pData;
			for (int i=0; i<wTraceCount; i++)
			{
				m_bFishingOffSeason=false;
				if (pFishPack->nCreateDelayTime==0)
				{
					addFish(pFishPack);
				}
				else
				{
					addStorgeFish(pFishPack);
				}
				pFishPack++;
			}	
			return true;
		}
	case SUB_S_CAPTURE_FISH://捕获鱼
		{
			CMD_S_CaptureFish * pCaptureFish=(CMD_S_CaptureFish *)pData;
			if (NULL==pCaptureFish)
			{
				ERROR_LOG;
				return true;
			}
			WORD wChairID=pCaptureFish->wChairID;
            /*
			if (pCaptureFish->bSuperCannon)
			{
				setSuperPao(wChairID);
			}
			else
			{
				cancelSuperPao(wChairID);
			}*/
            if(wChairID == m_nMeRealChairID)
            {
                setUserScore(wChairID,pCaptureFish->nUserScore);
            }
			else
            {
                setUserScore(wChairID, pCaptureFish->nUserScore+pCaptureFish->nTryScore);
            }
			setCapture(wChairID,pCaptureFish);

			return true;
		}
    case SUB_S_BOX_PRIZE:
        {
            return true;
        }
    case SUB_S_ADD_BOX_RES:
        {
            return true;
        }
	case SUB_S_CHANGE_SCENE://切换场景
		{
			CMD_S_ChangeScene *pChangeScene=(CMD_S_ChangeScene *)pData;
            m_nChangeSceneIndex = pChangeScene->SceneIndex;
            if (pChangeScene->RmoveID == 1)
            {
                if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
                {
                    changeFishScene(false);//宝箱来袭
                }
                else
                {
                    changeFishScene(true);
                }
            }
            else
            {
                changeFishScene(true);//鱼潮来临
            }
			return true;
		}
	case SUB_S_LOCKFISH://锁定鱼
		{
			CMD_S_LockFishId *pChangeScene=(CMD_S_LockFishId *)pData;
			lockFishID(pChangeScene->iChairID,pChangeScene->lFishID);
			return true;
		}
	case SUB_S_USER_SHOOT://发射子弹
		{
			if (!(wDataSize==sizeof(CMD_S_UserShoot))) return false;
			CMD_S_UserShoot * pUserShoot=(CMD_S_UserShoot *)pData;
			if (pUserShoot->wChairID!=m_pClientKernel->GetMeChairID())
			{
				userShoot(pUserShoot->wChairID,pUserShoot->fAngle,pUserShoot->wChairID,pUserShoot->bAndroid,pUserShoot->dwBulletID, pUserShoot->dwCount, pUserShoot->cbAttribute);
			}
			setUserScore(pUserShoot->wChairID,pUserShoot->nUserScore);
            if(!m_barrelLayer[pUserShoot->wChairID]->isVisible())
            {
                m_barrelLayer[pUserShoot->wChairID]->setVisible(true);
            }
            return true;
        }
        case SUB_S_UPDATE_BOSS_SCORE://改变BOSS倍率
		{
			if (wDataSize!=sizeof(CMD_S_UpdateBossScore))
			{
				ERROR_LOG;
				return true;
			}
			CMD_S_UpdateBossScore * pUpdateBossScore=(CMD_S_UpdateBossScore *)pData;
			setBossScore(pUpdateBossScore->nBossScore);
			return true;
		}
	case SUB_S_JUMP_BOMB://捕获博乐弹
		{
            if (wDataSize!=sizeof(CMD_S_JumpBomb))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_JumpBomb * pJumpBomb=(CMD_S_JumpBomb *)pData;
            setJumpBomb(pJumpBomb);
			return true;
		}
	case SUB_S_TASK_INFO:
		{
			return true;
		}
	case SUB_S_TASK_RECEIVED:				//点击可领取任务奖励按钮收到的消息
		{
			return true;
		}
	case SUB_S_TIME_PRIZE_INFO:			//当一天内玩家第二次及以后进来时受到时间消息
		{
			return true;
		}
	case SUB_S_TIME_PRIZE_RECEIVED:		//当玩家点击可领取按钮之后，收到的消息
		{
			return true;
		}
    case SUB_S_MOBILE_MONEY://话费鱼
        {
            if(wDataSize != sizeof(CMD_S_MobileMoney))
            {
                ERROR_LOG;
                return true;
            }
            if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                return true;
            }
            CMD_S_MobileMoney* pMobileMoney = (CMD_S_MobileMoney*)pData;
            if(!m_bHaveWnd)
            {
                m_bHaveWnd = true;
                m_wMobileMoney = pMobileMoney->wMoneyCount;
                m_bFirstMobileMoney = false;
                m_sShareContent = CCharCode::GB2312ToUTF8(pMobileMoney->szContent);
                m_sShareURL = CCharCode::GB2312ToUTF8(pMobileMoney->szURL);
                showFirstMobileMoney();
            }
            return true;
        }
    case SUB_S_FIRST_MOBILE_MONEY:
        {
            if(wDataSize != sizeof(CMD_S_MobileMoney))
            {
                ERROR_LOG;
                return true;
            }
            if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                return true;
            }
            CMD_S_MobileMoney* pMobileMoney = (CMD_S_MobileMoney*)pData;
            if(!m_bHaveWnd)
            {
                m_bHaveWnd = true;
                m_wMobileMoney = pMobileMoney->wMoneyCount;
                m_bFirstMobileMoney = true;
                m_sShareContent = CCharCode::GB2312ToUTF8(pMobileMoney->szContent);
                m_sShareURL = CCharCode::GB2312ToUTF8(pMobileMoney->szURL);
                showFirstMobileMoney();
            }
            return true;
        }
    case SUB_S_SCORE_FULL://用户分满了，弹出提示框，点击确定退出游戏
        {
            //停止自动开炮
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eXinShouRoomExit);
            this -> unschedule(schedule_selector(GameMainScene::startBullet));
            CReminderLayer* pReminderLayr = CReminderLayer::create(0);
            this -> addChild(pReminderLayr, 999);
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                pReminderLayr -> setRotation(180);
            }
            return true;
        }
    case SUB_S_BOSS_COMING://boss来袭
        {
            return true;
        }
    case SUB_S_MAN_EATING_FISH://出现食人鱼
        {
            return true;
        }
    case SUB_S_MAN_EATING_FISH_NEW_FISH://食人鱼新鱼
        {
            return true;
        }
    case SUB_S_EAT_SCORE://吃分变大
        {
            return true;
        }
    case SUB_S_MAN_EATING_FISH_FINISH://移除所有食人鱼
        {
            return true;
        }
    case SUB_S_EAT_3RD://食人鱼自爆
        {
            return true;
        }
    case SUB_S_MAN_EATING_BOSS_DIE://打死美人鱼
        {
            return true;
        }
    case SUB_S_MAN_EATING_FISH_SPEAK:
        {
            return true;
        }
    case SUB_S_MOBILE_MONEY_SUCCESS://收到从服务端发过来分享成功，可以显示分享成功获得话费的界面
        {
            if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                return true;
            }
            char temp[128];
            sprintf(temp, "分享成功，获得%d元话费", m_wMobileMoney);
            CMainLogic::sharedMainLogic()->ShowMessage(temp, eMBOK, eShareAction);

            FirstChargeLayer* pFirstChargeLayer = (FirstChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10300);
            if(pFirstChargeLayer != NULL)
            {
                pFirstChargeLayer->removeFromParent();
            }
            return true;
        }
    case SUB_S_RETURN_MOBILE_MONEY:
        {
            return true;
        }
    case SUB_S_MOBILE_CHARGE:
        {
            return true;
        }
    case SUB_S_TRY_CANNON://试用炮开始,三个技能、加减倍率、炮台不能点击（隐藏倍率），其他均能点击，如果当前正有技能，那么暂停一切技能
        {
            break;
        }
    case SUB_S_TRY_CANNON_CHECK:
        {
            break;
        }
    case SUB_S_TRY_CANNON_END://试用炮结束
        {
            break;
        }
    case SUB_S_EQUIPMENT_DRAGON_COUNT://幸运点
        {
            return true;
        }
    case SUB_S_NEW_GUID_GIFT://领取新手奖励
        {
            if(wDataSize < sizeof(CMD_S_NewGuidGift))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_NewGuidGift* pNewGuidGift = (CMD_S_NewGuidGift*)pData;
            if(pNewGuidGift != NULL)
            {
                //更新玩家分数
                if(pNewGuidGift->bSuccess)
                {
                    setUserScore(m_nMeRealChairID, pNewGuidGift->nCurrentScore);
                    GetPriseLayer* pPriseLayer = GetPriseLayer::create(1, pNewGuidGift->nScoreGift, "");
                    CCDirector::sharedDirector()->getRunningScene()->addChild(pPriseLayer, 15);
                }
            }
            break;
        }
    case SUB_S_WIN_PEARL://打中珍珠
        {
            if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                break;
            }
            if(wDataSize < sizeof(CMD_S_WinPearl))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_WinPearl* pWinPearl = (CMD_S_WinPearl*)pData;
            if(pWinPearl != NULL)
            {
                //getDantouType(pWinPearl->nPearlType);
            }
            break;
        }
    case SUB_S_EXCHANGE_PEARL:
        {
            if(wDataSize < sizeof(CMD_S_ExchangePearl))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_ExchangePearl* pExchangePearl = (CMD_S_ExchangePearl*)pData;
            if(pExchangePearl != NULL)
            {
                if(pExchangePearl->cbResult == 1)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(pExchangePearl->szResult).c_str(), eMBOK, eExitFBToHall);
                }
                else
                {
                    CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(pExchangePearl->szResult).c_str(), eMBOK);
                }
            }
            break;
        }
    case SUB_S_MATCH_RANK://比赛（新版本去掉比赛）
        {
            break;
        }
    case SUB_S_SEND_PEARL_INFO:
        {
            if(wDataSize < sizeof(CMD_S_SendPearlInfo))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_SendPearlInfo* pPearlInfo = (CMD_S_SendPearlInfo*)pData;
            if(pPearlInfo != NULL)
            {
                FuBenLayer* pFuBenLayer = FuBenLayer::create(1, pPearlInfo->nPearl1Price, pPearlInfo->nPearl2Price, pPearlInfo->nPearl3Price, pPearlInfo->nLastScore);
                this -> addChild(pFuBenLayer, 110);
                if(m_nMeRealChairID >= GAME_PLAYER/2)
                {
                    pFuBenLayer->setRotation(180);
                }
            }
            break;
        }
    case SUB_S_VIP_LEVEL://玩家VIP等级
        {
            if(wDataSize < sizeof(CMD_S_VipLevel))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_VipLevel* pVipLevel = (CMD_S_VipLevel*)pData;
            if(pVipLevel != NULL)
            {
                for(int i = 0; i < GAME_PLAYER; i++)
                {
                    if(pVipLevel->cbVipLevel[i] > 0)//此玩家是VIP
                    {
                        //将昵称设置为红色
                        //m_barrelLayer[i]->getLabelName()->setColor(ccc3(255, 0, 0));
                    }
                    else//此玩家不是VIP
                    {
                        //m_barrelLayer[i]->getLabelName()->setColor(ccc3(255, 255, 255));
                    }
                }
            }
            break;
        }
        case SUB_S_USE_PEARL://使用弹头
        {
            if(wDataSize < sizeof(CMD_S_UsePearl))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_UsePearl* UsePearl = (CMD_S_UsePearl*)pData;
            if(UsePearl != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameUserDantouSuccessTag);
                setUserScore(m_nMeRealChairID, UsePearl->nCurrentScore);
                CBeautfFishDeath* pBeautfFishDeath = CBeautfFishDeath::create(UsePearl->nScore, 1);
                this->addChild(pBeautfFishDeath, 200);
                if(m_nMeRealChairID>=GAME_PLAYER/2)
                {
                    pBeautfFishDeath->setRotation(180);
                }
                m_nDantouTotalNum--;
                if(m_nDantouTotalNum <= 0)
                {
                    m_spDantouDi -> setVisible(false);
                }
                else
                {
                    char temp[16];
                    sprintf(temp, "%d", m_nDantouTotalNum);
                    m_labelDantou -> setString(temp);
                }
            }
            break;
        }
        case SUB_S_BAO_JI://爆机
        {
            WORD* wChairID = (WORD*)pData;
            m_bBaojiStatus[*wChairID] = true;
            m_barrelLayer[*wChairID]->showBaoji(true);
            if((*wChairID >= GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER/2) || (*wChairID < GAME_PLAYER/2 && m_nMeRealChairID >= GAME_PLAYER/2))
            {
                m_barrelLayer[*wChairID]->getBaojiSprite()->setRotation(180);
            }
            break;
        }
        case SUB_S_BAO_JI_MESSAGE://爆机提醒，点击确定退出本房间
        {
            if(wDataSize < sizeof(CMD_S_BaoJiMessage))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_BaoJiMessage* pBaojiMessage = (CMD_S_BaoJiMessage*)pData;
            if(pBaojiMessage != NULL)
            {
                CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(pBaojiMessage->szContent).c_str(), eMBOK, eExitFBToHall);
            }
            break;
        }
        case SUB_S_SUPER_CANNON:
        {
            if(wDataSize < sizeof(CMD_S_SuperCannon))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_SuperCannon* pSuperCannon = (CMD_S_SuperCannon*)pData;
            if(pSuperCannon != NULL)
            {
                m_bUserSuperPao[pSuperCannon->wChairID] = pSuperCannon->bSuperCannnon;
                int nIndex = 2;
                if (m_nBeiLv[pSuperCannon->wChairID]<=m_nExchargeTimes/2*m_nCellScore)
                {
                    nIndex= 2 ;
                }
                else
                {
                    nIndex = 3;
                }
                if(pSuperCannon->wChairID>=GAME_PLAYER/2 && m_nMeRealChairID < GAME_PLAYER/2)
                {
                    m_barrelLayer[pSuperCannon->wChairID]->getSuperPaoSprite()->setRotation(180);
                }
                else if(pSuperCannon->wChairID < GAME_PLAYER/2 && m_nMeRealChairID >= GAME_PLAYER/2)
                {
                    m_barrelLayer[pSuperCannon->wChairID]->getSuperPaoSprite()->setRotation(180);
                }
                if(pSuperCannon->bSuperCannnon)
                {
                    m_barrelLayer[pSuperCannon->wChairID]->superMenu(nIndex, true);
                    playSoundByID(true, 36);
                    m_barrelLayer[pSuperCannon->wChairID]->getSuperPaoSprite()->setVisible(true);
                }
                else
                {
                    m_barrelLayer[pSuperCannon->wChairID]->superMenu(nIndex, false);
                    playSoundByID(true, 37);
                    m_barrelLayer[pSuperCannon->wChairID]->getSuperPaoSprite()->setVisible(false);
                }
                
            }
            break;
        }
        case SUB_S_USER_LUCKY:
        {
            if(wDataSize < sizeof(CMD_S_ServerData))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_ServerData* pServerData = (CMD_S_ServerData*)pData;
            char temp[64];
            sprintf(temp, "幸运值: %0.2f, %d", pServerData->fLucky, pServerData->nStorage);
            m_luckLabel -> setString(temp);
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                m_luckLabel -> setRotation(180);
            }
            break;
        }
        case SUB_S_USER_LUCKY2:
        {
            if(wDataSize < sizeof(CMD_S_ServerData))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_ServerData* pServerData = (CMD_S_ServerData*)pData;
            char temp[64];
            sprintf(temp, "实际值: %0.2f, %d", pServerData->fLucky, pServerData->nStorage);
            m_luckLabel2 -> setString(temp);
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                m_luckLabel2 -> setRotation(180);
            }
            break;
        }
        case SUB_S_CHAT:
        {
            if(wDataSize < sizeof(CMD_S_Chat))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_Chat* pChat = (CMD_S_Chat*)pData;
            if(pChat != NULL)
            {
                if(pChat->wChairID != m_nMeRealChairID)
                {
                    char tempName[64];
                    sprintf(tempName, "%s:", CCharCode::GB2312ToUTF8(pChat->szNickName).c_str());
                    char tempData[256];
                    sprintf(tempData, "%s %s", tempName, CCharCode::GB2312ToUTF8(pChat->szContent).c_str());
                    m_barrelLayer[pChat->wChairID]->setChatDataName(tempName, tempData);
                    m_barrelLayer[pChat->wChairID]->setShowOneChatBg(true);
                    //开启定时器
                    m_barrelLayer[pChat->wChairID]->scheduleOnce(schedule_selector(BarrelLayer::hideChatBg), 5.0f);
                }
            }
            break;
        }
        case SUB_S_PPL_START:
        {
            if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
                break;
            if(wDataSize < sizeof(CMD_S_PPLStart))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_PPLStart* pPPLStart = (CMD_S_PPLStart*)pData;
            if(pPPLStart != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, ePPLStart);
                char temp[8];
                sprintf(temp, "%d", pPPLStart->nCountDown);
                m_spRemindBg -> setVisible(true);
                m_labelRemind -> setString(temp);
                m_nRemindNum = pPPLStart->nCountDown;
                m_nShowMenuTime = pPPLStart->nDefaultStartTime;
                
                m_bPPLZhuanComplete = false;
                m_bPaipaileStatus = true;
                for(int i = 0; i < GAME_PLAYER; i++)
                {
                    m_bPPLReceiveCancel[i] = false;
                    m_bPPLChairID[i] = pPPLStart->bChairID[i];
                }
                this -> schedule(schedule_selector(GameMainScene::showPaiRemindBg), 1.0f);
                SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                SimpleAudioEngine::sharedEngine()->playEffect("Music/bisaidaojishi.mp3");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                SimpleAudioEngine::sharedEngine()->playEffect("Music/bisaidaojishi.ogg");
#endif
                break;
            }
            break;
        }
        case SUB_S_PPL_RESULT://拍的过程中结果
        {
            if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
                break;
            if(wDataSize < sizeof(CMD_S_PPLResult))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_PPLResult* pPPLResult = (CMD_S_PPLResult*)pData;
            if(pPPLResult != NULL)
            {
//                int disX = rand()%800-400;
//                int disY = rand()%400-200;
//                int nMult = rand()%50;
//                createMoney(pPPLResult->wChairID, ccp(CLIENT_WIDTH/2+disX, CLIENT_HEIGHT/2+disY), nMult, 0, m_nBeiLv[pPPLResult->wChairID]);
                int nMult = rand()%3+4;
                if(nMult == 5)
                {
                    nMult = 4;
                }
                int nRotation = 0;
                int nIndexX = 0;
                int nIndexY = 0;
                if(pPPLResult->wChairID == 0)
                {
                    nRotation = 125;
                    createMoney3(pPPLResult->wChairID, ccp(CLIENT_WIDTH/2-70, CLIENT_HEIGHT/2-70), nMult, nRotation, m_nBeiLv[pPPLResult->wChairID]);
                }
                else if(pPPLResult->wChairID == 1)
                {
                    nRotation = 60;
                    createMoney3(pPPLResult->wChairID, ccp(CLIENT_WIDTH/2+40, CLIENT_HEIGHT/2-90), nMult, nRotation, m_nBeiLv[pPPLResult->wChairID]);
                }
                else if(pPPLResult->wChairID == 2)
                {
                    nRotation = 125;
                    createMoney3(pPPLResult->wChairID, ccp(CLIENT_WIDTH/2+40, CLIENT_HEIGHT/2+90), nMult, nRotation, m_nBeiLv[pPPLResult->wChairID]);
                }
                else if(pPPLResult->wChairID == 3)
                {
                    nRotation = 60;
                    createMoney3(pPPLResult->wChairID, ccp(CLIENT_WIDTH/2-70, CLIENT_HEIGHT/2+70), nMult, nRotation, m_nBeiLv[pPPLResult->wChairID]);
                }
                
                setUserScore(pPPLResult->wChairID, pPPLResult->nTotalScore);
            }
            break;
        }
        case SUB_S_PPL_END://拍的结果
        {
            if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
                break;
            if(wDataSize < sizeof(CMD_S_PPLResult))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_PPLResult* pPPLResult = (CMD_S_PPLResult*)pData;
            if(pPPLResult != NULL)
            {
                if(pPPLResult->wChairID == m_nMeRealChairID)
                {
                    m_bPaipaileStatus = false;
                    m_bIsPaipaileStatus = false;
                }
                CCNode* paiNode = (CCNode*)this->getChildByTag(ePaiNodeTag+pPPLResult->wChairID);
                if(paiNode != NULL)
                {
                    CCSprite* paiguang = (CCSprite*)paiNode->getChildByTag(6);
                    if(paiguang != NULL)
                    {
                        paiguang -> setVisible(false);
                        paiguang -> stopAllActions();
                    }
                    CCArray* array = (CCArray*)paiNode->getChildren();
                    for(int i = 0; i < array->count(); i++)
                    {
                        CCSprite* sprite = (CCSprite*)array->objectAtIndex(i);
                        if(sprite != NULL)
                        {
                            if(sprite->getTag() == 1 || sprite->getTag() == 2)
                            {
                                sprite -> runAction(CCSpawn::create(CCRotateBy::create(3.0f, 360*6), CCScaleTo::create(3.0f, 0.1f), NULL));
                            }
                            else
                            {
                                sprite->setVisible(false);
                            }
                        }
                    }
                    paiNode -> runAction(CCSequence::create(CCDelayTime::create(3.0f), CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall)), NULL));
                }
                if(pPPLResult->wChairID != m_nMeRealChairID)
                {
                    if(pPPLResult->nTotalScore != 0)
                    {
                        setPlate(pPPLResult->wChairID, pPPLResult->nTotalScore, 12, true);
                        //SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
                    }
                }
                else
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, ePPLCalc);
                    SimpleAudioEngine::sharedEngine()->playEffect("Music/paicalc.mp3");
                    paipaileGameOver(pPPLResult->nTotalScore);
                }
            }
            break;
        }
        case SUB_S_PPL_DEFAULT://现在改为默认开始（新版这个消息不用了）
        {
            if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
                break;
            FirstChargeLayer* pLayer = (FirstChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(29999);
            if(pLayer != NULL)
            {
                pLayer -> removeFromParent();
                WORD* pChairID = (WORD*)pData;
                if(pChairID != NULL)
                {
                    if(*pChairID == m_nMeRealChairID)
                    {
                        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, ePPLDefaultStart);
                        setPaiItemEnable(true);
                    }
                }
            }
            break;
        }
        case SUB_S_PPL_CANCEL://当收到取消消息时，要让圆盘转动完成之后消失
        {
            if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
                break;
            if(wDataSize < sizeof(CMD_S_PPLCancel))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_PPLCancel* pPPLCancel = (CMD_S_PPLCancel*)pData;
            if(pPPLCancel != NULL)
            {
                m_bPPLReceiveCancel[pPPLCancel->wChairID] = true;//设置收到取消消息为true
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, ePPLCancel);
                if(m_bPPLZhuanComplete)
                {
                    m_bPaipaileStatus = false;
                    m_bIsPaipaileStatus = false;
                    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
                    //移除界面
                    CCNode* paiNode = (CCNode*)this->getChildByTag(ePaiNodeTag+pPPLCancel->wChairID);
                    if(paiNode != NULL)
                    {
                        CCSprite* paiguang = (CCSprite*)paiNode->getChildByTag(6);
                        if(paiguang != NULL)
                        {
                            paiguang -> setVisible(false);
                            paiguang -> stopAllActions();
                        }
                        CCArray* array = (CCArray*)paiNode->getChildren();
                        for(int i = 0; i < array->count(); i++)
                        {
                            CCSprite* sprite = (CCSprite*)array->objectAtIndex(i);
                            if(sprite != NULL)
                            {
                                if(sprite->getTag() == 1 || sprite->getTag() == 2)
                                {
                                    sprite -> runAction(CCSpawn::create(CCRotateBy::create(3.0f, 360*6), CCScaleTo::create(3.0f, 0.1f), NULL));
                                }
                                else
                                {
                                    sprite->setVisible(false);
                                }
                            }
                        }
                        paiNode -> runAction(CCSequence::create(CCDelayTime::create(3.0f), CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall)), NULL));
                    }
                }
            }
            break;
        }
        case SUB_S_ONLINE_REWARD_TIME:
        {
            if(wDataSize < sizeof(CMD_S_OnlineRewardTime))
            {
                ERROR_CHECK;
                return false;
            }
            if(CMainLogic::sharedMainLogic()->m_bShowOnlineCoin && CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                CMD_S_OnlineRewardTime* pOnlineRewardTime = (CMD_S_OnlineRewardTime*)pData;
                if(pOnlineRewardTime != NULL)
                {
                    setOnlineRewardTime(pOnlineRewardTime->nSecond);
                }
            }
            break;
        }
        case SUB_S_ONLINE_REWARD_TIME_OVER:
        {
            if(CMainLogic::sharedMainLogic()->m_bShowOnlineCoin && CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                setOnlineTimeOver();
            }
            break;
        } 
	}
	return true;
}

bool GameMainScene::getTryBarrelStatus(int nChairID)
{
    return m_bTryBarrelStatus[nChairID];
}

void GameMainScene::resumeCannon()
{
    changeCannonMultiple(0, m_nCurrentCannonType);
    m_bTryBarrelStatus[m_nMeRealChairID] = false;//试用炮结束
}

bool GameMainScene::OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GAME_SCENE_FREE:
	case GAME_SCENE_PLAY:
		{
			CMD_S_GameScene * pGameScene=(CMD_S_GameScene *)pData;
			m_GameScene.nSceneIndex=pGameScene->nSceneIndex;
			m_GameScene.nMaxBullet=pGameScene->nMaxBullet;
			m_GameScene.lCellScore=pGameScene->lCellScore;
			m_GameScene.bNoFish = pGameScene->bNoFish;
            m_GameScene.nExercise = pGameScene->nExercise;
            m_GameScene.lMaxMultiple = pGameScene->lMaxMultiple;
			m_bWaiting = m_GameScene.bNoFish;
            if(m_GameScene.lCellScore==10)
            {
                m_bXinShouRoom = true;
            }
            
            m_nExchargeTimes = m_GameScene.lMaxMultiple/m_GameScene.lCellScore;
            if(m_GameScene.lMaxMultiple%m_GameScene.lCellScore != 0)
            {
                m_nExchargeTimes+=1;
            }
            if(!m_bInitCompleted)
            {
                setSceneIndex(m_GameScene.nSceneIndex);
            }
			setCellScore(m_GameScene.lCellScore);
			setMaxBullet(m_GameScene.nMaxBullet);
			//正在鱼阵中，请等待...
			m_spWait = CCSprite::create("CatchFish2001/waitWarning.png");
			m_spWait->setPosition(ccp(m_sizeVisible.width/2, m_sizeVisible.height/2));
			this->addChild(m_spWait, 5);
			m_spWait->setVisible(false);
			if (m_bWaiting)
			{
				m_spWait->setVisible(true);
				if (m_nMeRealChairID>=GAME_PLAYER/2)
					m_spWait->setRotation(180.0f);
			}
            
            if(m_GameScene.nExercise != 0)
            {
                if(!(CMainLogic::sharedMainLogic()->KIND_ID == FB_KIND_ID))
                {
                    CCLabelTTF* pLabel = CCLabelTTF::create("体验场每次登陆送10万，仅供体验，退出清零", "Arial", 48);
                    pLabel -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/3*2-40));
                    this -> addChild(pLabel, 100);
                    pLabel -> runAction(CCSequence::create(CCDelayTime::create(1.0f), CCFadeOut::create(1.0f), NULL));
                    if(m_nMeRealChairID>=GAME_PLAYER/2)
                    {
                        pLabel -> setRotation(180);
                        pLabel -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/3+40));
                    }
                    pLabel -> setVisible(false);
                }
                if(!this->isTouchEnabled())
                {
                    this->setTouchEnabled(true);
                }
                
            }
            else
            {
                CMainLogic::sharedMainLogic()->openMatchNotice();
            }
            

			const tagUserInfo * pUserData=m_pClientKernel->GetMeUserInfo();
			if(pUserData != NULL)
			{
				setMeInformation(pUserData->wChairID, pUserData->wChairID);
                if(m_nMeRealChairID == 1 || m_nMeRealChairID == 3)
                {
                    CCArray* array = m_pBeibaoMenu->getChildren();
                    if(array != NULL && array->count() == 1)
                    {
                        CCMenuItem* item = (CCMenuItem*)array->objectAtIndex(0);
                        if(item != NULL)
                        {
                            item -> setPosition(ccp(CLIENT_WIDTH-40, 40));
                        }
                    }
                }
                if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID <GAME_PLAYER)
                {
                    this -> setRotation(180);
                    m_fishWndLayer -> setRotation(180);
                    m_spChatBg -> setRotation(180);
                    m_spChatBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT-180));
                    m_spRemindBg -> setRotation(180);
                    CCMenu* pMenu = (CCMenu*)this -> getChildByTag(501);//显示按钮
                    if(pMenu != NULL)
                    {
                        CCArray* array = pMenu->getChildren();
                        if(array->count() != 0)
                        {
                            CCMenuItemImage* menuItem = (CCMenuItemImage*)array->objectAtIndex(0);
                            if(menuItem != NULL)
                            {
                                pMenu -> setPosition(ccp(-1158, -600));
                                menuItem->setRotation(180);
                            }
                        }
                    }
                    pMenu = (CCMenu*)this -> getChildByTag(502);//显示按钮
                    if(pMenu != NULL)
                    {
                        CCArray* array = pMenu->getChildren();
                        if(array->count() != 0)
                        {
                            CCMenuItemImage* menuItem = (CCMenuItemImage*)array->objectAtIndex(0);
                            if(menuItem != NULL)
                            {
                                pMenu -> setPosition(ccp(-1158, -600));
                                menuItem->setRotation(180);
                            }
                        }
                    }
                    CCLayer* pMenuLayer = (CCLayer*)this -> getChildByTag(503);
                    if(pMenuLayer != NULL)
                    {
                        pMenuLayer -> setRotation(180);
                    }
                    pMenuLayer = (CCLayer*)this -> getChildByTag(504);
                    if(pMenuLayer != NULL)
                    {
                        pMenuLayer -> setRotation(180);
                    }
                }
				for (int i=0;i<GAME_PLAYER;i++)
				{
					m_GameScene.lUserCellScore[i]=pGameScene->lUserCellScore[i];
					m_GameScene.lUserAllScore[i]=pGameScene->lUserAllScore[i];
                    m_GameScene.cbCannonType[i]=pGameScene->cbCannonType[i];
					const tagUserInfo * pUserData=m_pClientKernel->GetUserInfo(i);
					if(pUserData==NULL)continue;
					if (i == m_nMeRealChairID)
					{
                        m_barrelLayer[i]->cannonBg3Blink();
                        CCMenu* pMenu = (CCMenu*)m_barrelLayer[i]->getCannonBg()->getChildByTag(226);
                        if(pMenu != NULL)
                        {
                            pMenu->setVisible(true);
                        }
                        m_barrelLayer[i]->showCannonBg();
						m_nBeiLv[m_nMeRealChairID] = 0;
					}
					setUserScore(i,m_GameScene.lUserAllScore[i]);
					
					if(i != m_nMeRealChairID)
					{
						//更新炮筒
                        int nIndex=0;
                        if (m_GameScene.lUserCellScore[i]<=m_nExchargeTimes/2*m_nCellScore)
                            nIndex=2;
                        else if(m_GameScene.lUserCellScore[i]>m_nExchargeTimes/2*m_nCellScore && m_GameScene.lUserCellScore[i]<=m_nExchargeTimes*m_nCellScore)
                            nIndex=3;
                        m_barrelLayer[i]->setBarrel(nIndex);
                    }
                    else
                    {
                        
                        if(m_GameScene.lUserAllScore[i] != 0)//断线重连
                        {
                            if(!this->isTouchEnabled())
                            {
                                this->setTouchEnabled(true);
                            }
                        }
                    }
                    setBeiLv(i,m_GameScene.lUserCellScore[i]);
				}
				m_pClientKernel->SendSocketData(MDM_GF_GAME,217);
                
                tagUserInfo * pUserInfo = m_pClientKernel->GetMeUserInfo();
                int nScore = 0;
                if (pUserInfo != NULL)
                {
                    nScore = (int)pUserInfo->lScore;
                    this->schedule(schedule_selector(GameMainScene::outGameCount), 1.0f);
                    this -> schedule(schedule_selector(GameMainScene::checkGameStatusOnline), 1.0f);
                    //显示新手引导金币，不做其他任何用途
                    CMainLogic::sharedMainLogic()->m_nShowGuideScore = nScore;
                }
                else
                {
                    ERROR_CHECK;
                }
                
                m_bReceiveScene = true;
                changeScore(true, nScore);
                
                m_bInitCompleted = true;

			}
		}
	}
	return true;
}

bool GameMainScene::OnEventUserLeave(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)
{
	userComeInorLeave(wChairID,false,wChairID);
	m_barrelLayer[wChairID]->setVisible(false);
	m_nEveryBullet[wChairID] = 0;
//    m_spWaitUser->setVisible(true);
    
	return true;
}

//向服务端发消息
bool GameMainScene::changeScore(bool bAdd, int lScore)
{
	CMD_C_BuyBullet BuyBullet;
	BuyBullet.bAdd=bAdd;
	BuyBullet.lScore=lScore;
	m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_BUY_BULLET,&BuyBullet,sizeof(BuyBullet));
	return true;
}

bool GameMainScene::onShoot(float fRotation, DWORD dwBulletID, int nCount, BYTE cbAttribute)
{
	CMD_C_UserShoot UserShoot;
	UserShoot.fAngle=fRotation;
	UserShoot.dwBulletID=dwBulletID;
    UserShoot.dwCount = nCount;
    UserShoot.cbAttribute = cbAttribute;
	m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_USER_SHOOT,&UserShoot,sizeof(UserShoot));
	return true;
}

bool GameMainScene::changeCannonMultiple(BYTE cbAddMultiple, int nCannonType)
{
	CMD_C_CannonMultiple CannonMultiple;
	CannonMultiple.cbAddMultiple=cbAddMultiple;
    CannonMultiple.cbCannonType = nCannonType;
	m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_CANNON_MULTIPLE,&CannonMultiple,sizeof(CannonMultiple));
	return true;
}

bool GameMainScene::sendLockFishID(int FishID,int PlayerId)
{
	CMD_C_LockFishId Lockfish={0};
	Lockfish.lFishID=FishID;
	Lockfish.wChairID=PlayerId;
	m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_LOCKFISH,&Lockfish,sizeof(Lockfish));
	return true;
}

bool GameMainScene::changeFireSpeed(int index)
{
	CMD_C_CoinCount UserShoot;
	UserShoot.lCoinCount=index;
	//	SendSocketData(SUB_C_BUY_BULLETSPEED,&UserShoot,sizeof(UserShoot);
	return true;
}

bool GameMainScene::hitFish(int nFishID, DWORD dwBulletID, int nHitUser, bool bAndroid, int nOtherCount, int nXPos, int nYPos, int nOtherFishID[])
{
    //如果结算框存在，那么就不发送碰撞消息
    if(m_bHaveWndCalc)
        return true;
    if(nHitUser != m_nMeRealChairID)
        return true;
	CMD_C_HitFish HitFish={0};
	HitFish.dwBulletID=dwBulletID;
	HitFish.nFishID=nFishID;
	HitFish.bAndroid=bAndroid;
	HitFish.nHitUser=nHitUser;
	HitFish.nOtherCount=nOtherCount;
	HitFish.nXPos=nXPos;
	HitFish.nYPos=nYPos;
	memcpy(HitFish.nOtherFishID,nOtherFishID,sizeof(int)*nOtherCount);
	WORD wDataSize=sizeof(CMD_C_HitFish)-(MAX_HITFISH-nOtherCount)*sizeof(int);
	m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_HIT_FISH,&HitFish,wDataSize);
	return true;
}

void GameMainScene::setConcludeData()
{
	for(int i = 0; i < m_nFishCount; i++)
	{
		m_nFishDied[i] = 0;
	}
}

void GameMainScene::setFishType()
{
    if(KIND_ID == YQS_KIND_ID)
    {
        for(int i = 0; i < m_nFishCount; i++)
        {
            m_nFishType[i] = LKPYFishID[i] + 1000;
        }
    }
    else if(KIND_ID == NZNH_KIND_ID)
    {
        for(int i = 0; i < m_nFishCount; i++)
        {
            m_nFishType[i] = DYDFishID[i] + 1000;
        }
    }
    else
    {
        for(int i = 0; i < m_nFishCount; i++)
        {
            m_nFishType[i] = JCBYFishID[i] + 1000;
        }
    }
    
}

//退出游戏
void GameMainScene::exitGame()
{
    if(KIND_ID == NZNH_KIND_ID || KIND_ID == YQS_KIND_ID || KIND_ID == JCBY_KIND_ID)
    {
        CMainLogic::sharedMainLogic()->m_lUserScore = m_nUserScore[m_nMeRealChairID];
    }
    if(CMainLogic::sharedMainLogic()->m_bGameChargeStatus)
    {
        m_bShowExitGame = true;
        CMainLogic::sharedMainLogic()->ShowMessage("正在请求服务器...", eMBExitGame);
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage("正在结算", eMBExitGame);
    }
    changeScore(false, 0);
    m_bClickExitMenu = true;
    //停止各种定时器
    this->unschedule(schedule_selector(GameMainScene::createFish));
    //this->unschedule(schedule_selector(GameMainScene::update));
    this->unschedule(schedule_selector(GameMainScene::onFireAndroid));
    //关闭检测分数倒计时
    this -> unschedule(schedule_selector(GameMainScene::checkAutoFire));
    
    //关闭倒计时定时器
    this -> unschedule(schedule_selector(GameMainScene::outGameCount));
}

void GameMainScene::exitMainGame()
{
    //停止心跳包检测
    this -> unschedule(schedule_selector(GameMainScene::checkGameStatusOnline));
    this -> unschedule(schedule_selector(GameMainScene::disConnectGame));
    CMainLogic::sharedMainLogic()->setPlaying(false);
    CMainLogic::sharedMainLogic()->m_bAutoExitGame = true;
    if(CMainLogic::sharedMainLogic()->m_bGameChargeStatus)
    {
        if(CMainLogic::sharedMainLogic()->m_pNetworkService != NULL)
        {
            tagUserInfo * pUserInfo = m_pClientKernel->GetMeUserInfo();
            CMainLogic::sharedMainLogic()->SendStandUpPacket(pUserInfo->wTableID, pUserInfo->wChairID, 1);
        }
    }
    else
    {
        tagUserInfo * pUserInfo = m_pClientKernel->GetMeUserInfo();
        CMainLogic::sharedMainLogic()->SendStandUpPacket(pUserInfo->wTableID, pUserInfo->wChairID, 1);
        
        //@@切换到房间列表，临时不能切换到桌子界面
        CMainLogic::sharedMainLogic()->ShowMessage("正在返回到房间选择界面", eMBExitGame);
        //CMainLogic::sharedMainLogic()->switchLayer(eHallLayer, true);
    }
    //@@关闭音效
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

//加载特效
void GameMainScene::loadMagicPtc()
{
	#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    	Utils::preloadFile("CatchFish01.ptc");
    #endif
	CCSize size=getContentSize();
	int client_wi=(int)size.width;
	int client_he=(int)size.height;
//	m_pMagicDevice=new MP_Device_WRAP(client_wi, client_he);
//	m_pMagicDevice->Create();
	MP_Manager& MP=MP_Manager::GetInstance();
//	MP_Platform* platform=new MP_Platform_COCOS;
//	MP.Initialization(MAGIC_pXnY, platform, MAGIC_INTERPOLATION_ENABLE, MAGIC_CHANGE_EMITTER_DEFAULT, 1024, 1024, 1, 1.f, 0.1f, true);
//	unsigned long lSize = 0;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    const char * pParticle = (const char *)CCFileUtils::sharedFileUtils()->getFileData("CatchFish01/CatchFish01.ptc", "rb", &lSize);
//	MP.LoadEmittersFromFileInMemory(pParticle);
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	MP.LoadAllEmitters();
//#endif
//	
//    
//	MP.RefreshAtlas();
//	MP.RestoreAtlas();
//	MP.CloseFiles();
//	MP.Stop();
//	m_pMagicDevice->SetScene3d();
	for(unsigned int i = 0; i < m_vctParticle.size(); i++)
	{
		HM_EMITTER hmEmitter=MP.getHmEmitterByName(m_vctParticle[i]->sPath.c_str());
		if(hmEmitter)
		{
            
			Magic_CorrectEmitterPosition(hmEmitter, client_wi, client_he);
			m_vctParticle[i]->magicEmitter = CCMagicEmitter::create(hmEmitter);
			m_vctParticle[i]->magicEmitter -> setContentSize(CCSize(client_wi, client_he));
			m_vctParticle[i]->magicEmitter -> isIgnoreAnchorPointForPosition();
			m_vctParticle[i]->magicEmitter -> setAnchorPoint(ccp(0.5,0.5));
            if(m_vctParticle[i]->sPath == "penjinbi")
            {
                this->addChild(m_vctParticle[i]->magicEmitter, 202,1002);
            }
            else if(m_vctParticle[i]->sPath == "07")
            {
                this->addChild(m_vctParticle[i]->magicEmitter, 22);
            }
            else if(m_vctParticle[i]->sPath == "Stars1")
            {
                this->addChild(m_vctParticle[i]->magicEmitter, 22, 19990);
            }
            else if(m_vctParticle[i]->sPath == "Stars2")
            {
                this->addChild(m_vctParticle[i]->magicEmitter, 22, 19991);
            }
            else if(m_vctParticle[i]->sPath == "Stars3")
            {
                this->addChild(m_vctParticle[i]->magicEmitter, 22, 19992);
            }
            else if(m_vctParticle[i]->sPath == "lan")
            {
                this->addChild(m_vctParticle[i]->magicEmitter, 22, 19992);
            }
            else
            {
                this->addChild(m_vctParticle[i]->magicEmitter, 15);
            }
			
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			m_vctParticle[i]->magicEmitter->setVisible(false);
#endif
		}
	}
}

void GameMainScene::playParticle(const char * name, float x, float y, int nLotteryCount)
{
	if (!getParticle())	return;
	for(unsigned int i = 0; i < m_vctParticle.size(); i++)
	{
		if (!strcmp(name, m_vctParticle[i]->sPath.c_str()))
		{
			MP_Manager & MP=MP_Manager::GetInstance();
			MP_Emitter * emitter=NULL;
			emitter=MP.GetEmitterByName(name);
			if (emitter!=NULL)
			{
				HM_EMITTER hmEmitter = emitter->GetEmitter();
				if (hmEmitter)
				{
					MP_POSITION pos;
					emitter->GetPosition(pos);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
					m_vctParticle[i]->magicEmitter->setVisible(true);
#endif
					if (m_nMeRealChairID >= GAME_PLAYER/2)
					{
						pos.x = m_sizeVisible.width - x;
						pos.y = m_sizeVisible.height - y;
						m_vctParticle[i]->magicEmitter->setRotation(180);
					}
					else
					{
						pos.x = x;
						pos.y = y;
					}
					emitter->Move(pos);
					emitter->SetState(MAGIC_STATE_STOP);
					emitter->SetState(MAGIC_STATE_UPDATE);
				}
			}
			break;
		}
	}
}

//完全进入界面后执行的方法，弹出任务层
void GameMainScene::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CMainLogic::sharedMainLogic()->m_bExitGame = false;
#endif
    
    m_bEnterGameScene = true;
    CMainLogic::sharedMainLogic()->m_pMessageBox->m_bShow = false;
    
    //显示技能动画
    char temp[16];
    sprintf(temp, "%d", m_nLastSpeedUp);
    m_labelSpeedUp->setString(temp);
    sprintf(temp, "%d", m_nLastSanshe);
    m_labelSanshe->setString(temp);
    sprintf(temp, "%d", m_nLastBaoji);
    m_labelBaoji->setString(temp);
    if(m_nLastSpeedUp>0)
    {
        speedSp1->setVisible(true);
    }
    if(m_nLastSanshe>0)
    {
        speedSp2->setVisible(true);
    }
    if(m_nLastBaoji>0)
    {
        speedSp3->setVisible(true);
        
    }
    
    if(CMainLogic::sharedMainLogic()->m_bIOSPassed && KIND_ID == NZNH_KIND_ID && CMainLogic::sharedMainLogic()->m_bShowGuide)
    {
        CMainLogic::sharedMainLogic()->m_bShowGuide = false;
        Guide2Layer* pGuide2Layer = Guide2Layer::create(2, m_nMeRealChairID);
        this->addChild(pGuide2Layer, 200, 6000);
        if(m_nMeRealChairID >= GAME_PLAYER/2)
        {
            pGuide2Layer -> setRotation(180);
        }
    }
    if(CMainLogic::sharedMainLogic()->KIND_ID == FB_KIND_ID)
    {
        FuBenLayer* pFuBenLayer = FuBenLayer::create(0);
        this -> addChild(pFuBenLayer, 110);
        if(m_nMeRealChairID>=GAME_PLAYER/2)
        {
            pFuBenLayer->setRotation(180);
        }
    }
    
    if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        setMailNumber(CMainLogic::sharedMainLogic()->m_nMailCount);
        //    //查询弹头数量
        //    CMD_GPR_QueryItemCount QueryItemCount;
        //    QueryItemCount.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
        //    CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_ITEM, SUB_GPR_QUERY_ITEM_COUNT, &QueryItemCount,sizeof(QueryItemCount));
        
        //查询五福数量
        CMD_GPR_WufuCard WufuCard = {0};
        WufuCard.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_WUFU_CARD, &WufuCard,sizeof(WufuCard));
        
        m_pClientKernel->SendSocketData(MDM_GR_USER, SUB_GRR_HAVE_MATCH_PRIZE_REMIND);
        
        //玩家进入房间
        CMD_GPR_DakasaiRoomNotice DakasaiRoomNotice = {0};
        DakasaiRoomNotice.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
        DakasaiRoomNotice.wServerID = CMainLogic::sharedMainLogic()->m_CurrentServer.wServerID;
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_DAKASAI_ROOM_NOTICE, &DakasaiRoomNotice, sizeof(DakasaiRoomNotice));
        
        this -> schedule(schedule_selector(CMainLogic::requestMail),300);
        
        this -> schedule(schedule_selector(GameMainScene::scheduleShowQipao), 10);
        
        //像服务器请求在线奖励时间
        if(CMainLogic::sharedMainLogic()->m_bShowOnlineCoin)
        {
            m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_ONLINE_REWARD_TIME);
        }
    }
}

void GameMainScene::setOnlineRewardTime(int nSecond)
{
    m_nOnlineTime = nSecond;
    if(m_nOnlineTime == 0)//说明当前有奖可以领取
    {
        m_pOnlineMenu -> setVisible(true);
        m_pOnlineLabel -> setString("");
        m_pOnlineLingqu -> setVisible(true);
        m_pOnlineDaojishi -> setVisible(false);
        m_pOnline2Lingqu -> setVisible(true);
        m_pOnlineBg -> runAction(CCRepeatForever::create(CCSequence::create(CCRotateBy::create(0.1f, 20), CCRotateBy::create(0.2f, -40), CCRotateBy::create(0.1f, 20), NULL)));
    }
    else if(m_nOnlineTime < 0)//没有没有了在线奖励
    {
        m_pOnlineMenu->setVisible(false);
    }
    else
    {
        m_pOnlineMenu -> setVisible(true);
        scheduleOnlineReward();
        //开启在线时间奖励倒计时
        this -> schedule(schedule_selector(GameMainScene::scheduleOnlineReward), 1.0f);
    }
}

void GameMainScene::setOnlineTimeOver()//接收服务端消息，倒计时结束，可以领奖
{
    m_pOnlineLabel -> setString("");
    m_pOnlineLingqu -> setVisible(true);
    m_pOnlineDaojishi -> setVisible(false);
    m_pOnline2Lingqu -> setVisible(true);
    this -> unschedule(schedule_selector(GameMainScene::scheduleOnlineReward));
    m_nOnlineTime = 0;
    m_pOnlineBg -> runAction(CCRepeatForever::create(CCSequence::create(CCRotateBy::create(0.1f, 20), CCRotateBy::create(0.2f, -40), CCRotateBy::create(0.1f, 20), NULL)));
    OnlineRewardLayer* pOnlineLayer = (OnlineRewardLayer*)this->getChildByTag(eOnlinelayerTag);
    if(pOnlineLayer != NULL)
    {
        pOnlineLayer->m_pOnlineLabel -> setString("");
        pOnlineLayer->setOnlineStatus(1);
    }
}

void GameMainScene::receiveOnlineReward(int nSecond, int nScore, int nCurrentScore)
{
    //播放动画和设置玩家分数
    setUserScore(m_nMeRealChairID, nCurrentScore);
    GetPriseLayer* pPriseLayer = GetPriseLayer::create(1, nScore, "");
    CCDirector::sharedDirector()->getRunningScene()->addChild(pPriseLayer, 15);
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eOnlineGetCoin);
    if(nSecond < 0)//说明在线奖励已经领取完毕，则隐藏在线奖励框
    {
        m_pOnlineMenu->setVisible(false);
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eOnlineComplete);
    }
    else
    {
        m_pOnlineDaojishi -> setVisible(true);
        m_pOnlineLingqu -> setVisible(false);
        m_pOnline2Lingqu -> setVisible(false);
        m_pOnlineBg -> stopAllActions();
        m_pOnlineBg -> setRotation(0);
        m_nOnlineTime = nSecond;
        scheduleOnlineReward();
        this -> schedule(schedule_selector(GameMainScene::scheduleOnlineReward), 1.0f);
    }
    OnlineRewardLayer* pOnlineLayer = (OnlineRewardLayer*)this->getChildByTag(eOnlinelayerTag);
    if(pOnlineLayer != NULL)
    {
        pOnlineLayer->setOnlineStatus(2);
    }
}

void GameMainScene::scheduleOnlineReward()
{
    char temp[32];
    char temp2[32];
    if(m_nOnlineTime >= 3600)
    {
        sprintf(temp, "%d:%02d:%02d", m_nOnlineTime/3600, m_nOnlineTime%3600/60, m_nOnlineTime%60);
        sprintf(temp2, "距离下一次领奖 %d:%02d:%02d", m_nOnlineTime/3600, m_nOnlineTime%3600/60,m_nOnlineTime%60);
    }
    else if(m_nOnlineTime > 60)
    {
        sprintf(temp, "%02d:%02d", m_nOnlineTime/60, m_nOnlineTime%60);
        sprintf(temp2, "距离下一次领奖 %02d:%02d", m_nOnlineTime/60,m_nOnlineTime%60);
    }
    else
    {
        sprintf(temp, "00:%02d", m_nOnlineTime);
        sprintf(temp2, "距离下一次领奖 00:%02d", m_nOnlineTime);
    }
    m_pOnlineLabel -> setString(temp);
    //判断在线奖励界面是否打开，如果处于打开状态，则更新其状态
    OnlineRewardLayer* pOnlineLayer = (OnlineRewardLayer*)this->getChildByTag(eOnlinelayerTag);
    if(pOnlineLayer != NULL)
    {
        pOnlineLayer->m_pOnlineLabel -> setString(temp2);
    }
    m_nOnlineTime--;
    if(m_nOnlineTime < 0)//停止倒计时，显示点击领取标签
    {
        this -> unschedule(schedule_selector(GameMainScene::scheduleOnlineReward));
    }
}

int GameMainScene::getOnlineTime()
{
    return m_nOnlineTime;
}

void GameMainScene::setMailNumber(int num)
{
    CCArray* array = m_pMailMenu->getChildren();
    for(unsigned int i = 0; i < array->count(); i++)
    {
        CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
        if(num != 0)
        {
            CCSprite* pMailSprite = (CCSprite*)pItem->getChildByTag(10);
            if (pMailSprite != NULL)
            {
                pMailSprite->setVisible(true);
                pMailSprite->stopAllActions();
                CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(GameMainScene::showMailAction2));
                pMailSprite->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 1.2f),CCSpawn::create(func, CCScaleTo::create(0.5f, 0.9f), NULL), NULL)));
            }
        }
        else
        {
            CCSprite* pMailSprite = (CCSprite*)pItem->getChildByTag(10);
            CCSprite* pMailSprite2 = (CCSprite*)pItem->getChildByTag(11);
            if (pMailSprite != NULL && pMailSprite2 != NULL)
            {
                pMailSprite->setVisible(false);
                pMailSprite->stopAllActions();
                pMailSprite2->setVisible(false);
                pMailSprite2->stopAllActions();
            }
        }
    }
}

void GameMainScene::showMailAction2()
{
    CCArray* array = m_pMailMenu->getChildren();
    for(unsigned int i = 0; i < array->count(); i++)
    {
        CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
        CCSprite* pMailSprite2 = (CCSprite*)pItem->getChildByTag(11);
        if (pMailSprite2 != NULL)
        {
            pMailSprite2->setVisible(true);
            pMailSprite2->stopAllActions();
            pMailSprite2->runAction(CCSequence::create(CCFadeTo::create(0, 150), CCScaleTo::create(0.3f, 1.5f), CCFadeTo::create(0.2f, 0), CCScaleTo::create(0, 1.3f), NULL));
        }
    }
}

int GameMainScene::getUserBeiLv()
{
    return m_nBeiLv[m_nMeRealChairID];
}

int GameMainScene::getUserCurrentScore()
{
	return m_nUserScore[m_nMeRealChairID];
}

void GameMainScene::lockFish2()
{
    if(m_bFishingOffSeason) return;
    for(unsigned int i = 0; i < m_vctFishFactory.size(); i++)
    {
        if(m_nSuoDingFishIndex >= (int)m_vctFishFactory.size())
        {
            m_nSuoDingFishIndex = 0;
        }
        GameFish* pGameFish = m_vctFishFactory[m_nSuoDingFishIndex++];
        if(pGameFish == NULL)
            continue;
        if(pGameFish->getDied() || pGameFish -> getAppear())
            continue;
        CCPoint point = pGameFish->m_pFish->getPosition();
        if(point.x>=0&&point.x<=m_sizeVisible.width && point.y>=0 && point.y<=m_sizeVisible.height)
        {
            if(pGameFish -> getMultiple() >= 20 || pGameFish->getPropertyOfFish()&enJumpBomb
               || pGameFish->getPropertyOfFish()&enFreeze || pGameFish->getPropertyOfFish()&enBomb
               || pGameFish->getPropertyOfFish()&enBoss || pGameFish->getPropertyOfFish()&enRangeBomb || pGameFish->getPropertyOfFish()&enLine)
            {
                m_bUserLock = true;
                m_barrelLayer[m_nMeRealChairID]->lockRealFish(pGameFish);
                sendLockFishID(pGameFish->getFishID(),m_nMeRealChairID);
                m_barrelLayer[m_nMeRealChairID]->setLockFishID(pGameFish->getFishID());
                m_nLockFishID[m_nMeRealChairID] = pGameFish->getFishID();
                m_bLockFishID[m_nMeRealChairID] = true;
                haveSuoDingAnimate(true);
                break;
            }
        }
    }
}

void GameMainScene::setNameLength(WORD wChairID ,std::string nameStr)
{
    int lMax = 0;
    CCLabelTTF * nameTTF = CCLabelTTF::create(nameStr.c_str(), "Arial", 20);
    float strLength = nameTTF->getContentSize().width;
    if (strLength - 95>5) {
        if (wChairID == m_nMeRealChairID) {
            lMax = (180<strLength)?180:strLength;
        }
        else
        {
            lMax = (180<strLength)?180:strLength;
        }
    }
    else
    {
        lMax = strLength;
    }
    
    std::string  string1 = nameStr;
    std::string  string2;
    if (strLength>lMax)
    {
        const char * s = nameStr.c_str();
        int i = 0, j = 0;
        while (s[i])
        {
            if ((s[i] & 0xc0) != 0x80)
            {
                j++;
                if (j>1) {
                    std::string  string3 = string1.substr(0,i).c_str();
                    nameTTF ->setString(string3.c_str());
                    strLength = nameTTF->getContentSize().width;
                    
                    if ((strLength)>lMax) {
                        break;
                    }
                    else
                    {
                        string2 = string1.substr(0,i).c_str();
                    }
                }
            }
            i++;
        }
    }
    else
    {
        string2 = nameStr;
    }
    m_barrelLayer[wChairID]->getLabelName()->setString(string2.c_str());
}

void GameMainScene::haveSuoDingAnimate(bool bShow)
{
    CCLayer* pMenuLayer = (CCLayer*)this->getChildByTag(504);
    if(pMenuLayer != NULL)
    {
        CCMenu* pMenu = (CCMenu*)pMenuLayer->getChildByTag(106);
        if(pMenu != NULL)
        {
            CCArray* array = (CCArray*)pMenu->getChildren();
            for(unsigned int i = 0; i < array->count(); i++)
            {
                CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
                if(pItem->getTag() == 310)
                {
                    CCSprite* suoding = (CCSprite*)pItem->getChildByTag(10);
                    if(suoding != NULL)
                    {
                        suoding->setVisible(bShow);
                    }
                }
            }
        }
    }
}

void GameMainScene::resumeBackgroundMusic(float dt)
{
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(CCUserDefault::sharedUserDefault()->getFloatForKey("sound", 1.0f));
}

void GameMainScene::exitGame2(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    exitGame();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void GameMainScene::openReplaceSceneTime()
{
	this -> scheduleOnce(schedule_selector(GameMainScene::backHallScene), 0.1f);
}

void GameMainScene::backHallScene()
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

bool GameMainScene::getAutoFireStatus()
{
    return m_bAutoFire;
}

void GameMainScene::sendQuerySkillStatus()
{
    CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_SKILL, SUB_GRR_QUERY_SKILL);
}

void GameMainScene::resetSkillStauts()
{
    m_nLastBaoji = CMainLogic::sharedMainLogic()->m_nBaojiTimes;
    m_nLastSanshe = CMainLogic::sharedMainLogic()->m_nSansheTimes;
    m_nLastSpeedUp = CMainLogic::sharedMainLogic()->m_nSpeedUpTimes;
    
    //显示技能动画
    char temp[16];
    sprintf(temp, "%d", m_nLastSpeedUp);
    m_labelSpeedUp->setString(temp);
    sprintf(temp, "%d", m_nLastSanshe);
    m_labelSanshe->setString(temp);
    sprintf(temp, "%d", m_nLastBaoji);
    m_labelBaoji->setString(temp);
    if(m_nLastSpeedUp>0 && m_bSpeedUp == false)
    {
        speedSp1->setVisible(true);
    }
    else
    {
        speedSp1 -> setVisible(false);
    }
    if(m_nLastSanshe>0 && m_bIsSansheStatus == false)
    {
        speedSp2->setVisible(true);
    }
    else
    {
        speedSp2 -> setVisible(false);
    }
    if(m_nLastBaoji>0 && m_bIsBaojiStatus == false)
    {
        speedSp3->setVisible(true);
    }
    else
    {
        speedSp3 -> setVisible(false);
    }
}

void GameMainScene::openVIP()
{
    CMainLogic::sharedMainLogic()->ShowMessage("正在打开VIP", eMBNull);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/LeleconfigVersion.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(GameMainScene::onHttpVIPVersionComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
    //隐藏比赛信息
    setMatchShowStatus(false);
}

void GameMainScene::onHttpVIPVersionComplete(CCHttpClient* sender, CCHttpResponse* response)
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
                    if(m_nMeRealChairID>=GAME_PLAYER/2)
                    {
                        pVIPLayer->setRotation(180);
                    }
                    if(m_bAutoFire)
                    {
                        this -> unschedule(schedule_selector(GameMainScene::startBullet));
                    }
                }
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
}

void GameMainScene::sendVIP()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/LeleVipConfigInfo.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(GameMainScene::onHttpVIPComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void GameMainScene::onHttpVIPComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    if(m_nMeRealChairID >= GAME_PLAYER/2)
    {
        pVIPLayer -> setRotation(180);
    }
    if(m_bAutoFire)
    {
        this -> unschedule(schedule_selector(GameMainScene::startBullet));
    }
    
    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
    
}

void GameMainScene::checkGameStatusOnline()
{
    m_nTimesOnline--;
    if(m_nTimesOnline<0)
    {
        //发送检测包
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
        this->scheduleOnce(schedule_selector(GameMainScene::disConnectGame), 3.0f);
        m_nTimesOnline = 10;
    }
}

void GameMainScene::disConnectGame()
{
    this -> unschedule(schedule_selector(GameMainScene::checkGameStatusOnline));
//    CMainLogic::sharedMainLogic()->m_nRoomSocketHandle = CMainLogic::sharedMainLogic()->m_pNetworkService->asynConnect(CMainLogic::sharedMainLogic()->m_sNodeIP.c_str(), CMainLogic::sharedMainLogic()->m_CurrentServer.wServerPort);
    CMainLogic::sharedMainLogic()->ShowMessage("与服务器已经断开连接", eMBOK, eDisConnectNet);
    
    //重新连接房间，发送登录包
//    scheduleGameLink();
//    this -> schedule(schedule_selector(GameMainScene::scheduleGameLink), 4.0f, 4, 4);
//    m_nTimesOnline = 5;
//    CheckNetworkLayer* pNetworkLayer = CheckNetworkLayer::create();
//    this -> addChild(pNetworkLayer, 200, 30001);
//    if(m_nMeRealChairID >= GAME_PLAYER/2)
//    {
//        pNetworkLayer -> setRotation(180);
//    }
//    pNetworkLayer -> openTouchAndTime();
//    
//    //停止倒计时定时器
//    this -> unschedule(schedule_selector(GameMainScene::outGameCount));
}

void GameMainScene::scheduleGameLink()
{
    m_nTimesOnline = 10;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CMainLogic::sharedMainLogic()->m_nRoomSocketHandle = CMainLogic::sharedMainLogic()->m_pNetworkService->asynConnect(CMainLogic::sharedMainLogic()->m_sNodeIP.c_str(), CMainLogic::sharedMainLogic()->m_CurrentServer.wServerPort);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CNetInfo * pNetInfo = NULL;
    if (!NATIVE_TEST)
    {
        pNetInfo = CMainLogic::sharedMainLogic()->m_pNetworkService->Connect(CMainLogic::sharedMainLogic()->m_sNodeIP.c_str(), CMainLogic::sharedMainLogic()->m_CurrentServer.wServerPort, NULL, NULL, true);
    }
    else
    {
        pNetInfo = CMainLogic::sharedMainLogic()->m_pNetworkService->Connect(PLATFORM_IP, CMainLogic::sharedMainLogic()->m_CurrentServer.wServerPort, NULL, NULL, false);
    }
#endif
}

void GameMainScene::sendMobileCharge(int mobileCount)
{
    CMD_C_MobileCharge MobileCharge;
    MobileCharge.wMobileCount = mobileCount;
    m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_MOBILE_CHARGE,&MobileCharge,sizeof(MobileCharge));
}

void GameMainScene::removeSharedUI(CCObject* object)
{
    this->unschedule(schedule_selector(GameMainScene::removeMobileUI));
    playSoundByID(true, 30);
    m_bHaveWnd = false;
    m_bHaveWndCalc = false;
    setTouchEnabled(true);
    setMenuTouch(true);
    setAutoFire();
    sendShareInfo(eShareCancel);
    FirstChargeLayer* pFirstChargeLayer = (FirstChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10300);
    if(pFirstChargeLayer != NULL)
    {
        pFirstChargeLayer->removeFromParent();
    }
    //如果是第一次打中话费，那么有一个话费飞向换话费图标的动画
    if(m_bFirstMobileMoney)
    {
        CCSprite* sprite = CCSprite::create("signIn/huafeiIcon.png");
        sprite -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
        this -> addChild(sprite, 132);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall));
        if(m_nMeRealChairID >= GAME_PLAYER/2)
        {
            sprite -> setRotation(180);
            sprite -> runAction(CCSequence::create(CCMoveTo::create(0.5f, ccp(CLIENT_WIDTH-80, CLIENT_HEIGHT/2+80)),CCFadeOut::create(0.5f), func, NULL));
        }
        else
        {
            sprite -> runAction(CCSequence::create(CCMoveTo::create(0.5f, ccp(80,CLIENT_HEIGHT/2-80)),CCFadeOut::create(0.5f), func, NULL));
        }
    }
}

void GameMainScene::removeMobileUI(float dt)
{
    m_bHaveWnd = false;
    m_bHaveWndCalc = false;
    setTouchEnabled(true);
    setMenuTouch(true);
    setAutoFire();
    if(dt != 10)
    {
        sendShareInfo(eShareCancel);
    }
    FirstChargeLayer* pFirstChargeLayer = (FirstChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10300);
    if(pFirstChargeLayer != NULL)
    {
        pFirstChargeLayer->removeFromParent();
    }
    
    if(m_bFirstMobileMoney)
    {
        CCSprite* sprite = CCSprite::create("signIn/huafeiIcon.png");
        sprite -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
        this -> addChild(sprite, 132);
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall));
        if(m_nMeRealChairID >= GAME_PLAYER/2)
        {
            sprite -> setRotation(180);
            sprite -> runAction(CCSequence::create(CCMoveTo::create(0.5f, ccp(60,170)),CCFadeOut::create(0.5f), func, NULL));
        }
        else
        {
            sprite -> runAction(CCSequence::create(CCMoveTo::create(0.5f, ccp(1220,550)),CCFadeOut::create(0.5f), func, NULL));
        }
    }
}

void GameMainScene::sharedDelay()
{
    m_bClickShared = false;
}

void GameMainScene::sharedMobileMoney(CCObject* object)
{
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
	if(GameMainScene::_instance != NULL)
    {
        if(GameMainScene::_instance -> getFirstMobileMoney())
        {
            GameMainScene::_instance->scheduleOnce(schedule_selector(GameMainScene::showShareSuccess), 0.0f);
        }
        else
        {
            CMainLogic::sharedMainLogic()->sendShareSuccess();
        }
    }    
#else
	this->unschedule(schedule_selector(GameMainScene::removeMobileUI));
    startShare(m_sShareURL, m_sShareContent);
#endif
}

void GameMainScene::startShare(std::string str1, std::string str2)
{
    CMainLogic::sharedMainLogic()->m_nShareInterface = 4;
    //只做微信分享
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(ChargeWebView::checkWX())
    {
        CMainLogic::sharedMainLogic()->m_bSharedState = true;
        WeiXinCharge = 2;
        ChargeWebView::openIOSShare(str1, str2, 1);
		//发送分享开始
    	sendShareInfo(eShareClick);
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
    //发送分享开始
    sendShareInfo(eShareClick);
#endif
}

void GameMainScene::sendShareInfo(int shareAction)
{
    CMD_C_ShareInfo ShareInfo;
    ShareInfo.nShareReason = eShareMobileMoney;
    ShareInfo.nShareAction = shareAction;
    m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_SHARE_INFO, &ShareInfo, sizeof(ShareInfo));
}

//捕获博乐弹
void GameMainScene::setJumpBomb(CMD_S_JumpBomb * pJumpBomb)
{
    if (NULL==pJumpBomb) return;
    float fXStart=(float)(pJumpBomb->ptStart.x);
    float fYStart=(float)(m_sizeVisible.height-pJumpBomb->ptStart.y);
    float fXJump[3],fYJump[3];
    CCPoint ptJump[3];
    for (int i=0; i<3; i++)
    {
        fXJump[i]=pJumpBomb->ptJump[i].x;
        fYJump[i]=pJumpBomb->ptJump[i].y;
        ptJump[i]=ccp(fXJump[i],fYJump[i]);
    }
    GameFish* pJumpFish = createFishByID(1043);
    if(pJumpFish == NULL)  return;
    CCScaleTo* scaleTo0 = CCScaleTo::create(0, 0);
    CCScaleTo* scaleTo1 = CCScaleTo::create(0, 0);
    CCEaseSineOut* sineOut = CCEaseSineOut::create(scaleTo0);
    CCEaseSineIn* sineIn = CCEaseSineIn::create(scaleTo1);
    if (pJumpBomb->nSpriteID == 41)
    {
        pJumpFish->m_pFish->setScale(0.5f);
        scaleTo0 = CCScaleTo::create(0.8f, 2.0f);
        scaleTo1 = CCScaleTo::create(0.8f, 1);
        sineOut = CCEaseSineOut::create(scaleTo0);
        sineIn = CCEaseSineIn::create(scaleTo1);
    }
    else if (pJumpBomb->nSpriteID == 42)
    {
        pJumpFish->m_pFish->setScale(1.0f);
        scaleTo0 = CCScaleTo::create(0.8f, 3.0);
        scaleTo1 = CCScaleTo::create(0.8f, 1.5f);
        sineOut = CCEaseSineOut::create(scaleTo0);
        sineIn = CCEaseSineIn::create(scaleTo1);
    }
    
    pJumpFish->m_pFish->setPosition(ccp(fXStart, fYStart));
    this->addChild(pJumpFish->m_pFish, 2);
    CCSequence* seq0 = CCSequence::create(sineOut, sineIn, NULL);
    CCRepeat* repeat = CCRepeat::create(seq0, 3);
    CCMoveTo* moveTo0 = CCMoveTo::create(1.6f, ptJump[0]);
    CCEaseSineInOut* sineInOut1 = CCEaseSineInOut::create(moveTo0);
    CCMoveTo* moveTo1 = CCMoveTo::create(1.6f, ptJump[1]);
    CCEaseSineInOut* sineInOut2 = CCEaseSineInOut::create(moveTo1);
    CCMoveTo* moveTo2 = CCMoveTo::create(1.6f, ptJump[2]);
    CCEaseSineInOut* sineInOut3 = CCEaseSineInOut::create(moveTo2);
    CCCallFuncN* func0 = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeJumpDieFish));
    CCCallFuncN* func1 = CCCallFuncN::create(this,callfuncN_selector(GameMainScene::removeNodeCall));
    CCSequence* seq1 = CCSequence::create(sineInOut1, func0, sineInOut2, func0, sineInOut3, func0, CCDelayTime::create(0.1f), func1, NULL);
    CCSpawn* spawn = CCSpawn::create(seq1, repeat, NULL);
    pJumpFish->m_pFish->runAction(spawn);
    delete pJumpFish;
    pJumpFish = NULL;
}

void GameMainScene::removeJumpDieFish(CCNode* pNode)
{
    playSoundByID(true, 18);
    CCPoint point = pNode->getPosition();
    shakeScreen();
    int nRange = 0;
    if (pNode->getScale() >= 1.1f)
    {
        playParticle("07", point.x, point.y);
        nRange = 350;
    }
    else if (pNode->getScale() >= 0.4f)
    {
        playParticle("07", point.x, point.y);
        nRange = 200;
    }
    int nOtherCount=0;
    int nOtherFishID[MAX_HITFISH];
    memset(nOtherFishID,0,sizeof(nOtherFishID));
    for (unsigned int i = 0; i < m_vctFishFactory.size(); i++)
    {
        GameFish* pGameFish = m_vctFishFactory[i];
        if(pGameFish == NULL || pGameFish->getDied()) continue;
        float fX = pGameFish->m_pFish->getPositionX() - point.x;
        float fY = pGameFish->m_pFish->getPositionY() - point.y;
        if (fX*fX + fY*fY <= nRange*nRange)
        {
            if (pGameFish->isProperty(enJumpBomb)||pGameFish->isProperty(enBomb)||pGameFish->isProperty(enRangeBomb)||pGameFish->isProperty(enFreeze)) continue;
            if (nOtherCount>=MAX_HITFISH)
            {
                break;
            }
            nOtherFishID[nOtherCount++]=pGameFish->getFishID();
        }
    }
    float tx = point.x;
    float ty = point.y;
    hitFish(nOtherFishID[0], 0, m_nMeRealChairID, false, nOtherCount, (int)tx, (int)ty, nOtherFishID);
}

void GameMainScene::showFirstMobileMoney()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    SimpleAudioEngine::sharedEngine()->playEffect("Music/openbox.mp3");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->playEffect("Music/openbox.ogg");
#endif
	FirstChargeLayer* pFirstChargeLayer = NULL;
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
    pFirstChargeLayer = FirstChargeLayer::create(6, false);
#else
    pFirstChargeLayer = FirstChargeLayer::create(6, m_bFirstMobileMoney);
#endif
    CCDirector::sharedDirector()->getRunningScene()->addChild(pFirstChargeLayer, 19, 10300);
    this -> unschedule(schedule_selector(GameMainScene::startBullet));
    this -> scheduleOnce(schedule_selector(GameMainScene::removeMobileUI), 30.0f);
}

void GameMainScene::showShareSuccess()
{
    char temp[256];
    sprintf(temp, "分享成功，获得%d元话费", GameMainScene::_instance->getMobileMoney());
    CMainLogic::sharedMainLogic()->ShowMessage(temp, eMBOK);
    GameMainScene::_instance->removeMobileUI(10);
}

void GameMainScene::showMatchBegin(int nMatchID, std::string sMatchDate, std::string sMatchTitle)
{
    CCountDownRemind* pCountDownLayer = (CCountDownRemind*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(eCmptRemindTag);
    if(pCountDownLayer != NULL)
    {
        //CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(eCmptRemindTag);
    }
    //初始化本次比赛的日期和场次
    m_nMatchID = nMatchID;
    m_sMatchTitle = CCharCode::GB2312ToUTF8(sMatchTitle.c_str());
    m_sMatchDate = CCharCode::GB2312ToUTF8(sMatchDate.c_str());
    
    //如果玩家在试用炮状态的话，那么提示玩家试用炮积分不算在捕鱼积分之内
    if(m_bTryBarrelStatus[m_nMeRealChairID])
    {
        CMainLogic::sharedMainLogic()->ShowMessage("比赛期间试用炮捕鱼将不累计积分", eMBOK);
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    SimpleAudioEngine::sharedEngine()->playEffect("Music/bisaidaojishi2.mp3");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->playEffect("Music/bisaidaojishi2.ogg");
#endif
    
}

//更新比赛过程中的数据
void GameMainScene::showMatchScoreUpdate(std::vector<CMD_GRO_MatchGroupInfo> vMatchGroupInfo, int nRanking, SCORE nScore, int nSecond, int nMatchNum, std::string sMatchTitle)
{
    if(CCDirector::sharedDirector()->getRunningScene()->getTag() != eGameMainSceneTag)
    {
        return;
    }
    CCmptStatus* pCmptStatusLayer = (CCmptStatus*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(eCmptStatusTag);
    if(pCmptStatusLayer != NULL)
    {
        pCmptStatusLayer->changeInfo(nScore, nRanking, nSecond);
        pCmptStatusLayer->changeRankInfo(vMatchGroupInfo);
        CRankNewLayer* pRankNewLayer = (CRankNewLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10200);
        if (pRankNewLayer != NULL)
        {
            pCmptStatusLayer -> setVisible(false);
        }
    }
    else
    {
        //实时比赛状况
        CCmptStatus* pCmptStatusLayer = CCmptStatus::create(sMatchTitle.c_str(), nScore, nRanking, nSecond, nMatchNum);
        CCDirector::sharedDirector()->getRunningScene()->addChild(pCmptStatusLayer, 22, eCmptStatusTag);
        pCmptStatusLayer->changeInfo(nScore, nRanking, nSecond);
        pCmptStatusLayer->changeRankInfo(vMatchGroupInfo);
        CRankNewLayer* pRankNewLayer = (CRankNewLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10200);
        if (pRankNewLayer != NULL)
        {
            pCmptStatusLayer -> setVisible(false);
        }
    }
}

void GameMainScene::showMatchEnd(BYTE cbShare, int nRanking, SCORE nScore, std::string sMatchPrise, int nMatchID, std::string szMatchDate)
{
    CCountDownRemind* pCountDownLayer = (CCountDownRemind*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(eCmptRemindTag);
    if(pCountDownLayer != NULL)
    {
        //CCDirector::sharedDirector()->getRunningScene() -> removeChildByTag(eCmptRemindTag);
    }
    //关闭积分变化界面
    CCmptStatus* pCmptStatus = (CCmptStatus*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(eCmptStatusTag);
    if(pCmptStatus != NULL)
    {
        pCmptStatus->removeFromParent();
    }
    //打开派奖界面
    CSendPrizeLayer* pSendPrizrLayer = CSendPrizeLayer::create(cbShare, nRanking, nScore, sMatchPrise, nMatchID, szMatchDate);
    this->addChild(pSendPrizrLayer, 998, eCmptSendPrizeTag);
    if(m_nMeRealChairID >= GAME_PLAYER/2)
    {
        pSendPrizrLayer->setRotation(180);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    SimpleAudioEngine::sharedEngine()->playEffect("Music/bisaijieshu.mp3");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->playEffect("Music/bisaijieshu.ogg");
#endif
}

void GameMainScene::showMatchInfoNotify(int nPeople, int nMatchID, int nScore, bool bMatchStatus, std::string sMatchtitle, std::string sMatchPrise)//status=0为未报名，1为已报名
{
    m_spMatchGuide1 -> setVisible(false);
    m_spMatchGuide2 -> setVisible(false);
    m_nMatchID = nMatchID;
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eMatchInfoNotify);
    //报名提示框
    CEnrollRemind* pEnrollRemindLayer = CEnrollRemind::create(sMatchtitle, sMatchPrise, nPeople, nScore, bMatchStatus);
    CCDirector::sharedDirector()->getRunningScene()->addChild(pEnrollRemindLayer, 22, eCmptNotifyTag);
    //判断当前是不是比赛界面，如果是的话，则先隐藏提示框
    CRankNewLayer* pRankNewLayer = (CRankNewLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10200);
    if (pRankNewLayer != NULL)
    {
        pEnrollRemindLayer -> setVisible(false);
    }
    DaojuLayer* pDaojuLayer = (DaojuLayer*)this->getChildByTag(1989);
    if(pDaojuLayer != NULL)
    {
        pEnrollRemindLayer -> setVisible(false);
    }
    ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
    if (pChargeLayer != NULL)
    {
        pEnrollRemindLayer -> setVisible(false);
    }
    LotteryLayer* pLotteryLayer = (LotteryLayer*)this->getChildByTag(19989);
    if(pLotteryLayer != NULL)
    {
        pEnrollRemindLayer -> setVisible(false);
    }
    VIPLayer* pVIPLayer = (VIPLayer*)this->getChildByTag(19995);
    if(pVIPLayer != NULL)
    {
        pEnrollRemindLayer -> setVisible(false);
    }
    FirstChargeLayer* pFirstChargeLayer = (FirstChargeLayer*)::CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10302);
    if(pFirstChargeLayer != NULL)
    {
        pEnrollRemindLayer -> setVisible(false);
    }
    
}

void GameMainScene::showMatchSignResult(bool bMatchResult, bool bMatchStatus, int nMatchID, SCORE llUserScore, std::string sDescription)
{
    CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(sDescription.c_str()).c_str(), eMBOK);
    if(bMatchResult)//报名成功  在游戏内5、3、1分钟提示报名
    {
        CEnrollRemind* pEnrollRemindLayer = (CEnrollRemind*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(eCmptNotifyTag);
        if(pEnrollRemindLayer != NULL)
        {
            //将报名按钮设置为已报名
            if(nMatchID == m_nMatchID)
            {
                pEnrollRemindLayer->setJoined(true);
            }
        }
        m_nUserScore[m_nMeRealChairID] = (int)llUserScore;
        char temp[24];
        sprintf(temp, "%lld", llUserScore);
        m_barrelLayer[m_nMeRealChairID]->getGamelastAtlas()->setString(temp);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        SimpleAudioEngine::sharedEngine()->playEffect("Music/bisaibaoming.mp3");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        SimpleAudioEngine::sharedEngine()->playEffect("Music/bisaibaoming.ogg");
#endif
    }
    else//报名失败
    {
        if(bMatchStatus)//已报名  在游戏内5、3、1分钟提示报名
        {
            CEnrollRemind* pEnrollRemindLayer = (CEnrollRemind*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(eCmptNotifyTag);
            if(pEnrollRemindLayer != NULL)
            {
                //将报名按钮设置为已报名
                pEnrollRemindLayer->setJoined(true);
            }
        }
        else
        {
            
        }
    }
}

//bPriseStatus=0为领取失败  ＝1为领取成功
//cbPriseType为领奖类型（多种类型，参考签到奖励类型）（只有一种）
//nPriseCount为本次获得奖励数量
//llUserScore为玩家当前金币数量（奖励之后的）
//sDescription为描述信息
void GameMainScene::showMatchPriseResult(bool bPriseStatus, BYTE cbPriseType, int nPriseCount, int nMatchID, SCORE llUserScore, std::string sDescription)
{
    if(bPriseStatus)//领取成功
    {
        CRankNewLayer* pRankNewLayer = (CRankNewLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10200);
        if (pRankNewLayer != NULL)
        {
            pRankNewLayer->refreshMatchingInfo(3, nMatchID);//3代表 历史界面
            //CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(sDescription.c_str()).c_str(), eMBOK);
        }
        GetPriseLayer* pPriseLayer = GetPriseLayer::create(cbPriseType, nPriseCount, sDescription);
        CCDirector::sharedDirector()->getRunningScene()->addChild(pPriseLayer, 15);
        //删除发奖界面
        CSendPrizeLayer* pSendPrizeLayer = (CSendPrizeLayer*)this->getChildByTag(eCmptSendPrizeTag);
        if(pSendPrizeLayer != NULL)
        {
            pSendPrizeLayer -> removeFromParent();
        }
        m_nUserScore[m_nMeRealChairID] = (int)llUserScore;
        char temp[24];
        sprintf(temp, "%d", m_nUserScore[m_nMeRealChairID]);
        m_barrelLayer[m_nMeRealChairID]->getGamelastAtlas()->setString(temp);
//        if(cbPriseType == 3)//话费奖励
//        {
//            CCSprite* sprite = CCSprite::create("signIn/huafeiIcon.png");
//            
//            this -> addChild(sprite, 132);
//            sprite -> setScale(1.3f);
//            CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameMainScene::removeNodeCall));
//            if(m_nMeRealChairID >= GAME_PLAYER/2)
//            {
//                sprite -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+40));
//                sprite -> setRotation(180);
//                sprite -> runAction(CCSequence::create(CCDelayTime::create(4.0f),CCSpawn::create(CCMoveTo::create(0.5f, ccp(CLIENT_WIDTH-80,CLIENT_HEIGHT/2+75)), CCScaleTo::create(0.5f, 0.77f), NULL),CCFadeOut::create(0.5f), func, NULL));
//            }
//            else
//            {
//                sprite -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-40));
//                sprite -> runAction(CCSequence::create(CCDelayTime::create(4.0f),CCSpawn::create(CCMoveTo::create(0.5f, ccp(80,CLIENT_HEIGHT/2-75)), CCScaleTo::create(0.5f, 0.77f), NULL),CCFadeOut::create(0.5f), func, NULL));
//            }
//        }
    }
    else//领取失败
    {
        CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(sDescription.c_str()).c_str(), eMBOK);
    }
}

void GameMainScene::showMatchGuide()
{
    m_spMatchGuide1->setVisible(true);
    m_spMatchGuide2->setVisible(true);
}

void GameMainScene::showMatchGuideEnd(bool bJoinSuccess)//true为报名成功，false为报名失败
{
    CMainLogic::sharedMainLogic()->ShowMessage("报名成功，比赛开始时系统将给您倒计时提示，现在请继续捕鱼吧");
}

void GameMainScene::addJiasuPrize(CCNode* pNode, void* nCount)
{
    long nJiasuCount = (long)nCount;
    m_nLastSpeedUp += nJiasuCount;
    char sJiasu[4];
    sprintf(sJiasu, "%d", m_nLastSpeedUp);
    m_labelSpeedUp->setString(sJiasu);
}

void GameMainScene::addSanshePrize(CCNode* pNode, void* nCount)
{
    long nSansheCount = (long)nCount;
    m_nLastSanshe += nSansheCount;
    char sSanshe[4];
    sprintf(sSanshe, "%d", m_nLastSanshe);
    m_labelSanshe->setString(sSanshe);
}

void GameMainScene::addSuodingPrize(CCNode* pNode, void* nCount)
{
    long nSuodingCount = (long)nCount;
    m_nLastBaoji += nSuodingCount;
    char sSuoding[4];
    sprintf(sSuoding, "%d", m_nLastBaoji);
    m_labelBaoji->setString(sSuoding);
}

//打开背包、充值、奖券、兑换话费、换炮界面的时候，要隐藏这两个界面，关闭的时候再显示出来
void GameMainScene::setMatchShowStatus(bool bShow)
{
    //五分钟、三分钟、一分钟提醒界面
    CEnrollRemind* pEnrollRemindLayer = (CEnrollRemind*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(eCmptNotifyTag);
    if(pEnrollRemindLayer != NULL)
    {
        pEnrollRemindLayer->setVisible(bShow);
    }
    //分数更新界面
    CCmptStatus* pCmptStatus = (CCmptStatus*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(eCmptStatusTag);
    if(pCmptStatus != NULL)
    {
        pCmptStatus->setVisible(bShow);
    }
}

void GameMainScene::showLabaUI(int nLabaTimes, int nLastCoin)
{
    FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create(3);
    CCDirector::sharedDirector()->getRunningScene()->addChild(pFirstChargeLayer, 19, 10301);
    pFirstChargeLayer -> setLabaLastTime(nLabaTimes, nLastCoin);
}

void GameMainScene::showFreeCoinUI(int nTime, int nScore, std::string szDescription)
{
    GiveCoinLayer* pGiveCoinLayer = GiveCoinLayer::create(nTime, false, szDescription, nScore);
    this->addChild(pGiveCoinLayer, 200, eGiveCoinLayerTag);
    if(m_nMeRealChairID >= GAME_PLAYER/2 && m_nMeRealChairID <GAME_PLAYER)
    {
        pGiveCoinLayer->setRotation(180);
    }
}

void GameMainScene::updateCurrentScore(int nScore)
{
    setUserScore(m_nMeRealChairID, nScore);
}

void GameMainScene::sendGuideFinish()//新手引导结束
{
    m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_NEW_GUID_GIFT);
}

void GameMainScene::sendQiangRedPacket(int nPacketID)
{
    CMD_C_AskQiang AskQiang = {0};
    AskQiang.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    AskQiang.dwHongBaoID = nPacketID;
    m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_QIANG_HONG_BAO, &AskQiang, sizeof(AskQiang));
    CMainLogic::sharedMainLogic()->ShowMessage("正在抢红包，请稍候", eMBExitGame);
}

//气泡定时器
void GameMainScene::scheduleShowQipao()
{
    //随机选择一条鱼
    int nSize1 = (int)m_vctFishFactory.size();
    if(nSize1 == 0)
        return ;
    int nIndex = rand()%nSize1;
    GameFish* pGameFish = m_vctFishFactory[nIndex];
    if(pGameFish == NULL || pGameFish->getDied() || pGameFish->getAppear())
        return ;
    if(KIND_ID == NZNH_KIND_ID)
    {
        if(pGameFish->getSpriteID() == 1021)//哪吒
        {
            CCSprite* pQipao = CCSprite::create("CatchFish2006/qipao.png");
            pQipao -> setAnchorPoint(ccp(0, 0));
            pQipao -> setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                pQipao->setRotation(180);
            }
            m_fishLayer -> addChild(pQipao, 2, pGameFish->getFishID());
            m_arrQipao -> addObject(pQipao);
            CCSize size(167,68);
            int nDataIndex = rand()%2;
            int nSize = 26;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            nSize = 22;
#endif
            CCLabelTTF* pLabel = CCLabelTTF::create(FishChatData[2+nDataIndex], "Arial", nSize,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            pLabel->setColor(ccc3(255, 255, 255));
            pLabel->setPosition(ccp(88, 68));
            pQipao->addChild(pLabel, 5);
        }
        else if(pGameFish->getSpriteID() == 1026)//龙王
        {
            CCSprite* pQipao = CCSprite::create("CatchFish2006/qipao.png");
            pQipao -> setAnchorPoint(ccp(0, 0));
            pQipao -> setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
            m_fishLayer -> addChild(pQipao, 2, pGameFish->getFishID());
            m_arrQipao -> addObject(pQipao);
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                pQipao->setRotation(180);
            }
            CCSize size(167,68);
            int nDataIndex = rand()%2;
            int nSize = 26;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            nSize = 22;
#endif
            CCLabelTTF* pLabel = CCLabelTTF::create(FishChatData[4+nDataIndex], "Arial", nSize,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            pLabel->setColor(ccc3(255, 255, 255));
            pLabel->setPosition(ccp(88, 68));
            pQipao->addChild(pLabel, 5);
        }
        else if(pGameFish->getSpriteID() == 1042)//五福鱼
        {
            CCSprite* pQipao = CCSprite::create("CatchFish2006/qipao.png");
            pQipao -> setAnchorPoint(ccp(0, 0));
            pQipao -> setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
            m_fishLayer -> addChild(pQipao, 2, pGameFish->getFishID());
            m_arrQipao -> addObject(pQipao);
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                pQipao->setRotation(180);
            }
            CCSize size(167,68);
            int nDataIndex = rand()%2;
            int nSize = 26;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            nSize = 22;
#endif
            CCLabelTTF* pLabel = CCLabelTTF::create(FishChatData[nDataIndex], "Arial", nSize,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            pLabel->setColor(ccc3(255, 255, 255));
            pLabel->setPosition(ccp(88, 68));
            pQipao->addChild(pLabel, 5);
        }
        else if(pGameFish->getSpriteID() != 1018 && pGameFish->getSpriteID() != 1024)//龟丞相、青龙
        {
            CCSprite* pQipao = CCSprite::create("CatchFish2006/qipao.png");
            pQipao -> setAnchorPoint(ccp(0, 0));
            pQipao -> setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
            m_fishLayer -> addChild(pQipao, 2, pGameFish->getFishID());
            m_arrQipao -> addObject(pQipao);
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                pQipao->setRotation(180);
            }
            CCSize size(167,68);
            int nDataIndex = rand()%40;
            int nSize = 26;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            nSize = 22;
#endif
            CCLabelTTF* pLabel = CCLabelTTF::create(FishChatData[8+nDataIndex], "Arial", nSize,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            pLabel->setColor(ccc3(255, 255, 255));
            pLabel->setPosition(ccp(88, 68));
            pQipao->addChild(pLabel, 5);
        }
    }
    else if(KIND_ID == JCBY_KIND_ID)
    {
        if(1)//if(pGameFish->getSpriteID() != 1018 && pGameFish->getSpriteID() != 1024)//龟丞相、青龙
        {
            CCSprite* pQipao = CCSprite::create("CatchFish2006/qipao.png");
            pQipao -> setAnchorPoint(ccp(0, 0));
            pQipao -> setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
            m_fishLayer -> addChild(pQipao, 2, pGameFish->getFishID());
            m_arrQipao -> addObject(pQipao);
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                pQipao->setRotation(180);
            }
            CCSize size(167,68);
            int nDataIndex = rand()%40;
            int nSize = 26;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            nSize = 22;
#endif
            CCLabelTTF* pLabel = CCLabelTTF::create(FishChatData[8+nDataIndex], "Arial", nSize,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            pLabel->setColor(ccc3(255, 255, 255));
            pLabel->setPosition(ccp(88, 68));
            pQipao->addChild(pLabel, 5);
        }
    }
    else if(KIND_ID == YQS_KIND_ID)
    {
        if(pGameFish->getSpriteID() == 1021)//金马
        {
            CCSprite* pQipao = CCSprite::create("CatchFish2006/qipao.png");
            pQipao -> setAnchorPoint(ccp(0, 0));
            pQipao -> setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
            m_fishLayer -> addChild(pQipao, 2, pGameFish->getFishID());
            m_arrQipao -> addObject(pQipao);
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                pQipao->setRotation(180);
            }
            CCSize size(167,68);
            int nDataIndex = rand()%2;
            int nSize = 26;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            nSize = 22;
#endif
            CCLabelTTF* pLabel = CCLabelTTF::create(FishChatData[6+nDataIndex], "Arial", nSize,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            pLabel->setColor(ccc3(255, 255, 255));
            pLabel->setPosition(ccp(88, 68));
            pQipao->addChild(pLabel, 5);
        }
        else if(pGameFish->getSpriteID() == 1037)
        {
            CCSprite* pQipao = CCSprite::create("CatchFish2006/qipao.png");
            pQipao -> setAnchorPoint(ccp(0, 0));
            pQipao -> setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
            m_fishLayer -> addChild(pQipao, 2, pGameFish->getFishID());
            m_arrQipao -> addObject(pQipao);
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                pQipao->setRotation(180);
            }
            CCSize size(167,68);
            int nDataIndex = rand()%2;
            int nSize = 26;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            nSize = 22;
#endif
            CCLabelTTF* pLabel = CCLabelTTF::create(FishChatData[nDataIndex], "Arial", nSize,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            pLabel->setColor(ccc3(255, 255, 255));
            pLabel->setPosition(ccp(88, 68));
            pQipao->addChild(pLabel, 5);
        }
        else if(pGameFish->getSpriteID() != 1020 && pGameFish->getSpriteID() != 1022 && pGameFish->getSpriteID() != 1023)//金龙、凤凰、四不像
        {
            CCSprite* pQipao = CCSprite::create("CatchFish2006/qipao.png");
            pQipao -> setAnchorPoint(ccp(0, 0));
            pQipao -> setPosition(ccp(pGameFish->m_pFish->getContentSize().width/2, pGameFish->m_pFish->getContentSize().height/2));
            m_fishLayer -> addChild(pQipao, 2, pGameFish->getFishID());
            m_arrQipao -> addObject(pQipao);
            if(m_nMeRealChairID >= GAME_PLAYER/2)
            {
                pQipao->setRotation(180);
            }
            CCSize size(167,68);
            int nDataIndex = rand()%40;
            int nSize = 26;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            nSize = 22;
#endif
            CCLabelTTF* pLabel = CCLabelTTF::create(FishChatData[8+nDataIndex], "Arial", nSize,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
            pLabel->setColor(ccc3(255, 255, 255));
            pLabel->setPosition(ccp(88, 68));
            pQipao->addChild(pLabel, 5);
        }
    }
    
    int nCount = m_arrQipao->count();
    if(nCount == 0)
        return ;
    this -> scheduleOnce(schedule_selector(GameMainScene::scheduleHideQipao), 5.0f);
}

void GameMainScene::scheduleHideQipao()
{
    for(int i = 0; i < m_arrQipao->count(); i++)
    {
        CCSprite* pQipao = (CCSprite*)m_arrQipao->objectAtIndex(i);
        if(pQipao == NULL) continue;
        pQipao -> removeFromParent();
    }
    m_arrQipao -> removeAllObjects();
}
