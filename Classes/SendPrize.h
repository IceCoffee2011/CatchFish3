//
//  SendPrize.hpp
//  CatchFish3
//
//  Created by 王军闯 on 16/5/30.
//
//

#ifndef SendPrize_hpp
#define SendPrize_hpp

#include <stdio.h>

class CSendPrizeLayer : public CBASELAYER
{
public:
    static CSendPrizeLayer* create(BYTE cbShare, int nRank, SCORE nScore, std::string sPrize, int nMatchID, std::string szMatchDate);
    virtual bool init(BYTE cbShare, int nRank, SCORE nScore, std::string sPrize, int nMatchID, std::string szMatchDate);
    void menuCallBack(CCObject* pObj);
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    //分享
    void onHttpSharedComplete(CCHttpClient* sender, CCHttpResponse* response);
    void sendSharedCompleted();
    void sendShareInfo(std::string str1, std::string str2);
private:
    bool m_bShareSucceed;
    bool m_bNeedShare;
    int m_nMatchID;
    int m_nRank;
    std::string m_sMatchDate;
};

#endif /* SendPrize_hpp */
