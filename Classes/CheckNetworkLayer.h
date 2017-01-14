#ifndef __CatchFish3__CheckNetworkLayer__
#define __CatchFish3__CheckNetworkLayer__

class CheckNetworkLayer : public CCLayer
{
public:
    CREATE_FUNC(CheckNetworkLayer);
    virtual bool init();
    void loadUI();
    void showTime(float dt);
    void OnButtonBack(CCObject* object);
    
    void openTouchAndTime();
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
private:
    CCScale9Sprite* pSpDi;
    CCLabelTTF* m_labelTime;
    int m_nLastTime;
    CCSprite* m_spBackLogon;
    CCMenu* m_menuBackLogon;
};

#endif /* defined(__CatchFish3__CheckNetworkLayer__) */
