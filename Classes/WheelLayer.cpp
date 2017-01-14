#include "header.h"

const char* WheelPrizeName[4] = {"share/wheelbei.png", "share/wheelcoin.png", "share/wheelwufu.png", "share/wheelhuafei.png"};
const CCPoint WheelPrizePoint[10] = {ccp(0, 0), ccp(0, 0), ccp(0, 0), ccp(0, 0), ccp(0, 0), ccp(0, 0), ccp(0, 0), ccp(0, 0), ccp(0, 0), ccp(0, 0)};
const char* MusicEffect[8] = {"Music/lw_01.mp3", "Music/lw_02.mp3", "Music/lw_03.mp3", "Music/lw_04.mp3", "Music/lw_05.mp3", "Music/lw_06.mp3", "Music/lw_07.mp3", "Music/lw_08.mp3"};
const char* PrizeInfoName[4] = {"奖杯在周赛榜可赢取实物大奖", "金币在游戏中捕鱼时使用", "收集福卡兑换京东卡", "话费券用于兑换话费充值卡"};
WheelLayer::WheelLayer()
{
    
}

WheelLayer::~WheelLayer()
{
    
}

bool WheelLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_cbPrizeIndex = 0;
    m_nMatchCup = 0;
    m_nPlayEffectTimes = 0;
    m_nPlayIndex = 1;
    m_fWheelRotation = 0;
    m_bReceiveResult = false;
    m_bClickWheelMenu = false;
    m_cbPrizeIndex = 0;
    
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    
    CMainLogic::sharedMainLogic()->m_bReceiveShowWheel = false;
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    loadUI();
    loadPrizeItem();
    
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWheelOpenTag);
    
    CMD_GPR_ReceiveWheel ReceiveWheel = {0};
    ReceiveWheel.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_RECEIVE_WHEEL, &ReceiveWheel, sizeof(ReceiveWheel));
    
    this -> setTouchEnabled(true);
    
    return true;
}

void WheelLayer::loadUI()
{
    m_spWheelBg = CCSprite::create("share/wheelbg.png");
    m_spWheelBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spWheelBg);
    CCSprite* pLabelDi = CCSprite::create("share/wheelinfodi.png");
    pLabelDi -> setPosition(ccp(m_spWheelBg->getContentSize().width/2, 100));
    m_spWheelBg -> addChild(pLabelDi);
    CCLabelTTF* pLabelInfo = CCLabelTTF::create("每日登录即可抽奖", "Arial-BoldMT", 32);
    pLabelInfo -> setPosition(ccp(pLabelDi->getContentSize().width/2, pLabelDi->getContentSize().height/2));
    pLabelDi -> addChild(pLabelInfo);
    CCSprite* pWheelTitle = CCSprite::create("share/wheeltitle.png");
    pWheelTitle -> setPosition(ccp(m_spWheelBg->getContentSize().width/2, 600));
    m_spWheelBg -> addChild(pWheelTitle, 2);
    CCSprite* pWheelDeng1 = CCSprite::create("share/wheeldeng2.png");
    pWheelDeng1 -> setPosition(ccp(m_spWheelBg->getContentSize().width/2, m_spWheelBg->getContentSize().height/2+18));
    m_spWheelBg -> addChild(pWheelDeng1);
    float fDelay = 0.5f;
    pWheelDeng1 -> runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(fDelay, 0), CCDelayTime::create(1.0f), CCFadeTo::create(fDelay, 255), NULL)));
    CCSprite* pWheelDeng2 = CCSprite::create("share/wheeldeng2.png");
    pWheelDeng2 -> setPosition(ccp(m_spWheelBg->getContentSize().width/2, m_spWheelBg->getContentSize().height/2+18));
    m_spWheelBg -> addChild(pWheelDeng2);
    pWheelDeng2 -> setRotation(22.5);
    pWheelDeng2 -> setOpacity(0);
    pWheelDeng2 -> runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(0.5f), CCFadeTo::create(fDelay, 255), CCFadeTo::create(fDelay, 0), CCDelayTime::create(0.5f), NULL)));
    
    m_spWheelBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
    
    CCSprite* pMenuDi = CCSprite::create("share/wheelmenudi.png");
    pMenuDi -> setPosition(ccp(m_spWheelBg->getContentSize().width/2, m_spWheelBg->getContentSize().height/2+13));
    m_spWheelBg -> addChild(pMenuDi, 9);

    CCMenuItemImage* pWheelItem = CCMenuItemImage::create("share/wheelitem.png", "share/wheelitem.png", this, menu_selector(WheelLayer::clickMenu));
    pWheelItem -> setPosition(ccp(m_spWheelBg->getContentSize().width/2, m_spWheelBg->getContentSize().height/2+13));
    pWheelItem -> setTag(101);
    m_menuWheel = CCMenu::create(pWheelItem, NULL);
    m_menuWheel -> setPosition(CCPointZero);
    m_menuWheel -> setAnchorPoint(CCPointZero);
    m_menuWheel -> setTouchPriority(-130);
    m_spWheelBg -> addChild(m_menuWheel, 10);
    
}

