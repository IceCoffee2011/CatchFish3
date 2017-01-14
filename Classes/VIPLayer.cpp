#include "header.h"

const char* sInfo4[1] = {"3.可以任意更换自定义头像"};

VIPLayer::~VIPLayer()
{
    for(int i = 0; i < m_vctVIPInfo.size(); i++)
    {
        delete m_vctVIPInfo[i];
    }
    m_vctVIPInfo.clear();
    m_arrVipInfo->release();
}

VIPLayer* VIPLayer::create(bool bLocalOpen, int nVipLevel)
{
    VIPLayer* pRet = new VIPLayer;
    if(pRet && pRet->init(bLocalOpen, nVipLevel))
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

bool VIPLayer::init(bool bLocalOpen, int nVipLevel)
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_bLocalOpen = bLocalOpen;
    m_bClickCharge = false;
    m_nUpgradeMoney = 0;
    m_bIsEnable = true;
    m_nVipLevel = CMainLogic::sharedMainLogic()->m_nVipLevel;
    if (nVipLevel > 0)
    {
        m_nCurrentVipLevel = nVipLevel;
    }
    else if(m_nVipLevel != 0)
    {
        m_nCurrentVipLevel = m_nVipLevel;
    }
    else
    {
        m_nCurrentVipLevel = nVipLevel;
    }
    
    m_nNoticeLevel = nVipLevel;
    m_arrVipInfo = CCArray::create();
    m_arrVipInfo->retain();
    m_vctVIPInfo.clear();
    
    loadUI();
    
    if(bLocalOpen)//使用本地文件创建VIP信息
    {
        //从本地读取VIP配置文件进行创建
        readVipConfig();
    }
    
    this -> setTouchEnabled(true);
    return true;
}

