//
//  CCNotificationManager.h
//  CatchFish
//
//  Created by 河北腾游 on 15/7/9.
//
//

#ifndef __CatchFish__CCNotificationManager__
#define __CatchFish__CCNotificationManager__

#define NM CCNotificationManager::getNotificationManager()

class CCNotificationManager
{
public:
    static CCNotificationManager* getNotificationManager();
    void notification(const char * message,long delay,int repeats,const char * key);
    void notification2(const char* message, const char* delay);
    void notification3(const char* message, const char* delay);
private:
    CCNotificationManager();
private:
    static CCNotificationManager* m_pNotifiMgr;
};

#endif /* defined(__CatchFish__CCNotificationManager__) */
