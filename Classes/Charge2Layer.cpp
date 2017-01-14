//
//  Charge2Layer.cpp
//  CatchFish
//
//  Created by 河北腾游 on 15/4/15.
//
//

#include "header.h"
#include "IOSiAP_Bridge.h"

extern std::string orderIDStr;
extern std::string chargeTypeStr;
extern int nchargeRMB;


const int ChargeNumTag1[8] = {107, 108, 109, 110, 111, 112, 113, 114};
const char* ChargeTypeInfo1[8] = {"18000金币","120000金币","240000金币",  "620000金币", "104万金币", "204万金币", "392万金币","1352万金币"};
const char* ChargeTypeUI1[8] = {"charge/chargejinbi1.png","charge/chargejinbi2.png","charge/chargejinbi3.png", "charge/chargejinbi4.png", "charge/chargejinbi5.png", "charge/chargejinbi6.png",  "charge/chargejinbi7.png","charge/chargejinbi8.png"};
const char* IOSChargeZengCoin[6] = { "1", "3",  "8", "15", "38", "64"};
const char* ChargeTypeMoney2[8] = {"1", "6", "12", "30", "50", "98", "188", "648"};
const char* ChargeTypeCount2[8] = {"12", "24", "60", "100", "196", "376", "776", "1296"};
const char* ChargeJinbiInfo2[7] = {"charge/chargejinbiinfo1.png", "charge/chargejinbiinfo2.png", "charge/chargejinbiinfo3.png", "charge/chargejinbiinfo4.png", "charge/chargejinbiinfo5.png", "charge/chargejinbiinfo6.png", "charge/chargejinbiinfo7.png"};

Charge2Layer::~Charge2Layer()
{
    m_arrChargeNum->release();
    delete bridge;
    bridge = NULL;
}

Charge2Layer* Charge2Layer::create(CMD_GPR_IAPProductList* IAPProductList, int nOpenType)
{
    Charge2Layer* pRet = new Charge2Layer();
    if(pRet && pRet->init(IAPProductList, nOpenType))
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

bool Charge2Layer::init(CMD_GPR_IAPProductList* IAPProductList, int nOpenType)
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("h3all/quickstart.plist");
    
    m_arrChargeNum = CCArray::create();
    m_arrChargeNum->retain();
    m_nOpenType = nOpenType;
    m_nCannonStatus = 0;
    
    memcpy(&m_IAPProductList, IAPProductList, sizeof(CMD_GPR_IAPProductList));
    bridge = new IOSiAP_Bridge(m_IAPProductList);
   
    loadUI();
    loadChargeMoneyUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void Charge2Layer::loadUI()
{
    CCSprite* pChargeBg = CCSprite::create("barrelUI/lotterybg.png");
    pChargeBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(pChargeBg);
    pChargeBg -> setScale(2.0f);
    CCSprite* pChargeTopBg = CCSprite::create("charge/chargetopbg.png");
    pChargeTopBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT-pChargeTopBg->getContentSize().height/2));
    this -> addChild(pChargeTopBg);
    pChargeTopBg -> setOpacity(0);
    m_labelInfo = CCLabelTTF::create("温馨提示: 升级VIP后，每笔充值都可在《邮件》中领取返利！", "Arial", 32);
    m_labelInfo -> setPosition(ccp(35, 15));
    m_labelInfo -> setAnchorPoint(CCPointZero);
    this -> addChild(m_labelInfo);
    
    m_spriteCoin = CCSprite::create("charge/coinitem2.png");
    m_spriteCoin -> setPosition(ccp(117, pChargeTopBg->getContentSize().height/2+3));
    pChargeTopBg -> addChild(m_spriteCoin, 2);
    
    //VIP、充值返利按钮
    CCMenuItemImage* chargeBackItem = CCMenuItemImage::create("charge/vipitem0.png", "charge/vipitem0.png", this, menu_selector(Charge2Layer::clickVIPMenu));
    chargeBackItem -> setTag(101);
    chargeBackItem -> setPosition(ccp(1150-114, 670));
    CCMenuItemImage* backItem = CCMenuItemImage::create("barrelUI/lotterycloseitem.png", "barrelUI/lotterycloseitem.png", this, menu_selector(Charge2Layer::clickBack));
    backItem -> setScale(2.0f);
    backItem -> setPosition(ccp(CLIENT_WIDTH-backItem->boundingBox().size.width/2, CLIENT_HEIGHT-backItem->boundingBox().size.height/2));
    
    m_BackMenu = CCMenu::create(chargeBackItem, backItem, NULL);
    m_BackMenu -> setPosition(CCPointZero);
    m_BackMenu -> setAnchorPoint(CCPointZero);
    this -> addChild(m_BackMenu);
    m_BackMenu -> setTouchPriority(-202);
    
    if(GameMainScene::_instance != NULL)
    {
        chargeBackItem -> setVisible(false);
    }
}

