#include "header.h"

SuperExtraLayer* SuperExtraLayer::create(LONGLONG lExtra)
{
    SuperExtraLayer* pRet = new SuperExtraLayer;
    if(pRet && pRet->init(lExtra))
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

bool SuperExtraLayer::init(LONGLONG lExtra)
{
    if (!CCLayer::init())
    {
        return  false;
    }
    
    CCLayerColor* pBgLayer = CCLayerColor::create(ccc4(0, 0, 0, 100));
    this->addChild(pBgLayer);
    
    loadUI(lExtra);
    
    this->setTouchEnabled(true);
    
    return true;
}

void SuperExtraLayer::loadUI(LONGLONG lExtra)
{
    CCSprite* pCircle1 = CCSprite::createWithSpriteFrameName("circle1.png");
    pCircle1->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-50));
    this->addChild(pCircle1, 1);
    
    CCSprite* pCircle2 = CCSprite::createWithSpriteFrameName("circle2.png");
    pCircle2->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-50));
    this->addChild(pCircle2, 1);
    
    CCSprite* pHalfCircle = CCSprite::createWithSpriteFrameName("halfCircle.png");
    pHalfCircle->setScale(1.0f);
    pHalfCircle->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+30));
    this->addChild(pHalfCircle, 1);
    
    CCSprite* pLeftHorn = CCSprite::createWithSpriteFrameName("laba.png");
    pLeftHorn->setFlipX(true);
    pLeftHorn->setPosition(ccp(CLIENT_WIDTH/2-50, CLIENT_HEIGHT/2-20));
    this->addChild(pLeftHorn, 1);
    
    CCSprite* pRightHorn = CCSprite::createWithSpriteFrameName("laba.png");
    pRightHorn->setPosition(ccp(CLIENT_WIDTH/2+50, CLIENT_HEIGHT/2-20));
    this->addChild(pRightHorn, 1);
    
    CCSprite* pCirclePanDI = CCSprite::create("CatchFish2001/fishscoreyuan2.png");
    pCirclePanDI->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-50));
    this->addChild(pCirclePanDI, 1);
    CCSprite* pCirclePan = CCSprite::create("CatchFish2001/fishscoreyuan1.png");
    pCirclePan->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-50));
    this->addChild(pCirclePan, 1);
    
    CCSprite* pJLFont = CCSprite::createWithSpriteFrameName("JLfont.png");
    pJLFont->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+40));
    this->addChild(pJLFont, 1);
    
    CCSprite* pBuTiao = CCSprite::createWithSpriteFrameName("butiao.png");
    pBuTiao->setPosition(ccp(CLIENT_WIDTH/2, pCirclePan->getPositionY()-60));
    this->addChild(pBuTiao, 1);
    
    char sExtar[16];
    sprintf(sExtar, ":%lld", lExtra);
    CCLabelAtlas* pExtraNum = CCLabelAtlas::create(sExtar, "SuperPirate/extraNum.png", 29, 51, '0');
    pExtraNum->setAnchorPoint(ccp(0.5f, 0.5f));
    pExtraNum->setPosition(pCirclePan->getPosition());
    this->addChild(pExtraNum, 1);
    
    
    CCSprite* pSamll1 = CCSprite::createWithSpriteFrameName("star1.png");
    CCSprite* pSamll2 = CCSprite::createWithSpriteFrameName("star1.png");
    CCSprite* pBig1 = CCSprite::createWithSpriteFrameName("star1.png");
    CCSprite* pBig2 = CCSprite::createWithSpriteFrameName("star1.png");
    CCSprite* pBig3 = CCSprite::createWithSpriteFrameName("star1.png");
    pSamll1->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-40));
    pSamll2->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-40));
    pBig1->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-20));
    pBig2->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-20));
    pBig3->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(pSamll1, 1);
    this->addChild(pSamll2, 1);
    this->addChild(pBig1, 1);
    this->addChild(pBig2, 1);
    this->addChild(pBig3, 1);
    
