#include "header.h"

CClientKernel * CClientKernel::m_ClientKernel = NULL;

CClientKernel::CClientKernel()
{
	m_dwUserID=0L;
	m_wTableID=INVALID_TABLE;
	m_wChairID=INVALID_CHAIR;

	//游戏信息
	m_pMeUserItem=NULL;
	m_cbGameStatus=GAME_STATUS_FREE;
	for (int i=0; i<MAX_CHAIR; i++)
		m_pUserItem[i] = NULL;

	m_pIClientKernelSink = NULL;

	m_ClientKernel = this;
}

CClientKernel::~CClientKernel(void)
{
	DestroyClientKernel();
}

bool CClientKernel::DestroyClientKernel()
{
	//SendData(IPC_MIAN_IPC_KERNEL,IPC_SUB_IPC_CLIENT_CLOSE);

	//设置接口
	m_ClientKernel = NULL;
	m_pIClientKernelSink=NULL;

	for (int i=0; i<MAX_CHAIR; i++)
	{
		if (m_pUserItem[i] != NULL)
		{
			delete m_pUserItem[i];
			m_pUserItem[i] = NULL;
		}
	}

	return true;
}

//内核重置
void CClientKernel::ResetClientKernel()
{
	m_pMeUserItem=NULL;
	m_cbGameStatus=GAME_STATUS_FREE;
	m_wTableID=INVALID_TABLE;
	m_wChairID=INVALID_CHAIR;

	if (m_pIClientKernelSink!=NULL)
		m_pIClientKernelSink->ResetGameFrame();
}

//发送函数
bool CClientKernel::SendSocketData(WORD wMainCmdID, WORD wSubCmdID)
{
	bool bRet = false;
	CMainLogic * pMainLogic = CMainLogic::sharedMainLogic();
	bRet = pMainLogic->SendData(pMainLogic->m_nRoomSocketHandle,wMainCmdID,wSubCmdID);

	return bRet;
}

//发送函数
bool CClientKernel::SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	if (wDataSize>SOCKET_TCP_PACKET)
	{
		ERROR_CHECK;
		return false;
	}

	bool bRet = false;
	CMainLogic * pMainLogic = CMainLogic::sharedMainLogic();
	bRet = pMainLogic->SendData(pMainLogic->m_nRoomSocketHandle,wMainCmdID,wSubCmdID,pData,wDataSize);

	return bRet;
}

//发送准备
bool CClientKernel::SendUserReady(void * pBuffer, WORD wDataSize)
{
	return SendSocketData(MDM_GF_FRAME,SUB_GF_USER_READY,pBuffer,wDataSize);
}

//@@删除无用代码
BYTE CClientKernel::GetGameStatus()
{
	return m_cbGameStatus;
}

void CClientKernel::SetGameStatus(BYTE bGameStatus)
{
	m_cbGameStatus=bGameStatus;
}

WORD CClientKernel::GetMeChairID()
{
	return m_wChairID;
}

tagUserInfo * CClientKernel::GetMeUserInfo()
{
	return m_pMeUserItem;
}

void CClientKernel::SetMeUserInfo(tagUserInfo * pMeUserInfo)
{
	m_pMeUserItem = pMeUserInfo;
}

tagUserInfo * CClientKernel::GetUserInfo(WORD wChairID)
{
	if (wChairID>=MAX_CHAIR)
	{
		return NULL;
	}
	return m_pUserItem[wChairID];
}

tagUserInfo * CClientKernel::SearchUserItem(DWORD dwUserID)
{
	for (int i=0; i<MAX_CHAIR; i++)
	{
		if (m_pUserItem[i] && m_pUserItem[i]->dwUserID == dwUserID)
		{
			return m_pUserItem[i];
		}
	}

	return NULL;
}

//增加用户
bool CClientKernel::ActiveUserItem(tagUserInfo * pUserData)
{
	//@@修正问题，原来是直接使用CMainLogic内的指针，现在是创建新的并拷贝，前者会导致玩家离开座位时，状态提前更新，导致错误
	m_pUserItem[pUserData->wChairID] = new tagUserInfo;
    memcpy(m_pUserItem[pUserData->wChairID], pUserData, sizeof(tagUserInfo));

	if (m_dwUserID==pUserData->dwUserID)
	{
		m_pMeUserItem=m_pUserItem[pUserData->wChairID];
        ////printf("m_pMeUserItem is %x\n", m_pMeUserItem);
		m_wTableID=m_pMeUserItem->wTableID;
		m_wChairID=m_pMeUserItem->wChairID;
	}	

	if (m_pIClientKernelSink!=NULL)
    {
        ////printf("pUserData->wChairID is %d\n", pUserData->wChairID);
		m_pIClientKernelSink->OnEventUserEnter(m_pUserItem[pUserData->wChairID], pUserData->wChairID, false);
    }

	return true;
}

//删除用户
bool CClientKernel::DeleteUserItem(DWORD dwUserID)
{
	tagUserInfo * pUserData=SearchUserItem(dwUserID);
	
	if (pUserData==NULL) return true;
	int GameTypeID = CMainLogic::sharedMainLogic()->KIND_ID;
	
	//@@用于debug，一般不发生
    if (GameTypeID==27 && pUserData->wChairID >= GAME_NIU_PLAYER) {
        ERROR_CHECK;
        return true;
    }
    else if (GameTypeID==26 && pUserData->wChairID >= GAME_NIU_PLAYER_2)
    {
        ERROR_CHECK;
        return true;
    }
    else if (GameTypeID==NZNH_KIND_ID && pUserData->wChairID >= GAME_PLAYER)
    {
        ERROR_CHECK;
        return true;
    }
    else if (GameTypeID==YQS_KIND_ID && pUserData->wChairID >= GAME_PLAYER)
    {
        ERROR_CHECK;
        return true;
    }
    else if (GameTypeID==JCBY_KIND_ID && pUserData->wChairID >= GAME_PLAYER)
    {
        ERROR_CHECK;
        return true;
    }
    
	if (m_pIClientKernelSink!=NULL)
	{
		m_pIClientKernelSink->OnEventUserLeave(pUserData, pUserData->wChairID, false);
	}

	WORD wChairID = pUserData->wChairID;
	delete m_pUserItem[wChairID];
	m_pUserItem[wChairID] = NULL;

	return true;
}

