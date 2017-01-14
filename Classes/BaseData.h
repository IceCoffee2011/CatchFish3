#ifndef _BASEDATA_H__
#define _BASEDATA_H__

//1金币、2奖券、3话费、4初级珍珠、5中级珍珠、6高级珍珠、7加速、8散射、9锁定，10藏宝图，11-19是9个炮
//1金币、2奖券、3话费、4铜弹头、5银弹头、6金弹头、7加速、8散射、9锁定，10藏宝图，11-19是9个炮
#define TwoFishNet				1			//两个渔网（普通）
#define ThreeFishNet			3			//三个渔网（普通）
#define FourFishNet				5			//四个渔网（普通）
#define TwoSuperFishNet			7			//两个渔网（能量炮）
#define ThreeSuperFishNet		9			//三个渔网（能量炮）
#define FourSuperFishNet		11			//四个渔网（能量炮）

#define MaxBulletCount			10			//最大子弹数目

#define CalcBox_Tag				10001		//结算框tag值
#define FishCountSub				40			//鱼的种类
#define SPEED_NORMAL            1/4.0f      //正常状态子弹
#define SPEED_UP                1/6.0f     //加速状态子弹
#define BARREL_ROTATION			180/32.0f

#define FULL_COUNT					52

//游戏内场景层级tag值
//游戏内道具层DaojuLayer  1989
//游戏内喇叭界面FirstChargeLayer  1990
//游戏内食人鱼说话背景图           1988

//在线时间奖励
struct tagIntervalPrize
{
	int nHour[5];
	int nMinute[5];
	int nSecond[5];
};

//音效配置
struct tagSoundCfg
{
	int nSoundID;
	bool bLoop;
	std::string sPath;
};

//特效
struct tagParticleCfg
{
	std::string sPath;
	CCMagicEmitter* magicEmitter;
};

struct tagLockPos
{
	bool bHave;
	float fXPos;
	float fYPos;
};

struct tagLockLine
{
	int nBubbleCount;
	tagLockPos LockNumber;
	tagLockPos LockBubble[20];
};

struct tagPathPoint
{
	int xPos;
	int yPos;
	int rotation;
	int moveTime;
	int speed;
};

struct tagPathIndex
{
	tagPathPoint pPathIndex[5];
	int pPathIndexId;
};


struct tagTask
{
	int wTaskId;
	unsigned short wFishTypeId;//鱼的种类
	unsigned short wCurrentCount;//打死鱼的数量
	unsigned short wFishCount;//鱼的任务数量
	bool bComplete;//已完成；次日清除（数据库操作）
	bool bReceived;//领取奖励
	bool bGeted;
	int nProfit;//奖励金币
	int nTimeSecond;//时间限制（秒），小于0则没有时间限制
	std::string sTaskName;
	std::string sTaskDescription;
	std::string sTaskSimpleDes;
};

struct tagPrize
{
	unsigned short wPrizeId;
	int nSecond;						//秒
	//int nCurrentSecond;				//当前过了多少秒
	int nGoldCount;					//奖励金币
	//bool bComplete;					//是否达到在线时间
	//bool bReceive;					//是否领取
	bool bState;						//状态
};

enum eUserBehaviorLogonTag
{
    eBtYoukeLogon = 1001,//游客按钮
    eBtUserLogon,//登录按钮
    eBtUserRegister,//注册按钮
    eBtGetUserTel,//发送验证码
    eBtRegisterCompleteTel,//手机号注册完成
    eBtRegisterCompleteAcc,//账号注册完成
    eBtLogon,//登录
    eBtFaceBack,//反馈
    eBtBack,//返回
};

enum eUserBehaviorHallTag
{
    eHallBeibao = 1101,//背包按钮
    eHallRank,//排行按钮
    eHallMail,//邮件按钮
    eHallSetting,//设置按钮
    eHallService,//客服按钮
    eHallFreecoin,//免费金币按钮
    eHallVIP,//VIP按钮
    eHallLottery,//兑换话费按钮
    eHallCharge,//充值按钮
    eHallBack,//返回按钮
    eHallDuihuan,//兑换码按钮
    eHallbtFirstCharge,//首充按钮
    eHallLaba,//喇叭按钮
    eHallQuick,//快速开始按钮
    eHallRoom1,//金蟾新手房
    eHallRoom2,//百炮房
    eHallRoom3,//千炮房
    eHallRoom4,//万炮房
    eHall3Room1,//副本房间
    eHall4Room1,//摇钱树新手房
    eHall4Room2,//摇钱树百炮房
    eHall4Room3,//摇钱树千炮房
    eHall4Room4,//摇钱树万炮房
    eHall5Room1,//哪吒闹海新手房
    eHall5Room2,//哪吒闹海百炮房
    eHall5Room3,//哪吒闹海千炮房
    eHall5Room4,//哪吒闹海万炮房
    eHallReceiveFirstCharge,//首充领取返利按钮
    eHallNoticeToVIP,//大厅公告查看VIP
    eHallRedPacket,//发红包按钮
    eHallGonggao,//公告按钮
    eHallWeekMatch,//周赛榜按钮
    eHallWufu,//五福按钮
    
};

