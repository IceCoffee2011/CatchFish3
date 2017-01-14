//
//  NoticeOfHall.cpp
//  CatchFish3
//
//  Created by 王军闯 on 16/5/25.
//
//

#include "header.h"

CNoticeOfHall::~CNoticeOfHall()
{
    m_vctNotice.clear();
    m_vctNoticeStr.clear();
    m_noticeArray->release();
}

CNoticeOfHall* CNoticeOfHall::create(bool bLocalOpen, int nHall)
{
    CNoticeOfHall* pRet = new CNoticeOfHall;
    if(pRet && pRet->init(bLocalOpen, nHall))
    {
        pRet -> autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CNoticeOfHall::init(bool bLocalOpen, int nHall)
{
    if (!CCLayer::init())
    {
        return false;
    }
    m_vctNotice.clear();
    m_vctNoticeStr.clear();
    m_noticeArray = CCArray::create();
    m_noticeArray->retain();
    m_nOpenType = nHall;
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    this->setTouchEnabled(true);
    createNoticeScrollView();
    
    if(bLocalOpen)
    {
        readLocalNotice(nHall);
    }
    
    return true;
}

void CNoticeOfHall::createNoticeScrollView()
{
    CCSprite* pNoticeBg = CCSprite::create("c2harge/userinfobg.png");
    pNoticeBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT*0.46f));
    this->addChild(pNoticeBg);
//    CCSprite* pNoticeTitle = CCSprite::create("h3all/hallNoticeTitle.png");
//    pNoticeTitle->setPosition(ccp(pNoticeBg->getContentSize().width/2, pNoticeBg->getContentSize().height*0.9f));
//    pNoticeBg->addChild(pNoticeTitle);
    CCMenuItemImage* pCloseItem = CCMenuItemImage::create("h2all/dakacloseitem.png","h2all/dakacloseitem.png", this, menu_selector(CNoticeOfHall::closeNotice));
    pCloseItem->setPosition(ccp(pNoticeBg->getContentSize().width-10, pNoticeBg->getContentSize().height-10));
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    pMenu->setAnchorPoint(CCPointZero);
    pNoticeBg->addChild(pMenu);
    pMenu->setTouchPriority(-202);
    //    pNoticeBg->runAction(CCSequence::create(CCFadeIn::create(0.01f), CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.15f), CCScaleTo::create(0.07f, 1.0f), NULL));
    pNoticeBg->runAction(CCSequence::create(CCFadeIn::create(0.01f), CCScaleTo::create(0, 1.0f),CCScaleTo::create(0.1f, 1.3f), CCScaleTo::create(0.07f, 1.2f), NULL));
    
    m_pNoticeScrollView = CCScrollView::create();
    m_pNoticeScrollView->setAnchorPoint(ccp(0, 0));
    m_pNoticeScrollView->setPosition(ccp(32, 48));
    m_pNoticeScrollView->setContentSize(CCSizeMake(716, 338));
    m_pNoticeScrollView->setViewSize(CCSizeMake(716, 338));
    CCLayer* pContainerLayer = CCLayer::create();//创建一个层，作为滚动的内容
    pContainerLayer->setPosition(ccp(0, -120));
    m_pNoticeScrollView->setContainer(pContainerLayer);
    m_pNoticeScrollView->setBounceable(true);
    m_pNoticeScrollView->setDelegate(this);
    m_pNoticeScrollView->setTouchPriority(-202);
    m_pNoticeScrollView->setDirection(kCCScrollViewDirectionVertical);
    pNoticeBg->addChild(m_pNoticeScrollView);
}

