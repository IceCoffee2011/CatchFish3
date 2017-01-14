#include "header.h"

BuyBarrelLayer::BuyBarrelLayer()
{
    
}

BuyBarrelLayer::~BuyBarrelLayer()
{
    
}

//nOpenType=1为购买炮台，=2为购买技能，3为试用炮结束后购买炮界面
BuyBarrelLayer* BuyBarrelLayer::create(int nSelectedTag, int nOpenType, int nTotalScore)
{
    BuyBarrelLayer* pRet = new BuyBarrelLayer();
    if(pRet && pRet->init(nSelectedTag, nOpenType, nTotalScore))
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

bool BuyBarrelLayer::init(int nSelectedTag, int nOpenType, int nTotalScore)
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    m_nSelectedTag = nSelectedTag;
    m_nBuyType = nOpenType;
    m_nTotalScore = nTotalScore;
    
    loadUI();
    
    
    this -> setTouchEnabled(true);
    
    return true;
}

void BuyBarrelLayer::loadUI()
{
    m_visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    setPosition(ccp(m_visibleSize.width/2, m_visibleSize.height/2));
    
    pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), 1280, 720);
    this->addChild(pLayerColor);
    
    float width = 740;
    float height = 300;
    
    pSpDi = CCScale9Sprite::create("public2wnd/bgdi.png");
    pSpDi -> setContentSize(CCSizeMake(width, height));
    addChild(pSpDi);
    pSpDi -> setPosition(ccp(m_visibleSize.width/2, m_visibleSize.height/2));
    
    CCSprite* pSpLeft1 = CCSprite::create("public2wnd/bgleft1.png");
    pSpLeft1 -> setPosition(ccp(0, pSpDi->getContentSize().height));
    pSpDi -> addChild(pSpLeft1, 1);
    CCSprite* pSpLeft2 = CCSprite::create("public2wnd/bgleft1.png");
    pSpLeft2 -> setPosition(ccp(0, 0));
    pSpLeft2 -> setFlipY(true);
    pSpDi -> addChild(pSpLeft2, 1);
    CCSprite* pSpRight1 = CCSprite::create("public2wnd/bgleft1.png");
    pSpRight1 -> setPosition(ccp(pSpDi->getContentSize().width, pSpDi->getContentSize().height));
    pSpDi -> addChild(pSpRight1, 1);
    pSpRight1 -> setFlipX(true);
    CCSprite* pSpRight2 = CCSprite::create("public2wnd/bgleft1.png");
    pSpRight2 -> setPosition(ccp(pSpDi->getContentSize().width, 0));
    pSpDi -> addChild(pSpRight2, 1);
    pSpRight2 -> setFlipX(true);
    pSpRight2 -> setFlipY(true);
    //close menu
    CCMenuItemImage* closeItem = CCMenuItemImage::create("mail/mailcloseItem0.png","mail/mailcloseItem1.png", this, menu_selector(BuyBarrelLayer::closeWnd));
    closeItem -> setPosition(ccp(pSpRight1->getContentSize().width-20, pSpRight1->getContentSize().height-20));
    CCMenu* pCloseMenu = CCMenu::create(closeItem, NULL);
    pCloseMenu -> setPosition(CCPointZero);
    pCloseMenu -> setAnchorPoint(CCPointZero);
    pSpRight1 -> addChild(pCloseMenu);
    pCloseMenu -> setTouchPriority(-130);
    
    CCSprite* pSpMiddleLeft = CCSprite::create("public2wnd/bgmiddleleft.png");
    pSpMiddleLeft -> setPosition(ccp(83, pSpDi->getContentSize().height/2));
    pSpMiddleLeft -> setScaleY(((float)height-pSpLeft2->getContentSize().height)/pSpMiddleLeft->getContentSize().height);
    pSpDi -> addChild(pSpMiddleLeft);
    CCSprite* pSpMiddleRight = CCSprite::create("public2wnd/bgmiddleright.png");
    pSpMiddleRight -> setPosition(ccp(pSpDi->getContentSize().width-106, pSpDi->getContentSize().height/2));
    pSpMiddleRight -> setScaleY((height-pSpLeft2->getContentSize().height)/pSpMiddleRight->getContentSize().height);
    pSpDi -> addChild(pSpMiddleRight);
    CCSprite* pSpMiddleTop = CCSprite::create("public2wnd/bgmiddletop.png");
    pSpMiddleTop -> setPosition(ccp(pSpDi->getContentSize().width/2, pSpDi->getContentSize().height-26));
    pSpMiddleTop -> setScaleX((width-pSpRight2->getContentSize().width)/pSpMiddleTop->getContentSize().width);
    pSpDi -> addChild(pSpMiddleTop);
    CCSprite* pSpMiddleBottom = CCSprite::create("public2wnd/bgmiddlebottom.png");
    pSpMiddleBottom -> setPosition(ccp(pSpDi->getContentSize().width/2, 26));
    pSpMiddleBottom -> setScaleX((width-pSpRight2->getContentSize().width)/pSpMiddleBottom->getContentSize().width);
    pSpDi -> addChild(pSpMiddleBottom);
    CCSize sizeBG = pSpDi->getContentSize();
    int nSize = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    nSize = 36;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    nSize = 30;
