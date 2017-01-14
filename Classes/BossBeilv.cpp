#include "header.h"

BossBeilv* BossBeilv::create(const char *string, const char *charMapFile, unsigned int itemWidth, int unsigned itemHeight, unsigned int startCharMap)
{
	BossBeilv *pRet = new BossBeilv();
	if(pRet && pRet->initWithString(string, charMapFile, itemWidth, itemHeight, startCharMap))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

BossBeilv::BossBeilv()
{
	unBossTag = 0;
}