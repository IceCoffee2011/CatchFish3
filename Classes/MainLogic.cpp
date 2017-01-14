#include "header.h"



#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#define MACHINE "guanwang"
//#define MACHINE "fenxiang"
//#define MACHINE "toutiao1"
//#define MACHINE "toutiao2"
//#define MACHINE "toutiao3"
//#define MACHINE "toutiao4"
//#define MACHINE "toutiao5"
//#define MACHINE "toutiao6"
//#define MACHINE "toutiao7"
//#define MACHINE "toutiao8"
//#define MACHINE "toutiao9"
//#define MACHINE "toutiao10"
//#define MACHINE "toutiao11"
//#define MACHINE "toutiao12"
//#define MACHINE "toutiao13"
//#define MACHINE "toutiao14"
//#define MACHINE "toutiao15"
//#define MACHINE "toutiao16"
//#define MACHINE "toutiao17"
//#define MACHINE "toutiao18"
//#define MACHINE "toutiao19"
//#define MACHINE "toutiao20"
//#define MACHINE "toutiao00"
//#define MACHINE "toutiao01"
//#define MACHINE "toutiao02"
//#define MACHINE "toutiao03"
//#define MACHINE "toutiao04"
//#define MACHINE "toutiao05"
//#define MACHINE "toutiao06"
//#define MACHINE "toutiao07"
//#define MACHINE "toutiao08"
//#define MACHINE "toutiao09"
//#define MACHINE "toutiao010"
//#define MACHINE "toutiao011"
//#define MACHINE "toutiao012"
//#define MACHINE "toutiao013"
//#define MACHINE "toutiao014"
//#define MACHINE "toutiao015"
//#define MACHINE "toutiao016"
//#define MACHINE "toutiao017"
//#define MACHINE "toutiao018"
//#define MACHINE "toutiao019"
//#define MACHINE "toutiao020"
//#define MACHINE "toutiao021"
//#define MACHINE "toutiao022"
//#define MACHINE "toutiao023"
//#define MACHINE "toutiao024"
//#define MACHINE "toutiao025"
//#define MACHINE "toutiao026"
//#define MACHINE "toutiao027"
//#define MACHINE "toutiao028"
//#define MACHINE "toutiao029"
//#define MACHINE "toutiao030"
//#define MACHINE "dkbdtg001"
//#define MACHINE "dkbdtg002"
//#define MACHINE "dkbdtg003"
//#define MACHINE "dkbdtg004"
//#define MACHINE "dksmtg001"
//#define MACHINE "dksmtg002"
//#define MACHINE "dksgtg001"
//#define MACHINE "dksgtg002"
//#define MACHINE "dktoutiao"
//#define MACHINE "dktoutiao1"
//#define MACHINE "dktoutiao2"
//#define MACHINE "dktoutiao3"
//#define MACHINE "dktoutiao4"
//#define MACHINE "dktoutiao5"
//#define MACHINE "dktoutiao6"
//#define MACHINE "dktoutiao7"
//#define MACHINE "dktoutiao8"
//#define MACHINE "dktoutiao9"
//#define MACHINE "dktoutiao10"
//#define MACHINE "dktoutiao11"
//#define MACHINE "dktoutiao12"
//#define MACHINE "dktoutiao13"
//#define MACHINE "dktoutiao14"
//#define MACHINE "dktoutiao15"
//#define MACHINE "dktoutiao00"
//#define MACHINE "dktoutiao01"
//#define MACHINE "dktoutiao02"
//#define MACHINE "dktoutiao03"
//#define MACHINE "dktoutiao04"
//#define MACHINE "dktoutiao05"
//#define MACHINE "dktoutiao06"
//#define MACHINE "dktoutiao07"
//#define MACHINE "dktoutiao08"
//#define MACHINE "dktoutiao09"
//#define MACHINE "dktoutiao010"
//#define MACHINE "dktoutiao011"
//#define MACHINE "dktoutiao012"
//#define MACHINE "dktoutiao013"
//#define MACHINE "dktoutiao014"
//#define MACHINE "dktoutiao015"
//#define MACHINE "wangyi01"
//#define MACHINE "wangyi02"
//#define MACHINE "wangyi03"
//#define MACHINE "wangyi04"
#define MACHINE "wangyi05"
//#define MACHINE "zhihuitui1"
//#define MACHINE "zhihuitui2"
//#define MACHINE "zhihuitui3"
//#define MACHINE "zhihuitui4"
//#define MACHINE "zhihuitui5"
//#define MACHINE "zhihuitui00"
//#define MACHINE "zhihuitui01"
//#define MACHINE "zhihuitui02"
//#define MACHINE "zhihuitui03"
//#define MACHINE "zhihuitui04"
//#define MACHINE "zhihuitui05"
//#define MACHINE "toutiao001"
//#define MACHINE "toutiao002"
//#define MACHINE "toutiao003"
//#define MACHINE "toutiao004"
//#define MACHINE "toutiao005"
#include "sys/utsname.h"
#include <sys/socket.h> // Per msqr
extern int nWxChargeResult;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "CCNotificationManager.h"
#define MACHINE "iosappstore"
extern int ChargeSuccess;
extern std::string AlipayChargeSuccess;
extern int WeiXinCharge;
#endif

#define VERSION_ANDROID     "android_1.9"
#define VERSION_IOS         "ios_1.9"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
    //方法名与java类中的包名+方法名，以下划线连接
    void Java_cn_sharesdk_hellocpp_HelloCpp_WXShareResult(JNIEnv*  env, jobject thiz, jint a)
    {
        CMainLogic::sharedMainLogic()->shareResult(a);
    }
}
#endif

std::string Charge1URLstr = "";
std::string Charge2URLstr = "";

CMainLogic * CMainLogic::_instance = NULL;

bool getSwitchByLogon(int m, int n)
{
    return (m & (1 << (n-1)));
}

CMainLogic::CMainLogic()
{
    _instance = this;
    m_vctNotification.clear();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void CMainLogic::sendRoomExit()
{
    if(GameMainScene::_instance != NULL)
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey("UserScore", GameMainScene::_instance->getUserCurrentScore());
        GameMainScene::_instance->exitGame();
        GameMainScene::_instance->exitMainGame();
        SendData(m_nRoomSocketHandle, MDM_KN_COMMAND, SUB_KN_SHUT_MANUAL);
    }
    else if(GameLayer::_instance != NULL)
    {
        GameLayer::_instance->exitGame();
        SendData(m_nRoomSocketHandle, MDM_KN_COMMAND, SUB_KN_SHUT_MANUAL);
    }
}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void CMainLogic::sendAndroidRoomExit()
{
    if(GameMainScene::_instance != NULL)
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey("UserScore", GameMainScene::_instance->getUserCurrentScore());
        GameMainScene::_instance->exitGame();
        GameMainScene::_instance->exitMainGame();
        SendData(m_pNetworkService->GetNetInfo(m_nHallSocketHandle), MDM_KN_COMMAND, SUB_KN_SHUT_MANUAL);
    }
    else if(GameLayer::_instance != NULL)
    {
        GameLayer::_instance->exitGame();
        SendData(m_pNetworkService->GetNetInfo(m_nHallSocketHandle), MDM_KN_COMMAND, SUB_KN_SHUT_MANUAL);
    }
}
#endif

CMainLogic::~CMainLogic()
{
    if(getPlaying())
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        sendRoomExit();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        sendAndroidRoomExit();
#endif
    }
    if (m_pMessageBox)
    {
        delete m_pMessageBox;
        m_pMessageBox = NULL;
    }
    if(m_pNoticeLayer)
    {
        delete m_pNoticeLayer;
        m_pNoticeLayer = NULL;
    }
    if(m_pLabaNoticeLayer)
    {
        delete m_pLabaNoticeLayer;
        m_pLabaNoticeLayer = NULL;
    }
    if(m_pDakasaiNotice)
    {
        delete m_pDakasaiNotice;
        m_pDakasaiNotice = NULL;
    }
    if(m_pLaba2Layer)
    {
        delete m_pLaba2Layer;
        m_pLaba2Layer = NULL;
    }
    CCArmatureDataManager::sharedArmatureDataManager()->purge();
    DictionaryHelper::shareHelper()->purgeDictionaryHelper();
    
    if (m_pNetworkService)
    {
        delete m_pNetworkService;
        m_pNetworkService = NULL;
    }
    for(int i = 0; i < m_vctNotification.size(); i++)
    {
        delete m_vctNotification[i];
    }
    m_vctNotification.clear();
}

void CMainLogic::synchData(float dt)
{
    //在这里判断是发送登录包还是发送分数请求，根据UserID是否为0来区分，如果UserID为0，那么就判断为第一次登录，发送登录包，如果UserID不为0，那么就发送请求分数、奖券数量、VIP信息
    if(dt == 1.0f)
    {
        ShowMessage("正在连接服务器请稍候...", eMBNull);
    }
    
    if(m_nUserID != 0)//发送登录包
    {
        if(m_pNetworkService != NULL)
        {
            //向登录服务器发送检测包
            m_nCheckSocketShut = 10;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            SendData(m_pNetworkService->GetNetInfo(m_nHallSocketHandle), MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			SendData(m_nHallSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#endif
			CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
            pScheduler->scheduleSelector(schedule_selector(CMainLogic::receiveHallLinkInfo), this, 1.5f, 0, 0, false);
        }
    }
}

void CMainLogic::receiveHallLinkInfo(float dt)
{
    if(m_pNetworkService != NULL)
    {
        //没有收到大厅的回应，玩家已经掉线，那么重新连接
        m_nClickBank = 10;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CNetInfo * pNetInfo = NULL;
        if (!NATIVE_TEST)
        {
            pNetInfo = m_pNetworkService->Connect(m_sNodeIP.c_str(), PORT_LOGON, NULL, NULL, true);
        }
        else
        {
            pNetInfo = m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON, NULL, NULL, false);
        }
        m_nHallSocketHandle = m_pNetworkService->GetNetID(pNetInfo);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		if (!NATIVE_TEST)
        {
            m_nHallSocketHandle = m_pNetworkService->Connect(m_sNodeIP.c_str(), PORT_LOGON);
        }
        else
        {
            m_nHallSocketHandle = m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON);
        }
#endif
    }
}

void CMainLogic::sendExchangeComplete()
{
    CMD_GPR_CurrentScore CurrentScore;
    CurrentScore.dwUserID = m_nUserID;
    SendData(m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_CURRENT_SCORE,&CurrentScore,sizeof(CurrentScore));
}

bool CMainLogic::init()
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    m_pNetworkService=new CNetworkService;
    m_pNetworkService->Init();
    m_pNetworkService->SetNetworkServiceSink(this);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	signal(SIGPIPE, func);
	m_pNetworkService=new CaratManager;
    m_pNetworkService->SetCaratSink(this);
#endif
    
    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
    pScheduler->scheduleSelector(schedule_selector(CMainLogic::Tick), this, 0.1f, false);
    
    m_nLogonType=eLogon;
    
    for(int i = 0; i < 3; i++)
    {
        m_nSkillCount[i] = 9;
    }
    
    m_vRoomNZNH.clear();
    m_vRoomJCBY.clear();
    m_vRoomLKPY.clear();
    m_vRoomPGW.clear();
    m_vRoomRB.clear();
    m_vRoomPirate.clear();
    m_vRoomFB.clear();
    m_vUserInfo.clear();
    m_vGameType.clear();
    m_vctNotice.clear();
    m_vctMail.clear();
    m_vctVipExpConfig.clear();
    m_vctVIPNotice.clear();
    m_vctLaba2Notice.clear();
    m_vctLabaNotice.clear();
    m_vMoreGamesInfo.clear();
    m_vNewAdvertInfo.clear();
#if defined(IOS_ANDROID)
    m_bAnySDKPassed = true;
#else
    m_bAnySDKPassed = false;//anysdk审核是否通过
#endif
    m_bReceiveVipLogon = true;
    m_bIsShowLogonGift = false;
    m_bHaveAdvert = false;
    m_bEnterService = false;
    m_bBackgroundEnterService = false;
    m_bCanBandingTel = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    m_sMobileProduct = "";
    m_bOpenPhoto = false;
    m_bExitGame = false;
    getMobileProduct();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_sPublicIP = "";
#endif
    m_pMessageBox = new CMessageBox();
    m_pNoticeLayer = new NoticeLayer();
    m_pLabaNoticeLayer = new LabaNoticeLayer(565);
    m_pLaba2Layer = new Laba2Layer(565);
    m_pDakasaiNotice = new DakasaiNotice(565);
    m_pTableScene = NULL;
    m_pLogonScene = NULL;
    m_pHallScene = NULL;
    m_pCheckNetworkLayer = NULL;
    
    m_bAutoExitGame = false;
    m_bGameDisConnect = true;
    bPlaying = false;
    m_bLogonSuccess = false;
	m_bChargeSuccess = false;
    bIsSignIn = false;
    m_bSharedState = false;
    m_bChargeState = false;
    m_bGameToVIP = false;
    m_bFirstCharge = false;
    m_bFirstLogon = false;
    m_bGameExitHall = false;
    m_bDisConnect = false;
    m_b2DisConnect = false;
    m_bFirstRequestMail = false;
    m_bClickIcon = true;
    m_bEnterGiveCoin = false;
    m_bIsOpenLogon = false;
    m_bReceiveHallLink = false;
    m_bStartOpenCheck = false;
    m_bRoomHaveLottery = false;
    m_bFirstChargeUIStatus = false;
    m_bCanShare = true;
    m_bShowGuide = false;
    m_bNeedLogon = false;
    m_bGameChargeStatus = false;
	m_bNetPageCharge = false;
    m_bBuyTryCannon = false;
    m_bShowTryCannon = false;
    m_bEnterLoading = false;
    m_bNetCanStatus = false;
    m_bBYToSuperPirse = false;
    m_bSuperToBY = false;
    m_bShowNoticeLayer = false;
    m_bShowFuben = false;
    m_bReceiveShowWheel = false;
    m_bShowWheelNoSign = false;
    m_nBuyCannonIndex = 0;
	m_nShareInterface = 0;
    m_nBuyCannon2Index = 0;
    m_nBuyGameType = 1;
    m_nShowChargeType = 0;
    m_nShowCoinArmSkill = 0;
    m_nExitGameToSpecialArm = 0;
    m_nGiveScore = 0;
    m_nBankruptcyCount = 0;
    m_cbCanGetBankruptcy = 0;
    m_n2DisConnect = 7;
    m_nMessageParam = 0;
    m_nPortTelephone = 0;
    m_nReadingMailID = 0;
    m_cbMemberOrder = 0;
    m_nPrise = 0;
    m_llChargeCount = 0;
    m_nLottery = 0;
    m_nHallSocketHandle = 0;
    m_nVipLevel = 0;
    m_nVipExp = 0;
    m_nGameType = 0;
    m_nExpPercent = 0;
    m_nLogonGift = 0;
    m_nVipLogonGift = 0;
    m_nChargeBack = 0;
    m_nCurrentInstruct = 0;
    m_nShowGuideScore = 0;
    m_cbCountTimes = 0;
    m_nClickBank = 0;
    m_nUserID = 0;
    m_nCheckSocketShut = 0;
    m_nCurrentViewIndex = 0;
    m_nMailCount = 0;
    m_nRegisterType = 0;
    m_nSpeedUpTimes = 0;
    m_nSansheTimes = 0;
    m_nBaojiTimes = 0;
    m_nCannonStatus = 0;
    m_nSuperPirateItemCount = 0;
    m_lFirstTime = 0;
    m_lSecondTime = 0;
    m_nGameRoomType = 0;
    m_bCanGetPiratePrise = 0;
    m_lSuperPirateWard = 9999999999;
    m_fNoticeSpeed = 200;
    m_nBindReward = 0;
    m_nShareReward = 0;
    m_lUserScore = 0;
    m_cbSuperPirateGuided = true;
    m_sServiceQQ = "";
    m_sChargeURL = "";
    m_sChargeCardURL = "";
    Charge1URLstr = "";
    Charge2URLstr = "";
    m_sChargeNotice = "";
    m_sTelephone = "";
    m_sVerificationCode = "";
    m_sAlipaySign = "";
    m_sWXOrderID = "";
    m_sModifyPassword = "";
    m_sOrderID = "";
    m_sMachine = CCUserDefault::sharedUserDefault()->getStringForKey("MACHINE", "iosappstore");
    m_bClickMessageCharge = false;
    m_bFirstChargeCanGet = false;
    m_bForeGround = false;
    m_bShowNiuGame = false;//显示更多游戏按钮
    m_bShowChouJiang = false;
    m_bLogonStatus = true;
    m_bBYGameToSuper = false;
    m_bCanEnterPiraseFreeCoin = false;
    m_bAutoCheckOnlineInfo = true;
    m_nPirasePrise = 0;
    m_nGameToHallVipLevel = 1;

    m_sAndroidMachine = MACHINE;

    
    for(int i = 0; i < 10; i++)
    {
        m_cbWheelType[i] = 0;
        m_nWheelCount[i] = 0;
    }
    m_bCanOneCharge = false;
    m_nOneCoinCount = 0;
    m_cbOneHuafei = 0;
    m_cbOneSpeedCount = 0;
    m_cbOneSuodingCount = 0;
    
    m_bShowNotifyInfo = CCUserDefault::sharedUserDefault()->getBoolForKey("ShowBisaiNotifyInfo", true);
    
    m_vctMatchGroupInfo.clear();
    m_vctOnlineRewardInfo.clear();
    
    m_bShowMatch = false;//比赛开关
    m_bShowZhouMatch = false;//周赛开关
    m_bShowBeibao = false;//背包开关
    m_bShowChat = false;//聊天开关
    m_bShowOnlineCoin = false;//在线奖励开关
    m_bShowOneCharge = false;//1元购开关
    m_bShowShareInfo = false;//分享返利按钮开关
    m_nUserWeekRank = 0;
    m_nUserWeekBeiNum = 0;
    m_bShowAdvert = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_bIOSPassed = true;
    m_bAndroidOpenStatus = true;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_bIOSPassed = true;
    m_bAndroidOpenStatus = false;
#endif
    
    CCHttpClient::getInstance()->setTimeoutForConnect(10);
    CCHttpClient::getInstance()->setTimeoutForRead(10);
    // run
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
    m_pLogonScene = CLogonLayer::scene();
    CCDirector::sharedDirector()->runWithScene(m_pLogonScene);
	//init anysdk
    PluginChannel::getInstance()->loadPlugins();
#else
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_pHallScene = CHallLayer::scene();
//    m_pLogonScene = CLogonLayer::scene();
//    CCDirector::sharedDirector()->runWithScene(m_pLogonScene);
	CCDirector::sharedDirector()->runWithScene(m_pHallScene);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCScene* scene = EnterLayer::scene();
    CCDirector::sharedDirector()->runWithScene(scene);
#endif
#endif
    //请求苹果下的渠道
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(CCUserDefault::sharedUserDefault()->getBoolForKey("FirstRequestChannel"), true)
    {
        requestIOSChannel();
    }
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(1)//头条包隐藏海盗
    {
        m_bAndroidOpenStatus = true;
    }
    else
    {
        m_bAndroidOpenStatus =CCUserDefault::sharedUserDefault()->getBoolForKey("AndroidOpenStatusTouTiao", false);
        if(!m_bAndroidOpenStatus)
        {
            requestAndroidOpenStatus();
        }
        
    }
#endif

    requestChargeNotice();
    
    //请求广告框
    requestAdvertXML();
    
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
	initConnect();
#endif 
    
    return true;
}

void getLogonUrl2(DWORD dwBaseData, char szServerName[32])
{
//    const char* nNumber[] = {
//        "d23482", "d23683", "d34694", "d34891", "d88665", "e23482", "e23683", "e34694", "e34891", "e88665"
//    };
    //lelelogin01.66y.com//a20161123.66y.com
    const char* nNumber[] = {
        "l58846", "l36561", "l25741", "l15684", "l36461", "l36741", "l98714", "l32199", "l87955", "l32548"
    };
    int nNodeIndex = dwBaseData % 10;
    sprintf(szServerName, "%s", nNumber[nNodeIndex]);
    std::string str = szServerName;
    str += '.';
    str += '6';
    str += '6';
    str += 'y';
    str += '.';
    str += 'c';
    str += 'o';
    str += 'm';
    sprintf(szServerName, "%s", str.c_str());
}
//刚启动时连接服务器
void CMainLogic::initConnect()
{
    m_bNetCanStatus = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(CCUserDefault::sharedUserDefault()->getStringForKey("NEWSPECIALNODE") != "")//专业节点
    {
     	CMainLogic::sharedMainLogic()->m_sNodeIP = CCUserDefault::sharedUserDefault()->getStringForKey("NEWSPECIALNODE").c_str();
    }
     else 
    {
        JniMethodInfo info;
        bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
        jobject jobj;
        if(bHave)
        {
            jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
            bool bHave2 = false;
            bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","getUIDevice1", "(I)Ljava/lang/String;");
            if(bHave2)
            {
                jstring jstr4 = (jstring)info.env->CallObjectMethod(jobj, info.methodID, 4);
                std::string machineStr = JniHelper::jstring2string(jstr4);	//生产日期
                
                long long imeiNum = atoll(machineStr.c_str());
                
                int onSignDay = imeiNum/1000;//取秒数，最后一位
                
                onSignDay = onSignDay%10;
                
                char szServerName[32];
                getLogonUrl2(onSignDay, szServerName);
                m_sNodeIP = szServerName;
            }
        }
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
     if(CCUserDefault::sharedUserDefault()->getStringForKey("NEWSPECIALNODE") != "")//专业节点
     {
         CMainLogic::sharedMainLogic()->m_sNodeIP = CCUserDefault::sharedUserDefault()->getStringForKey("NEWSPECIALNODE").c_str();
     }
     else if(CMainLogic::sharedMainLogic()->m_sPublicIP == "")
     {
         std::string machineStr = ChargeWebView::getUIDevice();
         char szMachineID[LEN_MACHINE_ID];
         memcpy(szMachineID, machineStr.c_str(), LEN_MACHINE_ID);
         unsigned int onSignDay = 0;
         sscanf(&szMachineID[LEN_MACHINE_ID-2], "%x", &onSignDay);
         char szServerName[32];
         onSignDay = onSignDay%10;
         getLogonUrl2(onSignDay, szServerName);
         m_sNodeIP = szServerName;//"a20161123.66y.com";//
     }
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    bool bSpecialNodeIP = CCUserDefault::sharedUserDefault()->getBoolForKey("NEWSPECIALNODEIP", true);//默认第一次使用域名登录
    CNetInfo * pNetInfo = NULL;
    if (!NATIVE_TEST)
    {
        pNetInfo = m_pNetworkService->Connect(m_sNodeIP.c_str(), PORT_LOGON, NULL, NULL, bSpecialNodeIP);//正常运营
    }
    else
    {
        pNetInfo = m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON, NULL, NULL, false);//IP测试
    }
    if(pNetInfo != NULL)
    {
        m_nHallSocketHandle = m_pNetworkService->GetNetID(pNetInfo);
    }
    
    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
    pScheduler->scheduleSelector(schedule_selector(CMainLogic::sendHallLink), this, 5.0f, false);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (!NATIVE_TEST)
    {
        m_nHallSocketHandle = m_pNetworkService->Connect(m_sNodeIP.c_str(), PORT_LOGON);//正常运营
    }
    else
    {
        m_nHallSocketHandle = m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON);//IP测试
    }
#endif
}

//imei,获取设备型号  生产厂商  共网IP（从网站获取）

//Http错误处理
void CMainLogic::HttpErrorHandle(CCHttpResponse* response, int nResponseError)
{
    if (response == NULL)
    {
        return;
    }
    //printf("Code = %d, Error = %s\n", response->getResponseCode(), response->getErrorBuffer());
    std::string sError = response->getErrorBuffer();
    std::string sResponseTag = response->getHttpRequest()->getTag();
    if (response->getResponseCode() == -1)
    {
        if (sError.find("Operation timed out") != std::string::npos)
        {
            switch (nResponseError) {
                case eWXOrderErr:
                {
                    ShowMessage("非常抱歉，订单信息获取失败了，请再试一次。", eMBOK);
                    break;
                }
                case eWXChargeSuccessErr:
                {
                    ShowMessage("验证支付结果超时，点击“确定”进行重试，点击“取消”请联系客服领取金币", eMBOKCancel, eReCheckWXChargeResult);
                    break;
                }
                case eAlipayOrderErr:
                {
                    ShowMessage("非常抱歉，订单信息获取失败了，请再试一次。", eMBOK);
                    break;
                }
                case eAlipaySuccessErr:
                {
                    ShowMessage("验证支付结果超时，点击“确定”进行重试，点击“取消”请联系客服领取金币", eMBOKCancel, eReCheckAlipayChargeResult);
                    break;
                }
                default:
                {
                    ShowMessage("非常抱歉，当前系统比较繁忙，请稍后再试一次。", eMBOK);
                    break;
                }
            }
        }
        else if (sError.find("Couldn't resolve host") != std::string::npos)
        {
            ShowMessage("DNS错误，请检查您的网络是否通畅");
        }
        else if (sError.find("Connection time-out") != std::string::npos)
        {
            switch (nResponseError) {
                case eWXOrderErr:
                {
                    ShowMessage("抱歉，请求超时，请检查一下您的网络是否通畅，然后再试一次。", eMBOK);
                    break;
                }
                case eWXChargeSuccessErr:
                {
                    ShowMessage("验证支付结果超时，点击“确定”进行重试，点击“取消”请联系客服领取金币", eMBOKCancel, eReCheckWXChargeResult);
                    break;
                }
                case eAlipayOrderErr:
                {
                    ShowMessage("抱歉，请求超时，请检查一下您的网络是否通畅，然后再试一次。", eMBOK);
                    break;
                }
                case eAlipaySuccessErr:
                {
                    ShowMessage("验证支付结果超时，点击“确定”进行重试，点击“取消”请联系客服领取金币", eMBOKCancel, eReCheckAlipayChargeResult);
                    break;
                }
                default:
                {
                    ShowMessage("连接超时，请检查您的网络是否通畅", eMBOK);
                    break;
                }
            }
        }
        else
        {
            switch (nResponseError) {
                case eAlipaySuccessErr:
                {
                    char temp[256];
                    sprintf(temp, "未知错误，请截图保存：%s。点击“确定”进行重试，点击“取消”请联系客服领取金币", sError.c_str());
                    ShowMessage(temp, eMBOKCancel, eReCheckAlipayChargeResult);
                    break;
                }
                case eWXChargeSuccessErr:
                {
                    char temp[256];
                    sprintf(temp, "未知错误，请截图保存：%s。点击“确定”进行重试，点击“取消”请联系客服领取金币", sError.c_str());
                    ShowMessage(temp, eMBOKCancel, eReCheckWXChargeResult);
                    break;
                }
                default:
                {
                    ShowMessage("非常抱歉，当前系统比较繁忙，请稍后再试一次。", eMBOK);
                    break;
                }
            }
        }
    }
    else if (response->getResponseCode() == 404)
    {
        ShowMessage("网站错误，页面不存在");
    }
    //把充值卡片设置为可以点击的
    ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
    if(pChargeLayer != NULL)
    {
        pChargeLayer -> setWebViewCharge(false);
    }
}

void CMainLogic::Tick(unsigned int nCurrentTick)
{
    if (m_pNetworkService != NULL)
    {
        m_pNetworkService->Tick(nCurrentTick);
    }
}

