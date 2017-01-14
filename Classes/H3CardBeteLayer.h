#ifndef H3CardBeteLayer_h
#define H3CardBeteLayer_h

class H3CardBeteLayer : public CBASELAYER, public CClientKernelSink
{
public:
    H3CardBeteLayer();
    ~H3CardBeteLayer();
    static H3CardBeteLayer* getInstance();
    static H3CardBeteLayer* _instance;
    static CCScene* scene();
    CREATE_FUNC(H3CardBeteLayer);
    virtual bool init();
    
    void loadRes();
    
    void loadBgUI();
    
    void startSendBackCard();
    void scheduleSendClientCard();
    void scheduleShowCard1Animate();
    void scheduleShowCard2Animate();
    void scheduleShowCard3Animate();
    void scheduleShowCard4Animate();
    void scheduleShowCard5Animate();
    void scheduleShowCard6Animate();
    void showReceiveLiangCard();
    void showCardAnimate();//调用六个亮牌定时器
    void showCard2Animate(CCNode* pNode);//显示红黑中各自最后一张牌的亮牌动画
    void showBlackType(CCNode* pNode);
    void showRedType(CCNode* pNode);
    
    void removeNode(CCNode* pNode);
    
    void clickMenu(CCObject* object);//退出按钮、充值按钮、排行榜按钮
    void clickBeteMenu(CCObject* object);//下注按钮
    void openJiangchi(CCObject* object);//奖池按钮
    void clickTouxiang(CCObject* object);//玩家信息按钮，加上自己，一共9个
    void clickWanjiaList(CCObject* object);//玩家列表按钮
    void clickZoushi(CCObject* object);//走势图
    void clickHongbao(CCObject* object);//发红包，燃放礼花
    
    void scheduleShowJackpot();
    
    void onekeyBet();
    //触摸
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    
    std::string getNickName(std::string strName);
    std::string getString2FromNumber(SCORE llScore);
    std::string getString3FromNumber(SCORE llScore);
    void showNoBetBg();
    bool checkBetCoinOrOwnCoin();//检查是否能下注
    void setPlayerBet(int nPosIndex, int nBetIndex, SCORE llBetScore, bool bIsMy);//玩家座位位置（10个）、下注位置（3个），黑、红、特殊牌型
    void setBetReturn();//下注区域筹码回到玩家座位上（10个，从0开始，自己是8）
    void showMyBetNum(int nArea);//显示自己下注多少
    bool getOwnBetArea(CCTouch* pTouch, CCSprite* pBetSprite, CCRenderTexture* rtRender);
    
    //与服务端通信
    void sendOwnBet(int nBetIndex);
    
    //ClientKernel
    CClientKernel * m_pClientKernel;
    
    virtual void ResetGameFrame() {}
    virtual void CloseGameFrame() {}
    virtual bool OnEventSocket(const TCP_Command & Command, const void * pBuffer, WORD wDataSize);
    virtual bool OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize);
    virtual void OnEventUserMemberOrder(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) {}
    virtual void OnEventUserStatus(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) {return;}
    virtual bool OnEventUserLeave(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser);
    virtual bool OnEventUserEnter(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser);
    virtual void OnEventUserScore(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) {}
    
    void setUserBetInfo(SCORE llLow, SCORE llMid, SCORE llHigh);//设置自己可以选择下注的筹码
    void setOwnScore(SCORE llScore);//设置自己当前金币数量
    void setOwnBetCoin(SCORE llBlack, SCORE llRed, SCORE llSpecial);//设置自己已经下注
    void setBetAreaTotal(SCORE llBlack, SCORE llRed, SCORE llSpecial);//设置三个区域总下注
    void showPlayerWinScore(int nPlayerPos, SCORE llScore);
    
    void setPlayerInfo(DWORD nUserID[8]);
    void setTableInfo(int nTableID, int nNum);
    
    void setZoushi();
    
    void resetData();
    
    void showWinAnimate();
    void showZoushiAnimate();
    void startNewZoushiAnimate();
    
    
    void scheduleShowBet();
    void removeThis(CCNode* pNode);
    void betIndexAction(bool bStart);
    void scheduleRemind();
    void remindCallFunc(CCNode* pNode);
    void betFinish();
    void bigWinnerShow();
    void vsAction();
    void betStart();
    
    int m_nPailu1[20];//黑底板还是亮底板(0表示黑色底板，1表示红色底板）
    int m_nPailu3[20];//（0表示单张，1表示小对子，2大对（9以上），3顺子，4金花，5顺金，6豹子）
    int m_nPailu2[20];//（0表示黑胜，1表示红胜）
    //点击头像，查看玩家信息，客户端保存ID
    int m_nClickPlayerPos;//点击玩家位置（0-8）
    DWORD m_dwPlayerUserID[9];
    
