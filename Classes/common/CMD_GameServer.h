#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GR_LOGON				1									//登录信息

//登录模式
#define SUB_GR_LOGON_USERID			1									//I D 登录
#define SUB_GR_LOGON_MOBILE			2									//手机登录
#define SUB_GR_LOGON_ACCOUNTS		3									//帐户登录

//登录结果
#define SUB_GR_LOGON_SUCCESS		100									//登录成功
#define SUB_GR_LOGON_FAILURE		101									//登录失败
#define SUB_GR_LOGON_FINISH			102									//登录完成

//升级提示
#define SUB_GR_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////

//I D 登录
struct CMD_GR_LogonUserID
{
	//版本信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwProcessVersion;					//进程版本

	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	WORD							wKindID;							//类型索引
};

//手机登录
struct CMD_GR_LogonMobile
{
	//版本信息
	WORD							wGameID;							//游戏标识
	DWORD							dwProcessVersion;					//进程版本

	//桌子区域
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数

	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//帐号登录
struct CMD_GR_LogonAccounts
{
	//版本信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwProcessVersion;					//进程版本

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//登录成功
struct CMD_GR_LogonSuccess
{
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限
};

//登录失败
struct CMD_GR_LogonFailure
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//升级提示
struct CMD_GR_UpdateNotify
{
	//升级标志
	BYTE							cbMustUpdatePlaza;					//强行升级
	BYTE							cbMustUpdateClient;					//强行升级
	BYTE							cbAdviceUpdateClient;				//建议升级

	//当前版本
	DWORD							dwCurrentPlazaVersion;				//当前版本
	DWORD							dwCurrentFrameVersion;				//当前版本
	DWORD							dwCurrentClientVersion;				//当前版本
};

//假桌子
#define DTP_GP_AGENTINFO            1

struct DTP_GP_AgentInfo
{
    DWORD dwUserID;
    WORD wChairID;
    char szNameName[32];
};

//////////////////////////////////////////////////////////////////////////////////
//配置命令

#define MDM_GR_CONFIG				2									//配置信息

#define SUB_GR_CONFIG_COLUMN		100									//列表配置
#define SUB_GR_CONFIG_SERVER		101									//房间配置
#define SUB_GR_CONFIG_PROPERTY		102									//道具配置
#define SUB_GR_CONFIG_FINISH		103									//配置完成
#define SUB_GR_CONFIG_USER_RIGHT	104									//玩家权限

//////////////////////////////////////////////////////////////////////////////////

//列表配置
struct CMD_GR_ConfigColumn
{
	BYTE							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItem[MAX_COLUMN];				//列表描述
};

//房间配置
struct CMD_GR_ConfigServer
{
	//房间属性
	WORD							wTableCount;						//桌子数目
	WORD							wChairCount;						//椅子数目

