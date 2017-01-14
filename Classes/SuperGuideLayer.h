#ifndef __CatchFish3__SuperGuideLayer__
#define __CatchFish3__SuperGuideLayer__

class SuperGuideLayer : public CBASELAYER
{
public:
    static SuperGuideLayer* create();
    virtual bool init();
    void loadUI();
    
    void clickMenu(CCObject* object);
    
    CCAnimate* getAnimate();
    
    void setShowNode4();
    void setShowNode5();
    void setShowNode6();
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    int getYaZhu();
    int getCurrentIndex();
    
private:
    int m_nCurrentIndex;
    CCNode* m_node1;
    CCNode* m_node2;
    CCNode* m_node3;
    CCNode* m_node4;
    CCNode* m_node5;
    CCNode* m_node6;
    CCLabelAtlas* m_labelYa;
    CCLabelAtlas* m_p4CurrentBetNum;
    CCLabelAtlas* m_p5CurrentBetNum;
};

#endif