void CNoticeOfHall::showNotice(bool bTitle, std::string sNotice)
{
    //计算公告的位置
    float height = 0;
    float fWidth = 716.0f;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    float fVertial = 45.0f;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    float fVertial = 52.0f;
#endif
    if (m_vctNotice.size() > 20)
    {
        CCLabelTTF* pLabel = (CCLabelTTF*)m_noticeArray->objectAtIndex(0);
        m_noticeArray -> removeObject(pLabel);
        pLabel -> removeFromParent();
        m_vctNotice.erase(m_vctNotice.begin());
    }
    for(unsigned int i = 0; i < m_vctNotice.size(); i++)
    {
        tagNoticeOfHall NoticeInfo = (tagNoticeOfHall)m_vctNotice[i];
        float width = NoticeInfo.fWidth;
        height = height + width/fWidth*fVertial+40;//每一个公告的高度
    }
    
    tagNoticeOfHall NoticeInfo;
    NoticeInfo.labelNotice = CCLabelTTF::create(sNotice.c_str(), "Arial-BoldMT", 32);
    NoticeInfo.fWidth = NoticeInfo.labelNotice->getContentSize().width;
    if (NoticeInfo.fWidth == 0)
    {
        CCImage* pTTFImage = new CCImage();//7b2f11
        CCImage::ETextAlign eAlign;
        pTTFImage->initWithString(sNotice.c_str(), 0, 0, eAlign, "Arial-BoldMT", 32);
        NoticeInfo.fWidth = pTTFImage->getWidth();
        delete pTTFImage;
    }
    NoticeInfo.labelNotice = HorizontalSpacingANDVerticalSpacing(bTitle, NoticeInfo.labelNotice->getString(), "Arial-BoldMT", 32, 1.5f, 1, fWidth-48);
    
    CCLayer* pContainerLayer = (CCLayer*)m_pNoticeScrollView->getContainer();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    float fY = height + NoticeInfo.fWidth/fWidth*fVertial+40;//对最后一条的位置有影响
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    float fY = height + NoticeInfo.fWidth/fWidth*fVertial+35;//对最后一条的位置有影响
#endif
    if(fY < 330)
    {
        m_pNoticeScrollView->setTouchEnabled(false);
        NoticeInfo.labelNotice -> setPosition(ccp(13, 432 - height));
    }
    else
    {
        m_pNoticeScrollView->setTouchEnabled(true);
        pContainerLayer->setContentSize(CCSizeMake(716, fY));
        m_pNoticeScrollView->setContentSize(CCSizeMake(716, fY));
        pContainerLayer->setPosition(ccp(0, 338-fY));
        NoticeInfo.labelNotice -> setPosition(ccp(13, NoticeInfo.fWidth/fWidth*fVertial));
        
        float width = 0;
        for(unsigned int i = 0; i < m_vctNotice.size(); i++)
        {
            tagNoticeOfHall NoticeInfo = (tagNoticeOfHall)m_vctNotice[i];
            width = width + NoticeInfo.fWidth/fWidth*fVertial+40;
            NoticeInfo.labelNotice -> setPosition(ccp(13, fY-width+NoticeInfo.fWidth/fWidth*fVertial));
        }
    }
    NoticeInfo.labelNotice->setAnchorPoint(ccp(0, 0.5f));
    pContainerLayer->addChild(NoticeInfo.labelNotice);
    m_noticeArray->addObject(NoticeInfo.labelNotice);
    
    m_vctNotice.push_back(NoticeInfo);
}

CCLabelTTF* CNoticeOfHall::HorizontalSpacingANDVerticalSpacing(bool bTitle, std::string _string, const char *fontName, float fontSize, float horizontalSpacing, float verticalSpacing, float lineWidth)
{
    if (bTitle)
    {
        fontSize = 40;
    }
    CCArray* labelTTF_arr = CCArray::create();
    int index = 0;
    int index_max = (int)strlen(_string.c_str());
    bool is_end = true;
    while (is_end) {
        if (_string[index] >= 0 && _string[index] <= 127) {
            std::string englishStr = _string.substr(index,1).c_str();
            const char* englishChar = englishStr.c_str();
            labelTTF_arr->addObject(CCLabelTTF::create(englishChar, fontName, fontSize));
            index+= 1;
        }
        else{
            std::string chineseStr = _string.substr(index,3).c_str();
            const char* chineseChar =  chineseStr.c_str();
            labelTTF_arr->addObject(CCLabelTTF::create(chineseChar, fontName, fontSize));
            index+= 3;
        }
        if (index>=index_max) {
            is_end=false;
        }
    }
    //以上步骤是根据ASCII码找出中英文字符，并创建成一个CCLabelTTF对象存入labelTTF_arr数组中。
    //下面创建的原理是在CCLabelTTF对象上添加子对象CCLabelTTF，以此组合成一句话，以左上角第一个字为锚点。。
    CCLabelTTF* returnTTF = (CCLabelTTF*)labelTTF_arr->objectAtIndex(0);
    returnTTF->setColor(ccc3(123, 47, 17));//98, 52, 35
    if (bTitle)
    {
        returnTTF->setColor(ccRED);
    }
    float nowWidth = returnTTF->getContentSize().width;
    CCLabelTTF* dangqiangTTF = returnTTF;
    CCLabelTTF* lineBeginTTF = returnTTF;
    
    int arr_count = labelTTF_arr->count();
    for (int i=1; i < arr_count; i++) {
        CCLabelTTF* beforeTTF = (CCLabelTTF*)labelTTF_arr->objectAtIndex(i);
        beforeTTF->setColor(ccc3(123, 47, 17));
        if (bTitle)
        {
            beforeTTF->setColor(ccRED);
        }
        beforeTTF->setAnchorPoint(ccp(0, 0.5));
        nowWidth+=beforeTTF->getContentSize().width;
        if (nowWidth >= lineWidth) {
            nowWidth = returnTTF->getContentSize().width;
            dangqiangTTF = lineBeginTTF;
            beforeTTF->setPosition(ccp(0, -dangqiangTTF->getContentSize().height*0.5-verticalSpacing));
            lineBeginTTF = beforeTTF;
        }else{
            beforeTTF->setPosition(ccp(dangqiangTTF->getContentSize().width+horizontalSpacing, dangqiangTTF->getContentSize().height*0.5));
        }
        dangqiangTTF->addChild(beforeTTF);
        dangqiangTTF = beforeTTF;
    }
    
    return returnTTF;
}

