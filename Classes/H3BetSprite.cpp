#include "header.h"

BetSprite::BetSprite()
{
}

BetSprite::~BetSprite()
{
    
}

BetSprite* BetSprite::create(SCORE nBet, int nChairID)
{
    BetSprite* pRet = new BetSprite();
    if(pRet && pRet->init(nBet, nChairID))
    {
        pRet -> autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        return NULL;
    }
}

bool BetSprite::init(SCORE nBet, int nChairID)
{
    if(!CCSprite::init())
    {
        return false;
    }
    
    m_nChairID = nChairID;
    if(nBet < 100)//紫色(10-50)
    {
        initWithSpriteFrameName("h3card_chip_chip_4.png");
        char tempBet[16];
        sprintf(tempBet, "%lld", nBet);
        CCLabelTTF* pLabel = CCLabelTTF::create(tempBet, "Arial", 22);
        pLabel -> setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
        addChild(pLabel);
        pLabel -> setColor(ccc3(0, 0, 0));
    }
    else if(nBet < 1000)//绿色（100-500）
    {
        initWithSpriteFrameName("h3card_chip_chip_1.png");
        char tempBet[16];
        sprintf(tempBet, "%lld", nBet);
        CCLabelTTF* pLabel = CCLabelTTF::create(tempBet, "Arial", 22);
        pLabel -> setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
        addChild(pLabel);
        pLabel -> setColor(ccc3(0, 0, 0));
    }
    else if(nBet < 10000)//黄色（1千-5千）
    {
        initWithSpriteFrameName("h3card_chip_chip_0.png");
        char tempBet[16];
        sprintf(tempBet, "%lld千", nBet/1000);
        CCLabelTTF* pLabel = CCLabelTTF::create(tempBet, "Arial", 22);
        pLabel -> setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
        addChild(pLabel);
        pLabel -> setColor(ccc3(0, 0, 0));
    }
    else if(nBet < 100000)//蓝色(1万-5万）
    {
        initWithSpriteFrameName("h3card_chip_chip_2.png");
        char tempBet[16];
        sprintf(tempBet, "%lld万", nBet/10000);
        CCLabelTTF* pLabel = CCLabelTTF::create(tempBet, "Arial", 22);
        pLabel -> setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
        addChild(pLabel);
        pLabel -> setColor(ccc3(0, 0, 0));
    }
    else if(nBet < 500000)//红色(10万）
    {
        initWithSpriteFrameName("h3card_chip_chip_3.png");
        char tempBet[16];
        sprintf(tempBet, "%lld万", nBet/10000);
        CCLabelTTF* pLabel = CCLabelTTF::create(tempBet, "Arial", 18);
        pLabel -> setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
        addChild(pLabel);
        pLabel -> setColor(ccc3(0, 0, 0));
    }
    else if(nBet < 1000000)//小长方形板注（50万）
    {
        int nRand = rand()%3;
        char temp[32];
        sprintf(temp, "h3card_chip_chip_%d.png", 50+nRand);
        initWithSpriteFrameName(temp);
        char tempBet[16];
        sprintf(tempBet, "%lld万", nBet/10000);
        CCLabelTTF* pLabel = CCLabelTTF::create(tempBet, "Arial", 22);
        pLabel -> setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
        addChild(pLabel);
        pLabel -> setColor(ccc3(119, 64, 28));
        if(nRand == 1)
        {
            pLabel -> setRotation(20);
        }
        else if(nRand == 2)
        {
            pLabel -> setRotation(-20);
        }
    }
    else//大长方形板注(100万）
    {
        int nRand = rand()%3;
        char temp[32];
        sprintf(temp, "h3card_chip_chip_%d.png", 60+nRand);
        initWithSpriteFrameName(temp);
        char tempBet[16];
        sprintf(tempBet, "%lld万", nBet/10000);
        CCLabelTTF* pLabel = CCLabelTTF::create(tempBet, "Arial", 22);
        pLabel -> setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
        addChild(pLabel);
        pLabel -> setColor(ccc3(119, 64, 28));
        if(nRand == 1)
        {
            pLabel -> setRotation(20);
        }
        else if(nRand == 2)
        {
            pLabel -> setRotation(-20);
        }
    }
    
    return true;
}


