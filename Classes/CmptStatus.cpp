//
//  CmptStatus.cpp
//  CatchFish3
//
//  Created by 王军闯 on 16/5/31.
//
//

#include "header.h"

enum
{
    eEnrollSp = 30,
    eRankBgTag,
    eOpenRankTag,
    eCloseRankTag,
    eOpenCmptStatus,
    eCloseCmptStatus,
    eRankTitleTag,
};

CCmptStatus::~CCmptStatus()
{
    m_arrScoreTTF->release();
    m_arrNickNameTTF->release();
}

CCmptStatus* CCmptStatus::create(std::string sCmptName, SCORE nCurntScore, int nCurntRank, int nStayTime, int nMatchNum)
{
    CCmptStatus* pRet = new CCmptStatus;
    if(pRet && pRet->init(sCmptName, nCurntScore, nCurntRank, nStayTime, nMatchNum))
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

bool CCmptStatus::init(std::string sCmptName, SCORE nCurntScore, int nCurntRank, int nStayTime, int nMatchNum)
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_bRankOpened = false;
    m_arrNickNameTTF = CCArray::create();
    m_arrNickNameTTF -> retain();
    m_arrScoreTTF = CCArray::create();
    m_arrScoreTTF -> retain();
    initUI(sCmptName, nCurntScore, nCurntRank, nStayTime, nMatchNum);
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

void CCmptStatus::initUI(std::string sCmptName, SCORE nCurntScore, int nCurntRank, int nStayTime, int nMatchNum)
{
    m_nMatchNum = nMatchNum;
    m_nStayTime = nStayTime;
    CCSprite* pEnrollBg = CCSprite::create("competition/enrollBg.png");//报名背景
    pEnrollBg->setPosition(ccp(-pEnrollBg->getContentSize().width/2, CLIENT_HEIGHT* 0.8f-40));
    this->addChild(pEnrollBg, 0, eEnrollSp);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(CEnrollRemind::showBisaiMenu));
    CCSprite* pLogo = CCSprite::create("competition/cmptBtn.png");//比赛Logo
    pLogo->setPosition(ccp(pEnrollBg->getContentSize().width*0.11f-6, pEnrollBg->getContentSize().height*0.52f+8-16));//10
    pEnrollBg->addChild(pLogo);
    pEnrollBg->runAction(CCSequence::create(CCMoveBy::create(0.3f, ccp(pEnrollBg->getContentSize().width, 0)), func, NULL));
    CCLabelTTF* pCmptNameTTF = CCLabelTTF::create(sCmptName.c_str(), "Arial-BoldMT", 28);//比赛名称
    pCmptNameTTF->setAnchorPoint(ccp(0, 0.5f));
    pCmptNameTTF->setPosition(ccp(pEnrollBg->getContentSize().width*0.3f-47, pEnrollBg->getContentSize().height*0.8f));
    pEnrollBg->addChild(pCmptNameTTF, 1, eRankTitleTag);
    pCmptNameTTF->setColor(ccYELLOW);
    CCLabelTTF* pScoreLabelTTF = CCLabelTTF::create("当前积分：", "Arial-BoldMT", 28);//当前积分标签
    pScoreLabelTTF->setAnchorPoint(ccp(0, 0.5f));
    pScoreLabelTTF->setPosition(ccp(pCmptNameTTF->getPositionX(), pEnrollBg->getContentSize().height*0.6f));
    pEnrollBg->addChild(pScoreLabelTTF);
    pScoreLabelTTF->setColor(ccYELLOW);
    char sScore[16];
    sprintf(sScore, "%lld", nCurntScore);
    m_pCrntScore = CCLabelTTF::create(sScore, "Arial-BoldMT", 28);//当前积分
    m_pCrntScore->setAnchorPoint(ccp(0, 0.5f));
    m_pCrntScore->setPosition(ccp(pScoreLabelTTF->getContentSize().width, pScoreLabelTTF->getContentSize().height/2));
    pScoreLabelTTF->addChild(m_pCrntScore);
    m_pCrntScore->setColor(ccYELLOW);
    CCLabelTTF* pRankLabelTTF = CCLabelTTF::create("当前排名：", "Arial-BoldMT", 28);//当前排名标签
    pRankLabelTTF->setAnchorPoint(ccp(0, 0.5f));
    pRankLabelTTF->setPosition(ccp(pCmptNameTTF->getPositionX(), pEnrollBg->getContentSize().height*0.38f));
    pEnrollBg->addChild(pRankLabelTTF);
    pRankLabelTTF->setColor(ccYELLOW);
    char sRank[16];
    sprintf(sRank, "%d", nCurntRank);
    m_pCrntRank = CCLabelTTF::create(sRank, "Arial-BoldMT", 28);//当前排名
    m_pCrntRank->setAnchorPoint(ccp(0, 0.5f));
    m_pCrntRank->setPosition(ccp(pRankLabelTTF->getContentSize().width, pRankLabelTTF->getContentSize().height/2));
    pRankLabelTTF->addChild(m_pCrntRank);
    m_pCrntRank->setColor(ccYELLOW);
    
    CCSprite* pRemind = CCSprite::create("competition/cmptRemind.png");
    pRemind->setPosition(ccp(pEnrollBg->getContentSize().width*0.7f-20, pEnrollBg->getContentSize().height*0.18f));
    pEnrollBg->addChild(pRemind);
    char sRemind[8];
    sprintf(sRemind, "%d:%d", m_nStayTime/60, m_nStayTime%60);//倒计时提醒
    m_pRemindAtlas = CCLabelAtlas::create(sRemind, "competition/remindNum.png", 19, 23, '0');
    m_pRemindAtlas->setAnchorPoint(CCPointZero);
    m_pRemindAtlas->setPosition(ccp(pRemind->getContentSize().width*1.08f, 3));
    pRemind->addChild(m_pRemindAtlas);
    remindSchedule(0);
    this->schedule(schedule_selector(CCmptStatus::remindSchedule), 1);
    //创建按钮
    CCSprite* pCloseCmpt = CCSprite::create("competition/backBtn.png");//关闭状态框
    m_pCloseCmptItem = CCMenuItemSprite::create(pCloseCmpt, pCloseCmpt, this, menu_selector(CCmptStatus::menuCallBack));
    m_pCloseCmptItem->setPosition(ccp(pEnrollBg->getContentSize().width*0.94f-10, pEnrollBg->getContentSize().height*0.5f));
    m_pCloseCmptItem->setTag(eCloseCmptStatus);
    CCSprite* pOpenCmpt = CCSprite::create("competition/backBtn.png");//打开状态框
    pOpenCmpt->setFlipX(true);
    m_pOpenCmptItem = CCMenuItemSprite::create(pOpenCmpt, pOpenCmpt, this, menu_selector(CCmptStatus::menuCallBack));
    m_pOpenCmptItem->setPosition(m_pCloseCmptItem->getPosition());
    m_pOpenCmptItem->setTag(eOpenCmptStatus);
    CCSprite* pOpenRank = CCSprite::create("competition/openBtn.png");//打开排行榜
    m_pOpenRankItem = CCMenuItemSprite::create(pOpenRank, pOpenRank, this, menu_selector(CCmptStatus::menuCallBack));
    m_pOpenRankItem->setPosition(ccp(pEnrollBg->getContentSize().width/2, pEnrollBg->getContentSize().height*0.16f));
    m_pOpenRankItem->setTag(eOpenRankTag);
    CCSprite* pCloseRank = CCSprite::create("competition/openBtn.png");//关闭排行榜
    pCloseRank->setFlipY(true);
    m_pCloseRankItem = CCMenuItemSprite::create(pCloseRank, pCloseRank, this, menu_selector(CCmptStatus::menuCallBack));
    m_pCloseRankItem->setPosition(m_pOpenRankItem->getPosition());
    m_pCloseRankItem->setTag(eCloseRankTag);
    CCMenu* pMenu = CCMenu::create(m_pCloseCmptItem, m_pOpenCmptItem, m_pOpenRankItem, m_pCloseRankItem, NULL);
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    pEnrollBg->addChild(pMenu);
    pMenu->setTouchPriority(-300);
    m_pOpenCmptItem->setVisible(false);
    m_pCloseRankItem->setVisible(false);
    //加载排行界面
    CCSprite* pRankBg = CCSprite::create("competition/enrollRankBg.png");
    pRankBg->setPosition(ccp(pEnrollBg->getContentSize().width/2, -pEnrollBg->getContentSize().height*1.35f+10));
    pEnrollBg->addChild(pRankBg, 0, eRankBgTag);
    
    CCSprite* pPerRankBg = CCSprite::create("competition/enrollPerRankBg.png");
    pPerRankBg->setPosition(ccp(pRankBg->getContentSize().width/2, pRankBg->getContentSize().height-pPerRankBg->getContentSize().height/2));
    pRankBg->addChild(pPerRankBg);
    CCSprite* pInfo = CCSprite::create("competition/topInfo.png");
    pInfo->setPosition(ccp(pPerRankBg->getContentSize().width/2, pPerRankBg->getContentSize().height/2));
    pPerRankBg->addChild(pInfo);
    
    CCScrollView* pRankScrollView = CCScrollView::create();
    pRankScrollView->setAnchorPoint(ccp(0, 0));
    pRankScrollView->setPosition(ccp(pRankBg->getContentSize().width*0.01f, pRankBg->getContentSize().height*0.01f));
    pRankScrollView->setContentSize(CCSizeMake(pRankBg->getContentSize().width*0.98f, pRankBg->getContentSize().height*0.98f-pPerRankBg->getContentSize().height));
    pRankScrollView->setViewSize(CCSizeMake(pRankBg->getContentSize().width*0.98f, pRankBg->getContentSize().height*0.98f-pPerRankBg->getContentSize().height));
    
    CCLayer* pContainerLayer = CCLayer::create();//创建一个层，作为滚动的内容
    pRankScrollView->setContainer(pContainerLayer);
    pRankScrollView->setBounceable(false);
    pRankScrollView->setDelegate(this);
    pRankScrollView->setTouchPriority(-301);
    pRankScrollView->setDirection(kCCScrollViewDirectionVertical);
    pRankBg->addChild(pRankScrollView);
    
    pContainerLayer -> setContentSize(ccp(pRankBg->getContentSize().width*0.98f, 63*(m_nMatchNum)+20));
    pContainerLayer->setPosition(ccp(0, -63*((float)m_nMatchNum-6.0f)-10));
    
    for (int i = 0 ; i < m_nMatchNum; i++)
    {
        CCSprite* pPerRankBg = CCSprite::create("competition/enrollPerRankBg.png");
        pPerRankBg->setPosition(ccp(pRankBg->getContentSize().width/2, (pPerRankBg->getContentSize().height)*(m_nMatchNum-i)-pPerRankBg->getContentSize().height/2+20));
        pContainerLayer->addChild(pPerRankBg);
        CCLabelTTF* m_pNickNameTTF = CCLabelTTF::create("0", "", 24);
        m_pNickNameTTF->setPosition(ccp(pPerRankBg->getContentSize().width*0.485f, pPerRankBg->getContentSize().height/2));
        pPerRankBg->addChild(m_pNickNameTTF);
        m_arrNickNameTTF -> addObject(m_pNickNameTTF);
        
        CCLabelTTF* m_pScoreTTF = CCLabelTTF::create("0", "", 24);
        m_pScoreTTF->setPosition(ccp(pPerRankBg->getContentSize().width*0.818f, pPerRankBg->getContentSize().height/2));
        pPerRankBg->addChild(m_pScoreTTF);
        m_arrScoreTTF -> addObject(m_pScoreTTF);
        if (i == 0)
        {
            m_pNickNameTTF->setColor(ccc3(56, 172, 252));
            m_pScoreTTF->setColor(ccc3(56, 172, 252));
            CCSprite* pLevel = CCSprite::create( "competition/first.png");
            pLevel->setPosition(ccp(pPerRankBg->getContentSize().width*0.18f, pPerRankBg->getContentSize().height/2));
            pPerRankBg->addChild(pLevel);
        }
        else if(i == 1)
        {
            m_pNickNameTTF->setColor(ccc3(220, 110, 57));
            m_pScoreTTF->setColor(ccc3(220, 110, 57));
            CCSprite* pLevel = CCSprite::create( "competition/second.png");
            pLevel->setPosition(ccp(pPerRankBg->getContentSize().width*0.18f, pPerRankBg->getContentSize().height/2));
            pPerRankBg->addChild(pLevel);
        }
        else if(i == 2)
        {
            m_pNickNameTTF->setColor(ccc3(187, 234, 89));
            m_pScoreTTF->setColor(ccc3(187, 234, 89));
            CCSprite* pLevel = CCSprite::create( "competition/third.png");
            pLevel->setPosition(ccp(pPerRankBg->getContentSize().width*0.18f, pPerRankBg->getContentSize().height/2));
            pPerRankBg->addChild(pLevel);
        }
        else
        {
            char sLevel[8];
            sprintf(sLevel, "%d", i+1);
            CCLabelAtlas* pLevelAtlas = CCLabelAtlas::create(sLevel, "competition/enrollRankNum.png", 26, 31, '0');
            pLevelAtlas->setAnchorPoint(ccp(0.5f, 0.5f));
            pLevelAtlas->setPosition(ccp(pPerRankBg->getContentSize().width*0.18f, pPerRankBg->getContentSize().height/2));
            pPerRankBg->addChild(pLevelAtlas);
        }
        if(i >= 0 && i <= 2)//前三名加特效
        {
            CCSprite* pRankLight = CCSprite::createWithSpriteFrameName("rankLight0.png");
            pRankLight->setPosition(ccp(pPerRankBg->getContentSize().width/2, pPerRankBg->getContentSize().height/2));
            pPerRankBg->addChild(pRankLight);
            pRankLight->runAction(createRankLight());
        }
    }
    pRankBg->setVisible(false);
}

