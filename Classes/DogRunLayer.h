#ifndef DogRunLayer_h
#define DogRunLayer_h

class DogRunLayer : public CBASELAYER
{
public:
    DogRunLayer();
    ~DogRunLayer();
    CREATE_FUNC(DogRunLayer);
    virtual bool init();
    void loadBG();
    void loadDogDaijiAnimate();
    void loadBeteUI();
    
    void setMeBetMoney();
    void setChampion(int nChampion1, int nChampion2);
    
    void onEnterTransitionDidFinish();
    void removeNodeCall(CCNode* pNode);
    
    void dogRunAnimate();
    void dogRunStart();
    void dogStopAnimate();
    void showCalcLayer();
    
    void createRoadBlock(float x, float y, int nType);
    void showRoadBlock();
    void resetSpeed1(CCNode* pNode, void* speed);
    void resetSpeed2(CCNode* pNode, void* speed);
    void resetBgSpeed();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    void setCalcData(int nMult, SCORE lWin, SCORE lCaichi, SCORE lBet);
    void setCalcRank(char sName[5][32], SCORE lscore[5], SCORE lCaichi[5]);
    std::string m_sNameRank[5];//排行榜
    SCORE m_llScoreRank[5];//排行榜
    SCORE m_llCaiChiRank[5];//排行榜
    int m_nChampion1;//冠军
    int m_nChampion2;//亚军
    int m_nBeilv;   //本局倍率
    SCORE m_llGetCoin;//玩家输赢
    SCORE m_llReturnBet;//返回玩家下注
    SCORE m_llCaiChi;//彩池奖励
    
private:
    CCSprite* m_spBg1;
    CCLayer* m_layerContainer;
    CCLabelBMFont* m_pLabelMeBet[15];
    CCSprite* m_pGaojiasuo;
    CCSprite* m_pJinmao;
    CCSprite* m_pSamoye;
    CCSprite* m_pShapi;
    CCSprite* m_pTaidi;
    CCSprite* m_pLachang;
    bool m_bLayerRun;//跑道是否运动
    int m_nDogSpeed[6];
    int m_nBgSpeed;//跑道速度
    CCPoint m_pointEnd;//终点坐标
    CCSprite* m_spMatchEnd;
    CCArray* m_arrDog;
    
    CCLayerColor* m_pLayerColor;
    CCArray* m_arrJiasu;
    CCArray* m_arrJiansu;
    CCSprite* m_spGuanjun;
    CCSprite* m_spYajun;
    
    
    
};

#endif
