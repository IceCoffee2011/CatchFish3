//
//  SettingLayer.cpp
//  CatchFish
//
//  Created by 河北腾游 on 15/3/2.
//
//

#include "header.h"

bool FishWndLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    m_pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(m_pLayerColor, -1);
    m_pLayerColor -> setVisible(false);
    
    loadH2elp();
    loadC2alc();
    
    return true;
}

void FishWndLayer::loadH2elp()
{
    m_pHelpBg = CCSprite::create("s2etting/helpAndCalcBg2.png");
    m_pHelpBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(m_pHelpBg);
    
    CCSprite * pHelpTitle = CCSprite::create("s2etting/helptitle2.png");
    pHelpTitle->setPosition(ccp(m_pHelpBg->getContentSize().width/2, m_pHelpBg->getContentSize().height-pHelpTitle->getContentSize().height+8));
    m_pHelpBg->addChild(pHelpTitle);
    
    CCMenuItemImage * closeItem = CCMenuItemImage::create("Dragon/dragoncloseitem.png","Dragon/dragoncloseitem.png", this, menu_selector(FishWndLayer::closeWnd));
    closeItem->setPosition(ccp(m_pHelpBg->getContentSize().width-20, m_pHelpBg->getContentSize().height-20));
    closeItem->setScale(1.2f);
    closeItem->setTag(202);
    CCMenu * closeMenu = CCMenu::create(closeItem,NULL);
    closeMenu->setTouchPriority(-200);
    closeMenu->setPosition(CCPointZero);
    m_pHelpBg->addChild(closeMenu);
    
    loadHelpSprite();
    m_pHelpBg->setVisible(false);
     
}

void FishWndLayer::loadC2alc()
{
    m_pCalcBg = CCSprite::create("s2etting/helpAndCalcBg2.png");
    m_pCalcBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(m_pCalcBg);
    
    CCSprite * pCalcTitle = CCSprite::create("s2etting/jiesuantitle2.png");
    pCalcTitle->setPosition(ccp(m_pCalcBg->getContentSize().width/2, m_pCalcBg->getContentSize().height-pCalcTitle->getContentSize().height+8));
    m_pCalcBg->addChild(pCalcTitle);
    
    CCMenuItemImage * closeItem = CCMenuItemImage::create("Dragon/dragoncloseitem.png","Dragon/dragoncloseitem.png", this, menu_selector(FishWndLayer::closeWnd));
    closeItem->setPosition(ccp(m_pCalcBg->getContentSize().width-20, m_pCalcBg->getContentSize().height-20));
    closeItem->setScale(1.2f);
    closeItem->setTag(203);
    CCMenu * closeMenu = CCMenu::create(closeItem,NULL);
    closeMenu->setTouchPriority(-200);
    closeMenu->setPosition(CCPointZero);
    m_pCalcBg->addChild(closeMenu);
    
    CCSprite* exitGame = CCSprite::create("s2etting/gameexit.png");
    CCSprite* exitGame2 = CCSprite::create("s2etting/gameexit2.png");
    CCMenuItemSprite* exitItem = CCMenuItemSprite::create(exitGame, exitGame2, this, menu_selector(FishWndLayer::exitGame));
    exitItem -> setPosition(ccp(m_pCalcBg->getContentSize().width/2, exitItem->getContentSize().height/2-15));
    CCMenu* pMenu = CCMenu::create(exitItem,NULL);
    pMenu -> setPosition(CCPointZero);
    pMenu -> setAnchorPoint(CCPointZero);
    m_pCalcBg -> addChild(pMenu,2);
    pMenu -> setTouchPriority(-200);
    
    loadWndCalcSprite();
    m_pCalcBg->setVisible(false);
}

void FishWndLayer::removeWnd()
{
    m_pLayerColor -> setVisible(false);
    m_pHelpBg -> setVisible(false);
    m_pCalcBg -> setVisible(false);
    GameMainScene::GetInstance()->setTouchEnabled(true);
    GameMainScene::GetInstance()->setMenuTouch(true);
    GameMainScene::GetInstance()->setWndHave(false);
    GameMainScene::GetInstance()->setWndCalcHave(false);
}

