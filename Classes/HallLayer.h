#ifndef __HALLLAYER_H__
#define __HALLLAYER_H__

#pragma pack()

#define        TARGET_SCALE          1.0f
#define        NOTICE_STRING         "Welcome to 669BOLE Catch Fish Game!"


class CHallLayer : public CBASELAYER
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(CHallLayer);
    ~CHallLayer();
    void loadSelectedGameTypeNode(bool bGameTypeSelected, std::vector<tagGameKind>& vGameKind);
    void clickEnterGame(CCObject* object);
    void setRoomInfo(std::vector<tagGameServer> & vGameServer);
    void loadUI();
    void notice();
    void setWeekNum(int nWeekNum);
    void setNotice(float dt);                  //设置公告
    void gameCallBack(CCObject* obj);
    void buttonCallBack(CCObject* obj);
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent);
    
    void setMainLogic(CMainLogic * pMainLogic);
    void setUserNickName(const char * str);
    void setNickNameColor();
    void setUserScore(int64_t score);
    void setUserID(unsigned int gameID);
    void setGender(int gender);
    void setTicket(int nTicket);
    void setVipExp();//设置VIP等级以及经验值
    
    void onHttpComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    CCLabelTTF * m_cclUserScore;
    
    void onHttpSharedComplete(CCHttpClient* sender, CCHttpResponse* response);
    void sendSharedCompleted();

	void onEnterTransitionDidFinish();
    void requestMail(float dt);
    
    void setMailNumber(int num);
    void sendRank();
    void onHttpRankComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void sendRankVersion();
    void onHttpRankVersionComplete(CCHttpClient* sender, CCHttpResponse* response);
    void saveRankVersion(std::string str);
    
    void setNameLength(std::string nameStr);
    
    //发送登录消息
    void constructGuestPacket();//构造游客数据
    void sendLogonInfo();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	bool getExitOtherGame();
	void openOtherGame();
	void openNiuDownLoad();
    bool canExitFishGameAndroid(std::string str);
#endif

    void loadGameType();
    
    //按钮光效动画
    CCAnimate* getNiuAnimate();
    CCAnimate* getMenuAnimate(int nIndex);
    
    
    //访问VIP版本号
    void sendVIPVersion(std::string sTag);
    void onHttpVIPVersionComplete(CCHttpClient* sender, CCHttpResponse* response);
    void sendVIP();
    void onHttpVIPComplete(CCHttpClient* sender, CCHttpResponse* response);
    //兑换商城
    void sendLottery();
    void onHttpLotteryComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    /*************************更多游戏*************************/
    void TimerToLoadMoreGames(float dt);
    
    void hideOneChargeMenu();
    //隐藏首充按钮，显示剩余领取次数界面
    void showLastTimes(int count);
    void showFirstCharge();
    void showFirstChargeAnimate(bool bChargeBack = false);
    void showNiuGame();
    void showNiuGameMenu();
    
    void loadMagicPtc();
    void loadFirstMagicPtc();
    void playParticle(float x, float y);
    
    
    void scheduleBubble(float ft);
    void removeSelf(CCNode* pNode);
    void createBubble1();
    void createBubble0();
    
    void callBackForMoreGamesMenu(CCNode * node);
    void clickLink(std::string sTag);
    
    void hideFirstChargeAnimate();
    
    
    CCMenu* m_menuReceive;//首充之后领取按钮
    bool m_bEnterHall;
    void checkNoticeCount();
    void showHallNotice();
    void resumeNoticeCount();
    std::string m_sHallNotice;
    std::vector<NoticeVct> m_vctStringNotice;
    std::vector<VIPNotice> m_vctLabelNotice;
    