	//房间配置
	WORD							wServerType;						//房间类型
	DWORD							dwServerRule;						//房间规则
};

//道具配置
struct CMD_GR_ConfigProperty
{
	BYTE							cbPropertyCount;					//道具数目
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//道具描述
};

//玩家权限
struct CMD_GR_ConfigUserRight
{
	DWORD							dwUserRight;						//玩家权限
};
//////////////////////////////////////////////////////////////////////////////////
//用户命令

#define MDM_GR_USER					3									//用户信息

//用户动作
#define SUB_GR_USER_RULE			1									//用户规则
#define SUB_GR_USER_LOOKON			2									//旁观请求
#define SUB_GR_USER_SITDOWN			3									//坐下请求
#define SUB_GR_USER_STANDUP			4									//起立请求
#define SUB_GR_USER_INVITE			5									//用户邀请
#define SUB_GR_USER_INVITE_REQ		6									//邀请请求
#define SUB_GR_USER_REPULSE_SIT  	7									//拒绝玩家坐下
#define SUB_GR_USER_KICK_USER       8                                   //踢出用户
#define SUB_GR_USER_INFO_REQ        9                                   //请求用户信息
#define SUB_GR_USER_CHAIR_REQ       10                                  //请求更换位置
#define SUB_GR_USER_CHAIR_INFO_REQ  11                                  //请求椅子用户信息
#define SUB_GR_USER_WAIT_DISTRIBUTE 12									//等待分配

//用户状态
#define SUB_GR_USER_ENTER			100									//用户进入
#define SUB_GR_USER_SCORE			101									//用户分数
#define SUB_GR_USER_STATUS			102									//用户状态
#define SUB_GR_REQUEST_FAILURE		103									//请求失败

//聊天命令
#define SUB_GR_USER_CHAT			201									//聊天消息
#define SUB_GR_USER_EXPRESSION		202									//表情消息
#define SUB_GR_WISPER_CHAT			203									//私聊消息
#define SUB_GR_WISPER_EXPRESSION	204									//私聊表情
#define SUB_GR_COLLOQUY_CHAT		205									//会话消息
#define SUB_GR_COLLOQUY_EXPRESSION	206									//会话表情

//道具命令
#define SUB_GR_PROPERTY_BUY			300									//购买道具
#define SUB_GR_PROPERTY_SUCCESS		301									//道具成功
#define SUB_GR_PROPERTY_FAILURE		302									//道具失败
#define SUB_GR_PROPERTY_MESSAGE     303                                 //道具消息
#define SUB_GR_PROPERTY_EFFECT      304                                 //道具效应
#define SUB_GR_PROPERTY_TRUMPET		305                                 //喇叭消息

//////////////////////////////////////////////////////////////////////////////////

//旁观请求
struct CMD_GR_UserLookon
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
};

//坐下请求
struct CMD_GR_UserSitDown
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	TCHAR							szPassword[LEN_PASSWORD];			//桌子密码
};

//起立请求
struct CMD_GR_UserStandUp
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	BYTE							cbForceLeave;						//强行离开
};

//邀请用户 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};

//邀请用户请求 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};

//用户分数
struct CMD_GR_UserScore
{
	DWORD							dwUserID;							//用户标识
	tagUserScore					UserScore;							//积分信息
};

//用户分数
struct CMD_GR_MobileUserScore
{
	DWORD							dwUserID;							//用户标识
	tagMobileUserScore				UserScore;							//积分信息
};

//用户状态
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//用户标识
	tagUserStatus					UserStatus;							//用户状态
};

//请求失败
struct CMD_GR_RequestFailure
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[256];				//描述信息
};


//用户聊天
struct CMD_GR_C_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户聊天
struct CMD_GR_S_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户表情
struct CMD_GR_C_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwTargetUserID;						//目标用户
};

//用户表情
struct CMD_GR_S_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
};

//用户私聊
struct CMD_GR_C_WisperChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户私聊
struct CMD_GR_S_WisperChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//私聊表情
struct CMD_GR_C_WisperExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwTargetUserID;						//目标用户
};

//私聊表情
struct CMD_GR_S_WisperExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
};

//用户会话
struct CMD_GR_ColloquyChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwConversationID;					//会话标识
	DWORD							dwTargetUserID[16];					//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//邀请用户
struct CMD_GR_C_InviteUser
{
	WORD							wTableID;							//桌子号码
	DWORD							dwSendUserID;						//发送用户
};

//邀请用户
struct CMD_GR_S_InviteUser
{
	DWORD							dwTargetUserID;						//目标用户
};

//购买道具
struct CMD_GR_C_PropertyBuy
{
	BYTE                            cbRequestArea;						//请求范围
	BYTE							cbConsumeScore;						//积分消费
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引	
	DWORD							dwTargetUserID;						//使用对象
};

//道具成功
struct CMD_GR_S_PropertySuccess
{
	BYTE                            cbRequestArea;						//使用环境
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwSourceUserID;						//目标对象
	DWORD							dwTargetUserID;						//使用对象
};

//道具失败
struct CMD_GR_PropertyFailure
{
	WORD                            wRequestArea;                       //请求区域
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[256];				//描述信息
};

