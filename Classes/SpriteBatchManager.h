#ifndef _SPRITEBATCHMANAGER_H__
#define _SPRITEBATCHMANAGER_H__

class SpriteBatchManager : public CCObject
{
private:
	CCDictionary* m_pSpriteBatchs;
public:
	static SpriteBatchManager* sharedSpriteBatchManager();

	SpriteBatchManager();
	~SpriteBatchManager();
	CCSpriteBatchNode* addSpriteBatch(const char* fileName);
	CCSpriteBatchNode* spriteBatchForKey(const char* key);

	void removeAllSpriteBatch();
	void removeSpriteBatch(CCSpriteBatchNode* spriteBatchNode);
	void removeSpriteBatchForKey(const char* key);
};

#endif