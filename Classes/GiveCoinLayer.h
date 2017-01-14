//
//  GiveCoinLayer.h
//  CatchFish
//
//  Created by xiaosong1531 on 15/10/20.
//
//

#ifndef __CatchFish__GiveCoinLayer__
#define __CatchFish__GiveCoinLayer__

class GiveCoinLayer : public CBASELAYER
{
public:
    static GiveCoinLayer* create(int nOpenType = 0, bool bShowFirstCharge = false, std::string sDescription = "", int nScore = 0);
    virtual bool init(int nOpenType = 0, bool bShowFirstCharge = false, std::string sDescription = "", int nScore = 0);
    void loadUI();
    void closeMenu(CCObject* object);
    void clickMenu(CCObject* object);
    void removeWnd();
    
    void loadVIPUI(int nScore);
    
    void loadFirstChargeUI();
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    CC_SYNTHESIZE(int, m_nChargeType, ChargeType);
private:
    CCSprite* m_nodeGiveCoin;
    CCLabelTTF* m_pLabelTTF;
    int m_nOpenType;
    bool m_bShowFirstCharge;
    std::string m_sDescription;
    CCLabelAtlas* m_getCoinAtlas;
};
#endif /* defined(__CatchFish__GiveCoinLayer__) */
