#include "header.h"

bool CMainLogic::OnSocketRoom(const TCP_Command & Command, const void * pData, unsigned short wDataSize)
{
	bool bRet = false;
	switch (Command.wMainCmdID)
	{
	case MDM_KN_COMMAND:
		{
            if(Command.wSubCmdID == SUB_KN_DETECT_SOCKET)
            {
                bRet = SendData(m_nRoomSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET);
            }
            else if(Command.wSubCmdID == SUB_KN_DETECT_MANUAL)
            {
                //printf("111112\n");
                //收到检测包，停止定时器
                if(GameLayer::_instance != NULL)
                {
                    GameLayer::_instance->unschedule(schedule_selector(GameLayer::disConnectGame));
                    bRet = true;
                }
                else
                {
                    if(GameMainScene::_instance != NULL)
                    {
                        GameMainScene::_instance->unschedule(schedule_selector(GameMainScene::disConnectGame));
                    }
                    CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
                    pScheduler->unscheduleSelector(schedule_selector(CMainLogic::openCheckReceiveGameMes), this);
                    bRet = true;
                }
            }
		}
		break;
	case MDM_GR_LOGON:
		{
			////printf("登录消息\n");
			bRet = OnSocketMainLogon(Command, pData, wDataSize);
		}
		break;
	case MDM_GR_CONFIG:
		{
            bRet = OnSocketMainConfig(Command, pData, wDataSize);
		}
		break;
	case MDM_GR_USER:
		{
			////printf("用户信息\n");
			bRet = OnSocketMainUser(Command, pData, wDataSize);
		}
		break;
	case MDM_GR_STATUS:
		{
			////printf("状态信息\n");
			bRet = OnSocketMainStatus(Command, pData, wDataSize);
		}
		break;
	case MDM_GR_INSURE:
		{
			////printf("银行消息\n");
			bRet = OnSocketMainInsure(Command, pData, wDataSize);
		}
		break;
	case MDM_GR_MANAGE:
		{
			////printf("管理消息\n");
            if(Command.wSubCmdID == SUB_GR_CLOWN_FAKEANNOUNCEMENT)
            {
                if (wDataSize != sizeof(CMD_GR_CLOWNFAKEANNOUNCE))
                {
                    return false;
                }
                CMD_GR_CLOWNFAKEANNOUNCE* pNotice = (CMD_GR_CLOWNFAKEANNOUNCE*)pData;
                if(GameLayer::_instance != NULL)
                {
                    GameLayer::_instance->showNotice(CCharCode::GB2312ToUTF8(pNotice->szClownFakeAnnounceMent));
                }
            }
            if(Command.wSubCmdID == SUB_GR_CLOWN_BIG_REWARD)
            {
                if (wDataSize != sizeof(CMD_GR_CLOWNBIGREWARD))
                {
                    return false;
                }
                CMD_GR_CLOWNBIGREWARD* nBigWard = (CMD_GR_CLOWNBIGREWARD*)pData;
                m_lSuperPirateWard = nBigWard->lBigReward;
                if(GameLayer::_instance != NULL)
                {
                    GameLayer::_instance->changeBigPrize(m_lSuperPirateWard);
                }
            }
            bRet = true;
		}
		break;
	case MDM_CM_SYSTEM:
		{
			//printf("系统消息\n");
            CMD_CM_SystemMessage* pSystemMessage = (CMD_CM_SystemMessage*)pData;
            WORD wHeadSize = sizeof(CMD_CM_SystemMessage) - sizeof(pSystemMessage->szString);
			if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR))))
            {
                bRet = false;
            }
            WORD wType = pSystemMessage->wType;
            //@@捕鱼消息
            if (wType&SMT_CATCH_FISH)
            {
                TCP_Command MessageCommand;
                MessageCommand.wSubCmdID = SUB_GF_SYSTEM_MESSAGE;
                MessageCommand.wMainCmdID = Command.wMainCmdID;
               
                if (CClientKernel::m_ClientKernel != NULL)
                {
                    bRet = CClientKernel::m_ClientKernel->OnGameSocket(MessageCommand,(void*)pData,wDataSize);
                    
                    //游戏内公告
                    CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
                    if (wDataSize<=(sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString))) return false;
                    if (pSystemMessage->wType&0x0020)
                    {
                        if(GameMainScene::_instance)
                        {
                            if(GameMainScene::_instance->m_bEnterGameScene)
                            {
                                //setNotice(CCharCode::GB2312ToUTF8(pSystemMessage->szString), false, "", true);
                                if(GameMainScene::_instance != NULL)//在游戏中
                                {
                                    if(m_bIOSPassed)
                                    {
                                        ShowNotice(CCharCode::GB2312ToUTF8(pSystemMessage->szString));
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    ERROR_CHECK;
                    bRet = true;
                }
            }
            //显示消息
            if (wType&SMT_CHAT && wType&SMT_CLOSE_LINK)
            {
                if(GameLayer::_instance != NULL)
                {
                    ShowMessage(CCharCode::GB2312ToUTF8(pSystemMessage->szString).c_str(), eMBSuperOK, eDisConnectNet);
                }
                else if(GameMainScene::_instance != NULL)
                {
                    ShowMessage(CCharCode::GB2312ToUTF8(pSystemMessage->szString).c_str(), eMBOK, eDisConnectNet);
                }
                
                m_bAutoExitGame = true;
            }
            return bRet;
		}
		break;
	case MDM_GF_GAME:
	case MDM_GF_FRAME:
		{
			////printf("游戏消息\n");
			////printf("框架消息\n");
            if (CClientKernel::m_ClientKernel != NULL)
            {
                bRet = CClientKernel::m_ClientKernel->OnGameSocket(Command,(void*)pData,wDataSize);
            }
            else
            {
				ERROR_CHECK;
                bRet = true;
            }
		}
		break;
	case MDM_GR_MATCH:
		{
			////printf("比赛消息\n");
			bRet = true;
		}
		break;
    case MDM_GR_SKILL:
        {
            switch (Command.wSubCmdID)
            {
                case SUB_GRO_QUERY_SKILL://在房间内查询技能
                {
                    bRet = OnRoomQuerySkill(pData, wDataSize);
                    break;
                }
                case SUB_GRO_USE_SKILL:
                {
                    bRet = OnRoomUseSkill(pData, wDataSize);
                    break;
                }
                case SUB_GRO_GIVE_SKILL:
                {
                    bRet = OnRoomGiveSkill(pData, wDataSize);
                    break;
                }
                default:
                {
                    bRet = true;
                    break;
                }
            }
            break;
        }
	}

	if (!bRet)
	{
		////printf("Command.wMainCmdID=%d\n",Command.wMainCmdID);
		ERROR_CHECK;
	}

	return bRet;
}

bool CMainLogic::OnRoomQuerySkill(const void * pData, unsigned short wDataSize)
{
    if (wDataSize < sizeof(CMD_GRO_QuerySkill))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRO_QuerySkill* pQuerySkill = (CMD_GRO_QuerySkill*)pData;
    if(pQuerySkill != NULL)
    {
        m_nCannonStatus = pQuerySkill->nCannonStatus;
        m_nSpeedUpTimes = pQuerySkill->nJiaSu;
        m_nSansheTimes = pQuerySkill->nSanshe;
        m_nBaojiTimes = pQuerySkill->nBaoji;
        if(GameMainScene::_instance != NULL && GameMainScene::_instance->m_bEnterGameScene)
        {
            GameMainScene::_instance->resetSkillStauts();
        }
    }
    return true;
}

bool CMainLogic::OnRoomGiveSkill(const void * pData, unsigned short wDataSize)
{
    if (wDataSize < sizeof(CMD_GRO_GiveSkill))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRO_GiveSkill* pGiveSkill = (CMD_GRO_GiveSkill*)pData;
    if(pGiveSkill != NULL)
    {
        if(GameMainScene::_instance != NULL)
        {
            if(m_bIOSPassed)
            {
                GameMainScene::_instance->showSkillStatus(pGiveSkill->nSkillID, pGiveSkill->nCount);
            }
            
        }
    }
    return true;
}

bool CMainLogic::OnRoomUseSkill(const void * pData, unsigned short wDataSize)
{
    if (wDataSize < sizeof(CMD_GRO_UseSkill))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRO_UseSkill* pUseSkill = (CMD_GRO_UseSkill*)pData;
    if(pUseSkill != NULL)
    {
        if(GameMainScene::_instance != NULL)
        {
            //printf("nSkillID is %hhu, count is %d\n", pUseSkill->nSkillID, pUseSkill->nCount);
            if(pUseSkill->nSkillID>=0)
            {
                GameMainScene::_instance->setSkillStatus(pUseSkill->nSkillID, pUseSkill->nCount);
            }
        }
    }
    return true;
}

bool CMainLogic::OnSocketMainUser(const TCP_Command & Command, const void * pData, unsigned short wDataSize)
{
	bool bRet = false;

	switch (Command.wSubCmdID)
	{
	case SUB_GR_USER_ENTER:			//用户进入
		{
			bRet = OnSocketSubUserEnter(pData,wDataSize);
		}
		break;
	case SUB_GR_USER_SCORE:			//用户积分
		{
			bRet = OnSocketSubUserScore(pData,wDataSize);
		}
		break;
	case SUB_GR_USER_STATUS:		//用户状态
		{
			bRet = OnSocketSubUserStatus(pData,wDataSize);
		}
		break;
    case SUB_GR_REQUEST_FAILURE:
        {
            CMD_GR_RequestFailure * pRequestFailure = (CMD_GR_RequestFailure *)pData;
            
            std::string sFailure = CCharCode::GB2312ToUTF8(pRequestFailure->szDescribeString);
            std::string str = "捷足先登";
            std::string::size_type idx = sFailure.find(str);
            std::string str2 = "您正在游戏中";
            std::string::size_type idx2 = sFailure.find(str2);
            if(idx != std::string::npos)
            {
                //printf("111111111   222222222\n");
                m_pMessageBox->removeVector();
                ShowMessage("正在为您分配房间，请耐心等待...", eMBLabaNull);
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
            else if(idx2 != std::string::npos)
            {
                ShowMessage(sFailure.c_str(), eMBOK, eMBBackHall);
            }
            else
            {
                ShowMessage(sFailure.c_str(), eMBOK, eMBBackHall);
            }
            bRet = true;
        }
        break;
    case 307:
        {
            //CMD_GR_S_SendTrumpet* SendTrumpet = (CMD_GR_S_SendTrumpet*)pData;
            
            //获取当前场景，然后显示内容
            m_pMessageBox->Update();
            CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(21);
            //setNotice(SendTrumpet->szTrumpetContent, true, CCharCode::GB2312ToUTF8(SendTrumpet->szSendNickName).c_str(), true);
            if(GameMainScene::_instance != NULL)//在游戏中
            {
                //ShowNotice(SendTrumpet->szTrumpetContent);
            }
            bRet = true;
        }
        break;
    case 309:
        {
            CMD_CS_C_VIPUpgrade* pVIPUpgrade = (CMD_CS_C_VIPUpgrade *)pData;
            std::string sContent = CCharCode::GB2312ToUTF8(pVIPUpgrade->szTrumpetContent);
            bRet = true;
        }
        break;
    case 302:
        {
            CMD_GR_PropertyFailure* propertyFailure = (CMD_GR_PropertyFailure*)pData;
            CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8( propertyFailure->szDescribeString).c_str(), eMBOK);
            bRet = true;
            break;
        }
    case 500://收到服务端的站起消息
        {
            exitGameToHall();//手动返回到大厅
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            SendData(m_nRoomSocketHandle, MDM_KN_COMMAND, SUB_KN_SHUT_MANUAL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			SendData(m_pNetworkService->GetNetInfo(m_nRoomSocketHandle), MDM_KN_COMMAND, SUB_KN_SHUT_MANUAL);
#endif
            bRet = true;
            break;
        }
    case SUB_GR_MATCH_SIGNUP_RES://比赛报名
        {
            bRet = OnMatchSignupRes(pData, wDataSize);
            break;
        }
    case SUB_GR_MATCH_BEGIN://比赛开始
        {
            bRet = OnMatchBegin(pData, wDataSize);
            break;
        }
    case SUB_GR_MATCH_END://比赛开始
        {
            bRet = OnMatchEnd(pData, wDataSize);
            break;
        }
    case SUB_GR_MATCH_SCORE_UPDATE://更新比赛分数排名
        {
            bRet = OnMatchScoreUpdate(pData, wDataSize);
            break;
        }
    case SUB_GR_MATCH_GET_MATCH_PRIZE_RES://领取比赛奖励
        {
            bRet = OnMatchGetPriseRes(pData, wDataSize);
            break;
        }
    case SUB_GR_MATCH_INFO_NOTIFY://比赛信息前几分钟通知
        {
            bRet = OnMatchInfoNotify(pData, wDataSize);
            break;
        }
    case SUB_GR_MATCH_TIME_NOTIFY://比赛倒计时提醒
        {
            bRet = OnMatchTimeNotify(pData, wDataSize);
            break;
        }
    case SUB_GR_MATCH_SHAREINFO_RES:
        {
            bRet = OnMatchShareInfo(pData, wDataSize);
            break;
        }
    case SUB_GR_BUY_SKILL_RES://游戏内金币购买技能
        {
            bRet = OnGameBuySkillRes(pData, wDataSize);
            break;
        }
    case SUB_GR_USE_LABA_RES://游戏内使用喇叭
        {
            bRet = OnGameUserLaba(pData, wDataSize);
            break;
        }
    case SUB_GRO_QUERYBANKCUPTINFO:
        {
            bRet = OnQueryBankCuptinfo(pData, wDataSize);
            break;
        }
    case SUB_GRO_GET_BANKCRUPT_SCORE:
        {
            bRet = OnGetBankCuptScore(pData, wDataSize);
            break;
        }
    case SUB_GRO_HAVE_MATCH_PRIZE_REMIND:
        {
            bRet = OnHaveMatchPrizeRemind(pData, wDataSize);
            break;
        }
    case SUB_GRO_CAN_MATCH_SHARE:
        {
            bRet = OnCanMatchShare(pData, wDataSize);
            break;
        }
    case SUB_GRO_QIANG_RESULT:
        {
            bRet = OnQiangGameResult(pData, wDataSize);
            break;
        }
    case SUB_GRO_ONLINE_REWARD_INFO:
        {
            bRet = OnOnlineReWardInfo(pData, wDataSize);
            break;
        }
    case SUB_GRO_RECEIVE_ONLINE_REWARD:
        {
            bRet = OnReceiveOnlineReward(pData, wDataSize);
            break;
        }
	}

	if (!bRet)
	{
		ERROR_CHECK;
        //printf("Command.wSubCmdID=%d\n", Command.wSubCmdID);
	}

	return bRet;
}

bool CMainLogic::OnSocketMainLogon(const TCP_Command & Command, const void * pData, unsigned short wDataSize)
{
	bool bRet = false;
	switch (Command.wSubCmdID)
	{
	case SUB_GR_LOGON_SUCCESS:	//登录成功
		{
			//ShowMessage("正在读取房间信息...");
			bRet = true;
		}
		break;
	case SUB_GR_LOGON_FAILURE:	//登录失败
		{
			return OnSocketSubLogonFailure(pData,wDataSize);
		}
		break;
	case SUB_GR_LOGON_FINISH:	//登录完成
		{
			return OnSocketSubLogonFinish(pData,wDataSize);
		}
		break;
	default:
		{
			bRet = true;
		}
	}

	return bRet;
}

bool CMainLogic::OnSocketSubUserEnter(const void * pData, WORD wDataSize)
{
	if (wDataSize<sizeof(tagUserInfoHead))
	{
		ERROR_CHECK;
		return false;
	}

	tagUserInfo * pUserInfo = new tagUserInfo;
	memset(pUserInfo,0,sizeof(tagUserInfo));

	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pData;

	bool bHideUserInfo = false;
	bool bMySelfUserItem = (m_nUserID==pUserInfoHead->dwUserID);
	bool bMasterUserOrder = false;

	//读取信息
	if ((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		//用户属性
		pUserInfo->wFaceID=pUserInfoHead->wFaceID;
		pUserInfo->dwGameID=pUserInfoHead->dwGameID;
		pUserInfo->dwUserID=pUserInfoHead->dwUserID;
		pUserInfo->dwGroupID=pUserInfoHead->dwGroupID;
		pUserInfo->dwCustomID=pUserInfoHead->dwCustomID;

		//用户状态
		pUserInfo->wTableID=pUserInfoHead->wTableID;
		pUserInfo->wChairID=pUserInfoHead->wChairID;
		pUserInfo->cbUserStatus=pUserInfoHead->cbUserStatus;

		//用户属性
		pUserInfo->cbGender=pUserInfoHead->cbGender;
		pUserInfo->cbMemberOrder=pUserInfoHead->cbMemberOrder;
		pUserInfo->cbMasterOrder=pUserInfoHead->cbMasterOrder;

		//用户积分
		pUserInfo->lScore=pUserInfoHead->lScore;
		pUserInfo->lGrade=pUserInfoHead->lGrade;
		pUserInfo->lInsure=pUserInfoHead->lInsure;
		pUserInfo->dwWinCount=pUserInfoHead->dwWinCount;
		pUserInfo->dwLostCount=pUserInfoHead->dwLostCount;
		pUserInfo->dwDrawCount=pUserInfoHead->dwDrawCount;
		pUserInfo->dwFleeCount=pUserInfoHead->dwFleeCount;
		pUserInfo->dwUserMedal=pUserInfoHead->dwUserMedal;
		pUserInfo->dwExperience=pUserInfoHead->dwExperience;
		pUserInfo->lLoveLiness=pUserInfoHead->lLoveLiness;

		//变量定义
		VOID * pDataBuffer=NULL;
		tagDataDescribe DataDescribe;
		CRecvPacketHelper RecvPacket(pUserInfoHead+1,wDataSize-sizeof(tagUserInfoHead));

		//扩展信息
		while (true)
		{
			pDataBuffer=RecvPacket.GetData(DataDescribe);
			if (DataDescribe.wDataDescribe==DTP_NULL) break;
			switch (DataDescribe.wDataDescribe)
			{
			case DTP_GR_NICK_NAME:		//用户昵称
				{
					if (pDataBuffer!=NULL && DataDescribe.wDataSize<=sizeof(pUserInfo->szNickName))
					{
						memcpy(&pUserInfo->szNickName,pDataBuffer,DataDescribe.wDataSize);
						pUserInfo->szNickName[CountArray(pUserInfo->szNickName)-1]=0;
					}
					else
					{
						ERROR_CHECK;
					}
					break;
				}
			case DTP_GR_GROUP_NAME:		//用户社团
				{
					if (pDataBuffer!=NULL && DataDescribe.wDataSize<=sizeof(pUserInfo->szGroupName))
					{
						memcpy(&pUserInfo->szGroupName,pDataBuffer,DataDescribe.wDataSize);
						pUserInfo->szGroupName[CountArray(pUserInfo->szGroupName)-1]=0;
					}
					else
					{
						ERROR_CHECK;
					}
					break;
				}
			case DTP_GR_UNDER_WRITE:	//个性签名
				{
					if (pDataBuffer!=NULL && DataDescribe.wDataSize<=sizeof(pUserInfo->szUnderWrite))
					{
						memcpy(pUserInfo->szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
						pUserInfo->szUnderWrite[CountArray(pUserInfo->szUnderWrite)-1]=0;
					}
					else
					{
						ERROR_CHECK;
					}
					break;
				}
			}
		}
	}
	else//是防作弊&&不是我&&不是管理员
	{
		//用户信息
		pUserInfo->dwUserID=pUserInfoHead->dwUserID;
		strcpy(pUserInfo->szNickName,"游戏玩家");

		//用户状态
		pUserInfo->wTableID=pUserInfoHead->wTableID;
		pUserInfo->wChairID=pUserInfoHead->wChairID;
		pUserInfo->cbUserStatus=pUserInfoHead->cbUserStatus;

		//用户属性
		pUserInfo->cbGender=pUserInfoHead->cbGender;
		pUserInfo->cbMemberOrder=pUserInfoHead->cbMemberOrder;
		pUserInfo->cbMasterOrder=pUserInfoHead->cbMasterOrder;
	}

	//是否存在
	tagUserInfo * pUserInfo2 = SearchUserByUserID(pUserInfo->dwUserID);

	//不存在，则添加
	if (NULL == pUserInfo2)
	{
        ActiveUserItem(pUserInfo);
	}

	////////////////////////////////////////////////////////////////////////////////

	if (pUserInfo->dwUserID == m_nUserID)
	{
		//CClientKernel::m_ClientKernel->SetMeUserInfo(pUserInfo);
		m_pMeUserItem = pUserInfo;
	}

	return true;
}

bool CMainLogic::OnSocketSubUserScore(const void * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_GR_UserScore))
	{
		ERROR_CHECK;
		return false;
	}

	//寻找用户
	CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)pData;
	tagUserInfo * pUserInfo=SearchUserByUserID(pUserScore->dwUserID);
	tagUserScore & UserScore = pUserScore->UserScore;
    
    //printf("jifen is %lld\n", pUserScore->UserScore.lScore);

	//用户判断
	if (pUserInfo==NULL)
	{
		ERROR_CHECK;
		return true;
	}

	bool bMySelfUserItem=(m_nUserID==pUserScore->dwUserID);
	bool bAvertCheatMode=false;
	bool bMasterUserOrder=false;

	//更新用户
	if ((bAvertCheatMode==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		pUserInfo->lScore=UserScore.lScore;
		pUserInfo->lGrade=UserScore.lGrade;
		pUserInfo->lInsure=UserScore.lInsure;
		pUserInfo->dwWinCount=UserScore.dwWinCount;
		pUserInfo->dwLostCount=UserScore.dwLostCount;
		pUserInfo->dwDrawCount=UserScore.dwDrawCount;
		pUserInfo->dwFleeCount=UserScore.dwFleeCount;
		pUserInfo->dwUserMedal=UserScore.dwUserMedal;
		pUserInfo->dwExperience=UserScore.dwExperience;
		pUserInfo->lLoveLiness=UserScore.lLoveLiness;
	}
    //更新大厅内的分数
    if(pUserInfo->dwUserID == m_nUserID)
    {
        m_lUserScore = pUserInfo->lScore;
    }

	return true;
}

bool CMainLogic::OnSocketSubUserStatus(const void * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_GR_UserStatus))
	{
		ERROR_CHECK;
		return false;
	}

	//寻找用户
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pData;
	tagUserInfo * pUserInfo=SearchUserByUserID(pUserStatus->dwUserID);

	//用户判断
	if (pUserInfo==NULL)
	{
		ERROR_CHECK;
		return true;
	}

    //保存更新前的状态
    tagUserStatus UserStatus = {0};
    UserStatus.wTableID = pUserInfo->wTableID;
    UserStatus.wChairID = pUserInfo->wChairID;
    UserStatus.cbUserStatus = pUserInfo->cbUserStatus;

	//设置状态
	if (pUserStatus->UserStatus.cbUserStatus==US_NULL) 
	{
		//删除用户
		DeleteUserItem(pUserInfo->dwUserID);
	}
	else
	{
		//更新用户
		pUserInfo->wTableID=pUserStatus->UserStatus.wTableID;
		pUserInfo->wChairID=pUserStatus->UserStatus.wChairID;
		pUserInfo->cbUserStatus=pUserStatus->UserStatus.cbUserStatus;
        
        OnUserItemUpdate(pUserInfo, UserStatus);
	}

    if (CClientKernel::m_ClientKernel != NULL)
    {
        if (pUserInfo != m_pMeUserItem && pUserInfo->wTableID == m_pMeUserItem->wTableID && pUserInfo->wChairID < MAX_CHAIR)
        {
            CClientKernel::m_ClientKernel->ActiveUserItem(pUserInfo);
        }

        CClientKernel::m_ClientKernel->UpdateUserItemStatus(pUserStatus->dwUserID, &pUserStatus->UserStatus, 0);
    }

	return true;
}

