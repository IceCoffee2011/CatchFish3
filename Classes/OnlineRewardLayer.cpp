#include "header.h"

OnlineRewardLayer::OnlineRewardLayer()
{
    
}

OnlineRewardLayer::~OnlineRewardLayer()
{
    m_arrOnline -> release();
}

OnlineRewardLayer * OnlineRewardLayer::create(std::vector<CMD_GRO_OneOnlineRewardInfo> vOnlineRewardInfo)
{
    OnlineRewardLayer * pRet = new OnlineRewardLayer();
    if (pRet && pRet->init(vOnlineRewardInfo))
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

bool OnlineRewardLayer::init(std::vector<CMD_GRO_OneOnlineRewardInfo> vOnlineRewardInfo)
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CMainLogic::sharedMainLogic()->m_bAutoCheckOnlineInfo = true;
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    m_arrOnline = CCArray::create();
    m_arrOnline -> retain();
    
    loadUI();
    loadOnlineData(vOnlineRewardInfo);
    
    return true;
}

void OnlineRewardLayer::loadUI()
{
    m_spBg = CCSprite::create("olinereward/onlinebg.png");
    m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spBg);
    CCSprite* pTitle = CCSprite::create("olinereward/onlinetitle.png");
    pTitle -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height-pTitle->getContentSize().height/2-15));
    m_spBg -> addChild(pTitle);
    CCSprite* pHeiBg = CCSprite::create("olinereward/onlineheibg.png");
    pHeiBg -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height-100));
    m_spBg -> addChild(pHeiBg);
    CCSprite* pTimebg = CCSprite::create("olinereward/onlinetimebg.png");
    pTimebg -> setPosition(ccp(pHeiBg->getContentSize().width/2, pHeiBg->getContentSize().height/2));
    pHeiBg -> addChild(pTimebg);
    m_pOnlineLabel = CCLabelTTF::create("", "Arial-BoldMT", 32);
    m_pOnlineLabel -> setPosition(ccp(pTimebg->getContentSize().width/2, pTimebg->getContentSize().height/2));
    pTimebg -> addChild(m_pOnlineLabel);
    
    
    CCSprite* pInfo = CCSprite::create("olinereward/onlineinfo.png");
    pInfo -> setPosition(ccp(m_spBg->getContentSize().width/2, 110));
    m_spBg -> addChild(pInfo);
    CCLabelTTF* pLabelInfo = CCLabelTTF::create("* 每日在游戏内累计达到一定时长，即可获得奖励。", "Arial-BoldMT", 32);
    pLabelInfo -> setColor(ccc3(123, 47, 17));
    pLabelInfo -> setPosition(ccp(30, pInfo->getContentSize().height/3*2));
    pLabelInfo -> setAnchorPoint(ccp(0, 0.5f));
    pInfo -> addChild(pLabelInfo);
    
    CCMenuItemImage* closeItem = CCMenuItemImage::create("h2all/dakacloseitem.png", "h2all/dakacloseitem.png", this, menu_selector(OnlineRewardLayer::clickMenu));
    closeItem -> setTag(101);
    closeItem -> setPosition(ccp(m_spBg->getContentSize().width-closeItem->getContentSize().width/2+20, m_spBg->getContentSize().height-closeItem->getContentSize().height/2+20));
    CCMenu* pMenu = CCMenu::create(closeItem, NULL);
    pMenu -> setPosition(CCPointZero);
    pMenu -> setAnchorPoint(CCPointZero);
    m_spBg -> addChild(pMenu);
    pMenu -> setTouchPriority(-130);
    
    this -> setTouchEnabled(true);
    
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void OnlineRewardLayer::loadOnlineData(std::vector<CMD_GRO_OneOnlineRewardInfo> vOnlineRewardInfo)
{
    int nCount = (int)vOnlineRewardInfo.size();
    m_pOnlineScroll = CCScrollView::create();
    m_pOnlineLayer = CCLayer::create();
    m_pOnlineLayer -> setContentSize(CCSizeMake(220*nCount, 310));//360
    m_pOnlineLayer -> setAnchorPoint(CCPointZero);
    m_pOnlineLayer -> setPosition(ccp(0,0));
    m_pOnlineScroll->setPosition(ccp(20, 170));     //根据cocostudio调整的位置
    m_pOnlineScroll->setViewSize(CCSizeMake(950, 310));//设置view的大小
    m_pOnlineScroll->setContentOffset(ccp(0,0));
    m_pOnlineScroll->setContentSize(CCSizeMake(950, 310));//设置scrollview区域的大小
    m_pOnlineScroll->setContainer(m_pOnlineLayer); //设置需要滚动的内容
    m_pOnlineScroll->setDirection(kCCScrollViewDirectionHorizontal);  //设置滚动的方向，有三种可以选择
    m_pOnlineScroll->setBounceable(true);
    m_pOnlineScroll->setTouchPriority(-130);
    m_spBg -> addChild(m_pOnlineScroll);
    
    showOnlineRewardUI(vOnlineRewardInfo);
    
}

