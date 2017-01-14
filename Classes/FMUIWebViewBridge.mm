//
//  FMUIWebViewBridge.cpp
//  WebViewDemo
//
//  Created by 河北腾游 on 15/4/23.
//
//

//

//  FMUIWebViewBridge.cpp

//  WebViewDemo

//

//  Created by Yanghui Liu on 12-6-5.

//  Copyright (c) 2012年 FMinutes company. All rights reserved.

//

#import "FMUIWebViewBridge.h"

#import "EAGLView.h"



@implementation FMUIWebViewBridge

- (id)init{
    
    self = [super init];
    
    if (self) {
        
        // init code here.
        
    }
    
    return self;
    
}

- (void)dealloc{
    
    //[mBackButton release];
    
    //[mToolbar release];
    
    [mWebView release];
    
    [mView release];
    
    
    [activityIndicatorView release];
    
    [super dealloc];
    
}

-(void) setLayerWebView : (FMLayerWebView*) iLayerWebView URLString:(const char*) urlString{
    
    mLayerWebView = iLayerWebView;
    
    cocos2d::CCSize size = mLayerWebView-> getContentSize();
    
    CCLog("width is %f, height is %f\n", size.width, size.height);
    
    mView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, size.width , size.height)];
    
    // create webView
    
    //Bottom size
    
    int wBottomMargin = size.height*0.10;
    
    int wWebViewHeight = size.height - wBottomMargin;
    
    CCLog("width is %f, wWebViewHeight is %d", size.width,wWebViewHeight);
    
    int width = [[EAGLView sharedEGLView] size].width;
    int height = [[EAGLView sharedEGLView] size].height;
    
    int rectWidth = (width<height)?height:width;
    int rectHeight = (width<height)?width:height;
    
    mWebView = [[UIWebView alloc] initWithFrame:CGRectMake(0, 0, rectWidth, rectHeight)];
    
    //mWebView = [[UIWebView alloc] initWithFrame:CGRectMake(0, 0, 480, 320)];
    
    //[mWebView setBackgroundColor:[UIColor clearColor]];
    mWebView.delegate = self;
    
    NSString *urlBase = [NSString stringWithCString:urlString encoding:NSUTF8StringEncoding];
    
    [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:urlBase ]]];
    
    [mWebView setUserInteractionEnabled:NO]; //don't let the user scroll while things are
    
    //create a tool bar for the bottom of the screen to hold the back button
    /*
    mToolbar = [UIToolbar new];
    
    [mToolbar setFrame:CGRectMake(0, 0, 70, 40)];
    
    mToolbar.barStyle = UIBarStyleBlackOpaque;
    
    //Create a button
    
    mBackButton = [[UIBarButtonItem alloc] initWithTitle:@"返回"
                   
                                                   style: UIBarButtonItemStyleDone
                   
                                                  target: self
                   
                                                  action:@selector(backClicked:)];
    
    //[mToolbar setBounds:CGRectMake(0.0, 0.0, 95.0, 34.0)];
    
    [mToolbar setItems:[NSArray arrayWithObjects:mBackButton,nil] animated:YES];
    
    [mView addSubview:mToolbar];
    
    //[mToolbar release];
    */
    
    
    
    // add the webView to the view
    
    [mView addSubview:mWebView];
    
    UIButton* button = [[[UIButton alloc] initWithFrame:CGRectMake(0, 0, 52, 66)] autorelease];
    [button setTitleColor:[UIColor redColor] forState:UIControlStateNormal];
    [button addTarget:self action:@selector(backClicked:) forControlEvents:UIControlEventTouchDown];
    [button setImage:[UIImage imageNamed:@"h2all/backItem.png"] forState:UIControlStateNormal];
    [mView addSubview:button];
    
    
    activityIndicatorView = [[UIActivityIndicatorView  alloc]
                             initWithFrame:CGRectMake(260.0,150.0,30.0,30.0)];
    activityIndicatorView.activityIndicatorViewStyle= UIActivityIndicatorViewStyleGray;
    [activityIndicatorView startAnimating];//启动
    activityIndicatorView.hidesWhenStopped = YES;
    
    [mView addSubview:activityIndicatorView];
    
    [[EAGLView sharedEGLView] addSubview:mView];
    NSLog(@"size width is %f, height is %f", [[EAGLView sharedEGLView] size].width, [[EAGLView sharedEGLView] size].height);
    
}


- (void)webViewDidStartLoad:(UIWebView *)thisWebView {
    
}

- (void)webViewDidFinishLoad:(UIWebView *)thisWebView{
    
    [mWebView setUserInteractionEnabled:YES];

    [activityIndicatorView stopAnimating];
    
    
    mLayerWebView->webViewDidFinishLoad();
    
}


- (void)webView:(UIWebView *)thisWebView didFailLoadWithError:(NSError *)error {
    
    if ([error code] != -999 && error != NULL) {
        //error -999 happens when the user clicks on something before it's done loading.
        
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Network Error"
                                                        message:@"Unable to load the page. Please keep network connection."
                              
                                                       delegate:self cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        
        [alert show];
        
        [alert release];
        
        
    }
    
}


-(void) backClicked:(id)sender {
    
    mWebView.delegate = nil; //keep the webview from firing off any extra messages
    
    //remove items from the Superview...just to make sure they're gone
    
    //[mToolbar removeFromSuperview];
    
    [mWebView removeFromSuperview];
    
    [mView removeFromSuperview];
    
    mLayerWebView->onBackbuttonClick();
    
}

@end
