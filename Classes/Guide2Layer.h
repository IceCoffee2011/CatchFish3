#ifndef Guide2Layer_h
#define Guide2Layer_h

class Guide2Layer : public CBASELAYER
{
public:
    static Guide2Layer* create(int nOpenType, int nChairID = 0);
    virtual bool init(int nOpenType, int nChairID = 0);
    
    void loadHallUI();
    void clickMenu(CCObject* object);
    
    void loadGameUI();
    
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void registerWithTouchDispatcher();
private:
    CCNode* m_node1;
    CCNode* m_node2;
    CCNode* m_node3;
    CCNode* m_node4;
    int m_nOpenType;
    int m_nChairID;
};

#endif
