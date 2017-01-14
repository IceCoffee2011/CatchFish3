//
//  MailLayer.cpp
//  CatchFish
//
//  Created by 河北腾游 on 15/8/6.
//
//

#include "header.h"

MailLayer::MailLayer()
{
}

MailLayer* MailLayer::create()
{
    MailLayer* pRet = new MailLayer;
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
    }
    return pRet;
}

bool MailLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCLayerColor* layer = CCLayerColor::create(ccc4(0,0,0,190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this -> addChild(layer);
    
    m_nCurrentMailID = 0;
    m_nSelectedMailID = 0;
    m_sDuihuanma = "";
    m_sGongzhonghao = "";
    
    
    loadUIBg();
    loadUIContain();
    
    return true;
}

time_t StringToDatetime(char *str)
{
    tm tm_;
    int year, month, day, hour, minute,second;
    sscanf(str,"%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    tm_.tm_year  = year-1900;
    tm_.tm_mon   = month-1;
    tm_.tm_mday  = day;
    tm_.tm_hour  = hour;
    tm_.tm_min   = minute;
    tm_.tm_sec   = second;
    tm_.tm_isdst = 0;
    
    time_t t_ = mktime(&tm_); //已经减了8个时区
    return t_; //秒时间
}

void MailLayer::loadUIBg()
{
    m_nodeMail = CCSprite::create();
    m_nodeMail->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_nodeMail);
    
    pMailBg = CCSprite::create("mail/mailbg.png");
    pMailBg -> setPosition(ccp(0, -20));
    m_nodeMail -> addChild(pMailBg);
    
    CCMenuItemImage* closeButton = CCMenuItemImage::create("Dragon/dragoncloseitem.png", "Dragon/dragoncloseitem.png",this,menu_selector(MailLayer::closeLayer));
    closeButton->setPosition(ccp(pMailBg->getContentSize().width-40, pMailBg->getContentSize().height-40));
    closeButton -> setScale(1.2f);
    CCMenu* pMenuClose = CCMenu::create(closeButton, NULL);
    pMenuClose->setPosition(CCPointZero);
    pMenuClose->setAnchorPoint(CCPointZero);
    pMailBg->addChild(pMenuClose, 2);
    pMenuClose->setTouchPriority(-200);
    
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        CCLabelTTF* pLabelTTF = CCLabelTTF::create("暂时还没有邮件哦", "Airal", 46);
        pLabelTTF -> setPosition(ccp(pMailBg->getContentSize().width/2, pMailBg->getContentSize().height/2-20));
        pMailBg -> addChild(pLabelTTF);
        
        m_nodeMail->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
        this -> setTouchEnabled(true);
        return ;
    }
    if(CMainLogic::sharedMainLogic()->m_vctMail.size() == 0)
    {
        CCLabelTTF* pLabelTTF = CCLabelTTF::create("暂时还没有邮件哦", "Airal", 46);
        pLabelTTF -> setPosition(ccp(pMailBg->getContentSize().width/2, pMailBg->getContentSize().height/2-20));
        pMailBg -> addChild(pLabelTTF);
    }
    
    
    
    unsigned int count = (unsigned int)CMainLogic::sharedMainLogic()->m_vctMail.size();
    if(count == 1)
    {
        m_tableView = CCTableView::create(this,CCSizeMake(850, 115));
        m_tableView -> setPosition(ccp(60, 44+310));
        m_tableView -> setBounceable(false);
    }
    else if(count == 2)
    {
        m_tableView = CCTableView::create(this,CCSizeMake(850, 240));
        m_tableView -> setPosition(ccp(60, 44+190));
        m_tableView -> setBounceable(false);
    }
    else if(count == 3)
    {
        m_tableView = CCTableView::create(this,CCSizeMake(850, 365));
        m_tableView -> setPosition(ccp(60, 44+60));
        m_tableView -> setBounceable(false);
    }
    else
    {
        m_tableView = CCTableView::create(this,CCSizeMake(850, 420));
        m_tableView -> setPosition(ccp(55, 64));
        m_tableView -> setBounceable(true);
    }
    
    m_tableView -> setDirection(kCCScrollViewDirectionVertical);
    
    m_tableView -> setDelegate(this);
    m_tableView -> setAnchorPoint(CCPointZero);
    m_tableView -> setTouchPriority(-200);
    
    m_tableView -> setTouchEnabled(true);
    pMailBg -> addChild(m_tableView, 3);
    
    
    m_nodeMail->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
    this -> setTouchEnabled(true);
}

