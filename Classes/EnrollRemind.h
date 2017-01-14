//
//  EnrollRemind.hpp
//  CatchFish3
//
//  Created by 王军闯 on 16/5/31.
//
//

#ifndef EnrollRemind_hpp
#define EnrollRemind_hpp

#include <stdio.h>

class CEnrollRemind : public CBASELAYER
{
public:
    static CEnrollRemind* create(std::string sCmptName, std::string sCmpPrize, int nPersonNum, int nEnrollFee, bool bCanEnroll);
    virtual bool init(std::string sCmptName, std::string sCmpPrize, int nPersonNum, int nEnrollFee, bool bCanEnroll);
    void initUI(std::string sCmptName, std::string sCmpPrize, int nPersonNum, int nEnrollFee, bool bCanEnroll);
    void menuCallBack(CCObject* pObj);
    void disappearSchedule(float ft);
    void setJoined(bool bShowJoined);
    void showBisaiMenu();
    
    
private:
    CCMenu* m_pJoinedMenu;
    CCMenu* m_pEnrollMenu;
    CCSprite* pEnrollBg;
};

#endif /* EnrollRemind_hpp */
