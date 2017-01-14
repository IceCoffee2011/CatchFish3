//
//  SettingLayer.h
//  CatchFish
//
//  Created by 河北腾游 on 15/3/12.
//
//

#ifndef __CatchFish__SettingLayer__
#define __CatchFish__SettingLayer__

class SettingLayer : public CCLayer
{
public:
    SettingLayer();
    ~SettingLayer();
    virtual bool init();
    CREATE_FUNC(SettingLayer);
    void sliderValueChanged(CCObject* sender, CCControlEvent controlEvent);
    void switchValueChanged(CCObject* sender, CCControlEvent controlEvent);
    void closeWnd(CCObject * obj);
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    void showSettingWnd();
    
    void removeWnd();
    
    void setEffectValume();
    void openTime();
    
    CCSize vSize;
    
    CCSprite * m_nodeRegister;
    CCControlSlider * yinyueSlider;
    CCControlSlider * effortSlider;
    bool m_bShadow;
    bool m_bParticle;
    float m_fEffect;
    float m_fOldEffect;
    float m_fSound;
};

#endif /* defined(__CatchFish__SettingLayer__) */
