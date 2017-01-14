#include "header.h"

enum{
    eSignInBgTag = 30,
    eSignInMarkBg,
    eSignInMarkNormal,
    eSignInMarkVip,
};

const char* sSignInItemName[9] = {"signIn/coinIcon.png",
    "CatchFish2001/coinIcon.png",
    "signIn/huafeiIcon.png",
    "CatchFish2001/tongdantou.png",
    "CatchFish2001/yindantou.png",
    "CatchFish2001/jindantou.png",
    "CatchFish2001/speedup1.png",
    "CatchFish2001/sanshe1.png",
    "CatchFish2001/suodingsp2.png",};

const char* sZhenZhuName[3] = {"铜弹头", "银弹头", "金弹头"};

CSignInLayer::~CSignInLayer()
{
    m_arrVIP -> release();
    m_arrNormal -> release();
}

bool CSignInLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("CatchFish01/fish19.plist");
    m_arrNormal = CCArray::create();
    m_arrNormal -> retain();
    m_arrVIP = CCArray::create();
    m_arrVIP -> retain();
    m_bClickTouch = true;
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    this->addChild(pLayerColor);
    
    loadUI();
    
    return true;
}

void CSignInLayer::loadUI()
{
    CCSprite* pSignInBg = CCSprite::create("signIn/signInBg.png");
    pSignInBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this->addChild(pSignInBg, 0, eSignInBgTag);
    CCScale9Sprite* pSignMarkBg = CCScale9Sprite::create("signIn/signInMarkBg.png");
    pSignMarkBg->setAnchorPoint(ccp(0, 0.5f));
    pSignMarkBg->setPosition(ccp(pSignInBg->getContentSize().width*0.065f, pSignInBg->getContentSize().height*0.448f));
    pSignInBg->addChild(pSignMarkBg, 0, eSignInMarkBg);
    pSignMarkBg->setContentSize(CCSizeMake(75, 17));
    
    CCSprite* pSignMarkNormal = CCSprite::create("signIn/signInMark.png");
    pSignMarkNormal->setPosition(ccp(pSignInBg->getContentSize().width*0.127f, pSignInBg->getContentSize().height*0.466f));
    pSignInBg->addChild(pSignMarkNormal, 1, eSignInMarkNormal);
    CCSprite* pSignMarkVip = CCSprite::create("signIn/signInMark.png");
    pSignMarkVip->setRotation(180);
    pSignMarkVip->setPosition(ccp(pSignInBg->getContentSize().width*0.127f, pSignInBg->getContentSize().height*0.433f));
    pSignInBg->addChild(pSignMarkVip, 1, eSignInMarkVip);
    //关闭签到框按钮
    CCSprite* closeSignInSprite1 = CCSprite::create("newchoujiang/closeOn.png");
    CCSprite* closeSignInSprite2 = CCSprite::create("newchoujiang/closeOff.png");
    CCMenuItemSprite* closeSignInWnd = CCMenuItemSprite::create(closeSignInSprite1,closeSignInSprite2,this,menu_selector(CSignInLayer::closeWnd));
    closeSignInWnd -> setPosition(ccp(pSignInBg->getContentSize().width-closeSignInWnd->getContentSize().width/2+20, pSignInBg->getContentSize().height-closeSignInWnd->getContentSize().height));
    CCMenu* pMenuSignIn = CCMenu::create(closeSignInWnd,NULL);
    pMenuSignIn -> setPosition(CCPointZero);
    pMenuSignIn -> setAnchorPoint(CCPointZero);
    pSignInBg -> addChild(pMenuSignIn);
    pMenuSignIn->setTouchPriority(-202);
    
    pSignInBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
    
    this -> setTouchEnabled(true);
}

void CSignInLayer::closeWnd(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CCSprite* pSignInBg = (CCSprite*)this->getChildByTag(eSignInBgTag);//签到框
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(CSignInLayer::removeWnd));
    pSignInBg->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.1f), func, NULL));
}

