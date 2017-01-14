#include "header.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../platform/android/jni/JniHelper.h"
#include "jni.h"

extern int nWxChargeResult;

extern "C"
{
    //方法名与java类中的包名+方法名，以下划线连接
    void Java_cn_sharesdk_hellocpp_HelloCpp_WXChargeResult(JNIEnv*  env, jobject thiz, jint a)
    {
		if(CMainLogic::sharedMainLogic()->m_nBuyGameType == 1)//购买金币
		{
			if(CMainLogic::sharedMainLogic()->m_bGameChargeStatus)//在游戏中
			{
				if(a == 1)//支付成功
				{
					CMainLogic::sharedMainLogic()->m_bChargeSuccess = true;
				}
				else
				{
					CMainLogic::sharedMainLogic()->m_bChargeSuccess = false;
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeCancel);
				}
				//0.1秒之后切换场景
				if(GameMainScene::_instance != NULL)
				{
					CMainLogic::sharedMainLogic()->m_sAlipaySign = "";
					CMainLogic::sharedMainLogic()->m_bNetPageCharge = false;
					GameMainScene::_instance -> openReplaceSceneTime();
				}
                else if(GameLayer::_instance != NULL)
                {
                    CMainLogic::sharedMainLogic()->m_sAlipaySign = "";
                    CMainLogic::sharedMainLogic()->m_bNetPageCharge = false;
                    GameLayer::_instance -> openReplaceSceneTime();
                }
			}
			else
			{
				CMainLogic::sharedMainLogic()->m_bChargeSuccess = false;
                if(CMainLogic::sharedMainLogic()->m_bFirstChargeUIStatus)//10元首充
                {
                    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
                    if(a == 1)//微信支付成功
                    {
                        CMainLogic::sharedMainLogic()->showDelayWechatResult();
                    }
                }
                else
                {
                    //这里java调用nativePaySuccess(1);，这里就会被调用。jint a = 1
                    ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
                    if(pChargeLayer != NULL)
                    {
                        CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
                        if(a == 1)
                        {
                            //微信支付成功
                            pChargeLayer->scheduleOnce(schedule_selector(ChargeLayer::delayWXChargeSuccess), 0.1f);
                        }
                        else
                        {
                            //微信支付失败或者取消
                            pChargeLayer->setWebViewCharge(false);
                            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeCancel);
                        }
                    }
                }
				
			}
		}
    }
    
    //方法名与java类中的包名+方法名，以下划线连接
    void Java_cn_sharesdk_hellocpp_HelloCpp_AlipayChargeResult(JNIEnv*  env, jobject thiz, jint a, jstring jstrSign)
    {
		CMainLogic::sharedMainLogic()->m_bChargeSuccess = false;
		//判断充值类型（金币、武器、技能）
		if(CMainLogic::sharedMainLogic()->m_nBuyGameType == 1)//购买金币
		{
			//判断是否是游戏中充值
			if(CMainLogic::sharedMainLogic()->m_bGameChargeStatus)
			{
				if(a == 1)//支付成功
				{
					CMainLogic::sharedMainLogic()->m_sAlipaySign = JniHelper::jstring2string(jstrSign);
				}
				else
				{
					CMainLogic::sharedMainLogic()->m_sAlipaySign = "";
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeCancel);
				}
				//0.1秒之后切换场景
				if(GameMainScene::_instance != NULL)
				{
					CMainLogic::sharedMainLogic()->m_bNetPageCharge = false;
					GameMainScene::_instance -> openReplaceSceneTime();
				}
                else if(GameLayer::_instance != NULL)
                {
                    CMainLogic::sharedMainLogic()->m_bNetPageCharge = false;
                    GameLayer::_instance -> openReplaceSceneTime();
                }
			}
			else
			{
				//这里java调用nativePaySuccess(1);，这里就会被调用。jint a = 1
                if(CMainLogic::sharedMainLogic()->m_bFirstChargeUIStatus)//10元首充
                {
                    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
                    if(a == 1)//微信支付成功
                    {
                        CMainLogic::sharedMainLogic()->showDelayAlipayResult();
                    }
                }
                else
                {
                    ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
                    if(pChargeLayer != NULL)
                    {
                        CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
                        if(a == 1)
                        {
                            pChargeLayer->setAlipaySign(JniHelper::jstring2string(jstrSign));
                            //支付宝支付成功
                            pChargeLayer->scheduleOnce(schedule_selector(ChargeLayer::delayAlipayChargeSuccess), 0.1f);
                        }
                        else
                        {
                            //支付宝支付失败或者取消
                            pChargeLayer->setWebViewCharge(false);
                            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeCancel);
                        }
                    }
                }
			}
		}
    }
	
	//方法名与java类中的包名+方法名，以下划线连接
    void Java_cn_sharesdk_hellocpp_HelloCpp_YeePayChargeResult(JNIEnv*  env, jobject thiz, jint a)
    {
		CMainLogic::sharedMainLogic()->m_bChargeSuccess = false;
		CMainLogic::sharedMainLogic()->updateCurrentScore();
		if(CMainLogic::sharedMainLogic()->m_nBuyGameType == 1)//购买金币
		{
			//如果是在游戏中充值，那么切换场景
			if(CMainLogic::sharedMainLogic()->m_bGameChargeStatus)
			{
				if(GameMainScene::_instance != NULL)
				{
					CMainLogic::sharedMainLogic()->m_sAlipaySign = "";
					CMainLogic::sharedMainLogic()->m_bNetPageCharge = true;//网页充值返回
					CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
					GameMainScene::_instance -> openReplaceSceneTime();
				}
                else if(GameLayer::_instance != NULL)
                {
                    CMainLogic::sharedMainLogic()->m_sAlipaySign = "";
                    CMainLogic::sharedMainLogic()->m_bNetPageCharge = true;//网页充值返回
                    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
                    GameLayer::_instance -> openReplaceSceneTime();
                }
			}
			else
			{
				ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
				if(pChargeLayer != NULL)
				{
					pChargeLayer -> setWebViewCharge(false);
				}
			}
		}
    }
}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "FMLayerWebView.h"
#include "IOSiAP_Bridge.h"
extern int WeiXinCharge;
extern int ChargeSuccess;
extern std::string AlipayChargeSuccess;
#endif

bool getCannonByType3(int m, int n)
{
    return (m & (1 << (n-1)));
}

void ChargeLayer::setAlipaySign(std::string str)
{
    m_sAlipaySign = str;
	CMainLogic::sharedMainLogic()->m_sAlipaySign = str;
}

void ChargeLayer::delayAlipayChargeSuccess(float dt)
{
	sendAlipayChargeSuccess();
}

void ChargeLayer::delayWXChargeSuccess(float dt)
{
	sendWXChargeSuccess();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
enum tagChargeType
{
    Alipay_Tag = 100,       //支付宝
    Wechat_Tag,             //微信
    Bank_Tag,               //银行卡
    IOSCharge_Tag,          //苹果内购
    Card_Tag,               //点卡
    Telephone_Tag,          //话费
    Message_Tag,            //短信
    ChargeValue01_tag,      //3元
    ChargeValue02_tag,      //5元
    ChargeValue1_tag,            //10元
    ChargeValue2_tag,             //20元
    ChargeValue3_tag,              //50元
    ChargeValue4_tag,         //100元
    ChargeValue5_tag,         //200元
    ChargeValue6_tag,        //500元
    eZengLabel_Tag,              //赠送标签
};
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
enum tagChargeType
{
    Alipay_Tag = 100,       //支付宝
    Wechat_Tag,             //微信
    Bank_Tag,               //银行卡
    Card_Tag,               //点卡
    Telephone_Tag,          //话费
    Message_Tag,            //短信
    ChargeValue01_tag,      //3元
    ChargeValue02_tag,      //5元
    ChargeValue1_tag,            //10元
    ChargeValue2_tag,             //20元
    ChargeValue3_tag,              //50元
    ChargeValue4_tag,         //100元
    ChargeValue5_tag,         //200元
    ChargeValue6_tag,        //500元
    eZengLabel_Tag,              //赠送标签
};
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
extern std::string orderIDStr;
extern std::string chargeTypeStr;
extern int nchargeRMB;
const int ChargeNumTag[8] = {107, 108, 109, 110, 111, 112, 113, 114};
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const int ChargeNumTag[8] = {106, 107, 108, 109, 110, 111, 112, 113};
#endif
const char* ChargeTypeInfo[8] = {"18000金币","120000金币","240000金币",  "620000金币", "104万金币", "204万金币", "392万金币","1352万金币"};
const char* ChargeTypeMoney1[8] = {"1", "6", "12", "30", "50", "98", "188", "648"};
const char* ChargeTypeUI[8] = {"charge/chargejinbi1.png","charge/chargejinbi2.png","charge/chargejinbi3.png", "charge/chargejinbi4.png", "charge/chargejinbi5.png", "charge/chargejinbi6.png",  "charge/chargejinbi7.png","charge/chargejinbi8.png"};
const char* ChargeJinbiInfo[7] = {"charge/chargejinbiinfo1.png", "charge/chargejinbiinfo2.png", "charge/chargejinbiinfo3.png", "charge/chargejinbiinfo4.png", "charge/chargejinbiinfo5.png", "charge/chargejinbiinfo6.png", "charge/chargejinbiinfo7.png"};
const char* ChargeTypeIOSCMoney[12] = {"1", "6", "12", "30", "50", "98", "188", "648", "18", "18", "18", "18"};
const char* ProductIDs[12] = {"com.youkuss.lelecoin9", "com.youkuss.lelecoin10", "com.youkuss.lelecoin11", "com.youkuss.lelecoin12",  "com.youkuss.lelecoin13",  "com.youkuss.lelecoin14", "com.youkuss.lelecoin15", "com.youkuss.lelecoin16", "com.youkuss.lelea2", "com.youkuss.lelea3", "com.youkuss.lelea4", "com.youkuss.lelea6"};
ChargeLayer::~ChargeLayer()
{
    m_arrChargeType -> release();
    m_arrChargeNum -> release();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    delete bridge;
    bridge = NULL;
#endif
}

//nOpenType == 0为普通充值类型
//nOpenType == 10为首充10元
//nOpenType == 1为升级VIP，充值金额自己生成

//注：购买武器和技能，只能使用支付宝、微信、银行卡其中一种支付方式

ChargeLayer* ChargeLayer::create(int nOpenType, int nUpgradeMoney, int nOpenCharge)
{
    ChargeLayer* pRet = new ChargeLayer;
    if(pRet && pRet->init(nOpenType, nUpgradeMoney, nOpenCharge))
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

bool ChargeLayer::init(int nOpenType, int nUpgradeMoney, int nOpenCharge)
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_bOpenGonggao = false;
    m_nOpenType = nOpenType;
    m_nUpgradeMoney = nUpgradeMoney;//升级所需要的金额
    m_nOpenCharge = nOpenCharge;
    m_nCannonStatus = 0;
    m_nArrayCurrentIndex = 0;
    CMainLogic::sharedMainLogic()->m_bFirstChargeUIStatus = false;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    for(int i = 0; i < 12; i++)
    {
        CMD_GPR_IAPProduct IAPProduct = {0};
        IAPProduct.wRMB = atoi(ChargeTypeIOSCMoney[i]);
        IAPProduct.wType = 0;
        sprintf(IAPProduct.szProductID, "%s", ProductIDs[i]);
        m_IAPProductList.product[i] = IAPProduct;
    }
	bridge = new IOSiAP_Bridge(m_IAPProductList);
#endif
    
    
    m_arrChargeType = CCArray::create();
    m_arrChargeType -> retain();
    m_arrChargeNum = CCArray::create();
    m_arrChargeNum -> retain();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("charge/firstchargetx.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3all/tehuihuodong.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3all/quickstart.plist");
    
    m_bIsOpenCard = true;
    bWebViewCharge = false;
    m_sOrderID = "";
    m_sWXOrderID = "";
    m_sAlipaySign = "";
    m_nPrise = 0;
    
    CCSprite* pChargeBg = CCSprite::create("barrelUI/lotterybg.png");
    pChargeBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(pChargeBg);
    pChargeBg -> setScale(2.0f);
    CCSprite* pChargeTopBg = CCSprite::create("charge/chargetopbg.png");
    pChargeTopBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT-pChargeTopBg->getContentSize().height/2));
    this -> addChild(pChargeTopBg);
    pChargeTopBg -> setOpacity(0);
    
    
    m_spriteCoin = CCSprite::create("charge/coinitem2.png");
    m_spriteCoin -> setPosition(ccp(117, pChargeTopBg->getContentSize().height/2+3));
    pChargeTopBg -> addChild(m_spriteCoin, 2);
    
    //VIP、充值返利按钮
    CCMenuItemImage* chargeBackItem = CCMenuItemImage::create("charge/vipitem0.png", "charge/vipitem0.png", this, menu_selector(ChargeLayer::clickVIPMenu));
    chargeBackItem -> setTag(101);
    chargeBackItem -> setPosition(ccp(1150-114, 670));
    CCMenuItemImage* firstChargeItem = CCMenuItemImage::create("charge/chargefirstitem.png", "charge/chargefirstitem.png", this, menu_selector(ChargeLayer::clickVIPMenu));
    firstChargeItem -> setTag(103);
    firstChargeItem -> setPosition(ccp(960-114, 670));
    CCMenuItemImage* oneChargeItem = CCMenuItemImage::create("charge/onechargeitem.png", "charge/onechargeitem.png", this, menu_selector(ChargeLayer::clickVIPMenu));
    oneChargeItem -> setTag(105);
    oneChargeItem -> setPosition(ccp(770-114, 670));
    m_oneChargeMenu = CCMenu::create(oneChargeItem, NULL);
    m_oneChargeMenu -> setPosition(CCPointZero);
    m_oneChargeMenu -> setAnchorPoint(CCPointZero);
    this -> addChild(m_oneChargeMenu);
    m_oneChargeMenu -> setTouchPriority(-200);
    m_oneChargeMenu->setVisible(false);
    
