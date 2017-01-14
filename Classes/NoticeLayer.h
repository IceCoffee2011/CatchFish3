//
//  NoticeLayer.h
//  CatchFish
//
//  Created by xiaosong1531 on 15/12/9.
//
//

#ifndef __CatchFish__NoticeLayer__
#define __CatchFish__NoticeLayer__

class NoticeLayer : public CBASELAYER
{
public:
    NoticeLayer();
    CREATE_FUNC(NoticeLayer);
    virtual bool init();
    void loadUI();
    
    void showNotice();
    void removeNotice();
    void resumeNotice();
    
    bool m_bShowCurrent;        //是否正在显示中
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
private:
    CCLabelTTF* m_labelNoticeStr;
    CCSprite* m_spNoticeBg;
    
    std::vector<VIPNotice> m_vctVIPNotice;
};

#endif /* defined(__CatchFish__NoticeLayer__) */