void VIPLayer::loadUI()
{
    CCSprite* pSpVipBg = CCSprite::create("v2ip/vipbg.png");
    pSpVipBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(pSpVipBg);
    pSpVipBg -> setScale(2.0f);
    CCLabelTTF* pLabel = CCLabelTTF::create("VIP享受更多优惠活动，等级越高优惠越多", "Arial-BoldMT", 32);
    pLabel -> setPosition(ccp(10, 10));
    pLabel -> setAnchorPoint(ccp(0, 0));
    this -> addChild(pLabel);
    pLabel -> setColor(ccc3(255, 212, 32));

    CCSprite* pSpLogo2 = CCSprite::create("v2ip/viplogo2.png");
    pSpLogo2 -> setPosition(ccp(CLIENT_WIDTH/2-30, CLIENT_HEIGHT-pSpLogo2->getContentSize().height-5));
    this->addChild(pSpLogo2);
    char temp[16];
    sprintf(temp, "%d", m_nVipLevel);
    CCLabelAtlas* pLabelLogo2 = CCLabelAtlas::create(temp, "v2ip/vipnum3.png", 29, 36, '0');
    pLabelLogo2 -> setAnchorPoint(ccp(0.5f, 0.5f));
    pLabelLogo2 -> setPosition(ccp(pSpLogo2->getContentSize().width+10+20, pSpLogo2->getContentSize().height/2));
    pSpLogo2 -> addChild(pLabelLogo2);
    
    //top vip info
    CCSprite* pVipLogo = CCSprite::create("v2ip/viplogo.png");
    pVipLogo -> setPosition(ccp(CLIENT_WIDTH/3-25, CLIENT_HEIGHT-80));
    this -> addChild(pVipLogo);
    CCLabelAtlas* pVipLevel = CCLabelAtlas::create(temp, "v2ip/vipnum1.png", 30, 36, '0');
    pVipLevel -> setAnchorPoint(ccp(0, 0.5f));
    pVipLevel -> setPosition(ccp(pVipLogo->getContentSize().width+20, pVipLogo->getContentSize().height/2));
    pVipLogo -> addChild(pVipLevel);
    if(m_nVipLevel < 9)
    {
        CCSprite* pVipLogoNext = CCSprite::create("v2ip/viplogo.png");
        pVipLogoNext -> setPosition(ccp(CLIENT_WIDTH/3*2-20, CLIENT_HEIGHT-80));
        this -> addChild(pVipLogoNext);
        sprintf(temp, "%d", m_nVipLevel+1);
        CCLabelAtlas* pVipLevelNext = CCLabelAtlas::create(temp, "v2ip/vipnum1.png", 30, 36, '0');
        pVipLevelNext -> setAnchorPoint(ccp(0, 0.5f));
        pVipLevelNext -> setPosition(ccp(pVipLogoNext->getContentSize().width+20, pVipLogoNext->getContentSize().height/2));
        pVipLogoNext -> addChild(pVipLevelNext);
    }
    
    
    //vip slider
    //经验值进度条
    int nowExp = 50;
    int allExp = 100;
    float fPercent = (float)nowExp/allExp;
    CCControlSlider* m_levelSlider = CCControlSlider::create(CCSprite::create("v2ip/slider1.png"), CCSprite::create("v2ip/slider2.png"), CCSprite::create());
    m_levelSlider->setValue(fPercent);
    m_levelSlider->setTouchEnabled(false);
    m_levelSlider->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT-80));
    this->addChild(m_levelSlider, 2);
    m_LessTTF = CCLabelTTF::create("", "Arial", 26);
    m_LessTTF->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT-120));
    this->addChild(m_LessTTF, 2);
    m_LessTTF -> setColor(ccc3(242, 230, 34));
    if(m_nVipLevel == CMainLogic::sharedMainLogic()->m_vctVipExpConfig.size())
    {
        m_levelSlider->setValue(1.0f);
        char temp[64];
        sprintf(temp, "恭喜您升级到VIP%d", m_nVipLevel);
        m_LessTTF->setString(temp);
        m_bClickCharge = false;
        m_nUpgradeMoney = 0;
    }
    else
    {
        if(m_nVipLevel < CMainLogic::sharedMainLogic()->m_vctVipExpConfig.size())
        {
            int nNeedExp = CMainLogic::sharedMainLogic()->m_vctVipExpConfig[m_nVipLevel].nChargeCount;//下一级经验
            int nCurrentExp = CMainLogic::sharedMainLogic()->m_nVipExp;//现有经验
            m_levelSlider->setValue((float)nCurrentExp/nNeedExp);
            int nPrise = (int)(CMainLogic::sharedMainLogic()->m_vctVipExpConfig[m_nVipLevel].nChargeCount-CMainLogic::sharedMainLogic()->m_nVipExp)/CMainLogic::sharedMainLogic()->m_nExpPercent;
            if(CMainLogic::sharedMainLogic()->m_nExpPercent != 0)
            {
                char temp[64];
                sprintf(temp, "再充值%d元,便可升级到为VIP%d", nPrise, m_nVipLevel+1);
                m_LessTTF->setString(temp);
                m_bClickCharge = true;
                m_nUpgradeMoney = nPrise;
            }
            if(m_nUpgradeMoney>1000)
            {
                m_bClickCharge = false;
                m_nUpgradeMoney = 0;
            }
        }
    }
    
    //top ui
    CCSprite* pSpScoreBg = CCSprite::create("v2ip/vipkuang.png");
    pSpScoreBg -> setPosition(ccp(CLIENT_WIDTH/3*2+270, 30));//CLIENT_WIDTH/2+170, 30
    this -> addChild(pSpScoreBg);
    CCSprite* pSpScoreLogo = CCSprite::create("v2ip/vipcoin.png");
    pSpScoreLogo -> setPosition(ccp(0, pSpScoreBg->getContentSize().height/2));
    pSpScoreBg -> addChild(pSpScoreLogo);
    if(GameMainScene::_instance != NULL)
    {
        sprintf(temp, "%d", GameMainScene::_instance->getUserCurrentScore());
    }
    else
    {
        sprintf(temp, "%lld", CMainLogic::sharedMainLogic()->m_lUserScore);
    }
    m_labelScore = CCLabelAtlas::create(temp,"CatchFish2001/catchfishnum.png",14,19,'0');
    m_labelScore -> setAnchorPoint(ccp(0, 0.5f));
    m_labelScore->setScale(1.3f);
    m_labelScore -> setPosition(ccp(30, pSpScoreBg->getContentSize().height/2));
    pSpScoreBg -> addChild(m_labelScore);
    
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        pSpScoreBg->setPosition(ccp(CLIENT_WIDTH/3*2+270, 30));
    }
    
    //left right close menu
    CCMenuItemImage* leftItem = CCMenuItemImage::create("v2ip/vipnextitem.png", "v2ip/vipnextitem.png", this, menu_selector(VIPLayer::clickMenu));
    leftItem -> setTag(101);
    leftItem -> setPosition(ccp(40, CLIENT_HEIGHT/2));
    CCMenuItemImage* rightItem = CCMenuItemImage::create("v2ip/vipnextitem.png", "v2ip/vipnextitem.png", this, menu_selector(VIPLayer::clickMenu));
    rightItem -> setTag(102);
    rightItem -> setPosition(ccp(CLIENT_WIDTH-40, CLIENT_HEIGHT/2));
    rightItem -> setRotation(180);
    CCMenuItemImage* closeItem = CCMenuItemImage::create("v2ip/vipcloseitem.png", "v2ip/vipcloseitem.png", this, menu_selector(VIPLayer::closeMenu));
    closeItem -> setPosition(ccp(CLIENT_WIDTH-closeItem->getContentSize().width/2-10, CLIENT_HEIGHT-closeItem->getContentSize().height/2-10));
    CCMenuItemImage* pShengItem = CCMenuItemImage::create("v2ip/vipshengjiitem0.png", "v2ip/vipshengjiitem0.png", this, menu_selector(VIPLayer::clickMenu));
    pShengItem -> setTag(104);
    pShengItem -> setPosition(ccp(CLIENT_WIDTH/3*2+170, CLIENT_HEIGHT-80));
    m_VIPMenu = CCMenu::create(closeItem, rightItem, leftItem, pShengItem, NULL);
    m_VIPMenu -> setPosition(CCPointZero);
    m_VIPMenu -> setAnchorPoint(CCPointZero);
    this -> addChild(m_VIPMenu, 10);
    m_VIPMenu -> setTouchPriority(-130);
    if(CMainLogic::sharedMainLogic()->m_nVipLevel==9)
    {
        pShengItem -> setVisible(false);
    }
}