//道具消息
struct CMD_GR_S_PropertyMessage
{
	//道具信息
	WORD                            wPropertyIndex;                     //道具索引
	WORD                            wPropertyCount;                     //道具数目
	DWORD                           dwSourceUserID;                     //目标对象
	DWORD                           dwTargerUserID;                     //使用对象
};


//道具效应
struct CMD_GR_S_PropertyEffect
{
	DWORD                           wUserID;					        //用 户I D
	BYTE							cbMemberOrder;						//会员等级
};

//发送喇叭
struct CMD_GR_C_SendTrumpet
{
	BYTE                           cbRequestArea;                        //请求范围 
	WORD                           wPropertyIndex;                      //道具索引 
	DWORD                          TrumpetColor;                        //喇叭颜色
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //喇叭内容
};

//发送喇叭
struct CMD_GR_S_SendTrumpet
{
	WORD                           wPropertyIndex;                      //道具索引 
	DWORD                          dwSendUserID;                         //用户 I D
	DWORD                          TrumpetColor;                        //喇叭颜色
	TCHAR                          szSendNickName[32];				    //玩家昵称 
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //喇叭内容
};

#define SUB_GR_VIPUPGRADE           309

//VIP公告
struct CMD_CS_C_VIPUpgrade
{
    BYTE        cbType;//10为系统公告，20为奖券公告，30为VIP公告
    TCHAR       szTrumpetContent[256];
};


//用户拒绝黑名单坐下
struct CMD_GR_UserRepulseSit
{
	WORD							wTableID;							//桌子号码
	WORD							wChairID;							//椅子位置
	DWORD							dwUserID;							//用户 I D
	DWORD							dwRepulseUserID;					//用户 I D
};

//////////////////////////////////////////////////////////////////////////////////

//规则标志
#define UR_LIMIT_SAME_IP			0x01								//限制地址
#define UR_LIMIT_WIN_RATE			0x02								//限制胜率
#define UR_LIMIT_FLEE_RATE			0x04								//限制逃率
#define UR_LIMIT_GAME_SCORE			0x08								//限制积分

//用户规则
struct CMD_GR_UserRule
{
	BYTE							cbRuleMask;							//规则掩码
	WORD							wMinWinRate;						//最低胜率
	WORD							wMaxFleeRate;						//最高逃率
	LONG							lMaxGameScore;						//最高分数 
	LONG							lMinGameScore;						//最低分数
};

//请求用户信息
struct CMD_GR_UserInfoReq
{
	DWORD                           dwUserIDReq;                        //请求用户
	WORD							wTablePos;							//桌子位置
};

//请求用户信息
struct CMD_GR_ChairUserInfoReq
{
	WORD							wTableID;							//桌子号码
	WORD							wChairID;							//椅子位置
};
//////////////////////////////////////////////////////////////////////////////////
//状态命令

#define MDM_GR_STATUS				4									//状态信息

#define SUB_GR_TABLE_INFO			100									//桌子信息
#define SUB_GR_TABLE_STATUS			101									//桌子状态

//////////////////////////////////////////////////////////////////////////////////

//桌子信息
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//桌子数目
	tagTableStatus					TableStatusArray[512];				//桌子状态
};

//桌子状态
struct CMD_GR_TableStatus
{
	WORD							wTableID;							//桌子号码
	tagTableStatus					TableStatus;						//桌子状态
};

//////////////////////////////////////////////////////////////////////////////////
//银行命令

#define MDM_GR_INSURE				5									//用户信息

//银行命令
#define SUB_GR_QUERY_INSURE_INFO	1									//查询银行
#define SUB_GR_SAVE_SCORE_REQUEST	2									//存款操作
#define SUB_GR_TAKE_SCORE_REQUEST	3									//取款操作
#define SUB_GR_TRANSFER_SCORE_REQUEST	4								//取款操作
#define SUB_GR_QUERY_USER_INFO_REQUEST	5								//查询用户

#define SUB_GR_USER_INSURE_INFO		100									//银行资料
#define SUB_GR_USER_INSURE_SUCCESS	101									//银行成功
#define SUB_GR_USER_INSURE_FAILURE	102									//银行失败
#define SUB_GR_USER_TRANSFER_USER_INFO	103								//用户资料
#define SUB_GR_USER_TRANSFER_INFO	105