enum eFirstChargeMail
{
    eHallSendDuihuanma=1201,//发送兑换码按钮
    eHallSendDuihuanSuccess,//兑换码成功
    eHallSendDuihuanFail,//兑换码失败
    eHallFirstChargeClose,//首充界面关闭按钮
    eHallOnlineService,//在线客服按钮
    eHallSendLaba,//发送喇叭按钮
    eHallSendLabaSuccess,//发送喇叭成功
    eHallSendLabaFail,//发送喇叭失败
    eHallMailChakan,//邮件查看按钮
    eHallMailLingqu,//邮件领取按钮
    eHallMailLingquSuccess,//邮件领取成功
    eHallMailLingquFail,//邮件领取失败
    eHallMailLingquFailComplete,//已经领取过了
};

enum eHallFreeCoinVIP
{
    eBtFreeCoinGive=1301,//破产救济按钮
    eBtFreeCoinShare,//分享按钮
    eBtFreeCoinLogon,//签到按钮
    eBtFreeCoinBanding,//绑定手机按钮
    eBtGiveLingqu,//救济金领取按钮
    eLingquSuccess,//成功
    eLingquFail,//失败
    eSendTelCodeNext,//绑定手机下一步按钮
    eSendBandingComplete,//绑定手机确定按钮
    eVIPUpgrade,//VIP界面中升级按钮
};

enum eUserBehaviorHallLotteryTag
{
    eHallLotteryToThirtyCMCC = 1401, //30话费:移动
    eHallLotteryToFiftyCMCC,        //50话费:移动
    eHallLotteryToHundredCMCC,      //100话费:移动
    eHallLotteryToThirtyCUCC,       //30话费:联通
    eHallLotteryToFiftyCUCC,        //50话费:联通
    eHallLotteryToHundredCUCC,      //100话费:联通
    eHallLotteryToThirtyCTCC,       //30话费:电信
    eHallLotteryToFiftyCTCC,        //50话费:电信
    eHallLotteryToHundredCTCC,      //100话费:电信
    eHallLotteryDuiHuanSuccess,//兑换成功
    ehallLotteryDuiHuanFail,//兑换失败
};

enum eUserBehaviorHallRank
{
    eHallRankVIP = 2901,//大厅VIP排行按钮
    eHallRankMoney,//大厅财富排行按钮
    eHallRankRule,//大厅比赛规则按钮
};

enum eUserBehaviorGameTag
{
    eGameRankVIP=3001,//游戏VIP排行按钮
    eGameRankRule,//游戏比赛规则按钮
    eGameLottery,//游戏内话费兑换按钮
    eGameCharge,//游戏充值按钮
    eGameAutoFire,//自动开炮按钮
    eGameCancelAutoFire,//取消自动开炮
    eGameSuoding,//锁定按钮
    eGameSpeedup,//加速按钮
    eGameMenu1,//菜单显示按钮
    eGameMenu2,//菜单隐藏按钮
    eGameSetting,//设置按钮
    eGameHelp,//帮助按钮
    eGameExit,//退出按钮
    eGameExit2,//结算按钮（真正退出）
    eGameJiaMult,//加炮按钮
    eGameJianMult,//减炮按钮
    eGameNoticeVIP,//游戏内公告查看VIP按钮
    eGameBeibao,//游戏内背包按钮
    eGameZengSpeedup,//赠送加速技能按钮
    eGameZengBaoji,//赠送锁定技能按钮
    eGameNoCoinToFreeCoin,//游戏内金币不足进入免费金币
    eGameNoCoinToCharge,//游戏内金币不足进行充值
    
};

