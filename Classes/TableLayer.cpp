#include "header.h"
enum
{
	eBackToHall=200,
    eChangeTable,
	eQuickGame,
    eLabaLayer,
};

CCScene* CTableLayer::scene(int nChairCount)
{
	CCScene * scene = CCScene::create();
	CTableLayer * layer = CTableLayer::create(nChairCount);
    layer->setTag(eTableLayer);
	scene->addChild(layer);
	return scene;
}

CTableLayer* CTableLayer::create(int nChairCount)
{
    CTableLayer* pRet = new CTableLayer;
    if(pRet && pRet->init(nChairCount))
    {
        pRet -> autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

CTableLayer::~CTableLayer()
{

}

bool CTableLayer::init(int nChairCount)
{
	if (!CCLayer::init())
	{
		return false;
	}
    m_nEnterGame = 0;
    
    m_layerTable = CCLayer::create();
    this -> addChild(m_layerTable);
    m_PirateLayer = CCLayer::create();
    this -> addChild(m_PirateLayer);
    //m_layerTable -> setVisible(false);
    m_NiuLayer = CCLayer::create();
    this -> addChild(m_NiuLayer);
    m_layerColor = CCLayerColor::create(ccc4(0, 0, 0, 255), CLIENT_WIDTH, CLIENT_HEIGHT);
    m_NiuLayer->addChild(m_layerColor, 100);
    CCMenuItemImage* pBackItem = CCMenuItemImage::create("h3all/backItem.png", "h3all/backItem.png", this, menu_selector(CTableLayer::menuItemCall));
    pBackItem->setPosition(ccp(CLIENT_WIDTH*0.07, CLIENT_HEIGHT*0.9f));
    pBackItem->setTag(eBackToHall);
    m_menuBack = CCMenu::create(pBackItem, NULL);
    m_menuBack->setPosition(CCPointZero);
    m_layerColor->addChild(m_menuBack, 1);
    m_menuBack -> setVisible(false);
//    CCSprite* pSpBg = CCSprite::create("h2all/buyubg.jpg");
//    pSpBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
//    m_NiuLayer -> addChild(pSpBg, 101);
//    CCSprite* pSpLogo = CCSprite::create("l2ogon/logonsprite.png");
//    pSpLogo -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/3*2));
//    m_NiuLayer -> addChild(pSpLogo, 101);
//    
//    CCSprite* pSpLoadBg = CCSprite::create("CatchFish2001/loadspritebg.png");
//    pSpLoadBg -> setPosition(ccp(890, pSpLoadBg->getContentSize().height/2+20));
//    m_NiuLayer -> addChild(pSpLoadBg, 101);
//    CCSprite* pSpLoad2 = CCSprite::create("CatchFish2001/loadimage20.png");
//    pSpLoad2 -> setPosition(ccp(CLIENT_WIDTH-pSpLoad2->getContentSize().width/2-30, pSpLoad2->getContentSize().height/2+20));
//    m_NiuLayer -> addChild(pSpLoad2, 101);
//    
//    CCSprite* pSpLoad3 = CCSprite::create("CatchFish2001/loadsprite.png");
//    pSpLoad3 -> setPosition(ccp(40, 22));
//    pSpLoadBg -> addChild(pSpLoad3, -1);
//    CCMoveBy* moveBy = CCMoveBy::create(0.15f, ccp(0, 8));
//    CCMoveBy* moveBy2 = CCMoveBy::create(0.15f, ccp(0, -8));
//    pSpLoad3 -> runAction(CCRepeatForever::create(CCSequence::create(moveBy, moveBy2, NULL)));
    
    m_spImage1 = CCSprite::create("CatchFish2001/loadimage2.png");
    m_spImage1 -> setPosition(ccp(CLIENT_WIDTH/2, 60));//CLIENT_HEIGHT/2-
    m_NiuLayer -> addChild(m_spImage1, 101);
    m_spImage1 -> setVisible(false);
    m_spImage2 = CCSprite::create("CatchFish2001/loadimage2.png");
    m_spImage2 -> setPosition(ccp(CLIENT_WIDTH/2, 60));
    m_NiuLayer -> addChild(m_spImage2, 101);
    m_spImage2 -> setVisible(false);
    m_spImage3 = CCSprite::create("CatchFish2001/loadimage3.png");
    m_spImage3 -> setPosition(ccp(CLIENT_WIDTH/2, 60));
    m_NiuLayer -> addChild(m_spImage3, 101);
    m_spImage3 -> setVisible(false);
    
    int a = rand()%3;
    if(a == 0)
    {
        m_spImage1 -> setVisible(true);
    }
    else if(a == 1)
    {
        m_spImage2 -> setVisible(true);
    }
    else if(a == 2)
    {
        m_spImage3 -> setVisible(true);
    }
    
    this -> schedule(schedule_selector(CTableLayer::changeLabel),1.5f);
    
    srand((int)time(NULL));
    m_layerTable -> setVisible(false);
    m_NiuLayer -> setVisible(false);//捕鱼
    m_PirateLayer -> setVisible(false);
    if(CMainLogic::sharedMainLogic()->KIND_ID == NZNH_KIND_ID || CMainLogic::sharedMainLogic()->KIND_ID == FB_KIND_ID || CMainLogic::sharedMainLogic()->KIND_ID == YQS_KIND_ID || CMainLogic::sharedMainLogic()->KIND_ID == JCBY_KIND_ID)
    {
        m_NiuLayer -> setVisible(true);//捕鱼
    }
    else
    {
        m_PirateLayer->setVisible(true);
    }
    m_nChairCount = nChairCount;

	m_vTableAttribute.clear();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	m_bFirstEnter = true;
#endif

	m_bScroll = false;
    m_bChangeTable = false;
    

	m_wSelectTableID = 0;
	m_wLastTableID = 65535;

	updateTableShow();
    
    CMainLogic::sharedMainLogic()->m_bGameExitHall = true;
	return true;
}

void CTableLayer::menuItemCall(CCObject* obj)
{
    CCMenuItem* pTempItem = (CCMenuItem*)obj;
    switch(pTempItem->getTag())
    {
        case eBackToHall://桌子返回大厅
        {
            CMainLogic::sharedMainLogic()->ShowMessage("正在返回到房间选择界面", eMBNull);
            CMainLogic::sharedMainLogic()->switchLayer(eHallLayer, true);
            break;
        }
        case eChangeTable://切换桌子
        {
            pTempItem -> setScale(1.0f);
            pTempItem->stopAllActions();
            pTempItem->runAction(CCSequence::create(CCScaleTo::create(0.1f, 1.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
            //重新创建头像
            for (int i = 0; i < m_nChairCount; i++)
            {
                if(m_spBoyAccount[i] != NULL)
                {
                    m_spBoyAccount[i]->removeFromParent();
                    m_spBoyAccount[i] = CCSprite::create("h2all/touxiangnan.png");
                    m_spBoyAccount[i]->setPosition(m_pSitDownItem[i]->getPosition());
                    m_layerTable->addChild(m_spBoyAccount[i], 4);
                    m_spBoyAccount[i]->setScale(0);
                    m_spBoyAccount[i]->setVisible(false);
                }
                if(m_spGirlAccount[i] != NULL)
                {
                    m_spGirlAccount[i]->removeFromParent();
                    m_spGirlAccount[i] = CCSprite::create("h2all/touxiangnv.png");
                    m_spGirlAccount[i]->setPosition(m_pSitDownItem[i]->getPosition());
                    m_layerTable->addChild(m_spGirlAccount[i], 4);
                    m_spGirlAccount[i]->setScale(0);
                }
            }
            for (unsigned int i = 0; i < m_vTableAttribute.size(); i++)
            {
                if (m_vTableAttribute[i].nPlayerCount < m_vTableAttribute[i].wChairCount)
                {
                    if (m_wSelectTableID == m_wTableCount-1)
                    {
                        m_wSelectTableID = 0;
                    }
                    if (i > m_wSelectTableID)
                    {
                        m_bChangeTable = true;
                        CCMenuItem* pSelectTable = CCMenuItem::create();
                        pSelectTable->setTag(100+i);
                        funcTableView(pSelectTable);
                        break;
                    }
                }
                if(i == m_vTableAttribute.size()-1)
                {
                    for(int j = 0; j < m_vTableAttribute.size(); j++)
                    {
                        if (m_vTableAttribute[j].nPlayerCount < m_vTableAttribute[j].wChairCount)
                        {
                            m_bChangeTable = true;
                            CCMenuItem* pSelectTable = CCMenuItem::create();
                            pSelectTable->setTag(100+j);
                            funcTableView(pSelectTable);
                            break;
                        }
                    }
                }
            }
            break;
        }
        case eQuickGame:
        {
            pTempItem -> setScale(1.0f);
            pTempItem->stopAllActions();
            pTempItem->runAction(CCSequence::create(CCScaleTo::create(0.1f, 1.8f), CCScaleTo::create(0.1f, 1.0f), NULL));
            WORD wTableID = INVALID_TABLE;
            WORD wChairID = INVALID_CHAIR;
            findSeat(wTableID, wChairID);
            //CMainLogic::sharedMainLogic()->ShowMessage("正在初始化游戏", eMBNull);
            CMainLogic::sharedMainLogic()->SendSitDownPacket(wTableID, wChairID, "");
            break;
        }
        case eLabaLayer:
        {
            
            break;
        }
    }
}

void CTableLayer::funcTableView(CCObject* obj)
{
	if (m_bScroll)
	{
		return;
	}
    
    CCMenuItem* pTemp = (CCMenuItem *)obj;
    WORD wSelectTableID = pTemp->getTag() - 100;
    if(m_wSelectTableID == wSelectTableID)
    {
        return ;
    }
    
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    
    //重新创建头像
    for (int i = 0; i < m_nChairCount; i++)
    {
        if(m_spBoyAccount[i] != NULL)
        {
            m_spBoyAccount[i]->removeFromParent();
            m_spBoyAccount[i] = CCSprite::create("h2all/touxiangnan.png");
            m_spBoyAccount[i]->setPosition(m_pSitDownItem[i]->getPosition());
            m_layerTable->addChild(m_spBoyAccount[i], 4);
            m_spBoyAccount[i]->setScale(0);
            m_spBoyAccount[i]->setVisible(false);
        }
        if(m_spGirlAccount[i] != NULL)
        {
            m_spGirlAccount[i]->removeFromParent();
            m_spGirlAccount[i] = CCSprite::create("h2all/touxiangnv.png");
            m_spGirlAccount[i]->setPosition(m_pSitDownItem[i]->getPosition());
            m_layerTable->addChild(m_spGirlAccount[i], 4);
            m_spGirlAccount[i]->setScale(0);
        }
    }

    if (!m_bChangeTable)
    {
        if (m_wSelectTableID == wSelectTableID || m_pScrollView->getContentOffset().y+pTemp->getPositionY() < -43 || m_pScrollView->getContentOffset().y+pTemp->getPositionY()>482)
            return;
    }
    m_bChangeTable = false;

	m_wLastTableID = m_wSelectTableID;
	m_wSelectTableID = wSelectTableID;
	m_spSelectTable->setPosition(ccp(m_vTableAttribute[wSelectTableID].pTable->getPositionX()+m_vTableAttribute[wSelectTableID].pTable->getContentSize().width/2+8, m_vTableAttribute[wSelectTableID].pTable->getPositionY()));

	updateTableShow();
}

void CTableLayer::funcSeat2Click(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    int tag = item->getTag();
    m_wSelectTableID = tag-100;
    CCMenuItem* item2 = CCMenuItem::create();
    item2->setTag(490+m_wSelectTableID);
    funcSeatClick(item2);
}

void CTableLayer::funcSeatClick(CCObject* obj)
{
	CCMenuItem* tempItem = (CCMenuItem*)obj;
	for (unsigned int i = 0; i < m_vTableAttribute.size(); i++)
	{
		if (m_wSelectTableID == m_vTableAttribute[i].wTableID)
		{
			tagTableAttribute & TableAttribute = m_vTableAttribute[i];

			WORD wTableID = i;
			WORD wChairID = tempItem->getTag()-490;

			if (TableAttribute.pUserInfo[wChairID] == NULL)
			{
                if(CMainLogic::sharedMainLogic()->KIND_ID == NZNH_KIND_ID || CMainLogic::sharedMainLogic()->KIND_ID == CJHD_KIND_ID || CMainLogic::sharedMainLogic()->KIND_ID == YQS_KIND_ID || CMainLogic::sharedMainLogic()->KIND_ID == JCBY_KIND_ID)
                {
                    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                }
                //CMainLogic::sharedMainLogic()->ShowMessage("正在初始化游戏", eMBNull);
                CMainLogic::sharedMainLogic()->SendSitDownPacket(wTableID, wChairID, "");
			}
            
            break;
		}
	}
}

void CTableLayer::update(float ft)
{
	if (m_pScrollView->isTouchMoved())
	{
		m_bScroll = true;
	}
	else
	{
		m_bScroll = false;
	}
}

void CTableLayer::setObjVisible(CCNode* pNode, void * pData)
{
    pNode->setVisible((bool)(pData>0));
}

int CTableLayer::getPlayerCount(WORD wTableID)
{
	return m_vTableAttribute[wTableID].nPlayerCount;
}

//配置桌子
void CTableLayer::configTable(WORD wTableCount, WORD wChairCount, const char* szServerName, bool bEnter)
{
	m_wTableCount = wTableCount;
	m_wChairCount = wChairCount;

	//初始化UI
	CCSprite* pScrollListSp = CCSprite::create();
	pScrollListSp->setPosition(ccp(CLIENT_WIDTH*0.72, CLIENT_HEIGHT*0.48));
	m_layerTable->addChild(pScrollListSp);
    
    
	m_spSelectTable = CCSprite::create();//当前选中的桌子
	m_spSelectTable->setAnchorPoint(ccp(1, 0.5));
	CCLayer* containerLayer = CCLayer::create();//创建scrollView
	containerLayer->addChild(m_spSelectTable, 1);
	containerLayer->setContentSize(CCSizeMake(m_spSelectTable->getContentSize().width, m_spSelectTable->getContentSize().height*wTableCount));
	m_pScrollView = CCScrollView::create(CCSizeMake(containerLayer->getContentSize().width+5, m_spSelectTable->getContentSize().height*3.6), containerLayer);
	m_pScrollView->setPosition(ccp(-20, 50));
	m_pScrollView->setContentOffset(ccp(0, m_spSelectTable->getContentSize().height*3.6-containerLayer->getContentSize().height));
	m_pScrollView->setDirection(kCCScrollViewDirectionVertical);//设置滑动方向
	pScrollListSp->addChild(m_pScrollView);
	this->setTouchEnabled(false);

	CCArray* TableItemAry = CCArray::create();
	for (unsigned int i = 0; i < wTableCount; i++)
	{
        CCSprite* pTableView = CCSprite::create();//桌子列表条
		tagTableAttribute TableAttribute = {0};
		{
			TableAttribute.nPlayerCount = 0;
			TableAttribute.wTableID = i;
			TableAttribute.wChairCount = wChairCount;

			char playerNum[10];
			sprintf(playerNum, "%d/%d", TableAttribute.nPlayerCount, m_nChairCount);
			CCLabelTTF * pLabelTTF = CCLabelTTF::create(playerNum, "Arial", 24);//桌子人数n/6
			pLabelTTF->setPosition(ccp(pTableView->getContentSize().width*0.5, pTableView->getContentSize().height/2+24));
			pTableView->addChild(pLabelTTF, 1);
            pLabelTTF -> setColor(ccc3(125, 47, 0));
			TableAttribute.pPlayerNumTTF = pLabelTTF;

            CCSprite* pPercentBg = CCSprite::create();
			pPercentBg->setPosition(ccp(pTableView->getContentSize().width*0.50+4, pTableView->getContentSize().height/2-6));
			pTableView->addChild(pPercentBg);
			CCScale9Sprite* pPlayerPercentSp = CCScale9Sprite::create();//玩家人数所占比例的图片显示
			pPlayerPercentSp->setAnchorPoint(ccp(0, 0.5));
			pPlayerPercentSp->setPosition(ccp(0, pPercentBg->getContentSize().height/2));
			pPercentBg->addChild(pPlayerPercentSp);
			pPlayerPercentSp->setVisible(false);
			TableAttribute.pPlayerPercentShow = pPlayerPercentSp;

			TableAttribute.pFullSp = CCSprite::create();//满人标识
			TableAttribute.pFullSp->setAnchorPoint(ccp(1, 0.5));
			TableAttribute.pFullSp->setPosition(ccp(pTableView->getContentSize().width*0.95, pTableView->getContentSize().height/2));
			TableAttribute.pFullSp->setVisible(false);
			pTableView->addChild(TableAttribute.pFullSp);

            TableAttribute.pNotFullSp = CCMenuItemImage::create("", "", this, menu_selector(CTableLayer::funcSeat2Click));//可进标识
			TableAttribute.pNotFullSp->setAnchorPoint(ccp(1, 0.5));
            TableAttribute.pNotFullSp->setTag(100+i);
			TableAttribute.pNotFullSp->setPosition(ccp(pTableView->getContentSize().width*0.95, pTableView->getContentSize().height/2));
            CCMenu* pMenu = CCMenu::create(TableAttribute.pNotFullSp, NULL);
            pMenu->setPosition(CCPointZero);
            pMenu->setAnchorPoint(CCPointZero);
			pTableView->addChild(pMenu);
		}

		CCMenuItemSprite * pTable = CCMenuItemSprite::create(pTableView, pTableView, this, menu_selector(CTableLayer::funcTableView));
		pTable->setPosition(ccp(pScrollListSp->getContentSize().width*0.55, containerLayer->getContentSize().height-50-(pTableView->getContentSize().height+15)*i));
		pTable->setTag(100+i);
		TableItemAry->addObject(pTable);
		if (i == 0)
		{
			m_spSelectTable->setPosition(ccp(pTable->getPositionX()+pTable->getContentSize().width/2+8, pTable->getPositionY()));
			m_wSelectTableID = 0;
		}

		TableAttribute.pTable = pTable;
		m_vTableAttribute.push_back(TableAttribute);
	}
    m_spTableFree = CCSprite::create();//桌子精灵灰
    m_spTablePlaying = CCSprite::create();//桌子精灵亮
    m_spTableFree->setPosition(ccp(CLIENT_WIDTH*0.28, CLIENT_HEIGHT*0.45));
    m_layerTable->addChild(m_spTableFree, 1);
    m_spTablePlaying->setPosition(m_spTableFree->getPosition());
    m_layerTable->addChild(m_spTablePlaying, 1);
    m_spTablePlaying->setVisible(false);
	CCMenu* pTableMenu = CCMenu::createWithArray(TableItemAry);
	pTableMenu->setPosition(CCPointZero);
	pTableMenu->setTouchPriority(1);
	containerLayer->addChild(pTableMenu);

	CCArray* seatItemAry = CCArray::create();                  //座位
	for (unsigned int i = 0; i < m_nChairCount; i++)
	{
		m_pSitDownItem[i] = CCMenuItemImage::create("", "", this, menu_selector(CTableLayer::funcSeatClick));
        //将座位居中
        if(m_nChairCount == 2)
        {
            if (i < (m_nChairCount+1)/2)
            {
                CCSprite* pSpUserInfo = CCSprite::create("h3all/userInfoBg.png");
                pSpUserInfo->setPosition(ccp(370, 110));
                m_layerTable->addChild(pSpUserInfo);
                m_pSitDownItem[i]->setPosition(ccp(248, 110));
            }
            else
            {
                CCSprite* pSpUserInfo = CCSprite::create("h3all/userInfoBg.png");
                pSpUserInfo->setPosition(ccp(370, 560));
                m_layerTable->addChild(pSpUserInfo);
                m_pSitDownItem[i]->setPosition(ccp(248, 560));
            }
        }
        else if(m_nChairCount == 4)
        {
            if (i < (m_nChairCount+1)/2)
                m_pSitDownItem[i]->setPosition(ccp(270+200*i, 200));
            else
                m_pSitDownItem[i]->setPosition(ccp(270+200*(m_nChairCount-1-i), 560));
        }
        else
        {
            if (i < (m_nChairCount+1)/2)
                m_pSitDownItem[i]->setPosition(ccp(230+140*i, 200));
            else
                m_pSitDownItem[i]->setPosition(ccp(230+140*(m_nChairCount-1-i), 560));
        }
		
		m_pSitDownItem[i]->setTag(490+i);
		seatItemAry->addObject(m_pSitDownItem[i]);
        m_pSitDownItem[i]->setOpacity(0);
        
		m_spBoyAccount[i] = CCSprite::create("h2all/touxiangnan.png");
		m_spBoyAccount[i]->setPosition(m_pSitDownItem[i]->getPosition());
		m_layerTable->addChild(m_spBoyAccount[i], 4);
		m_spBoyAccount[i]->setScale(0);
		m_spBoyAccount[i]->setVisible(false);
		m_spGirlAccount[i] = CCSprite::create("h2all/touxiangnv.png");
		m_spGirlAccount[i]->setPosition(m_pSitDownItem[i]->getPosition());
		m_layerTable->addChild(m_spGirlAccount[i], 4);
		m_spGirlAccount[i]->setScale(0);
		m_spGirlAccount[i]->setVisible(false);
		m_pNickLabelTTF[i] = CCLabelTTF::create("", "Arial", 22);
        m_spSeatIndicate[i] = CCSprite::create();
        //这里要获取所有玩家VIP的等级
        m_pVIPSlider[i] = CCScale9Sprite::create("h2all/vipslider.png");
        
        m_pVIPHead[i] = CCSprite::create("h2all/vipslider2.png");
        
        m_spVipLogo[i] = CCSprite::create("h2all/vipSprite.png");
        m_labelVIP[i] = CCLabelAtlas::create("3", "h2all/vipnumber.png", 19, 23, '0');
        //m_pNickLabel2TTF[i] = CCLabelTTF::create("昵称:", "Arial", 20);
		if (i < (m_nChairCount+1)/2)
		{
			m_pNickLabelTTF[i]->setPosition(ccp(m_pSitDownItem[i]->getPositionX()+80, m_pSitDownItem[i]->getPositionY()+20));
            //m_labelVIP[i] -> setPosition(ccp(m_pSitDownItem[i]->getPositionX()+80, m_pSitDownItem[i]->getPositionY()-20));
            m_spVipLogo[i]->setPosition(ccp(m_pSitDownItem[i]->getPositionX()+130, m_pSitDownItem[i]->getPositionY()-20));
            m_pVIPHead[i]->setPosition(ccp(251, 110));
            m_pVIPSlider[i]->setPosition(ccp(280, 90));
            m_spSeatIndicate[i]->setPosition(ccp(130,110));
		}
		else
		{
			m_pNickLabelTTF[i]->setPosition(ccp(m_pSitDownItem[i]->getPositionX()+80, m_pSitDownItem[i]->getPositionY()+m_spBoyAccount[i]->getContentSize().height*0.75-40));
            m_spVipLogo[i] -> setPosition(ccp(m_pSitDownItem[i]->getPositionX()+130, m_pSitDownItem[i]->getPositionY()+m_spBoyAccount[i]->getContentSize().height*0.75-85));
            m_pVIPHead[i]->setPosition(ccp(251, 560));
            m_pVIPSlider[i]->setPosition(ccp(280, 540));
            m_spSeatIndicate[i]->setPosition(ccp(130,560));
		}
        m_pVIPSlider[i]->setScaleY(1.1f);
        m_pVIPSlider[i]->setAnchorPoint(ccp(0, 0.5));
        m_layerTable->addChild(m_pVIPSlider[i], 1);
        m_layerTable->addChild(m_pVIPHead[i], 2);
        
        m_pVIPSlider[i]->setVisible(false);
        
        m_pNickLabelTTF[i]->setAnchorPoint(ccp(0, 0.5f));
		m_layerTable->addChild(m_pNickLabelTTF[i], 1);
        m_labelVIP[i] -> setAnchorPoint(ccp(0, 0.5f));
        m_labelVIP[i] -> setPosition(ccp(m_spVipLogo[i]->getContentSize().width+10,m_spVipLogo[i]->getContentSize().height/2));
        m_spVipLogo[i]->addChild(m_labelVIP[i]);
        m_spVipLogo[i]->setVisible(false);
        m_layerTable->addChild(m_spVipLogo[i], 2);
        m_layerTable->addChild(m_spSeatIndicate[i], 2);
        m_spSeatIndicate[i]->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.3f, ccp(20,0)), CCMoveBy::create(0.3f, ccp(-20, 0)), NULL)));
	}
	CCMenu* pSeatMenu = CCMenu::createWithArray(seatItemAry);
	pSeatMenu->setPosition(CCPointZero);
	m_layerTable->addChild(pSeatMenu, 1);
	scheduleUpdate();
    
    if(bEnter)
    {
        this->scheduleOnce(schedule_selector(CTableLayer::enterTable), 0.0f);
    }
    
}

void CTableLayer::enterTable(float dt)
{
    CCMenuItem* item = CCMenuItem::create();
    item->setTag(202);
    menuItemCall(item);
    
}

void CTableLayer::setTableStatus(WORD wTableID, bool bPlaying)
{
	if (wTableID >= m_vTableAttribute.size())
	{
		return;
	}

	m_vTableAttribute[wTableID].bPlaying = bPlaying;
    
    //更新桌面显示，@@修正第一次进入房间，桌面状态不同步的问题
    if (wTableID == m_wSelectTableID)
    {
        m_spTablePlaying->setVisible(bPlaying);
        m_spTableFree->setVisible(!bPlaying);
        //第一次进来时更新桌子界面上扑克
        if(CMainLogic::sharedMainLogic()->KIND_ID == 26 || CMainLogic::sharedMainLogic()->KIND_ID == 27)
        {
            updateTableCard(wTableID);
        }
    }
}

bool CTableLayer::SetClientUserItem(WORD wTableID, WORD wChairID, tagUserInfo * pUserInfo)//pUserInfo为NULL时，表示离开座位
{
	if (wTableID >= m_vTableAttribute.size())
	{
		return false;
	}

	m_vTableAttribute[wTableID].pUserInfo[wChairID] = pUserInfo;

	//更新桌子列表显示
	updateTableList(wTableID);

	//更新桌子座位显示
	if (m_wSelectTableID == m_vTableAttribute[wTableID].wTableID)
	{
		updateSeat(wChairID, m_vTableAttribute[wTableID]);
	}

	return true;
}

tagUserInfo * CTableLayer::GetClientUserItem(WORD wTableID, WORD wChairID)
{
    if (wTableID >= m_vTableAttribute.size())
    {
        return false;
    }
    
    return m_vTableAttribute[wTableID].pUserInfo[wChairID];
}

void CTableLayer::updateTableList(WORD wTableID)
{
	for (unsigned int i = 0; i < m_vTableAttribute.size(); i++)
	{
		if (wTableID == m_vTableAttribute[i].wTableID)
		{
			tagTableAttribute & TableAttribute = m_vTableAttribute[i];

			//统计玩家数量
			int nPlayerCount = 0;
			for (int j = 0; j < TableAttribute.wChairCount; j++)
			{
				if (TableAttribute.pUserInfo[j] != NULL)
				{
					nPlayerCount++;
				}
			}
			TableAttribute.nPlayerCount = nPlayerCount;

			//更新桌子列表中玩家数量
			char sPlayCount[32];
			sprintf(sPlayCount, "%d/%d", nPlayerCount, TableAttribute.wChairCount);
			TableAttribute.pPlayerNumTTF->setString(sPlayCount);
			if (nPlayerCount != 0)
			{
				TableAttribute.pPlayerPercentShow->setVisible(true);
			}
            else
            {
                TableAttribute.pPlayerPercentShow->setVisible(false);
            }
			if (nPlayerCount == 1)
			{
                if(m_nChairCount == 6)
                {
                    TableAttribute.pPlayerPercentShow->setContentSize(CCSizeMake(23, 23));
                }
                else if(m_nChairCount == 4)
                {
                    TableAttribute.pPlayerPercentShow->setContentSize(CCSizeMake(30, 23));
                }
                else if(m_nChairCount == 2)
                {
                    TableAttribute.pPlayerPercentShow->setContentSize(CCSizeMake(54, 23));
                }
			}
			else
            {
                if(m_nChairCount == 6)
                {
                    TableAttribute.pPlayerPercentShow->setContentSize(CCSizeMake(18.3*nPlayerCount, 23));
                }
                else if(m_nChairCount == 4)
                {
                    TableAttribute.pPlayerPercentShow->setContentSize(CCSizeMake(28.4*nPlayerCount, 23));
                }
                else if(m_nChairCount == 2)
                {
                    TableAttribute.pPlayerPercentShow->setContentSize(CCSizeMake(54*nPlayerCount, 23));
                }
            }
				

			//更新桌子列表中满人标识
			if (nPlayerCount == TableAttribute.wChairCount)
			{
				TableAttribute.pFullSp->setVisible(true);
				TableAttribute.pNotFullSp->setVisible(false);
			}
			else
			{
				TableAttribute.pFullSp->setVisible(false);
				TableAttribute.pNotFullSp->setVisible(true);
			}
			break;
		}
	}
}

void CTableLayer::updateTableShow()
{
	for (unsigned int i = 0; i < m_vTableAttribute.size(); i++)
	{
		if (m_wSelectTableID == m_vTableAttribute[i].wTableID)
		{
			tagTableAttribute & TableAttribute = m_vTableAttribute[i];
            
            m_spTablePlaying->setVisible(TableAttribute.bPlaying);
            m_spTableFree->setVisible(!TableAttribute.bPlaying);

			//更新桌子座位显示
			for (int j = 0; j < TableAttribute.wChairCount; j++)
			{
				updateSeat(j, TableAttribute);
			}
            
            //更新桌面显示
            

			break;
		}
	}
}

void CTableLayer::updateTableCard(WORD wTableID)
{
    if(wTableID == m_wSelectTableID)
    {
        if(m_spTablePlaying->isVisible())
        {
            //有人再玩，显示扑克
            tagTableAttribute & lastTableAttribute = m_vTableAttribute[wTableID];
            for(int i = 0; i < m_nChairCount; i++)
            {
                bool bLastHavePlayer = lastTableAttribute.pUserInfo[i] != NULL;
                if(bLastHavePlayer)
                {
                    m_spNiuCard[i]->setVisible(true);
                }
            }
            
        }
    }
}

void CTableLayer::updateSeat(WORD wChairID, tagTableAttribute & TableAttribute)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(m_bFirstEnter)
    {
        m_bFirstEnter = false;
    }
#endif
	float fDelay = 0.2f;

	//复位所有状态
    if(m_spBoyAccount[wChairID] != NULL)
    {
        m_spBoyAccount[wChairID]->setVisible(false);
        m_spBoyAccount[wChairID]->setScale(1.0f);
    }
	if(m_spGirlAccount[wChairID] != NULL)
    {
        m_spGirlAccount[wChairID]->setVisible(false);
        m_spGirlAccount[wChairID]->setScale(1.0f);
    }
	m_pSitDownItem[wChairID]->setVisible(true);
	m_pNickLabelTTF[wChairID]->setString("");
    m_spSeatIndicate[wChairID]->setVisible(true);
    m_spVipLogo[wChairID]->setVisible(false);
    m_pVIPHead[wChairID]->setVisible(false);
    m_pVIPSlider[wChairID]->setVisible(false);
    
    if(CMainLogic::sharedMainLogic()->KIND_ID == 26)
    {
        if(wChairID<=1)
        {
            m_spNiuCard[wChairID]->setVisible(false);
        }
        
    }
    if(CMainLogic::sharedMainLogic()->KIND_ID == 27)
    {
        if(wChairID<=3)
        {
            m_spNiuCard[wChairID]->setVisible(false);
        }
    }

	//当前座位的数据
	bool bCurrentHavePlayer = TableAttribute.pUserInfo[wChairID] != NULL;
	BYTE cbCurrentGender = 0;
	if (bCurrentHavePlayer)
	{
		cbCurrentGender = TableAttribute.pUserInfo[wChairID]->cbGender;
	}

	if (m_wLastTableID != 65535)
	{
		//上次座位的数据
		tagTableAttribute & lastTableAttribute = m_vTableAttribute[m_wLastTableID];

		bool bLastHavePlayer = lastTableAttribute.pUserInfo[wChairID] != NULL;
		BYTE cbLastGender = 0;
		if (bLastHavePlayer)
		{
			cbLastGender = lastTableAttribute.pUserInfo[wChairID]->cbGender;
		}

		//上次座位上的玩家渐变消失；切换动作的前半步
		if (bLastHavePlayer)
		{
			if (cbLastGender == 1)
			{
                if(m_spBoyAccount[wChairID] != NULL)
                {
                    m_spBoyAccount[wChairID]->setVisible(true);
                    m_spBoyAccount[wChairID]->runAction(CCSequence::create(CCScaleTo::create(fDelay, 0), CCCallFuncND::create(this, callfuncND_selector(CTableLayer::setObjVisible), (void *)0), NULL));
                }
			}
			else
			{
                if(m_spGirlAccount[wChairID] != NULL)
                {
                    m_spGirlAccount[wChairID]->setVisible(true);
                    m_spGirlAccount[wChairID]->runAction(CCSequence::create(CCScaleTo::create(fDelay, 0), CCCallFuncND::create(this, callfuncND_selector(CTableLayer::setObjVisible), (void *)0), NULL));
                }
			}
			m_pSitDownItem[wChairID]->setVisible(false);
		}
	}

	//当前座位上的玩家渐变出现；切换动作的后半步
	if (bCurrentHavePlayer)
	{
        m_pNickLabelTTF[wChairID]->setString(CCharCode::GB2312ToUTF8(TableAttribute.pUserInfo[wChairID]->szNickName).c_str());//玩家昵称
        m_spSeatIndicate[wChairID]->setVisible(false);
        m_spVipLogo[wChairID]->setVisible(true);
        m_pVIPHead[wChairID]->setVisible(true);
        m_pVIPSlider[wChairID]->setVisible(true);
        
        
        if(CMainLogic::sharedMainLogic()->KIND_ID == 26 || CMainLogic::sharedMainLogic()->KIND_ID == 27)
        {
            if(m_spTablePlaying->isVisible())
            {
                m_spNiuCard[wChairID]->setVisible(true);
            }
        }
		if (cbCurrentGender == 0)
		{
			 if(m_spBoyAccount[wChairID] != NULL)
            {
                m_spBoyAccount[wChairID]->runAction(CCSequence::create(CCDelayTime::create(fDelay), CCScaleTo::create(0, 0), CCCallFuncND::create(this, callfuncND_selector(CTableLayer::setObjVisible), (void *)1), CCScaleTo::create(fDelay, 1), NULL));
            }
		}
		else
		{
			if(m_spGirlAccount[wChairID])
            {
                m_spGirlAccount[wChairID]->runAction(CCSequence::create(CCDelayTime::create(fDelay), CCScaleTo::create(0, 0), CCCallFuncND::create(this, callfuncND_selector(CTableLayer::setObjVisible), (void *)1), CCScaleTo::create(fDelay, 1), NULL));
            }
		}
		m_pSitDownItem[wChairID]->setVisible(false);
		m_pSitDownItem[wChairID]->stopAllActions();
	}
	else
	{
		m_pSitDownItem[wChairID]->runAction(CCSequence::create(CCDelayTime::create(fDelay*2), CCCallFuncND::create(this, callfuncND_selector(CTableLayer::setObjVisible), (void *)1), NULL));
	}
}