void CSignInLayer::removeWnd()
{
    this -> removeFromParentAndCleanup(true);
    //如果收到新手引导消息，并且百炮房存在，并且玩家分数大于百炮房进入时分数
    bool bShowFirstCharge = true;
    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            FreeCoinLayer* pFreeCoinLayer = (FreeCoinLayer*)pHallLayer->getChildByTag(19996);
            if(pFreeCoinLayer != NULL)
            {
                pFreeCoinLayer->setFreeCoinBg(true);
                bShowFirstCharge = false;
            }
        }
    }
    if(!bShowFirstCharge)
        return;
    if(CMainLogic::sharedMainLogic()->m_bShowGuide == false)
    {
        bShowFirstCharge = true;
    }
    else
    {
        bShowFirstCharge = false;
    }
    //判断是不是分享包，是不是第一次启动
    if(CMainLogic::sharedMainLogic()->m_bIOSPassed && CMainLogic::sharedMainLogic()->m_sAndroidMachine == "fenxiang" && CCUserDefault::sharedUserDefault()->getBoolForKey("FirstLaunch", true))
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("FirstLaunch", false);
        //弹出分享码页面
        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            bShowFirstCharge = false;
            ShareLayer* pShareLayer = ShareLayer::create(1);
            pHallLayer -> addChild(pShareLayer, 200, 19982);
        }
    }
    //先判断是否有转盘界面
    if(bShowFirstCharge && CMainLogic::sharedMainLogic()->m_bIOSPassed && CMainLogic::sharedMainLogic()->m_bReceiveShowWheel)
    {
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                bShowFirstCharge = false;
                WheelLayer* pWheelLayer = WheelLayer::create();
                pHallLayer -> addChild(pWheelLayer, 199, 19981);
            }
        }
    }
    bool bShowFirstCharge2 = true;
    if(bShowFirstCharge && CMainLogic::sharedMainLogic()->m_bShowOneCharge && CMainLogic::sharedMainLogic()->m_bCanOneCharge && CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        //1元充值礼包
        bShowFirstCharge2 = false;
        if(CCUserDefault::sharedUserDefault()->getStringForKey("OneFirstCharge", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("OneCharge"))
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("OneFirstCharge", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("OneCharge"));
            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    bShowFirstCharge = false;
                    OneMoneyLayer* pOneMoneyLayer = OneMoneyLayer::create();
                    CCDirector::sharedDirector()->getRunningScene()->addChild(pOneMoneyLayer, 12, 20004);
                }
            }
        }
    }
    if(bShowFirstCharge && bShowFirstCharge2 && CMainLogic::sharedMainLogic()->m_cbCountTimes == 0 && CMainLogic::sharedMainLogic()->m_bIOSPassed)//10元充值礼包
    {
        if(CCUserDefault::sharedUserDefault()->getStringForKey("FirstCharge", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("Charge"))
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("FirstCharge", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("Charge"));
            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    bShowFirstCharge = false;
                    FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create();
                    pHallLayer->addChild(pFirstChargeLayer, 200);
                }
            }
        }
    }
    //每天弹一次广告框
    if(bShowFirstCharge && CMainLogic::sharedMainLogic()->m_bIOSPassed && CMainLogic::sharedMainLogic()->m_bShowAdvert)
    {
        if(CCUserDefault::sharedUserDefault()->getStringForKey("ShowAdvert", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("Advert"))
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("ShowAdvert", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("Advert"));
            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    bShowFirstCharge = false;
                    AdvertLayer* pAdvertLayer = AdvertLayer::create();
                    pHallLayer->addChild(pAdvertLayer, 200);
                }
            }
        }
    }
    if(bShowFirstCharge && CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                if(CCUserDefault::sharedUserDefault()->getBoolForKey("NoticeDataSuccess", false))
                {
                    pHallLayer -> scheduleOnce(schedule_selector(CHallLayer::showNoticeLayer), 0.0f);
                }
            }
        }
    }
}

void CSignInLayer::setSignMark(int nNormalDay, int nVipDay)
{
    CCSprite* pSignInBg = (CCSprite*)this->getChildByTag(eSignInBgTag);//签到框
    if(pSignInBg != NULL)
    {
        //标记条
        CCScale9Sprite* pSignMarkBg = (CCScale9Sprite*)pSignInBg->getChildByTag(eSignInMarkBg);
        if (pSignMarkBg != NULL)
        {
            //显示最大的签到天数
            if (nNormalDay > nVipDay)
            {
                pSignMarkBg->setContentSize(CCSizeMake(75+150*(nNormalDay-1), 17));
            }
            else
            {
                pSignMarkBg->setContentSize(CCSizeMake(75+150*(nVipDay-1), 17));
            }
        }
        //正常签到标记
        CCSprite* pSignMarkNormal = (CCSprite*)pSignInBg->getChildByTag(eSignInMarkNormal);
        if (pSignMarkNormal != NULL)
        {
            pSignMarkNormal->setPosition(ccp(pSignInBg->getContentSize().width*0.127f+150*(nNormalDay-1), pSignInBg->getContentSize().height*0.466f));
        }
        //VIP签到标记
        CCSprite* pSignMarkVip = (CCSprite*)pSignInBg->getChildByTag(eSignInMarkVip);
        if (pSignMarkVip != NULL)
        {
            pSignMarkVip->setPosition(ccp(pSignInBg->getContentSize().width*0.127f+150*(nVipDay-1), pSignInBg->getContentSize().height*0.433f));
        }
    }
}

