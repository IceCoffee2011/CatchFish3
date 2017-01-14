#ifndef CMD_H3Card_h
#define CMD_H3Card_h

#include "header.h"

#pragma pack(1)

const BYTE H3CardListData[52] =
{
    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
    0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
    0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
    0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D	//黑桃 A - K
};



#define LOGIC_MASK_COLOR            0xF0
#define LOGIC_MASK_VALUE            0x0F



//////////////////////////////////////////////////////////////////////////////////
//状态定义
#define MAX_RICHER 8
#define BET_TIME   15
#define SAMPLE_NUM 20
enum
{
    POS_BLACK,
    POS_RED,
    POS_SPECIAL
};
enum
{
    GAME_START,        // 1
    GAME_NEW_GAME,     //
    GAME_BET_BEGIN,
    GAME_BET_END,
    GAME_OPEN_CARDS,
    GAME_RESULT,
    GAME_OPEN_JACKPOT
};


enum
{
    ACTION_C_START = 0x00000000,
    ACTION_C_BET,
    ACTION_C_RANK,
    ACTION_C_CHAT,
    ACTION_C_USER_INFO,
};

enum
{
    ACTION_S_START = 0x00001000,
    ACTION_S_NOTICE,
};


enum
{
    SUB_CMD_STATUS,
    SUB_CMD_B_BET,
    SUB_CMD_S_BET,
    SUB_CMD_OPEN_CARDS,
    SUB_CMD_REULT,
    SUB_CMD_SELF_RESULT,            //自已输赢结果
    SUB_CMD_DECK_INFO,              // 桌面玩家,7个土豪和1个幸运星
    SUB_CMD_DECK_WITH_MONEY,        // 桌面玩家赢家赢的金币
    SUB_CMD_TOP_WINNER,             //本轮赢得最多的玩家和赢的金币数
    
    SUB_CMD_ERROR,
    SUB_CMD_ONLINE_INfO,
    SUB_CMD_TREND,
    SUB_CMD_BET_LEVEL,     //根据玩家的金币数，更新下可注的金额
    SUB_CMD_JACKPOT,   // 大奖池,显示大奖池金额
    SUB_CMD_JACKPOT_RESULT,
    SUB_CMD_JACKOIT_AWARD,
    SUB_CMD_S_CHAT,
    SUB_CMD_B_CHAT,
    SUB_CMD_S_USER_INFO,
};

enum
{
    CARDS_235,
    CARDS_TYPE_SINGLE, // 单张
    CARDS_TYPE_PAIR,
    CARDS_TYPE_STRAIGHT,
    CARDS_TYPE_FLUSH, // 同花
    CARDS_TYPE_FLUSH_STRAIGHT,
    CARDS_TYPE_THREE,
};

enum
{
    CARD_TYPE_SINGLE, // 单张
    CARD_TYPE_PAIR,
    CARD_TYPE_PAIR_BIG,
    CARD_TYPE_STRAIGHT,
    CARD_TYPE_FLUSH, // 同花
    CARD_TYPE_FLUSH_STRAIGHT,
    CARD_TYPE_THREE,
};


struct CMD_C_BET
{
    int		    nPos;    // 0 POS_BLACK 1 POS_RED 2 POS_SPECIAL
    LONGLONG    llBet;
};


struct CMD_B_BET
{
    DWORD			 dwUserID;
    int 	         nPos;  // 0 POS_BLACK 1 POS_RED 2 POS_SPECIAL
    LONGLONG         llBet;
    LONGLONG         llDeskMoney[3];
};

struct CMD_S_BET
{
    LONGLONG         llCurrentScore;
    LONGLONG         llBet[3];
};

struct CMD_S_ERROR
{
    int nLevel;
    int nErrorCode;
};

struct CMD_S_SELF_RESULT
{
    LONGLONG llCurrentScore;
    LONGLONG llDeltaScore;
};

struct CMD_S_GAME_SCENE
{
    
    LONGLONG  llMoney;
    LONGLONG  llLow;
    LONGLONG  llMid;
    LONGLONG  llHigh;
};

struct CMD_B_GAME_STATUS
{
    int    m_nStatus;
    int    nCountdown;
};


struct CMD_B_OPEN_CARDS
{
    BYTE      cbBlackCards[3];
    BYTE      cbRedCards[3];  // 0 type, 1,2,3
    int       nBlackType;
    int       nRedType;
    int       nBlackValue;
    int       nRedValue;
};


struct CMD_B_DECK_INFO
{
    DWORD  dwUserIDs[MAX_RICHER];
};

struct CMD_B_DECK_WITH_MONEY
{
    LONGLONG  llMoneys[MAX_RICHER];
};

struct CMD_B_RESULT
{
    int nWinner;   // 0 black win, 1 red  win
};

struct CMD_B_TOP_WINNER
{
    DWORD    dwUserID;
    LONGLONG llMoney;
};


struct CMD_B_ONLINE_INfO
{
    int nRoomId;
    int nOnlineNum;
};


struct Trend_Item
{
    int nWinnerSide;  // 0 black win, 1 red win
    int nCardType;    // 0 单张 1  小2， 2  大二， 3 顺子， 4同花 5是同花顺， 6 baozi
};

struct CMD_B_Trend
{
    Trend_Item items[SAMPLE_NUM];
};

struct CMD_B_JACKPOT
{
    LONGLONG llMoney;
};

struct CMD_B_JACKPOT_RESULT
{
    int nPos;
    int nTotalPos;
};

struct JackSpot_Item
{
    DWORD    dwUserID;
    LONGLONG llMoney;
};

struct CMD_B_JACKPOT_AWARD
{
    int nNum;
    JackSpot_Item items[20];
};

struct CMD_C_CHAT
{
    int nChatType;
    DWORD dwTo;
};

struct CMD_S_CHAT
{
    LONGLONG llMoney;
};

struct CMD_B_CHAT
{
    int nChatType;
    DWORD dwFrom;
    DWORD dwTo;
};

struct CMD_S_BET_LEVEL
{
    LONGLONG  llLow;
    LONGLONG  llMid;
    LONGLONG  llHigh;
};

struct CMD_C_USE_INFO
{
    DWORD dwUserId;
};

struct CMD_S_USE_INFO
{
    LONGLONG data[10];//0金币数，1表示游戏局数，2表示最大赢取金币数，3表示20局下注金币数，4表示20局获胜多少局
};

#pragma pack()

#endif
