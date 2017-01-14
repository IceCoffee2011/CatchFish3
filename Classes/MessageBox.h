#ifndef _MESSAGE_BOX_
#define _MESSAGE_BOX_

enum eRequestType
{
	eNull,//无请求
	eLeaveRoom,//是否退出房间
	eLeaveHall,//是否退出大厅
    eTransferScore,//转账
    eTransferScore2,
    eGuestLogon,
    eDisConnectNet,
    eChargeMoney,
    eChargeGameMoney,
    eVersion,
    eBQSignIn,
    eSignInExtra,//签满七天，领取额外奖励
	eExitHall,
	eExitGame,
    eShareAction,
    eExitNiuGameToHall,
    eTakeChargeCount,//充值成功，是否立即取出
    eMBEnterNiuRoom,
    eReCheckWXChargeResult,//重新验证充值结果
    eReCheckAlipayChargeResult,//支付宝验证充值结果失败
    eMBBackHall,
    eMBFreeCharge,          //从救济金界面跳转到充值界面
    eShowVipUpgrade,
    eGameToVIP,     //从游戏界面进入到VIP界面
    eExitGameToHall,
    eExitToHall,
    eDeleteUserInfoLayer,
    eDisConnectBackToLogon,
    eShowBindingTel,
    eChargeBackHall,
    eCloseDuihuan,
    eCancelLogon,
    eExitFBToHall,
    eExitGameBYToHall,
    eExitGameHDToHall,
    eShowSuperCharge,
    eEnterBYToSuper,
    eShowMobileBinding,
    eExitSuperToHall,
    eCloseGiveLayer,
    eDuihuanWufuCard,
};

enum eOperateType
{
    eMBNull,
	eMBOK=1,//确定
	eMBOKCancel,//确定取消
    eMBOKBuy,//确定购买
    eMBLabaNull,
    eMBRequestNull,//请求网络消息，5s
    eMBIAPNull,
    eMBExitGame,
    eMBSuperOK,
    eMBSuperOKCancel,
    eMBGameNull,
};

struct tagMessage
{
	eOperateType	eOperate;		//操作类型
	eRequestType	eRequest;		//请求类型
	bool			bWait;			//等待动画
	char			szBuffer[256];	//消息内容
};

class CMessageBox : public cocos2d::CCLayer, CCTargetedTouchDelegate
{
public:
	CMessageBox(void);
	~CMessageBox(void);
	virtual bool init();
	static cocos2d::CCScene* scene();
	CREATE_FUNC(CMessageBox);

	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    void loadSuperUI();

	void ShowMessageBox();
	void ShowMessageBox(eOperateType operate, eRequestType request, const char * pBuffer, bool bWait=false);
	void OnButtonOK(CCObject* obj);
	void OnButtonCancel(CCObject* obj);
	void Update();
    
    void showNetWorkError(float dt);
    void showNetWorkError2(float dt);
    void showNetWorkError3(float dt);
    void showNetWorkError4(float dt);
    void showNetWorkError5(float dt);
    bool m_bShow;
    
    void removeVector();

private:
    CCLayerColor* pLayerColor;
    //CCSprite* m_spMessageBg;
    CCSprite* pSpDi;
    CCScale9Sprite* m_pGameGongGaoBg;
	CCSize m_visibleSize;
	CCMenu* m_pMenuOK;
	CCMenu* m_pMenuOKCancel;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCMenu* m_pMenuBuy;
#endif

	tagMessage m_Message;
	std::vector<tagMessage> m_vMessage;

	CCLabelTTF* m_cclMessage;
	CCSprite* m_ccsWait;
    CCLabelTTF* m_ccl2Message;
    CCSprite* m_spSuperDi;
    CCLabelTTF* m_ccl3Message;
    CCMenu* m_menuSuperOK;
    CCMenu* m_menuSuperOKCancel;
};

#endif
