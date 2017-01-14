#ifndef ChargeUI_h
#define ChargeUI_h

class ChargeUI : public CBASELAYER
{
public:
    static ChargeUI* create(int nChargeCount, int nChargeType);
    ~ChargeUI();
    virtual bool init(int nChargeCount, int nChargeType);
    void loadUI();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void IOSCharge(int nChargeCount);
    void onHttpIAPComplete(CCHttpClient* sender, CCHttpResponse* response);
#endif
    
    void clickMenu(CCObject* object);
    void closeMenu(CCObject* object);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    
    int getChargeType();//查看是否是VIP升级（1为VIP升级，2为10元限购，3为1元购）
    int getChargeOpenType();//1为微信，2为支付宝
    
private:
    int m_nChargeCount;//充值金额
    int m_nChargeType;//充值类型（1为VIP升级，0为其他金额充值）
    int m_nChargeOpenType;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSiAP_Bridge* bridge;
    CMD_GPR_IAPProductList m_IAPProductList;
#endif
};

#endif
