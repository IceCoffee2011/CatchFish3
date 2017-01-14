#include "header.h"

enum ItemTag{
    eShowDaoJu=400,
    eShowName,
    eShowPrice,
    eShowIntroduce,
    eBlink,
};

enum MenuTag{
    eZSLayerZengSong = 700,
    eZSLayerClose,
    eShiYong,
    eZhuangbei,
    eDuiHuan,
    eZengSong,
    eMenuTag,
    eCloseMenu,
    eBuyJiNeng,
    eBuyPao,
    eCheckVip,
    eDuiHuanJinBi,
    eDuiHuanLottery,
    eDuiHuanClose,
    eChangeBarrel,
    eUseSuperPirateItem,
    eUseLaba,
};

const char* Daoju1Name[6] = {"h3all/bbtongdantou.png","h3all/bbyindantou.png", "h3all/bbjindantou.png", "h3all/jiasusprite.png", "h3all/suodingsprite.png", "h3all/labasprite.png"};

const char* Daoju1Title[6] = {"铜弹头", "银弹头", "金弹头","加速", "锁定", "喇叭"};

const char* Daoju1Description[6] = {
    "在捕鱼过程中使用此弹头后可获得20万金币。",
    "在捕鱼过程中使用此弹头后可获得40万金币",
    "在捕鱼过程中使用此弹头后可获得100万金币",
    "15秒内每发射1炮可打出5枚炮弹。",
    "15秒内可以自由点击屏幕上的鱼进行锁定。",
    "使用喇叭发布消息可以让全服的人都看到。"
};

DaojuLayer::~DaojuLayer()
{
    m_nTouchVip = 1;
    m_nZengItem = 0;
}

DaojuLayer* DaojuLayer::create(DWORD ChuJiCount, DWORD ZhongJiCount, DWORD GaoJiCount)
{
    DaojuLayer* pRet = new DaojuLayer();
    if(pRet && pRet->init(ChuJiCount, ZhongJiCount, GaoJiCount))
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

bool DaojuLayer::init(DWORD ChuJiCount, DWORD ZhongJiCount, DWORD GaoJiCount)
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_nTouchVip = 1;
    m_nZengItem = 0;
    m_nDaojuIndex = 0;
    m_nSuperPirateItemCount = 0;
    m_nChujiCount = ChuJiCount;
    m_nZhongjiCount = ZhongJiCount;
    m_nGaojiCount = GaoJiCount;
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    loadUI();
    loadZengSongLayer();
    
    this->setTouchEnabled(true);
    return true;
}

