#ifndef __CatchFish3__LabaNoticeLayer__
#define __CatchFish3__LabaNoticeLayer__

class LabaNoticeLayer : public CBASELAYER
{
public:
    LabaNoticeLayer(int nHeight);
    ~LabaNoticeLayer();
    virtual bool init();
    void loadUI();
    void showLabaNotice();
    void removeNotice();
    
    
    bool m_bShowCurrent;
private:
    CCSprite* m_spNoticeBg;
    CCLabelTTF* m_labelNoticeStr;
    int m_nHeight;
};

#endif