CMainLogic * CMainLogic::sharedMainLogic()
{
    return _instance;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
bool CMainLogic::SendData(int nNetID, WORD wMainCmdID, WORD wSubCmdID, const void * pData, WORD wDataSize)
{
    if (NULL == m_pNetworkService)
    {
        ERROR_CHECK;
        return false;
    }
    
    CNetInfo * pNetInfo = m_pNetworkService->GetNetInfo(nNetID);
    if (NULL == pNetInfo)
    {
        ERROR_CHECK;
        return false;
    }
    
    return SendData(pNetInfo, wMainCmdID, wSubCmdID, pData, wDataSize);
}

bool CMainLogic::SendData(CNetInfo * pNetInfo, WORD wMainCmdID, WORD wSubCmdID, const void * pData, WORD wDataSize)
{
    TCP_Head NetHead;
    NetHead.CommandInfo.wMainCmdID = wMainCmdID;
    NetHead.CommandInfo.wSubCmdID = wSubCmdID;
    
    if (NULL == m_pNetworkService)
    {
        ERROR_CHECK;
        return false;
    }
    
    m_pNetworkService->Send(pNetInfo, pData, wDataSize, &NetHead);
    
    return true;
}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
bool CMainLogic::SendData(int nID, WORD wMainCmdID, WORD wSubCmdID, const void * pData, WORD wDataSize)
{
    if (NULL == m_pNetworkService)
    {
        ERROR_CHECK;
        return false;
    }
    
    m_pNetworkService->Send(nID, wMainCmdID, wSubCmdID, pData, wDataSize);
    
    return true;
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void CMainLogic::OnEventTCPSocketLink(CNetInfo* pNetInfo, int nError, const char* sErrMsg)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void CMainLogic::OnEventTCPSocketLink(int nID, int nError)
#endif
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (NULL == pNetInfo)
    {
        ERROR_CHECK;
        return;
    }
#endif
    if (nError != 0)
    {
        ////printf("尝试了所有的服务器都无法成功连接服务器，请留意网站维护公告！\n");
        return ;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (m_nHallSocketHandle == m_pNetworkService->GetNetID(pNetInfo))
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (m_nHallSocketHandle == nID)
#endif
    {

        if(m_nClickBank == 9)//发送登录消息
        {
            if(m_pLogonScene != NULL)
            {
                CLogonLayer* pLogonLayer = (CLogonLayer*)m_pLogonScene->getChildByTag(eLogonLayerTag);
                if(pLogonLayer != NULL)
                {
                    pLogonLayer->unschedule(schedule_selector(CLogonLayer::logonHallSchedule));
                    pLogonLayer->sendLogonInfo();
                    m_nClickBank = 0;
                    return;
                }
            }
        }
        else if(m_nClickBank == 10)//发送同步登录信息
        {
            sendExchangeComplete();
            m_nClickBank = 0;
            return;
        }
        else if(m_nClickBank == 11)
        {
            CMD_GPR_CurrentScore CurrentScore;
            CurrentScore.dwUserID = m_nUserID;
            SendData(m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_CURRENT_SCORE,&CurrentScore,sizeof(CurrentScore));
            m_nClickBank = 0;
            return;
        }
        else if(m_nClickBank == 13)
        {
            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                LotteryLayer* pLotteryLayer = (LotteryLayer*)pHallLayer->getChildByTag(19989);
                if(pLotteryLayer != NULL)
                {
                    //pLotteryLayer->sendMailInfo();
                }
            }
            m_nClickBank = 0;
            return;
        }
        else if(m_nClickBank == 15)
        {
            requestMail();
            sendVIPGrade();
            m_nClickBank = 0;
            return;
        }
        else if(m_nClickBank == 18)
        {
            requestMail();
            sendVIPGrade();
            m_nClickBank = 0;
            return;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        SendData(pNetInfo, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		SendData(nID, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#endif
        if(m_nClickBank == 16)
        {
            return;
        }
        if(m_bNeedLogon)
        {
            m_bNeedLogon = false;
            m_pMessageBox->Update();
            return;
        }
        
        /*var info*/
        /*
        if(m_bLogonStatus)
        {
            m_bLogonStatus = false;
            if(m_pLogonScene != NULL)
            {
                CLogonLayer* pLogonLayer = (CLogonLayer*)m_pLogonScene->getChildByTag(eLogonLayerTag);
                if(pLogonLayer != NULL)
                {
                    pLogonLayer -> sendLogonInfo();
                }
            }
        }*/
#if !defined(ANYSDK_QH360_000023) && !defined(ANYSDK_BD_110000) && !defined(ANYSDK_YYB_000550)
        //发送登录消息
        if(m_bLogonStatus)
        {
            m_bLogonStatus = false;
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer -> sendLogonInfo();
                }
            }
        }
#endif
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    else if (m_nRoomSocketHandle == m_pNetworkService->GetNetID(pNetInfo))
    {
        OnLinkRoom(pNetInfo, nError, sErrMsg);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	else if (m_nRoomSocketHandle == nID)
    {
        OnLinkRoom(nID, nError);
    }
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void CMainLogic::OnEventTCPSocketRead(CNetInfo * pNetInfo, const void * pData, int nSize)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void CMainLogic::OnEventTCPSocketRead(int nID, const void * pData, int nSize)
#endif
{
    int nHeadSize = sizeof(TCP_Head);
    if (nSize < nHeadSize)
    {
        ERROR_CHECK;
        return;
    }
    
    TCP_Head * pHead = (TCP_Head *)pData;
    
    if (pHead == NULL)
    {
        ERROR_CHECK;
        return;
    }
    
    int wPacketSize = pHead->TCPInfo.wPacketSize;
    
    if (wPacketSize != nSize)
    {
        ERROR_CHECK;
        return;
    }
    
    const void * pPacketData = NULL;
    
    if (nSize > nHeadSize)
    {
        pPacketData = (const char *)pData + nHeadSize;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (pNetInfo == NULL)
    {
        ERROR_CHECK;
        return;
    }
    
    if (m_nHallSocketHandle == m_pNetworkService->GetNetID(pNetInfo))
    {
        OnSocketHall(pHead->CommandInfo, pPacketData, nSize - nHeadSize);
    }
    else if (m_nRoomSocketHandle == m_pNetworkService->GetNetID(pNetInfo))
    {
        OnSocketRoom(pHead->CommandInfo, pPacketData, nSize - nHeadSize);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (m_nHallSocketHandle == nID)
    {
        OnSocketHall(pHead->CommandInfo, pPacketData, nSize - nHeadSize);
    }
    else if (m_nRoomSocketHandle == nID)
    {
        OnSocketRoom(pHead->CommandInfo, pPacketData, nSize - nHeadSize);
    }
#endif
}

void CMainLogic::showCurrentDisNetWork()
{
//    ShowMessage("当前网络不稳定", eMBOK, eDisConnectBackToLogon);
//    if(GameMainScene::_instance != NULL)//停止心跳包检测
//    {
//        GameMainScene::_instance->unschedule(schedule_selector(GameMainScene::checkGameStatusOnline));
//        GameMainScene::_instance->unschedule(schedule_selector(GameMainScene::disConnectGame));
//    }
//    else if(GameLayer::_instance != NULL)//停止心跳包检测
//    {
//        GameLayer::_instance->unschedule(schedule_selector(GameLayer::checkGameStatusOnline));
//        GameLayer::_instance->unschedule(schedule_selector(GameLayer::disConnectGame));
//    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void CMainLogic::OnEventTCPSocketShut(CNetInfo * pNetInfo, int nError)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void CMainLogic::OnEventTCPSocketShut(int nID, int nError)
#endif
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    int nNetID = m_pNetworkService->GetNetID(pNetInfo);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
int nNetID = nID;
#endif
    printf("nNetID is %d, m_nRoomSocketHandle is %d, m_nHallSocketHandle is %d, nError id %d\n", nNetID, m_nRoomSocketHandle, m_nHallSocketHandle, nError);
    //@@断开连接后，是切换的时机
    if (nNetID == m_nRoomSocketHandle)
    {
        printf("m_bAutoExitGame is %d", m_bAutoExitGame);
        if(!m_bAutoExitGame)//如果不是系统维护，或者不是点击退出游戏按钮
        {
            m_bGameDisConnect = false;//不用手动再断开了
            if(m_pHallScene == NULL)
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
                pScheduler->scheduleSelector(schedule_selector(CMainLogic::showCurrentDisNetWork), this, 0.0f, 0, 0, false);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                ShowMessage("当前网络不稳定", eMBOK, eDisConnectBackToLogon);
#endif
            }
            m_bAutoExitGame = false;
        }
        if(nError == 60)
        {
            //@@保证再次进入房间时，桌子信息是正确的
            for (unsigned int i = 0; i < m_vUserInfo.size(); i++)
            {
                delete m_vUserInfo[i];
            }
            m_vUserInfo.clear();
        }
        
    }
    else if (nNetID == m_nHallSocketHandle)
    {
        //@@保证再次进入房间时，桌子信息是正确的
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		if(m_pNetworkService != NULL)
        {
            m_nClickBank = 16;
            if (!NATIVE_TEST)
            {
                m_nHallSocketHandle = m_pNetworkService->Connect(m_sNodeIP.c_str(), PORT_LOGON);
            }
            else
            {
                m_nHallSocketHandle = m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON);
            }
        }
        //根据这个变量判断定时器有没有开启
        if(!m_b2DisConnect)
        {
            m_n2DisConnect = 7;
            //开启定时器，只执行七次
            CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
            pScheduler->scheduleSelector(schedule_selector(CMainLogic::showDisConnect), this, 1.0f, false);
            m_b2DisConnect = true;
        }
#endif
    }
    
}

void CMainLogic::exitGameToHall()
{
    if(m_bGameChargeStatus)//如果是因为充值断开的连接，那么不需要返回大厅
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("FirstRoomToHall", false);
        //开始下订单
        if(GameMainScene::_instance != NULL || GameLayer::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->ShowMessage("正在获取订单", eMBNull);
            m_bClickIcon = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)//判断充值类型（苹果内购还是其他类型）
            if(m_bIOSPassed)
            {
                if(m_nShowChargeType == 4)//苹果内购
                {
                    ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
                    if(pChargeLayer != NULL)
                    {
                        pChargeLayer->IOSCharge();
                    }
                }
                else//其他支付方式
                {
                    ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
                    if(pChargeLayer != NULL)
                    {
                        pChargeLayer -> selectedChargeSuccess();
                    }
                    else
                    {
                        //选择充值方式（微信或者支付宝）
                        if(GameMainScene::_instance != NULL)
                        {
                            GiveCoinLayer* pGiveCoinLayer = (GiveCoinLayer*)GameMainScene::_instance->getChildByTag(eGiveCoinLayerChargeTag);
                            if(pGiveCoinLayer != NULL)
                            {
                                if(pGiveCoinLayer->getChargeType() == 1)//支付宝
                                {
                                    alipayCharge(10, 1, false);
                                }
                                else if(pGiveCoinLayer->getChargeType() == 2)//微信
                                {
                                    wxCharge(10, 1, false);
                                }
                            }
                            //判断是否是在ChargeUI界面
                            ChargeUI* pChargeUI = (ChargeUI*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20003);
                            if(pChargeUI != NULL)
                            {
                                if(pChargeUI->getChargeOpenType() == 1)//微信
                                {
                                    if(pChargeUI->getChargeType() == 2)//10元限购
                                    {
                                        wxCharge(10, 1, 0);
                                    }
                                    else if(pChargeUI->getChargeType() == 3)//1元购
                                    {
                                        wxCharge(1, 3, 0);
                                    }
                                }
                                else if(pChargeUI->getChargeOpenType() == 2)//支付宝
                                {
                                    if(pChargeUI->getChargeType() == 2)//10元限购
                                    {
                                        alipayCharge(10, 1, 0);
                                    }
                                    else if(pChargeUI->getChargeType() == 3)//1元购
                                    {
                                        alipayCharge(1, 3, 0);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if(m_nShowChargeType == 4)//苹果内购
                {
                    Charge2Layer* pCharge2Layer = (Charge2Layer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20002);
                    if(pCharge2Layer != NULL)
                    {
                        pCharge2Layer->selectedChargeSuccess();
                    }
                }
                else//其他支付方式
                {
                    ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
                    if(pChargeLayer != NULL)
                    {
                        pChargeLayer -> selectedChargeSuccess();
                    }
                }
            }
            
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
            if(pChargeLayer != NULL)
            {
                pChargeLayer -> selectedChargeSuccess();
            }
            else
            {
                //选择充值方式（微信或者支付宝）
                if(GameMainScene::_instance != NULL)
                {
                    GiveCoinLayer* pGiveCoinLayer = (GiveCoinLayer*)GameMainScene::_instance->getChildByTag(eGiveCoinLayerChargeTag);
                    if(pGiveCoinLayer != NULL)
                    {
                        if(pGiveCoinLayer->getChargeType() == 1)//支付宝
                        {
                            alipayCharge(10, 1, false);
                        }
                        else if(pGiveCoinLayer->getChargeType() == 2)//微信
                        {
                            wxCharge(10, 1, false);
                        }
                    }
                    //判断是否在ChargeUI界面
                    //判断是否是在ChargeUI界面
                    ChargeUI* pChargeUI = (ChargeUI*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20003);
                    if(pChargeUI != NULL)
                    {
                        if(pChargeUI->getChargeOpenType() == 1)//微信
                        {
                            if(pChargeUI->getChargeType() == 2)//10元限购
                            {
                                wxCharge(10, 1, 0);
                            }
                            else if(pChargeUI->getChargeType() == 3)//1元购
                            {
                                wxCharge(1, 3, 0);
                            }
                        }
                        else if(pChargeUI->getChargeOpenType() == 2)//支付宝
                        {
                            if(pChargeUI->getChargeType() == 2)//10元限购
                            {
                                alipayCharge(10, 1, 0);
                            }
                            else if(pChargeUI->getChargeType() == 3)//1元购
                            {
                                alipayCharge(1, 3, 0);
                            }
                        }
                    }
                }
            }
#endif
        }
    }
    else
    {
        backToHall();
    }
    
    //@@保证再次进入房间时，桌子信息是正确的
    for (unsigned int i = 0; i < m_vUserInfo.size(); i++)
    {
        delete m_vUserInfo[i];
    }
    m_vUserInfo.clear();
}

void CMainLogic::backToLogon()
{
    //@@保证再次进入房间时，桌子信息是正确的
    for (unsigned int i = 0; i < m_vUserInfo.size(); i++)
    {
        delete m_vUserInfo[i];
    }
    m_vUserInfo.clear();
    m_pLogonScene = NULL;
    
    CCScene* pLogonScene = CLogonLayer::scene();
    CMainLogic::sharedMainLogic()->m_bEnterLoading = false;
    if(GameLayer::_instance != NULL)
    {
        SimpleAudioEngine::sharedEngine()->stopEffect(GameLayer::_instance->getGameRollingEffectID());
        SimpleAudioEngine::sharedEngine()->stopEffect(GameLayer::_instance->getPrizeRollingEffectID());
    }
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3f,pLogonScene));
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/4.mp3", true);
    m_pLogonScene = pLogonScene;
    m_pHallScene = NULL;
    m_pTableScene = NULL;
    m_pCheckNetworkLayer = NULL;
    m_bLinkHallEnter = false;
}

void CMainLogic::backToHall()
{
    if(m_pLogonScene != NULL)
        return ;
    m_bShowGuide = false;
    m_bClickIcon = false;
    m_pHallScene = CHallLayer::scene();
    m_pMessageBox->Update();
    CHallLayer * layer = (CHallLayer *)m_pHallScene->getChildByTag(eHallLayerTag);
    m_bGameChargeStatus = false;
    m_nShowChargeType = 0;
    m_nShowCoinArmSkill = 0;
    layer->setMainLogic(this);
    layer->setUserScore(m_lUserScore);
    layer -> loadSelectedGameTypeNode(false, m_vGameType);//游戏房间选择界面
    layer->setUserNickName(m_sNickName.c_str());
    layer->setTicket(m_nLottery);
    layer->setUserID(m_nGameID);
    layer->notice();
    layer->setWeekNum(m_nUserWeekBeiNum);
    layer->setVipExp();//设置VIP等级、经验值百分比
    layer->setGender(m_nTouxiangID);
    layer->loadUI();
    if(m_nLogonGift != 0 || m_bCanShare || m_cbCanGetBankruptcy>0 || !m_bReceiveVipLogon || m_bCanBandingTel)
    {
        layer->showfreecoinAction(true);
    }
    layer->showFirstCharge();
    layer->showNiuGame();
    CMainLogic::sharedMainLogic()->m_bEnterLoading = false;
    if(GameLayer::_instance != NULL)
    {
        SimpleAudioEngine::sharedEngine()->stopEffect(GameLayer::_instance->getGameRollingEffectID());
        SimpleAudioEngine::sharedEngine()->stopEffect(GameLayer::_instance->getPrizeRollingEffectID());
    }
    CCDirector::sharedDirector()->replaceScene(m_pHallScene);//pushScene会导致memory leak
    m_bShowNoticeLayer = false;
    if(CCUserDefault::sharedUserDefault()->getBoolForKey("FirstRoomToHall", true))
    {
        m_bShowNoticeLayer = true;
        CCUserDefault::sharedUserDefault()->setBoolForKey("FirstRoomToHall", false);
        
    }
    m_pTableScene = NULL;
    m_pLogonScene = NULL;
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/4.mp3", true);
    if(m_bGameExitHall)//从房间界面或者游戏界面，返回到大厅，请求玩家当前分数和邮件列表
    {
        CMD_GPR_CurrentScore CurrentScore;
        CurrentScore.dwUserID = m_nUserID;
        SendData(m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_CURRENT_SCORE,&CurrentScore,sizeof(CurrentScore));
        //请求邮件ID
        CMD_GPR_MailIDList MailIDList;
        MailIDList.dwUserID = m_nUserID;
        SendData(m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_QUERY_MAIL_ID_LIST, &MailIDList, sizeof(MailIDList));
    }
}

void CMainLogic::getScoreTime(float dt)
{
    ShowMessage("抱歉，我们与登录服务器的连接断开了，请再重新登陆一次。",eMBOK,eDisConnectNet,false);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void CMainLogic::OnLinkHall2(CNetInfo * pNetInfo, int nError, const char * sErrMsg)
{
    SendData(pNetInfo, MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET);
}
#endif
void CMainLogic::OnLinkHall(int nNetID)
{
    char szMD5Result[128];
    CEncryptMD5::toString32(m_sPassword.c_str(),szMD5Result);
    
    switch (m_nLogonType)
    {
        case eLogon:
        {
            BYTE cbDataBuffer[SOCKET_TCP_PACKET];
            CMD_GP_LogonAccounts* pLogonAccounts = (CMD_GP_LogonAccounts*)cbDataBuffer;
            //发送定义
            WORD wHeadSize = sizeof(CMD_GP_LogonAccounts);
            CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);
            //设置变量
            memset(pLogonAccounts,0,sizeof(*pLogonAccounts));
            //系统信息
            pLogonAccounts->dwPlazaVersion=VERSION_PLAZA;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            std::string uuidStr = ChargeWebView::getUIDevice();
            memcpy(pLogonAccounts->szMachineID, uuidStr.c_str(), LEN_MACHINE_ID);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            JniMethodInfo info;
            bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
            jobject jobj;
            if(bHave)
            {
                jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
                bool bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","getUIDevice1", "(I)Ljava/lang/String;");
                if(bHave2)
                {
                    jstring jstr9 = (jstring)info.env->CallObjectMethod(jobj, info.methodID, 9);
                    std::string machineStr9 = JniHelper::jstring2string(jstr9);	//IMEI
                    memcpy(pLogonAccounts->szMachineID, machineStr9.c_str(), LEN_MACHINE_ID);
                }
            }
#endif
            //登录信息
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
            if (CCUserDefault::sharedUserDefault()->getStringForKey("AnySDKAccount", "") == m_sAccount)
            {
                strncpy(pLogonAccounts->szPassword,CMainLogic::sharedMainLogic()->m_sPassword.c_str(),sizeof(pLogonAccounts->szPassword));
            }
            else
            {
                DTP_GP_Password DTPPassword = {};
                strncpy(DTPPassword.cbData,CMainLogic::sharedMainLogic()->m_sPassword.c_str(),sizeof(DTPPassword.cbData));
                DTPPassword.wCount = CMainLogic::sharedMainLogic()->m_sPassword.length();
                BYTE cbKey = 0xAA;
                for (WORD i = 0; i < DTPPassword.wCount; i++)
                {
                    DTPPassword.cbData[i] ^= cbKey;
                    cbKey++;
                }
                SendPacket.AddPacket(&DTPPassword, sizeof(DTPPassword), DTP_GP_PASSWORD);
                strncpy(pLogonAccounts->szPassword,szMD5Result,sizeof(pLogonAccounts->szPassword));
            }
#else
            DTP_GP_Password DTPPassword = {};
            strncpy(DTPPassword.cbData,CMainLogic::sharedMainLogic()->m_sPassword.c_str(),sizeof(DTPPassword.cbData));
            DTPPassword.wCount = CMainLogic::sharedMainLogic()->m_sPassword.length();
            BYTE cbKey = 0xAA;
            for (WORD i = 0; i < DTPPassword.wCount; i++)
            {
                DTPPassword.cbData[i] ^= cbKey;
                cbKey++;
            }
            SendPacket.AddPacket(&DTPPassword, sizeof(DTPPassword), DTP_GP_PASSWORD);
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            SendPacket.AddPacket(CCharCode::UTF8ToGB2312(ChargeWebView::getIOSIDFA().c_str()).c_str(), DTP_GP_CLIENTIDFA);
            SendPacket.AddPacket(VERSION_IOS, DTP_GP_CLIENTVERSION);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            SendPacket.AddPacket(VERSION_ANDROID, DTP_GP_CLIENTVERSION);
#endif
            strncpy(pLogonAccounts->szPassword,szMD5Result,sizeof(pLogonAccounts->szPassword));
#endif
            
            strncpy(pLogonAccounts->szAccounts,m_sAccount.c_str(),sizeof(pLogonAccounts->szAccounts));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            strncpy(pLogonAccounts->szPassPortID,m_sMachine.c_str(),sizeof(pLogonAccounts->szPassPortID));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
            strncpy(pLogonAccounts->szPassPortID, CHANNEL_NUM, sizeof(pLogonAccounts->szPassPortID));
#else
            strncpy(pLogonAccounts->szPassPortID, MACHINE, sizeof(pLogonAccounts->szPassPortID));
#endif
#endif
            pLogonAccounts->cbValidateFlags=MB_VALIDATE_FLAGS|LOW_VER_VALIDATE_FLAGS;
            WORD wSendSize = SendPacket.GetDataSize()+sizeof(CMD_GP_LogonAccounts);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            SendData(m_pNetworkService->GetNetInfo(nNetID), MDM_GP_LOGON, SUB_GP_LOGON_ACCOUNTS, cbDataBuffer, wSendSize);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			SendData(nNetID, MDM_GP_LOGON, SUB_GP_LOGON_ACCOUNTS, cbDataBuffer, wSendSize);
#endif
            break;
        }
        case eRegister:
        {
            BYTE cbDataBuffer[SOCKET_TCP_PACKET];
            WORD wHeadSize = 0;
            if(m_nRegisterType == 1)//手机号注册
            {
                sendUserBehavior(0, eBtRegisterCompleteTel);
                CMD_GP_MBNumRegister* pMBNumRegister = (CMD_GP_MBNumRegister*)cbDataBuffer;
                wHeadSize = sizeof(CMD_GP_MBNumRegister);
                memset(pMBNumRegister, 0, sizeof(*pMBNumRegister));
                pMBNumRegister->RegisterAccounts.dwPlazaVersion=VERSION_PLAZA;
                pMBNumRegister->RegisterAccounts.cbGender = 1;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                std::string uuidStr = ChargeWebView::getUIDevice();
                memcpy(pMBNumRegister->RegisterAccounts.szMachineID, uuidStr.c_str(), LEN_MACHINE_ID);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                JniMethodInfo info;
                bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
                jobject jobj;
                if(bHave)
                {
                    jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
                    bool bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","getUIDevice1", "(I)Ljava/lang/String;");
                    if(bHave2)
                    {
                        jstring jstr9 = (jstring)info.env->CallObjectMethod(jobj, info.methodID, 9);
                        std::string machineStr9 = JniHelper::jstring2string(jstr9);	//IMEI
                        memcpy(pMBNumRegister->RegisterAccounts.szMachineID, machineStr9.c_str(), LEN_MACHINE_ID);
                    }
                }
#endif
                //登录信息
                strncpy(pMBNumRegister->RegisterAccounts.szLogonPass,szMD5Result,sizeof(pMBNumRegister->RegisterAccounts.szLogonPass));
                strncpy(pMBNumRegister->RegisterAccounts.szInsurePass,szMD5Result,sizeof(pMBNumRegister->RegisterAccounts.szInsurePass));
                strncpy(pMBNumRegister->RegisterAccounts.szAccounts,m_sAccount.c_str(),sizeof(pMBNumRegister->RegisterAccounts.szAccounts));
                strncpy(pMBNumRegister->RegisterAccounts.szNickName,m_sNickName.c_str(),sizeof(pMBNumRegister->RegisterAccounts.szNickName));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                strncpy(pMBNumRegister->RegisterAccounts.szPassPortID,m_sMachine.c_str(),sizeof(pMBNumRegister->RegisterAccounts.szPassPortID));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
                strncpy(pMBNumRegister->RegisterAccounts.szPassPortID, CHANNEL_NUM, sizeof(pMBNumRegister->RegisterAccounts.szPassPortID));
#else
                strncpy(pMBNumRegister->RegisterAccounts.szPassPortID, MACHINE, sizeof(pMBNumRegister->RegisterAccounts.szPassPortID));
#endif
#endif
                
                strncpy(pMBNumRegister->RegisterAccounts.szMobilePhone, m_sTelephone.c_str(), sizeof(pMBNumRegister->RegisterAccounts.szMobilePhone));
                strncpy(pMBNumRegister->szVerificationCode, m_sVerificationCode.c_str(), sizeof(pMBNumRegister->szVerificationCode));
                pMBNumRegister->RegisterAccounts.cbValidateFlags=MB_VALIDATE_FLAGS|LOW_VER_VALIDATE_FLAGS;
            }
            else if(m_nRegisterType == 2)//账号注册
            {
                sendUserBehavior(0, eBtRegisterCompleteAcc);
                CMD_GP_RegisterAccounts* pRegisterAccounts = (CMD_GP_RegisterAccounts*)cbDataBuffer;
                wHeadSize = sizeof(CMD_GP_RegisterAccounts);
                //设置变量
                memset(pRegisterAccounts,0,sizeof(*pRegisterAccounts));
                //系统信息
                pRegisterAccounts->dwPlazaVersion=VERSION_PLAZA;
                pRegisterAccounts->cbGender=1;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                std::string uuidStr = ChargeWebView::getUIDevice();
                memcpy(pRegisterAccounts->szMachineID, uuidStr.c_str(), LEN_MACHINE_ID);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                JniMethodInfo info;
                bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
                jobject jobj;
                if(bHave)
                {
                    jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
                    bool bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","getUIDevice1", "(I)Ljava/lang/String;");
                    if(bHave2)
                    {
                        jstring jstr9 = (jstring)info.env->CallObjectMethod(jobj, info.methodID, 9);
                        std::string machineStr9 = JniHelper::jstring2string(jstr9);	//IMEI
                        memcpy(pRegisterAccounts->szMachineID, machineStr9.c_str(), LEN_MACHINE_ID);
                    }
                }
#endif
                //登录信息
                strncpy(pRegisterAccounts->szLogonPass,szMD5Result,sizeof(pRegisterAccounts->szLogonPass));
                strncpy(pRegisterAccounts->szInsurePass,szMD5Result,sizeof(pRegisterAccounts->szInsurePass));
                strncpy(pRegisterAccounts->szAccounts,m_sAccount.c_str(),sizeof(pRegisterAccounts->szAccounts));
                strncpy(pRegisterAccounts->szNickName,m_sNickName.c_str(),sizeof(pRegisterAccounts->szNickName));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                strncpy(pRegisterAccounts->szPassPortID,m_sMachine.c_str(),sizeof(pRegisterAccounts->szPassPortID));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
                strncpy(pRegisterAccounts->szPassPortID, CHANNEL_NUM, sizeof(pRegisterAccounts->szPassPortID));
#else
                strncpy(pRegisterAccounts->szPassPortID, MACHINE, sizeof(pRegisterAccounts->szPassPortID));
#endif
#endif
                pRegisterAccounts->cbValidateFlags=MB_VALIDATE_FLAGS|LOW_VER_VALIDATE_FLAGS;
            }
            
            //发送定义
            
            CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);
            //将密码加密
            DTP_GP_Password DTPPassword = {};
            strncpy(DTPPassword.cbData,CMainLogic::sharedMainLogic()->m_sPassword.c_str(),sizeof(DTPPassword.cbData));
            DTPPassword.wCount = CMainLogic::sharedMainLogic()->m_sPassword.length();
            BYTE cbKey = 0xAA;
            for (WORD i = 0; i < DTPPassword.wCount; i++)
            {
                DTPPassword.cbData[i] ^= cbKey;
                cbKey++;
            }
            
            SendPacket.AddPacket(&DTPPassword, sizeof(DTPPassword), DTP_GP_PASSWORD);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            SendPacket.AddPacket(CCharCode::UTF8ToGB2312(ChargeWebView::getIOSIDFA().c_str()).c_str(), DTP_GP_CLIENTIDFA);
            SendPacket.AddPacket(VERSION_IOS, DTP_GP_CLIENTVERSION);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            SendPacket.AddPacket(VERSION_ANDROID, DTP_GP_CLIENTVERSION);
#endif
            if(m_nRegisterType == 1)//手机号注册
            {
                WORD wSendSize = SendPacket.GetDataSize()+sizeof(CMD_GP_RegisterAccounts);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				SendData(m_pNetworkService->GetNetInfo(nNetID), MDM_GP_LOGON, SUB_GPR_MBREIGSTER, cbDataBuffer, wSendSize);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				SendData(nNetID, MDM_GP_LOGON, SUB_GPR_MBREIGSTER, cbDataBuffer, wSendSize);
#endif
			}
            else if(m_nRegisterType == 2)//账号注册
            {		
                WORD wSendSize = SendPacket.GetDataSize()+sizeof(CMD_GP_RegisterAccounts);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				SendData(m_pNetworkService->GetNetInfo(nNetID), MDM_GP_LOGON, SUB_GP_REGISTER_ACCOUNTS, cbDataBuffer, wSendSize);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				SendData(nNetID, MDM_GP_LOGON, SUB_GP_REGISTER_ACCOUNTS, cbDataBuffer, wSendSize);
#endif
			}
            break;
        }
        case eGuestRegister:
        {
            BYTE cbDataBuffer[SOCKET_TCP_PACKET];
            CMD_GP_GuestRegister* pGuestRegister = (CMD_GP_GuestRegister*)cbDataBuffer;
            //发送定义
            WORD wHeadSize = sizeof(CMD_GP_GuestRegister);
            CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);
            //设置变量
            memset(pGuestRegister,0,sizeof(*pGuestRegister));
            //系统信息
            pGuestRegister->dwPlazaVersion=VERSION_PLAZA;
            pGuestRegister->cbGender=1;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            std::string uuidStr = ChargeWebView::getUIDevice();
            memcpy(pGuestRegister->szMachineID, uuidStr.c_str(), LEN_MACHINE_ID);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            JniMethodInfo info;
            bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
            jobject jobj;
            if(bHave)
            {
                jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
                bool bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","getUIDevice1", "(I)Ljava/lang/String;");
                if(bHave2)
                {
                    jstring jstr9 = (jstring)info.env->CallObjectMethod(jobj, info.methodID, 9);
                    std::string machineStr9 = JniHelper::jstring2string(jstr9);	//IMEI
                    memcpy(pGuestRegister->szMachineID, machineStr9.c_str(), LEN_MACHINE_ID);
                }
            }
#endif
            //将密码加密
            std::string passwordStr = "youke";
            DTP_GP_Password DTPPassword = {};
            strncpy(DTPPassword.cbData,passwordStr.c_str(),sizeof(DTPPassword.cbData));
            DTPPassword.wCount = passwordStr.length();
            BYTE cbKey = 0xAA;
            for (WORD i = 0; i < DTPPassword.wCount; i++)
            {
                DTPPassword.cbData[i] ^= cbKey;
                cbKey++;
            }
            SendPacket.AddPacket(&DTPPassword, sizeof(DTPPassword), DTP_GP_PASSWORD);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            SendPacket.AddPacket(CCharCode::UTF8ToGB2312(ChargeWebView::getIOSIDFA().c_str()).c_str(), DTP_GP_CLIENTIDFA);
            SendPacket.AddPacket(VERSION_IOS, DTP_GP_CLIENTVERSION);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            SendPacket.AddPacket(VERSION_ANDROID, DTP_GP_CLIENTVERSION);
#endif
            strncpy(pGuestRegister->szAccounts,"youke",sizeof(pGuestRegister->szAccounts));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            strncpy(pGuestRegister->szPassPortID,m_sMachine.c_str(),sizeof(pGuestRegister->szPassPortID));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
            strncpy(pGuestRegister->szPassPortID, CHANNEL_NUM, sizeof(pGuestRegister->szPassPortID));
#else
            strncpy(pGuestRegister->szPassPortID, MACHINE, sizeof(pGuestRegister->szPassPortID));
#endif
#endif
            pGuestRegister->cbValidateFlags=MB_VALIDATE_FLAGS|LOW_VER_VALIDATE_FLAGS;
            WORD wSendSize = SendPacket.GetDataSize()+sizeof(CMD_GP_GuestRegister);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			SendData(m_pNetworkService->GetNetInfo(nNetID), MDM_GP_LOGON, SUB_GP_GUEST_REGISTER, cbDataBuffer, wSendSize);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			SendData(nNetID, MDM_GP_LOGON, SUB_GP_GUEST_REGISTER, cbDataBuffer, wSendSize);
#endif
			break;
        }
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void CMainLogic::OnLinkRoom(CNetInfo * pNetInfo, int nError, const char * sErrMsg)
{
    SendData(pNetInfo, MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET);
    
    char szMD5Result[128];
    CEncryptMD5::toString32(m_sPassword.c_str(),szMD5Result);
    
    CMD_GR_LogonUserID LogonUserID = {0};
    
    LogonUserID.dwPlazaVersion = VERSION_PLAZA;
    LogonUserID.dwUserID = m_nUserID;
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
	if(CCUserDefault::sharedUserDefault()->getStringForKey("AnySDKAccount", "") == m_sAccount)
    {
		strcpy(LogonUserID.szPassword, m_sPassword.c_str());
	}
	else
	{
    	strcpy(LogonUserID.szPassword, szMD5Result);
	}
#else
	strcpy(LogonUserID.szPassword, szMD5Result);
#endif
    LogonUserID.wKindID = m_CurrentServer.wKindID;
    
    SendData(pNetInfo, MDM_GR_LOGON, SUB_GR_LOGON_USERID, &LogonUserID, sizeof(LogonUserID));
}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void CMainLogic::OnLinkRoom(int nID, int nError, const char * sErrMsg)
{
    SendData(nID, MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET);
    
    char szMD5Result[128];
    CEncryptMD5::toString32(m_sPassword.c_str(),szMD5Result);
    
    CMD_GR_LogonUserID LogonUserID = {0};
    
    LogonUserID.dwPlazaVersion = VERSION_PLAZA;
    LogonUserID.dwUserID = m_nUserID;
    strcpy(LogonUserID.szPassword, szMD5Result);
    LogonUserID.wKindID = m_CurrentServer.wKindID;
    
    SendData(nID, MDM_GR_LOGON, SUB_GR_LOGON_USERID, &LogonUserID, sizeof(LogonUserID));
}
#endif

bool CMainLogic::OnSocketHall(const TCP_Command & Command, const void * pData, unsigned short wDataSize)
{
    bool bRet = false;
    switch (Command.wMainCmdID)
    {
        case MDM_KN_COMMAND:
        {
            if(Command.wSubCmdID == SUB_KN_DETECT_SOCKET)
            {
                bRet = SendData(m_nHallSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET);
            }
            else if(Command.wSubCmdID == SUB_KN_DETECT_MANUAL)
            {
                bRet = true;
                m_bReceiveHallLink = true;
                m_bStartOpenCheck = true;
                if(m_b2DisConnect)
                {
                    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
                    pScheduler->unscheduleSelector(schedule_selector(CMainLogic::showDisConnect), this);
                    m_b2DisConnect = false;
                }
                
                if(m_pCheckNetworkLayer != NULL)
                {
                    m_pCheckNetworkLayer->removeFromParent();
                    m_pCheckNetworkLayer = NULL;
                }
                if(m_bNeedLogon)
                {
                    m_pMessageBox->Update();
                    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
                    pScheduler->unscheduleSelector(schedule_selector(CMainLogic::openCheckLinkTime), this);
                    m_bNeedLogon = false;
                    return true;
                }
                if(m_nCheckSocketShut == 9)//登录检测消息
                {
                    if(m_pLogonScene != NULL)
                    {
                        CLogonLayer* pLogonLayer = (CLogonLayer*)m_pLogonScene->getChildByTag(eLogonLayerTag);
                        pLogonLayer->unschedule(schedule_selector(CLogonLayer::receiveHallLink));
                        pLogonLayer->sendLogonInfo();
                        m_nCheckSocketShut = 0;
                    }
                }
                else if(m_nCheckSocketShut == 10)//从其他程序切回来，收到发送的检测包
                {
                    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
                    pScheduler->unscheduleSelector(schedule_selector(CMainLogic::receiveHallLinkInfo), this);
                    //                    this->unschedule(schedule_selector(CMainLogic::receiveHallLinkInfo));
                    sendExchangeComplete();
					m_nCheckSocketShut = 0;
                }
                else if(m_nCheckSocketShut == 11)//请求当前分数
                {
                    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
                    pScheduler->unscheduleSelector(schedule_selector(CMainLogic::receive2HallLinkInfo), this);
                    //                    this->unschedule(schedule_selector(CMainLogic::receive2HallLinkInfo));
                    CMD_GPR_CurrentScore CurrentScore;
                    CurrentScore.dwUserID = m_nUserID;
                    SendData(m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_CURRENT_SCORE,&CurrentScore,sizeof(CurrentScore));
                	m_nCheckSocketShut = 0;
				}
                else if(m_nCheckSocketShut == 13)//兑换商城
                {
                    CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                    if(pHallLayer != NULL)
                    {
                        LotteryLayer* pLotteryLayer = (LotteryLayer*)pHallLayer->getChildByTag(19989);
                        if(pLotteryLayer != NULL)
                        {
//                            pLotteryLayer->unschedule(schedule_selector(LotteryLayer::receiveHallLinkInfo));
//                            pLotteryLayer->sendMailInfo();
                        }
                    }
					m_nCheckSocketShut = 0;
                }
                else if(m_nCheckSocketShut == 15)//充值返利邮件
                {
                    CCNode* pNode = CCDirector::sharedDirector()->getRunningScene();
                    if(pNode != NULL)
                    {
                        ChargeLayer* pChargeLayer = (ChargeLayer*)pNode->getChildByTag(20000);
                        if(pChargeLayer != NULL)
                        {
                            pChargeLayer -> unschedule(schedule_selector(ChargeLayer::receiveHallLink2));
                            requestMail();
                            sendVIPGrade();
                        }
                    }
					m_nCheckSocketShut = 0;
                }
                else if(m_nCheckSocketShut == 16)//抢福袋游戏中检测登录服务器，不做任何处理
                {
                }
                else if(m_nCheckSocketShut == 18)
                {
                    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
                    pScheduler->unscheduleSelector(schedule_selector(CMainLogic::receiveIOSHallLink2), this);
                    requestMail();
                    sendVIPGrade();
					m_nCheckSocketShut = 0;
                }
            }
        }
            break;
        case MDM_GP_LOGON:
        {
            if(m_pLogonScene != NULL)
            {
                CLogonLayer* pLogonLayer = (CLogonLayer*)m_pLogonScene->getChildByTag(eLogonLayerTag);
                if(pLogonLayer != NULL)
                {
                    if(pLogonLayer->getLogonStatus())
                    {
                        pLogonLayer->unschedule(schedule_selector(CLogonLayer::mesNetInfo));
                    }
                }
            }
            bRet = OnSocketHallLogon(Command, pData, wDataSize);
        }
            break;
        case MDM_GP_SERVER_LIST:
        {
            bRet = OnSocketHallServerList(Command, pData, wDataSize);
        }
            break;
        case MDM_GP_USER_SERVICE://服务命令（签到、抽奖）充值次数为0或1
        {
            //签到消息
            bRet = OnSocketServerMessage(Command, pData, wDataSize);
        }
            break;
        case MDM_GP_USER_NOTICE://公告信息
        {
            bRet = OnSocketNoticeMessage(Command, pData, wDataSize);
        }
            break;
        case MDM_GP_ITEM://道具系统
        {
            bRet = OnSocketItemMessage(Command, pData, wDataSize);
        }
            break;
        case MDM_GP_SKILL:
        {
            bRet = OnSocketSkillMessage(Command, pData, wDataSize);
        }
            break;
    }
    if (!bRet)
    {
        //printf("command is %d, %d\n", Command.wMainCmdID, Command.wSubCmdID);
        ERROR_CHECK;
    }
    
    return bRet;
}

bool CMainLogic::OnSocketSkillMessage(const TCP_Command & Command, const void* pData, unsigned short wDataSize)
{
    bool bRet = false;
    switch (Command.wSubCmdID)
    {
        case SUB_GPO_QUERY_SKILL://查询技能
        {
            bRet = OnQuerySkill(pData, wDataSize);
            break;
        }
        case SUB_GPO_NEW_GUID_QUERY://新手引导
        {
            bRet = OnNewGuidQuery(pData, wDataSize);
            break;
        }
        case SUB_GPO_SUPER_GOLD:
        {
            bRet = OnSuperGold(pData, wDataSize);
            break;
        }
    }
    return bRet;
}

bool CMainLogic::OnSuperGold(const void * pData, unsigned short wDataSize)
{
    if (wDataSize < sizeof(CMD_GPO_SuperGold))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_SuperGold* pSuperGold = (CMD_GPO_SuperGold*)pData;
    if(pSuperGold != NULL)
    {
        m_bCanEnterPiraseFreeCoin = ((pSuperGold->cbCan&8) == 0);
        m_nPirasePrise = pSuperGold->nSuperGold;
    }
    return true;
}

bool CMainLogic::OnNewGuidQuery(const void* pData, unsigned short wDataSize)
{
    if (wDataSize < sizeof(CMD_GPO_NewGuidQuery))
    {
        ERROR_CHECK;
        return false;
    }
    m_bShowGuide = false;
    //return true;
    CMD_GPO_NewGuidQuery* pNewGuidQuery = (CMD_GPO_NewGuidQuery*)pData;
    if(pNewGuidQuery != NULL)
    {
        m_bShowGuide = pNewGuidQuery->bCan;
        printf("m_bShowGuide is %d\n", m_bShowGuide);
    }
    if(m_sAndroidMachine == "fenxiang")
    {
        m_bShowGuide = false;
    }
//    if(m_lUserScore >= 50000)
//    {
//        m_bShowGuide = false;
//    }
    if(m_bShowGuide && m_bIOSPassed)
    {
        bool bExist = false;
        unsigned long count = m_vRoomNZNH.size();
        SCORE minScore = 0;
        for(int i = 0; i < count; i++)
        {
            int num = CMainLogic::sharedMainLogic()->m_vRoomNZNH[i].wServerPort/10000-1;
            if(num == 0)
            {
                bExist = true;
                minScore = CMainLogic::sharedMainLogic()->m_vRoomNZNH[i].lMinEnterScore;
                break;
            }
        }
        if(bExist && CMainLogic::sharedMainLogic()->m_lUserScore>=minScore)
        {
            Guide2Layer* pGuideLayer = Guide2Layer::create(1);
            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer -> addChild(pGuideLayer, 199);
                }
            }
        }
        else
        {
            m_bShowGuide = false;
        }
    }
    return true;
}

bool CMainLogic::OnQuerySkill(const void* pData, unsigned short wDataSize)
{
    if (wDataSize < sizeof(CMD_GPO_QuerySkill))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_QuerySkill* pQuerySkill = (CMD_GPO_QuerySkill*)pData;
    if(pQuerySkill != NULL)
    {
        //printf("%d, %d, %d, %d\n", pQuerySkill->nCannonStatus, pQuerySkill->nJiaSu, pQuerySkill->nSanshe, pQuerySkill->nBaoji);
        m_nCannonStatus = pQuerySkill->nCannonStatus;
        if(m_bIOSPassed)//如果已经通过审核
        {
            m_pMessageBox->Update();
            CCNode * pNode = CCDirector::sharedDirector()->getRunningScene();
            if(pNode != NULL)
            {
                ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
                if(pChargeLayer != NULL)
                {
                }
                else
                {
                    ChargeLayer* pChargeLayer = ChargeLayer::create();
                    pNode -> addChild(pChargeLayer, 11, 20000);
                    for(int i = 0; i < 3; i++)
                    {
                        m_nSkillCount[i] = pQuerySkill->cbCount[i];
                    }
                }
                
            }
        }
        else//如果没有通过审核，则打开苹果充值
        {
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_IAP_PRODUCT_LIST);
        }
        
    }
    
    return true;
}

bool CMainLogic::OnSocketItemMessage(const TCP_Command & Command, const void* pData, unsigned short wDataSize)
{
    bool bRet = false;
    
    switch (Command.wSubCmdID)
    {
        case SUB_GPO_QUERY_ITEM_COUNT://查询道具数量
        {
            bRet = OnQueryItemCount(pData,wDataSize);
            break;
        }
        case SUB_GPO_TRANSFER_ITEM://赠送道具
        {
            bRet = OnTransferItem(pData, wDataSize);
            break;
        }
        case SUB_GPO_COMBINE_ITEM://合成龙珠
        {
            bRet = OnCombineItem(pData, wDataSize);
            break;
        }
        case SUB_GPO_USE_DRAGON://使用龙珠兑换金币
        {
            bRet = OnUseDragon(pData, wDataSize);
            break;
        }
        case SUB_GPO_EQUIPMENT_DRAGON://装备龙珠
        {
            bRet = OnUserEquipmentDragon(pData, wDataSize);
            break;
        }
        case SUB_GPO_EXCHANGE_LOTTERY://兑换奖券
        {
            bRet = OnUserExchangeLottery(pData, wDataSize);
            break;
        }
    }
    return bRet;
}

bool CMainLogic::OnUserExchangeLottery(const void * pData, unsigned short wDataSize)
{
    m_pMessageBox->Update();
    if (wDataSize < sizeof(CMD_GPO_ExchangeLottery))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_ExchangeLottery* pExchangeLottery = (CMD_GPO_ExchangeLottery*)pData;
    if(pExchangeLottery != NULL)
    {
        if(pExchangeLottery->nResultCode == 0)
        {
            m_nLottery = pExchangeLottery->nCurrentLottery;
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer -> setTicket(m_nLottery);
                    SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                    pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                    DaojuLayer* pDaojuLayer = (DaojuLayer*)pHallLayer->getChildByTag(19987);
                    if(pDaojuLayer != NULL)
                    {
                        pDaojuLayer->updateSpriteUI(pExchangeLottery->nChuJiCount, pExchangeLottery->nZhongJiCount, pExchangeLottery->nGaoJiCount);
                        pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                        SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                    }
                }
            }
        }
        else
        {
        }
        CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(pExchangeLottery->szDescribeString).c_str());
    }
    return true;
}

bool CMainLogic::OnQueryItemCount(const void * pData, unsigned short wDataSize)
{
    //printf("helloworld\n");
    m_pMessageBox->Update();
    if (wDataSize < sizeof(CMD_GPO_QueryItemCount))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_QueryItemCount* pQueryItemCount = (CMD_GPO_QueryItemCount*)pData;
    if(pQueryItemCount == NULL)
        return true;
    //printf("%d, %d, %d, %d, %d, %d, %d, %d\n", pQueryItemCount->nChuJiCount, pQueryItemCount->nZhongJiCount, pQueryItemCount->nGaoJiCount, pQueryItemCount->nZhiZunCount, pQueryItemCount->nCannonStatus, pQueryItemCount->nJiaSu, pQueryItemCount->nSanShe, pQueryItemCount->nBaoJi);
    m_nCannonStatus = pQueryItemCount->nCannonStatus;
    m_nBaojiTimes = pQueryItemCount->nBaoJi;
    m_nSansheTimes = pQueryItemCount->nSanShe;
    m_nSpeedUpTimes = pQueryItemCount->nJiaSu;
    m_nSuperPirateItemCount = pQueryItemCount->nTreasureMap;
    int nChuJiCount = pQueryItemCount->nChuJiCount;//龙珠碎片数量
    int nZhongJiCount = pQueryItemCount->nZhongJiCount;//小龙珠数量
    int nGaoJiCount = pQueryItemCount->nGaoJiCount;//大龙珠数量
    //展示道具界面，并根据数量进行排序展示
    if(m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            DaojuLayer* pDaojuLayer = DaojuLayer::create(nChuJiCount, nZhongJiCount, nGaoJiCount);
            pHallLayer -> addChild(pDaojuLayer, 200, 19987);
        }
    }
    else if(GameMainScene::_instance != NULL)
    {
        //如果是查询弹头数量，那么只显示弹头数量
        if(GameMainScene::_instance->getSelectDantouNum())
        {
            GameMainScene::_instance->setDantouNum(nChuJiCount, nZhongJiCount, nGaoJiCount);
        }
        else
        {
            //停止自动开炮
            if(GameMainScene::_instance->getAutoFireStatus())
            {
                GameMainScene::_instance->unschedule(schedule_selector(GameMainScene::startBullet));
            }
            if(GameMainScene::_instance->getClickDantouMenu())
            {
                FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create(7);
                CCDirector::sharedDirector()->getRunningScene()->addChild(pFirstChargeLayer, 19, 10302);
                pFirstChargeLayer->setDantouNum(nChuJiCount, nZhongJiCount, nGaoJiCount);
            }
            else
            {
                DaojuLayer* pDaojuLayer = DaojuLayer::create(nChuJiCount, nZhongJiCount, nGaoJiCount);
                GameMainScene::_instance -> addChild(pDaojuLayer, 200, 1989);
                if(GameMainScene::_instance->getMeRealChairID()>=GAME_PLAYER/2)
                {
                    pDaojuLayer->setRotation(180);
                    pDaojuLayer->m_pUserID->setPosition(pDaojuLayer->m_pUserID->getPosition());
                    pDaojuLayer->m_pUserNum->setPosition(pDaojuLayer->m_pUserNum->getPosition());
                    
                }
            }
        }
    }
    return true;
}

