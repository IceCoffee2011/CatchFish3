//
//  FirstChargeLayer.h
//  CatchFish
//
//  Created by xiaosong1531 on 15/10/19.
//
//

#ifndef __CatchFish__FirstChargeLayer__
#define __CatchFish__FirstChargeLayer__

class FirstChargeLayer : public CBASELAYER, public CCEditBoxDelegate
{
public:
    static FirstChargeLayer* create(int nOpenType = 0, bool bShowGetMenu = false);
    virtual bool init(int nOpenType = 0, bool bShowGetMenu = false);
    void loadUI();
    void loadServiceUI();
    void loadDuiHuanUI();
    void loadLabaUI();
    
    void loadPaiPaiLeBg();
    void showPPLTime();
    
    void loadHallScoreHighRemind();
    void loadHallRemindCharge(int nType);
    
    void loadRemindCharge();
    void clickRemindCharge(CCObject* object);
    
    void loadDantouLayer();
    void selectDantou(CCObject* object);
    void setDantouNum(int nChujiCount, int nZhongjiCount, int nGaojiCount);
    
    void loadFirstMobileMoney(bool bShowGetMenu);
    void clickMobileFirst(CCObject* object);
    
    void clickMenu(CCObject* object);
    void closeMenu(CCObject* object);
    void removeWnd();
    
    void setLabaLastTime(int nLabaTimes, int nLastCoin=-1);
    void setLabaLabel();
    
    void sendDuiHuan(CCObject* object);
    void sendLaba(CCObject* object);
    bool efficacyNickName(const char* str);
    
    void onHttpDuihuanComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxEditingDidBegin(CCEditBox* editBox) {};
    virtual void editBoxEditingDidEnd(CCEditBox* editBox) {};
    virtual void editBoxReturn(CCEditBox* editBox){};
    
    CC_SYNTHESIZE(int, m_nLabaTimes, LabaTimes);
private:
    CCSprite* m_nodeFirstCharge;
    int m_nOpenType;
    
    CCSprite* m_pServiceDi;
    CCSprite* m_nodeService;
    CCSprite* m_spDuiHuan;
    CCEditBox* m_pEditBox;
    CCSprite* m_spLaba;
    CCLabelTTF* m_spLabaInfo;
    CCEditBox* m_pLabaEditBox;
    CCMenu* m_labaMenu;
    CCLabelAtlas* m_labelTong;
    CCLabelAtlas* m_labelYin;
    CCLabelAtlas* m_labelJin;
    CCSprite* m_spRemindCharge;
    CCLabelTTF* m_PPLLabel;
    
};

#endif /* defined(__CatchFish__FirstChargeLayer__) */
