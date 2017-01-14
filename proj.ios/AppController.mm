#import "AppController.h"
#import "EAGLView.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "WXApi.h"
#import <AlipaySDK/AlipaySDK.h>
#import <TencentOpenAPI/TencentOAuth.h>
#import <AVFoundation/AVFoundation.h>
#include "cocos2d.h"
USING_NS_CC;
//#include "ChargeLayer.h"
#include "header.h"

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;
std::string TouXiangName = "";
bool bDownLoadOwnUserCompleted = false;
int OwnUserID = 0;

int WeiXinCharge = 0;

int ChargeSuccess = 0;
std::string AlipayChargeSuccess = "";

std::string orderIDStr = "";
std::string chargeTypeStr = "";
int nchargeRMB = 0;

/*
 1为微信充值，2为成功，3为充值失败（或者取消）
 4为支付宝充值，5为支付成功，6为支付失败（或者取消）
 */

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:nil];
    [[AVAudioSession sharedInstance] setActive: YES error: nil];
    
    [WXApi registerApp:@"wx51d18d729a87b2b8" withDescription:@"ios_1.0"];
    
    [[TencentOAuth alloc] initWithAppId:@"1104714991" andDelegate:nil];

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    
    // Init the EAGLView
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGB565
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0];

    // Use RootViewController manage EAGLView 
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];
    
    [[UIApplication sharedApplication] setStatusBarHidden:true];
    
    //使屏幕保持常亮状态
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];

    //判断是否允许接收推送通知
    float sysVersion=[[UIDevice currentDevice]systemVersion].floatValue;
    if (sysVersion>=8.0) {
        UIUserNotificationType type=UIUserNotificationTypeBadge | UIUserNotificationTypeAlert | UIUserNotificationTypeSound;
        UIUserNotificationSettings *setting=[UIUserNotificationSettings settingsForTypes:type categories:nil];
        [[UIApplication sharedApplication]registerUserNotificationSettings:setting];
    }

    cocos2d::CCApplication::sharedApplication()->run();

    return YES;
}

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
    printf("aaaaaaa\n");
    if(WeiXinCharge == 1)
    {
        return [WXApi handleOpenURL:url delegate:self];
    }
    else
    {
        return [QQApiInterface handleOpenURL:url delegate:self];
    }
}

- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey, id> *)options NS_AVAILABLE_IOS(9_0) // no equiv. notification. return NO if the application can't open for some reason
{
    printf("bbbbbbbb\n");
    NSLog(@"url is %@", url.host);
    if([url.host isEqualToString:@"com.youkuss.catchfish"])
    {
        if(CMainLogic::sharedMainLogic() != NULL)
        {
            CMainLogic::sharedMainLogic()->synchData();//同步数据
        }
        return true;
    }
    if ([url.host isEqualToString:@"safepay"]) {
        
        [[AlipaySDK defaultService] processOrderWithPaymentResult:url
                                                  standbyCallback:^(NSDictionary *resultDic)
         {
         }];
        
    }
    if([url.host isEqualToString:@"pay"] || [url.host isEqualToString:@"platformId=wechat"])
    {
        return [WXApi handleOpenURL:url delegate:self];
    }
    else if([url.host isEqualToString:@"response_from_qq"])
    {
        return [QQApiInterface handleOpenURL:url delegate:self];
    }
    else
    {
        return true;
    }
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    printf("ccccccc\n");
    NSLog(@"url is %@\n", url.host);
    if([url.host isEqualToString:@"com.youkuss.catchfish"])
    {
        if(CMainLogic::sharedMainLogic() != NULL)
        {
            CMainLogic::sharedMainLogic()->synchData();//同步数据
        }
        return true;
    }
    if ([url.host isEqualToString:@"safepay"]) {
        
        [[AlipaySDK defaultService] processOrderWithPaymentResult:url
                                         standbyCallback:^(NSDictionary *resultDic)
        {
        }];
        
    }
    if([url.host isEqualToString:@"pay"] || [url.host isEqualToString:@"platformId=wechat"])
    {
        return [WXApi handleOpenURL:url delegate:self];
    }
    else if([url.host isEqualToString:@"response_from_qq"])
    {
        return [QQApiInterface handleOpenURL:url delegate:self];
    }
    else
    {
        return true;
    }
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    //application.applicationIconBadgeNumber = 0;//应用程序右上角的数字=0（消失）
    //[[UIApplication sharedApplication] cancelAllLocalNotifications];//取消所有的通知
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    
    cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    application.applicationIconBadgeNumber = 0;//应用程序右上角的数字=0（消失）
    [[UIApplication sharedApplication] cancelAllLocalNotifications];//取消所有的通知
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}

