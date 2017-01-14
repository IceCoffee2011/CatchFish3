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

#define DTP_GP_IOS2PASSED           32
#define DTP_GP_IOS2HEADBUG          33
#define DTP_GP_IOS2HTTPBUG          34

struct DTP_GP_IOS2HttpBug
{
    BYTE cbPassed;
};

struct DTP_GP_IOS2HeadBug
{
    BYTE cbPassed;
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
    DWORD dwGiftScore;
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
    DWORD							dwRefereeID;							//推荐人ID
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


//////////////////////////////////////////////////////////////////////////
//签到信息查询
struct CMD_GP_QuerySigninInfo
{
	DWORD dwUserID;
};

//签到查询每天信息
struct tagSignInDayInfo
{
	WORD  wDayNum;														//签到天数
	SCORE llPrize;														//签到奖励
};

struct CMD_GP_UserSigninInfo
{
    WORD                            wSignInDay;                         //该第几天签到（1-7）
	SCORE							llExtraPrize;						//额外奖励
	SCORE							llBQCost;							//补签花费
	tagSignInDayInfo                SignInDay[7];						//每天信息
};

//点击签到操作
struct CMD_GP_SignIn
{
	DWORD							dwUserID;							//用户ID
	WORD							wBQDay;								//补签天数, 0代表不补签
};

//点击签到结果
struct CMD_GP_SignIn_Result
{
	DWORD                           bIsSuccess;                         //签到是否成功
	DWORD                           bIsAllDaySigned;                    //七天是否签满
	TCHAR							szErrorDescribe[128];				//结果描述
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

//用户签到
#define SUB_GP_QUERY_SIGNIN_INFO			412							//查询签到
#define SUB_GP_QUERY_SIGNIN_INFO_RESULT		413							//签到信息
#define SUB_GP_USER_SIGNIN_REQ				414							//点击签到
#define SUB_GP_USER_SIGNIN_RESULT			415							//签到结果

//用户抽奖
#define SUB_GP_QUERY_LOTTERY_INFO			416							//查询抽奖
#define SUB_GP_QUERY_LOTTERY_INFO_RESULT	417							//查询结果
#define SUB_GP_USER_LOTTERY					418							//用户抽奖
#define SUB_GP_USER_LOTTERY_RESULT			419							//用户抽奖
#define SUB_GP_LOTTERY_FINISH				420							//抽奖结束
#define SUB_GP_LOTTERY_MSG					421							//抽奖消息

#define SUB_GP_QUERY_RECHARGE_INFO			422							//充值信息
#define SUB_GP_QUERY_RECHARGE_INFO_RES		423							//充值信息


#define SUB_GPR_QUERY_MAIL_ID_LIST			500
#define SUB_GPO_QUERY_MAIL_ID_LIST			501
#define SUB_GPR_QUERY_MAIL_INFO				502							//获取邮件信息
#define SUB_GPO_QUERY_MAIL_INFO				503
#define SUB_GPR_QUERY_MAIL_CONTENT			504
#define SUB_GPO_QUERY_MAIL_CONTENT			505
#define SUB_GPR_QUERY_MAIL_ITEM             506
#define SUB_GPO_QUERY_MAIL_ITEM             507

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
    WORD wHeapLevel;//钱堆级别
    DWORD dwGift;//赠送金币
    char szPruductName[16];//金币
    char szProductID[32];//商品ID
};

struct CMD_GPR_IAPProductList
{
    CMD_GPR_IAPProduct product[8];
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

#pragma pack()

#endif