void CMainLogic::OnUserItemUpdate(tagUserInfo * pUserInfo, tagUserStatus & LastStatus)
{
    WORD wNowTableID = pUserInfo->wTableID, wLastTableID = LastStatus.wTableID;
    WORD wNowChairID = pUserInfo->wChairID, wLastChairID = LastStatus.wChairID;
    BYTE cbNowStatus = pUserInfo->cbUserStatus, cbLastStatus = LastStatus.cbUserStatus;

    CTableLayer * layer = NULL;
    if (m_pTableScene != NULL)
    {
        layer = (CTableLayer *)m_pTableScene->getChildByTag(eTableLayer);
    }

    //桌子离开
    if ((wLastTableID != INVALID_TABLE) && ((wLastTableID != wNowTableID) || (wLastChairID != wNowChairID)))
    {
        tagUserInfo * pTempUserInfo = NULL;
        if (layer != NULL)
        {
            pTempUserInfo = layer->GetClientUserItem(wLastTableID, wLastChairID);
        }
        if (pTempUserInfo == pUserInfo && layer != NULL)
        {
            layer->SetClientUserItem(wLastTableID, wLastChairID, NULL);
        }
    }
    //桌子加入
    if ((wNowTableID != INVALID_TABLE) && (cbNowStatus != US_LOOKON) && ((wNowTableID != wLastTableID) || (wNowChairID != wLastChairID)))
    {
        
        if (layer != NULL)
        {
            layer->SetClientUserItem(wNowTableID, wNowChairID, pUserInfo);
        }
        if(m_pMeUserItem != NULL)
        {
            if (pUserInfo->dwUserID == m_pMeUserItem->dwUserID)
            {
                m_pMeUserItem->wTableID=pUserInfo->wTableID;
                m_pMeUserItem->wChairID=pUserInfo->wChairID;
                CMainLogic::sharedMainLogic()->setPlaying(true);
                m_pTableScene = NULL;
                //m_nGameType == 1代表金蟾，2代表海盗，3代表副本，4代表李逵劈鱼、5代表哪吒闹海、6代表跑狗王
                if(m_nGameType == 1 || m_nGameType == 3 || m_nGameType == 4 || m_nGameType == 5)
                {
                    //进入捕鱼游戏
                    CCDirector::sharedDirector()->replaceScene(LoadRes::scene());
                }
                else if(m_nGameType == 2)//进入小丑
                {
                    CCDirector::sharedDirector()->replaceScene(GameLayer::scene());
                }
                else if(m_nGameType == 6)//跑狗王
                {
                    CCDirector::sharedDirector()->replaceScene(DogBeteLayer::scene());
                }
                else if(m_nGameType == 7)//红黑大战
                {
                    CCScene* scene = H3CardBeteLayer::scene();
                    CCDirector::sharedDirector()->replaceScene(scene);
                }
            }
        }
    }
}

