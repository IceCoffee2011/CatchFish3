#include "header.h"

extern const char* DogChampion[15];

enum eCircleUsedStatus
{
    eCircleUnUsed = 101,//未碰撞
    eCircleUsed = 102,//已发生碰撞
};

/*490, 410, 330, 250, 170, 90*/

DogRunLayer::DogRunLayer()
{
    
}

DogRunLayer::~DogRunLayer()
{
    m_arrDog -> release();
    m_arrJiasu -> release();
    m_arrJiansu -> release();
}

bool DogRunLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_arrDog = CCArray::create();
    m_arrDog -> retain();
    m_arrJiansu = CCArray::create();
    m_arrJiansu -> retain();
    m_arrJiasu = CCArray::create();
    m_arrJiasu -> retain();
    
    m_bLayerRun = false;
    m_nChampion1 = 1;
    m_nChampion2 = 2;
    m_nBgSpeed = 0;
    m_nBeilv = 0;
    m_llGetCoin = 0;//玩家输赢
    m_llReturnBet = 0;//返回玩家下注
    m_llCaiChi = 0;//彩池奖励
    for(int i = 0; i < 6; i++)
    {
        m_nDogSpeed[i] = 12;
    }
    for(int i = 0; i < 5; i++)
    {
        m_llScoreRank[i] = 0;
        m_llCaiChiRank[i] = 0;
        m_sNameRank[i] = "";
    }
    
    
    loadBG();
    loadDogDaijiAnimate();
    loadBeteUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void DogRunLayer::loadBG()
{
    m_spBg1 = CCSprite::create("dog/dog_matchBg1.png");
    m_spBg1 -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spBg1, 2);
    m_spBg1 -> setOpacity(0);
    CCScrollView * pBigPrizeScroll = CCScrollView::create();
    m_layerContainer = CCLayer::create();
    m_layerContainer -> setContentSize(CCSizeMake(1136*5, 640));
    pBigPrizeScroll->setContainer(m_layerContainer);
    pBigPrizeScroll->setTouchEnabled(true);
    pBigPrizeScroll->setViewSize(CCSizeMake(CLIENT_WIDTH, CLIENT_HEIGHT));
    pBigPrizeScroll->setPosition(ccp(0, 0));
    pBigPrizeScroll->setContentOffset(ccp(144, 0));
    m_layerContainer->setPosition(ccp(72, 40));
    pBigPrizeScroll->setDirection(kCCScrollViewDirectionHorizontal);
    this->addChild(pBigPrizeScroll, 1);
    for(int i = 0; i < 6; i++)
    {
        CCSprite* pBg = CCSprite::create("dog/dog_matchBg1.png");
        pBg -> setPosition(ccp(pBg->getContentSize().width/2+pBg->getContentSize().width*i, pBg->getContentSize().height/2));
        m_layerContainer -> addChild(pBg);
    }
    CCSprite* pMatchXian = CCSprite::createWithSpriteFrameName("dog_match_xian.png");
    pMatchXian -> setPosition(ccp(147, 320-59));
    m_layerContainer -> addChild(pMatchXian);
    char temp[32];
    for(int i = 0; i < 6; i++)
    {
        sprintf(temp, "dog_match_%d.png", i+1);
        CCSprite* sprite = CCSprite::createWithSpriteFrameName(temp);
        sprite -> setPosition(ccp(190, m_spBg1->getContentSize().height-160-80*i));
        m_layerContainer -> addChild(sprite);
        CCSprite* sprite2 = CCSprite::createWithSpriteFrameName(temp);
        sprite2 -> setPosition(ccp(m_spBg1->getContentSize().width*4+550, m_spBg1->getContentSize().height-160-80*i+10));
        m_layerContainer -> addChild(sprite2);
        CCSprite* sprite3 = CCSprite::createWithSpriteFrameName("dog_match_bone.png");
        sprite3 -> setPosition(ccp(m_spBg1->getContentSize().width*4+550, m_spBg1->getContentSize().height-160-80*i-20));
        m_layerContainer -> addChild(sprite3);
        printf("fff is %f, y is %f\n", sprite2->getPositionX(), sprite2->getPositionY());
    }
    //结尾线
    m_spMatchEnd = CCSprite::createWithSpriteFrameName("dog_match_end.png");
    m_spMatchEnd -> setPosition(ccp(m_spBg1->getContentSize().width*4+490, 320-59));
    m_layerContainer -> addChild(m_spMatchEnd);
    
    //冠军亚军
    CCSprite* pCrown1 = CCSprite::createWithSpriteFrameName("dog_match_crown_bottom1.png");
    pCrown1 -> setPosition(ccp(210, m_spBg1->getContentSize().height-pCrown1->getContentSize().height/2));
    m_spBg1 -> addChild(pCrown1);
    CCSprite* pCrown2 = CCSprite::createWithSpriteFrameName("dog_match_crown_bottom2.png");
    pCrown2 -> setPosition(ccp(220+pCrown1->getContentSize().width, m_spBg1->getContentSize().height-pCrown2->getContentSize().height/2));
    m_spBg1 -> addChild(pCrown2);
    m_spGuanjun = CCSprite::createWithSpriteFrameName("animal1/gaojiasuo_daiji1.png");
    m_spGuanjun -> setPosition(ccp(pCrown1->getContentSize().width/2, pCrown1->getContentSize().height/2));
    pCrown1 -> addChild(m_spGuanjun);
    m_spGuanjun->setScale(0.8f);
    m_spGuanjun->setFlipX(true);
    m_spYajun = CCSprite::createWithSpriteFrameName("animal3/jinmao_daiji1.png");
    m_spYajun -> setPosition(ccp(pCrown2->getContentSize().width/2, pCrown2->getContentSize().height/2));
    pCrown2 -> addChild(m_spYajun);
    m_spYajun->setScale(0.8f);
    m_spYajun->setFlipX(true);
    