public:
    CCSprite* m_pCardBeteBg;
    CCSprite* m_pSeleteBete;
    CCLabelBMFont* m_pLabelOwnScore;
    CCLabelTTF* m_labelLowCoin;
    CCLabelTTF* m_labelMiddleCoin;
    CCLabelTTF* m_labelHighCoin;
    CCLabelTTF* m_labelCurrentTable;//当前是第多少桌
    CCLabelTTF* m_labelBeteInfo;//当前状态说明：开始下注、正在开牌等
    
    CCSprite* m_spXuanZhong;
    bool m_bXuanZhong;
    
    int m_nSendCardIndex;
    CCArray* m_arrCardBack;//背面牌（发牌）数组
    CCArray* m_arrCardLiang;//亮牌数组
    
    //结算时胜方押注区亮底
    CCSprite* m_pCardYingBg;
    CCSprite* m_pBetBlackBg;
    CCSprite* m_pBetRedBg;
    CCSprite* m_pBetSpecialBg;
    CCSprite* m_pBetBlack2Bg;
    CCRenderTexture* m_rtBetBlackBg;
    CCRenderTexture* m_rtBetRedBg;
    CCRenderTexture* m_rtBetSpecialBg;
    CCArray* m_arrBetBlackSprite;
    CCArray* m_arrBetRedSprite;
    CCArray* m_arrBetSpecialSprite;
    CCSprite* m_pXiazhuBlackDi;
    CCSprite* m_pXiazhuRedDi;
    CCSprite* m_pXiazhuSpecialDi;
    CCLabelTTF* m_labelBetBlack;
    CCLabelTTF* m_labelBetRed;
    CCLabelTTF* m_labelBetSpecial;
    CCLabelTTF* m_labelOwnBetBlack;
    CCLabelTTF* m_labelOwnBetRed;
    CCLabelTTF* m_labelOwnBetSpecial;
    
    CCSprite* m_pContainBg;//不能同时下注背景
    
    //幸运星、富豪背景
    CCArray* m_arrRicherBg;
    
    //状态
    /*默认是0， 1表示开始下注， 2表示下注结束*/
    int m_nCurrentStatus;
    SCORE m_llCurrentSelectBet;
    SCORE m_llOwnUserScore;
    SCORE m_llLowCoin;
    SCORE m_llMiddleCoin;
    SCORE m_llHighCoin;
    SCORE m_llBlackBetCoin;
    SCORE m_llRedBetCoin;
    SCORE m_llSpecialBetCoin;
    int m_nBetArea;//不能同时在红黑方下注
    
    BYTE m_cbBlackCard[3];
    BYTE m_cbRedCard[3];
    int m_nBlackType;//黑色牌型
    int m_nRedType;//红色牌型
    
    int m_nBetTimes;//下注倒计时
    bool m_bSpecialCardType;//是否是特殊牌型
    int m_nCardResult;//0表示黑方胜，1表示红方胜
    
    //大赢家信息
    DWORD m_dwBigWinUserID;
    SCORE m_llBigWinScore;
    
    //奖池label
    CCLabelBMFont* m_labelJackpot;
    SCORE m_llJackpot;
    SCORE m_llNewJackpot;
    
    //走势图
    CCMenu* m_pZoushiMenu;
    CCArray* m_arrPailu1;
    CCArray* m_arrPailu2;
    
    //重复投功能
    SCORE m_llRepeatBlackBet;
    SCORE m_llRepeatRedBet;
    SCORE m_llRepeatSpecialBet;
    
    int m_nCurrentOnlineNum;
    
};

#endif
