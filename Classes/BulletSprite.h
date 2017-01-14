#ifndef _HELLOWORLD_BULLETSPRITE_H__
#define _HELLOWORLD_BULLETSPRITE_H__

class BulletSprite : public CCSprite
{
public:
	BulletSprite();
	~BulletSprite();
	static BulletSprite* create(int type);
	virtual bool init(int type);
	void setHave(bool have);
	bool getHave();
    void runAnimate();		//本身动画
	void setBulletRotation(float rotation);
	float getBulletRotation();
	float getBulletSpeed();				//获取子弹运动的速度
	int getType();						//获取子弹的类型
	void setType(int type);				//设置子弹的类型
    
	CC_SYNTHESIZE(bool, bIsStop, IsStop);  
	CC_SYNTHESIZE(bool, bSuper, Super);
	CC_SYNTHESIZE(bool, bAndroid, Android);  
	CC_SYNTHESIZE(int, nBulletID, BulletID);
	CC_SYNTHESIZE(int, nRealChairID, RealChairID);
	CC_SYNTHESIZE(int, nMultiple, Multiple);
    CC_SYNTHESIZE(int, nAttribute, Attribute);
    std::vector<int> m_vctFishID;
private:
	float m_fRotation;
	bool m_bHave;
	int m_nType;
	float m_fSpeed;			//子弹速度
};

#endif