enum eHallCharge
{
    eHallCharge1 = 3301,//大厅648元
    eHallCharge2,//大厅188元
    eHallCharge3,//大厅98元
    eHallCharge4,//大厅50元
    eHallCharge5,//大厅30元
    eHallCharge6,//大厅12元
    eHallCharge7,//大厅6元
    eHallCharge8,//大厅1元
    eHallAlipay,//大厅支付宝
    eHallWeChat,//大厅微信
    eHallBank,//大厅银行卡
    eHallIOSPay,//大厅苹果内购
    eHallFirstCharge,//大厅充值界面限购按钮
    eHallCloseBack,//大厅充值界面返回上一级按钮
    eHallClose,//大厅充值界面关闭按钮
    eHallChargeToVIP,//大厅充值界面查看VIP按钮
    eHallChargeSuccess,//充值成功
    eHallChargeCancel,//充值取消
    eHallChargeFail,//充值失败
    eHallChargeOrderFail,//下订单失败
    eHallChargeOrderSuccess,//下订单成功
    eHallBankChargeClose,//银行卡充值关闭按钮
};

enum eGameCharge
{
    eGameCharge1 = 3201,//游戏内648元
    eGameCharge2,//游戏内188元
    eGameCharge3,//游戏内98元
    eGameCharge4,//游戏内58元
    eGameCharge5,//游戏内30元
    eGameCharge6,//游戏内12元
    eGameCharge7,//游戏内6元
    eGameCharge8,//游戏内1元
    eGameAlipay,//游戏内支付宝
    eGameWeChat,//游戏内微信
    eGameBank,//游戏内银行卡
    eGameIOSPay,//游戏内苹果内购
    eGameFirstCharge,//游戏内限购按钮
    eGameCloseBack,//游戏内充值返回上一级按钮
    eGameClose,//游戏内充值关闭按钮
    eGameChargeToVIP,//游戏内充值界面查看VIP按钮
};

enum eGameSuperCharge
{
    eGameSuperCharge1 = 4101,//游戏内648元
    eGameSuperCharge2,//游戏内388元
    eGameSuperCharge3,//游戏内188元
    eGameSuperCharge4,//游戏内98元
    eGameSuperCharge5,//游戏内50元
    eGameSuperCharge6,//游戏内30元
    eGameSuperCharge7,//游戏内6元
    eGameSuperCharge8,//游戏内12元
    eGameSuperAlipay,//游戏内支付宝
    eGameSuperWeChat,//游戏内微信
    eGameSuperBank,//游戏内银行卡
    eGameSuperIOSPay,//游戏内苹果内购
    eGameSuperFirstCharge,//游戏内限购按钮
    eGameSuperCloseBack,//游戏内充值返回上一级按钮
    eGameSuperClose,//游戏内充值关闭按钮
    eGameSuperChargeToVIP,//游戏内充值界面查看VIP按钮
    eGameSuperBarrel1,//游戏内疾风炮
    eGameSuperBarrel2,//游戏内生化炮
    eGameSuperBarrel3,//游戏内闪电炮
    eGameSuperBarrel4,//游戏内激光炮
    eGameSuperSkill1,//游戏内加速技能
    eGameSuperSkill2,//游戏内闪射技能
    eGameSuperSkill3,//游戏内暴击技能
};

enum eHallMobile
{
	eAndroidExitGame=3401,//安卓返回键
	eAndroidExitGameOK,//安卓退出游戏
	eAndroidExitGameCancel,//安卓取消退出游戏
    eAndroidExitGameHallToLogon,//安卓返回键大厅到登录
    eAndroidExitGameBYToHall,//安卓返回键捕鱼到大厅
    eAndroidExitGameBYToHallOK,//安卓返回键捕鱼到大厅确定
    eAndroidExitGameBYToHallCancel,//安卓返回键捕鱼到大厅取消
    eXinShouRoomExit,//新手房打到15万金币
    eXinShouRoomExitOK,//新手房打到15万金币退出游戏
    
};

//超级海盗
enum eSuperPirateGuide
{
    eSuperPirateGuidePlusTag=3501,//新手指引--加注
    eSuperPirateGuideMinusTag,//新手指引--减注
    eSuperPirateGuideGoTag,//新手指引--go按钮
    eSuperPirateGuideStopTag,//新手指引--stop按钮
    eSuperPirateGuideNanTag,//新手指引--猜男按钮
    eSuperPirateGuideNvTag,//新手指引--猜女按钮
    eSuperPirateGuideGetPrizeTag,//新手指引--收金按钮
    eSuperPirateGuideFinishedTag,//新手指引--结束
};

