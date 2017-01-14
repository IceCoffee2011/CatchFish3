//
//  NoticeLayer.cpp
//  CatchFish
//
//  Created by xiaosong1531 on 15/12/9.
//
//

#include "header.h"

NoticeLayer::NoticeLayer()
{
    init();
}

bool NoticeLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_bShowCurrent = false;
    
    m_vctVIPNotice.clear();
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);
    //loadUI();
    this->setTouchEnabled(true);
    
    return true;
}

void NoticeLayer::loadUI()
{
    this->removeAllChildren();
    m_spNoticeBg = CCSprite::create("h2all/noticeBg.png");
    m_spNoticeBg->setPosition(ccp(CLIENT_WIDTH/2, NOTICE_HEIGHT+10));
    this->addChild(m_spNoticeBg, 1);
    m_labelNoticeStr = CCLabelTTF::create("","Arial",32);//创建通知
    m_labelNoticeStr -> setAnchorPoint(ccp(0, 0.5));
    m_labelNoticeStr -> setPosition(ccp(m_spNoticeBg->getPositionX()+m_spNoticeBg->getContentSize().width/2, NOTICE_HEIGHT+10));
    CCDrawNode* pNoticeZone = CCDrawNode::create();//设置滚动通知的显示区域
    CCPoint point[4] = {ccp(CLIENT_WIDTH/2-m_spNoticeBg->getContentSize().width/2+26+30,NOTICE_HEIGHT-26+10),ccp(CLIENT_WIDTH/2+m_spNoticeBg->getContentSize().width/2-15,NOTICE_HEIGHT-26+10), ccp(CLIENT_WIDTH/2+m_spNoticeBg->getContentSize().width/2-15, NOTICE_HEIGHT+26+10),ccp(CLIENT_WIDTH/2-m_spNoticeBg->getContentSize().width/2+26+30,NOTICE_HEIGHT+26+10)};
    pNoticeZone->drawPolygon(point,4,ccc4f(255,255,255,255),2,ccc4f(255,255,255,255));
    CCClippingNode* pCliper = CCClippingNode::create();
    pCliper->setStencil(pNoticeZone);
    pCliper->setAnchorPoint(CCPointZero);
    pCliper->addChild(m_labelNoticeStr);
    this->addChild(pCliper, 1);
}