void OnlineRewardLayer::setOnlineRewardInfo(std::vector<CMD_GRO_OneOnlineRewardInfo> vOnlineRewardInfo)
{
    m_pOnlineLayer->removeAllChildren();
    m_arrOnline->removeAllObjects();
    showOnlineRewardUI(vOnlineRewardInfo);
}

void OnlineRewardLayer::showOnlineRewardUI(std::vector<CMD_GRO_OneOnlineRewardInfo> vOnlineRewardInfo)
{
    int nCount = (int)vOnlineRewardInfo.size();
    char temp[16];
    for(int i = 0; i < nCount; i++)
    {
        printf("43432 dddd  %d, %d,  %hhu\n", vOnlineRewardInfo[i].nScore, vOnlineRewardInfo[i].nSecond, vOnlineRewardInfo[i].cbStatus);
        CCSprite* pOnlineBg = CCSprite::create("olinereward/onlinenolingqu.png");
        pOnlineBg -> setPosition(ccp(110+220*i, 165));
        m_pOnlineLayer -> addChild(pOnlineBg);
        m_arrOnline -> addObject(pOnlineBg);
        CCSprite* pOnlineBg2 = CCSprite::create("olinereward/onlinecanlingqu.png");
        pOnlineBg2 -> setPosition(ccp(pOnlineBg->getContentSize().width/2, pOnlineBg->getContentSize().height/2));
        pOnlineBg -> addChild(pOnlineBg2);
        pOnlineBg2 -> setTag(99);
        CCSprite* pOnlineBg3 = CCSprite::create("olinereward/onlinelingqu.png");
        pOnlineBg3 -> setPosition(ccp(pOnlineBg->getContentSize().width/2, pOnlineBg->getContentSize().height/2));
        pOnlineBg -> addChild(pOnlineBg3, 3, 98);
        if(vOnlineRewardInfo[i].cbStatus == 0)//未到时间
        {
            pOnlineBg2 -> setVisible(false);
            pOnlineBg3 -> setVisible(false);
        }
        else if(vOnlineRewardInfo[i].cbStatus == 1)//可领取
        {
            pOnlineBg -> setOpacity(0);
            pOnlineBg3 -> setVisible(false);
        }
        else if(vOnlineRewardInfo[i].cbStatus == 2)//已领取
        {
            pOnlineBg2 -> setVisible(false);
        }
        sprintf(temp, "%d金币", vOnlineRewardInfo[i].nScore);
        CCLabelTTF* pLabelScore = CCLabelTTF::create(temp, "Arial-BoldMT", 36);
        pLabelScore -> setPosition(ccp(pOnlineBg->getContentSize().width/2, 50));
        pOnlineBg -> addChild(pLabelScore, 2);
        pLabelScore -> setColor(ccc3(167, 76, 27));
        sprintf(temp, "%d分钟", vOnlineRewardInfo[i].nSecond/60);
        CCLabelTTF* pLabelTime = CCLabelTTF::create(temp, "Arial-BoldMT", 30);
        pLabelTime -> setPosition(ccp(pOnlineBg->getContentSize().width/2, pOnlineBg->getContentSize().height-26));
        pOnlineBg -> addChild(pLabelTime, 2, 100);
        if(vOnlineRewardInfo[i].cbStatus == 1)
        {
            pLabelTime -> setString("可领取");
        }
    }
}

