//
//  ChargeWebView.h
//  CatchFish
//
//  Created by 河北腾游 on 15/3/7.
//
//

#ifndef __CatchFish__ChargeWebView__
#define __CatchFish__ChargeWebView__

#include <string>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;


class ChargeWebView : public CCNode
{
public:
    ChargeWebView();
    static void linkToFull(const char* gameId);//充值
    static void lickToFeedback(const char* gameId="0", const char* account="0");//反馈
    static void linkToDownLoad();
    static void linkToNiuDownLoad();
    static std::string getUIDevice();
    static std::string getDeviceInfo(int num);
    static std::string getIOSIDFA();
    static std::string getIOSMAC();
    static std::string getIOSUUID();
    //支付宝充值
    static void alipayCharge(const char* orderStr, int prise, int nBuyGameType);
    //微信充值
    static void wxCharge(std::string appid, std::string partnerId,std::string prepayId,std::string nonceStr,int timeStamp,std::string package,std::string sign);
	
    static bool findUserIDFile(const char* UserID);
    std::string findUserVersionFile(int UserID, int nVersion, int TableID, int SeatID);
    void onHttpUserIDComplete(CCHttpClient* sender, CCHttpResponse* response);
    int wTableID;
    int wSeatID;
    int m_nUserID;
    bool bDownloadComplete;
    std::string sFileName;
    
    static void writeFile(std::string str);
    
    static bool canExitFishGame(std::string str);
    static void openFishGame(std::string str);
    static void linkToMoreGamesDownLoad(std::string str);
    
    static void openAppStore();
    static std::string OnInternet();
    
    static long long getCurrentTime();
    static std::string getCurrentTimes();
    
    static void openIOSShare(std::string str1, std::string str2, int nType);
    static bool checkWX();
    
    static void openQQShare(std::string str1, std::string str2, int nType);
    static bool checkQQ();
    
    static void copy(std::string str);
    static std::string checkPaste();//获取剪切板内容
};

class SandBox
{
public:
    static std::string findUserFiles(int UserID);
};

#endif /* defined(__CatchFish__ChargeWebView__) */