//登录失败
bool CMainLogic::OnSocketSubLogonFailure(const void * pData, WORD wDataSize)
{
	CMD_GR_LogonFailure * pLogonFailure=(CMD_GR_LogonFailure *)pData;
	if (wDataSize<(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString)))
	{
		ERROR_CHECK;
		return false;
	}

	////关闭处理
	//m_TCPSocketModule->CloseSocket();

	//显示消息
    ShowMessage(CCharCode::GB2312ToUTF8(pLogonFailure->szDescribeString).c_str());

	////关闭房间
	//PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

bool CMainLogic::OnSocketSubLogonFinish(const void * pData, WORD wDataSize)
{
    ////printf("%s %d\n", __FUNCTION__, __LINE__);
	//SendSitDownPacket(0, 0, "");
    //CCDirector::sharedDirector()->replaceScene(LoadRes::scene());//pushScene
    //@@@@断线重连
    //SendSitDownPacket(0, 0, "");
    if(GameMainScene::_instance == NULL)
    {
        if (m_pMeUserItem->cbUserStatus == 5)
        {
            m_pTableScene = NULL;
            if(m_nGameType == 1 || m_nGameType == 3 || m_nGameType == 4 || m_nGameType == 5)
            {
                CCDirector::sharedDirector()->replaceScene(LoadRes::scene());//pushScene
            }
            else if(m_nGameType == 2)
            {
                CCDirector::sharedDirector()->replaceScene(GameLayer::scene());//pushScene
            }
        }
    }
    else
    {
//        //重连成功
//        CheckNetworkLayer* pNetworkLayer = (CheckNetworkLayer*)GameMainScene::_instance->getChildByTag(30001);
//        if(pNetworkLayer != NULL)
//        {
//            pNetworkLayer -> removeFromParent();
//        }
//        GameMainScene::_instance->unschedule(schedule_selector(GameMainScene::scheduleGameLink));
//        GameMainScene::_instance->schedule(schedule_selector(GameMainScene::outGameCount), 1.0f);
//        SendGameOption();
    }

    
	return true;
}