void DaojuLayer::loadUI()
{
    m_pSpBg = CCSprite::create("beibao/beibaobg.png");
    m_pSpBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(m_pSpBg, 1);
    for (int i=0; i<25; i++) {
        CCSprite* pBgDaoJu = CCSprite::create("beibao/beibaokuangbg.png");
        pBgDaoJu->setPosition(ccp(540+128*(i%5), 574-120*(i/5)));
        m_pSpBg->addChild(pBgDaoJu, 0);
    }
    
    for (int i=0; i<6; i++) {
        CCSprite* pDaoJu= CCSprite::create(Daoju1Name[i]);
        pDaoJu->setScale(0.88);
        pDaoJu->setPosition(ccp(540+128*(i%5), 574-120*(i/5)));
        m_pSpBg->addChild(pDaoJu, 1, 100+i);
        if (i == 0)
        {
            CCSprite* pBlink = CCSprite::create("beibao/beibaoselect.png");
            pBlink->setPosition(pDaoJu->getPosition());
            m_pSpBg->addChild(pBlink, 100, eBlink);
            CCFadeIn* fadeIn = CCFadeIn::create(0.5);
            CCFadeOut* fadeOut = CCFadeOut::create(0.5);
            CCRepeatForever* forever = CCRepeatForever::create(CCSequence::create(fadeIn,fadeOut,NULL));
            pBlink->runAction(forever);
        }
        
        if (i<5)
        {
            char strZhenZhu[16];
            if (i == 0) {
                sprintf(strZhenZhu, "%d", m_nChujiCount);
            }
            else if (i == 1){
                sprintf(strZhenZhu, "%d", m_nZhongjiCount);
            }
            else if (i == 2){
                sprintf(strZhenZhu, "%d", m_nGaojiCount);
            }
            else if (i == 3){
                sprintf(strZhenZhu, "%d", CMainLogic::sharedMainLogic()->m_nSpeedUpTimes);
            }
            else if (i == 4){
                sprintf(strZhenZhu, "%d", CMainLogic::sharedMainLogic()->m_nBaojiTimes);
            }
            
            CCLabelAtlas* numAtlas = CCLabelAtlas::create(strZhenZhu, "h3all/zhunzhunum.png", 14, 17, '0');
            numAtlas -> setAnchorPoint(ccp(1, 0.5f));
            numAtlas -> setScale(1.5f);
            numAtlas -> setPosition(ccp(108, 18));
            pDaoJu -> addChild(numAtlas, 1, 1);
        }
    }
    
    //左侧展示
    CCSprite* pSanBg = CCSprite::create("beibao/beibaoguang.png");
    pSanBg->setPosition(ccp(240, 470));
    m_pSpBg->addChild(pSanBg, 1, 200);
    pSanBg->runAction(CCRepeatForever::create(CCRotateBy::create(3, 180)));
    CCSprite* pSan = CCSprite::create("beibao/beibaoxing.png");
    pSan->setPosition(ccp(240, 450));
    pSan->setScale(1.2);
    m_pSpBg->addChild(pSan, 1, 201);
    
    CCLabelTTF* pShowNameTTF = CCLabelTTF::create(Daoju1Title[0], "Arial-BoldMT", 48);
    pShowNameTTF->setPosition(ccp(240, 590));
    pShowNameTTF->setColor(ccc3(161, 87, 24));
    m_pSpBg->addChild(pShowNameTTF, 1, eShowName);
    CCSprite* pShow = CCSprite::create(Daoju1Name[0]);
    pShow->setPosition(ccp(240, 470));
    m_pSpBg->addChild(pShow, 1, eShowDaoJu);
    
    CCLabelTTF* pPriceTTF = CCLabelTTF::create("", "Arial-BoldMT", 28);
    pPriceTTF->setColor(ccc3(161, 87, 24));
    pPriceTTF->setPosition(ccp(240, 330));
    m_pSpBg->addChild(pPriceTTF, 1, eShowPrice);
    
    CCSize size = CCSizeMake(360, 170);
    CCLabelTTF* pIntroduceTTF = CCLabelTTF::create(Daoju1Description[0], "Arial-BoldMT", 28,size,kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    pIntroduceTTF->setAnchorPoint(ccp(0, 1));
    pIntroduceTTF->setColor(ccc3(161, 87, 24));
    pIntroduceTTF->setPosition(ccp(70, 450-pIntroduceTTF->getContentSize().height));
    m_pSpBg->addChild(pIntroduceTTF, 1, eShowIntroduce);
    CCMenuItemImage * closeItem = CCMenuItemImage::create("Dragon/dragoncloseitem.png","Dragon/dragoncloseitem.png",this,menu_selector(DaojuLayer::closeMenu));
    closeItem->setPosition(ccp(m_pSpBg->getContentSize().width-20, m_pSpBg->getContentSize().height-20));
    CCMenu* closeMenu = CCMenu::create(closeItem, NULL);
    closeMenu->setTouchPriority(-130);
    closeMenu -> setPosition(CCPointZero);
    m_pSpBg->addChild(closeMenu,2,eCloseMenu);
    
    if(GameMainScene::_instance == NULL)
    {
        CCMenuItemImage* buyItem = CCMenuItemImage::create("h3all/zengsongitem.png", "h3all/zengsongitem.png", this, menu_selector(DaojuLayer::clickMenu));
        buyItem -> setTag(eZengSong);//赠送
        CCMenu* menu = CCMenu::create(buyItem, NULL);
        menu -> alignItemsHorizontallyWithPadding(10);
        menu -> setPosition(ccp(240, 80));
        menu -> setTouchPriority(-130);
        m_pSpBg -> addChild(menu, 1, eMenuTag);
    }
    else
    {
        CCMenuItemImage* shiyongItem = CCMenuItemImage::create("h3all/shiyongitem.png", "h3all/shiyongitem.png", this, menu_selector(DaojuLayer::clickMenu));
        CCMenuItemImage* buyItem = CCMenuItemImage::create("h3all/zengsongitem.png", "h3all/zengsongitem.png", this, menu_selector(DaojuLayer::clickMenu));
        shiyongItem -> setTag(eShiYong);//开启
        buyItem -> setTag(eZengSong);//赠送
        CCMenu* menu = CCMenu::create(shiyongItem, buyItem, NULL);
        menu -> alignItemsHorizontallyWithPadding(10);
        menu -> setPosition(ccp(240, 80));
        menu -> setTouchPriority(-130);
        m_pSpBg -> addChild(menu, 1, eMenuTag);
    }
    
    m_pSpBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void DaojuLayer::loadZengSongLayer()
{
    //赠送界面
    m_pZengLayer = CCLayerColor::create(ccc4(0, 0, 0, 100), CLIENT_WIDTH, CLIENT_HEIGHT);
    m_pZengLayer -> ignoreAnchorPointForPosition(false);
    m_pZengLayer -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_pZengLayer, 2);
    m_layerZeng = CCSprite::create("Dragon/zengsongbg.png");
    m_layerZeng -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_layerZeng, 2);
    m_pZengLayer->setVisible(false);
    m_layerZeng->setVisible(false);
    m_pUserID = CCEditBox::create(CCSize(440,55), CCScale9Sprite::create("Dragon/bankkuang.png"));
    m_pUserID->setFontSize(16);
    m_pUserID->setMaxLength(20);
    m_pUserID->setFontColor(ccc3(198,80,9));
    m_pUserID->setReturnType(kKeyboardReturnTypeDone);
    m_pUserID->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_pUserID->setInputMode(kEditBoxInputModePhoneNumber);
    m_layerZeng->addChild(m_pUserID);
    m_pUserID->setTouchPriority(-130);
    m_pUserID -> setPosition(ccp(460, 295));
    m_pUserNum = CCEditBox::create(CCSize(440,55), CCScale9Sprite::create("Dragon/bankkuang.png"));
    m_pUserNum->setFontSize(16);
    m_pUserNum->setMaxLength(20);
    m_pUserNum->setFontColor(ccc3(198,80,9));
    m_pUserNum->setReturnType(kKeyboardReturnTypeDone);
    m_pUserNum->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_pUserNum->setInputMode(kEditBoxInputModePhoneNumber);
    m_layerZeng->addChild(m_pUserNum);
    m_pUserNum->setTouchPriority(-130);
    m_pUserNum -> setPosition(ccp(460, 205));
    CCMenuItemImage* zfont = CCMenuItemImage::create("Dragon/zegnsongitem.png", "Dragon/zegnsongitem.png", this, menu_selector(DaojuLayer::clickMenu));
    CCMenuItemImage* zengCloseItem = CCMenuItemImage::create("Dragon/dragoncloseitem.png","Dragon/dragoncloseitem.png",this,menu_selector(DaojuLayer::clickMenu));
    zfont -> setPosition(ccp(m_layerZeng->getContentSize().width/2, 70));
    zengCloseItem -> setPosition(ccp(m_layerZeng->getContentSize().width-20,m_layerZeng->getContentSize().height-20));
    zfont -> setTag(eZSLayerZengSong);//赠送
    zengCloseItem -> setTag(eZSLayerClose);//赠送界面关闭按钮
    
    CCMenu* pZengMenu = CCMenu::create(zfont, zengCloseItem, NULL);
    pZengMenu -> setPosition(CCPointZero);
    pZengMenu -> setAnchorPoint(CCPointZero);
    m_layerZeng -> addChild(pZengMenu);
    pZengMenu -> setTouchPriority(-130);
}

void DaojuLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool DaojuLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if (m_pZengLayer->isVisible())
    {
        return true;
    }
    CCPoint point = pTouch->getLocation();
    point = m_pSpBg->convertToNodeSpace(point);
    for(int i = 0; i < 6; i++)
    {
        CCSprite* pDaoju = (CCSprite*)m_pSpBg->getChildByTag(100+i);
        if(pDaoju == NULL)
        {
            return false;
        }
        CCRect rect = pDaoju->boundingBox();
        if(rect.containsPoint(point))
        {
            m_nDaojuIndex = i;
            if(i != 5)
            {
                SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            }
            
            
            if (i<3)
            {
                m_nZengItem = i;
            }
            else
            {
                m_nZengItem = -1;
            }
            
            CCMenu* beforMenu = (CCMenu*)m_pSpBg->getChildByTag(eMenuTag);
            if (beforMenu != NULL) {
                beforMenu->removeAllChildrenWithCleanup(true);
                beforMenu->removeFromParentAndCleanup(true);
            }
            
            CCSprite* pBlink = (CCSprite*)m_pSpBg->getChildByTag(eBlink);
            if (pBlink != NULL) {
                pBlink->stopAllActions();
                pBlink->setPosition(pDaoju->getPosition());
                CCFadeIn* fadeIn = CCFadeIn::create(0.5);
                CCFadeOut* fadeOut = CCFadeOut::create(0.5);
                CCRepeatForever* forever = CCRepeatForever::create(CCSequence::create(fadeIn,fadeOut,NULL));
                pBlink->runAction(forever);
            }
            //左侧
            CCLabelTTF* pShowNameTTF = (CCLabelTTF*)m_pSpBg->getChildByTag(eShowName);
            if (pShowNameTTF != NULL) {
                pShowNameTTF->setString(Daoju1Title[i]);
            }
            CCSprite* pShow = (CCSprite*)m_pSpBg->getChildByTag(eShowDaoJu);
            if (pShow != NULL) {
                CCTexture2D* texture2D = CCTextureCache::sharedTextureCache()->addImage(Daoju1Name[i]);
                CCSize size  = CCSprite::create(Daoju1Name[i])->getContentSize();
                CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(texture2D, CCRectMake(0,0,size.width,size.height));
                pShow->setDisplayFrame(frame);
            }
            
            CCLabelTTF* pIntroduceTTF = (CCLabelTTF*)m_pSpBg->getChildByTag(eShowIntroduce);
            if (pIntroduceTTF != NULL) {
                pIntroduceTTF->setString(Daoju1Description[i]);
            }
            
            //价格或者获取途径
            CCLabelTTF* pPriceTTF = (CCLabelTTF*)m_pSpBg->getChildByTag(eShowPrice);
            if (pPriceTTF != NULL) {
                if (i<3) {
                    pPriceTTF->setString("");
                }
                else if (i < 5) {
                    pPriceTTF->setString("价格：2000金币/个");
                }
                else if(i == 5)
                {
                    pPriceTTF->setString("");//("价格：10万金币/个");
                }
                pPriceTTF->setVisible(true);
            }
            //珍珠
            if (pDaoju->getTag()>=100 && pDaoju->getTag()<103)
            {
                if(GameMainScene::_instance == NULL)
                {
                    CCMenuItemImage* buyItem = CCMenuItemImage::create("h3all/zengsongitem.png", "h3all/zengsongitem.png", this, menu_selector(DaojuLayer::clickMenu));
                    buyItem -> setTag(eZengSong);//赠送
                    CCMenu* menu = CCMenu::create(buyItem, NULL);
                    menu -> alignItemsHorizontallyWithPadding(10);
                    menu -> setPosition(ccp(240, 80));
                    menu -> setTouchPriority(-130);
                    m_pSpBg -> addChild(menu, 1, eMenuTag);
                }
                else
                {
                    CCMenuItemImage* shiyongItem = CCMenuItemImage::create("h3all/shiyongitem.png", "h3all/shiyongitem.png", this, menu_selector(DaojuLayer::clickMenu));
                    CCMenuItemImage* buyItem = CCMenuItemImage::create("h3all/zengsongitem.png", "h3all/zengsongitem.png", this, menu_selector(DaojuLayer::clickMenu));
                    shiyongItem -> setTag(eShiYong);//开启
                    buyItem -> setTag(eZengSong);//赠送
                    CCMenu* menu = CCMenu::create(shiyongItem, buyItem, NULL);
                    menu -> alignItemsHorizontallyWithPadding(10);
                    menu -> setPosition(ccp(240, 80));
                    menu -> setTouchPriority(-130);
                    m_pSpBg -> addChild(menu, 1, eMenuTag);
                }
                
            }
            //技能
            else if (pDaoju->getTag()>=103 && pDaoju->getTag()<105)
            {
                CCMenuItemImage* itemBuy = CCMenuItemImage::create("h3all/goumaiitem.png", "h3all/goumaiitem.png", this, menu_selector(DaojuLayer::clickMenu));
                itemBuy->setTag(eBuyJiNeng);
                itemBuy->setPosition(ccp(0, 0));
                CCMenu* menu = CCMenu::create(itemBuy, NULL);
                menu->setPosition(ccp(240, 80));
                menu->setTouchPriority(-130);
                m_pSpBg->addChild(menu, 1, eMenuTag);
            }
            else if(pDaoju->getTag() == 105)//喇叭
            {
                CCMenuItemImage* itemShiyong = CCMenuItemImage::create("h3all/shiyongitem.png", "h3all/shiyongitem.png", this, menu_selector(DaojuLayer::clickMenu));
                itemShiyong->setTag(eUseLaba);
                itemShiyong->setPosition(ccp(0, 0));
                CCMenu* menu = CCMenu::create(itemShiyong, NULL);
                menu->setPosition(ccp(240, 80));
                menu->setTouchPriority(-130);
                m_pSpBg->addChild(menu, 1, eMenuTag);
                clickMenu(itemShiyong);
            }
            
            if (i>=3 && i<=4)//购买技能
            {
                CMainLogic::sharedMainLogic()->m_nBuyCannon2Index = pDaoju->getTag()-100+18;
            }

            break;
        }
    }
    
    return true;
}

void DaojuLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
}

void DaojuLayer::clickMenu(CCObject* obj)
{
    CCMenuItem* item = (CCMenuItem*)obj;
    switch (item->getTag()) {
        case eShiYong://开启
        {
            //判断当前珍珠数量
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            if(m_nZengItem == 0)
            {
                if(m_nChujiCount == 0)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，您当前拥有铜弹头数量不足。", eMBOK);
                    break;
                }
            }
            else if(m_nZengItem == 1)
            {
                if(m_nZhongjiCount == 0)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，您当前拥有银弹头数量不足。", eMBOK);
                    break;
                }
            }
            else if(m_nZengItem == 2)
            {
                if(m_nGaojiCount == 0)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，您当前拥有金弹头数量不足。", eMBOK);
                    break;
                }
            }
            if(GameMainScene::_instance == NULL)
            {
                CMD_GPR_UseDragon UseDragon;
                UseDragon.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                UseDragon.nDragonType = m_nZengItem;
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_ITEM, SUB_GPR_USE_DRAGON, &UseDragon,sizeof(UseDragon));
                CMainLogic::sharedMainLogic()->ShowMessage("正在使用，请稍候", eMBExitGame);
            }
            else
            {
                if(m_nZengItem == 0)//铜弹头
                {
                    GameMainScene::_instance->setSelectDantouType(0);
                    GameMainScene::_instance->showDantouInfo();
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameUserTongDantou);
                }
                else if(m_nZengItem == 1)//银弹头
                {
                    GameMainScene::_instance->setSelectDantouType(1);
                    GameMainScene::_instance->showDantouInfo();
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameUserYinDantou);
                }
                else if(m_nZengItem == 2)//金弹头
                {
                    GameMainScene::_instance->setSelectDantouType(2);
                    GameMainScene::_instance->showDantouInfo();
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameUserJinDantou);
                }
                this -> removeFromParent();
                if(GameMainScene::_instance != NULL)
                {
                    GameMainScene::GetInstance()->setWndHave(false);
                    GameMainScene::GetInstance()->setAutoFire();
                    GameMainScene::GetInstance()->setMatchShowStatus(true);
                    GameMainScene::GetInstance()->setClickDantouMenu(false);
                    GameMainScene::GetInstance()->setDantouUsing(true);
                }
            }
            break;
        }
        case eZengSong://赠送
        {
            //判断当前珍珠数量
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            if(m_nZengItem == 0)
            {
                if(m_nChujiCount == 0)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("对不起，您拥有的铜弹头数量不足，不能进行本次操作。", eMBOK);
                    break;
                }
            }
            else if(m_nZengItem == 1)
            {
                if(m_nZhongjiCount == 0)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("对不起，您拥有的银弹头数量不足，不能进行本次操作。", eMBOK);
                    break;
                }
            }
            else if(m_nZengItem == 2)
            {
                if(m_nGaojiCount == 0)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("对不起，您拥有的金弹头数量不足，不能进行本次操作。", eMBOK);
                    break;
                }
            }
