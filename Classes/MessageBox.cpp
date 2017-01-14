#include "header.h"

CMessageBox::CMessageBox(void)
{
	init();
    
    m_bShow = false;

	m_Message.eOperate=eMBOK;
	m_Message.eRequest=eNull;
	m_Message.bWait=false;
	memset(m_Message.szBuffer,0,sizeof(m_Message.szBuffer));
}

CMessageBox::~CMessageBox(void)
{

}

bool CMessageBox::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
    
	m_visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	setPosition(ccp(m_visibleSize.width/2, m_visibleSize.height/2));
    
    pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), m_visibleSize.width, m_visibleSize.height);
    this->addChild(pLayerColor);
    pLayerColor->setVisible(false);

    float width = 640;
    float height = 300;
    
    pSpDi = CCSprite::create("messagebox/messageboxbg.png");
    pSpDi -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    addChild(pSpDi);
    
//    pSpDi = CCScale9Sprite::create("public2wnd/bgdi.png");
//    pSpDi -> setContentSize(CCSizeMake(width, height));
//    addChild(pSpDi);
//    pSpDi -> setPosition(ccp(m_visibleSize.width/2, m_visibleSize.height/2));
//    
//    CCSprite* pSpLeft1 = CCSprite::create("public2wnd/bgleft1.png");
//    pSpLeft1 -> setPosition(ccp(0, pSpDi->getContentSize().height));
//    pSpDi -> addChild(pSpLeft1, 1);
//    CCSprite* pSpLeft2 = CCSprite::create("public2wnd/bgleft1.png");
//    pSpLeft2 -> setPosition(ccp(0, 0));
//    pSpLeft2 -> setFlipY(true);
//    pSpDi -> addChild(pSpLeft2, 1);
//    CCSprite* pSpRight1 = CCSprite::create("public2wnd/bgleft1.png");
//    pSpRight1 -> setPosition(ccp(pSpDi->getContentSize().width, pSpDi->getContentSize().height));
//    pSpDi -> addChild(pSpRight1, 1);
//    pSpRight1 -> setFlipX(true);
//    CCSprite* pSpRight2 = CCSprite::create("public2wnd/bgleft1.png");
//    pSpRight2 -> setPosition(ccp(pSpDi->getContentSize().width, 0));
//    pSpDi -> addChild(pSpRight2, 1);
//    pSpRight2 -> setFlipX(true);
//    pSpRight2 -> setFlipY(true);
//    
//    CCSprite* pSpMiddleLeft = CCSprite::create("public2wnd/bgmiddleleft.png");
//    pSpMiddleLeft -> setPosition(ccp(83, pSpDi->getContentSize().height/2));
//    pSpMiddleLeft -> setScaleY(((float)height-pSpLeft2->getContentSize().height)/pSpMiddleLeft->getContentSize().height);
//    pSpDi -> addChild(pSpMiddleLeft);
//    CCSprite* pSpMiddleRight = CCSprite::create("public2wnd/bgmiddleright.png");
//    pSpMiddleRight -> setPosition(ccp(pSpDi->getContentSize().width-106, pSpDi->getContentSize().height/2));
//    pSpMiddleRight -> setScaleY((height-pSpLeft2->getContentSize().height)/pSpMiddleRight->getContentSize().height);
//    pSpDi -> addChild(pSpMiddleRight);
//    CCSprite* pSpMiddleTop = CCSprite::create("public2wnd/bgmiddletop.png");
//    pSpMiddleTop -> setPosition(ccp(pSpDi->getContentSize().width/2, pSpDi->getContentSize().height-26));
//    pSpMiddleTop -> setScaleX((width-pSpRight2->getContentSize().width)/pSpMiddleTop->getContentSize().width);
//    pSpDi -> addChild(pSpMiddleTop);
//    CCSprite* pSpMiddleBottom = CCSprite::create("public2wnd/bgmiddlebottom.png");
//    pSpMiddleBottom -> setPosition(ccp(pSpDi->getContentSize().width/2, 26));
//    pSpMiddleBottom -> setScaleX((width-pSpRight2->getContentSize().width)/pSpMiddleBottom->getContentSize().width);
//    pSpDi -> addChild(pSpMiddleBottom);

	CCSize sizeBG = pSpDi->getContentSize();
	int nSize = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	nSize = 36;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	nSize = 30;