void VIPLayer::readVipConfig()
{
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + std::string("VipNewConfig.xml");
    TiXmlDocument* document = new TiXmlDocument(xmlPath.c_str());
    if(document != NULL)
    {
        document -> LoadFile();
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("vipConfigInfos"))
        {
            delete document;
            return;
        }
        TiXmlElement * pCurrentElement = rootElement->FirstChildElement();
        while (NULL != pCurrentElement)
        {
            const char * pValue=pCurrentElement->Value();
            if (!strcmp(pValue,"vipConfigInfo"))
            {
                Tag_VIP2Info* pVIPInfo = new Tag_VIP2Info;
                m_vctVIPInfo.push_back(pVIPInfo);
                TiXmlElement* firstElement = pCurrentElement -> FirstChildElement();
                const char * valueId=firstElement->Value();
                if (!strcmp(valueId,"level"))
                {
                    int nLevel=atoi(firstElement->GetText());
                    pVIPInfo->nLevel = nLevel;
                }
                TiXmlElement* secondElement = firstElement -> NextSiblingElement();
                const char * nickname=secondElement->Value();
                if (!strcmp(nickname,"gold"))
                {
                    pVIPInfo->sReward = secondElement->GetText();
                }
                TiXmlElement* ThirdElement = secondElement -> NextSiblingElement();
                const char * score=ThirdElement->Value();
                if (!strcmp(score,"chargeBackPercent"))
                {
                    float score=atof(ThirdElement->GetText());
                    pVIPInfo->fChargeBackPercent = score;
                }
                TiXmlElement* FourElement = ThirdElement -> NextSiblingElement();
                const char * gailv=FourElement->Value();
                if (!strcmp(gailv,"buyugailv"))
                {
                    pVIPInfo->sHitLv = FourElement->GetText();
                }
            }
            pCurrentElement=pCurrentElement->NextSiblingElement();
        }
    }
    document->Clear();
    delete document;
    
    loadVIPInfoUI((int)m_vctVIPInfo.size());
}

