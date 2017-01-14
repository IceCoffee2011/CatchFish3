#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

class GameMainScene : public CBASELAYER, public CClientKernelSink
{
public:
	virtual bool init();  
	static cocos2d::CCScene* scene();
	GameMainScene();
	~GameMainScene();
	CREATE_FUNC(GameMainScene);

	virtual void onEnterTransitionDidFinish();												//进入界面后执行的方法

	static GameMainScene* GetInstance();													//单例
	static GameMainScene* _instance;
	static CClientKernel* sharedClientKernel();												//连接大厅
	static void setCmdLine(LPTSTR lpCmdLine);
	static std::string m_sCmdLine;
	CClientKernel * m_pClientKernel;
	void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);                            //屏幕触摸
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	CC_SYNTHESIZE(bool, m_bEffect, EffectStatus);                                           //是否有音效
	CC_SYNTHESIZE(bool, m_bShadow, FishShadow);                                             //是否显示阴影
	CC_SYNTHESIZE(bool, m_bParticle, Particle);                                             //是否显示特效
	void setMenuTouch(bool bTouch);
	void initBaseThings();                                                                  //波纹背景
	void loadBarrel();											              			    //显示炮台
	void addSpriteBatch();                                                                  //初始化batchNode
	void addChildToBatch(bool bAdd, CCNode* pNode, std::string filePath, bool bLine = false, int nZorder = 0);
	void boolAppearShadow(bool bAppear);                                                    //是否显示阴影
	void shadowCallFunc(GameFish* pGameFish, bool bAppear);                                 //阴影回调函数
	int getOwnTotalMoney();                                                                 //获取剩余金币量
	int getMeRealChairID();                                                                 //获取自己真实ID
	BarrelLayer* getBarrelLayer();								              				//获取炮台
	FishWndLayer* getFishWndLayer();                                                        //获取窗口层
	void itemCallBack(CCObject* obj);							              				//按钮回调
	void readFishXML(const char* filePath);                                                 //读取配置
	GameFish* getGameFishByID(int nID);                                                     //根据ID获取鱼
	GameFish* createFishByID(int nID);                                                      //根据ID创建鱼
	void playSoundByID(bool bEffect, int nID);                                              //根据ID播放音效
	void outGameCount(float ft);                                                            //未发射子弹起n秒后弹出退出提醒
    void onFireAndroid(float ft);                           //机器人锁定
	void changeSceneScehdule(float ft);							              				//切换背景图时回调
	void fishOpacityCall(CCNode* pNode);                                                    //切换场景时鱼渐变消失
	void noticeRemoveCall(CCNode* pNode);						              				//移除公告
	void update(float dt);																	//子弹移动，检测碰撞
	void startBullet(float dt);																//发射子弹
    void start2Bullet();//发射子弹
	void bulletStatus();                                                                    //子弹发射状态
	unsigned int getNewBulletID();                                                          //获取子弹标识
	void autoFire();																		//自动开炮
    void cancelAutoFire();//取消自动开炮
    void checkAutoFire(); //检测玩家分数，当不足时，弹出充值界面
	void createFishNet(CCPoint point,float rotation,int type, int nMultiple, WORD wChairID, BYTE cbAttribute, int nIndex = 0);								//绘制渔网(坐标、旋转角度、类型）
    CCAnimate* fishNetAnimate(int nNum, int nType);
	CCAnimate* fishNetAnimate(int type);															//渔网动画
	void removeNodeCall(CCNode* pNode);														//移除子节点
    void removeBulletCall(CCNode* pNode, void* nType);	
	void readFishPathXML(const char* filePath);                                             //读取路线XML
	tagPathIndex* getFishPathIndex(int id);                                                 //获取路线
	void createFish(float dt);                                                  			//添加延迟鱼
	void addFish(tagFishPack* pFishPack);                                                   //服务端产生鱼
	void addStorgeFish(tagFishPack* pFishPack);                                             //延迟鱼
	void lockFish();                                                                        //锁定鱼
	void drawLockLine(int nChairID, GameFish* pGameFish);									//绘制锁定线
	void removeLockLine(int nID);                                                           //移除锁定线
	float getBarrelRotation(CCPoint ptStart, CCPoint ptEnd);                    			//设置炮筒的方向为此鱼的游动方向
	void createMoney(int wChairID, CCPoint fishPoint, int fishMult,float rotation, int nFishScore, bool bTryCannon = false);	        //产生金币
	void createMoney2(int wChairID, CCPoint fishPoint, int fishMult,float rotation);			//产生金币
    void createMoney3(int wChairID, CCPoint fishPoint, int fishMult,float rotation, int nFishScore);//拍拍乐金币
	void addMoney2(int wChairID, GoldSprite* sprite, CCPoint endPoint);
	void hideGold(CCNode* node);															//隐藏金币银币
	void createMoneyCall(CCNode* pNode, void* v);                                           //创建鱼死金币
	void shakeScreen();                                                                     //震屏
	void freezedScreen(float dt);															//定屏
	void setHitFish(int bulletID,int bulletRealID,bool bAndroid, GameFish* fish);                          //设置捕获鱼
	void userComeInorLeave(int PlayerID,bool ComeOrLeave,int RealUserid);                                                                       //玩家进入或者出去
	void setMeInformation(int nPlayerID, int nMeRealID);                                                                                        //自己信息
	void setCellScore(int nCellScore);                                                                                                          //单元分
    int getCellScore();
	void setMaxBullet(int nMaxBullet);                                                                                                          //设置最大子弹数量
	void setBeiLv(WORD wChairID, int nScore);                                                                                                   //设置玩家倍率
	void setUserScore(WORD wChairID, int nScore);                                                                                               //设置玩家分数
	void setSceneIndex(int nIndex);                                                                                                             //设置背景
	void changeScene();
    void changeFishScene(bool bYuchao);                                                                         //true为鱼潮来临false为宝箱来袭
	void setSuperPao(WORD wChairID);                                                                                                            //设置超级炮玩家
	void cancelSuperPao(WORD wChairID);                                                                                                         //取消超级炮玩家
	void userShoot(WORD wChairID, float fRotation, int RealChairID, bool bAndroid, DWORD dwBulletID, int nCount, BYTE cbAttribute);//发射子弹
    void userShoot2(WORD wChairID, float fRotation1, int RealChairID, bool bAndroid, DWORD dwBulletID,int nCount, BYTE cbAttribute);
	void lockFishID(WORD wChairID, int nFishID); //锁定鱼
	void setCapture(WORD wChairID, CMD_S_CaptureFish * pCaptureFish);//捕获鱼
	void showScoreInfo(WORD wChairID, CMD_S_CaptureFish * pCaptureFish);//显示堆叠金币、得分圆盘、打鱼分数
	void setPlate(WORD wChairID, int nScore, int nFishID, bool bShowName = false);
	void userAddMoney(WORD wChairID, int nFishID, int nFishScore, int nFishMultiple, int nLotteryCount, bool bPlayParticle, int nTryScore, int nShowTryCannon);//加钱
    void setBossScore(int nScore); //设置BOSS分数           
	void setConcludeData();//结算
	CCArray* getMenuArray();
	void setFishType();//设置鱼死亡的ID
	void setAutoFire();
	void exitGame();
    void exitMainGame();
	void loadMagicPtc();
	void playParticle(const char * name, float x, float y, int nLotteryCount = 0);
	void removeGold(CCNode* node, void * nGoldType);
	void androidLock(float dt);						//机器人锁定

	void setWndCalcHave(bool have);
	bool getWndCalcHave();
	void setWndHave(bool have);						//设置窗口显示或者消失
	bool getWndHave();
	
	void checkCollision(float ft);
	void speedUpFire();					//加速射击按钮
    void cancelSpeedUpFire();           //取消加速射击
	CCSprite* createSprite(std::string sName, int num);
	void moneyAnimation(CCNode* node, void* bTryCannon);								//积分版上的钱币精灵执行动画以及特效
	//ClientKernel
	virtual void ResetGameFrame() {}
	virtual void CloseGameFrame() {}
	virtual bool OnEventSocket(const TCP_Command & Command, const void * pBuffer, WORD wDataSize);
	virtual bool OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize);
	virtual void OnEventUserMemberOrder(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) {}
	virtual void OnEventUserStatus(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) {return;}
	virtual bool OnEventUserLeave(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser);
	virtual bool OnEventUserEnter(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser);
	virtual void OnEventUserScore(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) {}
	virtual bool onShoot(float fRotation, DWORD dwBulletID, int nCount, BYTE cbAttribute);    //发送子弹消息
	virtual bool hitFish(int nFishID, DWORD dwBulletID, int nHitUser, bool bAndroid, int nOtherCount, int nXPos, int nYPos, int nOtherFishID[]);//击中鱼儿
	virtual bool changeScore(bool bAdd, int lScore);//上下分
	virtual bool changeCannonMultiple(BYTE cbAddMultiple, int nCannonType);//切换炮倍率
	virtual bool changeFireSpeed(int index);//切换炮的速度
	virtual bool sendLockFishID(int FishID,int PlayerID);//发送锁定鱼ID

	CCMenuItemSprite* createMenuItem(std::string sName, int num);
	int getUserBeiLv();		//获取玩家倍率
	int getUserCurrentScore();	//获取玩家的当前分数
	CCSequence* fishDieRandAct(float fDuration);
    CCSequence* fishDieRandAct2(float fDuration);
    
    void showMeScore(CCNode* pNode, void* fishScore);
    
    void lockFish2();           //锁定
    bool getInitCompleted();
    
    void setNameLength(WORD wChairID ,std::string nameStr);
    
    //按钮动作
    void menuTouchFalse(CCNode* node);
    void menuTouchFalse2(CCNode* node);
    void menuAction(CCNode* node);
    void menuAction2(CCNode* node);
    void menuTouchTrue();
    void menuTouchFalse();
    void menuItemTouchTrue();
    void menuItemTouchFalse();
    void showMenu();
    
    void loadUIGiveBg();
    
    void haveSuoDingAnimate(bool bShow);
    void resumeBackgroundMusic(float dt);//恢复背景音乐的播放
    
    //金蟾副本打中珍珠展示界面
    void exitGame2(CCObject* object);
    
    //技能效果
    void cancelBaojiStatus(CCNode* node);
    void cancelSansheStatus(CCNode* node);
    void cancelSpeedupStatus(CCNode* node);
    //使用技能
    void setSkillStatus(int nSkillID, int nCount);
    //系统赠送技能
    void showSkillStatus(int nSkillID, int nCount);
    void hideLabelAnimate(CCNode* node);
    void showZengSkillAnimate(int nSkillID);//0为加速，1为散射，2为锁定
    
    void onHttpLotteryComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    int getBulletType(int nMult);
    
    void openVIP();
    void onHttpVIPVersionComplete(CCHttpClient* sender, CCHttpResponse* response);
    void sendVIP();
    void onHttpVIPComplete(CCHttpClient* sender, CCHttpResponse* response);
    

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	void openReplaceSceneTime();
	void backHallScene();