void MailLayer::loadUIContain()
{
    m_nodeContain = CCSprite::create();
    m_nodeContain -> setVisible(false);
    m_nodeMail->addChild(m_nodeContain);

    float width = 780;
    float height = 340;
    m_pSpDi = CCScale9Sprite::create("public2wnd/bgdi.png");
    m_pSpDi -> setContentSize(CCSizeMake(width, height));
    m_pSpDi -> setPosition(ccp(0, -60));
    m_nodeContain -> addChild(m_pSpDi, 1, 1000);
    
    CCSprite* pSpLeft1 = CCSprite::create("public2wnd/bgleft1.png");
    pSpLeft1 -> setPosition(ccp(0, m_pSpDi->getContentSize().height));
    m_pSpDi -> addChild(pSpLeft1, 1);
    CCSprite* pSpLeft2 = CCSprite::create("public2wnd/bgleft1.png");
    pSpLeft2 -> setPosition(ccp(0, 0));
    pSpLeft2 -> setFlipY(true);
    m_pSpDi -> addChild(pSpLeft2, 1);
    CCSprite* pSpRight1 = CCSprite::create("public2wnd/bgleft1.png");
    pSpRight1 -> setPosition(ccp(m_pSpDi->getContentSize().width, m_pSpDi->getContentSize().height));
    m_pSpDi -> addChild(pSpRight1, 1);
    pSpRight1 -> setFlipX(true);
    CCSprite* pSpRight2 = CCSprite::create("public2wnd/bgleft1.png");
    pSpRight2 -> setPosition(ccp(m_pSpDi->getContentSize().width, 0));
    m_pSpDi -> addChild(pSpRight2, 1);
    pSpRight2 -> setFlipX(true);
    pSpRight2 -> setFlipY(true);
    
    CCSprite* pSpMiddleLeft = CCSprite::create("public2wnd/bgmiddleleft.png");
    pSpMiddleLeft -> setPosition(ccp(83, m_pSpDi->getContentSize().height/2));
    pSpMiddleLeft -> setScaleY(((float)height-pSpLeft2->getContentSize().height)/pSpMiddleLeft->getContentSize().height);
    m_pSpDi -> addChild(pSpMiddleLeft, 1);
    CCSprite* pSpMiddleRight = CCSprite::create("public2wnd/bgmiddleright.png");
    pSpMiddleRight -> setPosition(ccp(m_pSpDi->getContentSize().width-106, m_pSpDi->getContentSize().height/2));
    pSpMiddleRight -> setScaleY((height-pSpLeft2->getContentSize().height)/pSpMiddleRight->getContentSize().height);
    m_pSpDi -> addChild(pSpMiddleRight, 1);
    CCSprite* pSpMiddleTop = CCSprite::create("public2wnd/bgmiddletop.png");
    pSpMiddleTop -> setPosition(ccp(m_pSpDi->getContentSize().width/2, m_pSpDi->getContentSize().height-26));
    pSpMiddleTop -> setScaleX((width-pSpRight2->getContentSize().width)/pSpMiddleTop->getContentSize().width);
    m_pSpDi -> addChild(pSpMiddleTop, 1);
    
    CCSprite* pSpMiddleBottom = CCSprite::create("public2wnd/bgmiddlebottom.png");
    pSpMiddleBottom -> setPosition(ccp(m_pSpDi->getContentSize().width/2, 26));
    pSpMiddleBottom -> setScaleX((width-pSpRight2->getContentSize().width)/pSpMiddleBottom->getContentSize().width);
    m_pSpDi -> addChild(pSpMiddleBottom, 1);
    
    CCLabelTTF * ttfTotal = CCLabelTTF::create("邮件内容:", "Arial-BoldMT", 46);
    ttfTotal -> setAnchorPoint(ccp(0.5, 0.5));
    ttfTotal-> setPosition(ccp(90, height));
    m_pSpDi -> addChild(ttfTotal, 1);
    ttfTotal -> setColor(ccc3(255, 255, 0));

    
    m_pSpDi -> setOpacity(0);
    pSpLeft1 -> setOpacity(0);
    pSpLeft2 -> setOpacity(0);
    pSpRight1 -> setOpacity(0);
    pSpRight2 -> setOpacity(0);
    pSpMiddleLeft -> setOpacity(0);
    pSpMiddleRight -> setOpacity(0);
    pSpMiddleTop -> setOpacity(0);
    pSpMiddleBottom -> setOpacity(0);

    //关闭按钮
    CCMenuItemImage* pCloseItem = CCMenuItemImage::create("mail/advertItem1.png","mail/advertItem2.png", this, menu_selector(MailLayer::closeContent));
    pCloseItem -> setPosition(ccp(width+50, 17));
    CCMenu* pCloseMenu = CCMenu::create(pCloseItem, NULL);
    pCloseMenu -> setPosition(CCPointZero);
    pCloseMenu -> setAnchorPoint(CCPointZero);
    pCloseItem -> setScale(1.5);
    m_pSpDi -> addChild(pCloseMenu, 1);
    pCloseMenu -> setTouchPriority(-200);
    pCloseMenu -> setVisible(false);
    //领取按钮
    CCMenuItemImage* pGetItem = CCMenuItemImage::create("mail/emailGet.png", "mail/emailGet.png", this, menu_selector(MailLayer::getClickMenu));
    pGetItem -> setPosition(ccp(width-100, 15));
    m_menuGet = CCMenu::create(pGetItem, NULL);
    m_menuGet -> setPosition(CCPointZero);
    pGetItem -> setScale(0.7);
    m_menuGet -> setAnchorPoint(CCPointZero);
    m_pSpDi -> addChild(m_menuGet, 1);
    m_menuGet -> setTouchPriority(-200);
    m_menuGet -> setVisible(false);
    //已领取状态
    m_spGet = CCSprite::create("mail/emailGot.png");
    m_spGet -> setPosition(ccp(width-100, 15));
    m_spGet -> setScale(0.7f);
    m_pSpDi -> addChild(m_spGet, 1);
    m_spGet -> setVisible(false);
}

