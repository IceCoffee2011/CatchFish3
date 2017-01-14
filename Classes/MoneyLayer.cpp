#include "header.h"

MoneyLayer::MoneyLayer()
{
}

MoneyLayer::~MoneyLayer()
{
	m_arrMoney -> release();
}

bool MoneyLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	m_bHave = false;
	m_arrMoney = CCArray::create();
	m_arrMoney -> retain();

	m_labelAtlasMoney = CCLabelAtlas::create("","CatchFish01/goldNum3.png",12,24,'0');
	m_labelAtlasMoney -> setPosition(ccp(0,0));
	m_labelAtlasMoney -> setAnchorPoint(ccp(0.5,0.5));
	this -> addChild(m_labelAtlasMoney,1);
	
	return true;
}

void MoneyLayer::loadMoneySprite(CCNode* node)
{
	CCSprite* money = (CCSprite*)node;
    if(money != NULL)
    {
        CCSprite* sprite = CCSprite::createWithSpriteFrameName("qianbi.png");
        sprite -> setPosition(ccp(0,money->getPositionY()-3));
        batchNode->addChild(sprite);
        m_arrMoney -> addObject(sprite);
        
        m_labelAtlasMoney -> setPosition(ccp(0,money->getPositionY() + 9));
        m_spMoneyColor->setPosition(m_labelAtlasMoney->getPosition());
    }
	
}

bool MoneyLayer::getHave()
{
	return m_bHave;
}

void MoneyLayer::setHave(bool have)
{
	m_bHave = have;
}

void MoneyLayer::moneySpriteAnimate(int wChairID, int time,int count)
{
	int MeChairID = GameMainScene::GetInstance()->getMeRealChairID();
	m_nPlateScore = count;
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	m_spRollMoney= CCSprite::createWithSpriteFrame(cache -> spriteFrameByName("money_01.png"));
	m_spRollMoney -> setPosition(ccp(0,m_spRollMoney -> getContentSize().height/2 - 10));
	batchNode->addChild(m_spRollMoney);
	char temp[20];
	sprintf(temp,"%d",count);
	m_labelAtlasMoney = CCLabelAtlas::create(temp,"CatchFish01/goldNum3.png",12,24,'0');
	m_labelAtlasMoney -> setPosition(ccp(0,m_spRollMoney -> getContentSize().height/2 - 10));
	m_labelAtlasMoney -> setAnchorPoint(ccp(0.5,0.5));
	this -> addChild(m_labelAtlasMoney,1);
	if(MeChairID >= 0 && MeChairID < GAME_PLAYER/2 && wChairID >= GAME_PLAYER/2 && wChairID < GAME_PLAYER)
	{
		m_labelAtlasMoney->setRotation(180);
	}
	else if(MeChairID >= GAME_PLAYER/2 && MeChairID < GAME_PLAYER && wChairID >= 0 && wChairID < GAME_PLAYER/2)
	{
		m_labelAtlasMoney->setRotation(180);
	}
	m_spMoneyColor = CCSprite::createWithSpriteFrameName("SCbg0.png");
	m_spMoneyColor -> setVisible(false);
	batchNode->addChild(m_spMoneyColor);
	CCArray* array = CCArray::create();
	for(int i = 1; i <= 10; i++)
	{
		sprintf(temp,"money_0%i.png",i);
		CCSpriteFrame* frame = cache -> spriteFrameByName(temp);
		array -> addObject(frame);
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(array,0.01f);
	animation -> setRestoreOriginalFrame(true);
	CCAnimate* animate = CCAnimate::create(animation);
	CCMoveBy* moveBy = CCMoveBy::create(0.08f,ccp(0,4));
	CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(MoneyLayer::loadMoneySprite));
	CCSpawn* spawn = CCSpawn::create(animate,moveBy,NULL);
	CCSequence* seq = CCSequence::create(spawn,func,NULL);
	CCCallFunc* func3 = CCCallFunc::create(this,callfunc_selector(MoneyLayer::addColorSprite));
	m_spRollMoney -> runAction(CCSequence::create(CCRepeat::create(seq,time),CCHide::create(),func3,NULL));
}

void MoneyLayer::addColorSprite()
{
	m_spMoneyColor->setVisible(true);
// 	//计算数字宽度
	int nNumberCount=0;
	int nNumber = m_nPlateScore;
	for (int k=0; nNumber>0; nNumber=nNumber/10,k++)
		nNumberCount++;
	m_spMoneyColor -> setScaleX(nNumberCount);
}

CCLabelAtlas* MoneyLayer::getAtlasMoney()
{
	return m_labelAtlasMoney;
}