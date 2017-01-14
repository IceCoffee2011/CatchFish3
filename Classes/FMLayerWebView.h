//
//  FMLayerWebView.h
//  WebViewDemo
//
//  Created by 河北腾游 on 15/4/23.
//
//


//

//  FMLayerWebView.h

//  WebViewDemo

//

//  Created by Yanghui Liu on 12-6-5.

//  Copyright (c) 2012年 FMinutes company. All rights reserved.

//

#ifndef WebViewDemo_FMLayerWebView_h

#define WebViewDemo_FMLayerWebView_h


//#include "CCCommon.h"

#include "cocos2d.h"

USING_NS_CC;

class FMLayerWebView : public CCLayer{
    
public:
    
    FMLayerWebView();
    
    ~FMLayerWebView();
    
    virtual bool init(int chargeType, unsigned int userID, const char* name, int money);
    
    virtual bool init(int userID, int nOpenType);
    
    static FMLayerWebView* create(int chargeType, unsigned int userID, const char* name, int money);
    
    //CREATE_FUNC(FMLayerWebView);
    //LAYER_NODE_FUNC(FMLayerWebView);
    
    static FMLayerWebView* create(int userID, int nOpenType);
    
    static FMLayerWebView* create(const char* gameid="0", const char* account="0");
    virtual bool init(const char* gameid="0", const char* account="0");
    void webViewDidFinishLoad();
    
    void onBackbuttonClick();
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    static FMLayerWebView* create(int nUserID, int nChargeMoney, int nChargeType, int nBuyType, int nShiyong = 0);
    virtual bool init(int nUserID, int nChargeMoney, int nChargeType, int nBuyType, int nShiyong = 0);
    
private:
    
    int mWebViewLoadCounter;
    
};

#endif



