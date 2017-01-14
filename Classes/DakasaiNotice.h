#ifndef __CatchFish3__DakasaiNotice__
#define __CatchFish3__DakasaiNotice__

class DakasaiNotice : public CBASELAYER
{
public:
    DakasaiNotice(int nHeight);
    ~DakasaiNotice();
    virtual bool init();
    void loadUI();
    void showLabaNotice();
    void resumeNotice();
    
    bool m_bShowCurrent;
private:
    CCSprite* m_spNoticeBg;
    CCLabelTTF* m_labelNoticeStr;
    int m_nHeight;
    
    std::vector<VIPNotice> m_vctVIPNotice;
};

#endif
