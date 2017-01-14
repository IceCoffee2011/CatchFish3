//
//  FMLayerWebView.cpp
//  WebViewDemo
//
//  Created by 河北腾游 on 15/4/23.
//
//
#include "FMLayerWebView.h"
#include "EncryptMD5.h"
#include "FMUIWebViewBridge.h"
#include "SimpleAudioEngine.h"
#include "header.h"

using namespace CocosDenshion;

#define CHARGE_BANK_URL "http://lelepay.66y.com/Mp_Pay/yeepay/OneClickPayment.aspx"
#define CHARGE_CARD_URL   "http://lelepay.66y.com/Mp_Pay/yeeka/Pay_gcard.aspx"
#define CHARGE_MOBILE_URL "http://lelepay.66y.com/Mp_Pay/yeeka/Pay_mob.aspx"
#define CHARGE_MESSAGE_URL "http://lelepay.66y.com/pay/sms.aspx"
extern std::string Charge1URLstr;
extern std::string Charge2URLstr;


static FMUIWebViewBridge *g_FMUIWebViewBridge=nil;

FMLayerWebView::FMLayerWebView(){
    
}

FMLayerWebView::~FMLayerWebView(){
    
    [g_FMUIWebViewBridge release];
    
}

FMLayerWebView* FMLayerWebView::create(int chargeType, unsigned int userID, const char* name, int money)
{
    FMLayerWebView* pRet = new FMLayerWebView();
    if(pRet && pRet->init(chargeType, userID, name, money))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

FMLayerWebView* FMLayerWebView::create(int userID, int nOpenType)
{
    FMLayerWebView* pRet = new FMLayerWebView();
    if(pRet && pRet->init(userID, nOpenType))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}
FMLayerWebView* FMLayerWebView::create(const char* gameid, const char* account)
{
    FMLayerWebView* pRet = new FMLayerWebView();
    if(pRet && pRet->init(gameid, account))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

void FMLayerWebView::webViewDidFinishLoad(){
    
}

void FMLayerWebView::onBackbuttonClick()
{
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallBankChargeClose);
    SimpleAudioEngine::sharedEngine()->playEffect("Music/button.wav");
    this->removeFromParentAndCleanup(true);
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    CMainLogic::sharedMainLogic()->m_bEnterService = false;
    if(CMainLogic::sharedMainLogic()->m_bBackgroundEnterService)
    {
        CMainLogic::sharedMainLogic()->m_bBackgroundEnterService = false;
        CMainLogic::sharedMainLogic()->backToLogon();
    }
    CMainLogic::sharedMainLogic()->updateCurrentScore();
    //先判断是购买类型（金币、武器、技能）
    if(CMainLogic::sharedMainLogic()->m_nBuyGameType == 1)//购买金币
    {
        if(CMainLogic::sharedMainLogic()->m_bGameChargeStatus)
        {
            CMainLogic::sharedMainLogic()->switchLayer(eHallLayer, false, false, 3, 1);
        }
        else
        {
            ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
            if(pChargeLayer != NULL)
            {
                pChargeLayer -> setWebViewCharge(false);
            }
        }
    }
    
}

bool FMLayerWebView::init(int userID, int nOpenType)
{
    if ( !CCLayer::init() ){
        
        return false;
        
    }
    if(nOpenType == 0)
    {
        char temp2[32];
        sprintf(temp2, "%dbuyu2015",userID);
        char szMD5Result[128];
        CEncryptMD5::toString32(temp2,szMD5Result);
        char tempURL[256];
        sprintf(tempURL, "http://%s/UserService/USRecordInsure2.aspx?uid=%d&num=%s",NORMAL_URL, userID, szMD5Result);
        g_FMUIWebViewBridge = [[FMUIWebViewBridge alloc] init];
        
        [g_FMUIWebViewBridge setLayerWebView:this URLString:tempURL];
        //printf("tempUrl is %s\n", tempURL);
    }
    else if(nOpenType == 1)
    {
        char tempURL[256];
        sprintf(tempURL, "http://%s/RaceRule.htm", NORMAL_URL);
        g_FMUIWebViewBridge = [[FMUIWebViewBridge alloc] init];
        
        [g_FMUIWebViewBridge setLayerWebView:this URLString:tempURL];
    }
    else if(nOpenType == 2)
    {
        char tempURL[256];
        sprintf(tempURL, "http://%s/hdRule.htm", NORMAL_URL);
        g_FMUIWebViewBridge = [[FMUIWebViewBridge alloc] init];
        
        [g_FMUIWebViewBridge setLayerWebView:this URLString:tempURL];
    }
    this -> setTouchEnabled(true);
    
    return true;
}

bool FMLayerWebView::init(const char* gameid, const char* account)
{
    if ( !CCLayer::init() ){
        
        return false;
    }
    char temp[150];
    sprintf(temp, "http://%s/Feedback.aspx?gameid=%s&accname=%s",NORMAL_URL,gameid,account);
    g_FMUIWebViewBridge = [[FMUIWebViewBridge alloc] init];
    
    [g_FMUIWebViewBridge setLayerWebView:this URLString:temp];
    
    //printf("tempUrl is %s\n", temp);
    
    this -> setTouchEnabled(true);
    
    return true;
}
bool FMLayerWebView::init(int chargeType, unsigned int userID, const char* name, int money){
    
    if ( !CCLayer::init() ){
        
        return false;
        
    }
    char tempURL[128];
    if(chargeType == 0)
    {
        if(Charge1URLstr == "")
        {
            sprintf(tempURL, "%s", CHARGE_BANK_URL);
        }
        else
        {
            sprintf(tempURL, "%s/Mp_Pay/yeepay/OneClickPayment.aspx", Charge1URLstr.c_str());
        }
    }
    else if(chargeType == 1)
    {
        if(Charge2URLstr == "")
        {
            sprintf(tempURL, "%s", CHARGE_CARD_URL);
        }
        else
        {
            sprintf(tempURL, "%s/Mp_Pay/yeeka/Pay_gcard.aspx", Charge2URLstr.c_str());
        }
    }
    else if(chargeType == 2)
    {
        if(Charge2URLstr == "")
        {
            sprintf(tempURL, "%s", CHARGE_MOBILE_URL);
        }
        else
        {
            sprintf(tempURL, "%s/Mp_Pay/yeeka/Pay_mob.aspx", Charge2URLstr.c_str());
        }
    }
    else if(chargeType == 3)
    {
        if(Charge1URLstr == "")
        {
            sprintf(tempURL, "%s", CHARGE_MESSAGE_URL);
        }
        else
        {
            sprintf(tempURL, "%s/pay/sms.aspx", Charge1URLstr.c_str());
        }
    }
    //printf("tempURL is %s\n", tempURL);
    char szMD5Result[128];
    
    char temp[256];
    if(chargeType == 0)
    {
        int bFirstCharge = 0;
        int nVipUpgrade = 0;
        ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
        if(pChargeLayer != NULL)
        {
            if(pChargeLayer->m_nOpenType == 10)
            {
                bFirstCharge = 1;
            }
            
            if(pChargeLayer->m_nOpenType == 1)
            {
                nVipUpgrade = 1;
            }
        }
        else
        {
            ChargeUI* pChargeUI = (ChargeUI*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20003);
            if(pChargeUI != NULL)
            {
                bFirstCharge = 0;
                if(pChargeUI->getChargeType() == 1)
                {
                    nVipUpgrade = 1;
                }
            }
        }
        
        char temp3[128];
        sprintf(temp3, "uid=%d&acc=%s&mon=%d&time=%lu&firstCharge=%d&vup=%d", userID, name, money, time(0), bFirstCharge, nVipUpgrade);
        int nType = 1;
        if(GameMainScene::_instance != NULL)
        {
            nType = 2;
        }
        else if(GameLayer::_instance != NULL)
        {
            nType = 3;
        }
        else
        {
            nType = 1;
        }
        CEncryptMD5::toString32(temp3,szMD5Result);
        sprintf(temp, "%s?%s&md=%s&ctype=%d", tempURL, temp3, szMD5Result, nType);
        
    }
    else
    {
        char temp3[128];//uname=&mon=&time=&firstCharge=&num=
        sprintf(temp3, "uid=%d&acc=%s&mon=%d&time=%lu&firstCharge=0", userID, name, money, time(0));
        CEncryptMD5::toString32(temp3,szMD5Result);
        sprintf(temp, "%s?%s&md=%s", tempURL, temp3, szMD5Result);
    }
    
    g_FMUIWebViewBridge = [[FMUIWebViewBridge alloc] init];
    
    [g_FMUIWebViewBridge setLayerWebView:this URLString:temp];
    
    this -> setTouchEnabled(true);
    
    return true;
    
}

FMLayerWebView* FMLayerWebView::create(int nUserID, int nChargeMoney, int nChargeType, int nBuyType, int nShiyong)//最后一个参数为0代表不是试用炮， 为1为试用炮
{
    FMLayerWebView* pRet = new FMLayerWebView();
    if(pRet && pRet->init(nUserID, nChargeMoney, nChargeType, nBuyType, nShiyong))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool FMLayerWebView::init(int nUserID, int nChargeMoney, int nChargeType, int nBuyType, int nShiyong)
{
    if ( !CCLayer::init() ){
        return false;
    }
    
    char temp[256];
    char szMD5Result[128];
    char chargeURL[256];
    int nType = 1;
    if(GameMainScene::_instance != NULL)
    {
        nType = 2;
    }
    else if(GameLayer::_instance != NULL)
    {
        nType = 3;
    }
    else
    {
        nType = 1;
    }
    
    if(nBuyType == 1)//购买技能
    {
        sprintf(temp, "uid=%d&tfee=%d&time=%lu&stype=%d&sflag=%d&vup=0", CMainLogic::sharedMainLogic()->m_nUserID, nChargeMoney, time(0), 1, nChargeType);
        CEncryptMD5::toString32(temp,szMD5Result);
        sprintf(chargeURL, "http://lelepay.66y.com/skpay/yeepay/oneclickpayment.aspx?uid=%d&tfee=%d&time=%lu&stype=%d&sflag=%d&vup=0&info=%s&ctype=%d", CMainLogic::sharedMainLogic()->m_nUserID, nChargeMoney, time(0), 1, nChargeType, szMD5Result, nType);
    }
    else if(nBuyType == 0)//购买武器
    {
        sprintf(temp, "uid=%d&tfee=%d&time=%lu&stype=%d&sflag=%d&vup=%d", CMainLogic::sharedMainLogic()->m_nUserID, nChargeMoney, time(0), 0, nChargeType, nShiyong);
        CEncryptMD5::toString32(temp,szMD5Result);
        sprintf(chargeURL, "http://lelepay.66y.com/skpay/yeepay/oneclickpayment.aspx?uid=%d&tfee=%d&time=%lu&stype=%d&sflag=%d&vup=%d&info=%s&ctype=%d", CMainLogic::sharedMainLogic()->m_nUserID, nChargeMoney, time(0), 0, nChargeType, nShiyong, szMD5Result, nType);
    }
    
    
    g_FMUIWebViewBridge = [[FMUIWebViewBridge alloc] init];
    
    [g_FMUIWebViewBridge setLayerWebView:this URLString:chargeURL];
    
    this -> setTouchEnabled(true);
    
    return true;
}

void FMLayerWebView::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -202, true);
}

bool FMLayerWebView::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}