#define SUB_GO_USER_TRANSRER_DRAGON 106                                 //赠送龙珠
#define SUB_GPO_USER_GET_DRAGON     108                                 //获得龙珠


//////////////////////////////////////////////////////////////////////////////////

//查询银行
struct CMD_GR_C_QueryInsureInfoRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
};

//存款请求
struct CMD_GR_C_SaveScoreRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	SCORE							lSaveScore;							//存款数目
};

//取款请求
struct CMD_GR_C_TakeScoreRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	SCORE							lTakeScore;							//取款数目
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
};

//转账金币
struct CMD_GP_C_TransferScoreRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账金币
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
};

//查询用户
struct CMD_GR_C_QueryUserInfoRequest
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//银行资料
struct CMD_GR_S_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	SCORE							lTransferPrerequisite;				//转账条件
};

//银行成功
struct CMD_GR_S_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //游戏动作
	SCORE							lUserScore;							//身上金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//赠送信息
struct DBO_GR_UserInsureTransferInfo
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
struct CMD_GR_S_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //游戏动作
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//用户信息
struct CMD_GR_S_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwTargetGameID;						//目标用户
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//在游戏中获得龙珠
struct CMD_GR_UserWinDragon
{
    SCORE lWinDragon;
    SCORE lWinScore;
};

//////////////////////////////////////////////////////////////////////////////////
//管理命令

#define MDM_GR_MANAGE				6									//管理命令

#define SUB_GR_SEND_WARNING			1									//发送警告
#define SUB_GR_SEND_MESSAGE			2									//发送消息
#define SUB_GR_LOOK_USER_IP			3									//查看地址
#define SUB_GR_KILL_USER			4									//踢出用户
#define SUB_GR_LIMIT_ACCOUNS		5									//禁用帐户
#define SUB_GR_SET_USER_RIGHT		6									//权限设置

//房间设置
#define SUB_GR_QUERY_OPTION			7									//查询设置
#define SUB_GR_OPTION_SERVER		8									//房间设置
#define SUB_GR_OPTION_CURRENT		9									//当前设置

#define SUB_GR_LIMIT_USER_CHAT		10									//限制聊天

#define SUB_GR_KICK_ALL_USER		11									//踢出用户
#define SUB_GR_DISMISSGAME		    12									//解散游戏

#define SUB_GR_CLOWN_FAKEANNOUNCEMENT	13								//小丑假公告
#define SUB_GR_CLOWN_BIG_REWARD		14									//巨奖
//////////////////////////////////////////////////////////////////////////////////

//小丑假公告
struct CMD_GR_CLOWNFAKEANNOUNCE
{
    TCHAR	szClownFakeAnnounceMent[256];		//踢出提示
};
//小丑巨奖
struct CMD_GR_CLOWNBIGREWARD
{
    LONGLONG lBigReward;
};
//发送警告
struct CMD_GR_SendWarning
{
	WORD							wChatLength;						//信息长度
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szWarningMessage[LEN_USER_CHAT];	//警告消息
};

//系统消息
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//游戏消息
	BYTE							cbRoom;								//游戏消息
	BYTE							cbAllRoom;							//游戏消息
	WORD							wChatLength;						//信息长度
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};

//查看地址
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//目标用户
};

//踢出用户
struct CMD_GR_KickUser
{
	DWORD							dwTargetUserID;						//目标用户
};

//禁用帐户
struct CMD_GR_LimitAccounts
{
	DWORD							dwTargetUserID;						//目标用户
};

//权限设置
struct CMD_GR_SetUserRight
{
	//目标用户
	DWORD							dwTargetUserID;						//目标用户

	//绑定变量
	BYTE							cbGameRight;						//帐号权限
	BYTE							cbAccountsRight;					//帐号权限

	//权限变化
	BYTE							cbLimitRoomChat;					//大厅聊天
	BYTE							cbLimitGameChat;					//游戏聊天
	BYTE							cbLimitPlayGame;					//游戏权限
	BYTE							cbLimitSendWisper;					//发送消息
	BYTE							cbLimitLookonGame;					//旁观权限
};