void VIPLayer::loadVIPInfoUI(int nLevelTotal)
{
    //根据当前VIP总等级来创建多少个CCSprite，并加入到数组中
    for(unsigned int i = 0; i < m_vctVIPInfo.size(); i++)
    {
//        if(m_vctVIPInfo[i]->nLevel < CMainLogic::sharedMainLogic()->m_nVipLevel)
//            continue;
        CCSprite* pVipInfoBg = CCSprite::create();
        pVipInfoBg -> setPosition(ccp(0, 0));
        this -> addChild(pVipInfoBg);
        m_arrVipInfo -> addObject(pVipInfoBg);
        pVipInfoBg -> setTag(m_vctVIPInfo[i]->nLevel);
        pVipInfoBg -> setVisible(false);
        
        CCSprite* pTeQuan2 = CCSprite::create("v2ip/viptequanbg1.png");
        pTeQuan2 -> setPosition(ccp(400, 500));
        pVipInfoBg -> addChild(pTeQuan2);
        char temp[32];
        sprintf(temp, "%d", m_vctVIPInfo[i]->nLevel);
        CCLabelAtlas* pLabelTeQuan = CCLabelAtlas::create(temp, "v2ip/vipnum1.png", 30, 36, '0');
        pLabelTeQuan -> setAnchorPoint(ccp(0.5f, 0.5f));
        pLabelTeQuan -> setPosition(ccp(pTeQuan2->getContentSize().width/2, 30));
        pTeQuan2 -> addChild(pLabelTeQuan);
        CCSprite* pTeQuan1 = CCSprite::create("v2ip/viplogo.png");
        pTeQuan1 -> setAnchorPoint(ccp(1, 0.5f));
        pTeQuan1 -> setPosition(ccp(-5, pLabelTeQuan->getContentSize().height/2));
        pLabelTeQuan -> addChild(pTeQuan1);
        
        CCSprite* pTeQuan3 = CCSprite::create("v2ip/viptequan.png");
        pTeQuan3 -> setAnchorPoint(ccp(0, 0.5f));
        pTeQuan3 -> setPosition(ccp(pLabelTeQuan->getContentSize().width+5, pLabelTeQuan->getContentSize().height/2));
        pLabelTeQuan -> addChild(pTeQuan3);
        
        
        CCScrollView * pScrollView = CCScrollView::create(CCSizeMake(460, 370));
        CCLayer * pContainLayer = CCLayer::create();
        pContainLayer->setContentSize(CCSizeMake(460, 370));
        pScrollView->setContainer(pContainLayer);
        pScrollView->setContentOffset(ccp(0, 0));
        pScrollView->setPosition(ccp(190, 90));
        pScrollView->setTouchPriority(-130);
        pScrollView->setDirection(kCCScrollViewDirectionVertical);//设置滑动方向
        pScrollView->setBounceable(false);
        pVipInfoBg->addChild(pScrollView);
        
//        CCSprite* pVipInfo1 = CCSprite::create("v2ip/vipinfobg.png");
//        pContainLayer -> addChild(pVipInfo1);
        CCSprite* pVipInfo2 = CCSprite::create("v2ip/vipinfobg.png");
        pContainLayer -> addChild(pVipInfo2);
        CCSprite* pVipInfo3 = CCSprite::create("v2ip/vipinfobg.png");
        pContainLayer -> addChild(pVipInfo3);
        char tempB[64];
        char tempC[63];
        sprintf(tempB, "1.充值即可获得%0.1f%%返利", m_vctVIPInfo[i]->fChargeBackPercent);
        sprintf(tempC, "2.每日最高领取%s救济金", m_vctVIPInfo[i]->sReward.c_str());
        CCLabelTTF* pLabel2 = CCLabelTTF::create(tempB, "Arial", 32);
        pLabel2 -> setAnchorPoint(ccp(0, 0));
        pLabel2 -> setPosition(ccp(13, 8));
        pVipInfo2 -> addChild(pLabel2);
        pLabel2 -> setColor(ccc3(232, 230, 34));
        CCLabelTTF* pLabel3 = CCLabelTTF::create(tempC, "Arial", 32);
        pLabel3 -> setAnchorPoint(ccp(0, 0));
        pLabel3 -> setPosition(ccp(13, 8));
        pVipInfo3 -> addChild(pLabel3);
        pLabel3 -> setColor(ccc3(232, 230, 34));
        if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)//审核中...
        {
            pContainLayer->setContentSize(CCSizeMake(460, 370+90*i-180));
            pContainLayer->setPosition(ccp(0, -pContainLayer->getContentSize().height+370));
            float fHight = pContainLayer->getContentSize().height;
            pVipInfo2 -> setPosition(ccp(210, fHight-50-90*0));
            pVipInfo3 -> setPosition(ccp(210, fHight-50-90*1));
            
        }
        else
        {
            pContainLayer->setContentSize(CCSizeMake(460, 370-90));
            pContainLayer->setPosition(ccp(0, -pContainLayer->getContentSize().height+370));
            float fHight = pContainLayer->getContentSize().height;
            pVipInfo2 -> setPosition(ccp(210, fHight-50-90*0));
            pVipInfo3 -> setPosition(ccp(210, fHight-50-90*1));
//            
//            CCSprite* pVipInfo4 = CCSprite::create("v2ip/vipinfobg.png");
//            pVipInfo4 -> setPosition(ccp(210, fHight-50-90*2));
//            pContainLayer -> addChild(pVipInfo4);
//            CCLabelTTF* pLabel4 = CCLabelTTF::create(sInfo4[0], "Arial", 32);
//            pLabel4 -> setAnchorPoint(ccp(0, 0));
//            pLabel4 -> setPosition(ccp(13, 8));
//            pVipInfo4 -> addChild(pLabel4);
//            pLabel4 -> setColor(ccc3(232, 230, 34));
            
            CCSprite* pVipInfo5 = CCSprite::create("v2ip/vipinfobg.png");
            pVipInfo5 -> setPosition(ccp(210, fHight-50-90*2));
            pContainLayer -> addChild(pVipInfo5);
            char temp[64];
            sprintf(temp, "3.%s", m_vctVIPInfo[i]->sHitLv.c_str());
            CCLabelTTF* pLabel5 = CCLabelTTF::create(temp, "Arial", 32);
            pLabel5 -> setAnchorPoint(ccp(0, 0));
            pLabel5 -> setPosition(ccp(13, 8));
            pVipInfo5 -> addChild(pLabel5);
            pLabel5 -> setColor(ccc3(232, 230, 34));
        }
        
        int nHeight = 20;
        CCSprite* pVipScore = CCSprite::create("v2ip/vipscore.png");
        pVipScore -> setPosition(ccp(890, 370+nHeight));
        pVipInfoBg -> addChild(pVipScore);
        if(1)
        {
            CCSprite* pSpGuang = CCSprite::createWithSpriteFrameName("vipguang1.png");
            pSpGuang -> setPosition(ccp(890, 370+nHeight));
            pVipInfoBg -> addChild(pSpGuang, 3);
            pSpGuang -> setScale(1.3f);
            pSpGuang -> setFlipY(true);
            ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE };
            pSpGuang->setBlendFunc(cbl);
            CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
            CCArray* guangArr = CCArray::create();
            char temp[16];
            for(int i = 1; i <= 20; i++)
            {
                sprintf(temp, "vipguang%d.png", i);
                CCSpriteFrame* frame = cache->spriteFrameByName(temp);
                guangArr -> addObject(frame);
            }
            CCAnimation* animation = CCAnimation::createWithSpriteFrames(guangArr, 0.15f);
            CCAnimate* animate = CCAnimate::create(animation);
            pSpGuang -> runAction(CCRepeatForever::create(animate));
        }

        CCSprite* pVipScoreBg = CCSprite::create("v2ip/vipscorebg.png");
        pVipScoreBg -> setPosition(ccp(890, 245+nHeight));
        pVipScoreBg -> setTag(202);
        pVipInfoBg -> addChild(pVipScoreBg);
        if(m_vctVIPInfo[i]->nLevel == 1)
        {
            sprintf(temp, "返利+%0.1f%%", m_vctVIPInfo[i]->fChargeBackPercent);
        }
        else if(m_vctVIPInfo[i]->nLevel == 2)
        {
            sprintf(temp, "返利+%0.1f%%", m_vctVIPInfo[i]->fChargeBackPercent);
        }
        else if(m_vctVIPInfo[i]->nLevel == 3)
        {
            sprintf(temp, "返利+%0.1f%%", m_vctVIPInfo[i]->fChargeBackPercent);
        }
        else if(m_vctVIPInfo[i]->nLevel == 4)
        {
            sprintf(temp, "返利+%0.1f%%", m_vctVIPInfo[i]->fChargeBackPercent);
        }
        else if(m_vctVIPInfo[i]->nLevel == 5)
        {
            sprintf(temp, "返利+%0.1f%%", m_vctVIPInfo[i]->fChargeBackPercent);
        }
        else if(m_vctVIPInfo[i]->nLevel == 6)
        {
            sprintf(temp, "返利+%0.1f%%", m_vctVIPInfo[i]->fChargeBackPercent);
        }
        else if(m_vctVIPInfo[i]->nLevel == 7)
        {
            sprintf(temp, "返利+%0.1f%%", m_vctVIPInfo[i]->fChargeBackPercent);
        }
        else if(m_vctVIPInfo[i]->nLevel == 8)
        {
            sprintf(temp, "返利+%0.1f%%", m_vctVIPInfo[i]->fChargeBackPercent);
        }
        else
        {
            sprintf(temp, "返利+%0.1f%%", m_vctVIPInfo[i]->fChargeBackPercent);
        }
        CCLabelTTF* pLabelBack = CCLabelTTF::create(temp, "Arial", 36);
        pLabelBack -> setPosition(ccp(pVipScoreBg->getContentSize().width/2, pVipScoreBg->getContentSize().height/2));
        pVipScoreBg -> addChild(pLabelBack);
        pLabelBack -> setTag(204);
        
        CCLabelTTF* pLabelInfo2 = CCLabelTTF::create("充值的返利将通过邮件发放", "Arial", 32);
        pLabelInfo2 -> setPosition(ccp(890, 190));
        pVipInfoBg -> addChild(pLabelInfo2);
        
        pLabelBack->setVisible(false);
        if (m_nNoticeLevel > 0)
        {
            if (m_nNoticeLevel == m_vctVIPInfo[i]->nLevel)
            {
                pVipInfoBg -> setVisible(true);
                m_nCurrentVipLevel = m_nNoticeLevel;
            }
            else
            {
                pVipInfoBg -> setPosition(ccp(CLIENT_WIDTH, CLIENT_HEIGHT));
            }
        }
        else
        {
            if(m_nVipLevel == m_vctVIPInfo[i]->nLevel)
            {
                pVipInfoBg -> setVisible(true);
            }
            if(m_nVipLevel != m_vctVIPInfo[i]->nLevel)
            {
                pVipInfoBg -> setPosition(ccp(CLIENT_WIDTH, CLIENT_HEIGHT));
            }
        }
        
        pLabelBack->setVisible(true);
        
        
    }
    if(m_nVipLevel == 0)
    {
        CCSprite* pVipInfoBg = (CCSprite*)m_arrVipInfo->objectAtIndex(0);
        if(pVipInfoBg != NULL)
        {
            if(m_nNoticeLevel>1)
            {
                pVipInfoBg -> setVisible(false);
            }
            else
            {
                pVipInfoBg -> setVisible(true);
                pVipInfoBg -> setPosition(ccp(0, 0));
            }
        }
        
    }
}