bool CMainLogic::OnTransferItem(const void * pData, unsigned short wDataSize)//赠送道具
{
    m_pMessageBox->Update();
    if (wDataSize < sizeof(CMD_GPO_TransferItem))
    {
        ERROR_CHECK;
        return false;
    }
    //更新界面
    CMD_GPO_TransferItem * pTransferItem = (CMD_GPO_TransferItem*)pData;
    if(pTransferItem != NULL)
    {
        //printf("%d, %d, %d, %d, %s\n", pTransferItem->nItemID, pTransferItem->nSourceCount, pTransferItem->nVariationCount, pTransferItem->nResultCode, CCharCode::GB2312ToUTF8(pTransferItem->szDescribeString).c_str());
        if(pTransferItem->nResultCode == 0)
        {
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    DaojuLayer* pDaojuLayerLayer = (DaojuLayer*)pHallLayer->getChildByTag(19987);
                    if(pDaojuLayerLayer != NULL)
                    {
                        pDaojuLayerLayer -> updateZhenzhuUI(pTransferItem->nItemID, pTransferItem->nSourceCount+pTransferItem->nVariationCount);
                        pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                        SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                    }
                }
            }
            else if(GameMainScene::_instance != NULL)
            {
                if(pTransferItem->nItemID == 0)
                {
                    sendUserBehavior(m_nUserID, eGameTongDantouSuccess);
                }
                else if(pTransferItem->nItemID == 1)
                {
                    sendUserBehavior(m_nUserID, eGameYinDantouSuccess);
                }
                else if(pTransferItem->nItemID == 2)
                {
                    sendUserBehavior(m_nUserID, eGameJinDantouSuccess);
                }
                DaojuLayer* pDaojuLayerLayer = (DaojuLayer*)GameMainScene::_instance->getChildByTag(1989);
                if(pDaojuLayerLayer != NULL)
                {
                    pDaojuLayerLayer -> updateZhenzhuUI(pTransferItem->nItemID, pTransferItem->nSourceCount+pTransferItem->nVariationCount);
                    GameMainScene::_instance->playParticle("penjinbi", CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                    SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                }
            }
        }
        else
        {
        }
        CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(pTransferItem->szDescribeString).c_str());
        
    }
    return true;
}

bool CMainLogic::OnCombineItem(const void * pData, unsigned short wDataSize)//合成道具
{
    m_pMessageBox->Update();
    if (wDataSize < sizeof(CMD_GPO_CombineItem))
    {
        ERROR_CHECK;
        return false;
    }
    //更新界面
    CMD_GPO_CombineItem* pCombineItem = (CMD_GPO_CombineItem*)pData;
    if(pCombineItem != NULL)
    {
    }
    return true;
}

bool CMainLogic::OnUseDragon(const void * pData, unsigned short wDataSize)//使用龙珠兑换金币
{
    m_pMessageBox->Update();
    if (wDataSize < sizeof(CMD_GPO_UseDragon))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_UseDragon* pUserDragon = (CMD_GPO_UseDragon*)pData;
    if(pUserDragon != NULL)
    {
        //printf("User is %lld, %d, %d, %d,     %d, %s\n", pUserDragon->lCurrentScore, pUserDragon->nChuJiCount, pUserDragon->nZhongJiCount, pUserDragon->nGaoJiCount, pUserDragon->nResultCode, CCharCode::GB2312ToUTF8(pUserDragon->szDescribeString).c_str());
        if(pUserDragon->nResultCode == 0)
        {
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    m_lUserScore = pUserDragon->lCurrentScore;
                    pHallLayer -> setUserScore(pUserDragon->lCurrentScore);
                    DaojuLayer* pDaojuLayer = (DaojuLayer*)pHallLayer->getChildByTag(19987);
                    if(pDaojuLayer != NULL)
                    {
                        pDaojuLayer->updateSpriteUI(pUserDragon->nChuJiCount, pUserDragon->nZhongJiCount, pUserDragon->nGaoJiCount);
                        pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                        SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                    }
                }
            }
        }
        else
        {
        }
        CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(pUserDragon->szDescribeString).c_str());
    }
    return true;
}

bool CMainLogic::OnUserEquipmentDragon(const void * pData, unsigned short wDataSize)
{
    m_pMessageBox->Update();
    if (wDataSize < sizeof(CMD_GPO_EquipmentDragon))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_EquipmentDragon* pEquipmentDragon = (CMD_GPO_EquipmentDragon*)pData;
    if(pEquipmentDragon != NULL)
    {
        //printf("equip is %d, %d, %d, %d, %d, %d, %s\n", pEquipmentDragon->nEquipmentDragonCount, pEquipmentDragon->nChuJiCount, pEquipmentDragon->nZhongJiCount, pEquipmentDragon->nGaoJiCount, pEquipmentDragon->nZhiZunCount,pEquipmentDragon->nResultCode, CCharCode::GB2312ToUTF8(pEquipmentDragon->szDescribeString).c_str());
        if(pEquipmentDragon->nResultCode == 0)
        {
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    DaojuLayer* pDaojuLayer = (DaojuLayer*)pHallLayer->getChildByTag(19987);
                    if(pDaojuLayer != NULL)
                    {
                        pDaojuLayer->updateSpriteUI(pEquipmentDragon->nChuJiCount, pEquipmentDragon->nZhongJiCount, pEquipmentDragon->nGaoJiCount);
                        pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                        SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                    }
                }
            }
        }
        else
        {
        }
        CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(pEquipmentDragon->szDescribeString).c_str());
    }
    return true;
}

bool CMainLogic::OnSocketServerMessage(const TCP_Command & Command, const void* pData, unsigned short wDataSize)
{
    bool bRet = false;
    
    switch (Command.wSubCmdID)
    {
        case SUB_GP_QUERY_SIGNIN_INFO_RESULT://获取签到信息
        {
            bRet = OnGetSignInMessage(pData,wDataSize);
            break;
        }
        case SUB_GP_USER_SIGNIN_RESULT://签到成功
        {
            bRet = OnSocketSignInSucceed(pData,wDataSize);
            break;
        }
        case SUB_GP_QUERY_LOTTERY_INFO_RESULT://用户获取抽奖次数
        {
            bRet = OnGetBonusInfo(pData, wDataSize);
            break;
        }
        case SUB_GP_USER_LOTTERY_RESULT://用户获取抽奖结果
        {
            bRet = OnGetBonusCompleted(pData, wDataSize);
            break;
        }
        case SUB_GP_LOTTERY_MSG://获取公告
        {
            bRet = OnGetBonusMessage(pData, wDataSize);
            break;
        }
        case SUB_GP_QUERY_RECHARGE_INFO_RES:
        {
            bRet = OnGetChargeTime(pData, wDataSize);
            break;
        }
        case SUB_GPO_QUERY_MAIL_ID_LIST://获取邮件ID
        {
            bRet = OnGetMailListID(pData, wDataSize);
            break;
        }
        case SUB_GPO_QUERY_MAIL_INFO://获取邮件标题
        {
            bRet = onGetMailTitle(pData, wDataSize);
            break;
        }
        case SUB_GPO_QUERY_MAIL_CONTENT://获取邮件内容
        {
            bRet = onGetMailContent(pData, wDataSize);
            break;
        }
        case SUB_GPO_QUERY_MAIL_ITEM://领取邮件奖励
        {
            bRet = onGetMailPrise(pData, wDataSize);
            break;
        }
        case SUB_GP_USER_INSURE_INFO://银行查询
        {
            bRet = onQueryInsureInfo(pData, wDataSize);
            break;
        }
        case SUB_GP_USER_INSURE_SUCCESS://银行成功
        {
            bRet = OnInsureSuccess(pData, wDataSize);
            break;
        }
        case SUB_GP_USER_INSURE_FAILURE://银行失败
        {
            bRet = OnInsureFail(pData, wDataSize);
            break;
        }
        case SUB_GP_QUERY_USER_INFO_RESULT://转账第一次请求确认信息
        {
            bRet = OnTransferUserInfo(pData, wDataSize);
            break;
        }
        case SUB_GP_USER_TRANSFER_INFO://转账成功
        {
            bRet = OnTransferSuccess(pData, wDataSize);
            break;
        }
        case SUB_GPO_CHARGE_VALUE:
        {
            bRet = OnChargeSuccess(pData, wDataSize);
            break;
        }
        case SUB_GPO_VIP_INFO:
        {
            bRet = OnVIPInfo(pData, wDataSize);
            break;
        }
        case SUB_GPO_VIP_RESULT:
        {
            bRet = OnVIPResult(pData, wDataSize);
            break;
        }
        case SUB_GPO_CURRENT_SCORE://从游戏返回到大厅时，请求玩家分数，显示在大厅上
        {
            bRet = OnGetCurrentScore(pData, wDataSize);
            break;
        }
        case SUB_GPO_LOGONGIFT://领取登录奖励
        {
            bRet = OnGetLogonGift(pData, wDataSize);
            break;
        }
        case SUB_GPO_CHARGE1ST_REQUEST://首充成功，领取消息
        {
            bRet = OnCharge1stRequest(pData, wDataSize);
            break;
        }
        case SUB_GPO_CHARGE1ST_GET://领取首充礼包
        {
            bRet = OnCharge1stGet(pData, wDataSize);
            break;
        }
        case SUB_GP_WEALTH_BAG_TIME:
        {
            bRet = true;
            break;
        }
        case SUB_GPO_WEALTH_BAG_ENROLL:
        {
            bRet = true;
            break;
        }
        case SUB_GP_WEALTH_BAG_BEGIN:
        {
            bRet = true;
            break;
        }
        case SUB_GP_WEALTH_BAG_END:
        {
            bRet = true;
            break;
        }
        case SUB_GP_WEALTH_BAG_RESULT:
        {
            bRet = true;
            break;
        }
        case SUB_GP_WEALTH_BAG_BEFORE://显示福袋倒计时
        {
            bRet = true;
            break;
        }
        case SUB_GPO_SHARE_URL:
        {
            bRet = OnGetShareURL(pData, wDataSize);
            break;
        }
        case SUB_GPO_SHARE_URL2:
        {
            bRet = OnGetShareURL2(pData, wDataSize);
            break;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        case SUB_GPO_IAP_PRODUCT_LIST:
        {
            bRet = OnGetPurchase(pData, wDataSize);
            break;
        }
#endif
        case SUB_GPO_BROADCAST_SPEED_RES:
        {
            CMD_GP_BroadCast_Speed_Res* pBroadCastSpeed = (CMD_GP_BroadCast_Speed_Res*)pData;
            if(pBroadCastSpeed != NULL)
            {
                m_fNoticeSpeed = pBroadCastSpeed->fBroadCastSpeed;
            }
            return true;
        }
        case SUB_GP_MBCODE_VERIFY_RES:
        {
            bRet = OnTelRegisterResult(pData, wDataSize);
            break;
        }
        case SUB_GPR_CHECK_MOBILE_EXIT_RES://是否可以绑定手机号
        {
            bRet = OnCanBandingTelResult(pData, wDataSize);
            break;
        }
        case SUB_GPO_MOBILE_VERIFY_RES://绑定手机号结果
        {
            bRet = OnBandingTelResult(pData, wDataSize);
            break;
        }
        case SUB_GPR_BINGDING_RES:
        {
            bRet = OnBandRewardRes(pData, wDataSize);
            break;
        }
        case SUB_GPO_BROAD_LABA:
        {
            bRet = OnBroadLabaResult(pData, wDataSize);
            break;
        }
        case SUB_GPO_MOBILE_COUNT:
        {
            bRet = OnMobileCount(pData, wDataSize);
            break;
        }
        case SUB_GPO_MOBILE_CHARGE:
        {
            bRet = OnMobileCharge(pData, wDataSize);
            break;
        }
        case SUB_GPO_LABA_TIMES:
        {
            bRet = OnLabaTimes(pData, wDataSize);
            break;
        }//15033029306
        case SUB_GP_BUY_SKILL_RESULT:
        {
            bRet = OnBuySkillResult(pData, wDataSize);
            break;
        }
        case SUB_GPO_QUERY_FAKE_SERVERINFO_RES:
        {
            bRet = OnFakeServerInfoRes(pData, wDataSize);
            break;
        }
        case SUB_GPO_FA_HONG_BAO:
        {
            bRet = OnFaHongBao(pData, wDataSize);
            break;
        }
        case SUB_GPO_ASK_QIANG:
        {
            bRet = OnAskQiang(pData, wDataSize);
            break;
        }
        case SUB_GPO_QIANG_RESULT:
        {
            bRet = OnQiangResult(pData, wDataSize);
            break;
        }
        case SUB_GPO_DAKASAI_NOTICE:
        {
            bRet = OnDakasaiNotice(pData, wDataSize);
            break;
        }
        case SUB_GPO_WUFU_CARD:
        {
            bRet = OnWufuCard(pData, wDataSize);
            break;
        }
        case SUB_GPO_WEEK_BEI:
        {
            bRet = OnWeekBei(pData, wDataSize);
            break;
        }
        case SUB_GPO_QUERY_WHEEL:
        {
            bRet = OnQueryWheel(pData, wDataSize);
            break;
        }
        case SUB_GPO_RECEIVE_WHEEL:
        {
            bRet = OnReceiveWheel(pData, wDataSize);
            break;
        }
            
    }
    return bRet;
}

bool CMainLogic::OnReceiveWheel(const void* pData, WORD wDataSize)
{
    printf("收到转盘结果的消息\n");
    if (wDataSize < sizeof(CMD_GPO_ReceiveWheel))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_ReceiveWheel* pReceiveWheel = (CMD_GPO_ReceiveWheel*)pData;
    if(pReceiveWheel != NULL)
    {
        printf("cbType is %hhu, score is %d, cup is %d\n", pReceiveWheel->cbIndex, pReceiveWheel->nCurrentScore, pReceiveWheel->nCurrentMatchCup);
        if(m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                WheelLayer* pWheelLayer = (WheelLayer*)pHallLayer->getChildByTag(19981);
                if(pWheelLayer != NULL)
                {
                    m_lUserScore = pReceiveWheel->nCurrentScore;
                    pWheelLayer->setWheelPrize(pReceiveWheel->cbIndex, pReceiveWheel->nCurrentMatchCup);
                }
            }
        }
    }
    return true;
}

bool CMainLogic::OnQueryWheel(const void* pData, WORD wDataSize)
{
    printf("收到查询转盘的消息\n");
    if (wDataSize < sizeof(CMD_GPO_QueryWheel))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_QueryWheel* pQueryWheel = (CMD_GPO_QueryWheel*)pData;
    if(pQueryWheel != NULL)
    {
        m_bReceiveShowWheel = true;
        for(int i = 0; i < 10; i++)
        {
            m_cbWheelType[i] = pQueryWheel->cbType[i];
            m_nWheelCount[i] = pQueryWheel->nCount[i];
            printf("%hhu, %d\n", m_cbWheelType[i], m_nWheelCount[i]);
        }
        if(m_bShowWheelNoSign)//如果玩家都签到了，那么就在登录界面弹出幸运转盘
        {
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    WheelLayer* pWheelLayer = WheelLayer::create();
                    pHallLayer -> addChild(pWheelLayer, 199, 19981);
                }
            }
        }
    }
    
    return true;
}

bool CMainLogic::OnWeekBei(const void* pData, WORD wDataSize)
{
    if(!m_bIOSPassed)
        return true;
    if (wDataSize < sizeof(CMD_GPO_WeekBei))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_WeekBei* pWeekBei = (CMD_GPO_WeekBei*)pData;
    if(pWeekBei != NULL)
    {
        if(m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                m_nUserWeekBeiNum = pWeekBei->nWeekBei;
                pHallLayer->setWeekNum(m_nUserWeekBeiNum);
            }
        }
    }
    
    return true;
}

bool CMainLogic::OnWufuCard(const void* pData, WORD wDataSize)
{
    m_pMessageBox->Update();
    if(!m_bIOSPassed)
        return true;
    if (wDataSize < sizeof(CMD_GPO_WufuCard))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_WufuCard* pWufuCard = (CMD_GPO_WufuCard*)pData;
    if(pWufuCard != NULL)
    {
        if(m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                WufuLayer* pWufuLayer = WufuLayer::create(pWufuCard->nCard);
                pHallLayer -> addChild(pWufuLayer, 200, 19983);
            }
        }
        else if(GameMainScene::_instance != NULL)
        {
            //如果是查询弹头数量，那么只显示弹头数量
            if(GameMainScene::_instance->getSelectDantouNum())
            {
                GameMainScene::_instance->setWufuNum(pWufuCard->nCard);
            }
            else
            {
                //停止自动开炮
                if(GameMainScene::_instance->getAutoFireStatus())
                {
                    GameMainScene::_instance->unschedule(schedule_selector(GameMainScene::startBullet));
                }
                WufuLayer* pWufuLayer = WufuLayer::create(pWufuCard->nCard);
                GameMainScene::_instance -> addChild(pWufuLayer, 100, 19983);
                if(GameMainScene::_instance->getMeRealChairID()>=GAME_PLAYER/2)
                {
                    pWufuLayer->setRotation(180);
                }
            }
            
        }
        
    }
    
    return true;
}

bool CMainLogic::OnDakasaiNotice(const void* pData, WORD wDataSize)
{
    if(!m_bIOSPassed)
        return true;
    if (wDataSize < sizeof(CMD_GPO_DakasaiNotice))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_DakasaiNotice* pDakasaiNotice = (CMD_GPO_DakasaiNotice*)pData;
    if(pDakasaiNotice != NULL)
    {
        ShowDakasaiNotice(CCharCode::GB2312ToUTF8(pDakasaiNotice->szDakasaiNotice));
    }
    return true;
}

bool CMainLogic::OnQiangResult(const void* pData, WORD wDataSize)
{
    m_pMessageBox->Update();
    if(!m_bIOSPassed)
        return true;
    if (wDataSize < sizeof(CMD_GPO_QiangResult))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_QiangResult* pQiangResult = (CMD_GPO_QiangResult*)pData;
    if(pQiangResult != NULL)
    {
        if(pQiangResult->bSuccess)
        {
            if(m_pHallScene != NULL)
            {
                m_lUserScore = pQiangResult->nCurrentScore;
                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer->setUserScore(CMainLogic::sharedMainLogic()->m_lUserScore);
                    pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                    SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                }
                
            }
            RedPacketLayer* pRedLayer = RedPacketLayer::create(2);
            CCDirector::sharedDirector()->getRunningScene()->addChild(pRedLayer, 20, 29998);
            pRedLayer -> setReceiveData(pQiangResult->nScore, CCharCode::GB2312ToUTF8(pQiangResult->szContent1), CCharCode::GB2312ToUTF8(pQiangResult->szContent2));
            sendUserBehavior(m_nUserID, eRodPacketedTag);
        }
        else
        {
            RedPacketLayer* pRedLayer = RedPacketLayer::create(4);
            CCDirector::sharedDirector()->getRunningScene()->addChild(pRedLayer, 20, 29998);
            pRedLayer -> setNoRedName(CCharCode::GB2312ToUTF8(pQiangResult->szContent1));
            sendUserBehavior(m_nUserID, eRodNoPacketTag);
        }
    }
    return true;
}

bool CMainLogic::OnAskQiang(const void* pData, WORD wDataSize)
{
    if(!m_bIOSPassed)
        return true;
    if (wDataSize < sizeof(CMD_GPO_AskQiang))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_AskQiang* pAskQiang = (CMD_GPO_AskQiang*)pData;
    if(pAskQiang != NULL)
    {
        if(pAskQiang->dwUserID == 0 || pAskQiang->dwUserID == m_nUserID)
        {
            if(m_pHallScene != NULL || GameMainScene::_instance != NULL)
            {
                RedPacketLayer* pRedLayer = RedPacketLayer::create(3, pAskQiang->dwHongBaoID);
                CCDirector::sharedDirector()->getRunningScene()->addChild(pRedLayer, 20, 29998);
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eReceiveRedPacket);
                int index = rand()%2;
                if(index == 0)
                {
                    SimpleAudioEngine::sharedEngine()->playEffect("Music/redqiang1.mp3");
                }
                else
                {
                    SimpleAudioEngine::sharedEngine()->playEffect("Music/redqiang2.mp3");
                }
            }
        }
    }
    
    return true;
}

bool CMainLogic::OnFaHongBao(const void* pData, WORD wDataSize)
{
    m_pMessageBox->Update();
    if (wDataSize < sizeof(CMD_GPO_FaHongBao))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_FaHongBao* pFaHongBao = (CMD_GPO_FaHongBao*)pData;
    if(pFaHongBao != NULL)
    {
        if(pFaHongBao->bSuccess)//发送红包成功
        {
            m_lUserScore = pFaHongBao->nCurrentScore;
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer->setUserScore(m_lUserScore);
                }
            }
        }
        else//发送红包失败
        {
            
        }
        ShowMessage(CCharCode::GB2312ToUTF8(pFaHongBao->szContent).c_str(), eMBOK);
    }
    
    return true;
}

        
bool CMainLogic::OnFakeServerInfoRes(const void* pData, WORD wDataSize)
{
    if(!m_bIOSPassed)
        return true;
    CMD_GPR_Query_Fake_Serverinfo_Res* pFakeServerinfoRes = (CMD_GPR_Query_Fake_Serverinfo_Res*)pData;
    if(pFakeServerinfoRes != NULL)
    {
        for(int i = 0; i < pFakeServerinfoRes->wServerCnt; i++)
        {
            if (m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer->updateRoomUserCount(pFakeServerinfoRes->aFakeServerInfo[i].wServerPort, pFakeServerinfoRes->aFakeServerInfo[i].wOnlineUserCnt);
                }
            }
        }
    }
    return true;
}

bool CMainLogic::OnBuySkillResult(const void* pData, WORD wDataSize)
{
    m_pMessageBox->Update();
    if (wDataSize < sizeof(CMD_GPO_BuySkill_Result))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_BuySkill_Result* pBuySkillResult = (CMD_GPO_BuySkill_Result*)pData;
    if(pBuySkillResult != NULL)
    {
        if(pBuySkillResult->bSuccess)
        {
            m_lUserScore = pBuySkillResult->llScore;
            if(pBuySkillResult->cbSkillID == 0)
            {
                m_nSpeedUpTimes = pBuySkillResult->nCount;
            }
            else if(pBuySkillResult->cbSkillID == 1)
            {
                m_nSansheTimes= pBuySkillResult->nCount;
            }
            else if(pBuySkillResult->cbSkillID == 2)
            {
                m_nBaojiTimes = pBuySkillResult->nCount;
            }
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer -> setUserScore(m_lUserScore);//更新大厅分数
                    DaojuLayer* pDaojuLayer = (DaojuLayer*)pHallLayer->getChildByTag(19987);
                    if(pDaojuLayer != NULL)
                    {
                        pDaojuLayer->updateSkillUI(pBuySkillResult->cbSkillID, pBuySkillResult->nCount);//更新背包界面
                        BuyBarrelLayer* pBuyBarrelLayer = (BuyBarrelLayer*)pDaojuLayer->getChildByTag(1009);
                        if(pBuyBarrelLayer != NULL)
                        {
                            pBuyBarrelLayer -> closeWnd(NULL);//关闭兑换界面
                        }
                    }
                }
            }
        }
        ShowMessage(CCharCode::GB2312ToUTF8(pBuySkillResult->szDescribeString).c_str(), eMBOK);
    }
    return true;
}

bool CMainLogic::OnLabaTimes(const void* pData, WORD wDataSize)
{
    m_pMessageBox->Update();
    if (wDataSize < sizeof(CMD_GPO_LabaTimes))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_LabaTimes* pLabaTimes = (CMD_GPO_LabaTimes*)pData;
    if(pLabaTimes != NULL)
    {
        if(m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create(3);
                pHallLayer -> addChild(pFirstChargeLayer, 201, 19986);
                pFirstChargeLayer -> setLabaLastTime(pLabaTimes->nLabaTimes, pLabaTimes->nEnrollmentfee);
            }
        }
        else if(GameMainScene::_instance != NULL)//游戏内打开喇叭
        {
            GameMainScene::_instance->showLabaUI(pLabaTimes->nLabaTimes, pLabaTimes->nEnrollmentfee);
        }
    }
    
    return true;
}

bool CMainLogic::OnBandRewardRes(const void* pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GPR_BindRewadRes))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPR_BindRewadRes* pBindRewardRes = (CMD_GPR_BindRewadRes*)pData;
    if(pBindRewardRes != NULL)
    {
        m_nBindReward = pBindRewardRes->nBindReward;
        m_nShareReward = pBindRewardRes->nShareReward;
        m_cbSuperPirateGuided = pBindRewardRes->cbNewGuid;
    }
    return true;
}

bool CMainLogic::OnMobileCharge(const void* pData, WORD wDataSize)
{
    return true;
}

bool CMainLogic::OnMobileCount(const void* pData, WORD wDataSize)
{
    m_pMessageBox->Update();
    if (wDataSize < sizeof(CMD_GPO_MobileCount))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_MobileCount* pMobileCount = (CMD_GPO_MobileCount*)pData;
    if(pMobileCount != NULL)
    {
        if(m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                LotteryLayer* pLotteryLayer = (LotteryLayer*)pHallLayer->getChildByTag(19989);
                if(pLotteryLayer != NULL)
                {
                    pLotteryLayer -> setLabelMoney(pMobileCount->nCount);
                }
                
            }
        }
        else if(GameMainScene::_instance != NULL)
        {
            LotteryLayer* pLotteryLayer = (LotteryLayer*)GameMainScene::_instance->getChildByTag(19989);
            if(pLotteryLayer != NULL)
            {
                pLotteryLayer -> setLabelMoney(pMobileCount->nCount);
            }
        }
        
    }
    return true;
}

bool CMainLogic::OnBroadLabaResult(const void* pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GPO_BroadLaba))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_BroadLaba* pBroadLaba = (CMD_GPO_BroadLaba*)pData;
    if(pBroadLaba != NULL)
    {
        if(pBroadLaba->cbSuccess == 1)//成功
        {
            //ShowMessage(CCharCode::GB2312ToUTF8(pBroadLaba->szContent).c_str(), eMBOK, eCloseDuihuan);
            ShowMessage(CCharCode::GB2312ToUTF8(pBroadLaba->szContent).c_str(), eMBOK);
            //更新当前分数
            m_lUserScore = pBroadLaba->lUserScore;
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer -> setUserScore(m_lUserScore);
                    FirstChargeLayer* pFirstChargeLayer = (FirstChargeLayer*)pHallLayer->getChildByTag(19986);
                    if(pFirstChargeLayer != NULL)
                    {
                        pFirstChargeLayer -> setLabaLabel();
                    }
                }
            }
            sendUserBehavior(m_nUserID, eHallSendLabaSuccess);
        }
        else//失败
        {
            sendUserBehavior(m_nUserID, eHallSendLabaFail);
            ShowMessage(CCharCode::GB2312ToUTF8(pBroadLaba->szContent).c_str(), eMBOK);
        }
    }
    return true;
}

bool CMainLogic::OnBandingTelResult(const void* pData, WORD wDataSize)
{
    if (wDataSize < sizeof(DBO_GP_MBBindRes))
    {
        ERROR_CHECK;
        return false;
    }
    DBO_GP_MBBindRes* pMBBindRes = (DBO_GP_MBBindRes*)pData;
    if(pMBBindRes != NULL)
    {
        if(pMBBindRes->cbResult == 1)//成功，需要更改密码为刚修改过的
        {
            m_lUserScore = pMBBindRes->nCurrentGold;
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer->setUserScore(m_lUserScore);
                }
            }
            sendUserBehavior(m_nUserID, eLingquSuccess);
            m_sPassword = m_sModifyPassword;
            if(CCUserDefault::sharedUserDefault()->getBoolForKey("NewGuestBank"))
            {
                CCUserDefault::sharedUserDefault()->setStringForKey("NewGuestPassword", m_sModifyPassword);
            }
            else
            {
                CCUserDefault::sharedUserDefault()->setStringForKey("NewPassword", m_sModifyPassword);
                //判断当前账号和游客账号是否一致，如果一致的话，那么也修改游客密码
                if(CCUserDefault::sharedUserDefault()->getStringForKey("NewAccount") == CCUserDefault::sharedUserDefault()->getStringForKey("NewGuestAccount"))
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("NewGuestPassword", m_sModifyPassword);
                }
            }
            m_bCanBandingTel = false;
            //接收加包信息
            void* pDataBuffer = NULL;
            tagDataDescribe DataDescribe;
            CRecvPacketHelper RecvPacket(pMBBindRes+1, wDataSize-sizeof(DBO_GP_MBBindRes));
            while(true)
            {
                //提取数据
                pDataBuffer = RecvPacket.GetData(DataDescribe);
                if(DataDescribe.wDataDescribe == DTP_NULL)
                {
                    break;
                }
                switch (DataDescribe.wDataDescribe) {
                    case DTP_GP_UI_NICKNAME://邮件内容
                    {
                        std::string sNickName = (char*)pDataBuffer;
                        if(m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                m_sNickName = CCharCode::GB2312ToUTF8(sNickName.c_str());
                                pHallLayer->setUserNickName(m_sNickName.c_str());
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                    SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                }
            }
            CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(pMBBindRes->szDescribeString).c_str(), eMBOK, eDeleteUserInfoLayer);
            
            //将心形隐藏
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    if(m_nLogonGift != 0 || m_bCanShare || m_cbCanGetBankruptcy>0 || !m_bReceiveVipLogon || m_bCanBandingTel)
                    {
                        pHallLayer->showfreecoinAction(true);
                    }
                    else
                    {
                        pHallLayer->showfreecoinAction(false);
                    }
                }
            }
        }
        else
        {
            CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(pMBBindRes->szDescribeString).c_str(), eMBOK);
            sendUserBehavior(m_nUserID, eLingquFail);
        }
        
    }
    return true;
}

bool CMainLogic::OnCanBandingTelResult(const void* pData, WORD wDataSize)
{
    if (wDataSize < sizeof(DBR_GP_CHECK_BINDNUM_EXIT_RES))
    {
        ERROR_CHECK;
        return false;
    }
    DBR_GP_CHECK_BINDNUM_EXIT_RES* pBindNumExitRes = (DBR_GP_CHECK_BINDNUM_EXIT_RES*)pData;
    if(pBindNumExitRes != NULL)
    {
        if(pBindNumExitRes->cbIsExit == 1)//存在，不能绑定
        {
            m_bCanBandingTel = false;
        }
        else//不存在，可以绑定
        {
            m_bCanBandingTel = true;
        }
        //显示心形
        if(m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                if(m_nLogonGift != 0 || m_bCanShare || m_cbCanGetBankruptcy>0 || !m_bReceiveVipLogon || m_bCanBandingTel)
                {
                    pHallLayer->showfreecoinAction(true);
                }
            }
        }
    }
    
    
    return true;
}

bool CMainLogic::OnTelRegisterResult(const void* pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GPO_MBVerificationCode))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_MBVerificationCode* pMBVerificationCode = (CMD_GPO_MBVerificationCode*)pData;
    if(pMBVerificationCode != NULL)
    {
        //printf("%d, %s\n", pMBVerificationCode->bIsVerificationCodeSend, CCharCode::GB2312ToUTF8(pMBVerificationCode->szDescribeString).c_str());
        //printf("%d, %s\n", pMBVerificationCode->bIsVerificationCodeSend, pMBVerificationCode->szDescribeString);
        if(!pMBVerificationCode->bIsVerificationCodeSend)//获取验证码失败
        {
            CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(pMBVerificationCode->szDescribeString).c_str());
            //将注册界面的验证码按钮设置为可以点击的，并且停止定时器
            if(m_pLogonScene != NULL)
            {
                CLogonLayer* pLogonLayer = (CLogonLayer*)m_pLogonScene->getChildByTag(eLogonLayerTag);
                if(pLogonLayer != NULL)
                {
                    pLogonLayer->resetStatus();
                }
            }
            else if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    FreeCoinLayer* pFreeCoinLayer = (FreeCoinLayer*)pHallLayer->getChildByTag(19996);
                    if(pFreeCoinLayer != NULL)
                    {
                        pFreeCoinLayer->resetStatus();
                    }
                }
            }
        }
    }
    return true;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