void FishWndLayer::closeWnd(CCObject* object)
{
    CCMenuItemImage* menuItem = (CCMenuItemImage*)object;
    switch (menuItem->getTag())
    {
        case 202://close help
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
            CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(FishWndLayer::removeWnd));
            m_pHelpBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
            break;
        }
        case 203://close CaclWnd
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
            GameMainScene::GetInstance()->setAutoFire();
            CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(FishWndLayer::removeWnd));
            m_pCalcBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
            break;
        }
        default:
            break;
    }

}
#pragma mark -- Help
void FishWndLayer::loadHelpSprite()
{
    if(CMainLogic::sharedMainLogic()->KIND_ID == YQS_KIND_ID)
    {
        CCScrollView* m_helpScrollView = CCScrollView::create();
        CCLayer* containerLayer = CCLayer::create();
        containerLayer -> setContentSize(CCSizeMake(1000, 650));
        containerLayer -> setAnchorPoint(CCPointZero);
        containerLayer -> setPosition(ccp(0,-200));
        m_helpScrollView->setPosition(ccp(0,30));     //根据cocostudio调整的位置
        m_helpScrollView->setViewSize(CCSizeMake(1000, 450));//设置view的大小
        m_helpScrollView->setContentSize(CCSizeMake(1000, 650));//设置scrollview区域的大小
        m_helpScrollView->setContainer(containerLayer); //设置需要滚动的内容
        m_helpScrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
        m_helpScrollView->setBounceable(true);
        m_pHelpBg->addChild(m_helpScrollView);
        CCSize scrollSize = m_helpScrollView->getContentSize();
        int nFishCount = 32;
        GameFish* fish[nFishCount];
        CCLabelTTF* mulTTF[nFishCount];
        const char* str[32] = {"x2","x2","x3","x4","x5","x6","x7","x8","x9","x10","x12","x15","x18","x20","x25","x30","x40","x100","x120","x150","x200","x200","x250","x300","半屏炸弹","半屏炸弹","全屏炸弹","x3-300","x60","x80","x100","漂流瓶"};
        for(int i = 0; i < nFishCount; i++)
        {
            mulTTF[i] = CCLabelTTF::create(str[i], "Arial", 24);
        }
        const int nFishID[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
        for(int i = 0; i < nFishCount; i++)
        {
            fish[i] = NULL;
            fish[i] = GameMainScene::GetInstance()->createFishByID(1000+nFishID[i]);
            containerLayer->addChild(fish[i]->m_pFish);
            fish[i]->m_pFish->setPosition(ccp(110+160*(i%6), 610-107*(i/6)));
            mulTTF[i]->setPosition(ccp(110+160*(i%6), 575-112*(i/6)));
            mulTTF[i]->setColor(ccc3(179, 119, 31));
            containerLayer->addChild(mulTTF[i]);
        }
        
        fish[0]->m_pFish->setScale(1.0f);
        fish[1]->m_pFish->setScale(1.2f);
        fish[2]->m_pFish->setScale(1.2f);
        fish[3]->m_pFish->setScale(1.0f);
        fish[4]->m_pFish->setScale(1.0f);
        fish[5]->m_pFish->setScale(0.8f);
        
        fish[6]->m_pFish->setScale(1.0f);
        fish[7]->m_pFish->setScale(0.65f);
        fish[8]->m_pFish->setScale(0.9f);
        fish[9]->m_pFish->setScale(0.7f);
        fish[10]->m_pFish->setScale(0.7f);
        fish[11]->m_pFish->setScale(0.5f);
        
        fish[12]->m_pFish->setScale(0.6f);
        fish[13]->m_pFish->setScale(0.6f);
        fish[14]->m_pFish->setScale(0.4f);
        fish[15]->m_pFish->setScale(0.5f);
        fish[16]->m_pFish->setScale(0.5f);
        fish[17]->m_pFish->setScale(0.35f);
        
        fish[18]->m_pFish->setScale(0.3f);
        fish[19]->m_pFish->setScale(0.35f);
        fish[20]->m_pFish->setScale(0.4f);
        fish[21]->m_pFish->setScale(0.3f);
        fish[22]->m_pFish->setScale(0.3f);
        fish[23]->m_pFish->setScale(0.3f);
        
        fish[24]->m_pFish->setScale(0.3f);
        fish[25]->m_pFish->setScale(0.3f);
        
        fish[26]->m_pFish->setScale(0.35f);
        fish[27]->m_pFish->setScale(0.3f);
        fish[28]->m_pFish->setScale(0.3f);
        fish[29]->m_pFish->setScale(0.3f);
        fish[29]->m_pFish->setScale(0.3f);
        fish[30]->m_pFish->setScale(0.3f);
        fish[31]->m_pFish->setScale(0.5f);
        for(int i = 0; i < 4; i++)
        {
            CCArray* childFishAry = fish[28+i]->m_pFish->getChildren();
            if (childFishAry!=NULL)
            {
                for (unsigned int i = 0; i < childFishAry->count(); i++)
                {
                    CCSprite* childFish = (CCSprite*)childFishAry->objectAtIndex(i);
                    CCArray* childShadowAry = childFish->getChildren();
                    if (childShadowAry!=NULL)
                    {
                        for (unsigned int j = 0; j < childShadowAry->count(); j++)
                        {
                            CCSprite* childShadow = (CCSprite*)childShadowAry->objectAtIndex(j);
                            childShadow->setVisible(false);
                        }
                    }
                }
            }
        }
        
        
        for (unsigned int i = 0; i < nFishCount; i++)
        {
            delete fish[i];
        }
    }
    else if(CMainLogic::sharedMainLogic()->KIND_ID == NZNH_KIND_ID)
    {
        CCScrollView* m_helpScrollView = CCScrollView::create();
        CCLayer* containerLayer = CCLayer::create();
        containerLayer -> setContentSize(CCSizeMake(1000, 760));
        containerLayer -> setAnchorPoint(CCPointZero);
        containerLayer -> setPosition(ccp(0,-310));
        m_helpScrollView->setPosition(ccp(0,30));     //根据cocostudio调整的位置
        m_helpScrollView->setViewSize(CCSizeMake(1000, 450));//设置view的大小
        m_helpScrollView->setContentSize(CCSizeMake(1000, 760));//设置scrollview区域的大小
        m_helpScrollView->setContainer(containerLayer); //设置需要滚动的内容
        m_helpScrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
        m_helpScrollView->setBounceable(true);
        m_pHelpBg->addChild(m_helpScrollView);
        CCSize scrollSize = m_helpScrollView->getContentSize();
        int nFishCount = 37;
        GameFish* fish[nFishCount];
        CCLabelTTF* mulTTF[nFishCount];
        const char* str[37] = {"x2","x2","x3","x4","x5","x6","x7","x8","x9","x10","x12","x15","x18","x20","25","30","x40","x60","x80","x100","x150","x200","x250","x300","x300","50","x3-330","全屏炸弹","半屏炸弹","定屏","x18","x24","x30","x36","x48","x60","出红鱼"};
        for(int i = 0; i < nFishCount; i++)
        {
            mulTTF[i] = CCLabelTTF::create(str[i], "Arial", 24);
        }
        const int nFishID[37] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36};
        for(int i = 0; i < nFishCount; i++)
        {
            fish[i] = NULL;
            fish[i] = GameMainScene::GetInstance()->createFishByID(1000+nFishID[i]);
            containerLayer->addChild(fish[i]->m_pFish);
            fish[i]->m_pFish->setPosition(ccp(110+160*(i%6), 720-107*(i/6)));
            mulTTF[i]->setPosition(ccp(110+160*(i%6), 685-112*(i/6)));
            mulTTF[i]->setColor(ccc3(179, 119, 31));
            containerLayer->addChild(mulTTF[i]);
        }
        
        fish[0]->m_pFish->setScale(1.0f);
        fish[1]->m_pFish->setScale(1.1f);
        fish[2]->m_pFish->setScale(1.0f);
        fish[3]->m_pFish->setScale(1.0f);
        fish[4]->m_pFish->setScale(0.8f);
        fish[5]->m_pFish->setScale(0.7f);
        
        fish[6]->m_pFish->setScale(1.0f);
        fish[7]->m_pFish->setScale(0.65f);
        fish[8]->m_pFish->setScale(0.7f);
        fish[9]->m_pFish->setScale(0.7f);
        fish[10]->m_pFish->setScale(0.7f);
        fish[11]->m_pFish->setScale(0.5f);
        
        fish[12]->m_pFish->setScale(0.6f);
        fish[13]->m_pFish->setScale(0.6f);
        fish[14]->m_pFish->setScale(0.4f);
        fish[15]->m_pFish->setScale(0.5f);
        fish[16]->m_pFish->setScale(0.5f);
        fish[17]->m_pFish->setScale(0.5f);
        
        fish[18]->m_pFish->setScale(0.3f);
        fish[19]->m_pFish->setScale(0.35f);
        fish[20]->m_pFish->setScale(0.5f);
        fish[21]->m_pFish->setScale(0.3f);
        fish[22]->m_pFish->setScale(0.35f);
        fish[23]->m_pFish->setScale(0.25f);
        
        fish[24]->m_pFish->setScale(0.4f);
        fish[25]->m_pFish->setScale(0.5f);
        fish[26]->m_pFish->setScale(0.35f);
        
        fish[27]->m_pFish->setScale(0.4f);
        fish[28]->m_pFish->setScale(0.4f);
        fish[29]->m_pFish->setScale(0.3f);
        
        fish[30]->m_pFish->setScale(0.3f);
        fish[31]->m_pFish->setScale(0.3f);
        fish[32]->m_pFish->setScale(0.3f);
        fish[33]->m_pFish->setScale(0.3f);
        fish[34]->m_pFish->setScale(0.3f);
        fish[35]->m_pFish->setScale(0.3f);
        fish[36]->m_pFish->setScale(0.3f);
        for(int i = 0; i < 7; i++)
        {
            CCArray* childFishAry = fish[30+i]->m_pFish->getChildren();
            if (childFishAry!=NULL)
            {
                for (unsigned int i = 0; i < childFishAry->count(); i++)
                {
                    CCSprite* childFish = (CCSprite*)childFishAry->objectAtIndex(i);
                    CCArray* childShadowAry = childFish->getChildren();
                    if (childShadowAry!=NULL)
                    {
                        for (unsigned int j = 0; j < childShadowAry->count(); j++)
                        {
                            CCSprite* childShadow = (CCSprite*)childShadowAry->objectAtIndex(j);
                            childShadow->setVisible(false);
                        }
                    }
                }
            }
        }
        
        for (unsigned int i = 0; i < nFishCount; i++)
        {
            delete fish[i];
        }
    }
    else
    {
        CCScrollView* m_helpScrollView = CCScrollView::create();
        CCLayer* containerLayer = CCLayer::create();
        containerLayer -> setContentSize(CCSizeMake(1000, 760));
        containerLayer -> setAnchorPoint(CCPointZero);
        containerLayer -> setPosition(ccp(0,-310));
        m_helpScrollView->setPosition(ccp(0,30));     //根据cocostudio调整的位置
        m_helpScrollView->setViewSize(CCSizeMake(1000, 450));//设置view的大小
        m_helpScrollView->setContentSize(CCSizeMake(1000, 760));//设置scrollview区域的大小
        m_helpScrollView->setContainer(containerLayer); //设置需要滚动的内容
        m_helpScrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
        m_helpScrollView->setBounceable(true);
        m_pHelpBg->addChild(m_helpScrollView);
        CCSize scrollSize = m_helpScrollView->getContentSize();
        //const int JCBYFishID[36] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,30,31,32,33,34,35,36,37,26,40};
        GameFish* fish[38];
        CCLabelTTF* mulTTF[38];
        const char* str[38] = {"x2","x2","x3","x4","x5","x6","x7","x8","x9","x10","x12","x15","x18","x20","25","30","x40","x100","x150","x200","x250","x250","x25","定屏","半屏炸弹","全屏炸弹","x300","x300","x3-330","x12","x16","x32","x40","x42","x49","x90","x100","出红鱼"};
        for(int i = 0; i < 38; i++)
        {
            mulTTF[i] = CCLabelTTF::create(str[i], "Arial", 24);
        }
        const int nFishID[38] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,30,31,32,33,34,35,36,37,40};
        for(int i = 0; i < 38; i++)
        {
            fish[i] = NULL;
            fish[i] = GameMainScene::GetInstance()->createFishByID(1000+nFishID[i]);
            containerLayer->addChild(fish[i]->m_pFish);
            fish[i]->m_pFish->setPosition(ccp(110+160*(i%6), 720-107*(i/6)));
            mulTTF[i]->setPosition(ccp(110+160*(i%6), 685-112*(i/6)));
            mulTTF[i]->setColor(ccc3(179, 119, 31));
            containerLayer->addChild(mulTTF[i]);
        }
        
        fish[0]->m_pFish->setScale(1.0f);
        fish[1]->m_pFish->setScale(1.2f);
        fish[2]->m_pFish->setScale(1.2f);
        fish[3]->m_pFish->setScale(1.0f);
        fish[4]->m_pFish->setScale(1.0f);
        fish[5]->m_pFish->setScale(0.8f);
        
        fish[6]->m_pFish->setScale(1.0f);
        fish[7]->m_pFish->setScale(0.65f);
        fish[8]->m_pFish->setScale(0.9f);
        fish[9]->m_pFish->setScale(0.7f);
        fish[10]->m_pFish->setScale(0.7f);
        fish[11]->m_pFish->setScale(0.5f);
        
        fish[12]->m_pFish->setScale(0.6f);
        fish[13]->m_pFish->setScale(0.6f);
        fish[14]->m_pFish->setScale(0.4f);
        fish[15]->m_pFish->setScale(0.5f);
        fish[16]->m_pFish->setScale(0.5f);
        fish[17]->m_pFish->setScale(0.4f);
        
        fish[18]->m_pFish->setScale(0.4f);
        fish[19]->m_pFish->setScale(0.4f);
        fish[20]->m_pFish->setScale(0.25f);//
        fish[21]->m_pFish->setScale(0.3f);//
        fish[22]->m_pFish->setScale(0.5f);
        fish[23]->m_pFish->setScale(0.25f);
        
        fish[24]->m_pFish->setScale(0.4f);
        fish[25]->m_pFish->setScale(0.5f);
        fish[26]->m_pFish->setScale(0.3f);
        fish[27]->m_pFish->setScale(0.3f);
        fish[28]->m_pFish->setScale(0.3f);
        fish[29]->m_pFish->setScale(0.3f);
        
        fish[30]->m_pFish->setScale(0.3f);
        fish[31]->m_pFish->setScale(0.3f);
        fish[32]->m_pFish->setScale(0.3f);
        fish[33]->m_pFish->setScale(0.3f);
        fish[34]->m_pFish->setScale(0.3f);
        fish[35]->m_pFish->setScale(0.3f);
        
        fish[36]->m_pFish->setScale(0.3f);
        fish[37]->m_pFish->setScale(0.3f);
        for(int i = 0; i < 4; i++)
        {
            fish[29+i]->m_pFish->setRotation(270);
        }
        
        for(int i = 0; i < 9; i++)
        {
            CCArray* childFishAry = fish[29+i]->m_pFish->getChildren();
            if (childFishAry!=NULL)
            {
                for (unsigned int i = 0; i < childFishAry->count(); i++)
                {
                    CCSprite* childFish = (CCSprite*)childFishAry->objectAtIndex(i);
                    CCArray* childShadowAry = childFish->getChildren();
                    if (childShadowAry!=NULL)
                    {
                        for (unsigned int j = 0; j < childShadowAry->count(); j++)
                        {
                            CCSprite* childShadow = (CCSprite*)childShadowAry->objectAtIndex(j);
                            childShadow->setVisible(false);
                        }
                    }
                }
            }
        }
        
        for (unsigned int i = 0; i < 38; i++)
        {
            delete fish[i];
        }
    }
    
}

