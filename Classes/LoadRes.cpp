#include "header.h"

CCScene* LoadRes::scene()
{
    CCScene *scene = CCScene::create();
    LoadRes *layer = LoadRes::create();
    scene->addChild(layer);
    return scene;
}

void LoadRes::readLoadingXML(const char* filePath)
{
    unsigned long size;
    char *pFileContent =(char*)CCFileUtils::sharedFileUtils()->getFileData(filePath, "r", &size);
    TiXmlDocument* myDocument = new TiXmlDocument;
    myDocument->Parse(pFileContent,0,TIXML_ENCODING_UTF8);
    TiXmlElement* rootElement = myDocument->RootElement();//Root
    if (rootElement == NULL)
    {
        return;
    }
    std::string nodename=rootElement->Value();
    if (0!=nodename.compare("client"))
        return;
    TiXmlElement * pChildElement=rootElement->FirstChildElement();
    while (pChildElement!=NULL)
    {
        TiXmlElement * pCurrentElement=pChildElement;
        const char * pValue=NULL;
        pChildElement=pChildElement->NextSiblingElement();
        nodename=pCurrentElement->Value();
        if (0==nodename.compare("loading"))
        {
            pValue=pCurrentElement->Attribute("nNumberOfSprites");
            if (NULL!=pValue)
            {
                m_nNumberOfSprites=atoi(pValue);
            }
            pValue=pCurrentElement->Attribute("nNumOfCallFuncSps");
            if (NULL!=pValue)
            {
                m_nNumOfCallFuncSps=atoi(pValue);
            }
            pValue=pCurrentElement->Attribute("filepath");
            if (NULL!=pValue)
            {
                m_vctTexture.push_back(pValue);
            }
        }
    }
    delete myDocument;
}

bool LoadRes::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
    m_nNumberOfLoadedSprites = 0;
    m_nNumberOfSprites = 0;
    m_nNumOfCallFuncSps = 0;
    m_vctTexture.clear();
    m_bInitConnect = true;
    CMainLogic::sharedMainLogic()->m_bEnterLoading = true;

    std::string dirpath = CMainLogic::sharedMainLogic()->DIRPATH3;
    if(CMainLogic::sharedMainLogic()->KIND_ID == YQS_KIND_ID)
    {
        dirpath = dirpath+"LoadingLK.xml";
    }
    else if(CMainLogic::sharedMainLogic()->KIND_ID == NZNH_KIND_ID)
    {
        dirpath = dirpath+"LoadingNZNH.xml";
    }
    else
    {
        dirpath = dirpath+"Loading.xml";
    }
    
    readLoadingXML(dirpath.c_str());
    
//    CCSprite* pSpBg = CCSprite::create("h2all/buyubg.jpg");
//    pSpBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
//    this -> addChild(pSpBg);
//    CCSprite* pSpLogo = CCSprite::create("l2ogon/logonsprite.png");
//    pSpLogo -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/3*2));
//    this -> addChild(pSpLogo);
//    CCSprite* pSpLoadBg = CCSprite::create("CatchFish2001/loadspritebg.png");
//    pSpLoadBg -> setPosition(ccp(890, pSpLoadBg->getContentSize().height/2+20));
//    this -> addChild(pSpLoadBg);
//    CCSprite* pSpLoad2 = CCSprite::create("CatchFish2001/loadimage20.png");
//    pSpLoad2 -> setPosition(ccp(CLIENT_WIDTH-pSpLoad2->getContentSize().width/2-30, pSpLoad2->getContentSize().height/2+20));
//    this -> addChild(pSpLoad2);
//    
//    CCSprite* pSpLoad3 = CCSprite::create("CatchFish2001/loadsprite.png");
//    pSpLoad3 -> setPosition(ccp(40, 22));
//    pSpLoadBg -> addChild(pSpLoad3, -1);
//    CCMoveBy* moveBy = CCMoveBy::create(0.15f, ccp(0, 8));
//    CCMoveBy* moveBy2 = CCMoveBy::create(0.15f, ccp(0, -8));
//    pSpLoad3 -> runAction(CCRepeatForever::create(CCSequence::create(moveBy, moveBy2, NULL)));
    
    m_spImage1 = CCSprite::create("CatchFish2001/loadimage2.png");
    m_spImage1 -> setPosition(ccp(CLIENT_WIDTH/2, 60));//CLIENT_HEIGHT/2-
    this -> addChild(m_spImage1);
    
    
    srand((int)time(NULL));
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    return true;
}

