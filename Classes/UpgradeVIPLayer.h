#ifndef __CatchFish3__UpgradeVIPLayer__
#define __CatchFish3__UpgradeVIPLayer__

class UpgradeVIPLayer : public CBASELAYER
{
public:
    CREATE_FUNC(UpgradeVIPLayer);
    virtual bool init();
    void loadUI();
    void clickMenu(CCObject* object);
    void clickClose(CCObject* object);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher();
    
    void onEnterTransitionDidFinish();
};

#endif