#endif
    
    CCSize size(500,270);
    m_cclMessage = CCLabelTTF::create("", "Arial", nSize,size,kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    m_cclMessage->setColor(ccc3(188, 1.3, 21));
    m_cclMessage->setPosition(ccp(sizeBG.width/2+100, sizeBG.height/2-20));
    pSpDi->addChild(m_cclMessage, 5);
    
    if(m_nBuyType == 2)//购买技能
    {
        m_cclMessage -> setPosition(ccp(sizeBG.width/2+100, sizeBG.height/2-10));
        CCSprite* pSpBg = CCSprite::create("barrelUI/selectbarrelbg2.png");
        pSpBg -> setPosition(ccp(120, pSpDi->getContentSize().height/2+40));
        pSpDi -> addChild(pSpBg, 5);
        CCLabelTTF* pLabelTime = CCLabelTTF::create("2000金币/1个", "Arial", 32);
        pLabelTime -> setPosition(ccp(pSpBg->getContentSize().width/2, -30));
        pSpBg -> addChild(pLabelTime, 5);
        pLabelTime -> setColor(ccc3(188, 1.3, 21));
        if(m_nSelectedTag == 1)//加速
        {
            CCSprite* sprite = CCSprite::create("charge/chargespeedup.png");
            sprite -> setPosition(ccp(pSpBg->getContentSize().width/2, pSpBg->getContentSize().height/2));
            pSpBg -> addChild(sprite);
            CCLabelTTF* pLabelName = CCLabelTTF::create("加速", "Arial", 24);
            pLabelName -> setPosition(ccp(pSpBg->getContentSize().width/2, 28));
            pSpBg -> addChild(pLabelName);
            std::string sDesription = "在继承传统捕鱼加速技术的同时，渔民们开发出的新型加速技能，可以在15秒内每发射1炮同时打出5枚炮弹。";
            m_cclMessage->setString(sDesription.c_str());
        }
        else if(m_nSelectedTag == 3)//锁定
        {
            CCSprite* sprite = CCSprite::create("charge/chargesuoding.png");
            sprite -> setPosition(ccp(pSpBg->getContentSize().width/2, pSpBg->getContentSize().height/2));
            pSpBg -> addChild(sprite);
            CCLabelTTF* pLabelName = CCLabelTTF::create("锁定", "Arial", 24);
            pLabelName -> setPosition(ccp(pSpBg->getContentSize().width/2, 28));
            pSpBg -> addChild(pLabelName);
            std::string sDesription = "常年捕鱼的人们开发出一种100%提高捕鱼精准度的技能，使用后15秒内可以自由点击屏幕上的鱼进行锁定。";
            m_cclMessage->setString(sDesription.c_str());
        }
        CCSprite* pkuang = CCSprite::create("barrelUI/skillkuang.png");
        pkuang -> setPosition(ccp(pSpDi->getContentSize().width/2-15, 2));
        pSpDi -> addChild(pkuang, 6);
        m_labelSkillNum = CCLabelAtlas::create("1", "Dragon/fubennum.png", 32, 48, '0');
        m_labelSkillNum -> setAnchorPoint(ccp(0.5f, 0.5f));
        m_labelSkillNum -> setScale(0.8f);
        m_labelSkillNum -> setPosition(ccp(pSpDi->getContentSize().width/2-15, 2));
        pSpDi -> addChild(m_labelSkillNum, 6);
        CCMenuItemImage* jianItem = CCMenuItemImage::create("Dragon/fubenjianItem.png", "Dragon/fubenjianItem.png", this, menu_selector(BuyBarrelLayer::clickMenu));
        jianItem -> setTag(105);
        jianItem -> setPosition(ccp(pSpDi->getContentSize().width/2-150, 2));
        CCMenuItemImage* jiaItem = CCMenuItemImage::create("Dragon/fubenjiaItem.png", "Dragon/fubenjiaItem.png", this, menu_selector(BuyBarrelLayer::clickMenu));
        jiaItem -> setTag(106);
        jiaItem -> setPosition(ccp(pSpDi->getContentSize().width/2+120, 2));
        
        CCMenuItemImage* menuOK2 = CCMenuItemImage::create("barrelUI/barrelItem3.png", "barrelUI/barrelItem3.png", this, menu_selector(BuyBarrelLayer::clickMenu));
        menuOK2->setPosition(ccp(pSpDi->getContentSize().width/5*4+70, 2));
        CCMenu* pMenu = CCMenu::create(menuOK2, jiaItem, jianItem, NULL);
        pMenu -> setPosition(CCPointZero);
        pMenu -> setAnchorPoint(CCPointZero);
        pSpDi -> addChild(pMenu, 6);
        menuOK2 -> setTag(103);
        pMenu -> setTouchPriority(-130);
    }
    
}

