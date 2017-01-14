#include "header.h"

const CCPoint fubenPoint[3] = {ccp(514-40, 400), ccp(514+110, 400), ccp(514+260, 400)};

FuBenLayer::~FuBenLayer()
{
    m_arrZhenzhu->release();
}

FuBenLayer* FuBenLayer::create(int nOpenType, int nChuji, int nZhongji, int nGaoji, int nLastScore)
{
    FuBenLayer* pRet = new FuBenLayer();
    if(pRet && pRet->init(nOpenType, nChuji, nZhongji, nGaoji, nLastScore))
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

bool FuBenLayer::init(int nOpenType, int nChuji, int nZhongji, int nGaoji, int nLastScore)
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_arrZhenzhu = CCArray::create();
    m_arrZhenzhu -> retain();
    m_nOpenType = nOpenType;
    m_nChuji = nChuji;
    m_nZhongji = nZhongji;
    m_nGaoji = nGaoji;
    m_lUserDuiScore = (CMainLogic::sharedMainLogic()->m_lUserScore-nLastScore)-(CMainLogic::sharedMainLogic()->m_lUserScore-nLastScore)%100000;
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    loadUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void FuBenLayer::loadUI()
{
    if(m_nOpenType == 0)
    {
        m_spBg = CCSprite::create("Dragon/fubeninfobg.png");
        m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
        this -> addChild(m_spBg);
        CCMenuItemImage* closeItem = CCMenuItemImage::create("Dragon/fubeninfoclose.png", "Dragon/fubeninfoclose.png", this, menu_selector(FuBenLayer::clickClose));
        closeItem -> setPosition(ccp(m_spBg->getContentSize().width/2, 90));
        CCMenu* pCloseMenu = CCMenu::create(closeItem, NULL);
        pCloseMenu -> setPosition(CCPointZero);
        pCloseMenu -> setAnchorPoint(CCPointZero);
        pCloseMenu -> setTouchPriority(-200);
        m_spBg -> addChild(pCloseMenu);
        
        m_spBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
    }
    else if(m_nOpenType == 1)
    {
        m_spBg = CCSprite::create("Dragon/fubenbg.png");
        m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
        this -> addChild(m_spBg);
        
        CCSprite* sprite1 = CCSprite::create("beibao/beibaokuangbg.png");
        sprite1 -> setPosition(ccp(514-75, 360));
        m_spBg -> addChild(sprite1);
        sprite1 -> setTag(201);
        CCSprite* sprite2 = CCSprite::create("beibao/beibaokuangbg.png");
        sprite2 -> setPosition(ccp(514+75, 360));
        m_spBg -> addChild(sprite2);
        sprite2 -> setTag(202);
        CCSprite* sprite3 = CCSprite::create("beibao/beibaokuangbg.png");
        sprite3 -> setPosition(ccp(514+225, 360));
        m_spBg -> addChild(sprite3);
        sprite3 -> setTag(203);
        CCSprite* pSprite1 = CCSprite::create("h3all/bbtongdantou.png");
        pSprite1 -> setPosition(ccp(sprite1->getContentSize().width/2, sprite1->getContentSize().height/2));
        sprite1 -> addChild(pSprite1);
        CCSprite* pSprite2 = CCSprite::create("h3all/bbyindantou.png");
        pSprite2 -> setPosition(ccp(sprite2->getContentSize().width/2, sprite2->getContentSize().height/2));
        sprite2 -> addChild(pSprite2);
        CCSprite* pSprite3 = CCSprite::create("h3all/bbjindantou.png");
        pSprite3 -> setPosition(ccp(sprite3->getContentSize().width/2, sprite3->getContentSize().height/2));
        sprite3 -> addChild(pSprite3);
        sprite1 -> setScale(0.7f);
        sprite2 -> setScale(0.7f);
        sprite3 -> setScale(0.7f);
        m_arrZhenzhu->addObject(sprite1);
        m_arrZhenzhu->addObject(sprite2);
        m_arrZhenzhu->addObject(sprite3);
        
        m_spSelect = CCSprite::create("Dragon/fubenselect.png");
        m_spSelect -> setPosition(fubenPoint[0]);
        m_spBg -> addChild(m_spSelect);
        
        m_labelAtlas = CCLabelAtlas::create("3", "Dragon/fubennum.png", 32, 48, '0');
        m_labelAtlas -> setAnchorPoint(ccp(0.5f, 0.5f));
        m_labelAtlas -> setPosition(ccp(m_spBg->getContentSize().width/2+80, 230));
        m_spBg -> addChild(m_labelAtlas);
        char temp[32];
        sprintf(temp, "%lld", CMainLogic::sharedMainLogic()->m_lUserScore);
        m_labelScore = CCLabelTTF::create(temp, "Arial-BoldMT", 32);
        m_labelScore -> setPosition(ccp(m_spBg->getContentSize().width/2+135, 168));
        m_labelScore -> setAnchorPoint(ccp(0, 0.5f));
        m_spBg -> addChild(m_labelScore);
        m_labelScore -> setColor(ccc3(217, 36, 1));
        
        char tempScore[128];
        sprintf(tempScore, "当前可兑换金币: %lld", m_lUserDuiScore);
        CCLabelTTF* pLabel = CCLabelTTF::create(tempScore, "Arial-BoldMT", 30);
        pLabel -> setPosition(ccp(m_spBg->getContentSize().width/2+80, 134));
        m_spBg -> addChild(pLabel);
        pLabel -> setColor(ccc3(255, 0, 0));
        
        int num = (int)m_lUserDuiScore/m_nChuji;
        sprintf(temp, "%d", num);
        m_labelAtlas -> setString(temp);
        m_nExchageCount = num;
        m_nCurrentType = 0;
        
        CCMenuItemImage* duihuanItem = CCMenuItemImage::create("Dragon/fubenduihuanItem.png", "Dragon/fubenduihuanItem.png", this, menu_selector(FuBenLayer::clickMenu));
        duihuanItem -> setTag(101);
        duihuanItem -> setScale(0.9f);
        duihuanItem -> setPosition(ccp(m_spBg->getContentSize().width/2+80, 71));
        CCMenuItemImage* jiaItem = CCMenuItemImage::create("Dragon/fubenjiaItem.png", "Dragon/fubenjiaItem.png", this, menu_selector(FuBenLayer::clickMenu));
        jiaItem -> setTag(103);
        jiaItem -> setPosition(ccp(m_spBg->getContentSize().width/4*3+10, 230));
        CCMenuItemImage* jianItem = CCMenuItemImage::create("Dragon/fubenjianItem.png", "Dragon/fubenjianItem.png", this, menu_selector(FuBenLayer::clickMenu));
        jianItem -> setTag(102);
        jianItem -> setPosition(ccp(m_spBg->getContentSize().width/4+135, 230));
        
        CCMenuItemImage* closeItem = CCMenuItemImage::create("freecoin/freecoincloseitem1.png","freecoin/freecoincloseitem1.png", this, menu_selector(FuBenLayer::clickClose));
        closeItem -> setPosition(ccp(m_spBg->getContentSize().width-20, m_spBg->getContentSize().height-70));
        
        CCMenu* pCloseMenu = CCMenu::create(duihuanItem, jiaItem, jianItem, closeItem, NULL);
        pCloseMenu -> setPosition(CCPointZero);
        pCloseMenu -> setAnchorPoint(CCPointZero);
        pCloseMenu -> setTouchPriority(-200);
        m_spBg -> addChild(pCloseMenu);
        
        
        
        m_spBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
    }
}

void FuBenLayer::clickMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case 101://兑换
        {
            if(GameMainScene::_instance != NULL)
            {
                int num = atoi(m_labelAtlas->getString());
                if(num == 0)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("对不起，您先选择兑换数量", eMBOK);
                    break;
                }
                if(m_nExchageCount == 0)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("对不起，您当前金币不足，兑换失败了。", eMBOK);
                    break;
                }
                CMainLogic::sharedMainLogic()->ShowMessage("正在兑换，请稍候", eMBExitGame);
                CMD_C_ExchangePearl ExchangePearl;
                ExchangePearl.nPearlType=m_nCurrentType;
                ExchangePearl.nPearlCount=m_nExchageCount;
                GameMainScene::_instance->m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_EXCHANGE_PEARL,&ExchangePearl,sizeof(ExchangePearl));
                
            }
            
            break;
        }
        case 102://减
        {
            int num = atoi(m_labelAtlas->getString());
            if(num == 0)
            {
                m_nExchageCount = 0;
                break;
            }
            else
            {
                m_nExchageCount--;
                char temp[24];
                sprintf(temp, "%d", m_nExchageCount);
                m_labelAtlas -> setString(temp);
            }
            break;
        }
        case 103://加
        {
            if(m_nCurrentType == 0)//初级珍珠
            {
                int num = (int)m_lUserDuiScore/m_nChuji;
                if(m_nExchageCount>=num)
                {
                    break;
                }
                else
                {
                    m_nExchageCount++;
                    char temp[24];
                    sprintf(temp, "%d", m_nExchageCount);
                    m_labelAtlas -> setString(temp);
                }
            }
            else if(m_nCurrentType == 1)//中级珍珠
            {
                int num = (int)m_lUserDuiScore/m_nZhongji;
                if(m_nExchageCount>=num)
                {
                    break;
                }
                else
                {
                    m_nExchageCount++;
                    char temp[24];
                    sprintf(temp, "%d", m_nExchageCount);
                    m_labelAtlas -> setString(temp);
                }
            }
            else if(m_nCurrentType == 2)//高级珍珠
            {
                int num = (int)m_lUserDuiScore/m_nGaoji;
                if(m_nExchageCount>=num)
                {
                    break;
                }
                else
                {
                    m_nExchageCount++;
                    char temp[24];
                    sprintf(temp, "%d", m_nExchageCount);
                    m_labelAtlas -> setString(temp);
                }
            }
            break;
        }
        default:
            break;
    }
}

