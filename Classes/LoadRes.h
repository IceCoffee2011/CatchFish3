#ifndef _LOADRES_H__
#define _LOADRES_H__

class LoadRes : public CCLayer
{
public:
    CREATE_FUNC(LoadRes);
    virtual bool init();
    static CCScene* scene();
    void loadResCallFunc(CCObject* obj);
    void readLoadingXML(const char* filePath);
    virtual void onEnterTransitionDidFinish();	//进入界面后执行的方法
    void loadPicture();
    void update(float f);
    void checkNetConnect(float f);
    void changeLabel(float f);
private:
    bool m_bInitConnect;
    int m_nNumberOfSprites;
    int m_nNumOfCallFuncSps;
    int m_nNumberOfLoadedSprites;
    std::vector<std::string>m_vctTexture;
    CCSprite* m_spImage1;
};

#endif       //_LOADRES_H__