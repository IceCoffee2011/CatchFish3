#ifndef __CatchFish3__BuyBarrelLayer__
#define __CatchFish3__BuyBarrelLayer__

class BuyBarrelLayer : public CBASELAYER
{
public:
    BuyBarrelLayer();
    ~BuyBarrelLayer();
    static BuyBarrelLayer* create(int nSelectedTag, int nOpenType, int nTotalScore = 0);
    virtual bool init(int nSelectedTag, int nOpenType, int nTotalScore = 0);
    void loadUI();
    
    void closeWnd(CCObject* object);
    void clickMenu(CCObject* object);
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
private:
    CCLayerColor* pLayerColor;
    CCScale9Sprite* pSpDi;
    CCSize m_visibleSize;
    CCLabelTTF* m_cclMessage;
    int m_nSelectedTag;//当前卡片的tag值
    int m_nBuyType;//1为炮筒，2为技能
    int m_nTotalScore;
    CCLabelAtlas* m_labelSkillNum;
};

#endif
