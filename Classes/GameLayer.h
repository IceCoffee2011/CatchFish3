#ifndef __GAMELAYER__
#define __GAMELAYER__

#define SHOW_NUM           3 //数字个数
#define ROLENUM             10 //英雄个数
#define PRIZENUM            4 //prize转动图片英雄个数

struct tagNoticeInfo
{
    CCLabelTTF* labelNotice;
    float fWidth;
};

struct tagRankInfo
{
    std::string sNickName;
    std::string sScore;
};

class GameLayer : public CBASELAYER, public CClientKernelSink, public CCScrollViewDelegate
{
public:
	virtual bool init();  
	static cocos2d::CCScene* scene();
	CREATE_FUNC(GameLayer);
    static GameLayer* _instance;
    static GameLayer* GetInstance();
    
    CCAnimate* getAnimate(int nCount, std::string sName, float interval);
    
    void loadUINode1();
    void loadUINode2();

	~GameLayer();
	GameLayer();


	static CClientKernel * sharedClientKernel();
	static void setCmdLine(LPTSTR lpCmdLine);

public:
	virtual void ResetGameFrame();
	virtual void CloseGameFrame();
	virtual bool OnEventSocket(const TCP_Command & Command, const void * pBuffer, WORD wDataSize);
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	virtual bool OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize);
	virtual void OnEventUserMemberOrder(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser);
	virtual void OnEventUserStatus(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser);
	virtual bool OnEventUserLeave(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser);
	virtual bool OnEventUserEnter(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser);
	virtual void OnEventUserScore(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser);
	bool OnSubReadyGame(const void * pBuffer, WORD wDataSize);
	bool OnSubStartGame(const void * pBuffer, WORD wDataSize);
	bool OnSubReceiveResult(const void * pBuffer, WORD wDataSize);
    bool OnSubReceiveItemResult(const void * pBuffer, WORD wDataSize);
	bool OnSubPicRoll(const void * pBuffer, WORD wDataSize);
	bool OnSubChangeBet(const void * pBuffer, WORD wDataSize);
	bool OnSubStopScroll(const void * pBuffer, WORD wDataSize);
	bool OnSubClownBet(const void * pBuffer, WORD wDataSize);
	bool OnSubChangePrize(const void * pBuffer, WORD wDataSize);
	bool OnSubGetPrize(const void * pBuffer, WORD wDataSize);
    bool OnUserCantPlay(const void * pBuffer, WORD wDataSize);
    
    void exitGame();
    
    void gameCallBack(CCObject* object);
    void clickMenu(CCObject* object);
    void setMenuItemVisible(int nMenuTag, bool bShow, CCMenu* menu);
    void setMenuTouchEnable(bool bEnable, CCMenu* menu);
    void setMenuItemTouchEnable(int nItemTag, bool bEnable, CCMenu* menu);
    void playFireAnimate();
    void callFuncNForDownStart();
    void playGameWinAnimate();
    void delayToPlayStarSound();
    void setItemNum(int nItemNum);
    void setGoType();
    
    /*转动*/
    void sendPlusGameBet();
    void sendMinusGameBet();
    void sendGameRoll();
    void TimerToStopRoll(float dt);
    void TimerToEnterToPrizeLayer(float dt);
    void startGameRoll();
    void stopGameRoll();
    void delayToShowRollEnd(CCNode* node);
    //猜大小界面
    void sendChangePrizeTimes(int nTimes);
    void sendCancelChangePrizeTimes(int nTimes);
    void beganPrizeRoll();
    void TimerToAutoCalculate(float dt);
    void sendAutoCalculate();
    void TimerToRollLayer(float dt);
    void setGuessItemTouchEnable(bool bTouch);
    void playGetPrizeAnimate();
    void delayRemoveShouJinAnimate(CCNode* node);
    void playGetPrizeExtra();
    CCSequence* getSeqAnimate();
    void playPrizeBetChangeAnimate(LONGLONG lChangedPrizeBet);
    void runToTargetBet(float dt);
    void removeOutSide(CCNode* node);
    void delayToPlayChangePrizeSound();
    
    
    void dealGuessResult();
    void startPrizeRoll();
    void stopPrizeRoll();
    void setGuessShow();
    
    //心跳包定时器
    void checkGameStatusOnline();
    void disConnectGame();
    
    //排行
    void createRankInfo();
    void onHttpRankComplete(CCHttpClient* sender, CCHttpResponse* response);
    //改变巨奖显示数量
    void changeBigPrize(LONGLONG lBigPrize);
    //公告
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    void createNoticeScrollView();
    void showNotice(std::string sNotice);
    CCLabelTTF* HorizontalSpacingANDVerticalSpacing(std::string _string, const char *fontName, float fontSize, float horizontalSpacing, float verticalSpacing, float lineWidth);
    //装饰灯动画
    void lightRedCall(CCNode* pNode);
    void redLightBlink();
    void redLightScroll();
    
    void hideRankLayerColor();
    
    void onEnterTransitionDidFinish();
    void onExit();
    void setYaZhuLabel(std::string str);
    void setStatusYaZhuLabel(bool bStatus);
    void dealGuessResultGuide(int nResult);
    void setEnterGuided(bool bEnterGuide);
    
    void shakeScreen();
    
    void showGuideLayer();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void openReplaceSceneTime();
    void backHallScene();
