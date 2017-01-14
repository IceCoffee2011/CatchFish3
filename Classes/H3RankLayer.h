#ifndef H3RankLayer_h
#define H3RankLayer_h

struct TodayEarn
{
    int nRankID;
    int nFaceID;
    SCORE llScore;
    std::string sName;
};

struct EstEarn
{
    int nRankID;
    int nFaceID;
    SCORE llScore;
    std::string sName;
    std::string sTime;
};

struct FoodEarn
{
    int nFaceID;
    SCORE llScore;
    SCORE llTotalScore;
    std::string sName;
    std::string sInfo;
};

class H3RankLayer : public CBASELAYER
{
public:
    H3RankLayer();
    ~H3RankLayer();
    CREATE_FUNC(H3RankLayer);
    virtual bool init();
    
    void loadUI();
    
    void loadTodayEarn();
    void loadWeekEarn();
    void loadEstEarn();
    void loadFoodEarn();
    
    void showTodayEarn();
    void showWeekEarn();
    void showEstEarn();
    void showFoodEarn();
    
    void requestTodayEarn();
    void onHttpTodayEarnComplete(CCHttpClient* pSender, CCHttpResponse* response);
    
    void requestWeekEarn();
    void onHttpWeekEarnComplete(CCHttpClient* pSender, CCHttpResponse* response);
    
    void requestEstEarn();
    void onHttpEstEarnComplete(CCHttpClient* pSender, CCHttpResponse* response);
    
    void requestFoodEarn();
    void onHttpFoodEarnComplete(CCHttpClient* pSender, CCHttpResponse* response);
    
    std::string getStringScore(SCORE llScore);
    
    void clickMenu(CCObject* object);
    
    void clickClose(CCObject* object);
    void removeWnd();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
private:
    CCSprite* m_spBg;
    CCSprite* m_pRankBg;
    CCNode* m_nodeTodayEarn;//今日盈利榜
    CCNode* m_nodeWeekEarn;//本周盈利榜
    CCNode* m_nodeEstEarn;//至尊盈利榜
    CCNode* m_nodeFoodEarn;//进入富豪榜
    
    CCMenu* m_menuTodayEarn;
    CCMenu* m_menuWeekEarn;
    CCMenu* m_menuEstEarn;
    CCMenu* m_menuFoodEarn;
    CCSprite* m_spTodayEarn;
    CCSprite* m_spWeekEarn;
    CCSprite* m_spEstEarn;
    CCSprite* m_spFoodEarn;
    
    CCScrollView* m_scrollTodayEarn;
    CCLayer* m_layerTodayEarn;
    CCScrollView* m_scrollWeekEarn;
    CCLayer* m_layerWeekEarn;
    CCScrollView* m_scrollEstEarn;
    CCLayer* m_layerEstEarn;
    CCScrollView* m_scrollFoodEarn;
    CCLayer* m_layerFoodEarn;
    
    std::vector<TodayEarn> m_vctTodayEarn;
    std::vector<TodayEarn> m_vctWeekEarn;
    std::vector<EstEarn> m_vctEstEarn;
    std::vector<TodayEarn> m_vctFoodEarn;
    FoodEarn m_foodEarn;
    
    CCLabelTTF* m_labelInfo;
    CCScale9Sprite* m_spInfoBg;
    bool m_bClickInfoMenu;
};

#endif
