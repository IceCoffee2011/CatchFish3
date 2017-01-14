#include "header.h"

BulletSprite::BulletSprite()
{}

BulletSprite::~BulletSprite()
{}

BulletSprite* BulletSprite::create(int type)
{
	BulletSprite* pRet = new BulletSprite();
	if(pRet && pRet->init(type))
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

//type 是子弹的类型，当type = 1时，为2个炮筒的子弹；当type = 3时，是三个炮筒的子弹；当type = 5时，是四个炮筒的子弹
//type=7是能量炮两个炮筒，type = 9是能量炮三个炮筒，type=11是能量炮四个炮筒的子弹
bool BulletSprite::init(int type)
{
	if(!CCSprite::init())
	{
		return false;
	}
	m_bHave = false;
	m_nType = type;
	bIsStop = false;
    nAttribute = 0;
	m_fSpeed = 17;
    nMultiple = 0;
    m_vctFishID.clear();
    char temp[20];
    sprintf(temp,"bullet_0%i.png",type);
    initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache() -> spriteFrameByName(temp));
    m_bHave = false;
    
    runAnimate();
    
    return true;
}

bool BulletSprite::getHave()
{
	return m_bHave;
}

void BulletSprite::setHave(bool have)
{
	m_bHave = have;
}

void BulletSprite::runAnimate()
{
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCArray* array = CCArray::create();
    char temp[20];
    for(int i = 0; i < 2; i++)
    {
        sprintf(temp,"bullet_0%i.png",m_nType+i);
        CCSpriteFrame* frame = cache -> spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array,0.5f);
    CCAnimate* animate = CCAnimate::create(animation);
    this -> runAction(CCRepeatForever::create(animate));
}

float BulletSprite::getBulletRotation()
{
	return m_fRotation;
}

void BulletSprite::setBulletRotation(float rotation)
{
	m_fRotation = rotation;
}

float BulletSprite::getBulletSpeed()
{
	return m_fSpeed;
}

int BulletSprite::getType()
{
	return m_nType;
}

void BulletSprite::setType(int type)
{
	m_nType = type;
}