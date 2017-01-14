#include "header.h"

const char* PailuType[2] = {"h3card_pailu_black.png", "h3card_pailu_red.png"};
const char* PailuDiType[2] = {"h3card_pailu_gaopai.png", "h3card_pailu_dapaixing.png"};
const char* PailuMideleType[2] = {"h3card_pailu_hei.png", "h3card_pailu_hong.png"};
const char* PailuName[7] = {"单张", "对子","对子", "顺子", "金花", "顺金", "豹子"};

PailuLayer::PailuLayer()
{
    
}

PailuLayer::~PailuLayer()
{
    m_arrWin -> release();
    m_arrPailu -> release();
    m_arrCardType -> release();
}

bool PailuLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    m_arrWin = CCArray::create();
    m_arrWin -> retain();
    m_arrCardType = CCArray::create();
    m_arrCardType -> retain();
    m_arrPailu = CCArray::create();
    m_arrPailu -> retain();
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    loadUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void PailuLayer::loadUI()
{
    m_spBg = CCSprite::create("h3card/nopack_window_frame.png");
    m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-20));
    this -> addChild(m_spBg);
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.55f), CCScaleTo::create(0.07f, 1.45f), NULL));
    CCSprite* pTitle = CCSprite::createWithSpriteFrameName("h3card_pailu_biaoti.png");
    pTitle -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height-3));
    m_spBg -> addChild(pTitle);
    pTitle -> setScale(0.68f);
    m_spTop1 = CCSprite::createWithSpriteFrameName("h3card_pailu_beijing.png");
    m_spTop1 -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height-54));
    m_spBg -> addChild(m_spTop1);
    m_spTop1 -> setScale(0.68f);
    int nPer = 0;
    for(int i = 0; i < 20; i++)
    {
        if(H3CardBeteLayer::_instance->m_nPailu2[i] == 0)
        {
            nPer++;
        }
    }
    char temp[16];
    sprintf(temp, "%d%%", nPer*5);
    CCLabelTTF* plabel1 = CCLabelTTF::create(temp, "Arial", 22);
    plabel1 -> setPosition(ccp(190, 66));
    m_spTop1 -> addChild(plabel1);
    sprintf(temp, "%d%%", 100-nPer*5);
    CCLabelTTF* plabel2 = CCLabelTTF::create(temp, "Arial", 22);
    plabel2 -> setPosition(ccp(662, 66));
    m_spTop1 -> addChild(plabel2);
    plabel1 -> setColor(ccc3(206, 181, 133));
    plabel2 -> setColor(ccc3(206, 181, 133));
    
    for(int i = 0; i < 20; i++)
    {
        int nIndex = H3CardBeteLayer::_instance->m_nPailu2[i];
        CCSprite* spBlackOrRed = CCSprite::createWithSpriteFrameName(PailuType[nIndex]);
        spBlackOrRed -> setPosition(ccp(32+41*i, 20));
        m_spTop1 -> addChild(spBlackOrRed);
        m_arrWin -> addObject(spBlackOrRed);
    }
    
    CCSprite* spTitle1 = CCSprite::createWithSpriteFrameName("h3card_pailu_biaoti1.png");
    spTitle1 -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/3-4));
    spTitle1 -> setScale(0.68f);
    m_spBg -> addChild(spTitle1);
    CCSprite* spTitle2 = CCSprite::createWithSpriteFrameName("h3card_pailu_biaoti2.png");
    spTitle2 -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/3*2+25));
    m_spBg -> addChild(spTitle2);
    spTitle2 -> setScale(0.68f);
    m_spbg1 = CCSprite::createWithSpriteFrameName("h3card_pailu_zoushibg.png");
    m_spbg1 -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2+10));
    m_spBg -> addChild(m_spbg1);
    m_spbg1 -> setScale(0.68f);
    
    for(int i = 0; i < 20; i++)
    {
        CCNode* node = CCNode::create();
        m_spbg1 -> addChild(node);
        m_arrPailu -> addObject(node);
        int nIndex = rand()%2;
        int nIndex2 = rand()%5+1;
        for(int j = 0; j < nIndex2; j++)
        {
            CCSprite* sprb = CCSprite::createWithSpriteFrameName(PailuMideleType[nIndex]);
            sprb -> setPosition(ccp(18+36*i, 162-14-27*j));
            node -> addChild(sprb);
            
        }
    }
    
    m_spNew = CCSprite::createWithSpriteFrameName("h3card_pailu_new.png");
    m_spNew -> setPosition(ccp(660, -120));//-80
    m_spbg1 -> addChild(m_spNew, 2);
    
    int nOpen = CCUserDefault::sharedUserDefault()->getIntegerForKey("FirstOpenH3Pailu", 1);
    if(nOpen == 1)//首次打开，默认显示全部牌型走势，如果不是1，那么显示19个牌型走势
    {
        for(int i = 0; i < 20; i++)
        {
            int nIndex = H3CardBeteLayer::_instance->m_nPailu1[i];
            CCSprite* spdi = CCSprite::createWithSpriteFrameName(PailuDiType[nIndex]);
            spdi -> setPosition(ccp(33+72.4*(i/2), -95-40*(i%2)));
            m_spbg1 -> addChild(spdi);
            int nIndex2 = H3CardBeteLayer::_instance->m_nPailu3[i];
            CCLabelTTF* pLabel = CCLabelTTF::create(PailuName[nIndex2], "Arial", 20);
            pLabel -> setPosition(ccp(spdi->getContentSize().width/2, spdi->getContentSize().height/2));
            spdi -> addChild(pLabel);
            m_arrCardType -> addObject(spdi);
            if(nIndex2 > 1)
            {
                pLabel -> setColor(ccc3(125, 78, 13));
            }
            else
            {
                pLabel -> setColor(ccc3(232, 206, 143));
            }
        }
    }
    else
    {
        for(int i = 0; i < 19; i++)
        {
            int nIndex = H3CardBeteLayer::_instance->m_nPailu1[i+1];
            CCSprite* spdi = CCSprite::createWithSpriteFrameName(PailuDiType[nIndex]);
            spdi -> setPosition(ccp(33+72.4*(i/2), -95-40*(i%2)));
            m_spbg1 -> addChild(spdi);
            int nIndex2 = H3CardBeteLayer::_instance->m_nPailu3[i+1];
            CCLabelTTF* pLabel = CCLabelTTF::create(PailuName[nIndex2], "Arial", 20);
            pLabel -> setPosition(ccp(spdi->getContentSize().width/2, spdi->getContentSize().height/2));
            spdi -> addChild(pLabel);
            m_arrCardType -> addObject(spdi);
            if(nIndex2 > 1)
            {
                pLabel -> setColor(ccc3(125, 78, 13));
            }
            else
            {
                pLabel -> setColor(ccc3(232, 206, 143));
            }
        }
        m_spNew -> setPosition(ccp(660, -80));
    }
    
    
    
    
    CCSprite* pCloseNormal = CCSprite::createWithSpriteFrameName("h3card_common_closenormal.png");
    CCSprite* pCloseSelected = CCSprite::createWithSpriteFrameName("h3card_common_closeselected.png");
    CCMenuItemSprite* pCloseItem = CCMenuItemSprite::create(pCloseNormal,pCloseSelected, this, menu_selector(PailuLayer::clickMenu));
    pCloseItem->setPosition(ccp(m_spBg->getContentSize().width-10, m_spBg->getContentSize().height-10));
    pCloseItem -> setScale(0.75f);
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    pMenu->setAnchorPoint(CCPointZero);
    m_spBg->addChild(pMenu);
    pMenu->setTouchPriority(-200);
}

