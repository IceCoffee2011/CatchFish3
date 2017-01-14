#include "header.h"

RedPacketLayer* RedPacketLayer::create(int nOpenType, int nRedPacketID)
{
    RedPacketLayer* pRet = new RedPacketLayer();
    if(pRet && pRet->init(nOpenType, nRedPacketID))
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

bool RedPacketLayer::init(int nOpenType, int nRedPacketID)
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    m_nRedPacketCoin = 0;
    
    if(nOpenType == 1)
    {
        m_nRedOpenType = 1;
        loadSendPacket();//发红包
        this -> setTouchEnabled(true);
    }
    else if(nOpenType == 2)
    {
        loadReceivePacket();//收红包
        this -> setTouchEnabled(true);
    }
    else if(nOpenType == 3)
    {
        loadRobPacket();//抢红包
        m_nRedPacketID = nRedPacketID;
    }
    else if(nOpenType == 4)
    {
        loadNoRedPacket();//没抢到
        this -> setTouchEnabled(true);
    }
    return true;
}

void RedPacketLayer::loadSendPacket()
{
    CCSprite* pSendBg = CCSprite::create("hall/redsendbg.png");
    pSendBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(pSendBg);
    pSendBg -> setScale(1.3f);
    CCMenuItemImage* closeItem = CCMenuItemImage::create("hall/redcloseitem.png", "hall/redcloseitem.png", this, menu_selector(RedPacketLayer::closeMenu));
    closeItem -> setPosition(ccp(pSendBg->getContentSize().width-closeItem->getContentSize().width, pSendBg->getContentSize().height-closeItem->getContentSize().height-8));
    CCMenuItemImage* sendItem = CCMenuItemImage::create("hall/redsenditem.png", "hall/redsenditem.png", this, menu_selector(RedPacketLayer::clickMenu));
    sendItem -> setTag(101);
    sendItem -> setPosition(ccp(pSendBg->getContentSize().width/2, 80));
   
    char temp[16];
    sprintf(temp, "%lld", CMainLogic::sharedMainLogic()->m_lUserScore);
    CCLabelTTF* pLabelScore = CCLabelTTF::create(temp, "Arial-BoldMT", 28);
    pLabelScore -> setPosition(ccp(pSendBg->getContentSize().width/2, 200));
    pLabelScore -> setColor(ccc3(231, 137, 60));
    pSendBg -> addChild(pLabelScore);
    
    m_editPeople = CCEditBox::create(CCSize(300, 52), CCScale9Sprite::create("hall/rededitbox.png"));
    m_editPeople->setMaxLength(10);//限制最长字符
    m_editPeople->setReturnType(kKeyboardReturnTypeDone);
    m_editPeople->setInputFlag(kEditBoxInputFlagSensitive);
    m_editPeople->setInputMode(kEditBoxInputModePhoneNumber);
    m_editPeople->setPosition(ccp(pSendBg->getContentSize().width/2, 365));
    m_editPeople->setPlaceHolder("红包个数");
    m_editPeople->setPlaceholderFontName("Arial-BoldMT");
    m_editPeople->setPlaceholderFontSize(18);
    pSendBg->addChild(m_editPeople, 10);
    m_editPeople -> setTouchPriority(-208);
//    m_editPeople -> setFontColor(ccc3(0, 0, 0));
    m_editPeople -> setFontName("Arial-BoldMT");
    
    m_editCount = CCEditBox::create(CCSize(300, 52), CCScale9Sprite::create("hall/rededitbox.png"));
    m_editCount->setMaxLength(10);//限制最长字符
    m_editCount->setReturnType(kKeyboardReturnTypeDone);
    m_editCount->setInputFlag(kEditBoxInputFlagSensitive);
    m_editCount->setInputMode(kEditBoxInputModePhoneNumber);
    m_editCount->setPosition(ccp(pSendBg->getContentSize().width/2, 290));
    m_editCount->setPlaceHolder("派发总金额");
    m_editCount->setPlaceholderFontName("Arial-BoldMT");
    m_editCount->setPlaceholderFontSize(18);
    pSendBg->addChild(m_editCount, 10);
    m_editCount -> setTouchPriority(-208);
//    m_editCount -> setFontColor(ccc3(0, 0, 0));
    m_editCount -> setFontName("Arial-BoldMT");
    
    m_editDes = CCEditBox::create(CCSize(300, 52), CCScale9Sprite::create("hall/rededitbox.png"));
    m_editDes->setMaxLength(60);//限制最长字符
    m_editDes->setReturnType(kKeyboardReturnTypeDone);
    m_editDes->setInputFlag(kEditBoxInputFlagSensitive);
    m_editDes->setInputMode(kEditBoxInputModeEmailAddr);
    m_editDes->setPosition(ccp(pSendBg->getContentSize().width/2, 150));
    m_editDes->setPlaceHolder("恭喜发财");
    m_editDes->setPlaceholderFontName("Arial-BoldMT");
    m_editDes->setPlaceholderFontSize(18);
    pSendBg->addChild(m_editDes, 10);
    m_editDes -> setTouchPriority(-208);
//    m_editDes -> setFontColor(ccc3(0, 0, 0));
    m_editDes -> setFontName("Arial-BoldMT");
    m_editDes -> setText("恭喜发财");
    
    CCMenuItemFont* font = CCMenuItemFont::create("给好友发红包", this, menu_selector(RedPacketLayer::clickMenu));
    font -> setTag(105);
    font -> setPosition(ccp(pSendBg->getContentSize().width-font->getContentSize().width/2-5, 27));
    font -> setFontNameObj("Arial-BoldMT");
    font -> setFontSizeObj(24);
    font -> setColor(ccc3(231, 137, 60));
    m_menuFriends = CCMenu::create(font, NULL);
    m_menuFriends -> setAnchorPoint(CCPointZero);
    m_menuFriends -> setPosition(CCPointZero);
    pSendBg -> addChild(m_menuFriends);
    m_menuFriends -> setTouchPriority(-208);
    
    CCMenu* pMenu = CCMenu::create(closeItem, sendItem, NULL);
    pMenu -> setAnchorPoint(CCPointZero);
    pMenu -> setPosition(CCPointZero);
    pSendBg -> addChild(pMenu);
    pMenu -> setTouchPriority(-208);
    
}