#endif
    
    CCSize size(600,300);
	m_cclMessage = CCLabelTTF::create("", "Arial", nSize,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    //m_cclMessage->setColor(ccc3(188, 1.3, 21));
    m_cclMessage->setColor(ccc3(153, 94, 17));
	m_cclMessage->setPosition(ccp(sizeBG.width/2, sizeBG.height/2+40));
	pSpDi->addChild(m_cclMessage, 5);
    
    

	CCMenuItemImage* menuOK1 = CCMenuItemImage::create("messagebox/messageboxok.png", "messagebox/messageboxok.png", this, menu_selector(CMessageBox::OnButtonOK));
	menuOK1->setPosition(ccp(sizeBG.width/2, sizeBG.height/5.8+20));

	m_pMenuOK = CCMenu::create(menuOK1, NULL);
	m_pMenuOK->setPosition(ccp(0,0));
	m_pMenuOK->setTouchPriority(-210);
	pSpDi->addChild(m_pMenuOK, 5);

	CCMenuItemImage* menuOK2 = CCMenuItemImage::create("messagebox/messageboxok.png", "messagebox/messageboxok.png", this, menu_selector(CMessageBox::OnButtonOK));
	menuOK2->setPosition(ccp(sizeBG.width*5/7, sizeBG.height/5.8+20));

	CCMenuItemImage* menuCancel2 = CCMenuItemImage::create("messagebox/messageboxcancel.png", "messagebox/messageboxcancel.png", this, menu_selector(CMessageBox::OnButtonCancel));
	menuCancel2->setPosition(ccp(sizeBG.width*2/7, sizeBG.height/5.8+20));

	m_pMenuOKCancel = CCMenu::create(menuOK2, menuCancel2, NULL);
	m_pMenuOKCancel->setPosition(ccp(0,0));
	m_pMenuOKCancel->setTouchPriority(-210);
	pSpDi->addChild(m_pMenuOKCancel, 5);
    
    loadSuperUI();
    
//    m_ccsWait = CCSprite::create("messagebox/loading.png");
//    m_ccsWait->setPosition(ccp(sizeBG.width/2, sizeBG.height/3));
//    pSpDi->addChild(m_ccsWait);
//    m_ccsWait->runAction(CCRepeatForever::create(CCRotateBy::create(2.0f, 360)));
//    m_ccsWait->setVisible(false);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCMenuItemImage* menuBuy = CCMenuItemImage::create("messagebox/buy1.png", "messagebox/buy1.png", this, menu_selector(CMessageBox::OnButtonOK));
    menuBuy -> setPosition(ccp(sizeBG.width*2/7, sizeBG.height/5.8+20));
    
    CCMenuItemImage* menuCancel3 = CCMenuItemImage::create("messagebox/cancel1.png", "messagebox/cancel1.png", this, menu_selector(CMessageBox::OnButtonCancel));
    menuCancel3->setPosition(ccp(sizeBG.width*5/7, sizeBG.height/5.8+20));
    
    m_pMenuBuy = CCMenu::create(menuBuy, menuCancel3, NULL);
    m_pMenuBuy->setPosition(ccp(0,0));
    m_pMenuBuy->setTouchPriority(-210);
    pSpDi->addChild(m_pMenuBuy, 5);
#endif
    
    m_pGameGongGaoBg = CCScale9Sprite::create("CatchFish01/gamegonggaobg.png");//公告背景图
    m_pGameGongGaoBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    m_pGameGongGaoBg->setContentSize(CCSizeMake(CLIENT_WIDTH, 80));
    addChild(m_pGameGongGaoBg);
    m_pGameGongGaoBg->setVisible(false);
    m_ccl2Message = CCLabelTTF::create("", "Arial", nSize,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    m_ccl2Message->setColor(ccc3(188, 1.3, 21));
    m_ccl2Message->setPosition(ccp(m_pGameGongGaoBg->getContentSize().width/2, m_pGameGongGaoBg->getContentSize().height/2));
    m_pGameGongGaoBg->addChild(m_ccl2Message, 5);
    m_ccl2Message->setColor(ccc3(255, 255, 255));

	//触控事件
	setTouchEnabled(true);
	//锚点有效
	ignoreAnchorPointForPosition(false);

	return true;
}

void CMessageBox::loadSuperUI()
{
    m_spSuperDi = CCSprite::create("public2wnd/superbgdi.png");
    //m_spSuperDi -> setContentSize(CCSizeMake(width, height));
    addChild(m_spSuperDi);
    m_spSuperDi -> setPosition(ccp(m_visibleSize.width/2, m_visibleSize.height/2));
    
//    CCSprite* pSpLeft1 = CCSprite::create("public2wnd/superbgleft1.png");
//    pSpLeft1 -> setPosition(ccp(0, m_spSuperDi->getContentSize().height));
//    m_spSuperDi -> addChild(pSpLeft1, 1);
//    CCSprite* pSpLeft2 = CCSprite::create("public2wnd/superbgleft1.png");
//    pSpLeft2 -> setPosition(ccp(0, 0));
//    pSpLeft2 -> setFlipY(true);
//    m_spSuperDi -> addChild(pSpLeft2, 1);
//    CCSprite* pSpRight1 = CCSprite::create("public2wnd/superbgleft1.png");
//    pSpRight1 -> setPosition(ccp(m_spSuperDi->getContentSize().width, m_spSuperDi->getContentSize().height));
//    m_spSuperDi -> addChild(pSpRight1, 1);
//    pSpRight1 -> setFlipX(true);
//    CCSprite* pSpRight2 = CCSprite::create("public2wnd/superbgleft1.png");
//    pSpRight2 -> setPosition(ccp(m_spSuperDi->getContentSize().width, 0));
//    m_spSuperDi -> addChild(pSpRight2, 1);
//    pSpRight2 -> setFlipX(true);
//    pSpRight2 -> setFlipY(true);
//    
//    CCSprite* pSpMiddleLeft = CCSprite::create("public2wnd/superbgmiddleleft.png");
//    pSpMiddleLeft -> setPosition(ccp(6.4, m_spSuperDi->getContentSize().height/2));
//    pSpMiddleLeft -> setScaleY(((float)height-pSpLeft2->getContentSize().height)/pSpMiddleLeft->getContentSize().height);
//    m_spSuperDi -> addChild(pSpMiddleLeft);
//    CCSprite* pSpMiddleRight = CCSprite::create("public2wnd/superbgmiddleleft.png");
//    pSpMiddleRight -> setPosition(ccp(m_spSuperDi->getContentSize().width-6.3, m_spSuperDi->getContentSize().height/2));
//    pSpMiddleRight -> setScaleY((height-pSpLeft2->getContentSize().height)/pSpMiddleRight->getContentSize().height);
//    pSpMiddleRight->setFlipX(true);
//    m_spSuperDi -> addChild(pSpMiddleRight);
//    CCSprite* pSpMiddleTop = CCSprite::create("public2wnd/superbgmiddletop.png");
//    pSpMiddleTop -> setPosition(ccp(m_spSuperDi->getContentSize().width/2, m_spSuperDi->getContentSize().height-1.5));
//    pSpMiddleTop -> setScaleX((width-pSpRight2->getContentSize().width)/pSpMiddleTop->getContentSize().width);
//    m_spSuperDi -> addChild(pSpMiddleTop);
//    CCSprite* pSpMiddleBottom = CCSprite::create("public2wnd/superbgmiddlebottom.png");
//    pSpMiddleBottom -> setPosition(ccp(m_spSuperDi->getContentSize().width/2, -0.8));
//    pSpMiddleBottom -> setScaleX((width-pSpRight2->getContentSize().width)/pSpMiddleBottom->getContentSize().width);
//    m_spSuperDi -> addChild(pSpMiddleBottom);
    
    CCSize sizeBG = m_spSuperDi->getContentSize();
    int nSize = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    nSize = 36;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    nSize = 30;
#endif
    
    CCSize size(600,300);
    m_ccl3Message = CCLabelTTF::create("", "Arial", nSize,size,kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    m_ccl3Message->setColor(ccc3(255, 221, 28));
    m_ccl3Message->setPosition(ccp(sizeBG.width/2, sizeBG.height/2+55));
    m_spSuperDi->addChild(m_ccl3Message, 5);
    
    
    CCMenuItemImage* menuOK1 = CCMenuItemImage::create("public2wnd/superOk1.png", "public2wnd/superOk2.png", this, menu_selector(CMessageBox::OnButtonOK));
    menuOK1->setPosition(ccp(sizeBG.width/2, sizeBG.height/5.8));
    m_menuSuperOK = CCMenu::create(menuOK1, NULL);
    m_menuSuperOK->setPosition(ccp(0,0));
    m_menuSuperOK->setTouchPriority(-210);
    m_spSuperDi->addChild(m_menuSuperOK, 5);
    
    CCMenuItemImage* menuOK2 = CCMenuItemImage::create("public2wnd/superOk1.png", "public2wnd/superOk2.png", this, menu_selector(CMessageBox::OnButtonOK));
    menuOK2->setPosition(ccp(sizeBG.width*2/7, sizeBG.height/5.8));
    CCMenuItemImage* menuCancel2 = CCMenuItemImage::create("public2wnd/superCancel1.png", "public2wnd/superCancel2.png", this, menu_selector(CMessageBox::OnButtonCancel));
    menuCancel2->setPosition(ccp(sizeBG.width*5/7, sizeBG.height/5.8));
    
    m_menuSuperOKCancel = CCMenu::create(menuOK2, menuCancel2, NULL);
    m_menuSuperOKCancel->setPosition(ccp(0,0));
    m_menuSuperOKCancel->setTouchPriority(-210);
    m_spSuperDi->addChild(m_menuSuperOKCancel, 5);
}

//显示消息
void CMessageBox::ShowMessageBox(eOperateType operate, eRequestType request, const char * pBuffer, bool bWait)
{
	tagMessage Message;
	memset(&Message,0,sizeof(Message));

	Message.bWait = bWait;
	Message.eOperate = operate;
	Message.eRequest = request;
	strcpy(Message.szBuffer, pBuffer);

	m_vMessage.push_back(Message);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate((CCLayer*)this);
    
    //注册
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate((CCLayer *)this, -209, true);
    
	if (m_vMessage.size()>1)
	{
        for(unsigned int i = 0; i < m_vMessage.size(); i++)
        {
            ////printf("111111%s\n", m_vMessage[i].szBuffer);
        }
		//@@当新消息到来，如果当前队列不为空，则自动处理，如果请求类型为eNull，则自动Update()，将当前消息更新掉
		CMainLogic::sharedMainLogic()->MessageResult(m_Message.eRequest,m_Message.eOperate);
		return;
	}

	

	m_Message.eOperate = operate;
	m_Message.eRequest = request;
	m_Message.bWait = bWait;
	strcpy(m_Message.szBuffer,pBuffer);

	//显示
	ShowMessageBox();
}

//显示消息
void CMessageBox::ShowMessageBox()
{
	switch(m_Message.eOperate)
	{
    case eMBNull://@@某些操作不允许消除消息框
        {
            m_bShow = true;
            pLayerColor->setVisible(false);
            pSpDi->setVisible(false);
            m_pGameGongGaoBg->setVisible(true);
            m_pMenuOK->setVisible(false);
            m_pMenuOKCancel->setVisible(false);
            m_spSuperDi->setVisible(false);
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            m_pMenuBuy->setVisible(false);
#endif
            this -> setVisible(false);
            this -> scheduleOnce(schedule_selector(CMessageBox::showNetWorkError5), 1.0f);
            this -> scheduleOnce(schedule_selector(CMessageBox::showNetWorkError),25);
        }
        break;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        case eMBIAPNull:
        {
            m_bShow = true;
            pLayerColor->setVisible(false);
            pSpDi->setVisible(false);
            m_spSuperDi->setVisible(false);
            m_pGameGongGaoBg->setVisible(true);
            m_pMenuOK->setVisible(false);
            m_pMenuOKCancel->setVisible(false);
            this -> setVisible(false);
            this -> scheduleOnce(schedule_selector(CMessageBox::showNetWorkError5),1.0f);
            this -> scheduleOnce(schedule_selector(CMessageBox::showNetWorkError),180);
            break;
        }
#endif
        case eMBExitGame:
        {
            m_bShow = true;
            pLayerColor->setVisible(false);
            pSpDi->setVisible(false);
            m_pGameGongGaoBg->setVisible(true);
            m_pMenuOK->setVisible(false);
            m_spSuperDi->setVisible(false);
            m_pMenuOKCancel->setVisible(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            m_pMenuBuy->setVisible(false);
#endif
            this -> setVisible(false);
            this -> scheduleOnce(schedule_selector(CMessageBox::showNetWorkError5),1.0f);
            this -> scheduleOnce(schedule_selector(CMessageBox::showNetWorkError),11);
            break;
        }
    case eMBRequestNull:
        {
            m_bShow = true;
            pLayerColor->setVisible(true);
            pSpDi->setVisible(true);
            m_pGameGongGaoBg->setVisible(false);
            m_pMenuOK->setVisible(false);
            m_pMenuOKCancel->setVisible(false);
            m_spSuperDi->setVisible(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            m_pMenuBuy->setVisible(false);
#endif
            this -> setVisible(false);
            this -> scheduleOnce(schedule_selector(CMessageBox::showNetWorkError5),1.0f);
            this -> schedule(schedule_selector(CMessageBox::showNetWorkError3),1.0f);
        }
        break;
    case eMBLabaNull:
        {
            m_bShow = true;
            pLayerColor->setVisible(false);
            pSpDi->setVisible(true);
            m_pGameGongGaoBg->setVisible(false);
            m_pMenuOK->setVisible(false);
            m_spSuperDi->setVisible(false);
            m_pMenuOKCancel->setVisible(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            m_pMenuBuy->setVisible(false);
#endif
            this -> scheduleOnce(schedule_selector(CMessageBox::showNetWorkError2),25);
        }
        break;
	case eMBOK:
		{
            m_bShow = true;
            pLayerColor->setVisible(true);
            pSpDi->setVisible(true);
            m_pGameGongGaoBg->setVisible(false);
			m_pMenuOK->setVisible(true);
            m_spSuperDi->setVisible(false);
			m_pMenuOKCancel->setVisible(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            m_pMenuBuy->setVisible(false);
#endif
		}
		break;
	case eMBOKCancel:
		{
            m_bShow = true;
            pLayerColor->setVisible(true);
            pSpDi->setVisible(true);
            m_pGameGongGaoBg->setVisible(false);
            m_spSuperDi->setVisible(false);
			m_pMenuOK->setVisible(false);
			m_pMenuOKCancel->setVisible(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            m_pMenuBuy->setVisible(false);
#endif
		}
		break;
    case eMBOKBuy:
        {
            m_bShow = true;
            pLayerColor->setVisible(true);
            pSpDi->setVisible(true);
            m_spSuperDi->setVisible(false);
            m_pGameGongGaoBg->setVisible(false);
            m_pMenuOK->setVisible(false);
            m_pMenuOKCancel->setVisible(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            m_pMenuBuy->setVisible(true);
#endif
            break;
        }
    case eMBSuperOK:
        {
            m_bShow = true;
            pLayerColor->setVisible(true);
            pSpDi->setVisible(false);
            m_pGameGongGaoBg->setVisible(false);
            m_spSuperDi->setVisible(true);
            m_menuSuperOKCancel->setVisible(false);
            m_menuSuperOK->setVisible(true);

            break;
        }
    case eMBSuperOKCancel:
        {
            m_bShow = true;
            pLayerColor->setVisible(true);
            pSpDi->setVisible(false);
            m_pGameGongGaoBg->setVisible(false);
            m_spSuperDi->setVisible(true);
            m_menuSuperOKCancel->setVisible(true);
            m_menuSuperOK->setVisible(false);
            break;
        }
        case eMBGameNull:
        {
            m_bShow = true;
            pLayerColor->setVisible(false);
            pSpDi->setVisible(false);
            m_pGameGongGaoBg->setVisible(true);
            m_pMenuOK->setVisible(false);
            m_spSuperDi->setVisible(false);
            m_pMenuOKCancel->setVisible(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            m_pMenuBuy->setVisible(false);
#endif
            this -> setVisible(false);
            this -> scheduleOnce(schedule_selector(CMessageBox::showNetWorkError5),1.0f);
            this -> scheduleOnce(schedule_selector(CMessageBox::showNetWorkError3),11);
            break;
        }
    }
    
    if(m_Message.eOperate == eMBSuperOKCancel || m_Message.eOperate == eMBSuperOK)
    {
        m_ccl3Message->setString(m_Message.szBuffer);
    }
    else
    {
        if(m_Message.eOperate != eMBNull && m_Message.eOperate != eMBIAPNull && m_Message.eOperate != eMBExitGame && m_Message.eOperate != eMBGameNull)
        {
            m_cclMessage->setString(m_Message.szBuffer);
        }
        else
        {
            m_ccl2Message->setString(m_Message.szBuffer);
        }
    }
    
    
}

void CMessageBox::Update()
{
    m_bShow = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	CMainLogic::sharedMainLogic()->m_bExitGame = false;
#endif
    //关闭网络断开定时器
    this -> unschedule(schedule_selector(CMessageBox::showNetWorkError));
    this -> unschedule(schedule_selector(CMessageBox::showNetWorkError2));
    this -> unschedule(schedule_selector(CMessageBox::showNetWorkError3));
    this -> unschedule(schedule_selector(CMessageBox::showNetWorkError4));
    this -> unschedule(schedule_selector(CMessageBox::showNetWorkError5));
    this -> setVisible(true);
	std::vector<tagMessage>::iterator iter = m_vMessage.begin();
	if (iter != m_vMessage.end())
	{
		m_vMessage.erase(iter);
	}
    ////printf("m_vMessage size is %lu\n", m_vMessage.size());
	m_Message.eOperate=eMBOK;
	m_Message.eRequest=eNull;
	m_Message.bWait=false;
	memset(m_Message.szBuffer,0,sizeof(m_Message.szBuffer));

	if (m_vMessage.size()>0)
	{
		m_Message.eOperate = m_vMessage[0].eOperate;
		m_Message.bWait = m_vMessage[0].bWait;
		m_Message.eRequest = m_vMessage[0].eRequest;
		strcpy(m_Message.szBuffer,m_vMessage[0].szBuffer);

		//显示
		ShowMessageBox();
		return;
	}
	else
	{
		//@@自动从父节点移除
		CCNode * pParent = getParent();
		if (pParent != NULL)
		{
			////printf("MessageBox remove\n");
			pParent->removeChild(this);
		}
	}

	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate((CCLayer *)this);
}

void CMessageBox::OnButtonOK(CCObject* obj)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
	CMainLogic::sharedMainLogic()->MessageResult(m_Message.eRequest,m_Message.eOperate,true);
	Update();
}

void CMessageBox::OnButtonCancel(CCObject* obj)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
	CMainLogic::sharedMainLogic()->MessageResult(m_Message.eRequest,m_Message.eOperate,false);
	Update();
}

bool CMessageBox::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	return true;
}

void CMessageBox::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
}

void CMessageBox::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
}

void CMessageBox::showNetWorkError(float dt)
{
    CMainLogic::sharedMainLogic()->ShowMessage("抱歉，网络断开了，请检查一下网络连接再进入游戏。",eMBOK,eDisConnectNet,false);
}

void CMessageBox::showNetWorkError2(float dt)
{
    CMainLogic::sharedMainLogic()->ShowMessage("抱歉，您的请求超时了。",eMBOK);
}

void CMessageBox::showNetWorkError3(float dt)
{
    CMainLogic::sharedMainLogic()->ShowMessage("抱歉，您的请求超时了，请稍后再试",eMBOK);
}

void CMessageBox::showNetWorkError5(float dt)
{
    this -> setVisible(true);
}

void CMessageBox::showNetWorkError4(float dt)
{
    CMainLogic::sharedMainLogic()->backToHall();
}

void CMessageBox::removeVector()
{
    m_vMessage.clear();
}