enum eSuperPirateSystem
{
    eSuperPirateExitTag=3601,//退出按钮
    eSuperPirateRankTag,//排行榜打开按钮
    eSuperPirateRankCloseTag,//排行榜关闭按钮
    eSuperPirateHelpTag,//帮助打开按钮
    eSuperPirateHelpCloseTag,//帮助关闭按钮
    eSuperPirateHelpToTwoTag,//帮助下一页（第二页）按钮
    eSuperPirateHelpToThreeTag,//帮助下一页（第三页）按钮
    eSuperPirateHelpToFourTag,//帮助下一页（第四页）按钮
    eSuperPirateHelpToFiveTag,//帮助下一页（第五页）按钮
    eSuperPirateHelpBackFourTag,//帮助上一页（第四页）按钮
    eSuperPirateHelpBackThreeTag,//帮助上一页（第三页）按钮
    eSuperPirateHelpBackTwoTag,//帮助上一页（第二页）按钮
    eSuperPirateHelpBackOneTag,//帮助上一页（第一页）按钮
    eSuperPirateSoundOnTag,//声音开启按钮
    eSuperPirateSoundOffTag,//声音关闭按钮
	eSuperPirateChargeTag,//充值按钮
};

enum eSuperPirateGameLayer
{
    eSuperPirateMinusBetTag=3701,//+（第一个界面加注）按钮
    eSuperPiratePlusBetTag,//-（第一个界面减注）按钮
    eSuperPirateAutoTag,//开启自动按钮
    eSuperPirateCancelAutoTag,//取消自动按钮
    eSuperPirateGoTag,//go按钮
    eSuperPirateStopTag,//stop按钮
};

enum eSuperPiratePrizeLayer
{
    eSuperPirateTwoPrizeTag=3801,//2倍按钮
    eSuperPirateThreePrizeTag,//3倍按钮
    eSuperPirateFivePrizeTag,//5倍按钮
    eSuperPirateCancelTwoPrizeTag,//取消2倍按钮
    eSuperPirateCancelThreePrizeTag,//取消3倍按钮
    eSuperPirateCancelFivePrizeTag,//取消5倍按钮
    eSuperPirateNanTag,//猜男按钮
    eSuperPirateNvTag,//猜女按钮
    eSuperPirateGetPrizeTag,//收金按钮
};

enum eSuperPirateResult
{
    eSuperResult1=3901, 	//第一个界面不中
    eSuperResult2,//	1倍
    eSuperResult3,//	2倍
    eSuperResult4,//	3倍
    eSuperResult5,//	5倍
    eSuperResult6,//	10倍
    eSuperResult7,//	20倍
    eSuperResult8,//	30倍
    eSuperResult9,//	40倍
    eSuperResult10,// 	50倍
    eSuperResult11,//	100倍
    eSuperResult12,//   1星
    eSuperResult13,//	2星
    eSuperResult14,//	3星
    eSuperResult15,//	4星
    eSuperResult16,//	5星
    eSuperResult17,//	6星
    eSuperResult18,//	7星
    eSuperResult19,//	8星
    eSuperResult20,//猜大小不中
};

enum eNewJiaAnchor
{
    eHallSignIn1=4001,//点击普通用户签到
    eHallSignIn2,//	普通用户签到成功
    eHallSignIn3,//	普通用户签到失败
    eHallSignIn4,//	点击VIP签到
    eHallSignIn5,// VIP签到成功
    eHallSignIn6,// VIP签到失败
    eGameCoinToSkill1,//游戏内金币兑换加速技能
    eGameCoinToSkill3,//游戏内金币兑换锁定技能
    eHallCoinToSkill1,//大厅金币兑换加速技能
    eHallCoinToSkill3,//大厅金币兑换锁定技能
    eGameUserLaba,//游戏内发送喇叭
};

enum eDay0526Anchor
{
    eSuperScoreMinus = 5001,//海盗内分数低于5000以下
    eSuperScoreMinusOK,//海盗内分数低于5000以下确定按钮
    eHallTouxiang,//点击大厅头像按钮
    eHallSelectedPicture,//选择头像按钮
    eHallUploadPicture,//上传头像按钮
    eHallUploadSuccess,//上传头像完成
    eHallPictureClose,//头像页面关闭按钮
};