bool CMainLogic::OnGetPurchase(const void* pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GPR_IAPProductList))
    {
        ERROR_CHECK;
        return false;
    }
    m_pMessageBox->Update();
    m_pMessageBox->Update();
    
    CMD_GPR_IAPProductList* IAPProductList = (CMD_GPR_IAPProductList*)pData;
    //创建充值界面
    CCNode * pNode = CCDirector::sharedDirector()->getRunningScene();
    if(pNode != NULL)
    {
        int nOpenType = 0;
        ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
        if(pChargeLayer != NULL)
        {
            nOpenType = pChargeLayer->m_nOpenCharge;
        }
        Charge2Layer* pCharge2Layer = Charge2Layer::create(IAPProductList, nOpenType);
        pNode -> addChild(pCharge2Layer, 11, 20002);
        if(m_nBuyCannon2Index>=12&&m_nBuyCannon2Index<=15)//购买指定炮台
        {
            pCharge2Layer->setVisible(false);
            if(GameMainScene::_instance != NULL)
            {
                m_bGameChargeStatus = true;
                m_nBuyGameType = 2;
                m_nShowChargeType = 4;
                GameMainScene::_instance->exitGame();
            }
        }
        else if(m_nBuyCannon2Index>=31&&m_nBuyCannon2Index<=34)//购买使用炮
        {
            pCharge2Layer->setVisible(false);
            if(GameMainScene::_instance != NULL)
            {
                m_bGameChargeStatus = true;
                m_nBuyGameType = 2;
                m_nShowChargeType = 4;
                GameMainScene::_instance->exitGame();
                
            }
        }
    }
    return true;
}
#endif

bool CMainLogic::OnSocketNoticeMessage(const TCP_Command & Command, const void* pData, unsigned short wDataSize)
{
    bool bRet = false;
    
    switch (Command.wSubCmdID)
    {
        case SUB_GPO_VIPUPGRADE://获取VIP公告
        {
            bRet = true;
            if(KIND_ID == FB_KIND_ID)
                break;
            CMD_CS_C_VIPUpgrade* pVIPUpgrade = (CMD_CS_C_VIPUpgrade *)pData;
            std::string sContent = CCharCode::GB2312ToUTF8(pVIPUpgrade->szTrumpetContent);
            if(pVIPUpgrade->cbType == 35)//比赛公告
            {
                if(m_bIOSPassed)
                {
                    if(sContent != "")
                    {
                        ShowLaba2Notice(sContent, pVIPUpgrade->cbType);
                    }
                }
            }
            else
            {
                //判断玩家是在大厅还是在游戏
                if(m_pHallScene != NULL)//在大厅
                {
                    if(sContent!="")
                    {
                        addNoticeToVct(pVIPUpgrade->cbType, sContent);
                    }
                }
                else if(GameMainScene::_instance != NULL)//在游戏中
                {
                    if(m_bIOSPassed)
                    {
                        if(sContent != "")
                        {
                            ShowNotice(sContent, pVIPUpgrade->cbType);
                        }
                    }
                }
            }
            break;
        }
        case SUB_GPR_BOARDCAST://系统公告
        {
            CMD_CS_C_VIPUpgrade* pVIPUpgrade = (CMD_CS_C_VIPUpgrade *)pData;
            std::string sContent = CCharCode::GB2312ToUTF8(pVIPUpgrade->szTrumpetContent);
            
            //判断玩家是在大厅还是在游戏
            if(m_pHallScene != NULL)//在大厅
            {
                if(sContent != "")
                {
                    addNoticeToVct(pVIPUpgrade->cbType, sContent);
                }
            }
            else if(GameMainScene::_instance != NULL)//在游戏中
            {
                if(m_bIOSPassed)
                {
                    ShowNotice(sContent, pVIPUpgrade->cbType);
                }
            }
            bRet = true;
            break;
        }
        case SUB_GP_SystemBroadLaBa:
        {
            CMD_GPR_BroadLaba* pBroadLaba = (CMD_GPR_BroadLaba*)pData;
            if(pBroadLaba != NULL)
            {
                ShowLabaNotice(CCharCode::GB2312ToUTF8(pBroadLaba->szContent).c_str(), pBroadLaba->szNickName);
            }
            bRet = true;
            break;
        }
    }
    return bRet;
}

bool CMainLogic::OnGetShareURL2(const void * pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GPR_ShareURL))
    {
        ERROR_CHECK;
        return false;
    }
    m_pMessageBox->Update();
    CMD_GPR_ShareURL2* pShareURL = (CMD_GPR_ShareURL2*)pData;
    if(pShareURL != NULL)
    {
        //判断当前是那个界面：免费金币界面、兑换商城界面
        if(GameMainScene::_instance != NULL)
        {
            GameMainScene::_instance->startShare(pShareURL->szURL, CCharCode::GB2312ToUTF8(pShareURL->szContent));
        }
    }
    return true;
}

bool CMainLogic::OnGetShareURL(const void * pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GPR_ShareURL))
    {
        ERROR_CHECK;
        return false;
    }
    m_pMessageBox->Update();
    CMD_GPR_ShareURL* pShareURL = (CMD_GPR_ShareURL*)pData;
    if(pShareURL != NULL)
    {
        CRankNewLayer* pRankNewLayer = (CRankNewLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10200);
        if(pRankNewLayer != NULL)
        {
            pRankNewLayer->sendShareInfo(pShareURL->szURL, CCharCode::GB2312ToUTF8(pShareURL->szContent));
            return true;
        }
        //判断当前是那个界面：免费金币界面、兑换商城界面
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                FreeCoinLayer* pFreeCoinLayer = (FreeCoinLayer*)pHallLayer->getChildByTag(19996);
                if(pFreeCoinLayer != NULL)
                {
                    pFreeCoinLayer->sendShareInfo(pShareURL->szURL, CCharCode::GB2312ToUTF8(pShareURL->szContent));
                    return true;
                }
            }
        }
        else if(GameMainScene::_instance != NULL)
        {
            CSendPrizeLayer* pSendPrizeLayer = (CSendPrizeLayer*)GameMainScene::_instance->getChildByTag(eCmptSendPrizeTag);
            if(pSendPrizeLayer != NULL)
            {
                pSendPrizeLayer->sendShareInfo(pShareURL->szURL, CCharCode::GB2312ToUTF8(pShareURL->szContent));
                return true;
            }
            else
            {
                GameMainScene::_instance->startShare(pShareURL->szURL, CCharCode::GB2312ToUTF8(pShareURL->szContent));
            }
            
        }
    }
    return true;
}

bool CMainLogic::OnCharge1stGet(const void * pData, WORD wDataSize)//######
{
    if (wDataSize < sizeof(CMD_GPO_Charge1stGet))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_Charge1stGet* pCharge1stGet = (CMD_GPO_Charge1stGet*)pData;
    if(pCharge1stGet != NULL)
    {
        ShowMessage(CCharCode::GB2312ToUTF8(pCharge1stGet->szDescription).c_str());
        if(pCharge1stGet->cbSuccess == 1)//领取成功
        {
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    m_bFirstChargeCanGet = false;
                    pHallLayer->setUserScore(pCharge1stGet->lCurrentScore);//更新当前分数
                    pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                    SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                    pHallLayer->showLastTimes(pCharge1stGet->cbCount);//显示剩余领取次数
                }
            }
        }
        else
        {
            
        }
    }
    return true;
}

bool CMainLogic::OnCharge1stRequest(const void * pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GPO_Charge1stRequest))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_Charge1stRequest* pCharge1stRequest = (CMD_GPO_Charge1stRequest*)pData;
    if(pCharge1stRequest != NULL)
    {
        //当前领取第几次，返回值为1
        BYTE cbCount = pCharge1stRequest->cbCount;
        if(cbCount)
        {
            //验证首充成功，显示界面
            char temp[128];
            sprintf(temp, "恭喜您首次充值成功，获取大礼包");
            ShowMessage(temp, eMBOK);
            //更新大厅界面
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer->showLastTimes(cbCount);
                }
            }
        }
        else//验证首充失败，显示界面
        {
            ShowMessage(CCharCode::GB2312ToUTF8(pCharge1stRequest->szDescription).c_str());
        }
    }
    return true;
}

//领取登录奖励
bool CMainLogic::OnGetLogonGift(const void * pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GPO_LogonGift))
    {
        ERROR_CHECK;
        return false;
    }
    
    CMD_GPO_LogonGift* pLogonGift = (CMD_GPO_LogonGift*)pData;
    //printf("pLogonGift is %lld\n", pLogonGift->llCurrentScore);
    if(pLogonGift != NULL)
    {
        if(pLogonGift->llCurrentScore == -1)//领取失败
        {
            ShowMessage(CCharCode::GB2312ToUTF8(pLogonGift->szDescription).c_str(), eMBOK);
        }
        else
        {
            m_lUserScore = pLogonGift->llCurrentScore;
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    if(m_nLogonGift != 0 || !m_bReceiveVipLogon)
                    {
                        
                    }
                    else
                    {
                        FreeCoinLayer* pFreeCoinLayer = (FreeCoinLayer*)pHallLayer->getChildByTag(19996);
                        if(pFreeCoinLayer != NULL)
                        {
                            pFreeCoinLayer->stopLogonItem();
                        }
                    }
                    
                }
            }
        }
    }
    
    return true;
}

bool CMainLogic::OnGetCurrentScore(const void * pData, WORD wDataSize)
{
    //更新大厅界面上玩家分数、VIP等级、奖券数量
    if(m_nCheckSocketShut == 10 || m_nClickBank == 10)
    {
        m_pMessageBox->Update();
    }
    if (wDataSize < sizeof(CMD_GPO_CurrentScore))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_CurrentScore* pCurrentScore = (CMD_GPO_CurrentScore*)pData;
    m_lUserScore = pCurrentScore->lCurrentScore;
    m_nLottery = pCurrentScore->nLotteryCount;
    m_nVipLevel = pCurrentScore->cbVIPLevel;
    m_nVipExp = pCurrentScore->dwVIPExp;
    m_nExpPercent = pCurrentScore->dwExpPerCent;
    m_nChargeBack = pCurrentScore->dwChargeBack;
    m_cbCountTimes = pCurrentScore->cbCount;
    m_bFirstChargeCanGet = pCurrentScore->cbCanGet;
    if(m_cbCountTimes != 0)//不能首充，所以充值界面上“查看VIP”按钮
    {
        ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
        if(pChargeLayer != NULL)
        {
            pChargeLayer -> setFirstChargeStatus();
        }
    }
    //判断VIP有没有升级
    if(CCUserDefault::sharedUserDefault()->getIntegerForKey("USERVIPLEVEL") != m_nVipLevel)
    {
        //发送VIP升级消息
        sendVIPGrade();
        requestMail();
        //m_bReceiveVipLogon = false;
        CCUserDefault::sharedUserDefault()->setIntegerForKey("USERVIPLEVEL", m_nVipLevel);
    }
    if(m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            pHallLayer -> setNickNameColor();
            pHallLayer -> setUserScore(m_lUserScore);
            pHallLayer -> setTicket(m_nLottery);
            pHallLayer -> setVipExp();
            if(pHallLayer->getInitCompleted())
            {
                pHallLayer -> showFirstChargeAnimate(true);
            }
            
            if(m_nChargeBack != 0)
            {
                pHallLayer->showVIPAction(true);
            }
            else
            {
                pHallLayer->showVIPAction(false);
            }
        }
    }
    return true;
}

//VIP
bool CMainLogic::OnVIPInfo(const void * pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GPO_VIPInfo))
    {
        ERROR_CHECK;
        return false;
    }
    if(!m_bClickIcon)
    {
        m_pMessageBox->Update();
        m_pMessageBox->Update();
    }
    
    
    CMD_GPO_VIPInfo* pVIPInfo = (CMD_GPO_VIPInfo*)pData;
    m_nVipLogonGift = pVIPInfo->dwEverydayGift;
    m_nChargeBack = pVIPInfo->dwChargeBack;
    if(m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene -> getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            if(m_nChargeBack != 0)
            {
                pHallLayer->showVIPAction(true);
            }
            else
            {
                pHallLayer->showVIPAction(false);
            }
        }
    }
    
    if(m_nVipLogonGift == 0)
    {
        //m_bReceiveVipLogon = true;//当前没有金币可以领取，或者是已经领取完了
    }
    else
    {
        //m_bReceiveVipLogon = false;
    }
    m_bIsOpenLogon = true;
    if(m_nLogonGift == 0)
    {
        return true;
    }
    
    //判断当前界面上是否有广告
    if(m_bHaveAdvert)
    {
        m_bIsShowLogonGift = true;
        return true;
    }
    
    if(m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene -> getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            if(m_nChargeBack != 0)
            {
                pHallLayer->showVIPAction(true);
            }
            else
            {
                pHallLayer->showVIPAction(false);
            }
            if(m_nCurrentViewIndex == 1)
            {
                
            }
            else
            {
                //先判断大厅初始化是否完成，如果没有完成，则当完成之后再显示出来
                //如果大厅初始化已经完成，则直接显示出来
                m_bShowChouJiang = true;
                if(pHallLayer->m_bEnterHall)
                {
                    if(CCUserDefault::sharedUserDefault()->getStringForKey("ShowLogonGift", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("logon"))
                    {
                        CCUserDefault::sharedUserDefault()->setStringForKey("ShowLogonGift", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("logon"));
                    }
                    m_bShowChouJiang = false;
                }
            }
            m_nCurrentViewIndex = 3;
        }
    }
    return true;
}

bool CMainLogic::OnVIPResult(const void * pData, WORD wDataSize)
{
    m_pMessageBox->Update();
    
    CMD_GPO_VIPResult* pVIPResult = (CMD_GPO_VIPResult*)pData;
    SCORE lscore = pVIPResult->lScore;
    bool bIsClick = false;
    if(lscore != -1)
    {
        m_lUserScore = lscore;
        bIsClick = false;
    }
    else
    {
        bIsClick = true;
        ShowMessage(CCharCode::GB2312ToUTF8(pVIPResult->szDescription).c_str(), eMBOK);
        return true;
    }
    
    
    //将按钮变为灰色状态
    if(m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            pHallLayer -> setUserScore(m_lUserScore);
            
            if(m_nCurrentInstruct == SUB_GPR_VIP_EVERYDAY_GIFT)
            {
                if(m_nLogonGift != 0 || m_bCanShare || m_cbCanGetBankruptcy>0 || !m_bReceiveVipLogon || m_bCanBandingTel)
                {
                    pHallLayer->showfreecoinAction(true);
                }
                else
                {
                    pHallLayer->showfreecoinAction(false);
                }
                if(m_nLogonGift != 0 || !m_bReceiveVipLogon)
                {
                    
                }
                else
                {
                    FreeCoinLayer* pFreeCoinLayer = (FreeCoinLayer*)pHallLayer->getChildByTag(19996);
                    if(pFreeCoinLayer != NULL)
                    {
                        pFreeCoinLayer->stopLogonItem();
                    }
                }
            }
        }
    }
    
    return true;
}

//银行查询
bool CMainLogic::onQueryInsureInfo(const void * pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GP_UserInsureInfo))
    {
        ERROR_CHECK;
        return false;
    }
    m_pMessageBox->Update();
    m_pMessageBox->Update();
    
    
    return true;
}

bool CMainLogic::OnInsureSuccess(const void * pData, WORD wDataSize)
{
    CMD_GP_UserInsureSuccess * pUserInsureSuccess=(CMD_GP_UserInsureSuccess *)pData;
    if (wDataSize < (sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)))
    {
        return false;
    }
    
    return true;
}

bool CMainLogic::OnInsureFail(const void * pData, WORD wDataSize)
{
    CMD_GP_UserInsureFailure * pUserInsureFailure=(CMD_GP_UserInsureFailure *)pData;
    if (wDataSize<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)))
    {
        return false;
    }
    
    if(CCharCode::GB2312ToUTF8(pUserInsureFailure->szDescribeString) != "")
    {
        ShowMessage(CCharCode::GB2312ToUTF8(pUserInsureFailure->szDescribeString).c_str(),eMBOK,eLeaveHall,true);
    }
    else
    {
        ShowMessage("非常抱歉，本次赠送失败了，再试一次吧。", eMBOK);
    }
    
    
    return true;
}

bool CMainLogic::OnTransferUserInfo(const void * pData, WORD wDataSize)
{
    CMD_GP_UserTransferUserInfo * pTransferUserInfo=(CMD_GP_UserTransferUserInfo *)pData;
    
    return true;
}

bool CMainLogic::OnTransferSuccess(const void * pData, WORD wDataSize)
{
    DBO_GP_UserInsureTransferInfo * pTransferUserInfo=(DBO_GP_UserInsureTransferInfo *)pData;
    char temp[256];
    sprintf(temp, "[%s]于［%d-%d-%d %d:%d:%d]赠送给玩家[%s（ID:%d）]%lld游戏币!", pTransferUserInfo->szSourceName, pTransferUserInfo->timeTransfer.wYear,pTransferUserInfo->timeTransfer.wMonth, pTransferUserInfo->timeTransfer.wDay, pTransferUserInfo->timeTransfer.wHour, pTransferUserInfo->timeTransfer.wMinute, pTransferUserInfo->timeTransfer.wSecond, CCharCode::GB2312ToUTF8(pTransferUserInfo->szTargetName).c_str(), pTransferUserInfo->dwTargetID, pTransferUserInfo->lSwapScore);
    ShowMessage(temp, eMBOK);
    
    return true;
}

bool compare1(tagMailInfo obj1, tagMailInfo obj2)
{
    if (obj1.dwMailID < obj2.dwMailID)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CMainLogic::OnGetMailListID(const void * pData, WORD wDataSize)//获取邮件ID
{
    if (wDataSize < sizeof(CMD_GPO_MailIDList))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_MailIDList* MailIDList = (CMD_GPO_MailIDList*)pData;
    for(int i = 0; i < MailIDList->cbCount; i++)
    {
        bool bExist = false;
        for(int j = 0; j < m_vctMail.size(); j++)
        {
            if(MailIDList->dwMailID[i] == m_vctMail[j].dwMailID)
            {
                bExist = true;
                break;
            }
        }
        if(!bExist)
        {
            if(m_vctMail.size() == 30)
            {
                m_vctMail.erase(m_vctMail.begin());
            }
            tagMailInfo MailInfo = {0};
            MailInfo.dwMailID = MailIDList->dwMailID[i];
            MailInfo.cbMailStatus = false;
            MailInfo.cbGeted = false;
            MailInfo.szMailTime = "";
            MailInfo.szMailTitle = "";
            MailInfo.szMailContent = "";
            MailInfo.szDuihuanma = "";
            MailInfo.szGongzhonghao = "";
            m_vctMail.push_back(MailInfo);
        }
    }
    //对vct中进行前后位置兑换
    std::sort(m_vctMail.begin(), m_vctMail.end(), compare1);
    
    
    //去请求标题
    //点击按钮，向服务端发送请求，内容是容器中id后边对应的标题没有内容，说明这些还没有请求过
    CMD_GPR_MailInfo MailInfo;
    int count = 0;
    for(unsigned int i = 0; i < m_vctMail.size(); i++)
    {
        if(m_vctMail[i].cbGeted == 3)//系统邮件
        {
            if(m_vctMail[i].szMailContent == "")//要获取系统邮件
            {
                MailInfo.dwMailID[count] = m_vctMail[i].dwMailID;
                count++;
            }
        }
        else if(m_vctMail[i].cbMailStatus == 0 || m_vctMail[i].cbGeted == 1)
        {
            MailInfo.dwMailID[count] = m_vctMail[i].dwMailID;
            count++;
        }
    }
    MailInfo.cbCount = count;
    SendData(m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_QUERY_MAIL_INFO, &MailInfo, sizeof(DWORD)*count+sizeof(BYTE));
    return true;
}

bool CMainLogic::onGetMailTitle(const void* pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GPO_MailInfo))
    {
        ERROR_CHECK;
        return false;
    }
    
    char * pBuffer = (char*)pData;
    unsigned int* pCount = (unsigned int*)pBuffer;
    CMD_GPO_MailInfo* pMailInfo = (CMD_GPO_MailInfo*)&pBuffer[4];
    for(unsigned int i = 0; i < *pCount; i++)
    {
        for(int j = 0; j < m_vctMail.size(); j++)
        {
            if(m_vctMail[j].dwMailID == pMailInfo->dwMailID)
            {
                m_vctMail[j].cbGeted = pMailInfo->cbGeted;
                m_vctMail[j].cbMailStatus = pMailInfo->cbMailStatus;
                m_vctMail[j].szMailTitle = CCharCode::GB2312ToUTF8(pMailInfo->szMailTitle);
                m_vctMail[j].szMailTime = CCharCode::GB2312ToUTF8(pMailInfo->szMailTime);
            }
        }
        pMailInfo++;
    }
    int count = 0;
    for(int i = 0; i < m_vctMail.size(); i++)
    {
        //如果邮件未读也没有领取功能则＋＋， 如果邮件已读并且有领取功能但是未领取也＋＋
        if((!m_vctMail[i].cbMailStatus && m_vctMail[i].cbGeted == 0) || (m_vctMail[i].cbMailStatus && m_vctMail[i].cbGeted == 1) || (!m_vctMail[i].cbMailStatus && m_vctMail[i].cbGeted == 1) || (m_vctMail[i].cbGeted == 3 && m_vctMail[i].szMailContent == ""))
        {
            count++;
        }
    }
    m_nMailCount = count;
    if(m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            pHallLayer -> setMailNumber(m_nMailCount);
        }
    }
    else if(GameMainScene::_instance != NULL)
    {
        GameMainScene::_instance -> setMailNumber(m_nMailCount);
    }
    
    m_bFirstRequestMail = true;
    
    return true;
}

bool CMainLogic::onGetMailContent(const void * pData, WORD wDataSize)
{
    MailLayer* pMailLayer = NULL;
    if(m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            //刷新当前分数
            pHallLayer -> setUserScore(m_lUserScore);
            pMailLayer = (MailLayer*)pHallLayer->getChildByTag(19998);
            if(pMailLayer == NULL)
            {
                return false;
            }
        }
    }
    else if(GameMainScene::_instance != NULL)
    {
        pMailLayer = (MailLayer*)GameMainScene::_instance->getChildByTag(19998);
        if(pMailLayer == NULL)
        {
            return false;
        }
    }
    
    if (wDataSize < sizeof(CMD_GPO_MailContexts))
    {
        ERROR_CHECK;
        return false;
    }
    char* pContain = NULL;
    CMD_GPO_MailContexts* pMailContexts = (CMD_GPO_MailContexts*)pData;
    if(pMailContexts->cbGetMailContext == 1)
    {
        //接收加包信息
        void* pDataBuffer = NULL;
        tagDataDescribe DataDescribe;
        CRecvPacketHelper RecvPacket(pMailContexts+1, wDataSize-sizeof(CMD_GPO_MailContexts));
        while(true)
        {
            //提取数据
            pDataBuffer = RecvPacket.GetData(DataDescribe);
            if(DataDescribe.wDataDescribe == DTP_NULL)
            {
                break;
            }
            switch (DataDescribe.wDataDescribe) {
                case DTP_GP_MAIL_CONTENT://邮件内容
                {
                    char * pBuffer = (char*)pDataBuffer;
                    pContain = (char*)&pBuffer[4];
                    break;
                }
                case DTP_GP_MAIL_CHUJI_DRAGON://珍珠等级
                {
                    if (DataDescribe.wDataSize != sizeof(CMD_GPO_ItemInfo))
                    {
                        ERROR_CHECK;
                        return false;
                    }
                    CMD_GPO_ItemInfo* pItemInfo = (CMD_GPO_ItemInfo*)pDataBuffer;
                    char nameStr[32];
                    sprintf(nameStr, "h3all/bbtongdantou.png");
                    pMailLayer->setMailItem(nameStr, pItemInfo->nItemCount);
                    break;
                }
                case DTP_GP_MAIL_ZHONGJI_DRAGON:
                {
                    if (DataDescribe.wDataSize != sizeof(CMD_GPO_ItemInfo))
                    {
                        ERROR_CHECK;
                        return false;
                    }
                    CMD_GPO_ItemInfo* pItemInfo = (CMD_GPO_ItemInfo*)pDataBuffer;
                    char nameStr[32];
                    sprintf(nameStr, "h3all/bbyindantou.png");
                    pMailLayer->setMailItem(nameStr, pItemInfo->nItemCount);
                    break;
                }
                case DTP_GP_MAIL_GAOJI_DRAGON:
                {
                    if (DataDescribe.wDataSize != sizeof(CMD_GPO_ItemInfo))
                    {
                        ERROR_CHECK;
                        return false;
                    }
                    CMD_GPO_ItemInfo* pItemInfo = (CMD_GPO_ItemInfo*)pDataBuffer;
                    char nameStr[32];
                    sprintf(nameStr, "h3all/bbjindantou.png");
                    pMailLayer->setMailItem(nameStr, pItemInfo->nItemCount);
                    break;
                }
                case DTP_GP_MAIL_SKILL_JS://技能
                {
                    if (DataDescribe.wDataSize != sizeof(CMD_GPO_ItemInfo))
                    {
                        ERROR_CHECK;
                        return false;
                    }
                    CMD_GPO_ItemInfo* pItemInfo = (CMD_GPO_ItemInfo*)pDataBuffer;
                    char nameStr[32];
                    sprintf(nameStr, "h3all/jiasusprite.png");
                    pMailLayer->setMailItem(nameStr, pItemInfo->nItemCount);
                    break;
                }
                case DTP_GP_MAIL_SKILL_BJ:
                {
                    if (DataDescribe.wDataSize != sizeof(CMD_GPO_ItemInfo))
                    {
                        ERROR_CHECK;
                        return false;
                    }
                    CMD_GPO_ItemInfo* pItemInfo = (CMD_GPO_ItemInfo*)pDataBuffer;
                    char nameStr[32];
                    sprintf(nameStr, "h3all/suodingsprite.png");
                    pMailLayer->setMailItem(nameStr, pItemInfo->nItemCount);
                    break;
                }
                case DTP_GP_MAIL_GIFTGOLD://金币
                {
                    if (DataDescribe.wDataSize != sizeof(CMD_GPO_ItemInfo))
                    {
                        ERROR_CHECK;
                        return false;
                    }
                    CMD_GPO_ItemInfo* pItemInfo = (CMD_GPO_ItemInfo*)pDataBuffer;
                    char nameStr[32];
                    sprintf(nameStr, "mail/mailcoin.png");
                    pMailLayer->setMailItem(nameStr, pItemInfo->nItemCount);
                    break;
                }
                default:
                    break;
            }
        }
    }
//    char * pBuffer = (char*)pData;
//    char* pContain = (char*)&pBuffer[4];
    for(unsigned int i = 0; i < m_vctMail.size(); i++)
    {
        if(m_vctMail[i].dwMailID == m_nReadingMailID)
        {
            if(pContain==NULL||strlen(pContain)<2)
            {
                m_vctMail[i].szMailContent = "";
                m_vctMail[i].szDuihuanma = "";
                m_vctMail[i].szGongzhonghao = "";
            }
            else
            {
                std::string szContent = CCharCode::GB2312ToUTF8(pContain);
                //检测字符串中有没有要复制的字符串
                int size = szContent.find("[=]");
                if(size == -1)//说明字符串中没有复制字符串
                {
                    m_vctMail[i].szMailContent = szContent;
                    m_vctMail[i].szDuihuanma = "";
                    m_vctMail[i].szGongzhonghao = "";
                }
                else
                {
                    std::string szDuihuan1 = szContent.substr(size+3);
                    int nDuihuanSize = szDuihuan1.find("[=]");
                    if(nDuihuanSize == -1)//说明只有公众号
                    {
                        m_vctMail[i].szMailContent = szContent.substr(0, size);
                        m_vctMail[i].szGongzhonghao = szDuihuan1;
                        m_vctMail[i].szDuihuanma = "";
                    }
                    else//既有公众号也有兑换码
                    {
                        m_vctMail[i].szMailContent = szContent.substr(0, size);
                        m_vctMail[i].szGongzhonghao = szDuihuan1.substr(0, nDuihuanSize);
                        m_vctMail[i].szDuihuanma = szDuihuan1.substr(nDuihuanSize+3);
                    }
                }
                //m_vctMail[i].szMailContent = CCharCode::GB2312ToUTF8(pContain);
            }
            
            m_vctMail[i].cbMailStatus = 1;
            break;
        }
        
    }
    //显示未读数量
    int count = 0;
    for(int i = 0; i < m_vctMail.size(); i++)
    {
        //如果邮件未读也没有领取功能则＋＋， 如果邮件已读并且有领取功能但是未领取也＋＋
        if((!m_vctMail[i].cbMailStatus && m_vctMail[i].cbGeted == 0) || (m_vctMail[i].cbMailStatus && m_vctMail[i].cbGeted == 1) || (!m_vctMail[i].cbMailStatus && m_vctMail[i].cbGeted == 1) || (!m_vctMail[i].cbMailStatus && m_vctMail[i].cbGeted == 3))
        {
            count++;
        }
    }
    m_nMailCount = count;
    m_pMessageBox -> Update();
    if(m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            pHallLayer -> setMailNumber(m_nMailCount);
            MailLayer* pMailLayer = (MailLayer*)pHallLayer->getChildByTag(19998);
            if(pMailLayer != NULL)
            {
                pMailLayer->showMailContain(m_nReadingMailID, true);
            }
        }
    }
    else if(GameMainScene::_instance != NULL)
    {
        GameMainScene::_instance -> setMailNumber(m_nMailCount);
        MailLayer* pMailLayer = (MailLayer*)GameMainScene::_instance->getChildByTag(19998);
        if(pMailLayer != NULL)
        {
            pMailLayer->showMailContain(m_nReadingMailID, true);
        }
    }
    //把结构体中的内容保存到xml文件中
    writeMailXML();
    return true;
}

void CMainLogic::func(int sig)
{
    printf("func sig\n");
}

//领取邮件奖励
bool CMainLogic::onGetMailPrise(const void* pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GPO_GetMailItem))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_GetMailItem* pMailGetScore = (CMD_GPO_GetMailItem*)pData;
    if(pMailGetScore != NULL)
    {
        ShowMessage(CCharCode::GB2312ToUTF8(pMailGetScore->szResult).c_str(), eMBOK);
        if(pMailGetScore->cbSuccess == 0)//领取成功
        {
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if (pHallLayer != NULL)
                {
                    SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                    pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                }
            }
            sendUserBehavior(m_nUserID, eHallMailLingquSuccess);
        }
        else if(pMailGetScore->cbSuccess == 1)//已经领取过了
        {
            sendUserBehavior(m_nUserID, eHallMailLingquFailComplete);
        }
        else//领取失败
        {
            sendUserBehavior(m_nUserID, eHallMailLingquFail);
            return true;
        }
    }
 
    if(pMailGetScore->cbSuccess == 0)
    {
        //接收加包信息
        void* pDataBuffer = NULL;
        tagDataDescribe DataDescribe;
        CRecvPacketHelper RecvPacket(pMailGetScore+1, wDataSize-sizeof(CMD_GPO_GetMailItem));
        while(true)
        {
            //提取数据
            pDataBuffer = RecvPacket.GetData(DataDescribe);
            if(DataDescribe.wDataDescribe == DTP_NULL)
            {
                break;
            }
            switch (DataDescribe.wDataDescribe) {
                case DTP_GP_MAIL_LOTTERY://奖券
                {
                    if (DataDescribe.wDataSize != sizeof(CMD_GPO_ItemInfo))
                    {
                        ERROR_CHECK;
                        return false;
                    }
                    CMD_GPO_ItemInfo* pItemInfo = (CMD_GPO_ItemInfo*)pDataBuffer;
                    if (pItemInfo != NULL) {
                        m_nLottery = pItemInfo->nItemCount;
                        if (m_pHallScene != NULL) {
                            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                            if (pHallLayer != NULL) {
                                pHallLayer->setTicket(m_nLottery);
                            }
                        }
                    }
                    break;
                }
                case DTP_GP_MAIL_GIFTGOLD://金币
                {
                    if (DataDescribe.wDataSize != sizeof(CMD_GPO_ItemInfo))
                    {
                        ERROR_CHECK;
                        return false;
                    }
                    CMD_GPO_ItemInfo* pItemInfo = (CMD_GPO_ItemInfo*)pDataBuffer;
                    if (pItemInfo != NULL) {
                        m_lUserScore = pItemInfo->nItemCount;
                        if (m_pHallScene != NULL) {
                            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                            if (pHallLayer != NULL) {
                                pHallLayer->setUserScore(m_lUserScore);
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
    
    //将领取按钮设置为“已领取”
    if(m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            //刷新当前分数
            pHallLayer -> setUserScore(m_lUserScore);
            MailLayer* pMailLayer = (MailLayer*)pHallLayer->getChildByTag(19998);
            if(pMailLayer != NULL)
            {
                pMailLayer -> setMenuState();
            }
        }
    }
    //将大厅邮件角标减少
    for(unsigned int i = 0; i < m_vctMail.size(); i++)
    {
        if(m_vctMail[i].dwMailID == m_nReadingMailID)
        {
            m_vctMail[i].cbMailStatus = 1;
            m_vctMail[i].cbGeted = 2;
            break;
        }
        
    }
    //显示未读数量
    int count = 0;
    for(int i = 0; i < m_vctMail.size(); i++)
    {
        //如果邮件未读也没有领取功能则＋＋， 如果邮件已读并且有领取功能但是未领取也＋＋
        if((!m_vctMail[i].cbMailStatus && m_vctMail[i].cbGeted == 0) || (m_vctMail[i].cbMailStatus && m_vctMail[i].cbGeted == 1) || (!m_vctMail[i].cbMailStatus && m_vctMail[i].cbGeted == 3) || (!m_vctMail[i].cbMailStatus && m_vctMail[i].cbGeted == 1))
        {
            count++;
        }
    }
    m_nMailCount = count;
    if(m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            pHallLayer -> setMailNumber(m_nMailCount);
            MailLayer* pMailLayer = (MailLayer*)pHallLayer->getChildByTag(19998);
            if(pMailLayer != NULL)
            {
                pMailLayer->showMailContain(m_nReadingMailID, false);
            }
        }
    }
    else if(GameMainScene::_instance != NULL)
    {
        GameMainScene::_instance -> setMailNumber(m_nMailCount);
        MailLayer* pMailLayer = (MailLayer*)GameMainScene::_instance->getChildByTag(19998);
        if(pMailLayer != NULL)
        {
            pMailLayer->showMailContain(m_nReadingMailID, true);
        }
    }
    return true;
}

bool CMainLogic::OnGetSignInMessage(const void * pData, WORD wDataSize)//获取签到信息
{
    printf("收到签到的消息\n");
    if(!m_bFirstLogon)
    {
        m_pMessageBox->Update();
    }
    if (wDataSize < sizeof(CMD_GP_UserSigninInfo))
    {
        ERROR_CHECK;
        return false;
    }
    if(!m_bIOSPassed)
    {
        return true;
    }
    CMD_GP_UserSigninInfo* pUserSignInInfo = (CMD_GP_UserSigninInfo*)pData;
    if(pUserSignInInfo != NULL)
    {
        if(pUserSignInInfo->cbNum1>=1&&pUserSignInInfo->cbNum1<=7)
        {
            if(pUserSignInInfo->normal[pUserSignInInfo->cbNum1-1].cbAttrib==0)//本天没签
            {
                m_nLogonGift = pUserSignInInfo->normal[pUserSignInInfo->cbNum1-1].nCount;
            }
            else
            {
                m_nLogonGift = 0;
            }
        }
        if(m_nVipLevel>=1)
        {
            if(pUserSignInInfo->cbNum2>=1&&pUserSignInInfo->cbNum2<=7)
            {
                if(pUserSignInInfo->vip[pUserSignInInfo->cbNum2-1].cbAttrib==0)//本天没签
                {
                    m_bReceiveVipLogon = false;//VIP可以签到，但是未签
                }
                else
                {
                    m_bReceiveVipLogon = true;//VIP本日已签到
                }
            }
        }
        else
        {
            m_bReceiveVipLogon = true;//不是VIP用户，不能签到
        }
        //发送转盘查询消息
        CMD_GPR_QueryWheel QueryWheel = {0};
        QueryWheel.dwUserID = m_nUserID;
        SendData(m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_QUERY_WHEEL, &QueryWheel, sizeof(QueryWheel));
        if(m_nLogonGift == 0 && m_bReceiveVipLogon && m_bFirstLogon)
        {
            m_bShowWheelNoSign = true;
            return true;
        }

        if(m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                CSignInLayer* pSignInLayer = CSignInLayer::create();
                pHallLayer -> addChild(pSignInLayer, 200, 19984);
                pSignInLayer->m_cbNormalDay = pUserSignInInfo->cbNum1;
                pSignInLayer->m_cbVipDay = pUserSignInInfo->cbNum2;
                for(int i = 0; i < 7; i++)
                {
                    pSignInLayer->m_normalDay[i].cbType = pUserSignInInfo->normal[i].cbType;
                    pSignInLayer->m_normalDay[i].cbAttrib = pUserSignInInfo->normal[i].cbAttrib;
                    pSignInLayer->m_normalDay[i].nCount = pUserSignInInfo->normal[i].nCount;
                    pSignInLayer->m_vipDay[i].cbType = pUserSignInInfo->vip[i].cbType;
                    pSignInLayer->m_vipDay[i].cbAttrib = pUserSignInInfo->vip[i].cbAttrib;
                    pSignInLayer->m_vipDay[i].nCount = pUserSignInInfo->vip[i].nCount;
                }
                if(pSignInLayer->m_cbNormalDay>=1&&pSignInLayer->m_cbNormalDay<=7)
                {
                    if(pSignInLayer->m_normalDay[pSignInLayer->m_cbNormalDay-1].cbAttrib==0)//本天没签
                    {
                        m_nLogonGift = pSignInLayer->m_normalDay[pSignInLayer->m_cbNormalDay-1].nCount;
                    }
                    else
                    {
                        m_nLogonGift = 0;
                    }
                }
                if(m_nVipLevel>=1)
                {
                    if(pSignInLayer->m_cbVipDay>=1&&pSignInLayer->m_cbVipDay<=7)
                    {
                        if(pSignInLayer->m_vipDay[pSignInLayer->m_cbVipDay-1].cbAttrib==0)//本天没签
                        {
                            m_bReceiveVipLogon = false;//VIP可以签到，但是未签
                        }
                        else
                        {
                            m_bReceiveVipLogon = true;//VIP本日已签到
                        }
                    }
                }
                else
                {
                    m_bReceiveVipLogon = true;//不是VIP用户，不能签到
                }
                
                pSignInLayer -> updateUI();
                pSignInLayer -> setSignMark(pSignInLayer->m_cbNormalDay,pSignInLayer->m_cbVipDay);
            }
        }
    }
    return true;
}

bool CMainLogic::OnSocketSignInSucceed(const void * pData, WORD wDataSize)//签到成功
{
    if (wDataSize < sizeof(CMD_GP_SignIn_Result))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GP_SignIn_Result* pSignInResult = (CMD_GP_SignIn_Result*)pData;
    if(pSignInResult == NULL) return true;
    if(pSignInResult->bSuccess)
    {
        m_pMessageBox->Update();
        if(m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                if(pSignInResult->cbUserStatus == 0)
                {
                    sendUserBehavior(m_nUserID, eHallSignIn2);
                }
                else
                {
                    sendUserBehavior(m_nUserID, eHallSignIn5);
                }
                m_lUserScore = pSignInResult->llScoreCont;
                m_nLottery = (int)pSignInResult->llLotteryCount;
                pHallLayer->setTicket(m_nLottery);
                pHallLayer->setUserScore(m_lUserScore);
                SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
                pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                CSignInLayer* pSignInLayer = (CSignInLayer*)pHallLayer->getChildByTag(19984);
                if(pSignInLayer != NULL)
                {
                    pSignInLayer->refreshSignedDay(pSignInResult->cbUserStatus);
                    pSignInLayer->showSignInSuccess(pSignInResult->cbUserStatus);
                    if(pSignInResult->cbUserStatus == 0)//普通用户签到
                    {
                        m_nLogonGift = 0;
                    }
                    else//VIP签到
                    {
                        m_bReceiveVipLogon = true;//VIP本日已签到
                    }
                    //更新免费金币心形图标和免费金币界面中签到奖励跳动动画
                    if(m_nLogonGift != 0 || m_bCanShare || m_cbCanGetBankruptcy>0 || !m_bReceiveVipLogon || m_bCanBandingTel)
                    {
                        pHallLayer->showfreecoinAction(true);
                    }
                    else
                    {
                        pHallLayer->showfreecoinAction(false);
                    }
                    if(m_nLogonGift != 0 || !m_bReceiveVipLogon)
                    {
                        
                    }
                    else
                    {
                        FreeCoinLayer* pFreeCoinLayer = (FreeCoinLayer*)pHallLayer->getChildByTag(19996);
                        if(pFreeCoinLayer != NULL)
                        {
                            pFreeCoinLayer->stopLogonItem();
                        }
                    }
                }
            }
        }
    }
    else
    {
        ShowMessage(CCharCode::GB2312ToUTF8(pSignInResult->szDescription).c_str(), eMBOK);
        if(pSignInResult->cbUserStatus == 0)
        {
            sendUserBehavior(m_nUserID, eHallSignIn3);
        }
        else
        {
            sendUserBehavior(m_nUserID, eHallSignIn6);
        }
        
    }
    
    return true;
}

//用户进入点击抽奖，获取免费次数
bool CMainLogic::OnGetBonusInfo(const void * pData, WORD wDataSize)
{
    return true;
}

//用户抽奖，服务端发送结果
bool CMainLogic::OnGetBonusCompleted(const void * pData, WORD wDataSize)
{
    return true;
}

//抽奖公告
bool CMainLogic::OnGetBonusMessage(const void * pData, WORD wDataSize)
{
    return true;
}

bool CMainLogic::OnGetChargeTime(const void* pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GP_RechargeInfo_Res))
    {
        ERROR_CHECK;
        return false;
    }
    return true;
}