void BuyBarrelLayer::clickMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    if(item == NULL) return ;
    if(item->getTag() == 103)//跳转都充值技能界面(21-23)
    {
        //本框消失，弹出充值界面，并且已经选好了要买那哪个技能
        //this -> removeFromParent();
        //停止自动开炮
        int nCount = atoi(m_labelSkillNum->getString());
        if(GameMainScene::_instance != NULL)
        {
            if(nCount*2000 > GameMainScene::_instance->getUserCurrentScore())
            {
                CMainLogic::sharedMainLogic()->ShowMessage("对不起，您当前金币不足无法购买", eMBOK);
                return ;
            }
            else
            {
                //游戏内金币购买技能
                CMD_GR_BuySkill BuySkill = {0};
                BuySkill.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                BuySkill.cbSkillID = m_nSelectedTag-1;
                BuySkill.nCount = atoi(m_labelSkillNum->getString());
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nRoomSocketHandle, MDM_GR_USER, SUB_GR_BUY_SKILL, &BuySkill, sizeof(BuySkill));
                CMainLogic::sharedMainLogic()->ShowMessage("正在购买，请稍候", eMBExitGame);
            }
            if(GameMainScene::_instance->getAutoFireStatus())
            {
                GameMainScene::_instance->unschedule(schedule_selector(GameMainScene::startBullet));
            }
        }
        else
        {
            if(nCount*2000 > CMainLogic::sharedMainLogic()->m_lUserScore)
            {
                CMainLogic::sharedMainLogic()->ShowMessage("对不起，您当前金币不足无法购买", eMBOK);
                return ;
            }
            else
            {
                //大厅购买技能
                CMD_GPR_BuySkill BuySkill = {0};
                BuySkill.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                BuySkill.cbSkillID = m_nSelectedTag-1;
                BuySkill.nCount = atoi(m_labelSkillNum->getString());
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GP_BUY_SKILL, &BuySkill, sizeof(BuySkill));
                CMainLogic::sharedMainLogic()->ShowMessage("正在购买，请稍候", eMBExitGame);
            }
            
        }
        if(m_nSelectedTag == 1)
        {
            if(GameMainScene::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameCoinToSkill1);
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallCoinToSkill1);
            }
        }
        else if(m_nSelectedTag == 2)
        {
        }
        else if(m_nSelectedTag == 3)
        {
            if(GameMainScene::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameCoinToSkill3);
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallCoinToSkill3);
            }
        }
        
    }
    else if(item->getTag() == 105)//减少金币兑换技能数量
    {
        int num = atoi(m_labelSkillNum->getString());
        if(num <= 1)
        {
            return;
        }
        else
        {
            num--;
            char temp[8];
            sprintf(temp, "%d", num);
            m_labelSkillNum -> setString(temp);
        }
    }
    else if(item->getTag() == 106)//增加金币兑换技能数量
    {
        int nLast = 0;
        if(GameMainScene::_instance != NULL)
        {
            nLast = GameMainScene::_instance->getUserCurrentScore()/2000;
        }
        else
        {
            nLast = (int)CMainLogic::sharedMainLogic()->m_lUserScore/2000;
        }
        int num = atoi(m_labelSkillNum->getString());
        if(num < nLast)
        {
           num++;
        }
        char temp[8];
        sprintf(temp, "%d", num);
        m_labelSkillNum -> setString(temp);
    }
}

void BuyBarrelLayer::closeWnd(CCObject* object)
{
    if(object != NULL)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    }
    this -> removeFromParent();
    if(m_nSelectedTag == 1 || m_nSelectedTag == 2 || m_nSelectedTag == 3)
    {
        if(GameMainScene::_instance != NULL)
        {
            DaojuLayer* pDaojuLayer = (DaojuLayer*)GameMainScene::_instance->getChildByTag(1989);
            if(pDaojuLayer == NULL)
            {
                GameMainScene::_instance->setAutoFire();
            }
        }
    }
}

void BuyBarrelLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool BuyBarrelLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}