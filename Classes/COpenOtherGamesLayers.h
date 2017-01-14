//
//  COpenOtherGamesLayers.hpp
//  otherGames
//
//  Created by Guang on 15/12/7.
//
//

#ifndef COpenOtherGamesLayers_hpp
#define COpenOtherGamesLayers_hpp

class OpenOtherGamesLayer : public CBASELAYER,CCScrollViewDelegate
{
public:
    ~OpenOtherGamesLayer();
    virtual bool init();
    static OpenOtherGamesLayer* create();
    void closeOtherGamesWnd(CCObject * obj);
    void SetGameImg();
    void loadBall();
    void setBallPosition();
    
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void adjustScrollView(float offset);
    void confirmGamePicture(CCPoint p);
    void OpenURlForGameDown(std::string GameURL);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void linkToMoreGamesDownLoad(std::string str);
#endif
    void TimerToSetScrollViewTouch(float dt);
    
private:
    CCSize                      m_Size;
    CCScrollView *              m_OtherGamesView;
    CCLayer *                   m_containLayer;
    float                       m_fStart;
    float                       m_ContentOffset;
    int                         m_nPage;
    bool                        m_bSetLayerPosition;
    float                       m_xPos;
    float                       m_fEnd;
    int                         m_nAdCount;
};

#endif /* COpenOtherGamesLayers_hpp */