bool CMainLogic::OnSocketHallLogon(const TCP_Command & Command, const void * pData, unsigned short wDataSize)
{
    switch (Command.wSubCmdID)
    {
        case SUB_GP_LOGON_SUCCESS:
        {
            CMD_GP_LogonSuccess * pLogonSuccess = (CMD_GP_LogonSuccess *)pData;
            if (pLogonSuccess == NULL)
            {
                ERROR_CHECK;
                break;
            }
            m_sNickName = CCharCode::GB2312ToUTF8(pLogonSuccess->szNickName);
            m_lUserScore = pLogonSuccess->lUserScore;
            if(m_lUserScore == 0)
            {
                m_lUserScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("UserScore", 0);
            }
            m_nUserID = pLogonSuccess->dwUserID;
            m_nGameID = pLogonSuccess->dwGameID;
            m_nGender = pLogonSuccess->cbGender;
            m_nTouxiangID = pLogonSuccess->wFaceID;
            printf("m_nTouxiangID is %d\n", m_nTouxiangID);
            
            //接收加包信息
            void* pDataBuffer = NULL;
            tagDataDescribe DataDescribe;
            CRecvPacketHelper RecvPacket(pLogonSuccess+1, wDataSize-sizeof(CMD_GP_LogonSuccess));
            while(true)
            {
                //提取数据
                pDataBuffer = RecvPacket.GetData(DataDescribe);
                if(DataDescribe.wDataDescribe == DTP_NULL)
                    break;
                switch (DataDescribe.wDataDescribe) {
                    case DTP_GP_RECHARGE_INFO:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_RechargeInfo))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_RechargeInfo* pFirstCharge = (DTP_GP_RechargeInfo*)pDataBuffer;
                        m_bFirstCharge = pFirstCharge->cbIsRecharge;
                        //1就是充过值，0代表首充
                        break;
                    }
                    case DTP_GP_SPECIALNODEIP:
                    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        char temp[64];
                        memcpy(temp, pDataBuffer, DataDescribe.wDataSize);
                        temp[DataDescribe.wDataSize] = 0;
                        std::string specialnode = (char*)temp;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                        std::string specialnode = (char*)pDataBuffer;
#endif
                        CCUserDefault::sharedUserDefault()->setStringForKey("NEWSPECIALNODE", specialnode);
                        CCUserDefault::sharedUserDefault()->setBoolForKey("NEWSPECIALNODEIP", false);
                        break;
                    }
                    case DTP_GP_SPECIALNODEURL:
                    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        char temp[64];
                        memcpy(temp, pDataBuffer, DataDescribe.wDataSize);
                        temp[DataDescribe.wDataSize] = 0;
                        std::string specialnode = (char*)temp;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                        std::string specialnode = (char*)pDataBuffer;
#endif
                        CCUserDefault::sharedUserDefault()->setStringForKey("NEWSPECIALNODE", specialnode);
                        CCUserDefault::sharedUserDefault()->setBoolForKey("NEWSPECIALNODEIP", true);
                        break;
                    }
                    case DTP_GP_MEMBER_ORDER:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_MemberOrder))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_MemberOrder* pMemberOrder = (DTP_GP_MemberOrder*)pDataBuffer;
                        m_cbMemberOrder = pMemberOrder->cbMemberOrder;
                        break;
                    }
                    case DTP_GP_GAME_CELLSCORE:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_Game_CellScore))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_Game_CellScore* pCellScore = (DTP_GP_Game_CellScore*)pDataBuffer;
                        DTP_GP_Game_CellScore CellScore = {0};
                        CellScore.wServerID = pCellScore->wServerID;
                        CellScore.nCellScore = pCellScore->nCellScore;
                        m_vctCellScore.push_back(CellScore);
                        break;
                    }
                    case DTP_GP_GIFT_TICKET:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_GiftTicket))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_GiftTicket* pGiftTicket = (DTP_GP_GiftTicket*)pDataBuffer;
                        m_nLottery = pGiftTicket->dwGiftTicket;
                        break;
                    }
                    case DTP_GP_IOSCHARGEHIDE:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_IOSChargeHide))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_IOSChargeHide* pIOSChargeHide = (DTP_GP_IOSChargeHide*)pDataBuffer;
                        if(pIOSChargeHide->nID == 6)
                        {
                            m_sChargeURL = (char*)pIOSChargeHide->szURL;
                            Charge1URLstr = m_sChargeURL;
                        }
                        else if(pIOSChargeHide->nID == 7)
                        {
                            m_sChargeCardURL = (char*)pIOSChargeHide->szURL;
                            Charge2URLstr = m_sChargeCardURL;
                        }
                        break;
                    }
                    case DTP_GP_VIPINFO://玩家VIP信息
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_VipInfo))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_VipInfo* pVipInfo = (DTP_GP_VipInfo*)pDataBuffer;
                        m_nVipLevel = pVipInfo->cbVIPLevel;
                        m_nVipExp = pVipInfo->dwVIPExp;
                        m_nExpPercent = pVipInfo->dwExpPerCent;
                        CCUserDefault::sharedUserDefault()->setIntegerForKey("USERVIPLEVEL", m_nVipLevel);
                        break;
                    }
                    case DTP_GP_CUSTOMERSERVICEQQ://客服QQ
                    {
                        m_sServiceQQ = (char*)pDataBuffer;
                        break;
                    }
                    case DTP_GP_LOGONGIFT://登陆奖励，每天只领取一次
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_LogonGift))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_LogonGift* pLogonGift = (DTP_GP_LogonGift*)pDataBuffer;
                        //m_nLogonGift = pLogonGift->dwGiftScore;
                        m_bCanShare = pLogonGift->nCanShare;//是否能领取分享奖励
                        m_nBankruptcyCount = pLogonGift->nBankruptcyCount;//目前领取了几次
                        m_cbCanGetBankruptcy = pLogonGift->cbCanGetBankruptcy;//是否能领取救济金
                        m_nGiveScore = pLogonGift->nGiveScore;
                        m_nGiveTotal = pLogonGift->nBankruptcyCountMax;
                        break;
                    }
                    case DTP_GP_CHARGE_FIRSTINFO:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_ChargeFirstInfo))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_ChargeFirstInfo* pChargeFirstInfo = (DTP_GP_ChargeFirstInfo*)pDataBuffer;
                        if(pChargeFirstInfo != NULL)
                        {
                            m_cbCountTimes = pChargeFirstInfo->cbCount;
                            m_bFirstChargeCanGet = pChargeFirstInfo->cbCanGet;
                        }
                        break;
                    }
                    case DTP_GP_SHOWNIUGAME:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_ShowNiuGame))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_ShowNiuGame* pChargeFirstInfo = (DTP_GP_ShowNiuGame*)pDataBuffer;
                        m_bShowNiuGame = pChargeFirstInfo->cbShowNiuGame;
                        
                        break;
                    }
                    case DTP_GP_PASS_PORT_ID:
                    {
                        m_sMachine = (char*)pDataBuffer;
                        CCUserDefault::sharedUserDefault()->setStringForKey("MACHINE", m_sMachine);
                        break;
                    }
                    case DTP_GP_SHOW_FUBEN:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_ShowFuben))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_ShowFuben* pShowFuben = (DTP_GP_ShowFuben*)pDataBuffer;
                        if(pShowFuben->cbShowFuben >= 2)
                        {
                            m_bShowFuben = true;
                        }
                        else
                        {
                            m_bShowFuben = false;
                        }
                        break;
                    }
                    case DTP_GP_BI_SAI_STATUS:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_ButtonSwitch))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_ButtonSwitch* pShowMatch = (DTP_GP_ButtonSwitch*)pDataBuffer;
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 1))
                        {
                            m_bShowMatch = true;
                        }
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 2))
                        {
                            m_bShowZhouMatch = true;
                        }
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 3))
                        {
                            m_bShowBeibao = true;
                        }
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 4))
                        {
                            m_bShowChat = true;
                        }
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 5))
                        {
                            m_bShowOnlineCoin = true;
                        }
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 6))
                        {
                            m_bShowShareInfo = true;
                        }
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 7))
                        {
                            m_bShowOneCharge = true;
                        }
                        break;
                    }
                    case DTP_GP_WEEK_CMPT_DATA:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_WeekCmptData))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_WeekCmptData* pWeekCmptData = (DTP_GP_WeekCmptData*)pDataBuffer;
                        if(pWeekCmptData != NULL)
                        {
                            m_nUserWeekBeiNum = pWeekCmptData->nWeekCmptCup;
                            m_nUserWeekRank = pWeekCmptData->nWeekCmptRank;
                        }
                        break;
                    }
                    case DTP_GP_ONE_CHARGE:
                    {
                        printf("size is %lu\n", sizeof(DTP_GP_OneFirstCharge));
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_OneFirstCharge))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_OneFirstCharge* pOneFirstCharge = (DTP_GP_OneFirstCharge*)pDataBuffer;
                        if(pOneFirstCharge != NULL)
                        {
                            printf("%d, %d, %hhu, %hhu, %hhu\n", pOneFirstCharge->bCanCharge, pOneFirstCharge->nCoinScore, pOneFirstCharge->cbHuaFeiNum, pOneFirstCharge->cbJiaSuCount, pOneFirstCharge->cbSuoDingCount);
                            m_bCanOneCharge = pOneFirstCharge->bCanCharge;
                            m_nOneCoinCount = pOneFirstCharge->nCoinScore;
                            m_cbOneHuafei = pOneFirstCharge->cbHuaFeiNum;
                            m_cbOneSpeedCount = pOneFirstCharge->cbJiaSuCount;
                            m_cbOneSuodingCount = pOneFirstCharge->cbSuoDingCount;
                        }
                        break;
                    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    case DTP_GP_IOSPASSED:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_IOSPassed))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_IOSPassed* pIOSPassed = (DTP_GP_IOSPassed*)pDataBuffer;
                        if(pIOSPassed->cbPassed >= 13)
                        {
                            m_bIOSPassed = true;
                        }
                        break;
                    }
#endif
                    default:
                        break;
                }
            }
            ShowMessage("正在获取房间信息...",eMBNull,eLeaveHall,true);
            
            m_vctMail.clear();
            
            createMailXML();
            
            if(!CCUserDefault::sharedUserDefault()->getBoolForKey("VipNewConfig", false))
            {
                //本地不存在时，将资源中的文件写入到documents目录下
                createLocalVIPConfig();
            }
            readVIPConfig();
            
            readMailXML();
            //保存银行密码
            CCUserDefault::sharedUserDefault()->setStringForKey("NewPassword", CCUserDefault::sharedUserDefault()->getStringForKey("NewPassword"));
            
        }
            break;
        case SUB_GP_GUEST_REGISTER_SUCCESS:
        {
            CMD_GP_GuestRegisterSuccess * pGuestRegisterSuccess = (CMD_GP_GuestRegisterSuccess *)pData;
            if (pGuestRegisterSuccess == NULL)
            {
                ERROR_CHECK;
                break;
            }
            m_sNickName = CCharCode::GB2312ToUTF8(pGuestRegisterSuccess->szNickName);
            m_lUserScore = pGuestRegisterSuccess->lUserScore;
            m_nUserID = pGuestRegisterSuccess->dwUserID;
            m_nGameID = pGuestRegisterSuccess->dwGameID;
            m_nGender = pGuestRegisterSuccess->cbGender;
            m_nTouxiangID = pGuestRegisterSuccess->wFaceID;
            
            CCUserDefault::sharedUserDefault()->setIntegerForKey("USERVIPLEVEL", 0);
            //接收加包信息
            void* pDataBuffer = NULL;
            tagDataDescribe DataDescribe;
            CRecvPacketHelper RecvPacket(pGuestRegisterSuccess+1, wDataSize-sizeof(CMD_GP_GuestRegisterSuccess));
            while(true)
            {
                //提取数据
                pDataBuffer = RecvPacket.GetData(DataDescribe);
                if(DataDescribe.wDataDescribe == DTP_NULL)
                    break;
                switch (DataDescribe.wDataDescribe) {
                    case DTP_GP_SPECIALNODEIP:
                    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        char temp[64];
                        memcpy(temp, pDataBuffer, DataDescribe.wDataSize);
                        temp[DataDescribe.wDataSize] = 0;
                        std::string specialnode = (char*)temp;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                        std::string specialnode = (char*)pDataBuffer;
#endif
                        CCUserDefault::sharedUserDefault()->setStringForKey("NEWSPECIALNODE", specialnode);
                        CCUserDefault::sharedUserDefault()->setBoolForKey("NEWSPECIALNODEIP", false);
                        break;
                    }
                    case DTP_GP_SPECIALNODEURL:
                    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        char temp[64];
                        memcpy(temp, pDataBuffer, DataDescribe.wDataSize);
                        temp[DataDescribe.wDataSize] = 0;
                        std::string specialnode = (char*)temp;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                        std::string specialnode = (char*)pDataBuffer;
#endif
                        CCUserDefault::sharedUserDefault()->setStringForKey("NEWSPECIALNODE", specialnode);
                        CCUserDefault::sharedUserDefault()->setBoolForKey("NEWSPECIALNODEIP", true);
                        break;
                    }
                    case DTP_GP_IOSCHARGEHIDE:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_IOSChargeHide))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_IOSChargeHide* pIOSChargeHide = (DTP_GP_IOSChargeHide*)pDataBuffer;
                        if(pIOSChargeHide->nID == 6)
                        {
                            m_sChargeURL = (char*)pIOSChargeHide->szURL;
                            Charge1URLstr = m_sChargeURL;
                        }
                        else if(pIOSChargeHide->nID == 7)
                        {
                            m_sChargeCardURL = (char*)pIOSChargeHide->szURL;
                            Charge2URLstr = m_sChargeCardURL;
                        }
                        break;
                    }
                    case DTP_GP_VIPINFO://玩家VIP信息
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_VipInfo))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_VipInfo* pVipInfo = (DTP_GP_VipInfo*)pDataBuffer;
                        m_nVipLevel = pVipInfo->cbVIPLevel;
                        m_nVipExp = pVipInfo->dwVIPExp;
                        m_nExpPercent = pVipInfo->dwExpPerCent;
                        
                        break;
                    }
                    case DTP_GP_CUSTOMERSERVICEQQ://客服QQ
                    {
                        m_sServiceQQ = (char*)pDataBuffer;
                        break;
                    }
                    case DTP_GP_LOGONGIFT://登陆奖励，每天只领取一次
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_LogonGift))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_LogonGift* pLogonGift = (DTP_GP_LogonGift*)pDataBuffer;
                        //m_nLogonGift = pLogonGift->dwGiftScore;
                        m_bCanShare = true;
                        m_nGiveScore = pLogonGift->nGiveScore;
                        m_nBankruptcyCount = pLogonGift->nBankruptcyCount;
                        m_cbCanGetBankruptcy = pLogonGift->cbCanGetBankruptcy;
                        m_nGiveTotal = pLogonGift->nBankruptcyCountMax;
                        break;
                    }
                        
                    case DTP_GP_CHARGE_FIRSTINFO:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_ChargeFirstInfo))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_ChargeFirstInfo* pChargeFirstInfo = (DTP_GP_ChargeFirstInfo*)pDataBuffer;
                        if(pChargeFirstInfo != NULL)
                        {
                            m_cbCountTimes = pChargeFirstInfo->cbCount;
                            m_bFirstChargeCanGet = pChargeFirstInfo->cbCanGet;
                        }
                        break;
                    }
                    case DTP_GP_SHOWNIUGAME:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_ShowNiuGame))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_ShowNiuGame* pChargeFirstInfo = (DTP_GP_ShowNiuGame*)pDataBuffer;
                        m_bShowNiuGame = pChargeFirstInfo->cbShowNiuGame;
                        
                        break;
                    }
                    case DTP_GP_PASS_PORT_ID:
                    {
                        m_sMachine = (char*)pDataBuffer;
                        CCUserDefault::sharedUserDefault()->setStringForKey("MACHINE", m_sMachine);
                        break;
                    }
                    case DTP_GP_SHOW_FUBEN:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_ShowFuben))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_ShowFuben* pShowFuben = (DTP_GP_ShowFuben*)pDataBuffer;
                        if(pShowFuben->cbShowFuben >= 2)
                        {
                            m_bShowFuben = true;
                        }
                        else
                        {
                            m_bShowFuben = false;
                        }
                        break;
                    }
                    case DTP_GP_BI_SAI_STATUS:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_ButtonSwitch))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_ButtonSwitch* pShowMatch = (DTP_GP_ButtonSwitch*)pDataBuffer;
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 1))
                        {
                            m_bShowMatch = true;
                        }
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 2))
                        {
                            m_bShowZhouMatch = true;
                        }
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 3))
                        {
                            m_bShowBeibao = true;
                        }
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 4))
                        {
                            m_bShowChat = true;
                        }
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 5))
                        {
                            m_bShowOnlineCoin = true;
                        }
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 6))
                        {
                            m_bShowShareInfo = true;
                        }
                        if(getSwitchByLogon(pShowMatch->nButtonSwitch, 7))
                        {
                            m_bShowOneCharge = true;
                        }
                        break;
                    }
                    case DTP_GP_ONE_CHARGE:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_OneFirstCharge))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_OneFirstCharge* pOneFirstCharge = (DTP_GP_OneFirstCharge*)pDataBuffer;
                        if(pOneFirstCharge != NULL)
                        {
                            printf("%d, %d, %hhu, %hhu, %hhu\n", pOneFirstCharge->bCanCharge, pOneFirstCharge->nCoinScore, pOneFirstCharge->cbHuaFeiNum, pOneFirstCharge->cbJiaSuCount, pOneFirstCharge->cbSuoDingCount);
                            m_bCanOneCharge = pOneFirstCharge->bCanCharge;
                            m_nOneCoinCount = pOneFirstCharge->nCoinScore;
                            m_cbOneHuafei = pOneFirstCharge->cbHuaFeiNum;
                            m_cbOneSpeedCount = pOneFirstCharge->cbJiaSuCount;
                            m_cbOneSuodingCount = pOneFirstCharge->cbSuoDingCount;
                        }
                        break;
                    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    case DTP_GP_IOSPASSED:
                    {
                        if (DataDescribe.wDataSize != sizeof(DTP_GP_IOSPassed))
                        {
                            ERROR_CHECK;
                            return false;
                        }
                        DTP_GP_IOSPassed* pIOSPassed = (DTP_GP_IOSPassed*)pDataBuffer;
                        if(pIOSPassed->cbPassed >= 13)
                        {
                            m_bIOSPassed = true;
                        }
                        break;
                    }
#endif
                    default:
                        break;
                }
            }
            
            
            //设置为首充
            m_bFirstCharge = false;
            ShowMessage("正在获取房间信息...",eMBNull,eLeaveHall,true);
            
            m_vctMail.clear();
            
            createMailXML();
            
            if(!CCUserDefault::sharedUserDefault()->getBoolForKey("VipNewConfig", false))
            {//本地不存在时，将资源中的文件写入到documents目录下
                createLocalVIPConfig();
            }
            
            readVIPConfig();
            
            readMailXML();
            
            //保存账号密码
            CCUserDefault::sharedUserDefault()->setStringForKey("NewGuestAccount", pGuestRegisterSuccess->szAccounts);
            CCUserDefault::sharedUserDefault()->setStringForKey("NewGuestPassword", pGuestRegisterSuccess->szPassword);
            CMainLogic::sharedMainLogic()->m_sAccount = CCUserDefault::sharedUserDefault()->getStringForKey("NewGuestAccount", "");
            CMainLogic::sharedMainLogic()->m_sPassword = CCUserDefault::sharedUserDefault()->getStringForKey("NewGuestPassword", "");
            
            //游客登录状态标记，用于给游客自动填充银行密码
            CCUserDefault::sharedUserDefault()->setBoolForKey("NewGuestBank", true);
        }
            break;
        case SUB_GP_LOGON_FINISH:
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("NewAccount", CMainLogic::sharedMainLogic()->m_sAccount.c_str());
            CCUserDefault::sharedUserDefault()->setStringForKey("NewPassword", CMainLogic::sharedMainLogic()->m_sPassword.c_str());
            m_bLogonSuccess = true;
            switchLayer(eHallLayer);
            m_bFirstLogon = true;
            m_bForeGround = false;
            bIsSignIn = false;
            
            //发送领取登录VIP奖励
            CMD_GPR_VIPUser VIPUser;
            VIPUser.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            SendData(m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_VIP_INFO, &VIPUser,sizeof(VIPUser));
            
            //发送签到信息
//            if(CCUserDefault::sharedUserDefault()->getStringForKey("ShowSignIn", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("signin"))
//            {
//                CCUserDefault::sharedUserDefault()->setStringForKey("ShowSignIn", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("signin"));
                CMD_GP_QuerySigninInfo SigninInfo;
                SigninInfo.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GP_QUERY_SIGNIN_INFO, &SigninInfo,sizeof(SigninInfo));
//            }
            
            
            sendNoticeVersion("hallnotice");
            
            //发送是否可以绑定手机号
            CMD_GPR_NumCheckExit NumCheckExit;
            NumCheckExit.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            SendData(m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_CHECK_MOBILE_EXIT, &NumCheckExit,sizeof(NumCheckExit));
            
            CMD_GPR_BindReward BangDing;
            BangDing.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            SendData(m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_BINGDING, &BangDing,sizeof(BangDing));
            
            //向服务端发送邮件请求
            CMD_GPR_MailIDList MailIDList;
            MailIDList.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            SendData(m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_QUERY_MAIL_ID_LIST, &MailIDList, sizeof(MailIDList));
            m_bFirstRequestMail = false;
            
//            char temp[32];
//            sprintf(temp, "%dguide", m_nUserID);
//            if(m_bIOSPassed)
//            {
//                bool bShowGuide = CCUserDefault::sharedUserDefault()->getBoolForKey(temp, true);
//                if(bShowGuide)
//                {
//                    
//                    CCUserDefault::sharedUserDefault()->setBoolForKey(temp, false);
//                }
//            }
//            else
//            {
//                CCUserDefault::sharedUserDefault()->setBoolForKey(temp, false);
//            }
            
            CMD_GPR_NewGuidQuery NewGuidQuery;
            NewGuidQuery.dwUserID = m_nUserID;
            SendData(m_nHallSocketHandle, MDM_GP_SKILL, SUB_GPR_NEW_GUID_QUERY, &NewGuidQuery, sizeof(NewGuidQuery));
            
            CMD_GPR_SuperGold SuperGold;
            SuperGold.dwUserID = m_nUserID;
            SendData(m_nHallSocketHandle, MDM_GP_SKILL, SUB_GPR_SUPER_GOLD, &SuperGold, sizeof(SuperGold));
            
            
            //向网站发送手机信息
            sendUserTelInfo();
            
            //请求推送消息
            requestNotification();
            
            //发送公告速度请求
            SendData(m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_BROADCAST_SPEED);
        }
            break;
        case SUB_GP_LOGON_FAILURE:
        {
            CMD_GR_LogonFailure * pLogonFailure = (CMD_GR_LogonFailure *)pData;
            std::string str = CCharCode::GB2312ToUTF8(pLogonFailure->szDescribeString);
            ShowMessage(str.c_str(), eMBOK, eDisConnectNet);
            //@@@@@@@@
            //保存账号密码
            CCUserDefault::sharedUserDefault()->setStringForKey("NewAccount", CCUserDefault::sharedUserDefault()->getStringForKey("SuccessAccount").c_str());
            CCUserDefault::sharedUserDefault()->setStringForKey("NewPassword", CCUserDefault::sharedUserDefault()->getStringForKey("successPassword").c_str());
            
        }
            break;
        case SUB_GP_LOGINING_INFO:
        {
            CMD_GP_LoginingInfo* pLoginInfo = (CMD_GP_LoginingInfo*)pData;
            if(pLoginInfo != NULL)
            {
                char temp[128];
                sprintf(temp, "您前面还有%d人在排队，请稍候", pLoginInfo->wLoginingNum);
                ShowMessage(temp, eMBOK, eCancelLogon);
            }
            
            break;
        }
        case SUB_GP_VALIDATE_MBCARD:
        {
        }
            break;
        case SUB_GP_VALIDATE_PASSPOSTID:
        {
        }
            break;
    }
    
    return true;
}

