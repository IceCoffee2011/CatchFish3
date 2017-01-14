#ifndef __MAIN_H__
#define __MAIN_H__

//#define ANYSDK_QH360_000023    //奇虎360
//#define ANYSDK_BD_110000    //百度游戏
//#define ANYSDK_YYB_000550    //腾讯应用宝
#define IOS_ANDROID //苹果、安卓

#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#include <string>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <algorithm>
#endif

#include "CCStdC.h"

#include "cocos2d.h"
#include "cocos-ext.h"

//音效
#include "SimpleAudioEngine.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

#include "CCEGLView.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "Utils.h"
#endif

typedef double DOUBLE;

typedef unsigned char BYTE;
typedef unsigned char * LPBYTE;
typedef char CHAR;
typedef wchar_t WCHAR;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int COLORREF;
typedef int LONG;
typedef unsigned int UINT;
typedef char TCHAR;
typedef long long LONGLONG;
typedef void VOID;
typedef void * LPCVOID;
typedef const char * LPCSTR;
typedef const wchar_t * LPCWSTR;
typedef const char * LPCTSTR;
typedef char * LPTSTR;
//typedef bool BOOL;
typedef unsigned long ULONG;

typedef unsigned int WPARAM;
typedef long LPARAM;
typedef unsigned int HANDLE;
typedef unsigned int HWND;
typedef unsigned int LRESULT;
typedef void COPYDATASTRUCT;

#define MAX_PATH 260
#define NULL 0
#define GAME_PLAYER 4
#define GAME_NIU_PLAYER_100 100     //百人牛牛
//六人捕鱼
#define GAME_NIU_PLAYER 4       //四人牛牛
#define GAME_NIU_PLAYER_2 2     //二人牛牛
#define CLIENT_WIDTH 1280
#define CLIENT_HEIGHT 720

#define       THIRTH_BOUNTRY          500           //5万
#define       TWENTH_BOUNTRY          600           //免费1次
#define       ELEVENTH_BOUNTRY        670           //10万
#define       TENTH_BOUNTRY           740           //20万
#define       NINTH_BOUNTRY           800           //免费2次
#define       EIGHTH_BOUNTRY          850           //50万
#define       SEVENTH_BOUNTRY         900           //100万
#define       SIXTH_BOUNTRY           925           //免费3次
#define       FIVETH_BOUNTRY          950           //200万
#define       FOURTH_BOUNTRY          970           //500万
#define       THIRD_BOUNTRY           985           //免费5次
#define       SECOND_BOUNTRY          995           //1000万
#define       FIRST_BOUNTRY           999           //2000万


#define NOTICE_COUNT                    30



#define       BONUE_COUNT_ROTATION    27.69
#define       BONUE_COUNT             13
#define       BONUE_ROTATION          18

enum extCmpt
{
    eCmptRemindTag = 1777,
    eCmptNotifyTag,
    eCmptStatusTag,
    eCmptSendPrizeTag,
    eGiveCoinLayerTag,
    eDantouInfoTag,
    eGiveCoinLayerChargeTag,
    ePaiNodeTag,
    eOnlinelayerTag=1800,
    eOneMoneyLayerTag,
};

struct SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
};

struct NoticeVct
{
    bool bHave;
    int nID;
    std::string sNotice;
};

struct LabaNotice
{
    std::string sNickName;
    std::string sContent;
};

struct VIPNotice
{
    std::string sNotice;
    std::string sColor;
    std::string sTag;
};

struct MoreGamesInfo
{
    int         nID;
    std::string sIdentifier;
    std::string sPackage;
    std::string sImageName;
    std::string sImageURL;
    std::string sDownLoadURL;
    std::string sIntroduction;
};

//大厅版本，PROCESS_VERSION(11,0,3)
#define VERSION_PLAZA (DWORD)((6<<24)+(11<<16)+(0<<8)+(3))
#define VERSION_FRAME (DWORD)((6<<24)+(6<<16)+(0<<8)+(3))
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define ERROR_CHECK CCLOG("error occurs at %s-%d", __FUNCTION__, __LINE__);
#define ERROR_LOG CCLOG("error occurs at %s-%d", __FUNCTION__, __LINE__);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define ERROR_CHECK printf("error occurs at %s-%d\n", __FUNCTION__, __LINE__);
#define ERROR_LOG printf("error occurs at %s-%d\n", __FUNCTION__, __LINE__);
#endif

//#define PLATFORM_IP "192.168.1.200"  //翔飞
//#define PLATFORM_IP "192.168.1.12" //李哲
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define PLATFORM_IP "120.26.10.66"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define PLATFORM_IP "lele2test.66y.com"//"test2101.66y.com"//

#endif
  
#define NATIVE_TEST     1
//#define NORMAL_URL  "lelewap.66y.com"
#define NORMAL_URL  "lelewaptest.66y.com"

#define JCBY_KIND_ID        2010 //金蟾捕鱼
#define NZNH_KIND_ID        2003 //哪吒闹海
#define YQS_KIND_ID         2006 //摇钱树
#define FB_KIND_ID          2007 //副本游戏
#define CJHD_KIND_ID        2008 //超级海盗
#define PGW_KIND_ID         114  //跑狗王
#define H3CARD_KIND_ID      2100 //红黑大战

#define PLATFORM_LINK           "http://www.4699.com"

#define NOTICE_HEIGHT       720*0.77+40


#define NOTICE_SPEED        50

#include <time.h>

