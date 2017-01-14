#include "header.h"

bool SuperHelpLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    m_nCurrentIndex = 1;
    int nStarExtra[4] = {1,2,3,5};
    memcpy(m_nStartExtra, nStarExtra, sizeof(m_nStartExtra));
    if (GameLayer::_instance != NULL)
    {
        m_nStartExtra[0] = GameLayer::_instance->m_nFiveStarExtra;
        m_nStartExtra[1] = GameLayer::_instance->m_nSixStarExtra;
        m_nStartExtra[2] = GameLayer::_instance->m_nSevenStarExtra;
        m_nStartExtra[3] = GameLayer::_instance->m_nEightStarExtra;
    }
    
    loadUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void SuperHelpLayer::loadUI()
{
    m_spBg = CCSprite::createWithSpriteFrameName("SuperPirateHelp1.png");
    m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spBg);
    m_spBg -> setOpacity(0);
    CCSprite* pClose1 = CCSprite::createWithSpriteFrameName("rankClose1.png");
    CCSprite* pClose2 = CCSprite::createWithSpriteFrameName("rankClose2.png");
    CCMenuItemSprite* closeItem = CCMenuItemSprite::create(pClose1, pClose2, this, menu_selector(SuperHelpLayer::clickClose));
    closeItem -> setPosition(ccp(m_spBg->getContentSize().width-40, m_spBg->getContentSize().height-40));
    CCMenu* pCloseMenu = CCMenu::create(closeItem, NULL);
    pCloseMenu -> setPosition(CCPointZero);
    pCloseMenu -> setAnchorPoint(CCPointZero);
    m_spBg -> addChild(pCloseMenu, 2);
    closeItem -> setScale(1.2f);
    pCloseMenu -> setTouchPriority(-200);
    
    m_spHelp1 = CCSprite::createWithSpriteFrameName("SuperPirateHelp1.png");
    m_spHelp1 -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2));
    m_spBg -> addChild(m_spHelp1);
    m_spHelp2 = CCSprite::createWithSpriteFrameName("SuperPirateHelp2.png");
    m_spHelp2 -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2));
    m_spBg -> addChild(m_spHelp2);
    char sStarExtar[8];
    
    for (int i=0; i<4; i++)
    {
        sprintf(sStarExtar, "%d", m_nStartExtra[i]);
        CCLabelAtlas* pStarExtraTTF = CCLabelAtlas::create(sStarExtar, "SuperPirate/helpExtraNum.png", 16, 22, '0');
        pStarExtraTTF->setAnchorPoint(ccp(0.5, 0.5));
        float xPos = 0;
        float yPos = 0;
        if (i==0)
        {
            xPos = 250;
            yPos = 220;
        }
        else if (i==1)
        {
            xPos = 543;
            yPos = 220;
        }
        else if (i==2)
        {
            xPos = 250;
            yPos = 154;
        }
        else if (i==3)
        {
            xPos = 543;
            yPos = 154;
        }
        pStarExtraTTF->setPosition(ccp(xPos, yPos));
        m_spHelp2 -> addChild(pStarExtraTTF);
    }
    m_spHelp3 = CCSprite::createWithSpriteFrameName("SuperPirateHelp3.png");
    m_spHelp3 -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2));
    m_spBg -> addChild(m_spHelp3);
    m_spHelp4 = CCSprite::createWithSpriteFrameName("SuperPirateHelp4.png");
    m_spHelp4 -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2));
    m_spBg -> addChild(m_spHelp4);
    m_spHelp5 = CCSprite::createWithSpriteFrameName("SuperPirateHelp5.png");
    m_spHelp5 -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2));
    m_spBg -> addChild(m_spHelp5);
    m_spHelp2 -> setVisible(false);
    m_spHelp3 -> setVisible(false);
    m_spHelp4 -> setVisible(false);
    m_spHelp5 -> setVisible(false);
    
    CCSprite* pBefore1 = CCSprite::createWithSpriteFrameName("SuperPirateHelpBefore1.png");
    CCSprite* pBefore2 = CCSprite::createWithSpriteFrameName("SuperPirateHelpBefore2.png");
    CCMenuItemSprite* beforeItem = CCMenuItemSprite::create(pBefore1, pBefore2, this, menu_selector(SuperHelpLayer::clickMenu));
    beforeItem -> setPosition(ccp(m_spBg->getContentSize().width/3, 45));
    beforeItem -> setTag(101);
    CCSprite* pNext1 = CCSprite::createWithSpriteFrameName("SuperPirateHelpNext1.png");
    CCSprite* pNext2 = CCSprite::createWithSpriteFrameName("SuperPirateHelpNext2.png");
    CCMenuItemSprite* nextItem = CCMenuItemSprite::create(pNext1, pNext2, this, menu_selector(SuperHelpLayer::clickMenu));
    nextItem -> setPosition(ccp(m_spBg->getContentSize().width/3*2, 45));
    nextItem -> setTag(102);
    CCMenu* pMenu = CCMenu::create(beforeItem, nextItem, NULL);
    pMenu -> setPosition(CCPointZero);
    pMenu -> setAnchorPoint(CCPointZero);
    m_spBg -> addChild(pMenu, 2);
    pMenu -> setTouchPriority(-200);
    
    CCSprite* pHelpXie = CCSprite::createWithSpriteFrameName("SuperPirateHelpXie.png");
    pHelpXie -> setPosition(ccp(m_spBg->getContentSize().width/2, 45));
    m_spBg -> addChild(pHelpXie, 2);
    m_labelBefore = CCLabelAtlas::create("1", "SuperPirate/SuperPirateHelpNum.png", 22, 18, '1');
    m_labelBefore -> setPosition(ccp(m_spBg->getContentSize().width/2-20, 45));
    m_labelBefore -> setAnchorPoint(ccp(0.5f, 0.5f));
    m_spBg -> addChild(m_labelBefore, 2);
    m_labelNext = CCLabelAtlas::create("5", "SuperPirate/SuperPirateHelpNum.png", 22, 18, '1');
    m_labelNext -> setPosition(ccp(m_spBg->getContentSize().width/2+20, 45));
    m_labelNext -> setAnchorPoint(ccp(0.5f, 0.5f));
    m_spBg -> addChild(m_labelNext, 2);
    
    //requestHttpForHelp();
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void SuperHelpLayer::clickMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    if(item != NULL)
    {
        if(item->getTag() == 101)//上一页
        {
            if(m_nCurrentIndex == 2)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateHelpToTwoTag);
                
                m_nCurrentIndex = 1;
                m_spHelp1->setVisible(true);
                m_spHelp2->setVisible(false);
                m_labelBefore->setString("1");
                m_labelNext->setString("5");
            }
            else if(m_nCurrentIndex == 3)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateHelpToThreeTag);
                
                m_nCurrentIndex = 2;
                m_spHelp2->setVisible(true);
                m_spHelp3->setVisible(false);
                m_labelBefore->setString("2");
                m_labelNext->setString("5");
            }
            else if(m_nCurrentIndex == 4)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateHelpToFourTag);
                
                m_nCurrentIndex = 3;
                m_spHelp3->setVisible(true);
                m_spHelp4->setVisible(false);
                m_labelBefore->setString("3");
                m_labelNext->setString("5");
            }
            else if(m_nCurrentIndex == 5)
            {
                m_nCurrentIndex = 4;
                m_spHelp4->setVisible(true);
                m_spHelp5->setVisible(false);
                m_labelBefore->setString("4");
                m_labelNext->setString("5");
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateHelpToFiveTag);
            }
        }
        else if(item->getTag() == 102)//下一页
        {
            if(m_nCurrentIndex == 4)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateHelpBackFourTag);
                
                m_nCurrentIndex = 5;
                m_spHelp5->setVisible(true);
                m_spHelp4->setVisible(false);
                m_labelBefore->setString("5");
                m_labelNext->setString("5");
            }
            else if(m_nCurrentIndex == 3)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateHelpBackThreeTag);
                
                m_nCurrentIndex = 4;
                m_spHelp4->setVisible(true);
                m_spHelp3->setVisible(false);
                m_labelBefore->setString("4");
                m_labelNext->setString("5");
            }
            else if(m_nCurrentIndex == 2)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateHelpBackTwoTag);
                
                m_nCurrentIndex = 3;
                m_spHelp3->setVisible(true);
                m_spHelp2->setVisible(false);
                m_labelBefore->setString("3");
                m_labelNext->setString("5");
            }
            else if(m_nCurrentIndex == 1)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateHelpBackOneTag);
                
                m_nCurrentIndex = 2;
                m_spHelp2->setVisible(true);
                m_spHelp1->setVisible(false);
                m_labelBefore->setString("2");
                m_labelNext->setString("5");
            }
        }
    }
}

void SuperHelpLayer::clickClose(CCObject* object)
{
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSuperPirateHelpCloseTag);
    
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(SuperHelpLayer::removeWnd));
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
}

void SuperHelpLayer::removeWnd()
{
    this -> removeFromParent();
}


void SuperHelpLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool SuperHelpLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}