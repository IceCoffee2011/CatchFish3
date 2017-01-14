#ifndef LotteryLayer_hpp
#define LotteryLayer_hpp

struct TelCardConfig
{
    int nID;
    int nPrize;
    int nLotteryCount;
    int nCardType;
};

class LotteryLayer : public CBASELAYER
{
public:
    ~LotteryLayer();
    static LotteryLayer* create(bool bLocalOpen, int nLotteryNum = -1);
    virtual bool init(bool bLocalOpen, int nLotteryNum = -1);
    void loadUI();
    
    void readLotteryConfig();
    void parseData(std::string str);
    void saveFile(std::string str);
    void loadUILotteryBg(int nCount);
    
    void sendExchangeData(CCObject* object);
    
    void sendUserInfo();
    void OnHttpExchangeLotteryComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void setLabelMoney(int nCount);
    void clickExchangeType(CCObject* object);
    void closeMenu(CCObject* object);
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
private:
    CCMenu* m_menuYidong;
    CCMenu* m_menuLiantong;
    CCMenu* m_menuDianxin;
    CCSprite* m_spYidong;
    CCSprite* m_spLiantong;
    CCSprite* m_spDianxin;
    int m_nSelectID;
    int m_nPriseName;
    CCLabelAtlas* m_labelMobile;
    CCLabelTTF* m_labelMobileInfo;
    
    std::vector<TelCardConfig*> m_vctTelCard;
    bool m_bLocalOpen;
    CCNode* m_nodeYidong;
    CCNode* m_nodeLiantong;
    CCNode* m_nodeDianxin;
    bool m_bRequestStatus;
    int m_nCurrentMobileCount;
    CCSprite* m_spSelect;
    
};

#endif 
