#include "header.h"

GameLine::GameLine()
{
	
}

GameLine::~GameLine()
{}

bool GameLine::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
    
    chairID = -1;
    m_pBatchNode = CCSpriteBatchNode::create("CatchFish01/fish6.png",100);
    this->addChild(m_pBatchNode, 1);

	return true;
}

void GameLine::setLine(int wChairID,CCPoint ptBase, int nPtCount, CCPoint ptOther[],CMD_S_CaptureFish* pCaptureFish)
{
    memset(&CaptureFish, 0, sizeof(CaptureFish));
	chairID = wChairID;
	CaptureFish.wChairID = pCaptureFish->wChairID;
	CaptureFish.bSuperCannon = pCaptureFish->bSuperCannon;
	CaptureFish.nUserScore = pCaptureFish->nUserScore;
	CaptureFish.nProperty = pCaptureFish->nProperty;
	CaptureFish.nCaptureCount = pCaptureFish->nCaptureCount;
	CaptureFish.nTotalMultiple = pCaptureFish->nTotalMultiple;
	CaptureFish.nTotalScore = pCaptureFish->nTotalScore;
    CaptureFish.nTryScore = pCaptureFish->nTryScore;
    CaptureFish.nLottteryCount = pCaptureFish->nLottteryCount;
    CaptureFish.bPlayParticle = pCaptureFish->bPlayParticle;
	memcpy(CaptureFish.nFishScore,pCaptureFish->nFishScore,sizeof(CaptureFish.nFishScore));
	memcpy(CaptureFish.nFishID,pCaptureFish->nFishID,sizeof(CaptureFish.nFishID));
	memcpy(CaptureFish.nFishMultiple,pCaptureFish->nFishMultiple,sizeof(CaptureFish.nFishMultiple));
	
	for(int i = 0; i < nPtCount; i++)
	{
		float f = sqrt((ptBase.x - ptOther[i].x)*(ptBase.x-ptOther[i].x)+(ptBase.y - ptOther[i].y)*(ptBase.y - ptOther[i].y));
		CCSprite* lineSprite = NULL;
		
		float xPos = ptOther[i].x - ptBase.x;
		float yPos = ptOther[i].y - ptBase.y;
		float rotation = 0;
		if(ptBase.x != ptOther[i].x)
		{
			rotation = atan2(yPos,xPos)*(180/M_PI);
		}
		else
		{
			rotation = 0;
		}
		//线帧动画
		CCArray* lineArray = CCArray::create();
		char temp[24];
		float fy = 0.0f;
		if(pCaptureFish->nProperty&enSameDie  || pCaptureFish->nProperty&enRangeBomb || pCaptureFish->nProperty&enBomb || pCaptureFish->nProperty&enLine)
		{
			lineSprite = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lianxian1_1.png"));
			lineSprite -> setAnchorPoint(ccp(0.0f,0.5f));
			for(int i = 1; i <= 5; i++)
			{
				sprintf(temp,"lianxian1_%d.png",i);
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
				lineArray -> addObject(frame);
			}
			fy = 1;
		}
//		else if(pCaptureFish->nProperty&enBomb || pCaptureFish->nProperty&enLine)
//		{
//			lineSprite = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("quanping01.png"));
//			lineSprite -> setAnchorPoint(ccp(0.0f,0.5f));
//			for(int i = 1; i <= 4; i++)
//			{
//				sprintf(temp,"quanping0%d.png",i);
//				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
//				lineArray -> addObject(frame);
//			}
//			fy = 1;
//		}
		if(lineSprite == NULL)
			return;
        lineSprite -> setScale(0);
		CCAnimation* animation = CCAnimation::createWithSpriteFrames(lineArray,0.05f);
		animation->setLoops(-1);
		CCAnimate* animate = CCAnimate::create(animation);
		lineSprite ->setRotation(-rotation);
		lineSprite -> setPosition(ptBase);
		m_pBatchNode->addChild(lineSprite);
		float scaleX = f/lineSprite->getContentSize().width;
		CCScaleTo* scaleTo0 = CCScaleTo::create(0.0f, 0.0f, fy);
		CCScaleTo* scaleTo1 = CCScaleTo::create(0.2f, scaleX,fy);
		CCScaleTo* scaleTo2 = CCScaleTo::create(0.2f, 0.0f, fy);
		CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(GameLine::lineLose));
		CCSequence* seq = CCSequence::create(scaleTo0, scaleTo1, CCDelayTime::create(1.0f), scaleTo2, func,NULL);
		CCSpawn* spawn = CCSpawn::create(seq, animate, NULL);
		lineSprite -> runAction(spawn);
	}
	CCCallFuncN* func2 = CCCallFuncN::create(this,callfuncN_selector(GameLine::sendFishDied));
	this -> runAction(CCSequence::create(CCDelayTime::create(1.5f),func2,NULL));
}

void GameLine::lineLose(CCNode* node)
{
	node -> removeFromParentAndCleanup(true);
}

void GameLine::sendFishDied(CCNode* node)
{
	GameMainScene::GetInstance()->showScoreInfo(chairID,&CaptureFish);
	this->removeFromParent();
	//delete this;
}