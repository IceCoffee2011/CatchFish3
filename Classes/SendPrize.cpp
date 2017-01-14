//
//  SendPrize.cpp
//  CatchFish3
//
//  Created by 王军闯 on 16/5/30.
//
//

#include "header.h"

enum
{
    eShareToGet = 30,
    eDrctToGet,
    eContinueEnroll,
    eCloseWnd,
};

#if defined(IOS_ANDROID)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
extern int WeiXinCharge;
#endif

#endif

CSendPrizeLayer* CSendPrizeLayer::create(BYTE cbShare, int nRank, SCORE nScore, std::string sPrize, int nMatchID, std::string szMatchDate)
{
    CSendPrizeLayer* pRet = new CSendPrizeLayer;
    if(pRet && pRet->init(cbShare, nRank, nScore, sPrize, nMatchID, szMatchDate))
    {
        pRet -> autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CSendPrizeLayer::init(BYTE cbShare, int nRank, SCORE nScore, std::string sPrize, int nMatchID, std::string szMatchDate)
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_nMatchID = nMatchID;
    m_sMatchDate = szMatchDate;
    m_nRank = nRank;
    if(nScore == 0)
    {
        m_bShareSucceed = true;
    }
    else
    {
        m_bShareSucceed = false;
    }
    this->setTouchEnabled(true);
    CCSprite* pSunLight = CCSprite::create("signIn/signsuccess5.png");//太阳光
    pSunLight -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.65f));
    this -> addChild(pSunLight, 200);
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 120), CLIENT_WIDTH, CLIENT_HEIGHT*2);
    pLayerColor -> ignoreAnchorPointForPosition(false);
    pLayerColor -> setPosition(ccp(pSunLight->getContentSize().width/2, pSunLight->getContentSize().height/2));
    pSunLight -> addChild(pLayerColor, -2);
    CCSprite* sprite6 = CCSprite::create("signIn/signsuccess6.png");//太阳转圈背景
    sprite6 -> setPosition(ccp(pSunLight->getContentSize().width/2, pSunLight->getContentSize().height/2));
    pSunLight -> addChild(sprite6, -1);
    sprite6->setScale(1.2f);
    pSunLight -> setOpacity(0);
    pSunLight->runAction(CCFadeIn::create(0.3f));
    sprite6 -> setOpacity(0);
    sprite6 -> runAction(CCRepeatForever::create(CCRotateBy::create(2.0f, 180)));
    sprite6 -> runAction(CCFadeIn::create(0.5f));
    CCSprite* sprite3 = CCSprite::create("competition/cmptBg.png");
    sprite3 -> setPosition(ccp(pSunLight->getContentSize().width/2, pSunLight->getContentSize().height*0.63f));
    pSunLight -> addChild(sprite3, 200);
    CCSprite* sprite41 = CCSprite::create("competition/cmptStar1.png");
    sprite41 -> setPosition(ccp(sprite3->getContentSize().width/2, sprite3->getContentSize().height*0.3f));
    sprite3 -> addChild(sprite41, 200);
    CCSprite* sprite42 = CCSprite::create("competition/cmptStar2.png");
    sprite42 -> setPosition(ccp(sprite3->getContentSize().width*0.6f, sprite3->getContentSize().height*0.44f));
    sprite3 -> addChild(sprite42, 200);
    CCSprite* pRankSp = NULL;
    if (nRank == 1)
    {
        pRankSp = CCSprite::create("competition/guanjun.png");
    }
    else if(nRank == 2)
    {
        pRankSp = CCSprite::create("competition/yajun.png");
    }
    else if(nRank == 3)
    {
        pRankSp = CCSprite::create("competition/jijun.png");
    }
    else
    {
        pRankSp = CCSprite::create("competition/levelBg.png");
        char sRank[4];
        sprintf(sRank, "%d", nRank);
        CCLabelAtlas* pLevlAtlas = CCLabelAtlas::create(sRank, "competition/levelNum.png", 63, 86, '0');
        pLevlAtlas->setAnchorPoint(ccp(0.5f, 0.5f));
        pLevlAtlas->setPosition(ccp(pRankSp->getContentSize().width/2, pRankSp->getContentSize().height/2));
        pRankSp->addChild(pLevlAtlas);
    }
    pRankSp -> setPosition(ccp(sprite3->getContentSize().width/2, sprite3->getContentSize().height/2+10));
    sprite3 -> setScale(5.0f);
    sprite3 -> addChild(pRankSp, 200);
    sprite3 -> runAction(CCSequence::create(CCHide::create(), CCDelayTime::create(0.1f), CCShow::create(), CCScaleTo::create(0.1f, 1.0f), NULL));
    for (int i = 0; i < 3; i++)
    {
        CCSprite* pPrizeBg = CCSprite::create("competition/infoBg.png");
        pPrizeBg->setPosition(ccp(pSunLight->getContentSize().width/2, pSunLight->getContentSize().height*0.22f-pPrizeBg->getContentSize().height*i));
        pSunLight->addChild(pPrizeBg);
        CCLabelTTF* pDescTTF = CCLabelTTF::create("我的排名", "", 36);//左侧列表
        pDescTTF->setAnchorPoint(ccp(0, 0.5f));
        pDescTTF->setPosition(ccp(pPrizeBg->getContentSize().width*0.25f, pPrizeBg->getContentSize().height/2));
        pPrizeBg->addChild(pDescTTF);
        
        CCLabelTTF* pResultTTF = CCLabelTTF::create("", "", 36);//右侧相应的奖励
        pResultTTF->setAnchorPoint(ccp(0, 0.5f));
        pResultTTF->setPosition(ccp(pPrizeBg->getContentSize().width*0.6f, pPrizeBg->getContentSize().height/2));
        pPrizeBg->addChild(pResultTTF);
        pResultTTF->setColor(ccYELLOW);
        if (i == 0)
        {
            char sRank[16];
            sprintf(sRank, "第 %d 名", nRank);
            pResultTTF->setString(sRank);
        }
        else if (i == 1)
        {
            pDescTTF->setString("我的积分");
            char sScore[16];
            sprintf(sScore, "%lld", nScore);
            pResultTTF->setString(sScore);
        }
        else if(i == 2)
        {
            pDescTTF->setString("我的奖励");
            pResultTTF->setString(sPrize.c_str());
        }
    }
    CCMenuItemImage* pShareGetItem = CCMenuItemImage::create("competition/shareToGet.png", "competition/shareToGet.png", this, menu_selector(CSendPrizeLayer::menuCallBack));//分享领取按钮
    pShareGetItem->setPosition(ccp(0, -pSunLight->getContentSize().height*0.55f));
    pShareGetItem->setTag(eShareToGet);
    CCMenuItemImage* pDerctGetItem = CCMenuItemImage::create("competition/drctGet.png", "competition/drctGet.png", this, menu_selector(CSendPrizeLayer::menuCallBack));//直接领取按钮drctGet
    pDerctGetItem->setPosition(pShareGetItem->getPosition());
    pDerctGetItem->setTag(eDrctToGet);
    CCMenuItemImage* pContineEnrollItem = CCMenuItemImage::create("competition/continueCmpt.png", "competition/continueCmpt.png", this, menu_selector(CSendPrizeLayer::menuCallBack));//继续报名按钮
    pContineEnrollItem->setPosition(ccp(pSunLight->getContentSize().width, pShareGetItem->getPositionY()));
    pContineEnrollItem->setTag(eContinueEnroll);
    CCMenuItemImage* pCloseItem = CCMenuItemImage::create("Dragon/dragoncloseitem.png", "Dragon/dragoncloseitem.png", this, menu_selector(CSendPrizeLayer::menuCallBack));//关闭按钮
    pCloseItem->setPosition(ccp(pSunLight->getContentSize().width*2.2f, pSunLight->getContentSize().height*1.05f));
    pCloseItem->setTag(eCloseWnd);
    CCMenu* pMenu = CCMenu::create(pShareGetItem, pDerctGetItem, pContineEnrollItem, pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    pMenu->setAnchorPoint(CCPointZero);
    pSunLight->addChild(pMenu);
    pMenu->setTouchPriority(-149);
    m_bNeedShare = false;
    if (cbShare == 0)//不需要分享但是有奖励
    {
        pShareGetItem->setVisible(false);
        pDerctGetItem->setVisible(true);
        pContineEnrollItem->setPosition(ccp(pSunLight->getContentSize().width, pShareGetItem->getPositionY()));
    }
    else if(cbShare == 1)//需要分享领取奖励
    {
        m_bNeedShare = true;
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
        pShareGetItem->setVisible(false);
        pDerctGetItem->setVisible(true);
#else
        pShareGetItem->setVisible(true);
        pDerctGetItem->setVisible(false);
#endif
        pContineEnrollItem->setPosition(ccp(pSunLight->getContentSize().width, pShareGetItem->getPositionY()));
    }
    else if(cbShare == 2)//不领取奖励
    {
        pShareGetItem->setVisible(false);
        pDerctGetItem->setVisible(false);
        pContineEnrollItem->setPosition(ccp(pSunLight->getContentSize().width/2, pShareGetItem->getPositionY()));
    }
    
    return true;
}

