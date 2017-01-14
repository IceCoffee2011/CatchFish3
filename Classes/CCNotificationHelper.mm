//
//  CCNotificationHelper.cpp
//  CatchFish
//
//  Created by 河北腾游 on 15/7/9.
//
//

#include "CCNotificationHelper.h"
#import <Foundation/Foundation.h>

#import <UIKit/UIKit.h>

CCNotificationHelper* CCNotificationHelper::m_pNotification = NULL;
CCNotificationHelper* CCNotificationHelper::sharedInstance()
{
    if (!m_pNotification)
    {
        m_pNotification = new CCNotificationHelper();
        return m_pNotification;
    }
    return m_pNotification;
     
}
 
void CCNotificationHelper::pushMessage(const char * message,long delay,int repeats,const char * key)
{
    // repeats  0:不重复 1:/DAY  2:/HOUR 3:/MINUTE 4:SECOND
    NSString * castStr = [[NSString alloc] initWithUTF8String:message];
    NSString * keyStr = [[NSString alloc] initWithUTF8String:key];
    
    //1、增加一个本地推送
    NSDate *date = [NSDate dateWithTimeIntervalSinceNow:delay];
    //创建一个本地推送
    UILocalNotification *noti = [[[UILocalNotification alloc] init] autorelease];
    if (noti)
    {
        //设置推送时间
        noti.fireDate = date;
        //设置时区
        noti.timeZone = [NSTimeZone defaultTimeZone];
        //设置重复间隔
        if(repeats == 1)
        {
            noti.repeatInterval  = kCFCalendarUnitDay;
        }else if (repeats == 2)
        {
            noti.repeatInterval  = kCFCalendarUnitHour;
        }else if (repeats == 3)
        {
            noti.repeatInterval  = kCFCalendarUnitMinute;
        }else if (repeats == 4)
        {
            noti.repeatInterval  = kCFCalendarUnitSecond;
        }else
        {
            noti.repeatInterval  = 0;
        }
        
        //推送声音
        noti.soundName = UILocalNotificationDefaultSoundName;
        //内容
        noti.alertBody = castStr;
        
        //        //显示在icon上的红色圈中的数子
        noti.applicationIconBadgeNumber = 1;
        
        //设置userinfo 方便在之后需要撤销的时候使用
        NSDictionary *infoDic = [NSDictionary dictionaryWithObject:keyStr forKey:keyStr];
        noti.userInfo = infoDic;
        //添加推送到uiapplication
        [[UIApplication sharedApplication] scheduleLocalNotification:noti];
    }
    [castStr release];
    [keyStr release];
}

//每天定时发送
void CCNotificationHelper::pushMessage2(const char * message,const char* delay)
{
    UILocalNotification *notification=[[[UILocalNotification alloc] init] autorelease];
    NSDateFormatter *formatter=[[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"HH:mm:ss"];//还有其他格式,如mm:ss,ss,hh:mm:ss，yyyy-MM-dd HH:mm:ss,HH大写表示24小时计算，小写表示12小时计算，
    NSString* dataStr = [NSString stringWithFormat:@"%s",delay];
    NSDate *now=[formatter dateFromString:dataStr];//设置每天的十二点通知
    notification.fireDate=now;//设置触发时间
    notification.timeZone=[NSTimeZone defaultTimeZone];//设置默认时区，另外也可以写一个时区如:[dateformatter setDateFormat:@"yyyy-MM-dd HH:mm:ss +0800"];//表示东八区
    //如果repeatInterval为零，则表示不重复
    notification.repeatInterval=NSDayCalendarUnit;//设置重复的时间间隔，NSSecondCalendarUnit每秒重复,NSHourCalendarUnit每小时重复,NSDayCalendarUnit每天重复,NSMonthCalendarUnit每月重复
    
    NSString* messageStr = [[NSString alloc] initWithUTF8String:message];
    notification.alertBody=messageStr;//设置提示内容
    notification.soundName=UILocalNotificationDefaultSoundName;//设置默认的提示声音
    notification.applicationIconBadgeNumber=1; //应用程序的右上角小数字
    [formatter release];
    [[UIApplication sharedApplication] scheduleLocalNotification:notification];
    [messageStr release];
}

void CCNotificationHelper::pushMessage3(const char * message,const char* delay)
{
    // repeats  0:不重复 1:/DAY  2:/HOUR 3:/MINUTE 4:SECOND
    NSString * castStr = [[NSString alloc] initWithUTF8String:message];
    
    int delaytime = atoi(delay);
    //1、增加一个本地推送
    NSDate *date = [NSDate dateWithTimeIntervalSinceNow:delaytime];
    //创建一个本地推送
    UILocalNotification *noti = [[[UILocalNotification alloc] init] autorelease];
    if (noti)
    {
        //设置推送时间
        noti.fireDate = date;
        //设置时区
        noti.timeZone = [NSTimeZone defaultTimeZone];
        
        noti.repeatInterval  = 0;
        
        //推送声音
        noti.soundName = UILocalNotificationDefaultSoundName;
        //内容
        noti.alertBody = castStr;
        
        noti.applicationIconBadgeNumber = 1;
        
        //添加推送到uiapplication
        [[UIApplication sharedApplication] scheduleLocalNotification:noti];
    }
    [castStr release];
}

void CCNotificationHelper::removeNotification()
{
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
    [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
}

CCNotificationHelper::CCNotificationHelper()
{
}