//发送坐下
bool CMainLogic::SendSitDownPacket(unsigned short wTableID, unsigned short wChairID, char * szPassword)
{
    CMD_GR_UserSitDown UserSitDown = {0};
    
	UserSitDown.wTableID=wTableID;
	UserSitDown.wChairID=wChairID;
	if ((szPassword!=NULL)&&(szPassword[0]!=0))
	{
		strncpy(UserSitDown.szPassword,szPassword,sizeof(UserSitDown.szPassword));
	}

	SendData(m_nRoomSocketHandle,MDM_GR_USER,SUB_GR_USER_SITDOWN,&UserSitDown,sizeof(UserSitDown));

    //SendGameOption();
    
	return true;
}

void CMainLogic::BeforeGameStart()
{
    if (CClientKernel::m_ClientKernel == NULL)
    {
        ERROR_CHECK;
        return;
    }
    
    //CClientKernel::m_ClientKernel->SetMeUserInfo(m_pMeUserItem);
    
    CClientKernel::m_ClientKernel->m_dwUserID=m_nUserID;
    CClientKernel::m_ClientKernel->ActiveUserItem(m_pMeUserItem);
    
    //发送所有已经在游戏的玩家
    for (unsigned int i = 0; i<m_vUserInfo.size(); i++)
    {
        tagUserInfo * pUserInfo = m_vUserInfo[i];
        if (NULL == pUserInfo) continue;
        if (pUserInfo->wTableID!=m_pMeUserItem->wTableID) continue;
        if (pUserInfo->cbUserStatus==US_LOOKON) continue;
        
        CClientKernel::m_ClientKernel->ActiveUserItem(pUserInfo);
    }
}

void CMainLogic::SendGameOption()
{
    //发送配置完成
    CMD_GF_GameOption GameOption = {0};
    
    //构造数据
    GameOption.dwFrameVersion=VERSION_FRAME;
    GameOption.cbAllowLookon=false;
    GameOption.dwClientVersion=VERSION_FRAME;
    
    //发送数据
    SendData(m_nRoomSocketHandle,MDM_GF_FRAME,SUB_GF_GAME_OPTION,&GameOption,sizeof(GameOption));
}

bool CMainLogic::SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave)
{
    CMD_GR_UserStandUp UserStandUp = {0};
    
    UserStandUp.wTableID = wTableID;
    UserStandUp.wChairID = wChairID;
    UserStandUp.cbForceLeave = cbForceLeave;
    
    SendData(m_nRoomSocketHandle, MDM_GR_USER, SUB_GR_USER_STANDUP, &UserStandUp, sizeof(UserStandUp));
    
    return true;
}

bool CMainLogic::OnSocketMainInsure(const TCP_Command & Command, const void * pData, unsigned short wDataSize)
{
    bool bRet = false;
    
    switch (Command.wSubCmdID)
    {
        case SUB_GR_USER_INSURE_INFO:
        {
            bRet = OnSocketSubInsureInfo(pData,wDataSize);
            break;
        }
        case SUB_GR_USER_INSURE_SUCCESS:
        {
            bRet = OnSocketSubInsureSuccess(pData,wDataSize);
            break;
        }
        case SUB_GR_USER_INSURE_FAILURE:
        {
            bRet = OnSocketSubInsureFailure(pData,wDataSize);
            break;
        }
        case SUB_GR_USER_TRANSFER_INFO:
        {
            bRet = OnSocketSubTransferInfo(pData,wDataSize);
            break;
        }
        case SUB_GR_USER_TRANSFER_USER_INFO:
        {
            bRet = OnSocketSubTransferUserInfo(pData,wDataSize);
            break;
        }
        case SUB_GO_USER_TRANSRER_DRAGON:
        {
            bRet = OnSocketSubTransferDragon(pData, wDataSize);
            break;
        }
        case SUB_GPO_USER_GET_DRAGON:
        {
            bRet = OnSocketSubGetDragon(pData, wDataSize);
            break;
        }
    }
    
    if (!bRet)
    {
        ERROR_CHECK;
    }
    
    return bRet;
}

bool CMainLogic::OnSocketSubGetDragon(const void * pData, WORD wDataSize)
{
    CMD_GR_UserWinDragon* pUserWinDragon = (CMD_GR_UserWinDragon*)pData;
    if(pUserWinDragon != NULL)
    {
        
    }
    return true;
}

bool CMainLogic::OnSocketSubTransferDragon(const void * pData, WORD wDataSize)
{
    return true;
}

bool CMainLogic::OnSocketSubInsureInfo(const void * pData, WORD wDataSize)
{
    if (wDataSize < sizeof(CMD_GR_S_UserInsureInfo))
    {
        ERROR_CHECK;
        return false;
    }
    
    return true;
}

bool CMainLogic::OnSocketSubInsureSuccess(const void * pData, WORD wDataSize)
{
    CMD_GR_S_UserInsureSuccess * pUserInsureSuccess=(CMD_GR_S_UserInsureSuccess *)pData;
    if (wDataSize < (sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)))
    {
        return false;
    }
    
    
    return true;
}

bool CMainLogic::OnSocketSubInsureFailure(const void * pData, WORD wDataSize)
{
   
    return true;
}

bool CMainLogic::OnSocketSubTransferInfo(const void * pData, WORD wDataSize)
{
    return true;
}

bool CMainLogic::OnSocketSubTransferUserInfo(const void * pData, WORD wDataSize)
{
    
    return true;
}

void CMainLogic::transferScore()
{
    
}

void CMainLogic::transferScore2()
{
}

bool CMainLogic::OnSocketMainStatus(const TCP_Command & Command, const void * pData, unsigned short wDataSize)
{
    switch (Command.wSubCmdID)
    {
        case SUB_GR_TABLE_INFO:
        {
            CMD_GR_TableInfo * pTableInfo = (CMD_GR_TableInfo *)pData;
            WORD wHeadSize = sizeof(CMD_GR_TableInfo) - sizeof(pTableInfo->TableStatusArray);
            if ((wHeadSize + pTableInfo->wTableCount * sizeof(pTableInfo->TableStatusArray[0])) != wDataSize)
            {
                ERROR_CHECK;
                return false;
            }
            
            CTableLayer * layer = NULL;
            if (m_pTableScene != NULL)
            {
                layer = (CTableLayer *)m_pTableScene->getChildByTag(eTableLayer);
            }
            
            for (WORD i = 0; i < pTableInfo->wTableCount; i++)
            {
                if (layer != NULL)
                {
                    layer->setTableStatus(i, pTableInfo->TableStatusArray[i].cbPlayStatus);
                }
            }
            
            break;
        }
        case SUB_GR_TABLE_STATUS:
        {
            if (wDataSize != sizeof(CMD_GR_TableStatus))
            {
                ERROR_CHECK;
                return false;
            }
            CMD_GR_TableStatus * pTableStatus = (CMD_GR_TableStatus *)pData;
            
            CTableLayer * layer = NULL;
            if (m_pTableScene != NULL)
            {
                layer = (CTableLayer *)m_pTableScene->getChildByTag(eTableLayer);
            }
            
            if (m_pTableScene == NULL)
            {
                //ERROR_CHECK;
                return true;
            }
            
            if (layer != NULL)
            {
                layer->setTableStatus(pTableStatus->wTableID, pTableStatus->TableStatus.cbPlayStatus);
            }
            
            break;
        }
    }
    return true;
}