void MailLayer::getClickMenu(CCObject* object)
{
    if(GameMainScene::_instance != NULL)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("对不起，领取失败。请返回游戏大厅再进行此操作", eMBOK);
    }
    else
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallMailLingqu);
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        //发送领取消息
        CMainLogic::sharedMainLogic()->ShowMessage("正在领取，请稍候", eMBExitGame);
        CMD_GPR_MailGetInfo MailGetInfo = {0};
        MailGetInfo.dwMailID = m_nCurrentMailID;
        CMainLogic::sharedMainLogic() -> SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_QUERY_MAIL_ITEM, &MailGetInfo, sizeof(CMD_GPR_MailGetInfo));
    }
    
}

void MailLayer::removeWnd()
{
    this -> removeFromParentAndCleanup(true);
}

void MailLayer::closeLayer(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(MailLayer::removeWnd));
        m_nodeMail->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
        return ;
    }
    if (m_nodeContain->isVisible()) {
        m_nodeContain -> setVisible(false);
        m_tableView -> setTouchEnabled(true);
        m_tableView -> setVisible(true);
        
        //删除公众号按钮和兑换码按钮
        CCMenu* pMenu = (CCMenu*)m_pSpDi->getChildByTag(1998);
        if(pMenu != NULL)
        {
            pMenu -> removeFromParent();
        }
        pMenu = (CCMenu*)m_pSpDi->getChildByTag(1999);
        if(pMenu != NULL)
        {
            pMenu -> removeFromParent();
        }
        for (int i=0; i<4; i++) {
            CCScrollView* pScrollView = (CCScrollView*)m_pSpDi->getChildByTag(1000);
            if (pScrollView != NULL) {
                pScrollView->removeFromParentAndCleanup(true);
            }
            CCSprite* pMailItem = (CCSprite*)m_pSpDi->getChildByTag(1010+i);
            if (pMailItem != NULL) {
                pMailItem->removeFromParentAndCleanup(true);
            }
            CCSprite* pMailItem2 = (CCSprite*)m_pSpDi->getChildByTag(1020+i);
            if (pMailItem2 != NULL) {
                pMailItem2->removeFromParentAndCleanup(true);
            }
        }
        return;
    }
    
    //把结构体中的内容保存到xml文件中
    CMainLogic::sharedMainLogic()->writeMailXML();
    
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(MailLayer::removeWnd));
    m_nodeMail->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
    
    //开启请求邮件定时器
    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
    {
        if(CCDirector::sharedDirector()->getRunningScene() == CMainLogic::sharedMainLogic()->m_pHallScene)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                pHallLayer -> schedule(schedule_selector(CMainLogic::requestMail), 300);
            }
        }
    }
    else if(GameMainScene::_instance != NULL)
    {
        GameMainScene::_instance->schedule(schedule_selector(CMainLogic::requestMail), 300);
    }
}

void MailLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -200, true);
}

bool MailLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    m_point = pTouch->getLocation();
    m_point = pMailBg->convertToNodeSpace(m_point);
    return true;
}

void MailLayer::scrollViewDidScroll(CCScrollView* view)
{
    
}

void MailLayer::scrollViewDidZoom(CCScrollView* view)
{
    
}

//CCTableViewDelegate
void MailLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)//当点击单元格格时触发的方法
{
    //单元格中CCTableViewCell类中有一个属性idx，可以判断点击的这个单元格的索引
    int idx = cell->getIdx();
    //通过idx来查找点击的是哪条标题，向服务端发送消息
    if(idx <= CMainLogic::sharedMainLogic()->m_vctMail.size())
    {
        //点击单元格，不管已读还是未读，都想服务端请求邮件内容
        m_nSelectedMailID = CMainLogic::sharedMainLogic()->m_vctMail[idx].dwMailID;
        openMailContent();
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
        CMainLogic::sharedMainLogic()->ShowMessage("正在获取邮件内容", eMBExitGame);
        CCSprite* pSpCellBg = (CCSprite*)cell->getChildByTag(100);
        if(pSpCellBg != NULL)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallMailChakan);
            CCSprite* pSpRead = (CCSprite*)pSpCellBg->getChildByTag(100);
            if(pSpRead != NULL)
            {
                pSpRead -> setVisible(true);
            }
            pSpRead = (CCSprite*)pSpCellBg->getChildByTag(104);
            if(pSpRead != NULL)
            {
                pSpRead -> setVisible(false);
            }
        }
    }
}

void MailLayer::openMailContent()
{
    m_tableView->setVisible(false);
    CMD_GPR_MailContent MailContent;
    MailContent.dwMailID = m_nSelectedMailID;
    CMainLogic::sharedMainLogic()->m_nReadingMailID = MailContent.dwMailID;
    CMainLogic::sharedMainLogic() -> SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GPR_QUERY_MAIL_CONTENT, &MailContent, sizeof(MailContent));
}

void MailLayer::tableCellHighlight(CCTableView* table,CCTableViewCell* cell)//按下去的时候就是高亮显示，这里可以设置高
{
    
}

void MailLayer::tableCellUnhighlight(CCTableView* table,CCTableViewCell* cell)//松开的时候，取消高亮状态
{
    
}

//CCTableViewDataSource
unsigned int MailLayer::numberOfCellsInTableView(CCTableView* table)//返回这个表格有多少单元格
{
    unsigned int count = (unsigned int)CMainLogic::sharedMainLogic()->m_vctMail.size();
    return count;
}

CCSize MailLayer::cellSizeForTable(CCTableView* table)//返回每个Cell的大小
{
    return CCSizeMake(850, 110);
}

