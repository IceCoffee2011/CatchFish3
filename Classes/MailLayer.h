//
//  MailLayer.h
//  CatchFish
//
//  Created by 河北腾游 on 15/8/6.
//
//

#ifndef __CatchFish__MailLayer__
#define __CatchFish__MailLayer__

class MailLayer : public CCLayer, CCTableViewDelegate, CCTableViewDataSource
{
public:
    MailLayer();
    static MailLayer* create();
    virtual bool init();
    void loadUIBg();
    void loadUIContain();
    void removeWnd();
    
    void closeContent(CCObject* object);
    void closeLayer(CCObject* object);
    void getClickMenu(CCObject* object);
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    //协议
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    
    //CCTableViewDelegate
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);//当点击单元格格时触发的方法
    //单元格中CCTableViewCell类中有一个属性idx，可以判断点击的这个单元格的索引
    virtual void tableCellHighlight(CCTableView* table,CCTableViewCell* cell);//按下去的时候就是高亮显示，这里可以设置高
    virtual void tableCellUnhighlight(CCTableView* table,CCTableViewCell* cell);//松开的时候，取消高亮状态
    
    //CCTableViewDataSource
    virtual unsigned int numberOfCellsInTableView(CCTableView* table);//返回这个表格有多少单元格
    virtual CCSize cellSizeForTable(CCTableView* table);//返回每个Cell的大小
    virtual CCTableViewCell* tableCellAtIndex(CCTableView* table,unsigned int idx);//生成单元格，即单元格的内容
    
    void showMailContain(int nMailID, bool bShowAction = true);//显示邮件内容
    void setMenuState();
    
    void clickCopy(CCObject* object);
    
    void openMailContent();
    
    void setMailItem(char* itemStr, int count);
    
public:
    CCScale9Sprite* m_pSpDi;
private:
    CCSprite* m_spBg;
    CCTableView* m_tableView;
    CCLabelTTF* m_contentTTF;
    CCMenu* m_menuGet;
    
    CCSprite* m_spGet;
    int m_nCurrentMailID;
    int m_nSelectedMailID;
    
    CCSprite* m_nodeMail;
    CCSprite* m_nodeContain;
    CCSprite* pMailBg;
    CCPoint m_point;
    std::string m_sDuihuanma;
    std::string m_sGongzhonghao;
};

#endif /* defined(__CatchFish__MailLayer__) */
