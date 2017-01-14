//
//  SuperExtraLayer.hpp
//  CatchFish3
//
//  Created by Guang on 16/5/25.
//
//

#ifndef SuperExtraLayer_hpp
#define SuperExtraLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class SuperExtraLayer : public CCLayer
{
public:
    static SuperExtraLayer* create(LONGLONG lExtra);
    virtual bool init(LONGLONG lExtra);
    void loadUI(LONGLONG lExtra);
    void removeself();
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
};

#endif /* SuperExtraLayer_h */
