#include "header.h"

CReminderLayer* CReminderLayer::create(int nType)
{
    CReminderLayer* pRet = new CReminderLayer;
    if(pRet && pRet->init(nType))
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

bool CReminderLayer::init(int nType)
{
    if (!CCLayer::init())
    {
        return false;
    }
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    loadRemindUI(nType);
    this->setTouchEnabled(true);
    
    return true;
}

void CReminderLayer::loadRemindUI(int nType)
{
    CCSprite* pSpBg = CCSprite::create("JQ/tishi06.png");
    pSpBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(pSpBg, 1);
    pSpBg->setScale(0);
    pSpBg->setOpacity(0);
    pSpBg->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(0.1f),CCScaleTo::create(0.1, 1.2f),NULL),CCScaleTo::create(0.1f, 1.0f),NULL));
    
    CCSprite* pTitleBg =  CCSprite::create("JQ/tishi02.png");
    pTitleBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+220));
    this->addChild(pTitleBg, 1);
    pTitleBg->setOpacity(0);
    pTitleBg->setScale(0);
    pTitleBg->runAction(CCSequence::create(CCSpawn::create(CCScaleTo::create(0.1f, 1.2f),CCFadeIn::create(0.1f),NULL),CCScaleTo::create(0.1f, 1.0f), NULL));
    
    CCSprite* pTitleStar =  CCSprite::create("JQ/tishi04.png");
    pTitleStar->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+250));
    this->addChild(pTitleStar, 1);
    pTitleStar->setOpacity(0);
    pTitleStar->runAction(CCSequence::create(CCDelayTime::create(0.05f),CCFadeIn::create(0.1f),NULL));
    
    CCSprite* pTitleFont =  CCSprite::create("JQ/tishi03.png");
    //pTitleFont->setPosition(ccp(CLIENT_WIDTH/2-2, CLIENT_HEIGHT/2+223));
    pTitleFont->setPosition(ccp(CLIENT_WIDTH+pTitleFont->getContentSize().width+50, CLIENT_HEIGHT/2+220));
    this->addChild(pTitleFont, 1);
    pTitleFont->runAction(CCSequence::create(CCMoveTo::create(0.1f, ccp(CLIENT_WIDTH/2-30, CLIENT_HEIGHT/2+223)),CCMoveTo::create(0.03f, ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+223)),NULL));
    
    for (int i=0; i<3; i++)
    {
        CCSprite* pStar = CCSprite::create("JQ/tishi05.png");
        if (i == 0) {
            pStar->setPosition(ccp(CLIENT_WIDTH/2-105, CLIENT_HEIGHT/2+215));
        }
        else if (i == 1)
        {
            pStar->setPosition(ccp(CLIENT_WIDTH/2-3, CLIENT_HEIGHT/2+185));
        }
        else if (i == 2)
        {
            pStar->setPosition(ccp(CLIENT_WIDTH/2+196, CLIENT_HEIGHT/2+172));
        }
        this->addChild(pStar, 1);
        pStar->setOpacity(0);
        pStar->runAction(CCSequence::create(CCDelayTime::create(0.05f),CCFadeIn::create(0.1f),NULL));
    }
    
    
    CCMenuItemImage* confirmItem = CCMenuItemImage::create("JQ/tishi01.png", "JQ/tishi01.png", this, menu_selector(CReminderLayer::clickMenu));
    confirmItem->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-150));
    CCMenu* confirmMenu = CCMenu::create(confirmItem, NULL);
    confirmMenu->setPosition(ccp(0, 0));
    confirmMenu->setTouchPriority(-202);
    this->addChild(confirmMenu, 1);
    confirmItem->setScale(0.9);
    confirmItem->setVisible(false);
    confirmItem->runAction(CCSequence::create(CCDelayTime::create(0.15f),CCShow::create(),CCScaleTo::create(0.05, 0.7f),CCScaleTo::create(0.05, 1.0f), NULL));
    
    
    if (nType == 0)
    {
        CCSprite* pContent = CCSprite::create("JQ/tishi07.png");
        pContent->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+20));
        this->addChild(pContent, 1);
        pContent->setOpacity(0);
        pContent->runAction(CCSequence::create(CCDelayTime::create(0.01),CCFadeTo::create(0.25, 255),NULL));
    }
    else if(nType == 1)
    {
        CCSprite* pContent = CCSprite::create("JQ/tishi08.png");
        pContent->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+20));
        this->addChild(pContent, 1);
        pContent->setOpacity(0);
        pContent->runAction(CCSequence::create(CCDelayTime::create(0.01),CCFadeTo::create(0.25, 255),NULL));
    }
    else
    {
        
    }
    

}

void CReminderLayer::clickMenu(CCObject* obj)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    this->removeFromParentAndCleanup(true);
    if(GameMainScene::_instance != NULL)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eXinShouRoomExitOK);
        GameMainScene::_instance->exitGame();
    }
}

void CReminderLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -201, true);
}

bool CReminderLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}