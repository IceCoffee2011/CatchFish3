#include "header.h"

GameFish::GameFish(std::string firstName, std::string commonName, int frameCount, int loop, float duration,float width,float height,float x,float y, bool groupFish, std::vector<GameFishData*>vctFishData, std::string filePath, int fish, int diedAnimate, int spriteID,float lockScale, float fScale, int nFishScore)
{
	nFishID=0;
	nSpriteID=spriteID;
	sFirstName = firstName;
	sCommonName = commonName;
	sFilePath = filePath;
	sParticle = "";
	sParticle2 = "";
	nFrameCount = frameCount;
	nLoop = loop;
	fDuration = duration;
	nWidth = width;
	nHeight = height;
	fXPos = x;
	fYPos = y;
	fRotation = 0;
	nHitUser = 2;
	nMultiple = 0;
	nProperty = 0;
	nBossScore = 3;
	bHave = false;
	bDied = false;
	bAppear = false;
    bTryCannon = false;
	bGroupFish = groupFish;
	nBossTag = 0;
	nFish = fish;
	nDiedAnimate = diedAnimate;
	fLockScale = lockScale;
	vctGameFishData.clear();
	for (int i = 0; i < PATH_POINT_COUNT; i++)
	{
		nRotationArray[i] = 0;
		nMoveTimeArray[i] = 0;
		nSpeedArray[i] = 0;
	}
	if (!bGroupFish)
	{
		vctFishData.clear();
		m_pFish = CCSprite::createWithSpriteFrameName(sFirstName.c_str());
		CCCallFuncN* callFunc = CCCallFuncN::create(m_pFish, callfuncN_selector(GameFish::fishDieAppearCall));
        m_pFish->setScale(fScale);
        m_pFish->runAction(CCSequence::create(createAnimate(), callFunc, NULL));
        m_pFishShadow = CCSprite::createWithSpriteFrameName(sFirstName.c_str());
        m_pFishShadow->runAction(CCSequence::create(createAnimate(), callFunc, NULL));
        m_pFishShadow->setColor(ccc3(0, 0, 0));
        m_pFishShadow->setOpacity(60);
        m_pFishShadow->setPosition(ccp(m_pFishShadow->getContentSize().width*0.3, m_pFishShadow->getContentSize().height*0.3));
		m_pFish->addChild(m_pFishShadow, -1);
	}
	else
	{
//		for (unsigned int i = 0; i < vctFishData.size(); i++)
//		{
//			vctGameFishData.push_back(vctFishData[i]);
//		}
		m_pFish = CCSprite::createWithSpriteFrameName(sFirstName.c_str());
		m_pFish->setAnchorPoint(CCPointZero);
		m_pFish->setContentSize(CCSizeMake(nWidth, nHeight));
        
		for (unsigned int i = 0; i < vctFishData.size(); i++)
		{
			GameFish* tempFish = GameMainScene::GetInstance()->getGameFishByID(vctFishData[i]->getSpriteID());
			if (tempFish != NULL)
			{
				GameFish* subFish = new GameFish(tempFish->getFirstName(), tempFish->getCommonName(), tempFish->getFrameCount(), tempFish->getLoopTimes(), tempFish->getDuration(), tempFish->getWidth(), tempFish->getHeight(), tempFish->getXPos(), tempFish->getYPos(), tempFish->getGroupFish(), tempFish->vctGameFishData, tempFish->getFilePath(),tempFish->getFish(),tempFish->getDiedAnimate(),tempFish->getSpriteID(),tempFish->getLockScale(), tempFish->getScale(),tempFish->getFishScore());
				subFish->m_pFish->setZOrder(vctFishData[i]->getZorder());
				subFish->m_pFish->setPosition(ccp(vctFishData[i]->getPosX(), vctFishData[i]->getPosY()));
                subFish->m_pFish->setScale(fScale);
				subFish->m_pFish->setScale(vctFishData[i]->getSubScale());
                float fRotateTime = 1.0f;
                bool bChangeTime = false;
                bool bRotation = true;
                if(CMainLogic::sharedMainLogic()->KIND_ID == YQS_KIND_ID)
                {
                    if (subFish->getFish() == 0 && (subFish->getSpriteID() == 997 || subFish->getSpriteID() == 998 || subFish->getSpriteID() == 999))
                    {
                        bChangeTime = true;
                    }
                }
                else if(CMainLogic::sharedMainLogic()->KIND_ID == NZNH_KIND_ID)
                {
                    if (subFish->getFish() == 0 && subFish->getSpriteID() == 1999)
                    {
                        fRotateTime = 0.1f;
                    }
                    if(subFish->getFish() == 0 && (subFish->getSpriteID() == 997 || subFish->getSpriteID() == 1997 || subFish->getSpriteID() == 998 || subFish->getSpriteID() == 1998))
                    {
                        bRotation = false;
                    }
                }
                else
                {
                    if (subFish->getFish() == 0 && subFish->getSpriteID() == 998)
                    {
                        bChangeTime = true;
                    }
                    if (bChangeTime)
                    {
                        fRotateTime = 0.1f;
                    }
                }
                if(bRotation)
                {
                    CCRotateBy* rotateBy = CCRotateBy::create(fRotateTime, vctFishData[i]->getRotateBy());
                    subFish->m_pFish->runAction(CCRepeatForever::create(rotateBy));
                }
				
				if(subFish->getFish() == 1)//是鱼
				{
					subFish->m_pFish->setTag(100);
				}
				if(subFish->getFish() == 0)//是圆盘
				{
					subFish->m_pFish->setTag(101);
				}
				m_pFish->addChild(subFish->m_pFish);
				delete subFish;
				subFish = NULL;
			}
		}
		m_pFish->setAnchorPoint(ccp(0.5, 0.5));
		m_pFish->setOpacity(0);
		m_pFishShadow = CCSprite::create();
	}
}