void FishWndLayer::showHelpSprite()
{
    m_pLayerColor -> setVisible(true);
    m_pHelpBg -> setScale(0.0f);
    m_pCalcBg -> setVisible(false);
    m_pHelpBg -> setVisible(true);
    m_pHelpBg->runAction(CCSequence::create(CCFadeIn::create(0), CCScaleTo::create(0, 0.6f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

#pragma mark -- WndCalc
void FishWndLayer::loadWndCalcSprite()
{
    if(CMainLogic::sharedMainLogic()->KIND_ID == NZNH_KIND_ID)
    {
        CCScrollView* m_helpScrollView = CCScrollView::create();
        m_containerLayer = CCLayer::create();
        m_containerLayer -> setContentSize(CCSizeMake(1000, 760));
        m_containerLayer -> setAnchorPoint(CCPointZero);
        m_containerLayer -> setPosition(ccp(0,-310));
        m_helpScrollView->setPosition(ccp(0,30));     //根据cocostudio调整的位置
        m_helpScrollView->setViewSize(CCSizeMake(1000, 450));//设置view的大小
        m_helpScrollView->setContentSize(CCSizeMake(1000, 760));//设置scrollview区域的大小
        m_helpScrollView->setContainer(m_containerLayer); //设置需要滚动的内容
        m_helpScrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
        m_helpScrollView->setBounceable(true);
        m_pCalcBg->addChild(m_helpScrollView);
        CCSize scrollSize = m_helpScrollView->getContentSize();
        int nFishCount = 37;
        GameFish* fish[nFishCount];
        CCLabelTTF* mulTTF[nFishCount];
        const char* str[37] = {"x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0"};
        for(int i = 0; i < nFishCount; i++)
        {
            mulTTF[i] = CCLabelTTF::create(str[i], "Arial", 24);
        }
        const int nFishID[37] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36};
        for(int i = 0; i < nFishCount; i++)
        {
            fish[i] = NULL;
            fish[i] = GameMainScene::GetInstance()->createFishByID(1000+nFishID[i]);
            m_containerLayer->addChild(fish[i]->m_pFish);
            fish[i]->m_pFish->setPosition(ccp(110+160*(i%6), 720-107*(i/6)));
            mulTTF[i]->setPosition(ccp(110+160*(i%6), 685-112*(i/6)));
            mulTTF[i]->setColor(ccc3(179, 119, 31));
            mulTTF[i]->setTag(1000+nFishID[i]);
            m_containerLayer->addChild(mulTTF[i]);
        }
        
        fish[0]->m_pFish->setScale(1.0f);
        fish[1]->m_pFish->setScale(1.1f);
        fish[2]->m_pFish->setScale(1.0f);
        fish[3]->m_pFish->setScale(1.0f);
        fish[4]->m_pFish->setScale(0.8f);
        fish[5]->m_pFish->setScale(0.7f);
        
        fish[6]->m_pFish->setScale(1.0f);
        fish[7]->m_pFish->setScale(0.65f);
        fish[8]->m_pFish->setScale(0.7f);
        fish[9]->m_pFish->setScale(0.7f);
        fish[10]->m_pFish->setScale(0.7f);
        fish[11]->m_pFish->setScale(0.5f);
        
        fish[12]->m_pFish->setScale(0.6f);
        fish[13]->m_pFish->setScale(0.6f);
        fish[14]->m_pFish->setScale(0.4f);
        fish[15]->m_pFish->setScale(0.5f);
        fish[16]->m_pFish->setScale(0.5f);
        fish[17]->m_pFish->setScale(0.5f);
        
        fish[18]->m_pFish->setScale(0.3f);
        fish[19]->m_pFish->setScale(0.35f);
        fish[20]->m_pFish->setScale(0.5f);
        fish[21]->m_pFish->setScale(0.3f);
        fish[22]->m_pFish->setScale(0.35f);
        fish[23]->m_pFish->setScale(0.25f);
        
        fish[24]->m_pFish->setScale(0.4f);
        fish[25]->m_pFish->setScale(0.5f);
        fish[26]->m_pFish->setScale(0.35f);
        
        fish[27]->m_pFish->setScale(0.4f);
        fish[28]->m_pFish->setScale(0.4f);
        fish[29]->m_pFish->setScale(0.3f);
        
        fish[30]->m_pFish->setScale(0.3f);
        fish[31]->m_pFish->setScale(0.3f);
        fish[32]->m_pFish->setScale(0.3f);
        fish[33]->m_pFish->setScale(0.3f);
        fish[34]->m_pFish->setScale(0.3f);
        fish[35]->m_pFish->setScale(0.3f);
        fish[36]->m_pFish->setScale(0.3f);
        for(int i = 0; i < 7; i++)
        {
            CCArray* childFishAry = fish[30+i]->m_pFish->getChildren();
            if (childFishAry!=NULL)
            {
                for (unsigned int i = 0; i < childFishAry->count(); i++)
                {
                    CCSprite* childFish = (CCSprite*)childFishAry->objectAtIndex(i);
                    CCArray* childShadowAry = childFish->getChildren();
                    if (childShadowAry!=NULL)
                    {
                        for (unsigned int j = 0; j < childShadowAry->count(); j++)
                        {
                            CCSprite* childShadow = (CCSprite*)childShadowAry->objectAtIndex(j);
                            childShadow->setVisible(false);
                        }
                    }
                }
            }
        }
        
        for (unsigned int i = 0; i < nFishCount; i++)
        {
            delete fish[i];
        }
    }
    else if(CMainLogic::sharedMainLogic()->KIND_ID == YQS_KIND_ID)
    {
        CCScrollView* m_helpScrollView = CCScrollView::create();
        m_containerLayer = CCLayer::create();
        m_containerLayer -> setContentSize(CCSizeMake(1000, 650));
        m_containerLayer -> setAnchorPoint(CCPointZero);
        m_containerLayer -> setPosition(ccp(0,-200));
        m_helpScrollView->setPosition(ccp(0,30));     //根据cocostudio调整的位置
        m_helpScrollView->setViewSize(CCSizeMake(1000, 450));//设置view的大小
        m_helpScrollView->setContentSize(CCSizeMake(1000, 650));//设置scrollview区域的大小
        m_helpScrollView->setContainer(m_containerLayer); //设置需要滚动的内容
        m_helpScrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
        m_helpScrollView->setBounceable(true);
        m_pCalcBg->addChild(m_helpScrollView);
        CCSize scrollSize = m_helpScrollView->getContentSize();
        int nFishCount = 32;
        GameFish* fish[nFishCount];
        CCLabelTTF* mulTTF[nFishCount];
        const char* str[32] = {"x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0"};
        for(int i = 0; i < nFishCount; i++)
        {
            mulTTF[i] = CCLabelTTF::create(str[i], "Arial", 24);
        }
        const int nFishID[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
        for(int i = 0; i < nFishCount; i++)
        {
            fish[i] = NULL;
            fish[i] = GameMainScene::GetInstance()->createFishByID(1000+nFishID[i]);
            m_containerLayer->addChild(fish[i]->m_pFish);
            fish[i]->m_pFish->setPosition(ccp(110+160*(i%6), 610-107*(i/6)));
            mulTTF[i]->setPosition(ccp(110+160*(i%6), 575-112*(i/6)));
            mulTTF[i]->setColor(ccc3(179, 119, 31));
            mulTTF[i]->setTag(1000+nFishID[i]);
            m_containerLayer->addChild(mulTTF[i]);
        }
        
        fish[0]->m_pFish->setScale(1.0f);
        fish[1]->m_pFish->setScale(1.2f);
        fish[2]->m_pFish->setScale(1.2f);
        fish[3]->m_pFish->setScale(1.0f);
        fish[4]->m_pFish->setScale(1.0f);
        fish[5]->m_pFish->setScale(0.8f);
        
        fish[6]->m_pFish->setScale(1.0f);
        fish[7]->m_pFish->setScale(0.65f);
        fish[8]->m_pFish->setScale(0.9f);
        fish[9]->m_pFish->setScale(0.7f);
        fish[10]->m_pFish->setScale(0.7f);
        fish[11]->m_pFish->setScale(0.5f);
        
        fish[12]->m_pFish->setScale(0.6f);
        fish[13]->m_pFish->setScale(0.6f);
        fish[14]->m_pFish->setScale(0.4f);
        fish[15]->m_pFish->setScale(0.5f);
        fish[16]->m_pFish->setScale(0.5f);
        fish[17]->m_pFish->setScale(0.35f);
        
        fish[18]->m_pFish->setScale(0.3f);
        fish[19]->m_pFish->setScale(0.35f);
        fish[20]->m_pFish->setScale(0.4f);
        fish[21]->m_pFish->setScale(0.3f);
        fish[22]->m_pFish->setScale(0.3f);
        fish[23]->m_pFish->setScale(0.3f);
        
        fish[24]->m_pFish->setScale(0.3f);
        fish[25]->m_pFish->setScale(0.3f);
        
        fish[26]->m_pFish->setScale(0.35f);
        fish[27]->m_pFish->setScale(0.3f);
        fish[28]->m_pFish->setScale(0.3f);
        fish[29]->m_pFish->setScale(0.3f);
        fish[29]->m_pFish->setScale(0.3f);
        fish[30]->m_pFish->setScale(0.3f);
        fish[31]->m_pFish->setScale(0.5f);
        for(int i = 0; i < 4; i++)
        {
            CCArray* childFishAry = fish[28+i]->m_pFish->getChildren();
            if (childFishAry!=NULL)
            {
                for (unsigned int i = 0; i < childFishAry->count(); i++)
                {
                    CCSprite* childFish = (CCSprite*)childFishAry->objectAtIndex(i);
                    CCArray* childShadowAry = childFish->getChildren();
                    if (childShadowAry!=NULL)
                    {
                        for (unsigned int j = 0; j < childShadowAry->count(); j++)
                        {
                            CCSprite* childShadow = (CCSprite*)childShadowAry->objectAtIndex(j);
                            childShadow->setVisible(false);
                        }
                    }
                }
            }
        }
        
        
        for (unsigned int i = 0; i < nFishCount; i++)
        {
            delete fish[i];
        }
    }
    else
    {
        CCScrollView* m_helpScrollView = CCScrollView::create();
        m_containerLayer = CCLayer::create();
        m_containerLayer -> setContentSize(CCSizeMake(1000, 760));
        m_containerLayer -> setAnchorPoint(CCPointZero);
        m_containerLayer -> setPosition(ccp(0,-310));
        m_helpScrollView->setPosition(ccp(0,30));     //根据cocostudio调整的位置
        m_helpScrollView->setViewSize(CCSizeMake(1000, 450));//设置view的大小
        m_helpScrollView->setContentSize(CCSizeMake(1000, 760));//设置scrollview区域的大小
        m_helpScrollView->setContainer(m_containerLayer); //设置需要滚动的内容
        m_helpScrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
        m_helpScrollView->setBounceable(true);
        m_pCalcBg->addChild(m_helpScrollView);
        CCSize scrollSize = m_helpScrollView->getContentSize();
        GameFish* fish[38];
        CCLabelTTF* mulTTF[38];
        const char* str[38] = {"x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0","x0"};
        for(int i = 0; i < 38; i++)
        {
            mulTTF[i] = CCLabelTTF::create(str[i], "Arial", 24);
        }
        const int nFishID[38] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,30,31,32,33,34,35,36,37,40};
        for(int i = 0; i < 38; i++)
        {
            fish[i] = NULL;
            fish[i] = GameMainScene::GetInstance()->createFishByID(1000+nFishID[i]);
            m_containerLayer->addChild(fish[i]->m_pFish);
            fish[i]->m_pFish->setPosition(ccp(110+160*(i%6), 720-107*(i/6)));
            mulTTF[i]->setPosition(ccp(110+160*(i%6), 685-112*(i/6)));
            mulTTF[i]->setColor(ccc3(179, 119, 31));
            mulTTF[i]->setTag(1000+nFishID[i]);
            m_containerLayer->addChild(mulTTF[i]);
        }
        fish[0]->m_pFish->setScale(1.0f);
        fish[1]->m_pFish->setScale(1.2f);
        fish[2]->m_pFish->setScale(1.2f);
        fish[3]->m_pFish->setScale(1.0f);
        fish[4]->m_pFish->setScale(1.0f);
        fish[5]->m_pFish->setScale(0.8f);
        
        fish[6]->m_pFish->setScale(1.0f);
        fish[7]->m_pFish->setScale(0.65f);
        fish[8]->m_pFish->setScale(0.9f);
        fish[9]->m_pFish->setScale(0.7f);
        fish[10]->m_pFish->setScale(0.7f);
        fish[11]->m_pFish->setScale(0.5f);
        
        fish[12]->m_pFish->setScale(0.6f);
        fish[13]->m_pFish->setScale(0.6f);
        fish[14]->m_pFish->setScale(0.4f);
        fish[15]->m_pFish->setScale(0.5f);
        fish[16]->m_pFish->setScale(0.5f);
        fish[17]->m_pFish->setScale(0.4f);
        
        fish[18]->m_pFish->setScale(0.4f);
        fish[19]->m_pFish->setScale(0.4f);
        fish[20]->m_pFish->setScale(0.25f);//
        fish[21]->m_pFish->setScale(0.3f);//
        fish[22]->m_pFish->setScale(0.5f);
        fish[23]->m_pFish->setScale(0.25f);
        
        fish[24]->m_pFish->setScale(0.4f);
        fish[25]->m_pFish->setScale(0.5f);
        fish[26]->m_pFish->setScale(0.3f);
        fish[27]->m_pFish->setScale(0.3f);
        fish[28]->m_pFish->setScale(0.3f);
        fish[29]->m_pFish->setScale(0.3f);
        
        fish[30]->m_pFish->setScale(0.3f);
        fish[31]->m_pFish->setScale(0.3f);
        fish[32]->m_pFish->setScale(0.3f);
        fish[33]->m_pFish->setScale(0.3f);
        fish[34]->m_pFish->setScale(0.3f);
        fish[35]->m_pFish->setScale(0.3f);
        
        fish[36]->m_pFish->setScale(0.3f);
        fish[37]->m_pFish->setScale(0.3f);
        for(int i = 0; i < 4; i++)
        {
            fish[29+i]->m_pFish->setRotation(270);
        }
        
        for(int i = 0; i < 9; i++)
        {
            CCArray* childFishAry = fish[29+i]->m_pFish->getChildren();
            if (childFishAry!=NULL)
            {
                for (unsigned int i = 0; i < childFishAry->count(); i++)
                {
                    CCSprite* childFish = (CCSprite*)childFishAry->objectAtIndex(i);
                    CCArray* childShadowAry = childFish->getChildren();
                    if (childShadowAry!=NULL)
                    {
                        for (unsigned int j = 0; j < childShadowAry->count(); j++)
                        {
                            CCSprite* childShadow = (CCSprite*)childShadowAry->objectAtIndex(j);
                            childShadow->setVisible(false);
                        }
                    }
                }
            }
        }
        
        for (unsigned int i = 0; i < 38; i++)
        {
            delete fish[i];
        }
    }
}

void FishWndLayer::showWndCalcSprite()
{
    m_pLayerColor -> setVisible(true);
    m_pCalcBg -> setScale(0.0f);
    m_pHelpBg -> setVisible(false);
    m_pCalcBg -> setVisible(true);
    m_pCalcBg->runAction(CCSequence::create(CCFadeIn::create(0), CCScaleTo::create(0, 0.6f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void FishWndLayer::updateCalcWnd(int nFishType[], int nFishCount[], int nTotalCount)
{
    char temp[10];
    for(int i = 0; i < nTotalCount; i++)
    {
        CCLabelTTF* label = (CCLabelTTF*)m_containerLayer->getChildByTag(nFishType[i]);
        if(label != NULL)
        {
            sprintf(temp,"x%d",nFishCount[i]);
            label -> setString(temp);
        }
    }
}

void FishWndLayer::exitGame(CCObject* object)
{
    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameExit2);
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CMainLogic::sharedMainLogic()->m_bEnterGiveCoin = false;
    CMainLogic::sharedMainLogic()->m_bGameToVIP = false;
    CMainLogic::sharedMainLogic()->m_bBYGameToSuper = false;
    CMainLogic::sharedMainLogic()->m_nExitGameToSpecialArm = 0;
    CMainLogic::sharedMainLogic()->m_bGameChargeStatus = false;
    CMainLogic::sharedMainLogic()->m_nShowCoinArmSkill = 0;
    CMainLogic::sharedMainLogic()->m_bBuyTryCannon = false;
    GameMainScene::GetInstance()->exitGame();
}