//    CCSprite* pHuodong = CCSprite::createWithSpriteFrameName("tehuihuodong1.png");
//    CCMenuItemSprite* houdongItem = CCMenuItemSprite::create(pHuodong, pHuodong, this, menu_selector(ChargeLayer::clickVIPMenu));
//    houdongItem -> setTag(104);
//    houdongItem -> setPosition(ccp(770-114, 670));
//    houdongItem -> runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 1.2f), CCScaleTo::create(0.5f, 1.0f), NULL)));
//    CCSprite* pHuodong2 = CCSprite::createWithSpriteFrameName("tehuihuodong1.png");
//    pHuodong2 -> setPosition(ccp(pHuodong->getContentSize().width/2, pHuodong->getContentSize().height/2));
//    pHuodong -> addChild(pHuodong2);
//    char tempTehui[32];
//    CCArray* tehuiArray = CCArray::create();
//    for(int i = 1; i <= 16; i++)
//    {
//        sprintf(tempTehui, "tehuihuodong%d.png", i);
//        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(tempTehui);
//        tehuiArray -> addObject(frame);
//    }
//    CCAnimation* tehuiAnimation = CCAnimation::createWithSpriteFrames(tehuiArray, 0.15f);
//    CCAnimate* tehuiAnimate = CCAnimate::create(tehuiAnimation);
//    pHuodong2 -> runAction(CCRepeatForever::create(tehuiAnimate));
    
    CCSprite* firstChargeTx = CCSprite::createWithSpriteFrameName("firstchargetx1.png");
    firstChargeTx -> setPosition(ccp(firstChargeItem->getContentSize().width/2, firstChargeItem->getContentSize().height/2));
    
    CCArray* firstChargeArr = CCArray::create();
    char firstChargeTemp[32];
    for(int i = 1; i <= 30; i++)
    {
        sprintf(firstChargeTemp, "firstchargetx%d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(firstChargeTemp);
        firstChargeArr -> addObject(frame);
    }
    CCAnimation* firstChargeAnimation = CCAnimation::createWithSpriteFrames(firstChargeArr, 0.05f);
    CCAnimate* firstChargeAnimate = CCAnimate::create(firstChargeAnimation);
    firstChargeTx -> runAction(CCRepeatForever::create(firstChargeAnimate));
    if(CMainLogic::sharedMainLogic()->m_bShowOneCharge && CMainLogic::sharedMainLogic()->m_bCanOneCharge)
    {
        m_oneChargeMenu->setVisible(true);
        oneChargeItem -> addChild(firstChargeTx);
    }
    else
    {
        firstChargeItem -> addChild(firstChargeTx);
    }
    
    m_firstChargeMenu = CCMenu::create(firstChargeItem, NULL);
    m_firstChargeMenu -> setPosition(CCPointZero);
    m_firstChargeMenu -> setAnchorPoint(CCPointZero);
    this -> addChild(m_firstChargeMenu);
    m_firstChargeMenu -> setTouchPriority(-200);
    m_firstChargeMenu -> setVisible(false);
    if(CMainLogic::sharedMainLogic()->m_cbCountTimes == 0)//可以首充
    {
        m_firstChargeMenu -> setVisible(true);
    }
    else
    {
        //houdongItem -> setPosition(ccp(960-114, 670));
        oneChargeItem -> setPosition(ccp(960-114, 670));
    }
    if(nOpenType == 10 || nOpenType == 1)//首充或者升级VIP
    {
        m_firstChargeMenu -> setVisible(false);
        chargeBackItem -> setVisible(false);
        m_oneChargeMenu -> setVisible(false);
    }
    
    CCMenuItemImage* backItem = CCMenuItemImage::create("barrelUI/lotterycloseitem.png", "barrelUI/lotterycloseitem.png", this, menu_selector(ChargeLayer::clickBack));
    backItem -> setScale(2.0f);
    backItem -> setPosition(ccp(CLIENT_WIDTH-backItem->boundingBox().size.width/2, CLIENT_HEIGHT-backItem->boundingBox().size.height/2));
    
    m_BackMenu = CCMenu::create(chargeBackItem, backItem, NULL);
    m_BackMenu -> setPosition(CCPointZero);
    m_BackMenu -> setAnchorPoint(CCPointZero);
    this -> addChild(m_BackMenu);
    m_BackMenu -> setTouchPriority(-200);
    
//    if(GameLayer::_instance != NULL)
//    {
//        chargeBackItem -> setVisible(false);
//        //houdongItem -> setPosition(ccp(1150-114, 670));
//    }
    m_labelInfo = CCLabelTTF::create("温馨提示: 升级VIP后，每笔充值都可在《邮件》中领取返利！", "Arial", 32);
    m_labelInfo -> setPosition(ccp(35, 15));
    m_labelInfo -> setAnchorPoint(CCPointZero);
    this -> addChild(m_labelInfo);
    
    if(nOpenType == 0)
    {
        loadUI();
        load2PublicChargeType();
    }
    else if(nOpenType == 10 || nOpenType == 1)//首充或者升级VIP，武器按钮和技能按钮是不可以点击的
    {
        //houdongItem -> setVisible(false);
    }
    
    if(!CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        m_firstChargeMenu -> setVisible(false);
        m_oneChargeMenu -> setVisible(false);
    }
    
    //houdongItem->setVisible(false);
    
    
    this -> setTouchEnabled(true);
    
    return true;
}

void ChargeLayer::loadUI()
{
    m_nodeChargeType = CCNode::create();
    this -> addChild(m_nodeChargeType, 1);
    m_nodeChargeType->setVisible(false);
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    m_nodeChargeType->addChild(pLayerColor);
    
    loadChargeTypeUI();
}

void ChargeLayer::loadChargeTypeUI()
{
    m_spChargeTypeBg = CCSprite::create("charge/chargetypebg.png");
    m_spChargeTypeBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    m_nodeChargeType -> addChild(m_spChargeTypeBg);
    CCMenuItemImage* closeItem = CCMenuItemImage::create("charge/closetypeItem.png", "charge/closetypeItem.png", this, menu_selector(ChargeLayer::clickBack));
    closeItem -> setPosition(ccp(m_spChargeTypeBg->getContentSize().width-45, m_spChargeTypeBg->getContentSize().height-35));
    closeItem -> setScale(1.2f);
    CCMenu* pCloseMenu = CCMenu::create(closeItem, NULL);
    pCloseMenu -> setPosition(CCPointZero);
    pCloseMenu -> setAnchorPoint(CCPointZero);
    m_spChargeTypeBg -> addChild(pCloseMenu);
    pCloseMenu -> setTouchPriority(-200);
    
    CCSprite* pWechat = CCSprite::create("charge/wechatitem.png");
    pWechat -> setTag(101);
    pWechat -> setPosition(ccp(m_spChargeTypeBg->getContentSize().width/3-45, m_spChargeTypeBg->getContentSize().height/3*2-30));
    m_spChargeTypeBg -> addChild(pWechat);
    m_arrChargeType -> addObject(pWechat);
    CCSprite* pAlipay = CCSprite::create("charge/alipayitem.png");
    pAlipay -> setTag(100);
    pAlipay -> setPosition(ccp(m_spChargeTypeBg->getContentSize().width/3*2+45, m_spChargeTypeBg->getContentSize().height/3*2-30));
    m_spChargeTypeBg -> addChild(pAlipay);
    m_arrChargeType -> addObject(pAlipay);
    CCSprite* pBank = CCSprite::create("charge/bankitem.png");
    pBank -> setTag(102);
    pBank -> setPosition(ccp(m_spChargeTypeBg->getContentSize().width/3-45, m_spChargeTypeBg->getContentSize().height/3-40));
    m_spChargeTypeBg -> addChild(pBank);
    m_arrChargeType -> addObject(pBank);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCSprite* pApple = CCSprite::create("charge/appleitem.png");
    pApple -> setTag(103);
    pApple -> setPosition(ccp(m_spChargeTypeBg->getContentSize().width/3*2+45, m_spChargeTypeBg->getContentSize().height/3-40));
    m_spChargeTypeBg -> addChild(pApple);
    m_arrChargeType -> addObject(pApple);
#endif
}

