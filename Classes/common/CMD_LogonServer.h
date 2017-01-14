#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GP_LOGON				1									//广场登录

//登录模式
#define SUB_GP_LOGON_GAMEID			1									//I D 登录
#define SUB_GP_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_GP_REGISTER_ACCOUNTS	3									//注册帐号
#define SUB_GP_GUEST_REGISTER		4									//游客注册

#define DTP_GP_IOSCHARGEHIDE        22
#define DTP_GP_IOSPASSED            23
#define DTP_GP_MEMBER_ORDER         24
#define DTP_GP_GIFT_TICKET          25
#define DTP_GP_SHOWFAKETABLE        26
#define DTP_GP_VIPINFO				27  //玩家的VIP信息
#define DTP_GP_CUSTOMERSERVICEQQ	28  //客服QQ
#define DTP_GP_LOGONGIFT			29  //登陆奖励，每天可以领取一次
#define DTP_GP_GAME_CELLSCORE       30
#define DTP_GP_CHARGE_FIRSTINFO     31

#define DTP_GP_PASS_PORT_ID           32
#define DTP_GP_SHOWNIUGAME          34
#define DTP_GP_SHOW_FUBEN           35

#define DTP_GP_BI_SAI_STATUS		36

#define DTP_GP_WEEK_CMPT_DATA		37

#define DTP_GP_ONE_CHARGE           38

struct DTP_GP_OneFirstCharge
{
    int nCoinScore;
    BYTE cbHuaFeiNum;
    BYTE cbJiaSuCount;
    BYTE cbSuoDingCount;
    bool bCanCharge;
};


struct DTP_GP_WeekCmptData
{
    int nWeekCmptRank;
    int nWeekCmptCup;
};

//开关附加属性
enum enButtonSwitch
{
    enDayCmptClosed=0x01,	//日赛关闭
    enWeekCmptClosed=0x02,	//周赛关闭
    enPackageClosed=0x04,	//背包关闭
    enChatClosed=0x08,		//聊天关闭
    enOnlineCoinClosed=0x16,//在线奖励关闭
};
//比赛开关：0是比赛全部关闭，1是全天赛开周赛关，2是全部开
struct DTP_GP_ButtonSwitch
{
    int nButtonSwitch;
};

struct DTP_GP_ShowFuben
{
    BYTE cbShowFuben;
};

struct DTP_GP_ShowNiuGame
{
    BYTE cbShowNiuGame;
};

struct DTP_GP_IOS2Passed
{
    BYTE cbPassed;
};

struct DTP_GP_ChargeFirstInfo//首充礼包领取剩余次数
{
    BYTE cbCount;
    BYTE cbCanGet;
};

struct DTP_GP_LogonGift
{
    BYTE cbCanGetBankruptcy;
    int nBankruptcyCount;
    int nBankruptcyCountMax;
    int nGiveScore;
    DWORD dwGiftScore;
    int nCanShare;
};

struct DTP_GP_VipInfo
{
    BYTE cbVIPLevel;//VIP等级
    DWORD dwVIPExp;//VIP经验
    DWORD dwExpPerCent;//一分钱对应的经验值
};


struct DTP_GP_ShowFakeTable
{
    BYTE bShowFakeTable;
};


struct DTP_GP_Game_CellScore
{
    WORD wServerID;
    int nCellScore;
};

struct DTP_GP_GamePlayer
{
    DWORD dwGamePlayer;
};

struct DTP_GP_GiftTicket
{
    DWORD dwGiftTicket;
};

struct DTP_GP_MemberOrder
{
    BYTE cbMemberOrder;
};

struct DTP_GP_IOSChargeHide
{
    int nID;
    char szURL[64];
};

struct DTP_GP_IOSPassed
{
    BYTE cbPassed;
};

//登录结果
#define SUB_GP_LOGON_SUCCESS		100									//登录成功
#define SUB_GP_LOGON_FAILURE		101									//登录失败
#define SUB_GP_LOGON_FINISH			102									//登录完成
#define SUB_GP_VALIDATE_MBCARD      103                                 //登录失败
#define SUB_GP_VALIDATE_PASSPOSTID  104                                 //登录失败
#define SUB_GP_GUEST_REGISTER_SUCCESS	105									//游客注册成功

//升级提示
#define SUB_GP_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////
//

#define MB_VALIDATE_FLAGS           0x01                                //效验密保
#define LOW_VER_VALIDATE_FLAGS      0x02                                //效验低版本

//I D 登录
struct CMD_GP_LogonGameID
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	DWORD							dwGameID;							//游戏 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	BYTE							cbValidateFlags;			        //校验标识
    
};

//帐号登录
struct CMD_GP_LogonAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	BYTE							cbValidateFlags;			        //校验标识
};






/*
 //注册帐号
 struct CMD_GP_RegisterAccounts
 {
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
 
	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码
 
	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwRefererID;						//推荐人ID
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//安全手机
	BYTE							cbValidateFlags;			        //校验标识
 };
 */
//注册帐号
struct CMD_GP_RegisterAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
    //DWORD							dwRefererID;						//推荐人ID
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//安全手机
	BYTE							cbValidateFlags;			        //校验标识
};

//CMD_GP_LogonAccounts、CMD_GP_RegisterAccounts、CMD_GP_Guest_Reg共同使用
#define DTP_GP_USERDATA1 10
#define DTP_GP_USERDATA2 11
#define DTP_GP_USERDATA3 12
#define DTP_GP_USERDATA4 13
#define DTP_GP_USERDATA5 14
#define DTP_GP_USERDATA6 15
#define DTP_GP_USERDATA7 16
#define DTP_GP_USERDATA8 17

