//
//  ReminderLayer.hpp
//  CatchFish3
//
//  Created by Guang on 16/5/11.
//
//

#ifndef ReminderLayer_hpp
#define ReminderLayer_hpp

#include <stdio.h>

class CReminderLayer : public CBASELAYER
{
public:
    virtual bool init(int nType);
    static CReminderLayer* create(int nType);
    
    void loadRemindUI(int nType);
    
    void clickMenu(CCObject* obj);
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
};

#endif /* ReminderLayer_hpp */
