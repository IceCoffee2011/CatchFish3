#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE
#pragma pack(1)
/////////////////////////////////////////////////////////////////////////////////
//服务定义
/*
#define KIND_0

#if defined(KIND_0)
//游戏属性
#define KIND_ID						2000
#define GAME_NAME					TEXT("保卫钓鱼岛")
#define CLIENT_EXE_NAME				TEXT("CatchFish01.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish01.dll")
#define DIRPATH1 "Music/"
#define DIRPATH2 "CatchFish01/"
#define DIRPATH3 "CatchFish01/DYD/"
#define PASSWORD "znsimple@##$$FRR((*&!!@#$%35@12,,1"
#define FishCount						30
const int FishID[FishCount] = {0,1,2,3,4,25,5,6,7,8,9,10,11,12,13,14,15,16,21,20,27,24,26,22,23,29,39,40,41,42};
#elif defined(KIND_1)
//游戏属性
#define KIND_ID						2001
#define GAME_NAME					TEXT("金蟾捕鱼")
#define CLIENT_EXE_NAME				TEXT("CatchFish02.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish02.dll")
#define DIRPATH1 "Music/"
#define DIRPATH2 "CatchFish02/"
#define DIRPATH3 "CatchFish01/JCBY/"
#define PASSWORD "znsimple@##$$FRR((*&!!@#$%35@12,,1"
#define FishCount						36
const int FishID[FishCount] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,30,31,32,33,34,35,36,37,17,18,19,20,24,25,26,21,22,23,40};
#elif defined(KIND_2)
//游戏属性
#define KIND_ID						2002
#define GAME_NAME					TEXT("倚天屠龙")
#define CLIENT_EXE_NAME				TEXT("CatchFish03.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish03.dll")
#define DIRPATH1 "Music/"
#define DIRPATH2 "CatchFish03/"
#define DIRPATH3 "CatchFish01/YTTL/"
#define PASSWORD "znsimple@##$$FRR((*&!!@#$%35@12,,1"
#define FishCount						31
const int FishID[FishCount] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,18,19,17,30,31,32,24,25,26,20,21,22,23,40};
#elif defined(KIND_3)
//游戏属性
#define KIND_ID						2003
#define GAME_NAME					TEXT("哪吒闹海")
#define CLIENT_EXE_NAME				TEXT("CatchFish04.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish04.dll")
#define DIRPATH1 "Music/"
#define DIRPATH2 "CatchFish04/Res.zip"
#define PASSWORD "znsimple@##$$FRR((*&!!@#$%35@12,,1"
#elif defined(KIND_4)
//游戏属性
#define KIND_ID						2004
#define GAME_NAME					TEXT("西游争霸")
#define CLIENT_EXE_NAME				TEXT("CatchFish05.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish05.dll")
#define DIRPATH1 "Music/"
#define DIRPATH2 "CatchFish05/"
#define PASSWORD "znsimple@##$$FRR((*&!!@#$%35@12,,1"
#define FishCount						36
const int FishID[FishCount] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,30,31,32,33,34,35,36,37,18,19,26,25,24,27,17,21,22,23,40};
#elif defined(KIND_5)
//游戏属性
#define KIND_ID						2005
#define GAME_NAME					TEXT("渔王争霸")
#define CLIENT_EXE_NAME				TEXT("CatchFish06.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish06.dll")
#define DIRPATH1 "Music/"
#define DIRPATH2 "CatchFish06/"
#define PASSWORD "znsimple@##$$FRR((*&!!@#$%35@12,,1"
#define FishCount						35
const int FishID[FishCount] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,30,31,32,33,34,35,36,37,17,18,19,24,25,20,21,22,23,40};
#endif
*/
//组件属性
//#define GAME_PLAYER					6									//游戏人数
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+1					//游戏进行

#define PATH_POINT_COUNT			5									//路径使用的坐标点数量

#define MAX_HITFISH 100

