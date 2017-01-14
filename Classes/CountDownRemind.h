//
//  CountDownRemind.hpp
//  CatchFish3
//
//  Created by 王军闯 on 16/6/1.
//
//

#ifndef CountDownRemind_hpp
#define CountDownRemind_hpp

#include <stdio.h>

//0为开始之前比赛倒计时  1为比赛结束倒计时

class CCountDownRemind : public CBASELAYER
{
public:
    static CCountDownRemind* create(BYTE cbMatchStatus, int nCount, std::string sMatchTitle);
    virtual bool init(BYTE cbMatchStatus, int nStayTime, std::string sMatchTitle);
    void menuCallBack(CCObject* pObj);
    void disappearSchedule(float ft);
private:
    bool m_cbMatchStatus;
    int m_nCount;
    CCSprite* m_spBg;
};

#endif /* CountDownRemind_hpp */
