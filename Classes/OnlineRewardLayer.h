#ifndef OnlineRewardLayer_hpp
#define OnlineRewardLayer_hpp


class OnlineRewardLayer : public CBASELAYER
{
public:
    OnlineRewardLayer();
    ~OnlineRewardLayer();
    virtual bool init(std::vector<CMD_GRO_OneOnlineRewardInfo> vOnlineRewardInfo);
    static OnlineRewardLayer * create(std::vector<CMD_GRO_OneOnlineRewardInfo> vOnlineRewardInfo);
    void loadUI();
    void loadOnlineData(std::vector<CMD_GRO_OneOnlineRewardInfo> vOnlineRewardInfo);
    
    void setOnlineRewardInfo(std::vector<CMD_GRO_OneOnlineRewardInfo> vOnlineRewardInfo);
    void showOnlineRewardUI(std::vector<CMD_GRO_OneOnlineRewardInfo> vOnlineRewardInfo);
    
    void setOnlineStatus(int nStatus);
    
    void clickMenu(CCObject* object);
    void removeWnd();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    CCLabelTTF* m_pOnlineLabel;
    
private:
    CCSprite* m_spBg;
    CCScrollView* m_pOnlineScroll;
    CCLayer* m_pOnlineLayer;
    CCArray* m_arrOnline;
    
    
};

#endif /* OnlineRewardLayer_hpp */
