#ifndef H3PlayerInfoLayer_h
#define H3PlayerInfoLayer_h

class PlayerInfoLayer : public CBASELAYER
{
public:
    PlayerInfoLayer();
    ~PlayerInfoLayer();
    static PlayerInfoLayer* create(int nUserID);//头像ID，幸运星为0，自己为8
    virtual bool init(int nUserID);
    
    void loadUI();
    //0金币数，1表示游戏局数，2表示最大赢取金币数，3表示20局下注金币数，4表示20局获胜多少局
    void updateUI(SCORE lScore, SCORE lTotalNum, SCORE lWinScore, SCORE lTotalBet, SCORE lWinNum);
    
    void clickMenu(CCObject* object);
    void removeWnd();
    
    std::string getStringScore(SCORE llScore);
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    
private:
    CCSprite* m_spBg;
    CCNode* m_nodeMyInfo;
    CCNode* m_nodeEmo;
    CCScrollView* m_scrollEmo;
    CCLayer* m_layerEmo;
    CCArray* m_arrEmo;
    CCPoint m_startPoint;
    int m_nOpenUserID;
    
    CCLabelBMFont* m_pLabelScore;
    CCLabelTTF* m_pLabel1;
    CCLabelTTF* m_pLabel2;
    CCLabelTTF* m_pLabel3;
    CCLabelTTF* m_pLabel4;
};

#endif
