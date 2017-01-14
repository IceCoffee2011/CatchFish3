#ifndef __CatchFish3__RankLayer__
#define __CatchFish3__RankLayer__

struct tagRankTimes//排行榜比赛名次
{
    int nID;
    int nLevel;
    SCORE llScore;
    std::string sNickName;
    
};

struct tagRankVIPTimes
{
    int nID;
    int nLevel;
    SCORE llVipExp;
    std::string sNickName;
};

struct tagVipRankInfo
{
    std::string sLevel;
    std::string sBarrel;
    std::string sLogonCoin;
    std::string sChargeExtra;
};

struct tagMoneyRankInfo
{
    std::string sCharge;
    std::string sChargeExtra;
};

struct tagMatchInfo
{
    std::string sLevel;
    std::string sCoin;
    std::string sZhenzhu;
    std::string sLottery;
};

struct tagWeekMatchInfo
{
    std::string sLevel;
    std::string sPrize;
};


class RankLayer : public CBASELAYER, public CCScrollViewDelegate
{
public:
    ~RankLayer();
    CREATE_FUNC(RankLayer);
    virtual bool init();
    
    void loadUI();
    void loadMatchXML();
    void loadMatchUI();
    
    void loadUIVip();
    void sendVipData();
    void updateVIPData(std::string str);
    void onHttpVIPComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void loadUIMoney();
    void sendMoneyData();
    void onHttpMoneyComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    
    void loadUIDayRank();
    void sendDayData();
    void onHttpDayComplete(CCHttpClient* sender, CCHttpResponse* response);
    void updateDayData(std::string str);
    
    void loadUIWeekRank();
    void sendWeekData();
    void onHttpWeekComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void showCurrentMenu(int nIndex);
    void clickMenu(CCObject* object);
    void closeMenu(CCObject* object);
    
    void showDayRank();
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
private:
    CCSprite* m_spRankBg;
    CCArray* m_arrNormal;
    CCArray* m_arrSelect;
    CCArray* m_arrRankInfo;
    int m_nCurrentIndex;//当前是哪个界面（1为VIP，2为财富，3为每日比赛，4为每周比赛
    
    std::string m_sVip1Title;
    std::string m_sVip2Title;
    
    std::string m_sMoney1Title;
    std::string m_sMoney2Title;
    
    std::string m_sDayMatchTitle;
    
    std::string m_sWeekMatchTitle;
    
    //vip
    CCNode* m_VipNode;
    CCScrollView* m_scrollVipView;
    CCLayer* m_containVipLayer;
    CCArray* m_arrVipCell;
    std::vector<tagRankVIPTimes*>m_vctVipRankTimes;
    CCScrollView* m_scrollVipInfoView;
    //money
    CCNode* m_MoneyNode;
    CCScrollView* m_scrollMoneyView;
    CCLayer* m_containMoneyLayer;
    CCArray* m_arrMoneyCell;
    std::vector<tagRankTimes*>m_vctMoneyRankTimes;
    CCScrollView* m_scrollMoneyInfoView;
    //day
    CCNode* m_DayNode;
    CCScrollView* m_scrollDayView;
    CCLayer* m_containDayLayer;
    CCArray* m_arrDayCell;
    std::vector<tagRankTimes*> m_vctDayRankTimes;
    CCScrollView* m_scrollWeekInfoView;
    //week
    CCNode* m_WeekNode;
    CCScrollView* m_scrollWeekView;
    CCLayer* m_containWeekLayer;
    CCArray* m_arrWeekCell;
    std::vector<tagRankTimes*> m_vctWeekRankTimes;
    
    //matchinfo
    CCNode* m_MatchNode;
    CCScrollView* m_scrollMatchView;
    CCLayer* m_containMatchLayer;
    std::vector<tagVipRankInfo> m_vctVipRank;
    std::vector<tagMoneyRankInfo> m_vctMoneyRank;
    std::vector<tagMatchInfo> m_vctMatch;
    std::vector<tagWeekMatchInfo> m_vctWeekMatch;
    CCLabelTTF* m_labelDayRankTitle;
    CCLabelTTF* m_labelDayYesRank;//昨天排名
    CCSprite* m_pPriseSp;
    
    CCLabelTTF* m_labelWeekRankTitle;
    CCLabelTTF* m_labelWeekYesRank;//上周排名
    CCLabelTTF* m_labelWeekPriseInfo;//周冠军奖品
    CCLabelTTF* m_labelWeekTwoInfo;
    CCLabelTTF* m_labelWeekThreeInfo;
    CCLabelTTF* m_labelWeekFourInfo;
    
    CCLabelTTF* m_lVIPJifen;
    CCLabelTTF* m_lMoneyJifen;
    CCLabelTTF* m_lDayJifen;
    CCLabelTTF* m_lWeekJifen;
    CCLabelTTF* m_lRankWeek;
    CCLabelTTF* m_lRankVIP;
    CCLabelTTF* m_lRankMoney;
    CCLabelTTF* m_lRankDay;
    
    CCSprite* m_spAnimateDay;
    CCSprite* m_spAnimateMoney;
    CCSprite* m_spAnimateWeek;
    CCSprite* m_spAnimateVIP;
    
};

#endif
