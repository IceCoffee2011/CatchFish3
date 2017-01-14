//
//  ChargeLayer.h
//  CatchFish
//
//  Created by 河北腾游 on 15/4/15.
//
//

#ifndef __CatchFish__ChargeLayer__
#define __CatchFish__ChargeLayer__

#include "cocos-ext.h"
USING_NS_CC_EXT;

/*
    参数说名：
    nOpenType = 0为普通充值； =1为升级VIP，=10为首充10元,
              =11为霹雳炮，=12为火球炮，=13生化炮，=14为VIP1炮，15为VIP2炮，15为VIP3炮，17为VIP4炮
              =21为购买加速，=22为购买散射，=23为购买暴击
              =31为购买试用VIP1炮，=32为购买试用VIP2炮，=33为购买试用VIP3炮，=34为购买试用VIP4炮
    nUpgradeMoney 当不等于0时，为升级VIP时的充值金额或者指定炮的价格
    nOpenCharge为进入时为金币充值、武器充值、技能充值（0为金币，1为武器，2为技能）
 */

class ChargeLayer : public CCLayer, public CCScrollViewDelegate
{
public:
    ~ChargeLayer();
    virtual bool init(int nOpenType = 0, int nUpgradeMoney = 0, int nOpenCharge = 0);
    static ChargeLayer* create(int nOpenType = 0, int nUpgradeMoney = 0, int nOpenCharge = 0);//第一个参数代表是普通充值还是首充还是升级VIP充值，第二个参数代表本次充值金额，第三个参数代表打开进入时是充值金币还是充值武器还是充值技能（0为金币，1为武器，2为技能）
    void loadUI();
    void loadChargeTypeUI();
    
    void clickBack(CCObject* object);
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    
    void selectedChargeSuccess();
    
    void selectedChargeTypeAndPrise(int type, int index);
    void wxCharge(int prise);
    void onHttpAnySDKPayComplete(CCHttpClient* sender, CCHttpResponse* response);
    void onHttpSharedComplete(CCHttpClient* sender, CCHttpResponse* response);
	// android  charge
	void AndroidAlipay(int prise);
	void AndroidYeepay(int YeepayType, int prise);
    
    //IOS charge
    void IOSAlipay(int prise);
	void onHttpAlipayComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void onHttpIOSMessageComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    CC_SYNTHESIZE(bool, bWebViewCharge, WebViewCharge);
    
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    void load2PublicChargeType();
    void showChargeUIParticle(CCSprite* sprite);
    
    void clickVIPMenu(CCObject* object);
    
    void notifyIOSWXCompleted();
    void notifyIOSAlipayCompleted();
    
    //向网站发送成功的消息
    void sendAlipayChargeSuccess();//支付宝
    void sendWXChargeSuccess();//微信
    void onAlipayChargeSuccess(CCHttpClient* sender, CCHttpResponse* response);
    void onWXChargeSuccess(CCHttpClient* sender, CCHttpResponse* response);
    
    void delayAlipayChargeSuccess(float dt);
    void delayWXChargeSuccess(float dt);
    void setAlipaySign(std::string str);
    
    void showUpgradeVIP();
    
    void requestMail();
    void receiveHallLink2(float dt);
    
    void setFirstChargeStatus();
    
    void sendUserBehavior(int type,int index);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void selectedChargeTypeAndPriseIOS(int type);
    void IOSCharge();
    void onHttpIAPComplete(CCHttpClient* sender, CCHttpResponse* response);
#endif
    
    int m_nOpenType;
    int m_nOpenCharge;
    
    int m_nCurrentIndex;
    int m_nCurrentType;
    
    CC_SYNTHESIZE(bool, m_bOpenGonggao, OpenGonggao);
    CC_SYNTHESIZE(int, m_nCurrentSelectedType, CurrentSelectedType);//当前选择哪种充值方式
private:
    CCNode* m_nodeChargeType;       //充值类型选择界面
    CCArray* m_arrChargeType;       //充值类型选择数组
    CCSprite* m_spChargeTypeBg;
    
    CCNode* m_nodeChargeNum;        //充值金额选择界面
    CCArray* m_arrChargeNum;        //充值数量选择数组
    
    CCPoint m_startPoint;
    CCMenu* m_BackMenu;
    CCMenu* m_firstChargeMenu;
    CCMenu* m_oneChargeMenu;

    bool m_bIsOpenCard;     //是否打开充值数量界面
	std::string m_sOrderID;
    std::string m_sWXOrderID;//微信预支付订单号
    std::string m_sAlipaySign;
	int m_nPrise;
    int m_nUnicomOrTelecom;
    
    int m_nUpgradeMoney;
    
    CCSprite* m_nodeSpDi;
    CCSprite* m_spriteCoin;
    
    int m_nCannonStatus;
    int m_nArrayCurrentIndex;
    
    CCLabelTTF* m_labelInfo;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSiAP_Bridge* bridge;
    CMD_GPR_IAPProductList m_IAPProductList;
#endif
};

#endif /* defined(__CatchFish__ChargeLayer__) */