bool CTableLayer::findSeat(WORD & wTableID, WORD & wChairID)
{
	bool bHave = false;
	for (unsigned int i = 0; i < m_vTableAttribute.size(); i++)
	{
		tagTableAttribute & TableAttribute = m_vTableAttribute[i];
		for (int j = 0; j < TableAttribute.wChairCount; j++)
		{
			if (TableAttribute.pUserInfo[j] == NULL)
			{
                if(m_nEnterGame--)
                {
                    continue;
                }
				bHave = true;
				wTableID = i;
				wChairID = j;
				return bHave;//两层循环，不能简单break
			}
		}
	}
	return bHave;
}

void CTableLayer::setEnterGame()
{
    static int index = 0;
    index++;
    m_nEnterGame = index;
}

void CTableLayer::onEnterTransitionDidFinish()
{
    //
    if(CMainLogic::sharedMainLogic()->KIND_ID == NZNH_KIND_ID || CMainLogic::sharedMainLogic()->KIND_ID == FB_KIND_ID || CMainLogic::sharedMainLogic()->KIND_ID == YQS_KIND_ID || CMainLogic::sharedMainLogic()->KIND_ID == JCBY_KIND_ID)
    {
        //发送查询炮台和技能消息
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_SKILL, SUB_GRR_QUERY_SKILL);
    }
    else if(CMainLogic::sharedMainLogic()->KIND_ID == CJHD_KIND_ID)
    {
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle,MDM_GR_GET_BIG_REWARD,SUB_GRR_GET_BIG_REWARD);
//        if(CMainLogic::sharedMainLogic()->m_bBYToSuperPirse)
//        {
//            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_USER, SUB_GR_FIRST_SUPERREWARD);
//            CMainLogic::sharedMainLogic()->m_bBYToSuperPirse = false;
//        }
        
        
    }
    
}

void CTableLayer::changeLabel(float f)
{
    int a = rand()%3;
    if(a == 0)
    {
        m_spImage1 -> setVisible(true);
        m_spImage2 -> setVisible(false);
        m_spImage3 -> setVisible(false);
    }
    else if(a == 1)
    {
        m_spImage2 -> setVisible(true);
        m_spImage1 -> setVisible(false);
        m_spImage3 -> setVisible(false);
    }
    else if(a == 2)
    {
        m_spImage3 -> setVisible(true);
        m_spImage1 -> setVisible(false);
        m_spImage2 -> setVisible(false);
    }
}