void ChargeLayer::clickBack(CCObject* object)
{
    if(object != NULL)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    }
    CMainLogic::sharedMainLogic()->m_nBuyCannon2Index = 0;
    CMainLogic::sharedMainLogic()->m_nBuyCannonIndex = 0;
    if(!m_bIsOpenCard)
    {
        m_BackMenu -> setTouchEnabled(true);
        m_firstChargeMenu -> setTouchEnabled(true);
        m_oneChargeMenu -> setTouchEnabled(true);
        if(m_nOpenCharge == 0)//购买金币
        {
            m_nodeChargeType -> setVisible(false);
            m_nodeChargeNum -> setVisible(true);
            m_bIsOpenCard = true;
            m_nodeSpDi->setVisible(true);
            //m_scrollPublic->setTouchEnabled(true);
        }
        if(GameMainScene::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameCloseBack);
        }
        else if(GameLayer::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHaidaoCloseChargeTag);
        }
        else
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallCloseBack);
        }
        CMainLogic::sharedMainLogic()->m_bEnterService = false;
    }
    else
    {
        CMainLogic::sharedMainLogic()->m_bChargeState = false;
        CMainLogic::sharedMainLogic()->m_bGameToVIP = false;
        CMainLogic::sharedMainLogic()->m_bBYGameToSuper = false;
        CMainLogic::sharedMainLogic()->m_bEnterService = false;
        CMainLogic::sharedMainLogic()->m_nExitGameToSpecialArm = 0;
        if(GameMainScene::_instance != NULL)
        {
            //如果是因为没有金币打开的充值，那么这里就要去查询免费金币
            if(GameMainScene::GetInstance()->getHaveNoScore())
            {
                GameMainScene::GetInstance()->setHaveNoScore(false);
                //如果是在千炮房或者万炮房，不提示免费金币
                if(GameMainScene::GetInstance()->getCellScore()<1000)
                {
                    if(CMainLogic::sharedMainLogic()->m_nBankruptcyCount < CMainLogic::sharedMainLogic()->m_nGiveTotal && GameMainScene::_instance->getUserCurrentScore() < CMainLogic::sharedMainLogic()->m_nGiveScore)//有领取次数，并且玩家分数小于领取限制分数，才能领取救济金
                    {
                        CMainLogic::sharedMainLogic()->m_cbCanGetBankruptcy = 1;
                        //发送是否可以领取救济金的消息
                        GameMainScene::_instance->m_pClientKernel->SendSocketData(MDM_GR_USER, SUB_GRR_QUERYBANKCUPTINFO);
                    }
                }
            }
            GameMainScene::GetInstance()->setWndHave(false);
            GameMainScene::GetInstance()->setAutoFire();
            GameMainScene::GetInstance()->setMatchShowStatus(true);
            //判断是否下分了，如果下分了，那么就切换场景
            if(GameMainScene::GetInstance()->getShowExitGame())
            {
                CMainLogic::sharedMainLogic()->m_bGameChargeStatus = false;
                CMainLogic::sharedMainLogic()->switchLayer(eHallLayer);
            }
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameClose);
        }
        else if(GameLayer::_instance != NULL)
        {
            if(GameLayer::GetInstance()->getShowExitGame())
            {
                CMainLogic::sharedMainLogic()->m_bGameChargeStatus = false;
                CMainLogic::sharedMainLogic()->switchLayer(eHallLayer);
            }
        }
        else
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallClose);
        }
        this->removeFromParent();
    }
}

void ChargeLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -161, true);
}

bool ChargeLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    m_startPoint = pTouch->getLocation();
    return true;
}

void ChargeLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    if(bWebViewCharge)
        return;
    CCPoint endPoint = pTouch->getLocation();
    if(abs((int)m_startPoint.x-(int)endPoint.x) > 10)
        return ;
    if(m_nOpenType == 10)//首充
    {
        CCPoint point = m_nodeChargeType->convertToNodeSpace(endPoint);
        for(int i = 0; i < m_arrChargeType->count(); i++)
        {
            CCSprite* pButtonBg = (CCSprite*)m_arrChargeType->objectAtIndex(i);
            CCRect rect = pButtonBg->boundingBox();
            if(rect.containsPoint(point))
            {
                CMainLogic::sharedMainLogic()->m_bClickMessageCharge = false;
                m_nCurrentSelectedType = pButtonBg->getTag();
                selectedChargeTypeAndPrise(pButtonBg->getTag(), 0);
                break;
            }
        }
        return;
    }
    if(m_nOpenType == 1)//升级VIP
    {
        if(abs((int)m_startPoint.x-(int)endPoint.x) > 10)
            return ;
        else
        {
            CCPoint point = m_nodeChargeType->convertToNodeSpace(endPoint);
            for(int i = 0; i < m_arrChargeType->count(); i++)
            {
                CCSprite* pButtonBg = (CCSprite*)m_arrChargeType->objectAtIndex(i);
                CCRect rect = pButtonBg->boundingBox();
                if(rect.containsPoint(point))
                {
                    CMainLogic::sharedMainLogic()->m_bClickMessageCharge = false;
                    m_nCurrentSelectedType = pButtonBg->getTag();
                    selectedChargeTypeAndPrise(pButtonBg->getTag(), 0);
                    break;
                }
            }
        }
        return;
    }
    if(m_nOpenCharge == 0)//充值金币
    {
        if(m_bIsOpenCard == false)//选择支付方式
        {
            if(abs((int)m_startPoint.x-(int)endPoint.x) > 10)
                return ;
            else
            {
                CCPoint point = m_spChargeTypeBg->convertToNodeSpace(endPoint);
                for(int i = 0; i < m_arrChargeType->count(); i++)
                {
                    CCSprite* pButtonBg = (CCSprite*)m_arrChargeType->objectAtIndex(i);
                    CCRect rect = pButtonBg->boundingBox();
                    if(rect.containsPoint(point))
                    {
                        m_nCurrentSelectedType = pButtonBg->getTag();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                        if(pButtonBg->getTag() == IOSCharge_Tag)
                        {
                            //传入选项卡的tag值
                            selectedChargeTypeAndPriseIOS(m_nArrayCurrentIndex);
                        }
                        else
#endif
                        {
                            selectedChargeTypeAndPrise(m_nCurrentSelectedType, m_nArrayCurrentIndex);
                        }
                        break;
                    }
                }
            }
        }
        else//充值数量界面,选择充值金额，进入到充值方式界面
        {
            endPoint = m_nodeChargeNum->convertToNodeSpace(endPoint);
            for(int i = 0; i < m_arrChargeNum->count(); i++)
            {
                CCSprite* pChargeBumBg = (CCSprite*)m_arrChargeNum->objectAtIndex(i);
                CCRect rect = pChargeBumBg->boundingBox();
                if(rect.containsPoint(endPoint))
                {
                    if(!CMainLogic::sharedMainLogic()->m_bAnySDKPassed)
                    {
                        m_nArrayCurrentIndex = i;
                        m_nCurrentSelectedType = Wechat_Tag;
                        selectedChargeTypeAndPrise(Wechat_Tag, i);
                        sendUserBehavior(0, i);
                    }
                    else
                    {
                        m_BackMenu -> setTouchEnabled(false);
                        m_firstChargeMenu -> setTouchEnabled(false);
                        m_oneChargeMenu -> setTouchEnabled(false);
                        m_nodeChargeType->setVisible(true);
                        m_bIsOpenCard = false;
                        //m_nodeChargeNum->setVisible(false);
                        m_nArrayCurrentIndex = i;
                        //m_nodeSpDi->setVisible(false);
                        //m_scrollPublic->setTouchEnabled(false);
                        sendUserBehavior(0, i);
                    }
                    break;
                }
            }
        }
    }
}

void ChargeLayer::selectedChargeSuccess()
{
    int index = m_nCurrentIndex;
    int type = m_nCurrentType;
    int prise = 0;
    if(bWebViewCharge)
    {
        return;
    }
    if(m_nCurrentSelectedType == Alipay_Tag || m_nCurrentSelectedType == Wechat_Tag || m_nCurrentSelectedType == Bank_Tag)//支付宝支付
    {
        prise = atoi(ChargeTypeMoney1[index]);
        m_nPrise = prise;
    }
    if(m_nOpenType == 10)
    {
        prise = 10;
        m_nPrise = prise;
    }
    if(m_nOpenType == 2)
    {
        prise = 1;//1元购
        m_nPrise = prise;
    }
    if(m_nOpenType == 1)
    {
        prise = m_nUpgradeMoney;
        m_nPrise = m_nUpgradeMoney;
    }
    
    if(m_nCurrentSelectedType == Wechat_Tag)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("正在转向支付页面，请稍候", eMBNull);
        bWebViewCharge = true;
        wxCharge(m_nPrise);
        if(GameMainScene::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameWeChat);
        }
        else if(GameLayer::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameSuperWeChat);
        }
        else
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallWeChat);
        }
    }
    else if(m_nCurrentSelectedType == Alipay_Tag)
    {
        CMainLogic::sharedMainLogic()->ShowMessage("正在转向支付页面，请稍候", eMBNull);
        bWebViewCharge = true;
        IOSAlipay(m_nPrise);
        if(GameMainScene::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameAlipay);
        }
        else if(GameLayer::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameSuperAlipay);
        }
        else
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallAlipay);
        }
        
    }
    else if(m_nCurrentSelectedType == Bank_Tag)
    {
        //开启定时器，向服务端发送消息，请求是否充值完成
        if(GameMainScene::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameBank);
        }
        else if(GameLayer::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameSuperBank);
        }
        else
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallBank);
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        AndroidYeepay(0, prise);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        bWebViewCharge = true;
        CMainLogic::sharedMainLogic()->m_bEnterService = true;
        FMLayerWebView* pLayerWebView = FMLayerWebView::create(0, CMainLogic::sharedMainLogic()->m_nUserID, CMainLogic::sharedMainLogic()->m_sAccount.c_str(), prise);
        pLayerWebView -> setPosition(CCPointZero);
        this -> addChild(pLayerWebView, 300);
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
#endif
    }
}