void PailuLayer::updateUI()
{
    CCSprite* spdi1 = (CCSprite*)m_arrWin->objectAtIndex(0);
    if(spdi1 != NULL)
    {
        spdi1 -> removeFromParent();
        m_arrWin->removeObject(spdi1);
    }
    for(int i = 0; i < m_arrWin->count(); i++)
    {
        CCSprite* spdi = (CCSprite*)m_arrWin->objectAtIndex(i);
        if(spdi == NULL) continue;
        spdi -> setPosition(ccp(32+41*i, 20));
    }
    int nIndex = H3CardBeteLayer::_instance->m_nPailu2[19];
    CCSprite* spBlackOrRed = CCSprite::createWithSpriteFrameName(PailuType[nIndex]);
    spBlackOrRed -> setPosition(ccp(32+41*19, 20));
    m_spTop1 -> addChild(spBlackOrRed);
    m_arrWin -> addObject(spBlackOrRed);
    
    int nOpen = CCUserDefault::sharedUserDefault()->getIntegerForKey("FirstOpenH3Pailu");
    if(nOpen == 1)//说明默认显示全
    {
        CCSprite* spdi2 = (CCSprite*)m_arrCardType->objectAtIndex(0);
        CCSprite* spdi3 = (CCSprite*)m_arrCardType->objectAtIndex(1);
        if(spdi2 != NULL && spdi3 != NULL)
        {
            spdi2 -> removeFromParent();
            spdi3 -> removeFromParent();
            m_arrCardType -> removeObject(spdi2);
            m_arrCardType -> removeObject(spdi3);
        }
        for(int i = 0; i < m_arrCardType->count(); i++)
        {
            CCSprite* spdi = (CCSprite*)m_arrCardType->objectAtIndex(i);
            if(spdi == NULL) continue;
            spdi -> setPosition(ccp(33+72.4*(i/2), -95-40*(i%2)));
        }
        int nIndex = H3CardBeteLayer::_instance->m_nPailu1[19];
        CCSprite* spdi = CCSprite::createWithSpriteFrameName(PailuDiType[nIndex]);
        spdi -> setPosition(ccp(33+72.4*(18/2), -95-40*(18%2)));
        m_spbg1 -> addChild(spdi);
        int nIndex2 = H3CardBeteLayer::_instance->m_nPailu3[19];
        CCLabelTTF* pLabel = CCLabelTTF::create(PailuName[nIndex2], "Arial", 20);
        pLabel -> setPosition(ccp(spdi->getContentSize().width/2, spdi->getContentSize().height/2));
        spdi -> addChild(pLabel);
        m_arrCardType -> addObject(spdi);
        if(nIndex2 > 1)
        {
            pLabel -> setColor(ccc3(125, 78, 13));
        }
        else
        {
            pLabel -> setColor(ccc3(232, 206, 143));
        }
        m_spNew -> setPosition(ccp(660, -80));
        CCUserDefault::sharedUserDefault()->setIntegerForKey("FirstOpenH3Pailu", 2);
    }
    else//说明目前牌型上正空缺一个位置
    {
        m_spNew -> setPosition(ccp(660, -120));
        int nIndex = H3CardBeteLayer::_instance->m_nPailu1[19];
        CCSprite* spdi = CCSprite::createWithSpriteFrameName(PailuDiType[nIndex]);
        spdi -> setPosition(ccp(33+72.4*(19/2), -95-40*(19%2)));
        m_spbg1 -> addChild(spdi);
        int nIndex2 = H3CardBeteLayer::_instance->m_nPailu3[19];
        CCLabelTTF* pLabel = CCLabelTTF::create(PailuName[nIndex2], "Arial", 20);
        pLabel -> setPosition(ccp(spdi->getContentSize().width/2, spdi->getContentSize().height/2));
        spdi -> addChild(pLabel);
        m_arrCardType -> addObject(spdi);
        if(nIndex2 > 1)
        {
            pLabel -> setColor(ccc3(125, 78, 13));
        }
        else
        {
            pLabel -> setColor(ccc3(232, 206, 143));
        }
        CCUserDefault::sharedUserDefault()->setIntegerForKey("FirstOpenH3Pailu", 1);
    }
    
    
    
}

void PailuLayer::clickMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(PailuLayer::removeWnd));
    m_spBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.55f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
}
void PailuLayer::removeWnd()
{
    this -> removeFromParent();
}

void PailuLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool PailuLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}