//命令定义
enum enServerSubCmd
{
	enServerSubCmd_Begin=59,
	SUB_S_GAME_SCENE,									//场景消息
	SUB_S_TRACE_POINT,									//轨迹坐标
	SUB_S_USER_SHOOT,									//发射炮弹
	SUB_S_CAPTURE_FISH,									//捕获鱼群
	SUB_S_BULLET_COUNT,									//子弹数目
	SUB_S_COIN_COUNT,									//金币数目
	SUB_S_FISH_COUNT,									//鱼群数目
	SUB_S_CHANGE_SCENE,									//切换场景
	SUB_S_SCORE_INFO,									//分数信息
	SUB_S_SET_CELL_SCORE,								//单元积分
	SUB_S_BONUS_INFO,									//彩金信息
	SUB_S_GAME_MESSAGE,									//游戏消息
	SUB_S_LOCKFISH,
	SUB_S_UPDATE_BOSS_SCORE,
	SUB_S_JUMP_BOMB,
	SUB_S_TASK_INFO,
	SUB_S_TIME_PRIZE_INFO,
	SUB_S_TIME_PRIZE_RECEIVED,
	SUB_S_TASK_RECEIVED,
    SUB_S_MOBILE_MONEY,
    SUB_S_MOBILE_MONEY_SUCCESS,
    SUB_S_RETURN_MOBILE_MONEY,
    SUB_S_MOBILE_CHARGE,
    SUB_S_EQUIPMENT_DRAGON_COUNT,//龙珠数量
    SUB_S_TRY_CANNON,
    SUB_S_TRY_CANNON_END,
    SUB_S_NEW_GUID_GIFT=86,
    SUB_S_TRY_CANNON_CHECK=87,
    SUB_S_WIN_PEARL=88,
    SUB_S_MATCH_RANK=89,
    SUB_S_SEND_PEARL_INFO,
    SUB_S_EXCHANGE_PEARL,
    SUB_S_FIRST_MOBILE_MONEY,
    SUB_S_SCORE_FULL,//用户分满了
    SUB_S_BOSS_COMING,//boss来袭
    SUB_S_VIP_LEVEL=95,
    SUB_S_BOX_PRIZE,
    SUB_S_MAN_EATING_FISH,
    SUB_S_MAN_EATING_FISH_NEW_FISH,
    SUB_S_EAT_SCORE,
    SUB_S_MAN_EATING_FISH_FINISH,
    SUB_S_EAT_3RD,
    SUB_S_MAN_EATING_BOSS_DIE=102,
    SUB_S_MAN_EATING_FISH_SPEAK=103,
    SUB_S_ADD_BOX_RES=104,
    SUB_S_USE_PEARL=105,
    SUB_S_BAO_JI,
    SUB_S_BAO_JI_MESSAGE,
    SUB_S_SUPER_CANNON=108,
    SUB_S_USER_LUCKY=109,
    SUB_S_USER_LUCKY2,
    SUB_S_CHAT,
    SUB_S_PPL2_START,//拍拍乐开始（原版，现在已不用）
    SUB_S_PPL2_RESULT,
    SUB_S_PPL2_END,
    SUB_S_PPL2_DEFAULT,
    SUB_S_PPL2_CANCEL=116,
    SUB_S_PPL_START,//拍拍乐开始（新版，可适用于一个桌子多个人）
    SUB_S_PPL_RESULT,//拍拍乐过程
    SUB_S_PPL_END,//拍拍乐结束
    SUB_S_PPL_DEFAULT,//拍拍乐默认开始
    SUB_S_PPL_CANCEL=121,//拍拍乐取消
    SUB_S_ONLINE_REWARD_TIME,//返回在线时长
    SUB_S_ONLINE_REWARD_TIME_OVER,//在线倒计时结束
    enServerSubCmd_End
};

struct CMD_S_OnlineRewardTime
{
    int nSecond;//如果倒计时为0，那么有奖要领。如果所有奖都已领取的话，返回-1。
};

struct CMD_S_PPLStart
{
    bool bChairID[4];
    int nCountDown;
    int nDefaultStartTime;
};

struct CMD_S_PPLResult
{
    WORD wChairID;
    int nScore;
    int nTotalScore;
};

struct CMD_S_PPLCancel
{
    WORD wChairID;
};

