#include "header.h"

const char* CardBigColor[4] = {"bigtag_0.png", "bigtag_1.png", "bigtag_2.png", "bigtag_3.png"};
const char* CardSmallColor[4] = {"smalltag_0.png", "smalltag_1.png", "smalltag_2.png", "smalltag_3.png"};
const char* CardBigRedBlack[6] = {"bigtag_red_10.png", "bigtag_red_11.png", "bigtag_red_12.png", "bigtag_black_10.png", "bigtag_black_11.png", "bigtag_black_12.png"};
const char* CardNum[26] = {"red_0.png", "red_1.png", "red_2.png", "red_3.png", "red_4.png", "red_5.png", "red_6.png", "red_7.png", "red_8.png", "red_9.png", "red_10.png", "red_11.png", "red_12.png", "black_0.png", "black_1.png", "black_2.png", "black_3.png", "black_4.png", "black_5.png", "black_6.png", "black_7.png", "black_8.png", "black_9.png", "black_10.png", "black_11.png", "black_12.png"};

#define kInAngleZ   270
#define kInDeltaZ   90
#define kOutAngleZ  0
#define kOutDeltaZ  90

enum{
    tag_inCard = 1,
    tag_outCard,
};

MyScaleTo* MyScaleTo::create(float duration, float s)
{
    MyScaleTo *pScaleTo = new MyScaleTo();
    pScaleTo->initWithDuration(duration, s);
    pScaleTo->autorelease();
    
    return pScaleTo;
}

bool MyScaleTo::initWithDuration(float duration, float s)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_fEndScaleX = s;
        m_fEndScaleY = s;
        
        return true;
    }
    
    return false;
}

MyScaleTo* MyScaleTo::create(float duration, float sx, float sy)
{
    MyScaleTo *pScaleTo = new MyScaleTo();
    pScaleTo->initWithDuration(duration, sx, sy);
    pScaleTo->autorelease();
    
    return pScaleTo;
}

bool MyScaleTo::initWithDuration(float duration, float sx, float sy)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_fEndScaleX = sx;
        m_fEndScaleY = sy;
        
        return true;
    }
    
    return false;
}

CCObject* MyScaleTo::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    MyScaleTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (MyScaleTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new MyScaleTo();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    CCActionInterval::copyWithZone(pZone);
    
    
    pCopy->initWithDuration(m_fDuration, m_fEndScaleX, m_fEndScaleY);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void MyScaleTo::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_fStartScaleX = pTarget->getScaleX();
    m_fStartScaleY = pTarget->getScaleY();
    m_fDeltaX = m_fEndScaleX - m_fStartScaleX;
    m_fDeltaY = m_fEndScaleY - m_fStartScaleY;
}

void MyScaleTo::update(float time)
{
    if (m_pTarget)
    {
        m_pTarget->setScaleX(m_fStartScaleX + m_fDeltaX * time);
    }
}

CardSprite::CardSprite()
{
    
}

CardSprite::~CardSprite()
{
    m_openAnimIn -> release();
    m_openAnimOut -> release();
}

CardSprite* CardSprite::create(const char* inCardImageName, const char* outCardImageName, BYTE cbCardData, float duration)
{
    CardSprite* pSprite = new CardSprite();
    if(pSprite && pSprite->init(inCardImageName, outCardImageName, cbCardData, duration))
    {
        pSprite -> autorelease();
        return pSprite;
    }
    else
    {
        delete pSprite;
        pSprite = NULL;
        return NULL;
    }
}

bool CardSprite::init(const char* inCardImageName, const char* outCardImageName, BYTE cbCardData, float duration)
{
    if(!CCSprite::init())
    {
        return false;
    }
    
    initData(inCardImageName, outCardImageName, cbCardData, duration);
    
    return true;
}

void CardSprite::initData(const char* inCardImageName, const char* outCardImageName, BYTE cbCardData, float duration)
{
    m_bOpened = false;
    CCSprite* inCard = CCSprite::createWithSpriteFrameName(inCardImageName);
    inCard -> setPosition(CCPointZero);
    inCard -> setVisible(false);
    inCard -> setAnchorPoint(ccp(0.5f, 1.0f));
    inCard -> setTag(tag_inCard);
    addChild(inCard);
    
    CCSprite* outCard = CCSprite::createWithSpriteFrameName(outCardImageName);
    outCard -> setPosition(CCPointZero);
    outCard -> setTag(tag_outCard);
    outCard -> setAnchorPoint(ccp(0.5f, 1.0f));
    addChild(outCard);
    
    BYTE cbCardColor = GetCardColor(cbCardData)/16;
    BYTE cbCardValue = GetCardValue(cbCardData);
    if(cbCardColor >= 0 && cbCardColor <= 3)
    {
        CCSprite* pSmallColor = CCSprite::createWithSpriteFrameName(CardSmallColor[cbCardColor]);
        pSmallColor -> setPosition(ccp(pSmallColor->getContentSize().width/2+8, inCard->getContentSize().height/2+5));
        inCard -> addChild(pSmallColor);
        if(cbCardValue >= 1 && cbCardValue <= 13)
        {
            CCSprite* pCardNum = CCSprite::createWithSpriteFrameName(CardNum[13*(cbCardColor%2)+cbCardValue-1]);
            pCardNum -> setPosition(ccp(pCardNum->getContentSize().width/2-4, inCard->getContentSize().height-pCardNum->getContentSize().height/2-4));
            inCard -> addChild(pCardNum);
            CCSprite* pCardContain = NULL;
            if(cbCardValue <= 10)
            {
                pCardContain = CCSprite::createWithSpriteFrameName(CardBigColor[cbCardColor]);
            }
            else
            {
                pCardContain = CCSprite::createWithSpriteFrameName(CardBigRedBlack[3*(cbCardColor%2)+cbCardValue-11]);
            }
            if(pCardContain != NULL)
            {
                pCardContain -> setPosition(ccp(inCard->getContentSize().width/2+5, inCard->getContentSize().height/2-5));
                inCard -> addChild(pCardContain);
            }
        }
    }
    
    //m_openAnimIn = (CCActionInterval*)CCSequence::create(CCDelayTime::create(duration*0.5f), CCShow::create(), CCOrbitCamera::create(duration*0.5f, 1, 0, kInAngleZ, kInDeltaZ, 0, 0), NULL);
    m_openAnimIn = (CCActionInterval*)CCSequence::create(CCDelayTime::create(duration*0.5f), CCShow::create(), MyScaleTo::create(duration*2, 1.0f), NULL);
    m_openAnimIn -> retain();
    
    //m_openAnimOut = (CCActionInterval*)CCSequence::create(CCOrbitCamera::create(duration*0.5f, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0), CCHide::create(), CCDelayTime::create(duration*0.5f), NULL);
    m_openAnimOut = (CCActionInterval*)CCSequence::create(MyScaleTo::create(duration*0.5f, 0.2f), CCHide::create(), CCDelayTime::create(duration*2), NULL);
    m_openAnimOut -> retain();
}

void CardSprite::openCard()
{
    CCSprite* inCard = (CCSprite*)this->getChildByTag(tag_inCard);
    CCSprite* outCard = (CCSprite*)this->getChildByTag(tag_outCard);
    inCard -> runAction(m_openAnimIn);
    outCard -> runAction(m_openAnimOut);
}


