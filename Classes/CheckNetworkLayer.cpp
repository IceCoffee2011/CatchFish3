//
//  CheckNetworkLayer.cpp
//  CatchFish3
//
//  Created by xiaosong1531 on 15/12/28.
//
//

#include "header.h"


bool CheckNetworkLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    if(GameMainScene::_instance != NULL)
    {
        m_nLastTime = 60;
    }
    else
    {
        m_nLastTime = 30;
    }
    
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    loadUI();
    
    return true;
}

void CheckNetworkLayer::loadUI()
{
    float width = 640;
    float height = 300;
    pSpDi = CCScale9Sprite::create("public2wnd/bgdi.png");
    pSpDi -> setContentSize(CCSizeMake(width, height));
    addChild(pSpDi);
    CCSprite* pSpLeft1 = CCSprite::create("public2wnd/bgleft1.png");
    pSpLeft1 -> setPosition(ccp(0, pSpDi->getContentSize().height));
    pSpDi -> addChild(pSpLeft1, 1);
    CCSprite* pSpLeft2 = CCSprite::create("public2wnd/bgleft1.png");
    pSpLeft2 -> setPosition(ccp(0, 0));
    pSpLeft2 -> setFlipY(true);
    pSpDi -> addChild(pSpLeft2, 1);
    CCSprite* pSpRight1 = CCSprite::create("public2wnd/bgleft1.png");
    pSpRight1 -> setPosition(ccp(pSpDi->getContentSize().width, pSpDi->getContentSize().height));
    pSpDi -> addChild(pSpRight1, 1);
    pSpRight1 -> setFlipX(true);
    CCSprite* pSpRight2 = CCSprite::create("public2wnd/bgleft1.png");
    pSpRight2 -> setPosition(ccp(pSpDi->getContentSize().width, 0));
    pSpDi -> addChild(pSpRight2, 1);
    pSpRight2 -> setFlipX(true);
    pSpRight2 -> setFlipY(true);
    
    CCSprite* pSpMiddleLeft = CCSprite::create("public2wnd/bgmiddleleft.png");
    pSpMiddleLeft -> setPosition(ccp(83, pSpDi->getContentSize().height/2));
    pSpMiddleLeft -> setScaleY(((float)height-pSpLeft2->getContentSize().height)/pSpMiddleLeft->getContentSize().height);
    pSpDi -> addChild(pSpMiddleLeft);
    CCSprite* pSpMiddleRight = CCSprite::create("public2wnd/bgmiddleright.png");
    pSpMiddleRight -> setPosition(ccp(pSpDi->getContentSize().width-106, pSpDi->getContentSize().height/2));
    pSpMiddleRight -> setScaleY((height-pSpLeft2->getContentSize().height)/pSpMiddleRight->getContentSize().height);
    pSpDi -> addChild(pSpMiddleRight);
    CCSprite* pSpMiddleTop = CCSprite::create("public2wnd/bgmiddletop.png");
    pSpMiddleTop -> setPosition(ccp(pSpDi->getContentSize().width/2, pSpDi->getContentSize().height-26));
    pSpMiddleTop -> setScaleX((width-pSpRight2->getContentSize().width)/pSpMiddleTop->getContentSize().width);
    pSpDi -> addChild(pSpMiddleTop);
    CCSprite* pSpMiddleBottom = CCSprite::create("public2wnd/bgmiddlebottom.png");
    pSpMiddleBottom -> setPosition(ccp(pSpDi->getContentSize().width/2, 26));
    pSpMiddleBottom -> setScaleX((width-pSpRight2->getContentSize().width)/pSpMiddleBottom->getContentSize().width);
    pSpDi -> addChild(pSpMiddleBottom);
    pSpDi -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    CCLabelTTF* pLabel = CCLabelTTF::create("网络已经断开，正在重新连接", "Arial", 32);
    pLabel -> setPosition(ccp(pSpDi->getContentSize().width/2, pSpDi->getContentSize().height/3*2));
    pSpDi -> addChild(pLabel, 5);
    pLabel->setColor(ccc3(188, 1.3, 21));
    m_labelTime = CCLabelTTF::create("", "Arial", 32);
    m_labelTime -> setPosition(ccp(pSpDi->getContentSize().width/2,pSpDi->getContentSize().height/2-20));
    pSpDi->addChild(m_labelTime, 5);
    m_labelTime->setColor(ccc3(188, 1.3, 21));
    
    CCMenuItemImage* pBackItem = CCMenuItemImage::create("messagebox/backlogon.png", "messagebox/backlogon.png", this, menu_selector(CheckNetworkLayer::OnButtonBack));
    pBackItem -> setPosition(ccp(pSpDi->getContentSize().width/2, pSpDi->getContentSize().height/5.8-20));
    m_menuBackLogon = CCMenu::create(pBackItem, NULL);
    m_menuBackLogon -> setPosition(CCPointZero);
    m_menuBackLogon -> setTouchPriority(-210);
    pSpDi -> addChild(m_menuBackLogon, 5);
    m_menuBackLogon -> setVisible(false);
    m_spBackLogon = CCSprite::create("messagebox/backlogonsp.png");
    m_spBackLogon -> setPosition(ccp(pSpDi->getContentSize().width/2, pSpDi->getContentSize().height/5.8-20));
    pSpDi -> addChild(m_spBackLogon, 5);
}

void CheckNetworkLayer::OnButtonBack(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CMainLogic::sharedMainLogic()->MessageResult(eDisConnectNet,eMBRequestNull,true);
}

void CheckNetworkLayer::showTime(float dt)
{
    m_nLastTime--;
    char temp[16];
    sprintf(temp, "%d", m_nLastTime);
    m_labelTime -> setString(temp);
    int nLastTime = 23;
    if(GameMainScene::_instance != NULL)
    {
        nLastTime = 30;
    }
    if(m_nLastTime < nLastTime)
    {
        m_spBackLogon -> setVisible(false);
        m_menuBackLogon -> setVisible(true);
    }
    if(m_nLastTime <= 0)
    {
        this -> unschedule(schedule_selector(CheckNetworkLayer::showTime));
        this -> removeFromParent();
        CMainLogic::sharedMainLogic()->m_pCheckNetworkLayer = NULL;
        CMainLogic::sharedMainLogic()->MessageResult(eDisConnectNet,eMBRequestNull,true);
    }
}

void CheckNetworkLayer::openTouchAndTime()
{
    if(GameMainScene::_instance != NULL)
    {
        m_nLastTime = 60;
    }
    else
    {
        m_nLastTime = 30;
    }
    
    this -> setTouchEnabled(true);
    this -> schedule(schedule_selector(CheckNetworkLayer::showTime), 1.0f);
}

void CheckNetworkLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -207, true);
}

bool CheckNetworkLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}