//房间设置
struct CMD_GR_OptionCurrent
{
	DWORD							dwRuleMask;							//规则掩码
	tagServerOptionInfo				ServerOptionInfo;					//房间配置
};

//房间设置
struct CMD_GR_ServerOption
{
	tagServerOptionInfo				ServerOptionInfo;					//房间配置
};

//踢出所有用户
struct CMD_GR_KickAllUser
{
	TCHAR							szKickMessage[LEN_USER_CHAT];		//踢出提示
};

//解散游戏
struct CMD_GR_DismissGame
{
	WORD							wDismissTableNum;		            //解散桌号
};
//////////////////////////////////////////////////////////////////////////////////

//设置标志
#define OSF_ROOM_CHAT				1									//大厅聊天
#define OSF_GAME_CHAT				2									//游戏聊天
#define OSF_ROOM_WISPER				3									//大厅私聊
#define OSF_ENTER_TABLE				4									//进入游戏
#define OSF_ENTER_SERVER			5									//进入房间
#define OSF_SEND_BUGLE				12									//发送喇叭

//房间设置
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//设置标志
	BYTE							cbOptionValue;						//设置标志
};

//限制聊天
struct CMD_GR_LimitUserChat
{
	DWORD							dwTargetUserID;						//目标用户
	BYTE							cbLimitFlags;						//限制标志
	BYTE							cbLimitValue;						//限制与否
};
//////////////////////////////////////////////////////////////////////////////////
//比赛命令

#define MDM_GR_MATCH				7									//比赛命令

#define SUB_GR_MATCH_FEE			400									//报名费用
#define SUB_GR_MATCH_NUM			401									//等待人数
#define SUB_GR_LEAVE_MATCH			402									//退出比赛
#define SUB_GR_MATCH_INFO			403									//比赛信息
#define SUB_GR_MATCH_WAIT_TIP		404									//等待提示
#define SUB_GR_MATCH_RESULT			405									//比赛结果
#define SUB_GR_MATCH_STATUS			406									//比赛状态
#define SUB_GR_MATCH_DESC			408									//比赛描述

////////////////改动以下时 请将游戏里面CMD_GAME.H的同时改动////////////////////////////
#define SUB_GR_MATCH_INFO_ER_SPARROWS	410									//比赛信息(2人麻将)
//赛事信息（2人麻将特有）
struct CMD_GR_Match_ER_Sparrows
{
	WORD							wALLGameRound;						//总共游戏轮
	WORD							wALLGameRoundFan;					//总共游戏翻
	WORD							wCurGameRound;						//当前游戏轮
	WORD							wCurGameRoundFan;					//当前游戏番
	WORD							wLocalGameFan;						//本地游戏番
	WORD							wOtherGameFan;						//对家游戏番
	WORD							wNextGameRoundFan;					//下轮游戏番
};

//比赛人数
struct CMD_GR_Match_Num
{
	DWORD							dwWaitting;							//等待人数
	DWORD							dwTotal;							//开赛人数
};

//赛事信息
struct CMD_GR_Match_Info
{
	TCHAR							szTitle[4][64];						//信息标题
    WORD							wGameCount;							//游戏局数
};

//提示信息
struct CMD_GR_Match_Wait_Tip
{
	SCORE							lScore;								//当前积分
	WORD							wRank;								//当前名次
	WORD							wCurTableRank;						//本桌名次
	WORD							wUserCount;							//当前人数
	WORD							wPlayingTable;						//游戏桌数
	TCHAR							szMatchName[LEN_SERVER];			//比赛名称
};

//比赛结果
struct CMD_GR_MatchResult
{
	TCHAR							szDescribe[256];					//得奖描述
	DWORD							dwGold;								//金币奖励
	DWORD							dwMedal;							//奖牌奖励
	DWORD							dwExperience;						//经验奖励
};