#endif
    
    bool m_bEnterGameScene;
    CMD_S_GameScene m_GameScene;                                              //游戏状态
    
    
    
    void resumeCannon();
    
    void resetSkillStauts();
    void sendQuerySkillStatus();
    
    bool getAutoFireStatus();
    bool getTryBarrelStatus(int nChairID);
    
    //闪电炮动画
    CCSequence* shandianAction(int nIndex);
    
    void disConnectGame();
    void checkGameStatusOnline();
    void scheduleGameLink();
    
    //心形动画
    void setMailNumber(int num);
    void showMailAction2();
    
    //显示金币、奖券、技能的发奖效果
    void addCoinPrize(CCNode* pNode, void* nCount);
    void addJiasuPrize(CCNode* pNode, void* nCount);
    void addSanshePrize(CCNode* pNode, void* nCount);
    void addSuodingPrize(CCNode* pNode, void* nCount);
    
    //新手引导结束
    void sendGuideFinish();
    
    //在线时间奖励倒计时
    void setOnlineRewardTime(int nSecond);
    void scheduleOnlineReward();
    int getOnlineTime();
    void setOnlineTimeOver();
    void receiveOnlineReward(int nSecond, int nScore, int nCurrentScore);
    
    //话费鱼
    void sendMobileCharge(int mobileCount);
    void removeSharedUI(CCObject* object);
    void removeMobileUI(float dt);
    void sendShareInfo(int shareAction);
    void sharedMobileMoney(CCObject* object);
    void startShare(std::string str1, std::string str2);
    void sharedDelay();
    void showMobileMenu();
    
    void removeJumpDieFish(CCNode* pNode);
    void setJumpBomb(CMD_S_JumpBomb * pJumpBomb);
    
    void showShareSuccess();
    
    void showDantouInfo();
    void getDantouType(int nType);
    void playDantouSound(CCNode* pNode, void* nCount);
    
    int m_nExchargeTimes;//炮筒切换次数
    
    CC_SYNTHESIZE(bool, m_bCanRequestRank, CanRequestRank);
    CC_SYNTHESIZE(bool, m_bSpeedUp, SpeedUp);
    CC_SYNTHESIZE(bool, m_bIsBaojiStatus, IsBaojiStatus);
    CC_SYNTHESIZE(bool, m_bIsSansheStatus, IsSansheStatus);
    CC_SYNTHESIZE(int, m_nCurrentCannonIndex, CurrentCannonIndex);
    CC_SYNTHESIZE(bool, m_bOnline, Online);
    CC_SYNTHESIZE(bool, m_bShowExitGame, ShowExitGame);
    
    CC_SYNTHESIZE(int, m_wMobileMoney, MobileMoney);
    CC_SYNTHESIZE(bool,m_bFirstMobileMoney, FirstMobileMoney);
    CC_SYNTHESIZE(int, m_nMatchID, MatchID);
    CC_SYNTHESIZE(std::string, m_sMatchDate, MatchDate);
    CC_SYNTHESIZE(std::string, m_sMatchTitle, MatchTitle);
    CC_SYNTHESIZE(bool, m_bClickDantouMenu, ClickDantouMenu);
    CC_SYNTHESIZE(int, m_nSelectDantouType, SelectDantouType);
    CC_SYNTHESIZE(bool , m_bDantouUsing, DantouUsing);
    CC_SYNTHESIZE(bool, m_bHaveNoScore, HaveNoScore);
    CC_SYNTHESIZE(bool, m_bSelectDantouNum, SelectDantouNum);
    CC_SYNTHESIZE(int, m_nShowMenuTime, ShowMenuTime);
    CC_SYNTHESIZE(bool, m_bAutoOneCharge, AutoOneCharge);
    
    void setDantouNum(int nTongDantou, int nYinDantou, int nJinDantou);
    void setWufuNum(int nCard[5]);
    
    void showFirstMobileMoney();
    
    void showBossScore(CCNode* node, void* nScore);
    CCMenu* getBisaiMenu();
    
    void showMatchGuide();
    void showMatchGuideEnd(bool bJoinSuccess);
    
    void showMatchBegin(int nMatchID, std::string sMatchDate, std::string sMatchTitle);
    void showMatchEnd(BYTE cbShare, int nRanking, SCORE nScore, std::string sMatchPrise, int nMatchID, std::string szMatchDate);
    void showMatchInfoNotify(int nPeople, int nMatchID, int nScore, bool bMatchStatus, std::string sMatchtitle, std::string sMatchPrise);
    void showMatchSignResult(bool bMatchResult, bool bMatchStatus, int nMatchID, SCORE llUserScore, std::string sDescription);
    void showMatchPriseResult(bool bPriseStatus, BYTE cbPriseType, int nPriseCount, int nMatchID, SCORE llUserScore, std::string sDescription);
    void showMatchScoreUpdate(std::vector<CMD_GRO_MatchGroupInfo> vMatchGroupInfo, int nRanking, SCORE nScore, int nSecond, int nMatchNum, std::string sMatchTitle);
    
    void setMatchShowStatus(bool bShow);
    
    //显示喇叭界面
    void showLabaUI(int nLabaTimes, int nLastCoin);
    
    void showFreeCoinUI(int nTime, int nScore, std::string szDescription);
    void updateCurrentScore(int nScore);
    
    void showPaiRemindBg();
    void startPaiZhuanUI(int nChairID);
    void showPaiPaiLeUI2(CCNode* node, void* chairID);
    void showPaiPaiLeUI3(CCNode* node, void* chairID);
    void showPaiPaiLeUI();
    void setPaiItemEnable(bool bEnable);
    void showPaiDaojishi();
    void playPaiDaojishi();
    void paipaileGameOver(int nScore);
    void showBarrelPlate(CCNode* pNode, void* nScore);
    
    void sendQiangRedPacket(int nPacketID);
    
    //气泡定时器
    void scheduleShowQipao();
    void scheduleHideQipao();
    
    