void ChargeLayer::selectedChargeTypeAndPrise(int type, int index)
{
    m_nCurrentIndex = index;
    m_nCurrentType = type;
    CMainLogic::sharedMainLogic()->m_nBuyGameType = 1;//购买金币
    //一旦点击了卡片，那么就要判断在没在游戏中，如果在的话，就要先退出游戏
    if(GameMainScene::_instance != NULL)//在游戏中
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        CMainLogic::sharedMainLogic()->m_bGameChargeStatus = true;
        GameMainScene::_instance->exitGame();//离开座位
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(GameMainScene::GetInstance()->getShowExitGame())
        {
            selectedChargeSuccess();
        }
        else
        {
            CMainLogic::sharedMainLogic()->m_bGameChargeStatus = true;
            GameMainScene::_instance->exitGame();//离开座位
        }
#endif
    }
    else if(GameLayer::_instance != NULL)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        CMainLogic::sharedMainLogic()->m_bGameChargeStatus = true;
        GameLayer::_instance->exitGame();//离开座位
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(GameLayer::_instance->getShowExitGame())
        {
            selectedChargeSuccess();
        }
        else
        {
            CMainLogic::sharedMainLogic()->m_bGameChargeStatus = true;
            GameLayer::_instance->exitGame();//离开座位
        }
#endif
    }
    else
    {
        selectedChargeSuccess();
    }
}

void ChargeLayer::onHttpIOSMessageComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK);
}

void ChargeLayer::IOSAlipay(int prise)
{
    CMainLogic::sharedMainLogic()->m_bFirstChargeUIStatus = false;
    int bFirstCharge = 0;
    if(m_nOpenType == 10)
    {
        bFirstCharge = 1;
    }
    if(m_nOpenType == 2)
    {
        bFirstCharge = 3;
    }
    int nVipUpgrade = 0;
    if(m_nOpenType == 1)
    {
        nVipUpgrade = 1;
    }
    
    char tempOrder[32];
    sprintf(tempOrder, "alipay%ld%d", time(0), CMainLogic::sharedMainLogic()->m_nUserID);
    char temp[128];
    sprintf(temp, "uid=%d&tfee=%d&otn=%s&time=%ld&firstCharge=%d&vup=%d",CMainLogic::sharedMainLogic()->m_nUserID,prise,tempOrder,time(0),bFirstCharge, nVipUpgrade);
    char szMD5Result[128];
    CEncryptMD5::toString32(temp,szMD5Result);
    char tempData[256];
    int nType = 1;
    if(GameMainScene::_instance != NULL)
    {
        nType = 2;
    }
    else if(GameLayer::_instance != NULL)
    {
        nType = 3;
    }
    else
    {
        nType = 1;
    }
    sprintf(tempData, "%s&info=%s&ctype=%d", temp, szMD5Result, nType);
    
    m_sOrderID = tempOrder;
    m_nPrise = prise;
    CCHttpRequest* request = new CCHttpRequest();
    char chargeURL[64];
    if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
    {
        sprintf(chargeURL, "http://lelepay.66y.com/alipay20151028/alipayOrder.aspx");
    }
    else
    {
        sprintf(chargeURL, "%s/alipay20151028/alipayOrder.aspx", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str());
    }
    request -> setUrl(chargeURL);
    request -> setRequestType(CCHttpRequest::kHttpPost);
    request -> setResponseCallback(this, httpresponse_selector(ChargeLayer::onHttpAlipayComplete));
    request -> setRequestData(tempData, (int)strlen(tempData));
    CCHttpClient::getInstance()->send(request);
    CC_SAFE_RELEASE(request);
    CMainLogic::sharedMainLogic()->m_bChargeState = true;
    request = NULL;
}

void ChargeLayer::onHttpAlipayComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->HttpErrorHandle(response, eAlipayOrderErr);
        return;
    }
    //开启定时器，向服务端发送消息，请求是否充值完成
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    
    if(!strcmp(str.c_str(), "success"))
    {
        bWebViewCharge = false;
		CMainLogic::sharedMainLogic()->m_bChargeState = true;
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeOrderSuccess);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChargeSuccess = 5;//支付宝充值
        CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
        ChargeWebView::alipayCharge(m_sOrderID.c_str(), m_nPrise, 1);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        AndroidAlipay(m_nPrise);
#endif
    }
    else
    {
        bWebViewCharge = false;
        CMainLogic::sharedMainLogic()->m_bChargeState = false;
        CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK, eChargeBackHall);
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeOrderFail);
    }
}

void ChargeLayer::AndroidAlipay(int prise)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo info;
    bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if(bHave)
    {
        jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
        bool bHave2 = false;
        bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","pay", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
        if(bHave2)
        {
            char subjectTemp[64];
            sprintf(subjectTemp, "金币");
            char bodyTemp[128];
            sprintf(bodyTemp, "商品详细描述");
            char priseTemp[16];
            sprintf(priseTemp, "%d", prise);
            jstring subject = info.env->NewStringUTF(subjectTemp);
            jstring body = info.env->NewStringUTF(bodyTemp);
            jstring prise = info.env->NewStringUTF(priseTemp);
            jstring orderID = info.env->NewStringUTF(m_sOrderID.c_str());
			jint type = 1;
            info.env->CallVoidMethod(jobj, info.methodID, subject, body, prise, orderID, type);
        }
    }
#endif
}

void ChargeLayer::wxCharge(int prise)//@@金币充值
{
    CMainLogic::sharedMainLogic()->m_bFirstChargeUIStatus = false;
    int bFirstCharge = 0;
    if(m_nOpenType == 10)//10元限购
    {
        bFirstCharge = 1;
    }
    if(m_nOpenType == 2)//1元购
    {
        bFirstCharge = 3;
    }
    int nVipUpgrade = 0;
    if(m_nOpenType == 1)
    {
        nVipUpgrade = 1;
    }
    int nType = 1;//判断在哪充值
    if(GameMainScene::_instance != NULL)
    {
        nType = 2;
    }
    else if(GameLayer::_instance != NULL)
    {
        nType = 3;
    }
    else
    {
        nType = 1;
    }
    char szMD5Result[128];
    char chargeURL[256];
    char temp1[128];
    char tempPrise[16];
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
    sprintf(temp1, "%d%d%ld%d%d%s%ddakasdk", CMainLogic::sharedMainLogic()->m_nUserID, prise, time(0), bFirstCharge, nVipUpgrade, CHANNEL_NUM, nType);
    CEncryptMD5::toString32(temp1,szMD5Result);
    sprintf(chargeURL, "http://dakasdkpay.4699.com/AnySdk/OrderAll.aspx?uid=%d&tfee=%d&time=%ld&firstCharge=%d&vup=%d&channelid=%s&ctype=%d&sign=%s", CMainLogic::sharedMainLogic()->m_nUserID, prise, time(0), bFirstCharge, nVipUpgrade, CHANNEL_NUM, nType, szMD5Result);
#else
    const char* payName = CMainLogic::sharedMainLogic()->m_sAccount.c_str();
    sprintf(tempPrise, "%d", prise*100);
    sprintf(temp1, "payUid=%d&payName=%s&payMoney=%s&time=%ld&firstCharge=%d&vup=%d", CMainLogic::sharedMainLogic()->m_nUserID, payName, tempPrise, time(0), bFirstCharge, nVipUpgrade);
    CEncryptMD5::toString32(temp1,szMD5Result);
    if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
    {
        sprintf(chargeURL, "http://lelepay.66y.com/Mp_Pay/wxlele2/WXPayOrder.aspx?%s&info=%s&ctype=%d", temp1, szMD5Result, nType);
    }
    else
    {
        sprintf(chargeURL, "%s/Mp_Pay/wxlele2/WXPayOrder.aspx?%s&info=%s&ctype=%d", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str(), temp1, szMD5Result, nType);
    }
#endif
    CCHttpRequest* request = new CCHttpRequest();
    request -> setUrl(chargeURL);
    request -> setRequestType(CCHttpRequest::kHttpPost);
    request -> setTag("http test");
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
    request -> setResponseCallback(this, httpresponse_selector(ChargeLayer::onHttpAnySDKPayComplete));
#else
    request -> setResponseCallback(this, httpresponse_selector(ChargeLayer::onHttpSharedComplete));
#endif
    CCHttpClient::getInstance()->send(request);
    CC_SAFE_RELEASE(request);
    request = NULL;
}

void ChargeLayer::onHttpAnySDKPayComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->HttpErrorHandle(response, eWXOrderErr);
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    
    std::string status = "";
    TiXmlDocument* document = new TiXmlDocument();
    int len = strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        std::string nodename=rootElement->Value();
        if (0!=nodename.compare("orderinfo"))
        {
            return;
        }
        status=rootElement->Attribute("status");
        if (status != "")
        {
            TiXmlElement* pCurrentElement=rootElement->FirstChildElement("info");
            while(pCurrentElement != NULL)
            {
                if (status == "1")
                {
                    std::string userid=pCurrentElement->Attribute("userid");
                    std::string orderid=pCurrentElement->Attribute("orderid");
                    std::string sTfee=pCurrentElement->Attribute("tfee");
                    if (userid != "" && orderid != "" && sTfee != "")
                    {
                        CMainLogic::sharedMainLogic()->m_bChargeState = true;
#if defined(ANYSDK_QH360_000023) || defined(ANYSDK_BD_110000) || defined(ANYSDK_YYB_000550)
                        PluginChannel::getInstance()->pay(atoi(sTfee.c_str()), orderid);
#endif
                    }
                }
                else if(status == "0")
                {
                    std::string sError=pCurrentElement->Attribute("error");
                    if (sError != "")
                    {
                        CMainLogic::sharedMainLogic()->ShowMessage(sError.c_str());
                    }
                }
                pCurrentElement=pCurrentElement->NextSiblingElement();
            }
        }
    }
    delete document;//调起微信支付
}

void ChargeLayer::onHttpSharedComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->HttpErrorHandle(response, eWXOrderErr);
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    
    bool bSuccess = false;
    //调起微信支付
    std::string appid = "";
    std::string partnerId = "";
    std::string prepayId = "";
    std::string nonceStr = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    int timeStamp = 0;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string timeStamp = "";