struct CMD_S_Chat
{
    WORD wChairID;
    char szNickName[LEN_NICKNAME];
    char szContent[128];
};

struct CMD_S_ServerData
{
    float fLucky;
    int nStorage;
};

struct CMD_S_SuperCannon
{
    WORD wChairID;
    bool bSuperCannnon;
};

struct CMD_S_BaoJiMessage
{
    char szContent[128];
};

struct CMD_S_UsePearl
{
    int nScore;
    int nCurrentScore;
};

struct CMD_S_AddBoxRes
{
    LONG lUserAllScore;
};

struct CMD_S_ManEaingFishSpeak
{
    WORD wElapse;
    char szContent[128];
};

struct CMD_S_EatingFishBossDie
{
    WORD wChairID;
    int nFishID[GAME_PLAYER][3];
    int nFishScore[GAME_PLAYER][3];
    int nCurrentScore;
};

struct CMD_S_ManEatingFish
{
    WORD wChairID;
    int nFishID[3];
    int nSpriteID[3];
    int nMultiple[3];
};

struct CMD_S_ManEatingFishNewFish
{
    WORD wChairID;
    WORD wIndex;
    int nFishID;
    int nSpriteID;
    int nCurrentScore;
    int nAteScore;
    int nMultiple;
};

struct CMD_S_EatScore
{
    WORD wChairID;
    WORD wIndex;
    int nFishID;
    int nSpriteID;
    int nEatScore;
    int nCurrentScore;
    int nMultiple;
};

struct CMD_S_BoxPrize
{
    BYTE cbType;//0未中奖、1金币、2奖券、3话费、4初级珍珠、5中级珍珠、6高级珍珠、7加速、8散射、9暴击，10藏宝图
    int nCount;
    int nFishID;//鱼ID
};

struct CMD_S_VipLevel
{
    BYTE cbVipLevel[GAME_PLAYER];
};

struct CMD_S_ExchangePearl
{
    BYTE cbResult;
    char szResult[64];
};

struct CMD_S_SendPearlInfo
{
    int nPearl1Price;
    int nPearl2Price;
    int nPearl3Price;
    int nLastScore;
};

struct CMD_S_MatchRank
{
    SCORE lMatchScore;//比赛积分
    int nCurrentRank;//当前排名
    int nChangeRank;//排名变化
};

struct CMD_S_WinPearl
{
    int nPearlType;
    int nPearlCount;
};

struct CMD_S_TryCannonCheck
{
    bool bCan;
};

struct CMD_S_NewGuidGift
{
    bool bSuccess;
    int nCannonStatus;
    int nLotteryCount;
    int nScoreGift;
    int nCurrentScore;
};

struct CMD_S_UserCharge
{
    WORD wChairID;
    int nScore;
    int nCurrentScore;
};

struct CMD_S_TryCannonEnd
{
    int nTotalTryScore;
};

//鱼的附加属性
enum enFishAttrib
{
	enFreeze=0x01,//定屏
	enBomb=0x02,//炸弹
	enBoss=0x04,//动态倍率鱼
	enSameDie=0x08,//同类炸弹
	enLine=0x10,//连线鱼
	enRangeBomb=0x20,//范围炸弹
	enLighting=0x40,//闪电鱼
	enJumpBomb=0x80,//超级炸弹
};

struct CPoint
{
	unsigned int x;
	unsigned int y;
};

//
struct tagFishPack
{
	int nFishID;//鱼ID
	int nSpriteID;//鱼类型
	int nPathIndex1;//鱼轨迹
	int nCreateTime;
	int nCreateDelayTime;
	int nMultiple;
	int nProperty;

	int nXPos;//初始坐标，当nPathIndex1为-1时有效
	int nYPos;
	float fRotation[5];//初始角度，当nPathIndex1为-1时有效
	int nMoveTime[5];
	int nSpeed[5];
};

//倍率信息
struct CMD_S_CannonMultiple
{
	WORD							wChairID;						//设置玩家
    BYTE                            cbCannonType;                   //0--12
	int								nCannonMultiple;
};

