#ifndef _HELLOWORLD_MONEYLAYER_H__
#define _HELLOWORLD_MONEYLAYER_H__

class MoneyLayer : public CCLayer
{
public:
	MoneyLayer();
	~MoneyLayer();
	CREATE_FUNC(MoneyLayer);
	virtual bool init();
	void loadMoneySprite(CCNode* node);			             //钱堆增长动画
	void moneySpriteAnimate(int wChairID, int time,int count);			 //钱堆滚动动画
	void removeMoneyArray(CCNode* node);		             //移除钱堆
	bool getHave();
	void setHave(bool have);
	void addColorSprite();						             //增加颜色
	CC_SYNTHESIZE(CCSpriteBatchNode* , batchNode, BatchNode);
	CCLabelAtlas* getAtlasMoney();
private:
	CCLabelAtlas* m_labelAtlasMoney;					     //金币的多少
	int m_nPlateScore;								         //金币数量
	CCSprite* m_spMoneyColor;					             //金币的颜色
	CCSprite* m_spRollMoney;					             //滚动的金币	
	bool m_bHave;
	CCArray* m_arrMoney;							         //盛放钱堆的数组
};

#endif