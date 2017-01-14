#ifndef H3PlayerListLayer_h
#define H3PlayerListLayer_h

class PlayerListLayer : public CBASELAYER
{
public:
    PlayerListLayer();
    ~PlayerListLayer();
    CREATE_FUNC(PlayerListLayer);
    virtual bool init();
    
    void loadUI();
    
    void clickMenu(CCObject* object);
    void removeWnd();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
private:
    CCSprite* m_spBg;
};

#endif