bool CMainLogic::OnSocketMainConfig(const TCP_Command & Command, const void * pData, unsigned short wDataSize)
{
    switch (Command.wSubCmdID)
    {
        case SUB_GR_CONFIG_SERVER:
        {
            if (wDataSize < sizeof(CMD_GR_ConfigServer))
            {
                ERROR_CHECK;
                return false;
            }
            if(GameMainScene::_instance != NULL)
            {
            }
            else
            {
                CMD_GR_ConfigServer * pConfigServer = (CMD_GR_ConfigServer *)pData;
                //printf("桌子数据： %d\n", pConfigServer->wTableCount);
                //printf("椅子数目： %d\n", pConfigServer->wChairCount);
                
                m_pTableScene = CTableLayer::scene(pConfigServer->wChairCount);
                CCDirector::sharedDirector()->replaceScene(m_pTableScene);
                m_pHallScene = NULL;
                m_pLogonScene = NULL;
                
                CTableLayer * layer = (CTableLayer *)m_pTableScene->getChildByTag(eTableLayer);
                
                if (layer != NULL)
                {
                    if(0)//if(m_pMeUserItem  != NULL && m_pMeUserItem->cbUserStatus == 5)
                    {
                        BeforeGameStart();
                        SendGameOption();
                    }
                    else
                    {
                        layer->configTable(pConfigServer->wTableCount, pConfigServer->wChairCount, m_sServerName.c_str(), true);
                    }
                    
                }
            }
            break;
        }
    }
    return true;
}

void CMainLogic::setNotice(std::string noticeStr, bool bContainIcon, std::string nickName, bool bMatch)
{
    if(GameMainScene::_instance)
    {
        if(strcmp(noticeStr.c_str(), ""))//
        {
            if(bMatch)
            {
                Trumpet* pTrumpet = new Trumpet;
                pTrumpet->noticeStr = noticeStr;
                pTrumpet->noticeNode = NULL;
                pTrumpet->bContainIcon = bContainIcon;
                pTrumpet->sNickName = nickName;
                m_vctNotice.push_back(pTrumpet);
                if (m_vctNotice.size() == 1)
                    noticeFunc();
            }
            else
            {
                if(GameMainScene::_instance->m_GameScene.nExercise == 0)//不是体验房，需要发送比赛公告
                {
                    Trumpet* pTrumpet = new Trumpet;
                    pTrumpet->noticeStr = noticeStr;
                    ////printf("公告is  %s\n", noticeStr.c_str());
                    pTrumpet->noticeNode = NULL;
                    pTrumpet->bContainIcon = bContainIcon;
                    pTrumpet->sNickName = nickName;
                    m_vctNotice.push_back(pTrumpet);
                    if (m_vctNotice.size() == 1)
                        noticeFunc();
                }
            }
            
        }
    }
}

void CMainLogic::noticeFunc()
{
    std::string currentStr = m_vctNotice[0]->noticeStr;
    m_vctNotice[0]->noticeNode = CCNode::create();
    //CCNode * pNode = CCDirector::sharedDirector()->getRunningScene();
    
    //if(pNode != NULL)
    //{
        if(GameMainScene::_instance)
        {
            GameMainScene::_instance -> addChild(m_vctNotice[0]->noticeNode, 28);
            if(GameMainScene::_instance->getMeRealChairID() >= GAME_PLAYER/2)
            {
                m_vctNotice[0]->noticeNode->setRotation(180);
                m_vctNotice[0]->noticeNode->setPosition(ccp(CLIENT_WIDTH, CLIENT_HEIGHT));
            }
        }
    //}
    
    if(m_vctNotice[0]->bContainIcon)//喇叭公告
    {
        char temp[128];
        sprintf(temp, "【%s】", m_vctNotice[0]->sNickName.c_str());
        CCLabelTTF* pLabelName = CCLabelTTF::create(temp, "Arial", 50);
        pLabelName -> setPosition(ccp(130+pLabelName->getContentSize().width/2, CLIENT_HEIGHT/4*3));
        m_vctNotice[0]->noticeNode->addChild(pLabelName, 2);
        pLabelName->setColor(ccc3(197, 174, 46));
        CCSprite* pSpIcon = CCSprite::create();//t2able/laba.png
        pSpIcon -> setPosition(ccp(20, pLabelName->getContentSize().height/2*2.5));
        pLabelName -> addChild(pSpIcon);
        pSpIcon -> runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f, ccp(0,12)), CCMoveBy::create(0.5f, ccp(0,-12)), NULL)));
        pSpIcon -> runAction(CCSequence::create(CCFadeIn::create(0.5), CCDelayTime::create(6.0), CCFadeOut::create(0.5),CCHide::create(), NULL));
        pLabelName -> runAction(CCSequence::create(CCFadeIn::create(0.5), CCDelayTime::create(6.0), CCFadeOut::create(0.5),CCHide::create(), NULL));
        
        CCSize size(CLIENT_WIDTH-220-pLabelName->getContentSize().width-50, 0);
        CCLabelTTF* pLabelGonggao = CCLabelTTF::create(currentStr.c_str(), "Arial", 50,size,kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
        pLabelGonggao -> setPosition(ccp((CLIENT_WIDTH-220-pLabelName->getContentSize().width-20)/2+120+pLabelName->getContentSize().width+5, CLIENT_HEIGHT/4*3));
        m_vctNotice[0]->noticeNode -> addChild(pLabelGonggao, 2);
        pLabelGonggao -> setDimensions(size);
        CCCallFuncN* callFunc = CCCallFuncN::create(this, callfuncN_selector(CMainLogic::noticeRemoveCall));
        pLabelGonggao->setColor(ccc3(197, 174, 46));
        
        
        pLabelGonggao -> runAction(CCSequence::create(CCFadeIn::create(0.5), CCDelayTime::create(6.0), CCFadeOut::create(0.5),CCHide::create(),callFunc, NULL));
        CCSize size1 = pLabelGonggao->getContentSize();
        CCScale9Sprite* pGameGongGaoBg = CCScale9Sprite::create("CatchFish01/gamegonggaobg.png");//公告背景图
        pGameGongGaoBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/4*3));
        pGameGongGaoBg->setContentSize(CCSizeMake(CLIENT_WIDTH, size1.height+40));
        m_vctNotice[0]->noticeNode -> addChild(pGameGongGaoBg);
        pGameGongGaoBg -> runAction(CCSequence::create(CCFadeIn::create(0.5), CCDelayTime::create(6.0), CCFadeOut::create(0.5),CCHide::create(), NULL));
    }
    else
    {
        CCSize size(CLIENT_WIDTH-240,0);
        
        CCLabelTTF* pLabelGonggao = CCLabelTTF::create(currentStr.c_str(), "Arial", 50,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
        pLabelGonggao -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/4*3));
        m_vctNotice[0]->noticeNode -> addChild(pLabelGonggao, 2);
        pLabelGonggao -> setDimensions(size);
        pLabelGonggao->setColor(ccc3(224, 208, 69));
        CCCallFuncN* callFunc = CCCallFuncN::create(this, callfuncN_selector(CMainLogic::noticeRemoveCall));
        pLabelGonggao -> runAction(CCSequence::create(CCFadeIn::create(0.5), CCDelayTime::create(3.0), CCFadeOut::create(0.5),CCHide::create(),callFunc, NULL));
        CCSize size1 = pLabelGonggao->getContentSize();
        CCScale9Sprite* pGameGongGaoBg = CCScale9Sprite::create("CatchFish01/gamegonggaobg.png");//公告背景图
        pGameGongGaoBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/4*3));
        pGameGongGaoBg->setContentSize(CCSizeMake(CLIENT_WIDTH, size1.height));
        m_vctNotice[0]->noticeNode -> addChild(pGameGongGaoBg);
        pGameGongGaoBg -> runAction(CCSequence::create(CCFadeIn::create(0.5), CCDelayTime::create(3.0), CCFadeOut::create(0.5),CCHide::create(), NULL));
    }
    
}

void CMainLogic::noticeRemoveCall(CCNode* node)
{
    Trumpet* pTrumpet = m_vctNotice[0];
    pTrumpet->noticeNode -> removeAllChildren();
    pTrumpet->noticeNode -> removeFromParentAndCleanup(true);
    m_vctNotice.erase(m_vctNotice.begin());
    delete pTrumpet;
    pTrumpet = NULL;
    if (m_vctNotice.size()>0)
        noticeFunc();
}

//比赛公告
void CMainLogic::openMatchNotice()
{
    if(GameMainScene::_instance != NULL)
    {
        CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
        pScheduler->unscheduleSelector(schedule_selector(CMainLogic::sendMatchNotice), this);
        pScheduler->scheduleSelector(schedule_selector(CMainLogic::sendMatchNotice), this, 300.0f, false);
        //this -> schedule(schedule_selector(CMainLogic::sendMatchNotice), 300.0f);
    }
}

void CMainLogic::closeMatchNotice()
{
    if(GameMainScene::_instance != NULL)
    {
        CCScheduler *pScheduler = CCDirector::sharedDirector()->getScheduler();
        pScheduler->unscheduleSelector(schedule_selector(CMainLogic::sendMatchNotice), this);
//        this -> unschedule(schedule_selector(CMainLogic::sendMatchNotice));
    }
}

