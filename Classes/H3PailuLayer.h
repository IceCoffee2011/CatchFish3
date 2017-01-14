#ifndef H3PailuLayer_h
#define H3PailuLayer_h

class PailuLayer : public CBASELAYER
{
public:
    PailuLayer();
    ~PailuLayer();
    CREATE_FUNC(PailuLayer);
    virtual bool init();
    
    void loadUI();
    void clickMenu(CCObject* object);
    void removeWnd();
    
    void updateUI();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
private:
    CCSprite* m_spBg;
    CCArray* m_arrCardType;//下方
    CCArray* m_arrWin;//上方
    CCArray* m_arrPailu;//中间
    CCSprite* m_spNew;
    CCSprite* m_spTop1;
    CCSprite* m_spbg1;
};

#endif
