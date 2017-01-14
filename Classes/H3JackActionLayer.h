#ifndef H3JackActionLayer_h
#define H3JackActionLayer_h

class JackActionLayer : public CBASELAYER
{
public:
    JackActionLayer();
    ~JackActionLayer();
    CREATE_FUNC(JackActionLayer);
    virtual bool init();
    void loadUI();
    
    void loadJackUI();
    
    void showZhuanAction();
    void showStopAction(CCNode* node);
    void showJackAnimate(CCNode* node);
    
    void showJackUI();
    
    std::string getNickName(std::string strName);
    
    void removeWnd();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    CCArray* m_arrFuhao;
    int m_nJackPos;
    CCSprite* m_spRedDi;
    CCNode* m_node1;
    CCNode* m_node2;
    CCSprite* m_spBg;
};

#endif
