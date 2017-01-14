#ifndef __CatchFish3__TryBarrelLayer__
#define __CatchFish3__TryBarrelLayer__

class TryBarrelLayer : public CCLayerColor
{
public:
    static TryBarrelLayer* create(int nOpenType);//0为试用炮，1为加速，2为散射，3为锁定
    virtual bool init(int nOpenType);
    void loadUI();
    void clickMenu(CCObject* object);
    
    void removeWnd();
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
private:
    CCLayerColor* pLayerColor;
    CCScale9Sprite* pSpDi;
    CCSize m_visibleSize;
    CCLabelTTF* m_cclMessage;
    int m_nOpenType;
};

#endif