#define DTP_GP_PASSWORD 100//玩家密码
#define DTP_GP_LOCALIP 101//内网IP
#define DTP_GP_CLIENTVERSION 102//客户端版本号

#define DTP_GP_CLIENTIDFA  103

struct DTP_GP_Position
{
    float longitude;
    float latitude;
};

//密码加密
struct DTP_GP_Password
{
    WORD wCount;
    char cbData[LEN_PASSWORD];
};

//推荐人信息
struct DTP_GP_RefereeInfo
{
    DWORD                           dwRefereeID;		//推荐人ID
};

//游客注册
struct CMD_GP_GuestRegister
{
    //系统信息
    DWORD							dwPlazaVersion;						//广场版本
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
    
    //密码变量
    TCHAR							szLogonPass[LEN_MD5];				//登录密码
    TCHAR							szInsurePass[LEN_MD5];				//银行密码
    
    //注册信息
    WORD							wFaceID;							//头像标识
    BYTE							cbGender;							//用户性别
    TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
    TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
    TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
    TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
    TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//安全手机
    BYTE							cbValidateFlags;			        //校验标识
};

//游客注册成功
struct CMD_GP_GuestRegisterSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团标识
	DWORD							dwCustomID;							//自定标识
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力

	//用户成绩
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//用户银行

	//用户信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//配置信息
	BYTE                            cbShowServerStatus;                 //显示服务器状态
};

//手机号注册
#define SUB_GP_MBCODE_VERIFY	    427									//手机号验证
#define SUB_GPR_MBREIGSTER  	    5									//手机号注册
#define SUB_GP_MBLOGONO  	    	7									//手机号登陆

#define SUB_GP_MBCODE_VERIFY_RES	428									//手机号验证
#define SUB_GP_MBREIGSTER_RES  	    107									//手机号注册
#define SUB_GP_MBLOGONO_RES  	    108									//手机号登陆

//注册帐号请求发送验证码
struct CMD_GPR_MBVerificationCode
{
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
};

struct CMD_GPO_MBVerificationCode
{
    bool bIsVerificationCodeSend;
    TCHAR szDescribeString[128];				//描述消息
};

//手机注册帐号
struct CMD_GP_MBNumRegister
{
    TCHAR							szVerificationCode[LEN_MOBILE_PHONE];//验证码
    CMD_GP_RegisterAccounts			RegisterAccounts;
};

//登录成功
struct CMD_GP_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团标识
	DWORD							dwCustomID;							//自定标识
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力

	//用户成绩
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//用户银行

	//用户信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//配置信息
	BYTE                            cbShowServerStatus;                 //显示服务器状态
};

//充值信息
struct DTP_GP_RechargeInfo
{
    BYTE							cbIsRecharge;						//是否充值
};

//登录失败
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//登陆完成
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//中断时间
	WORD							wOnLineCountTime;					//更新时间
};

//登录失败
struct CMD_GP_ValidateMBCard
{
	UINT								uMBCardID;						//机器序列
};

//升级提示
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//社团信息
#define DTP_GP_MEMBER_INFO			2									//会员信息
#define	DTP_GP_UNDER_WRITE			3									//个性签名
#define DTP_GP_STATION_URL			4									//主页信息
#define DTP_GP_SPECIALNODEIP        20                                  //专业节点
#define DTP_GP_SPECIALNODEURL       21                                  //专业域名

#define DTP_GP_REG_AGENTID			1									//推荐人ID
#define DTP_GP_RECHARGE_INFO		5									//充值信息


//社团信息
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//社团索引
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
};

//会员信息
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间
};

//////////////////////////////////////////////////////////////////////////
#define SUB_GPR_CHARGE_VALUE           603
#define SUB_GPO_CHARGE_VALUE           604
#define SUB_GPR_CHARGE_CANCEL          605

struct CMD_GPR_ChargeValue
{
    DWORD dwUserID;
};

struct CMD_GPO_ChargeValue
{
    SCORE dwScore;
};

struct CMD_GPR_ChargeCancel
{
    DWORD dwUserID;
};

//捕鱼大咖签到系统

//用户签到
#define SUB_GP_QUERY_SIGNIN_INFO			646							//查询签到
#define SUB_GP_QUERY_SIGNIN_INFO_RESULT		647							//签到信息
#define SUB_GP_USER_SIGNIN_REQ				648							//点击签到
#define SUB_GP_USER_SIGNIN_RESULT			649							//签到结果

// 消息体
//签到信息查询
struct CMD_GP_QuerySigninInfo
{
    DWORD dwUserID;
};

//签到查询每天信息
struct tagSignInDayInfo
{
    BYTE cbType;//1金币、2奖券、3话费、4初级珍珠、5中级珍珠、6高级珍珠、7加速、8散射、9锁定，10藏宝图，11-19是9个炮
    BYTE cbAttrib;//本天签没签（1为已签）
    int nCount;
};

//签到查询结果
struct CMD_GP_UserSigninInfo
{
    int cbNum1;
    int cbNum2;
    tagSignInDayInfo normal[7];
    tagSignInDayInfo vip[7];
};

//点击签到操作
struct CMD_GP_SignIn
{
    DWORD							dwUserID;   //用户ID
    BYTE							cbVip;      //是否是Vip用户, 0 是普通用户签到, 1 是Vip签到
};