#endif
    std::string package = "";
    std::string sign = "";
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if (NULL!=rootElement)
        {
            std::string nodename=rootElement->Value();
            if (0!=nodename.compare("xml"))
            {
                return;
            }
            
            TiXmlElement* firstElement = rootElement->FirstChildElement();
            while (NULL != firstElement) {
                if(!strcmp(firstElement->Value(), "appid"))
                {
                    appid = firstElement->GetText();
                }
                if(!strcmp(firstElement->Value(), "partnerId"))
                {
                    partnerId = firstElement->GetText();
                }
                if(!strcmp(firstElement->Value(), "prepayId"))
                {
                    prepayId = firstElement->GetText();
                }
                if(!strcmp(firstElement->Value(), "nonceStr"))
                {
                    nonceStr = firstElement->GetText();
                }
                if(!strcmp(firstElement->Value(), "timeStamp"))
                {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
					timeStamp = atoi(firstElement->GetText());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
					timeStamp = firstElement->GetText();
#endif
                }
                if(!strcmp(firstElement->Value(), "package"))
                {
                    package = firstElement->GetText();
                }
                if(!strcmp(firstElement->Value(), "sign"))
                {
                    sign = firstElement->GetText();
                }
                else if(!strcmp(firstElement->Value(), "out_trade_no"))
                {
                    bSuccess = true;
                    m_sWXOrderID = firstElement->GetText();
                    CMainLogic::sharedMainLogic()->m_sWXOrderID = m_sWXOrderID;
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeOrderSuccess);
                }
                firstElement = firstElement -> NextSiblingElement();
            }
        }
    }
    else
    {
        bWebViewCharge = false;
        CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK);
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeOrderFail);
    }
    delete document;
	
    if(!bSuccess)
    {
        bWebViewCharge = false;
        CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK);
        return;
    }
    CMainLogic::sharedMainLogic()->m_bChargeState = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    WeiXinCharge = 1;
    ChargeSuccess = 5;
    bWebViewCharge = false;
    CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
	ChargeWebView::wxCharge(appid, partnerId, prepayId, nonceStr, timeStamp, package, sign);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo info;
	bool bHave = JniHelper::getStaticMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp", "getInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if(bHave)
	{
		jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID);
		bool bHave2 = false;
		bHave2 = JniHelper::getMethodInfo(info, "cn/sharesdk/hellocpp/HelloCpp","wxCharge", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
		if(bHave2)
		{
			jstring appidjStr = info.env->NewStringUTF(appid.c_str());
			jstring partnerIdjStr = info.env->NewStringUTF(partnerId.c_str());
			jstring prepayIdjStr = info.env->NewStringUTF(prepayId.c_str());
			jstring pakcagejStr = info.env->NewStringUTF(package.c_str());
			jstring nonceStrjStr = info.env->NewStringUTF(nonceStr.c_str());
			jstring timeStampjStr = info.env->NewStringUTF(timeStamp.c_str());
			jstring signjStr = info.env->NewStringUTF(sign.c_str());
			jint wxCharge = 1;
			info.env->CallVoidMethod(jobj, info.methodID, appidjStr, partnerIdjStr, prepayIdjStr, pakcagejStr, nonceStrjStr, timeStampjStr, signjStr, wxCharge);
			nWxChargeResult = 1;
		}
	}

#endif
}

void ChargeLayer::AndroidYeepay(int YeepayType, int prise)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	unsigned int userID = CMainLogic::sharedMainLogic()->m_nUserID;
	const char* account = CMainLogic::sharedMainLogic()->m_sAccount.c_str();
	char tempURL[256];
    int bFirstCharge = 0;
    if(m_nOpenType == 10)
    {
        bFirstCharge = 1;
    }
    int nVipUpgrade = 0;
    if(m_nOpenType == 1)
    {
        nVipUpgrade = 1;
    }
    char temp3[128];
    sprintf(temp3, "uid=%d&acc=%s&mon=%d&time=%lu&firstCharge=%d&vup=%d", userID, account, prise, time(0), bFirstCharge, nVipUpgrade);
    char szMD5Result[128];
    CEncryptMD5::toString32(temp3,szMD5Result);
	if(YeepayType == 0)
	{
        if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
        {
            sprintf(tempURL, "http://lelepay.66y.com/Mp_Pay/yeepay/OneClickPayment.aspx?%s&md=%s", temp3, szMD5Result);
        }
        else
        {
            sprintf(tempURL, "%s/Mp_Pay/yeepay/OneClickPayment.aspx?%s&md=%s", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str(), temp3, szMD5Result);
        }
		
	}
	else if(YeepayType == 1)
	{
        if(CMainLogic::sharedMainLogic()->m_sChargeCardURL == "")
        {
            char temp3[128];
            sprintf(temp3, "uid=%d&acc=%s&mon=%d&time=%lu&firstCharge=0", userID, account, prise, time(0));
            CEncryptMD5::toString32(temp3,szMD5Result);
            sprintf(tempURL, "http://lelepay.66y.com/Mp_Pay/yeeka/Pay_gcard.aspx?%s&md=%s", temp3, szMD5Result);
        }
        else
        {
            char temp3[128];
            sprintf(temp3, "uid=%d&acc=%s&mon=%d&time=%lu&firstCharge=0", userID, account, prise, time(0));
            CEncryptMD5::toString32(temp3,szMD5Result);
            sprintf(tempURL, "%s/Mp_Pay/yeeka/Pay_gcard.aspx?%s&md=%s", CMainLogic::sharedMainLogic()->m_sChargeCardURL.c_str(), temp3, szMD5Result);
        }
		
	}
	else if(YeepayType == 2)
	{
        if(CMainLogic::sharedMainLogic()->m_sChargeCardURL == "")
        {
            char temp3[128];
            sprintf(temp3, "uid=%d&acc=%s&mon=%d&time=%lu&firstCharge=0", userID, account, prise, time(0));
            CEncryptMD5::toString32(temp3,szMD5Result);
            sprintf(tempURL, "http://lelepay.66y.com/Mp_Pay/yeeka/Pay_mob.aspx?%s&md=%s", temp3, szMD5Result);
        }
        else
        {
            char temp3[128];
            sprintf(temp3, "uid=%d&acc=%s&mon=%d&time=%lu&firstCharge=0", userID, account, prise, time(0));
            CEncryptMD5::toString32(temp3,szMD5Result);
            sprintf(tempURL, "%s/Mp_Pay/yeeka/Pay_mob.aspx?%s&md=%s", CMainLogic::sharedMainLogic()->m_sChargeCardURL.c_str(), temp3, szMD5Result);
        }
		
	}

	JniMethodInfo minfo;  
	jobject jobj;  
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "cn/sharesdk/hellocpp/HelloCpp","getInstance","()Ljava/lang/Object;");
	if (isHave)  
	{  
		//调用Java静态函数，取得对象。  
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);  
		if (jobj != NULL)  
		{
			isHave = JniHelper::getMethodInfo(minfo,"cn/sharesdk/hellocpp/HelloCpp","openWebView","(Ljava/lang/String;)V");  
			if (isHave)  
			{  
				jstring urlStr = minfo.env->NewStringUTF(tempURL);
				//调用java非静态函数, 参数1：Java对象，上面已经取得   参数2：方法ID  
				minfo.env->CallVoidMethod(jobj, minfo.methodID, urlStr);  
			}  
		}  
	}  
#endif
}

void ChargeLayer::showChargeUIParticle(CCSprite* sprite)
{
    if(sprite != NULL)
    {
        CCSprite* firstSp = CCSprite::create();
        firstSp -> setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height/2+10));
        sprite -> addChild(firstSp);
        CCArray* firstChargeArr = CCArray::create();
        char temp[32];
        for(int i = 1;i <= 13; i++)
        {
            sprintf(temp, "hallfirstcharge%d.png", i);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(temp);
            firstChargeArr -> addObject(frame);
        }
        CCAnimation* firstChargeAnimation = CCAnimation::createWithSpriteFrames(firstChargeArr, 0.11f);
        CCAnimate* firstChargeAnimate = CCAnimate::create(firstChargeAnimation);
        firstSp -> runAction(CCRepeatForever::create(firstChargeAnimate));
    }
}