//    createRoadBlock(400, 490, 1);
//    createRoadBlock(1400, 490, 2);
//    createRoadBlock(1700, 490, 1);
//    createRoadBlock(3400, 490, 1);
//    
//    createRoadBlock(800, 410, 1);
//    createRoadBlock(1600, 410, 1);
//    createRoadBlock(2000, 410, 1);
//    createRoadBlock(3000, 410, 2);
//    
//    createRoadBlock(1000, 330, 1);
//    createRoadBlock(1000, 330, 1);
//    createRoadBlock(1000, 330, 2);
//    
//    createRoadBlock(800, 250, 1);
//    createRoadBlock(1800, 250, 2);
//    createRoadBlock(3000, 250, 2);
//    
//    createRoadBlock(3200, 170, 1);
//    createRoadBlock(1700, 170, 2);
//    createRoadBlock(600, 170, 2);
//    
//    createRoadBlock(1000, 90, 2);
//    createRoadBlock(2000, 90, 2);
//    createRoadBlock(3800, 90, 1);
    
}

void DogRunLayer::createRoadBlock(float x, float y, int nType)
{
    if(nType == 1)//加速
    {
        CCSprite* pCircle = CCSprite::createWithSpriteFrameName("dog_match_circle.png");
        pCircle -> setPosition(ccp(x, y));
        m_layerContainer -> addChild(pCircle);
        CCSprite* pRocket = CCSprite::createWithSpriteFrameName("dog_match_rocket.png");
        pRocket -> setPosition(ccp(pCircle->getContentSize().width/2, pCircle->getContentSize().height/2));
        pCircle -> addChild(pRocket, 1, 1);
        m_arrJiasu -> addObject(pCircle);
        pCircle -> setTag(eCircleUnUsed);
        pCircle -> setVisible(false);
        pCircle -> setScale(0);
    }
    else if(nType == 2)//减速
    {
        CCSprite* pCircle = CCSprite::createWithSpriteFrameName("dog_match_circle.png");
        pCircle -> setPosition(ccp(x, y));
        m_layerContainer -> addChild(pCircle);
        CCSprite* pBomb = CCSprite::createWithSpriteFrameName("dog_match_bomb.png");
        pBomb -> setPosition(ccp(pCircle->getContentSize().width/2, pCircle->getContentSize().height/2));
        pCircle -> addChild(pBomb, 1, 1);
        m_arrJiansu -> addObject(pCircle);
        pCircle -> setTag(eCircleUnUsed);
        pCircle -> setVisible(false);
        pCircle -> setScale(0);
    }
}

