#include "header.h"

Guide2Layer* Guide2Layer::create(int nOpenType, int nChairID)
{
    Guide2Layer* pRet = new Guide2Layer();
    if(pRet && pRet->init(nOpenType, nChairID))
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

bool Guide2Layer::init(int nOpenType, int nChairID)
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    
    
    m_nOpenType = nOpenType;
    m_nChairID = nChairID;
    
    if(nOpenType == 1)
    {
        CCLayerColor* layer = CCLayerColor::create(ccc4(0,0,0,190), CLIENT_WIDTH, CLIENT_HEIGHT);
        this -> addChild(layer);
        loadHallUI();
    }
    else if(nOpenType == 2)
    {
        loadGameUI();
    }
    
    this -> setTouchEnabled(true);
    
    return true;
}

void Guide2Layer::loadHallUI()
{
    m_node1 = CCNode::create();
    this -> addChild(m_node1);
    
    CCMenuItemImage* jcbyItem = CCMenuItemImage::create("hall/nezhanaohai.png", "hall/nezhanaohai.png", this, menu_selector(Guide2Layer::clickMenu));
    jcbyItem -> setPosition(ccp(CLIENT_WIDTH/4, CLIENT_HEIGHT/2-5));
    jcbyItem -> setTag(207);//这个tag不能轻易更改，跟大厅中金蟾的按钮tag保持一致
    CCMenu* jcbyMenu = CCMenu::create(jcbyItem, NULL);
    jcbyMenu -> setPosition(CCPointZero);
    jcbyMenu -> setAnchorPoint(CCPointZero);
    m_node1 -> addChild(jcbyMenu, NULL);
    jcbyMenu -> setTouchPriority(-200);
    CCSprite* pMatchRoomLabel = CCSprite::create("hall/roommatchlabel.png");
    pMatchRoomLabel -> setPosition(ccp(jcbyItem->getContentSize().width-pMatchRoomLabel->getContentSize().width+23, jcbyItem->getContentSize().height-pMatchRoomLabel->getContentSize().height/2-33));
    jcbyItem -> addChild(pMatchRoomLabel);
    CCSprite* pGuideUI1 = CCSprite::create("hall/guideui1.png");
    pGuideUI1 -> setPosition(ccp(jcbyItem->getContentSize().width+80, jcbyItem->getContentSize().height));
    jcbyItem -> addChild(pGuideUI1);
    
    m_node2 = CCNode::create();
    this -> addChild(m_node2);
    m_node2 -> setVisible(false);
    int nCount = (int)CMainLogic::sharedMainLogic()->m_vRoomNZNH.size();
    CCMenuItemImage* newRoomItem = CCMenuItemImage::create("hall/gamebottom2.png", "hall/gamebottom2.png", this, menu_selector(Guide2Layer::clickMenu));
    if(nCount < 4)
    {
        newRoomItem->setPosition(ccp(CLIENT_WIDTH/(nCount+1), 195+150));
    }
    else
    {
        newRoomItem->setPosition(ccp(CLIENT_WIDTH/(nCount+1)+10, 195+150));
    }
    newRoomItem -> setTag(206);
    CCMenu* newRoomMenu = CCMenu::create(newRoomItem, NULL);
    newRoomMenu -> setPosition(CCPointZero);
    newRoomMenu -> setAnchorPoint(CCPointZero);
    m_node2 -> addChild(newRoomMenu);
    newRoomMenu -> setTouchPriority(-200);
    CCSprite* pSpGameJibie = CCSprite::create("hall/gameroom1.png");
    pSpGameJibie->setPosition(ccp(newRoomItem->getContentSize().width/2-12, 40));
    newRoomItem->addChild(pSpGameJibie);
    CCSprite* pSpMatchScore = CCSprite::create("hall/matchscore1.png");
    pSpMatchScore->setPosition(ccp(newRoomItem->getContentSize().width-pSpMatchScore->getContentSize().width/2-34, newRoomItem->getContentSize().height-pSpMatchScore->getContentSize().height/2-30));
    newRoomItem->addChild(pSpMatchScore);
    CCLabelTTF* pLabelScore = CCLabelTTF::create("0", "Arial-BoldMT", 24);
    pLabelScore -> setPosition(ccp(155, 86));
    newRoomItem -> addChild(pLabelScore);
    CCSprite* pGuideUI2 = CCSprite::create("hall/guideui2.png");
    pGuideUI2 -> setPosition(ccp(newRoomItem->getContentSize().width+80, newRoomItem->getContentSize().height));
    newRoomItem -> addChild(pGuideUI2);
}

