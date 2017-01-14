//
//  FMUIWebViewBridge.h
//  WebViewDemo
//
//  Created by 河北腾游 on 15/4/23.
//
//

#ifndef __WebViewDemo__FMUIWebViewBridge__
#define __WebViewDemo__FMUIWebViewBridge__

#import <Foundation/Foundation.h>

#import <CoreLocation/CoreLocation.h>

#import <UIKit/UIKit.h>


#import "FMLayerWebView.h"


@interface FMUIWebViewBridge : NSObject<UIWebViewDelegate,UIAlertViewDelegate>{
    
    FMLayerWebView * mLayerWebView;
    
    UIView    *mView;
    
    UIWebView *mWebView;
    
    UIActivityIndicatorView* activityIndicatorView;
    
    //UIToolbar *mToolbar;
    
    //UIBarButtonItem *mBackButton;
    
}


-(void) setLayerWebView : (FMLayerWebView*) iLayerWebView URLString:(const char*) urlString;

-(void) backClicked:(id)sender;


@end

#endif /* defined(__WebViewDemo__FMUIWebViewBridge__) */