#define MAX_MATCH_DESC				4									//最多描述
//比赛描述
struct CMD_GR_MatchDesc
{
	TCHAR							szTitle[MAX_MATCH_DESC][16];		//信息标题
	TCHAR							szDescribe[MAX_MATCH_DESC][64];		//描述内容
	COLORREF						crTitleColor;						//标题颜色
	COLORREF						crDescribeColor;					//描述颜色
};

//////////////////////////////////////////////////////////////////////////////////
//框架命令

#define MDM_GF_FRAME				100									//框架命令

//////////////////////////////////////////////////////////////////////////////////
//框架命令

//用户命令
#define SUB_GF_GAME_OPTION			1									//游戏配置
#define SUB_GF_USER_READY			2									//用户准备
#define SUB_GF_LOOKON_CONFIG		3									//旁观配置

//聊天命令
#define SUB_GF_USER_CHAT			10									//用户聊天
#define SUB_GF_USER_EXPRESSION		11									//用户表情

//游戏信息
#define SUB_GF_GAME_STATUS			100									//游戏状态
#define SUB_GF_GAME_SCENE			101									//游戏场景
#define SUB_GF_LOOKON_STATUS		102									//旁观状态

//系统消息
#define SUB_GF_SYSTEM_MESSAGE		200									//系统消息
#define SUB_GF_ACTION_MESSAGE		201									//动作消息

//////////////////////////////////////////////////////////////////////////////////

//游戏配置
struct CMD_GF_GameOption
{
	BYTE							cbAllowLookon;						//旁观标志
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwClientVersion;					//游戏版本
};

//旁观配置
struct CMD_GF_LookonConfig
{
	DWORD							dwUserID;							//用户标识
	BYTE							cbAllowLookon;						//允许旁观
};

//旁观状态
struct CMD_GF_LookonStatus
{
	BYTE							cbAllowLookon;						//允许旁观
};

//游戏环境
struct CMD_GF_GameStatus
{
	BYTE							cbGameStatus;						//游戏状态
	BYTE							cbAllowLookon;						//旁观标志
};

//用户聊天
struct CMD_GF_C_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户聊天
struct CMD_GF_S_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户表情
struct CMD_GF_C_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwTargetUserID;						//目标用户
};

//用户表情
struct CMD_GF_S_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
};

//////////////////////////////////////////////////////////////////////////////////
//游戏命令

#define MDM_GF_GAME					200									//游戏命令

//////////////////////////////////////////////////////////////////////////////////
//携带信息

//其他信息
#define DTP_GR_TABLE_PASSWORD		1									//桌子密码

//用户属性
#define DTP_GR_NICK_NAME			10									//用户昵称
#define DTP_GR_GROUP_NAME			11									//社团名字
#define DTP_GR_UNDER_WRITE			12									//个性签名

//附加信息
#define DTP_GR_USER_NOTE			20									//用户备注
#define DTP_GR_CUSTOM_FACE			21									//自定头像

//////////////////////////////////////////////////////////////////////////////////

//请求错误
#define REQUEST_FAILURE_NORMAL		0									//常规原因
#define REQUEST_FAILURE_NOGOLD		1									//金币不足
#define REQUEST_FAILURE_NOSCORE		2									//积分不足
#define REQUEST_FAILURE_PASSWORD	3									//密码错误

//////////////////////////////////////////////////////////////////////////////////

//技能
#define MDM_GR_SKILL 1001


#define SUB_GRR_QUERY_SKILL 1
#define SUB_GRO_QUERY_SKILL 1001

struct CMD_GRR_QuerySkill
{
    DWORD dwUserID;
};

struct CMD_GRO_QuerySkill
{
    int nCannonStatus;//炮台状态
    int nJiaSu;//加速
    int nSanshe;//散射
    int nBaoji;//暴击
    BYTE cbCount[3];//技能次数
    BYTE cbPrise[7];//炮台价格
};

#define SUB_GRR_USE_SKILL 2
#define SUB_GRO_USE_SKILL 1002

struct CMD_GRR_UseSkill
{
    BYTE cbSkillID;
};

