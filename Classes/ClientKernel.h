#ifndef CLIENT_KERNEL_HEAD_FILE
#define CLIENT_KERNEL_HEAD_FILE

class CClientKernelSink
{
public:
	virtual void ResetGameFrame() = NULL;
	virtual void CloseGameFrame() = NULL;

	virtual bool OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize) = NULL;
	virtual bool OnEventSocket(const TCP_Command & Command, const void * pBuffer, WORD wDataSize) = NULL;

	virtual bool OnEventUserEnter(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) = NULL;
	virtual bool OnEventUserLeave(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) = NULL;
	virtual void OnEventUserScore(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) = NULL;
	virtual void OnEventUserStatus(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) = NULL;
	virtual void OnEventUserMemberOrder(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) = NULL;
};

class CClientKernel
{
public:

	CClientKernel();
	virtual ~CClientKernel();

	virtual bool DestroyClientKernel();

	virtual BYTE GetGameStatus();
	virtual void SetGameStatus(BYTE bGameStatus);

	//自己位置
	virtual WORD GetMeChairID();
	//获取自己
	virtual tagUserInfo * GetMeUserInfo();
	//设置自己
	void SetMeUserInfo(tagUserInfo * pMeUserInfo);
	//获取玩家
	virtual tagUserInfo * GetUserInfo(WORD wChairID);

	//发送函数
	virtual bool SendSocketData(WORD wMainCmdID, WORD wSubCmdID);
	virtual bool SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);

	//发送准备
	virtual bool SendUserReady(void * pBuffer, WORD wDataSize);

	bool OnGameSocket(const TCP_Command & Command, void * pData, unsigned short wDataSize);

	//搜索用户
	tagUserInfo * SearchUserItem(DWORD dwUserID);
	//添加用户
	bool ActiveUserItem(tagUserInfo * pUserData);
	//删除用户
	bool DeleteUserItem(DWORD dwUserID);
	//更新用户
	bool UpdateUserItemScore(DWORD dwUerID, const tagUserScore * pUserScore);
	//更新用户
	bool UpdateUserItemStatus(DWORD dwUserID, tagUserStatus * pUserStatus, WORD wNetDelay);

	//内核重置
	void ResetClientKernel();
    
    //切换用户视图
    WORD SwitchViewChairID(WORD wChairID);

	WORD m_wTableID;
	WORD m_wChairID;
	DWORD m_dwUserID;
	BYTE m_cbGameStatus;

	tagUserInfo * m_pMeUserItem;
	tagUserInfo * m_pUserItem[MAX_CHAIR];

	static CClientKernel * m_ClientKernel;
	CClientKernelSink * m_pIClientKernelSink;
};

//////////////////////////////////////////////////////////////////////////

#endif