enum eNewMatchAnchor
{
    eGameMatchOnGoing=6001,//游戏内--进行中
    eGameMatchMyEnrol,//游戏内--我的报名
    eGameMatchMyMatch,//游戏内--我的赛况
    eGameMatchRank,//游戏内--名人堂
    eGameMatchClose,//游戏内--关闭
    eGameMatchJoinFree,//游戏内--免费报名
    eGameMatchJoinCoin,//游戏内--金币报名
    eGameMatchLingQuDirect,//游戏内--直接领取
    eGameMatchLingQuShare,//游戏内--分享领取
    eHallMatchOnGoing,//大厅--进行中
    eHallMatchMyEnrol,//大厅--我的报名
    eHallMatchMyMatch,//大厅--我的赛况
    eHallMatchRank,//大厅--名人堂
    eHallMatchClose,//大厅--关闭
    eHallMatchJoinFree,//大厅--免费报名
    eHallMatchJoinCoin,//大厅--金币报名
    eHallMatchLingQuDirect,//大厅--直接领取
    eHallMatchLingQuShare,//大厅--分享领取
    eMatchShareCancle,//比赛前三名领取分享取消
    eMatchShareBegin,//比赛前三名领取分享开始
    eMatchShareSuccess,//比赛前三名领取分享成功
    eMatchShareFail,//比赛前三名领取分享失败
    eMatchInfoNotify,//比赛提醒报名提醒
    eMatchBeginTimeNotify,//比赛开始倒计时提醒
    eMatchBegin,//比赛开始
    eMatchEndTimeNotify,//比赛结束倒计时提醒
    eMatchEnd,//比赛结束
    eMatchSharePrise,//比赛分享领奖按钮
    eMatchNormalPrise,//比赛普通领奖按钮
    eMatchSignInNext,//比赛报名下一场
    eMatchPriseClose,//比赛结束界面关闭按钮
    eMatchEndSharePriseScuuess,//比赛结束界面分享领奖网站回到成功
};

enum eRemindUpdateAnchor
{
    eRemindUpdate1=5301,//弹出更新提示框
    eRemindUpdate2,//点击更新按钮
    eRemindUpdate3,//点击不更新按钮
};

enum eAnchor0707{
    eCanGetFreeCoinTag=5401,//请求领取救济金
    eGetFreeCoinTag,//领取救济金成功
    eGetFreeCoinFailTag,//领取救济金失败
    eMatchPrizeRemindTag,//进入房间领取奖励提醒（一天一次）
};

enum eAnchor0728{
    eGameClickDantouTag=5501,//点击弹头按钮
    eGameClickTongDantouTag,//使用铜弹头
    eGameClickYinDantouTag,//使用银弹头
    eGameClickJinDantouTag,//使用金弹头
    eGameUserDantouSuccessTag,//使用弹头成功
    eGameGetTongDantou,//打中铜弹头
    eGameGetYinDantou,//打中银弹头
    eGameGetJinDantou,//打中金弹头
    eGameTongDantouSuccess,//游戏内赠送铜弹头成功
    eGameYinDantouSuccess,//游戏内赠送银弹头成功
    eGameJinDantouSuccess,//游戏内赠送金弹头成功
    eGameUserTongDantou,//游戏内背包界面使用铜弹头
    eGameUserYinDantou,//游戏内背包界面使用银弹头
    eGameUserJinDantou,//游戏内背包界面使用金弹头
    eGameZengTongDantou,//游戏中赠送铜弹头
    eGameZengYinDantou,//游戏中赠送银弹头
    eGameZengJinDantou,//游戏中赠送金弹头
    eHallZengTongDantou,//大厅赠送铜弹头
    eHallZengYinDantou,//大厅赠送银弹头
    eHallZengJinDantou,//大厅赠送金弹头
};

