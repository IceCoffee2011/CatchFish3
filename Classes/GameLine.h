#ifndef _GAMELINE_H__
#define _GAMELINE_H__

#pragma once

class GameLine : public CCLayer
{
public:
	GameLine();
	~GameLine();
	CREATE_FUNC(GameLine);
	virtual bool init();
	void setLine(int wChairID, CCPoint ptBase, int nPtCount, CCPoint ptOther[], CMD_S_CaptureFish* pCaptureFish);
	void lineLose(CCNode* node);
	void sendFishDied(CCNode* node);
private:
	CCSpriteBatchNode* m_pBatchNode;
	CMD_S_CaptureFish CaptureFish;
	int chairID;
};

#endif