void ChargeLayer::load2PublicChargeType()
{
    m_nodeSpDi = CCSprite::create();
    m_nodeSpDi->setPosition(CCPointZero);
    this -> addChild(m_nodeSpDi);
    m_nodeChargeNum = CCNode::create();
    m_nodeSpDi -> addChild(m_nodeChargeNum, 10);
    
    for(int i = 0; i < 4; i++)
    {
        CCSprite* pSpButton = CCSprite::create("charge/chargeCardBg.png");
        pSpButton -> setPosition(ccp(i%4*260+120+pSpButton->getContentSize().width/2,440-i/4*(pSpButton->getContentSize().height)+40));
        m_nodeChargeNum -> addChild(pSpButton);
        CCSprite* pButtonDi = CCSprite::create("charge/chargebutton.png");
        pButtonDi -> setPosition(ccp(pSpButton->getContentSize().width/2, pButtonDi->getContentSize().height/2-6));
        pSpButton -> addChild(pButtonDi);
        CCLabelAtlas* pLabelYuan = CCLabelAtlas::create(ChargeTypeMoney1[i], "charge/chargenum1.png", 25, 35, '0');
        pLabelYuan -> setAnchorPoint(ccp(0.5, 0.5f));
        pLabelYuan -> setPosition(ccp(pSpButton->getContentSize().width/2-20, 16));
        pSpButton -> addChild(pLabelYuan);
        CCSprite* pSpYuan = CCSprite::create("charge/chargeyuan.png");
        pSpYuan -> setPosition(ccp(pLabelYuan->getContentSize().width+18, pLabelYuan->getContentSize().height/2));
        pLabelYuan -> addChild(pSpYuan);
        pSpButton -> setTag(ChargeNumTag[i]);
        m_arrChargeNum -> addObject(pSpButton);
        CCSprite* pSpJinbi = CCSprite::create(ChargeTypeUI[i]);
        pSpJinbi -> setPosition(ccp(pSpButton->getContentSize().width/2, pSpButton->getContentSize().height/3*2-40));
        pSpButton -> addChild(pSpJinbi);
        if(i > 0)
        {
            CCSprite* pJinbiBg = CCSprite::create("charge/chargejinbibg.png");
            pJinbiBg -> setPosition(ccp(pSpButton->getContentSize().width/2-4, 80));
            pSpButton -> addChild(pJinbiBg);
            CCSprite* pJinbiInfo = CCSprite::create(ChargeJinbiInfo[i-1]);
            pJinbiInfo -> setPosition(ccp(pJinbiBg->getContentSize().width/2, pJinbiBg->getContentSize().height/2+10));
            pJinbiBg -> addChild(pJinbiInfo);
        }
        
        CCLabelTTF* pLabelInfo = CCLabelTTF::create(ChargeTypeInfo[i], "Arial-BoldMT", 26);
        pLabelInfo -> setPosition(ccp(pSpButton->getContentSize().width/2, pSpButton->getContentSize().height-25));
        pSpButton -> addChild(pLabelInfo);
        pLabelInfo->setColor(ccc3(159, 81, 48));
        if(i == 0 || i == 2)
        {
            CCSprite* pRemai = CCSprite::create("charge/chargeremai.png");
            pRemai -> setPosition(ccp(20, pSpButton->getContentSize().height/4*3-10));
            pSpButton -> addChild(pRemai);
            showChargeUIParticle(pRemai);
        }
        
        
        
    }
    for(int i = 4; i < 8; i++)
    {
        CCSprite* pSpButton = CCSprite::create("charge/chargeCardBg.png");
        pSpButton -> setPosition(ccp((7-i)*260+120+pSpButton->getContentSize().width/2,440-i/4*(pSpButton->getContentSize().height-52)-28));
        m_nodeChargeNum -> addChild(pSpButton);
        CCSprite* pButtonDi = CCSprite::create("charge/chargebutton.png");
        pButtonDi -> setPosition(ccp(pSpButton->getContentSize().width/2, pButtonDi->getContentSize().height/2-6));
        pSpButton -> addChild(pButtonDi);
        CCLabelAtlas* pLabelYuan = CCLabelAtlas::create(ChargeTypeMoney1[i], "charge/chargenum1.png", 25, 35, '0');
        pLabelYuan -> setAnchorPoint(ccp(0.5, 0.5f));
        pLabelYuan -> setPosition(ccp(pSpButton->getContentSize().width/2-20, 16));
        pSpButton -> addChild(pLabelYuan);
        CCSprite* pSpYuan = CCSprite::create("charge/chargeyuan.png");
        pSpYuan -> setPosition(ccp(pLabelYuan->getContentSize().width+18, pLabelYuan->getContentSize().height/2));
        pLabelYuan -> addChild(pSpYuan);
        pSpButton -> setTag(ChargeNumTag[i]);
        m_arrChargeNum -> addObject(pSpButton);
        CCSprite* pSpJinbi = CCSprite::create(ChargeTypeUI[i]);
        pSpJinbi -> setPosition(ccp(pSpButton->getContentSize().width/2, pSpButton->getContentSize().height/3*2-40));
        pSpButton -> addChild(pSpJinbi);
        CCSprite* pJinbiBg = CCSprite::create("charge/chargejinbibg.png");
        pJinbiBg -> setPosition(ccp(pSpButton->getContentSize().width/2-4, 80));
        pSpButton -> addChild(pJinbiBg);
        CCSprite* pJinbiInfo = CCSprite::create(ChargeJinbiInfo[i-1]);
        pJinbiInfo -> setPosition(ccp(pJinbiBg->getContentSize().width/2, pJinbiBg->getContentSize().height/2+10));
        pJinbiBg -> addChild(pJinbiInfo);
        
        CCLabelTTF* pLabelInfo = CCLabelTTF::create(ChargeTypeInfo[i], "Arial-BoldMT", 26);
        pLabelInfo -> setPosition(ccp(pSpButton->getContentSize().width/2, pSpButton->getContentSize().height-25));
        pSpButton -> addChild(pLabelInfo);
        pLabelInfo->setColor(ccc3(159, 81, 48));
        
        if(i == 7 || i == 6)
        {
            CCSprite* pTejia = CCSprite::create("charge/chargetejia.png");
            pTejia -> setPosition(ccp(20, pSpButton->getContentSize().height/4*3-10));
            pSpButton -> addChild(pTejia);
            showChargeUIParticle(pTejia);
        }
        if(i == 5)
        {
            CCSprite* pRemai = CCSprite::create("charge/chargeremai.png");
            pRemai -> setPosition(ccp(20, pSpButton->getContentSize().height/4*3-10));
            pSpButton -> addChild(pRemai);
            showChargeUIParticle(pRemai);
        }
        
    }
}

void ChargeLayer::clickVIPMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case 101://VIP特权， 跳到VIP界面
        {
            CMainLogic::sharedMainLogic()->m_bChargeState = false;
            CMainLogic::sharedMainLogic()->m_bGameToVIP = false;
            CMainLogic::sharedMainLogic()->m_bBYGameToSuper = false;
            CMainLogic::sharedMainLogic()->m_nExitGameToSpecialArm = 0;
            if(GameMainScene::_instance != NULL)
            {
                GameMainScene::GetInstance()->setWndHave(false);
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameChargeToVIP);
                GameMainScene::GetInstance()->openVIP();
            }
            else if(GameLayer::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHaidaoChargeTag);
                GameLayer::_instance->openVIP();
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeToVIP);
            }
            this->removeFromParent();
            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    CMainLogic::sharedMainLogic()->ShowMessage("正在打开VIP，请稍候...", eMBNull);
                    pHallLayer->sendVIPVersion("VIP");
                }
            }
            break;
        }
        case 103://点击首充按钮
        {
            FirstChargeLayer* pFirstChargeLayer = FirstChargeLayer::create();
            this->addChild(pFirstChargeLayer, 200);
            if(GameMainScene::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameFirstCharge);
            }
            else if(GameLayer::_instance != NULL)
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHaidaoFirstChargeTag);
            }
            else
            {
                CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallFirstCharge);
            }
            break;
        }
        case 104://福利活动
        {
            CMainLogic::sharedMainLogic()->m_bEnterService = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            FMLayerWebView* pFMLayerWebView = FMLayerWebView::create(0, 2);
            pFMLayerWebView -> setPosition(CCPointZero);
            this -> addChild(pFMLayerWebView, 300);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            CMainLogic::sharedMainLogic()->openChargeInfo();
#endif
            break;
        }
        case 105:
        {
            CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eChargeOneMoneyTag);
            OneMoneyLayer* pOneMoneyLayer = OneMoneyLayer::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(pOneMoneyLayer, 12, 20004);
        }
        default:
            break;
    }
}

//同步通知IOS支付宝充值完成或者取消
void ChargeLayer::notifyIOSAlipayCompleted()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(ChargeSuccess == 1)//苹果支付宝完成
    {
        m_sAlipaySign = AlipayChargeSuccess;
		CMainLogic::sharedMainLogic()->m_sAlipaySign = m_sAlipaySign;
        sendAlipayChargeSuccess();
    }
    else if(ChargeSuccess == 2)//苹果支付宝充值取消或者失败
    {
        bWebViewCharge = false;
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeCancel);
    }
#endif
}

//同步通知IOS微信支付完成或者取消
void ChargeLayer::notifyIOSWXCompleted()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
    if(ChargeSuccess == 3)//苹果微信支付成功
    {
        sendWXChargeSuccess();
    }
    else if(ChargeSuccess == 4)//苹果微信充值取消或者失败
    {
        bWebViewCharge = false;
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeCancel);
    }
#endif
}

//向网站发送支付宝充值成功
void ChargeLayer::sendAlipayChargeSuccess()
{
    //向网站发送支付宝签名信息
    CCHttpRequest* request = new CCHttpRequest();
    std::string sAlipay = "";
    for(int i = 0; i < CMainLogic::sharedMainLogic()->m_sAlipaySign.length(); i++)
    {
        if(!(CMainLogic::sharedMainLogic()->m_sAlipaySign[i] == '\\'))// || AlipayChargeSuccess[i] == '"'
        {
            if(CMainLogic::sharedMainLogic()->m_sAlipaySign[i] == '+')
            {
                sAlipay+="%2B";
            }
            else
            {
                sAlipay+=CMainLogic::sharedMainLogic()->m_sAlipaySign[i];
            }
        }
    }
    char chargeURL[1024];
    if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
    {
        sprintf(chargeURL, "http://lelepay.66y.com/alipay20151028/alipayReturn.aspx?%s", sAlipay.c_str());
    }
    else
    {
        sprintf(chargeURL, "%s/alipay20151028/alipayReturn.aspx?%s", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str(), sAlipay.c_str());
    }
    
    request -> setUrl(chargeURL);
    request -> setRequestType(CCHttpRequest::kHttpPost);
    request -> setResponseCallback(this, httpresponse_selector(ChargeLayer::onAlipayChargeSuccess));
    CCHttpClient::getInstance()->send(request);
    CC_SAFE_RELEASE(request);
    request = NULL;
    CMainLogic::sharedMainLogic()->ShowMessage("正在验证充值结果", eMBNull);
    bWebViewCharge = false;
}

//向网站发送微信充值成功
void ChargeLayer::sendWXChargeSuccess()
{
    CCHttpRequest* request = new CCHttpRequest();
    char chargeURL[128];
    if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
    {
        sprintf(chargeURL, "http://lelepay.66y.com/Mp_Pay/wxlele2/WXPayReturn.aspx?otn=%s",CMainLogic::sharedMainLogic()->m_sWXOrderID.c_str());
    }
    else
    {
        sprintf(chargeURL, "%s/Mp_Pay/wxlele2/WXPayReturn.aspx?otn=%s", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str(), CMainLogic::sharedMainLogic()->m_sWXOrderID.c_str());
    }
    request -> setUrl(chargeURL);
    request -> setRequestType(CCHttpRequest::kHttpPost);
    request -> setResponseCallback(this, httpresponse_selector(ChargeLayer::onWXChargeSuccess));
    request -> setTag("WXSuccess");
    CCHttpClient::getInstance()->send(request);
    CC_SAFE_RELEASE(request);
    request = NULL;
    CMainLogic::sharedMainLogic()->ShowMessage("正在验证充值结果", eMBNull);
    bWebViewCharge = false;
}

