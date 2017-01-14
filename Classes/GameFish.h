#ifndef _GAMEFISH_H__
#define _GAMEFISH_H__

class GameFishData
{
public:
	GameFishData();
	~GameFishData();
	CC_SYNTHESIZE(int, nSpriteID, SpriteID);
	CC_SYNTHESIZE(float, fPosX, PosX);
	CC_SYNTHESIZE(float, fPosY, PosY);
	CC_SYNTHESIZE(int, nZorder, Zorder);
	CC_SYNTHESIZE(float, fRotate, RotateBy);
	CC_SYNTHESIZE(float, fScale, SubScale);
};

class GameFish
{
public:
	GameFish(std::string firstName, std::string commonName, int frameCount, int loop, float duration,float width,float height,float x,float y, bool groupFish, std::vector<GameFishData*>vctFishData, std::string filePath, int fish, int diedAnimate, int spriteID,float lockScale, float fScale, int nFishScore);
    GameFish(std::string firstName, std::string commonName, int frameCount, int loop, float duration,float width,float height,float x,float y, bool groupFish, std::vector<GameFishData*>vctFishData, std::string filePath, int fish, int diedAnimate, int spriteID,float lockScale, bool bIsCache, int nFishScore);
	~GameFish();
	CCAnimate* createAnimate();
    CC_SYNTHESIZE(float, fScale, Scale);
	CC_SYNTHESIZE(int, nSpriteID, SpriteID);                                        //精灵ID
	CC_SYNTHESIZE(int, nFishID, FishID);                                            //每条鱼特有的ID（tag）
	CC_SYNTHESIZE(int, nFrameCount, FrameCount);                                    //鱼的总帧数
	CC_SYNTHESIZE(int, nLoop, LoopTimes);                                           //每条鱼的循环次数
	CC_SYNTHESIZE(int, nBossTag, BossTag);											//BOSS鱼上倍率的tag值
	CC_SYNTHESIZE(float, fDuration, Duration);                                      //每帧的间隔
	CC_SYNTHESIZE(std::string, sFirstName, FirstName);                              //第一张图片名字
	CC_SYNTHESIZE(std::string, sCommonName, CommonName);                            //每条鱼的名称前缀
	CC_SYNTHESIZE(std::string, sFilePath, FilePath);                                //大图名称
	CC_SYNTHESIZE(int, nWidth, Width);                                              //鱼的宽度
	CC_SYNTHESIZE(int, nHeight, Height);                                            //鱼的高度
	CC_SYNTHESIZE(float, fXPos, XPos);                                              //检测碰撞时空白长度
	CC_SYNTHESIZE(float, fYPos, YPos);                                              //检测碰撞时空白高度
	CC_SYNTHESIZE(float, fRotation, FishRotation);                                  //鱼的当前角度
	CC_SYNTHESIZE(int, nSpeed, Speed);
	CC_SYNTHESIZE(int, nMultiple, Multiple);
	CC_SYNTHESIZE(bool, bHave, Have);
	CC_SYNTHESIZE(int, nProperty, PropertyOfFish);
	CC_SYNTHESIZE(int, nHitUser, HitUser);                                          //捕获玩家
	CC_SYNTHESIZE(int, nFish, Fish);												//fish=1为鱼，fish=0为圆盘
	CC_SYNTHESIZE(int, nDiedAnimate, DiedAnimate)	
	CC_SYNTHESIZE(bool, bGroupFish, GroupFish);                                     //是否是组合鱼
	CC_SYNTHESIZE(int, nBossScore, BossScore);										//boss分数
	CC_SYNTHESIZE(std::string, sParticle, MagParticle);
	CC_SYNTHESIZE(std::string, sParticle2, MagParticle2);
	CC_SYNTHESIZE(bool, bDied, Died);
	CC_SYNTHESIZE(bool, bAppear,Appear);											//剧情来鱼消失
	CC_SYNTHESIZE(float, fLockScale, LockScale);//锁定框上缩放范围
    CC_SYNTHESIZE(int, nFishScore, FishScore);
    CC_SYNTHESIZE(bool, bTryCannon, TryCannon);//是不是试用炮期间打死的鱼
	void fishDieAppearCall(CCNode* pNode);                                          //消失回调
	std::vector<GameFishData*>vctGameFishData;                                      //每条组合鱼包含的所有小鱼的容器
	void initData(tagPathIndex* fishPathIndex);
	void moveFish();
	void changePath();
	void setRotationArray(int nIndex, int nRotation);                               //设置旋转角度
	void setMoveTimeArray(int nIndex, int nMoveTime);                               //设置移动时间
	void setSpeedArray(int nIndex, int nSpeed);	                                    //设置移动速度
	int nRotationArray[PATH_POINT_COUNT];                                           //旋转角度
	int nMoveTimeArray[PATH_POINT_COUNT];                                           //移动时间
	int nSpeedArray[PATH_POINT_COUNT];                                              //移动速度
	bool isProperty(enFishAttrib nPro);												//鱼的附加属性
	std::vector<GameFishData*> getGameFishData();
	CCSprite* m_pFish;                                                              //创建鱼
	CCSprite* m_pFishShadow;
    CCAnimate* getShijinbiFishAnimate(int nIndex);
private:
	int nCurrentTime;
	int nPathIndex2;
	tagPathIndex* m_fishPath;
};

#endif    //_GAMEFISH_H__