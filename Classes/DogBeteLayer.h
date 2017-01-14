#ifndef DogBeteLayer_hpp
#define DogBeteLayer_hpp

class DogBeteLayer : public CBASELAYER, public CClientKernelSink
{
public:
    DogBeteLayer();
    ~DogBeteLayer();
    static DogBeteLayer* getInstance();
    static DogBeteLayer* _instance;
    static CCScene* scene();
    virtual bool init();
    CREATE_FUNC(DogBeteLayer);
    void loadRes();
    void loadChatBgUI();
    void loadDogUI();
    void loadBeteUI();
    
    void clickChatMenu(CCObject* object);
    void clickRebet(CCObject* object);//押注按钮
    void clickChargeMenu(CCObject* object);//充值按钮
    
    void scheduleBetTime();//下注倒计时
    void schedule56PrizeTime();//最大奖56倒计时
    
    void changeDogBigPrize(SCORE lBigPrize);
    CCAnimate* getDogAnimate(std::string sFirstName, int nFirstCount, int nTotalCount, float fDelayTime);
    void playDogAnimate();
    void setDogChampion(int nFirst, int nSecond);//设置冠军亚军，显示特效
    
    std::string getStringByScore(SCORE llBeteMoney);
    void resetData();//初始化或者清零数据
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    
    int m_nOwnOneBete[15];//我的每个选项卡押注大小
    
    CClientKernel * m_pClientKernel;
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
    
private:
    CCSprite* m_pDogBg;
    CCSprite* m_pBetGuang;
    CCLabelBMFont* m_pLabelUserScore;//玩家分数
    CCLabelBMFont* m_pLabelBetTime;//下注倒计时
    int m_nBetTime;//下注倒计时，从服务端接收
    CCLabelBMFont* m_pLabel56Time;//上次开5-6时间
    int m_n56Time;//上次开5-6时间（单位为秒）从服务端接收
    SCORE m_llCaichi;//奖池
    CCSprite* m_pDogBigPrize[10];//奖池
    
    CCArray* m_arrChampion;
    CCArray* m_arrBeteNormal;
    CCArray* m_arrBeteDown;
    int m_nCurrentBete;//玩家当前选择押注大小（2000， 1万，5万，10万）
    CCLabelBMFont* m_pLabelOwnBete;//我押注大小
    SCORE m_llOwnBete;//我的押注大小
    
    CCLabelBMFont* m_pLabelAllBete;//玩家总押注大小
    
    CCPoint m_pStartPoint;
    CCPoint m_pEndPoint;
    
    bool m_bBetStart;//下注开始
    
};

#endif