//            if(GameMainScene::_instance != NULL)
//            {
//                CMainLogic::sharedMainLogic()->ShowMessage("非常抱歉，本次操作失败了。您可以返回游戏大厅再试一试。", eMBOK);
//                break;
//            }
            m_layerZeng->setVisible(true);
            m_pZengLayer->setVisible(true);
            m_layerZeng->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
            CCMenu* menu = (CCMenu*)m_pSpBg->getChildByTag(eMenuTag);
            if (menu != NULL)
            {
                menu->setTouchEnabled(false);
            }
            CCMenu* closeMenu = (CCMenu*)m_pSpBg->getChildByTag(eCloseMenu);
            if (closeMenu != NULL)
            {
                closeMenu->setTouchEnabled(false);
            }
            break;
        }
        case eZSLayerZengSong://赠送界面上的按钮--赠送
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            presentZhenzhu();
            break;
        }
        case eZSLayerClose://赠送界面上的按钮--关闭
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
            m_pZengLayer->setVisible(false);
            m_layerZeng->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), CCHide::create(), NULL));
            CCMenu* menu = (CCMenu*)m_pSpBg->getChildByTag(eMenuTag);
            if (menu != NULL)
            {
                menu->setTouchEnabled(true);
            }
            CCMenu* closeMenu = (CCMenu*)m_pSpBg->getChildByTag(eCloseMenu);
            if (closeMenu != NULL)
            {
                closeMenu->setTouchEnabled(true);
            }
            break;
        }
        case eBuyJiNeng:
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            if((CMainLogic::sharedMainLogic()->m_nBuyCannon2Index>=21 && CMainLogic::sharedMainLogic()->m_nBuyCannon2Index<=22))
            {
                if(CMainLogic::sharedMainLogic()->m_nBuyCannon2Index == 21)
                {
                    BuyBarrelLayer* pBuyBarrelLayer = BuyBarrelLayer::create(1, 2);
                    pBuyBarrelLayer -> setPosition(ccp(0, 0));
                    this -> addChild(pBuyBarrelLayer, 100, 1009);
                }
                else if(CMainLogic::sharedMainLogic()->m_nBuyCannon2Index == 22)//现在改为锁定技能
                {
                    BuyBarrelLayer* pBuyBarrelLayer = BuyBarrelLayer::create(3, 2);
                    pBuyBarrelLayer -> setPosition(ccp(0, 0));
                    this -> addChild(pBuyBarrelLayer, 100, 1009);
                }
                
            }
            break;
        }
        case eUseLaba:
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            //进入喇叭界面
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallLaba);
            CMainLogic::sharedMainLogic()->ShowMessage("正在请求，请稍候", eMBExitGame);
            CMD_GPR_LabaTimes LabaTimes = {0};
            LabaTimes.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
            CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_LABA_TIMES, &LabaTimes,sizeof(LabaTimes));
            break;
        }
        default:
            break;
    }
}

