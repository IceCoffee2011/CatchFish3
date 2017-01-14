//
//  BandingLayer.hpp
//  CatchFish3
//
//  Created by Guang on 16/4/7.
//
//

#ifndef BandingLayer_hpp
#define BandingLayer_hpp

#include <stdio.h>

class BandingLayer : public CBASELAYER {
public:
    virtual bool init();
    CREATE_FUNC(BandingLayer);
    void buttonCallBack(CCObject* object);
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    void removeWnd();
    
    CCSprite* m_pBg;
};

#endif /* BandingLayer_hpp */
