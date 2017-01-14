#ifndef _MAIN_LOGIC_
#define _MAIN_LOGIC_

//class CLogonLayer;
//class CHallLayer;

enum eSwitchLayer
{
    eLogonLayer=1,
    eHallLayer,
    eTableLayer,
};

enum eLogonType
{
    eLogon=100,
    eRegister,
    eGuestRegister,
};

enum eResponseError
{
    eWXOrderErr=200,
    eAlipayOrderErr,
    eWXChargeSuccessErr,
    eAlipaySuccessErr,
};

struct Trumpet
{
    std::string noticeStr;
    std::string sNickName;
    CCNode* noticeNode;
    bool bContainIcon;
};

struct Tag_Notification
{
    bool bDelay;
    std::string sTime;
    std::string sNotice;
	int nHour;
	int nMinute;
	int nSecond;
};

struct Tag_VIPExpConfig
{
    int nLevel;
    int nChargeCount;
};

struct Tag_VIPInfoConfig
{
    int nLevel;//VIP等级
    int nReward;//领取奖励
    float fChargeBackPercent;//充值返点
    float fLotteryPercent;//奖券掉落概率
};

struct Tag_AdvertConfig
{
    bool bSHow;
    std::string sName;
    std::string sVersion;
    std::string sDownload;
};

