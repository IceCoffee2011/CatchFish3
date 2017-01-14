//
//  RankNewLayer.hpp
//  CatchFish3
//
//  Created by Guang on 16/5/29.
//
//

#ifndef RankNewLayer_hpp
#define RankNewLayer_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;



struct NewMatchingVct
{
    int         nMatchType;
    std::string sMatchName;
    std::string sMatchNum;
    std::string sMatchIndex;
    std::string sMatchTime;
    std::string sMatchCost;
    std::string sMatchGroupNum;
    int         nMatchGroupNum;
    int         nSecond;
    bool        bSignup;
    std::string sMatchJoinedNum;
    int         nMatchJoinedNum;
    std::string sMatchTakeremainder;
    std::string sMatchRule1;
    std::string sMatchRule2;
    std::string sMatchRule3;
    std::string sFirst;
    std::string sAwardFirst;
    std::string sSeconde;
    std::string sAwardSecond;
    std::string sThird;
    std::string sAwardThird;
    std::string sFourth;
    std::string sAwardFourth;
    std::string sJoinRoomName;
    int         nHuaFeiNum;
    
};

struct NewMatchJoinedVct
{
    int         nMatchType;
    std::string sMatchName;
    std::string sMatchNum;
    std::string sMatchIndex;
    std::string sMatchTime;
    std::string sMatchCost;
    std::string sMatchJoinedNum;
    std::string sMatchGroupNum;
    int         nSecond;
    std::string sMatchRule1;
    std::string sMatchRule2;
    std::string sMatchRule3;
    std::string sFirst;
    std::string sAwardFirst;
    std::string sSeconde;
    std::string sAwardSecond;
    std::string sThird;
    std::string sAwardThird;
    std::string sFourth;
    std::string sAwardFourth;
    int         nHuaFeiNum;         //用于来决定containlayer是用哪个颜色的底框
};

struct NewMatchEndVct
{
    int         nMatchType;
    std::string sMatchName;
    std::string sMatchNum;
    std::string sMatchIndex;
    std::string sMatchTime;
    std::string sMatchCost;
    std::string sMatchJoinedNum;
    std::string sMatchGroupNum;
    std::string sMatchRule1;
    std::string sMatchRule2;
    std::string sMatchRule3;
    std::string sRank;
    std::string sScore;
    std::string sAward;
    int         nGet; //0 代表没有领取， 1代表领取   2代表没有奖励
    int         nShared;//0代表不需要分享，1代表需要分享（0、1都没领取的时候才进行判断，领取了就无需判断）
    int         nHuaFeiNum;
    int         nRank;
    std::string sMatchDay;
};

struct NewMatchRankVct
{
    std::string sRank;
    std::string sNickName;
    std::string sUserVipLevel;
    std::string sScore;
};

class CRankNewLayer : public CBASELAYER, public CCScrollViewDelegate
{
public:
    ~CRankNewLayer();
    virtual bool init();
    CREATE_FUNC(CRankNewLayer);
    void loadUI();
    void loadMatchUI();
    void loadMatchingUI();
    void loadMatchJoinedUI();
    void loadMatchedUI();
    void loadMatchNodeUI();
    void loadRankUI();
    void setLeftMatchNode(int indexItem);
    void callBack(CCObject* obj);
    void setMenuItemVisible(int tag, bool bShow, CCMenu* menu);
    
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    void requestCatchFishMatchingHttp();
    void onHttpCatchFishMatchingComplete(CCHttpClient* sender, CCHttpResponse* response);
    void requestCatchFishMatchJoinedHttp();
    void onHttpCatchFishMatchJoinedComplete(CCHttpClient* sender, CCHttpResponse* response);
    void requestCatchFishHistoryMatchHttp();
    void onHttpCatchFishHistoryMatchComplete(CCHttpClient* sender, CCHttpResponse* response);
    void requestCatchFishMatchRankHttp();
    void onHttpMatchRankComplete(CCHttpClient* sender, CCHttpResponse* response);
    void requestCatchFishFinishJoinMatchHttp();
    void onHttpCatchFishFinishJoinMatchComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    void sendShareInfo(std::string str1, std::string str2);
    
    void requestShareEndSuccessMatchAward();
    void onHttpMatchEndAwardComplete(CCHttpClient* sender, CCHttpResponse* response);
    
    int selectHuaFeiNum(std::string sHuaFei);
    std::string selectStartMatchTime(std::string sMatchTime);
    void showMyJoinedMatchCountDownTime(float dt);
    void setTime();
    void playSelectItemAnimate(CCPoint p, int nPageIndex, int nItemIndex);
    
    void refreshMatchingInfo(int nMatchType, int nMatchID);
    void setAlreadyJoinedVisible();
    
    void setBisaiOpenStatus(CCObject* object);
    
    void setMatchShare(BYTE cbShare);
    
    
	CC_SYNTHESIZE(bool, m_bJoinStatus, JoinStatus);
    std::vector<NewMatchingVct*> m_MatchingVct;
    std::vector<NewMatchJoinedVct*> m_MatchJoinedVct;
    std::vector<NewMatchEndVct*> m_MatchEndVct;
    std::vector<NewMatchRankVct*> m_MatchRankVct;
    
    CCSprite* m_pSpMatchBg;
    CCNode* m_pMatchNode;
    CCSprite* m_pSpRankBg;
    CCSprite* m_pSpRankChildBg;
    CCSprite* m_spAnimate;
    CCMenu* m_systemMenu;
    CCMenu* m_matchMenu;
    CCSprite* m_pHeadShade;
    CCSprite* m_pItemWhiteShade;
    
    CCLabelTTF* m_pNoMatchesTTF;
    CCLabelTTF* m_pNoMatchInfoTTF;
    CCLabelTTF* m_pNoRankInfoTTF;
    
    CCScrollView* m_pMatchScrollView;
    CCLayerColor* m_pMatchContainLayer;
    CCScrollView* m_pRankScrollView;
    CCLayerColor* m_pRankContainLayer;
    
    CCSprite* m_pSpJoined;
    CCSprite* m_pSpLingQued;
    CCPoint m_startPoint;
    CCArray* m_pContainTouchItemArray;
    CCArray* m_pMatchNodeAwardArray;
    
    CCSprite* m_pNoLingQu;
    CCLabelTTF* m_pNumNoLingQuTTF;
    
    bool m_bHistoryRequst;
    bool m_bRankRequest;
    
    int m_nIndexPage;
    int m_nShowItem;
    
    int m_nGuangPageID;
    int m_nGuangItemID;
    
    bool m_bJoinedFirstRequest;
    bool m_bJoinAct;//根据报名结果来决定是否改变值，在改变容器内容
    int m_nLingQuMatchID;
    bool m_bJoinRefresh;
    int m_nJoinMatchID;
    bool m_bClickTouch;
    CCNode* m_nodeMenuStatus;
    CCMenu* m_menuOpenStatus;
    CCMenu* m_menuCloseStatus;
};

#endif /* RankNewLayer_h */