CCTableViewCell* MailLayer::tableCellAtIndex(CCTableView* table,unsigned int idx)//生成单元格，即单元格的内容
{
    CCTableViewCell* cell = table -> dequeueCell();
    if(!cell)
    {
        cell = new CCTableViewCell();
        cell -> autorelease();
    }
    cell -> removeAllChildrenWithCleanup(true);
    
    CCSprite* pSpCellBg = CCSprite::create("mail/mailcell.png");
    pSpCellBg -> setPosition(ccp(425, 55));
    cell -> addChild(pSpCellBg);
    pSpCellBg -> setTag(100);
    
    unsigned int count = (unsigned int)CMainLogic::sharedMainLogic()->m_vctMail.size();
    if(idx <= count-1)
    {
        char temp[256];
        if(CMainLogic::sharedMainLogic()->m_vctMail[idx].cbGeted == 3)
        {
            sprintf(temp, "%s", CMainLogic::sharedMainLogic()->m_vctMail[idx].szMailTitle.c_str());
            CCSprite* pSpMail1 = CCSprite::create("mail/mailsp1.png");
            pSpMail1 -> setPosition(ccp(100, pSpCellBg->getContentSize().height/2));
            pSpCellBg -> addChild(pSpMail1);
        }
        else
        {
            sprintf(temp, "%s", CMainLogic::sharedMainLogic()->m_vctMail[idx].szMailTitle.c_str());
            CCSprite* pSpMail2 = CCSprite::create("mail/mailsp2.png");
            pSpMail2 -> setPosition(ccp(100, pSpCellBg->getContentSize().height/2));
            pSpCellBg -> addChild(pSpMail2);
        }
        CCLabelTTF* pLabelTitle = CCLabelTTF::create(temp, "Arial", 30);
        pLabelTitle -> setAnchorPoint(ccp(0,0.5f));
        pLabelTitle -> setColor(ccc3(126, 64, 30));
        pLabelTitle -> setPosition(ccp(190, pSpCellBg->getContentSize().height/2));
        pSpCellBg -> addChild(pLabelTitle);
        
        int year, month, day, hour, minute,second;
        sscanf(CMainLogic::sharedMainLogic()->m_vctMail[idx].szMailTime.c_str(),"%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
        
        char temp2[16];
        sprintf(temp2, "%d/%d %d:%02d", month, day, hour, minute);
        CCLabelTTF* pLabelTime2 = CCLabelTTF::create(temp2, "Arial", 22);
        pLabelTime2 -> setPosition(ccp(760, pSpCellBg->getContentSize().height/2-30));
        pSpCellBg -> addChild(pLabelTime2);
        pLabelTime2 -> setColor(ccc3(111, 109, 101));
        CCSprite* pSpRead = CCSprite::create("mail/mailread.png");
        pSpRead -> setPosition(ccp(760, pSpCellBg->getContentSize().height/2+13));
        pSpCellBg -> addChild(pSpRead);
        pSpRead -> setTag(100);
        CCSprite* pSpPrise = CCSprite::create("mail/mailprise.png");
        pSpPrise -> setPosition(ccp(760, pSpCellBg->getContentSize().height/2+13));
        pSpCellBg -> addChild(pSpPrise);
        pSpPrise -> setTag(104);
        pSpPrise -> setVisible(false);
        
        if(!CMainLogic::sharedMainLogic()->m_vctMail[idx].cbMailStatus)//未读
        {
            pSpRead -> setVisible(false);
            pSpPrise -> setVisible(true);
            if(CMainLogic::sharedMainLogic()->m_vctMail[idx].cbGeted == 1)
            {
                pSpPrise -> setVisible(true);
            }
        }
        else if(CMainLogic::sharedMainLogic()->m_vctMail[idx].cbMailStatus && CMainLogic::sharedMainLogic()->m_vctMail[idx].cbGeted == 1)
        {
            pSpRead -> setVisible(false);
            pSpPrise -> setVisible(true);
        }
        else
        {
            //pSpCellyin -> setVisible(true);
        }
    }
    
    return cell;
}

void MailLayer::closeContent(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    m_nodeContain -> setVisible(false);
    m_tableView -> setTouchEnabled(true);
    m_tableView -> setVisible(true);
    //刷新单元格
    //m_tableView->reloadData();
}

void MailLayer::showMailContain(int nMailID, bool bShowAction)
{
    if(bShowAction)
    {
        m_nodeMail->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
    }
    m_tableView -> setTouchEnabled(false);
    //m_tableView -> setVisible(false);
    m_nodeContain -> setVisible(true);
    
    for(unsigned int i = 0; i < CMainLogic::sharedMainLogic()->m_vctMail.size(); i++)
    {
        if(CMainLogic::sharedMainLogic()->m_vctMail[i].dwMailID == nMailID)
        {
            m_nCurrentMailID = nMailID;
            if(CMainLogic::sharedMainLogic()->m_vctMail[i].cbGeted == 0)
            {
                m_menuGet -> setVisible(false);
                m_spGet -> setVisible(false);
            }
            else if(CMainLogic::sharedMainLogic()->m_vctMail[i].cbGeted == 1)
            {
                m_menuGet -> setVisible(true);
                m_spGet -> setVisible(false);
            }
            else if(CMainLogic::sharedMainLogic()->m_vctMail[i].cbGeted == 2)
            {
                m_spGet -> setVisible(true);
                m_menuGet -> setVisible(false);
            }
            else if(CMainLogic::sharedMainLogic()->m_vctMail[i].cbGeted == 3)
            {
                m_menuGet -> setVisible(false);
                m_spGet -> setVisible(false);
            }
            m_sDuihuanma = CMainLogic::sharedMainLogic()->m_vctMail[i].szDuihuanma;
            m_sGongzhonghao = CMainLogic::sharedMainLogic()->m_vctMail[i].szGongzhonghao;
            char temp[1024];
            sprintf(temp, "       %s", CMainLogic::sharedMainLogic()->m_vctMail[i].szMailContent.c_str());
            CCLabelTTF * ttf = CCLabelTTF::create(temp, "Arial", 36);
            CCLabelTTF * pContentTTF = NULL;
            float maxLen = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            maxLen = 2800;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            maxLen = 2331;
#endif
            if (ttf->getContentSize().height > 200 || ttf->getContentSize().width > maxLen)
            {
                if (CMainLogic::sharedMainLogic()->m_vctMail[i].cbGeted == 1 || CMainLogic::sharedMainLogic()->m_vctMail[i].cbGeted == 2) {
                    CCScrollView* pMailScrollView = CCScrollView::create(CCSizeMake(780, 216));
                    pMailScrollView->setPosition(ccp(780/2 - 400, 340/2 - 78));
                    pMailScrollView->setTouchPriority(-200);
                    m_pSpDi->addChild(pMailScrollView, 1, 1000);
                    CCLayerColor* pContainlayer = CCLayerColor::create(ccc4(0, 0, 0, 0),780,ttf->getContentSize().height);
                    pMailScrollView->setContainer(pContainlayer);
                    pMailScrollView->setDirection(kCCScrollViewDirectionVertical);//设置滑动方向
                    pMailScrollView->setBounceable(false);
                    pMailScrollView->setDelegate(this);
                    
                    pContentTTF = CCLabelTTF::create(temp, "Arial", 36,CCSizeMake(780, ttf->getContentSize().height),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
                    pContentTTF->setPosition(ccp(390, ttf->getContentSize().height/2-45));
                    pContentTTF->setColor(ccc3(255, 255, 255));
                    pContainlayer->addChild(pContentTTF, 3);
                    pContainlayer->setPosition(ccp(0, 216-ttf->getContentSize().height));
                }
                else if (CMainLogic::sharedMainLogic()->m_vctMail[i].cbGeted == 0 || CMainLogic::sharedMainLogic()->m_vctMail[i].cbGeted == 3)
                {
                    CCScrollView* pMailScrollView = CCScrollView::create(CCSizeMake(780, 400));
                    pMailScrollView->setPosition(ccp(780/2 - 400, 340/2 - 78 - 184));
                    pMailScrollView->setTouchPriority(-200);
                    m_pSpDi->addChild(pMailScrollView, 1, 1000);
                    CCLayerColor* pContainlayer = CCLayerColor::create(ccc4(0, 0, 0, 0),780,500);
                    pMailScrollView->setContainer(pContainlayer);
                    pMailScrollView->setDirection(kCCScrollViewDirectionVertical);//设置滑动方向
                    pMailScrollView->setBounceable(false);
                    pMailScrollView->setDelegate(this);
                    
                    pContentTTF = CCLabelTTF::create(temp, "Arial", 36,CCSizeMake(780, 500),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
                    pContentTTF->setPosition(ccp(390, (500/2)));
                    pContentTTF->setColor(ccc3(255, 255, 255));
                    pContainlayer->addChild(pContentTTF, 3);
                    pContainlayer->setPosition(ccp(0, 400-500));
                }
            }
            else
            {
                CCScrollView* pMailScrollView = CCScrollView::create(CCSizeMake(780, 216));
                pMailScrollView->setPosition(ccp(780/2 - 400, 340/2 - 78));
                pMailScrollView->setTouchPriority(-200);
                m_pSpDi->addChild(pMailScrollView, 1, 1000);
                CCLayerColor* pContainlayer = CCLayerColor::create(ccc4(0, 0, 0, 0),780,216);
                pMailScrollView->setContainer(pContainlayer);
                pMailScrollView->setDirection(kCCScrollViewDirectionVertical);//设置滑动方向
                pMailScrollView->setBounceable(false);
                pMailScrollView->setDelegate(this);
                
                pContentTTF = CCLabelTTF::create(temp, "Arial", 36,CCSizeMake(780, 216),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
                pContentTTF->setPosition(ccp(390, 108));
                pContentTTF->setColor(ccc3(255, 255, 255));
                pContainlayer->addChild(pContentTTF, 3);
                pContainlayer->setPosition(ccp(0, 0));
            }
            
            if(CMainLogic::sharedMainLogic()->m_vctMail[i].szGongzhonghao != "")
            {
                CCMenuItemImage* pItem = CCMenuItemImage::create("mail/mailgongItem.png", "mail/mailgongItem.png", this, menu_selector(MailLayer::clickCopy));
                pItem -> setPosition(ccp(680, 15));
                pItem -> setTag(101);
                CCMenu* pMenu = CCMenu::create(pItem, NULL);
                pMenu -> setPosition(CCPointZero);
                pMenu -> setAnchorPoint(CCPointZero);
                pMenu -> setTouchPriority(-200);
                m_pSpDi -> addChild(pMenu, 2);
            }
            if(CMainLogic::sharedMainLogic()->m_vctMail[i].szDuihuanma != "")
            {
                CCMenuItemImage* pItem = CCMenuItemImage::create("mail/mailduihuanItem.png", "mail/mailduihuanItem.png", this, menu_selector(MailLayer::clickCopy));
                pItem -> setPosition(ccp(480, 15));
                pItem -> setTag(102);
                CCMenu* pMenu = CCMenu::create(pItem, NULL);
                pMenu -> setPosition(CCPointZero);
                pMenu -> setAnchorPoint(CCPointZero);
                pMenu -> setTouchPriority(-200);
                m_pSpDi -> addChild(pMenu, 2);
            }
        }
    }
}

void MailLayer::clickCopy(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    if(item->getTag() == 101)//复制公众号
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChargeWebView::copy(m_sGongzhonghao);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo info;
        bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
        jobject jobj;
        if(bHave)
        {
            jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
            bool bHave2 = false;
            bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","copy", "(Ljava/lang/String;)V");
            if(bHave2)
            {
                jstring sGongZhonghao = info.env->NewStringUTF(m_sGongzhonghao.c_str());
                info.env->CallVoidMethod(jobj, info.methodID, sGongZhonghao);
            }
        }
#endif
        CMainLogic::sharedMainLogic()->ShowMessage("公众号复制成功！请在微信搜索公众号中粘贴搜索并关注乐乐捕鱼公众号！", eMBOK);
    }
    else if(item->getTag() == 102)//复制兑换码
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChargeWebView::copy(m_sDuihuanma);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo info;
        bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
        jobject jobj;
        if(bHave)
        {
            jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
            bool bHave2 = false;
            bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","copy", "(Ljava/lang/String;)V");
            if(bHave2)
            {
                jstring sDuihuanma = info.env->NewStringUTF(m_sDuihuanma.c_str());
                info.env->CallVoidMethod(jobj, info.methodID, sDuihuanma);
            }
        }
#endif
        CMainLogic::sharedMainLogic()->ShowMessage("验证码复制成功！请在乐乐捕鱼微信公众号下面粘贴发送此码！", eMBOK);
    }
}

void MailLayer::setMenuState()
{
    m_menuGet -> setVisible(false);
    m_spGet -> setVisible(true);
}

void MailLayer::setMailItem(char* itemStr, int count)
{
    for (int i=0; i<4; i++)
    {
        CCSprite* pItem = (CCSprite*)m_pSpDi->getChildByTag(1010+i);
        if (pItem == NULL)
        {
            CCSprite* pBg = CCSprite::create("h3all/daojukuang.png");
            pBg->setPosition(ccp(45+130*i, 17));
            m_pSpDi->addChild(pBg, 2, 1010+i);
            CCSprite* pBg2 = CCSprite::create("beibao/beibaokuangbg2.png");
            pBg2->setPosition(ccp(45+130*i, 17));
            m_pSpDi->addChild(pBg2, 1, 1020+i);
            
            CCSprite* pCannon = CCSprite::create(itemStr);
            pCannon -> setPosition(ccp(pBg->getContentSize().width/2, pBg->getContentSize().height/2));
            pBg -> addChild(pCannon, 1);

            if (count > 0)
            {
                char sNum[16];
                sprintf(sNum, "%d", count);
                CCLabelAtlas* countAtlas = CCLabelAtlas::create(sNum, "h3all/zhunzhunum.png", 14, 17, '0');
                countAtlas->setAnchorPoint(ccp(1, 0.5f));
                countAtlas->setPosition(ccp(pBg->getContentSize().width-8, 20));
                pBg->addChild(countAtlas, 1);
            }
            break;
        }
    }
}
