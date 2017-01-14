//
//  MoreGamesLayer.h
//  CatchFish
//
//  Created by xiaosong1531 on 15/12/9.
//
//

#ifndef __CatchFish__MoreGamesLayer__
#define __CatchFish__MoreGamesLayer__


class MoreGamesLayer : public CBASELAYER
{
public:
    static MoreGamesLayer* create();
    virtual bool init();
    void loadUI();
    void showAnimate();
    void loadScrollView(std::vector<MoreGamesInfo> vMoreGamesInfo);
    void clickClose(CCObject* object);
    void clickDownLoadMenu(CCObject* object);
    void clickOpenMenu(CCObject* object);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //是否存在此游戏
    bool getExitOtherGame(std::string str);
    //打开下载链接
    void linkToMoreGamesDownLoad(std::string str);
    //打开存在的游戏
    void openOtherGame(std::string str);
#endif
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    
private:
    CCSprite* m_spBg;
    bool m_bLocalOpen;
    CCScrollView* m_scrollView;
    CCLayer* m_containLayer;
    
    CCSprite* m_spAnimate;
    
    std::vector<MoreGamesInfo> m_vMoreGamesInfo;
};

#endif /* defined(__CatchFish__MoreGamesLayer__) */