void CSendPrizeLayer::menuCallBack(CCObject* pObj)
{
    CCMenuItem* pTemItem = (CCMenuItem*)pObj;
    int nItemTag = pTemItem->getTag();
    switch (nItemTag) {
        case eShareToGet:
        {
#if defined(IOS_ANDROID)
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eMatchSharePrise);
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            //分享领奖
            CMD_GR_MatchShareInfo MatchShareInfo = {0};
            MatchShareInfo.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            MatchShareInfo.nMatchID = m_nMatchID;
            MatchShareInfo.nRank = m_nRank;
            strncpy(MatchShareInfo.szMatchDate, CCharCode::UTF8ToGB2312(m_sMatchDate.c_str()).c_str(), sizeof(MatchShareInfo.szMatchDate));
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_USER, SUB_GR_MATCH_SHAREINFO, &MatchShareInfo, sizeof(MatchShareInfo));
            CMainLogic::sharedMainLogic()->ShowMessage("正在请求，请稍候", eMBExitGame);
#else
            CMainLogic::sharedMainLogic()->ShowMessage("此功能暂未开放", eMBOK);
#endif
        }
            break;
        case eDrctToGet://直接领奖
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            if (m_bNeedShare)
            {
                sendSharedCompleted();
            }
            else
            {
                //向服务器发送消息
                if(GameMainScene::_instance != NULL)
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eMatchNormalPrise);
                    m_bShareSucceed = true;
                    CMD_GRO_MatchGetMatchPrise MatchGetMatchPrise = {0};//用户领取奖励
                    MatchGetMatchPrise.nMatchID = m_nMatchID;
                    MatchGetMatchPrise.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                	strncpy(MatchGetMatchPrise.szMatchDate, CCharCode::UTF8ToGB2312(m_sMatchDate.c_str()).c_str(), sizeof(MatchGetMatchPrise.szMatchDate));
                	CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_USER, SUB_GR_MATCH_GET_MATCH_PRIZE, &MatchGetMatchPrise, sizeof(MatchGetMatchPrise));
                	CMainLogic::sharedMainLogic()->ShowMessage("正在请求，请稍候", eMBExitGame);
            	}
			}
            //CMainLogic::sharedMainLogic()->ShowMessage("领取成功");
        }
            break;
        case eContinueEnroll://继续报名
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            CRankNewLayer* pRankNewLayer = CRankNewLayer::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(pRankNewLayer, 10, 10200);
            this -> removeFromParent();
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eMatchSignInNext);
        }
            break;
        case eCloseWnd:
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
            //关闭
            if (!m_bShareSucceed)
            {
                static int nRemindTimes = 0;
                nRemindTimes++;
                if (nRemindTimes == 1)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("请到“我的赛况”去领取您的奖励");
                }
            }
            this->removeFromParent();
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eMatchPriseClose);
        }
            break;
        default:
            break;
    }
}