#endif
    
    void openVIP();
    void onHttpVIPVersionComplete(CCHttpClient* sender, CCHttpResponse* response);
    void sendVIP();
    void onHttpVIPComplete(CCHttpClient* sender, CCHttpResponse* response);
    
public:
	static std::string m_sCmdLine;
	CClientKernel * m_pClientKernel;
    
    CC_SYNTHESIZE(SCORE, m_lUserScore, UserScore);
    CC_SYNTHESIZE(int, m_nGameRollingEffectID, GameRollingEffectID);
    CC_SYNTHESIZE(int, m_nPrizeRollingEffectID, PrizeRollingEffectID);
    CC_SYNTHESIZE(bool, m_bShowExitGame, ShowExitGame);
    
    //五星之后额外奖励
    int m_nFiveStarExtra;
    int m_nSixStarExtra;
    int m_nSevenStarExtra;
    int m_nEightStarExtra;
    
private:
    int m_nTimesOnline;
    CCNode* m_node1;
    CCNode* m_node2;
    CCMenu* m_menuMain1;
    CCMenu* m_menuMain2;
    CCScrollView* m_GameScrollView;
    CCScrollView* m_PrizeScrollView;
    CCScrollView* m_GuessScrollView;
    CCSprite* m_spNums[SHOW_NUM];      //3张图片
    CCSprite* m_spPrize;               //Prize的转动图片
    CCLabelTTF* m_labelUserScore;
    CCLabelAtlas* m_labelCurrentYa;      //押注金币label
    LONGLONG        m_lChipScore;      //下注金币
    LONGLONG        m_lDefultScore;    //默认下注金币
    LONGLONG        m_lTotalScore;

    bool            m_bReceiveID;      //是否接收到转动结果消息
    bool            m_bRolling;        //控制转动的bool值
    
    //转动相关
    bool            m_bSame;                //rolling图案是否相同
    bool            m_bLackChip;            //是否可以继续下注
    int             m_nPrizeTimes;          //rolling奖励倍数
    LONGLONG        m_lRollPrize;           //转动相同后奖励
    LONGLONG        m_lNativePrize;         //本地控制的小丑底注
    int             m_nStarCount;           //星星个数
    int             m_nRollerID[SHOW_NUM];  //转动结果图案ID
    //prize
    bool            m_bPrizeRoll;           //Prize转动控制
    int             m_nPrizeID;             //结果id
    int             m_nSelectID;            //用户选则图案
    
    bool            m_bAuto;                //是否自动转动
    bool            m_bLackToGuess;         //是否足以猜大小
    
    int m_nMenuIndex;
    
    
    CCSprite* m_pBigPrize[10];              //巨奖是个数字
    CCScrollView* m_pNoticeScrollView;      //公告滚动层
    std::vector<tagNoticeInfo> m_vctNotice;
    CCSprite* m_pRankSp;                    //排行背景
    CCLayerColor* m_rankLayer;
    CCLabelTTF* m_pLevelRankTTF;            //自己排行名次
    CCLabelTTF* m_pNickNameTTF[11];         //所有排行昵称
    CCLabelTTF* m_pScoreTTF[11];            //所有排行分数
    std::vector<tagRankInfo*> m_vctRank;
    
    CCArray* m_noticeArray;
    
    CCMenuItemSprite* m_pVoiceOpenItem;
    CCMenuItemSprite* m_pVoiceCloseItem;
    float m_fSound;
    float m_fEffect;
    LONGLONG m_nCurrentYaZhu;
    bool m_bEnterGuided;
    CCSprite* pBackground;
    
    bool m_bInitUIComplete;//是否初始化完成
    bool m_bInitNETComplete;//是否接收到场景消息
    
    
    int m_nItemNum;
    LONGLONG m_lGetPrizeExtra;
    int m_nCalTime;
    CCLabelTTF* m_pCalTTF;
    
    bool m_bReceiveCoinCant;//收到金币不足消息
    std::string m_sCoinCantDes;
    bool m_bReceiveGameScene;
};

#endif // __HELLOWORLD_SCENE_H__