bool compareType(tagGameKind obj1, tagGameKind obj2)
{
    if (obj1.wSortID <= obj2.wSortID)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CMainLogic::OnSocketHallServerList(const TCP_Command & Command, const void * pData, unsigned short wDataSize)
{
    ////printf("列表信息\n");
    switch (Command.wSubCmdID)
    {
        case SUB_GP_LIST_TYPE:
        {
            ////printf("类型列表\n");
            
            WORD wItemCount=wDataSize/sizeof(tagGameType);
            tagGameType * pGameType=(tagGameType *)pData;
            
            for (WORD i=0; i<wItemCount; i++)
            {
                ////printf("\t%s\n",CCharCode::GB2312ToUTF8(pGameType->szTypeName).c_str());
                pGameType++;
            }
        }
            break;
        case SUB_GP_LIST_KIND:
        {
            m_vGameType.clear();
            WORD wItemCount=wDataSize/sizeof(tagGameKind);
            tagGameKind * pGameKind=(tagGameKind *)pData;
            
            for (WORD i=0; i<wItemCount; i++)
            {
                if(pGameKind->wKindID == NZNH_KIND_ID)
                {
                    m_vGameType.push_back(*pGameKind);
                }
                else if(pGameKind->wKindID == YQS_KIND_ID)
                {
                    if(!m_bIOSPassed)
                    {
                        //m_vGameType.push_back(*pGameKind);
                    }
                }
                else if(pGameKind->wKindID == JCBY_KIND_ID)
                {
                    m_vGameType.push_back(*pGameKind);
                }
                else if(pGameKind->wKindID == CJHD_KIND_ID)
                {
                    if(m_bIOSPassed)
                    {
                        m_vGameType.push_back(*pGameKind);
                    }
                }
                pGameKind++;
            }
            std::sort(m_vGameType.begin(), m_vGameType.end(), compareType);
        }
            break;
        case SUB_GP_LIST_NODE:
        {
            ////printf("节点列表\n");
            
            WORD wItemCount=wDataSize/sizeof(tagGameNode);
            tagGameNode * pGameNode=(tagGameNode *)pData;
            
            for (WORD i=0; i<wItemCount; i++)
            {
                ////printf("\t%s\n",CCharCode::GB2312ToUTF8(pGameNode->szNodeName).c_str());
                pGameNode++;
            }
        }
            break;
        case SUB_GP_LIST_PAGE:
        {
            ////printf("定制列表\n");
            
            WORD wItemCount=wDataSize/sizeof(tagGamePage);
            tagGamePage * pGamePage=(tagGamePage *)(pData);
            
            for (WORD i=0; i<wItemCount; i++)
            {
                ////printf("\t%s\n",CCharCode::GB2312ToUTF8(pGamePage->szDisplayName).c_str());
                pGamePage++;
            }
        }
            break;
        case SUB_GP_LIST_FINISH:
        {
            ////printf("发送完成\n");
        }
            break;
        case SUB_GP_LIST_SERVER:
        {
            WORD wItemCount=wDataSize/sizeof(tagGameServer);
            tagGameServer * pGameServer=(tagGameServer *)pData;
            
            m_vRoomNZNH.clear();
            m_vRoomJCBY.clear();
            m_vRoomPirate.clear();
            m_vRoomFB.clear();
            m_vRoomLKPY.clear();
            m_vRoomPGW.clear();
            m_vRoomRB.clear();
            for (WORD i=0; i<wItemCount; i++)
            {
                if(pGameServer->wKindID == JCBY_KIND_ID)
                {
                    m_vRoomJCBY.push_back(*pGameServer);
//                    printf("minScore is %lld, lMaxEnterScore1 is %lld, wServerPortID1 is %hu\n", (pGameServer)->lMinEnterScore,pGameServer->lMaxEnterScore, pGameServer->wServerPort);
                }
                else if(pGameServer->wKindID == NZNH_KIND_ID)
                {
                    m_vRoomNZNH.push_back(*pGameServer);
//                    printf("minScore is %lld, lMaxEnterScore1 is %lld, wServerPortID1 is %hu\n", (pGameServer)->lMinEnterScore,pGameServer->lMaxEnterScore, pGameServer->wServerPort);
                }
                else if(pGameServer->wKindID == CJHD_KIND_ID)
                {
                    m_vRoomPirate.push_back(*pGameServer);
//                    printf("minScore is %lld, lMaxEnterScore3 is %lld, wServerPortID1 is %hu\n", (pGameServer)->lMinEnterScore,pGameServer->lMaxEnterScore, pGameServer->wServerPort);
                }
                else if(pGameServer->wKindID == FB_KIND_ID)
                {
                    m_vRoomFB.push_back(*pGameServer);
//                    printf("minScore is %lld, lMaxEnterScore4 is %lld, wServerPortID1 is %hu\n", (pGameServer)->lMinEnterScore,pGameServer->lMaxEnterScore, pGameServer->wServerPort);
                }
                else if(pGameServer->wKindID == YQS_KIND_ID)
                {
                    //m_vRoomLKPY.push_back(*pGameServer);
//                    printf("minScore is %lld, lMaxEnterScore2 is %lld, wServerPortID1 is %hu\n", (pGameServer)->lMinEnterScore,pGameServer->lMaxEnterScore, pGameServer->wServerPort);
                }
                else if(pGameServer->wKindID == PGW_KIND_ID)
                {
                    //m_vRoomPGW.push_back(*pGameServer);
                }
                else if(pGameServer->wKindID == H3CARD_KIND_ID)
                {
                    printf("dfsfsdaf\n");
                    m_vRoomRB.push_back(*pGameServer);
                }
                pGameServer++;
            }
        }
            break;
        case SUB_GP_LIST_SMALL_SERVER:
        {
            
        }
            break;
    }
    
    return true;
}

void CMainLogic::ShowNotice(std::string noticeStr, int nID)
{
    if(m_vctVIPNotice.size() > NOTICE_COUNT)
        return;
    
    if(nID == 0)
    {
        NoticeVct noticeVct = {0};
        noticeVct.nID = 60;
        noticeVct.bHave = true;
        noticeVct.sNotice = noticeStr;
        m_vctVIPNotice.push_back(noticeVct);
    }
    else
    {
        bool bSuccessInsert = false;
        NoticeVct noticeVct = {0};
        noticeVct.bHave = true;
        noticeVct.nID = nID;
        noticeVct.sNotice = noticeStr;
        for(int i = 0; i < (int)m_vctVIPNotice.size(); i++)
        {
            if(nID <= m_vctVIPNotice[i].nID)
            {
                m_vctVIPNotice.insert(m_vctVIPNotice.begin()+i, noticeVct);
                bSuccessInsert = true;
                break;
            }
        }
        if(!bSuccessInsert)
        {
            m_vctVIPNotice.push_back(noticeVct);
        }
    }
    
    CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
    if(pScene == NULL)
    {
        return;
    }
    if(pScene != m_pHallScene)
    {
        if(GameMainScene::_instance == NULL)
        {
            return;
        }
        else
        {
            if(!GameMainScene::_instance->m_bEnterGameScene)
            {
                return;
            }
        }
    }
    CCNode * pNode = CCDirector::sharedDirector()->getRunningScene();
    CCNode * pParent = m_pNoticeLayer->getParent();
    if (pParent != pNode)//如果公告的父类不是当前的场景，则先加到现在场景中
    {
        if (pParent != NULL)
        {
            pParent->removeChild(m_pNoticeLayer);
        }
        pNode->addChild(m_pNoticeLayer,20);
        m_pNoticeLayer->loadUI();
        m_pNoticeLayer->showNotice();
        m_pNoticeLayer->setVisible(true);
    }
    else
    {
        if(!m_pNoticeLayer->m_bShowCurrent)
        {
            m_pNoticeLayer->showNotice();
            m_pNoticeLayer->setVisible(true);
        }
    }
}

void CMainLogic::ShowLaba2Notice(std::string noticeStr, int nID)
{
    if(m_vctLaba2Notice.size() > NOTICE_COUNT)
        return;
    
    if(nID == 0)
    {
        NoticeVct noticeVct = {0};
        noticeVct.nID = 60;
        noticeVct.bHave = true;
        noticeVct.sNotice = noticeStr;
        m_vctLaba2Notice.push_back(noticeVct);
    }
    else
    {
        bool bSuccessInsert = false;
        NoticeVct noticeVct = {0};
        noticeVct.bHave = true;
        noticeVct.nID = nID;
        noticeVct.sNotice = noticeStr;
        for(int i = 0; i < (int)m_vctLaba2Notice.size(); i++)
        {
            if(nID <= m_vctLaba2Notice[i].nID)
            {
                m_vctLaba2Notice.insert(m_vctLaba2Notice.begin()+i, noticeVct);
                bSuccessInsert = true;
                break;
            }
        }
        if(!bSuccessInsert)
        {
            m_vctLaba2Notice.push_back(noticeVct);
        }
    }
    
    CCScene * pScene = CCDirector::sharedDirector()->getRunningScene();
    if(pScene == NULL)
    {
        return;
    }
    if(pScene != m_pHallScene)
    {
        if(GameMainScene::_instance == NULL)
        {
            return;
        }
        else
        {
            if(!GameMainScene::_instance->m_bEnterGameScene)
            {
                return;
            }
        }
    }
    CCNode * pNode = CCDirector::sharedDirector()->getRunningScene();
    CCNode * pParent = m_pLaba2Layer->getParent();
    if (pParent != pNode)//如果公告的父类不是当前的场景，则先加到现在场景中
    {
        if (pParent != NULL)
        {
            pParent->removeChild(m_pLaba2Layer);
        }
        pNode->addChild(m_pLaba2Layer,20);
        m_pLaba2Layer->loadUI();
        m_pLaba2Layer->showNotice();
        m_pLaba2Layer->setVisible(true);
    }
    else
    {
        if(!m_pLaba2Layer->m_bShowCurrent)
        {
            m_pLaba2Layer->showNotice();
            m_pLaba2Layer->setVisible(true);
        }
    }
}

void CMainLogic::ShowLabaNotice(std::string labaStr, std::string sNickName)
{
    LabaNotice labaNotice;
    labaNotice.sContent = labaStr;
    labaNotice.sNickName = sNickName;
    m_vctLabaNotice.push_back(labaNotice);
    if(m_pHallScene != NULL || (GameMainScene::_instance != NULL && GameMainScene::_instance->m_bEnterGameScene))
    {
        CCNode * pNode = CCDirector::sharedDirector()->getRunningScene();
        CCNode * pParent = m_pLabaNoticeLayer->getParent();
        if (pParent != pNode)//如果公告的父类不是当前的场景，则先加到现在场景中
        {
            if (pParent != NULL)
            {
                pParent->removeChild(m_pLabaNoticeLayer);
            }
            pNode->addChild(m_pLabaNoticeLayer,28);
            m_pLabaNoticeLayer->loadUI();
            m_pLabaNoticeLayer->showLabaNotice();
            m_pLabaNoticeLayer->setVisible(true);
        }
        else
        {
            if(!m_pLabaNoticeLayer->m_bShowCurrent)
            {
                m_pLabaNoticeLayer->showLabaNotice();
                m_pLabaNoticeLayer->setVisible(true);
            }
        }
    }
}

void CMainLogic::ShowDakasaiNotice(std::string labaStr)
{
    NoticeVct noticeVct = {0};
    noticeVct.nID = 0;
    noticeVct.bHave = true;
    noticeVct.sNotice = labaStr;
    m_vctDakasaiNotice.push_back(noticeVct);
    if(m_pHallScene != NULL || (GameMainScene::_instance != NULL && GameMainScene::_instance->m_bEnterGameScene))
    {
        CCNode * pNode = CCDirector::sharedDirector()->getRunningScene();
        CCNode * pParent = m_pDakasaiNotice->getParent();
        if (pParent != pNode)//如果公告的父类不是当前的场景，则先加到现在场景中
        {
            if (pParent != NULL)
            {
                pParent->removeChild(m_pDakasaiNotice);
            }
            pNode->addChild(m_pDakasaiNotice,28);
            m_pDakasaiNotice->loadUI();
            m_pDakasaiNotice->showLabaNotice();
            m_pDakasaiNotice->setVisible(true);
        }
        else
        {
            if(!m_pDakasaiNotice->m_bShowCurrent)
            {
                m_pDakasaiNotice->showLabaNotice();
                m_pDakasaiNotice->setVisible(true);
            }
        }
    }
}

void CMainLogic::ShowMessage(const char * sMessage, eOperateType eOperate, eRequestType eRequest, bool bWait)
{
    CCNode * pNode = CCDirector::sharedDirector()->getRunningScene();
    CCNode * pParent = m_pMessageBox->getParent();
    if (pParent != pNode)
    {
        if (pParent != NULL)
        {
            pParent->removeChild(m_pMessageBox);
        }
        pNode->addChild(m_pMessageBox,25);
    }
    m_pMessageBox->ShowMessageBox(eOperate,eRequest,sMessage,bWait);
}

void CMainLogic::MessageResult(eRequestType eRequest, eOperateType eOperate, bool bConfirm)
{
    switch (eRequest)
    {
        case eNull:
        {
            m_pMessageBox->Update();
        }
            break;
        case eTransferScore:
        {
            if (bConfirm)
            {
                transferScore();
            }
        }
            break;
        case eTransferScore2:
        {
            if (bConfirm)
            {
                transferScore2();
            }
        }
            break;
        case eGuestLogon:
        {
            if (bConfirm)
            {
                m_pTableScene = NULL;
                m_pHallScene = NULL;
                if(m_pLogonScene != NULL)
                {
                    CLogonLayer * pLogonLayer = (CLogonLayer *)m_pLogonScene->getChildByTag(eLogonLayerTag);
                    if (pLogonLayer)
                    {
                        pLogonLayer->constructGuestPacket();
                        pLogonLayer->sendLogonPacket();
                    }
                }
                else
                {
                    m_pLogonScene = CLogonLayer::scene();
                    CLogonLayer * pLogonLayer = (CLogonLayer *)m_pLogonScene->getChildByTag(eLogonLayerTag);
                    if (pLogonLayer)
                    {
                        pLogonLayer->constructGuestPacket();
                        pLogonLayer->sendLogonPacket();
                    }
                }
            }
        }
            break;
        case eDisConnectNet:
        {
            m_pMessageBox->Update();
            m_nClickBank = 0;
            m_nCheckSocketShut = 0;
            if(m_pLogonScene != NULL)
                break;
            if(GameMainScene::_instance != NULL || GameLayer::_instance != NULL)
            {
                //发送离开房间的消息
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                sendRoomExit();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                sendAndroidRoomExit();
#endif
                //SendData(m_nRoomSocketHandle, MDM_KN_COMMAND, SUB_KN_SHUT_MANUAL);
            }
            //@@保证再次进入房间时，桌子信息是正确的
            for (unsigned int i = 0; i < m_vUserInfo.size(); i++)
            {
                delete m_vUserInfo[i];
            }
            m_vUserInfo.clear();
            m_pLogonScene = NULL;
            CCScene* pLogonScene = CLogonLayer::scene();
            CMainLogic::sharedMainLogic()->m_bEnterLoading = false;
            if(GameLayer::_instance != NULL)
            {
                SimpleAudioEngine::sharedEngine()->stopEffect(GameLayer::_instance->getGameRollingEffectID());
                SimpleAudioEngine::sharedEngine()->stopEffect(GameLayer::_instance->getPrizeRollingEffectID());
            }
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3f,pLogonScene));
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/4.mp3", true);
            m_pLogonScene = pLogonScene;
            m_pHallScene = NULL;
            m_pTableScene = NULL;
            m_pCheckNetworkLayer = NULL;
            m_bLinkHallEnter = false;
            //            initConnect();
        }
            break;
        case eChargeGameMoney:
        {
            if(bConfirm)
            {
                if(GameMainScene::_instance != NULL)
                {
                    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                    CCMenuItem* pItem = CCMenuItem::create();
                    pItem -> setTag(302);
                    GameMainScene::GetInstance() -> itemCallBack(pItem);
                }
            }
        }
            break;
        case eChargeBackHall:
        {
            m_bGameChargeStatus = false;
            m_nBuyCannonIndex = 0;
            m_nBuyCannon2Index = 0;
            switchLayer(eHallLayer);
            break;
        }
        case eCloseDuihuan:
        {
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    FirstChargeLayer* pFirstChargeLayer = (FirstChargeLayer*)pHallLayer->getChildByTag(19986);
                    if(pFirstChargeLayer != NULL)
                    {
                        pFirstChargeLayer -> removeFromParent();
                    }
                }
            }
            
            break;
        }
        case eCancelLogon:
        {
            SendData(m_nHallSocketHandle, MDM_GP_LOGON, SUB_GPR_CANCEL_LOGINING);
            if(m_pLogonScene == NULL)
            {
                if(GameLayer::_instance != NULL)
                {
                    SimpleAudioEngine::sharedEngine()->stopEffect(GameLayer::_instance->getGameRollingEffectID());
                    SimpleAudioEngine::sharedEngine()->stopEffect(GameLayer::_instance->getPrizeRollingEffectID());
                }
                CCScene* pLogonScene = CLogonLayer::scene();
                CMainLogic::sharedMainLogic()->m_bEnterLoading = false;
                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3f,pLogonScene));
                m_pLogonScene = pLogonScene;
                m_pHallScene = NULL;
                m_pTableScene = NULL;
                m_pCheckNetworkLayer = NULL;
                m_bLinkHallEnter = false;
            }
            break;
        }
        case eVersion:
        {
            if(bConfirm)
            {
                sendUserBehavior(m_nUserID, eRemindUpdate2);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                ChargeWebView::linkToDownLoad();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                CMainLogic::sharedMainLogic()->webRequestDownload();
#endif
            }
            else
            {
                sendUserBehavior(m_nUserID, eRemindUpdate3);
            }
            if(m_nCurrentViewIndex == 2)//弹出其他游戏的下载地址
            {
                
            }
            else if(m_nCurrentViewIndex == 3)//弹出登录奖励框
            {
                if(m_pHallScene != NULL)
                {
                    CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene -> getChildByTag(eHallLayerTag);
                    if(pHallLayer != NULL)
                    {
                        if(CCUserDefault::sharedUserDefault()->getStringForKey("ShowLogonGift", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("logon"))
                        {
                            CCUserDefault::sharedUserDefault()->setStringForKey("ShowLogonGift", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("logon"));
                        }
                    }
                }
            }
        }
            break;
        case eBQSignIn:
        {
        }
            break;
        case eSignInExtra://签满七天，领取额外奖励
        {
        }
            break;
        case eExitHall://退出大厅
        {
            if(bConfirm)
            {
                //CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                sendNotification(true);
#endif
            }
        }
            break;
        case eExitGame://退出游戏
        {
            if(bConfirm)
            {
                if(GameMainScene::_instance != NULL)
                {
                    GameMainScene::_instance->exitMainGame();
                }
                //CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                sendNotification(true);
				sendUserBehavior(m_nUserID, eAndroidExitGameOK);
#endif
            }
            else
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                m_bExitGame = false;
				sendUserBehavior(m_nUserID, eAndroidExitGameCancel);
#endif
            }
            
        }
            break;
        case eShareAction:
        {
            if(GameMainScene::_instance != NULL)
            {
                GameMainScene::GetInstance()->setWndHave(false);
                GameMainScene::GetInstance()->setWndCalcHave(false);
                GameMainScene::GetInstance()->setTouchEnabled(true);
                GameMainScene::GetInstance()->setMenuTouch(true);
                GameMainScene::GetInstance()->setAutoFire();
            }
        }
            break;
        case eGameToVIP:
        {
            if(bConfirm)
            {
                if(GameMainScene::_instance != NULL)
                {
                    m_bEnterGiveCoin = false;
                    m_bGameToVIP = true;
                    m_nExitGameToSpecialArm = 0;
                    GameMainScene::_instance->exitGame();
                }
            }
            break;
        }
        case eTakeChargeCount:
        {
            if(bConfirm)
            {
                //取出充值成功的金币
                CMD_GP_UserTakeScore UserTakeScore = {0};
                UserTakeScore.dwUserID = m_nUserID;
                UserTakeScore.lTakeScore = m_llChargeCount;
                std::string passwordText = "";
                if(CCUserDefault::sharedUserDefault()->getBoolForKey("NewGuestBank"))
                {
                    passwordText = CCUserDefault::sharedUserDefault()->getStringForKey("NewGuestPassword");
                }
                else
                {
                    passwordText = CCUserDefault::sharedUserDefault()->getStringForKey("NewPassword");
                }
                char szMD5Result[128];
                CEncryptMD5::toString32(passwordText.c_str(),szMD5Result);
                strncpy(UserTakeScore.szPassword,szMD5Result,sizeof(UserTakeScore.szPassword));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                std::string uuidStr = ChargeWebView::getUIDevice();
                memcpy(UserTakeScore.szMachineID, uuidStr.c_str(), LEN_MACHINE_ID);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                JniMethodInfo info;
                bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
                jobject jobj;
                if(bHave)
                {
                    jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
                    bool bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","getUIDevice", "()Ljava/lang/String;");
                    if(bHave2)
                    {
                        jstring jstr0 = (jstring)info.env->CallObjectMethod(jobj, info.methodID);
                        std::string machineStr = JniHelper::jstring2string(jstr0);
                        memcpy(UserTakeScore.szMachineID, machineStr.c_str(), LEN_MACHINE_ID);
                    }
                }
                else
                {
                    memcpy(UserTakeScore.szMachineID, "gaixuliehaohuoqushibai", LEN_MACHINE_ID);
                }
#endif
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GP_USER_TAKE_SCORE,&UserTakeScore,sizeof(UserTakeScore));
            }
            else
            {
                CMD_GPR_ChargeCancel ChargeCancel = {0};
                ChargeCancel.dwUserID = m_nUserID;
                SendData(m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_CHARGE_CANCEL, &ChargeCancel, sizeof(ChargeCancel));
            }
        }
            break;
        case eMBEnterNiuRoom:
        {
            if(bConfirm)
            {
                if(m_pTableScene != NULL)
                {
                    CTableLayer * layer = (CTableLayer *)m_pTableScene->getChildByTag(eTableLayer);
                    
                    if (layer != NULL)
                    {
                        layer -> setEnterGame();
                        CCMenuItem* item = CCMenuItem::create();
                        item->setTag(202);
                        layer->menuItemCall(item);
                    }
                }
            }
        }
            break;
        case eMBBackHall:
        {
            if(bConfirm)
            {
                if(m_pTableScene != NULL)
                {
                    CTableLayer * layer = (CTableLayer *)m_pTableScene->getChildByTag(eTableLayer);
                    
                    if (layer != NULL)
                    {
                        CCMenuItem* item = CCMenuItem::create();
                        item->setTag(200);
                        layer->menuItemCall(item);
                    }
                }
            }
        }
            break;
        case eReCheckWXChargeResult://微信获取充值结果失败
        {
            if(bConfirm)//重新请求支付结果
            {
                ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
                if(pChargeLayer != NULL)
                {
                    pChargeLayer->sendWXChargeSuccess();
                }
            }
        }
            break;
        case eReCheckAlipayChargeResult://支付宝获取充值结果失败
        {
            if(bConfirm)//重新请求支付结果
            {
                ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
                if(pChargeLayer != NULL)
                {
                    pChargeLayer->sendAlipayChargeSuccess();
                }
            }
        }
            break;
        case eMBFreeCharge://跳转到充值
        {
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    FreeCoinLayer* pFreeCoinLayer = (FreeCoinLayer*)pHallLayer->getChildByTag(19996);
                    if(pFreeCoinLayer != NULL)
                    {
                        pFreeCoinLayer->removeWnd();
                    }
                    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
                    CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器...", eMBExitGame);
                    CMD_GPR_QuerySkill QuerySkill = {0};
                    QuerySkill.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                    CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_SKILL, SUB_GPR_QUERY_SKILL, &QuerySkill, sizeof(QuerySkill));
                }
            }
        }
            break;
        case eShowVipUpgrade:
        {
            //显示VIP升级界面
            CCNode* pNode = CCDirector::sharedDirector()->getRunningScene();
            if(pNode != NULL)
            {
                bool bLayerExist = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                Charge2Layer* pCharge2Layer = (Charge2Layer*)pNode->getChildByTag(20002);
                if(pCharge2Layer != NULL)
                {
                    pCharge2Layer->showUpgradeVIP();
                    bLayerExist = true;
                    break;
                }
#endif
                ChargeLayer* pChargeLayer = (ChargeLayer*)pNode->getChildByTag(20000);
                if(pChargeLayer != NULL)
                {
                    pChargeLayer->showUpgradeVIP();
                    bLayerExist = true;
                }
                if(!bLayerExist)//如果界面不存在，说明是从其他地方进行VIP升级，那么在CMainLogic中显示出来
                {
                    SimpleAudioEngine::sharedEngine()->playEffect("Music/vipupgradeeffect.wav");
                    UpgradeVIPLayer* pUpgradeVIPLayer = UpgradeVIPLayer::create();
                    CCDirector::sharedDirector()->getRunningScene()->addChild(pUpgradeVIPLayer, 12);
                    float fSound = CCUserDefault::sharedUserDefault()->getFloatForKey("sound", 1);
                    if(fSound > 0.4f)
                    {
                        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.1f);
                    }
                }
            }
        }
            break;
        case eShowMobileBinding:
        {
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    BandingLayer* pBandingLayer = BandingLayer::create();
                    pHallLayer->addChild(pBandingLayer, 300);
                }
            }
            break;
        }
        case eExitSuperToHall:
        {
            if(GameLayer::_instance != NULL)
            {
                m_pMessageBox->Update();
                m_bSuperToBY = true;
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperScoreMinusOK);
                GameLayer::_instance->exitGame();
            }
            break;
        }
        case eCloseGiveLayer:
        {
            if(GameMainScene::_instance != NULL)
            {
                GiveCoinLayer* pGiveCoinLayer = (GiveCoinLayer*)GameMainScene::_instance->getChildByTag(eGiveCoinLayerTag);
                if(pGiveCoinLayer != NULL)
                {
                    pGiveCoinLayer->removeFromParent();
                }
            }
            break;
        }
        case eDuihuanWufuCard:
        {
            if(bConfirm)
            {
                if(m_pHallScene != NULL)
                {
                    CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                    if(pHallLayer != NULL)
                    {
                        WufuLayer* pWufuLayer = (WufuLayer*)pHallLayer->getChildByTag(19983);
                        if(pWufuLayer != NULL)
                        {
                            pWufuLayer->sendDuihuanCard(pWufuLayer->getWufuType());
                        }
                    }
                }
                else if(GameMainScene::_instance != NULL)
                {
                    WufuLayer* pWufuLayer = (WufuLayer*)GameMainScene::_instance->getChildByTag(19983);
                    if(pWufuLayer != NULL)
                    {
                        pWufuLayer->sendDuihuanCard(pWufuLayer->getWufuType());
                    }
                }
            }
            break;
        }
        case eShowBindingTel://显示绑定手机界面
        {
            CCNode* pNode = CCDirector::sharedDirector()->getRunningScene();
            if(pNode != NULL)
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                Charge2Layer* pCharge2Layer = (Charge2Layer*)pNode->getChildByTag(20002);
                if(pCharge2Layer != NULL)
                {
                    BandingLayer* pBandingLayer = BandingLayer::create();
                    pCharge2Layer->addChild(pBandingLayer, 100);
                    break;
                }
#endif
                ChargeLayer* pChargeLayer = (ChargeLayer*)pNode->getChildByTag(20000);
                if(pChargeLayer != NULL)
                {
                    BandingLayer* pBandingLayer = BandingLayer::create();
                    pChargeLayer->addChild(pBandingLayer, 100);
                }
                OneMoneyLayer* pOneMoneyLayer = (OneMoneyLayer*)pNode->getChildByTag(20004);
                if(pOneMoneyLayer != NULL)
                {
                    pOneMoneyLayer->removeFromParent();
                    BandingLayer* pBandingLayer = BandingLayer::create();
                    pNode->addChild(pBandingLayer,2);
                }
            }
            
            break;
        }
        case eExitFBToHall:
        {
            m_pMessageBox->Update();
            if(GameMainScene::_instance != NULL)
            {
                GameMainScene::_instance->exitGame();
            }
            break;
        }
        case eExitGameBYToHall:
        {
            if(bConfirm)//点击安卓返回键退出到登录界面
            {
                m_bAutoExitGame = true;
                if(GameMainScene::_instance != NULL)
                {
                    GameMainScene::_instance->exitGame();
                }
                sendUserBehavior(m_nUserID, eAndroidExitGameBYToHallOK);
            }
            else
            {
                sendUserBehavior(m_nUserID, eAndroidExitGameBYToHallCancel);
            }
            break;
        }
        case eShowSuperCharge:
        {
            ChargeLayer* pChargeLayer = ChargeLayer::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeLayer, 11, 20000);
            break;
        }
        case eEnterBYToSuper:
        {
            if(bConfirm)
            {
                CMainLogic::sharedMainLogic()->m_bEnterGiveCoin = false;
                CMainLogic::sharedMainLogic()->m_bGameToVIP = false;
                CMainLogic::sharedMainLogic()->m_nExitGameToSpecialArm = 0;
                CMainLogic::sharedMainLogic()->m_bBYGameToSuper = true;
                GameMainScene::_instance->exitGame();
            }
            break;
        }
        case eExitGameHDToHall:
        {
            if(bConfirm)//点击安卓返回键退出到登录界面
            {
                m_bAutoExitGame = true;
                if(GameLayer::_instance != NULL)
                {
                    GameLayer::_instance->exitGame();
                }
            }
            break;
        }
        case eExitGameToHall:
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            if(m_nBuyCannon2Index>0)//购买指定的炮台
            {
                m_nBuyCannon2Index = 0;
                m_nBuyCannonIndex = 0;
                Charge2Layer* pCharge2Layer = (Charge2Layer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20002);
                if(pCharge2Layer != NULL)
                {
                    pCharge2Layer -> removeFromParent();
                }
            }
            if(m_bGameChargeStatus)//如果在游戏中购买
            {
                m_bGameChargeStatus = false;
                m_nBuyCannonIndex = 0;
                m_nBuyCannon2Index = 0;
                switchLayer(eHallLayer);
            }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if(GameMainScene::_instance != NULL || GameLayer::_instance != NULL)
            {
                m_bGameChargeStatus = false;
                m_nBuyCannonIndex = 0;
                m_nBuyCannon2Index = 0;
                switchLayer(eHallLayer);
            }
#endif
        }
            break;
        case eExitToHall:
        {
            if(bConfirm)
            {
                CMainLogic::sharedMainLogic()->m_bEnterGiveCoin = false;
                CMainLogic::sharedMainLogic()->m_bGameToVIP = true;
                CMainLogic::sharedMainLogic()->m_nExitGameToSpecialArm = 0;
                GameMainScene::_instance->exitGame();
            }
        }
            break;
        case eDeleteUserInfoLayer:
        {
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    FreeCoinLayer* pFreeCoinLayer = (FreeCoinLayer*)pHallLayer->getChildByTag(19996);
                    if(pFreeCoinLayer != NULL)
                    {
                        pFreeCoinLayer->stopBindingAction();
                        
                        CCMenuItem* item = CCMenuItem::create();
                        item->setTag(115);
                        pFreeCoinLayer->clickMenu(item);
                    }
                }
            }
        }
            break;
        case eDisConnectBackToLogon:
        {
            if(m_pLogonScene != NULL)
                break;
            //@@保证再次进入房间时，桌子信息是正确的
            for (unsigned int i = 0; i < m_vUserInfo.size(); i++)
            {
                delete m_vUserInfo[i];
            }
            if(GameMainScene::_instance != NULL)
            {
                if(KIND_ID == NZNH_KIND_ID || KIND_ID == YQS_KIND_ID || KIND_ID == JCBY_KIND_ID)
                {
                    m_lUserScore = GameMainScene::_instance->getUserCurrentScore();
                }
            }
            if(GameLayer::_instance != NULL)
            {
                SimpleAudioEngine::sharedEngine()->stopEffect(GameLayer::_instance->getGameRollingEffectID());
                SimpleAudioEngine::sharedEngine()->stopEffect(GameLayer::_instance->getPrizeRollingEffectID());
            }
            m_vUserInfo.clear();
            m_pLogonScene = NULL;
            CMainLogic::sharedMainLogic()->m_bEnterLoading = false;
            CCScene* pLogonScene = CLogonLayer::scene();
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3f,pLogonScene));
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/4.mp3", true);
            m_pLogonScene = pLogonScene;
            m_pHallScene = NULL;
            m_pTableScene = NULL;
            m_pCheckNetworkLayer = NULL;
            m_bLinkHallEnter = true;
        }
            break;
    }
}

bool CMainLogic::OnChargeSuccess(const void * pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GPO_ChargeValue))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GPO_ChargeValue* pChargeValue = (CMD_GPO_ChargeValue*)pData;
    m_llChargeCount = pChargeValue->dwScore;
    if(pChargeValue->dwScore != 0)
    {
        //客户端手动判断是不是首充
        if(m_llChargeCount == 200000)//据目前，只有首充或者短信充值才有200000的可能，所以这里只要判断是不是短信充值就可以了
        {
            if(!m_bClickMessageCharge)//不是短信充值
            {
                CMD_GPR_Charge1stRequest Charge1stRequest;
                Charge1stRequest.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_CHARGE1ST_REQUEST,&Charge1stRequest,sizeof(Charge1stRequest));
                CMainLogic::sharedMainLogic()->ShowMessage("正在验证充值结果", eMBExitGame);
                return true;
            }
        }
        
        //显示是否立即充值界面
        char temp[128];
        sprintf(temp, "充值成功，%lld游戏币保存到银行中，是否立即取出？", m_llChargeCount);
        ShowMessage(temp, eMBOKCancel, eTakeChargeCount);
    }
    return true;
}

tagUserInfo * CMainLogic::SearchUserByUserID(unsigned int dwUserID)
{
    tagUserInfo * pUserInfo = NULL;
    for (unsigned int i = 0; i<m_vUserInfo.size(); i++)
    {
        if (dwUserID == m_vUserInfo[i]->dwUserID)
        {
            pUserInfo = m_vUserInfo[i];
            break;
        }
    }
    return pUserInfo;
}

void CMainLogic::ActiveUserItem(tagUserInfo * pUserInfo)
{
    if (pUserInfo != NULL)
    {
        m_vUserInfo.push_back(pUserInfo);
        
        //桌子加入
        BYTE cbUserStatus = pUserInfo->cbUserStatus;
        if ((cbUserStatus >= US_SIT) && (cbUserStatus!=US_LOOKON))
        {
            CTableLayer * layer = NULL;
            if (m_pTableScene != NULL)
            {
                layer = (CTableLayer *)m_pTableScene->getChildByTag(eTableLayer);
            }
            WORD wTableID = pUserInfo->wTableID;
            WORD wChairID = pUserInfo->wChairID;
            if (layer != NULL)
            {
                layer->SetClientUserItem(wTableID, wChairID, pUserInfo);
            }
        }
    }
}

void CMainLogic::DeleteUserItem(unsigned int dwUserID)
{
    for (std::vector<tagUserInfo *>::iterator iter = m_vUserInfo.begin(); iter != m_vUserInfo.end(); iter++)
    {
        if ((*iter)->dwUserID == dwUserID)
        {
            tagUserInfo * pUserInfo = (*iter);
            WORD wLastTableID = pUserInfo->wTableID;
            WORD wLastChairID = pUserInfo->wChairID;
            
            //桌子离开
            if ((wLastTableID != INVALID_TABLE) && (wLastChairID != INVALID_CHAIR))
            {
                CTableLayer * layer = NULL;
                tagUserInfo * pTempUserInfo = NULL;
                if (m_pTableScene != NULL)
                {
                    layer = (CTableLayer *)m_pTableScene->getChildByTag(eTableLayer);
                    pTempUserInfo = layer->GetClientUserItem(wLastTableID, wLastChairID);
                    if (pTempUserInfo == pUserInfo)
                    {
                        layer->SetClientUserItem(wLastTableID, wLastChairID, NULL);
                    }
                }
            }
            m_vUserInfo.erase(iter);
            break;
        }
    }
}

//第四个参数代表支付宝（1）、微信（2）、银行卡（3）
//第五个参数代表金币（1）、武器（2）、技能（3）
void CMainLogic::switchLayer(eSwitchLayer eLayer, bool bDisconnect, bool bTableBack, int nShowChargeType, int nCoinArmSkill)
{
    switch (eLayer)
    {
        case eLogonLayer:
        {
            //m_pNetworkService->Disconnect(m_nHallSocketHandle);
            if(m_pNetworkService != NULL)
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                sendAndroidRoomExit();
                //SendData(m_pNetworkService->GetNetInfo(m_nHallSocketHandle), MDM_KN_COMMAND, SUB_KN_SHUT_MANUAL);
                sendAndroidRoomExit();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                sendRoomExit();
				//SendData(m_nHallSocketHandle, MDM_KN_COMMAND, SUB_KN_SHUT_MANUAL);
#endif
			}
            CCDirector::sharedDirector()->replaceScene(CLogonLayer::scene());
            m_bLinkHallEnter = false;
            m_pHallScene = NULL;
            CMainLogic::sharedMainLogic()->m_bEnterLoading = false;
            //            initConnect();
            break;
        }
        case eHallLayer:
        {
            if (bDisconnect)
            {
                //m_pNetworkService->Disconnect(m_nRoomSocketHandle);
                if(m_pNetworkService != NULL)
                {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    sendAndroidRoomExit();
                    //SendData(m_pNetworkService->GetNetInfo(m_nRoomSocketHandle), MDM_KN_COMMAND, SUB_KN_SHUT_MANUAL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    sendRoomExit();
					//SendData(m_nRoomSocketHandle, MDM_KN_COMMAND, SUB_KN_SHUT_MANUAL);
#endif
				}
                //@@保证再次进入房间时，桌子信息是正确的
                for (unsigned int i = 0; i < m_vUserInfo.size(); i++)
                {
                    delete m_vUserInfo[i];
                }
                m_vUserInfo.clear();
                m_pTableScene = NULL;
            }
            else
            {
                m_pMessageBox->Update();
                //m_pHallScene = CHallLayer::scene();
                if(m_pHallScene == NULL)
                {
                    m_pHallScene = CHallLayer::scene();
                }
                CHallLayer * layer = (CHallLayer *)m_pHallScene->getChildByTag(eHallLayerTag);
                if(layer!=NULL)
                {
                    if(!layer->getInitNetComplete())
                    {
                        m_nShowChargeType = nShowChargeType;
                        m_nShowCoinArmSkill = nCoinArmSkill;
                        layer->setMainLogic(this);
                        layer->setUserScore(m_lUserScore);
                        layer -> loadSelectedGameTypeNode(true, m_vGameType);//游戏房间选择界面
                        layer->setUserNickName(m_sNickName.c_str());
                        layer->setUserID(m_nGameID);
                        layer->setTicket(m_nLottery);
                        layer->notice();
                        layer->setWeekNum(m_nUserWeekBeiNum);
                        layer->setVipExp();//设置VIP等级、经验值百分比
                        layer->setGender(m_nTouxiangID);
                        layer->loadUI();
                        
                        if(m_nChargeBack != 0)
                        {
                            layer->showVIPAction(true);
                        }
                        if(!m_bIOSPassed)//如果没有通过审核，那么分享是没有的
                        {
                            m_bCanShare = false;
                        }
                        if(m_nLogonGift != 0 || m_bCanShare || m_cbCanGetBankruptcy>0 || !m_bReceiveVipLogon)
                        {
                            layer->showfreecoinAction(true);
                        }
                        
                        layer->showFirstCharge();
                        layer->showNiuGame();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        if(m_bGameChargeStatus)
                        {
                            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/4.mp3");
                        }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                        if(m_bGameChargeStatus || ChargeSuccess == 5)
                        {
                            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/4.mp3");
                        }
#endif
                        m_bShowNoticeLayer = false;
                        //如果不是点击图标进入的游戏，而是点击“登陆按钮”、“游客”、“注册”进入的游戏，那么就要切换场景
                        if(!m_bClickIcon)
                        {
                            CMainLogic::sharedMainLogic()->m_bEnterLoading = false;
                            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, m_pHallScene));//pushScene会导致memory leak
                        }
                    }
                }
                m_pTableScene = NULL;
                m_pLogonScene = NULL;
            }
            CMainLogic::sharedMainLogic()->setPlaying(false);
            break;
        }
        case eTableLayer:
        {
            m_pHallScene = NULL;
            CMainLogic::sharedMainLogic()->m_bEnterLoading = false;
            CCDirector::sharedDirector()->replaceScene(m_pTableScene);
            break;
        }
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//更新分数
void CMainLogic::setUserScore(long long userScore)
{
    m_lUserScore = userScore;
}

void CMainLogic::webRequestPay()
{
    char szUrl[128];
    char szToken[128];
    char szMD5Result[128];
    sprintf(szToken, "%dbuyu2015", m_nGameID);
    CEncryptMD5::toString32(szToken, szMD5Result);
    sprintf(szUrl, "http://wap.66y.com/pay.aspx?uid=%d&num=%s", m_nGameID, szMD5Result);
    webRequest(szUrl);
}

void CMainLogic::webRequestFeedBack()
{
    char szUrl[128];
    sprintf(szUrl, "http://%s/Feedback.aspx?gameid=%d&accname=%s", NORMAL_URL,m_nGameID, m_sAccount.c_str());
    webRequest(szUrl);
}

void CMainLogic::webRequestDownload()
{
    webRequest("http://lelebuyu.66y.com");
}

void CMainLogic::openRankInfo()
{
    char temp[64];
    sprintf(temp, "http://%s/RaceRule.htm", NORMAL_URL);
    JniMethodInfo minfo;
    jobject jobj;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "cn/sharesdk/hellocpp/HelloCpp","getInstance","()Ljava/lang/Object;");
    if (isHave)
    {
        //调用Java静态函数，取得对象。
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        if (jobj != NULL)
        {
            isHave = JniHelper::getMethodInfo(minfo,"cn/sharesdk/hellocpp/HelloCpp","openWebView","(Ljava/lang/String;)V");
            if (isHave)
            {
                jstring urlStr = minfo.env->NewStringUTF(temp);
                //调用java非静态函数, 参数1：Java对象，上面已经取得   参数2：方法ID
                minfo.env->CallVoidMethod(jobj, minfo.methodID, urlStr);
            }
        }
    }
}

void CMainLogic::openChargeInfo()
{
    char temp[64];
    sprintf(temp, "http://%s/hdRule.htm", NORMAL_URL);
    JniMethodInfo minfo;
    jobject jobj;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "cn/sharesdk/hellocpp/HelloCpp","getInstance","()Ljava/lang/Object;");
    if (isHave)
    {
        //调用Java静态函数，取得对象。
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        if (jobj != NULL)
        {
            isHave = JniHelper::getMethodInfo(minfo,"cn/sharesdk/hellocpp/HelloCpp","openWebView","(Ljava/lang/String;)V");
            if (isHave)
            {
                jstring urlStr = minfo.env->NewStringUTF(temp);
                //调用java非静态函数, 参数1：Java对象，上面已经取得   参数2：方法ID
                minfo.env->CallVoidMethod(jobj, minfo.methodID, urlStr);
            }
        }
    }
}