void LoadRes::loadResCallFunc(CCObject* obj)
{
    ++m_nNumberOfLoadedSprites;

    if(m_nNumberOfLoadedSprites<=m_nNumOfCallFuncSps)
    {
        std::string texStr = CMainLogic::sharedMainLogic()->DIRPATH2+m_vctTexture[m_nNumberOfLoadedSprites-1]+".plist";
        CCSpriteFrameCache::sharedSpriteFrameCache() -> addSpriteFramesWithFile(texStr.c_str());
    }
    if (m_nNumberOfLoadedSprites == m_nNumberOfSprites)
    {
        CMainLogic::sharedMainLogic()->m_bEnterLoading = false;
        this -> scheduleUpdate();
    }
}

void LoadRes::onEnterTransitionDidFinish()
{
    CMainLogic::sharedMainLogic()->freeMemory();
    loadPicture();
    changeLabel(0);
    this -> schedule(schedule_selector(LoadRes::changeLabel),1.0f);
}

void LoadRes::loadPicture()
{
    for (unsigned int i = 0; i < m_vctTexture.size(); i++)
    {
        std::string texStr = CMainLogic::sharedMainLogic()->DIRPATH2+m_vctTexture[i]+".png";
        CCTextureCache::sharedTextureCache()->addImageAsync(texStr.c_str(), this, callfuncO_selector(LoadRes::loadResCallFunc));
        
    }
}

void LoadRes::update(float f)
{
    GameMainScene* pGameMainScene = GameMainScene::GetInstance();
    if(pGameMainScene)
    {
        if(m_bInitConnect)
        {
            //网络连接时间超过30秒，视为断开连接
            this -> scheduleOnce(schedule_selector(LoadRes::checkNetConnect), 30);
        }
        m_bInitConnect = false;
        bool bCompleted = pGameMainScene->getInitCompleted();
        if(bCompleted)
        {
            this -> unscheduleAllSelectors();
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0, GameMainScene::scene()));
			CMainLogic::sharedMainLogic()->m_bEnterLoading = true;
        }
    }
}

void LoadRes::checkNetConnect(float f)
{
    GameMainScene* pGameMainScene = GameMainScene::GetInstance();
    if(pGameMainScene != NULL)
    {
        if(pGameMainScene->m_pClientKernel != NULL)
        {
            tagUserInfo* pUserInfo = pGameMainScene->m_pClientKernel->GetMeUserInfo();
            if(pUserInfo != NULL)
            {
                CMainLogic::sharedMainLogic()->SendStandUpPacket(pUserInfo->wTableID, pUserInfo->wChairID, 1);
            }
        }
    }
    CMainLogic::sharedMainLogic()->ShowMessage("进入游戏超时，请稍后再试",eMBOK,eDisConnectNet);
}

void LoadRes::changeLabel(float f)
{
    char temp[32];
    int a = 0;
    if(CMainLogic::sharedMainLogic()->m_bIOSPassed)
    {
        a = rand()%14+1;
    }
    else
    {
        a = rand()%6+1;
    }
    sprintf(temp, "CatchFish2001/loadimage%d.png", a);
    m_spImage1->removeFromParent();
    m_spImage1 = CCSprite::create(temp);
    m_spImage1 -> setPosition(ccp(CLIENT_WIDTH/2, 60));//CLIENT_HEIGHT/2-
    this -> addChild(m_spImage1);
}