void CMainLogic::sendMatchNotice(float dt)
{
    CCHttpRequest* request = new CCHttpRequest();
    char temp[128];
    sprintf(temp, "http://%s/GetMobileInfo.aspx", NORMAL_URL);
    request -> setUrl(temp);
    request -> setRequestType(CCHttpRequest::kHttpGet);
    request -> setTag("http test");
    request -> setResponseCallback(this, httpresponse_selector(CMainLogic::onHttpMatchNoticeComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CMainLogic::onHttpMatchNoticeComplete(CCHttpClient* sender, CCHttpResponse* response)
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
    
    //对str进行解析，如果value为false， 那么没有内容，不做任何操作，如果为true， 那么有比赛公告
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("mobileannouncements"))
        {
            return;
        }
        std::string type=rootElement->Attribute("value");
        if(!std::strcmp(type.c_str(), "true"))//相同
        {
            TiXmlElement * pCurrentElement=rootElement->FirstChildElement();
            const char * pValue=pCurrentElement->Value();
            if (!strcmp(pValue,"mobcontent"))
            {
                std::string noticeStr = pCurrentElement->GetText();
                //setNotice(noticeStr, false, "", false);//比赛公告
                if(m_pHallScene != NULL)//在大厅
                {
                    CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
                    if(pHallLayer != NULL)
                    {
                        //pHallLayer->m_vctStringNotice.push_back(noticeStr);
                    }
                }
                else if(GameMainScene::_instance != NULL)//在游戏中
                {
                    //ShowNotice(noticeStr);
                }
            }
        }
        else
        {
            delete document;
            return;
        }
    }
    delete document;
}

//比赛消息
bool CMainLogic::OnMatchSignupRes(const void * pData, WORD wDataSize)//比赛报名结果
{
    if(!m_bIOSPassed)
        return true;
    if(!m_bShowMatch)
        return true;
    if (wDataSize<sizeof(CMD_GRO_MatchSignupRes))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRO_MatchSignupRes* pMatchSignupRes = (CMD_GRO_MatchSignupRes*)pData;
    if(pMatchSignupRes != NULL)
    {
        printf("报名结果  %s\n", CCharCode::GB2312ToUTF8(pMatchSignupRes->szDescription).c_str());
        m_pMessageBox->Update();
//        printf("CMD_GRO_MatchSignupRes: %d %d %d %lld %s\n",pMatchSignupRes->bMatchResult, pMatchSignupRes->bMatchStatus, pMatchSignupRes->nMatchID, pMatchSignupRes->llUserScore, CCharCode::GB2312ToUTF8(pMatchSignupRes->szDescription).c_str());
        /*bool bMatchResult;//报名结果，0为报名失败，1为报名成功
         bool bMatchStatus;//报名状态，0为未报名，1为已经报名
         int nMatchID;//比赛场次
         SCORE llUserScore;//玩家当前金币
         char szDescription[128];//描述信息*/
        //判断比赛界面是否存在
        CRankNewLayer* pRankNewLayer = (CRankNewLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10200);
        if(pRankNewLayer != NULL)
        {
            if (pMatchSignupRes->bMatchResult || (pMatchSignupRes->bMatchResult==false && pMatchSignupRes->bMatchStatus))
            {
                pRankNewLayer->refreshMatchingInfo(1, pMatchSignupRes->nMatchID);//1代表 报名界面
            }
            CRankNewGuideLayer* pRankNewGuideLayer = (CRankNewGuideLayer*)pRankNewLayer->getChildByTag(9001);
            if (pRankNewGuideLayer != NULL) //比赛指引报名
            {
                pRankNewLayer->setJoinStatus(pMatchSignupRes->bMatchResult);
            }
            else//正常报名
            {
//                if (pMatchSignupRes->bMatchResult)
//                {
//                    pRankNewLayer->refreshMatchingInfo(1, pMatchSignupRes->nMatchID);//1代表 报名界面
//                }
            }
        }
        if(GameMainScene::_instance != NULL)
        {
            GameMainScene::_instance -> showMatchSignResult(pMatchSignupRes->bMatchResult, pMatchSignupRes->bMatchStatus, pMatchSignupRes->nMatchID, pMatchSignupRes->llUserScore, pMatchSignupRes->szDescription);
        }
        else if(m_pHallScene != NULL)//更新大厅分数
        {
            CHallLayer* pHallLayer = (CHallLayer*)m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                CMainLogic::sharedMainLogic()->m_lUserScore = pMatchSignupRes->llUserScore;
                pHallLayer->setUserScore(CMainLogic::sharedMainLogic()->m_lUserScore);
            }
        }
    }
    return true;
}

bool CMainLogic::OnMatchShareInfo(const void * pData, WORD wDataSize)
{
    m_pMessageBox -> Update();
    if (wDataSize<sizeof(CMD_GR_MatchShareInfoRes))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GR_MatchShareInfoRes* pMatchShareInfoRes = (CMD_GR_MatchShareInfoRes*)pData;
    if(pMatchShareInfoRes != NULL)
    {
//        printf("url is %s, content is %s, %s\n", CCharCode::GB2312ToUTF8(pMatchShareInfoRes->szUrl).c_str(), CCharCode::GB2312ToUTF8(pMatchShareInfoRes->szContent).c_str(), pMatchShareInfoRes->szContent);
        CRankNewLayer* pRankNewLayer = (CRankNewLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10200);
        if(pRankNewLayer != NULL)
        {
            pRankNewLayer->sendShareInfo(CCharCode::GB2312ToUTF8(pMatchShareInfoRes->szUrl), CCharCode::GB2312ToUTF8(pMatchShareInfoRes->szContent));
        }
        if(GameMainScene::_instance != NULL)
        {
            CSendPrizeLayer* pSendPrizeLayer = (CSendPrizeLayer*)GameMainScene::_instance->getChildByTag(eCmptSendPrizeTag);
            if(pSendPrizeLayer != NULL)
            {
                pSendPrizeLayer->sendShareInfo(CCharCode::GB2312ToUTF8(pMatchShareInfoRes->szUrl), CCharCode::GB2312ToUTF8(pMatchShareInfoRes->szContent));
            }
            
        }
    }
    return true;
}

bool CMainLogic::OnMatchBegin(const void * pData, WORD wDataSize)//比赛开始
{
    if(!m_bIOSPassed)
        return true;
    if(!m_bShowMatch)
        return true;
    if (wDataSize<sizeof(CMD_GRO_MatchBegin))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRO_MatchBegin* pMatchBegin = (CMD_GRO_MatchBegin*)pData;
    if(pMatchBegin != NULL)
    {
//        printf("CMD_GRO_MatchBegin: %d %s %s\n",pMatchBegin->nMatchID, CCharCode::GB2312ToUTF8(pMatchBegin->szMatchDate).c_str(), CCharCode::GB2312ToUTF8(pMatchBegin->szMatchTitle).c_str());
        /*int nMatchID;//比赛场次
         char szMatchDate[16];//比赛日期*/
        if(GameMainScene::_instance != NULL && (KIND_ID == NZNH_KIND_ID || KIND_ID == YQS_KIND_ID || KIND_ID == JCBY_KIND_ID))
        {
            GameMainScene::_instance->showMatchBegin(pMatchBegin->nMatchID, pMatchBegin->szMatchDate, pMatchBegin->szMatchTitle);
            sendUserBehavior(m_nUserID, eMatchBegin);
        }
    }
    return true;
}