void CSignInLayer::refreshSignedDay(int nSignInType)//0为普通用户，1为VIP签到
{
    if(nSignInType == 0)
    {
        if(m_cbNormalDay>=1 && m_cbNormalDay<=7)
        {
            m_normalDay[m_cbNormalDay-1].cbAttrib = 1;
        }
    }
    else if(nSignInType == 1)
    {
        if(m_cbVipDay>=1 && m_cbVipDay<=7)
        {
            m_vipDay[m_cbVipDay-1].cbAttrib = 1;
        }
    }
    CCSprite* pSignInBg = (CCSprite*)this->getChildByTag(eSignInBgTag);//签到框
    if(pSignInBg != NULL)
    {
        for(int i = 0; i < 7; i++)
        {
            if(m_normalDay[i].cbAttrib == 1)
            {
                CCSprite* pColorNormal = (CCSprite*)pSignInBg->getChildByTag(200+i);
                if (pColorNormal != NULL)
                {
                    pColorNormal->setVisible(true);
                    
                }
                CCSprite* pNormal = (CCSprite*)pSignInBg->getChildByTag(100+i);
                if (pNormal != NULL)
                {
                    pNormal->stopAllActions();
                    pNormal -> setPosition(ccp(pSignInBg->getContentSize().width*0.125f+150*i, pSignInBg->getContentSize().height*0.6f));
                    pNormal -> removeChildByTag(601);
                }
            }
            if(m_vipDay[i].cbAttrib == 1)
            {
                CCSprite* pColorVip = (CCSprite*)pSignInBg->getChildByTag(400+i);
                if (pColorVip != NULL)
                {
                    pColorVip->setVisible(true);
                }
                CCSprite* pVIP = (CCSprite*)pSignInBg->getChildByTag(300+i);
                if (pVIP != NULL)
                {
                    pVIP->stopAllActions();
                    pVIP -> setPosition(ccp(pSignInBg->getContentSize().width*0.125f+150*i, pSignInBg->getContentSize().height*0.25f));
                    pVIP -> removeChildByTag(601);
                }
            }
        }
    }
}

void CSignInLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -201, true);
}

bool CSignInLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if(!m_bClickTouch)
    {
        return true;
    }
    CCPoint point = pTouch->getLocation();
    CCSprite* pSignInBg = (CCSprite*)this->getChildByTag(eSignInBgTag);
    if (pSignInBg == NULL)
    {
        return false;
    }
    point = pSignInBg->convertToNodeSpace(point);
    for(int i = 0; i < m_arrVIP->count(); i++)
    {
        CCSprite* sprite = (CCSprite*)m_arrVIP->objectAtIndex(i);
        if(sprite == NULL) continue;
        CCRect rect = sprite->boundingBox();
        if(rect.containsPoint(point))
        {
            if(sprite->getTag()-300+1 == m_cbVipDay && m_vipDay[i].cbAttrib == 0 && CMainLogic::sharedMainLogic()->m_nVipLevel>0)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallSignIn4);
                CMD_GP_SignIn SignIn;
                SignIn.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                SignIn.cbVip = 1;
                CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GP_USER_SIGNIN_REQ, &SignIn,sizeof(SignIn));
                CMainLogic::sharedMainLogic()->ShowMessage("正在签到，请稍候", eMBExitGame);
            }
            else
            {
                if(CMainLogic::sharedMainLogic()->m_nVipLevel == 0)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("您当前不是VIP用户，达到VIP1以上即可领取此奖励", eMBOK);
                }
                else
                {
                    if(m_cbVipDay<i+1)
                    {
                        char temp[128];
                        if(m_vipDay[m_cbVipDay-1].cbAttrib == 0)
                        {
                            sprintf(temp, "您再连续登录%d天即可领取当前奖励", i+2-m_cbVipDay);
                        }
                        else
                        {
                            sprintf(temp, "您再连续登录%d天即可领取当前奖励", i+1-m_cbVipDay);
                        }
                        
                        CMainLogic::sharedMainLogic()->ShowMessage(temp, eMBOK);
                    }
                    else
                    {
                        CMainLogic::sharedMainLogic()->ShowMessage("您已领取此奖励", eMBOK);
                    }
                }
            }
        }
    }
    for(int i = 0; i < m_arrNormal->count(); i++)
    {
        CCSprite* sprite = (CCSprite*)m_arrNormal->objectAtIndex(i);
        if(sprite == NULL) continue;
        CCRect rect = sprite->boundingBox();
        if(rect.containsPoint(point))
        {
            if(sprite->getTag()-100+1 == m_cbNormalDay)
            {
                if(m_normalDay[i].cbAttrib == 0)//发送领取消息
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallSignIn1);
                    CMD_GP_SignIn SignIn;
                    SignIn.dwUserID = CMainLogic::sharedMainLogic()->m_nUserID;
                    SignIn.cbVip = 0;
                    CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_GP_USER_SERVICE, SUB_GP_USER_SIGNIN_REQ, &SignIn,sizeof(SignIn));
                    CMainLogic::sharedMainLogic()->ShowMessage("正在签到，请稍候", eMBExitGame);
                }
                else
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("您已领取此奖励", eMBOK);
                }
            }
            else
            {
                if(m_cbNormalDay<i+1)
                {
                    char temp[128];
                    if(m_normalDay[m_cbNormalDay-1].cbAttrib == 0)
                    {
                        sprintf(temp, "您再连续登录%d天即可领取当前奖励", i+2-m_cbNormalDay);
                    }
                    else
                    {
                        sprintf(temp, "您再连续登录%d天即可领取当前奖励", i+1-m_cbNormalDay);
                    }
                    CMainLogic::sharedMainLogic()->ShowMessage(temp, eMBOK);
                }
                else
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("您已领取此奖励", eMBOK);
                }
            }
        }
    }
    return true;
}
/*
 BYTE m_cbNormalDay;
 BYTE m_cbVipDay;
 tagSignInDayInfo m_normalDay[7];
 tagSignInDayInfo m_vipDay[7];
 //签到查询每天信息
 struct tagSignInDayInfo
 {
 BYTE cbType;//1金币、2奖券、3话费、4铜弹头、5银弹头、6金弹头、7加速、8散射、9锁定
 BYTE cbAttrib;//本天签没签（1为已签）
 int nCount;
 };
 */