void NoticeLayer::showNotice()
{
    std::string str = "";
    float fWidth=0;
    if(CMainLogic::sharedMainLogic()->m_vctVIPNotice.size() != 0)
    {
        str = CMainLogic::sharedMainLogic()->m_vctVIPNotice[0].sNotice;
        CMainLogic::sharedMainLogic()->m_vctVIPNotice[0].bHave = false;
        m_bShowCurrent = true;
    }
    else
    {
        //隐藏自己
        this->setVisible(false);
        m_bShowCurrent = false;
        return;
    }
    int size = (int)str.find("#");
    
    if(size == -1)//说明整个字符串都不包含#
    {
        m_labelNoticeStr->setString(str.c_str());
        float fWidth = m_labelNoticeStr->getContentSize().width;
        CCMoveTo* pMoveTo0 = CCMoveTo::create((fWidth+m_spNoticeBg->getContentSize().width-71)/CMainLogic::sharedMainLogic()->m_fNoticeSpeed, ccp(m_spNoticeBg->getPositionX()-m_spNoticeBg->getContentSize().width/2-fWidth, NOTICE_HEIGHT+10));
        CCMoveTo* pMoveTo1 = CCMoveTo::create(0, ccp(m_spNoticeBg->getPositionX()+m_spNoticeBg->getContentSize().width/2, NOTICE_HEIGHT+10));
        CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(NoticeLayer::resumeNotice));
        CCSequence* pSeq = CCSequence::create(pMoveTo0, pMoveTo1, func2, NULL);
        m_labelNoticeStr->runAction(pSeq);
    }
    else if(size != 0)//说明第一个字符不是#
    {
        VIPNotice VipNotice0;
        VipNotice0.sColor = "";
        VipNotice0.sNotice = "";
        VipNotice0.sTag = "";
        std::string str0 = "";
        str0 = str.substr(0, size);
        std::string strColor0 = str0.substr(1, 6);
        VipNotice0.sColor = "FFFFFF";//默认白色
        VipNotice0.sNotice = str0;
        m_vctVIPNotice.push_back(VipNotice0);
        str = str.substr(size);
        size = (int)str.find("#");
        std::string str1 = "";
        std::string str2 = "";
        while (size != -1)
        {
            if(size == 0)
            {
                VIPNotice VipNotice;
                VipNotice.sColor = "";
                VipNotice.sNotice = "";
                VipNotice.sTag = "";
                //判断是否为L
                if (str.substr(1, 1) == "L")
                {
                    VipNotice.sTag = str.substr(2, 2);
                    VipNotice.sColor = "7CBC3D";
                    str = str.substr(4);
                    size = (int)str.find("#");
                }
                else
                {
                    str2 = str.substr(1, 6);
                    if (str2.length() == 6)//有颜色
                    {
                        VipNotice.sColor = str2;
                    }
                    else   //默认颜色
                    {
                        VipNotice.sColor = "FFFF00";
                    }
                    
                    str = str.substr(7);
                    size = (int)str.find("#");
                }
                
                if(size == -1)
                {
                    VipNotice.sNotice = str;
                }
                else if(size != 0)
                {
                    str1 = str.substr(0, size);
                    VipNotice.sNotice = str1;
                    str = str.substr(size);
                    size = (int)str.find("#");
                }
                m_vctVIPNotice.push_back(VipNotice);
            }
        }
        m_labelNoticeStr->setString(m_vctVIPNotice[0].sNotice.c_str());
        int r, g, b = 0;
        std::string s1, s2, s3 = "";
        s1 = m_vctVIPNotice[0].sColor.substr(0, 2);
        s2 = m_vctVIPNotice[0].sColor.substr(2, 2);
        s3 = m_vctVIPNotice[0].sColor.substr(4, 2);
        sscanf(s1.c_str(), "%x", &r);
        sscanf(s2.c_str(), "%x", &g);
        sscanf(s3.c_str(), "%x", &b);
        m_labelNoticeStr->setColor(ccc3(r, g, b));
        fWidth = m_labelNoticeStr->getContentSize().width;
        for(int i = 1; i < m_vctVIPNotice.size(); i++)
        {
            std::string sColor = m_vctVIPNotice[i].sColor;
            std::string sNotice = m_vctVIPNotice[i].sNotice;
            //获取颜色
            int r, g, b = 0;
            std::string str1, str2, str3 = "";
            str1 = sColor.substr(0,2);
            str2 = sColor.substr(2, 2);
            str3 = sColor.substr(4, 2);
            sscanf(str1.c_str(), "%x", &r);
            sscanf(str2.c_str(), "%x", &g);
            sscanf(str3.c_str(), "%x", &b);
            
            CCLabelTTF* pLabel = CCLabelTTF::create(sNotice.c_str(), "Arial", 28);
            pLabel->setColor(ccc3(r, g, b));
            pLabel -> setAnchorPoint(ccp(0, 0.5f));
            pLabel -> setPosition(ccp(fWidth, m_labelNoticeStr->getContentSize().height/2));
            pLabel -> setTag(100+i);
            
            m_labelNoticeStr -> addChild(pLabel);
            fWidth+=pLabel->getContentSize().width;
            if ("" != m_vctVIPNotice[i].sTag)//L
            {
                CCSprite * pLinkline = CCSprite::create("h3all/linkline.png");
                pLinkline->setScaleX(pLabel->getContentSize().width/pLinkline->getContentSize().width);
                pLinkline->setAnchorPoint(ccp(0, 0));
                pLinkline->setPosition(ccp(0, -2));
                pLabel->addChild(pLinkline);
            }
        }
        CCMoveTo* pMoveTo0 = CCMoveTo::create((fWidth+m_spNoticeBg->getContentSize().width-71)/CMainLogic::sharedMainLogic()->m_fNoticeSpeed, ccp(m_spNoticeBg->getPositionX()-m_spNoticeBg->getContentSize().width/2-fWidth, NOTICE_HEIGHT+10));
        CCMoveTo* pMoveTo1 = CCMoveTo::create(0, ccp(m_spNoticeBg->getPositionX()+m_spNoticeBg->getContentSize().width/2, NOTICE_HEIGHT+10));
        CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(NoticeLayer::resumeNotice));
        CCSequence* pSeq = CCSequence::create(pMoveTo0, pMoveTo1, func2, NULL);
        m_labelNoticeStr->runAction(pSeq);
    }
    else//说明第一个字符是#
    {
        std::string str1 = "";
        std::string str2 = "";
        while (size != -1)
        {
            if(size == 0)
            {
                VIPNotice VipNotice;
                VipNotice.sColor = "";
                VipNotice.sNotice = "";
                VipNotice.sTag = "";
                //判断是否为L
                if (str.substr(1, 1) == "L")
                {
                    VipNotice.sTag = str.substr(2, 2);
                    VipNotice.sColor = "7CBC3D";
                    str = str.substr(4);
                    size = (int)str.find("#");
                }
                else
                {
                    str2 = str.substr(1, 6);
                    if (str2.length() == 6)//有颜色
                    {
                        VipNotice.sColor = str2;
                    }
                    else   //默认颜色
                    {
                        VipNotice.sColor = "FFFFFF";
                    }
                    
                    str = str.substr(7);
                    size = (int)str.find("#");
                }
                
                if(size == -1)
                {
                    VipNotice.sNotice = str;
                }
                else if(size != 0)
                {
                    str1 = str.substr(0, size);
                    VipNotice.sNotice = str1;
                    str = str.substr(size);
                    size = (int)str.find("#");
                }
                m_vctVIPNotice.push_back(VipNotice);
            }
        }
        m_labelNoticeStr->setString(m_vctVIPNotice[0].sNotice.c_str());
        int r, g, b = 0;
        std::string s1, s2, s3 = "";
        s1 = m_vctVIPNotice[0].sColor.substr(0, 2);
        s2 = m_vctVIPNotice[0].sColor.substr(2, 2);
        s3 = m_vctVIPNotice[0].sColor.substr(4, 2);
        sscanf(s1.c_str(), "%x", &r);
        sscanf(s2.c_str(), "%x", &g);
        sscanf(s3.c_str(), "%x", &b);
        m_labelNoticeStr->setColor(ccc3(r, g, b));
        fWidth = m_labelNoticeStr->getContentSize().width;
        for(int i = 1; i < m_vctVIPNotice.size(); i++)
        {
            std::string sColor = m_vctVIPNotice[i].sColor;
            std::string sNotice = m_vctVIPNotice[i].sNotice;
            //获取颜色
            int r, g, b = 0;
            std::string str1, str2, str3 = "";
            str1 = sColor.substr(0,2);
            str2 = sColor.substr(2, 2);
            str3 = sColor.substr(4, 2);
            sscanf(str1.c_str(), "%x", &r);
            sscanf(str2.c_str(), "%x", &g);
            sscanf(str3.c_str(), "%x", &b);
            
            CCLabelTTF* pLabel = CCLabelTTF::create(sNotice.c_str(), "Arial", 28);
            pLabel->setColor(ccc3(r, g, b));
            pLabel -> setAnchorPoint(ccp(0, 0.5f));
            pLabel -> setPosition(ccp(fWidth, m_labelNoticeStr->getContentSize().height/2));
            pLabel -> setTag(100+i);
            m_labelNoticeStr -> addChild(pLabel);
            fWidth+=pLabel->getContentSize().width;
            if ("" != m_vctVIPNotice[i].sTag)//L
            {
                CCSprite * pLinkline = CCSprite::create("h3all/linkline.png");
                pLinkline->setScaleX(pLabel->getContentSize().width/pLinkline->getContentSize().width);
                pLinkline->setAnchorPoint(ccp(0, 0));
                pLinkline->setPosition(ccp(0, -2));
                pLabel->addChild(pLinkline);
            }
        }
        CCMoveTo* pMoveTo0 = CCMoveTo::create((fWidth+m_spNoticeBg->getContentSize().width-71)/CMainLogic::sharedMainLogic()->m_fNoticeSpeed, ccp(m_spNoticeBg->getPositionX()-m_spNoticeBg->getContentSize().width/2-fWidth, NOTICE_HEIGHT+10));
        CCMoveTo* pMoveTo1 = CCMoveTo::create(0, ccp(m_spNoticeBg->getPositionX()+m_spNoticeBg->getContentSize().width/2, NOTICE_HEIGHT+10));
        CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(NoticeLayer::resumeNotice));
        CCSequence* pSeq = CCSequence::create(pMoveTo0, pMoveTo1, func2, NULL);
        m_labelNoticeStr->runAction(pSeq);
    }
}