void Charge2Layer::loadChargeMoneyUI()
{
    m_nodeChargeNum = CCNode::create();
    this -> addChild(m_nodeChargeNum, 10);
    for(int i = 0; i < 4; i++)
    {
        CCSprite* pSpButton = CCSprite::create("charge/chargeCardBg.png");
        pSpButton -> setPosition(ccp(i%4*260+120+pSpButton->getContentSize().width/2,440-i/4*(pSpButton->getContentSize().height)+40));
        m_nodeChargeNum -> addChild(pSpButton);
        CCSprite* pButtonDi = CCSprite::create("charge/chargebutton.png");
        pButtonDi -> setPosition(ccp(pSpButton->getContentSize().width/2, pButtonDi->getContentSize().height/2-6));
        pSpButton -> addChild(pButtonDi);
        CCLabelAtlas* pLabelYuan = CCLabelAtlas::create(ChargeTypeMoney2[i], "charge/chargenum1.png", 25, 35, '0');
        pLabelYuan -> setAnchorPoint(ccp(0.5, 0.5f));
        pLabelYuan -> setPosition(ccp(pSpButton->getContentSize().width/2-20, 16));
        pSpButton -> addChild(pLabelYuan);
        CCSprite* pSpYuan = CCSprite::create("charge/chargeyuan.png");
        pSpYuan -> setPosition(ccp(pLabelYuan->getContentSize().width+18, pLabelYuan->getContentSize().height/2));
        pLabelYuan -> addChild(pSpYuan);
        pSpButton -> setTag(ChargeNumTag1[i]);
        m_arrChargeNum -> addObject(pSpButton);
        CCSprite* pSpJinbi = CCSprite::create(ChargeTypeUI1[i]);
        pSpJinbi -> setPosition(ccp(pSpButton->getContentSize().width/2, pSpButton->getContentSize().height/3*2-40));
        pSpButton -> addChild(pSpJinbi);
        if(i > 0)
        {
            CCSprite* pJinbiBg = CCSprite::create("charge/chargejinbibg.png");
            pJinbiBg -> setPosition(ccp(pSpButton->getContentSize().width/2-4, 80));
            pSpButton -> addChild(pJinbiBg);
            CCSprite* pJinbiInfo = CCSprite::create(ChargeJinbiInfo2[i-1]);
            pJinbiInfo -> setPosition(ccp(pJinbiBg->getContentSize().width/2, pJinbiBg->getContentSize().height/2+10));
            pJinbiBg -> addChild(pJinbiInfo);
        }
        
        CCLabelTTF* pLabelInfo = CCLabelTTF::create(ChargeTypeInfo1[i], "Arial-BoldMT", 26);
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
        CCLabelAtlas* pLabelYuan = CCLabelAtlas::create(ChargeTypeMoney2[i], "charge/chargenum1.png", 25, 35, '0');
        pLabelYuan -> setAnchorPoint(ccp(0.5, 0.5f));
        pLabelYuan -> setPosition(ccp(pSpButton->getContentSize().width/2-20, 16));
        pSpButton -> addChild(pLabelYuan);
        CCSprite* pSpYuan = CCSprite::create("charge/chargeyuan.png");
        pSpYuan -> setPosition(ccp(pLabelYuan->getContentSize().width+18, pLabelYuan->getContentSize().height/2));
        pLabelYuan -> addChild(pSpYuan);
        pSpButton -> setTag(ChargeNumTag1[i]);
        m_arrChargeNum -> addObject(pSpButton);
        CCSprite* pSpJinbi = CCSprite::create(ChargeTypeUI1[i]);
        pSpJinbi -> setPosition(ccp(pSpButton->getContentSize().width/2, pSpButton->getContentSize().height/3*2-40));
        pSpButton -> addChild(pSpJinbi);
        CCSprite* pJinbiBg = CCSprite::create("charge/chargejinbibg.png");
        pJinbiBg -> setPosition(ccp(pSpButton->getContentSize().width/2-4, 80));
        pSpButton -> addChild(pJinbiBg);
        CCSprite* pJinbiInfo = CCSprite::create(ChargeJinbiInfo2[i-1]);
        pJinbiInfo -> setPosition(ccp(pJinbiBg->getContentSize().width/2, pJinbiBg->getContentSize().height/2+10));
        pJinbiBg -> addChild(pJinbiInfo);
        
        CCLabelTTF* pLabelInfo = CCLabelTTF::create(ChargeTypeInfo1[i], "Arial-BoldMT", 26);
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

void Charge2Layer::showChargeUIParticle(CCSprite* sprite)
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

void Charge2Layer::selectedChargeSuccess()
{
    if(orderIDStr != "")
    {
        CMainLogic::sharedMainLogic()->ShowMessage("订单正在完成中，请耐心等待哟", eMBOK);
        return;
    }
    for(int i = 0; i < 8; i++)
    {
        CCSprite* pSpBg = (CCSprite*)m_arrChargeNum->objectAtIndex(i);
        if(m_nCurrentType == pSpBg->getTag())
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
            request -> setResponseCallback(this, httpresponse_selector(Charge2Layer::onHttpIAPComplete));
            CCHttpClient::getInstance()->send(request);
            CC_SAFE_RELEASE(request);
            request = NULL;
            
            CMainLogic::sharedMainLogic()->ShowMessage("正在获取订单", eMBNull);
            
            break;
        }
    }
}

void Charge2Layer::selectedChargeTypeAndPrise(int type)
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
    else
    {
        selectedChargeSuccess();
    }
}

