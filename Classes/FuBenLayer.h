#ifndef __CatchFish3__FuBenLayer__
#define __CatchFish3__FuBenLayer__

class FuBenLayer : public CBASELAYER
{
public:
    ~FuBenLayer();
    static FuBenLayer* create(int nOpenType, int nChuji=0, int nZhongji=0, int nGaoji=0, int nLastScore = 0);
    virtual bool init(int nOpenType, int nChuji=0, int nZhongji=0, int nGaoji=0, int nLastScore = 0);
    void loadUI();
    void clickMenu(CCObject* object);
    void clickClose(CCObject* object);
    void removeWnd();
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
private:
    CCSprite* m_spBg;
    int m_nOpenType;
    CCLabelAtlas* m_labelAtlas;
    CCLabelTTF* m_labelScore;
    CCSprite* m_spSelect;
    CCArray* m_arrZhenzhu;
    int m_nChuji;
    int m_nZhongji;
    int m_nGaoji;
    int m_nExchageCount;
    int m_nCurrentType;
    SCORE m_lUserDuiScore;
    
};

#endif /* defined(__CatchFish3__FuBenLayer__) */
