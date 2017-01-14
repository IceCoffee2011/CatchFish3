#ifndef __CatchFish__SignInLayer__
#define __CatchFish__SignInLayer__

class CSignInLayer : public CBASELAYER
{
public:
    ~CSignInLayer();
    virtual bool init();
   
    CREATE_FUNC(CSignInLayer);
    void loadUI();
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    void removeWnd();
    void closeWnd(CCObject* object);
    void setSignMark(int nNormalDay, int nVipDay);
    void refreshSignedDay(int nSignInType);
    
    void updateUI();
    
    void setClickTouch();
    
    void showSignInSuccess(int nSignInType);
    void removeSignInSuccess(CCNode* pNode);
    
    BYTE m_cbNormalDay;
    BYTE m_cbVipDay;
    tagSignInDayInfo m_normalDay[7];
    tagSignInDayInfo m_vipDay[7];
private:
    CCArray* m_arrNormal;
    CCArray* m_arrVIP;
    bool m_bClickTouch;
};

#endif /* defined(__CatchFish__SignInLayer__) */