void DaojuLayer::closeMenu(CCObject* obj)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(DaojuLayer::removeWnd));
    m_pSpBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
}

void DaojuLayer::removeWnd()
{
    this -> removeFromParent();
    if(GameMainScene::_instance != NULL)
    {
        GameMainScene::GetInstance()->setWndHave(false);
        GameMainScene::GetInstance()->setAutoFire();
        GameMainScene::GetInstance()->setMatchShowStatus(true);
    }
}

void DaojuLayer::updateSpriteUI(int nChujiCount, int nZhongjiCount, int nGaojiCount)
{
    m_nChujiCount = nChujiCount;
    m_nZhongjiCount = nZhongjiCount;
    m_nGaojiCount = nGaojiCount;
    
    CCSprite* pChuJi = (CCSprite*)m_pSpBg->getChildByTag(100);
    if (pChuJi != NULL) {
        CCLabelAtlas* numAtlas = (CCLabelAtlas*)pChuJi->getChildByTag(1);
        if (numAtlas != NULL) {
            char numStr[16];
            sprintf(numStr, "%d", nChujiCount);
            numAtlas->setString(numStr);
        }
    }
    CCSprite* pZhongJi = (CCSprite*)m_pSpBg->getChildByTag(101);
    if (pZhongJi != NULL) {
        CCLabelAtlas* numAtlas = (CCLabelAtlas*)pZhongJi->getChildByTag(1);
        if (numAtlas != NULL) {
            char numStr[16];
            sprintf(numStr, "%d", nZhongjiCount);
            numAtlas->setString(numStr);
        }
    }
    CCSprite* pGaoJi = (CCSprite*)m_pSpBg->getChildByTag(102);
    if (pGaoJi != NULL) {
        CCLabelAtlas* numAtlas = (CCLabelAtlas*)pGaoJi->getChildByTag(1);
        if (numAtlas != NULL) {
            char numStr[16];
            sprintf(numStr, "%d", nGaojiCount);
            numAtlas->setString(numStr);
        }
    }
}