CCAnimate* CCmptStatus::createRankLight()
{
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCArray* array = CCArray::create();
    char temp[16];
    for(int i = 0; i < 70; i++)
    {
        sprintf(temp, "rankLight%d.png", i);
        CCSpriteFrame* frame = cache->spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, 0.05f);
    animation->setLoops(-1);
    CCAnimate* animate = CCAnimate::create(animation);
    return animate;
}

void CCmptStatus::changeInfo(SCORE nCurntScore, int nCurntRank, int nStayTime)
{
    char sScore[16];
    sprintf(sScore, "%lld", nCurntScore);
    m_pCrntScore->setString(sScore);
    char sRank[16];
    sprintf(sRank, "%d", nCurntRank);
    m_pCrntRank->setString(sRank);
    m_nStayTime = nStayTime;
}

void CCmptStatus::menuCallBack(CCObject* pObj)
{
    CCSprite* pEnrollSp = (CCSprite*)this->getChildByTag(eEnrollSp);
    if (pEnrollSp == NULL)
    {
        return;
    }
    CCSprite* pRankBg = (CCSprite*)pEnrollSp->getChildByTag(eRankBgTag);
    if (pRankBg == NULL)
    {
        return;
    }
    CCMenuItem* pTempItem = (CCMenuItem*)pObj;
    int nTempTag = pTempItem->getTag();
    switch (nTempTag)
    {
        case eCloseCmptStatus://关闭比赛状态
        {
            m_pCloseCmptItem->setVisible(false);
            m_pOpenCmptItem->setVisible(true);
            pRankBg->setVisible(false);
            m_pRemindAtlas->setVisible(false);
            pEnrollSp->runAction(CCMoveBy::create(0.2f, ccp(-pEnrollSp->getContentSize().width+m_pCloseCmptItem->getContentSize().width*1.5f, 0)));
            CCLabelTTF* pLabel = (CCLabelTTF*)pEnrollSp->getChildByTag(eRankTitleTag);
            if(pLabel != NULL)
            {
                pLabel -> setVisible(false);
            }
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
        }
            break;
        case eOpenCmptStatus://打开比赛状态
        {
            m_pCloseCmptItem->setVisible(true);
            m_pOpenCmptItem->setVisible(false);
            if(m_bRankOpened)
            {
                pRankBg->setVisible(true);
            }
            m_pRemindAtlas->setVisible(true);
            CCLabelTTF* pLabel = (CCLabelTTF*)pEnrollSp->getChildByTag(eRankTitleTag);
            if(pLabel != NULL)
            {
                pLabel -> setVisible(true);
            }
            CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(CCmptStatus::showBisaiMenu));
            pEnrollSp->runAction(CCSequence::create(CCMoveBy::create(0.2f, ccp(pEnrollSp->getContentSize().width-m_pCloseCmptItem->getContentSize().width*1.5f, 0)), func, NULL));
        }
            break;
        case eOpenRankTag://打开排行
        {
            m_pOpenRankItem->setVisible(false);
            m_pCloseRankItem->setVisible(true);
            pRankBg->setVisible(true);
            m_bRankOpened = true;
        }
            break;
        case eCloseRankTag://关闭排行
        {
            m_pOpenRankItem->setVisible(true);
            m_pCloseRankItem->setVisible(false);
            m_bRankOpened = false;
            pRankBg->setVisible(false);
        }
            break;
            
        default:
            break;
    }
}
                                 
