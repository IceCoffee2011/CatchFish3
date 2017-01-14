//
//  CCNotificationHelper.h
//  CatchFish
//
//  Created by 河北腾游 on 15/7/9.
//
//

#ifndef __CatchFish__CCNotificationHelper__
#define __CatchFish__CCNotificationHelper__

class  CCNotificationHelper
{
public:
        static CCNotificationHelper* sharedInstance();
        void pushMessage(const char * message,long delay,int repeats,const char * key);
        void pushMessage2(const char * message,const char* delay);
        void pushMessage3(const char * message,const char* delay);
        void removeNotification();
private:
        CCNotificationHelper();
private:
        static CCNotificationHelper*  m_pNotification;
        bool   m_accept;
};
#define CNH     CCNotificationHelper::sharedInstance()

#endif
