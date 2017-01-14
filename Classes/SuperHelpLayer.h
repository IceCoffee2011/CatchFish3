#ifndef __CatchFish3__SuperHelpLayer__
#define __CatchFish3__SuperHelpLayer__

class SuperHelpLayer : public CBASELAYER
{
public:
    CREATE_FUNC(SuperHelpLayer);
    virtual bool init();
    void loadUI();
    void clickMenu(CCObject* object);
    void clickClose(CCObject* object);
    void removeWnd();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
private:
    CCSprite* m_spBg;
    CCSprite* m_spHelp1;
    CCSprite* m_spHelp2;
    CCSprite* m_spHelp3;
    CCSprite* m_spHelp4;
    CCSprite* m_spHelp5;
    int m_nCurrentIndex;
    CCLabelAtlas* m_labelBefore;
    CCLabelAtlas* m_labelNext;
    int m_nStartExtra[4];
};

#endif
