//
//  CCNotificationManager.cpp
//  CatchFish
//
//  Created by 河北腾游 on 15/7/9.
//
//

#include "CCNotificationManager.h"
#include "cocos-ext.h"
#include "cocos2d.h"
USING_NS_CC;
USING_NS_CC_EXT;
//#include "CCTimeHelper.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "CCNotificationHelper.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "CCNotificationAndroid.h"
#endif

CCNotificationManager* CCNotificationManager::m_pNotifiMgr = NULL;

CCNotificationManager::CCNotificationManager()
{
}

CCNotificationManager* CCNotificationManager::getNotificationManager()
{
        if (NULL == m_pNotifiMgr)
        {
            m_pNotifiMgr = new CCNotificationManager();
            return m_pNotifiMgr;
        }
        return m_pNotifiMgr;
}

void CCNotificationManager::notification(const char * message,long delay,int repeats,const char * key)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
      CNH->pushMessage(message ,delay ,repeats,key );
    //    CNH->pushMessage(message ,delay ,0,key );
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
     
        java_push(message,delay + GTH->getCurrentTime(),repeats);
#endif
}
void CCNotificationManager::notification2(const char * message, const char* delay)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CNH->pushMessage2(message ,delay);
    //    CNH->pushMessage(message ,delay ,0,key );
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    java_push(message,delay + GTH->getCurrentTime(),repeats);
#endif
}

void CCNotificationManager::notification3(const char* message, const char* delay)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CNH->pushMessage3(message ,delay);
    //    CNH->pushMessage(message ,delay ,0,key );
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    java_push(message,delay + GTH->getCurrentTime(),repeats);
#endif
}
