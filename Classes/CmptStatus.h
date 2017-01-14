//
//  CmptStatus.hpp
//  CatchFish3
//
//  Created by 王军闯 on 16/5/31.
//
//

#ifndef CmptStatus_hpp
#define CmptStatus_hpp

#include <stdio.h>

struct tagCmptRankInfo
{
    std::string sNickName;
    std::string sScore;
};

class CCmptStatus : public CBASELAYER, public CCScrollViewDelegate
{
public:
    ~CCmptStatus();
    static CCmptStatus* create(std::string sCmptName, SCORE nCurntScore, int nCurntRank, int nStayTime, int nMatchNum);
    virtual bool init(std::string sCmptName, SCORE nCurntScore, int nCurntRank, int nStayTime, int nMatchNum);
    void initUI(std::string sCmptName, SCORE nCurntScore, int nCurntRank, int nStayTime, int nMatchNum);
    void menuCallBack(CCObject* pObj);
    void remindSchedule(float ft);//倒计时更新
    void changeInfo(SCORE nCurntScore, int nCurntRank, int nStayTime);//改变数据
    void changeRankInfo(std::vector<CMD_GRO_MatchGroupInfo> vMatchGroupInfo);
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    CCAnimate* createRankLight();
    
    void showBisaiMenu();
    
private:
    CCLabelTTF* m_pCrntScore;                        //
    CCLabelTTF* m_pCrntRank;
    CCLabelAtlas* m_pRemindAtlas;
    int m_nStayTime;
    CCMenuItemSprite* m_pCloseCmptItem;
    CCMenuItemSprite* m_pOpenCmptItem;
    CCMenuItemSprite* m_pOpenRankItem;
    CCMenuItemSprite* m_pCloseRankItem;
    CCArray* m_arrNickNameTTF;
    CCArray* m_arrScoreTTF;
    bool m_bRankOpened;
    int m_nMatchNum;
};

#endif /* CmptStatus_hpp */
