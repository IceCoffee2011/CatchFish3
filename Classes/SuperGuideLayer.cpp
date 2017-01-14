#include "header.h"

SuperGuideLayer* SuperGuideLayer::create()
{
    SuperGuideLayer* pRet = new SuperGuideLayer();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool SuperGuideLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    
    
    CCLayerColor* pColorLayer1 = CCLayerColor::create(ccc4(0, 0, 0, 150), CLIENT_WIDTH, CLIENT_HEIGHT);
    this -> addChild(pColorLayer1);
    
    m_nCurrentIndex = 1;
    
    loadUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void SuperGuideLayer::loadUI()
{
    m_node1 = CCNode::create();
    this -> addChild(m_node1);
    CCSprite* p1Guide = CCSprite::createWithSpriteFrameName("superguide1.png");
    p1Guide -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    m_node1 -> addChild(p1Guide);
    
    m_node2 = CCNode::create();
    this -> addChild(m_node2);
    m_node2 -> setVisible(false);
    CCSprite* p2Guide1 = CCSprite::createWithSpriteFrameName("superguide2.png");
    p2Guide1 -> setPosition(ccp(CLIENT_WIDTH/3+130, CLIENT_HEIGHT/4+20));
    m_node2 -> addChild(p2Guide1);
    CCSprite* p2Guide2 = CCSprite::createWithSpriteFrameName("superguide3.png");
    p2Guide2 -> setPosition(ccp(CLIENT_WIDTH/3*2+170, CLIENT_HEIGHT/4+120));
    m_node2 -> addChild(p2Guide2);
    CCSprite* pJian1 = CCSprite::createWithSpriteFrameName("SuperPirateJian.png");
    CCSprite* pJian2 = CCSprite::createWithSpriteFrameName("SuperPirateJian2.png");
    CCMenuItemSprite* jianItem = CCMenuItemSprite::create(pJian1, pJian2, this, menu_selector(SuperGuideLayer::clickMenu));
    jianItem -> setPosition(ccp(CLIENT_WIDTH/5*2-150, 80));
    jianItem -> setTag(101);
    CCSprite* pJia1 = CCSprite::createWithSpriteFrameName("SuperPirateJia.png");
    CCSprite* pJia2 = CCSprite::createWithSpriteFrameName("SuperPirateJia2.png");
    CCMenuItemSprite* jiaItem = CCMenuItemSprite::create(pJia1, pJia2, this, menu_selector(SuperGuideLayer::clickMenu));
    jiaItem -> setPosition(ccp(CLIENT_WIDTH/5*3-84, 80));
    jiaItem -> setTag(102);
    CCSprite* pStart1 = CCSprite::createWithSpriteFrameName("SuperPirateStart.png");
    CCSprite* pStart2 = CCSprite::createWithSpriteFrameName("SuperPirateStart2.png");
    CCMenuItemSprite* startItem = CCMenuItemSprite::create(pStart1, pStart2, this, menu_selector(SuperGuideLayer::clickMenu));
    startItem -> setPosition(ccp(CLIENT_WIDTH/5*4+10, 93));
    startItem -> setTag(103);
    CCMenu* p2Menu = CCMenu::create(jiaItem, jianItem, startItem, NULL);
    p2Menu -> setAnchorPoint(CCPointZero);
    p2Menu -> setPosition(CCPointZero);
    m_node2 -> addChild(p2Menu, 100);
    p2Menu -> setTouchPriority(-200);
    CCSprite* pYafen = CCSprite::createWithSpriteFrameName("SuperPirateYafen.png");
    pYafen -> setPosition(ccp(CLIENT_WIDTH/5*2+10,72));
    m_node2 -> addChild(pYafen);
    CCLabelAtlas* pBetBgAtlas = CCLabelAtlas::create("999999", "SuperPirate/gameBetNum.png", 25, 36, '0');
    pBetBgAtlas -> setOpacity(50);
    pBetBgAtlas -> setAnchorPoint(ccp(0.5, 0.5));
    pBetBgAtlas -> setPosition(ccp(pYafen->getContentSize().width/2, pYafen->getContentSize().height/2+10));
    pYafen -> addChild(pBetBgAtlas, 2);
    m_labelYa = CCLabelAtlas::create("1000", "SuperPirate/gameBetNum.png", 25, 36, '0');
    m_labelYa -> setPosition(ccp(pYafen->getContentSize().width-40, pYafen->getContentSize().height/2+10));
    m_labelYa -> setAnchorPoint(ccp(1, 0.5f));
    pYafen -> addChild(m_labelYa, 2);
    CCSprite* p2Jiantou = CCSprite::createWithSpriteFrameName("superguidejian1.png");
    p2Jiantou -> setPosition(ccp(CLIENT_WIDTH/3*2+170, CLIENT_HEIGHT/4+20));
    m_node2 -> addChild(p2Jiantou);
    p2Jiantou -> runAction(CCRepeatForever::create(getAnimate()));
    p2Jiantou -> runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.3f, ccp(0, -10)), CCMoveBy::create(0.3f, ccp(0, 10)), NULL)));
    
    
    m_node3 = CCNode::create();
    this -> addChild(m_node3);
    m_node3 -> setVisible(false);
    CCSprite* p3Guide1 = CCSprite::createWithSpriteFrameName("superguide4.png");
    p3Guide1 -> setPosition(ccp(CLIENT_WIDTH/3*2+170, CLIENT_HEIGHT/4+120));
    m_node3 -> addChild(p3Guide1);
    CCSprite* p3Jiantou = CCSprite::createWithSpriteFrameName("superguidejian1.png");
    p3Jiantou -> setPosition(ccp(CLIENT_WIDTH/3*2+170, CLIENT_HEIGHT/4+20));
    m_node3 -> addChild(p3Jiantou);
    p3Jiantou -> runAction(CCRepeatForever::create(getAnimate()));
    p3Jiantou -> runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.3f, ccp(0, -10)), CCMoveBy::create(0.3f, ccp(0, 10)), NULL)));
    CCSprite* pStop1 = CCSprite::createWithSpriteFrameName("SuperPirateStop.png");
    CCSprite* pStop2 = CCSprite::createWithSpriteFrameName("SuperPirateStop2.png");
    CCMenuItemSprite* stopItem = CCMenuItemSprite::create(pStop1, pStop2, this, menu_selector(SuperGuideLayer::clickMenu));
    stopItem -> setPosition(ccp(CLIENT_WIDTH/5*4+10, 95));
    stopItem -> setTag(104);
    CCMenu* p3Menu = CCMenu::create(stopItem, NULL);
    p3Menu -> setAnchorPoint(CCPointZero);
    p3Menu -> setPosition(CCPointZero);
    m_node3 -> addChild(p3Menu, 100);
    p3Menu -> setTouchPriority(-200);
    
    m_node4 = CCNode::create();
    this -> addChild(m_node4);
    m_node4 -> setVisible(false);
    CCSprite* p4Guide1 = CCSprite::createWithSpriteFrameName("superguide5.png");
    p4Guide1 -> setPosition(ccp(CLIENT_WIDTH/5*3+90, CLIENT_HEIGHT/2+150));
    m_node4 -> addChild(p4Guide1, 2);
    CCSprite* p4Guide2 = CCSprite::createWithSpriteFrameName("superguide6.png");
    p4Guide2 -> setPosition(ccp(CLIENT_WIDTH/5*4+100, CLIENT_HEIGHT/4+30));
    m_node4 -> addChild(p4Guide2, 2);
    CCSprite* p4Jiantou1 = CCSprite::createWithSpriteFrameName("superguidejian1.png");
    p4Jiantou1 -> setPosition(ccp(CLIENT_WIDTH/5*3, CLIENT_HEIGHT/2+50));
    m_node4 -> addChild(p4Jiantou1);
    p4Jiantou1 -> runAction(CCRepeatForever::create(getAnimate()));
    p4Jiantou1 -> runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.3f, ccp(0, -10)), CCMoveBy::create(0.3f, ccp(0, 10)), NULL)));
    CCSprite* p4Jiantou2 = CCSprite::createWithSpriteFrameName("superguidejian1.png");
    p4Jiantou2 -> setPosition(ccp(CLIENT_WIDTH/5*3+155, CLIENT_HEIGHT/2+50));
    m_node4 -> addChild(p4Jiantou2);
    p4Jiantou2 -> runAction(CCRepeatForever::create(getAnimate()));
    p4Jiantou2 -> runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.3f, ccp(0, -10)), CCMoveBy::create(0.3f, ccp(0, 10)), NULL)));
    CCSprite* pNan1 = CCSprite::createWithSpriteFrameName("SuperPiratenan1.png");
    CCSprite* pNan2 = CCSprite::createWithSpriteFrameName("SuperPiratenan2.png");
    CCMenuItemSprite* nanItem = CCMenuItemSprite::create(pNan1, pNan2, this, menu_selector(SuperGuideLayer::clickMenu));
    nanItem -> setPosition(ccp(770, CLIENT_HEIGHT/2-75));
    nanItem -> setTag(105);
    CCSprite* pNv1 = CCSprite::createWithSpriteFrameName("SuperPiratenv1.png");
    CCSprite* pNv2 = CCSprite::createWithSpriteFrameName("SuperPiratenv2.png");
    CCMenuItemSprite* nvItem = CCMenuItemSprite::create(pNv1, pNv2, this, menu_selector(SuperGuideLayer::clickMenu));
    nvItem -> setPosition(ccp(923, CLIENT_HEIGHT/2-75));
    nvItem -> setTag(106);
    CCMenu* p4Menu = CCMenu::create(nanItem, nvItem, NULL);
    p4Menu -> setAnchorPoint(CCPointZero);
    p4Menu -> setPosition(CCPointZero);
    m_node4 -> addChild(p4Menu, 100);
    p4Menu -> setTouchPriority(-200);
    CCSprite* pMoneyBg1 = CCSprite::createWithSpriteFrameName("moneyBg1.png");
    pMoneyBg1->setPosition(ccp(846, 170));
    m_node4->addChild(pMoneyBg1);
    m_p4CurrentBetNum = CCLabelAtlas::create("2000", "SuperPirate/currentBetNum.png", 35, 60, '0');
    m_p4CurrentBetNum->setAnchorPoint(ccp(0.5f, 0.5f));
    m_p4CurrentBetNum->setPosition(ccp(846, 170));
    m_node4->addChild(m_p4CurrentBetNum);
    m_p4CurrentBetNum->setScale(0.7f);
    
    m_node5 = CCNode::create();
    this -> addChild(m_node5);
    m_node5 -> setVisible(false);
    CCSprite* p5MoneyBg1 = CCSprite::createWithSpriteFrameName("moneyBg1.png");
    p5MoneyBg1->setPosition(ccp(846, 170));
    m_node5->addChild(p5MoneyBg1);
    m_p5CurrentBetNum = CCLabelAtlas::create("4000", "SuperPirate/currentBetNum.png", 35, 60, '0');
    m_p5CurrentBetNum->setAnchorPoint(ccp(0.5f, 0.5f));
    m_p5CurrentBetNum->setPosition(ccp(846, 170));
    m_node5->addChild(m_p5CurrentBetNum);
    m_p5CurrentBetNum->setScale(0.7f);
    CCSprite* p5Guide1 = CCSprite::createWithSpriteFrameName("superguide7.png");
    p5Guide1 -> setPosition(ccp(CLIENT_WIDTH/2-100, CLIENT_HEIGHT/4+30));
    m_node5 -> addChild(p5Guide1);
    CCSprite* p5Guide2 = CCSprite::createWithSpriteFrameName("superguide8.png");
    p5Guide2 -> setPosition(ccp(CLIENT_WIDTH/5*4, CLIENT_HEIGHT/4+110));
    m_node5 -> addChild(p5Guide2);
    CCSprite* p5Jiantou = CCSprite::createWithSpriteFrameName("superguidejian1.png");
    p5Jiantou -> setPosition(ccp(CLIENT_WIDTH/3*2+176, CLIENT_HEIGHT/4+20));
    m_node5 -> addChild(p5Jiantou);
    p5Jiantou -> runAction(CCRepeatForever::create(getAnimate()));
    p5Jiantou -> runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.3f, ccp(0, -10)), CCMoveBy::create(0.3f, ccp(0, 10)), NULL)));
    CCSprite* pCal1 = CCSprite::createWithSpriteFrameName("SuperPirateCal.png");
    CCSprite* pCal2 = CCSprite::createWithSpriteFrameName("SuperPirateCal2.png");
    CCMenuItemSprite* calcItem = CCMenuItemSprite::create(pCal1, pCal2, this, menu_selector(SuperGuideLayer::clickMenu));
    calcItem -> setPosition(ccp(CLIENT_WIDTH/5*4+10, 95));
    calcItem -> setTag(107);
    CCMenu* p5Menu = CCMenu::create(calcItem, NULL);
    p5Menu -> setAnchorPoint(CCPointZero);
    p5Menu -> setPosition(CCPointZero);
    m_node5 -> addChild(p5Menu);
    p5Menu -> setTouchPriority(-200);
    
    m_node6 = CCNode::create();
    this -> addChild(m_node6);
    m_node6 -> setVisible(false);
    CCSprite* p6Guide1 = CCSprite::createWithSpriteFrameName("superguide9.png");
    p6Guide1 -> setPosition(ccp(CLIENT_WIDTH/2-280, CLIENT_HEIGHT/2+120));
    m_node6 -> addChild(p6Guide1);
    CCSprite* p6Guide2 = CCSprite::createWithSpriteFrameName("superguide10.png");
    p6Guide2 -> setPosition(ccp(CLIENT_WIDTH/4*3+130, CLIENT_HEIGHT-70));
    m_node6 -> addChild(p6Guide2);
    CCSprite* p6Guide3 = CCSprite::createWithSpriteFrameName("superguide11.png");
    p6Guide3 -> setPosition(ccp(CLIENT_WIDTH/2-200, CLIENT_HEIGHT/4));
    m_node6 -> addChild(p6Guide3);
    CCSprite* p6Guide4 = CCSprite::createWithSpriteFrameName("superguide12.png");
    p6Guide4 -> setPosition(ccp(CLIENT_WIDTH/2+150, CLIENT_HEIGHT/4));
    m_node6 -> addChild(p6Guide4);
    CCSprite* p6Guide5 = CCSprite::createWithSpriteFrameName("superguide13.png");
    p6Guide5 -> setPosition(ccp(CLIENT_WIDTH/4*3+130, CLIENT_HEIGHT/5*4-30-50));
    m_node6 -> addChild(p6Guide5);
    CCSprite* p6Guide6 = CCSprite::createWithSpriteFrameName("superguide14.png");
    p6Guide6 -> setPosition(ccp(CLIENT_WIDTH/4*3+130, CLIENT_HEIGHT/4*3-80-50));
    m_node6 -> addChild(p6Guide6);
    CCSprite* p6Guide7 = CCSprite::createWithSpriteFrameName("superguide15.png");
    p6Guide7 -> setPosition(ccp(CLIENT_WIDTH/4*3+130, CLIENT_HEIGHT/3*2-100-50));
    m_node6 -> addChild(p6Guide7);
}

