#ifndef AdvertLayer_h
#define AdvertLayer_h

class AdvertLayer : public CBASELAYER
{
public:
    AdvertLayer();
    ~AdvertLayer();
    CREATE_FUNC(AdvertLayer);
    virtual bool init();
    void clickMenu(CCObject* object);
    
    void removeWnd();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
private:
    CCSprite* m_spBg;
};

#endif