void Charge2Layer::onHttpIAPComplete(CCHttpClient* sender, CCHttpResponse* response)
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

void Charge2Layer::clickVIPMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CMainLogic::sharedMainLogic()->m_bChargeState = false;
    CMainLogic::sharedMainLogic()->m_bGameToVIP = false;
    CMainLogic::sharedMainLogic()->m_bBYGameToSuper = false;
    CMainLogic::sharedMainLogic()->m_nExitGameToSpecialArm = 0;
    if(GameMainScene::_instance != NULL)
    {
        GameMainScene::GetInstance()->setWndHave(false);
    }
    this->removeFromParent();
    if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        CCNode* pNode = CCDirector::sharedDirector()->getRunningScene();
        if(pNode != NULL)
        {
            ChargeLayer* pChargeLayer = (ChargeLayer*)pNode->getChildByTag(20000);
            if(pChargeLayer != NULL)
            {
                pChargeLayer->removeFromParent();
            }
        }
    }
    
    if(CMainLogic::sharedMainLogic()->m_pHallScene != NULL)
    {
        CHallLayer* pHallLayer = (CHallLayer*)CMainLogic::sharedMainLogic()->m_pHallScene->getChildByTag(eHallLayerTag);
        if(pHallLayer != NULL)
        {
            CMainLogic::sharedMainLogic()->ShowMessage("正在打开VIP，请稍候...", eMBNull);
            pHallLayer->sendVIPVersion("VIP");
        }
    }
}

void Charge2Layer::clickBack(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    this->removeFromParent();
    if(CMainLogic::sharedMainLogic()->m_bIOSPassed)//说明当前有其他充值界面
    {
        ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
        if(pChargeLayer != NULL)
        {
            pChargeLayer->clickBack(NULL);
        }
    }
    if(GameMainScene::_instance != NULL)
    {
        GameMainScene::GetInstance()->setAutoFire();
    }
}

void Charge2Layer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -201, true);
}

bool Charge2Layer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    m_startPoint = pTouch->getLocation();
    if(this->isVisible())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Charge2Layer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint endPoint = pTouch->getLocation();
    if(abs((int)m_startPoint.x-(int)endPoint.x) > 15)
        return ;
    //转换坐标
    if(m_nOpenType == 0)//使用苹果充值购买金币
    {
        endPoint = m_nodeChargeNum->convertToNodeSpace(endPoint);
        for(int i = 0; i < m_arrChargeNum->count(); i++)
        {
            CCSprite* pSpBg = (CCSprite*)m_arrChargeNum->objectAtIndex(i);
            if(pSpBg != NULL)
            {
                CCRect rect = pSpBg->boundingBox();
                if(rect.containsPoint(endPoint))
                {
                    CMainLogic::sharedMainLogic()->m_nBuyCannonIndex = 0;
                    selectedChargeTypeAndPrise(pSpBg->getTag());
                    break;
                }
            }
        }
    }
}

void Charge2Layer::showUpgradeVIP()
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