struct CMD_GRO_UseSkill
{
    BYTE nSkillID;//0加速，1散射，2暴击
    int nCount;
};

#define SUB_GRO_GIVE_SKILL 1003

struct CMD_GRO_GiveSkill
{
    BYTE nSkillID;//0加速，1散射，2暴击
    int nCount;
};

//小丑巨奖主动获取
#define MDM_GR_GET_BIG_REWARD 1002
#define SUB_GRR_GET_BIG_REWARD 1

#define SUB_GR_FIRST_SUPERREWARD	311									//第一次进超级海盗房间奖励金币

//比赛信息
#define SUB_GR_Match_SIGNUP			312									//比赛报名
#define SUB_GR_MATCH_SIGNUP_RES		313									//比赛报名结果
#define SUB_GR_MATCH_BEGIN			314									//比赛开始
#define SUB_GR_MATCH_END			315									//比赛结束
#define SUB_GR_MATCH_SCORE_UPDATE	316									//分数定时更新
#define SUB_GR_MATCH_SCORE_QUERY    317									//用户主动查询分数
#define SUB_GR_MATCH_SCORE_QUERY_RES     318							//查询分数结果
#define SUB_GR_MATCH_GET_MATCH_PRIZE     319							//用户领取奖励
#define SUB_GR_MATCH_GET_MATCH_PRIZE_RES 320							//用户领取奖励结果
#define SUB_GR_MATCH_INFO_NOTIFY		 321							//比赛倒计时 比赛信息
#define SUB_GR_MATCH_TIME_NOTIFY		 322							//比赛倒计时倒计时
#define SUB_GR_MATCH_SHAREINFO           323
#define SUB_GR_MATCH_SHAREINFO_RES       324

struct CMD_GR_MatchShareInfo
{
    DWORD dwUserID;
    int nRank;
    int nMatchID;
    char szMatchDate[16];
};

struct CMD_GR_MatchShareInfoRes
{
    char szUrl[64];
    char szContent[256];
};

//比赛消息
struct CMD_GRO_MatchInfoNotify                          //比赛倒计时 比赛信息
{
    int nMatchPeopleNum;//参赛人数
    int nMatchScore;//报名费
    int nMatchID;//比赛场次
    bool bMatchStatus;//报名状态，0为未报名，1为已报名
    char szMatchTitle[128];//比赛名称
    char szMatchPrise[128];//比赛奖励说明
};

struct CMD_GRR_MatchJoin                                //报名比赛
{
    int nMatchID;//比赛场次ID
    DWORD dwUserID;
};

struct CMD_GRO_MatchSignupRes                           //比赛报名结果
{
    bool bMatchResult;//报名结果，0为报名失败，1为报名成功
    bool bMatchStatus;//报名状态，0为未报名，1为已经报名
    int nMatchID;//比赛场次
    SCORE llUserScore;//玩家当前金币
    char szDescription[128];//描述信息
};

struct CMD_GPO_MatchTimeNotify                          //比赛倒计时倒计时
{
    BYTE cbMatchStatus;//0代表比赛之前倒计时，1代表比赛结束倒计时
    int nSecond;//倒计时提示时间
    char szMatchTitle[128];//比赛标题
};

struct CMD_GRO_MatchBegin                               //比赛开始
{
    int nMatchID;//比赛场次
    char szMatchDate[16];//比赛日期
    char szMatchTitle[128];//比赛标题
};

struct CMD_GRO_MatchGroupInfo//个人积分信息
{
    SCORE lScore;//积分
    char szNickName[LEN_NICKNAME];//昵称
};

struct CMD_GRO_MatchScoreQueryRes                       //分数定时更新
{
    int nMatchNum;//参数人数
    int nSecond;//比赛倒计时，以秒为单位
    SCORE lUserScore;//比赛积分
    int nRanking;//比赛排名
    char szMatchTitle[128];
    CMD_GRO_MatchGroupInfo MatchGroupInfo[0];
};

