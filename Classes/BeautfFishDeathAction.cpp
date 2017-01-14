//
//  beautfFishDeathAction.cpp
//  CatchFish3
//
//  Created by 王军闯 on 16/5/27.
//
//

#include "header.h"

CBeautfFishDeath* CBeautfFishDeath::create(int nNum, int nOpenType)
{
    CBeautfFishDeath* pRet = new CBeautfFishDeath;
    if(pRet && pRet->init(nNum, nOpenType))
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

bool CBeautfFishDeath::init(int nNum, int nOpenType)
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    m_nFishScore = nNum;
    m_nOpenType = nOpenType;
    
    this -> setVisible(false);
    
    if(nOpenType != 0)
    {
        this -> scheduleOnce(schedule_selector(CBeautfFishDeath::showBeautifuUI), 0.1f);
    }
    else
    {
        this -> scheduleOnce(schedule_selector(CBeautfFishDeath::showBeautifuUI), 1.0f);
    }
    return true;
}

void CBeautfFishDeath::showBeautifuUI()
{
    this -> setVisible(true);
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    this->setTouchEnabled(true);
    createAction(m_nFishScore);
}

void CBeautfFishDeath::createAction(int nNum, int nOpenType)
{
    m_nTargetScore = nNum;
    float fTime = 1.0;//30帧
    float fBaofuHeight = 90;
    CCSprite* pBaoFu = CCSprite::create("deathAct/baofu.png");
    pBaoFu->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+fBaofuHeight));
    this->addChild(pBaoFu);
    pBaoFu->setScale(0);
    CCDelayTime* pBaofuDelay = CCDelayTime::create(fTime*11/15);
    CCScaleTo* pBaofuScaleTo1 = CCScaleTo::create(0, 1);
    CCScaleTo* pBaofuScaleTo2 = CCScaleTo::create(fTime*7/30, 6);
    CCFadeOut* pBaofuFadeOut = CCFadeOut::create(fTime*7/30);
    CCMoveBy* pBaofuMoveBy = CCMoveBy::create(fTime*7/30, ccp(0, -fBaofuHeight));
    CCSpawn* pBaofuSpawn = CCSpawn::create(pBaofuScaleTo2, pBaofuFadeOut, pBaofuMoveBy, NULL);
    CCSequence* pBaofuSeq = CCSequence::create(pBaofuDelay, pBaofuScaleTo1, pBaofuSpawn, NULL);
    pBaoFu->runAction(pBaofuSeq);
    
    CCSprite* pBaoFu2 = CCSprite::create("deathAct/baofu.png");
    pBaoFu2->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(pBaoFu2);
    pBaoFu2->setScale(0);
    CCDelayTime* pBaofu2Delay = CCDelayTime::create(fTime*7/15);
    CCScaleTo* pBaofu2ScaleTo1 = CCScaleTo::create(0, 6);
    CCScaleTo* pBaofu2ScaleTo2 = CCScaleTo::create(fTime*7/30, 0);
    CCFadeOut* pBaofu2FadeOut = CCFadeOut::create(fTime*7/30);
    CCMoveBy* pBaofu2MoveBy = CCMoveBy::create(fTime*7/30, ccp(0, fBaofuHeight));
    CCSpawn* pBaofu2Spawn = CCSpawn::create(pBaofu2ScaleTo2, pBaofu2FadeOut, pBaofu2MoveBy, NULL);
    CCSequence* pBaofu2Seq = CCSequence::create(pBaofu2Delay, pBaofu2ScaleTo1, pBaofu2Spawn, NULL);
    pBaoFu2->runAction(pBaofu2Seq);
    
    CCSprite* pLightPoint = CCSprite::create("deathAct/lightPoint.png");
    pLightPoint->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(pLightPoint);
    pLightPoint->setScale(0);
    CCDelayTime* pDelay = CCDelayTime::create(fTime/30);
    CCScaleTo* pPointScaleTo1 = CCScaleTo::create(fTime/30, 1);
    CCScaleTo* pPointScaleTo2 = CCScaleTo::create(fTime/15, 3);
    CCScaleTo* pPointScaleTo3 = CCScaleTo::create(fTime*7/30, 6);
    CCFadeOut* pPointFadeOut = CCFadeOut::create(fTime*7/30);
    CCSpawn* pPointSpawn = CCSpawn::create(pPointScaleTo3, pPointFadeOut, NULL);
    CCSequence* pPointSeq = CCSequence::create(pDelay, pPointScaleTo1, pDelay, pPointScaleTo2, pDelay, pPointSpawn, NULL);
    pLightPoint->runAction(pPointSeq);
    
    CCSprite* pLightCircle = CCSprite::create("deathAct/lightCircle.png");
    pLightCircle->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(pLightCircle);
    pLightCircle->setScale(0);
    CCDelayTime* pCircleDelay = CCDelayTime::create(fTime/15);
    CCScaleTo* pCircleScaleTo1 = CCScaleTo::create(fTime/30, 0.2f);
    CCScaleTo* pCircleScaleTo2 = CCScaleTo::create(fTime*2/15, 2);
    CCScaleTo* pCircleScaleTo3 = CCScaleTo::create(fTime*2/15, 4);
    CCFadeOut* pCircleFadeOut = CCFadeOut::create(fTime*2/15);
    CCSpawn* pCircleSpawn = CCSpawn::create(pCircleScaleTo3, pCircleFadeOut, NULL);
    CCSequence* pCircleSeq = CCSequence::create(pCircleDelay, pCircleScaleTo1, pDelay, pCircleScaleTo2, pDelay, pCircleSpawn, NULL);
    pLightCircle->runAction(pCircleSeq);
    
    CCSprite* pLeftWing = CCSprite::create("deathAct/wing.png");
    pLeftWing->setAnchorPoint(ccp(1, 0.7f));
    pLeftWing->setPosition(ccp(CLIENT_WIDTH*0.45f, CLIENT_HEIGHT*0.58f));
    this->addChild(pLeftWing);
    pLeftWing->setRotation(-80);
    pLeftWing->setScale(0);
    CCDelayTime* pWingDelay = CCDelayTime::create(fTime/3);
    CCScaleTo* pLeftScaleTo1 = CCScaleTo::create(0, 1);
    CCFadeTo* pLeftFadeTo1 = CCFadeTo::create(0, 60);
    CCSpawn* pLeftSpawn1 = CCSpawn::create(pLeftScaleTo1, pLeftFadeTo1, NULL);
    CCFadeTo* pLeftFadeTo2 = CCFadeTo::create(fTime/4, 255);
    CCRotateBy* pLeftRotateBy = CCRotateBy::create(fTime/4, 80);
    CCSpawn* pLeftSpawn = CCSpawn::create(pLeftFadeTo2, pLeftRotateBy, NULL);
    CCSequence* pLeftSeq = CCSequence::create(pWingDelay, pLeftSpawn1, pLeftSpawn, NULL);
    pLeftWing->runAction(pLeftSeq);
    
    CCSprite* pRightWing = CCSprite::create("deathAct/wing.png");
    pRightWing->setAnchorPoint(ccp(0, 0.7f));
    pRightWing->setPosition(ccp(CLIENT_WIDTH*0.55f, pLeftWing->getPositionY()));
    this->addChild(pRightWing);
    pRightWing->setFlipX(true);
    pRightWing->setRotation(80);
    pRightWing->setScale(0);
    CCScaleTo* pRightScaleTo1 = CCScaleTo::create(0, 1);
    CCFadeTo* pRightFadeTo1 = CCFadeTo::create(0, 60);
    CCSpawn* pRightSpawn1 = CCSpawn::create(pRightScaleTo1, pRightFadeTo1, NULL);
    CCFadeTo* pRightFadeTo2 = CCFadeTo::create(fTime/4, 255);
    CCRotateBy* pRightRotateBy = CCRotateBy::create(fTime/4, -80);
    CCSpawn* pRightSpawn = CCSpawn::create(pRightFadeTo2, pRightRotateBy, NULL);
    CCSequence* pRightSeq = CCSequence::create(pWingDelay, pRightSpawn1, pRightSpawn, NULL);
    pRightWing->runAction(pRightSeq);
    
    CCSprite* pLight = CCSprite::create("deathAct/light.png");
    pLight->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(pLight);
    pLight->setScale(0);
    CCDelayTime* pLightDelay = CCDelayTime::create(fTime/10);
    CCScaleTo* pLightScaleTo = CCScaleTo::create(fTime*2/15, 1.5f);
    CCRotateTo* pLightRotate = CCRotateTo::create(5, 1080);
    CCSequence* pLightSeq = CCSequence::create(pLightDelay, pLightScaleTo, pLightRotate, NULL);
    pLight->runAction(pLightSeq);
    
    CCSprite* pLightStar = CCSprite::create("deathAct/lightStar.png");
    pLightStar->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(pLightStar);
    pLightStar->setScale(0);
    ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE };
    pLightStar->setBlendFunc(cbl);
    pLightStar->runAction(CCSequence::create(CCDelayTime::create(fTime*7/15), CCScaleTo::create(0, 1), NULL));
    
    CCSprite* pDunpai = CCSprite::create("deathAct/dunpai.png");
    pDunpai->setPosition(ccp(CLIENT_WIDTH*0.5+5, CLIENT_HEIGHT*0.43f));
    this->addChild(pDunpai);
    pDunpai->setScale(0);
    CCDelayTime* pDunDelay = CCDelayTime::create(fTime*4/15);
    CCScaleTo* pDunScaleTo1 = CCScaleTo::create(0, 0.3f);
    CCFadeTo* pDunFadeTo1 = CCFadeTo::create(0, 60);
    CCSpawn* pDunSpawn1 = CCSpawn::create(pDunScaleTo1, pDunFadeTo1, NULL);
    CCScaleTo* pDunScaleTo2 = CCScaleTo::create(fTime/10, 1);
    CCFadeTo* pDunFadeTo2 =CCFadeTo::create(fTime/10, 255);
    CCSpawn* pDunSpawn2 = CCSpawn::create(pDunScaleTo2, pDunFadeTo2, NULL);
    CCSequence* pDunSeq = CCSequence::create(pDunDelay, pDunSpawn1, pDunSpawn2, NULL);
    pDunpai->runAction(pDunSeq);
    
    CCSprite* pLightOutSide = CCSprite::create("deathAct/lightOutside.png");
    pLightOutSide->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.42f+5));
    this->addChild(pLightOutSide);
    pLightOutSide->setScale(0);
    CCDelayTime* pOutDelay = CCDelayTime::create(fTime*11/30);
    CCScaleTo* pOutScale = CCScaleTo::create(0, 1);
    CCBlink* pBlink = CCBlink::create(5, 15);
    CCSequence* pOutSeq = CCSequence::create(pOutDelay, pOutScale, pBlink, NULL);
    pLightOutSide->runAction(pOutSeq);
    
    CCSprite* pBaoFu3 = CCSprite::create("deathAct/baofu.png");
    pBaoFu3->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(pBaoFu3);
    pBaoFu3->setScale(0);
    CCDelayTime* pBaofu3Delay = CCDelayTime::create(fTime*13/30);
    CCScaleTo* pBaofu3ScaleTo1 = CCScaleTo::create(0, 6);
    CCScaleTo* pBaofu3ScaleTo2 = CCScaleTo::create(fTime/6, 0.8f);
    CCMoveBy* pBaofu3MoveBy = CCMoveBy::create(fTime/6, ccp(0, fBaofuHeight));
    CCSpawn* pBaofu3Spawn = CCSpawn::create(pBaofu3ScaleTo2, pBaofu3MoveBy, NULL);
    CCScaleTo* pBaofu3ScaleTo3 = CCScaleTo::create(fTime/30, 1);
    CCSequence* pBaofu3Seq = CCSequence::create(pBaofu3Delay, pBaofu3ScaleTo1, pBaofu3Spawn, pDelay, pBaofu3ScaleTo3, NULL);
    pBaoFu3->runAction(pBaofu3Seq);
    
    CCSprite* pNumBg = CCSprite::create("deathAct/numBg.png");
    pNumBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.4f));
    this->addChild(pNumBg);
    pNumBg->setScale(0);
    CCDelayTime* pNumBgDelay = CCDelayTime::create(fTime*17/30);
    CCScaleTo* pNumBgScaleTo = CCScaleTo::create(0, 1);
    CCFadeIn* pNumBgFadeIn = CCFadeIn::create(fTime*4/15);
    CCSequence* pNumBgSeq = CCSequence::create(pNumBgDelay, pNumBgScaleTo, pNumBgFadeIn, NULL);
    pNumBg->runAction(pNumBgSeq);
    
    char sNum[16];
    sprintf(sNum, "%d", nNum);
    m_pScoreAtlas = CCLabelAtlas::create(sNum, "deathAct/deathNum.png", 46, 64, '0');
    m_pScoreAtlas->setAnchorPoint(ccp(0.5f, 0.5f));
    m_pScoreAtlas->setPosition(ccp(pNumBg->getContentSize().width/2+20, pNumBg->getContentSize().height*0.55f));
    pNumBg->addChild(m_pScoreAtlas);
    m_pScoreAtlas->setScale(0);
    CCDelayTime* pScoreDelay = CCDelayTime::create(fTime*5/6);
    CCScaleTo* pScoreScaleTo = CCScaleTo::create(0, 0.8f);
    CCCallFunc* pScoreCallFunc = CCCallFunc::create(this, callfunc_selector(CBeautfFishDeath::playScoreAction));
    m_pScoreAtlas->runAction(CCSequence::create(pScoreDelay, pScoreCallFunc, pScoreScaleTo, NULL));
    
    CCSprite* pAddSp = CCSprite::create("deathAct/add.png");
    pAddSp->setPosition(ccp(-pAddSp->getContentSize().width/2, m_pScoreAtlas->getContentSize().height*0.57f));
    m_pScoreAtlas->addChild(pAddSp);
    
    CCDelayTime* pThisDelay = CCDelayTime::create(fTime*2.2f);
    CCCallFunc* pCallFunc = CCCallFunc::create(this, callfunc_selector(CBeautfFishDeath::removeThis));
    this->runAction(CCSequence::create(pThisDelay, pCallFunc, NULL));
}

void CBeautfFishDeath::playScoreAction()
{
    this->schedule(schedule_selector(CBeautfFishDeath::runToTargetNum), 0.05f);
}

void CBeautfFishDeath::runToTargetNum(float ft)
{
    static int nTimes = 0;
    nTimes++;
    int nTempMoney = m_nTargetScore*0.1f*nTimes;
    char sMoney[32];
    sprintf(sMoney, "%d", nTempMoney);
    m_pScoreAtlas->setString(sMoney);
    if (nTimes == 10)
    {
        this->unschedule(schedule_selector(CBeautfFishDeath::runToTargetNum));
        nTimes = 0;
        return;
    }
}

void CBeautfFishDeath::removeThis()
{
    this->removeFromParent();
}

void CBeautfFishDeath::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool CBeautfFishDeath::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}