void OnlineRewardLayer::setOnlineStatus(int nStatus)
{
    if(nStatus == 1)//说明领取奖励时间到了，有奖可以领取，那么之前此状态必定为未领取
    {
        //从前往后
        int nCount = (int)CMainLogic::sharedMainLogic()->m_vctOnlineRewardInfo.size();
        if(nCount != m_arrOnline->count())
            return ;
        for(int i = 0; i < nCount; i++)
        {
            if(CMainLogic::sharedMainLogic()->m_vctOnlineRewardInfo[i].cbStatus == 0)
            {
                CMainLogic::sharedMainLogic()->m_vctOnlineRewardInfo[i].cbStatus = 1;
                CCSprite* pOnlineBg = (CCSprite*)m_arrOnline->objectAtIndex(i);
                if(pOnlineBg != NULL)
                {
                    CCSprite* pOnlineBg2 = (CCSprite*)pOnlineBg->getChildByTag(99);
                    if(pOnlineBg2 != NULL)
                    {
                        pOnlineBg2 -> setVisible(true);
                        pOnlineBg -> setOpacity(0);
                    }
                    CCLabelTTF* pLabel = (CCLabelTTF*)pOnlineBg->getChildByTag(100);
                    if(pLabel != NULL)
                    {
                        pLabel -> setString("可领取");
                    }
                    break;
                }
            }
        }
    }
    else if(nStatus == 2)//说明此奖已领取过了
    {
        //从后往前
        int nCount = (int)CMainLogic::sharedMainLogic()->m_vctOnlineRewardInfo.size();
        if(nCount != m_arrOnline->count())
            return ;
        for(int i = m_arrOnline->count()-1; i >= 0; i--)
        {
            if(CMainLogic::sharedMainLogic()->m_vctOnlineRewardInfo[i].cbStatus == 1)
            {
                CMainLogic::sharedMainLogic()->m_vctOnlineRewardInfo[i].cbStatus = 2;
                CCSprite* pOnlineBg = (CCSprite*)m_arrOnline->objectAtIndex(i);
                if(pOnlineBg != NULL)
                {
                    CCSprite* pOnlineBg3 = (CCSprite*)pOnlineBg->getChildByTag(98);
                    if(pOnlineBg3 != NULL)
                    {
                        pOnlineBg -> setOpacity(255);
                        pOnlineBg3 -> setVisible(true);
                        CCSprite* pOnlineBg2 = (CCSprite*)pOnlineBg->getChildByTag(99);
                        if(pOnlineBg2 != NULL)
                        {
                            pOnlineBg2 -> setVisible(false);
                        }
                    }
                    CCLabelTTF* pLabel = (CCLabelTTF*)pOnlineBg->getChildByTag(100);
                    if(pLabel != NULL)
                    {
                        char temp[16];
                        sprintf(temp, "%d金币", CMainLogic::sharedMainLogic()->m_vctOnlineRewardInfo[i].nScore);
                        pLabel -> setString(temp);
                    }
                    break;
                }
            }
        }
    }
}

void OnlineRewardLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    if(item->getTag() == 101)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(OnlineRewardLayer::removeWnd));
        m_spBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
    }
}

void OnlineRewardLayer::removeWnd()
{
    this -> removeFromParent();
    
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eOnlineClose);
    
    if(GameMainScene::_instance != NULL)
    {
        
        GameMainScene::GetInstance()->setWndHave(false);
        GameMainScene::GetInstance()->setAutoFire();
        GameMainScene::GetInstance()->setMatchShowStatus(true);
    }
}

void OnlineRewardLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool OnlineRewardLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = m_pOnlineLayer->convertToNodeSpace(point);
    int nCount = (int)CMainLogic::sharedMainLogic()->m_vctOnlineRewardInfo.size();
    if(m_arrOnline->count() != nCount)
    {
        return true;
    }
    for(int i = 0; i < m_arrOnline->count(); i++)
    {
        if(CMainLogic::sharedMainLogic()->m_vctOnlineRewardInfo[i].cbStatus == 1)
        {
            CCSprite* pOnlineBg = (CCSprite*)m_arrOnline->objectAtIndex(i);
            if(pOnlineBg != NULL)
            {
                CCRect rect = pOnlineBg->boundingBox();
                if(rect.containsPoint(point))
                {
                    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
                    CMainLogic::sharedMainLogic()->ShowMessage("正在领取，请稍候", eMBGameNull);
                    if(GameMainScene::_instance != NULL)
                    {
                        GameMainScene::_instance->m_pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_ONLINE_REWARD_GET);
                    }
                    
                    break;
                }
            }
        }
        
    }
    return true;
}