//点击签到结果
struct CMD_GP_SignIn_Result
{
    bool bSuccess;
    BYTE cbUserStatus;//0为普通用户签到，1为VIP签到
    SCORE llScoreCont;
    SCORE llLotteryCount;
    TCHAR szDescription[128];
};

//抽奖查询
struct CMD_GP_QryLtryInfo
{
	DWORD							dwUserID;							//用户ID
};

//查询结果
struct CMD_GP_QryLtryInfo_Res
{
	WORD							wFreeCount;							//免费次数
	TCHAR							szErrorDescribe[128];				//结果描述
};

//抽奖操作
struct CMD_GP_UserLtry
{
	DWORD							dwUserID;							//用户ID
};

//抽奖结果
struct CMD_GP_UserLtry_Res
{
    WORD							wIsLtrySuc;							//抽奖成功
    WORD							wPrizeID;							//中奖等级
    WORD							wFreeCount;							//免费次数
    SCORE                           lUserScore;                         //玩家当前剩余金币
    TCHAR							szErrorDescribe[128];				//结果描述
};

//抽奖消息
struct CMD_GP_LtryMSG_Res
{
	TCHAR							szName[128];					//用户名称
	WORD							wPrizeID;						//中奖ID
};

#define SUB_GPR_LABA_TIMES          650                             //喇叭免费次数
#define SUB_GPO_LABA_TIMES          651

struct CMD_GPR_LabaTimes
{
    DWORD                           dwUserID;
};

struct CMD_GPO_LabaTimes
{
    int                             nLabaTimes;
    int                             nEnrollmentfee;
};

#define SUB_GP_BUY_SKILL            652									//金币兑换技能
#define SUB_GP_BUY_SKILL_RESULT		653									//金币兑换技能结果

struct CMD_GPR_BuySkill
{
    DWORD dwUserID;
    BYTE cbSkillID;
    int nCount;
};

struct CMD_GPO_BuySkill_Result
{
    bool bSuccess;
    BYTE cbSkillID;
    int nCount;
    SCORE llScore;
    TCHAR szDescribeString[128];
};

#define SUB_GPO_QUERY_FAKE_SERVERINFO_RES	655		//query fake server info

struct tagFakeServerInfo
{
    WORD wServerID;
    WORD wServerPort;
    WORD wOnlineUserCnt;
};

struct CMD_GPR_Query_Fake_Serverinfo_Res
{
    WORD wServerCnt;
    WORD wDataSize;
    tagFakeServerInfo  aFakeServerInfo[0];
};


//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_GP_SERVER_LIST			2									//列表信息

//获取命令
#define SUB_GP_GET_LIST				1									//获取列表
#define SUB_GP_GET_SERVER			2									//获取房间
#define SUB_GP_GET_ONLINE			3									//获取在线
#define SUB_GP_GET_COLLECTION		4									//获取收藏

//列表信息
#define SUB_GP_LIST_TYPE			100									//类型列表
#define SUB_GP_LIST_KIND			101									//种类列表
#define SUB_GP_LIST_NODE			102									//节点列表
#define SUB_GP_LIST_PAGE			103									//定制列表
#define SUB_GP_LIST_SERVER			104									//房间列表
#define SUB_GP_VIDEO_OPTION			105									//视频配置
#define SUB_GP_LIST_SMALL_SERVER    106                                 //房间列表新

//完成信息
#define SUB_GP_LIST_FINISH			200									//发送完成
#define SUB_GP_SERVER_FINISH		201									//房间完成

//在线信息
#define SUB_GR_KINE_ONLINE			300									//类型在线
#define SUB_GR_SERVER_ONLINE		301									//房间在线

//////////////////////////////////////////////////////////////////////////////////

//获取在线
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//房间数目
	WORD							wOnLineServerID[MAX_SERVER];		//房间标识
};

//类型在线
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//类型数目
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//类型在线
};

//房间在线
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//房间数目
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//房间在线
};

//////////////////////////////////////////////////////////////////////////////////
//服务命令

#define MDM_GP_USER_SERVICE			3									//用户服务

//账号服务
#define SUB_GP_MODIFY_MACHINE		100									//修改机器
#define SUB_GP_MODIFY_LOGON_PASS	101									//修改密码
#define SUB_GP_MODIFY_INSURE_PASS	102									//修改密码
#define SUB_GP_MODIFY_UNDER_WRITE	103									//修改签名

//修改头像
#define SUB_GP_USER_FACE_INFO		200									//头像信息
#define SUB_GP_SYSTEM_FACE_INFO		201									//系统头像
#define SUB_GP_CUSTOM_FACE_INFO		202									//自定头像

//个人资料
#define SUB_GP_USER_INDIVIDUAL		301									//个人资料
#define	SUB_GP_QUERY_INDIVIDUAL		302									//查询信息
#define SUB_GP_MODIFY_INDIVIDUAL	303									//修改资料

//VIP公告
#define SUB_GP_VIPUPGRADE           308

#define MDM_GP_USER_NOTICE          6
#define SUB_GPR_VIPUPGRADE          700
#define SUB_GPO_VIPUPGRADE          701
#define SUB_GPR_CHANGEITEM          702

#define SUB_GPR_BOARDCAST           703

#define SUB_GP_SystemBroadLaBa      704

#define SUB_GPR_EXCHANGE_MOBILE     705                                 //兑换话费公告

struct CMD_GPR_ExchangeMobile
{
    int         nExchangeType;//1为移动，2为联通，3为电信
    int         nExchangeCount;//30, 50, 100
    TCHAR       szNickName[LEN_NICKNAME];
};