void CMainLogic::webRequest(const char * sUrl)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"cn/sharesdk/hellocpp/HelloCpp","openUrl","(Ljava/lang/String;)V");
    if (isHave)
    {
        jstring url = minfo.env->NewStringUTF(sUrl);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, url);
    }
}

void CMainLogic::readXML()
{
    unsigned long size;
    char *pFileContent =(char*)CCFileUtils::sharedFileUtils()->getFileData("b2ank/Notification.xml", "r", &size);
    TiXmlDocument* document = new TiXmlDocument;
    document->Parse(pFileContent,0,TIXML_ENCODING_UTF8);
    
    if(document != NULL)
    {
        TiXmlElement * rootElement=document->RootElement();
        if (NULL!=rootElement)
        {
            std::string nodename=rootElement->Value();
            if (0!=nodename.compare("Data"))
            {
                return;
            }
            TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
            while (pCurrentElement != NULL) {
                const char * pValue = pCurrentElement->Value();
                if(!std::strcmp(pValue, "parm"))//相同
                {
                    Tag_Notification* Notification = new Tag_Notification;
                    m_vctNotification.push_back(Notification);
                    Notification->sTime = "";
                    Notification->sNotice = "";
                    Notification->nHour = 0;
                    Notification->nMinute = 0;
                    Notification->nSecond = 0;
                    Notification->bDelay = 0;
                    
                    pValue = pCurrentElement->Attribute("delay");
                    if(NULL != pValue)
                    {
                        Notification->bDelay = atoi(pValue);
                    }
                    pValue = pCurrentElement->Attribute("second");
                    if(NULL != pValue)
                    {
                        Notification->nSecond = atoi(pValue);
                    }
                    pValue = pCurrentElement->Attribute("time");
                    if(NULL != pValue)
                    {
                        Notification->sTime = pValue;
                    }
                    pValue = pCurrentElement->Attribute("hour");
                    if(NULL != pValue)
                    {
                        Notification->nHour = atoi(pValue);
                    }
                    pValue = pCurrentElement->Attribute("minute");
                    if(NULL != pValue)
                    {
                        Notification->nMinute = atoi(pValue);
                    }
                    pValue = pCurrentElement->Attribute("notice");
                    if(NULL != pValue)
                    {
                        Notification->sNotice = pValue;
                    }
                    pCurrentElement = pCurrentElement->NextSiblingElement();
                }
            }
        }
    }
    delete document;
}

void CMainLogic::sendNotification(bool bExist)
{
    JniMethodInfo info;
    bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if(bHave)
    {
        jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
        bool bHave2 = false;
        for (int i = 0; i < m_vctNotification.size(); i++)
        {
            if(m_vctNotification[i]->bDelay)//退出游戏多长时间之后推送
            {
                bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","openAlert", "(Ljava/lang/String;Ljava/lang/String;II)V");
                if(bHave2)
                {
                    char subjectTemp[64];
                    sprintf(subjectTemp, "乐乐捕鱼");
                    char bodyTemp[128];
                    sprintf(bodyTemp, "%s", m_vctNotification[i]->sNotice.c_str());
                    
                    jstring subject = info.env->NewStringUTF(subjectTemp);
                    jstring body = info.env->NewStringUTF(bodyTemp);
                    
                    int nTime = atoi(m_vctNotification[i]->sTime.c_str());
                    
                    info.env->CallVoidMethod(jobj, info.methodID, subject, body,1, nTime);
                }
            }
            else//定时推送
            {
                bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","openAlert1", "(Ljava/lang/String;Ljava/lang/String;IIII)V");
                if(bHave2)
                {
                    char subjectTemp[64];
                    sprintf(subjectTemp, "乐乐捕鱼");
                    char bodyTemp[128];
                    sprintf(bodyTemp, "%s", m_vctNotification[i]->sNotice.c_str());
                    
                    jstring subject = info.env->NewStringUTF(subjectTemp);
                    jstring body = info.env->NewStringUTF(bodyTemp);
                    
                    int nHour = m_vctNotification[i]->nHour;
                    int nMinute = m_vctNotification[i]->nMinute;
                    int nSecond = m_vctNotification[i]->nSecond;
                    
                    info.env->CallVoidMethod(jobj, info.methodID, subject, body,0, nHour, nMinute, nSecond);
                }
            }
        }
        if(bExist)
        {
            //如果账号为空，那么还设置为游客登录
            if(CCUserDefault::sharedUserDefault()->getStringForKey("NewAccount") == "")
            {
                CCUserDefault::sharedUserDefault()->setBoolForKey("NewGuestBank", false);
            }
            
        }
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
        PluginChannel::getInstance()->logout();
#endif
		CCDirector::sharedDirector()->end();
    }
}

std::string CMainLogic::getMobileProduct()
{
    JniMethodInfo info;
    bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if(bHave)
    {
        jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
        bool bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","getUIDevice1", "(I)Ljava/lang/String;");
        if(bHave2)
        {
            jstring jstr0 = (jstring)info.env->CallObjectMethod(jobj, info.methodID, 0);
            m_sMobileProduct = JniHelper::jstring2string(jstr0);	//硬件制造商
        }
    }
    return m_sMobileProduct;
}

void CMainLogic::requestAndroidOpenStatus()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/Leleandroidconfigure.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onHttpAndroidOpenStatus));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CMainLogic::onHttpAndroidOpenStatus(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    //对str进行xml解析，将30条数据保存到m_vctRankTimes中
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("configure"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL) {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "item"))//相同
            {
                pValue = pCurrentElement->Attribute("name");
                if(NULL != pValue)
                {
                    if(!std::strcmp(pValue, "toutiao1"))
                    {
                        const char * pStatus = pCurrentElement->Attribute("open");
                        if(pStatus != NULL)
                        {
                            if(atoi(pStatus) <= 0)
                            {
                                m_bAndroidOpenStatus = false;
                                CCUserDefault::sharedUserDefault()->setBoolForKey("AndroidOpenStatusTouTiao", false);
                            }
                            else
                            {
                                m_bAndroidOpenStatus = true;
                                CCUserDefault::sharedUserDefault()->setBoolForKey("AndroidOpenStatusTouTiao", true);
                            }
                        }
                    }
                }
                pCurrentElement = pCurrentElement->NextSiblingElement();
            }
        }
    }
    delete document;
}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void CMainLogic::sendIOSNotification()
{
    for (int i = 0; i < m_vctNotification.size(); i++) {
        if(m_vctNotification[i]->bDelay)//退出游戏多长时间之后推送
        {
            CCNotificationManager::getNotificationManager()->notification3(m_vctNotification[i]->sNotice.c_str(), m_vctNotification[i]->sTime.c_str());
        }
        else//定时推送
        {
            char temp[24];
            sprintf(temp, "%d:%d:%d", m_vctNotification[i]->nHour, m_vctNotification[i]->nMinute, m_vctNotification[i]->nSecond);
            CCNotificationManager::getNotificationManager()->notification2(m_vctNotification[i]->sNotice.c_str(), temp);
        }
    }
}
#endif

void CMainLogic::requestNotification()
{
    for(int i = 0; i < m_vctNotification.size(); i++)
    {
        delete m_vctNotification[i];
    }
    m_vctNotification.clear();
    CCHttpRequest* request = new CCHttpRequest();
    char temp[128];
    sprintf(temp, "http://%s/GetPush.aspx?userid=%d", NORMAL_URL, m_nUserID);
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setTag("http test");
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onHttpRequestComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CMainLogic::onHttpRequestComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    //对str进行xml解析，将30条数据保存到m_vctRankTimes中
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("Data"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL) {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "parm"))//相同
            {
                Tag_Notification* Notification = new Tag_Notification;
                m_vctNotification.push_back(Notification);
                Notification->sTime = "";
                Notification->sNotice = "";
                Notification->nHour = 0;
                Notification->nMinute = 0;
                Notification->nSecond = 0;
                Notification->bDelay = 0;
                
                pValue = pCurrentElement->Attribute("delay");
                if(NULL != pValue)
                {
                    Notification->bDelay = atoi(pValue);
                }
                pValue = pCurrentElement->Attribute("second");
                if(NULL != pValue)
                {
                    Notification->nSecond = atoi(pValue);
                }
                pValue = pCurrentElement->Attribute("time");
                if(NULL != pValue)
                {
                    Notification->sTime = pValue;
                }
                pValue = pCurrentElement->Attribute("hour");
                if(NULL != pValue)
                {
                    Notification->nHour = atoi(pValue);
                }
                pValue = pCurrentElement->Attribute("minute");
                if(NULL != pValue)
                {
                    Notification->nMinute = atoi(pValue);
                }
                pValue = pCurrentElement->Attribute("notice");
                if(NULL != pValue)
                {
                    Notification->sNotice = pValue;
                }
                pCurrentElement = pCurrentElement->NextSiblingElement();
            }
        }
    }
    delete document;
}

void CMainLogic::readMailXML()
{
    char mailXML[24];
    sprintf(mailXML, "%dNewMailInfo.xml", m_nUserID);
    std::string mailxml = mailXML;
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + mailxml;
    //加载文档，读取文件
    TiXmlDocument* document = new TiXmlDocument(xmlPath.c_str());
    document -> LoadFile();
    if(document != NULL)
    {
        TiXmlElement * rootElement=document->RootElement();
        if (NULL!=rootElement)
        {
            std::string nodename=rootElement->Value();
            if (0!=nodename.compare("Data"))
            {
                return;
            }
            TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
            while (pCurrentElement != NULL) {
                const char * pValue=NULL;
                std::string nodename = pCurrentElement->Value();
                if (0 == nodename.compare("information"))
                {
                    tagMailInfo MailInfo = {0};
                    pValue=pCurrentElement->Attribute("id");
                    if (NULL!=pValue)
                    {
                        int id = atoi(pValue);
                        MailInfo.dwMailID = id;
                    }
                    std::string pValueTime=pCurrentElement->Attribute("time");
                    if (pValueTime != "")
                    {
                        MailInfo.szMailTime = pValueTime;
                    }
                    std::string pValueTitle=pCurrentElement->Attribute("title");
                    if (pValueTitle != "")
                    {
                        MailInfo.szMailTitle= pValueTitle;
                    }
                    pValue=pCurrentElement->Attribute("read");
                    if (NULL!=pValue)
                    {
                        MailInfo.cbMailStatus = (BYTE)atoi(pValue);
                    }
                    pValue=pCurrentElement->Attribute("geted");
                    if (NULL!=pValue)
                    {
                        MailInfo.cbGeted = (BYTE)atoi(pValue);
                    }
                    std::string pValueStr = pCurrentElement->Attribute("content");
                    if (pValueStr != "")
                    {
                        MailInfo.szMailContent = pValueStr;
                    }
                    pValueStr = pCurrentElement->Attribute("duihuanma");
                    if (pValueStr != "")
                    {
                        MailInfo.szDuihuanma = pValueStr;
                    }
                    pValueStr = pCurrentElement->Attribute("gongzhonghao");
                    if (pValueStr != "")
                    {
                        MailInfo.szGongzhonghao = pValueStr;
                    }
                    if(MailInfo.dwMailID != 0)
                    {
                        m_vctMail.push_back(MailInfo);
                    }
                }
                pCurrentElement = pCurrentElement->NextSiblingElement();
            }
        }
    }
    delete document;
}

void CMainLogic::writeMailXML()
{
    char mailXML[24];
    sprintf(mailXML, "%dNewMailInfo.xml", m_nUserID);
    std::string mailxml = mailXML;
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + mailxml;
    
    //加载文档，读取文件
    TiXmlDocument* document = new TiXmlDocument(xmlPath.c_str());
    document -> LoadFile();
    if(document != NULL)
    {
        TiXmlElement * rootElement=document->RootElement();
        if (NULL!=rootElement)
        {
            std::string nodename=rootElement->Value();
            if (0!=nodename.compare("Data"))
            {
                return;
            }
            int i = 0;
            TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
            while (pCurrentElement != NULL) {
                if(i >= m_vctMail.size())
                    break;
                std::string nodename = pCurrentElement->Value();
                if (0 == nodename.compare("information"))
                {
                    pCurrentElement->SetAttribute("id", m_vctMail[i].dwMailID);
                    pCurrentElement->SetAttribute("time", m_vctMail[i].szMailTime.c_str());
                    pCurrentElement->SetAttribute("title", m_vctMail[i].szMailTitle.c_str());
                    pCurrentElement->SetAttribute("read", m_vctMail[i].cbMailStatus);
                    pCurrentElement->SetAttribute("geted", m_vctMail[i].cbGeted);
                    pCurrentElement->SetAttribute("content", m_vctMail[i].szMailContent.c_str());
                    pCurrentElement->SetAttribute("duihuanma", m_vctMail[i].szDuihuanma.c_str());
                    pCurrentElement->SetAttribute("gongzhonghao", m_vctMail[i].szGongzhonghao.c_str());
                }
                i++;
                pCurrentElement = pCurrentElement->NextSiblingElement();
            }
        }
    }
    document->SaveFile();
    delete document;
}

void CMainLogic::createMailXML()
{
    char temp[24];
    sprintf(temp, "%dNewMailInfo", m_nUserID);
    if(CCUserDefault::sharedUserDefault()->getBoolForKey(temp, false))
    {
        return;
    }
    char mailXML[24];
    sprintf(mailXML, "%s.xml", temp);
    //在Documents目录下创建一个可读写的文件
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string mailxml = mailXML;
    std::string xmlPath = path + mailxml;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string path2 = CCFileUtils::sharedFileUtils() -> fullPathForFilename("NewMailInfo.xml");
    FILE* bundleFile = fopen(path2.c_str(), "r");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    unsigned long size;
    char* path2 = (char*)CCFileUtils::sharedFileUtils()->getFileData("b2ank/NewMailInfo.xml", "r", &size);
    FILE* bundleFile = fopen(path2, "r");
#endif
    char* buf;
    if(bundleFile)//读取工程目录下score.xml文件中的内容到buf中
    {
        int len;
        fseek(bundleFile, 0, SEEK_END);
        len = (int)ftell(bundleFile);
        rewind(bundleFile);
        buf = (char*)malloc(len + 1);
//        if(!buf)
//        {
//            CCLog("There has no memory");
//        }
        
        int rLen = (int)fread(buf, sizeof(char), len, bundleFile);
        buf[rLen] = '\0';
        fclose(bundleFile);
    }
    
    FILE* file = fopen(xmlPath.c_str(), "w");
    if(file)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        fputs(buf, file);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        fputs(path2, file);
#endif
        fclose(file);
//        CCLog("save succeed");
        CCUserDefault::sharedUserDefault()->setBoolForKey(temp, "true");
    }
    else
    {
//        CCLog("save file error");
    }
}

void CMainLogic::createLocalVIPConfig()
{
    //在Documents目录下创建一个可读写的文件
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string mailxml = "VipNewConfig.xml";
    std::string xmlPath = path + mailxml;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string path2 = CCFileUtils::sharedFileUtils() -> fullPathForFilename("VipNewConfig.xml");
    //path2.c_str（）   将string类型转换成const char＊类型
    FILE* bundleFile = fopen(path2.c_str(), "r");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    unsigned long size;
    char* path2 = (char*)CCFileUtils::sharedFileUtils()->getFileData("b2ank/VipNewConfig.xml", "r", &size);
    FILE* bundleFile = fopen(path2, "r");
#endif
    char* buf;
    if(bundleFile)//读取工程目录下score.xml文件中的内容到buf中
    {
        int len;
        fseek(bundleFile, 0, SEEK_END);
        len = (int)ftell(bundleFile);
        rewind(bundleFile);
        buf = (char*)malloc(len + 1);
        if(!buf)
        {
//            CCLog("There has no memory");
        }
        
        int rLen = (int)fread(buf, sizeof(char), len, bundleFile);
        buf[rLen] = '\0';
        fclose(bundleFile);
    }
    
    FILE* file = fopen(xmlPath.c_str(), "w");
    if(file)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        fputs(buf, file);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        fputs(path2, file);
#endif
        fclose(file);
//        CCLog("save succeed");
        CCUserDefault::sharedUserDefault()->setBoolForKey("VipNewConfig", "true");
    }
    else
    {
//        CCLog("save file error");
    }
}

void CMainLogic::readVIPConfig()
{
    m_vctVipExpConfig.clear();
    std::string mailxml = "VipNewConfig.xml";
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + mailxml;
    
    //加载文档，读取文件
    TiXmlDocument* document = new TiXmlDocument(xmlPath.c_str());
    document -> LoadFile();
    if(document != NULL)
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("vipConfigInfos"))
        {
            delete document;
            return;
        }
        TiXmlElement * pCurrentElement = rootElement->FirstChildElement();
        while (NULL != pCurrentElement)
        {
            const char * pValue=pCurrentElement->Value();
            if (!strcmp(pValue,"expItem"))
            {
                Tag_VIPExpConfig VIPExpConfig = {0};
                TiXmlElement* firstElement = pCurrentElement -> FirstChildElement();
                const char * valueId=firstElement->Value();
                if (!strcmp(valueId,"level"))
                {
                    int nLevel=atoi(firstElement->GetText());
                    
                    VIPExpConfig.nLevel = nLevel;
                }
                TiXmlElement* secondElement = firstElement -> NextSiblingElement();
                const char * chargeCount=secondElement->Value();
                if (!strcmp(chargeCount,"chargeCount"))
                {
                    int nChargeCount=atoi(secondElement->GetText());
                    VIPExpConfig.nChargeCount = nChargeCount;
                }
                
                m_vctVipExpConfig.push_back(VIPExpConfig);
            }
            pCurrentElement=pCurrentElement->NextSiblingElement();
        }
    }
    document->SaveFile();
    delete document;
}

void CMainLogic::freeMemory()
{
    ActionManager::purge();
    CCAnimationCache::purgeSharedAnimationCache();
    CCArmatureDataManager::sharedArmatureDataManager()->purge();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
    CCTextureCache::sharedTextureCache()->removeAllTextures();
    
    DictionaryHelper::shareHelper()->purgeDictionaryHelper();
}

void CMainLogic::requestChargeNotice()
{
    CCHttpRequest* request = new CCHttpRequest();
    char temp[128];
    sprintf(temp, "http://%s/GetMobAnn.aspx?id=3", NORMAL_URL);
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setTag("http test");
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onHttpNoticeComplete));
    CCHttpClient::getInstance()->send(request);
    CC_SAFE_RELEASE(request);
    request = NULL;
}

void CMainLogic::onHttpNoticeComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    m_sChargeNotice = str;
}

void CMainLogic::checkChargeCompleted()
{
    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
    pScheduler->scheduleSelector(schedule_selector(CMainLogic::checkChargeCompleted), this, 1.0f, 0, 0, false);
    //this -> scheduleOnce(schedule_selector(CMainLogic::checkChargeCompleted), 1.0f);
}

void CMainLogic::checkChargeCompleted(float dt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(ChargeSuccess == 5 && m_pNetworkService != NULL)
    {
        //发送检测包
        m_nCheckSocketShut = 11;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        SendData(m_pNetworkService->GetNetInfo(m_nHallSocketHandle), MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		SendData(m_nHallSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#endif
		CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
        pScheduler->scheduleSelector(schedule_selector(CMainLogic::receive2HallLinkInfo), this, 1.0f, 0, 0, false);
        
        if(m_bGameChargeStatus == false && GameMainScene::_instance != NULL)//说明购买的是普通炮台或者技能
        {
            GameMainScene::_instance->sendQuerySkillStatus();
        }
        if(m_bGameChargeStatus || m_bBuyTryCannon)//在游戏中没有通过支付宝或者微信来进入前台，而是手动进入前台 或者购买的是试用炮，也需要返回到大厅
        {
            m_bGameChargeStatus = false;
            switchLayer(eHallLayer);
        }
        
    }
    
#endif
}

void CMainLogic::receive2HallLinkInfo(float dt)
{
    m_nClickBank = 11;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CNetInfo * pNetInfo = NULL;
    if (!NATIVE_TEST)
    {
        pNetInfo = m_pNetworkService->Connect(m_sNodeIP.c_str(), PORT_LOGON, NULL, NULL, true);
    }
    else
    {
        pNetInfo = m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON, NULL, NULL, false);
    }
    m_nHallSocketHandle = m_pNetworkService->GetNetID(pNetInfo);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (!NATIVE_TEST)
    {
        m_nHallSocketHandle = m_pNetworkService->Connect(m_sNodeIP.c_str(), PORT_LOGON);
    }
    else
    {
        m_nHallSocketHandle = m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON);
    }
#endif
}

void CMainLogic::updateCurrentScore()
{
    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
    pScheduler->scheduleSelector(schedule_selector(CMainLogic::synchData), this, 5.0f, 0, 0, false);
}

void CMainLogic::sendVIPGrade()
{
    if(CCUserDefault::sharedUserDefault()->getIntegerForKey("USERVIPLEVEL") != m_nVipLevel)
    {
        CMD_GP_VIPUpgrade VIPUpgrade = {0};
        VIPUpgrade.cbVIPLevel = m_nVipLevel;
        strncpy(VIPUpgrade.szNickName,CCharCode::UTF8ToGB2312(m_sNickName.c_str()).c_str(), sizeof(VIPUpgrade.szNickName));
        SendData(m_nHallSocketHandle,MDM_GP_USER_NOTICE,SUB_GPR_VIPUPGRADE,&VIPUpgrade,sizeof(VIPUpgrade));
        CCUserDefault::sharedUserDefault()->setIntegerForKey("USERVIPLEVEL", m_nVipLevel);
    }
    
}

/******************************************Advert.xml*************************************/


void CMainLogic::requestIAPMail()
{
    if(m_pNetworkService != NULL)
    {
        m_nCheckSocketShut = 18;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_pNetworkService->GetNetInfo(CMainLogic::sharedMainLogic()->m_nHallSocketHandle), MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		SendData(m_nHallSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#endif
		//this->scheduleOnce(schedule_selector(CMainLogic::receiveIOSHallLink2), 1.0f);
        CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
        pScheduler->scheduleSelector(schedule_selector(CMainLogic::receiveIOSHallLink2), this, 1.5f, 0, 0, false);
    }
    
}

void CMainLogic::receiveIOSHallLink2()
{
    if(m_pNetworkService != NULL)
    {
        m_nClickBank = 18;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CNetInfo * pNetInfo = NULL;
        if (!NATIVE_TEST)
        {
            pNetInfo = m_pNetworkService->Connect(m_sNodeIP.c_str(), PORT_LOGON, NULL, NULL, true);
        }
        else
        {
            pNetInfo = m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON, NULL, NULL, false);
        }
        m_nHallSocketHandle = m_pNetworkService->GetNetID(pNetInfo);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		if (!NATIVE_TEST)
        {
            m_nHallSocketHandle = m_pNetworkService->Connect(m_sNodeIP.c_str(), PORT_LOGON);
        }
        else
        {
            m_nHallSocketHandle = m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON);
        }
#endif
    }
    
}

void CMainLogic::requestMail()
{
    CMD_GPR_MailIDList MailIDList;
    MailIDList.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    CMainLogic::sharedMainLogic() -> SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_QUERY_MAIL_ID_LIST, &MailIDList, sizeof(MailIDList));
}

//每隔5分钟发送一次检测包，断了就重新连接登录服务器，连接成功之后不做任何处理
void CMainLogic::sendHallLink(float dt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string str = ChargeWebView::OnInternet();
    if(str == "")
    {
        CMainLogic::sharedMainLogic()->ShowMessage("您当前网络不可用，请检查网络设置", eMBOK, eDisConnectNet);
        return;
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string str = OnInternet();
    if(str == "2")
    {
        CMainLogic::sharedMainLogic()->ShowMessage("您当前网络不可用，请检查网络设置", eMBOK, eDisConnectNet);
        return;
    }
#endif
    if(m_bStartOpenCheck)
    {
        if(m_bReceiveHallLink)//如果收到了，就继续发检测包
        {
            m_nCheckSocketShut = 16;
            if(m_pNetworkService != NULL)
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                SendData(m_pNetworkService->GetNetInfo(m_nHallSocketHandle), MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				SendData(m_nHallSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#endif
			}
            
        }
        else//如果没有收到检测包，那么就重新连接
        {
            if(m_pNetworkService != NULL)
            {
                m_nClickBank = 16;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                CNetInfo * pNetInfo = NULL;
                if (!NATIVE_TEST)
                {
                    pNetInfo = m_pNetworkService->Connect(m_sNodeIP.c_str(), PORT_LOGON, NULL, NULL, true);
                }
                else
                {
                    pNetInfo = m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON, NULL, NULL, false);
                }
                m_nHallSocketHandle = m_pNetworkService->GetNetID(pNetInfo);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				if (!NATIVE_TEST)
                {
                    m_nHallSocketHandle = m_pNetworkService->Connect(m_sNodeIP.c_str(), PORT_LOGON);
                }
                else
                {
                    m_nHallSocketHandle = m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON);
                }
#endif
            }
            //根据这个变量判断定时器有没有开启
            if(!m_b2DisConnect)
            {
                m_n2DisConnect = 7;
                //开启定时器，只执行七次
                CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
                pScheduler->scheduleSelector(schedule_selector(CMainLogic::showDisConnect), this, 1.0f, false);
                m_b2DisConnect = true;
            }
        }
        m_bReceiveHallLink = false;
    }
}

void CMainLogic::showDisConnect()
{
    m_n2DisConnect--;
    if(m_n2DisConnect <= 0)
    {
        CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
        pScheduler->unscheduleSelector(schedule_selector(CMainLogic::showDisConnect), this);
        ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
        if(pChargeLayer == NULL)
        {
            //此时网络已经断开，给出提示框，正在重新连接
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    if(m_pCheckNetworkLayer == NULL)
                    {
                        m_pCheckNetworkLayer = CheckNetworkLayer::create();
                        pHallLayer -> addChild(m_pCheckNetworkLayer, 400);
                        m_pCheckNetworkLayer -> openTouchAndTime();
                    }
                }
            }
        }
    }
}

void CMainLogic::openCheckLink()
{
    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
    pScheduler->scheduleSelector(schedule_selector(CMainLogic::openCheckLinkTime), this, 1.0f, 0, 0, false);
}

void CMainLogic::openCheckLinkTime()
{
    if(m_pNetworkService != NULL)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CNetInfo * pNetInfo = NULL;
        if (!NATIVE_TEST) {
            pNetInfo = m_pNetworkService->Connect(m_sNodeIP.c_str(), PORT_LOGON, NULL, NULL, true);
        }
        else
        {
          pNetInfo = m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON, NULL, NULL, false);  
        }
        m_nHallSocketHandle = m_pNetworkService->GetNetID(pNetInfo);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		if (!NATIVE_TEST) {
            m_nHallSocketHandle = m_pNetworkService->Connect(m_sNodeIP.c_str(), PORT_LOGON);
        }
        else
        {
          m_nHallSocketHandle = m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON);
        }
#endif
    }
}

std::string CMainLogic::getCurrentUserIDTime(std::string sInfo)
{
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);
    char temp[32];
    sprintf(temp, "%s%d%d%02d%02d",sInfo.c_str(), m_nUserID, t_tm->tm_year, t_tm->tm_mon, t_tm->tm_mday);
    return temp;
}

void CMainLogic::openCheckGameConnect()
{
    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
    pScheduler->scheduleSelector(schedule_selector(CMainLogic::openCheckGameConnectTime), this, 2.0f, 0, 0, false);
}

void CMainLogic::openCheckGameConnectTime()
{
    if(m_bGameDisConnect)
    {
        SendData(m_nRoomSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
        CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
        pScheduler->scheduleSelector(schedule_selector(CMainLogic::openCheckReceiveGameMes), this, 2.0f, 0, 0, false);
    }
}

void CMainLogic::openCheckReceiveGameMes()
{
    //手动断开，回到大厅
    m_bGameDisConnect = false;
    ShowMessage("当前网络不稳定", eMBOK, eDisConnectBackToLogon);
}

void CMainLogic::addNoticeToVct(int nId, std::string sContent)
{
    CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
    if(pHallLayer != NULL)
    {
        if(pHallLayer->m_vctLabelNotice.size() <= NOTICE_COUNT)
        {
            NoticeVct noticeVct;
            noticeVct.bHave = true;
            noticeVct.nID = nId;
            noticeVct.sNotice = sContent;
            bool bSuccessInsert = false;
            for(int i = 0; i < (int)pHallLayer->m_vctStringNotice.size(); i++)
            {
                if(nId <= pHallLayer->m_vctStringNotice[i].nID)
                {
                    pHallLayer->m_vctStringNotice.insert(pHallLayer->m_vctStringNotice.begin()+i, noticeVct);
                    bSuccessInsert = true;
                    break;
                }
            }
            if(!bSuccessInsert)
            {
                pHallLayer->m_vctStringNotice.push_back(noticeVct);
            }
        }
    }
}

void CMainLogic::sendUserBehavior(int nUserID, int nActionTag)
{
    CCHttpRequest* request = new CCHttpRequest();
    char temp[256];
    char szToken[128];
    char szMD5Result[128];
    SCORE lScore = 0;
    if(GameMainScene::_instance != NULL)
    {
        lScore = GameMainScene::_instance->getUserCurrentScore();
    }
    else
    {
        lScore = m_lUserScore;
    }
    sprintf(szToken, "%d%d%llddakalog", nUserID, nActionTag,lScore);
    CEncryptMD5::toString32(szToken, szMD5Result);
    sprintf(temp, "lelelog.66y.com/OperateLog.aspx?userid=%d&code=%d&score=%lld&sign=%s", nUserID, nActionTag,lScore, szMD5Result);
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//判断Android手机是否联网
std::string CMainLogic::OnInternet()
{
    JniMethodInfo info;
    bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if(bHave)
    {
        jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
        
        bool bHave1 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","isConnectInterNet", "()Ljava/lang/String;");
        if(bHave1)
        {
            jstring jstr = (jstring)info.env->CallObjectMethod(jobj, info.methodID);
            std::string sNetStatus = JniHelper::jstring2string(jstr);//2为没有网络，0为wify，1为流量
            return sNetStatus;
        }
    }
    return "1";
}
#endif

void CMainLogic::sendUserTelInfo()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string sMachineID = ChargeWebView::getUIDevice();
    std::string sSystem = ChargeWebView::getDeviceInfo(2);
    std::string sMachineType = ChargeWebView::getDeviceInfo(5);
    std::string sSystemVersion = ChargeWebView::getDeviceInfo(0);
    std::string isOnInternet = ChargeWebView::OnInternet();
    CCHttpRequest* request = new CCHttpRequest();
    char temp[512];
    char szToken[512];
    char szMD5Result[128];
    sprintf(szToken, "%d%s%s1%s%s%s%s%sloginlog", m_nUserID, sMachineID.c_str(), sSystem.c_str(), sMachineType.c_str(), sSystemVersion.c_str(), isOnInternet.c_str(), m_sMachine.c_str(), VERSION_IOS);
    CEncryptMD5::toString32(szToken, szMD5Result);
    sprintf(temp, "lelelog.66y.com/MachineInfo.aspx?userid=%d&machineid=%s&os=%s&ai=1&machinetype=%s&version=%s&nettype=%s&channel=%s&clientver=%s&sign=%s", m_nUserID, sMachineID.c_str(), sSystem.c_str(), sMachineType.c_str(), sSystemVersion.c_str(), isOnInternet.c_str(),m_sMachine.c_str(),VERSION_IOS,szMD5Result);
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    CCHttpClient::getInstance()->send(request);
    request -> release();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string sOnInternet = OnInternet();
    JniMethodInfo info;
    bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if(bHave)
    {
        jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
        bool bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","getUIDevice1", "(I)Ljava/lang/String;");
        if(bHave2)
        {
            jstring jstr0 = (jstring)info.env->CallObjectMethod(jobj, info.methodID, 0);
            std::string sSystem = JniHelper::jstring2string(jstr0);	//硬件制造商
            jstring jstr1 = (jstring)info.env->CallObjectMethod(jobj, info.methodID, 1);
            std::string sMachineType = JniHelper::jstring2string(jstr1);//手机型号
            jstring jstr5 = (jstring)info.env->CallObjectMethod(jobj, info.methodID, 5);
            std::string sSystemVersion = JniHelper::jstring2string(jstr5);	//系统版本
            jstring jstr9 = (jstring)info.env->CallObjectMethod(jobj, info.methodID, 9);
            std::string sMachineID = JniHelper::jstring2string(jstr9);	//IMEI
            CCHttpRequest* request = new CCHttpRequest();
            char temp[512];
            char szToken[512];
            char szMD5Result[128];
            sprintf(szToken, "%d%s%s0%s%s%s%s%sloginlog", m_nUserID, sMachineID.c_str(), sSystem.c_str(), sMachineType.c_str(), sSystemVersion.c_str(), sOnInternet.c_str(), MACHINE, VERSION_ANDROID);
            CEncryptMD5::toString32(szToken, szMD5Result);
            sprintf(temp, "lelelog.66y.com/MachineInfo.aspx?userid=%d&machineid=%s&os=%s&ai=0&machinetype=%s&version=%s&nettype=%s&channel=%s&clientver=%s&sign=%s", m_nUserID, sMachineID.c_str(), sSystem.c_str(), sMachineType.c_str(), sSystemVersion.c_str(), sOnInternet.c_str(),MACHINE,VERSION_ANDROID,szMD5Result);
            request -> setUrl(temp);
            request -> setRequestType(CCHttpRequest::kHttpGet);
            CCHttpClient::getInstance()->send(request);
            request -> release();
        }
    }
    
#endif
    
}

void CMainLogic::sendSuperPirateGuideFinished()
{
    CMD_GPR_SuperPirateFinished SuperPirateGuideFinished;
    SuperPirateGuideFinished.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    SendData(m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_SUPERPIRATE_GUIDE_FINISHED, &SuperPirateGuideFinished,sizeof(SuperPirateGuideFinished));
}

void CMainLogic::sendShareSuccess()
{
    char temp[32];
    sprintf(temp, "%dbuyu2015",m_nUserID);
    char szMD5Result[128];
    CEncryptMD5::toString32(temp,szMD5Result);
    char urlStr[128];
    sprintf(urlStr, "http://%s/GiveTelFare.aspx?uid=%d&num=%s",NORMAL_URL, m_nUserID,szMD5Result);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(urlStr);
    request -> setRequestType(CCHttpRequest::kHttpPost);
    request -> setRequestData(szMD5Result, (int)strlen(szMD5Result));
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onHttpSharedComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
    ShowMessage("正在领取，请稍候", eMBExitGame);
}

void CMainLogic::onHttpSharedComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    int nResult = 0;
    std::string sResult = "";
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("xml"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while(pCurrentElement != NULL)
        {
            std::string value = pCurrentElement->Value();
            if(value == "text")
            {
                sResult = pCurrentElement->GetText();
            }
            else if(value == "Bool")
            {
                nResult = atoi(pCurrentElement->GetText());
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    document->Clear();
    delete document;
    if(nResult == 0)
    {
        CMainLogic::sharedMainLogic()->ShowMessage(sResult.c_str(), eMBOK);
        //删除框
        if(GameMainScene::_instance != NULL)
        {
            GameMainScene::_instance->removeMobileUI(10);
        }
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage(sResult.c_str(), eMBOK);
    }
}

void CMainLogic::sendNoticeVersion(std::string sPosition)
{
    char temp[128];
    sprintf(temp, "http://%s/GetGameNotice.aspx?flag=0&type=0", NORMAL_URL);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(temp);
    request -> setTag(sPosition.c_str());
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onHttpNoticeVersionCom));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CMainLogic::onHttpNoticeVersionCom(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        return;
    }
    if (!response->isSucceed())
    {
        //CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        return;
    }
    std::string sTag = response->getHttpRequest()->getTag();
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("noticelist"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL)
        {
            TiXmlElement* pFirstElement = pCurrentElement->FirstChildElement();
            if(!strcmp(pFirstElement->Value(), "hallNoticeVersion"))
            {
                std::string sVIPVersion = pFirstElement->GetText();
                if(sTag == "hallnotice")
                {
                    if(CCUserDefault::sharedUserDefault()->getStringForKey("NoticeVersion", "") == "")
                    {
                        CCUserDefault::sharedUserDefault()->setBoolForKey("OpenGonggao", false);
                        CCUserDefault::sharedUserDefault()->setStringForKey("NoticeNewVersion", sVIPVersion);
                        sendNoticeData(sTag);
                    }
                    else if(CCUserDefault::sharedUserDefault()->getStringForKey("NoticeVersion") != sVIPVersion)//有更新
                    {
                        CCUserDefault::sharedUserDefault()->setBoolForKey("OpenGonggao", false);
                        CCUserDefault::sharedUserDefault()->setStringForKey("NoticeNewVersion", sVIPVersion);
                        sendNoticeData(sTag);
                    }
                    else//本地已经保存
                    {
                        if(m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                //判断有没有打开过公告界面
                                if(!CCUserDefault::sharedUserDefault()->getBoolForKey("OpenGonggao", false))
                                {
                                    pHallLayer -> setGonggaoStatus(true);
                                }
                                if(pHallLayer->getOpenGonggao())
                                {
                                    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
                                    CNoticeOfHall* pNoticeOfHall = CNoticeOfHall::create(true, 1);
                                    pHallLayer->addChild(pNoticeOfHall, 300);
                                    pHallLayer -> setOpenGonggao(false);
                                    pHallLayer -> setGonggaoStatus(false);
                                }
                            }
                        }
                    }
                }
                else if(sTag == "chargenotice")
                {
                    if(CCUserDefault::sharedUserDefault()->getStringForKey("ChargeNoticeVersion", "") == "")
                    {
                        CCUserDefault::sharedUserDefault()->setStringForKey("ChargeNoticeNewVersion", sVIPVersion);
                        sendNoticeData(sTag);
                    }
                    else if(CCUserDefault::sharedUserDefault()->getStringForKey("ChargeNoticeVersion") != sVIPVersion)//有更新
                    {
                        CCUserDefault::sharedUserDefault()->setStringForKey("ChargeNoticeNewVersion", sVIPVersion);
                        sendNoticeData(sTag);
                    }
                    else//本地已经保存
                    {
                        CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
                        CCNode* pNode = (CCNode*)CCDirector::sharedDirector()->getRunningScene();
                        if(pNode != NULL)
                        {
                            ChargeLayer* pChargeLayer = (ChargeLayer*)pNode->getChildByTag(20000);
                            if(pChargeLayer != NULL && pChargeLayer -> getOpenGonggao())
                            {
                                CNoticeOfHall* pNoticeOfHall = CNoticeOfHall::create(true, 0);
                                pChargeLayer->addChild(pNoticeOfHall, 300);
                            }
                        }
                    }
                }
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
}

void CMainLogic::sendNoticeData(std::string sPosition)//type=0大厅    ＝1充值
{
    char temp[128];
    if(sPosition == "hallnotice")
    {
        sprintf(temp, "http://%s/GetGameNotice.aspx?flag=1&type=0", NORMAL_URL);
    }
    else
    {
        sprintf(temp, "http://%s/GetGameNotice.aspx?flag=1&type=1", NORMAL_URL);
    }
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(temp);
    request -> setTag(sPosition.c_str());
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onHttpNoticeDataCom));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CMainLogic::onHttpNoticeDataCom(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，本次请求失败了，请稍后再试一次。", eMBOK);
        return;
    }
    std::string sTag = response->getHttpRequest()->getTag();
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    if(sTag == "hallnotice")
    {
        m_pMessageBox->Update();
        std::string sNoticeNewVersion = CCUserDefault::sharedUserDefault()->getStringForKey("NoticeNewVersion");
        CCUserDefault::sharedUserDefault()->setStringForKey("NoticeVersion", sNoticeNewVersion);
        saveFile(str, "hall");
        if(m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                pHallLayer -> setGonggaoStatus(true);
                if(pHallLayer->getOpenGonggao())
                {
                    CNoticeOfHall* pNoticeOfHall = CNoticeOfHall::create(false, 1);
                    pHallLayer->addChild(pNoticeOfHall, 300);
                    pNoticeOfHall->parseData(str);
                    pHallLayer -> setOpenGonggao(false);
                    pHallLayer -> setGonggaoStatus(false);
                }
            }
        }
    }
    else if(sTag == "chargenotice")
    {
        m_pMessageBox->Update();
        std::string sNoticeNewVersion = CCUserDefault::sharedUserDefault()->getStringForKey("ChargeNoticeNewVersion");
        CCUserDefault::sharedUserDefault()->setStringForKey("ChargeNoticeVersion", sNoticeNewVersion);
        saveFile(str, "charge");
        CCNode* pNode = (CCNode*)CCDirector::sharedDirector()->getRunningScene();
        if(pNode != NULL)
        {
            ChargeLayer* pChargeLayer = (ChargeLayer*)pNode->getChildByTag(20000);
            if(pChargeLayer != NULL && pChargeLayer -> getOpenGonggao())
            {
                CNoticeOfHall* pNoticeOfHall = CNoticeOfHall::create(false, 0);
                pChargeLayer->addChild(pNoticeOfHall, 300);
                pNoticeOfHall->parseData(str);
            }
        }
    }
}

