#include "header.h"

static SpriteBatchManager* g_sharedSpriteBatchManager = NULL;

SpriteBatchManager* SpriteBatchManager::sharedSpriteBatchManager()
{
	if(!g_sharedSpriteBatchManager)
	{
		g_sharedSpriteBatchManager = new SpriteBatchManager();
	}
	return g_sharedSpriteBatchManager;
}

SpriteBatchManager::SpriteBatchManager()
{
	m_pSpriteBatchs = new CCDictionary();
}

SpriteBatchManager::~SpriteBatchManager()
{
	CC_SAFE_RELEASE(m_pSpriteBatchs);
	g_sharedSpriteBatchManager = NULL;
}

CCSpriteBatchNode* SpriteBatchManager::addSpriteBatch(const char* fileName)
{
	CCSpriteBatchNode* spriteBatchNode = NULL;
	spriteBatchNode = (CCSpriteBatchNode*)m_pSpriteBatchs->objectForKey(fileName);
	if(!spriteBatchNode)
	{
		spriteBatchNode = CCSpriteBatchNode::create(fileName,1000);
		m_pSpriteBatchs -> setObject(spriteBatchNode,fileName);
	}
	return spriteBatchNode;
}

void SpriteBatchManager::removeAllSpriteBatch()
{
	m_pSpriteBatchs -> removeAllObjects();
}

void SpriteBatchManager::removeSpriteBatch(CCSpriteBatchNode* spriteBatchNode)
{
	if(!spriteBatchNode)
	{
		return;
	}
	CCArray* key = m_pSpriteBatchs->allKeysForObject(spriteBatchNode);
	m_pSpriteBatchs->removeObjectsForKeys(key);
}

CCSpriteBatchNode* SpriteBatchManager::spriteBatchForKey(const char* key)
{
	return (CCSpriteBatchNode*)m_pSpriteBatchs->objectForKey(key);
}