void CNoticeOfHall::closeNotice(CCObject* pObj)
{
    if(m_nOpenType == 1)
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("OpenGonggao", true);
    }
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    this->removeFromParent();
}

void CNoticeOfHall::readLocalNotice(int nHall)
{
    std::string path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    std::string path2 = "";
    if(nHall == 1)
    {
        path2 = "hallHallNoticeData.xml";
    }
    else
    {
        path2 = "chargeHallNoticeData.xml";
    }
    std::string xmlPath = path + path2;
    TiXmlDocument* document = new TiXmlDocument(xmlPath.c_str());
    if(document != NULL)
    {
        document->LoadFile();
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("noticelist"))
        {
            return;
        }
        TiXmlElement* pCurrentElement=rootElement->FirstChildElement();//"noticeInfo"
        while(pCurrentElement != NULL)
        {
            TiXmlElement* pFirstElement = pCurrentElement->FirstChildElement();
            if(!strcmp(pFirstElement->Value(), "index"))
            {
            }
            TiXmlElement* pSecondElement = pFirstElement->NextSiblingElement();
            if(!strcmp(pSecondElement->Value(), "title"))
            {
                m_vctNoticeStr.push_back(pSecondElement->GetText());
            }
            TiXmlElement* pThirdElement = pSecondElement->NextSiblingElement();
            if(!strcmp(pThirdElement->Value(), "content"))
            {
                m_vctNoticeStr.push_back(pThirdElement->GetText());
            }
            pCurrentElement = pCurrentElement -> NextSiblingElement();
        }
    }
    delete document;
    showAllNotice();
}

void CNoticeOfHall::parseData(std::string str)
{
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("noticelist"))
        {
            return;
        }
        TiXmlElement* pCurrentElement=rootElement->FirstChildElement();//"noticeInfo"
        while(pCurrentElement != NULL)
        {
            TiXmlElement* pFirstElement = pCurrentElement->FirstChildElement();
            if(!strcmp(pFirstElement->Value(), "index"))
            {
            }
            TiXmlElement* pSecondElement = pFirstElement->NextSiblingElement();
            if(!strcmp(pSecondElement->Value(), "title"))
            {
                m_vctNoticeStr.push_back(pSecondElement->GetText());
            }
            TiXmlElement* pThirdElement = pSecondElement->NextSiblingElement();
            if(!strcmp(pThirdElement->Value(), "content"))
            {
                m_vctNoticeStr.push_back(pThirdElement->GetText());
            }
            pCurrentElement = pCurrentElement -> NextSiblingElement();
        }
    }
    delete document;
    showAllNotice();
}

void CNoticeOfHall::showAllNotice()
{
    for (int i = 0 ; i < m_vctNoticeStr.size(); i++)
    {
        if (i%2 == 0)
        {
            showNotice(true, m_vctNoticeStr[i]);
        }
        else
        {
            showNotice(false, m_vctNoticeStr[i]);
        }
    }
}

void CNoticeOfHall::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -201, true);
}

bool CNoticeOfHall::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

void CNoticeOfHall::scrollViewDidScroll(CCScrollView* view)
{

}

void CNoticeOfHall::scrollViewDidZoom(CCScrollView* view)
{
    
}


