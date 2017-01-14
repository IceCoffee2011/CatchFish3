#include "header.h"

GoldSprite::GoldSprite()
{}

GoldSprite::~GoldSprite()
{}

GoldSprite* GoldSprite::create()
{
	GoldSprite* pRet = new GoldSprite;
	if(pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		return NULL;
	}
}

bool GoldSprite::init()
{
	if(!CCSprite::init())
	{
		return false;
	}
	bHave = false;
	fRotation = 210.0f;
	nSpeed = 250;
	wChairID = 0;
	char temp[20];
	sprintf(temp,"gold_0_01.png");
	initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache() -> spriteFrameByName(temp));

	return true;
}
