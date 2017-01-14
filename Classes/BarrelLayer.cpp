#include "header.h"

enum
{
	BARREL = 100,
	CANNONBG3,
	PLATESCORE,
};

enum
{
    xiaopao=1,
};

const char* ZhuanPanName[13] = {"CatchFish2001/fishscorename1.png", "CatchFish2001/fishscorename2.png", "CatchFish2001/fishscorename3.png", "CatchFish2001/fishscorename4.png", "CatchFish2001/fishscorename5.png", "CatchFish2001/fishscorename6.png", "CatchFish2001/fishscorename7.png", "CatchFish2001/fishscorename8.png", "CatchFish2001/fishscorename9.png", "CatchFish2001/fishscorename10.png", "CatchFish2001/fishscorename11.png", "CatchFish2001/fishscorename12.png", "CatchFish2001/fishscorename13.png"};

BarrelLayer::BarrelLayer()
{}

BarrelLayer::~BarrelLayer()
{
}

BarrelLayer* BarrelLayer::create(int chairID)
{
	BarrelLayer *pRet = new BarrelLayer();
	if (pRet && pRet->init(chairID))
	{ 
		pRet->autorelease();
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	} 
}

void BarrelLayer::setMoneyPoint(int realID, int wChairID)
{
    if(realID == 0)
    {
        if(wChairID == 0)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(210+i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
        else if(wChairID == 1)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(50-i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
        else if(wChairID == 2)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(120+i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
        else if(wChairID == 3)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(50-i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
    }
    else if(realID == 1)
    {
        if(wChairID == 0)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(120+i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
        else if(wChairID == 1)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(-30-i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
        else if(wChairID == 2)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(120+i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
        else if(wChairID == 3)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(50-i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
    }
    else if(realID == 2)
    {
        if(wChairID == 0)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(120+i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
        else if(wChairID == 1)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(50-i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
        else if(wChairID == 2)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(210+i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
        else if(wChairID == 3)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(50-i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
    }
    else if(realID == 3)
    {
        if(wChairID == 0)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(120+i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
        else if(wChairID == 1)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(50-i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
        else if(wChairID == 2)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(120+i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
        else if(wChairID == 3)
        {
            for(int i = 0; i < 5; i++)
            {
                money_point[i] = ccp(-30-i*30,0);
                m_moneyLayer[i] -> setPosition(money_point[i]);
            }
        }
    }
    
}

void BarrelLayer::setChatSpriteBg(int realID, int wChairID)
{
    if(realID == 0 || realID == 1)
    {
        if(m_nChairID == 0)
        {
            m_spOneChatBg -> setPosition(ccp(0, 200));
            m_spOneChatBg -> setRotation(180);
            m_labelChatData -> setPosition(ccp(-200, 250));
            m_labelChatName -> setPosition(ccp(-200, 250));
        }
        else if(m_nChairID == 1)
        {
            m_spOneChatBg -> setPosition(ccp(m_spCannonBg->getContentSize().width, 200));
            m_spOneChatBg -> setFlipY(true);
            m_labelChatData -> setPosition(ccp(-25, 250));
            m_labelChatName -> setPosition(ccp(-25, 250));
        }
        else if(m_nChairID == 2)
        {
            m_spOneChatBg -> setPosition(ccp(0, 200));
            m_spOneChatBg -> setRotation(180);
            m_labelChatData -> setRotation(180);
            m_labelChatName -> setRotation(180);
            m_labelChatData -> setPosition(ccp(200, 170));
            m_labelChatName -> setPosition(ccp(200, 170));
        }
        else if(m_nChairID == 3)
        {
            m_spOneChatBg -> setPosition(ccp(m_spCannonBg->getContentSize().width, 200));
            m_spOneChatBg -> setFlipY(true);
            m_labelChatData -> setRotation(180);
            m_labelChatName -> setRotation(180);
            m_labelChatData -> setPosition(ccp(375, 170));
            m_labelChatName -> setPosition(ccp(375, 170));
        }
    }
    else
    {
        if(m_nChairID == 0)
        {
            m_spOneChatBg -> setPosition(ccp(0, 200));
            m_spOneChatBg -> setRotation(180);
            m_labelChatData -> setRotation(180);
            m_labelChatName -> setRotation(180);
            m_labelChatData -> setPosition(ccp(200, 170));
            m_labelChatName -> setPosition(ccp(200, 170));
        }
        else if(m_nChairID == 1)
        {
            m_spOneChatBg -> setPosition(ccp(m_spCannonBg->getContentSize().width, 200));
            m_spOneChatBg -> setFlipY(true);
            m_labelChatData -> setRotation(180);
            m_labelChatName -> setRotation(180);
            m_labelChatData -> setPosition(ccp(375, 170));
            m_labelChatName -> setPosition(ccp(375, 170));
        }
        else if(m_nChairID == 2)
        {
            m_spOneChatBg -> setPosition(ccp(0, 200));
            m_spOneChatBg -> setRotation(180);
            m_labelChatData -> setPosition(ccp(-200, 250));
            m_labelChatName -> setPosition(ccp(-200, 250));
        }
        else if(m_nChairID == 3)
        {
            m_spOneChatBg -> setPosition(ccp(m_spCannonBg->getContentSize().width, 200));
            m_spOneChatBg -> setFlipY(true);
            m_labelChatData -> setPosition(ccp(-25, 250));
            m_labelChatName -> setPosition(ccp(-25, 250));
        }
    }
}

bool BarrelLayer::init(int chairID)
{
	if ( !CCLayer::init() )
	{
		return false;
	}
	CCSize winSize = CCDirector::sharedDirector() -> getWinSize();
	m_nBarrelNum = 2;
	m_nChairID = chairID;
	m_fRotation = 0;
	m_bAutoFire = false;
	m_nPlateScore = 0;
    m_nCurrentBarrelBeilv = 2;
	m_bLock = false;
    m_bSuperBarrel = false;
    m_nVipLevel = 0;
    m_count = 0;
    for(int i = 0; i < 5; i++)
    {
        //money_point[i] = ccp(50-i*30,0);//50, 20, -10, -40, -70
        money_point[i] = ccp(280-70+i*30,0);//-70, -40, -10, 20, 50
    }
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    m_spCannonBg = CCSprite::createWithSpriteFrameName("CannonBg1.png");
	m_spCannonBg -> setPosition(ccp(winSize.width/2,m_spCannonBg->getContentSize().height/2));
	this ->addChild(m_spCannonBg);
    
    m_spCannonBg2 = CCSprite::createWithSpriteFrameName("CannonBg2.png");
    m_spCannonBg2 -> setPosition(ccp(m_spCannonBg->getContentSize().width/2,m_spCannonBg2->getContentSize().height/2));
    m_spCannonBg -> addChild(m_spCannonBg2,2);
    CCSprite* cannonBg3 = CCSprite::createWithSpriteFrameName("CannonBg3.png");
    cannonBg3 -> setPosition(ccp(m_spCannonBg->getContentSize().width/2, 0));
    cannonBg3 -> setTag(CANNONBG3);
    cannonBg3 -> setAnchorPoint(ccp(0.5f, 0));
    cannonBg3 -> setVisible(false);
    m_spCannonBg -> addChild(cannonBg3);
    
    m_spBaoji = CCSprite::create("CatchFish2006/gamebaoji.png");
    m_spBaoji -> setPosition(ccp(m_spCannonBg->getContentSize().width/2, m_spCannonBg->getContentSize().height+140));
    m_spCannonBg -> addChild(m_spBaoji, 20);
    m_spBaoji -> setVisible(false);
    
    //能量炮
    m_spSuperPao = CCSprite::create("CatchFish2006/superpaoui.png");
    m_spSuperPao -> setVisible(false);
    m_spCannonBg -> addChild(m_spSuperPao, 50);
    lrTuoyuanConfig config;
    config.aLength = 12;
    config.cLength = 0;
    config.centerPosition = ccp(m_spCannonBg->getContentSize().width+100,m_spCannonBg->getContentSize().height+45);
    if(m_nChairID == 0)
    {
        config.centerPosition = ccp(m_spCannonBg->getContentSize().width -200,m_spCannonBg->getContentSize().height+45);
    }
    else if(m_nChairID == 1)
    {
        config.centerPosition = ccp(m_spCannonBg->getContentSize().width+20,m_spCannonBg->getContentSize().height+45);
    }
    else if(m_nChairID == 2)
    {
        config.centerPosition = ccp(m_spCannonBg->getContentSize().width -200,m_spCannonBg->getContentSize().height+45);
    }
    else if(m_nChairID == 3)
    {
        config.centerPosition = ccp(m_spCannonBg->getContentSize().width +20,m_spCannonBg->getContentSize().height+45);
    }
    m_spSuperPao->runAction(CCRepeatForever::create(LRTuoyuanBy::actionWithDuration(2.0f,config)));
    
    //倍率标签
    m_labelAtlasMult = CCLabelAtlas::create("10","CatchFish2006/goldNum2.png",28,36,'0');
    m_labelAtlasMult->setAnchorPoint(ccp(0.5, 0.5));
    m_labelAtlasMult -> setPosition(ccp(m_spCannonBg2->getContentSize().width/2,m_spCannonBg2->getContentSize().height/2));
    m_spCannonBg2 -> addChild(m_labelAtlasMult);
    m_labelAtlasMult -> setScale(0.5f);
    
    //聊天背景图
    m_spOneChatBg = CCSprite::create("CatchFish2006/sendchatbg.png");
    m_spCannonBg -> addChild(m_spOneChatBg, 1);
    m_spOneChatBg -> setVisible(false);
    CCSize size(400,96);
    m_labelChatData = CCLabelTTF::create("", "Arial-BoldMT", 24, size, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    m_labelChatData -> setAnchorPoint(ccp(0, 1));
    m_spCannonBg -> addChild(m_labelChatData, 2);
    m_labelChatName = CCLabelTTF::create("", "Arial-BoldMT", 24, size, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    m_labelChatName -> setAnchorPoint(ccp(0, 1));
    m_spCannonBg -> addChild(m_labelChatName, 3);
    m_labelChatName -> setColor(ccc3(255, 0, 0));
    
    
    //玩家姓名
    m_labelTTFNickName = CCLabelTTF::create("w111111","Arial-BoldMT", 24);
    m_labelTTFNickName->setAnchorPoint(ccp(0, 0.5));
    m_labelTTFNickName->setScale(1.2f);
    m_labelTTFNickName -> setPosition(ccp(m_spCannonBg->getContentSize().width+20, m_spCannonBg->getContentSize().height/3 - 10));
    m_spCannonBg -> addChild(m_labelTTFNickName);
    
    m_spBarrel = CCSprite::createWithSpriteFrame(cache ->spriteFrameByName("Barrel2_01.png"));
    m_barrelLight = CCSprite::createWithSpriteFrame(cache ->spriteFrameByName("Barrel12_01.png"));
    
	loadUI();
	loadPlateScore();//初始化圆盘

	//加载积分板
    char temp[40];
    sprintf(temp,"shotNumBack_0%i.png",m_nChairID+1);
//    sprintf(temp,"shotNumBack_06.png");
    m_spScoreBlock = CCSprite::createWithSpriteFrameName(temp);
    m_spScoreBlock -> setPosition(ccp(m_spCannonBg->boundingBox().size.width + m_spScoreBlock->getContentSize().width/2+20,m_spScoreBlock->getContentSize().height/3*2+25));
    m_spCannonBg -> addChild(m_spScoreBlock,2);
//    CCSprite* pScoreBlock2 = CCSprite::createWithSpriteFrameName("shotNumBack_05.png");
//    pScoreBlock2 -> setPosition(ccp(m_spScoreBlock->getContentSize().width/2, m_spScoreBlock->getContentSize().height/2));
//    m_spScoreBlock -> addChild(pScoreBlock2);
//    pScoreBlock2 -> setTag(199);
//    pScoreBlock2 -> setVisible(false);
    m_spMoney = CCSprite::createWithSpriteFrameName("money2.png");
    m_spMoney -> setPosition(ccp(16,m_spScoreBlock->getContentSize().height/2));
    m_spScoreBlock->addChild(m_spMoney);
    m_spMoney -> setVisible(false);
    
    m_labelAtlasGameLast = CCLabelAtlas::create("0","CatchFish2006/goldNum0.png",23,39,'0');
    m_labelAtlasGameLast->setAnchorPoint(ccp(1, 0.5));
    m_labelAtlasGameLast -> setPosition(ccp(m_spScoreBlock->getContentSize().width-15, m_spScoreBlock->getContentSize().height/2));
    m_spScoreBlock -> addChild(m_labelAtlasGameLast);
    m_labelAtlasGameLast -> setScale(1.0f);
    
    for(int i =0; i < 5; i++)
    {
        CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create("CatchFish01/fish8.png");
        m_moneyLayer[i] = MoneyLayer::create();
        m_moneyLayer[i] -> setPosition(money_point[i]);
        m_moneyLayer[i]->addChild(batchNode);
        m_moneyLayer[i]->setBatchNode(batchNode);
        m_spCannonBg ->addChild(m_moneyLayer[i],3);
    }
    
    if(m_nChairID == 0)
    {
        m_nLabelWidth = m_spScoreBlock->getContentSize().width/2+20;
    }
    else if(m_nChairID == 1)
    {
        m_nLabelWidth = m_spScoreBlock->getContentSize().width/2-50;
    }
    else if(m_nChairID == 2)
    {
        m_nLabelWidth = m_spScoreBlock->getContentSize().width/2+20;
    }
    else if(m_nChairID == 3)
    {
        m_nLabelWidth = m_spScoreBlock->getContentSize().width/2-50;
    }
    
    m_nLabelIndex = 0;
    m_labelScore = CCLabelAtlas::create("", "CatchFish2001/moneyjianum.png", 36, 44, '0');
    m_labelScore -> setPosition(ccp(m_nLabelWidth, 100));
    m_labelScore -> setAnchorPoint(ccp(0.5f, 0.5f));
    m_spScoreBlock -> addChild(m_labelScore, 1);
    m_label2Score = CCLabelAtlas::create("", "CatchFish2001/moneyjianum.png", 36, 44, '0');
    m_label2Score -> setPosition(ccp(m_nLabelWidth, 100));
    m_label2Score -> setAnchorPoint(ccp(0.5f, 0.5f));
    m_spScoreBlock -> addChild(m_label2Score, 1);
 
    m_label3Score = CCLabelAtlas::create("", "CatchFish2001/moneyjianum.png", 36, 44, '0');
    m_label3Score -> setPosition(ccp(m_nLabelWidth, 100));
    m_label3Score -> setAnchorPoint(ccp(0.5f, 0.5f));
    m_spScoreBlock -> addChild(m_label3Score, 1);
    //加减炮按钮
    CCSprite* minusSp0 = CCSprite::create("CatchFish2006/CannonJianItem0.png");
    CCSprite* minusSp1 = CCSprite::create("CatchFish2006/CannonJianItem0.png");
    CCSprite* plusSp0 = CCSprite::create("CatchFish2006/CannonJiaItem0.png");
    CCSprite* plusSp1 = CCSprite::create("CatchFish2006/CannonJiaItem0.png");
    CCMenuItemSprite* minusItem = CCMenuItemSprite::create(minusSp0, minusSp1, this, menu_selector(BarrelLayer::changeBeilvCall));
    minusItem->setTag(312);
    CCMenuItemSprite* plusItem = CCMenuItemSprite::create(plusSp0, plusSp1, this, menu_selector(BarrelLayer::changeBeilvCall));
    plusItem->setTag(311);
    minusItem->setPosition(ccp(m_spCannonBg->getContentSize().width/12,m_spCannonBg2->getContentSize().height/3*2-10));
    plusItem->setPosition(ccp(m_spCannonBg->getContentSize().width*11/12,m_spCannonBg2->getContentSize().height/3*2-10));
    
    CCMenu* pMenu = CCMenu::create(minusItem, plusItem, NULL);
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    pMenu->setTag(226);
    pMenu->setVisible(false);
    m_spCannonBg->addChild(pMenu, 26);
    if(CMainLogic::sharedMainLogic()->KIND_ID == FB_KIND_ID)
    {
        pMenu->setTouchEnabled(false);
    }
    GameMainScene::GetInstance()->getMenuArray()->addObject(pMenu);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("CatchFish01/line.plist");
    
	loadLock();
    
    loadLockLine(1);
    
    this->scheduleUpdate();
	return true;
}

void BarrelLayer::setLabelIndex1Func()
{
    if(m_nLabelIndex>0)
    {
        m_nLabelIndex--;
    }
    m_labelScore -> setPosition(ccp(m_nLabelWidth, 100));
}

void BarrelLayer::setLabelIndex2Func()
{
    if(m_nLabelIndex>0)
    {
        m_nLabelIndex--;
    }
    m_label2Score -> setPosition(ccp(m_nLabelWidth, 100));
}

void BarrelLayer::setLabelIndex3Func()
{
    if(m_nLabelIndex>0)
    {
        m_nLabelIndex--;
    }
    m_label3Score -> setPosition(ccp(m_nLabelWidth, 100));
}

void BarrelLayer::setLabelScore(long nScore)
{
    if(m_nLabelIndex == 0)
    {
        m_labelScore->stopAllActions();
        char temp[16];
        sprintf(temp, ":%ld", nScore);
        m_labelScore->setString(temp);
        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(BarrelLayer::setLabelIndex1Func));
        m_labelScore -> runAction(CCSequence::create(CCFadeIn::create(0.2f), CCDelayTime::create(1.0f), CCFadeOut::create(0.5f), func, NULL));
        m_nLabelIndex = 1;
    }
    else if(m_nLabelIndex == 1)
    {
        m_labelScore -> setPosition(ccp(m_nLabelWidth, 140));
        m_label2Score->stopAllActions();
        char temp[16];
        sprintf(temp, ":%ld", nScore);
        m_label2Score->setString(temp);
        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(BarrelLayer::setLabelIndex2Func));
        m_label2Score -> runAction(CCSequence::create(CCFadeIn::create(0.2f), CCDelayTime::create(1.0f), CCFadeOut::create(0.5f), func, NULL));
        m_nLabelIndex = 2;
    }
    else if(m_nLabelIndex == 2)
    {
        m_labelScore -> setPosition(ccp(m_nLabelWidth, 180));
        m_label2Score -> setPosition(ccp(m_nLabelWidth, 140));
        m_label3Score->stopAllActions();
        char temp[16];
        sprintf(temp, ":%ld", nScore);
        m_label3Score->setString(temp);
        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(BarrelLayer::setLabelIndex3Func));
        m_label3Score -> runAction(CCSequence::create(CCFadeIn::create(0.2f), CCDelayTime::create(1.0f), CCFadeOut::create(0.5f), func, NULL));
        m_nLabelIndex = 0;
    }
    
}

void BarrelLayer::showCannonBg()
{
    m_spCannonBg -> setOpacity(255);
}

void BarrelLayer::clickCannonCharge(CCObject* object)
{
    if(CMainLogic::sharedMainLogic()->KIND_ID == FB_KIND_ID)
    {
        return ;
    }
    GameMainScene::GetInstance()->itemCallBack(object);
}

void BarrelLayer::changeBeilvCall(CCObject* obj)
{
    CCMenuItem* item = (CCMenuItem*)obj;
    CCMenu* menu = (CCMenu*)m_spCannonBg->getChildByTag(226);
    if(menu != NULL)
    {
        CCArray* array = menu->getChildren();
        for(unsigned int i = 0; i < array->count(); i++)
        {
            CCMenuItem* pItem = (CCMenuItem*)array->objectAtIndex(i);
            if(pItem->getTag() == item->getTag())
            {
                item -> setScale(1.0f);
                item->stopAllActions();
                item->runAction(CCSequence::create(CCScaleTo::create(0.1f, 1.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
                break;
            }
        }
    }
    if(m_spBarrel != NULL)
    {
        m_spBarrel -> runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.4f), CCScaleTo::create(0.15f, 1.0f), NULL));
    }
	GameMainScene::GetInstance()->itemCallBack(obj);
}

void BarrelLayer::loadLock()
{
	m_spLockBlock = CCSprite::create("CatchFish2001/lock.png");
	m_spLockBlock -> setPosition(ccp(m_spCannonBg->getContentSize().width +70,m_spCannonBg->getContentSize().height+50));
	m_spLockBlock -> setVisible(false);
	m_spCannonBg -> addChild(m_spLockBlock, 50);

	char temp[20];
	sprintf(temp,"lock_flag_0%i.png",m_nChairID+1);
    m_spLockNum = CCSprite::createWithSpriteFrameName("suodingsuror.png");
	m_spLockNum -> setVisible(false);
	m_spCannonBg -> addChild(m_spLockNum,10);
}

//追踪锁定
void BarrelLayer::lockRealFish(GameFish* gameFish)
{
	if(gameFish == NULL)  return;
	m_spLockBlock -> setPosition(ccp(m_spCannonBg->getContentSize().width +100,m_spCannonBg->getContentSize().height+45));
	m_spLockBlock -> setVisible(true);
	m_spLockBlock -> stopAllActions();
	m_spLockBlock -> removeAllChildren();
    lrTuoyuanConfig config;
    config.aLength = 12;
    config.cLength = 0;
    if(m_nChairID == 0)
    {
        config.centerPosition = ccp(m_spCannonBg->getContentSize().width+70,m_spCannonBg->getContentSize().height+45);
    }
    else if(m_nChairID == 1)
    {
        config.centerPosition = ccp(-40,m_spCannonBg->getContentSize().height+45);
    }
    else if(m_nChairID == 2)
    {
        config.centerPosition = ccp(m_spCannonBg->getContentSize().width+70,m_spCannonBg->getContentSize().height+45);
    }
    else if(m_nChairID == 3)
    {
        config.centerPosition = ccp(-40,m_spCannonBg->getContentSize().height+45);
    }
    
    m_spLockBlock->runAction(CCRepeatForever::create(LRTuoyuanBy::actionWithDuration(2.0f,config)));
  
    GameFish* pGameFish = GameMainScene::GetInstance()->createFishByID(gameFish->getSpriteID());
	if(pGameFish == NULL)  return;
	m_spLockBlock -> addChild(pGameFish->m_pFish);
	pGameFish->m_pFish -> setPosition(ccp(m_spLockBlock->getContentSize().width/2,m_spLockBlock->getContentSize().height/2));
	pGameFish->m_pFish -> setScale(pGameFish -> getLockScale());
	if(gameFish->isProperty(enLine))
	{
		pGameFish->m_pFish->setColor(ccc3(255,0,0));
	}
	pGameFish->m_pFish -> setRotation(-90);

    if(gameFish->getSpriteID() >= 1019 && gameFish->getSpriteID() <=1028)
    {
        pGameFish->m_pFish->setRotation(0);
    }
	
	delete pGameFish;
	pGameFish = NULL;
}

//改变炮筒
void BarrelLayer::changeBarrel(int index)
{
    setBarrel(index);
}

void BarrelLayer::setBarrel(int barrelMult)
{
    m_spCannonBg -> removeChild(m_spBarrel,true);
    char cBarrel[30];
    char cBarrelLight[30];
    if(!m_bSuperBarrel)
    {
        sprintf(cBarrel, "Barrel%d_01.png", barrelMult);
        sprintf(cBarrelLight,"Barrel1%d_01.png",barrelMult);
    }
    else
    {
        sprintf(cBarrel, "SuperBarrel%d_01.png", barrelMult);
        sprintf(cBarrelLight,"Barrel1%d_01.png",barrelMult);
    }
    
    m_spBarrel = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(cBarrel));
    m_barrelLight = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(cBarrelLight));
    m_nBarrelNum = barrelMult;
    m_barrelLight->setVisible(false);
    m_spBarrel -> runAction(CCFadeIn::create(0.3f));
    loadUI();
}

//能量炮和普通炮之间的切换
void BarrelLayer::superMenu(int nBarrelNum, bool bSuperCannon)//2,3
{
    m_spBarrel -> runAction(CCFadeOut::create(0.5));
    m_spCannonBg -> removeChild(m_spBarrel,true);
    m_bSuperBarrel = bSuperCannon;
    setBarrel(nBarrelNum);
}

//获取是否是能量炮
bool BarrelLayer::getSuperBarrel()
{
    return m_bSuperBarrel;
}

void BarrelLayer::setbarrelRotation(float rotation)
{
    m_fRotation = rotation;
}
float BarrelLayer::getBarrelRotation()
{
    return m_fRotation;
}

void BarrelLayer::loadUI()
{
    m_spBarrel -> setPosition(ccp(m_spCannonBg -> getContentSize().width/2,m_spCannonBg->getContentSize().height/3+10));
    if(CMainLogic::sharedMainLogic()->KIND_ID == 2002)	//倚天屠龙或西游记
    {
        m_spBarrel -> setPosition(ccp(m_spCannonBg -> getContentSize().width/2,m_spCannonBg->getContentSize().height/3+5));
    }
    m_barrelPoint = ccp(m_spBarrel->getPositionX(),m_spBarrel->getPositionY());
    m_spBarrel -> setAnchorPoint(ccp(0.5,0.2));
    m_spBarrel -> setRotation(m_fRotation);
    m_spCannonBg -> addChild(m_spBarrel,1);
    m_barrelLight->setPosition(ccp(m_spBarrel->getContentSize().width/2,m_spBarrel->getContentSize().height/2+12));
    m_barrelLight->setAnchorPoint(ccp(0.5,0));
    m_spBarrel->addChild(m_barrelLight);
    m_barrelLight->runAction(CCHide::create());
}

void BarrelLayer::playBarrelAnimate1()
{
    m_spBarrel->stopActionByTag(100);
    m_spBarrel->setPosition(m_barrelPoint);
    CCMoveBy* moveBy1 = CCMoveBy::create(0.05f,ccp(-14*sin(m_fRotation/(180/M_PI)),-14*cos(m_fRotation/(180/M_PI))));
    CCMoveTo* moveTo1 = CCMoveTo::create(0.05f,m_barrelPoint);
    CCSequence* seq = CCSequence::create(moveBy1,moveTo1,NULL);
    seq -> setTag(100);
    m_spBarrel->runAction(seq);
    m_barrelLight->runAction(CCSequence::create(CCShow::create(),playBarrelAnimate2(),CCHide::create(),NULL));
}

CCAnimate* BarrelLayer::playBarrelAnimate2()
{
    char temp[30];
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCArray* array = CCArray::create();
    int size = 0;
    size = 9;
    for(int i = 1; i < size; i++)
    {
        if(!m_bSuperBarrel)
        {
            sprintf(temp,"Barrel1%d_0%i.png",m_nBarrelNum,i);
        }
        else
        {
            //sprintf(temp,"SuperBarrel1%d_0%i.png",m_nBarrelNum,i);
            sprintf(temp,"Barrel1%d_0%i.png",m_nBarrelNum,i);
        }
        
        CCSpriteFrame* frame = cache -> spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array,0.05f);
    animation -> setRestoreOriginalFrame(true);
    CCAnimate* animate = CCAnimate::create(animation);
    return animate;
}

//底座闪烁动画
void BarrelLayer::cannonBg3Blink()
{
    CCSprite* sprite = (CCSprite*)m_spCannonBg -> getChildByTag(CANNONBG3);
    sprite -> setVisible(true);
    sprite -> runAction(CCSequence::create(CCDelayTime::create(0.5f),CCBlink::create(5,8),CCHide::create(),NULL));
}

void BarrelLayer::fishDiedScore(int wChairID,int fishScore,int fishMult)
{
    if(m_count == 5)
    {
        m_count = m_count%5;
    }
    int have = 0;
    int num = 0;
    for(int i = 0; i < 5; i++)
    {
        if(m_moneyLayer[i]->getHave())
        {
            have = 1;
            num++;
        }
    }
    if(have == 0)
    {
        for(int i = 0; i < 5; i++)
        {
            m_moneyLayer[i]->setPosition(money_point[i]);
        }
        m_count = 0;
    }
    int score = fishScore;//randNum*100*m_nBarrelMult;  //(1000 --- 20000)
    m_nPlateScore = score;
    if(num!= 5)
    {
        m_moneyLayer[m_count]->setHave(true);
        m_moneyLayer[m_count]->moneySpriteAnimate(wChairID,fishMult,fishScore);  //这个参数是一堆上产生金币的数量和数值
        CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(BarrelLayer::moveMoneyLayer2));
        m_moneyLayer[m_count] -> runAction(CCSequence::create(CCDelayTime::create(5),func,NULL));
        m_count++;
    }
    else
    {
        int index = -1;
        for(int i = 0; i < 5; i++)
        {
            CCPoint point = m_moneyLayer[i]->getPosition();
            if((int)point.x == (int)money_point[4].x)//说明这是最左边的钱堆层，以此向右移动
            {
                index = i;
                break;
            }
        }
        if(index != -1)
        {
            m_moneyLayer[(index + 1)%5]->stopAllActions();
            m_moneyLayer[(index+1)%5]->getBatchNode()->removeAllChildren();
            m_moneyLayer[(index+1)%5]->removeChild(m_moneyLayer[(index+1)%5]->getAtlasMoney());
            m_moneyLayer[(index + 1)%5]->setPosition(money_point[4]);
            moveMoneyLayer((index + 1)%5);
            m_moneyLayer[(index + 1)%5]->setHave(true);
            m_moneyLayer[(index + 1)%5]->moneySpriteAnimate(wChairID,fishMult,m_nPlateScore);
            m_count = (index + 2)%5;
            CCCallFuncN* func = CCCallFuncN::create(this,callfuncN_selector(BarrelLayer::moveMoneyLayer2));
            m_moneyLayer[(index + 1)%5] -> runAction(CCSequence::create(CCDelayTime::create(5),func,NULL));
        }
    }
}

void BarrelLayer::moveMoneyLayer2(CCNode* node)
{
    MoneyLayer* m_mainLayer = (MoneyLayer*)node;
    if(m_mainLayer -> getHave())
    {
        m_mainLayer->removeChild(m_mainLayer->getAtlasMoney());
        m_mainLayer->getBatchNode()->removeAllChildren();
        m_mainLayer -> setHave(false);
        m_mainLayer -> setPosition(money_point[4]);
        int index = 0;
        for(int i = 0; i < 5; i++)
        {
            if(m_mainLayer == m_moneyLayer[i])
            {
                index = i;
            }
        }
        moveMoneyLayer(index);
    }
}

void BarrelLayer::moveMoneyLayer(int index)
{
    if(index == 0)
    {
        for(int i = 4; i >= 1; i--)
        {
            m_moneyLayer[i]->setPosition(money_point[i-1]);
        }
    }
    else if(index == 1)
    {
        m_moneyLayer[0]->setPosition(money_point[3]);
        for(int i = 4; i >= 2; i--)
        {
            m_moneyLayer[i]->setPosition(money_point[i-2]);
        }
    }
    else if(index == 2)
    {
        m_moneyLayer[0]->setPosition(money_point[2]);
        m_moneyLayer[1]->setPosition(money_point[3]);
        for(int i = 4; i >= 3; i--)
        {
            m_moneyLayer[i]->setPosition(money_point[i-3]);
        }
    }
    else if(index == 3)
    {
        m_moneyLayer[4]->setPosition(money_point[0]);
        for(int i = 0; i <= 2; i++)
        {
            m_moneyLayer[i]->setPosition(money_point[i+1]);
        }
    }
    else if(index == 4)
    {
        for(int i = 0; i <= 3; i++)
        {
            m_moneyLayer[i]->setPosition(money_point[i]);
        }
    }
}

//圆盘初始化
void BarrelLayer::loadPlateScore()
{
    m_sp2PlateParents = CCSprite::create("CatchFish2001/fishscoreyuan2.png");
    m_sp2PlateParents -> setPosition(ccp(m_spCannonBg->getContentSize().width + 110,m_spCannonBg->getContentSize().height/2-30));
    m_spCannonBg -> addChild(m_sp2PlateParents,2);
    m_sp2PlateParents ->setOpacity(0);
    m_sp2PlateParents -> setScale(1.3f);
    
    m_spPlate = CCSprite::create("CatchFish2001/fishscoreyuan2.png");
	m_spPlate -> setPosition(ccp(m_sp2PlateParents->getContentSize().width/2,m_sp2PlateParents->getContentSize().height/2));
	m_sp2PlateParents -> addChild(m_spPlate,2);
	m_spPlate -> setTag(PLATESCORE);
	m_spPlate ->setVisible(false);
    
    m_sp2Plate = CCSprite::create("CatchFish2001/fishscoreyuan1.png");
    m_sp2Plate -> setPosition(ccp(m_sp2PlateParents->getContentSize().width/2,m_sp2PlateParents->getContentSize().height/2));
    m_sp2PlateParents -> addChild(m_sp2Plate,2);
    m_sp2Plate ->setVisible(false);
    
    m_sp3Plate = CCSprite::create("CatchFish2001/fishscoreyuanbg.png");
    m_sp3Plate -> setPosition(ccp(m_sp2PlateParents->getContentSize().width/2,40));
    m_sp2PlateParents -> addChild(m_sp3Plate,4);
    m_sp3Plate ->setVisible(false);

	m_labelAtlasPlateScore = CCLabelAtlas::create("","CatchFish2001/barrelpanzinum.png",29,50,'0');
	m_labelAtlasPlateScore ->setPosition(ccp(m_sp2PlateParents->getContentSize().width/2,m_sp2PlateParents->getContentSize().height/2));
	m_labelAtlasPlateScore -> setAnchorPoint(ccp(0.5,0.5));
	m_sp2PlateParents -> addChild(m_labelAtlasPlateScore, 3);
    m_sp4Plate = CCSprite::create(ZhuanPanName[0]);
    m_sp4Plate -> setPosition(ccp(m_sp3Plate->getContentSize().width/2,m_sp3Plate->getContentSize().height/2));
    m_sp3Plate -> addChild(m_sp4Plate,4);
    m_sp4Plate ->setVisible(false);
}

void BarrelLayer::clickYuanPan(int nScore, int nFishID, bool bShowName)
{
    m_sp2PlateParents->stopAllActions();
	m_spPlate->stopAllActions();
    float fPox = 0;
    if(m_nChairID == 0)
    {
        fPox = m_spCannonBg->getContentSize().width - 260;
    }
    else if(m_nChairID == 1)
    {
        fPox = m_spCannonBg->getContentSize().width + 110;
    }
    else if(m_nChairID == 2)
    {
        fPox = m_spCannonBg->getContentSize().width - 260;
    }
    else if(m_nChairID == 3)
    {
        fPox = m_spCannonBg->getContentSize().width + 110;
    }
	m_sp2PlateParents->setPosition(ccp(fPox,m_spCannonBg ->getContentSize().height/2-20));
	char tempNum[20];
	sprintf(tempNum,"%d",nScore);
	m_labelAtlasPlateScore -> setString(tempNum);
	m_spPlate -> stopAllActions();
    m_sp2Plate -> stopAllActions();
	m_labelAtlasPlateScore -> stopAllActions();
    m_sp3Plate->stopAllActions();
	CCRotateTo* rotateTo1 = CCRotateTo::create(0.29f,30);
	CCRotateTo* rotateTo2 = CCRotateTo::create(0.29f,-30);
	m_labelAtlasPlateScore ->runAction(CCRepeatForever::create(CCSequence::create(rotateTo1,rotateTo2,NULL)));
	m_spPlate -> setVisible(true);
    m_sp2Plate -> setVisible(true);
    if(bShowName)
    {
        m_sp3Plate -> setVisible(true);//修改大鱼得分圆盘
        m_sp4Plate -> setVisible(true);//修改大鱼得分圆盘
    }
    else
    {
        m_sp3Plate -> setVisible(false);//修改大鱼得分圆盘
        m_sp4Plate -> setVisible(false);//修改大鱼得分圆盘
    }
    
    m_sp4Plate->stopAllActions();
    m_sp4Plate -> removeFromParent();
    m_sp4Plate = CCSprite::create(ZhuanPanName[nFishID]);
    m_sp4Plate -> setPosition(ccp(m_sp3Plate->getContentSize().width/2,m_sp3Plate->getContentSize().height/2+7));
    m_sp3Plate -> addChild(m_sp4Plate,4);
    if(bShowName)
    {
        m_sp4Plate -> setVisible(true);//修改大鱼得分圆盘
    }
    else
    {
        m_sp4Plate -> setVisible(false);//修改大鱼得分圆盘
    }
    
	this -> unschedule(schedule_selector(BarrelLayer::fadeOutPlate));
	this -> scheduleOnce(schedule_selector(BarrelLayer::fadeOutPlate),4.0f);
    
	m_labelAtlasPlateScore -> runAction(CCFadeIn::create(0.5f));
	CCMoveTo* moveTo = CCMoveTo::create(0.3f,ccp(fPox,m_spCannonBg->getContentSize().height+145));
	CCSpawn* spawn = CCSpawn::create(moveTo,NULL);
	m_sp2PlateParents -> runAction(spawn);
    m_spPlate -> runAction(CCFadeIn::create(0.5f));
    m_spPlate -> runAction(CCRepeatForever::create(CCRotateBy::create(0.5f, -360)));
    m_sp2Plate -> runAction(CCFadeIn::create(0.5f));
    m_sp2Plate -> runAction(CCRepeatForever::create(CCRotateBy::create(0.5f, 360)));
    m_sp3Plate -> runAction(CCFadeIn::create(0.5f));
    m_sp4Plate -> runAction(CCFadeIn::create(0.5f));
}

void BarrelLayer::fadeOutPlate(float dt)
{
    float fPox = 0;
    if(m_nChairID == 0)
    {
        fPox = m_spCannonBg->getContentSize().width - 260;
    }
    else if(m_nChairID == 1)
    {
        fPox = m_spCannonBg->getContentSize().width + 110;
    }
    else if(m_nChairID == 2)
    {
        fPox = m_spCannonBg->getContentSize().width - 260;
    }
    else if(m_nChairID == 3)
    {
        fPox = m_spCannonBg->getContentSize().width + 110;
    }
	m_labelAtlasPlateScore -> runAction(CCFadeOut::create(0.5f));
	CCMoveTo* moveTo = CCMoveTo::create(0.3f,ccp(fPox,m_spCannonBg ->getContentSize().height/2-20));
	CCSpawn* spawn = CCSpawn::create(moveTo,NULL);
	m_sp2PlateParents -> runAction(spawn);
    m_spPlate->runAction(CCFadeOut::create(0.5f));
    m_sp2Plate->runAction(CCFadeOut::create(0.5f));
    m_sp3Plate->runAction(CCFadeOut::create(0.5f));
    m_sp4Plate->runAction(CCFadeOut::create(0.5f));
}

//获取炮台
CCSprite* BarrelLayer::getCannonBg()
{
	return m_spCannonBg;
}

CCSprite* BarrelLayer::getScoreBlockSprite()
{
	return m_spScoreBlock;
}

//获取炮筒
CCSprite* BarrelLayer::getBarrel()
{
	return m_spBarrel;
}

//获取倍率
int BarrelLayer::getBarrelMult()
{
	return m_nBarrelNum;
}

//获取玩家积分板上的积分
CCLabelAtlas* BarrelLayer::getGamelastAtlas()
{
	return m_labelAtlasGameLast;
}

//获取倍率标签
CCLabelAtlas* BarrelLayer::getMultAtlas()
{
	return m_labelAtlasMult;
}

//获取玩家姓名
CCLabelTTF* BarrelLayer::getLabelName()
{
	return m_labelTTFNickName;
}

//获取锁定框
CCSprite* BarrelLayer::getLockBlockSprite()
{
	return m_spLockBlock;
}

CCSprite* BarrelLayer::getSuperPaoSprite()
{
    return m_spSuperPao;
}
//获取锁定鱼ID
int BarrelLayer::getLockFishID()
{
	return m_nLockFishID;
}

void BarrelLayer::setLockFishID(int fishID)
{
	m_nLockFishID = fishID;
}

//玩家是否在锁定鱼状态中
bool BarrelLayer::getUserLockFish()
{
	return m_bLock;
}

void BarrelLayer::setUserLockFish(bool bLockFish)
{
	m_bLock = bLockFish;
}

//获取玩家锁定鱼标识精灵
CCSprite* BarrelLayer::getLockNumSprite()
{
	return m_spLockNum;
}

//获取圆盘
CCSprite* BarrelLayer::getSpritePlate()
{
	return m_sp2PlateParents;
}

void BarrelLayer::setLockLine(int nLine)
{
//    m_spLockLine -> removeFromParent();
//    loadLockLine(nLine);
}

void BarrelLayer::loadLockLine(int nLine)
{
    char temp[20];
    sprintf(temp, "h%d001.png",m_nChairID+1);
    m_spLockLine = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp));
    ccBlendFunc cbl = {GL_ONE_MINUS_DST_COLOR, GL_ONE};
    m_spLockLine -> setBlendFunc(cbl);
    m_spLockLine -> setAnchorPoint(ccp(0.0f,0.5f));
    
    CCArray* lineArray = CCArray::create();
    for(int i = 1; i <= 10; i++)
    {
        sprintf(temp,"h%d00%d.png",m_nChairID+1,i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
        lineArray -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(lineArray,0.15f);
    animation->setLoops(-1);
    CCAnimate* animate = CCAnimate::create(animation);
    m_spLockLine -> runAction(CCRepeatForever::create(animate));
    m_spLockLine -> setPosition(ccp(m_spCannonBg->getContentSize().width/2,m_spCannonBg->getContentSize().height/2-8));
    m_spCannonBg -> addChild(m_spLockLine);
    m_spLockLine -> setVisible(false);
    //    char temp[32];
//    int count = 0;
//    int nVip = 0;
//    float fDelayTime = 0;
//    m_spLockLine = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("suodingxianvip2_1.png"));
//    count = 8;
//    nVip = 2;
//    fDelayTime = 0.05f;
//    ccBlendFunc cbl = {GL_ONE_MINUS_DST_COLOR, GL_ONE};
//    m_spLockLine -> setBlendFunc(cbl);
//    m_spLockLine -> setAnchorPoint(ccp(0.0f,0.5f));
//    
//    CCArray* lineArray = CCArray::create();
//    for(int i = 1; i <= count; i++)
//    {
//        sprintf(temp,"suodingxianvip%d_%d.png", nVip, i);
//        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
//        lineArray -> addObject(frame);
//    }
//    CCAnimation* animation = CCAnimation::createWithSpriteFrames(lineArray,fDelayTime);
//    animation->setLoops(-1);
//    CCAnimate* animate = CCAnimate::create(animation);
//    m_spLockLine -> runAction(CCRepeatForever::create(animate));
//    m_spLockLine -> setPosition(ccp(m_spCannonBg -> getContentSize().width/2,m_spCannonBg->getContentSize().height/3-20));
//    m_spCannonBg -> addChild(m_spLockLine, -2);
//    m_spLockLine -> setVisible(false);
}

void BarrelLayer::setLockLinePoint()
{
    m_spLockLine -> setPosition(ccp(m_spCannonBg -> getContentSize().width/2,m_spCannonBg->getContentSize().height/3-10));
}

CCSprite* BarrelLayer::getLockLine()
{
    return m_spLockLine;
}

CCSprite* BarrelLayer::getMoneySprite()
{
    return m_spMoney;
}

CCSprite* BarrelLayer::getBaojiSprite()
{
    return m_spBaoji;
}

void BarrelLayer::showBaoji(bool bShow)
{
    if(bShow)
    {
        m_spBaoji -> runAction(CCRepeatForever::create(CCSequence::create(CCShow::create(), CCDelayTime::create(0.7f), CCHide::create(), CCDelayTime::create(0.2f), NULL)));
    }
    else
    {
        m_spBaoji->stopAllActions();
        m_spBaoji->setVisible(false);
    }
}

CCSprite* BarrelLayer::getOneChatBg()
{
    return m_spOneChatBg;
}

void BarrelLayer::setChatDataName(std::string sName, std::string sData)
{
    m_labelChatName -> setString(sName.c_str());
    m_labelChatData -> setString(sData.c_str());
}

void BarrelLayer::setShowOneChatBg(bool bShow)
{
    m_labelChatName -> setVisible(bShow);
    m_labelChatData -> setVisible(bShow);
    m_spOneChatBg -> setVisible(bShow);
}

void BarrelLayer::hideChatBg()
{
    setShowOneChatBg(false);
}
