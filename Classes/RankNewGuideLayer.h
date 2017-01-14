//
//  RankNewGuideLayer.hpp
//  CatchFish3
//
//  Created by Guang on 16/6/2.
//
//

#ifndef RankNewGuideLayer_hpp
#define RankNewGuideLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class CRankNewGuideLayer : public CBASELAYER
{
public:
    virtual bool init();
    CREATE_FUNC(CRankNewGuideLayer);
    
    void setMenuItemVisible(int tag, bool bShow);
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    void callBack(CCObject* obj);
    
    CCNode* m_pGuideStencil;
    
    CCSprite* m_pGuideFirst;
    CCSprite* m_pGuideSecond;
    CCSprite* m_pGuideThird;
    CCSprite* m_pGuideFourth;
    
    CCMenu* m_pMatchGuideMenu;
    
    CCSprite* m_pGuideArrow;
    
    int m_nGuideIndex;
};

#endif /* RankNewGuideLayer_hpp */