void DogRunLayer::loadDogDaijiAnimate()
{
    m_pGaojiasuo = CCSprite::createWithSpriteFrameName("animal1/gaojiasuo_daiji1.png");
    m_pGaojiasuo -> setPosition(ccp(m_pGaojiasuo->getContentSize().width/2, m_spBg1->getContentSize().height/4*3+20));
    m_layerContainer -> addChild(m_pGaojiasuo);
    m_pJinmao = CCSprite::createWithSpriteFrameName("animal3/jinmao_daiji1.png");
    m_pJinmao -> setPosition(ccp(m_pGaojiasuo->getPositionX(), m_spBg1->getContentSize().height/4*3-60));
    m_layerContainer -> addChild(m_pJinmao);
    m_pSamoye = CCSprite::createWithSpriteFrameName("animal4/samoye_daiji1.png");
    m_pSamoye -> setPosition(ccp(m_pGaojiasuo->getPositionX(), m_spBg1->getContentSize().height/4*3-60*2-20));
    m_layerContainer -> addChild(m_pSamoye);
    m_pShapi = CCSprite::createWithSpriteFrameName("animal6/shapi_daiji1.png");
    m_pShapi -> setPosition(ccp(m_pGaojiasuo->getPositionX(), m_spBg1->getContentSize().height/4*3-60*3-50));
    m_layerContainer -> addChild(m_pShapi);
    m_pTaidi = CCSprite::createWithSpriteFrameName("animal7/taidi_daiji1.png");
    m_pTaidi -> setPosition(ccp(m_pGaojiasuo->getPositionX(), m_spBg1->getContentSize().height/4*3-60*4-70));
    m_layerContainer -> addChild(m_pTaidi);
    m_pLachang = CCSprite::createWithSpriteFrameName("animal8/lachang_daiji1.png");
    m_pLachang -> setPosition(ccp(m_pGaojiasuo->getPositionX(), m_spBg1->getContentSize().height/4*3-60*5-90));
    m_layerContainer -> addChild(m_pLachang);
    m_pGaojiasuo -> setFlipX(180);
    m_pJinmao -> setFlipX(180);
    m_pSamoye -> setFlipX(180);
    m_pShapi -> setFlipX(180);
    m_pTaidi -> setFlipX(180);
    m_pLachang -> setFlipX(180);
    printf("%f\n", m_pLachang->getPositionY());
    
    m_arrDog -> addObject(m_pGaojiasuo);
    m_arrDog -> addObject(m_pJinmao);
    m_arrDog -> addObject(m_pSamoye);
    m_arrDog -> addObject(m_pShapi);
    m_arrDog -> addObject(m_pTaidi);
    m_arrDog -> addObject(m_pLachang);
    
    //执行狗待机动画
    m_pGaojiasuo -> runAction(CCRepeat::create(DogBeteLayer::_instance->getDogAnimate("animal1/gaojiasuo_daiji", 1, 3, 0.2), 5));
    m_pJinmao -> runAction(CCRepeat::create(DogBeteLayer::_instance->getDogAnimate("animal3/jinmao_daiji", 1, 3, 0.2), 5));
    m_pSamoye -> runAction(CCRepeat::create(DogBeteLayer::_instance->getDogAnimate("animal4/samoye_daiji", 1, 3, 0.2), 5));
    m_pShapi -> runAction(CCRepeat::create(DogBeteLayer::_instance->getDogAnimate("animal6/shapi_daiji", 1, 3, 0.2), 5));
    m_pTaidi -> runAction(CCRepeat::create(DogBeteLayer::_instance->getDogAnimate("animal7/taidi_daiji", 1, 3, 0.2), 5));
    m_pLachang -> runAction(CCRepeat::create(DogBeteLayer::_instance->getDogAnimate("animal8/lachang_daiji", 1, 3, 0.2), 5));
}