void WheelLayer::loadPrizeItem()
{
    m_spWheelDi = CCSprite::create("share/wheeldi.png");
    m_spWheelDi -> setPosition(ccp(m_spWheelBg->getContentSize().width/2, m_spWheelBg->getContentSize().height/2+13));
    m_spWheelBg -> addChild(m_spWheelDi);
    
    for(int i = 0; i < 10; i++)
    {
        int nIndex = CMainLogic::sharedMainLogic()->m_cbWheelType[i]-1;
        if(nIndex >= 4 || nIndex < 0)
            continue;
        CCSprite* pPrizeItem = CCSprite::create(WheelPrizeName[nIndex]);
        pPrizeItem->setPosition(ccp(m_spWheelDi->getContentSize().width/2+m_spWheelDi->getContentSize().width/2/4*3*cos((72-36*i)/(180/M_PI)), m_spWheelDi->getContentSize().width/2+m_spWheelDi->getContentSize().width/2/4*3*sin((72-36*i)/(180/M_PI))));
        m_spWheelDi -> addChild(pPrizeItem);
        pPrizeItem -> setRotation(18+36*i);
        char temp[16];
        int nCount = CMainLogic::sharedMainLogic()->m_nWheelCount[i];
        if(nCount >= 10000)
        {
            sprintf(temp, "%d:", nCount/10000);
        }
        else
        {
            sprintf(temp, "%d", nCount);
        }
        CCLabelAtlas* pLabel = CCLabelAtlas::create(temp, "share/wheelnum.png", 21, 23, '0');
        pLabel -> setAnchorPoint(ccp(0.5f, 0.5f));
        pLabel->setPosition(ccp(m_spWheelDi->getContentSize().width/2+(m_spWheelDi->getContentSize().width/2/4*2+10)*cos((72-36*i)/(180/M_PI)), m_spWheelDi->getContentSize().width/2+(m_spWheelDi->getContentSize().width/2/4*2+10)*sin((72-36*i)/(180/M_PI))));
        m_spWheelDi -> addChild(pLabel);
        pLabel -> setRotation(18+36*i);
        pLabel -> setScale(0.9f);
    }
}

void WheelLayer::setWheelPrize(BYTE cbIndex, int nCup)
{
    m_cbPrizeIndex = cbIndex;//取值范围是0-9
    m_bReceiveResult = true;
    m_nMatchCup = nCup;
    if(m_bClickWheelMenu)//如果已经点击了按钮，那么在4秒后将停止运动
    {
        startWheelAction();
    }
}

float WheelLayer::getStopRotation()
{
    float fRotation = 0;
    fRotation = 360-36*m_cbPrizeIndex-18;
    
    return fRotation;
}

