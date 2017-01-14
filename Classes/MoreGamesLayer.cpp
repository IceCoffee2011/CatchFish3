#include "header.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "FMLayerWebView.h"
#endif

MoreGamesLayer* MoreGamesLayer::create()
{
    MoreGamesLayer* pRet = new MoreGamesLayer;
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return pRet;
    }
}

bool MoreGamesLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_vMoreGamesInfo.clear();
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    loadUI();
    showAnimate();
    this -> setTouchEnabled(true);
    
    return true;
}

void MoreGamesLayer::loadUI()
{
    m_spBg = CCSprite::create("h2all/moregamesbg.png");
    m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spBg);
    
    CCMenuItemImage* closeButton = CCMenuItemImage::create("mail/mailcloseItem0.png","mail/mailcloseItem1.png",this,menu_selector(MoreGamesLayer::clickClose));
    closeButton->setPosition(ccp(m_spBg->getContentSize().width-20, m_spBg->getContentSize().height-20));
    CCMenu* pMenuClose = CCMenu::create(closeButton, NULL);
    pMenuClose->setPosition(CCPointZero);
    pMenuClose->setAnchorPoint(CCPointZero);
    m_spBg->addChild(pMenuClose, 2);
    pMenuClose->setTouchPriority(-200);
}

void MoreGamesLayer::showAnimate()
{
    m_spAnimate = CCSprite::create("h2all/loading1.png");
    m_spAnimate -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2));
    m_spBg -> addChild(m_spAnimate);
    m_spAnimate -> runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360)));
}

void MoreGamesLayer::loadScrollView(std::vector<MoreGamesInfo> vMoreGamesInfo)
{
    CHallLayer* pHallLayer = (CHallLayer*)this->getParent();
    if (pHallLayer != NULL)
    {
        pHallLayer->unschedule(schedule_selector(CHallLayer::TimerToLoadMoreGames));
    }
    m_spAnimate->stopAllActions();
    m_spAnimate->removeFromParent();
    
    for (int i=0; i<vMoreGamesInfo.size(); i++)
    {
        m_vMoreGamesInfo.push_back(vMoreGamesInfo[i]);
    }
    
    int count = (int)vMoreGamesInfo.size();
    CCLOG("count=%d",count);
    m_scrollView = CCScrollView::create();
    m_containLayer = CCLayer::create();
    m_containLayer -> setContentSize(CCSizeMake(896, 220*count));
    m_containLayer -> setAnchorPoint(CCPointZero);
    m_containLayer -> setPosition(ccp(0,-220*count+440));
    m_scrollView->setPosition(ccp(20,55));     //根据cocostudio调整的位置
    m_scrollView->setViewSize(CCSizeMake(896, 440));//设置view的大小
    m_scrollView->setContentOffset(ccp(0,0));
    m_scrollView->setContentSize(CCSizeMake(896, 220*count));//设置scrollview区域的大小
    m_scrollView->setContainer(m_containLayer); //设置需要滚动的内容
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);  //设置滚动的方向，有三种可以选择
    m_scrollView->setBounceable(false);
    m_spBg->addChild(m_scrollView);
    m_scrollView -> setTouchEnabled(true);
    m_scrollView -> setTouchPriority(-200);
    
    for(int i = 0; i < count; i++)
    {
        CCSprite* pSpCellbg = CCSprite::create("h2all/moregamescell.png");
        pSpCellbg -> setPosition(ccp(896/2, 220*count-220*i-110));
        m_containLayer->addChild(pSpCellbg);
        CCSize size(480, 100);
        CCLabelTTF* pLabelInfo = CCLabelTTF::create(m_vMoreGamesInfo[i].sIntroduction.c_str(), "Arial", 28, size,kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
        pLabelInfo -> setPosition(ccp(pSpCellbg->getContentSize().width-260, pSpCellbg->getContentSize().height/2+30));
        pSpCellbg -> addChild(pLabelInfo);
        pLabelInfo -> setColor(ccc3(0, 0, 0));
        std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
        std::string str = path + m_vMoreGamesInfo[i].sImageName;
        CCSprite* pSpPic = CCSprite::create(str.c_str());
        if(pSpPic != NULL)
        {
            pSpPic -> setPosition(ccp(pSpPic->getContentSize().width/2+15, pSpCellbg->getContentSize().height/2));
            pSpCellbg->addChild(pSpPic);
        }
        
        //检测手机上是否存在本游戏，如果不存在，则现在下载按钮，如果存在，则显示打开按钮
		bool bExit = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        bExit = ChargeWebView::canExitFishGame(m_vMoreGamesInfo[i].sIdentifier.c_str());
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        bExit = getExitOtherGame(m_vMoreGamesInfo[i].sPackage);
#endif
        CCMenuItemImage* gameItem = NULL;
        if(bExit)
        {
            gameItem = CCMenuItemImage::create("h2all/moregameItem3.png", "h2all/moregameItem4.png", this, menu_selector(MoreGamesLayer::clickOpenMenu));
        }
        else
        {
            gameItem = CCMenuItemImage::create("h2all/moregameItem1.png", "h2all/moregameItem2.png", this, menu_selector(MoreGamesLayer::clickDownLoadMenu));
        }
        gameItem -> setTag(m_vMoreGamesInfo[i].nID);
        gameItem -> setPosition(ccp(pSpCellbg->getContentSize().width/3*2+20, gameItem->getContentSize().height/2+10));
        CCMenu* pGameMenu = CCMenu::create(gameItem, NULL);
        pGameMenu->setPosition(CCPointZero);
        pGameMenu->setAnchorPoint(CCPointZero);
        pSpCellbg->addChild(pGameMenu);
        pGameMenu->setTouchPriority(-200);
    }
}

