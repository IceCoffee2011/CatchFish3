//
//  RankNewGuideLayer.cpp
//  CatchFish3
//
//  Created by Guang on 16/6/2.
//
//

#include "header.h"

bool CRankNewGuideLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h2all/byguidejian.plist");
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 130), CLIENT_WIDTH, CLIENT_HEIGHT);
    pLayerColor->setTag(10);
    CCClippingNode* Clipper = CCClippingNode::create();
    Clipper->setAnchorPoint(ccp(0, 0));
    Clipper->setPosition(ccp(0, 0));
    Clipper->setInverted(true);
    Clipper->setAlphaThreshold(0.5f);
    m_pGuideStencil = CCNode::create();
    Clipper->setStencil(m_pGuideStencil);
    CCSprite* pMatchLeftInfo = CCSprite::create("rank/matchItem1.png");
    pMatchLeftInfo->setPosition(ccp(208, CLIENT_HEIGHT/2+190));
    m_pGuideStencil->addChild(pMatchLeftInfo, -1, 1001);
    Clipper->addChild(pLayerColor);
    this->addChild(Clipper, 1);
    
    CCSprite* pMatchRightInfo = CCSprite::create("rank/matchNodeInfoBg.png");
    pMatchRightInfo->setScaleY(0.7f);
    pMatchRightInfo->setPosition(ccp(843, 338));//317  481
    m_pGuideStencil->addChild(pMatchRightInfo, -1, 1002);
    pMatchRightInfo->setVisible(false);
    
    CCMenuItemImage* joinFree = CCMenuItemImage::create("rank/matchJoinFree.png", "rank/matchJoinFree.png", this, menu_selector(CRankNewGuideLayer::callBack));
    CCMenuItemImage* closeItem = CCMenuItemImage::create("v2ip/vipcloseitem.png", "v2ip/vipcloseitem.png",this,menu_selector(CRankNewGuideLayer::callBack));
    joinFree->setTag(101);
    closeItem->setTag(102);
    closeItem->setScale(0.7f);
    joinFree->setPosition(ccp(CLIENT_WIDTH/2+200, 80));
    closeItem->setPosition(ccp(1237.5, 677.5));
    m_pMatchGuideMenu = CCMenu::create(joinFree, closeItem, NULL);
    m_pMatchGuideMenu->setPosition(ccp(0, 0));
    m_pMatchGuideMenu->setTouchPriority(-202);
    this->addChild(m_pMatchGuideMenu, 1);
    joinFree->setVisible(false);
    closeItem->setVisible(false);
    
    m_pGuideFirst = CCSprite::create("rank/guide1.png");
    m_pGuideSecond = CCSprite::create("rank/guide2.png");
    m_pGuideThird = CCSprite::create("rank/guide3.png");
    m_pGuideFourth = CCSprite::create("rank/guide4.png");
    m_pGuideFirst->setPosition(ccp(590, 530));
    m_pGuideSecond->setPosition(ccp(220, 430));
    m_pGuideThird->setPosition(ccp(CLIENT_WIDTH/2+200, 290));
    m_pGuideFourth->setPosition(ccp(930, 530));
    m_pGuideFirst->setScale(1.2f);
    m_pGuideSecond->setScale(1.2f);
    m_pGuideThird->setScale(1.2f);
    m_pGuideFourth->setScale(1.2f);
    this->addChild(m_pGuideFirst, 1);
    this->addChild(m_pGuideSecond, 1);
    this->addChild(m_pGuideThird, 1);
    this->addChild(m_pGuideFourth, 1);
    m_pGuideSecond->setVisible(false);
    m_pGuideThird->setVisible(false);
    m_pGuideFourth->setVisible(false);
    
    m_pGuideArrow = CCSprite::createWithSpriteFrameName("byguidejian2.png");
    m_pGuideArrow->setPosition(ccp(430, 550));
    this->addChild(m_pGuideArrow, 1);
    CCRepeatForever* pRep = CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f, ccp(10, 0)), CCMoveBy::create(0.5f, ccp(-10, 0)), NULL));
    m_pGuideArrow->runAction(pRep);
    m_pGuideArrow->setRotation(90);
    
    
    this->setTouchEnabled(true);
    
    m_nGuideIndex = 1;
    
    return true;
}

void CRankNewGuideLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -201, true);
}

bool CRankNewGuideLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    
    if (m_nGuideIndex == 1)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        m_pGuideFirst->setVisible(false);
        m_pGuideSecond->setVisible(true);
        m_pGuideArrow->setPosition(ccp(380, 430));
        m_pGuideArrow->setRotation(-90);
        if (m_pGuideStencil->getChildByTag(1001) != NULL)
        {
            m_pGuideStencil->getChildByTag(1001)->setVisible(false);
        }
        if (m_pGuideStencil->getChildByTag(1002) != NULL)
        {
            m_pGuideStencil->getChildByTag(1002)->setVisible(true);
        }
        m_nGuideIndex = 2;
    }
    else if (m_nGuideIndex == 2)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        m_pGuideSecond->setVisible(false);
        m_pGuideThird->setVisible(true);
        m_pGuideArrow->setPosition(ccp(CLIENT_WIDTH/2+200, 160));
        m_pGuideArrow->setRotation(0);
        m_pGuideArrow->stopAllActions();
        CCRepeatForever* pRep = CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f, ccp(0, 10)), CCMoveBy::create(0.5f, ccp(0, -10)), NULL));
        m_pGuideArrow->runAction(pRep);
        if (m_pGuideStencil->getChildByTag(1002) != NULL)
        {
            m_pGuideStencil->getChildByTag(1002)->setVisible(false);
        }
        setMenuItemVisible(101, true);
        m_nGuideIndex = 3;
    }
    return true;
}

void CRankNewGuideLayer::callBack(CCObject* obj)
{
    CCMenuItem* item = (CCMenuItem*)obj;
    switch (item->getTag()) {
        case 101:
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            //报名
            m_pGuideThird->setVisible(false);
            m_pGuideFourth->setVisible(true);
            m_pGuideArrow->setPosition(ccp(1170, 620));
            m_pGuideArrow->setRotation(-135);
            m_pGuideArrow->stopAllActions();
            CCRepeatForever* pRep = CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f, ccp(10, 10)), CCMoveBy::create(0.5f, ccp(-10, -10)), NULL));
            m_pGuideArrow->runAction(pRep);
            setMenuItemVisible(101, false);
            setMenuItemVisible(102, true);
            
            int nMatchID=-1;
            int nCost = 0;
            CRankNewLayer* pParentLayer = (CRankNewLayer*)this->getParent();
            if (pParentLayer != NULL) {
                for (int i=0; i<pParentLayer->m_MatchingVct.size(); i++)
                {
                    nCost = atoi(pParentLayer->m_MatchingVct[i]->sMatchCost.c_str());
                    if (nCost <= 0)
                    {
                        nMatchID = atoi(pParentLayer->m_MatchingVct[i]->sMatchIndex.c_str());
                        pParentLayer->setAlreadyJoinedVisible();
                        break;
                    }
                }
            }
            //向服务端发送报名消息（争光改），当收到报名成功的时候要判断是否是比赛指引进行的报名，此时报名成功不做处理
            CMD_GRR_MatchJoin MatchJoin = {0};
            MatchJoin.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            MatchJoin.nMatchID = nMatchID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_USER, SUB_GR_Match_SIGNUP, &MatchJoin, sizeof(MatchJoin));
            break;
        }
        case 102:
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
            CRankNewLayer* pParentLayer = (CRankNewLayer*)this->getParent();
            if (pParentLayer != NULL)
            {
                CCMenuItem* item = CCMenuItem::create();
                item -> setTag(1001);
                pParentLayer->callBack(item);
            }
            break;
        }
          default:
            break;
    }
}

void CRankNewGuideLayer::setMenuItemVisible(int tag, bool bShow)
{
    if (m_pMatchGuideMenu == NULL) {
        return ;
    }
    for (int i=0; i<m_pMatchGuideMenu->getChildrenCount(); i++)
    {
        CCMenuItem* item = (CCMenuItem*)m_pMatchGuideMenu->getChildren()->objectAtIndex(i);
        if (item != NULL && tag==item->getTag())
        {
            if (tag == item->getTag())
            {
                item->setVisible(bShow);
                break;
            }
        }
    }
}