void WheelLayer::clickMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    if(item->getTag() == 101)//抽奖
    {
        m_bClickWheelMenu = true;
        item -> setScale(1.0f);
        item->runAction(CCSequence::create(CCScaleTo::create(0.05f, 0.9f), CCScaleTo::create(0.05f, 1.0f), NULL));
        m_menuWheel -> setTouchEnabled(false);
        //开始转动
        if(m_bReceiveResult)
        {
            //CCEaseExponentialOut  CCEaseSineIn
            float fRotation = getStopRotation();
            CCEaseSineIn* pRep1 = CCEaseSineIn::create(CCRotateTo::create(2, 360*5));
            CCEaseExponentialOut* pRep2 = CCEaseExponentialOut::create(CCRotateTo::create(4, 360*2+fRotation));
            CCSequence* pSeq1 = CCSequence::create(pRep1, pRep2, CCCallFunc::create(this, callfunc_selector(WheelLayer::stopWheelAction)), NULL);
            m_spWheelDi -> runAction(pSeq1);
            m_fWheelRotation = m_spWheelDi->getRotation();
            playEffect();
            this -> schedule(schedule_selector(WheelLayer::playEffect), 0.42f);
        }
        else
        {
            CCRepeatForever* pRepeat = CCRepeatForever::create(CCRotateBy::create(0.5f, 360));
            pRepeat -> setTag(101);
            m_spWheelDi -> runAction(pRepeat);
        }
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWheelStartTag);
    }
    else if(item->getTag() == 102)//领取按钮，相当于关闭按钮
    {
        CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(WheelLayer::removeWnd));
        CCSprite* sprite = (CCSprite*)this->getChildByTag(99);
        if(sprite != NULL)
        {
            sprite->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), NULL));
        }
        m_spWheelBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eWheelStopTag);
        
    }
}

void WheelLayer::scheduleSpeed()
{
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void WheelLayer::playEffect()
{
    float f1[8] = { 0.25f, 0.23f, 0.21f, 0.18f, 0.18f, 0.18f, 0.21f, 0.25f };
    m_nPlayEffectTimes++;
    if(m_nPlayEffectTimes == 8)
    {
        this -> unschedule(schedule_selector(WheelLayer::playEffect));
        this -> schedule(schedule_selector(WheelLayer::playEffect2), 0.01f);
        return ;
    }
    if (m_nPlayEffectTimes == 5 || m_nPlayEffectTimes == 6)
        SimpleAudioEngine::sharedEngine()->playEffect(MusicEffect[0]);
    else
        SimpleAudioEngine::sharedEngine()->playEffect(MusicEffect[1]);
    this -> scheduleOnce(schedule_selector(WheelLayer::scheduleSpeed), f1[m_nPlayEffectTimes - 1]);
}

void WheelLayer::playEffect2()
{
    int nDis = abs((int)m_fWheelRotation-(int)m_spWheelDi->getRotation());
    if(nDis > 36)
    {
        m_fWheelRotation = m_spWheelDi->getRotation();
        m_nPlayIndex++;
        if(m_nPlayIndex>=7)
        {
            m_nPlayIndex = 2;
        }
        SimpleAudioEngine::sharedEngine()->playEffect(MusicEffect[m_nPlayIndex]);
    }
}



void WheelLayer::startWheelAction()
{
    m_spWheelDi->stopActionByTag(101);
    CCRotateBy* rotateBy = CCRotateBy::create(1, 720*1);
    float fRotation = getStopRotation();
    CCEaseExponentialOut* pEaseOut = CCEaseExponentialOut::create(CCRotateTo::create(2, 360+fRotation));
    //当转盘停止后，显示奖励界面
    CCCallFunc* rewardFunc = CCCallFunc::create(this, callfunc_selector(WheelLayer::stopWheelAction));
    m_spWheelDi->runAction(CCSequence::create(rotateBy, pEaseOut, rewardFunc, NULL));
}

void WheelLayer::stopWheelAction()
{
    printf("停止转动，出现二级界面");
    m_nPlayIndex++;
    if(m_nPlayIndex>=7)
    {
        m_nPlayIndex = 2;
    }
    SimpleAudioEngine::sharedEngine()->playEffect(MusicEffect[m_nPlayIndex]);
    SimpleAudioEngine::sharedEngine()->playEffect(MusicEffect[7]);
    this -> unschedule(schedule_selector(WheelLayer::playEffect2));
    this -> scheduleOnce(schedule_selector(WheelLayer::showGuangSprite), 0.3f);
    this -> scheduleOnce(schedule_selector(WheelLayer::showPrizeLayer), 1.1f);
}

void WheelLayer::showGuangSprite()
{
    CCSprite* pEffect1 = CCSprite::create("share/wheeleffect1.png");
    pEffect1 -> setPosition(ccp(m_spWheelBg->getContentSize().width/2, m_spWheelBg->getContentSize().height/2+13));
    m_spWheelBg -> addChild(pEffect1, 11);
    CCSprite* pEffect2 = CCSprite::create("share/wheeleffect2.png");
    pEffect2 -> setPosition(ccp(m_spWheelBg->getContentSize().width/2, m_spWheelBg->getContentSize().height/2+13));
    m_spWheelBg -> addChild(pEffect2, 11);
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(WheelLayer::removeNode));
    pEffect1 -> runAction(CCSequence::create(CCFadeOut::create(0.8f), func, NULL));
    
    pEffect2 -> runAction(CCSequence::create(CCSpawn::create(CCFadeOut::create(0.8f), CCScaleTo::create(0.8f, 1.5f), NULL), func, NULL));
}

