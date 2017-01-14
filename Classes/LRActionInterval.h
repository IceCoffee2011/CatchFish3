#ifndef _GAME_LRACTIONINTERVAL_H__
#define _GAME_LRACTIONINTERVAL_H__

#define PI 3.1415926

#pragma pack(1)
typedef struct _lrTuoyuanConfig {
     //中心点坐标
     CCPoint centerPosition;
     //椭圆a长，三角斜边
     float aLength;
     //椭圆c长，三角底边
     float cLength;
} lrTuoyuanConfig;
#pragma pack()

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
class LRTuoyuanBy : public CCActionInterval 
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
class  LRTuoyuanBy : public CCActionInterval
#endif

 
{  
public:  
	//用“动作持续时间”和“椭圆控制参数”初始化动作  
	bool initWithDuration(float t, const lrTuoyuanConfig& c);  
	virtual void update(float time);//利用update函数来不断的设定坐标  
public:  
	//用“动作持续时间”和“椭圆控制参数”创建动作  
	static LRTuoyuanBy *actionWithDuration(float t, const lrTuoyuanConfig& c); 

	static inline float tuoyuanXat(float a, float bx, float c, float t)
	{
		return -a*cos(2*PI*t)+a;
	}
	static inline float tuoyuanYat(float a, float by, float c, float t)
	{
		float b = sqrt(powf(a,2)-powf(c,2));
		return b*sin(2*PI*t);
	}

protected:  
    CCPoint m_startPosition;
    CCPoint s_startPosition;
	lrTuoyuanConfig m_sConfig;
};

#endif