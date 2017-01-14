#ifndef DaojuLayer_H
#define DaojuLayer_H

#include <stdio.h>
class DaojuLayer : public CBASELAYER
{
public:
    ~DaojuLayer();
    virtual bool init(DWORD ChuJiCount, DWORD ZhongJiCount, DWORD GaoJiCount);
    static DaojuLayer* create(DWORD ChuJiCount, DWORD ZhongJiCount, DWORD GaoJiCount);
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    
    void loadUI();
    void loadZengSongLayer();
    void loadDuiHuanLayer();
    void clickMenu(CCObject* obj);
    void closeMenu(CCObject* obj);
    
    void removeWnd();
    //更新
    void updateSpriteUI(int nChujiCount, int nZhongjiCount, int nGaojiCount);
    void updateZhenzhuUI(int nItemID, int nZhenzhuCount);
    //赠送
    void presentZhenzhu();
    
    //金币购买技能更新数量
    void updateSkillUI(int nSkillType, int nSkillCount);
    
    CCEditBox*      m_pUserID;
    CCEditBox*      m_pUserNum;
    
public:
    CCSprite*       m_pSpBg;
    
    //赠送
    CCSprite*       m_layerZeng;//赠送界面
    CCLayerColor*   m_pZengLayer;
    int             m_nZengItem;//赠送珍珠ID（1为初级，2为中级，3为高级，4为至尊）
    

private:
    int             m_nChujiCount;
    int             m_nZhongjiCount;
    int             m_nGaojiCount;
    int             m_nTouchVip;
    int             m_nDaojuIndex;
    int             m_nSuperPirateItemCount;
};

#endif