private:
    int m_nTimesOnline;
	CCLayer* m_fishLayer;
	MP_Device_WRAP* m_pMagicDevice;											  //粒子引擎
	CCSize m_sizeVisible;                                                     //屏幕尺寸
	FishWndLayer* m_fishWndLayer;                                             //购买鱼币、新手帮助等窗口层
	BarrelLayer* m_barrelLayer[GAME_PLAYER];                                            //各炮台层
	CCSprite* m_spBkgrnd;                                                     //初始（正常显示）背景
	CCSprite* m_spBkgrnd2;                                                    //需要切换到的场景背景
	CCSprite* m_spWait;                                                       //鱼阵进行中提醒
	CCSprite* m_spReCount;                                                    //倒计时图片
	int m_nMeRealChairID;                                                     //自己座位真实号
	int m_nBgMusicIndex;                                                      //背景音乐序列号
	int m_nPreSceneIndex;                                                     //背景ID
	int m_nSceneIndex;                                                        //背景序列
	int m_nCellScore;                                                         //单元分
	int m_nMaxBullet;														  //最大子弹数
	int m_nFreezeTime;														  //定屏时间
	int m_nReCount;                                                           //倒计时计数
	int m_nOutTime;                                                           //不发子弹起n秒后退出游戏
	CCLabelAtlas* m_labelTTFReCount;                                            //倒计时TTF
	CCArmature* m_armatureWave;                                               //切换场景时波浪动画
	std::vector<GameFish*>m_vctGameFish;                                      //鱼种类的容器
	std::vector<GameFish*>m_vctFishFactory;                                   //存储所有存在的鱼
	std::vector<tagFishPack*> m_vctFishStorg;								  //盛放那些有延迟时间没有出来的鱼
	std::vector<tagPathIndex*> m_fishPathIndex;                              //路线
	std::vector<tagSoundCfg *>	m_vctSound;                                   //音效配置
	std::vector<tagParticleCfg *>	m_vctParticle;                            //特效容器
	std::vector<BossBeilv*> m_labelBossBeilv;								//Boss倍率容器
    
	tagFishPack* m_pFishPack;												  //鱼的路径
	unsigned int m_dwBulletID;                			                      //子弹标识
	CCPoint m_ptStart;			                                              //触摸起点
	CCPoint m_ptEnd;                                                          //触摸终点
	OBB* m_Obb1;                                                              //碰撞检测成员
	OBB* m_Obb2;
	int m_nEveryBullet[GAME_PLAYER];										  //每个人当前子弹数量
	CCArray* m_arrBullet;													  //子弹数组
	CCArray* m_arrMoney;													  //金币数组
	//bool m_bSpeedUp;                                                          //是否加速
    bool m_bClickSpeedUp;//点击了加速按钮
	bool m_bCanShoot;                                                         //是否能发射子弹
	bool m_bUserLock;														  //本玩家是否在锁定中
	bool m_bAutoFire;//自动开炮
    bool m_bClickAutoFire;
	bool m_bFishingOffSeason;                                                 //休渔期
	bool m_bWaiting;                                                          //渔阵，是否在等待
	bool m_bFreeze;															  //定屏
	int m_nSuoDingFishIndex;
	tagLockLine	m_LockLine[GAME_PLAYER];                                      //锁定线
	bool m_bLockFishID[GAME_PLAYER];										  //是否锁定玩家ID
	bool m_bHaveUser[GAME_PLAYER];                                            //玩家数组
	bool m_bAndroid[GAME_PLAYER];                                             //是否是机器人
	bool m_bUserSuperPao[GAME_PLAYER];                                        //是否超级泡
	float m_fRotation[GAME_PLAYER];											  //炮筒旋转角度
	int m_nOwnTotalMoney[GAME_PLAYER];								     	  //玩家拥有金币数量
	int m_nLockFishID[GAME_PLAYER];                                           //锁定鱼ID
	int m_nRealChairID[GAME_PLAYER];                                          //真实玩家椅子号
	int m_nUserScore[GAME_PLAYER];                                            //上分数据
	int m_nBeiLv[GAME_PLAYER];                                                //玩家倍率
	
	static bool m_bHaveWndCalc;
	CCArray* m_arrMenu;
	static bool m_bWndHave;															//窗口是否存在

	bool m_bTouch;																//true时为按下屏幕状态，在end方法中设置为false
    bool m_bInitCompleted;                                                          //主场景是否初始化完成
    bool m_bReceiveScene;
	int m_nCurrentBulletCount;
    
    int m_nFishDied[FishCountSub];											      //鱼死亡的个数
    int m_nFishType[FishCountSub];												  //鱼的类型
    int m_nFishCount;//死鱼id个数
    WORD KIND_ID;
    bool m_bHaveWnd;                                                             //是否正在显示打中话费鱼的界面
    CCNode* m_nodeScore;
    CCSprite* m_pSpBlue;
    
    CCProgressTimer *baojiCD;
    CCSprite* m_spBaoji;
    bool m_bClickBaoji;
    CCProgressTimer* sansheCD;
    CCSprite* m_spSanche;
    bool m_bClickSanshe;
    CCProgressTimer* speedupCD;
    CCSprite* m_spSpeedUp;
    CCLabelAtlas* m_labelSpeedUp;
    CCLabelAtlas* m_labelBaoji;
    CCLabelAtlas* m_labelSanshe;
    CCSprite* speedSp1;
    CCSprite* speedSp2;
    CCSprite* speedSp3;
    int m_nLastSpeedUp;
    int m_nLastSanshe;
    int m_nLastBaoji;
	bool m_bMenuOpened;
    bool m_bTryBarrelStatus[GAME_PLAYER];
    bool m_bHaveTryCannon;//是否已经有过试用炮
    long long m_lUserScore;
    CCSprite* m_speedupAnimate;
    CCSprite* m_baojiAnimate;
    int m_nCurrentCannonType;//0-12
    int m_nTryLastScore;//试用总分
    bool m_bFirstEnter;
    bool m_bXinShouRoom;
    
    CCMenu* m_pBisaiMenu;
    CCMenu* m_pBeibaoMenu;
    CCMenu* m_pMenuMobile;
    CCMenu* m_pMenuDantou;
    CCMenu* m_pMailMenu;
    
    bool m_bClickExitMenu;
    bool m_bClickShared;
    bool m_bClickMenu;
    int m_nChangeSceneIndex;
    int m_nHistoryLockFishID;
    
    CCArray* m_arrLockFish;
    CCArray* m_arrQipao;
    
    CCSprite* m_spMatchGuide1;
    CCSprite* m_spMatchGuide2;
    
    std::vector<CCLayer*> m_vctBoxPrize;   //打中宝箱中奖容器
    int m_nShijinbiFishID[GAME_PLAYER][3];
    std::string m_sShareURL;
    std::string m_sShareContent;
    CCPoint m_pointDantou;
    CCSprite* m_dtParticle;
    CCSprite* m_spDantouDi;
    CCLabelAtlas* m_labelDantou;
    int m_nDantouTotalNum;
    bool m_bBaojiStatus[GAME_PLAYER];//是否处于爆机状态
    bool m_bSameDie[GAME_PLAYER];
    CCLabelTTF* m_luckLabel;
    CCLabelTTF* m_luckLabel2;
    
    //聊天输入框
    CCSprite* m_spChatBg;
    CCEditBox* m_chatBox;
    //拍拍乐
    CCSprite* m_spRemindBg;
    CCLabelAtlas* m_labelRemind;
    int m_nRemindNum;
    int m_nPPLChairID;//拍拍乐椅子号
    bool m_bPPLChairID[GAME_PLAYER];
    int m_nPaipaileTime;//拍拍乐拍的次数
    bool m_bPaipaileStatus;//为true不能改变倍率
    bool m_bIsPaipaileStatus;//为true不能开炮
    bool m_bPPLZhuanComplete;
    bool m_bPPLReceiveCancel[GAME_PLAYER];
    
    CCLabelTTF* m_pOnlineLabel;//在线时间奖励倒计时
    CCSprite* m_pOnlineLingqu;//立即领取
    CCSprite* m_pOnline2Lingqu;
    CCSprite* m_pOnlineDaojishi;
    CCSprite* m_pOnlineBg;
    int m_nOnlineTime;
    CCMenu* m_pOnlineMenu;
    
};

#endif // __GAMELAYER_H__