struct CMD_GP_VIPUpgrade
{
    BYTE        cbVIPLevel;
    TCHAR       szNickName[LEN_NICKNAME];
};

struct CMD_GP_ChangeItem
{
    DWORD       dwLotteryCount;
    TCHAR       szNickName[LEN_NICKNAME];
    TCHAR       szItemName[LEN_NICKNAME];
};

#define SUB_GPR_NOTICE_WUFU_CARD    719
struct CMD_GPR_NoticeWufuCard
{
    int         nType;//1为50元京东卡，2为100元京东卡
    TCHAR       szNickName[LEN_NICKNAME];
};

#define SUB_GPR_FA_HONG_BAO 706
#define SUB_GPO_FA_HONG_BAO 707

struct CMD_GPR_FaHongBao
{
    DWORD dwUserID;
    WORD wCount;
    int nScore;
    char szContent[128];
};
struct CMD_GPO_FaHongBao
{
    bool bSuccess;
    int nCurrentScore;
    char szContent[128];
};

#define SUB_GPO_ASK_QIANG 708
#define SUB_GPR_CONFIRM_QIANG 709
#define SUB_GPO_QIANG_RESULT 710

struct CMD_GPO_AskQiang
{
    DWORD dwUserID;
    DWORD dwHongBaoID;
};

struct CMD_GPO_QiangResult
{
    bool bSuccess;
    int nScore;
    int nCurrentScore;
    char szContent1[64];
    char szContent2[128];
};

#define SUB_GP_FA_GE_REN_HONG_BAO 711

struct CMD_GPR_FaGeRenHongBao
{
    DWORD dwUserID;
    DWORD dwGameID;
    int nScore;
    char szContent[128];
};

#define SUB_GP_QUERY_GE_REN_HONG_BAO       712
struct CMD_GPO_QueryGeRenHongBao
{
    DWORD dwUserID;
};

#define SUB_GPO_DAKASAI_NOTICE              713
struct CMD_GPO_DakasaiNotice
{
    char szDakasaiNotice[256];
};

#define SUB_GPR_DAKASAI_ROOM_NOTICE              714
struct CMD_GPR_DakasaiRoomNotice
{
    DWORD dwUserID;
    WORD wServerID;
};

#define SUB_GPR_WUFU_CARD          715
#define SUB_GPO_WUFU_CARD          716
struct CMD_GPR_WufuCard
{
    DWORD dwUserID;
};

struct CMD_GPO_WufuCard
{
    int nCard[5];
};

#define SUB_GPR_WEEK_BEI        717
#define SUB_GPO_WEEK_BEI        718

struct CMD_GPR_WeekBei
{
    DWORD dwUserID;
};

struct CMD_GPO_WeekBei
{
    int nWeekBei;
};

#define SUB_GPR_QUERY_WHEEL     720
#define SUB_GPO_QUERY_WHEEL     721

struct CMD_GPR_QueryWheel
{
    DWORD dwUserID;
};

struct CMD_GPO_QueryWheel
{
    BYTE cbType[10];
    int nCount[10];
};

#define SUB_GPR_RECEIVE_WHEEL   722
#define SUB_GPO_RECEIVE_WHEEL   723

struct CMD_GPR_ReceiveWheel
{
    DWORD dwUserID;
};

struct CMD_GPO_ReceiveWheel
{
    BYTE cbIndex;
    int nCurrentScore;
    int nCurrentMatchCup;
};

#define SUB_GPR_CHANGE_HEAD     726

struct CMD_GPR_ChangeHead
{
    DWORD dwUserID;
    int nHeadID;
};


//银行服务
#define SUB_GP_USER_SAVE_SCORE		400									//存款操作
#define SUB_GP_USER_TAKE_SCORE		401									//取款操作
#define SUB_GP_USER_TRANSFER_SCORE	402									//转账操作
#define SUB_GP_USER_INSURE_INFO		403									//银行资料
#define SUB_GP_QUERY_INSURE_INFO	404									//查询银行
#define SUB_GP_USER_INSURE_SUCCESS	405									//银行成功
#define SUB_GP_USER_INSURE_FAILURE	406									//银行失败
#define SUB_GP_QUERY_USER_INFO_REQUEST		407							//查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT		408							//用户信息
#define SUB_GP_USER_TRANSFER_INFO			409
#define SUB_GP_USER_CHECK_PASSWORD			410
#define SUB_GP_USER_CHECK_PASSWORD_RETURN	411

//用户抽奖
#define SUB_GP_QUERY_LOTTERY_INFO			416							//查询抽奖
#define SUB_GP_QUERY_LOTTERY_INFO_RESULT	417							//查询结果
#define SUB_GP_USER_LOTTERY					418							//用户抽奖
#define SUB_GP_USER_LOTTERY_RESULT			419							//用户抽奖
#define SUB_GP_LOTTERY_FINISH				420							//抽奖结束
#define SUB_GP_LOTTERY_MSG					421							//抽奖消息

#define SUB_GP_QUERY_RECHARGE_INFO			422							//充值信息
#define SUB_GP_QUERY_RECHARGE_INFO_RES		423							//充值信息


#define SUB_GPR_CHECK_MOBILE_EXIT			432							//查询是否绑定过手机号
#define SUB_GPR_CHECK_MOBILE_EXIT_RES		433							//查询是否绑定过手机号结果

struct CMD_GPR_NumCheckExit
{
    DWORD dwUserID;
};