void NoticeLayer::removeNotice()
{
    
}

void NoticeLayer::resumeNotice()
{
    for(int i = 0; i < CMainLogic::sharedMainLogic()->m_vctVIPNotice.size(); i++)
    {
        if(CMainLogic::sharedMainLogic()->m_vctVIPNotice[i].bHave == false)
        {
            CMainLogic::sharedMainLogic()->m_vctVIPNotice.erase(CMainLogic::sharedMainLogic()->m_vctVIPNotice.begin()+i);
        }
    }
    m_labelNoticeStr->removeAllChildren();
    m_vctVIPNotice.clear();
    showNotice();
}

void NoticeLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);
}

bool NoticeLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint point = pTouch->getLocation();
    if(m_labelNoticeStr != NULL)
    {
        point = m_labelNoticeStr->convertToNodeSpace(point);
        for (int i = 1; i < m_vctVIPNotice.size(); i++)
        {
            CCLabelTTF* pLabel = (CCLabelTTF*)m_labelNoticeStr->getChildByTag(100+i);
            if(pLabel != NULL)
            {
                std::string sNotice = pLabel->getString();
                if(!sNotice.find("查看"))
                {
                    CCRect rect = pLabel->boundingBox();
                    if (rect.containsPoint(point))
                    {
                        for (int k = 0; k<sNotice.length(); k++)
                        {
                            char cbNum = sNotice[k];
                            if (cbNum >= '0' && cbNum <= '9')
                            {
                                int nNum = cbNum-'0';
                                CMainLogic::sharedMainLogic()->m_nGameToHallVipLevel = nNum;
                                if (GameMainScene::_instance != NULL)
                                {
                                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameNoticeVIP);
                                    GameMainScene::_instance->openVIP();
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}