void DogRunLayer::loadBeteUI()
{
    for(int i = 0; i < 15; i++)
    {
        CCSprite* pBetBox = CCSprite::createWithSpriteFrameName("dog_match_bet_box.png");
        pBetBox -> setPosition(ccp(38+76*i, 23));
        m_spBg1 -> addChild(pBetBox);
        CCLabelBMFont* pChampion = CCLabelBMFont::create(DogChampion[i], "dog/bairen_qianliangminxuhao.fnt");
        pChampion -> setPosition(ccp(38, 28));
        pBetBox -> addChild(pChampion);
        pChampion -> setScale(0.8f);
        m_pLabelMeBet[i] = CCLabelBMFont::create("", "dog/dog_bet_fnt2.fnt");
        m_pLabelMeBet[i]->setPosition(ccp(38, 10));
        m_pLabelMeBet[i]->setScale(0.6f);
        pBetBox -> addChild(m_pLabelMeBet[i]);
    }
    
    m_pLayerColor = CCLayerColor::create(ccc4(255, 255, 255, 0), CLIENT_WIDTH, CLIENT_HEIGHT);
    this -> addChild(m_pLayerColor, 2);
}

void DogRunLayer::setMeBetMoney()
{
    char temp[16];
    for(int i = 0; i < 15; i++)
    {
        if(DogBeteLayer::_instance->m_nOwnOneBete[i] != 0)
        {
            sprintf(temp, "%d", DogBeteLayer::_instance->m_nOwnOneBete[i]);
            m_pLabelMeBet[i]->setString(temp);
        }
    }
}

void DogRunLayer::setChampion(int nChampion1, int nChampion2)
{
    m_nChampion1 = nChampion1;
    m_nChampion2 = nChampion2;
}

void DogRunLayer::onEnterTransitionDidFinish()
{
    //randy  go
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(DogRunLayer::removeNodeCall));
    CCSprite* pReady = CCSprite::createWithSpriteFrameName("dog_match_ready.png");
    pReady -> setPosition(ccp(m_spBg1->getContentSize().width/2, m_spBg1->getContentSize().height/2));
    m_spBg1 -> addChild(pReady);
    pReady -> setScale(0.5f);
    pReady -> setOpacity(100);
    pReady -> runAction(CCSequence::create(CCSpawn::create(CCFadeTo::create(0.5f, 255),CCScaleTo::create(0.6f, 1.0f), NULL), CCSpawn::create(CCScaleTo::create(0.6f, 0.5f), CCFadeTo::create(0.6f, 100), NULL), CCHide::create(), func, NULL));
    CCSprite* pGo = CCSprite::createWithSpriteFrameName("dog_match_go.png");
    pGo -> setPosition(ccp(m_spBg1->getContentSize().width/2, m_spBg1->getContentSize().height/2));
    m_spBg1 -> addChild(pGo);
    pGo -> setVisible(false);
    pGo -> setScale(0.6f);
    pGo -> setOpacity(100);
    pGo -> runAction(CCSequence::create(CCDelayTime::create(1.2f), CCShow::create(), CCSpawn::create(CCScaleTo::create(0.3f, 1.0f), CCFadeTo::create(0.3f, 255), NULL), CCSpawn::create(CCScaleTo::create(0.3f, 0.3f), CCFadeTo::create(0.3f, 100), NULL),  CCHide::create(), func, NULL));
    this -> scheduleOnce(schedule_selector(DogRunLayer::dogRunAnimate), 1.8f);
}

void DogRunLayer::removeNodeCall(CCNode* pNode)
{
    pNode -> removeFromParent();
}

void DogRunLayer::dogRunAnimate()
{
    CCAnimate* pGaojiasuoAnimate = DogBeteLayer::_instance->getDogAnimate("animal1/gaojiasuo", 1, 8, 0.1f);
    m_pGaojiasuo -> runAction(CCRepeatForever::create(pGaojiasuoAnimate));
    CCAnimate* pJinmaoAniate = DogBeteLayer::_instance->getDogAnimate("animal3/jinmao", 1, 8, 0.1f);
    m_pJinmao -> runAction(CCRepeatForever::create(pJinmaoAniate));
    CCAnimate* pSamoyeAnimate = DogBeteLayer::_instance->getDogAnimate("animal4/samoye", 1, 8, 0.1f);
    m_pSamoye -> runAction(CCRepeatForever::create(pSamoyeAnimate));
    CCAnimate* pShapiAnimate = DogBeteLayer::_instance->getDogAnimate("animal6/shapi", 1, 6, 0.1f);
    m_pShapi -> runAction(CCRepeatForever::create(pShapiAnimate));
    CCAnimate* pTaidiAnimate = DogBeteLayer::_instance->getDogAnimate("animal7/taidi", 1, 6, 0.1f);
    m_pTaidi -> runAction(CCRepeatForever::create(pTaidiAnimate));
    CCAnimate* pLachangAnimate = DogBeteLayer::_instance->getDogAnimate("animal8/lachang", 1, 6, 0.1f);
    m_pLachang -> runAction(CCRepeatForever::create(pLachangAnimate));
    this -> schedule(schedule_selector(DogRunLayer::dogRunStart), 0.02f);
    
    //显示路障
    showRoadBlock();
}

