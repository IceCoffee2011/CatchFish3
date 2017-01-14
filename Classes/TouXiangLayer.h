#ifndef TouXiangLayer_h
#define TouXiangLayer_h

class TouXiangLayer : public CBASELAYER
{
public:
    TouXiangLayer();
    ~TouXiangLayer();
    static TouXiangLayer* create(int nID);
    virtual bool init(int nID);
    
    void loadUI();
    
    void clickMenu(CCObject* object);
    void removeWnd();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
private:
    CCSprite* m_spBg;
    CCArray* m_arrTXTotal;
    CCSprite* m_spSelect;
    int m_nSelectID;
    int m_nCurrentID;
};

#endif
