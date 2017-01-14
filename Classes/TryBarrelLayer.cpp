#include "header.h"

const char* ZengSkillName[3] = {"CatchFish2001/speedup1.png", "CatchFish2001/sanshe1.png", "CatchFish2001/suodingsp2.png"};

TryBarrelLayer* TryBarrelLayer::create(int nOpenType)
{
    TryBarrelLayer* pRet = new TryBarrelLayer();
    if(pRet && pRet->init(nOpenType))
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

bool TryBarrelLayer::init(int nOpenType)
{
    if(!CCLayerColor::init())
    {
        return false;
    }
    
    m_nOpenType = nOpenType;
    
    loadUI();
    
    
    
    return true;
}

void TryBarrelLayer::loadUI()
{
    m_visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), 1280, 720);
    this->addChild(pLayerColor);
    
    float width = 580;
    float height = 390;
    pSpDi = CCScale9Sprite::create("public2wnd/bgdi.png");
    pSpDi -> setContentSize(CCSizeMake(width, height));
    addChild(pSpDi);
    pSpDi -> setPosition(ccp(m_visibleSize.width/2, m_visibleSize.height/2));
    
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
    CCSize sizeBG = pSpDi->getContentSize();
    int nSize = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    nSize = 36;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    nSize = 30;
#endif
    
    CCSize size(335,270);
    m_cclMessage = CCLabelTTF::create("", "Arial", nSize,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
    m_cclMessage->setColor(ccc3(188, 1.3, 21));
    m_cclMessage->setPosition(ccp(sizeBG.width/2, 0));
    pSpDi->addChild(m_cclMessage, 5);
    
    CCMenuItemImage* menuOK1 = CCMenuItemImage::create("messagebox/ok1.png", "messagebox/ok1.png", this, menu_selector(TryBarrelLayer::clickMenu));
    menuOK1->setPosition(ccp(pSpDi->getContentSize().width/2, -33));
    if(m_nOpenType == 1)
    {
        menuOK1 -> setTag(101);
    }
    else if(m_nOpenType == 2)
    {
        menuOK1 -> setTag(102);
    }
    else if(m_nOpenType == 3)
    {
        menuOK1 -> setTag(103);
    }
    CCMenu* m_pMenuOK = CCMenu::create(menuOK1,NULL);
    m_pMenuOK->setPosition(ccp(0,0));
    m_pMenuOK->setTouchPriority(-130);
    pSpDi->addChild(m_pMenuOK, 5);
    
    if(m_nOpenType == 1 || m_nOpenType == 2 || m_nOpenType == 3)//加速、散射、锁定
    {
        menuOK1->setVisible(false);
        
        //试用炮卡片按钮
        CCMenuItemImage* cannonItem = CCMenuItemImage::create("barrelUI/shiyongbg.png", "barrelUI/shiyongbg.png");
        cannonItem -> setPosition(ccp(pSpDi->getContentSize().width/2, pSpDi->getContentSize().height/2+100));
        CCSprite* pBarrel = CCSprite::create(ZengSkillName[m_nOpenType-1]);
        pBarrel -> setPosition(ccp(cannonItem->getContentSize().width/2-4-23, cannonItem->getContentSize().height/2));
        cannonItem -> addChild(pBarrel);
        CCLabelTTF* pLabel = CCLabelTTF::create("x1", "Arial", 36);
        pLabel -> setPosition(ccp(cannonItem->getContentSize().width/2+30, cannonItem->getContentSize().height/2));
        cannonItem -> addChild(pLabel);
        pLabel -> setColor(ccc3(188, 1.3, 21));
        pSpDi->setOpacity(0);
        pLayerColor->setOpacity(0);
        pSpLeft1 -> setOpacity(0);
        pSpLeft2 -> setOpacity(0);
        pSpMiddleBottom->setOpacity(0);
        pSpMiddleLeft->setOpacity(0);
        pSpMiddleRight->setOpacity(0);
        pSpMiddleTop->setOpacity(0);
        pSpRight1->setOpacity(0);
        pSpRight2->setOpacity(0);
        this -> setOpacity(false);
        
        CCMenu* pMenuTryCannon = CCMenu::create(cannonItem, NULL);
        pMenuTryCannon -> setPosition(CCPointZero);
        pMenuTryCannon -> setAnchorPoint(CCPointZero);
        pSpDi -> addChild(pMenuTryCannon, 1);
        CCScaleTo* scale1 = CCScaleTo::create(0.5f, 0.98f, 1.02f);
        CCScaleTo* scale2 = CCScaleTo::create(0.5f, 1.02f, 0.98f);
        CCSequence* seq1 = CCSequence::create(scale1, scale2, NULL);
        cannonItem -> runAction(CCRepeatForever::create(seq1));
        
        this -> scheduleOnce(schedule_selector(TryBarrelLayer::removeWnd), 2.0f);
    }
    
    
}

void TryBarrelLayer::removeWnd()
{
    switch (m_nOpenType) {
        case 1:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameZengSpeedup);
            GameMainScene::_instance->showZengSkillAnimate(0);
            break;
        }
        case 3:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameZengBaoji);
            GameMainScene::_instance->showZengSkillAnimate(2);
            break;
        }
        default:
            break;
    }
    this->removeFromParent();
}

void TryBarrelLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    if(item->getTag() != 100)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    }
    if(item->getTag() == 100)//closemenu
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
        GameMainScene::_instance->setAutoFire();
    }
    else if(item->getTag() == 101)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameZengSpeedup);
        GameMainScene::_instance->showZengSkillAnimate(0);
    }
    else if(item->getTag() == 103)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameZengBaoji);
        GameMainScene::_instance->showZengSkillAnimate(2);
    }
    this -> removeFromParent();
}

void TryBarrelLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool TryBarrelLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}