void CSendPrizeLayer::sendSharedCompleted()
{
    char szMD5Result[256];
    char userID[256];
    sprintf(userID, "uid=%d&mnum=%d&mday=%s&time=%lubuyudaka2016", CMainLogic::sharedMainLogic()->m_nUserID,m_nMatchID,m_sMatchDate.c_str(), time(0));
    CEncryptMD5::toString32(userID,szMD5Result);
    char urlStr[256];
    sprintf(urlStr, "http://%s/ShareNewed.aspx?uid=%d&mnum=%d&mday=%s&time=%lu&info=%s", NORMAL_URL, CMainLogic::sharedMainLogic()->m_nUserID,m_nMatchID,m_sMatchDate.c_str(), time(0), szMD5Result);
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(urlStr);
    request -> setRequestType(CCHttpRequest::kHttpPost);
    request -> setTag("http test");
    request -> setResponseCallback(this, httpresponse_selector(CSendPrizeLayer::onHttpSharedComplete));
    CCHttpClient::getInstance()->send(request);
    request -> release();
}

void CSendPrizeLayer::onHttpSharedComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    std::string sContent = "";
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();//Root
        if (rootElement == NULL)
        {
            return ;
        }
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("ShareInfo"))
            return ;
        TiXmlElement* pChildElement=rootElement->FirstChildElement();
        
        while (pChildElement!=NULL)
        {
            nodename=pChildElement->Value();
            if (0==nodename.compare("text"))
            {
                sContent = pChildElement->GetText();
                CMainLogic::sharedMainLogic()->ShowMessage(sContent.c_str(), eMBOK);
            }
            if (0==nodename.compare("bool"))
            {
                std::string sResult = pChildElement->GetText();
                if(sResult == "true")
                {
                    this -> removeFromParent();
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eMatchEndSharePriseScuuess);
                }
            }
            
            pChildElement = pChildElement->NextSiblingElement();
        }
    }
    delete document;
}

