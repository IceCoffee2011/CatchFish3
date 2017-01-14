#include "header.h"

extern const char* sSignInItemName[19];

GetPriseLayer* GetPriseLayer::create(int nType, int nCount, std::string sDescription, float fDelayTime)
{
    GetPriseLayer* pRet = new GetPriseLayer();
    if(pRet && pRet->init(nType, nCount, sDescription, fDelayTime))
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

bool GetPriseLayer::init(int cbPriseType, int nPriseCount, std::string sDescription, float fDelayTime)
{
    if(!CCLayer::init())
    {
        return false;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    SimpleAudioEngine::sharedEngine()->playEffect("Music/openbox.mp3");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->playEffect("Music/openbox.ogg");
#endif
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GetPriseLayer::removeWnd));
    CCSprite* sprite5 = CCSprite::create("signIn/signsuccess5.png");
    sprite5 -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(sprite5, 200);
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    pLayerColor -> ignoreAnchorPointForPosition(false);
    pLayerColor -> setPosition(ccp(sprite5->getContentSize().width/2, sprite5->getContentSize().height/2));
    sprite5 -> addChild(pLayerColor, -2);
    CCSprite* sprite6 = CCSprite::create("signIn/signsuccess6.png");
    sprite6 -> setPosition(ccp(sprite5->getContentSize().width/2, sprite5->getContentSize().height/2));
    sprite5 -> addChild(sprite6, -1);
    sprite5 -> setOpacity(0);
    sprite6 ->setOpacity(0);
    sprite6 -> runAction(CCRepeatForever::create(CCRotateBy::create(2.0f, 180)));
    sprite6 -> runAction(CCFadeIn::create(0.5f));
    sprite5 -> runAction(CCSequence::create(CCFadeIn::create(0.3f), CCDelayTime::create(fDelayTime-0.2f),func, NULL));
    CCSprite* sprite3 = CCSprite::create("signIn/signsuccess3.png");
    sprite3 -> setPosition(ccp(sprite5->getContentSize().width/2, sprite5->getContentSize().height/4*3-10));
    sprite5 -> addChild(sprite3, 200);
    CCSprite* sprite41 = CCSprite::create("signIn/signsuccess4.png");
    sprite41 -> setPosition(ccp(sprite3->getContentSize().width/4, sprite3->getContentSize().height/2+10));
    sprite3 -> addChild(sprite41, 200);
    CCSprite* sprite42 = CCSprite::create("signIn/signsuccess4.png");
    sprite42 -> setPosition(ccp(sprite3->getContentSize().width/4*3, sprite3->getContentSize().height/2+10));
    sprite3 -> addChild(sprite42, 200);
    CCSprite* sprite1 = CCSprite::create("signIn/signsuccess1.png");
    sprite1 -> setPosition(ccp(sprite3->getContentSize().width/4*2, sprite3->getContentSize().height/2+10));
    sprite3 -> setScale(3.0f);
    sprite3 -> addChild(sprite1, 200);
    sprite3 -> runAction(CCSequence::create(CCHide::create(), CCDelayTime::create(0.2f), CCShow::create(), CCScaleTo::create(0.2f, 1.0f), NULL));
    
    
    CCSprite* pAwardBg3 = CCSprite::create("signIn/signAwardBg.png");
    pAwardBg3->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-20));
    this->addChild(pAwardBg3, 200);
    pAwardBg3 -> runAction(CCRepeatForever::create(CCRotateTo::create(2, -180)));
    CCSprite* pAwardBg = CCSprite::create("signIn/signAwardBg.png");
    pAwardBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-20));
    this->addChild(pAwardBg, 200);
    pAwardBg -> setOpacity(0);
    CCSprite* pAward = NULL;
    char sNum[16];
    CCLabelAtlas* pNum = CCLabelAtlas::create("", "signIn/signsuccess2.png", 53, 79, '0');
    CCSprite* pMark = NULL;
    pAward = CCSprite::create(sSignInItemName[cbPriseType-1]);
    pAward->setPosition(ccp(pAwardBg->getContentSize().width/2, pAwardBg->getContentSize().height/2));
    pAwardBg->addChild(pAward);
    if (cbPriseType == 2)
    {
        pAward->setScale(0.8f);
    }
    else if (cbPriseType == 3)
    {
        pAward->setScale(1.3f);
        pAward->setPositionY(pAwardBg->getContentSize().height/2-10);
    }
    else if (cbPriseType>=10)
    {
        pAward->setScale(0.7f);
    }
    sprintf(sNum, "%d", nPriseCount);
    pNum ->setString(sNum);
    
    pAwardBg->setPositionX(CLIENT_WIDTH/2);
    
    pNum->setScale(0.7f);
    pNum->setAnchorPoint(ccp(0.5f, 0.5f));
    pNum->setPosition(ccp(pAward->getPositionX(),-pNum->boundingBox().size.height/2+50));
    pAwardBg->addChild(pNum);
    
    pMark = CCSprite::create("signIn/signsuccess8.png");
    pMark->setScale(0.6f);
    pMark->setAnchorPoint(ccp(1, 0.5f));
    pMark->setPosition(ccp(pNum->getPositionX()-pNum->boundingBox().size.width/2-5, pNum->getPositionY()));
    pAwardBg->addChild(pMark);
    pAwardBg->runAction(CCSequence::create(CCDelayTime::create(fDelayTime),CCCallFuncN::create(this, callfuncN_selector(GetPriseLayer::removeWnd)),NULL));
    pAwardBg3->runAction(CCSequence::create(CCDelayTime::create(fDelayTime),CCCallFuncN::create(this, callfuncN_selector(GetPriseLayer::removeWnd)),NULL));
    
    return true;
}

void GetPriseLayer::removeWnd(CCNode* pNode)
{
    pNode -> removeFromParent();
}