void DaojuLayer::updateZhenzhuUI(int nItemID, int nZhenzhuCount)
{
    if(nItemID == 0)
    {
        m_nChujiCount = nZhenzhuCount;
    }
    else if(nItemID == 1)
    {
        m_nZhongjiCount = nZhenzhuCount;
    }
    else if(nItemID == 2)
    {
        m_nGaojiCount = nZhenzhuCount;
    }
    updateSpriteUI(m_nChujiCount, m_nZhongjiCount, m_nGaojiCount);
}

void DaojuLayer::presentZhenzhu()
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    std::string userIdText = m_pUserID->getText();
    if(userIdText == "")
    {
        CMainLogic::sharedMainLogic()->ShowMessage("请您输入赠送人的ID",eMBOK);
        return;
    }
    std::string dragonNum = m_pUserNum->getText();
    if(dragonNum == "")
    {
        CMainLogic::sharedMainLogic()->ShowMessage("请您输入想要赠送的数量。",eMBOK);
        return;
    }
    if(atoi(dragonNum.c_str()) == 0)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，不能赠送0个，请输入具体的赠送数量。",eMBOK);
        return;
    }
    int nDragonNum = atoi(dragonNum.c_str());
    if(m_nZengItem == 0)//赠送小龙珠
    {
        if(nDragonNum > m_nChujiCount)
        {
            CMainLogic::sharedMainLogic()->ShowMessage("对不起，您拥有的铜弹头数量不足，请检查数量后再试一次。",eMBOK);
            return;
        }
    }
    else if(m_nZengItem == 1)//赠送大龙珠
    {
        if(nDragonNum > m_nZhongjiCount)
        {
            CMainLogic::sharedMainLogic()->ShowMessage("对不起，您拥有的银弹头数量不足，请检查数量后再试一次。",eMBOK);
            return;
        }
    }
    else if(m_nZengItem == 2)//赠送高级珍珠
    {
        if(nDragonNum > m_nGaojiCount)
        {
            CMainLogic::sharedMainLogic()->ShowMessage("对不起，您拥有的金弹头数量不足，请检查数量后再试一次。",eMBOK);
            return;
        }
    }
    std::string sPassword = "";
    //自动输入密码
    if(CCUserDefault::sharedUserDefault()->getBoolForKey("NewGuestBank"))
    {
        sPassword = CCUserDefault::sharedUserDefault()->getStringForKey("NewGuestPassword");
    }
    else
    {
        sPassword = CCUserDefault::sharedUserDefault()->getStringForKey("NewPassword").c_str();
    }
    
    if(GameMainScene::_instance != NULL)//游戏中赠送弹头
    {
        if(m_nZengItem == 0)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameZengTongDantou);
        }
        else if(m_nZengItem == 1)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameZengYinDantou);
        }
        else if(m_nZengItem == 2)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameZengJinDantou);
        }
    }
    else
    {
        if(m_nZengItem == 0)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallZengTongDantou);
        }
        else if(m_nZengItem == 1)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallZengYinDantou);
        }
        else if(m_nZengItem == 2)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallZengJinDantou);
        }
    }
    
    CMD_GPR_TransferItem TransferItem;
    TransferItem.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
    TransferItem.dwTargetGameID = atoi(userIdText.c_str());
    TransferItem.nItemID = m_nZengItem;
    TransferItem.nItemCount = nDragonNum;
    char szMD5Result[128];
    CEncryptMD5::toString32(sPassword.c_str(),szMD5Result);
    strncpy(TransferItem.szPassword,szMD5Result,sizeof(TransferItem.szPassword));
    CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_ITEM, SUB_GPR_TRANSFER_ITEM, &TransferItem,sizeof(TransferItem));
    CMainLogic::sharedMainLogic()->ShowMessage("正在赠送，请稍候", eMBExitGame);
}

//金币购买技能更新数量
void DaojuLayer::updateSkillUI(int nSkillType, int nSkillCount)
{
    //加速tag为103，散射为104，锁定为105
    //散射技能去掉后，加速tag为103，锁定为104，所以这里如果nSkillType如果为2的话，那么就要减1
    int nType = 0;
    if(nSkillType == 2)
    {
        nType = 1;
    }
    CCSprite* pKuang = (CCSprite*)m_pSpBg->getChildByTag(103+nType);
    if (pKuang != NULL)
    {
        CCLabelAtlas* numAtlas = (CCLabelAtlas*)pKuang->getChildByTag(1);
        if (numAtlas != NULL)
        {
            char numStr[16];
            sprintf(numStr, "%d", nSkillCount);
            numAtlas->setString(numStr);
        }
    }
}