#if __IPAD_OS_VERSION_MAX_ALLOWED >= __IPAD_6_0
- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    return UIInterfaceOrientationMaskAll;
}
#endif



#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}

- (void)dealloc {
    [window release];
    [super dealloc];
}

-(void)onResp:(BaseResp *)resp
{
    printf("555555\n");
    if ([resp isKindOfClass:[PayResp class]]) {
        PayResp *response = (PayResp *)resp;
        switch (response.errCode) {
            case WXSuccess:
            {
                ChargeSuccess = 3;
                printf("微信支付成功\n");
                break;
            }
            default:
            {
                ChargeSuccess = 4;
                printf("微信支付取消或者失败\n");
                break;
            }
        }
        if(CMainLogic::sharedMainLogic()->m_bFirstChargeUIStatus)
        {
            CMainLogic::sharedMainLogic()->notifyIOSWXCompleted();
            return ;
        }
        if(CMainLogic::sharedMainLogic()->m_nBuyGameType == 1)//购买金币
        {
            if(CMainLogic::sharedMainLogic()->m_bGameChargeStatus)//在游戏界面充值
            {
                //返回到大厅
                CMainLogic::sharedMainLogic()->switchLayer(eHallLayer, false, false, 2, 1);
            }
            else
            {
                ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
                if(pChargeLayer != NULL)
                {
                    pChargeLayer -> notifyIOSWXCompleted();
                }
            }
        }
    }
    else if([resp isKindOfClass:[SendMessageToWXResp class]]) {
        //把返回的类型转换成与发送时相对于的返回类型,这里为SendMessageToWXResp
        SendMessageToWXResp *sendResp = (SendMessageToWXResp *)resp;
        switch (sendResp.errCode) {
            case WXSuccess://分享成功
            {
                printf("微信朋友圈分享成功\n");
                CMainLogic::sharedMainLogic()->shareResult(true);
                break;
            }
            case WXErrCodeUserCancel://分享取消
            {
                printf("微信朋友圈分享取消\n");
                CMainLogic::sharedMainLogic()->shareResult(false);
                break;
            }
            case WXErrCodeSentFail://分享失败
            {
                printf("微信朋友圈分享失败\n");
                CMainLogic::sharedMainLogic()->shareResult(false);
                break;
            }
            default://分享失败
            {
                CMainLogic::sharedMainLogic()->shareResult(false);
                break;
            }
        }
        
    }
    else if([resp isKindOfClass:[SendMessageToQQResp class]])
    {
        printf("666666\n");
        SendMessageToQQResp *sendResp = (SendMessageToQQResp *)resp;
        NSLog(@"sendResp.type is %d, sendResp.result is %@", sendResp.type, sendResp.result);
        switch (sendResp.type) {
            case ESENDMESSAGETOQQRESPTYPE://分享结果
            {
                if ([sendResp.result isEqualToString:@"0"]) {
                    //分享成功
                    printf("分享成功\n");
                }
                else {
                    //分享失败或者取消
                    printf("分享失败\n");
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

-(void)onReq:(QQBaseReq *)req
{
    
}

- (void)isOnlineResponse:(NSDictionary *)response
{
    
}

@end
