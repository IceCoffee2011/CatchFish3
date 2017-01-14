#ifndef DogCalcLayer_h
#define DogCalcLayer_h

class DogCalcLayer : public CBASELAYER
{
public:
    DogCalcLayer();
    ~DogCalcLayer();
    virtual bool init(int nFirst, int nSecond);
    static DogCalcLayer* create(int nFirst, int nSecond);
    void loadUI();
    void setCalcRank(std::string sName[5], SCORE lScore[5], SCORE lCaiChi[5]);
    void scheduleRemove();
    
    void setLabel(int nBeilv, SCORE lScore, SCORE lCaiChi, SCORE lBet);
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
private:
    CCScale9Sprite* m_pBg;
    CCSprite* m_pAnimalBg1;
    CCSprite* m_pAnimalBg2;
    CCLabelBMFont* m_pLabelTotal;
    CCLabelTTF* m_pLabelCaichi;
    CCLabelTTF* m_pLabelGetCoin;
    CCLabelTTF* m_pLabelGuanjun;
    CCLabelTTF* m_pLabelYajun;
    CCLabelTTF* m_pLabelBeilv;
    int m_nFirst;
    int m_nSecond;
};

#endif