void CSendPrizeLayer::sendShareInfo(std::string str1, std::string str2)
{
    CMainLogic::sharedMainLogic()->m_nShareInterface = 3;
    //只做微信分享
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(ChargeWebView::checkWX())
    {
        CMainLogic::sharedMainLogic()->m_bSharedState = true;
        WeiXinCharge = 2;
        ChargeWebView::openIOSShare(str1, str2, 1);
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，检测到您还未安装微信客户端，分享失败", eMBOK);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    jobject jobj;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "cn/sharesdk/hellocpp/HelloCpp","getInstance","()Ljava/lang/Object;");
    if (isHave)
    {
        //调用Java静态函数，取得对象。
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        if (jobj != NULL)
        {
            isHave = JniHelper::getMethodInfo(minfo,"cn/sharesdk/hellocpp/HelloCpp","share2weixin","(Ljava/lang/String;Ljava/lang/String;I)V");
            if (isHave)
            {
                jstring jStr1 = minfo.env->NewStringUTF(str1.c_str());
                jstring jStr2 = minfo.env->NewStringUTF(str2.c_str());
                jint nType = 1;
                //调用java非静态函数, 参数1：Java对象，上面已经取得   参数2：方法ID
                minfo.env->CallVoidMethod(jobj, minfo.methodID, jStr1, jStr2, nType);
            }
        }
    }
#endif
}

void CSendPrizeLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -148, true);
}

bool CSendPrizeLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

