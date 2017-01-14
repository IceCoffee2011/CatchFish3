#ifndef _HELLOWORLD_GOLDSPRITE_H__
#define _HELLOWORLD_GOLDSPRITE_H__

class GoldSprite : public CCSprite
{
public:
	GoldSprite();
	~GoldSprite();
	static GoldSprite* create();
	virtual bool init();
	CC_SYNTHESIZE(bool, bHave, Have);
	CC_SYNTHESIZE(float, fRotation, MoneyRotation);
	CC_SYNTHESIZE(int, nSpeed, Speed);
	CC_SYNTHESIZE(WORD, wChairID, ChairID);
};

#endif