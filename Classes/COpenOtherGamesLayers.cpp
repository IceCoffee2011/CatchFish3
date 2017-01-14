//
//  COpenOtherGamesLayers.cpp
//  otherGames
//
//  Created by Guang on 15/12/7.
//
//

#include "header.h"
OpenOtherGamesLayer::~OpenOtherGamesLayer()
{
}

OpenOtherGamesLayer* OpenOtherGamesLayer::create()
{
    OpenOtherGamesLayer* pRet = new OpenOtherGamesLayer;
    if(pRet && pRet->init())
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

bool OpenOtherGamesLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }

    m_fStart=0;
    m_ContentOffset=0;
    m_nPage=0;
    m_nAdCount=0;

    m_nAdCount = (int)CMainLogic::sharedMainLogic()->m_vNewAdvertInfo.size();
    
    for (int i=0; i<m_nAdCount; i++)
    {
        std::string PicName = CMainLogic::sharedMainLogic()->m_vNewAdvertInfo[0].sImageName;
        std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
        std::string Picpath = path + PicName;
        CCSprite * Img = CCSprite::create(Picpath.c_str());
        if (NULL == Img)
        {
            continue;
        }
        else
        {
            m_Size = Img->boundingBox().size;
            break;
        }
    }
    
    CCLayerColor * baseLayer = CCLayerColor::create(ccc4(0, 0, 0, 190));
    baseLayer->setAnchorPoint(ccp(0, 0.5));
    baseLayer->setPosition(ccp(0, 0));
    baseLayer->setTag(100);
    this->addChild(baseLayer);
    
    
    
    m_OtherGamesView = CCScrollView::create(CCSizeMake(m_Size.width, m_Size.height));
    m_OtherGamesView->setPosition(ccp(640-m_Size.width/2, 360-m_Size.height/2));
    m_OtherGamesView->setTouchPriority(-200);
    baseLayer->addChild(m_OtherGamesView);
    m_containLayer = CCLayer::create();
    m_containLayer->setContentSize(CCSizeMake(m_Size.width*m_nAdCount, m_Size.height));
    m_OtherGamesView->setContainer(m_containLayer);
    m_OtherGamesView->setContentOffset(ccp(0, 0));
    
    
    m_OtherGamesView->setDirection(kCCScrollViewDirectionHorizontal);//设置滑动方向
    m_OtherGamesView->setBounceable(false);
    m_OtherGamesView->setDelegate(this);
    SetGameImg();
    loadBall();
    
    CCMenuItemImage * closeItem = CCMenuItemImage::create("mail/advertItem1.png","mail/advertItem2.png",this,menu_selector(OpenOtherGamesLayer::closeOtherGamesWnd));
    CCMenu * closeMenu = CCMenu::create(closeItem, NULL);
    //closeMenu -> setPosition(ccp(m_Size.width-10, m_Size.height-10));
    closeMenu->setPosition(ccp(640+m_Size.width/2, 360+m_Size.height/2));
    closeMenu->setTouchPriority(-200);
    this->addChild(closeMenu,100);
    
    this->setTouchEnabled(true);
    
    
    return true;
}

void OpenOtherGamesLayer::SetGameImg()
{
    int count = (int)CMainLogic::sharedMainLogic()->m_vNewAdvertInfo.size();
    for (int i=0; i<m_nAdCount; i++)
    {
        std::string PicName = CMainLogic::sharedMainLogic()->m_vNewAdvertInfo[i].sImageName;
        std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
        std::string Picpath = path + PicName;
        CCSprite * Img = CCSprite::create(Picpath.c_str());
		if(Img != NULL)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            Img->setPosition(ccp(m_Size.width/2 + m_Size.width*i, m_Size.height/2));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            Img->setPosition(ccp(m_Size.width/2 - 2 + m_Size.width*i, m_Size.height/2));
#endif
			Img->setTag(CMainLogic::sharedMainLogic()->m_vNewAdvertInfo[i].nID);
			m_containLayer->addChild(Img);
		}
        
    }
}

void OpenOtherGamesLayer::loadBall()
{
    if(m_nAdCount == 1)
        return;
    for (int i=0; i<m_nAdCount; i++)
    {
        CCLayer * layer = (CCLayer *)this->getChildByTag(100);
        
        CCSprite * pBall = CCSprite::create("freecoin/ball1.png");
        
        int nMiddleLocation = m_nAdCount/2;
        float fHalf = ((m_nAdCount+1)%2)*0.5; //当总数为偶数时，这个值为0.5；奇数时，为0
        float xPos = 640 + (i - (nMiddleLocation - fHalf))*56;
        
        pBall->setPosition(ccp(xPos, 85));
        layer->addChild(pBall, 2, i);
        pBall -> setScale(0.7f);
        if (i == m_nPage)
        {
            CCSprite * pBall2 = CCSprite::create("freecoin/ball2.png");
            pBall2->setScale(0.7);
            pBall2->setPosition(ccp(xPos, 85));
            layer->addChild(pBall2, 3, 101);
        }
    }
}

