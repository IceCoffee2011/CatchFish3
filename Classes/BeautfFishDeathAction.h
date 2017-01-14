//
//  beautfFishDeathAction.hpp
//  CatchFish3
//
//  Created by 王军闯 on 16/5/27.
//
//

#ifndef beautfFishDeathAction_hpp
#define beautfFishDeathAction_hpp

#include <stdio.h>

class CBeautfFishDeath : public CBASELAYER
{
public:
    static CBeautfFishDeath* create(int nNum, int nOpenType = 0);
    virtual bool init(int nNum, int nOpenType = 0);
    void createAction(int nNum, int nOpenType = 0);
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    void removeThis();
    void playScoreAction();
    void runToTargetNum(float ft);
    void showBeautifuUI();
private:
    CCLabelAtlas* m_pScoreAtlas;
    int m_nTargetScore;
    int m_nFishScore;
    int m_nOpenType;
};

#endif /* beautfFishDeathAction_hpp */