struct DBR_GP_CHECK_BINDNUM_EXIT_RES
{
    BYTE cbIsExit;
};

#define SUB_GPR_BINGDING                    643
#define SUB_GPR_BINGDING_RES                644
#define SUB_GPR_SUPERPIRATE_GUIDE_FINISHED  645

struct CMD_GPR_BindReward
{
    DWORD dwUserID;
};
struct CMD_GPR_BindRewadRes
{
    int nBindReward;
    int nShareReward;
    BYTE cbNewGuid;
};
struct CMD_GPR_SuperPirateFinished
{
    DWORD dwUserID;
};
//手机号绑定验证码请求
#define SUB_GPR_MOBILE_BIND					429							//手机号绑定验证码请求
#define SUB_GPR_MOBILE_BIND_VERIFY			430							//手机号绑定验证码验证
#define SUB_GPO_MOBILE_VERIFY_RES			431							//手机号绑定验证码验证结果

#define DTP_GP_UI_NICKNAME			1//昵称追加包

struct CMD_GPR_MB_BIND_REQUEST
{
    DWORD							dwUserID;							//用户ID
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
};

//手机号绑定验证码验证结构体
struct CMD_GP_MobileCaptchaVerific
{
    DWORD                           dwUserID;
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	 //绑定手机;
    TCHAR							szVerificationCode[LEN_MOBILE_PHONE];//验证码
    TCHAR							szLogonPass[LEN_MD5];				//登录密码
};

//绑定手机号处理结果
struct DBO_GP_MBBindRes
{
    BYTE                            cbResult;//0失败，1成功
    int								nCurrentGold;						//奖励之后的金币值
    TCHAR							szDescribeString[128];				//描述消息
};

#define SUB_GPR_QUERY_MAIL_ID_LIST			500
#define SUB_GPO_QUERY_MAIL_ID_LIST			501
#define SUB_GPR_QUERY_MAIL_INFO				502							//获取邮件信息
#define SUB_GPO_QUERY_MAIL_INFO				503
#define SUB_GPR_QUERY_MAIL_CONTENT			504
#define SUB_GPO_QUERY_MAIL_CONTENT			505
#define SUB_GPR_QUERY_MAIL_ITEM             506
#define SUB_GPO_QUERY_MAIL_ITEM             507

#define DTP_GP_MAIL_CONTENT					100
#define DTP_GP_MAIL_CHUJI_DRAGON			1
#define DTP_GP_MAIL_ZHONGJI_DRAGON			2
#define DTP_GP_MAIL_GAOJI_DRAGON			3
#define DTP_GP_MAIL_ZHIZUN_DRAGON			4
#define DTP_GP_MAIL_CANNON					5
#define DTP_GP_MAIL_SKILL_JS				6
#define DTP_GP_MAIL_SKILL_SS				7
#define DTP_GP_MAIL_SKILL_BJ				8
#define DTP_GP_MAIL_LOTTERY					9
#define DTP_GP_MAIL_GIFTGOLD				10

struct CMD_GPO_ItemInfo
{
    int nItemCount;							//道具数量
};

struct CMD_GPO_GetMailItem
{
    BYTE cbSuccess;
    char szResult[30];
};

struct CMD_GPO_MailContexts
{
    BYTE cbGetMailContext;
};

//广播速度
#define SUB_GPR_BROADCAST_SPEED				635							//请求广播速度
#define SUB_GPO_BROADCAST_SPEED_RES			636							//请求广播速度结果

struct CMD_GP_BroadCast_Speed_Res
{
    float fBroadCastSpeed;//广播速度
};

#define SUB_GPR_MOBILE_COUNT				637							//请求type类型的物品数量
#define SUB_GPO_MOBILE_COUNT                638							//请求type类型的物品数量结果

struct CMD_GPR_MobileCount
{
    DWORD dwUserId;
};

struct CMD_GPO_MobileCount
{
    int nCount;
};

#define SUB_GPR_MOBILE_CHARGE               639                         //兑换话费
#define SUB_GPO_MOBILE_CHARGE               640                         //兑换话费结果

struct CMD_GPR_MobileCharge
{
    DWORD dwUserID;
    int nMobileCharge;
};

struct CMD_GPO_MobileCharge
{
    bool bSuccess;
    int nCurrentMobileCharge;
};

//苹果内购充值
#define SUB_GPR_IAP_PRODUCT_LIST			600							//in app purchase
#define SUB_GPO_IAP_PRODUCT_LIST			601
#define SUB_GPO_IAP_TRANSACTIONRECEIPT      602

#define DTP_GP_SHAREINFO                    611                         //分享（应用名称、链接、显示内容)

struct CMD_GP_ShareInfo
{
    char szShareName[32];
    char szShareURL[64];
    std::string szShareContent;
};


struct CMD_GPR_IAPProduct
{
    WORD wRMB;//价格
    WORD wType;//0金币，1炮台
    char szProductID[25];//商品ID
};

struct CMD_GPR_IAPProductList
{
    CMD_GPR_IAPProduct product[12];
};
//邮件系统
struct tagMailInfo
{
    unsigned int dwMailID;
    BYTE cbMailStatus;
    BYTE cbGeted;
    std::string szMailTime;//例：2015-08-06 17:33:25，共计19个字符，即19个字节
    std::string szMailTitle;
    std::string szMailContent;
    std::string szDuihuanma;
    std::string szGongzhonghao;
};

struct CMD_GPR_MailIDList
{
    DWORD dwUserID;
};

