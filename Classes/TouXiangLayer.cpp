#include "header.h"

const char* txName[10] = {"beibao/tx1.png", "beibao/tx2.png", "beibao/tx3.png", "beibao/tx4.png", "beibao/tx5.png", "beibao/tx6.png", "beibao/tx7.png", "beibao/tx8.png", "beibao/tx9.png", "beibao/tx10.png"};

TouXiangLayer::TouXiangLayer()
{
    
}

TouXiangLayer::~TouXiangLayer()
{
    m_arrTXTotal -> release();
}

TouXiangLayer* TouXiangLayer::create(int nID)
{
    TouXiangLayer* pRet = new TouXiangLayer();
    if(pRet && pRet->init(nID))
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

bool TouXiangLayer::init(int nID)
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    m_arrTXTotal = CCArray::create();
    m_arrTXTotal -> retain();
    m_nSelectID = nID;
    m_nCurrentID = nID;
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    loadUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void TouXiangLayer::loadUI()
{
    m_spBg = CCSprite::create("beibao/touxiangbg.png");
    m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spBg);
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
    
    CCMenuItemImage* pCloseItem = CCMenuItemImage::create("h2all/dakacloseitem.png","h2all/dakacloseitem.png", this, menu_selector(TouXiangLayer::clickMenu));
    pCloseItem->setPosition(ccp(m_spBg->getContentSize().width, m_spBg->getContentSize().height));
    pCloseItem -> setTag(101);
    CCMenuItemImage* pOKItem = CCMenuItemImage::create("beibao/txokitem.png", "beibao/txokitem.png", this, menu_selector(TouXiangLayer::clickMenu));
    pOKItem -> setTag(102);
    pOKItem -> setPosition(ccp(m_spBg->getContentSize().width/2, 85));
    CCMenu* pMenu = CCMenu::create(pCloseItem, pOKItem, NULL);
    pMenu->setPosition(CCPointZero);
    pMenu->setAnchorPoint(CCPointZero);
    m_spBg->addChild(pMenu);
    pMenu->setTouchPriority(-200);
    
    m_spSelect = CCSprite::create("beibao/txselectkuang.png");
    m_spSelect -> setPosition(ccp(98, 342));
    m_spBg -> addChild(m_spSelect, 2);
    
    for(int i = 0; i < 10; i++)
    {
        CCSprite* pDi = CCSprite::create("beibao/txdikuang.png");
        pDi -> setPosition(ccp(98+152*(i%5), 342-142*(i/5)));
        m_spBg -> addChild(pDi, 1);
        m_arrTXTotal -> addObject(pDi);
        CCSprite* pTouxiang = CCSprite::create(txName[i]);
        pTouxiang -> setPosition(ccp(98+152*(i%5), 342-142*(i/5)));
        m_spBg -> addChild(pTouxiang, 3);
        pTouxiang -> setScale(0.7f);
        if(m_nSelectID == i+1)
        {
            m_spSelect -> setPosition(ccp(98+152*(i%5), 342-142*(i/5)));
        }
    }
    
}

void TouXiangLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    if(item->getTag() == 101)//关闭按钮
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(TouXiangLayer::removeWnd));
        m_spBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
    }
    else if(item->getTag() == 102)//确定按钮
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        if(m_nCurrentID != m_nSelectID)//如果两次的ID不一致，那么就向服务端保存头像，并删除此框
        {
            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer -> setGender(m_nSelectID);
                }
            }
            CMD_GPR_ChangeHead ChangeHead = {0};
            ChangeHead.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            ChangeHead.nHeadID = m_nSelectID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_CHANGE_HEAD,&ChangeHead,sizeof(ChangeHead));
            CMainLogic::sharedMainLogic()->m_nTouxiangID = m_nSelectID;
        }
        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(TouXiangLayer::removeWnd));
        m_spBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
    }
}

void TouXiangLayer::removeWnd()
{
    this -> removeFromParent();
}

void TouXiangLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool TouXiangLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = m_spBg->convertToNodeSpace(point);
    for(int i = 0; i < m_arrTXTotal->count(); i++)
    {
        CCSprite* pDi = (CCSprite*)m_arrTXTotal->objectAtIndex(i);
        if(pDi == NULL) continue;
        CCRect rect = pDi->boundingBox();
        if(rect.containsPoint(point))
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            m_spSelect->setPosition(pDi->getPosition());
            m_nSelectID = i+1;
            break;
        }
    }
    return true;
}