enum eResponseStatus
{
    eUnUse=0,
    eRequesting,
    eRequestSuccess,
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
class CMainLogic : public CCNode, public CNetworkServiceSink
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
class CMainLogic : public CCNode, public CaratSink
#endif
{
public:
	CMainLogic();
	~CMainLogic();

	bool init();
    
    void Tick(unsigned int nCurrentTick);

	static CMainLogic * sharedMainLogic();

	//网络通信
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	virtual void OnEventTCPSocketLink(CNetInfo * pNetInfo, int nError, const char * sErrMsg = NULL);
	virtual void OnEventTCPSocketShut(CNetInfo * pNetInfo, int nError);
	virtual void OnEventTCPSocketRead(CNetInfo * pNetInfo, const void * pData, int nSize);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	virtual void OnEventTCPSocketLink(int nID, int nError);
    virtual void OnEventTCPSocketShut(int nID, int nError);
    virtual void OnEventTCPSocketRead(int nID, const void * pData, int nSize);
#endif   
    
    void showCurrentDisNetWork();

	void OnLinkHall(int nNetID);
	bool OnSocketHall(const TCP_Command & Command, const void * pData, unsigned short wDataSize);
	bool OnSocketHallLogon(const TCP_Command & Command, const void * pData, unsigned short wDataSize);
	bool OnSocketHallServerList(const TCP_Command & Command, const void * pData, unsigned short wDataSize);
    bool OnSocketServerMessage(const TCP_Command & Command, const void* pData, unsigned short wDataSize);//用户服务（抽奖、签到）
    bool OnSocketNoticeMessage(const TCP_Command & Command, const void* pData, unsigned short wDataSize);
    bool OnSocketItemMessage(const TCP_Command & Command, const void* pData, unsigned short wDataSize);//道具系统
    bool OnSocketSkillMessage(const TCP_Command & Command, const void* pData, unsigned short wDataSize);//技能系统
    
    bool OnQuerySkill(const void* pData, unsigned short wDataSize);
    bool OnNewGuidQuery(const void* pData, unsigned short wDataSize);
    bool OnSuperGold(const void * pData, unsigned short wDataSize);
    
    bool OnUserExchangeLottery(const void * pData, unsigned short wDataSize);
    bool OnQueryItemCount(const void * pData, unsigned short wDataSize);
    bool OnTransferItem(const void * pData, unsigned short wDataSize);
    bool OnCombineItem(const void * pData, unsigned short wDataSize);
    bool OnUseDragon(const void * pData, unsigned short wDataSize);
    bool OnUserEquipmentDragon(const void * pData, unsigned short wDataSize);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	void OnLinkRoom(CNetInfo * pNetInfo, int nError, const char * sErrMsg = NULL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	void OnLinkRoom(int nID, int nError, const char * sErrMsg = NULL);
#endif
	bool OnSocketRoom(const TCP_Command & Command, const void * pData, unsigned short wDataSize);
	bool OnSocketMainUser(const TCP_Command & Command, const void * pData, unsigned short wDataSize);
	bool OnSocketMainLogon(const TCP_Command & Command, const void * pData, unsigned short wDataSize);
    bool OnSocketMainInsure(const TCP_Command & Command, const void * pData, unsigned short wDataSize);
    bool OnSocketMainStatus(const TCP_Command & Command, const void * pData, unsigned short wDataSize);
    bool OnSocketMainConfig(const TCP_Command & Command, const void * pData, unsigned short wDataSize);
    
    //在房间内查询技能
    bool OnRoomQuerySkill(const void * pData, unsigned short wDataSize);
    bool OnRoomUseSkill(const void * pData, unsigned short wDataSize);//使用技能
    bool OnRoomGiveSkill(const void * pData, unsigned short wDataSize);//系统赠送技能

	bool OnSocketSubUserEnter(const void * pData, WORD wDataSize);
	bool OnSocketSubUserScore(const void * pData, WORD wDataSize);
	bool OnSocketSubUserStatus(const void * pData, WORD wDataSize);

	bool OnSocketSubLogonFailure(const void * pData, WORD wDataSize);
	bool OnSocketSubLogonFinish(const void * pData, WORD wDataSize);
    
    bool OnSocketSubInsureInfo(const void * pData, WORD wDataSize);
    bool OnSocketSubInsureSuccess(const void * pData, WORD wDataSize);
    bool OnSocketSubInsureFailure(const void * pData, WORD wDataSize);
    bool OnSocketSubTransferInfo(const void * pData, WORD wDataSize);
    bool OnSocketSubTransferUserInfo(const void * pData, WORD wDataSize);
    
    //签到
    bool OnGetSignInMessage(const void * pData, WORD wDataSize);//获取签到信息
    bool OnSocketSignInSucceed(const void * pData, WORD wDataSize);//签到成功
    //抽奖
    bool OnGetBonusInfo(const void * pData, WORD wDataSize);//用户进入点击抽奖，获取免费次数
    bool OnGetBonusCompleted(const void * pData, WORD wDataSize);//用户抽奖，服务端发送结果
    bool OnGetBonusMessage(const void * pData, WORD wDataSize);//抽奖公告
    
    bool OnGetChargeTime(const void* pData, WORD wDataSize);
    
    void OnUserItemUpdate(tagUserInfo * pUserInfo, tagUserStatus & LastStatus);

	bool SendData(int nNetID, WORD wMainCmdID, WORD wSubCmdID, const void * pData = NULL, WORD wDataSize = 0);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	bool SendData(CNetInfo * pNetInfo, WORD wMainCmdID, WORD wSubCmdID, const void * pData = NULL, WORD wDataSize = 0);
#endif
	bool SendSitDownPacket(unsigned short wTableID, unsigned short wChairID, char * szPassword);
    bool SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave);
    
    void BeforeGameStart();
    void SendGameOption();
	//网络通信

	//消息框
	void ShowMessage(const char * sMessage, eOperateType eOperate = eMBOK, eRequestType eRequest = eNull, bool bWait = false);
	void MessageResult(eRequestType request, eOperateType result, bool bConfirm = false);
	//消息框
    
    void switchLayer(eSwitchLayer eLayer, bool bDisconnect = false, bool bTableBack = false, int nShowChargeType = 0, int nCoinArmSkill = 0);
    void transferScore();

	tagUserInfo * SearchUserByUserID(unsigned int dwUserID);
    void ActiveUserItem(tagUserInfo * pUserInfo);
	void DeleteUserItem(unsigned int dwUserID);
    
    //喇叭公告
    void setNotice(std::string noticeStr, bool bContainIcon, std::string nickName, bool bMatch);//第一个参数为显示内容，第二个参数为是否是喇叭，第三个参数是玩家姓名，第四个参数如果为true则为打鱼，如果为false则为比赛，打鱼公告参数为（“ss”,false,"",true)
    void noticeFunc();
    void noticeRemoveCall(CCNode* node);
    
    //比赛公告
    void openMatchNotice();
    void closeMatchNotice();
    void sendMatchNotice(float dt);
    void onHttpMatchNoticeComplete(CCHttpClient* sender, CCHttpResponse* response);
	
	//邮件系统
    void readMailXML();
    void writeMailXML();
    bool OnGetMailListID(const void * pData, WORD wDataSize);//获取邮件ID
    bool onGetMailTitle(const void* pData, WORD wDataSize);//获取邮件标题
    bool onGetMailContent(const void* pData, WORD wDataSize);//获取邮件内容
    bool onGetMailPrise(const void * pData, WORD wDataSize);//领取邮件奖励
    
    //银行操作
    bool onQueryInsureInfo(const void * pData, WORD wDataSize);//银行查询
    bool OnInsureSuccess(const void * pData, WORD wDataSize);//银行成功
    bool OnInsureFail(const void * pData, WORD wDataSize);//银行失败
    bool OnTransferUserInfo(const void * pData, WORD wDataSize);//转账
    void transferScore2();//确认转账
    bool OnTransferSuccess(const void * pData, WORD wDataSize);
    //龙珠操作
    bool OnSocketSubTransferDragon(const void * pData, WORD wDataSize);
    bool OnSocketSubGetDragon(const void * pData, WORD wDataSize);
    //登录时请求是否充值成功
    bool OnChargeSuccess(const void * pData, WORD wDataSize);
    
	//VIP
    bool OnVIPInfo(const void * pData, WORD wDataSize);
    bool OnVIPResult(const void * pData, WORD wDataSize);
    bool OnGetCurrentScore(const void * pData, WORD wDataSize);//从游戏返回到大厅时，更新玩家分数
    
    //领取登录奖励
    bool OnGetLogonGift(const void * pData, WORD wDataSize);
    bool OnCharge1stRequest(const void* pData, WORD wDataSize);
    bool OnCharge1stGet(const void * pData, WORD wDataSize);
	
	void requestNotification();
	void onHttpRequestComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    bool OnMatchShareInfo(const void * pData, WORD wDataSize);
    bool OnMatchSignupRes(const void * pData, WORD wDataSize);//比赛报名结果
    bool OnMatchBegin(const void * pData, WORD wDataSize);//比赛开始
    bool OnMatchEnd(const void * pData, WORD wDataSize);//比赛结束
    bool OnMatchScoreUpdate(const void * pData, WORD wDataSize);//更新比赛排名以及分数
    bool OnMatchGetPriseRes(const void * pData, WORD wDataSize);//玩家领取比赛奖励
    bool OnMatchInfoNotify(const void * pData, WORD wDataSize);//比赛信息前几分钟通知
    bool OnMatchTimeNotify(const void * pData, WORD wDataSize);//比赛倒计时提醒
    bool OnGameBuySkillRes(const void* pData, WORD wDataSize);//游戏内金币购买技能
    bool OnGameUserLaba(const void* pData, WORD wDataSize);//游戏内发送喇叭
    bool OnQueryBankCuptinfo(const void* pData, WORD wDataSize);//游戏内查询救济金剩余次数
    bool OnGetBankCuptScore(const void* pData, WORD wDataSize);
    bool OnHaveMatchPrizeRemind(const void* pData, WORD wDataSize);
    bool OnCanMatchShare(const void * pData, WORD wDataSize);
    bool OnQiangGameResult(const void* pData, WORD wDataSize);
    
    //在线奖励
    bool OnOnlineReWardInfo(const void* pData, WORD wDataSize);
    bool OnReceiveOnlineReward(const void* pData, WORD wDataSize);
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //苹果内购
    bool OnGetPurchase(const void* pData, WORD wDataSize);
    
    void requestIOSChannel();
    void onHttpChannelComplete(CCHttpClient* sender, CCHttpResponse* response);
#endif  
    
    bool OnTelRegisterResult(const void* pData, WORD wDataSize);
    bool OnCanBandingTelResult(const void* pData, WORD wDataSize);
    bool OnBandingTelResult(const void* pData, WORD wDataSize);
    bool OnBroadLabaResult(const void* pData, WORD wDataSize);
    
    bool OnMobileCount(const void* pData, WORD wDataSize);
    bool OnBandRewardRes(const void* pData, WORD wDataSize);
    bool OnMobileCharge(const void* pData, WORD wDataSize);
    bool OnLabaTimes(const void* pData, WORD wDataSize);
    bool OnBuySkillResult(const void* pData, WORD wDataSize);
    bool OnFakeServerInfoRes(const void* pData, WORD wDataSize);
    
    bool OnFaHongBao(const void* pData, WORD wDataSize);
    bool OnAskQiang(const void* pData, WORD wDataSize);
    bool OnQiangResult(const void* pData, WORD wDataSize);
    
    bool OnDakasaiNotice(const void* pData, WORD wDataSize);
    bool OnWufuCard(const void* pData, WORD wDataSize);
    bool OnWeekBei(const void* pData, WORD wDataSize);
    
    bool OnQueryWheel(const void* pData, WORD wDataSize);
    bool OnReceiveWheel(const void* pData, WORD wDataSize);
    

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	void webRequestPay();
	void webRequestFeedBack();
	void webRequestDownload();
	void webRequest(const char * sUrl);
	
    
    void openRankInfo();
    void openChargeInfo();
    std::string OnInternet();
    
    //更新分数
    void setUserScore(long long userScore);
	
	void readXML();
    std::vector<Tag_Notification*> m_vctNotification;
	void sendNotification(bool bExist);
	std::string getMobileProduct();
    void requestAndroidOpenStatus();
    void onHttpAndroidOpenStatus(CCHttpClient* sender, CCHttpResponse* response);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void sendIOSNotification();
    
    
    std::vector<Tag_Notification*> m_vctNotification;
#endif
    
	void getScoreTime(float dt);
    //发送点击按钮消息
    void initConnect();//启动时连接
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void OnLinkHall2(CNetInfo * pNetInfo, int nError, const char * sErrMsg = NULL);
#endif
    //在Document目录下创建一个可以读写的文件
    void createMailXML();
    
    void createLocalVIPConfig();//读取本地VIP文件
    void readVIPConfig();//读取VIP文件（每次程序启动都读取，主要是读取文件中充值金额，已获取当前玩家的经验值
    
    //HTTP错误处理
    void HttpErrorHandle(CCHttpResponse* response, int nResponseError);
    
    void freeMemory();
    
    //请求充值公告
    void requestChargeNotice();
    void onHttpNoticeComplete(CCHttpClient* sender, CCHttpResponse* response);
 
    void checkChargeCompleted();
    void checkChargeCompleted(float dt);
    void receive2HallLinkInfo(float dt);
    
    void updateCurrentScore();
    
    void synchData(float dt = 1.0f);
    void receiveHallLinkInfo(float dt);
    void sendExchangeComplete();
    
    //VIP公告
    void ShowNotice(std::string noticeStr, int nID = 0);
    //比赛公告
    void ShowLaba2Notice(std::string noticeStr, int nID = 0);
    //发送VIP升级
    void sendVIPGrade();
    //喇叭公告
    void ShowLabaNotice(std::string labaStr, std::string sNickName);
    void requestMail();
    
    void ShowDakasaiNotice(std::string noticeStr);
    
    void requestIAPMail();
    void receiveIOSHallLink2();
    
    void openCheckLink();
    void openCheckLinkTime();
    
    void openCheckGameConnect();
    void openCheckGameConnectTime();
    void openCheckReceiveGameMes();
    
    std::string getCurrentUserIDTime(std::string sInfo);//获取当前用户iD+时间
    
    void addNoticeToVct(int nId, std::string sContent);
    
    void sendUserBehavior(int nUserID, int nActionTag);
    
    void sendUserTelInfo();
    
    void sendSuperPirateGuideFinished();
    
    //分享域名
    bool OnGetShareURL(const void * pData, WORD wDataSize);
    bool OnGetShareURL2(const void * pData, WORD wDataSize);
    
    void sendShareSuccess();//打中话费分享成功
    void onHttpSharedComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    
    //首充
    //支付宝
    void alipayCharge(int prise, int nFirstCharge = 1, bool bVIPUpgrade = false);
    void onHttpAlipayComplete(CCHttpClient* sender, CCHttpResponse* response);
    void notifyIOSAlipayCompleted();
    void sendAlipayChargeSuccess();
    void onAlipayChargeSuccess(CCHttpClient* sender, CCHttpResponse* response);
    std::string m_sOrderID;
    int m_nPrise;
    //微信
    void wxCharge(int prise, int nFirstCharge = 1, bool bVIPUpgrade = false);//@@金币充值
    void onHttpShared2Complete(CCHttpClient* sender, CCHttpResponse* response);
    void notifyIOSWXCompleted();
    void sendWXChargeSuccess();//微信
    void onWXChargeSuccess(CCHttpClient* sender, CCHttpResponse* response);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void AndroidAlipay(int prise);
    void delayWXChargeSuccess(float dt);
    void delayAlipayChargeSuccess(float dt);
    void AndroidYeepay(int prise, int nVIPUpgrade);
    void showDelayAlipayResult();
    void showDelayWechatResult();
#endif
    
    void chargeTest(int prise, int nChargeType);
    
    //分享结果
    void shareResult(bool bResult);
    
    //返回大厅
    void exitGameToHall();
    void backToHall();
    void backToLogon();
    
    //广告框
    void requestAdvertXML();
    void onHttpAdvertComplete(CCHttpClient* sender, CCHttpResponse* response);
    void requestAdvertImageUrl(std::string sDownload, std::string sName);
    void onHttpAdvertImageUrlComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    //向网站请求大厅公告版本号
    void sendNoticeVersion(std::string sPosition);
    void onHttpNoticeVersionCom(CCHttpClient* sender, CCHttpResponse* response);
    void sendNoticeData(std::string sPosition);
    void onHttpNoticeDataCom(CCHttpClient* sender, CCHttpResponse* response);
    void saveFile(std::string str, std::string sPosition);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void sendRoomExit();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void sendAndroidRoomExit();
#endif
    
    void sendHallLink(float dt);
    void showDisConnect();
    bool m_bStartOpenCheck;
    bool m_bReceiveHallLink;
    bool m_b2DisConnect;
    int m_n2DisConnect;
    
    CheckNetworkLayer* m_pCheckNetworkLayer;
    
	static CMainLogic * _instance;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	CNetworkService * m_pNetworkService;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	CaratManager * m_pNetworkService;
#endif
	int m_nHallSocketHandle;
	int m_nRoomSocketHandle;

	CCScene * m_pLogonScene;
	CCScene * m_pHallScene;
    CCScene * m_pTableScene;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string m_sPublicIP;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	bool m_bOpenPhoto;
    bool m_bExitGame;
	std::string m_sMobileProduct;			//手机品牌
#endif
	int m_nLogonType;
	std::string m_sAccount;
	std::string m_sPassword;
	std::string m_sNickName;
    std::string m_sServerName;
    std::string m_sTelephone;           //手机号
    std::string m_sVerificationCode;    //短信验证码
    std::string m_sModifyPassword;      //绑定手机之后新密码
	unsigned int m_nUserID;
	long long m_lUserScore;
    unsigned int m_nGameID;
    int m_nGender;
    int m_nLottery;//奖券
    int m_nGameRoomType;
    int m_nTouxiangID;
    
    CC_SYNTHESIZE(bool, bPlaying, Playing);
	CC_SYNTHESIZE(bool, bIsSignIn, IsSignIn);

	//房间信息
    std::vector<tagGameServer> m_vRoomNZNH;
    std::vector<tagGameServer> m_vRoomJCBY;
    std::vector<tagGameServer> m_vRoomPirate;
    std::vector<tagGameServer> m_vRoomFB;
    std::vector<tagGameServer> m_vRoomLKPY;
    std::vector<tagGameServer> m_vRoomPGW;//跑狗王
    std::vector<tagGameServer> m_vRoomRB;//红黑大战
	tagGameServer m_CurrentServer;
    
    std::vector<DTP_GP_Game_CellScore> m_vctCellScore;
    
    std::vector<tagGameKind> m_vGameType;

	//用户信息
	std::vector<tagUserInfo *> m_vUserInfo;
	tagUserInfo * m_pMeUserItem;
    
    //喇叭公告
    std::vector<Trumpet*> m_vctNotice;                                      //公告容器

	CMessageBox * m_pMessageBox;
    
    NoticeLayer* m_pNoticeLayer;
    LabaNoticeLayer* m_pLabaNoticeLayer;
    Laba2Layer* m_pLaba2Layer;
    DakasaiNotice* m_pDakasaiNotice;
    
	
	//是否处于分享状态
	bool m_bSharedState;
	bool m_bChargeState;
    int m_nMessageParam;
	int m_nPortTelephone;		//手机运营商
    
    bool m_bFirstCharge;    //是否首充，false为首充，true为已经充过值了
    int m_nExitGameToSpecialArm;//从游戏中进入指定炮台的充值界面
    
    //游戏属性
    WORD    KIND_ID;
    WORD    FishCount;
    std::string DIRPATH1;
    std::string DIRPATH2;
    std::string DIRPATH3;
    std::string m_sNodeIP;
    
    std::string m_sChargeURL;
    std::string m_sChargeCardURL;
	
	//邮件系统
    std::vector<tagMailInfo> m_vctMail;
    int m_nMailCount;//未读邮件的数量
    int m_nReadingMailID;
    
    int m_nGameType;       //游戏类型，1为捕鱼，2为牛牛
    BYTE m_cbMemberOrder;       //会员等级
    SCORE m_llChargeCount;      //充值成功之后获取的金币
    bool m_bFirstLogon;
    
    bool m_bGameExitHall;
    bool m_bDisConnect;//是否掉线
	bool m_bFirstRequestMail;
    
    bool m_bClickIcon;               //是否点击图标进入大厅
    int m_nVipLevel;                    //当前VIP等级
    int m_nVipExp;                      //VIP经验
    int m_nExpPercent;
    bool m_bEnterGiveCoin;              //是否从游戏界面返回到大厅中的免费金币界面
    
    std::string m_sServiceQQ;           //客服QQ
    int m_nLogonGift;                   //登陆奖励
    std::vector<Tag_VIPExpConfig> m_vctVipExpConfig;
    bool m_bReceiveVipLogon;            //VIP登录奖励是否领取
    bool m_bIsOpenLogon;                //登录完成后是否收到了登录奖励消息
    int m_nVipLogonGift;                //VIP登录奖励
    int m_nChargeBack;                  //充值返利
    int m_nCurrentInstruct;             //当前指令
    bool m_bClickMessageCharge;         //是不是点击了短信充值
    BYTE m_cbCountTimes;                //首充礼包剩余领取次数
    bool m_bFirstChargeCanGet;         //当前次数能不能领取
    bool m_bIsShowLogonGift;            //当关闭广告的时候，是否弹出登录奖励框
    bool m_bHaveAdvert;                 //当前界面上是否有广告
    bool m_bShowChouJiang;              //显示登录奖励界面
    //检测连接状态
    int m_nClickBank;
    int m_nCheckSocketShut;
    
    std::string m_sChargeNotice;//充值公告
    int m_nCurrentViewIndex;//当前视图等级
    bool m_bShowNiuGame;
    
    std::vector<NoticeVct> m_vctLaba2Notice;
    std::vector<NoticeVct> m_vctVIPNotice;
    std::vector<MoreGamesInfo> m_vMoreGamesInfo;
    std::vector<MoreGamesInfo> m_vNewAdvertInfo;
    std::vector<LabaNotice> m_vctLabaNotice;
    std::vector<NoticeVct> m_vctDakasaiNotice;
    
    //免费金币上的心形显示
    bool m_bCanShare;//是否已经分享
    int m_nBankruptcyCount;//目前领取了几次
    int m_cbCanGetBankruptcy;//是否能领取救济金
    int m_nGiveScore;//救济金领取限制
    int m_nGiveTotal;//救济金每日领取次数限制
    
    bool m_bRoomHaveLottery;//房间内有无奖券
    bool m_bGameToVIP;
    
    bool m_bIOSPassed;
    int m_nUserWeekRank;
    int m_nUserWeekBeiNum;
    
    //技能和炮台状态
    int m_nSpeedUpTimes;
    int m_nSansheTimes;
    int m_nBaojiTimes;
    int m_nSuperPirateItemCount;
    int m_nCannonStatus;
    
    //注册方式
    int m_nRegisterType;//等于1为手机号注册，等于2为账号注册
    
    //新手引导
    bool m_bShowGuide;
    bool m_bCanGetPiratePrise;//可以领取海盗的免费金币
    
    SCORE m_lFirstTime;
    SCORE m_lSecondTime;
    
    bool m_bNeedLogon;
    
    bool m_bBuyTryCannon;//是否购买试用炮，如果是购买试用炮，那么下单的时候要结算游戏
    int m_nBuyGameType;//购买道具类型：1为金币，2为武器，3为技能
    bool m_bGameChargeStatus;//在游戏中点击充值按钮
    int m_nShowChargeType;//0为不显示充值层，1为支付宝返回结果，2为微信返回结果
    int m_nShowCoinArmSkill;//1为金币，2为武器，3为技能
    std::string m_sAlipaySign;//支付宝返回客户端签名信息
    std::string m_sWXOrderID;
	bool m_bNetPageCharge;
    int m_nSkillCount[3];
	bool m_bChargeSuccess;
    bool m_bShowTryCannon;
    int m_nBuyCannonIndex;//购买炮的ID（普通炮为11-17，使用炮为31-34）
    int m_nBuyCannon2Index;//在游戏中指定打开方式购买炮台
    
    bool m_bCanBandingTel;//是否可以绑定手机号
    
    bool m_bLogonStatus;
    bool m_bLinkHallEnter;
    
    bool m_bGameDisConnect;//在游戏中是否已经断开连接（用于后台切入前台使用）
    bool m_bAutoExitGame;//系统维护房间
    bool m_bEnterLoading;//进入到加载界面
    float m_fNoticeSpeed;
	int m_nGameToHallVipLevel;
    bool m_bNetCanStatus;
    bool m_bForeGround;//从后台进入前台
    LONGLONG m_lSuperPirateWard;
    
    int m_nBindReward;
    int	m_nShareReward;
    BYTE m_cbSuperPirateGuided;
    bool m_bBYGameToSuper;
    bool m_bCanEnterPiraseFreeCoin;//可以领取海盗4000金币
    int m_nPirasePrise;//捕鱼金币不够时进入海盗可领取4000金币
    bool m_bBYToSuperPirse;//从捕鱼跳小丑领免费金币
    
    bool m_bEnterService;
    bool m_bBackgroundEnterService;
    bool m_bSuperToBY;
    bool m_bShowNoticeLayer;
    
    std::vector<CMD_GRO_MatchGroupInfo> m_vctMatchGroupInfo;
    std::string m_sMachine;
    
    static void func(int sig);
    
    bool m_bAnySDKPassed;
    bool m_bAndroidOpenStatus;
    bool m_bShowNotifyInfo;
    bool m_bShowFuben;
    bool m_bFirstChargeUIStatus;
    int m_nShowGuideScore;
    bool m_bLogonSuccess;//登录完成
    int m_nShareInterface;
    bool m_bShowMatch;
    bool m_bShowZhouMatch;
    bool m_bShowBeibao;
    bool m_bShowChat;
    bool m_bShowOnlineCoin;
    std::vector<CMD_GRO_OneOnlineRewardInfo> m_vctOnlineRewardInfo;
    bool m_bAutoCheckOnlineInfo;
    //广告框
    std::vector<Tag_AdvertConfig> m_vctAdvertConfig;
    bool m_bShowAdvert;
    bool m_bShowShareInfo;
    
    bool m_bReceiveShowWheel;//收到显示转盘的消息
    bool m_bShowWheelNoSign;//签到已完成，登录时直接显示转盘
    BYTE m_cbWheelType[10];
    int m_nWheelCount[10];
    
    //1元购
    bool m_bCanOneCharge;
    bool m_bShowOneCharge;
    int m_nOneCoinCount;
    BYTE m_cbOneHuafei;
    BYTE m_cbOneSpeedCount;
    BYTE m_cbOneSuodingCount;

    std::string m_sAndroidMachine;
};

#endif