//更新用户
bool CClientKernel::UpdateUserItemScore(DWORD dwUserID, const tagUserScore * pUserScore)
{
	tagUserInfo * pUserInfo=SearchUserItem(dwUserID);
	if (pUserInfo==NULL) return false;

	pUserInfo->lScore=pUserScore->lScore;
	pUserInfo->lGrade=pUserScore->lGrade;
	pUserInfo->lInsure=pUserScore->lInsure;
	pUserInfo->dwWinCount=pUserScore->dwWinCount;
	pUserInfo->dwLostCount=pUserScore->dwLostCount;
	pUserInfo->dwDrawCount=pUserScore->dwDrawCount;
	pUserInfo->dwFleeCount=pUserScore->dwFleeCount;
	pUserInfo->dwUserMedal=pUserScore->dwUserMedal;
	pUserInfo->dwExperience=pUserScore->dwExperience;
	pUserInfo->lLoveLiness=pUserScore->lLoveLiness;

	if (m_pIClientKernelSink!=NULL)
	{
		m_pIClientKernelSink->OnEventUserScore(pUserInfo, pUserInfo->wChairID, false);
	}

	return true;
}

//更新用户
bool CClientKernel::UpdateUserItemStatus(DWORD dwUserID, tagUserStatus * pUserStatus, WORD wNetDelay)
{
	tagUserInfo * pUserInfo=SearchUserItem(dwUserID);
	if (pUserInfo==NULL) return false;

	//离开判断
	if (pUserStatus->cbUserStatus==US_NULL || pUserStatus->cbUserStatus==US_FREE)
	{
		if (pUserInfo != m_pMeUserItem)
		{
			DeleteUserItem(dwUserID);
		}
		return true;
	}

	pUserInfo->wTableID=pUserStatus->wTableID;
	pUserInfo->wChairID=pUserStatus->wChairID;
	pUserInfo->cbUserStatus=pUserStatus->cbUserStatus;

	bool bLookonUser=(pUserStatus->cbUserStatus==US_LOOKON);

	if (m_pIClientKernelSink!=NULL)
	{
		m_pIClientKernelSink->OnEventUserStatus(pUserInfo, pUserInfo->wChairID, bLookonUser);
	}

	return false;
}

bool CClientKernel::OnGameSocket(const TCP_Command & Command, void * pData, unsigned short wDataSize)
{
    //TCP_Command 已经分离
//	if (wDataSize<sizeof(TCP_Command))
//	{
//        //printf("wDataSize is %hu, sizeof(TCP_Command) is %lu\n", wDataSize, sizeof(TCP_Command));
//		ERROR_CHECK;
//		return false;
//	}

	if (m_pIClientKernelSink==NULL)
	{
		ERROR_CHECK;
		return true;
	}

	bool bResult=m_pIClientKernelSink->OnEventSocket(Command,pData,wDataSize);

	//框架消息
	if ((bResult==false)&&(Command.wMainCmdID==MDM_GF_FRAME))
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GF_GAME_STATUS:		//游戏状态
			{
				if (wDataSize!=sizeof(CMD_GF_GameStatus))
				{
					ERROR_CHECK;
					return false;
				}

				CMD_GF_GameStatus * pGameStatus=(CMD_GF_GameStatus *)pData;

				m_cbGameStatus=pGameStatus->cbGameStatus;

				return true;
			}
		case SUB_GF_GAME_SCENE:			//游戏场景
			{
				if (m_pIClientKernelSink!=NULL)
				{
					m_pIClientKernelSink->OnEventGameScene(m_cbGameStatus,false,pData,wDataSize);
				}
				else
				{
					ERROR_CHECK;
				}

				return true;
			}
		case SUB_GF_SYSTEM_MESSAGE:		//系统消息
			{
				CMD_CM_SystemMessage * pMessage=(CMD_CM_SystemMessage *)pData;
				if (wDataSize<=(sizeof(CMD_CM_SystemMessage)-sizeof(pMessage->szString)))
				{
					ERROR_CHECK;
					return false;
				}
                
                WORD wType = pMessage->wType;
                
                //@@捕鱼消息
                if (wType&SMT_CATCH_FISH)
                {
                    m_pIClientKernelSink->OnEventSocket(Command,pData,wDataSize);
                }

				if (pMessage->wType&SMT_CLOSE_GAME)
				{
					if (m_pIClientKernelSink!=NULL)
						m_pIClientKernelSink->CloseGameFrame();
				}

				return true;
			}
		case SUB_GF_USER_READY:			//用户准备
			{
				if (m_pMeUserItem==NULL || m_pMeUserItem->cbUserStatus>=US_READY)
					return true;

				SendUserReady(NULL,0);
				return true;
			}
		}
	}

	return true;
}

WORD CClientKernel::SwitchViewChairID(WORD wChairID)
{
    if (wChairID==INVALID_CHAIR) return INVALID_CHAIR;
    
    WORD wChairCount=4;
    WORD wViewChairID=(wChairID  + wChairCount*4/3)%wChairCount;
    
    return wViewChairID;
    
}
