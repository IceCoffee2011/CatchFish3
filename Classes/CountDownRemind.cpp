//
//  CountDownRemind.cpp
//  CatchFish3
//
//  Created by 王军闯 on 16/6/1.
//
//

#include "header.h"


CCountDownRemind* CCountDownRemind::create(BYTE cbMatchStatus, int nCount, std::string sMatchTitle)
{
    CCountDownRemind* pRet = new CCountDownRemind;
    if(pRet && pRet->init(cbMatchStatus, nCount, sMatchTitle))
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

bool CCountDownRemind::init(BYTE cbMatchStatus, int nCount, std::string sMatchTitle)
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_cbMatchStatus = cbMatchStatus;
    m_nCount = nCount;
    
    m_spBg = CCSprite::create("competition/cmptremindbg.png");
    m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_spBg);
    
    CCLabelTTF* pLabelTitle = CCLabelTTF::create(sMatchTitle.c_str(), "Arial-BoldMT", 60);
    pLabelTitle -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height-45));
    m_spBg -> addChild(pLabelTitle);
    char sCount[2];
    sprintf(sCount, "%d", m_nCount);
    CCLabelAtlas* pCountAtlas = CCLabelAtlas::create(sCount, "competition/countDownNum.png", 165, 233, '0');
    pCountAtlas->setAnchorPoint(ccp(0.5f, 0.5f));//倒计时数字
    pCountAtlas->setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2));
    m_spBg->addChild(pCountAtlas, 0, 30);
    pCountAtlas -> setScale(0.6f);
    
    CCLabelTTF* pRemindTTF = CCLabelTTF::create("注：倒计时结束后显示结果", "Arial-BoldMT", 50);
    pRemindTTF->setPosition(ccp(m_spBg->getContentSize().width/2, 40));
    m_spBg->addChild(pRemindTTF);
    pRemindTTF->setColor(ccc3(253, 170, 70));
    if (!m_cbMatchStatus)
    {
        pRemindTTF->setString("注：倒计时结束后马上开始比赛");
    }
    this->schedule(schedule_selector(CCountDownRemind::disappearSchedule), 1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        SimpleAudioEngine::sharedEngine()->playEffect("Music/bisaidaojishi.mp3");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        SimpleAudioEngine::sharedEngine()->playEffect("Music/bisaidaojishi.ogg");
#endif
    
    return true;
}

void CCountDownRemind::disappearSchedule(float ft)
{
    CCLabelAtlas* pCountAtlas = (CCLabelAtlas*)m_spBg->getChildByTag(30);
    if (pCountAtlas != NULL)
    {
        m_nCount--;
        char sCount[2];
        sprintf(sCount, "%d", m_nCount);
        pCountAtlas->setString(sCount);
        if (m_nCount == 0)
        {
            this->removeFromParent();
        }
    }
}