void DogRunLayer::showRoadBlock()
{
    for(int i = 0; i < m_arrJiansu->count(); i++)
    {
        CCSprite* pCircle = (CCSprite*)m_arrJiansu->objectAtIndex(i);
        if(pCircle == NULL) continue;
        pCircle -> runAction(CCSequence::create(CCShow::create(), CCScaleTo::create(0.5f, 1.05f), CCScaleTo::create(0.1f, 1.0f), NULL));
    }
    for(int i = 0; i < m_arrJiasu->count(); i++)
    {
        CCSprite* pCircle = (CCSprite*)m_arrJiasu->objectAtIndex(i);
        if(pCircle == NULL) continue;
        pCircle -> runAction(CCSequence::create(CCShow::create(), CCScaleTo::create(0.5f, 1.05f), CCScaleTo::create(0.1f, 1.0f), NULL));
    }
}

void DogRunLayer::dogRunStart()
{
    //如果有两只狗都超过了终点，那么就停止比赛
    m_pointEnd = m_spMatchEnd->getPosition();
    int nDogNum = 0;
    for(int i = 0; i < m_arrDog->count(); i++)
    {
        CCSprite* dog = (CCSprite*)m_arrDog->objectAtIndex(i);
        if(dog == NULL) continue;
        CCPoint point = dog->getPosition();
        if(point.x >= m_pointEnd.x)
        {
            nDogNum++;
            if(nDogNum >= 2)
            {
                //停止运动,并且显示冠军和亚军
                dogStopAnimate();
                this -> unschedule(schedule_selector(DogRunLayer::dogRunStart));
                break;
            }
        }
    }
    
    int nWidth = 150;
    
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(DogRunLayer::removeNodeCall));
    //检测碰撞
    for(int i = 0; i < m_arrJiasu->count(); i++)
    {
        CCSprite* pCircle = (CCSprite*)m_arrJiasu->objectAtIndex(i);
        if(pCircle == NULL) continue;
        if(pCircle->getTag() == eCircleUsed) continue;
        CCRect rect = pCircle->boundingBox();
        for(int j = 0; j < m_arrDog->count(); j++)
        {
            CCSprite* dog = (CCSprite*)m_arrDog->objectAtIndex(j);
            if(dog == NULL) continue;
            CCPoint point = dog->getPosition();
            CCPoint point2 = ccp(point.x, point.y+10);
            if(rect.containsPoint(point2))
            {
                pCircle -> setTag(eCircleUsed);
                CCSprite* pRocket = (CCSprite*)pCircle->getChildByTag(1);
                if(pRocket == NULL) continue;
                pRocket -> runAction(CCSequence::create(CCMoveBy::create(2.0, ccp(0, CLIENT_HEIGHT)), func, NULL));
                pCircle -> runAction(CCSequence::create(CCDelayTime::create(0.3f), CCFadeOut::create(0.7f), NULL));
                //判断是否最领先的狗，如果是的话，那么判断是让狗快速运动还是让地图快速运动
                m_nDogSpeed[j] += 4;
                printf("j is %d\n", j);
                if(m_layerContainer->getPositionX()+dog->getPositionX()>=m_spBg1->getContentSize().width/2+nWidth)
                {
                    m_nBgSpeed = -m_nDogSpeed[j];
                }
                this -> scheduleOnce(schedule_selector(DogRunLayer::resetBgSpeed), 0.8f);
                CCCallFuncND* resetSpeedFunc = CCCallFuncND::create(this, callfuncND_selector(DogRunLayer::resetSpeed1), (void*)(long)j);
                dog -> runAction(CCSequence::create(CCDelayTime::create(0.8f), resetSpeedFunc, NULL));
            }
        }
    }
    m_pGaojiasuo->setPosition(ccp(m_pGaojiasuo->getPositionX()+m_nDogSpeed[0], m_pGaojiasuo->getPositionY()));
    m_pJinmao->setPosition(ccp(m_pJinmao->getPositionX()+m_nDogSpeed[1], m_pJinmao->getPositionY()));
    m_pSamoye->setPosition(ccp(m_pSamoye->getPositionX()+m_nDogSpeed[2], m_pSamoye->getPositionY()));
    m_pShapi->setPosition(ccp(m_pShapi->getPositionX()+m_nDogSpeed[3], m_pShapi->getPositionY()));
    m_pTaidi->setPosition(ccp(m_pTaidi->getPositionX()+m_nDogSpeed[4], m_pTaidi->getPositionY()));
    m_pLachang->setPosition(ccp(m_pLachang->getPositionX()+m_nDogSpeed[5], m_pLachang->getPositionY()));
    m_layerContainer->setPosition(ccp(m_layerContainer->getPositionX()+m_nBgSpeed, m_layerContainer->getPositionY()));
    
    
    printf("999  %f,    %f\n", m_layerContainer->getPositionX()+m_pGaojiasuo->getPositionX(), m_spBg1->getContentSize().width/2+nWidth);
    printf("000  %f,    %f\n", m_layerContainer->getPositionX()+m_pJinmao->getPositionX(), m_spBg1->getContentSize().width/2+nWidth);
    if(m_layerContainer->getPositionX()+m_pGaojiasuo->getPositionX()>=m_spBg1->getContentSize().width/2+nWidth)
    {
        m_nBgSpeed = -m_nDogSpeed[0];
        printf("1111   %d\n", m_nBgSpeed);
    }
    if(m_layerContainer->getPositionX()+m_pJinmao->getPositionX()>=m_spBg1->getContentSize().width/2+nWidth)
    {
        m_nBgSpeed = -m_nDogSpeed[1];
        printf("2222   %d\n", m_nBgSpeed);
    }
    if(m_layerContainer->getPositionX()+m_pSamoye->getPositionX()>=m_spBg1->getContentSize().width/2+nWidth)
    {
        m_nBgSpeed = -m_nDogSpeed[2];
    }
    if(m_layerContainer->getPositionX()+m_pShapi->getPositionX()>=m_spBg1->getContentSize().width/2+nWidth)
    {
        m_nBgSpeed = -m_nDogSpeed[3];
    }
    
    if(m_layerContainer->getPositionX()+m_pTaidi->getPositionX()>=m_spBg1->getContentSize().width/2+nWidth)
    {
        m_nBgSpeed = -m_nDogSpeed[4];
    }
    if(m_layerContainer->getPositionX()+m_pLachang->getPositionX()>=m_spBg1->getContentSize().width/2+nWidth)
    {
        m_nBgSpeed = -m_nDogSpeed[5];
    }
    for(int i = 0; i < m_arrJiansu->count(); i++)
    {
        CCSprite* pCircle = (CCSprite*)m_arrJiansu->objectAtIndex(i);
        if(pCircle == NULL) continue;
        if(pCircle->getTag() == eCircleUsed) continue;
        CCRect rect = pCircle->boundingBox();
        for(int j = 0; j < m_arrDog->count(); j++)
        {
            CCSprite* dog = (CCSprite*)m_arrDog->objectAtIndex(j);
            if(dog == NULL) continue;
            CCPoint point = dog->getPosition();
            CCPoint point2 = ccp(point.x, point.y+10);
            if(rect.containsPoint(point2))
            {
                pCircle -> setTag(eCircleUsed);
                CCSprite* pBomb = (CCSprite*)pCircle->getChildByTag(1);
                if(pBomb == NULL) continue;
                pBomb -> runAction(CCSequence::create(DogBeteLayer::getInstance()->getDogAnimate("13/dog_bomb_bg", 1, 5, 0.1f), func, NULL));
                pCircle -> runAction(CCSequence::create(CCDelayTime::create(0.3f), CCFadeOut::create(0.7f), NULL));
                m_nDogSpeed[j] -= 4;
                //如果此狗在屏幕的右侧，并且是最右侧一条狗时，才设置跑道的速度，如果不是最右侧的狗，就不用设置跑道速度，当两个狗位置一样时，则判断这两个狗的速度，如果一致，则设置跑道速度
                if(m_layerContainer->getPositionX()+dog->getPositionX()>=m_spBg1->getContentSize().width/2+nWidth)
                {
                    
                    m_nBgSpeed = -m_nDogSpeed[j];
                }
                CCCallFuncND* resetSpeedFunc = CCCallFuncND::create(this, callfuncND_selector(DogRunLayer::resetSpeed2), (void*)(long)j);
                dog -> runAction(CCSequence::create(CCDelayTime::create(0.8f), resetSpeedFunc, NULL));
            }
        }
    }
}