//    pCirclePanDI->runAction(CCRepeatForever::create(CCRotateBy::create(0.2f, -150)));
//    pCirclePan->runAction(CCRepeatForever::create(CCRotateBy::create(0.2f, 120)));
//    return;
    pCircle1->setOpacity(0);
    pCircle1->setScale(0);
    pCircle2->setOpacity(0);
    pCircle2->setScale(0);
    pHalfCircle->setOpacity(0);
    pHalfCircle->setScale(0);
    pLeftHorn->setOpacity(0);
    pLeftHorn->setScale(0);
    pRightHorn->setOpacity(0);
    pRightHorn->setScale(0);
    pCirclePanDI->setOpacity(0);
    pCirclePanDI->setScale(0);
    pCirclePan->setOpacity(0);
    pCirclePan->setScale(0);
    pJLFont->setOpacity(0);
    pJLFont->setScale(0);
    pBuTiao->setOpacity(0);
    pBuTiao->setScale(0);
    pExtraNum->setOpacity(0);
    pExtraNum->setScale(0);
    pSamll1->setOpacity(0);
    pSamll1->setScale(0);
    pSamll2->setOpacity(0);
    pSamll2->setScale(0);
    pBig1->setOpacity(0);
    pBig1->setScale(0);
    pBig2->setOpacity(0);
    pBig2->setScale(0);
    pBig3->setOpacity(0);
    pBig3->setScale(0);
    
    float fStart=0.1f;
    float fDelay=0.9+0.5f;
    float fEnd=0.1f;
    
    pCircle1->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                           CCScaleTo::create(fStart, 1.2f),
                                                           NULL),
                                           CCScaleTo::create(0.1f, 1.0f),
                                           CCDelayTime::create(fDelay),
                                           CCSpawn::create(CCScaleTo::create(fEnd, 0),
                                                           CCMoveTo::create(fEnd, ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2)),
                                                           NULL),
                                           NULL));
    pCircle2->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                           CCScaleTo::create(fStart, 1.2f),
                                                           NULL),
                                           CCScaleTo::create(0.1f, 1.0f),
                                           CCDelayTime::create(fDelay),
                                           CCSpawn::create(CCScaleTo::create(fEnd, 0),
                                                           CCMoveTo::create(fEnd, ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2)),
                                                           NULL),
                                           NULL));
    pHalfCircle->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                              CCScaleTo::create(fStart, 1.2f),
                                                              NULL),
                                              CCDelayTime::create(fDelay+0.1f),
                                              CCSpawn::create(CCScaleTo::create(fEnd, 0),
                                                              CCMoveTo::create(fEnd, ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2)),
                                                              NULL),
                                              NULL));
    pLeftHorn->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                            CCScaleTo::create(fStart, 1.2f),
                                                            CCMoveBy::create(fStart, ccp(-120, 60)),
                                                            NULL),
                                            CCScaleTo::create(0.1f, 1.0f),
                                            CCDelayTime::create(fDelay),
                                            CCSpawn::create(CCScaleTo::create(fEnd, 0),
                                                            CCMoveTo::create(fEnd, ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2)),
                                                            NULL),
                                            NULL));
    pRightHorn->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                            CCScaleTo::create(fStart, 1.2f),
                                                            CCMoveBy::create(fStart, ccp(120, 60)),
                                                             NULL),
                                             CCScaleTo::create(0.1f, 1.0f),
                                             CCDelayTime::create(fDelay),
                                             CCSpawn::create(CCScaleTo::create(fEnd, 0),
                                                             CCMoveTo::create(fEnd, ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2)),
                                                             NULL),
                                             NULL));
    pCirclePanDI->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                               CCScaleTo::create(fStart, 1.2f),
                                                               NULL),
                                               CCScaleTo::create(0.1f, 1.0f),
                                               CCSpawn::create(CCRepeat::create(CCRotateBy::create(0.2f, -150),17),
                                                               CCSequence::create(CCDelayTime::create(fDelay),
                                                                                  CCScaleTo::create(fEnd, 0),
                                                                                  NULL),
                                                               NULL),
                                               NULL));
    pCirclePan->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                             CCScaleTo::create(fStart, 1.2f),
                                                             NULL),
                                             CCScaleTo::create(0.1f, 1.0f),
                                             CCSpawn::create(CCRepeat::create(CCRotateBy::create(0.2f, 120),17),
                                                             CCSequence::create(CCDelayTime::create(fDelay),
                                                                                CCScaleTo::create(fEnd, 0),
                                                                                NULL),
                                                             NULL),
                                             NULL));
    pJLFont->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                          CCScaleTo::create(0, 5.0f),
                                                        NULL),
                                          CCScaleTo::create(0.1f, 1.0f),
                                          CCDelayTime::create(fDelay),
                                          CCSpawn::create(CCScaleTo::create(fEnd, 0),
                                                          CCMoveTo::create(fEnd, ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2)),
                                                          NULL),
                                          NULL));
    pBuTiao->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                          CCScaleTo::create(fStart, 1.2f),
                                                                     NULL),
                                          CCScaleTo::create(0.1f, 1.0f),
                                          CCDelayTime::create(fDelay),
                                          CCSpawn::create(CCScaleTo::create(fEnd, 0),
                                                          CCMoveTo::create(fEnd, ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2)),
                                                          NULL),
                                          NULL));
    pExtraNum->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                            CCScaleTo::create(fStart, 1.2f),
                                                          NULL),
                                            CCScaleTo::create(0.1f, 1.0f),
                                            CCDelayTime::create(fDelay),
                                            CCSpawn::create(CCScaleTo::create(fEnd, 0),
                                                            CCMoveTo::create(fEnd, ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2)),
                                                            NULL),
                                            NULL));
    pSamll1->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                             CCScaleTo::create(fStart, 1.2f),
                                                             CCMoveBy::create(fStart, ccp(-190, 0)),
                                                             NULL),
                                        CCScaleTo::create(0.1f, 1.0f),
                                        CCRepeat::create(CCSequence::create(CCFadeTo::create(0.15f, 200),CCFadeTo::create(0.15f, 255),NULL), 4),
                                        CCScaleTo::create(fEnd, 0),
                                          NULL));
    pSamll2->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                             CCScaleTo::create(fStart, 1.2f),
                                                             CCMoveBy::create(fStart, ccp(190, 0)),
                                                             NULL),
                                        CCScaleTo::create(0.1f, 1.0f),
                                        CCRepeat::create(CCSequence::create(CCFadeTo::create(0.15f, 200),CCFadeTo::create(0.15f, 255),NULL), 4),
                                        CCScaleTo::create(fEnd, 0),
                                          NULL));
    pBig1->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                             CCScaleTo::create(fStart, 1.2f),
                                                             CCMoveBy::create(fStart, ccp(-125, 145)),
                                                             NULL),
                                        CCScaleTo::create(0.1f, 1.0f),
                                        CCRepeat::create(CCSequence::create(CCFadeTo::create(0.15f, 200),CCFadeTo::create(0.15f, 255),NULL), 4),
                                        CCScaleTo::create(fEnd, 0),
                                        NULL));
    pBig2->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                              CCScaleTo::create(fStart, 1.2f),
                                                              CCMoveBy::create(fStart, ccp(125, 145)),
                                                              NULL),
                                        CCScaleTo::create(0.1f, 1.0f),
                                        CCRepeat::create(CCSequence::create(CCFadeTo::create(0.15f, 200),CCFadeTo::create(0.15f, 255),NULL), 4),
                                        CCScaleTo::create(fEnd, 0),
                                        NULL));
    pBig3->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(fStart),
                                                              CCScaleTo::create(fStart, 1.5f),
                                                              CCMoveBy::create(fStart, ccp(0, 170)),
                                                              NULL),
                                        CCScaleTo::create(0.1f, 1.3f),
                                        CCRepeat::create(CCSequence::create(CCFadeTo::create(0.15f, 200),CCFadeTo::create(0.15f, 255),NULL), 4),
                                        CCScaleTo::create(fEnd, 0),
                                        NULL));

    this->runAction(CCSequence::create(CCDelayTime::create(2.1f),CCCallFunc::create(this, callfunc_selector(SuperExtraLayer::removeself)),NULL));
}

void SuperExtraLayer::removeself()
{
    this->removeFromParentAndCleanup(true);
}

void SuperExtraLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);//-129
}

bool SuperExtraLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