void FuBenLayer::clickClose(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(FuBenLayer::removeWnd));
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
}

void FuBenLayer::removeWnd()
{
    this -> removeFromParent();
}

void FuBenLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool FuBenLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if(m_nOpenType == 1)
    {
        CCPoint point = pTouch->getLocation();
        point = m_spBg->convertToNodeSpace(point);
        for(int i = 0; i < m_arrZhenzhu->count(); i++)
        {
            CCSprite* sprite = (CCSprite*)m_arrZhenzhu->objectAtIndex(i);
            if(sprite != NULL)
            {
                CCRect rect = sprite->boundingBox();
                if(rect.containsPoint(point))
                {
                    if(sprite->getTag() == 201)
                    {
                        m_spSelect->setPosition(fubenPoint[0]);
                        m_nCurrentType = 0;
                        char temp[24];
                        int num = (int)m_lUserDuiScore/m_nChuji;
                        sprintf(temp, "%d", num);
                        m_nExchageCount = num;
                        m_labelAtlas -> setString(temp);
                    }
                    else if(sprite->getTag() == 202)
                    {
                        m_spSelect->setPosition(fubenPoint[1]);
                        m_nCurrentType = 1;
                        char temp[24];
                        int num = (int)m_lUserDuiScore/m_nZhongji;
                        sprintf(temp, "%d", num);
                        m_nExchageCount = num;
                        m_labelAtlas -> setString(temp);
                    }
                    else if(sprite->getTag() == 203)
                    {
                        m_spSelect->setPosition(fubenPoint[2]);
                        m_nCurrentType = 2;
                        char temp[24];
                        int num = (int)m_lUserDuiScore/m_nGaoji;
                        sprintf(temp, "%d", num);
                        m_nExchageCount = num;
                        m_labelAtlas -> setString(temp);
                    }
                }
                
            }
        }
    }
    return true;
}