struct CMD_GPO_MailIDList
{
    BYTE cbCount;
    DWORD dwMailID[30];
};

struct CMD_GPR_MailInfo
{
    BYTE cbCount;
    DWORD dwMailID[30];
};

struct CMD_GPO_MailInfo
{
    unsigned int dwMailID;
    BYTE cbMailStatus;
    BYTE cbGeted;
    char szMailTime[20];//例：2015-08-06 17:33:25，共计19个字符，即19个字节
    char szMailTitle[64];
};

struct CMD_GPO_MailInfoSet
{
    unsigned int dwMailCount;
    CMD_GPO_MailInfo MailInfo[30];
};

struct CMD_GPR_MailContent
{
    unsigned int dwMailID;
};

struct CMD_GPR_MailGetInfo
{
    unsigned int dwMailID;
};

//邮件领取金币
struct CMD_GPR_MailGetScore
{
    SCORE lCurrentScore;
    bool bSuccess;
    char szMailContent[128];
};

#define CMD_GP_QUERY_INSURE_INFO	404


//查询银行
struct CMD_GP_QueryInsureInfo
{
    DWORD							dwUserID;							//用户 I D
};


#define SUB_GP_USER_INSURE_INFO		403									//银行资料


//银行资料
struct CMD_GP_UserInsureInfo
{
    WORD							wRevenueTake;						//税收比例
    WORD							wRevenueTransfer;					//税收比例
    WORD							wServerID;							//房间标识
    SCORE							lUserScore;							//用户金币
    SCORE							lUserInsure;						//银行金币
    SCORE							lTransferPrerequisite;				//转账条件
};

//操作结果
#define SUB_GP_OPERATE_SUCCESS		900									//操作成功
#define SUB_GP_OPERATE_FAILURE		901									//操作失败

//操作失败
struct CMD_GP_OperateFailure
{
    LONG							lResultCode;						//错误代码
    TCHAR							szDescribeString[128];				//描述消息
};

//操作成功
struct CMD_GP_OperateSuccess
{
    LONG							lResultCode;						//操作代码
    TCHAR							szDescribeString[128];				//成功消息
};

//////////////////////////////////////////////////////////////////////////////////

//充值查询
struct CMD_GP_RechargeInfo
{
    DWORD							dwUserID;							//用户ID
};

//充值信息结果
struct CMD_GP_RechargeInfo_Res
{
    BYTE							cbIsRecharge;
    TCHAR							szDescribeString[128];				//描述消息
};

//修改密码
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改密码
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改签名
struct CMD_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
};

//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////

//用户头像
struct CMD_GP_UserFaceInfo
{
	WORD							wFaceID;							//头像标识
	DWORD							dwCustomID;							//自定标识
};

//修改头像
struct CMD_GP_SystemFaceInfo
{
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改头像
struct CMD_GP_CustomFaceInfo
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
};

//////////////////////////////////////////////////////////////////////////////////

//绑定机器
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//绑定标志
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//////////////////////////////////////////////////////////////////////////////////

//个人资料
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//用户 I D
};

//查询信息
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//用户 I D
};