//网站返回支付宝充值成功
void ChargeLayer::onAlipayChargeSuccess(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->HttpErrorHandle(response, eAlipaySuccessErr);
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    
    //printf("alipay111111 %s\n", str.c_str());
    
    requestMail();
    
    bool bVIPUpgrade = false;
    std::string sInfo = "";
    bool bChargeResult = false;
    
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement != NULL)
        {
            TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
            while (pCurrentElement != NULL)
            {
                std::string nodename=pCurrentElement->Value();
                if (nodename == "text")
                {
                    sInfo = pCurrentElement->GetText();
                }
                else if(nodename == "Score")
                {
                    if(CMainLogic::sharedMainLogic()->m_lUserScore != atoll(pCurrentElement->GetText()))
                    {
                        bChargeResult = true;
                    }
                    //更新当前分数
                    CMainLogic::sharedMainLogic()->m_lUserScore = atoll(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setUserScore(CMainLogic::sharedMainLogic()->m_lUserScore);
                        }
                    }
                }
                else if(nodename == "VipLevel")//更新当前VIP等级
                {
                    int nLevel = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_nVipLevel != nLevel)
                    {
                        if(CMainLogic::sharedMainLogic()->m_nVipLevel == 0)//以前VIP等级为0的话，那么现在肯定升级了VIP，所以可以VIP签到
                        {
                            CMainLogic::sharedMainLogic()->m_bReceiveVipLogon = false;
                            //将大厅内的免费金币按钮上的心形展示出来
                            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                            {
                                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                                if(pHallLayer != NULL)
                                {
                                    pHallLayer -> setNickNameColor();
                                    pHallLayer->showfreecoinAction(true);
                                    if(nLevel == 9)
                                    {
                                        pHallLayer->m_pVipMenu->setVisible(false);
                                    }
                                }
                            }
                        }
                        CMainLogic::sharedMainLogic()->m_nVipLevel = nLevel;
                        bVIPUpgrade = true;
                    }
                    CMainLogic::sharedMainLogic()->m_nVipLevel = nLevel;
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setVipExp();
                        }
                    }
                }
                else if(nodename == "VipExp")//更新当前经验值
                {
                    CMainLogic::sharedMainLogic()->m_nVipExp = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setVipExp();
                        }
                    }
                }
                else if(nodename == "UserLet")//更新当前奖券
                {
                    CMainLogic::sharedMainLogic()->m_nLottery = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setTicket(CMainLogic::sharedMainLogic()->m_nLottery);
                        }
                    }
                }
                else if(nodename == "ChargeBack")//更新充值返利
                {
                    CMainLogic::sharedMainLogic()->m_nChargeBack = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_nChargeBack!=0)
                    {
                        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                pHallLayer->showVIPAction(true);
                            }
                        }
                    }
                }
                else if(nodename == "Fch")
                {
                    int nFirstCharge = atoi(pCurrentElement->GetText());
                    if(nFirstCharge == 1)//首充
                    {
                        CMainLogic::sharedMainLogic()->ShowMessage("恭喜您首充成功，获取了首充大礼包", eMBOK);
                        CMainLogic::sharedMainLogic()->m_cbCountTimes = 1;
                        CMainLogic::sharedMainLogic()->m_bFirstChargeCanGet = true;
                        //将首充按钮隐藏,显示VIP按钮
                        m_firstChargeMenu->setVisible(false);
                        CCArray* array = m_BackMenu->getChildren();
                        for(int i = 0; i < array->count(); i++)
                        {
                            CCMenuItem* item = (CCMenuItem*)array->objectAtIndex(i);
                            item -> setVisible(true);
                        }
                        //更改1元购位置
                        CCArray* array2 = m_oneChargeMenu->getChildren();
                        for(int i = 0; i < array2->count(); i++)
                        {
                            CCMenuItem* item = (CCMenuItem*)array->objectAtIndex(i);
                            item -> setPosition(ccp(960-114, 670));
                        }
                        //改变首充状态
                        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                pHallLayer->showLastTimes(1);
                            }
                        }
                    }
                    else if(nFirstCharge == 2)//1元购
                    {
                        CMainLogic::sharedMainLogic()->m_bCanOneCharge = false;
                        m_oneChargeMenu -> setVisible(false);
                        //改变1元购状态
                        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                pHallLayer->hideOneChargeMenu();
                            }
                        }
                    }
                }
                else if(nodename == "Count")//更新当前奖券数量
                {
                    CMainLogic::sharedMainLogic()->m_nLottery = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setTicket(CMainLogic::sharedMainLogic()->m_nLottery);
                        }
                    }
                }
                pCurrentElement = pCurrentElement->NextSiblingElement();
            }
        }
    }
    delete document;
    
    if(bChargeResult)
    {
        if(!bVIPUpgrade)
        {
            if(CMainLogic::sharedMainLogic()->m_bCanBandingTel)
            {
                if(CCUserDefault::sharedUserDefault()->getStringForKey("BindingTel", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"))
                {
                    CCUserDefault::sharedUserDefault()->setStringForKey("BindingTel", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"));
                    CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK, eShowBindingTel);
                }
                else
                {
                    CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK);
                }
            }
            else
            {
                CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK);
            }
        }
        else
        {
            CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK, eShowVipUpgrade);
        }
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeSuccess);
        //播放特效，判断登录奖励是否领取，如果已经领取了，那么2s后就将此框删除
        SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
        {
            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
            if(pHallLayer != NULL)
            {
                pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
            }
        }
    }
    else
    {
        CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeFail);
    }
}

//网站返回微信充值成功
void ChargeLayer::onWXChargeSuccess(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->HttpErrorHandle(response, eWXChargeSuccessErr);
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    
    //printf("alipay %s\n", str.c_str());
    
    requestMail();
    
    bool bVIPUpgrade = false;
    std::string sInfo = "";
    bool bChargeResult = false;
    
    TiXmlDocument* document = new TiXmlDocument();
    int len = (int)strlen(str.c_str());
    if(document->LoadFile2((unsigned char *)str.c_str(), len))
    {
        TiXmlElement* rootElement = document->RootElement();
        if(rootElement != NULL)
        {
            TiXmlElement* pCurrentElement = rootElement->FirstChildElement();
            while (pCurrentElement != NULL)
            {
                std::string nodename=pCurrentElement->Value();
                if (nodename == "text")
                {
                    sInfo = pCurrentElement->GetText();
                }
                else if(nodename == "Score")
                {
                    if(CMainLogic::sharedMainLogic()->m_lUserScore != atoll(pCurrentElement->GetText()))
                    {
                        bChargeResult = true;
                    }
                    //更新当前分数
                    CMainLogic::sharedMainLogic()->m_lUserScore = atoll(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setUserScore(CMainLogic::sharedMainLogic()->m_lUserScore);
                        }
                    }
                }
                else if(nodename == "VipLevel")//更新当前VIP等级
                {
                    int nLevel = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_nVipLevel != nLevel)
                    {
                        if(CMainLogic::sharedMainLogic()->m_nVipLevel == 0)//以前VIP等级为0的话，那么现在肯定升级了VIP，所以可以VIP签到
                        {
                            CMainLogic::sharedMainLogic()->m_bReceiveVipLogon = false;
                            //将大厅内的免费金币按钮上的心形展示出来
                            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                            {
                                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                                if(pHallLayer != NULL)
                                {
                                    pHallLayer -> setNickNameColor();
                                    pHallLayer->showfreecoinAction(true);
                                    if(nLevel == 9)
                                    {
                                        pHallLayer->m_pVipMenu->setVisible(false);
                                    }
                                }
                            }
                        }
                        CMainLogic::sharedMainLogic()->m_nVipLevel = nLevel;
                        bVIPUpgrade = true;
                    }
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setVipExp();
                        }
                    }
                }
                else if(nodename == "VipExp")//更新当前经验值
                {
                    CMainLogic::sharedMainLogic()->m_nVipExp = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setVipExp();
                        }
                    }
                }
                else if(nodename == "UserLet")//更新当前奖券
                {
                    CMainLogic::sharedMainLogic()->m_nLottery = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setTicket(CMainLogic::sharedMainLogic()->m_nLottery);
                        }
                    }
                }
                else if(nodename == "ChargeBack")//更新充值返利
                {
                    CMainLogic::sharedMainLogic()->m_nChargeBack = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_nChargeBack!=0)
                    {
                        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                pHallLayer->showVIPAction(true);
                            }
                        }
                    }
                }
                else if(nodename == "Fch")
                {
                    int nFirstCharge = atoi(pCurrentElement->GetText());
                    if(nFirstCharge == 1)//首充
                    {
                        CMainLogic::sharedMainLogic()->ShowMessage("恭喜您首充成功，获取了首充大礼包", eMBOK);
                        CMainLogic::sharedMainLogic()->m_cbCountTimes = 1;
                        CMainLogic::sharedMainLogic()->m_bFirstChargeCanGet = true;
                        //将首充按钮隐藏,显示VIP按钮
                        m_firstChargeMenu->setVisible(false);
                        CCArray* array = m_BackMenu->getChildren();
                        for(int i = 0; i < array->count(); i++)
                        {
                            CCMenuItem* item = (CCMenuItem*)array->objectAtIndex(i);
                            item -> setVisible(true);
                        }
                        //更改1元购位置
                        CCArray* array2 = m_oneChargeMenu->getChildren();
                        for(int i = 0; i < array2->count(); i++)
                        {
                            CCMenuItem* item = (CCMenuItem*)array->objectAtIndex(i);
                            item -> setPosition(ccp(960-114, 670));
                        }
                        //改变首充状态
                        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                pHallLayer->showLastTimes(1);
                            }
                        }
                    }
                    else if(nFirstCharge == 2)//1元购
                    {
                        CMainLogic::sharedMainLogic()->m_bCanOneCharge = false;
                        m_oneChargeMenu -> setVisible(false);
                        //改变1元购状态
                        if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                        {
                            CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                            if(pHallLayer != NULL)
                            {
                                pHallLayer->hideOneChargeMenu();
                            }
                        }
                    }
                }
                else if(nodename == "Count")//更新当前奖券数量
                {
                    CMainLogic::sharedMainLogic()->m_nLottery = atoi(pCurrentElement->GetText());
                    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
                    {
                        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                        if(pHallLayer != NULL)
                        {
                            pHallLayer->setTicket(CMainLogic::sharedMainLogic()->m_nLottery);
                        }
                    }
                }
                pCurrentElement = pCurrentElement->NextSiblingElement();
            }
        }
    }
    delete document;
    
    if(bChargeResult)
    {
        if(GameMainScene::_instance!=NULL || GameLayer::_instance != NULL)
        {
            CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK, eExitGameToHall);
        }
        else
        {
            if(!bVIPUpgrade)
            {
                if(CMainLogic::sharedMainLogic()->m_bCanBandingTel)
                {
                    if(CCUserDefault::sharedUserDefault()->getStringForKey("BindingTel", "") != CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"))
                    {
                        CCUserDefault::sharedUserDefault()->setStringForKey("BindingTel", CMainLogic::sharedMainLogic()->getCurrentUserIDTime("BindingTel"));
                        CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK, eShowBindingTel);
                    }
                    else
                    {
                        CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK);
                    }
                }
                else
                {
                    CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK);
                }
            }
            else
            {
                CMainLogic::sharedMainLogic()->ShowMessage(sInfo.c_str(), eMBOK, eShowVipUpgrade);
            }
            //播放特效，判断登录奖励是否领取，如果已经领取了，那么2s后就将此框删除
            SimpleAudioEngine::sharedEngine()->playEffect("Music/logoneffect.mp3");
            if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
            {
                CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
                if(pHallLayer != NULL)
                {
                    pHallLayer->playParticle(CLIENT_WIDTH/2, CLIENT_HEIGHT/2);
                }
            }
        }
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeSuccess);
    }
    else
    {
        CMainLogic::sharedMainLogic()->m_pMessageBox->Update();
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallChargeFail);
    }
}

