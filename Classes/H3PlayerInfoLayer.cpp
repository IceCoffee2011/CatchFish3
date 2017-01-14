#include "header.h"

extern char* txName[10] ;
const char* EmoName[8] = {"h3card_emo_btn_bomb_normal.png", "h3card_emo_btn_thumb_normal.png", "h3card_emo_btn_flower_normal.png", "h3card_emo_btn_egg_normal.png", "h3card_emo_btn_fishing_normal.png", "h3card_emo_btn_double_ace_normal.png", "h3card_emo_btn_chicken_normal.png", "h3card_emo_btn_diamond_normal.png"};

PlayerInfoLayer::PlayerInfoLayer()
{
    
}

PlayerInfoLayer::~PlayerInfoLayer()
{
    m_arrEmo -> release();
}

PlayerInfoLayer* PlayerInfoLayer::create(int nUserID)
{
    PlayerInfoLayer* pRet = new PlayerInfoLayer();
    if(pRet && pRet->init(nUserID))
    {
        pRet -> autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return pRet;
    }
}

bool PlayerInfoLayer::init(int nUserID)
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_arrEmo = CCArray::create();
    m_arrEmo -> retain();
    m_nOpenUserID = nUserID;
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    loadUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void PlayerInfoLayer::loadUI()
{
    m_spBg = CCSprite::create("h3card/nopack_window_frame.png");
    m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spBg);
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
    
    CCSprite* pTouxiangBg = CCSprite::createWithSpriteFrameName("h3card_common_head_frame.png");
    pTouxiangBg -> setPosition(ccp(100, m_spBg->getContentSize().height/4*3));
    m_spBg -> addChild(pTouxiangBg);
    pTouxiangBg -> setScale(0.65f);
    CCSprite* pTX = NULL;
    CCSprite* sp3 = NULL;
    CCLabelTTF* pLabelName = NULL;
   
    tagUserInfo* pUserInfo = CMainLogic::sharedMainLogic()->SearchUserByUserID(m_nOpenUserID);
    if(pUserInfo != NULL && pUserInfo->wFaceID-1>=0 && pUserInfo->wFaceID-1<10)
    {
        pLabelName = CCLabelTTF::create(CCharCode::GB2312ToUTF8(pUserInfo->szNickName).c_str(), "Arial", 26);
        pTX = CCSprite::create(txName[pUserInfo->wFaceID-1]);
        if(CMainLogic::sharedMainLogic()->m_nGender == 1)
        {
            sp3 = CCSprite::createWithSpriteFrameName("h3card_common_userinfomale.png");
        }
        else
        {
            sp3 = CCSprite::createWithSpriteFrameName("h3card_common_userinfofemale.png");
        }
    }
    else
    {
        pLabelName = CCLabelTTF::create("", "Arial", 26);
        sp3 = CCSprite::createWithSpriteFrameName("h3card_common_userinfomale.png");
        pTX = CCSprite::create(txName[0]);
    }
    pTX -> setPosition(ccp(100, m_spBg->getContentSize().height/4*3));
    m_spBg -> addChild(pTX);
    pTX -> setScale(0.82);
    
    pLabelName -> setAnchorPoint(ccp(0, 0.5f));
    pLabelName -> setPosition(ccp(230, m_spBg->getContentSize().height/4*3+30));
    m_spBg -> addChild(pLabelName);
    pLabelName -> setColor(ccc3(212, 212, 212));
    
    sp3 -> setPosition(ccp(m_spBg->getContentSize().width/3-10, m_spBg->getContentSize().height/4*3+30));
    m_spBg -> addChild(sp3);
    
    m_nodeEmo = CCNode::create();
    m_spBg -> addChild(m_nodeEmo);
    m_nodeEmo -> setVisible(false);
    
    m_scrollEmo = CCScrollView::create();
    m_layerEmo = CCLayer::create();
    m_layerEmo -> setContentSize(CCSizeMake(960, 112));
    m_layerEmo -> setAnchorPoint(CCPointZero);
    m_layerEmo -> setPosition(ccp(0,0));
    m_scrollEmo->setPosition(ccp(30,30));     //根据cocostudio调整的位置
    m_scrollEmo->setViewSize(CCSizeMake(553, 112));//设置view的大小
    m_scrollEmo->setContentSize(CCSizeMake(960, 112));//设置scrollview区域的大小
    m_scrollEmo->setContainer(m_layerEmo); //设置需要滚动的内容
    m_scrollEmo->setDirection(kCCScrollViewDirectionHorizontal);  //设置滚动的方向，有三种可以选择
    m_scrollEmo->setBounceable(true);
    m_nodeEmo->addChild(m_scrollEmo);
    m_scrollEmo -> setTouchEnabled(true);
    m_scrollEmo -> setTouchPriority(-130);
    
    for(int i = 0; i < 8; i++)
    {
        CCSprite* spdi1 = CCSprite::createWithSpriteFrameName("h3card_emo_btn_frame.png");
        spdi1 -> setPosition(ccp(60+120*i, 56));
        m_layerEmo -> addChild(spdi1);
        spdi1 -> setTag(101+i);
        m_arrEmo -> addObject(spdi1);
        CCSprite* spEmo = CCSprite::createWithSpriteFrameName(EmoName[i]);
        spEmo -> setPosition(ccp(spdi1->getContentSize().width/2, spdi1->getContentSize().height/2+15));
        spdi1 -> addChild(spEmo);
        spEmo -> setScale(0.8f);
        CCSprite* spCoin = CCSprite::createWithSpriteFrameName("h3card_emo_btn_icon_gold.png");
        spCoin -> setPosition(ccp(30, 20));
        spdi1 -> addChild(spCoin);
        spCoin -> setScale(0.6f);
        CCLabelTTF* pLabelScore = CCLabelTTF::create("1万", "Arial", 20);
        pLabelScore -> setPosition(ccp(spdi1->getContentSize().width/3*2, 20));
        spdi1 -> addChild(pLabelScore);
    }
    
    m_nodeMyInfo = CCNode::create();
    m_spBg -> addChild(m_nodeMyInfo);
    //m_nodeMyInfo -> setVisible(false);
    
    CCLabelTTF* pLabel1 = CCLabelTTF::create("最近使用过的表情", "Arial", 20);
    pLabel1 -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/3));
    m_nodeMyInfo -> addChild(pLabel1);
    pLabel1 -> setColor(ccc3(212, 212, 212));
    CCLabelTTF* pLabel2 = CCLabelTTF::create("魔法表情暂未开放，敬请期待", "Arial", 22);
    pLabel2 -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/4-30));
    m_nodeMyInfo -> addChild(pLabel2);
    pLabel2 -> setColor(ccc3(212, 212, 212));
    
    CCSprite* sp1 = CCSprite::createWithSpriteFrameName("h3card_common_playerinfo_line.png");
    sp1 -> setPosition(ccp(m_spBg->getContentSize().width/4-20, m_spBg->getContentSize().height/3));
    m_nodeMyInfo -> addChild(sp1);
    CCSprite* sp2 = CCSprite::createWithSpriteFrameName("h3card_common_playerinfo_line.png");
    sp2 -> setPosition(ccp(m_spBg->getContentSize().width/4*3+20, m_spBg->getContentSize().height/3));
    sp2 -> setFlipX(true);
    m_nodeMyInfo -> addChild(sp2);
    
    CCSprite* sp4 = CCSprite::createWithSpriteFrameName("h3card_common_chouma.png");
    sp4 -> setPosition(ccp(m_spBg->getContentSize().width/3-10, m_spBg->getContentSize().height/4*3-30));
    m_spBg -> addChild(sp4);
    
    m_pLabelScore = CCLabelBMFont::create("", "h3card/h3card_golden.fnt");
    m_pLabelScore -> setPosition(ccp(230, m_spBg->getContentSize().height/4*3-30));
    m_pLabelScore -> setAnchorPoint(ccp(0, 0.5f));
    m_pLabelScore -> setScale(0.8f);
    m_spBg -> addChild(m_pLabelScore);
    

    
    int nOpenPos = H3CardBeteLayer::_instance->m_nClickPlayerPos;
    if(nOpenPos == 0)
    {
        CCSprite* spXingID = CCSprite::createWithSpriteFrameName("h3card_player_luck.png");
        spXingID -> setPosition(ccp(m_spBg->getContentSize().width-100, m_spBg->getContentSize().height/4*3+30));
        m_spBg -> addChild(spXingID);
    }
    else if(nOpenPos < 8)
    {
        CCLabelBMFont* pLabelInfo = CCLabelBMFont::create("富豪", "h3card/h3card_golden.fnt");
        pLabelInfo -> setPosition(ccp(m_spBg->getContentSize().width-100, m_spBg->getContentSize().height/4*3+30));
        m_spBg -> addChild(pLabelInfo);
    }
    if(nOpenPos == 8)//点击自己
    {
        if(H3CardBeteLayer::_instance->m_dwPlayerUserID[0] == CMainLogic::sharedMainLogic()->m_nUserID)
        {
            CCSprite* spXingID = CCSprite::createWithSpriteFrameName("h3card_player_luck.png");
            spXingID -> setPosition(ccp(m_spBg->getContentSize().width-100, m_spBg->getContentSize().height/4*3+30));
            m_spBg -> addChild(spXingID);
        }
        else
        {
            bool bFuhao = false;
            for(int i = 0; i < 8; i++)
            {
                if(CMainLogic::sharedMainLogic()->m_nUserID == H3CardBeteLayer::_instance->m_dwPlayerUserID[i])
                {
                    bFuhao = true;
                }
            }
            if(bFuhao)
            {
                CCLabelBMFont* pLabelInfo = CCLabelBMFont::create("富豪", "h3card/h3card_golden.fnt");
                pLabelInfo -> setPosition(ccp(m_spBg->getContentSize().width-100, m_spBg->getContentSize().height/4*3+30));
                m_spBg -> addChild(pLabelInfo);
            }
        }
    }
    
    
    CCLabelTTF* pLabel3 = CCLabelTTF::create("20局下注：", "Arial", 22);
    pLabel3 -> setAnchorPoint(ccp(0, 0.5f));
    pLabel3 -> setPosition(ccp(100, m_spBg->getContentSize().height/2));
    m_spBg -> addChild(pLabel3);
    CCLabelTTF* pLabel4 = CCLabelTTF::create("20局获胜：", "Arial", 22);
    pLabel4 -> setAnchorPoint(ccp(0, 0.5f));
    pLabel4 -> setPosition(ccp(100, m_spBg->getContentSize().height/2-30));
    m_spBg -> addChild(pLabel4);
    pLabel3 -> setColor(ccc3(212, 212, 212));
    pLabel4 -> setColor(ccc3(212, 212, 212));
    CCLabelTTF* pLabel5 = CCLabelTTF::create("游戏局数：", "Arial", 22);
    pLabel5 -> setAnchorPoint(ccp(0, 0.5f));
    pLabel5 -> setPosition(ccp(m_spBg->getContentSize().width/2+30, m_spBg->getContentSize().height/2));
    m_spBg -> addChild(pLabel5);
    pLabel5 -> setColor(ccc3(212, 212, 212));
    CCLabelTTF* pLabel6 = CCLabelTTF::create("最大赢取：", "Arial", 22);
    pLabel6 -> setAnchorPoint(ccp(0, 0.5f));
    pLabel6 -> setPosition(ccp(m_spBg->getContentSize().width/2+30, m_spBg->getContentSize().height/2-30));
    m_spBg -> addChild(pLabel6);
    pLabel6 -> setColor(ccc3(212, 212, 212));
    
    m_pLabel1 = CCLabelTTF::create("", "Arial", 22);
    m_pLabel1 -> setAnchorPoint(ccp(0, 0.5f));
    m_pLabel1 -> setPosition(ccp(m_spBg->getContentSize().width/2-104, m_spBg->getContentSize().height/2));
    m_spBg -> addChild(m_pLabel1);
    m_pLabel1 -> setColor(ccc3(241, 218, 168));
    m_pLabel2 = CCLabelTTF::create("", "Arial", 22);
    m_pLabel2 -> setAnchorPoint(ccp(0, 0.5f));
    m_pLabel2 -> setPosition(ccp(m_spBg->getContentSize().width/2-104, m_spBg->getContentSize().height/2-30));
    m_spBg -> addChild(m_pLabel2);
    m_pLabel2 -> setColor(ccc3(241, 218, 168));
    m_pLabel3 = CCLabelTTF::create("", "Arial", 22);
    m_pLabel3 -> setAnchorPoint(ccp(0, 0.5f));
    m_pLabel3 -> setPosition(ccp(m_spBg->getContentSize().width/2+132, m_spBg->getContentSize().height/2));
    m_spBg -> addChild(m_pLabel3);
    m_pLabel3 -> setColor(ccc3(241, 218, 168));
    m_pLabel4 = CCLabelTTF::create("", "Arial", 22);
    m_pLabel4 -> setAnchorPoint(ccp(0, 0.5f));
    m_pLabel4 -> setPosition(ccp(m_spBg->getContentSize().width/2+132, m_spBg->getContentSize().height/2-30));
    m_spBg -> addChild(m_pLabel4);
    m_pLabel4 -> setColor(ccc3(241, 218, 168));
    
    CCSprite* pCloseNormal = CCSprite::createWithSpriteFrameName("h3card_common_closenormal.png");
    CCSprite* pCloseSelected = CCSprite::createWithSpriteFrameName("h3card_common_closeselected.png");
    CCMenuItemSprite* pCloseItem = CCMenuItemSprite::create(pCloseNormal,pCloseSelected, this, menu_selector(PlayerInfoLayer::clickMenu));
    pCloseItem->setPosition(ccp(m_spBg->getContentSize().width-10, m_spBg->getContentSize().height-10));
    pCloseItem -> setTag(101);
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    pMenu->setAnchorPoint(CCPointZero);
    m_spBg->addChild(pMenu);
    pMenu->setTouchPriority(-200);
}