void VIPLayer::clickMenu(CCObject* object)
{
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case 101:
        {
            if(m_nVipLevel == m_vctVIPInfo.size())
            {
                break;
            }
            item->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.4f), CCScaleTo::create(0.15f, 1.0f), NULL));
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            CCSprite* pLeft = NULL;
            CCSprite* pRight = NULL;
            for(unsigned int i = 0; i < m_arrVipInfo->count(); i++)
            {
                pLeft = (CCSprite*)m_arrVipInfo->objectAtIndex(i);
                if(m_nCurrentVipLevel == pLeft -> getTag())
                {
                    this->scheduleOnce(schedule_selector(VIPLayer::TimerToSetVipMenuTouchEnable), 0.5);
                    CCCallFunc* func1 = CCCallFunc::create(this, callfunc_selector(VIPLayer::setMenuTouch));
                    CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(VIPLayer::setMenuTouch2));
                    pLeft -> runAction(CCSequence::create(func1, CCSpawn::create(CCScaleTo::create(0.5f, 0), CCMoveTo::create(0.5f, ccp(CLIENT_WIDTH, CLIENT_HEIGHT/2)), NULL), func2, NULL));
                    if(m_nCurrentVipLevel == 1)//m_nVipLevel)//最左边，此时应该是最后的一个移动过来
                    {
                        pRight = (CCSprite*)m_arrVipInfo->objectAtIndex(m_arrVipInfo->count()-1);
                    }
                    else
                    {
                        int nextVipLevel = m_nCurrentVipLevel-1;
                        for(unsigned int j = 0; j < m_arrVipInfo->count(); j++)
                        {
                            pRight = (CCSprite*)m_arrVipInfo->objectAtIndex(j);
                            if(nextVipLevel == pRight -> getTag())
                            {
                                break;
                            }
                        }
                    }
                    if(pRight != NULL)
                    {
                        pRight -> setVisible(true);
                        pRight -> setPosition(ccp(-CLIENT_WIDTH/15,CLIENT_HEIGHT/2));
                        pRight -> setScale(0);
                        pRight -> runAction(CCSpawn::create(CCScaleTo::create(0.5f, 1.0f), CCMoveTo::create(0.5f, ccp(0, 0)), NULL));
                    }
                    if (m_nCurrentVipLevel == 1)
                    {
                        m_nCurrentVipLevel = 9;
                    }
                    else
                    {
                        m_nCurrentVipLevel --;
                    }
                    if(m_nVipLevel == 0)
                    {
                        if(m_nCurrentVipLevel == 0)
                        {
                            m_nCurrentVipLevel = (int)m_vctVIPInfo.size();
                        }
                    }
                    else
                    {
                        if(m_nCurrentVipLevel == m_nVipLevel-1)
                        {
                            //m_nCurrentVipLevel = (int)m_vctVIPInfo.size();
                        }
                    }
                    break;
                }
            }
            break;
        }
        case 102:
        {
            if(m_nVipLevel == m_vctVIPInfo.size())
            {
                break;
            }
            item->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.4f), CCScaleTo::create(0.15f, 1.0f), NULL));
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            CCSprite* pLeft = NULL;
            CCSprite* pRight = NULL;
            for(unsigned int i = 0; i < m_arrVipInfo->count(); i++)
            {
                pLeft = (CCSprite*)m_arrVipInfo->objectAtIndex(i);
                if(m_nCurrentVipLevel == pLeft -> getTag())
                {
                    this->scheduleOnce(schedule_selector(VIPLayer::TimerToSetVipMenuTouchEnable), 0.5);
                    CCCallFunc* func1 = CCCallFunc::create(this, callfunc_selector(VIPLayer::setMenuTouch));
                    CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(VIPLayer::setMenuTouch2));
                    pLeft -> runAction(CCSequence::create(func1, CCSpawn::create(CCScaleTo::create(0.5f, 0), CCMoveTo::create(0.5f, ccp(0, CLIENT_HEIGHT/2)), NULL), func2, NULL));
                    if(m_nCurrentVipLevel == m_vctVIPInfo.size())//最右边，此时应该是第一个移动过来
                    {
                        pRight = (CCSprite*)m_arrVipInfo->objectAtIndex(0);
                    }
                    else
                    {
                        int nextVipLevel = m_nCurrentVipLevel+1;
                        for(unsigned int j = 0; j < m_arrVipInfo->count(); j++)
                        {
                            pRight = (CCSprite*)m_arrVipInfo->objectAtIndex(j);
                            if(nextVipLevel == pRight -> getTag())
                            {
                                break;
                            }
                        }
                    }
                    if(pRight != NULL)
                    {
                        pRight -> setVisible(true);
                        pRight -> setPosition(ccp(CLIENT_WIDTH,CLIENT_HEIGHT/2));
                        pRight -> setScale(0);
                        pRight -> runAction(CCSpawn::create(CCScaleTo::create(0.5f, 1.0f), CCMoveTo::create(0.5f, ccp(0, 0)), NULL));
                    }
                    if (m_nCurrentVipLevel == 9) {
                        m_nCurrentVipLevel = 1;
                    }
                    else
                    {
                        m_nCurrentVipLevel++;
                    }
                    if(m_nCurrentVipLevel == m_vctVIPInfo.size()+1)
                    {
                        if(m_nVipLevel == 0)
                        {
//                            m_nCurrentVipLevel = 1;
                        }
                        else
                        {
//                            m_nCurrentVipLevel = m_nVipLevel;
                        }
                    }
                    break;
                }
            }
            break;
        }
        case 104:
        {
            SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
            
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eVIPUpgrade);
            if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
            {
                if(GameMainScene::_instance != NULL)//在游戏中
                {
                    //打开充值界面
                    CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器...", eMBExitGame);
                    CMD_GPR_QuerySkill QuerySkill = {0};
                    QuerySkill.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                    CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_SKILL, SUB_GPR_QUERY_SKILL, &QuerySkill, sizeof(QuerySkill));
                }
                else
                {
                    //判断是升级还是直接打开充值界面
                    if(m_bClickCharge)//升级
                    {
                        ChargeUI* pChargeUI = ChargeUI::create(m_nUpgradeMoney, 1);
                        CCDirector::sharedDirector()->getRunningScene()->addChild(pChargeUI, 13, 20003);
                    }
                    else
                    {
                        //打开充值界面
                        CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器...", eMBExitGame);
                        CMD_GPR_QuerySkill QuerySkill = {0};
                        QuerySkill.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_SKILL, SUB_GPR_QUERY_SKILL, &QuerySkill, sizeof(QuerySkill));
                    }
                }
            }
            else
            {
                CMainLogic::sharedMainLogic()->ShowMessage("正在连接服务器", eMBExitGame);
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle,MDM_GP_USER_SERVICE,SUB_GPR_IAP_PRODUCT_LIST);
            }
            this->removeFromParent();
            
            break;
        }
        default:
            break;
    }
}

