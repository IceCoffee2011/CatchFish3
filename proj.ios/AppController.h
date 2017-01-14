#import <UIKit/UIKit.h>
#import "WXApi.h"
#import <TencentOpenAPI/QQApiInterface.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, WXApiDelegate, QQApiInterfaceDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
}

@end

