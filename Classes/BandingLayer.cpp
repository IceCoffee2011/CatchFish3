//
//  BandingLayer.cpp
//  CatchFish3
//
//  Created by Guang on 16/4/7.
//
//

#include "header.h"

bool BandingLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
    CCLayerColor* bgLayer = CCLayerColor::create(ccc4(0, 0, 0, 190));
    this->addChild(bgLayer, 1);
    
    m_pBg = CCSprite::create("messagebox/bindingbg.png");
    m_pBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    bgLayer->addChild(m_pBg);
    m_pBg->setScale(0);
    
    char sBandingScore[16];
    sprintf(sBandingScore, "%d", CMainLogic::sharedMainLogic()->m_nBindReward);
    CCLabelTTF* pBandingScore = CCLabelTTF::create(sBandingScore, "Arial", 32);
    pBandingScore->setColor(ccc3(227, 2, 2));
    pBandingScore->setPosition(ccp(515, 252));
    m_pBg->addChild(pBandingScore);
    
    CCMenuItemImage* bandingItem = CCMenuItemImage::create("messagebox/bindingItem.png", "messagebox/bindingItem.png", this, menu_selector(BandingLayer::buttonCallBack));
    CCMenuItemImage* closeItem = CCMenuItemImage::create("h2all/dakacloseitem.png", "h2all/dakacloseitem.png", this, menu_selector(BandingLayer::buttonCallBack));
    bandingItem->setTag(101);
    closeItem->setTag(102);
    bandingItem->setPosition(ccp(m_pBg->getContentSize().width/2, 120));
    closeItem->setPosition(ccp(m_pBg->getContentSize().width-closeItem->getContentSize().width/3, m_pBg->getContentSize().height-closeItem->getContentSize().height/3));
    CCMenu* menu = CCMenu::create(bandingItem, closeItem, NULL);
    menu->setTouchPriority(-202);
    menu->setPosition(ccp(0, 0));
    m_pBg->addChild(menu, 1);
    
    m_pBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
    
    this->setTouchEnabled(true);
    
    return true;
}

void BandingLayer::buttonCallBack(CCObject* obj)
{
    CCMenuItemImage* item = (CCMenuItemImage*)obj;
    int tag = item->getTag();
    if (tag == 101)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eBtFreeCoinBanding);
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        CCNode* pNode = CCDirector::sharedDirector()->getRunningScene();
        if(pNode != NULL)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            Charge2Layer* pCharge2Layer = (Charge2Layer*)pNode->getChildByTag(20002);
            if(pCharge2Layer != NULL)
            {
                pCharge2Layer->removeFromParent();
            }
#endif
            ChargeLayer* pChargeLayer = (ChargeLayer*)pNode->getChildByTag(20000);
            if(pChargeLayer != NULL)
            {
                pChargeLayer->removeFromParent();
            }
        }
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                this->removeFromParentAndCleanup(true);
                FreeCoinLayer* pFreeCoinLayer = FreeCoinLayer::create();
                pHallLayer->addChild(pFreeCoinLayer, 200, 19996);
                
                pFreeCoinLayer->m_nodeFreeCoin->setVisible(false);
                pFreeCoinLayer->m_spBangingBg->setVisible(true);
                pFreeCoinLayer->m_spBangingBg->runAction(CCSequence::create(CCShow::create(), CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
            }
        }
    }
    else if (tag == 102)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(BandingLayer::removeWnd));
        m_pBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
    }
}

void BandingLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -201, true);
}

bool BandingLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

void BandingLayer::removeWnd()
{
   this->removeFromParentAndCleanup(true);
}
