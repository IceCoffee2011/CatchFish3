//
//  EnrollRemind.cpp
//  CatchFish3
//
//  Created by 王军闯 on 16/5/31.
//
//

#include "header.h"

enum
{
    eCmptTag = 30,
    eEnrollTag,
    eJoinedTag,
    eCmptTitleTag,
};

CEnrollRemind* CEnrollRemind::create(std::string sCmptName, std::string sCmpPrize, int nPersonNum, int nEnrollFee, bool bCanEnroll)
{
    CEnrollRemind* pRet = new CEnrollRemind;
    if(pRet && pRet->init(sCmptName, sCmpPrize, nPersonNum, nEnrollFee, bCanEnroll))
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

bool CEnrollRemind::init(std::string sCmptName, std::string sCmpPrize, int nPersonNum, int nEnrollFee, bool bCanEnroll)
{
    if(!CCLayer::init())
    {
        return false;
    }
    initUI(sCmptName, sCmpPrize, nPersonNum, nEnrollFee, bCanEnroll);
    
    //隐藏游戏中的比赛按钮
    if (GameMainScene::_instance != NULL)
    {
        CCMenu* pBisaiMenu = GameMainScene::GetInstance()->getBisaiMenu();
        CCMenuItem* pBisaiItem = (CCMenuItem*)pBisaiMenu->getChildByTag(326);
        if (pBisaiItem != NULL)
        {
            pBisaiItem->setVisible(false);
        }
    }
    
    return true;
}

void CEnrollRemind::showBisaiMenu()
{
    //隐藏游戏中的比赛按钮
    if (GameMainScene::_instance != NULL)
    {
        CCMenu* pBisaiMenu = GameMainScene::GetInstance()->getBisaiMenu();
        CCMenuItem* pBisaiItem = (CCMenuItem*)pBisaiMenu->getChildByTag(326);
        if (pBisaiItem != NULL)
        {
            pBisaiItem->setVisible(true);
        }
    }
}

void CEnrollRemind::initUI(std::string sCmptName, std::string sCmpPrize, int nPersonNum, int nEnrollFee, bool bCanEnroll)
{
    pEnrollBg = CCSprite::create("competition/enrollBg.png");//报名背景
    pEnrollBg->setPosition(ccp(-pEnrollBg->getContentSize().width/2, CLIENT_HEIGHT* 0.8f-40));
    this->addChild(pEnrollBg);
    CCSprite* pLogo = CCSprite::create("competition/cmptBtn.png");//比赛Logo
    pLogo->setPosition(ccp(pEnrollBg->getContentSize().width*0.11f-6, pEnrollBg->getContentSize().height*0.52f+8-16));
    pEnrollBg->addChild(pLogo, 1, eCmptTitleTag);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(CEnrollRemind::showBisaiMenu));
    pEnrollBg->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(pEnrollBg->getContentSize().width, 0)), func, NULL));
    
    CCLabelTTF* pCmptNameTTF = CCLabelTTF::create(CCharCode::GB2312ToUTF8(sCmptName.c_str()).c_str(), "Arial-BoldMT", 26);//比赛名字
    pCmptNameTTF->setAnchorPoint(ccp(0, 0.5f));
    pCmptNameTTF->setPosition(ccp(pEnrollBg->getContentSize().width*0.22f, pEnrollBg->getContentSize().height*0.78f));
    pEnrollBg->addChild(pCmptNameTTF);
    pCmptNameTTF->setColor(ccYELLOW);
    CCLabelTTF* pCmpPrizeTTF = CCLabelTTF::create(CCharCode::GB2312ToUTF8(sCmpPrize.c_str()).c_str(), "Arial-BoldMT", 26);//冠军奖励
    pCmpPrizeTTF->setAnchorPoint(ccp(0, 0.5f));
    pCmpPrizeTTF->setPosition(ccp(pCmptNameTTF->getPositionX(), pEnrollBg->getContentSize().height*0.54f));
    pEnrollBg->addChild(pCmpPrizeTTF);
    pCmpPrizeTTF->setColor(ccYELLOW);
    CCLabelTTF* pPersonTTF = CCLabelTTF::create("参赛人数：", "Arial-BoldMT", 24);//参赛人数
    pPersonTTF->setAnchorPoint(ccp(0, 0.5f));
    pPersonTTF->setPosition(ccp(pCmptNameTTF->getPositionX(), pEnrollBg->getContentSize().height*0.22f));
    pEnrollBg->addChild(pPersonTTF);
    pPersonTTF->setColor(ccYELLOW);
    char sPerson[4];
    sprintf(sPerson, "%d", nPersonNum);
    CCLabelTTF* pPersonNum = CCLabelTTF::create(sPerson, "", 24);//参赛数量
    pPersonNum->setPosition(ccp(pPersonTTF->getContentSize().width, pPersonTTF->getContentSize().height/2));
    pPersonTTF->addChild(pPersonNum);
    pPersonNum->setColor(ccYELLOW);
    
    CCLabelTTF* pFeeTTF = CCLabelTTF::create("入场费：", "Arial-BoldMT", 24);//入场费
    pFeeTTF->setAnchorPoint(ccp(0, 0.5f));
    pFeeTTF->setPosition(ccp(pEnrollBg->getContentSize().width*0.46f, pPersonTTF->getPositionY()));
    pEnrollBg->addChild(pFeeTTF);
    pFeeTTF->setColor(ccYELLOW);
    char sFee[16];
    sprintf(sFee, "%d", nEnrollFee);
    CCLabelTTF* pFeeNumTTF = CCLabelTTF::create(sFee, "", 24);//入场费用
    pFeeNumTTF->setPosition(ccp(pFeeTTF->getContentSize().width*1.3f, pFeeTTF->getContentSize().height/2));
    pFeeTTF->addChild(pFeeNumTTF);
    pFeeNumTTF->setColor(ccYELLOW);
    
    
    
    CCMenuItemImage* pEnrollItem = NULL;
    if(nEnrollFee == 0)
    {
        pEnrollItem = CCMenuItemImage::create("competition/enrollBtn3.png", "competition/enrollBtn3.png", this, menu_selector(CEnrollRemind::menuCallBack));//报名
    }
    else
    {
        pEnrollItem = CCMenuItemImage::create("competition/enrollBtn.png", "competition/enrollBtn.png", this, menu_selector(CEnrollRemind::menuCallBack));//报名
    }
    pEnrollItem->setPosition(ccp(pEnrollBg->getContentSize().width*0.86f, pEnrollBg->getContentSize().height*0.27f));
    pEnrollItem->setTag(eEnrollTag);
    m_pEnrollMenu = CCMenu::create(pEnrollItem, NULL);
    m_pEnrollMenu->setAnchorPoint(CCPointZero);
    m_pEnrollMenu->setPosition(CCPointZero);
    pEnrollBg->addChild(m_pEnrollMenu);
    m_pEnrollMenu->setTouchPriority(-300);
    this->schedule(schedule_selector(CEnrollRemind::disappearSchedule), 15.0f);
    //已报名图标
    CCMenuItemImage* joinedItem = CCMenuItemImage::create("competition/enrollBtn2.png", "competition/enrollBtn2.png", this, menu_selector(CEnrollRemind::menuCallBack));
    joinedItem->setPosition(ccp(pEnrollBg->getContentSize().width*0.86f, pEnrollBg->getContentSize().height*0.27f));
    joinedItem -> setTag(eJoinedTag);
    m_pJoinedMenu = CCMenu::create(joinedItem, NULL);
    m_pJoinedMenu -> setPosition(CCPointZero);
    m_pJoinedMenu -> setAnchorPoint(CCPointZero);
    m_pJoinedMenu -> setTouchPriority(-300);
    pEnrollBg->addChild(m_pJoinedMenu, 1);
    
    if (!bCanEnroll)
    {
        pEnrollItem->setVisible(true);
        m_pJoinedMenu->setVisible(false);
    }
    else
    {
        pEnrollItem->setVisible(false);
        m_pJoinedMenu->setVisible(true);
    }
    
    
    
    //比赛引导，如果参加过比赛指引的话，就将“ShowMatchGuide”设置为false
    if(CCUserDefault::sharedUserDefault()->getBoolForKey("ShowMatchGuide", true))
    {
        CCSprite* pGuideArrow = CCSprite::createWithSpriteFrameName("byguidejian2.png");
        pGuideArrow->setPosition(ccp(pEnrollItem->getContentSize().width/2, -40));
        pEnrollItem->addChild(pGuideArrow);
        pGuideArrow->setRotation(180);
        CCRepeatForever* pRep = CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f, ccp(0, 10)), CCMoveBy::create(0.5f, ccp(0, -10)), NULL));
        pGuideArrow->runAction(pRep);
        CCSprite* pGuide1 = CCSprite::create("competition/matchguide1.png");
        pGuide1->setPosition(ccp(pEnrollItem->getContentSize().width/2, -140));
        pEnrollItem->addChild(pGuide1);
        pGuideArrow -> setVisible(false);
        pGuide1 -> setVisible(false);
        pGuideArrow -> runAction(CCSequence::create(CCDelayTime::create(0.5f), CCShow::create(), NULL));
        pGuide1 -> runAction(CCSequence::create(CCDelayTime::create(0.5f), CCShow::create(), NULL));
    }
    else
    {
        CCSprite* pPoint = CCSprite::create("competition/point.png");
        pPoint->setAnchorPoint(ccp(0.5f, 0));
        pPoint->setPosition(ccp(pEnrollItem->getContentSize().width/2, -40));
        pEnrollItem->addChild(pPoint);
        CCSprite* pToCmpt = CCSprite::create("competition/toCmpt.png");
        pToCmpt->setPosition(ccp(pPoint->getContentSize().width/2, -10));
        pPoint->addChild(pToCmpt);
        pPoint->runAction(CCBlink::create(15, 10));
        //在参与比赛字体上加特效
        CCSprite* pStarMiddle = CCSprite::create("charge/chargegamebg1.png");
        pStarMiddle->setPosition(ccp(pToCmpt->getContentSize().width/2-3.5, pToCmpt->getContentSize().height/2+5));
        pStarMiddle->setOpacity(0);
        pStarMiddle->setScale(0.7f);
        pToCmpt->addChild(pStarMiddle, 1);
        CCSequence* seqMiddle =CCSequence::create(CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL), CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(1.2f),NULL);
        pStarMiddle->runAction(CCRepeatForever::create(seqMiddle));
        CCSprite* pStarDownR = CCSprite::create("charge/chargegamebg1.png");
        pStarDownR->setPosition(ccp(pToCmpt->getContentSize().width/2+7, pToCmpt->getContentSize().height/2-20));
        pToCmpt->addChild(pStarDownR, 1);
        pStarDownR->setOpacity(0);
        pStarDownR->setScale(0.7f);
        CCSequence* seqDR = CCSequence::create(CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(1.0f),NULL);
        pStarDownR->runAction(CCRepeatForever::create(seqDR));
        
        CCSprite* pStarDownL = CCSprite::create("charge/chargegamebg1.png");
        pStarDownL->setPosition(ccp(pToCmpt->getContentSize().width/2-40, pToCmpt->getContentSize().height/2-10));
        pToCmpt->addChild(pStarDownL, 1);
        pStarDownL->setOpacity(0);
        pStarDownL->setScale(0.5f);
        CCSequence* seqDL = CCSequence::create(CCDelayTime::create(0.4f),CCSpawn::create(CCScaleTo::create(0.2f, 1.3),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 0.8),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.8f),NULL);
        pStarDownL->runAction(CCRepeatForever::create(seqDL));
        
        CCSprite* pStarUpL = CCSprite::create("charge/chargegamebg1.png");
        pStarUpL->setPosition(ccp(pToCmpt->getContentSize().width/2-23, pToCmpt->getContentSize().height/2+45));
        pToCmpt->addChild(pStarUpL, 1);
        pStarUpL->setOpacity(0);
        pStarUpL->setScale(0.7f);
        CCSequence* seqUL = CCSequence::create(CCDelayTime::create(0.8f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 1.0),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.4f),NULL);
        pStarUpL->runAction(CCRepeatForever::create(seqUL));
        
        CCSprite* pStarUpR = CCSprite::create("charge/chargegamebg1.png");
        pStarUpR->setPosition(ccp(pToCmpt->getContentSize().width/2+31, pToCmpt->getContentSize().height/2-10));
        pToCmpt->addChild(pStarUpR, 1);
        pStarUpR->setOpacity(0);
        pStarUpR->setScale(0.3f);
        CCSequence* seqUR = CCSequence::create(CCDelayTime::create(1.0f),CCSpawn::create(CCScaleTo::create(0.2f, 1.5),CCFadeIn::create(0.2f),NULL),CCDelayTime::create(0.2f),CCSpawn::create(CCScaleTo::create(0.4f, 0.6),CCFadeOut::create(0.4f),NULL),CCDelayTime::create(0.2f),NULL);
        pStarUpR->runAction(CCRepeatForever::create(seqUR));
        
        CCSprite* pSan = CCSprite::create("charge/chargegamebg3.png");
        pSan->setPosition(ccp(pToCmpt->getContentSize().width/2, pToCmpt->getContentSize().height/2+10));
        pToCmpt->addChild(pSan, 1);
        pSan->setOpacity(0);
        pSan->setScale(0.5f);
        ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE };
        pSan->setBlendFunc(cbl);
        CCSequence* seqScale = CCSequence::create(CCScaleTo::create(0.56f, 1.06),CCScaleTo::create(0.6f, 1.0),CCScaleTo::create(0.76f, 0.5),CCDelayTime::create(0.08f),NULL);
        CCSequence* seqRotation = CCSequence::create(CCRotateBy::create(1.92f, 0),CCDelayTime::create(0.08f),NULL);
        CCSequence* seqOp = CCSequence::create(CCFadeTo::create(0.48f, 76.5),CCDelayTime::create(0.88f),CCFadeTo::create(0.56f, 0),CCDelayTime::create(0.08f),NULL);
        CCSpawn* spa = CCSpawn::create(seqScale, seqRotation, seqOp, NULL);
        pSan->runAction(CCRepeatForever::create(spa));
        
        CCSprite* pSan2 = CCSprite::create("charge/chargegamebg3.png");
        pSan2->setPosition(ccp(pToCmpt->getContentSize().width/2, pToCmpt->getContentSize().height/2+10));
        pToCmpt->addChild(pSan2, 1);
        ccBlendFunc cbl2 = {GL_SRC_ALPHA, GL_ONE };
        pSan2->setBlendFunc(cbl2);
        pSan2->setScale(0.5f);
        pSan2->setOpacity(0);
        CCSequence* seqScale2 = CCSequence::create(CCDelayTime::create(0.12f),CCScaleTo::create(0.64f, 1.07),CCScaleTo::create(1.12, 0.5),CCDelayTime::create(0.12f),NULL);
        CCSequence* seqRotation2 = CCSequence::create(CCRotateBy::create(1.88f, 0),CCDelayTime::create(0.12f),NULL);
        CCSequence* seqOp2 = CCSequence::create(CCDelayTime::create(0.2f),CCFadeIn::create(0.56),CCDelayTime::create(0.68),CCFadeOut::create(0.44f),CCDelayTime::create(0.12),NULL);
        CCSpawn* spa2 = CCSpawn::create(seqScale2,seqRotation2, seqOp2, NULL);
        pSan2->runAction(CCRepeatForever::create(spa2));
    }
}