void CCmptStatus::showBisaiMenu()
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

void CCmptStatus::changeRankInfo(std::vector<CMD_GRO_MatchGroupInfo> vMatchGroupInfo)
{
    char sScore[16];
    if(vMatchGroupInfo.size() != m_arrNickNameTTF->count())
    {
        return ;
    }
    if(vMatchGroupInfo.size() != m_arrScoreTTF->count())
    {
        return ;
    }
    for (int i = 0 ; i < vMatchGroupInfo.size(); i++)
    {
        sprintf(sScore, "%lld", vMatchGroupInfo[i].lScore);
        CCLabelTTF* m_pScoreTTF = (CCLabelTTF*)m_arrScoreTTF->objectAtIndex(i);
        if(m_pScoreTTF != NULL)
        {
            m_pScoreTTF->setString(sScore);
        }
        CCLabelTTF* m_pNickNameTTF = (CCLabelTTF*)m_arrNickNameTTF->objectAtIndex(i);
        if(m_pNickNameTTF != NULL)
        {
            m_pNickNameTTF->setString(CCharCode::GB2312ToUTF8(vMatchGroupInfo[i].szNickName).c_str());
        }
    }
}

void CCmptStatus::remindSchedule(float ft)
{
    m_nStayTime--;
    char sRemind[8];
    if (m_nStayTime/60 < 10 && m_nStayTime%60 < 10)
    {
        sprintf(sRemind, "0%d:0%d", m_nStayTime/60, m_nStayTime%60);//倒计时提醒
    }
    else if (m_nStayTime/60 < 10 && m_nStayTime%60 >= 10)
    {
        sprintf(sRemind, "0%d:%d", m_nStayTime/60, m_nStayTime%60);//倒计时提醒
    }
    else if (m_nStayTime/60 >= 10 && m_nStayTime%60 >= 10)
    {
        sprintf(sRemind, "%d:%d", m_nStayTime/60, m_nStayTime%60);//倒计时提醒
    }
    else if (m_nStayTime/60 >= 10 && m_nStayTime%60 < 10)
    {
        sprintf(sRemind, "%d:0%d", m_nStayTime/60, m_nStayTime%60);//倒计时提醒
    }
    m_pRemindAtlas->setString(sRemind);
    if (m_nStayTime == 0)
    {
        this->unschedule(schedule_selector(CCmptStatus::remindSchedule));
    }
}

void CCmptStatus::scrollViewDidScroll(CCScrollView* view)
{
}

void CCmptStatus::scrollViewDidZoom(CCScrollView* view)
{
}

