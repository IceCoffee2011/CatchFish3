#ifndef ShareLayer_h
#define ShareLayer_h

struct ExchangeInfo
{
    int nID;//唯一标识，兑换使用
    int nType;//0为京东卡，1为话费卡
    int nMoney;//兑换的金额
    int nCount;//兑换需要的友情值
};

class MyMenu : public CCMenu{
public:
    static MyMenu* create(CCMenuItem* item, ...);
    static MyMenu* createWithArray(CCArray* pArrayOfItems);
    static MyMenu* createWithItem(CCMenuItem* item);
    static MyMenu* createWithItems(CCMenuItem *firstItem, va_list args);
    virtual void registerWithTouchDispatcher()
    {
        printf("99999999\n");
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
    }
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
private:
    CCPoint m_fStart;
    CCPoint m_fEnd;
};

class ShareLayer : public CBASELAYER, public CCScrollViewDelegate
{
public:
    ShareLayer();
    ~ShareLayer();
    bool init(int nOpenType = 0);
    static ShareLayer* create(int nOpenType = 0);
    
    void loadUI();
    void loadFriendUI();
    void loadCodeUI();
    void loadInfoUI();
    void loadExchangeUI();
    
    void requestShareData();
    void onHttpShareComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void requestShareInfo();
    void onHttpShareVersionComplete(CCHttpClient* sender, CCHttpResponse* response);
    void sendShareData();
    void onHttpShareDataComplete(CCHttpClient* pSender, CCHttpResponse* response);
    void readShareData();
    
    void saveFile(std::string str);
    
    void showShareInfoUI();
    void showExchangeUI();
    
    void sendExchangeInfo(int nID);
    void onHttpExchangeComplete(CCHttpClient* pSender, CCHttpResponse* response);
    
    void clickMenu(CCObject* object);
    
    void onHttpGuideGiftComplete(CCHttpClient* pSender, CCHttpResponse* response);
    void closeGuideGiftLayer();
    
    void shareWX(int nType);//nType=0为好友，=1为朋友圈
    void shareQQ();
    
    void scheduleDelayShowPaste();
    void setPasteContent(std::string str);
    
    void clickClose(CCObject* object);
    
    void clickExchange(CCObject* object);
    
    void removeWnd();
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    std::string m_sPasteContent;
    
private:
    CCSprite* m_spShareBg;
    CCSprite* m_spFriendBg;//好友背景
    CCSprite* m_spCodeBg;//邀请码背景
    CCSprite* m_spInfoBg;//活动说明背景
    CCSprite* m_spExchangeBg;//兑换界面背景
    CCLayerColor* m_layerColor;
    CCSprite* m_spExchange2Bg;//选择话费卡背景
    
    CCLabelTTF* m_labelTotal;//总友情值
    CCLabelTTF* m_labelFriendNum;//好友总数
    CCLabelTTF* m_labelCurrent;//当前友情值
    CCLabelTTF* m_labelCode;//邀请码
    CCLabelTTF* m_labelScore;//新手红包金币
    
    CCLabelTTF* m_labelOne;
    CCLabelTTF* m_labelOneCount;
    CCLabelTTF* m_labelTwo;
    CCLabelTTF* m_labelTwoCount;
    CCLabelTTF* m_labelThree;
    CCLabelTTF* m_labelThreeCount;
    
    CCLabelTTF* m_labelMyFriend;//我的友情值（与当前友情值保持一致）
    
    CCEditBox* m_editBoxCode;//验证码输入框
    
    std::vector<std::string > m_vctInfo;
    std::vector<ExchangeInfo* > m_vctExchangeInfo;
    
    CCScrollView* m_scrollInfo;
    CCLayer* m_layerInfo;
    CCScrollView* m_scrollExchange;
    CCLayer* m_layerExchange;
    
    CCArray* m_arrShareMenu;
    CCMenu* m_menuGuideGift;//新手礼包按钮
    bool m_bShowExchangeCard;
    bool m_bShowGuideGift;//是否显示新手礼包
    
    std::string m_sShareUrl;//分享链接
    std::string m_sShareContent;//分享显示的内容
    
    int m_nSelectCardType;
    
    int m_nOpenType;
};

#endif