void CEnrollRemind::menuCallBack(CCObject* pObj)
{
    CCMenuItem* pTempItem = (CCMenuItem*)pObj;
    int nTempTag = pTempItem->getTag();
    switch (nTempTag)
    {
        case eCmptTag:
        {
            //跳转到比赛界面
            CMainLogic::sharedMainLogic()->ShowMessage("跳转到比赛界面！");
        }
            break;
        case eJoinedTag:
        {
            CMainLogic::sharedMainLogic()->ShowMessage("你已经报过名了，请耐心等待比赛开始。", eMBOK);
            break;
        }
        case eEnrollTag:
        {
            if(CCUserDefault::sharedUserDefault()->getBoolForKey("ShowMatchGuide", true))//进入比赛规则界面
            {
                this -> removeFromParent();
                if(GameMainScene::_instance != NULL)
                {
                    CCMenuItem* item = CCMenuItem::create();
                    item -> setTag(326);
                    GameMainScene::_instance->itemCallBack(item);
                }
            }
            else//报名
            {
                if(GameMainScene::_instance != NULL)
                {
                    CMD_GRR_MatchJoin MatchJoin = {0};
                    MatchJoin.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                    MatchJoin.nMatchID = GameMainScene::_instance->getMatchID();
                    CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_USER, SUB_GR_Match_SIGNUP, &MatchJoin, sizeof(MatchJoin));
                    CMainLogic::sharedMainLogic()->ShowMessage("正在报名，请稍候", eMBGameNull);
                }
            }
        }
            break;
            
        default:
            break;
    }
}

void CEnrollRemind::disappearSchedule(float ft)
{
    this->removeFromParent();
    if(CCUserDefault::sharedUserDefault()->getBoolForKey("ShowMatchGuide", true))
    {
        if(GameMainScene::_instance != NULL)
        {
            GameMainScene::_instance -> showMatchGuide();
        }
    }
}

void CEnrollRemind::setJoined(bool bShowJoined)
{
    if(bShowJoined)
    {
        m_pJoinedMenu->setVisible(true);
        m_pEnrollMenu->setVisible(false);
    }
    else
    {
        m_pJoinedMenu->setVisible(false);
        m_pEnrollMenu->setVisible(true);
    }
}
