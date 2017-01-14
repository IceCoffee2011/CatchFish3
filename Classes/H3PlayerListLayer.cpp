#include "header.h"

extern char* txName[10] ;

PlayerListLayer::PlayerListLayer()
{
    
}

PlayerListLayer::~PlayerListLayer()
{
    
}

bool PlayerListLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    
    loadUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void PlayerListLayer::loadUI()
{
    m_spBg = CCSprite::create("h3card/nopack_window_frame.png");
    m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spBg);
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.55f), CCScaleTo::create(0.07f, 1.45f), NULL));
    CCSprite* spTitle = CCSprite::createWithSpriteFrameName("h3card_common_player_tilte.png");
    spTitle -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height-11));
    m_spBg -> addChild(spTitle);
    spTitle -> setScale(0.75f);
    
    CCScale9Sprite* pTop = CCScale9Sprite::create("h3card/nopack_playerlist_top.png");
    pTop -> setContentSize(CCSize(560, 40));
    pTop -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height-60));
    m_spBg -> addChild(pTop);
    
    char tempRoom[64];
    sprintf(tempRoom, "当前共有%d位玩家!", H3CardBeteLayer::_instance->m_nCurrentOnlineNum);
    CCLabelTTF* pLabel = CCLabelTTF::create("tempRoom", "Arial", 16);
    pLabel -> setPosition(ccp(pTop->getContentSize().width/2, pTop->getContentSize().height/2));
    pLabel -> setColor(ccc3(206, 201, 126));
    pTop -> addChild(pLabel);
    
    int nCount = 100;
    CCScrollView* pScrollInfo = CCScrollView::create();
    CCLayer* pLayerInfo = CCLayer::create();
    pLayerInfo -> setContentSize(CCSizeMake(570, 60*nCount));
    pLayerInfo -> setAnchorPoint(CCPointZero);
    pLayerInfo -> setPosition(ccp(0,300-60*nCount));
    pScrollInfo->setPosition(ccp(27,22));
    pScrollInfo->setViewSize(CCSizeMake(560, 300));
    pScrollInfo->setContentSize(CCSizeMake(560, 60*nCount));
    pScrollInfo->setContainer(pLayerInfo);
    pScrollInfo->setDirection(kCCScrollViewDirectionVertical);
    pScrollInfo->setBounceable(true);
    m_spBg->addChild(pScrollInfo);
    pScrollInfo -> setTouchEnabled(true);
    pScrollInfo -> setTouchPriority(-130);
    char temp[16];
    for(int i = 0; i < nCount; i++)
    {
        CCScale9Sprite* pCellDi = CCScale9Sprite::createWithSpriteFrameName("h3card_player_wanjialiebiao.png");
        pCellDi -> setContentSize(CCSize(560, 60));
        pCellDi -> setPosition(ccp(0, 60*nCount-60*i-60));
        pScrollInfo -> addChild(pCellDi);
        if(i == 0)
        {
            CCSprite* spType = CCSprite::createWithSpriteFrameName("h3card_player_luck.png");
            spType -> setPosition(ccp(65, 30));
            pCellDi -> addChild(spType);
            spType -> setScale(0.75f);
        }
        else if(i == 1)
        {
            CCSprite* spType = CCSprite::createWithSpriteFrameName("h3card_player_fuhao1.png");
            spType -> setPosition(ccp(65, 30));
            pCellDi -> addChild(spType);
            spType -> setScale(0.75f);
        }
        else
        {
            sprintf(temp, "富豪 NO.%d", i);
            CCLabelBMFont* spType = CCLabelBMFont::create(temp, "h3card/h3card_golden.fnt");
            spType -> setPosition(ccp(65, 30));
            pCellDi -> addChild(spType);
            spType -> setScale(0.4f);
        }
        CCSprite* spTx = CCSprite::create(txName[0]);
        spTx -> setPosition(ccp(140, 30));
        spTx -> setScale(0.4f);
        pCellDi -> addChild(spTx);
        CCLabelTTF* pName = CCLabelTTF::create("xiaosong1531", "Arial", 18);
        pName -> setPosition(ccp(190, 45));
        pName -> setAnchorPoint(ccp(0, 0.5f));
        pCellDi -> addChild(pName);
        pName -> setColor(ccc3(207, 201, 131));
        CCSprite* spCoin = CCSprite::createWithSpriteFrameName("h3card_common_chouma.png");
        spCoin -> setPosition(ccp(190, 15));
        spCoin -> setScale(0.75f);
        pCellDi -> addChild(spCoin);
        CCLabelBMFont* pLabelScore = CCLabelBMFont::create("30.85万", "h3card/h3card_golden.fnt");
        pLabelScore -> setPosition(ccp(210, 15));
        pLabelScore -> setAnchorPoint(ccp(0, 0.5f));
        pCellDi -> addChild(pLabelScore);
        pLabelScore -> setScale(0.6f);
        CCLabelTTF* pLabel1 = CCLabelTTF::create("近20局：", "Arial", 20);
        pLabel1 -> setPosition(ccp(380, 30));
        pCellDi -> addChild(pLabel1);
        pLabel1 -> setColor(ccc3(229, 173, 115));
        CCLabelTTF* pLabel2 = CCLabelTTF::create("下注：10.3万", "Arial", 16);
        pLabel2 -> setPosition(ccp(420, 45));
        pLabel2 -> setAnchorPoint(ccp(0, 0.5f));
        pCellDi -> addChild(pLabel2);
        pLabel2 -> setColor(ccc3(192, 184, 172));
        CCLabelTTF* pLabel3 = CCLabelTTF::create("获胜15局", "Arial", 16);
        pLabel3 -> setPosition(ccp(420, 15));
        pLabel3 -> setAnchorPoint(ccp(0, 0.5f));
        pCellDi -> addChild(pLabel3);
        pLabel3 -> setColor(ccc3(192, 184, 172));
    }
    
    
    CCSprite* pCloseNormal = CCSprite::createWithSpriteFrameName("h3card_common_closenormal.png");
    CCSprite* pCloseSelected = CCSprite::createWithSpriteFrameName("h3card_common_closeselected.png");
    CCMenuItemSprite* pCloseItem = CCMenuItemSprite::create(pCloseNormal,pCloseSelected, this, menu_selector(PlayerListLayer::clickMenu));
    pCloseItem->setPosition(ccp(m_spBg->getContentSize().width-10, m_spBg->getContentSize().height-10));
    pCloseItem -> setScale(0.75f);
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    pMenu->setAnchorPoint(CCPointZero);
    m_spBg->addChild(pMenu);
    pMenu->setTouchPriority(-200);
}

void PlayerListLayer::clickMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(PlayerListLayer::removeWnd));
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.55f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
    
}
void PlayerListLayer::removeWnd()
{
    this -> removeFromParent();
}

void PlayerListLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool PlayerListLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}