void RedPacketLayer::loadReceivePacket()
{
    CCSprite* sprite = CCSprite::create("hall/redgetpacketbg.png");
    sprite -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+80));
    this -> addChild(sprite);
    sprite -> setScale(1.5f);
    CCMenuItemImage* getItem = CCMenuItemImage::create("hall/redgetitem.png", "hall/redgetitem.png", this, menu_selector(RedPacketLayer::clickMenu));
    getItem -> setTag(103);
    getItem -> setPosition(ccp(sprite->getContentSize().width/2, 50));
    CCMenu* getMenu = CCMenu::create(getItem, NULL);
    getMenu -> setPosition(CCPointZero);
    getMenu -> setAnchorPoint(CCPointZero);
    sprite -> addChild(getMenu);
    getMenu -> setTouchPriority(-208);
    m_labelGetCoin = CCLabelTTF::create("", "Arial-BoldMT", 30);
    m_labelGetCoin -> setPosition(ccp(sprite->getContentSize().width/2, 315));
    sprite -> addChild(m_labelGetCoin);
    m_labelGetCoin -> setColor(ccc3(255, 255, 122));
    m_labelGetName = CCLabelTTF::create("", "Arial-BoldMT", 22);
    m_labelGetName -> setPosition(ccp(sprite->getContentSize().width/2, 214));
    sprite -> addChild(m_labelGetName);
    m_labelGetName -> setColor(ccc3(255, 213, 52));
    CCSize size(205,90);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    m_labelGetData = CCLabelTTF::create("", "Arial-BoldMT", 20, size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    m_labelGetData = CCLabelTTF::create("", "Arial-BoldMT", 16, size, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
#endif
    m_labelGetData -> setAnchorPoint(ccp(0.5, 1));
    m_labelGetData -> setPosition(ccp(sprite->getContentSize().width/2, 185));
    sprite -> addChild(m_labelGetData);
}

void RedPacketLayer::setReceiveData(int nCoin, std::string sName, std::string sData)
{
    m_nRedPacketCoin = nCoin;
    m_labelGetData -> setString(sData.c_str());
    m_labelGetName -> setString(sName.c_str());
    char temp[16];
    sprintf(temp, "%d金币", nCoin);
    m_labelGetCoin -> setString(temp);
}

void RedPacketLayer::loadRobPacket()
{
//    int pointX = rand()%800-400;
//    int pointY = CLIENT_HEIGHT+300+rand()%150;
    CCSprite* pRedLine = CCSprite::create("hall/redline.png");
    pRedLine -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT+200));
    this -> addChild(pRedLine);
    CCMenuItemImage* robItem = CCMenuItemImage::create("hall/redpacket.png", "hall/redpacket.png", this, menu_selector(RedPacketLayer::clickMenu));
    robItem -> setTag(102);
    robItem -> setPosition(ccp(pRedLine->getContentSize().width/2, 0));
    CCMenu* robMenu = CCMenu::create(robItem, NULL);
    robMenu -> setPosition(CCPointZero);
    robMenu -> setAnchorPoint(CCPointZero);
    pRedLine -> addChild(robMenu);
    robMenu -> setTouchPriority(-206);
    //执行动作
    pRedLine -> runAction(CCMoveBy::create(1.0f, ccp(0, -300)));
}

