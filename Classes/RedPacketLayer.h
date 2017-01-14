#ifndef RedPacketLayer_h
#define RedPacketLayer_h

class RedPacketLayer : public CBASELAYER
{
public:
    static RedPacketLayer* create(int nOpenType, int nRedPacketID = -1);
    virtual bool init(int nOpenType, int nRedPacketID = -1);
    void loadSendPacket();
    void loadReceivePacket();
    void loadRobPacket();
    void loadNoRedPacket();
    
    void setReceiveData(int nCoin, std::string sName, std::string sData);
    void setNoRedName(std::string sName);
    
    void clickMenu(CCObject* object);
    void closeMenu(CCObject* object);
    
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void registerWithTouchDispatcher();
private:
    CCEditBox* m_editPeople;//红包人数
    CCEditBox* m_editCount;//红包数量
    CCEditBox* m_editDes;//红包描述
    CCLabelTTF* m_labelGetCoin;//收红包获得的金币
    CCLabelTTF* m_labelGetName;//收红包的名字
    CCLabelTTF* m_labelGetData;//收红包的内容
    int m_nRedPacketID;
    int m_nRedPacketCoin;
    int m_nRedOpenType;
    CCMenu* m_menuFriends;
};

#endif