void WheelLayer::showPrizeLayer()
{
    CCSprite* sprite = CCSprite::create();
    sprite -> setPosition(ccp(CLIENT_WIDTH/2-10, CLIENT_HEIGHT/2+13));
    this -> addChild(sprite, 15, 99);
    CCLayerColor* pColor = CCLayerColor::create(ccc4(0, 0, 0, 50), CLIENT_WIDTH, CLIENT_HEIGHT);
    this -> addChild(pColor, 14);
    pColor -> runAction(CCFadeTo::create(1.5, 190));
    CCSprite* pGuang = CCSprite::create("share/wheelgetguang.png");
    pGuang -> setPosition(ccp(0, 0));
    sprite -> addChild(pGuang);
    pGuang -> runAction(CCRepeatForever::create(CCRotateBy::create(6.0, 360)));
    CCSprite* pTitle = CCSprite::create("share/wheelgettitle.png");
    pTitle -> setPosition(ccp(0, 180));
    sprite -> addChild(pTitle);
    
    CCMenuItemImage* pGetItem = CCMenuItemImage::create("share/wheelgetitem.png", "share/wheelgetitem.png", this, menu_selector(WheelLayer::clickMenu));
    pGetItem -> setTag(102);
    pGetItem -> setPosition(ccp(0, -170));
    CCMenu* pReceiveMenu = CCMenu::create(pGetItem, NULL);
    pReceiveMenu -> setPosition(CCPointZero);
    pReceiveMenu -> setAnchorPoint(CCPointZero);
    sprite -> addChild(pReceiveMenu);
    pReceiveMenu -> setTouchPriority(-130);
    
    
    
    sprite -> setScale(0.2f);
    sprite -> runAction(CCSequence::create(CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.12f, 1.0f), NULL));
    
    //物品展示
    if(m_cbPrizeIndex >= 10)
        return ;
    int nNameIndex = CMainLogic::sharedMainLogic()->m_cbWheelType[m_cbPrizeIndex]-1;
    //0奖杯，1金币，2福卡，3话费
    if(nNameIndex > 3)
    {
        return ;
    }
    CCSprite* pPrizeType = CCSprite::create(WheelPrizeName[nNameIndex]);
    pPrizeType -> setPosition(ccp(0, 0));
    sprite -> addChild(pPrizeType);
    pPrizeType -> setScale(2.0f);
    int nPrizeCount = CMainLogic::sharedMainLogic()->m_nWheelCount[m_cbPrizeIndex];
    char temp[16];
    if(nPrizeCount >= 10000)
    {
        sprintf(temp, "%d:", nPrizeCount/10000);
    }
    else
    {
        sprintf(temp, "%d", nPrizeCount);
    }
    CCLabelAtlas* pLabel = CCLabelAtlas::create(temp, "share/wheelnum.png", 21, 23, '0');
    pLabel -> setAnchorPoint(ccp(0.5f, 0.5f));
    pLabel -> setPosition(ccp(pPrizeType->getContentSize().width/2, 0));
    pPrizeType -> addChild(pLabel);
    CCSprite* pLabelDi = CCSprite::create("share/wheelinfodi.png");
    pLabelDi -> setPosition(ccp(0, -270));
    sprite -> addChild(pLabelDi);
    CCLabelTTF* pLabelInfo = CCLabelTTF::create(PrizeInfoName[nNameIndex], "Arial-BoldMT", 32);
    pLabelInfo -> setPosition(ccp(pLabelDi->getContentSize().width/2, pLabelDi->getContentSize().height/2));
    pLabelDi -> addChild(pLabelInfo);
}

