#ifndef __CatchFish__Laba2Layer__
#define __CatchFish__Laba2Layer__

class Laba2Layer : public CBASELAYER
{
public:
    Laba2Layer(int nHeight);
    virtual bool init();
    void loadUI();
    
    void showNotice();
    void resumeNotice();
    
    bool m_bShowCurrent;        //是否正在显示中
private:
    CCLabelTTF* m_labelNoticeStr;
    CCSprite* m_spNoticeBg;
    std::vector<VIPNotice> m_vctVIPNotice;
    int m_nHeight;
};

#endif /* defined(__CatchFish__NoticeLayer__) */
