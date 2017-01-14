#ifndef __LOGINLAYER_H__
#define __LOGINLAYER_H__

class CLogonLayer : public CBASELAYER
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(CLogonLayer);
    ~CLogonLayer();
    void loadUIBg();
    void loadNodeLogon();
    void loadNodeRegister();
    void menuCallFunc(CCObject* obj);
    void enterAccountNode(CCObject* object);
    void enterRegisterNode(CCObject* object);
    void clickBackMenu(CCObject* object);
    void clickRegister(CCObject* object);
    void clickMessageMenu(CCObject* object);
    void clickBackGuestUI(CCObject* object);
    
    void animationFinishCallFunc(CCNode * pNode);
    void sendLogonPacket();
    void constructGuestPacket();
    
    void sendCheckHallData();
    
    void receiveHallLink();
    void sendLogonInfo();
    void logonHallSchedule();
    void mesNetInfo();
    
    bool efficacyAccounts(const char* str);
    bool efficacyNickName(const char* str);
    
    void onEnterTransitionDidFinish();
    void requestPublicIP();
    void onHttpPublicIPComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void mesDaojishi();
    void resetStatus();
    
    void sendTelRegister(float dt);
    
    void clickReportMenu(CCObject* object);
    
    CC_SYNTHESIZE(bool, m_bLogonStatus, LogonStatus);
    
    void checkSelfLogon();
    void onSelfLogonHttpComplete(CCHttpClient* sender, CCHttpResponse* response);
    void setSelfLogonVisible(bool bVisible);
    void delayLogin(float ft);
	void showText(const char* sss);
    void testCallFunc();
private:
    CCSprite* m_spBg;
    CCLabelTTF* m_labelVersion;
    bool m_bLogon;
    
    //快速登录、账号登陆
    CCNode* m_nodeSelected;
    //登陆面板
    CCNode* m_nodeLogon;
    CCEditBox* m_pUserNameBox;
    CCEditBox* m_pPasswordBox;
    //注册面板
    CCSprite* m_nodeRegister;
    
    CCEditBox* m_pEditBoxTel;
    CCEditBox* m_pEditBoxMessage;
    CCEditBox* m_pEditBoxPassword;
    CCEditBox* m_pEditBoxEnsurePassword;
    CCEditBox* m_pEditBoxAcc;
    CCEditBox* m_pEditBoxNickName;
    CCEditBox* m_pTelBox;
    CCMenu* m_menutel;
    CCMenu* m_menuacc;
    CCSprite* m_spriteTel;
    CCSprite* m_spriteAcc;
    CCSprite* m_accSprite;
    CCSprite* m_telSprite;
    int m_nRegisterType;
    CCMenu* m_sendMenu;
    CCMenu* m_resendMenu;
    CCLabelTTF* m_labelDaojishi;
    int m_nDaojishi;
    bool m_bClickMenu;
    
    std::string m_strMsg;
};

#endif // __LOGINLAYER_H__