//common
#include "common/Define.h"
#include "common/Packet.h"
#include "common/Struct.h"
#include "common/Property.h"
#include "common/CMD_Commom.h"
#include "common/CMD_LogonServer.h"
#include "common/CMD_GameServer.h"

#include "common/IPC_GameFrame.h"
#include "common/PacketAide.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
#include "Ads.h"
#include "Push.h"
#include "Share.h"
#include "Social.h"
#include "Analytics.h"
#include "const.h"
#include "PluginChannel.h"
#include "ZBase64.h"
using namespace anysdk::framework;
#endif
#if defined(ANYSDK_QH360_000023)
#define CHANNEL_NUM "000023"
#elif defined(ANYSDK_BD_110000)
#define CHANNEL_NUM "110000"
#elif defined(ANYSDK_YYB_000550)
#define CHANNEL_NUM "000550"
#endif

#define BUTTON_EFFECT "Music/button.wav"
#define BUTTON_CLOSE_EFFECT "Music/about.wav"

#include "mp_wrap.h"
#include "MagicScene.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define BUTTON_EFFECT "Music/button.wav"
#define BUTTON_CLOSE_EFFECT "Music/about.wav"
//#define MACHINE "iphone"
//#define MACHINE "wapiPhone"
//#define MACHINE "mwapiPhone"
//#define MACHINE "iosshenma"
#include "../Magic_Particles/mp_wrap.h"
#include "MagicScene.h"
#endif
//net
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "NetworkService.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "carat.h"
#endif

//helper
#include "CharCode.h"
#include "EncryptMD5.h"

#include "ClientKernel.h"

//场景Tag
enum eLayerTag
{
	eBegin=100,
	eLogonLayerTag,
	eHallLayerTag,
    eTableLayerTag,
    eGameMainSceneTag,
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

struct UserInfo
{
	unsigned int nVersion;
	int nUserID;
};
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define CBASELAYER	CBaseLayer
#include "CBaseLayer.h"
#include "curl/curl.h" 
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define CBASELAYER	CCLayer
#include "ChargeWebView.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSiAP_Bridge.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "EnterLayer.h"
#endif

#include "MessageBox.h"
#include "CheckNetworkLayer.h"
#include "NoticeLayer.h"
#include "Laba2Layer.h"
#include "LabaNoticeLayer.h"
#include "ReminderLayer.h"
#include "RedPacketLayer.h"
#include "Guide2Layer.h"
#include "SignInLayer.h"
#include "SettingLayer.h"
#include "RankLayer.h"
#include "ChargeUI.h"
#include "TouXiangLayer.h"
#include "FirstChargeLayer.h"
#include "FreeCoinLayer.h"
#include "MoreGamesLayer.h"
#include "COpenOtherGamesLayers.h"
#include "LotteryLayer.h"
#include "DakasaiNotice.h"
#include "DakasaiLayer.h"
#include "OneMoneyLayer.h"
#include "WufuLayer.h"
#include "ShareLayer.h"
#include "WheelLayer.h"
#include "VIPLayer.h"
#include "MailLayer.h"
#include "BaseData.h"
#include "UpgradeVIPLayer.h"
#include "ChargeLayer.h"
#include "DaojuLayer.h"
#include "FuBenLayer.h"
#include "GetPriseLayer.h"
#include "RankNewGuideLayer.h"
#include "RankNewLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "Charge2Layer.h"
#include "CPicturesLayer.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "uploadFile.h"
#include "PictureLayer.h"
#endif

#include "TableLayer.h"
#include "MainLogic.h"

#include "LogonLayer.h"
#include "HallLayer.h"
#include "AdvertLayer.h"


#include "AppDelegate.h"

#include "../tinyxml/tinyxml.h"
//#include "../jsoncpp/include/json/json.h"
#include "CMD_SuperPirate.h"
#include "SuperGuideLayer.h"
#include "SuperHelpLayer.h"
#include "SuperExtraLayer.h"
#include "GameLayer.h"

#include "CMD_DogKing.h"
#include "DogBeteLayer.h"
#include "DogRunLayer.h"
#include "DogCalcLayer.h"

//红黑大战
#include "CMD_H3Card.h"
#include "H3BetSprite.h"
#include "H3CardSprite.h"
#include "H3JackActionLayer.h"
#include "H3PlayerInfoLayer.h"
#include "H3PlayerListLayer.h"
#include "H3PailuLayer.h"
#include "H3RankLayer.h"
#include "H3CardBeteLayer.h"

#include "CMD_Game.h"

#include "OnlineRewardLayer.h"
#include "XJMath.h"
#include "OBB.h"
#include "GameLine.h"
#include "TryBarrelLayer.h"
#include "GoldSprite.h"
#include "MoneyLayer.h"
#include "BulletSprite.h"
#include "SpriteBatchManager.h"
#include "GameFish.h"
#include "BossBeilv.h"
#include "FishWndLayer.h"

#include "../actions/CCActionInterval.h"
#include "LRActionInterval.h"
#include "BuyBarrelLayer.h"
#include "BandingLayer.h"
#include "BarrelLayer.h"
#include "LoadRes.h"
#include "GiveCoinLayer.h"
#include "NoticeOfHall.h"
#include "BeautfFishDeathAction.h"
#include "SendPrize.h"
#include "EnrollRemind.h"
#include "CmptStatus.h"
#include "CountDownRemind.h"
#include "GameMainScene.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform_cocos.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../Magic_Particles/platform_cocos.h"
#endif


#endif    // __MAIN_H__