void SuperGuideLayer::setShowNode4()
{
    m_node4 -> setVisible(true);
    char temp[16];
    sprintf(temp, "%d", atoi(m_labelYa->getString())*2);
    m_p4CurrentBetNum->setString(temp);
}

void SuperGuideLayer::setShowNode5()
{
    m_node5 -> setVisible(true);
    char temp[16];
    sprintf(temp, "%d", atoi(m_labelYa->getString())*4);
    m_p5CurrentBetNum->setString(temp);
}

void SuperGuideLayer::setShowNode6()
{
    m_node6 -> setVisible(true);
}

CCAnimate* SuperGuideLayer::getAnimate()
{
    CCArray* array = CCArray::create();
    char temp[32];
    for(int i = 1; i <= 2; i++)
    {
        sprintf(temp, "superguidejian%d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, 0.3f);
    CCAnimate* animate = CCAnimate::create(animation);
    return animate;
}

void SuperGuideLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case 101://减
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateGuideMinusTag);
            
            int nScore = atoi(m_labelYa->getString());
            if(nScore<=100)
            {
                break;
            }
            else
            {
                char temp[16];
                sprintf(temp, "%d", nScore-100);
                m_labelYa->setString(temp);
            }
            
            break;
        }
        case 102://加
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateGuidePlusTag);
            
            int nScore = atoi(m_labelYa->getString());
            if(nScore>=1000)
            {
                break;
            }
            else
            {
                char temp[16];
                sprintf(temp, "%d", nScore+100);
                m_labelYa->setString(temp);
            }
            break;
        }
        case 103://start
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateGuideGoTag);
            
            m_nCurrentIndex = 3;
            m_node2->setVisible(false);
            m_node3->setVisible(true);
            //将主游戏中的押注分数改成现在这个
            if(GameLayer::_instance != NULL)
            {
                GameLayer::_instance->setYaZhuLabel(m_labelYa->getString());
                GameLayer::_instance->startGameRoll();
            }
            break;
        }
        case 104://stop
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateGuideStopTag);
            
            m_nCurrentIndex = 4;
            m_node3->setVisible(false);
            this -> setVisible(false);
            if(GameLayer::_instance != NULL)
            {
                GameLayer::_instance->TimerToStopRoll(0);
            }
            break;
        }
        case 105:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateGuideNanTag);
            
            m_nCurrentIndex = 5;
            m_node4 -> setVisible(false);
            this -> setVisible(false);
            if(GameLayer::_instance != NULL)
            {
                GameLayer::_instance->dealGuessResultGuide(0);
            }
            break;
        }
        case 106:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateGuideNvTag);
            
            m_nCurrentIndex = 5;
            m_node4 -> setVisible(false);
            this -> setVisible(false);
            if(GameLayer::_instance != NULL)
            {
                GameLayer::_instance->dealGuessResultGuide(1);
            }
            break;
        }
        case 107:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateGuideGetPrizeTag);
            
            m_nCurrentIndex = 6;
            m_node5 -> setVisible(false);
            this -> setVisible(false);
            if(GameLayer::_instance != NULL)
            {
                GameLayer::_instance->playGetPrizeAnimate();
            }
            break;
        }
        default:
            break;
    }
}

void SuperGuideLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool SuperGuideLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if(m_nCurrentIndex == 1)
    {
        m_nCurrentIndex = 2;
        m_node1->setVisible(false);
        m_node2->setVisible(true);
        if(GameLayer::_instance != NULL)
        {
            GameLayer::_instance->setStatusYaZhuLabel(false);
        }
    }
    else if(m_nCurrentIndex == 6)
    {
        this -> removeFromParent();
        if(GameLayer::_instance != NULL)
        {
            GameLayer::_instance->setEnterGuided(false);
        }
        
    }
    return true;
}

int SuperGuideLayer::getYaZhu()
{
    return atoi(m_labelYa->getString());
}

int SuperGuideLayer::getCurrentIndex()
{
    return m_nCurrentIndex;
}