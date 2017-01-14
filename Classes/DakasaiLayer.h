#ifndef DakasaiLayer_h
#define DakasaiLayer_h

struct TagDakasaiRank
{
    int nRank;//名次，从1开始
    int nNum;//奖杯数量
    int nLastNum;//昨天奖杯数量
    int nChargeNum;//奖杯数量变化
    std::string sName;//姓名
    std::string sPrize;//预期奖品
};

class DakasaiLayer : public CBASELAYER, public CCScrollViewDelegate
{
public:
    DakasaiLayer();
    ~DakasaiLayer();
    static DakasaiLayer* create(std::string str);
    virtual bool init(std::string str);
    void loadMenuUI();
    void loadMoreRankUI(std::string str);
    void loadRuleUI();
    void loadLastRankUI();
    
    void onHttpDakasaiComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void onHttpDakasaiLastComplete(CCHttpClient* pSender, CCHttpResponse* response);
    void onHttpVersionComplete(CCHttpClient* pSender, CCHttpResponse* response);
    
    void sendDakasaiData();
    void onHttpDakasaiDataComplete(CCHttpClient* pSender, CCHttpResponse* response);
    void saveFile(std::string str);
    void showRuleUI();
    void showPrizeUI();
    void readRuleData();
    
    void clickMenu(CCObject* object);
    void closeMenu(CCObject* object);
    void removeFunc();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
private:
    CCSprite* m_spBg;
    CCNode* m_nodeRank;
    CCNode* m_nodeLastRank;
    CCNode* m_nodeRule;
    CCNode* m_nodePrize;
    int m_nCurrentIndex;
    std::vector<TagDakasaiRank*> m_vctDakasai;
    std::vector<TagDakasaiRank*> m_vctLastDakasai;
    CCScrollView* m_pRankScrollView;
    CCLayer* m_Ranklayer;
    CCScrollView* m_pLastScrollView;
    bool m_bRequestSuccess;
    std::vector<std::string> m_vctRule;
    std::vector<std::string> m_vctPrize;
    CCScrollView* m_pRuleScrollView;
    CCScrollView* m_pPrizeScrollView;
    std::vector<VIPNotice> m_vctVIPNotice;
    CCMenu* m_moreMenu;
    CCMenu* m_lastMenu;
    CCMenu* m_ruleMenu;
    CCMenu* m_prizeMenu;
    CCSprite* m_spTitle1;
    CCSprite* m_spTitle2;
};

#endif