//修改资料
struct CMD_GP_ModifyIndividual
{
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//用户昵称
#define DTP_GP_UI_USER_NOTE			2									//用户说明
#define DTP_GP_UI_UNDER_WRITE		3									//个性签名
#define DTP_GP_UI_QQ				4									//Q Q 号码
#define DTP_GP_UI_EMAIL				5									//电子邮件
#define DTP_GP_UI_SEAT_PHONE		6									//固定电话
#define DTP_GP_UI_MOBILE_PHONE		7									//移动电话
#define DTP_GP_UI_COMPELLATION		8									//真实名字
#define DTP_GP_UI_DWELLING_PLACE	9									//联系地址

//////////////////////////////////////////////////////////////////////////////////

//存入金币
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入金币
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//提取金币
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账金币
struct CMD_GP_UserTransferScore
{
	DWORD							dwUserID;							//用户 I D
	BYTE                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//银行成功
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//赠送信息
struct DBO_GP_UserInsureTransferInfo
{
	DWORD							dwRecordID;
	DWORD							dwSourceID;
	DWORD							dwTargetID;
	SCORE							lSwapScore;
	SYSTEMTIME						timeTransfer;
	TCHAR							szSourceName[LEN_NICKNAME];
	TCHAR							szTargetName[LEN_NICKNAME];
};

//银行失败
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//提取结果
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
};

//查询用户
struct CMD_GP_QueryUserInfoRequest
{
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//用户信息
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//目标用户
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//密码验证
struct CMD_GP_CheckPassword
{
	DWORD							dwUserID;
	TCHAR							szPassword[LEN_PASSWORD];
};

//密码验证结果
struct CMD_GP_CheckPasswordReturn
{
	bool							bPasswordRight;
};
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_GP_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE	100									//数据查找
#define SUB_GP_C_SEARCH_CORRESPOND	101									//协调查找

//查找服务
#define SUB_GP_S_SEARCH_DATABASE	200									//数据查找
#define SUB_GP_S_SEARCH_CORRESPOND	201									//协调查找

//////////////////////////////////////////////////////////////////////////////////

//协调查找
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//协调查找
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//用户数目
	tagUserRemoteInfo				UserRemoteInfo[16];					//用户信息
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//VIP
#define SUB_GPR_VIP_INFO					610
#define SUB_GPR_VIP_EVERYDAY_GIFT			612
#define SUB_GPR_VIP_CHARGE_BACK				614

#define SUB_GPO_VIP_INFO					611
#define SUB_GPO_VIP_RESULT					613

struct CMD_GPR_VIPUser//610、612、614共用
{
    DWORD dwUserID;
};

struct CMD_GPO_VIPInfo
{
    BYTE cbVIPLevel;//VIP等级
    BYTE cbReserved[3];//保留
    DWORD dwVIPExp;//VIP经验
    DWORD dwVIPNextExp;//升级需要经验
    DWORD dwEverydayGift;//每日领取金币，如果为0，说明不能领取，或者已经领取
    DWORD dwChargeBack;//充值返现金币，如果为0，说明当前没有返现
};

struct CMD_GPO_VIPResult
{
    SCORE lScore;
    char szDescription[1024];
};

#define SUB_GPR_CURRENT_SCORE  616     //从游戏返回到大厅
#define SUB_GPO_CURRENT_SCORE  617

struct CMD_GPR_CurrentScore
{
    DWORD dwUserID;
};

struct CMD_GPO_CurrentScore
{
    SCORE lCurrentScore;
    int nLotteryCount;
    BYTE cbCount;
    BYTE cbCanGet;
    BYTE cbVIPLevel;
    DWORD dwVIPExp;
    DWORD dwExpPerCent;
    DWORD dwChargeBack;
};

#define SUB_GPR_LOGONGIFT       618     //登录奖励领取
#define SUB_GPO_LOGONGIFT       619     //收到登录领取奖励

struct CMD_GPR_LogonGift
{
    DWORD dwUserID;
};

struct CMD_GPO_LogonGift
{
    SCORE llCurrentScore;
    char szDescription[128];
};

#define SUB_GPR_CHARGE1ST_REQUEST			620//首充成功
#define SUB_GPO_CHARGE1ST_REQUEST			621

#define SUB_GPR_CHARGE1ST_GET				622//领取首充礼包的奖励
#define SUB_GPO_CHARGE1ST_GET				623

struct CMD_GPR_Charge1stRequest
{
    DWORD dwUserID;
};

struct CMD_GPO_Charge1stRequest
{
    BYTE cbCount;
    char szDescription[128];
};

struct CMD_GPR_Charge1stGet
{
    DWORD dwUserID;
};

struct CMD_GPO_Charge1stGet
{
    BYTE cbSuccess;
    BYTE cbCount;
    int nGiftScore;
    SCORE lCurrentScore;
    char szDescription[128];
};

#define SUB_GPR_CHARGE_SUCCESS                  624

struct SUB_GPR_ChargeSuccess
{
    DWORD dwUserID;
    char szOrderID[40];
    char szMD5[LEN_PASSWORD];
};

#define SUB_GP_WEALTH_BAG_TIME              625//福袋时间
#define SUB_GPR_WEALTH_BAG_ENROLL           626//报名
#define SUB_GPO_WEALTH_BAG_ENROLL           627//报名结果
#define SUB_GP_WEALTH_BAG_BEGIN             628//游戏开始
#define SUB_GP_WEALTH_BAG_END               629//游戏结束
#define SUB_GP_WEALTH_BAG_SCORE             630//客户端返回结果
#define SUB_GP_WEALTH_BAG_RESULT            631//服务端返回结果
#define SUB_GP_WEALTH_BAG_BEFORE            632

struct CMD_GPR_WealthBagEnroll
{
    DWORD dwUserID;
    TCHAR szNickName[LEN_NICKNAME];			//用户昵称
};

struct CMD_GPO_WealthBagScore
{
    int nWealthBagScore;
};

struct CMD_GPO_WealthBagBefore
{
    DWORD dwWealthBagBefore;
};

struct CMD_GPO_WealthBagEnroll
{
    bool bCan;
};

struct CMD_GPR_WealthBagScore
{
    DWORD dwUserID;
    int nScore;
};

struct CMD_GPO_WealthBagResult
{
    TCHAR szNickName[10][LEN_NICKNAME];
    int nScore[10];
    int nMyScore;
    int nMyRank;
    int nPlayerNum;
    SCORE lCurrentScore;
};

#define SUB_GPR_SHARE_URL           633     //分享域名
#define SUB_GPO_SHARE_URL           634     //接收分享域名

struct CMD_GPR_ShareURL
{
    char szURL[64];
    char szContent[256];
};

#define SUB_GPR_SHARE_URL2           641     //分享域名
#define SUB_GPO_SHARE_URL2           642     //接收分享域名

struct CMD_GPR_ShareURL2
{
    char szURL[64];
    char szContent[256];
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//登录命令
#define MDM_MB_LOGON				100									//广场登录

//登录模式
#define SUB_MB_LOGON_GAMEID			1									//I D 登录
#define SUB_MB_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_MB_REGISTER_ACCOUNTS	3									//注册帐号

//登录结果
#define SUB_MB_LOGON_SUCCESS		100									//登录成功
#define SUB_MB_LOGON_FAILURE		101									//登录失败

//升级提示
#define SUB_MB_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////

#define SUB_GP_LOGINING_INFO		201									//登录排队信息
#define SUB_GPR_CANCEL_LOGINING		7									//取消登录

struct CMD_GP_LoginingInfo
{
    WORD							wLoginingNum;						//正在登录的人数
};

//I D 登录
struct CMD_MB_LogonGameID
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	DWORD							dwGameID;							//游戏 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//帐号登录
struct CMD_MB_LogonAccounts
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//注册帐号
struct CMD_MB_RegisterAccounts
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//登录成功
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//登录失败
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//升级提示
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
};

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_MB_SERVER_LIST			101									//列表信息

//列表信息
#define SUB_MB_LIST_KIND			100									//种类列表
#define SUB_MB_LIST_SERVER			101									//房间列表
#define SUB_MB_LIST_FINISH			200									//列表完成

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//道具
#define MDM_GP_ITEM 1000

//查询道具数量
#define SUB_GPR_QUERY_ITEM_COUNT 1
#define SUB_GPO_QUERY_ITEM_COUNT 1001

struct CMD_GPR_QueryItemCount
{
    DWORD dwUserID;
};

struct CMD_GPO_QueryItemCount
{
    int nChuJiCount;
    int nZhongJiCount;
    int nGaoJiCount;
    int nTreasureMap;//藏宝图
    int nCannonStatus;//炮台状态
    int nJiaSu;//加速
    int nSanShe;//散射
    int nBaoJi;//暴击
};

//赠送道具
#define SUB_GPR_TRANSFER_ITEM 2
#define SUB_GPO_TRANSFER_ITEM 1002

struct CMD_GPR_TransferItem
{
    DWORD dwUserID;
    DWORD dwTargetGameID;
    int nItemID;
    int nItemCount;
    TCHAR szPassword[LEN_MD5];
};

struct CMD_GPO_TransferItem
{
    int nItemID;//赠送物品ID
    int nSourceCount;//原来物品数量
    int nVariationCount;//改变物品数量，与原来物品数量相加即为现在物品数量
    int nResultCode;//处理结果
    TCHAR szDescribeString[128];
};

//合成道具
#define SUB_GPR_COMBINE_ITEM 3
#define SUB_GPO_COMBINE_ITEM 1003

struct CMD_GPR_CombineItem
{
    DWORD dwUserID;
    int nCombineType;//1=合成小龙珠，2=合成大龙珠
};

struct CMD_GPO_CombineItem
{
    int nDragonPieceCount;//龙珠碎片数量
    int nSmallDragonCount;//小龙珠数量
    int nBigDragonCount;//大龙珠数量
    int nResultCode;//处理结果
    TCHAR szDescribeString[128];
};

//使用龙珠道具
#define SUB_GPR_USE_DRAGON 4
#define SUB_GPO_USE_DRAGON 1004

struct CMD_GPR_UseDragon
{
    DWORD dwUserID;
    int nDragonType;//1小龙珠，2大龙珠
};

struct CMD_GPO_UseDragon
{
    SCORE lCurrentScore;//当前分数
    int nChuJiCount;
    int nZhongJiCount;
    int nGaoJiCount;
    int nZhiZunCount;
    int nResultCode;//处理结果
    TCHAR szDescribeString[128];
};

//装备龙珠道具
#define SUB_GPR_EQUIPMENT_DRAGON 5
#define SUB_GPO_EQUIPMENT_DRAGON 1005

struct CMD_GPR_EquipmentDragon
{
    DWORD dwUserID;
    int nDragonType;//1小龙珠，2大龙珠
};

struct CMD_GPO_EquipmentDragon
{
    int nEquipmentDragonCount;//装备的结果
    int nChuJiCount;
    int nZhongJiCount;
    int nGaoJiCount;
    int nZhiZunCount;
    int nResultCode;//处理结果
    TCHAR szDescribeString[128];
};

//道具兑换
#define SUB_GPR_EXCHANGE_LOTTERY 6
#define SUB_GPO_EXCHANGE_LOTTERY 1006

struct CMD_GPR_ExchangeLottery
{
    DWORD dwUserID;
    int nDragonType;//0初级珍珠，1中级珍珠，2高级珍珠，3至尊珍珠
};

struct CMD_GPO_ExchangeLottery
{
    int nCurrentLottery;//当前奖券数量（兑换后）
    int nChuJiCount;
    int nZhongJiCount;
    int nGaoJiCount;
    int nZhiZunCount;
    int nResultCode;//处理结果
    TCHAR szDescribeString[128];
};

//道具
#define MDM_GP_SKILL 1001

#define SUB_GPR_QUERY_SKILL 1
#define SUB_GPO_QUERY_SKILL 1001

struct CMD_GPR_QuerySkill
{
    DWORD dwUserID;
};

struct CMD_GPO_QuerySkill
{
    int nCannonStatus;//炮台状态
    int nJiaSu;//加速
    int nSanshe;//散射
    int nBaoji;//暴击
    BYTE cbCount[3];//技能次数
};

#define SUB_GPR_NEW_GUID_QUERY 2
#define SUB_GPO_NEW_GUID_QUERY 1002

struct CMD_GPR_NewGuidQuery
{
    DWORD dwUserID;
};

struct CMD_GPO_NewGuidQuery
{
    bool bCan;
    
};

#define SUB_GPR_SUPER_GOLD  3
#define SUB_GPO_SUPER_GOLD  1003

struct CMD_GPR_SuperGold
{
    DWORD dwUserID;
};

struct CMD_GPO_SuperGold
{
    BYTE cbCan;
    int nSuperGold;
};

//喇叭功能
#define SUB_GPR_BROAD_LABA 435
#define SUB_GPO_BROAD_LABA 436

struct CMD_GPR_BroadLaba
{
    DWORD dwUserID;
    WORD wSize;
    char szNickName[64];
    char szContent[1024];
};

struct CMD_GPO_BroadLaba
{
    BYTE cbSuccess;
    SCORE lUserScore;
    char szContent[128];
};




#pragma pack()

#endif