CCAnimate* GameFish::getShijinbiFishAnimate(int nIndex)
{
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCArray* array = CCArray::create();
    char temp[32];
    for(int i = 1; i <= 31; i++)
    {
        if (nIndex == 1)
        {
            sprintf(temp, "shijinbi1fish%d.png", i);
        }
        else
        {
            sprintf(temp, "shijinbi2fish%d.png", i);
        }
        CCSpriteFrame* frame = cache->spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, 0.05f);
    animation->setLoops(-1);
    CCAnimate* animate = CCAnimate::create(animation);
    return animate;
}

GameFish::GameFish(std::string firstName, std::string commonName, int frameCount, int loop, float duration,float width,float height,float x,float y, bool groupFish, std::vector<GameFishData*>vctFishData, std::string filePath, int fish, int diedAnimate, int spriteID,float lockScale, bool bIsCache, int nFishScore)
{
    nFishID=0;
    nSpriteID=spriteID;
    sFirstName = firstName;
    sCommonName = commonName;
    sFilePath = filePath;
    sParticle = "";
    sParticle2 = "";
    nFrameCount = frameCount;
    nLoop = loop;
    fDuration = duration;
    nWidth = width;
    nHeight = height;
    fXPos = x;
    fYPos = y;
    fRotation = 0;
    nHitUser = 2;
    nMultiple = 0;
    nProperty = 0;
    nBossScore = 3;
    bHave = false;
    bDied = false;
    bAppear = false;
    bGroupFish = groupFish;
    nBossTag = 0;
    nFish = fish;
    nDiedAnimate = diedAnimate;
    fLockScale = lockScale;
    this->nFishScore = nFishScore;
    vctGameFishData.clear();
    for (int i = 0; i < PATH_POINT_COUNT; i++)
    {
        nRotationArray[i] = 0;
        nMoveTimeArray[i] = 0;
        nSpeedArray[i] = 0;
    }
    if (!bGroupFish)
    {
    }
    else
    {
        for (unsigned int i = 0; i < vctFishData.size(); i++)
        {
            vctGameFishData.push_back(vctFishData[i]);
        }
    }
}