void Guide2Layer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    if(item->getTag() == 207)//点击哪吒闹海
    {
        m_node1 -> setVisible(false);
        m_node2 -> setVisible(true);
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                pHallLayer->clickEnterGame(item);
            }
        }
    }
    else if(item->getTag() == 206)//点击新手房
    {
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                tagGameServer GameServer = {0};
                for (unsigned int i = 0; i<CMainLogic::sharedMainLogic()->m_vRoomNZNH.size(); i++)
                {
                    GameServer = CMainLogic::sharedMainLogic()->m_vRoomNZNH[i];
                    if(GameServer.wServerPort/10000-1==0)
                    {
                        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                        item->runAction(CCSequence::create(CCScaleTo::create(0.05f, 0.85f), CCScaleTo::create(0.05f, 1.0f), NULL));
                        CCMenuItem* item = CCMenuItem::create();
                        item -> setTag(GameServer.wServerPort);
                        pHallLayer -> gameCallBack(item);
                        break;
                    }
                }
            }
        }
    }
    else if(item->getTag() == 208)//领取新手引导奖励
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        this -> removeFromParent();
        if(GameMainScene::_instance != NULL)
        {
            GameMainScene::_instance->sendGuideFinish();
        }
    }
}

void Guide2Layer::loadGameUI()
{
    m_node3 = CCNode::create();
    this -> addChild(m_node3);
    if(CMainLogic::sharedMainLogic()->m_bShowMatch)
    {
        CCSprite* pMatch = CCSprite::create("competition/cmptBtn.png");
        pMatch -> setPosition(ccp(80, 569-117/2+10-40));
        pMatch -> setAnchorPoint(ccp(0.7f, 0.0f));
        m_node3 -> addChild(pMatch);
        CCSprite* pGuideUI3 = CCSprite::create("hall/guideui3.png");
        pGuideUI3 -> setPosition(ccp(pGuideUI3->getContentSize().width-90, pMatch->getContentSize().height/2));
        pMatch -> addChild(pGuideUI3);
    }
    
//    CCSprite* pDuihuan = CCSprite::create("h2all/mobileItem.png");
//    pDuihuan -> setPosition(ccp(80, CLIENT_HEIGHT/2-120));//1220,510
//    pDuihuan -> setAnchorPoint(ccp(0.5f,0.1f));
//    m_node3 -> addChild(pDuihuan);
//    CCSprite* pGuideUI4 = CCSprite::create("hall/guideui4.png");
//    pGuideUI4 -> setPosition(ccp(pGuideUI4->getContentSize().width-90, pDuihuan->getContentSize().height/2));
//    pDuihuan -> addChild(pGuideUI4);
    CCSprite* pDantou = CCSprite::create("h3all/wufuitem.png");
    pDantou -> setPosition(ccp(1220, 540));
    m_node3 -> addChild(pDantou);
    CCSprite* pGuideUI5 = CCSprite::create("hall/guideui5.png");
    pGuideUI5 -> setPosition(ccp(-pGuideUI5->getContentSize().width+190, pDantou->getContentSize().height/2));
    pDantou -> addChild(pGuideUI5);
    CCSprite* pSpeedUp = CCSprite::create("CatchFish2001/speeditem.png");
    pSpeedUp -> setPosition(ccp(1220, 330));//430
    m_node3 -> addChild(pSpeedUp);
    CCSprite* pLock = CCSprite::create("CatchFish2001/suodingitem.png");
    pLock -> setPosition(ccp(1220, 230));
    m_node3 -> addChild(pLock);
    CCSprite* pGuideUI6 = CCSprite::create("hall/guideui6.png");
    pGuideUI6 -> setPosition(ccp(-pGuideUI6->getContentSize().width+60, pSpeedUp->getContentSize().height/3-10));
    pSpeedUp -> addChild(pGuideUI6);
    
    CCSprite* pCannonBg = CCSprite::createWithSpriteFrameName("CannonBg1.png");
    pCannonBg -> setPosition(ccp(CLIENT_WIDTH/2,pCannonBg->getContentSize().height/2));
    m_node3 ->addChild(pCannonBg);
    CCSprite* pCannonBg2 = CCSprite::createWithSpriteFrameName("CannonBg2.png");
    pCannonBg2 -> setPosition(ccp(pCannonBg->getContentSize().width/2,pCannonBg2->getContentSize().height/2));
    pCannonBg -> addChild(pCannonBg2,2);
    //倍率标签
    CCLabelAtlas* m_labelAtlasMult = CCLabelAtlas::create("10","CatchFish2006/goldNum2.png",28,36,'0');
    m_labelAtlasMult->setAnchorPoint(ccp(0.5, 0.5));
    m_labelAtlasMult -> setPosition(ccp(pCannonBg2->getContentSize().width/2,pCannonBg2->getContentSize().height/2));
    pCannonBg2 -> addChild(m_labelAtlasMult);
    m_labelAtlasMult -> setScale(0.5f);
    //玩家姓名
    CCLabelTTF* m_labelTTFNickName = CCLabelTTF::create(CMainLogic::sharedMainLogic()->m_sNickName.c_str(),"Arial", 24);
    m_labelTTFNickName->setAnchorPoint(ccp(0, 0.5));
    m_labelTTFNickName->setScale(1.2f);
    m_labelTTFNickName -> setPosition(ccp(pCannonBg->getContentSize().width+20, pCannonBg->getContentSize().height/3 - 10));
    pCannonBg -> addChild(m_labelTTFNickName);
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSprite* m_spBarrel = CCSprite::createWithSpriteFrame(cache ->spriteFrameByName("Barrel2_01.png"));
    m_spBarrel -> setPosition(ccp(pCannonBg -> getContentSize().width/2,pCannonBg->getContentSize().height/3+10));
    m_spBarrel -> setAnchorPoint(ccp(0.5,0.2));
    pCannonBg -> addChild(m_spBarrel,1);
    
    //加载积分板
    char temp[40];
    sprintf(temp,"shotNumBack_0%i.png",m_nChairID+1);
//    sprintf(temp,"shotNumBack_05.png");
    CCSprite* m_spScoreBlock = CCSprite::createWithSpriteFrameName(temp);
    m_spScoreBlock -> setPosition(ccp(pCannonBg->boundingBox().size.width + m_spScoreBlock->getContentSize().width/2+20,m_spScoreBlock->getContentSize().height/3*2+25));
    pCannonBg -> addChild(m_spScoreBlock,2);
    
    sprintf(temp, "%d", CMainLogic::sharedMainLogic()->m_nShowGuideScore);
    CCLabelAtlas* m_labelAtlasGameLast = CCLabelAtlas::create(temp,"CatchFish2006/goldNum0.png",23,39,'0');
    m_labelAtlasGameLast->setAnchorPoint(ccp(1, 0.5));
    m_labelAtlasGameLast -> setPosition(ccp(m_spScoreBlock->getContentSize().width-15, m_spScoreBlock->getContentSize().height/2));
    m_spScoreBlock -> addChild(m_labelAtlasGameLast);
    m_labelAtlasGameLast -> setScale(1.0f);
    //加减炮按钮
    CCSprite* minusSp0 = CCSprite::create("CatchFish2006/CannonJianItem0.png");
    CCSprite* plusSp0 = CCSprite::create("CatchFish2006/CannonJiaItem0.png");
    minusSp0->setPosition(ccp(pCannonBg->getContentSize().width/12,pCannonBg2->getContentSize().height/3*2-10));
    plusSp0->setPosition(ccp(pCannonBg->getContentSize().width*11/12,pCannonBg2->getContentSize().height/3*2-10));
    pCannonBg->addChild(minusSp0, 26);
    pCannonBg->addChild(plusSp0, 26);
    
    CCSprite* pGuideUI7 = CCSprite::create("hall/guideui7.png");
    pGuideUI7 -> setPosition(ccp(plusSp0->getContentSize().width+20, 120));
    plusSp0 -> addChild(pGuideUI7);
    CCSprite* pGuideUI8 = CCSprite::create("hall/guideui8.png");
    pGuideUI8 -> setPosition(ccp(minusSp0->getContentSize().width/2-50, 120));
    minusSp0 -> addChild(pGuideUI8);
    if(m_nChairID == 0)
    {
        pCannonBg -> setPosition(ccp(CLIENT_WIDTH/2-220,pCannonBg->getContentSize().height/2));
        m_spScoreBlock->setPosition(ccp(-m_spScoreBlock->getContentSize().width+90, m_spScoreBlock->getContentSize().height/3*2+25));
        m_labelTTFNickName->setAnchorPoint(ccp(1.0f, 0.5f));
        m_labelTTFNickName->setPosition(ccp(-m_spScoreBlock->getContentSize().width+200, pCannonBg->getContentSize().height/3-10));
    }
    else if(m_nChairID == 1)
    {
        pCannonBg -> setPosition(ccp(220+CLIENT_WIDTH/2,pCannonBg->getContentSize().height/2));
        m_labelAtlasGameLast->setAnchorPoint(ccp(0, 0.5));
        m_labelAtlasGameLast -> setPosition(ccp(15, m_spScoreBlock->getContentSize().height/2));
        m_spScoreBlock -> setPosition(ccp(pCannonBg->boundingBox().size.width + m_spScoreBlock->getContentSize().width/2+20,m_spScoreBlock->getContentSize().height/3*2+25));
        m_labelTTFNickName->setAnchorPoint(ccp(0, 0.5f));
        m_labelTTFNickName -> setPosition(ccp(pCannonBg->getContentSize().width+20, pCannonBg->getContentSize().height/3-10));
    }
    else if(m_nChairID == 2)
    {
        pCannonBg -> setPosition(ccp(CLIENT_WIDTH/2-220,pCannonBg->getContentSize().height/2));
        m_spScoreBlock->setPosition(ccp(-m_spScoreBlock->getContentSize().width+90, m_spScoreBlock->getContentSize().height/3*2+25));
        m_labelTTFNickName->setAnchorPoint(ccp(1.0f, 0.5f));
        m_labelTTFNickName->setPosition(ccp(-m_spScoreBlock->getContentSize().width+200, pCannonBg->getContentSize().height/3-10));
    }
    else if(m_nChairID == 3)
    {
        pCannonBg -> setPosition(ccp(CLIENT_WIDTH/2+220,pCannonBg->getContentSize().height/2));
        m_labelAtlasGameLast->setAnchorPoint(ccp(0, 0.5f));
        m_labelAtlasGameLast->setPosition(ccp(15, m_spScoreBlock->getContentSize().height/2));
        m_labelTTFNickName->setAnchorPoint(ccp(1.0f, 0.5f));
        m_labelTTFNickName->setPosition(ccp(370, pCannonBg->getContentSize().height/3-10));
        m_spScoreBlock->setPosition(ccp(m_spScoreBlock->getContentSize().width+80, m_spScoreBlock->getContentSize().height/3*2+25));
    }
    
    m_node4 = CCNode::create();
    m_node4 -> setVisible(false);
    this -> addChild(m_node4);
    CCSprite* pGuideUI9 = CCSprite::create("hall/guideui9.png");
    pGuideUI9 -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    m_node4 -> addChild(pGuideUI9);
    CCMenuItemImage* getGuideItem = CCMenuItemImage::create("hall/getguideitem.png", "hall/getguideitem.png", this, menu_selector(Guide2Layer::clickMenu));
    getGuideItem -> setTag(208);
    getGuideItem -> setPosition(ccp(pGuideUI9->getContentSize().width/2, 60));
    CCMenu* pGuideMenu = CCMenu::create(getGuideItem, NULL);
    pGuideMenu -> setPosition(CCPointZero);
    pGuideMenu -> setAnchorPoint(CCPointZero);
    pGuideUI9 -> addChild(pGuideMenu);
    pGuideMenu -> setTouchPriority(-200);
}

void Guide2Layer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool Guide2Layer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if(m_nOpenType == 2)
    {
        if(GameMainScene::_instance != NULL)
        {
            if(m_node3 != NULL && m_node4 != NULL)
            {
                m_node3 -> setVisible(false);
                m_node4 -> setVisible(true);
            }
            
        }
    }
    return true;
}