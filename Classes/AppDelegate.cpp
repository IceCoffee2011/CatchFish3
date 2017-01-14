#include "header.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../platform/android/jni/JniHelper.h"
#include "jni.h"
bool bUploadCompleted = false;
std::string meTouXiangPathStr = "";
int nWxChargeResult = 0;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
bool bTouXiangUpdate = false;
long long nOwnUserVersion = 0l;
#endif



AppDelegate::AppDelegate()
{
	m_MainLogic = new CMainLogic;
}

AppDelegate::~AppDelegate() 
{
	SimpleAudioEngine::sharedEngine()->end();
	if (m_MainLogic)
	{
		delete m_MainLogic;
		m_MainLogic = NULL;
	}
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	//屏幕适配
	pEGLView->setDesignResolutionSize(1280, 720, kResolutionShowAll);

    // turn on display FPS
    pDirector->setDisplayStats(false);
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
	

	if (m_MainLogic)
	{
		m_MainLogic->init();
	}
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    m_nCurrentTime = (int)time(0);
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	if(m_MainLogic != NULL)
	{
		if(m_MainLogic->m_bSharedState || m_MainLogic->m_bChargeState || m_MainLogic->m_bOpenPhoto)
		{
		//@@@@@@@@ me
			//m_MainLogic->m_bSharedState = false;
			m_MainLogic->m_bChargeState = false;
			return;
		}
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
        return;
#endif
		//进入到后台，退出游戏，每次进来都是重新登录
		if(m_MainLogic->getPlaying())//游戏状态
		{
			if(GameMainScene::_instance != NULL)
			{
                m_MainLogic->sendAndroidRoomExit();
				m_MainLogic->sendNotification(false);
				CCDirector::sharedDirector()->end();
			}
            else if(GameLayer::_instance != NULL)
            {
                m_MainLogic->sendAndroidRoomExit();
				m_MainLogic->sendNotification(false);
				CCDirector::sharedDirector()->end();
            }
		}
	}
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if(m_MainLogic != NULL)
	{
		if(m_MainLogic->m_bSharedState || m_MainLogic->m_bChargeState)
		{
			return;
		}
        m_MainLogic->m_bShowGuide = false;
		if(m_MainLogic->getPlaying() || m_MainLogic->m_bEnterLoading)//游戏状态
		{
			if(m_MainLogic->m_bEnterLoading)
			{
                if(GameMainScene::_instance != NULL && GameMainScene::_instance->getOnline())
                {
                    if(m_MainLogic->KIND_ID == NZNH_KIND_ID || m_MainLogic->KIND_ID == YQS_KIND_ID || m_MainLogic->KIND_ID == JCBY_KIND_ID)
                    {
                        m_MainLogic->m_lUserScore = GameMainScene::_instance->getUserCurrentScore();
                    }
                }
//                m_MainLogic->sendRoomExit();
//				m_MainLogic->backToLogon();
			}
		}
		CMainLogic::sharedMainLogic()->sendIOSNotification();
	}
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    if(m_MainLogic != NULL && m_MainLogic->m_pNetworkService != NULL)
    {
        m_MainLogic->requestNotification();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if(m_MainLogic->m_bSharedState || m_MainLogic->m_bChargeState)
        {
            m_MainLogic->m_bSharedState = false;
            m_MainLogic->m_bChargeState = false;
        }
        else
        {
            m_MainLogic->m_bForeGround = true;
            m_MainLogic->m_bNeedLogon = true;
            if(GameMainScene::_instance != NULL)
            {
                int nCurrentTime = (int)time(0);
                if(nCurrentTime-m_nCurrentTime > 30)//如果在游戏中，则发送退出游戏的消息
                {
                    m_MainLogic->sendRoomExit();
                    m_MainLogic->backToLogon();
                }
            }
            else if(GameLayer::_instance != NULL)
            {
                int nCurrentTime = (int)time(0);
                if(nCurrentTime-m_nCurrentTime > 30)//如果在游戏中，则发送退出游戏的消息
                {
                    m_MainLogic->sendRoomExit();
                    m_MainLogic->backToLogon();
                }
            }
            else
            {
                //发送检测包
                m_MainLogic->ShowMessage("正在连接服务器...", eMBNull);
                if(!m_MainLogic->m_bEnterService)
                {
                    m_MainLogic->m_bBackgroundEnterService = false;
                    if(m_MainLogic->m_pHallScene != NULL)
                    {
                        m_MainLogic->backToLogon();
                    }
                }
                else
                {
                    m_MainLogic->m_bBackgroundEnterService = true;
                }
            }
            std::string str = ChargeWebView::OnInternet();
            if(str == "")
            {
                m_MainLogic->ShowMessage("您没有可用的网络，请检查网络设置", eMBOK, eDisConnectNet);
            }
            else
            {
                m_MainLogic->SendData(m_MainLogic->m_nHallSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
                m_MainLogic->openCheckLink();
            }
        }
        
        m_MainLogic->checkChargeCompleted();
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		if(nWxChargeResult == 1)
		{
			m_MainLogic->m_pMessageBox->Update();
			ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
            if(pChargeLayer != NULL)
			{
				pChargeLayer->setWebViewCharge(false);
			}
			nWxChargeResult = 0;
		}
#endif
    }
    
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