//切换场景
struct CMD_S_ChangeScene
{
	WORD							SceneIndex;						//当前场景
	WORD							RmoveID;						//预留这里
};

//子弹数目
struct CMD_S_BulletCount
{
	WORD							wChairID;						//玩家椅子
	bool							bAdd;
	int								lScore;
};

//金币数目
struct CMD_S_CoinCount
{
	WORD							wChairID;						//玩家椅子
	LONG							lCoinCount;						//金币数目
};

//场景消息
struct CMD_S_GameScene
{
	int								nSceneIndex;
	int								nMaxBullet;
	LONG							lCellScore;
    LONG                            lMaxMultiple;
	LONG							lUserAllScore[GAME_PLAYER];
	LONG							lUserCellScore[GAME_PLAYER];
    BYTE                            cbCannonType[GAME_PLAYER];
	int                             nExercise;
	bool                            bNoFish;
};

//发射炮弹
struct CMD_S_UserShoot
{
	WORD							wChairID;						//玩家椅子
    bool							bAndroid;
    BYTE                            cbAttribute;                    //子弹属性，0为普通子弹，1为暴击
	float							fAngle;							//发射角度
	int								nUserScore;
	DWORD							dwBulletID;
    DWORD                           dwCount;                        //子弹数量
};

//单元积分
struct CMD_S_LockFishId
{
	LONG							lFishID;						//单元积分
	int								iChairID;
};

//捕获鱼群
struct CMD_S_CaptureFish
{
    int                             nLottteryCount;
	WORD				 			wChairID;						//打中鱼的玩家
	bool							bSuperCannon;					//是否变换能量炮
    bool                            bPlayParticle;                  //是否播放大特效
	int								nUserScore;						//玩家当前分数
    int                             nTryScore;                      //试用炮获得的分数
	int								nProperty;						//打中的鱼的属性
	int								nCaptureCount;
	int								nTotalScore;
	int								nTotalMultiple;
	int								nFishID[MAX_HITFISH];			//打中的鱼的ID
	int								nFishMultiple[MAX_HITFISH];
	int								nFishScore[MAX_HITFISH];
};

//更新BOSS分数
struct CMD_S_UpdateBossScore
{
	int								nBossScore;
};

//JumpBomb
struct CMD_S_JumpBomb
{
	WORD							wChairID;
	int								nSpriteID;
	CPoint							ptStart;
	CPoint							ptJump[3];

	CPoint calcPoint(CPoint & pt)
	{
		CPoint ptRet;
		if (pt.x<640&&pt.y<355)
		{
			ptRet.x=640+rand()%630;
			ptRet.y=355+rand()%310;
		}
		else if (pt.x>640&&pt.y<355)
		{
			ptRet.x=640-rand()%630;
			ptRet.y=355+rand()%310;
		}
		else if (pt.x>640&&pt.y>355)
		{
			ptRet.x=640-rand()%630;
			ptRet.y=355-rand()%310;
		}
		else if (pt.x<640&&pt.y>355)
		{
			ptRet.x=640+rand()%630;
			ptRet.y=355-rand()%310;
		}
		return ptRet;
	}
};

struct CMD_S_TaskInfo
{
	int								nTaskID;//任务ID
	int								nFishCount;//用于记录任务进度，或者任务已完成
	int								nChange;
	bool								bGeted;
	bool								bComplete;
	bool								bReceived;
};

struct CMD_S_TimePrizeInfo
{
	int								nID;
	int								nTimeSecond;
	bool								bComplete;
	bool								bReceived;
};

struct CMD_S_TimePrizeReceived
{
	int								nID;
	int								nTimeSecond;
	int								nProfit;
	int								nCurrentScore;
};

struct CMD_S_TaskReceived
{
	int								nID;
	int								nNextID;
	int								nProfit;
	int								nCurrentScore;
};

struct CMD_S_MobileMoney
{
    WORD							wMoneyCount;
    char							szURL[64];
    char							szContent[256];
};

struct CMD_S_ReturnMobileMoney
{
    WORD                            wMobileMoney;
};

struct CMD_S_MobileCharge
{
    bool							bSuccess;
    WORD							wMoneyCurrent;//剩下的话费
};

