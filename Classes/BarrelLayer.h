#ifndef __HELLOWORLD_BARRELLAYER_H__
#define __HELLOWORLD_BARRELLAYER_H__


class BarrelLayer : public cocos2d::CCLayer
{
public:
	BarrelLayer();
	~BarrelLayer();
	virtual bool init(int chairID);

	static BarrelLayer* create(int chairID);

	void loadUI();                    //选择炮台
    void setMoneyPoint(int realID, int wChairID);
    void setChatSpriteBg(int realID, int wChairID);
    
	void clickYuanPan(int nScore, int nFishID, bool bShowName = false);
	void loadPlateScore();
	void fadeOutPlate(float dt);			                 //圆盘消失

	void loadLock();

	void clickAutoFire(CCObject* object);			         //自动开炮

	void setbarrelRotation(float rotation);					 //改变炮筒的角度
	float getBarrelRotation();								 //获取炮筒的角度
	CCSprite* getCannonBg();						         //获取炮台
	CCSprite* getBarrel();							         //获取炮筒
	int getBarrelMult();							         //获取倍率
	CCLabelAtlas* getGamelastAtlas();				         //获取玩家积分板上的积分
	CCLabelAtlas* getMultAtlas();							 //获取倍率标签
	CCLabelTTF* getLabelName();								 //获取玩家姓名
	void lockRealFish(GameFish* gameFish);					//追踪锁定
	CCSprite* getLockBlockSprite();							//获取锁定框
    CCSprite* getSuperPaoSprite();                          //获取能量炮精灵
	int getLockFishID();									//获取锁定鱼ID
	void setLockFishID(int fishID);							//设置锁定鱼ID
	bool getUserLockFish();									//玩家是否在锁定鱼状态中
	void setUserLockFish(bool bLockFish);
	CCSprite* getLockNumSprite();							//获取玩家锁定鱼标识精灵
	CCSprite* getSpritePlate();							    //返回圆盘
	CCSprite* getScoreBlockSprite();
	void changeBeilvCall(CCObject* obj);
    void clickCannonCharge(CCObject* object);
    void showCannonBg();
    void loadLockLine(int nLine);                                    //初始化锁定线
    CCSprite* getLockLine();
    CCSprite* getMoneySprite();
    
    void setLabelIndex1Func();
    void setLabelIndex2Func();
    void setLabelIndex3Func();
    CCLabelAtlas* m_labelScore;     //玩家得分，只有自己显示
    CCLabelAtlas* m_label2Score;     //玩家得分，只有自己显示
    CCLabelAtlas* m_label3Score;     //玩家得分，只有自己显示
    int m_nLabelIndex;
    void setLabelScore(long nScore);
    int m_nLabelWidth;
    
    //切换能量炮
    void superMenu(int nBarrelNum, bool bSuperCannon);
    bool getSuperBarrel();							         //是否是能量炮
    
    //设置锁定线的位置
    void setLockLinePoint();
    void setLockLine(int nLine);
    
    void changeBarrel(int index);
    void setBarrel(int barrelMult);
    void playBarrelAnimate1();
    CCAnimate* playBarrelAnimate2();
    void cannonBg3Blink();
    void fishDiedScore(int wChairID, int fishScore,int fishMult);
    void moveMoneyLayer(int index);
    void moveMoneyLayer2(CCNode* node);				         //钱堆层移动
    
    void showBaoji(bool bShow);
    CCSprite* getBaojiSprite();
    CCSprite* getOneChatBg();
    void setChatDataName(std::string sName, std::string sData);
    void setShowOneChatBg(bool bShow);
    void hideChatBg();
    
    
    CC_SYNTHESIZE(int, m_nVipLevel, VipLevel);
private:
	CCLabelAtlas* m_labelAtlasGameLast;
    CCSprite* m_spBaoji;
	int m_nChairID;
	CCSprite* m_spCannonBg;			                        //炮台
	CCSprite* m_spCannonBg2;
	CCSprite* m_spBarrel;				                    //炮筒
    CCSprite* m_barrelLight;
	int m_nBarrelNum;				                        //炮筒倍率
	CCPoint m_ptStart;			
	CCPoint m_ptEnd;
    CCPoint m_barrelPoint;                                  //炮筒初始位置
	float m_fRotation;				                       //炮筒旋转角度

    CCSprite* m_spMoney;
	CCLabelAtlas* m_labelAtlasMult;		                   //倍率标签

    CCSprite* m_sp2PlateParents;							//圆盘基准点
	CCSprite* m_spPlate;		                           //圆盘精灵
    CCSprite* m_sp2Plate;
    CCSprite* m_sp3Plate;
    CCSprite* m_sp4Plate;
	CCLabelAtlas* m_labelAtlasPlateScore;		           //圆盘上的分数
	int m_nPlateScore;					                   //圆盘上的分数
	CCSprite* m_spScoreBlock;		                       //积分板
    MoneyLayer* m_moneyLayer[5];	                       //钱堆金币

    CCLabelTTF* m_labelTTFNickName;		                   //玩家姓名
	bool m_bAutoFire;				                       //自动开炮
	
	CCSprite* m_spLockBlock;								//锁定框
	bool m_bLock;											//是否锁定，控制按钮
	CCSprite* m_spLockNum;									//锁定数字精灵
	int m_nLockFishID;										//锁定鱼ID
    CCSprite* m_spLockLine;
    int m_count;
    int m_nCurrentBarrelBeilv;//当前炮倍率等级
    
    CCLabelTTF* m_pLabel;
    CCPoint money_point[5];
    
    bool m_bSuperBarrel;//能量炮
    CCSprite* m_spSuperPao;
    CCSprite* m_spOneChatBg;
    CCLabelTTF* m_labelChatData;
    CCLabelTTF* m_labelChatName;
};

#endif