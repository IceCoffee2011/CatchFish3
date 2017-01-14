#ifndef __SELECTSEAT_H__
#define __SELECTSEAT_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#endif

struct tagNiuRankTimes//排行榜比赛名次
{
    int nID;
    long lScore;
    std::string sNickName;
};

//单个桌子属性
struct tagTableAttribute
{
	//桌子标志
	bool							bLocker;							//密码标志
	bool							bPlaying;							//游戏标志
	bool							bFocusFrame;						//框架标志

	//桌子状态
	WORD							wWatchCount;						//旁观数目
	DWORD							dwTableOwnerID;						//桌主索引

	//属性变量
	WORD							wTableID;							//桌子号码
	WORD							wChairCount;						//椅子数目

    int                             nPlayerCount;//桌子上玩家数目
	CCMenuItemSprite *				pTable;//桌子列表中的每一个按钮
	CCLabelTTF *                    pPlayerNumTTF;//用于显示桌子上玩家数目
	CCSprite *                      pFullSp;//满人标识
	CCMenuItemImage *               pNotFullSp;//未满标识
    CCScale9Sprite *                pPlayerPercentShow;//玩家人数所占比例的图片显示

    tagUserInfo *                   pUserInfo[MAX_CHAIR];		//用户信息
};

class CTableLayer : public CBASELAYER, public CCScrollViewDelegate
{
public:
	virtual bool init(int nChairCount);
	static CCScene * scene(int nChairCount);
    static CTableLayer* create(int nChairCount);
	//CREATE_FUNC(CTableLayer);
    ~CTableLayer();
	//配置桌子
    void configTable(WORD wTableCount, WORD wChairCount, const char* szServerName, bool bEnter);

	void funcTableView(CCObject* obj);
	void update(float ft);
	void funcSeatClick(CCObject* obj);
    void funcSeat2Click(CCObject* obj);
	void setObjVisible(CCNode* pNode, void * pData);
	void menuItemCall(CCObject* obj);

	void setPlayerNum(int nOrder, int nNum);
	int getPlayerCount(WORD wTableID);

	void setTableStatus(WORD wTableID, bool bPlaying);
	bool SetClientUserItem(WORD wTableID, WORD wChairID, tagUserInfo * pUserInfo);
    tagUserInfo * GetClientUserItem(WORD wTableID, WORD wChairID);

	void updateTableList(WORD wTableID);
	void updateTableShow();
	void updateSeat(WORD wChairID, tagTableAttribute & TableAttribute);

	bool findSeat(WORD & wTableID, WORD & wChairID);
    void setEnterGame();

    //void setNotice(std::string noticeStr);
    
    void updateTableCard(WORD wTableID);
	
    void onEnterTransitionDidFinish();
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    CCLayer* m_layerTable;
    CCLayer* m_NiuLayer;
    CCLayer* m_PirateLayer;
    CCLabelTTF * m_cclNiuUserScore;
    
    int m_nChairCount;      //房间内桌子数量，捕鱼为6人，二人牛牛为二人，四人牛牛为四人
    
    void enterTable(float dt);
    void changeLabel(float f);
private:
    CCLayerColor* m_layerColor;
	CCScrollView* m_pScrollView;//桌子列表
	bool m_bScroll;//防止滑动后松开鼠标，会触发点击事件
    bool m_bChangeTable;//是否切换点击的桌子

	CCMenuItemImage* m_pSitDownItem[100];//椅子按钮
	CCSprite* m_spBoyAccount[100];//男性头像，每个椅子位置有一个
	CCSprite* m_spGirlAccount[100];//女性头像，每个椅子位置有一个
	CCLabelTTF* m_pNickLabelTTF[100];//显示玩家姓名
    //CCLabelTTF* m_pNickLabel2TTF[100];
    CCSprite* m_spNiuCard[100];
    CCSprite* m_spVipLogo[100];
    CCLabelAtlas* m_labelVIP[100];
    CCSprite* m_pVIPHead[100];
    CCScale9Sprite* m_pVIPSlider[100];
    CCSprite* m_spSeatIndicate[100];

	CCSprite* m_spTableFree;   //无人时桌面
	CCSprite* m_spTablePlaying;//有人时桌面
	CCSprite* m_spSelectTable;//选中桌子标识
    
    CCSprite* m_spWaiting;
    
    CCLabelTTF* m_labelNotice;

	WORD m_wTableCount;//桌子数目
	WORD m_wChairCount;//椅子数目
	WORD m_wSelectTableID;
	WORD m_wLastTableID;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	 bool m_bFirstEnter;
#endif
   
	std::vector<tagTableAttribute> m_vTableAttribute;
    
    CCSprite* m_spNiuRank;
    CCSprite* m_spNiuNoticeBg;
    CCScrollView* m_scrollNiuView;
    CCLayer* m_containNiuLayer;
    CCLabelTTF* m_labelNiuNotice;
    
    CCLabelTTF * m_cclNiuNickName;
    CCLabelTTF * m_cclNiuUserID;
    
    CCMenu* m_menuBack;
    int m_nEnterGame;
    
    CCSprite* m_spImage1;
    CCSprite* m_spImage2;
    CCSprite* m_spImage3;
};

#endif
