#ifndef __CatchFish__Charge2Layer__
#define __CatchFish__Charge2Layer__

class Charge2Layer : public CCLayer, public CCScrollViewDelegate
{
public:
    ~Charge2Layer();
    static Charge2Layer* create(CMD_GPR_IAPProductList* IAPProductList, int nOpenType = 0);
    virtual bool init(CMD_GPR_IAPProductList* IAPProductList, int nOpenType = 0);
    void loadUI();
    
    void loadChargeMoneyUI();
    
    void selectedChargeSuccess();
    void selectedChargeTypeAndPrise(int type);
    void onHttpIAPComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void clickVIPMenu(CCObject* object);
    void clickBack(CCObject* object);
    
    void showUpgradeVIP();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    void showChargeUIParticle(CCSprite* sprite);
private:
    CCPoint m_startPoint;
    CCMenu* m_BackMenu;
    CCArray* m_arrChargeNum;
    CCNode* m_nodeChargeNum;
    
    IOSiAP_Bridge* bridge;
    CMD_GPR_IAPProductList m_IAPProductList;
    
    CCSprite* m_spriteCoin;
    
    int m_nOpenType;//打开方式，为0显示金币，为1显示武器
    int m_nCannonStatus;
    
    CCLabelTTF* m_labelInfo;
    int m_nCurrentType;
    int m_nCurrentIndex;
};

#endif /* defined(__CatchFish__Charge2Layer__) */