GameFish::~GameFish()
{
    
}

CCAnimate* GameFish::createAnimate()
{
	char temp[10];
	sprintf(temp,"%d",nSpriteID);
	std::string name = sCommonName+temp;
	CCAnimation * pAnimation = CCAnimationCache::sharedAnimationCache()->animationByName(name.c_str());
	CCAnimate* animate = NULL;
	if (pAnimation == NULL)
	{
		CCAnimation* animation = CCAnimation::create();
		for (int i = 0; i < nFrameCount; i++)
		{
			char s[10];
			sprintf(s, "%d.png", i+1);
			std::string str = sCommonName+s;
			CCSpriteFrame* SpFrame = CCSpriteFrameCache::sharedSpriteFrameCache() -> spriteFrameByName(str.c_str());
			animation->addSpriteFrame(SpFrame);
		}
		animation->setDelayPerUnit(fDuration);
		animation->setLoops(nLoop);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation, name.c_str());
		animate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName(name.c_str()));
	}
	else
	{
		animate = CCAnimate::create(pAnimation);
	}
	return animate;
}

void GameFish::fishDieAppearCall(CCNode* pNode)
{
    if(pNode->getParent() != NULL)
    {
        pNode->removeFromParent();
    }
	
}
//设置旋转角度
void GameFish::setRotationArray(int nIndex, int nRotation)
{
	nRotationArray[nIndex] = nRotation;
}
//设置移动时间
void GameFish::setMoveTimeArray(int nIndex, int nMoveTime)
{
	nMoveTimeArray[nIndex] = nMoveTime;
}
//设置移动速度
void GameFish::setSpeedArray(int nIndex, int nSpeed)
{
	nSpeedArray[nIndex] = nSpeed;
}

void GameFish::initData(tagPathIndex* fishPathIndex)
{
	m_fishPath = fishPathIndex;
	nCurrentTime = 0;
	nPathIndex2 = 0;
	if(m_fishPath != NULL)
	{
		for (int i = 0; i < PATH_POINT_COUNT; i++)
		{
			nRotationArray[i] = m_fishPath->pPathIndex[i].rotation;
			nMoveTimeArray[i] = m_fishPath->pPathIndex[i].moveTime;
			nSpeedArray[i] = m_fishPath->pPathIndex[i].speed;
		}
	}
	nSpeed = nSpeedArray[0];
	fRotation = nRotationArray[0];
}

void GameFish::moveFish()
{
	float f = 3.0f;
	float m_x = nSpeed*sin(fRotation/(180/M_PI))/f;
	float m_y = nSpeed*cos(fRotation/(180/M_PI))/f;
	m_pFish -> setPosition(ccp(m_pFish -> getPositionX()+m_x,m_pFish -> getPositionY()+m_y));
	m_pFish -> setRotation(fRotation-90);
}

void GameFish::changePath()
{
	nCurrentTime++;
	if(nCurrentTime >= nMoveTimeArray[nPathIndex2] && nPathIndex2 < 5)
	{
		nCurrentTime = 0;
		nPathIndex2++;
		nSpeed = nSpeedArray[nPathIndex2];
	}
	if(fRotation != nRotationArray[nPathIndex2])
	{
		if(m_fishPath != NULL)
		{
			float fR = (nRotationArray[nPathIndex2] - fRotation)/50;
			fRotation+=fR;
		}
	}
}

std::vector<GameFishData*> GameFish::getGameFishData()
{
	return vctGameFishData;
}

bool GameFish::isProperty(enFishAttrib bPro)
{
	return (nProperty&bPro) > 0;
}

GameFishData::GameFishData()
{
	nSpriteID = 0;
	fRotate = 0.0f;
	fPosX = 0.0f;
	fPosY = 0.0f;
	nZorder = 0;
	fScale = 1.0f;
}

GameFishData::~GameFishData()
{}