void RedPacketLayer::loadNoRedPacket()
{
    CCSprite* sprite = CCSprite::create("hall/rednopacket.png");
    sprite -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(sprite);
    sprite -> setScale(1.5f);
    m_labelGetName = CCLabelTTF::create("", "Arial-BoldMT", 22);
    m_labelGetName -> setPosition(ccp(sprite->getContentSize().width/2, 270));
    sprite -> addChild(m_labelGetName);
    m_labelGetName -> setColor(ccc3(255, 213, 52));
    CCMenuItemImage* noPacketItem = CCMenuItemImage::create("hall/rednopacketitem.png", "hall/rednopacketitem.png", this, menu_selector(RedPacketLayer::clickMenu));
    noPacketItem -> setTag(104);
    noPacketItem ->setPosition(ccp(sprite->getContentSize().width/2, 50));
    CCMenu* noPacketMenu = CCMenu::create(noPacketItem, NULL);
    noPacketMenu -> setPosition(CCPointZero);
    noPacketMenu -> setAnchorPoint(CCPointZero);
    sprite -> addChild(noPacketMenu);
    noPacketMenu -> setTouchPriority(-208);
}

void RedPacketLayer::setNoRedName(std::string sName)
{
    m_labelGetName->setString(sName.c_str());
}

void RedPacketLayer::clickMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag())
    {
        case 101://发红包
        {
            if(m_nRedOpenType == 1)
            {
                std::string sPeople = m_editPeople->getText();
                if(sPeople == "")
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("红包个数不能为空，请输入", eMBOK);
                    return ;
                }
                int nPeople = atoi(sPeople.c_str());
                if(nPeople == 0)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("红包个数不能为0，请重新输入", eMBOK);
                    return ;
                }
                std::string sCoin = m_editCount->getText();
                if(sCoin == "")
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("派发金额不能为空，请输入", eMBOK);
                    return ;
                }
                int nCoin = atoi(sCoin.c_str());
                if(nCoin == 0)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("派发金额不能为0，请重新输入", eMBOK);
                    return ;
                }
                int nUserScore = (int)CMainLogic::sharedMainLogic()->m_lUserScore;
                if(nCoin > nUserScore)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("您当前拥有金币不足，请重新输入派发红包金额", eMBOK);
                    return ;
                }
                std::string sData = m_editDes->getText();
                //检查完毕，开始向服务端发送消息，然后关闭此输入框
                CMD_GPR_FaHongBao FaHongBao = {0};
                FaHongBao.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                FaHongBao.wCount = nPeople;
                FaHongBao.nScore = nCoin;
                strncpy(FaHongBao.szContent,CCharCode::UTF8ToGB2312(sData.c_str()).c_str(),sizeof(FaHongBao.szContent));
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_FA_HONG_BAO,&FaHongBao,sizeof(FaHongBao));
                CMainLogic::sharedMainLogic()->ShowMessage("正在发送红包，请稍候", eMBExitGame);
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSendRedPacketUITag);
            }
            else
            {
                std::string sPeople = m_editPeople->getText();
                if(sPeople == "")
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("好友ID不能为空，请输入", eMBOK);
                    return ;
                }
                int nPeople = atoi(sPeople.c_str());
                if(nPeople == 0)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("好友ID不能为0，请重新输入", eMBOK);
                    return ;
                }
                std::string sCoin = m_editCount->getText();
                if(sCoin == "")
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("派发金额不能为空，请输入", eMBOK);
                    return ;
                }
                int nCoin = atoi(sCoin.c_str());
                if(nCoin == 0)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("派发金额不能为0，请重新输入", eMBOK);
                    return ;
                }
                int nUserScore = (int)CMainLogic::sharedMainLogic()->m_lUserScore;
                if(nCoin > nUserScore)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("您当前拥有金币不足，请重新输入派发红包金额", eMBOK);
                    return ;
                }
                std::string sData = m_editDes->getText();
                //检查完毕，开始向服务端发送消息，然后关闭此输入框
                CMD_GPR_FaGeRenHongBao FaGeRenHongBao = {0};
                FaGeRenHongBao.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                FaGeRenHongBao.dwGameID = nPeople;
                FaGeRenHongBao.nScore = nCoin;
                strncpy(FaGeRenHongBao.szContent,CCharCode::UTF8ToGB2312(sData.c_str()).c_str(),sizeof(FaGeRenHongBao.szContent));
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GP_FA_GE_REN_HONG_BAO,&FaGeRenHongBao,sizeof(FaGeRenHongBao));
                CMainLogic::sharedMainLogic()->ShowMessage("正在发送红包，请稍候", eMBExitGame);
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eSendFriendPacketTag);
            }
            break;
        }
        case 102://抢红包
        {
            this -> removeFromParent();
            if(GameMainScene::_instance != NULL)
            {
                GameMainScene::_instance->sendQiangRedPacket(m_nRedPacketID);
            }
            else
            {
                CMD_GPO_AskQiang AskQiang = {0};
                AskQiang.dwHongBaoID = m_nRedPacketID;
                AskQiang.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_CONFIRM_QIANG,&AskQiang,sizeof(AskQiang));
            }
            CMainLogic::sharedMainLogic()->ShowMessage("正在抢红包，请稍候", eMBExitGame);
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eRodPacketTag);
            break;
        }
        case 103:
        {
            this -> removeFromParent();
            break;
        }
        case 104:
        {
            this -> removeFromParent();
            break;
        }
        case 105://给好友发红包按钮
        {
            m_nRedOpenType = 2;
            m_editPeople->setText("");
            m_editPeople->setPlaceHolder("好友ID");
            m_menuFriends -> setVisible(false);
            break;
        }
        default:
            break;
    }
}

void RedPacketLayer::closeMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    if(m_nRedOpenType == 1)
    {
        this -> removeFromParent();
    }
    else
    {
        m_menuFriends -> setVisible(true);
        m_nRedOpenType = 1;
        m_editPeople -> setText("");
        m_editPeople -> setPlaceHolder("红包个数");
    }
}

bool RedPacketLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

void RedPacketLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -207, true);
}