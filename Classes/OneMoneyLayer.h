#ifndef OneMoneyLayer_h
#define OneMoneyLayer_h

class OneMoneyLayer : public CBASELAYER
{
public:
    OneMoneyLayer();
    ~OneMoneyLayer();
    CREATE_FUNC(OneMoneyLayer);
    virtual bool init();
    void loadUI();
    
    void clickMenu(CCObject* object);
    
    void removeWnd();
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
private:
    CCSprite* m_spOneBg;
};

#endif