bool CMainLogic::OnMatchEnd(const void * pData, WORD wDataSize)//比赛结束
{
    if(!m_bIOSPassed)
        return true;
    if(!m_bShowMatch)
        return true;
    if (wDataSize<sizeof(CMD_GRR_MatchEnd))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRR_MatchEnd* pMatchEnd  =(CMD_GRR_MatchEnd*)pData;
    if(pMatchEnd != NULL)
    {
        //删除分数排名框
        CCmptStatus* pCmptStatus = (CCmptStatus*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(eCmptStatusTag);
        if(pCmptStatus != NULL)
        {
            pCmptStatus -> showBisaiMenu();
            pCmptStatus -> removeFromParent();
        }
//        printf("CMD_GRR_MatchEnd: %hhu %lld %d %s %d, %s\n",pMatchEnd->cbShare,pMatchEnd->nScore, pMatchEnd->nRanking, CCharCode::GB2312ToUTF8(pMatchEnd->szMatchPrise).c_str(), pMatchEnd->nMatchID, CCharCode::GB2312ToUTF8(pMatchEnd->szMatchDate).c_str());
        /*BYTE cbShare;//是否需要分享  0为不需要分享，1为需要分享, 2为无奖励
         int nScore;//积分
         int nRanking;//排名
         char szMatchPrise[128];//比赛奖励*/
        if(GameMainScene::_instance != NULL && (KIND_ID == NZNH_KIND_ID || KIND_ID == YQS_KIND_ID || KIND_ID == JCBY_KIND_ID))
        {
            printf("cbShare is %hhu\n", pMatchEnd->cbShare);
            GameMainScene::_instance -> showMatchEnd(pMatchEnd->cbShare, pMatchEnd->nRanking, pMatchEnd->nScore,CCharCode::GB2312ToUTF8(pMatchEnd->szMatchPrise).c_str(), pMatchEnd->nMatchID, CCharCode::GB2312ToUTF8(pMatchEnd->szMatchDate));
            sendUserBehavior(m_nUserID, eMatchEnd);
        }
    }
    return true;
}

bool CMainLogic::OnMatchScoreUpdate(const void * pData, WORD wDataSize)//更新比赛排名以及分数
{
    if(!m_bIOSPassed)
        return true;
    if(!m_bShowMatch)
        return true;
    CMD_GRO_MatchScoreQueryRes* pMatchScoreQueryRes = (CMD_GRO_MatchScoreQueryRes*)pData;
    if(pMatchScoreQueryRes != NULL)
    {
        if(GameMainScene::_instance != NULL && GameMainScene::_instance->m_bEnterGameScene && (KIND_ID == NZNH_KIND_ID || KIND_ID == YQS_KIND_ID || KIND_ID == JCBY_KIND_ID))
        {
            int nCount = pMatchScoreQueryRes->nMatchNum;
            m_vctMatchGroupInfo.clear();
            for(int i = 0; i < nCount; i++)
            {
                CMD_GRO_MatchGroupInfo MatchGroupInfo = pMatchScoreQueryRes->MatchGroupInfo[i];
                m_vctMatchGroupInfo.push_back(MatchGroupInfo);
            }
            GameMainScene::_instance -> showMatchScoreUpdate(m_vctMatchGroupInfo, pMatchScoreQueryRes->nRanking, pMatchScoreQueryRes->lUserScore, pMatchScoreQueryRes->nSecond,pMatchScoreQueryRes->nMatchNum, CCharCode::GB2312ToUTF8(pMatchScoreQueryRes->szMatchTitle));
        }
        
    }
    return true;
}

bool CMainLogic::OnMatchGetPriseRes(const void * pData, WORD wDataSize)//玩家领取比赛奖励
{
    m_pMessageBox->Update();
    if(!m_bIOSPassed)
        return true;
    if(!m_bShowMatch)
        return true;
    if (wDataSize<sizeof(CMD_GRR_MatchGetMatchPriseRes))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRR_MatchGetMatchPriseRes* pMatchGetMatchPriseRes = (CMD_GRR_MatchGetMatchPriseRes*)pData;
    if(pMatchGetMatchPriseRes != NULL)
    {
//        printf("CMD_GRR_MatchGetMatchPriseRes: %d %d %d %lld %s %d\n",pMatchGetMatchPriseRes->bPriseStatus,pMatchGetMatchPriseRes->cbPriseType, pMatchGetMatchPriseRes->nPriseCount, pMatchGetMatchPriseRes->llUserScore,CCharCode::GB2312ToUTF8(pMatchGetMatchPriseRes->szDescription).c_str(),pMatchGetMatchPriseRes->nMatchID);
        /*bool bPriseStatus;//领取状态，0为领取失败，1为领取成功
         BYTE nPriseType;//比赛奖励类型
         int nPriseCount;//领取数量
         SCORE llUserScore;//玩家当前金币数量
         char szDescription[128];*/
        if(GameMainScene::_instance != NULL && (KIND_ID == NZNH_KIND_ID || KIND_ID == YQS_KIND_ID || KIND_ID == JCBY_KIND_ID))
        {
            CCLOG("pMatchGetMatchPriseRes->nMatchID=%d",pMatchGetMatchPriseRes->nMatchID);
            GameMainScene::_instance -> showMatchPriseResult(pMatchGetMatchPriseRes->bPriseStatus, pMatchGetMatchPriseRes->cbPriseType, pMatchGetMatchPriseRes->nPriseCount, pMatchGetMatchPriseRes->nMatchID, pMatchGetMatchPriseRes->llUserScore, pMatchGetMatchPriseRes->szDescription);
        }
    }
    return true;
}

bool CMainLogic::OnMatchInfoNotify(const void * pData, WORD wDataSize)//比赛信息前几分钟通知
{
    if(!m_bIOSPassed)
        return true;
    if(!m_bShowMatch)
        return true;
    if(!m_bShowNotifyInfo)//如果未开启比赛提醒，则不显示提醒界面
        return true;
    if (wDataSize<sizeof(CMD_GRO_MatchInfoNotify))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRO_MatchInfoNotify* pMatchInfoNotify = (CMD_GRO_MatchInfoNotify*)pData;
    if(pMatchInfoNotify != NULL)
    {
//        printf("CMD_GRO_MatchInfoNotify: %d %d %d %d %s %s\n",pMatchInfoNotify->nMatchPeopleNum,pMatchInfoNotify->nMatchScore, pMatchInfoNotify->nMatchID, pMatchInfoNotify->bMatchStatus,CCharCode::GB2312ToUTF8(pMatchInfoNotify->szMatchTitle).c_str(),CCharCode::GB2312ToUTF8(pMatchInfoNotify->szMatchPrise).c_str());
        /*int nMatchPeopleNum;//参赛人数
         int nMatchScore;//报名费
         int nMatchID;//比赛场次
         bool bMatchStatus;//报名状态，0为未报名，1为已报名
         char szMatchTitle[128];//比赛名称
         char szMatchPrise[128];//比赛奖励说明*/
        if(GameMainScene::_instance != NULL && GameMainScene::_instance->m_bEnterGameScene && (KIND_ID == NZNH_KIND_ID || KIND_ID == YQS_KIND_ID || KIND_ID == JCBY_KIND_ID))
        {
            GameMainScene::_instance -> showMatchInfoNotify(pMatchInfoNotify->nMatchPeopleNum, pMatchInfoNotify->nMatchID, pMatchInfoNotify->nMatchScore, pMatchInfoNotify->bMatchStatus, pMatchInfoNotify->szMatchTitle, pMatchInfoNotify->szMatchPrise);
        }
    }
    return true;
}

bool CMainLogic::OnMatchTimeNotify(const void * pData, WORD wDataSize)//比赛倒计时提醒
{
    if(!m_bIOSPassed)
        return true;
    if(!m_bShowMatch)
        return true;
    if (wDataSize<sizeof(CMD_GPO_MatchTimeNotify))
    {
        CCLOG("a = %d  %lu",wDataSize,sizeof(CMD_GPO_MatchTimeNotify));
        ERROR_CHECK;
        return false;
    }//0代表比赛之前倒计时，1代表比赛结束倒计时
    CMD_GPO_MatchTimeNotify* pMatchTimeNotify = (CMD_GPO_MatchTimeNotify*)pData;
    if(pMatchTimeNotify != NULL)
    {
//        printf("CMD_GPO_MatchTimeNotify=%d %d  %s\n",pMatchTimeNotify->cbMatchStatus,pMatchTimeNotify->nSecond,CCharCode::GB2312ToUTF8(pMatchTimeNotify->szMatchTitle).c_str());
        //开始倒计时提醒框  第一个参数为0代表比赛开始之前倒计时，  为1代表比赛结束倒计时  包含结束倒计时
        if(GameMainScene::_instance != NULL && GameMainScene::_instance->m_bEnterGameScene && (KIND_ID == NZNH_KIND_ID || KIND_ID == YQS_KIND_ID || KIND_ID == JCBY_KIND_ID))
        {
            CCountDownRemind* pCountDownLayer = CCountDownRemind::create(pMatchTimeNotify->cbMatchStatus, pMatchTimeNotify->nSecond, CCharCode::GB2312ToUTF8(pMatchTimeNotify->szMatchTitle).c_str());
            CCDirector::sharedDirector()->getRunningScene()->addChild(pCountDownLayer, 30, eCmptRemindTag);
            if(pMatchTimeNotify->cbMatchStatus == 0)
            {
                sendUserBehavior(m_nUserID, eMatchBeginTimeNotify);
            }
            else
            {
                sendUserBehavior(m_nUserID, eMatchEndTimeNotify);
            }
        }
    }
    return true;
}

bool CMainLogic::OnGameBuySkillRes(const void* pData, WORD wDataSize)
{
    m_pMessageBox->Update();
    if (wDataSize<sizeof(CMD_GR_BuySkill_Result))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GR_BuySkill_Result* pBuySkillResult = (CMD_GR_BuySkill_Result*)pData;
    if(pBuySkillResult != NULL)
    {
        if(pBuySkillResult->bSuccess)//兑换成功
        {
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
            if(GameMainScene::_instance != NULL)
            {
                GameMainScene::_instance->setUserScore(GameMainScene::_instance->getMeRealChairID(), (int)pBuySkillResult->llScore);//更新当前玩家分数
                //检查是否处于背包界面，并且更新数据
                DaojuLayer* pDaojuLayer = (DaojuLayer*)GameMainScene::_instance->getChildByTag(1989);
                if(pDaojuLayer != NULL)
                {
                    pDaojuLayer->updateSkillUI(pBuySkillResult->cbSkillID, pBuySkillResult->nCount);
                    BuyBarrelLayer* pBuyBarrelLayer = (BuyBarrelLayer*)pDaojuLayer->getChildByTag(1009);
                    if(pBuyBarrelLayer != NULL)
                    {
                        pBuyBarrelLayer -> closeWnd(NULL);//关闭兑换界面
                    }
                }
                //更新游戏内右侧技能角标数量
                GameMainScene::_instance->resetSkillStauts();
            }
        }
        ShowMessage(CCharCode::GB2312ToUTF8(pBuySkillResult->szDescribeString).c_str(), eMBOK);
    }
    return true;
}

bool CMainLogic::OnQiangGameResult(const void* pData, WORD wDataSize)
{
    m_pMessageBox->Update();
    if(!m_bIOSPassed)
        return true;
    if (wDataSize<sizeof(CMD_GRO_QiangResult))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRO_QiangResult* pQiangResult = (CMD_GRO_QiangResult*)pData;
    if(pQiangResult != NULL)
    {
        if(pQiangResult->bSuccess)
        {
            if(GameMainScene::_instance != NULL)
            {
                GameMainScene::_instance->setUserScore(GameMainScene::_instance->getMeRealChairID(), pQiangResult->nCurrentScore);
                GameMainScene::_instance->playParticle("penjinbi", CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
            }
            RedPacketLayer* pRedLayer = RedPacketLayer::create(2);
            CCDirector::sharedDirector()->getRunningScene()->addChild(pRedLayer, 20, 29998);
            pRedLayer -> setReceiveData(pQiangResult->nScore, CCharCode::GB2312ToUTF8(pQiangResult->szContent1), CCharCode::GB2312ToUTF8(pQiangResult->szContent2));
        }
        else
        {
            RedPacketLayer* pRedLayer = RedPacketLayer::create(4);
            CCDirector::sharedDirector()->getRunningScene()->addChild(pRedLayer, 20, 29998);
            pRedLayer -> setNoRedName(CCharCode::GB2312ToUTF8(pQiangResult->szContent1));
        }
    }
    return true;
}

bool CMainLogic::OnCanMatchShare(const void * pData, WORD wDataSize)
{
    if (wDataSize<sizeof(CMD_GRO_CanMatchShare))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRO_CanMatchShare* pCanMatchShare = (CMD_GRO_CanMatchShare*)pData;
    if(pCanMatchShare != NULL)
    {
        CRankNewLayer* pRankNewLayer = (CRankNewLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10200);
        if(pRankNewLayer != NULL)
        {
            pRankNewLayer->setMatchShare(pCanMatchShare->cbShare);
        }
        else
        {
            m_pMessageBox->Update();
        }
    }
    else
    {
        m_pMessageBox->Update();
    }
    return true;
}

bool CMainLogic::OnHaveMatchPrizeRemind(const void* pData, WORD wDataSize)
{
    if (wDataSize<sizeof(CMD_GRO_HavaPrizeRemind))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRO_HavaPrizeRemind* pRemind = (CMD_GRO_HavaPrizeRemind*)pData;
    if(pRemind != NULL)
    {
        if(GameMainScene::_instance != NULL && GameMainScene::_instance->m_bEnterGameScene)
        {
            ShowMessage(CCharCode::GB2312ToUTF8(pRemind->szDescription).c_str(), eMBOK);
            sendUserBehavior(m_nUserID, eMatchPrizeRemindTag);
        }
    }
    
    return true;
}

bool CMainLogic::OnGetBankCuptScore(const void* pData, WORD wDataSize)
{
    
    CMD_GRO_GetBankcruptScore* pGetBankCruptScore = (CMD_GRO_GetBankcruptScore*)pData;
    if(pData != NULL)
    {
        if(GameMainScene::_instance != NULL)
        {
            if(pGetBankCruptScore->bSuccess)
            {
                GameMainScene::_instance->updateCurrentScore(pGetBankCruptScore->nCurrentScore);
                m_nBankruptcyCount = pGetBankCruptScore->nAlreadyTime;
                ShowMessage(CCharCode::GB2312ToUTF8(pGetBankCruptScore->szDescription).c_str(), eMBOK, eCloseGiveLayer);
                sendUserBehavior(m_nUserID, eGetFreeCoinTag);
            }
            else
            {
                ShowMessage(CCharCode::GB2312ToUTF8(pGetBankCruptScore->szDescription).c_str(), eMBOK, eCloseGiveLayer);
                sendUserBehavior(m_nUserID, eGetFreeCoinFailTag);
            }
            m_cbCanGetBankruptcy = 0;
            
        }
    }
    return true;
}

bool CMainLogic::OnQueryBankCuptinfo(const void* pData, WORD wDataSize)
{
    if (wDataSize<sizeof(CMD_GRO_QueryBankcruptInfo))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRO_QueryBankcruptInfo* pQueryBankcuptinfo = (CMD_GRO_QueryBankcruptInfo*)pData;
    if(pQueryBankcuptinfo != NULL)
    {
        if(GameMainScene::_instance != NULL)
        {
            GameMainScene::_instance->showFreeCoinUI(pQueryBankcuptinfo->nLastTime, pQueryBankcuptinfo->nScore, CCharCode::GB2312ToUTF8(pQueryBankcuptinfo->szDescription).c_str());
            if(pQueryBankcuptinfo->nLastTime == 0)
            {
                m_cbCanGetBankruptcy = 0;
            }
        }
    }
    return true;
}

bool CMainLogic::OnGameUserLaba(const void* pData, WORD wDataSize)
{
    m_pMessageBox->Update();
    if (wDataSize<sizeof(CMD_GR_BroadLabaRes))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GR_BroadLabaRes* pBroadLabaRes = (CMD_GR_BroadLabaRes*)pData;
    if(pBroadLabaRes != NULL)
    {
        if(pBroadLabaRes->cbSuccess == 1)//成功
        {
            if(GameMainScene::_instance != NULL)
            {
                GameMainScene::_instance -> setUserScore(GameMainScene::_instance->getMeRealChairID(), (int)pBroadLabaRes->lUserScore);//更新当前分数
                FirstChargeLayer* pFirstChargeLayer = (FirstChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10301);
                if(pFirstChargeLayer != NULL)
                {
                    pFirstChargeLayer -> setLabaLabel();
                }
            }
        }
        ShowMessage(CCharCode::GB2312ToUTF8(pBroadLabaRes->szContent).c_str(), eMBOK);
    }
    return true;
}

bool CMainLogic::OnOnlineReWardInfo(const void* pData, WORD wDataSize)
{
    if(!m_bIOSPassed)
        return true;
    if(!m_bShowOnlineCoin)
        return true;
    if(!m_bAutoCheckOnlineInfo)
    {
        m_pMessageBox -> Update();
    }
    if(wDataSize < sizeof(CMD_GRO_OnlineRewardInfo))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRO_OnlineRewardInfo* pOnlineRewardInfo = (CMD_GRO_OnlineRewardInfo*)pData;
    if(pOnlineRewardInfo != NULL)
    {
        if(GameMainScene::_instance != NULL)
        {
            if(m_bAutoCheckOnlineInfo)//24点刷新
            {
                GameMainScene::_instance->setOnlineRewardTime(pOnlineRewardInfo->OneOnlineRewardInfo[0].nSecond);
                OnlineRewardLayer * pLayer = (OnlineRewardLayer*)GameMainScene::_instance->getChildByTag(eOnlinelayerTag);
                if(pLayer != NULL)//刷新数据
                {
                    m_vctOnlineRewardInfo.clear();
                    int nCount = pOnlineRewardInfo->nCount;
                    for(int i = 0; i < nCount; i++)
                    {
                        m_vctOnlineRewardInfo.push_back(pOnlineRewardInfo->OneOnlineRewardInfo[i]);
                    }
                    pLayer -> setOnlineRewardInfo(m_vctOnlineRewardInfo);
                }
            }
            else//客户端发起请求
            {
                if(GameMainScene::_instance->getAutoFireStatus())
                {
                    GameMainScene::_instance->unschedule(schedule_selector(GameMainScene::startBullet));
                }
                m_vctOnlineRewardInfo.clear();
                int nCount = pOnlineRewardInfo->nCount;
                for(int i = 0; i < nCount; i++)
                {
                    m_vctOnlineRewardInfo.push_back(pOnlineRewardInfo->OneOnlineRewardInfo[i]);
                }
                OnlineRewardLayer * pLayer = OnlineRewardLayer::create(m_vctOnlineRewardInfo);
                GameMainScene::_instance -> addChild(pLayer, 100, eOnlinelayerTag);
                if(GameMainScene::_instance->getMeRealChairID() >= GAME_PLAYER/2)
                {
                    pLayer -> setRotation(180);
                }
            }
        }
    }
    return true;
}

bool CMainLogic::OnReceiveOnlineReward(const void* pData, WORD wDataSize)
{
    m_pMessageBox -> Update();
    if(wDataSize < sizeof(CMD_GRO_ReceiveOnlineReward))
    {
        ERROR_CHECK;
        return false;
    }
    CMD_GRO_ReceiveOnlineReward* pReceiveOnlineReward = (CMD_GRO_ReceiveOnlineReward*)pData;
    if(pReceiveOnlineReward != NULL)
    {
        printf("bSuccess is %d, nSecondTime is %d, score is %d, nCurrentScore is %d\n", pReceiveOnlineReward->bSuccess, pReceiveOnlineReward->nNextSecond, pReceiveOnlineReward->nScore, pReceiveOnlineReward->nCurrentScore);
        if(GameMainScene::_instance != NULL)
        {
            if(pReceiveOnlineReward->bSuccess)
            {
                GameMainScene::_instance->receiveOnlineReward(pReceiveOnlineReward->nNextSecond, pReceiveOnlineReward->nScore, pReceiveOnlineReward->nCurrentScore);
            }
            else
            {
                CMainLogic::sharedMainLogic()->ShowMessage(CCharCode::GB2312ToUTF8(pReceiveOnlineReward->szDescription).c_str(), eMBOK);
            }
        }
    }
    return true;
}
