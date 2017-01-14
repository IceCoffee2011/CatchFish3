#ifndef WufuLayer_h
#define WufuLayer_h

class WufuLayer : public CBASELAYER
{
public:
    WufuLayer();
    ~WufuLayer();
    static WufuLayer* create(int nCard[5]);
    virtual bool init(int nCard[5]);
    void loadUI();
    void clickMenu(CCObject* object);
    
    void removeWnd();
    
    void requestHelpVersion();
    void onHttpHelpVersionComplete(CCHttpClient* pSender, CCHttpResponse* response);
    void sendWufuData();
    void onHttpHelpDataComplete(CCHttpClient* pSender, CCHttpResponse* response);
    void saveFile(std::string str);
    void readWufuRuleData();
    void showWufuHelpUI();
    
    void sendDuihuanCard(int nType);
    void onHttpDuihuanComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void updateCardUI();
    
    int getWufuType();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
private:
    CCSprite* m_pWufuBg;
    CCArray* m_arrAnCard;
    CCArray* m_arrLiangCard;
    int m_nCard[5];
    CCLayerColor* m_layerWufuHelp;
    CCSprite* m_spInfoBg;
    int m_nCurrentPage;
    int m_nType;
    std::vector<std::string> m_vctWufuRule;
    std::vector<VIPNotice> m_vctVIPNotice;
};

#endif
