//
//  NoticeOfHall.hpp
//  CatchFish3
//
//  Created by 王军闯 on 16/5/25.
//
//

#ifndef NoticeOfHall_hpp
#define NoticeOfHall_hpp

#include <stdio.h>

struct tagNoticeOfHall
{
    CCLabelTTF* labelNotice;
    float fWidth;
};

class CNoticeOfHall : public CBASELAYER, public CCScrollViewDelegate
{
public:
    ~CNoticeOfHall();
    static CNoticeOfHall* create(bool bLocalOpen, int nHall = 1);
    virtual bool init(bool bLocalOpen, int nHall = 1);
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    void closeNotice(CCObject* pObj);
    
    //读取本地文件
    void readLocalNotice(int nHall = 1);
    void parseData(std::string str);
    void showAllNotice();
    
    //创建滚动层
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    void createNoticeScrollView();
    void showNotice(bool bTitle, std::string sNotice);
    CCLabelTTF* HorizontalSpacingANDVerticalSpacing(bool bTitle, std::string _string, const char *fontName, float fontSize, float horizontalSpacing, float verticalSpacing, float lineWidth);
private:
    CCArray* m_noticeArray;
    CCScrollView* m_pNoticeScrollView;      //公告滚动层
    std::vector<tagNoticeOfHall> m_vctNotice;//公告
    std::vector<std::string> m_vctNoticeStr;//公告内容
    int m_nOpenType;
};

#endif /* NoticeOfHall_hpp */