void VIPLayer::setMenuTouch()
{
    m_VIPMenu->setTouchEnabled(false);
    m_bIsEnable = false;
}

void VIPLayer::setMenuTouch2()
{
    m_VIPMenu->setTouchEnabled(true);
    m_bIsEnable = true;
}

void VIPLayer::closeMenu(CCObject* object)
{
    
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CMainLogic::sharedMainLogic()->m_nGameToHallVipLevel = 1;
    
    this->removeFromParent();
    
    if(GameMainScene::_instance != NULL)
    {
        GameMainScene::GetInstance()->setWndHave(false);
        GameMainScene::GetInstance()->setAutoFire();
        GameMainScene::GetInstance()->setMatchShowStatus(true);
    }
}

void VIPLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool VIPLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_startPoint = pTouch->getLocation();
    return true;
}

void VIPLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(!m_bIsEnable)
        return ;
    CCPoint point = pTouch->getLocation();
    if(m_startPoint.y >= 80 && m_startPoint.y <= 560)
    {
        if(point.y >= 80 && point.y <= 560)
        {
            if(point.x-m_startPoint.x>120)
            {
                CCMenuItem* item = CCMenuItem::create();
                item->setTag(101);
                clickMenu(item);
            }
            else if(m_startPoint.x-point.x>120)
            {
                CCMenuItem* item = CCMenuItem::create();
                item->setTag(102);
                clickMenu(item);
            }
        }
    }
}

