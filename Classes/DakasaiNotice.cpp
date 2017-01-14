#include "header.h"

#define LABABOTICE_HEIGHT 495

DakasaiNotice::DakasaiNotice(int nHeight)
{
    m_nHeight = nHeight;
    init();
}

DakasaiNotice::~DakasaiNotice()
{
    
}

bool DakasaiNotice::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_bShowCurrent = false;
    
    return true;
}

void DakasaiNotice::loadUI()
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
    CCSprite* pLogo = CCSprite::create("h3all/labalogo.png");
    pLogo -> setPosition(ccp(158-80, m_spNoticeBg->getContentSize().height/2-3));
    m_spNoticeBg -> addChild(pLogo);
    pLogo->setScale(0.35f);
    
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
    
    m_labelNoticeStr = CCLabelTTF::create("","Arial-BoldMT",32);//创建通知
    m_labelNoticeStr -> setAnchorPoint(ccp(0, 0.5));
    m_labelNoticeStr -> setPosition(ccp(175-80, m_spNoticeBg->getContentSize().height/2-3));
    m_spNoticeBg -> addChild(m_labelNoticeStr);
    m_labelNoticeStr -> setScale(0.5f);
    
}

void DakasaiNotice::showLabaNotice()
{
    std::string str = "";
    float fWidth=0;
    if(CMainLogic::sharedMainLogic()->m_vctDakasaiNotice.size() != 0 && CMainLogic::sharedMainLogic()->m_vctDakasaiNotice[0].bHave)
    {
        str = CMainLogic::sharedMainLogic()->m_vctDakasaiNotice[0].sNotice;
        CMainLogic::sharedMainLogic()->m_vctDakasaiNotice[0].bHave = false;
        m_bShowCurrent = true;
        for(int i = 0; i < CMainLogic::sharedMainLogic()->m_vctDakasaiNotice.size(); i++)
        {
            if(CMainLogic::sharedMainLogic()->m_vctDakasaiNotice[i].bHave == false)
            {
                CMainLogic::sharedMainLogic()->m_vctDakasaiNotice.erase(CMainLogic::sharedMainLogic()->m_vctDakasaiNotice.begin()+i);
            }
        }
        m_labelNoticeStr->removeAllChildren();
        m_vctVIPNotice.clear();
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
            
            CCLabelTTF* pLabel = CCLabelTTF::create(sNotice.c_str(), "Arial-BoldMT", 32);
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
            
            CCLabelTTF* pLabel = CCLabelTTF::create(sNotice.c_str(), "Arial-BoldMT", 32);
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
    }
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(DakasaiNotice::resumeNotice));
    m_labelNoticeStr -> runAction(CCSequence::create(CCDelayTime::create(4.0f), func, NULL));
}

void DakasaiNotice::resumeNotice()
{
    for(int i = 0; i < CMainLogic::sharedMainLogic()->m_vctDakasaiNotice.size(); i++)
    {
        if(CMainLogic::sharedMainLogic()->m_vctDakasaiNotice[i].bHave == false)
        {
            CMainLogic::sharedMainLogic()->m_vctDakasaiNotice.erase(CMainLogic::sharedMainLogic()->m_vctDakasaiNotice.begin()+i);
        }
    }
    m_labelNoticeStr->removeAllChildren();
    m_vctVIPNotice.clear();
    showLabaNotice();
}