void DogRunLayer::resetBgSpeed()
{
    printf("hello world\n");
    m_nBgSpeed = -12;
}

void DogRunLayer::resetSpeed1(CCNode* pNode, void* speed)
{
    long nSpeed = (long)speed;
    if(nSpeed>=0 && nSpeed<=5)
    {
        m_nDogSpeed[nSpeed] -= 4;
    }
}

void DogRunLayer::resetSpeed2(CCNode* pNode, void* speed)
{
    long nSpeed = (long)speed;
    if(nSpeed>=0 && nSpeed<=5)
    {
        m_nDogSpeed[nSpeed] += 4;
    }
}

void DogRunLayer::dogStopAnimate()
{
    for(int i = 0; i < m_arrDog->count(); i++)
    {
        CCSprite* dog = (CCSprite*)m_arrDog->objectAtIndex(i);
        if(dog == NULL) continue;
        dog -> stopAllActions();
        if(i+1 == m_nChampion1)//冠军
        {
            CCSprite* champion = CCSprite::createWithSpriteFrameName("dog_rank_guanjun.png");
            champion -> setPosition(ccp(dog->getContentSize().width+champion->getContentSize().width/2+10, dog->getContentSize().height/2));
            dog -> addChild(champion);
            CCSprite* pRank = CCSprite::createWithSpriteFrameName("dog_rank_1.png");
            pRank -> setPosition(ccp(15, champion->getContentSize().height/2));
            champion -> addChild(pRank);
        }
        else if(i+1 == m_nChampion2)//亚军
        {
            CCSprite* champion = CCSprite::createWithSpriteFrameName("dog_rank_yajun.png");
            champion -> setPosition(ccp(dog->getContentSize().width+champion->getContentSize().width/2+10, dog->getContentSize().height/2));
            dog -> addChild(champion);
            CCSprite* pRank = CCSprite::createWithSpriteFrameName("dog_rank_2.png");
            pRank -> setPosition(ccp(15, champion->getContentSize().height/2));
            champion -> addChild(pRank);
        }
    }
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(DogRunLayer::showCalcLayer));
    m_pLayerColor->runAction(CCSequence::create(CCFadeIn::create(0.3f), CCFadeOut::create(0.4f), CCDelayTime::create(1.0f), func, NULL));
}

void DogRunLayer::showCalcLayer()
{
    
}

void DogRunLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool DogRunLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

void DogRunLayer::setCalcData(int nMult, SCORE lWin, SCORE lCaichi, SCORE lBet)
{
    m_nBeilv = nMult;
    m_llGetCoin = lWin;
    m_llCaiChi = lCaichi;
    m_llReturnBet = lBet;
}

void DogRunLayer::setCalcRank(char sName[5][32], SCORE lscore[5], SCORE lCaichi[5])
{
    for(int i = 0; i < 5; i++)
    {
        m_sNameRank[i] = CCharCode::GB2312ToUTF8(sName[i]);
        m_llScoreRank[i] = lscore[i];
        m_llCaiChiRank[i] = lCaichi[i];
        printf("m_sName is %s\n", m_sNameRank[i].c_str());
    }
}