void VIPLayer::parseData(std::string str)
{
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("vipConfigInfos"))
        {
            delete document;
            return;
        }
        TiXmlElement * pCurrentElement = rootElement->FirstChildElement();
        while (NULL != pCurrentElement)
        {
            const char * pValue=pCurrentElement->Value();
            if (!strcmp(pValue,"vipConfigInfo"))
            {
                Tag_VIP2Info* pVIPInfo = new Tag_VIP2Info;
                m_vctVIPInfo.push_back(pVIPInfo);
                TiXmlElement* firstElement = pCurrentElement -> FirstChildElement();
                const char * valueId=firstElement->Value();
                if (!strcmp(valueId,"level"))
                {
                    int nLevel=atoi(firstElement->GetText());
                    pVIPInfo->nLevel = nLevel;
                }
                TiXmlElement* secondElement = firstElement -> NextSiblingElement();
                const char * nickname=secondElement->Value();
                if (!strcmp(nickname,"gold"))
                {
                    pVIPInfo->sReward = secondElement->GetText();
                }
                TiXmlElement* ThirdElement = secondElement -> NextSiblingElement();
                const char * score=ThirdElement->Value();
                if (!strcmp(score,"chargeBackPercent"))
                {
                    float score=atof(ThirdElement->GetText());
                    pVIPInfo->fChargeBackPercent = score;
                }
                TiXmlElement* FourElement = ThirdElement -> NextSiblingElement();
                const char * gailv=FourElement->Value();
                if (!strcmp(gailv,"buyugailv"))
                {
                    pVIPInfo->sHitLv = FourElement->GetText();
                }
            }
            pCurrentElement=pCurrentElement->NextSiblingElement();
        }
    }
    delete document;
    
    loadVIPInfoUI((int)m_vctVIPInfo.size());
    
    saveFile(str);
}

void VIPLayer::saveFile(std::string str)
{
    //在Documents目录下创建一个可读写的文件
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string xmlPath = path + std::string("VipConfig.xml");
    const char* buf = str.c_str();
    FILE* file = fopen(xmlPath.c_str(), "w");
    if(file)
    {
        fputs(buf, file);
        fclose(file);
        CCLog("save succeed");
    }
    else
    {
        CCLog("save file error");
    }
}

void VIPLayer::TimerToSetVipMenuTouchEnable(float dt)
{
    m_VIPMenu->setTouchEnabled(true);
}

void VIPLayer::setLabelScore()
{
    char temp[16];
    sprintf(temp, "%lld", CMainLogic::sharedMainLogic()->m_lUserScore);
    m_labelScore->setString(temp);
}