std::string PlayerInfoLayer::getStringScore(SCORE llScore)
{
    char temp[16];
    if(llScore >= 100000000)
    {
        if(llScore%100000000 == 0)
        {
            sprintf(temp, "%lld亿", llScore/100000000);
        }
        else
        {
            if(llScore%10000000 == 0)
            {
                sprintf(temp, "%.1f亿", llScore/100000000.0);
            }
            else
            {
                sprintf(temp, "%.2f亿", llScore/100000000.0);
            }
        }
    }
    else if(llScore >= 10000)
    {
        if(llScore%10000 == 0)
        {
            sprintf(temp, "%lld万", llScore/10000);
        }
        else
        {
            if(llScore%1000 == 0)
            {
                sprintf(temp, "%.1f万", llScore/10000.0);
            }
            else
            {
                sprintf(temp, "%.2f万", llScore/10000.0);
            }
        }
    }
    else
    {
        sprintf(temp, "%lld", llScore);
    }
    return temp;
}

//0金币数，1表示游戏局数，2表示最大赢取金币数，3表示20局下注金币数，4表示20局获胜多少局
void PlayerInfoLayer::updateUI(SCORE lScore, SCORE lTotalNum, SCORE lWinScore, SCORE lTotalBet, SCORE lWinNum)
{
    char temp[16];
    sprintf(temp, "%lld局", lWinNum);
    m_pLabel2 -> setString(temp);
    sprintf(temp, "%lld", lTotalNum);
    m_pLabel3 -> setString(temp);
    m_pLabel1 -> setString(getStringScore(lTotalBet).c_str());
    m_pLabel4 -> setString(getStringScore(lWinScore).c_str());
    m_pLabelScore -> setString(getStringScore(lScore).c_str());
}

void PlayerInfoLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    if(item->getTag() == 101)//关闭按钮
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(PlayerInfoLayer::removeWnd));
        m_spBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
    }
}

void PlayerInfoLayer::removeWnd()
{
    this -> removeFromParent();
}

void PlayerInfoLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool PlayerInfoLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    m_startPoint = pTouch->getLocation();
    
//    CCRect rect = m_spBg->boundingBox();
//    if(!rect.containsPoint(point))
//    {
//        this -> removeFromParent();
//    }
    return true;
}

void PlayerInfoLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint point = pTouch->getLocation();
    if(abs((int)point.x-(int)m_startPoint.x)>10)
    {
        return ;
    }
//    point = m_spBg->convertToNodeSpace(point);
    CCRect rectBg = m_spBg->boundingBox();
    if(!rectBg.containsPoint(point))
    {
        return ;
    }
    point = m_layerEmo->convertToNodeSpace(point);
    for(int i = 0; i < m_arrEmo->count(); i++)
    {
        CCSprite* spdi = (CCSprite*)m_arrEmo->objectAtIndex(i);
        if(spdi == NULL) continue;
        CCRect rect = spdi->boundingBox();
        if(rect.containsPoint(point))
        {
            printf("contain in %d\n", i+1);
            break;
        }
    }
}
