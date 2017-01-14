#ifndef __CatchFish__VIPLayer__
#define __CatchFish__VIPLayer__

struct Tag_VIP2Info
{
    int nLevel;//VIP等级
    float fChargeBackPercent;//充值返点
    std::string sReward;//领取奖励
    std::string sHitLv;//捕鱼命中率
};

class VIPLayer : public CBASELAYER
{
public:
    ~VIPLayer();
    virtual bool init(bool bLocalOpen, int nVipLevel = 1);
    static VIPLayer* create(bool bLocalOpen, int nVipLevel = 1);
    void loadUI();
    
    void readVipConfig();
    void loadVIPInfoUI(int nLevelTotal);
    void parseData(std::string str);
    
    void saveFile(std::string str);
    
    void setMenuTouch();
    void setMenuTouch2();
    
    void closeMenu(CCObject* object);
    
    void clickMenu(CCObject* object);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    
    void TimerToSetVipMenuTouchEnable(float dt);
    void setLabelScore();
    
private:
    bool m_bLocalOpen;
    int m_nVipLevel;
    int m_nCurrentVipLevel;
    CCLabelAtlas* m_labelScore;
    CCLabelTTF* m_LessTTF;
    CCMenu* m_VIPMenu;
    
    CCArray* m_arrVipInfo;          //存放CCNode
    std::vector<Tag_VIP2Info*> m_vctVIPInfo;
    
    bool m_bClickCharge;
    int m_nUpgradeMoney;
    CCPoint m_startPoint;
    bool m_bIsEnable;
    int m_nNoticeLevel;
};

#endif /* defined(__CatchFish__VIPLayer__) */