void CSignInLayer::updateUI()
{
    CCSprite* pSignInBg = (CCSprite*)this->getChildByTag(eSignInBgTag);
    //普通签到
    for(int i = 0; i < 7; i++)
    {
        CCSprite* pNormalDay = NULL;
        CCLabelAtlas* pLabelNormalNum = NULL;
        CCLabelAtlas* pLabelMoney = NULL;           //签到获取奖励数目
        CCSprite* pColorSp = NULL;                  //已领取
        CCSprite* pPriseTypeSp = NULL;              //奖励类型icon
        char temp[5];
        sprintf(temp, "%d",i+1);
        char sPrise[8];
        sprintf(sPrise, "%d", m_normalDay[i].nCount);
        if(m_normalDay[i].cbType == 3)//话费
        {
            pNormalDay = CCSprite::create("signIn/signInDayBg1.png");
            pLabelNormalNum = CCLabelAtlas::create(temp, "signIn/signDayNum1.png", 16, 25, '0');
            pLabelNormalNum -> setPosition(ccp(pNormalDay->getContentSize().width/2, pNormalDay->getContentSize().height-32));
            pLabelMoney = CCLabelAtlas::create(sPrise, "signIn/signPriseNum1.png", 16, 20, '0');
            pLabelMoney -> setPosition(ccp(pNormalDay->getContentSize().width*0.26f, 36));
            pColorSp = CCSprite::create("signIn/colorLayer2.png");
        }
        else
        {
            pNormalDay = CCSprite::create("signIn/signInDayBg0.png");
            pLabelNormalNum = CCLabelAtlas::create(temp, "signIn/signDayNum0.png", 17, 19, '0');
            pLabelNormalNum -> setPosition(ccp(pNormalDay->getContentSize().width/2, pNormalDay->getContentSize().height-18));
            pLabelMoney = CCLabelAtlas::create(sPrise, "signIn/signPriseNum0.png", 17, 31, '0');
            pLabelMoney -> setPosition(ccp(pNormalDay->getContentSize().width/2, 26));
            pColorSp = CCSprite::create("signIn/colorLayer0.png");
        }
        if(m_normalDay[i].cbType <= 9 && m_normalDay[i].cbType >= 1)
        {
            pPriseTypeSp = CCSprite::create(sSignInItemName[m_normalDay[i].cbType-1]);
            if(pPriseTypeSp != NULL)
            {
                if(m_normalDay[i].cbType == 3)
                {
                    pPriseTypeSp->setPosition(ccp(pNormalDay->getContentSize().width/2, pNormalDay->getContentSize().height*0.34f-10));
                    pPriseTypeSp -> setScale(0.8f);
                }
                else if(m_normalDay[i].cbType == 2)
                {
                    pPriseTypeSp->setPosition(ccp(pNormalDay->getContentSize().width/2, pNormalDay->getContentSize().height*0.34f));
                    pPriseTypeSp -> setScale(0.6f);
                }
                else if(m_normalDay[i].cbType == 4 || m_normalDay[i].cbType == 5 || m_normalDay[i].cbType == 6)
                {
                    pPriseTypeSp->setPosition(ccp(pNormalDay->getContentSize().width/2, pNormalDay->getContentSize().height*0.34f-15));
                    pPriseTypeSp -> setScale(0.6f);
                    //增加珍珠底图
                    CCSprite* pSpDi = CCSprite::create("CatchFish2001/gamezhenzhubg.png");
                    pSpDi -> setPosition(ccp(pNormalDay->getContentSize().width/2, pNormalDay->getContentSize().height*0.34+26));
                    pSpDi->setScale(0.47f);
                    pNormalDay->addChild(pSpDi);
                    pLabelMoney -> setVisible(false);
                    pSpDi->setVisible(false);
                    CCLabelTTF* pLabelName = CCLabelTTF::create(sZhenZhuName[m_normalDay[i].cbType-4], "Arial-BoldMT", 24);
                    pLabelName -> setPosition(ccp(pNormalDay->getContentSize().width/2, 28));
                    pNormalDay -> addChild(pLabelName);
                    
                }
                else
                {
                    pPriseTypeSp->setPosition(ccp(pNormalDay->getContentSize().width/2, pNormalDay->getContentSize().height*0.34f));
                }
                
                pPriseTypeSp->setAnchorPoint(ccp(0.5f, 0));
                pNormalDay->addChild(pPriseTypeSp, 1);
            }
        }
        
        
        pNormalDay -> setPosition(ccp(pSignInBg->getContentSize().width*0.125f+150*i, pSignInBg->getContentSize().height*0.6f));
        pSignInBg -> addChild(pNormalDay, 0, 100+i);
        m_arrNormal -> addObject(pNormalDay);
        pLabelNormalNum -> setAnchorPoint(ccp(0.5,0.5));
        pNormalDay -> addChild(pLabelNormalNum);
        pLabelMoney -> setAnchorPoint(ccp(0.5,0.5));
        pNormalDay -> addChild(pLabelMoney);
        
        pColorSp->setOpacity(100);
        pColorSp->setPosition(pNormalDay->getPosition());
        pSignInBg->addChild(pColorSp, 0, 200+i);
        CCSprite* pSignInOver = CCSprite::create("signIn/signInOver.png");
        pSignInOver->setPosition(ccp(pColorSp->getContentSize().width/2, pColorSp->getContentSize().height/2));
        pColorSp->addChild(pSignInOver);
        if(m_normalDay[i].cbAttrib == 0)//未签到
        {
            pColorSp->setVisible(false);
        }
        if(m_cbNormalDay == i+1)
        {
            if(m_normalDay[i].cbAttrib==0)
            {
                pNormalDay->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f, ccp(0, 15)), CCMoveBy::create(0.5f, ccp(0, -15)), NULL)));
                CCSprite* pGuang = CCSprite::createWithSpriteFrameName("newbarrelguang1.png");
                pGuang -> setPosition(ccp(pNormalDay->getContentSize().width/2, pNormalDay->getContentSize().height/2));
                pNormalDay -> addChild(pGuang, 1, 601);
                CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
                char temp[24];
                CCArray* array = CCArray::create();
                for(int i = 1; i <= 20; i++)
                {
                    sprintf(temp, "newbarrelguang%d.png", i);
                    CCSpriteFrame* frame = cache->spriteFrameByName(temp);
                    array -> addObject(frame);
                }
                CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, 0.07f);
                CCAnimate* animate = CCAnimate::create(animation);
                pGuang -> runAction(CCRepeatForever::create(animate));
                pGuang -> setScale(1.5f);
            }
        }
    }
    //VIP签到
    for(int i = 0; i < 7; i++)
    {
        CCSprite* pVIPDay = NULL;
        CCLabelAtlas* pLabelVIPNum = NULL;
        CCLabelAtlas* pLabelMoney = NULL;           //签到获取奖励数目
        CCSprite* pColorSp2 = NULL;                 //已领取
        CCSprite* pPriseTypeIcon = NULL;              //奖励类型icon
        char temp[5];
        sprintf(temp, "%d",i+1);
        char sPrise[8];
        sprintf(sPrise, "%d", m_vipDay[i].nCount);
        if(m_vipDay[i].cbType == 3)//话费
        {
            pVIPDay = CCSprite::create("signIn/signInDayBg1.png");
            pLabelVIPNum = CCLabelAtlas::create(temp, "signIn/signDayNum1.png", 16, 25, '0');
            pLabelVIPNum -> setPosition(ccp(pVIPDay->getContentSize().width/2, pVIPDay->getContentSize().height-32));
            pLabelMoney = CCLabelAtlas::create(sPrise, "signIn/signPriseNum1.png", 16, 20, '0');
            pLabelMoney -> setPosition(ccp(pVIPDay->getContentSize().width*0.26f, 36));
            pColorSp2 = CCSprite::create("signIn/colorLayer2.png");
        }
        else
        {
            pVIPDay = CCSprite::create("signIn/signInDayBg2.png");
            pLabelVIPNum = CCLabelAtlas::create(temp, "signIn/signDayNum0.png", 17, 19, '0');
            pLabelVIPNum -> setPosition(ccp(pVIPDay->getContentSize().width*0.685f, pVIPDay->getContentSize().height-18));
            pLabelMoney = CCLabelAtlas::create(sPrise, "signIn/signPriseNum2.png", 17, 31, '0');
            pLabelMoney -> setPosition(ccp(pVIPDay->getContentSize().width/2-8, 28));
            pColorSp2 = CCSprite::create("signIn/colorLayer1.png");
        }
        if(m_vipDay[i].cbType <= 9 && m_vipDay[i].cbType >= 1)
        {
            pPriseTypeIcon = CCSprite::create(sSignInItemName[m_vipDay[i].cbType-1]);
            if(pPriseTypeIcon != NULL)
            {
                if(m_vipDay[i].cbType == 3)
                {
                    pPriseTypeIcon->setPosition(ccp(pVIPDay->getContentSize().width/2, pVIPDay->getContentSize().height*0.34f-10));
                    pPriseTypeIcon->setScale(0.8f);
                }
                else if(m_vipDay[i].cbType == 2)
                {
                    pPriseTypeIcon->setPosition(ccp(pVIPDay->getContentSize().width/2, pVIPDay->getContentSize().height*0.34f));
                    pPriseTypeIcon -> setScale(0.6f);
                }
                else if(m_vipDay[i].cbType == 4 || m_vipDay[i].cbType == 5 || m_vipDay[i].cbType == 6)
                {
                    pPriseTypeIcon->setPosition(ccp(pVIPDay->getContentSize().width/2, pVIPDay->getContentSize().height*0.34f-15));
                    pPriseTypeIcon -> setScale(0.6f);
                    //增加珍珠底图
                    CCSprite* pSpDi = CCSprite::create("CatchFish2001/gamezhenzhubg.png");
                    pSpDi -> setPosition(ccp(pVIPDay->getContentSize().width/2, pVIPDay->getContentSize().height*0.34+26));
                    pSpDi->setScale(0.47f);
                    pVIPDay->addChild(pSpDi);
                    pLabelMoney -> setVisible(false);
                    pSpDi->setVisible(false);
                    CCLabelTTF* pLabelName = CCLabelTTF::create(sZhenZhuName[m_vipDay[i].cbType-4], "Arial-BoldMT", 24);
                    pLabelName -> setPosition(ccp(pVIPDay->getContentSize().width/2, 28));
                    pVIPDay -> addChild(pLabelName);
                    pLabelName -> setColor(ccc3(200, 42, 0));
                }
                else
                {
                    pPriseTypeIcon->setPosition(ccp(pVIPDay->getContentSize().width/2, pVIPDay->getContentSize().height*0.34f));
                }
                
                pPriseTypeIcon->setAnchorPoint(ccp(0.5f, 0));
                pVIPDay->addChild(pPriseTypeIcon, 1);
            }
        }
        pVIPDay -> setPosition(ccp(pSignInBg->getContentSize().width*0.125f+150*i, pSignInBg->getContentSize().height*0.25f));
        pSignInBg -> addChild(pVIPDay, 0, 300+i);
        m_arrVIP -> addObject(pVIPDay);
        pLabelVIPNum -> setAnchorPoint(ccp(0.5,0.5));
        pVIPDay -> addChild(pLabelVIPNum);
        pLabelMoney -> setAnchorPoint(ccp(0.5,0.5));
        pVIPDay -> addChild(pLabelMoney);
        pColorSp2->setOpacity(100);
        pColorSp2->setPosition(pVIPDay->getPosition());
        pSignInBg->addChild(pColorSp2, 0, 400+i);
        CCSprite* pSignInOver2 = CCSprite::create("signIn/signInOver.png");
        pSignInOver2->setPosition(ccp(pColorSp2->getContentSize().width/2, pColorSp2->getContentSize().height/2));
        pColorSp2->addChild(pSignInOver2);
        if(m_vipDay[i].cbAttrib == 0)//未签到
        {
            pColorSp2->setVisible(false);
        }
        if(m_cbVipDay == i+1 && CMainLogic::sharedMainLogic()->m_nVipLevel>=1)
        {
            if(m_vipDay[i].cbAttrib==0)
            {
                pVIPDay->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f, ccp(0, 15)), CCMoveBy::create(0.5f, ccp(0, -15)), NULL)));
                CCSprite* pGuang = CCSprite::createWithSpriteFrameName("newbarrelguang1.png");
                pGuang -> setPosition(ccp(pVIPDay->getContentSize().width/2, pVIPDay->getContentSize().height/2));
                pVIPDay -> addChild(pGuang, 1, 601);
                CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
                char temp[24];
                CCArray* array = CCArray::create();
                for(int i = 1; i <= 20; i++)
                {
                    sprintf(temp, "newbarrelguang%d.png", i);
                    CCSpriteFrame* frame = cache->spriteFrameByName(temp);
                    array -> addObject(frame);
                }
                CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, 0.07f);
                CCAnimate* animate = CCAnimate::create(animation);
                pGuang -> runAction(CCRepeatForever::create(animate));
                pGuang -> setScale(1.5f);
            }
        }
    }
}

