#ifndef __CatchFish3__GetPriseLayer__
#define __CatchFish3__GetPriseLayer__

class GetPriseLayer : public CBASELAYER
{
public:
    static GetPriseLayer* create(int nType, int nCount, std::string sDescription, float fDelayTime = 3.2f);
    virtual bool init(int cbPriseType, int nPriseCount, std::string sDescription, float fDelayTime = 3.2f);
    void removeWnd(CCNode* pNode);
};

#endif /* defined(__CatchFish3__GetPriseLayer__) */