struct CMD_GRR_MatchEnd                                //比赛结束
{
    BYTE cbShare;//是否需要分享  0为不需要分享，1为需要分享, 2为无奖励
    int nMatchID;
    int nRanking;//排名
    SCORE nScore;//积分
    char szMatchPrise[64];//比赛奖励
    char szMatchDate[64];
};

struct CMD_GRO_MatchGetMatchPrise                       //用户领取奖励
{
    int nMatchID;
    DWORD dwUserID;
    char szMatchDate[16];
};

struct CMD_GRR_MatchGetMatchPriseRes                    //用户领取奖励结果
{
    bool bPriseStatus;//领取状态，0为领取失败，1为领取成功
    BYTE cbPriseType;//比赛奖励类型
    int nPriseCount;//领取数量
    SCORE llUserScore;//玩家当前金币数量
    char szDescription[128];
    int nMatchID;
};

//游戏内金币购买技能
#define SUB_GR_BUY_SKILL           325
#define SUB_GR_BUY_SKILL_RES       326

struct CMD_GR_BuySkill
{
    DWORD dwUserID;
    BYTE cbSkillID;
    int nCount;
};

struct CMD_GR_BuySkill_Result
{
    bool bSuccess;
    BYTE cbSkillID;
    int nCount;
    SCORE llScore;
    TCHAR szDescribeString[128];
};

//游戏内发送喇叭
#define SUB_GR_USE_LABA             327
#define SUB_GR_USE_LABA_RES         328

struct CMD_GR_BroadLaba
{
    DWORD dwUserID;
    WORD wSize;
    char szNickName[64];
    char szContent[1024];
};

struct CMD_GR_BroadLabaRes
{
    BYTE cbSuccess;
    SCORE lUserScore;
    char szContent[128];
};

#define SUB_GRR_QUERYBANKCUPTINFO   329
#define SUB_GRO_QUERYBANKCUPTINFO   330
struct CMD_GRO_QueryBankcruptInfo                    //用户领取奖励结果
{
    int nLastTime;
    int nScore;
    char szDescription[128];
};

#define SUB_GRR_GET_BANKCRUPT_SCORE		331
#define SUB_GRO_GET_BANKCRUPT_SCORE		332
struct CMD_GRO_GetBankcruptScore
{
    bool bSuccess;
    int nCurrentScore;
    int nAlreadyTime;
    char szDescription[128];
};

#define SUB_GRR_HAVE_MATCH_PRIZE_REMIND   333
#define SUB_GRO_HAVE_MATCH_PRIZE_REMIND   334

struct CMD_GRO_HavaPrizeRemind
{
    char szDescription[128];
};

#define SUB_GRR_CAN_MATCH_SHARE             335
#define SUB_GRO_CAN_MATCH_SHARE             336

struct CMD_GRR_CanMatchShare
{
    DWORD dwUserID;
    int nRank;
    int nMatchID;
    char szMatchDate[16];
};

struct CMD_GRO_CanMatchShare
{
    BYTE cbShare;
};

//游戏内收到红包
#define SUB_GRO_QIANG_RESULT                337
struct CMD_GRO_QiangResult
{
    bool bSuccess;
    int nScore;
    int nCurrentScore;
    char szContent1[64];
    char szContent2[128];
};

#define SUB_GRO_ONLINE_REWARD_INFO          339

//各个玩家在线奖励信息
struct CMD_GRO_OneOnlineRewardInfo
{
    BYTE cbStatus;//状态，0为未到时间、1为可领取、2为已领取
    int nSecond;//时间，单位为秒
    int nScore;//金币
};
//在线奖励信息汇总
struct CMD_GRO_OnlineRewardInfo
{
    int nCount;
    CMD_GRO_OneOnlineRewardInfo OneOnlineRewardInfo[0];
};

#define SUB_GRO_RECEIVE_ONLINE_REWARD       340

struct CMD_GRO_ReceiveOnlineReward
{
    bool bSuccess;//领取结果
    int nNextSecond;//下一次倒计时，如果为-1，则领取完毕
    int nScore;//本次领取金币数
    int nCurrentScore;//玩家当前拥有金币数
    char szDescription[128];
};

#pragma pack()

#endif