void ChargeLayer::showUpgradeVIP()
{
    SimpleAudioEngine::sharedEngine()->playEffect("Music/vipupgradeeffect.wav");
    UpgradeVIPLayer* pUpgradeVIPLayer = UpgradeVIPLayer::create();
    this -> addChild(pUpgradeVIPLayer, 200);
    float fSound = CCUserDefault::sharedUserDefault()->getFloatForKey("sound", 1);
    if(fSound > 0.4f)
    {
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.1f);
    }
}

void ChargeLayer::requestMail()
{
    if(CMainLogic::sharedMainLogic()->m_pNetworkService != NULL)
    {
        CMainLogic::sharedMainLogic()->m_nCheckSocketShut = 15;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_pNetworkService->GetNetInfo(CMainLogic::sharedMainLogic()->m_nHallSocketHandle), MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CMainLogic::sharedMainLogic()->SendData(CMainLogic::sharedMainLogic()->m_nHallSocketHandle, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL);     
#endif 
        this->scheduleOnce(schedule_selector(ChargeLayer::receiveHallLink2), 1.0f);
    }
}

void ChargeLayer::receiveHallLink2(float dt)
{
    if(CMainLogic::sharedMainLogic()->m_pNetworkService != NULL)
    {
        CMainLogic::sharedMainLogic()->m_nClickBank = 15;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CNetInfo * pNetInfo = NULL;
        if (!NATIVE_TEST)
        {
            pNetInfo = CMainLogic::sharedMainLogic()->m_pNetworkService->Connect(CMainLogic::sharedMainLogic()->m_sNodeIP.c_str(), PORT_LOGON, NULL, NULL, true);
        }
        else
        {
            pNetInfo = CMainLogic::sharedMainLogic()->m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON, NULL, NULL, false);
        }
        CMainLogic::sharedMainLogic()->m_nHallSocketHandle = CMainLogic::sharedMainLogic()->m_pNetworkService->GetNetID(pNetInfo);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		if (!NATIVE_TEST)
        {
            CMainLogic::sharedMainLogic()->m_nHallSocketHandle = CMainLogic::sharedMainLogic()->m_pNetworkService->Connect(CMainLogic::sharedMainLogic()->m_sNodeIP.c_str(), PORT_LOGON);
        }
        else
        {
            CMainLogic::sharedMainLogic()->m_nHallSocketHandle = CMainLogic::sharedMainLogic()->m_pNetworkService->Connect(PLATFORM_IP, PORT_LOGON);
        }
#endif    
	}
}

void ChargeLayer::setFirstChargeStatus()
{
    if(GameMainScene::_instance != NULL || GameLayer::_instance != NULL)
    {
        return;
    }
    if(CMainLogic::sharedMainLogic()->m_cbCountTimes != 0)
    {
        m_firstChargeMenu -> setVisible(false);
        CCArray* array = m_BackMenu->getChildren();
        for(int i = 0; i < array->count(); i++)
        {
            CCMenuItem* item = (CCMenuItem*)array->objectAtIndex(i);
            item -> setVisible(true);
        }
        //更改1元购位置
        CCArray* array2 = m_oneChargeMenu->getChildren();
        for(int i = 0; i < array2->count(); i++)
        {
            CCMenuItem* item = (CCMenuItem*)array->objectAtIndex(i);
            item -> setPosition(ccp(960-114, 670));
        }
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void ChargeLayer::IOSCharge()
{
    if(orderIDStr != "")
    {
        CMainLogic::sharedMainLogic()->ShowMessage("请等待当前订单完成", eMBOK);
        return;
    }
    if(GameMainScene::_instance != NULL || GameLayer::_instance != NULL)
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameIOSPay);
    }
    else
    {
        CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallIOSPay);
    }
    
    for(int i = 0; i < 8; i++)
    {
        //CCSprite* pSpBg = (CCSprite*)m_arrChargeNum->objectAtIndex(i);
        if(m_nArrayCurrentIndex == i)
        {
            //向后台发送未支付订单
            char tempOrder[32];
            sprintf(tempOrder, "IAP%ld%d", time(0), CMainLogic::sharedMainLogic()->m_nUserID);
            orderIDStr = tempOrder;
            char szMD5Result[128];
            char userID[128];
            sprintf(userID, "uid=%d&otn=%s&tfee=%d&time=%lu", CMainLogic::sharedMainLogic()->m_nUserID,tempOrder,m_IAPProductList.product[i].wRMB, time(0));
            CEncryptMD5::toString32(userID,szMD5Result);
            char temp[256];
            int nType = 0;
            if(GameMainScene::_instance != NULL)
            {
                nType = 2;
            }
            else if(GameLayer::_instance != NULL)
            {
                nType = 3;
            }
            else
            {
                nType = 1;
            }
            if(CMainLogic::sharedMainLogic()->m_sChargeURL == "")
            {
                sprintf(temp, "http://lelepay.66y.com/Apple/AppUrl.aspx?uid=%d&otn=%s&tfee=%d&time=%lu&info=%s&ctype=%d", CMainLogic::sharedMainLogic()->m_nUserID,tempOrder,m_IAPProductList.product[i].wRMB, time(0), szMD5Result, nType);
            }
            else
            {
                sprintf(temp, "%s/Apple/AppUrl.aspx?uid=%d&otn=%s&tfee=%d&time=%lu&info=%s&ctype=%d", CMainLogic::sharedMainLogic()->m_sChargeURL.c_str(), CMainLogic::sharedMainLogic()->m_nUserID,tempOrder,m_IAPProductList.product[i].wRMB, time(0), szMD5Result, nType);
            }
            
            chargeTypeStr = m_IAPProductList.product[i].szProductID;
            nchargeRMB = m_IAPProductList.product[i].wRMB;
            CCHttpRequest* request = new CCHttpRequest();
            request -> setUrl(temp);
            
            request -> setRequestType(CCHttpRequest::kHttpPost);
            request -> setResponseCallback(this, httpresponse_selector(ChargeLayer::onHttpIAPComplete));
            CCHttpClient::getInstance()->send(request);
            CC_SAFE_RELEASE(request);
            request = NULL;
            
            CMainLogic::sharedMainLogic()->ShowMessage("正在获取订单", eMBNull);
            
            break;
        }
    }
}

void ChargeLayer::selectedChargeTypeAndPriseIOS(int type)
{
    m_nCurrentType = type;
    //一旦点击了卡片，那么就要判断在没在游戏中，如果在的话，就要先退出游戏
    if(GameMainScene::_instance != NULL)//在游戏中
    {
        CMainLogic::sharedMainLogic()->m_nShowChargeType = 4;
        CMainLogic::sharedMainLogic()->m_bGameChargeStatus = true;
        CMainLogic::sharedMainLogic()->m_nBuyGameType = 1;//购买金币
        GameMainScene::_instance->exitGame();//离开座位
    }
    else if(GameLayer::_instance != NULL)
    {
        CMainLogic::sharedMainLogic()->m_nShowChargeType = 4;
        CMainLogic::sharedMainLogic()->m_bGameChargeStatus = true;
        CMainLogic::sharedMainLogic()->m_nBuyGameType = 1;//购买金币
        GameLayer::_instance->exitGame();//离开座位
    }
    else
    {
        IOSCharge();
    }
}

void ChargeLayer::onHttpIAPComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        CMainLogic::sharedMainLogic()->HttpErrorHandle(response, eWXOrderErr);
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string str;
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        char a = (*buffer)[i];
        str.append(1,a);
    }
    if(!strcmp(str.c_str(), "success"))
    {
        CMainLogic::sharedMainLogic()->ShowMessage("正在请求支付", eMBIAPNull);
        char tempProductID[32];
        sprintf(tempProductID, "%s", chargeTypeStr.c_str());
        bridge->requestProducts(tempProductID);
    }
    else
    {
        CMainLogic::sharedMainLogic()->ShowMessage(str.c_str(), eMBOK, eChargeBackHall);
    }
}
#endif

void ChargeLayer::sendUserBehavior(int type, int index)
{
    if(GameMainScene::_instance != NULL)
    {
        if(type == 0)//游戏内点击充值按钮进入充值界面
        {
            switch (index) {
                case 0:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameCharge1);
                    break;
                }
                case 1:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameCharge2);
                    break;
                }
                case 2:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameCharge3);
                    break;
                }
                case 3:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameCharge4);
                    break;
                }
                case 4:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameCharge5);
                    break;
                }
                case 5:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameCharge6);
                    break;
                }
                case 6:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameCharge7);
                    break;
                }
                case 7:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eGameCharge8);
                    break;
                }
                default:
                    break;
            }
            
        }
    }
    else if(GameLayer::_instance != NULL)
    {
        if(type == 0)//游戏内点击充值按钮进入充值界面
        {
            switch (index) {
                case 0:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHaidaoCharge1);
                    break;
                }
                case 1:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHaidaoCharge2);
                    break;
                }
                case 2:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHaidaoCharge3);
                    break;
                }
                case 3:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHaidaoCharge4);
                    break;
                }
                case 4:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHaidaoCharge5);
                    break;
                }
                case 5:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHaidaoCharge6);
                    break;
                }
                case 6:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHaidaoCharge7);
                    break;
                }
                case 7:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHaidaoCharge8);
                    break;
                }
                default:
                    break;
            }
        }
    }
    else
    {
        if(type == 0)
        {
            switch (index) {
                case 0:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallCharge1);
                    break;
                }
                case 1:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallCharge2);
                    break;
                }
                case 2:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallCharge3);
                    break;
                }
                case 3:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallCharge4);
                    break;
                }
                case 4:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallCharge5);
                    break;
                }
                case 5:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallCharge6);
                    break;
                }
                case 6:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallCharge7);
                    break;
                }
                case 7:
                {
                    CMainLogic::sharedMainLogic()->sendUserBehavior(CMainLogic::sharedMainLogic()->m_nUserID, eHallCharge8);
                    break;
                }
                default:
                    break;
            }
        }
    }
}
