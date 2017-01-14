#ifndef _GAMEMAINSCENE_BOSSBEILV_H__
#define _GAMEMAINSCENE_BOSSBEILV_H__

class BossBeilv : public CCLabelAtlas
{
public:
	BossBeilv();
	static BossBeilv * create(const char *string, const char *charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap);
	CC_SYNTHESIZE(unsigned int, unBossTag, BossTag);
};


#endif