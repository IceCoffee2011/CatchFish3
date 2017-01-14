//
//  SettingLayer.h
//  CatchFish
//
//  Created by ∫”±±Ã⁄”Œ on 15/3/2.
//
//

#ifndef CatchFish_SettingLayer_h
#define CatchFish_SettingLayer_h

class FishWndLayer : public CCLayer
{
public:
    virtual bool init();
    CREATE_FUNC(FishWndLayer);
    void loadH2elp();
    void loadC2alc();
    void closeWnd(CCObject* object);
    void loadHelpSprite();
    void showHelpSprite();
    void loadWndCalcSprite();
    void showWndCalcSprite();
    void updateCalcWnd(int nFishType[], int nFishCount[], int nTotalCount);
    void exitGame(CCObject* object);
    void removeWnd();
    
private:
    CCLayerColor* m_pLayerColor;
    CCLayer* m_containerLayer;
    
    CCSprite * m_pHelpBg;
    CCSprite * m_pCalcBg;
};

#endif