enum eAnchor0922{
    eHallFirstChargeAlipay=7001,//大厅首充支付宝
    eHallFirstChargeWechat,//大厅首充微信
    eGameFirstChargeAlipay,//游戏首充支付宝
    eGameFirstChargeWechat,//游戏首充微信
    eGameNoScoreAlipay,//游戏内分数不足首充支付宝按钮
    eGameNoScoreWechat=7006,//游戏内分数不足微信按钮
    eHallQianpao,//98元千炮房充值按钮
    eHallWanpao,//188元万炮房充值按钮
    eChargeUIAlipay,//支付宝支付
    eChargeUIWechat,//微信支付
    eChargeUIBank=7011,//银行卡支付
    eChargeUIIOSPay,//苹果内购
    eGameNoScoreClose,//游戏内分数不足首充关闭按钮
    ePPLStart,//拍拍乐开始
    ePPLDefaultStart,//拍拍乐默认开始
    ePPLZhunbei=7016,//拍拍乐准备
    ePPLCalc,//拍拍乐结束
    ePPLCancel,//拍拍乐取消
    eSendRedPacketTag,//大厅发红包按钮
    eSendRedPacketUITag,//红包界面发红包按钮
    eReceiveRedPacket=7021,//收到红包消息
    eRodPacketTag,//发送抢红包消息
    eRodPacketedTag,//抢到红包
    eRodNoPacketTag,//没有抢到红包
    eSendFriendPacketTag,//给好友发红包
    eWeekMatch1=7026,//周赛本周排名按钮
    eWeekMatch2,//周赛比赛规则按钮
    eWeekMatch3,//周赛奖品介绍按钮
    eWeekMatch4,//周赛上周排行按钮
    eWeekMatch5,//周赛关闭按钮
    eWufuClose=7031,//五福关闭按钮
    eWufu50yuan,//兑换50元京东卡
    eWufu100yuan,//兑换100元京东卡
    eWufuHelp,//五福帮助按钮
    eWufuSuccess,//兑换京东卡成功
    eWufuFail=7036,//兑换京东卡失败
    eWufuGet,//游戏内获得五福卡
    eUserShareSuccess,//分享成功
    eUserShareFail,//分享失败
    eGameClickWufu,//游戏内点击五福按钮
    eOnlineGetCoin=7041,//领取在线奖励
    eOnlineRequest,//请求在线奖励
    eOnlineClose,//关闭在线奖励
    eOnlineComplete,//在线奖励全部领取完毕
    eWheelOpenTag,//打开转盘
    eWheelStartTag=7046,//开始转转盘
    eWheelStopTag,//关闭转盘
    eShareHallTag,//大厅推广返现按钮
    eShareExchangeTag,//分享兑换奖励按钮
    eShareInfoTag,//分享活动说明按钮
    eShareNewGiftTag=7051,//分享新手奖励按钮
    eShareFriendTag,//分享邀请好友按钮
    eShareCloseTag,//分享关闭按钮
    eShareCopyTag,//复制按钮
    eShareWX1Tag,//邀请微信好友按钮
    eShareWX2Tag=7056,//邀请微信朋友圈按钮
    eShareQQTag,//邀请QQ好友
    eShareFriendCloseTag,//分享邀请好友界面关闭按钮
    eShareInfoCloseTag,//分享说明界面关闭按钮
    eShareExchangeGiftTag,//兑换奖励界面兑换按钮
    eShareExchangeCloseTag=7061,//兑换奖励界面关闭按钮
    eShareNewGiftPasteTag,//分享新手奖励粘贴按钮
    eShareNewGiftCloseTag,//分享新手奖励关闭按钮
    eShareNewGiftGetTag,//分享新手奖励领取按钮
    eOneMoneyHallTag,//1元购大厅按钮
    eOneMoneyClsoeTag=7066,//1元购关闭按钮
    eOneMoneyGetTag,//1元购获取按钮
    eOneHallWXChargeTag,//大厅内1元够微信充值按钮
    eOneHallAlipayChargeTag,//大厅内1元购支付宝充值按钮
    eTenHallWXChargeTag,//大厅内10元限购礼包微信充值按钮
    eTenHallAlipayChargeTag=7071,//大厅内10元限购礼包支付宝充值按钮
    eOneGameWXChargeTag,//游戏内1元购微信充值按钮
    eOneGameAlipayChargeTag,//游戏内1元购支付宝充值按钮
    eTenGameWXChargeTag,//游戏内10元限购礼包微信充值按钮
    eTenGameAlipayChargeTag,//游戏内10元限购礼包支付宝支付按钮
    eTenGetTag=7076,//10元限购礼包获取按钮
    eChargeOneMoneyTag,//充值界面点击1元购按钮
};

enum eSmallGameTag
{
    eHaidaoChargeTag=8001,//海盗内打开充值界面
    eHaidaoCloseChargeTag,//海盗内充值返回上一级按钮
    eHaidaoFirstChargeTag,//海盗内点击10元限购按钮
    eHaidaoCharge1,//海盗内1元
    eHaidaoCharge2,//海盗内6元
    eHaidaoCharge3,//海盗内12元
    eHaidaoCharge4,//海盗内30元
    eHaidaoCharge5,//海盗内50元
    eHaidaoCharge6,//海盗内98元
    eHaidaoCharge7,//海盗内188元
    eHaidaoCharge8,//海盗内648元
    eHallHaidaoTag,//大厅点击海盗图标
    eHallPaogouwangTag,//大厅点击跑狗王图标
};

#endif