void WheelLayer::removeNode(CCNode* pNode)
{
    pNode->removeFromParent();
}

void WheelLayer::removeWnd()
{
    this -> removeFromParent();
    if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
    {
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/4.mp3", true);
    }
    //修改大厅内的金币和奖杯数量
    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            pHallLayer->setUserScore(CMainLogic::sharedMainLogic()->m_lUserScore);
            pHallLayer->setWeekNum(m_nMatchCup);
        }
    }
    bool bShowFirstCharge = true;
    bool bShowFirstCharge2 = true;
    if(bShowFirstCharge && CMainLogic::sharedMainLogic()->m_bShowOneCharge && CMainLogic::sharedMainLogic()->m_bCanOneCharge && CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        bShowFirstCharge2 = false;
        //1元充值礼包
        if(CCUserDefault::sharedUserDefault()->getStringForKey("OneFirstCharge", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("OneCharge"))
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("OneFirstCharge", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("OneCharge"));
            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    bShowFirstCharge = false;
                    OneMoneyLayer* pOneMoneyLayer = OneMoneyLayer::create();
                    CCDirector::sharedDirector()->getRunningScene()->addChild(pOneMoneyLayer, 12, 20004);
                }
            }
        }
    }
    if(bShowFirstCharge && bShowFirstCharge2 && CMainLogic::sharedMainLogic()->m_cbCountTimes == 0 && CMainLogic::sharedMainLogic()->m_bIOSPassed)//10元充值列表
    {
        if(CCUserDefault::sharedUserDefault()->getStringForKey("FirstCharge", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("Charge"))
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("FirstCharge", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("Charge"));
            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    bShowFirstCharge = false;
                    FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create();
                    pHallLayer->addChild(pFirstChargeLayer, 200);
                }
            }
        }
    }
    //每天弹一次广告框
    if(bShowFirstCharge && CMainLogic::sharedMainLogic()->m_bIOSPassed && CMainLogic::sharedMainLogic()->m_bShowAdvert)
    {
        if(CCUserDefault::sharedUserDefault()->getStringForKey("ShowAdvert", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("Advert"))
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("ShowAdvert", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("Advert"));
            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    bShowFirstCharge = false;
                    AdvertLayer* pAdvertLayer = AdvertLayer::create();
                    pHallLayer->addChild(pAdvertLayer, 200);
                }
            }
        }
    }
    if(bShowFirstCharge && CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                if(CCUserDefault::sharedUserDefault()->getBoolForKey("NoticeDataSuccess", false))
                {
                    pHallLayer -> scheduleOnce(schedule_selector(CHallLayer::showNoticeLayer), 0.0f);
                }
            }
        }
    }
}

void WheelLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool WheelLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

