//
//  FreeCoinLayer.h
//  CatchFish
//
//  Created by xiaosong1531 on 15/10/8.
//
//

#ifndef __CatchFish__FreeCoinLayer__
#define __CatchFish__FreeCoinLayer__

class FreeCoinLayer : public CBASELAYER
{
public:
    ~FreeCoinLayer();
    static FreeCoinLayer* create(int nOpenType = 0);
    virtual bool init(int nOpenType = 0);
    void loadUI();
    void loadUIMenu();
    void loadUIGave();
    void loadUILogon();
    void loadBangDingUI();
    
    void sendGiveInfo();
    void onHttpGiveInfoComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void sendGetCoin();
    void onHttpGetCoinComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void clickMenu(CCObject* object);
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    void setFreeCoinBg(bool bShow);
    void onHttpSharedComplete(CCHttpClient* sender, CCHttpResponse* response);
    void sendSharedCompleted();
    void setDelayTimeToSendShareSuccess();
    
    void sendShareInfo(std::string str1, std::string str2);
    
    void removeWnd();
    void removeWnd2();
    
    SCORE getCurrentScore();
    
    void openLogonGive();
    
    void stopLogonItem();
    
    bool checkTelephone();
    void mesDaojishi(float dt);
    void resetStatus();
    void stopBindingAction();

    //CCScale9Sprite* pSpDi;
    CCSprite* m_spFreeCoinBg;
    CCSprite* m_nodeFreeCoin;
   
    CCSprite* m_spBangingBg;
    CCSprite* m_spPasswordBg;
    CCLabelTTF* m_labelDaojishi;
    CCMenuItemImage* m_sendCodeItem;
    CCSprite* m_spDisable;
    CCEditBox* m_pTelBox;//手机号输入框
    CCEditBox* m_pCodeBox;//验证码输入框
    CCEditBox* m_pPassword1Box;//密码输入框
    CCEditBox* m_pPassword2Box;//确认密码输入框
    CCEditBox* m_pNickNameBox;//昵称输入框
    int m_nDaojishi;
private:     
    CCLabelTTF* m_lGaveTimes;
    CCLabelTTF* m_lShareTimes;
    //CCScale9Sprite* m_nodeGave;
    
    CCSprite* m_spGave;
    
    CCMenu* m_menuFree;
    CCMenu* m_menuClose;
    CCLabelTTF* m_lGaveCoin;
    CCLabelTTF* m_lGaveLastTimes;
    bool m_bClickShared;
    bool m_bReceiveGive;
    
    bool m_bCanGet; //破产救济是否可以领取
    int m_nTotal;   //破产救济总共领取次数
    int m_nLeft;    //破产救济剩余领取次数
    int m_nGold;    //破产救济每次领取金币数
    SCORE m_llCurrentScore;//玩家当前分数
    int m_nConditionScore;//领取最低限制
    std::string m_sFailInfo;
    
    CCMenu* m_menuReceive1;
    CCMenu* m_menuReceive2;
    int m_nOpenType;
};

#endif /* defined(__CatchFish__FreeCoinLayer__) */
