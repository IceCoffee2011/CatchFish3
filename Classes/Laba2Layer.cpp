//
//  Laba2Layer.cpp
//  CatchFish
//
//  Created by xiaosong1531 on 15/12/9.
//
//

#include "header.h"

Laba2Layer::Laba2Layer(int nHeight)
{
    m_nHeight = nHeight;
    init();
}

bool Laba2Layer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_bShowCurrent = false;
    
    m_vctVIPNotice.clear();
    
    return true;
}

void Laba2Layer::loadUI()
{
    this->removeAllChildren();
    CCSprite* sprite2 = CCSprite::createWithSpriteFrameName("labakuang1.png");
    sprite2 -> setPosition(ccp(CLIENT_WIDTH/4*3-5, m_nHeight-6.5));
    this -> addChild(sprite2, 1);
    sprite2 -> setScale(2.0f);
    m_spNoticeBg = CCSprite::createWithSpriteFrameName("labakuang1.png");
    m_spNoticeBg -> setPosition(ccp(CLIENT_WIDTH/4+5, m_nHeight));
    this -> addChild(m_spNoticeBg, 2);
    m_spNoticeBg->setScale(2.0f);
    m_spNoticeBg->setFlipX(true);
    m_spNoticeBg->setFlipY(true);
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    char temp[24];
    CCArray* array = CCArray::create();
    for(int i = 1; i <= 14; i++)
    {
        sprintf(temp, "labakuang%d.png", i);
        CCSpriteFrame* frame = cache->spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, 0.07f);
    CCAnimate* animate = CCAnimate::create(animation);
    CCAnimation* animation2 = CCAnimation::createWithSpriteFrames(array, 0.07f);
    CCAnimate* animate2 = CCAnimate::create(animation2);
    m_spNoticeBg -> runAction(CCRepeatForever::create(animate2));
    sprite2 -> runAction(CCRepeatForever::create(animate));
    
    m_labelNoticeStr = CCLabelTTF::create("","Arial",32);//创建通知
    m_labelNoticeStr -> setAnchorPoint(ccp(0, 0.5));
    m_labelNoticeStr -> setPosition(ccp(1120, m_nHeight-3));
    CCDrawNode* pNoticeZone = CCDrawNode::create();//设置滚动通知的显示区域
    CCPoint point[4] = {ccp(180,m_nHeight-26+10),ccp(1120,m_nHeight-26+10), ccp(1120, m_nHeight+26+10),ccp(180,m_nHeight+26+10)};
    pNoticeZone->drawPolygon(point,4,ccc4f(255,0,0,255),2,ccc4f(255,0,0,255));
    CCClippingNode* pCliper = CCClippingNode::create();
    pCliper->setStencil(pNoticeZone);
    pCliper->setAnchorPoint(CCPointZero);
    pCliper->addChild(m_labelNoticeStr);
    this->addChild(pCliper, 3);
}

void Laba2Layer::showNotice()
{
    std::string str = "";
    float fWidth=0;
    if(CMainLogic::sharedMainLogic()->m_vctLaba2Notice.size() != 0)
    {
        str = CMainLogic::sharedMainLogic()->m_vctLaba2Notice[0].sNotice;
        CMainLogic::sharedMainLogic()->m_vctLaba2Notice[0].bHave = false;
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
        CCMoveTo* pMoveTo0 = CCMoveTo::create((fWidth+m_spNoticeBg->getContentSize().width*2-71)/CMainLogic::sharedMainLogic()->m_fNoticeSpeed, ccp(m_spNoticeBg->getPositionX()-m_spNoticeBg->getContentSize().width/2-fWidth, m_nHeight-3));
        CCMoveTo* pMoveTo1 = CCMoveTo::create(0, ccp(1120, m_nHeight-3));
        CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(Laba2Layer::resumeNotice));
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
        CCMoveTo* pMoveTo0 = CCMoveTo::create((fWidth+m_spNoticeBg->getContentSize().width*2-71)/CMainLogic::sharedMainLogic()->m_fNoticeSpeed, ccp(m_spNoticeBg->getPositionX()-m_spNoticeBg->getContentSize().width/2-fWidth, m_nHeight-3));
        CCMoveTo* pMoveTo1 = CCMoveTo::create(0, ccp(1120, m_nHeight-3));
        CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(Laba2Layer::resumeNotice));
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
        CCMoveTo* pMoveTo0 = CCMoveTo::create((fWidth+m_spNoticeBg->getContentSize().width*2-71)/CMainLogic::sharedMainLogic()->m_fNoticeSpeed, ccp(m_spNoticeBg->getPositionX()-m_spNoticeBg->getContentSize().width/2-fWidth, m_nHeight-3));
        CCMoveTo* pMoveTo1 = CCMoveTo::create(0, ccp(1120, m_nHeight-3));
        CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(Laba2Layer::resumeNotice));
        CCSequence* pSeq = CCSequence::create(pMoveTo0, pMoveTo1, func2, NULL);
        m_labelNoticeStr->runAction(pSeq);
    }
}

void Laba2Layer::resumeNotice()
{
    for(int i = 0; i < CMainLogic::sharedMainLogic()->m_vctLaba2Notice.size(); i++)
    {
        if(CMainLogic::sharedMainLogic()->m_vctLaba2Notice[i].bHave == false)
        {
            CMainLogic::sharedMainLogic()->m_vctLaba2Notice.erase(CMainLogic::sharedMainLogic()->m_vctLaba2Notice.begin()+i);
        }
    }
    m_labelNoticeStr->removeAllChildren();
    m_vctVIPNotice.clear();
    showNotice();
}