void OpenOtherGamesLayer::scrollViewDidScroll(CCScrollView* view)
{
}

void OpenOtherGamesLayer::scrollViewDidZoom(CCScrollView* view)
{
}

void OpenOtherGamesLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool OpenOtherGamesLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint p = pTouch->getLocation();
    m_fStart = p.x;
    return true;
}
void OpenOtherGamesLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint p = pTouch->getLocation();
    float fEnd = p.x;
    float distance = fEnd - m_fStart;
    
    if (fabs(distance) < 5 )
    {
        p = m_containLayer->convertToNodeSpace(p);
        if (p.x<0 || p.x>m_containLayer->boundingBox().size.width)
        {
            return;
        }
        else
        {
            confirmGamePicture(p);
        }
    }
    else if (fabs(distance) >= 80)
    {
        adjustScrollView(distance);
    }
    else
    {
        adjustScrollView(0);
    }
}

void OpenOtherGamesLayer::adjustScrollView(float offset)
{
    if (offset < 0)
    {
        m_nPage++;
    }
    else if (offset > 0)
    {
        m_nPage--;
    }
    
    if (m_nPage < 0)
    {
        m_nPage = 0;
    }
    else if (m_nPage > (m_nAdCount - 1))
    {
        m_nPage = (int)(m_nAdCount - 1);
    }
    float adjust_xPos = -m_Size.width*m_nPage;
    m_xPos = adjust_xPos;
    m_OtherGamesView->setContentOffsetInDuration(ccp(adjust_xPos,0), 0.2f);
    this->schedule(schedule_selector(OpenOtherGamesLayer::TimerToSetScrollViewTouch), 0.2f);
    setBallPosition();
}

void OpenOtherGamesLayer::setBallPosition()
{
    if(m_nAdCount == 1)
        return;
    CCLayer * layer = (CCLayer *)this->getChildByTag(100);
    CCSprite * pBall2 = (CCSprite *)layer->getChildByTag(101);
    
    int nMiddleLocation = m_nAdCount/2;
    float fHalf = ((m_nAdCount+1)%2)*0.5; //当总数为偶数时，这个值为0.5；奇数时，为0
    float xPos = 640 + (m_nPage - (nMiddleLocation - fHalf))*56;
    
    if (pBall2 != NULL)
    {
        pBall2->setPosition(ccp(xPos, 85));
    }
    else
    {
        pBall2 = CCSprite::create("freecoin/ball1.png");
        pBall2->setScale(0.7);
        pBall2->setPosition(ccp(xPos, 85));
        layer->addChild(pBall2, 3, 101);
    }
}

void OpenOtherGamesLayer::TimerToSetScrollViewTouch(float dt)
{
    this->unschedule(schedule_selector(OpenOtherGamesLayer::TimerToSetScrollViewTouch));
    CCPoint p = ccp(m_xPos, 0);
    
    m_OtherGamesView->setContentOffsetInDuration(p,0.2f);
    
}

//确定点击了那个图片
void OpenOtherGamesLayer::confirmGamePicture(CCPoint p)
{
    //p 传过来的时候就已经完成了坐标转换
    for (int i=0; i<m_nAdCount; i++)
    {
        CCSprite * gamePic = (CCSprite *)m_containLayer->getChildByTag(CMainLogic::sharedMainLogic()->m_vNewAdvertInfo[i].nID);
        if (NULL != gamePic)
        {
            
            CCRect rect = gamePic->boundingBox();
            if (rect.containsPoint(p))
            {
                std::string sURL = CMainLogic::sharedMainLogic()->m_vNewAdvertInfo[i].sDownLoadURL;
                OpenURlForGameDown(sURL);
                this->removeFromParentAndCleanup(true);
            }
        }
    }
}

//点击了关卡进入浏览器中游戏下载地址
void OpenOtherGamesLayer::OpenURlForGameDown(std::string GameURL)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ChargeWebView::linkToMoreGamesDownLoad(GameURL);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    linkToMoreGamesDownLoad(GameURL);
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void OpenOtherGamesLayer::linkToMoreGamesDownLoad(std::string str)
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

void OpenOtherGamesLayer::closeOtherGamesWnd(CCObject * obj)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    this->removeFromParentAndCleanup(true);
    if(CMainLogic::sharedMainLogic()->m_bIsShowLogonGift)
    {
        CMainLogic::sharedMainLogic()->m_bHaveAdvert = false;
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                if(CCUserDefault::sharedUserDefault()->getStringForKey("ShowLogonGift", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("logon"))
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("ShowLogonGift", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("logon"));
                }
            }
        }
    }
}