//////////////////////////////////////////////////////////////////////////
//命令定义

enum enClientSubCmd
{
	enClientSubCmd_Begin=60,
	SUB_C_BUY_BULLETSPEED,								//购买子弹
	SUB_C_BUY_BULLET,									//购买子弹
	SUB_C_USER_SHOOT,									//发射炮弹
	SUB_C_CAPTURE_FISH,									//捕获鱼群
	SUB_C_GET_SCORE_INFO,								//分数信息
	SUB_C_CALC_SCORE,									//计算分数
	SUB_C_SET_CELL_SCORE,								//单元积分
	SUB_C_HIT_FISH,										//捕获鱼群
	SUB_C_CANNON_MULTIPLE,								//设置概率
	SUB_C_SET_COMEINUSER,								//设置概率
	SUB_C_LOCKFISH,									//锁定鱼
	SUB_C_TIME_PRIZE_RECEVIE,
	SUB_C_GET_TASK,
	SUB_C_TASK_RECEIVE,
    SUB_C_SHARE_INFO,
    SUB_C_GET_MOBILE_MONEY,
    SUB_C_MOBILE_CHARGE,
    SUB_C_TRY_CANNON,
    SUB_C_NEW_GUID_GIFT=79,
    SUB_C_TRY_CANNON_CHECK=80,
    SUB_C_EXCHANGE_PEARL,
    SUB_C_ADD_BOX_PRIZE,
    SUB_C_USE_PEARL=83,
    SUB_C_CHAT,
    SUB_C_PPL_CONFIRM,
    SUB_C_PPL_PUSH,
    SUB_C_QIANG_HONG_BAO=87,
    SUB_C_ONLINE_REWARD_TIME=88,//请求在线时间倒计时
    SUB_C_ONLINE_REWARD_INFO,//请求在线奖励信息
    SUB_C_ONLINE_REWARD_GET,//领取在线时间奖励
	enClientSubCmd_End
};

struct CMD_C_AskQiang
{
    DWORD dwUserID;
    DWORD dwHongBaoID;
};

struct CMD_C_Chat
{
    WORD wChairID;
    char szContent[128];
};

struct CMD_C_UsePearl
{
    BYTE cbType;
    int nFishID;
};

struct CMD_C_ExchangePearl
{
    int nPearlType;
    int nPearlCount;
};

//单元积分
struct CMD_C_LockFishId
{
	LONG							lFishID;						//单元积分
	WORD							wChairID;
	char							szCommand[MAX_PATH];
};

//购买子弹
struct CMD_C_BuyBullet
{
	int								lScore;							//上下分数
	bool							bAdd;
};

//发射炮弹
struct CMD_C_UserShoot
{
	float							fAngle;							//发射角度
	DWORD							dwBulletID;						//子弹标识
    DWORD                           dwCount;                        //子弹数量
    BYTE                            cbAttribute;                    //子弹属性
};

struct CMD_S_EquipmentDragonCount
{
    int nEquipmentDragonCount[GAME_PLAYER];
};

//击中鱼群
struct CMD_C_HitFish
{
	int								nFishID;
	unsigned int					dwBulletID;
	int								nHitUser;
	bool							bAndroid;
	int								nOtherCount;
	int								nXPos;
	int								nYPos;
	int								nOtherFishID[MAX_HITFISH];
};

struct CMD_C_CoinCount
{
	LONG							lCoinCount;						//金币数目
};

//设置概率
struct CMD_C_CannonMultiple
{
	BYTE							cbAddMultiple;
    BYTE                            cbCannonType;//0--12
};

struct CMD_C_GetTask
{
	int							nTaskID;
};

struct CMD_C_GetTaskProfit
{
	int							nTaskID;
};

enum eShareReason
{
    eShareMobileMoney = 100,
};

enum eShareAction
{
    eShareClick = 100,
    eShareSucceed,
    eShareFail,
    eShareCancel,
};

struct CMD_C_ShareInfo
{
    int nShareReason;
    int nShareAction;
};

struct CMD_C_MobileCharge
{
    WORD wMobileCount;
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()

#endif