void CSignInLayer::setClickTouch()
{
    m_bClickTouch = true;
}

void CSignInLayer::showSignInSuccess(int nSignInType)
{
    m_bClickTouch = false;
    this -> scheduleOnce(schedule_selector(CSignInLayer::setClickTouch), 2.3f);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    SimpleAudioEngine::sharedEngine()->playEffect("Music/openbox.mp3");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->playEffect("Music/openbox.ogg");
#endif
    CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(CSignInLayer::removeSignInSuccess));
    CCSprite* sprite5 = CCSprite::create("signIn/signsuccess5.png");
    sprite5 -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(sprite5, 200);
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    pLayerColor -> ignoreAnchorPointForPosition(false);
    pLayerColor -> setPosition(ccp(sprite5->getContentSize().width/2, sprite5->getContentSize().height/2));
    sprite5 -> addChild(pLayerColor, -2);
    CCSprite* sprite6 = CCSprite::create("signIn/signsuccess6.png");
    sprite6 -> setPosition(ccp(sprite5->getContentSize().width/2, sprite5->getContentSize().height/2));
    sprite5 -> addChild(sprite6, -1);
    sprite5 -> setOpacity(0);
    sprite6 ->setOpacity(0);
    sprite6 -> runAction(CCRepeatForever::create(CCRotateBy::create(2.0f, 180)));
    sprite6 -> runAction(CCFadeIn::create(0.5f));
    sprite5 -> runAction(CCSequence::create(CCFadeIn::create(0.3f), CCDelayTime::create(2.0f),func, NULL));
    CCSprite* sprite3 = CCSprite::create("signIn/signsuccess3.png");
    sprite3 -> setPosition(ccp(sprite5->getContentSize().width/2, sprite5->getContentSize().height/4*3-10));
    sprite5 -> addChild(sprite3, 200);
    CCSprite* sprite41 = CCSprite::create("signIn/signsuccess4.png");
    sprite41 -> setPosition(ccp(sprite3->getContentSize().width/4, sprite3->getContentSize().height/2+10));
    sprite3 -> addChild(sprite41, 200);
    CCSprite* sprite42 = CCSprite::create("signIn/signsuccess4.png");
    sprite42 -> setPosition(ccp(sprite3->getContentSize().width/4*3, sprite3->getContentSize().height/2+10));
    sprite3 -> addChild(sprite42, 200);
    CCSprite* sprite1 = CCSprite::create("signIn/signsuccess1.png");
    sprite1 -> setPosition(ccp(sprite3->getContentSize().width/4*2, sprite3->getContentSize().height/2+10));
    sprite3 -> setScale(3.0f);
    sprite3 -> addChild(sprite1, 200);
    sprite3 -> runAction(CCSequence::create(CCHide::create(), CCDelayTime::create(0.2f), CCShow::create(), CCScaleTo::create(0.2f, 1.0f), NULL));
    
    
    CCSprite* pAwardBg3 = CCSprite::create("signIn/signAwardBg.png");
    pAwardBg3->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-20));
    this->addChild(pAwardBg3, 200);
    pAwardBg3 -> runAction(CCRepeatForever::create(CCRotateTo::create(2, -180)));
    CCSprite* pAwardBg = CCSprite::create("signIn/signAwardBg.png");
    pAwardBg->setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-20));
    this->addChild(pAwardBg, 200);
    pAwardBg -> setOpacity(0);
    CCSprite* pAward = NULL;
    char sNum[16];
    CCLabelAtlas* pNum = CCLabelAtlas::create("", "signIn/signsuccess2.png", 53, 79, '0');
    CCSprite* pMark = NULL;
    if(nSignInType == 0)
    {
        if(m_cbNormalDay>=1 && m_cbNormalDay<=7)
        {
            m_normalDay[m_cbNormalDay-1].cbAttrib = 1;
            int nTypeID = m_normalDay[m_cbNormalDay-1].cbType;
            pAward = CCSprite::create(sSignInItemName[nTypeID-1]);
            pAward->setPosition(ccp(pAwardBg->getContentSize().width/2, pAwardBg->getContentSize().height/2));
            pAwardBg->addChild(pAward);
            if (nTypeID == 2)
            {
                pAward->setScale(0.8f);
            }
            else if (nTypeID == 3)
            {
                pAward->setScale(1.2f);
                pAward->setPositionY(pAwardBg->getContentSize().height/2-10);
            }
            else if (nTypeID>=10)
            {
                pAward->setScale(0.7f);
            }
            sprintf(sNum, "%d", m_normalDay[m_cbNormalDay-1].nCount);
            pNum ->setString(sNum);
            
            pAwardBg->setPositionX(CLIENT_WIDTH/2);
            
            pNum->setScale(0.7f);
            pNum->setAnchorPoint(ccp(0.5f, 0.5f));
            pNum->setPosition(ccp(pAward->getPositionX(),-pNum->boundingBox().size.height/2+50));
            pAwardBg->addChild(pNum);
            
            pMark = CCSprite::create("signIn/signsuccess8.png");
            pMark->setScale(0.6f);
            pMark->setAnchorPoint(ccp(1, 0.5f));
            pMark->setPosition(ccp(pNum->getPositionX()-pNum->boundingBox().size.width/2-5, pNum->getPositionY()));
            pAwardBg->addChild(pMark);
        }
    }
    else if(nSignInType == 1)
    {
        CCSprite* pAwardBg2 = CCSprite::create("signIn/signAwardBgVIP.png");
        pAwardBg2->setPosition(ccp(pAwardBg->getContentSize().width/2, pAwardBg2->getContentSize().height));
        pAwardBg->addChild(pAwardBg2, 2);
        
        if(m_cbVipDay>=1 && m_cbVipDay<=7)
        {
            m_vipDay[m_cbVipDay-1].cbAttrib = 1;
            int nTypeID = m_vipDay[m_cbVipDay-1].cbType;
            pAward = CCSprite::create(sSignInItemName[nTypeID-1]);
            pAward->setPosition(ccp(pAwardBg->getContentSize().width/2, pAwardBg->getContentSize().height/2));
            pAwardBg->addChild(pAward);
            if (nTypeID == 2)
            {
                pAward->setScale(0.8f);
            }
            else if (nTypeID == 3)
            {
                pAward->setScale(1.2f);
                pAward->setPositionY(pAwardBg->getContentSize().height/2-10);
            }
            sprintf(sNum, "%d", m_vipDay[m_cbVipDay-1].nCount);
            pNum ->setString(sNum);
            pAwardBg->setPositionX(CLIENT_WIDTH/2);
            pNum->setScale(0.7f);
            pNum->setAnchorPoint(ccp(0.5f, 0.5f));
            pNum->setPosition(ccp(pAward->getPositionX(),-pNum->boundingBox().size.height/2+50));
            pAwardBg->addChild(pNum);
            
            pMark = CCSprite::create("signIn/signsuccess8.png");
            pMark->setScale(0.6f);
            pMark->setAnchorPoint(ccp(1, 0.5f));
            pMark->setPosition(ccp(pNum->getPositionX()-pNum->boundingBox().size.width/2-5, pNum->getPositionY()));
            pAwardBg->addChild(pMark);
        }
    }
    pAwardBg->runAction(CCSequence::create(CCDelayTime::create(2.2f),CCCallFuncN::create(this, callfuncN_selector(CSignInLayer::removeSignInSuccess)),NULL));
    pAwardBg3->runAction(CCSequence::create(CCDelayTime::create(2.2f),CCCallFuncN::create(this, callfuncN_selector(CSignInLayer::removeSignInSuccess)),NULL));
}

void CSignInLayer::removeSignInSuccess(CCNode* pNode)
{
    pNode -> removeFromParent();
}