//    void menuGoToWealthBag(CCObject* object);
    
    //心形动画
    void showMailAction2();
    
    void showfreecoinAction(bool bShow);
    void showFreecoinAction2();
    
    void showVIPAction(bool bShow);
    void showVIPAction2();
    
    void addChargeLayer();
    
    void setClickMenu();
    void showNoticeLayer();
    
    void update(float dt);
    void menuCallFunc(CCObject* obj);
    
    void showQuickStartAnimate();
    
    void setGonggaoStatus(bool bShow);
    void setGonggaoStatus2();
    
    CC_SYNTHESIZE(bool, m_bInitCompleted, InitCompleted);
    CC_SYNTHESIZE(bool, m_nInitNetComplete, InitNetComplete);
    CC_SYNTHESIZE(bool, m_bOpenGonggao, OpenGonggao);
    
    /************头像************/
    void EditTouXiang(CCObject* object);
    void createDefaultTouXiang(int gender);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void onHttpVersionComplete(CCHttpClient* sender, CCHttpResponse* response);
    std::vector<ChargeWebView*> m_vChargeWebView;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void createTouXiang(string pathStr);
    void onHttpVersionComplete(CCHttpClient* sender, CCHttpResponse* response);
    void downloadPicture(int nUserID);
    void onGetFinished(CCHttpClient* client, CCHttpResponse* response);
#endif
    
    void showMatchGuideEnd(bool bJoinSuccess);
    
    void updateRoomUserCount(WORD wServerID, WORD nUserCount);
    
    void addMenuParticle(CCMenuItem* item);
    
    CCMenu* m_pVipMenu;
//    CCClippingNode* m_pHeadClipping;
    int m_nOwnVersion;
    
    CCNode* m_nodeSelectedGameType;

private:
    CCScrollView* m_pScrollViewGameType;                    //游戏类型滚动试图
    CCArray* m_arrGameType;
    int m_nIsSelectedGameType;                             //1为游戏类型选项、2为游戏房间选项、3为小游戏类型选项
    CCNode* m_nodeSmallGame;
    CCScrollView* m_pScrollView;
    CCArray* m_arrGameRoom;
    CCPoint m_ptStart;
    std::string m_sNickName;
    
    MP_Device_WRAP* m_pMagicDevice;											  //粒子引擎
    
    CMainLogic * m_pMainLogic;
    //分数
    int64_t m_nUserScore;
    CCLabelTTF * m_cclNickName;
    CCLabelTTF * m_cclUserID;
    
    CCLabelTTF* m_labelNoticeStr;                           //公告
    CCSprite* m_spNoticeBg;                                 //公告背景
    CCSprite* m_spBg;
    
    int m_nMenuIndex;
    
    //vip经验值
    CCSprite* m_pVIPBg;
//    CCSprite* m_pVIPHead;
    CCScale9Sprite* m_pVIPSlider;
    CCLabelAtlas* m_labelVIP;
    
    //按钮
    CCMenu* m_menuCoin;
    
    //加一层，街机捕鱼、欢乐牛牛
    CCNode* m_nodeGameType;//游戏类型
    bool m_bShowZhayan;
    CCSprite* m_pSpNiuNiu;
    int m_nZhayanIndex;
    
    CCMenu* m_menuFirst;//首充按钮
    CCMenu* m_menuOneCharge;
    
    CCLabelTTF* m_labelReceive;
    
    bool m_bFirstChargeAnimate;
    
    bool m_bShowNiuGame;
    
    int     m_nRequsetStatus;
    int     m_nDamageCount;
    bool    m_bRequestDamage;
    int     m_nMoreGamesCount;
    std::vector<MoreGamesInfo> m_vMoreGamesInfo;
    std::string m_sMoreGamesInfo;
    std::string m_sAdVersionOfRequest;
    
    bool m_bReceiveHallNotice;
    bool m_bRequestAdvert;
    CCMenu* m_menuWealth;
    
    CCSprite* m_pSpVIP;
    CCSprite* m_pSpUserScoreBg;
    CCSprite* m_pSpUserMatchBg;
    CCLabelTTF* m_labelMatchNum;
    
    bool m_bExistRoom;
    
    int m_nServerPort;
    bool m_bInitNetComplete;
    CCSprite* m_quickSprite;
    CCPoint m_pointEnd;
    bool m_bCanClickMenu;
    
    std::vector<std::string> m_vctDefaultNotice;
    int m_nNoticeIndex;
    std::string m_strMsg;
//    CCSprite* m_spMailGuangBg;
//    CCSprite* m_sp2MailGuangBg;
    bool m_bFBAndPriate;
};
#endif // __HALLLAYER_H__