void CMainLogic::saveFile(std::string str, std::string sPosition)
{
    //在Documents目录下创建一个可读写的文件
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string path2 = sPosition+"HallNoticeData.xml";
    std::string xmlPath = path + path2;
    const char* buf = str.c_str();
    FILE* file = fopen(xmlPath.c_str(), "w");
    if(file)
    {
        fputs(buf, file);
        fclose(file);
        CCUserDefault::sharedUserDefault()->setBoolForKey("NoticeDataSuccess", true);
    }
    else
    {
        
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void CMainLogic::requestIOSChannel()
{
    std::string sMAC = ChargeWebView::getIOSMAC();
    std::string sIDFA = ChargeWebView::getIOSIDFA();
    std::string sUUID = ChargeWebView::getIOSUUID();
    CCHttpRequest* request = new CCHttpRequest();
    char szMD5Result[256];
    char userID[256];
    sprintf(userID, "openudid=%s&mac=%s&idfa=%s&time=%lubuyudaka2016", sUUID.c_str(), sMAC.c_str(), sIDFA.c_str(), time(0));
    CEncryptMD5::toString32(userID,szMD5Result);
    char temp[256];
    sprintf(temp, "http://%s/GetSoure.aspx?openudid=%s&mac=%s&idfa=%s&time=%lu&info=%s", NORMAL_URL, sUUID.c_str(), sMAC.c_str(), sIDFA.c_str(), time(0), szMD5Result);
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    //request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onHttpChannelComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();

}

void CMainLogic::onHttpChannelComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    CCUserDefault::sharedUserDefault()->setBoolForKey("FirstRequestChannel", false);
    m_sMachine = str;
    CCUserDefault::sharedUserDefault()->setStringForKey("MACHINE", str);
}
#endif

//首充
/*
参数说明：
    第一个参数：充值金额
    第二个参数：0表示普通充值，1表示10元限购，3表示1元购
    第三个参数：0表示普通充值，1表示VIP升级
 */
void CMainLogic::alipayCharge(int prise, int nFirstCharge, bool bVIPUpgrade)
{
    ShowMessage("正在转向支付界面，请稍候", eMBNull);
    m_nPrise = prise;
    char tempOrder[32];
    sprintf(tempOrder, "alipay%ld%d", time(0), m_nUserID);
    char temp[128];
    sprintf(temp, "uid=%d&tfee=%d&otn=%s&time=%ld&firstCharge=%d&vup=%d",CMainLogic::sharedMainLogic()->m_nUserID,prise,tempOrder,time(0),nFirstCharge, bVIPUpgrade);
    char szMD5Result[128];
    CEncryptMD5::toString32(temp,szMD5Result);
    char tempData[256];
    sprintf(tempData, "%s&info=%s&ctype=%d", temp, szMD5Result, 1);
    printf("alipaycharge temp is %s\n", tempData);
    m_sOrderID = tempOrder;
    CCHttpRequest* request = new CCHttpRequest();
    char chargeURL[64];
    if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
    {
        sprintf(chargeURL, "http://lelepay.66y.com/alipay20151028/alipayOrder.aspx");
    }
    else
    {
        sprintf(chargeURL, "%s/alipay20151028/alipayOrder.aspx", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str());
    }
    request -> setUrl(chargeURL);
    request -> setRequestType(CCHttpRequest::kHttpPost);
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onHttpAlipayComplete));
    request -> setRequestData(tempData, (int)strlen(tempData));
    CCHttpClient::getInstance()->send(request);
    CC_SAFE_RELEASE(request);
    CMainLogic::sharedMainLogic()->m_bChargeState = true;
    request = NULL;
}

void CMainLogic::onHttpAlipayComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->HttpErrorHandle(response, eAlipayOrderErr);
        return;
    }
    //开启定时器，向服务端发送消息，请求是否充值完成
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    
    if(!strcmp(str.c_str(), "success"))
    {
        CMainLogic::sharedMainLogic()->m_bChargeState = true;
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeOrderSuccess);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChargeSuccess = 5;//支付宝充值
        CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
        ChargeWebView::alipayCharge(m_sOrderID.c_str(), m_nPrise, 1);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        AndroidAlipay(m_nPrise);
#endif
    }
    else
    {
        CMainLogic::sharedMainLogic()->m_bChargeState = false;
        CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK, eChargeBackHall);
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeOrderFail);
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void CMainLogic::AndroidAlipay(int prise)
{
    JniMethodInfo info;
    bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if(bHave)
    {
        jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
        bool bHave2 = false;
        bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","pay", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
        if(bHave2)
        {
            char subjectTemp[64];
            sprintf(subjectTemp, "金币");
            char bodyTemp[128];
            sprintf(bodyTemp, "商品详细描述");
            char priseTemp[16];
            sprintf(priseTemp, "%d", prise);
            jstring subject = info.env->NewStringUTF(subjectTemp);
            jstring body = info.env->NewStringUTF(bodyTemp);
            jstring prise = info.env->NewStringUTF(priseTemp);
            jstring orderID = info.env->NewStringUTF(m_sOrderID.c_str());
            jint type = 1;
            info.env->CallVoidMethod(jobj, info.methodID, subject, body, prise, orderID, type);
        }
    }
}

void CMainLogic::showDelayAlipayResult()
{
    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
    pScheduler->scheduleSelector(schedule_selector(CMainLogic::delayAlipayChargeSuccess), this, 0.1f, 0, 0, false);
}

void CMainLogic::showDelayWechatResult()
{
    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
    pScheduler->scheduleSelector(schedule_selector(CMainLogic::delayWXChargeSuccess), this, 0.1f, 0, 0, false);
}

void CMainLogic::delayWXChargeSuccess(float dt)
{
    sendWXChargeSuccess();
}

void CMainLogic::delayAlipayChargeSuccess(float dt)
{
    sendAlipayChargeSuccess();
}

void CMainLogic::AndroidYeepay(int prise, int nVIPUpgrade)
{
    unsigned int userID = CMainLogic::sharedMainLogic()->m_nUserID;
    const char* account = CMainLogic::sharedMainLogic()->m_sAccount.c_str();
    char tempURL[256];
    char temp3[128];
    sprintf(temp3, "uid=%d&acc=%s&mon=%d&time=%lu&firstCharge=%d&vup=%d", userID, account, prise, time(0), 0, nVIPUpgrade);
    char szMD5Result[128];
    CEncryptMD5::toString32(temp3,szMD5Result);
    if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
    {
        sprintf(tempURL, "http://lelepay.66y.com/Mp_Pay/yeepay/OneClickPayment.aspx?%s&md=%s", temp3, szMD5Result);
    }
    else
    {
        sprintf(tempURL, "%s/Mp_Pay/yeepay/OneClickPayment.aspx?%s&md=%s", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str(), temp3, szMD5Result);
    }
    
    JniMethodInfo minfo;
    jobject jobj;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "cn/sharesdk/hellocpp/HelloCpp","getInstance","()Ljava/lang/Object;");
    if (isHave)
    {
        //调用Java静态函数，取得对象。
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        if (jobj != NULL)
        {
            isHave = JniHelper::getMethodInfo(minfo,"cn/sharesdk/hellocpp/HelloCpp","openWebView","(Ljava/lang/String;)V");
            if (isHave)
            {
                jstring urlStr = minfo.env->NewStringUTF(tempURL);
                //调用java非静态函数, 参数1：Java对象，上面已经取得   参数2：方法ID
                minfo.env->CallVoidMethod(jobj, minfo.methodID, urlStr);  
            }  
        }  
    }
}

#endif

//同步通知IOS支付宝充值完成或者取消
void CMainLogic::notifyIOSAlipayCompleted()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(ChargeSuccess == 1)//苹果支付宝完成
    {
        m_sAlipaySign = AlipayChargeSuccess;
        CMainLogic::sharedMainLogic()->m_sAlipaySign = m_sAlipaySign;
        sendAlipayChargeSuccess();
    }
    else if(ChargeSuccess == 2)//苹果支付宝充值取消或者失败
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeCancel);
    }
#endif
}

//向网站发送支付宝充值成功
void CMainLogic::sendAlipayChargeSuccess()
{
    //向网站发送支付宝签名信息
    CCHttpRequest* request = new CCHttpRequest();
    std::string sAlipay = "";
    for(int i = 0; i < CMainLogic::sharedMainLogic()->m_sAlipaySign.length(); i++)
    {
        if(!(CMainLogic::sharedMainLogic()->m_sAlipaySign[i] == '\\'))// || AlipayChargeSuccess[i] == '"'
        {
            if(CMainLogic::sharedMainLogic()->m_sAlipaySign[i] == '+')
            {
                sAlipay+="%2B";
            }
            else
            {
                sAlipay+=CMainLogic::sharedMainLogic()->m_sAlipaySign[i];
            }
        }
    }
    char chargeURL[1024];
    if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
    {
        sprintf(chargeURL, "http://lelepay.66y.com/alipay20151028/alipayReturn.aspx?%s", sAlipay.c_str());
    }
    else
    {
        sprintf(chargeURL, "%s/alipay20151028/alipayReturn.aspx?%s", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str(), sAlipay.c_str());
    }
    
    request -> setUrl(chargeURL);
    request -> setRequestType(CCHttpRequest::kHttpPost);
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onAlipayChargeSuccess));
    CCHttpClient::getInstance()->send(request);
    CC_SAFE_RELEASE(request);
    request = NULL;
    CMainLogic::sharedMainLogic()->ShowMessage("正在验证充值结果", eMBNull);
    //这里可以将充值界面删除
    ChargeUI* pChargeUI = (ChargeUI*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20003);
    if(pChargeUI != NULL)
    {
        pChargeUI -> removeFromParent();
    }
}

//网站返回支付宝充值成功
void CMainLogic::onAlipayChargeSuccess(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->HttpErrorHandle(response, eAlipaySuccessErr);
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    
    //printf("alipay111111 %s\n", str.c_str());
    
    requestMail();
    
    bool bVIPUpgrade = false;
    std::string sInfo = "";
    bool bChargeResult = false;
    
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement != NULL)
        {
            TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
            while (pCurrentElement != NULL)
            {
                std::string nodename=pCurrentElement->Value();
                if (nodename == "text")
                {
                    sInfo = pCurrentElement->GetText();
                }
                else if(nodename == "Score")
                {
                    if(CMainLogic::sharedMainLogic()->m_lUserScore != atoll(pCurrentElement->GetText()))
                    {
                        bChargeResult = true;
                    }
                    //更新当前分数
                    CMainLogic::sharedMainLogic()->m_lUserScore = atoll(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setUserScore(CMainLogic::sharedMainLogic()->m_lUserScore);
                        }
                    }
                }
                else if(nodename == "VipLevel")//更新当前VIP等级
                {
                    int nLevel = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_nVipLevel != nLevel)
                    {
                        if(CMainLogic::sharedMainLogic()->m_nVipLevel == 0)//以前VIP等级为0的话，那么现在肯定升级了VIP，所以可以VIP签到
                        {
                            CMainLogic::sharedMainLogic()->m_bReceiveVipLogon = false;
                            //将大厅内的免费金币按钮上的心形展示出来
                            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                            {
                                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                                if(pHallLayer != NULL)
                                {
                                    pHallLayer -> setNickNameColor();
                                    pHallLayer->showfreecoinAction(true);
                                    if(nLevel == 9)
                                    {
                                        pHallLayer->m_pVipMenu->setVisible(false);
                                    }
                                }
                            }
                        }
                        CMainLogic::sharedMainLogic()->m_nVipLevel = nLevel;
                        bVIPUpgrade = true;
                    }
                    CMainLogic::sharedMainLogic()->m_nVipLevel = nLevel;
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setVipExp();
                        }
                    }
                }
                else if(nodename == "VipExp")//更新当前经验值
                {
                    CMainLogic::sharedMainLogic()->m_nVipExp = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setVipExp();
                        }
                    }
                }
                else if(nodename == "UserLet")//更新当前奖券
                {
                    CMainLogic::sharedMainLogic()->m_nLottery = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setTicket(CMainLogic::sharedMainLogic()->m_nLottery);
                        }
                    }
                }
                else if(nodename == "ChargeBack")//更新充值返利
                {
                    CMainLogic::sharedMainLogic()->m_nChargeBack = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_nChargeBack!=0)
                    {
                        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                pHallLayer->showVIPAction(true);
                            }
                        }
                    }
                }
                else if(nodename == "Fch")
                {
                    int nFirstCharge = atoi(pCurrentElement->GetText());
                    if(nFirstCharge == 1)//首充
                    {
                        CMainLogic::sharedMainLogic()->ShowMessage("恭喜您首充成功，获取了首充大礼包", eMBOK);
                        CMainLogic::sharedMainLogic()->m_cbCountTimes = 1;
                        CMainLogic::sharedMainLogic()->m_bFirstChargeCanGet = true;
                        //改变首充状态
                        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                pHallLayer->showLastTimes(1);
                            }
                        }
                    }
                    else if(nFirstCharge == 2)//1元购
                    {
                        CMainLogic::sharedMainLogic()->m_bCanOneCharge = false;
                        //改变1元购状态
                        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                pHallLayer->hideOneChargeMenu();
                            }
                        }
                    }
                }
                else if(nodename == "Count")//更新当前奖券数量
                {
                    CMainLogic::sharedMainLogic()->m_nLottery = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setTicket(CMainLogic::sharedMainLogic()->m_nLottery);
                        }
                    }
                }
                pCurrentElement = pCurrentElement->NextSiblingElement();
            }
        }
    }
    delete document;
    
    if(bChargeResult)
    {
        if(!bVIPUpgrade)
        {
            if(CMainLogic::sharedMainLogic()->m_bCanBandingTel)
            {
                if(CCUserDefault::sharedUserDefault()->getStringForKey("BindingTel", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"))
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("BindingTel", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"));
                    CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK, eShowBindingTel);
                }
                else
                {
                    CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK);
                }
            }
            else
            {
                CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK);
            }
        }
        else
        {
            CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK, eShowVipUpgrade);
        }
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeSuccess);
        //播放特效，判断登录奖励是否领取，如果已经领取了，那么2s后就将此框删除
        SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
            }
        }
    }
    else
    {
        CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeFail);
    }
}

/*
参数说明：
    第一个参数：充值金额
    第二个参数：充值类型：0表示普通充值，1表示首充，3表示1元购（取值范围：0，1，3）
    第三个参数：是否是升级VIP充值（只有两个值（0或者1））
 */
void CMainLogic::wxCharge(int prise, int nFirstCharge, bool bVIPUpgrade)//@@金币充值
{
    ShowMessage("正在转向支付界面，请稍候", eMBNull);
    char szMD5Result[128];
    char chargeURL[256];
    char temp1[128];
    char tempPrise[16];
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
    sprintf(temp1, "%d%d%ld%d%d%s%ddakasdk", CMainLogic::sharedMainLogic()->m_nUserID, prise, time(0), bFirstCharge, bVIPUpgrade, CHANNEL_NUM, nType);
    CEncryptMD5::toString32(temp1,szMD5Result);
    sprintf(chargeURL, "http://dakasdkpay.4699.com/AnySdk/OrderAll.aspx?uid=%d&tfee=%d&time=%ld&firstCharge=%d&vup=%d&channelid=%s&ctype=%d&sign=%s", CMainLogic::sharedMainLogic()->m_nUserID, prise, time(0), nFirstCharge, bVIPUpgrade, CHANNEL_NUM, nType, szMD5Result);
#else
    const char* payName = CMainLogic::sharedMainLogic()->m_sAccount.c_str();
    sprintf(tempPrise, "%d", prise*100);
    sprintf(temp1, "payUid=%d&payName=%s&payMoney=%s&time=%ld&firstCharge=%d&vup=%d", CMainLogic::sharedMainLogic()->m_nUserID, payName, tempPrise, time(0), nFirstCharge, bVIPUpgrade);
    CEncryptMD5::toString32(temp1,szMD5Result);
    if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
    {
        sprintf(chargeURL, "http://lelepay.66y.com/Mp_Pay/wxlele2/WXPayOrder.aspx?%s&info=%s&ctype=%d", temp1, szMD5Result, 1);
    }
    else
    {
        sprintf(chargeURL, "%s/Mp_Pay/wxlele2/WXPayOrder.aspx?%s&info=%s&ctype=%d", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str(), temp1, szMD5Result, 1);
    }
#endif
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(chargeURL);
    printf("wecharge temp is %s\n", chargeURL);
    request -> setRequestType(CCHttpRequest::kHttpPost);
    request -> setTag("http test");
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onHttpAnySDKPayComplete));
#else
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onHttpShared2Complete));
#endif
    CCHttpClient::getInstance()->send(request);
    CC_SAFE_RELEASE(request);
    request = NULL;
}

void CMainLogic::onHttpShared2Complete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->HttpErrorHandle(response, eWXOrderErr);
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    
    bool bSuccess = false;
    //调起微信支付
    std::string appid = "";
    std::string partnerId = "";
    std::string prepayId = "";
    std::string nonceStr = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    int timeStamp = 0;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string timeStamp = "";
#endif
    std::string package = "";
    std::string sign = "";
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if (NULL!=rootElement)
        {
            std::string nodename=rootElement->Value();
            if (0!=nodename.compare("xml"))
            {
                return;
            }
            
            TiXmlElement* firstElement = rootElement->FirstChildElement();
            while (NULL != firstElement) {
                if(!strcmp(firstElement->Value(), "appid"))
                {
                    appid = firstElement->GetText();
                }
                if(!strcmp(firstElement->Value(), "partnerId"))
                {
                    partnerId = firstElement->GetText();
                }
                if(!strcmp(firstElement->Value(), "prepayId"))
                {
                    prepayId = firstElement->GetText();
                }
                if(!strcmp(firstElement->Value(), "nonceStr"))
                {
                    nonceStr = firstElement->GetText();
                }
                if(!strcmp(firstElement->Value(), "timeStamp"))
                {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    timeStamp = atoi(firstElement->GetText());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    timeStamp = firstElement->GetText();
#endif
                }
                if(!strcmp(firstElement->Value(), "package"))
                {
                    package = firstElement->GetText();
                }
                if(!strcmp(firstElement->Value(), "sign"))
                {
                    sign = firstElement->GetText();
                }
                else if(!strcmp(firstElement->Value(), "out_trade_no"))
                {
                    bSuccess = true;
                    m_sWXOrderID = firstElement->GetText();
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeOrderSuccess);
                }
                firstElement = firstElement -> NextSiblingElement();
            }
        }
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK);
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeOrderFail);
    }
    delete document;
    
    if(!bSuccess)
    {
        CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK);
        return;
    }
    CMainLogic::sharedMainLogic()->m_bChargeState = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    WeiXinCharge = 1;
    ChargeSuccess = 5;
    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
    ChargeWebView::wxCharge(appid, partnerId, prepayId, nonceStr, timeStamp, package, sign);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo info;
    bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if(bHave)
    {
        jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
        bool bHave2 = false;
        bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","wxCharge", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
        if(bHave2)
        {
            jstring appidjStr = info.env->NewStringUTF(appid.c_str());
            jstring partnerIdjStr = info.env->NewStringUTF(partnerId.c_str());
            jstring prepayIdjStr = info.env->NewStringUTF(prepayId.c_str());
            jstring pakcagejStr = info.env->NewStringUTF(package.c_str());
            jstring nonceStrjStr = info.env->NewStringUTF(nonceStr.c_str());
            jstring timeStampjStr = info.env->NewStringUTF(timeStamp.c_str());
            jstring signjStr = info.env->NewStringUTF(sign.c_str());
            jint wxCharge = 1;
            info.env->CallVoidMethod(jobj, info.methodID, appidjStr, partnerIdjStr, prepayIdjStr, pakcagejStr, nonceStrjStr, timeStampjStr, signjStr, wxCharge);
            nWxChargeResult = 1;
        }
    }
    
#endif
}

//同步通知IOS微信支付完成或者取消
void CMainLogic::notifyIOSWXCompleted()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
    if(ChargeSuccess == 3)//苹果微信支付成功
    {
        sendWXChargeSuccess();
    }
    else if(ChargeSuccess == 4)//苹果微信充值取消或者失败
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeCancel);
    }
#endif
}

//向网站发送微信充值成功
void CMainLogic::sendWXChargeSuccess()
{
    CCHttpRequest* request = new CCHttpRequest();
    char chargeURL[128];
    if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
    {
        sprintf(chargeURL, "http://lelepay.66y.com/Mp_Pay/wxlele2/WXPayReturn.aspx?otn=%s",CMainLogic::sharedMainLogic()->m_sWXOrderID.c_str());
    }
    else
    {
        sprintf(chargeURL, "%s/Mp_Pay/wxlele2/WXPayReturn.aspx?otn=%s", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str(), CMainLogic::sharedMainLogic()->m_sWXOrderID.c_str());
    }
    request -> setUrl(chargeURL);
    request -> setRequestType(CCHttpRequest::kHttpPost);
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onWXChargeSuccess));
    request -> setTag("WXSuccess");
    CCHttpClient::getInstance()->send(request);
    CC_SAFE_RELEASE(request);
    request = NULL;
    CMainLogic::sharedMainLogic()->ShowMessage("正在验证充值结果", eMBNull);
    //这里可以将充值界面删除
    ChargeUI* pChargeUI = (ChargeUI*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20003);
    if(pChargeUI != NULL)
    {
        pChargeUI -> removeFromParent();
    }
}

//网站返回微信充值成功
void CMainLogic::onWXChargeSuccess(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->HttpErrorHandle(response, eWXChargeSuccessErr);
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    
    requestMail();
    
    bool bVIPUpgrade = false;
    std::string sInfo = "";
    bool bChargeResult = false;
    
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement != NULL)
        {
            TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
            while (pCurrentElement != NULL)
            {
                std::string nodename=pCurrentElement->Value();
                if (nodename == "text")
                {
                    sInfo = pCurrentElement->GetText();
                }
                else if(nodename == "Score")
                {
                    if(CMainLogic::sharedMainLogic()->m_lUserScore != atoll(pCurrentElement->GetText()))
                    {
                        bChargeResult = true;
                    }
                    //更新当前分数
                    CMainLogic::sharedMainLogic()->m_lUserScore = atoll(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setUserScore(CMainLogic::sharedMainLogic()->m_lUserScore);
                        }
                    }
                }
                else if(nodename == "VipLevel")//更新当前VIP等级
                {
                    int nLevel = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_nVipLevel != nLevel)
                    {
                        if(CMainLogic::sharedMainLogic()->m_nVipLevel == 0)//以前VIP等级为0的话，那么现在肯定升级了VIP，所以可以VIP签到
                        {
                            CMainLogic::sharedMainLogic()->m_bReceiveVipLogon = false;
                            //将大厅内的免费金币按钮上的心形展示出来
                            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                            {
                                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                                if(pHallLayer != NULL)
                                {
                                    pHallLayer -> setNickNameColor();
                                    pHallLayer->showfreecoinAction(true);
                                    if(nLevel == 9)
                                    {
                                        pHallLayer->m_pVipMenu->setVisible(false);
                                    }
                                }
                            }
                        }
                        CMainLogic::sharedMainLogic()->m_nVipLevel = nLevel;
                        bVIPUpgrade = true;
                    }
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setVipExp();
                        }
                    }
                }
                else if(nodename == "VipExp")//更新当前经验值
                {
                    CMainLogic::sharedMainLogic()->m_nVipExp = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setVipExp();
                        }
                    }
                }
                else if(nodename == "UserLet")//更新当前奖券
                {
                    CMainLogic::sharedMainLogic()->m_nLottery = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setTicket(CMainLogic::sharedMainLogic()->m_nLottery);
                        }
                    }
                }
                else if(nodename == "ChargeBack")//更新充值返利
                {
                    CMainLogic::sharedMainLogic()->m_nChargeBack = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_nChargeBack!=0)
                    {
                        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                pHallLayer->showVIPAction(true);
                            }
                        }
                    }
                }
                else if(nodename == "Fch")
                {
                    int nFirstCharge = atoi(pCurrentElement->GetText());
                    if(nFirstCharge == 1)//首充
                    {
                        CMainLogic::sharedMainLogic()->ShowMessage("恭喜您首充成功，获取了首充大礼包", eMBOK);
                        CMainLogic::sharedMainLogic()->m_cbCountTimes = 1;
                        CMainLogic::sharedMainLogic()->m_bFirstChargeCanGet = true;
                        //改变首充状态
                        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                pHallLayer->showLastTimes(1);
                            }
                        }
                    }
                    else if(nFirstCharge == 2)//1元购
                    {
                        CMainLogic::sharedMainLogic()->m_bCanOneCharge = false;
                        //改变1元购状态
                        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                pHallLayer->hideOneChargeMenu();
                            }
                        }
                    }
                }
                else if(nodename == "Count")//更新当前奖券数量
                {
                    CMainLogic::sharedMainLogic()->m_nLottery = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setTicket(CMainLogic::sharedMainLogic()->m_nLottery);
                        }
                    }
                }
                pCurrentElement = pCurrentElement->NextSiblingElement();
            }
        }
    }
    delete document;
    
    if(bChargeResult)
    {
        if(GameMainScene::_instance!=NULL)
        {
            CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK, eExitGameToHall);
        }
        else
        {
            if(!bVIPUpgrade)
            {
                if(CMainLogic::sharedMainLogic()->m_bCanBandingTel)
                {
                    if(CCUserDefault::sharedUserDefault()->getStringForKey("BindingTel", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"))
                    {
                        CCUserDefault::sharedUserDefault()->setStringForKey("BindingTel", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"));
                        CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK, eShowBindingTel);
                    }
                    else
                    {
                        CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK);
                    }
                }
                else
                {
                    CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK);
                }
            }
            else
            {
                CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK, eShowVipUpgrade);
            }
            //播放特效，判断登录奖励是否领取，如果已经领取了，那么2s后就将此框删除
            SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                }
            }
        }
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeSuccess);
    }
    else
    {
        CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeFail);
    }
}

void CMainLogic::shareResult(bool bResult)
{
    if(bResult)//分享成功
    {
        if(m_nShareInterface == 1)//大厅免费金币
        {
            if(m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    FreeCoinLayer* pFreeCoinLayer = (FreeCoinLayer*)pHallLayer->getChildByTag(19996);
                    if(pFreeCoinLayer != NULL)
                    {
                        pFreeCoinLayer -> setDelayTimeToSendShareSuccess();
                    }
                }
            }
        }
        else if(m_nShareInterface == 2)//比赛界面领取奖励
        {
            CRankNewLayer* pRankNewLayer = (CRankNewLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10200);
            if(pRankNewLayer != NULL)
            {
                pRankNewLayer->requestShareEndSuccessMatchAward();
            }
        }
        else if(m_nShareInterface == 3)//比赛结束时，冠军领取奖励
        {
            if(GameMainScene::_instance != NULL)
            {
                CSendPrizeLayer* pSendPrizeLayer = (CSendPrizeLayer*)GameMainScene::_instance->getChildByTag(eCmptSendPrizeTag);
                if(pSendPrizeLayer != NULL)
                {
                    pSendPrizeLayer->sendSharedCompleted();
                }
            }
        }
        else if(m_nShareInterface == 4)
        {
            if(GameMainScene::_instance != NULL)
            {
                if(GameMainScene::_instance -> getFirstMobileMoney())
                {
                    GameMainScene::_instance->scheduleOnce(schedule_selector(GameMainScene::showShareSuccess), 0.0f);
                }
                else
                {
                    CMainLogic::sharedMainLogic()->sendShareSuccess();
                }
            }
        }
        else if(m_nShareInterface == 5)
        {
            //好友推广分享成功
        }
        sendUserBehavior(m_nUserID, eUserShareSuccess);
    }
    else//分享失败
    {
        sendUserBehavior(m_nUserID, eUserShareFail);
        printf("分享失败\n");
    }
}

void CMainLogic::requestAdvertXML()
{
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl("http://cdn.66y.com/leleclient/LeleAdvert.xml");
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onHttpAdvertComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CMainLogic::onHttpAdvertComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    m_vctAdvertConfig.clear();
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("advert"))
        {
            return;
        }
        TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
        while (pCurrentElement != NULL)
        {
            const char * pValue = pCurrentElement->Value();
            if(!std::strcmp(pValue, "item"))//相同
            {
                Tag_AdvertConfig AdvertConfig = {};
                std::string sShow = pCurrentElement->Attribute("show");
                AdvertConfig.bSHow = atoi(sShow.c_str());
                std::string sVersion = pCurrentElement->Attribute("version");
                AdvertConfig.sVersion = sVersion;
                std::string sName = pCurrentElement->Attribute("name");
                AdvertConfig.sName = sName;
                std::string sUrl = pCurrentElement->Attribute("downloadUrl");
                AdvertConfig.sDownload = sUrl;
                m_vctAdvertConfig.push_back(AdvertConfig);
            }
            pCurrentElement = pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
    
    //开始对比广告的版本号
    if(m_vctAdvertConfig.size() != 1)//目前只支持同时显示一个广告框（当然可以框可以换）
        return ;
    if(!m_vctAdvertConfig[0].bSHow)//广告框不显示
        return ;
    std::string sAdvertVersion = CCUserDefault::sharedUserDefault()->getStringForKey("AdvertVersion", "");
    if(sAdvertVersion == "" || sAdvertVersion != m_vctAdvertConfig[0].sVersion)//说明有图片要下载
    {
        requestAdvertImageUrl(m_vctAdvertConfig[0].sDownload, m_vctAdvertConfig[0].sName);
    }
    else
    {
        m_bShowAdvert = true;
    }
}

void CMainLogic::requestAdvertImageUrl(std::string sDownload, std::string sName)
{
    if ("" == sDownload || "" == sName)
    {
        return;
    }
    //加载网络图片
    CCHttpClient* httpClient = CCHttpClient::getInstance();
    CCHttpRequest* httpReq =new CCHttpRequest();
    httpReq->setRequestType(CCHttpRequest::kHttpGet);
    httpReq->setUrl(sDownload.c_str());
    httpReq->setTag(sName.c_str());
    httpReq->setResponseCallback(this,httpresponse_selector(CMainLogic::onHttpAdvertImageUrlComplete));
    httpClient->setTimeoutForConnect(30);
    httpClient->send(httpReq);
    httpReq->release();
    httpReq=NULL;
}

void CMainLogic::onHttpAdvertImageUrlComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response->isSucceed())
    {
        return ;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string bufffff(buffer->begin(),buffer->end());
    std::string imgName = response->getHttpRequest()->getTag();
    
    //保存到本地文件
    path += imgName;
    
    FILE * fp = fopen(path.c_str(), "wb+");
    fwrite(bufffff.c_str(), 1, buffer->size(), fp);
    fclose(fp);
    
    CCSprite * pSpPic = CCSprite::create(path.c_str());
    if (NULL == pSpPic)//再次下载的还是失败的话，再次下载
    {
        for (int i=0; i<m_vctAdvertConfig.size(); i++)
        {
            if (imgName == m_vctAdvertConfig[i].sName)
            {
                requestAdvertImageUrl(m_vctAdvertConfig[i].sDownload, m_vctAdvertConfig[i].sName);
            }
        }
    }
    else//下载成功则m_nDamageCount个数减去1
    {
        printf("下载完成!!!");
        m_bShowAdvert = true;
        CCUserDefault::sharedUserDefault()->setStringForKey("AdvertVersion", m_vctAdvertConfig[0].sVersion);
    }
}

void CMainLogic::chargeTest(int prise, int nChargeType)
{
    CCHttpRequest* request = new CCHttpRequest();
    char temp[128];
    sprintf(temp, "uid=%d&tfee=%d&time=%lu&firstCharge=%d&vup=%d", CMainLogic::sharedMainLogic()->m_nUserID, prise, time(0), nChargeType, 0);
    char szMD5Result[128];
    CEncryptMD5::toString32(temp,szMD5Result);
    char chargeURL[256];
    sprintf(chargeURL, "http://lele2testp.66y.com/Mp_Pay/viptest.aspx?%s&info=%s", temp, szMD5Result);
    request -> setUrl(chargeURL);
    printf("chargeUrl is %s\n", chargeURL);
    request -> setRequestType(CCHttpRequest::kHttpPost);
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onWXChargeSuccess));
    CCHttpClient::getInstance()->send(request);
    CC_SAFE_RELEASE(request);
    request = NULL;
    CMainLogic::sharedMainLogic()->ShowMessage("正在验证充值结果", eMBNull);
}
