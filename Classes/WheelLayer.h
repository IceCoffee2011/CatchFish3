#ifndef WheelLayer_h
#define WheelLayer_h

class WheelLayer : public CBASELAYER
{
public:
    WheelLayer();
    ~WheelLayer();
    virtual bool init();
    CREATE_FUNC(WheelLayer);
    void loadUI();
    
    void loadPrizeItem();
    
    void setWheelPrize(BYTE cbIndex, int nCup);
    float getStopRotation();
    
    void clickMenu(CCObject* object);
    void playEffect();
    void playEffect2();
    void startWheelAction();
    void stopWheelAction();
    
    void showGuangSprite();//显示光效
    void removeNode(CCNode* pNode);
    void showPrizeLayer();
    
    void removeWnd();
    
    void scheduleSpeed();
    
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
private:
    CCSprite* m_spWheelBg;
    CCSprite* m_spWheelDi;
    
    CCMenu* m_menuWheel;
    
    BYTE m_cbPrizeIndex;//中奖下标
    int m_nMatchCup;//玩家当前周赛奖杯数
    
    bool m_bReceiveResult;
    bool m_bClickWheelMenu;
    
    float m_fWheelRotation;
    
    int m_nPlayEffectTimes;//播放音效次数
    int m_nPlayIndex;
    
};

#endif