void MoreGamesLayer::clickClose(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CHallLayer * pHallLayer = (CHallLayer *)this->getParent();
    if (pHallLayer != NULL)
    {
        pHallLayer->unschedule(schedule_selector(CHallLayer::TimerToLoadMoreGames));
    }
    this->removeFromParent();
}

void MoreGamesLayer::clickDownLoadMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    int nTag = item->getTag();
    for(int i = 0; i < m_vMoreGamesInfo.size(); i++)
    {
        if(nTag == m_vMoreGamesInfo[i].nID)
        {
            //打开游戏的下载地址
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            ChargeWebView::linkToMoreGamesDownLoad(m_vMoreGamesInfo[i].sDownLoadURL);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            linkToMoreGamesDownLoad(m_vMoreGamesInfo[i].sDownLoadURL);
#endif 
            this->removeFromParent();
            break;
        }
    }
}

void MoreGamesLayer::clickOpenMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    int nTag = item->getTag();
    for(int i = 0; i < m_vMoreGamesInfo.size(); i++)
    {
        if(nTag == m_vMoreGamesInfo[i].nID)
        {
            //跳转到另一个游戏
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            ChargeWebView::openFishGame(m_vMoreGamesInfo[i].sIdentifier);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            openOtherGame(m_vMoreGamesInfo[i].sPackage);
#endif
            this->removeFromParent();
            break;
        }
    }
}

//获取Android的所有应用，检测是否存在此游戏
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
bool MoreGamesLayer::getExitOtherGame(std::string str)
{
    JniMethodInfo info;
    bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if(bHave)
    {
        jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
        
        bool bHave1 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","isAvilible2", "(Ljava/lang/String;)Z");
        if(bHave1)
        {
            jstring appidjStr = info.env->NewStringUTF(str.c_str());
            bool bExist = (bool)info.env->CallBooleanMethod(jobj, info.methodID, appidjStr);
            return bExist;
        }
    }
    return false;
}

//打开存在的游戏
void MoreGamesLayer::openOtherGame(std::string str)
{
    JniMethodInfo info;
    bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if(bHave)
    {
        jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
        bool bHave1 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","openMoreGame", "(Ljava/lang/String;)V");
        CCLOG("BBBBBB........");
        if(bHave1)
        {
            CCLOG("CCCCCC........");
            jstring appidjStr = info.env->NewStringUTF(str.c_str());
            info.env->CallVoidMethod(jobj, info.methodID, appidjStr);
        }
    }
}

//不存在的游戏，打开下载链接
void MoreGamesLayer::linkToMoreGamesDownLoad(std::string str)
{
    JniMethodInfo info;
    bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if(bHave)
    {
        jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
        bool bHave1 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","openGameLoad", "(Ljava/lang/String;)V");
        if(bHave1)
        {
            jstring appidjStr = info.env->NewStringUTF(str.c_str());
            info.env->CallVoidMethod(jobj, info.methodID, appidjStr);
        }
    }
}
#endif

void MoreGamesLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool MoreGamesLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

void MoreGamesLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    
}
