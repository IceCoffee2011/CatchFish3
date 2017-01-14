#include "header.h"

#define BETE_MONEY_TAG      501     //玩家押注多少

enum DogTag
{
    eChatItemTag = 101,//聊天按钮
    eLihuaItemTag,//礼花按钮
    eZoushiItemTag,//走势按钮
};

enum DogAnimateTag
{
    eGaojisuoTag = 301,
    eJinmaoTag,
    eSamoyeTag,
    eShapiTag,
    eTaidiTag,
    eLachangTag,
};

const char* BetMoney[4] = {"10万", "5万", "1万", "2000"};
const char* DogMultiple[15] = {"x4", "x6", "x8", "x10", "x20", "x10", "x12", "x24", "x40", "x18", "x24", "x60", "x36", "x80", "x100"};
const char* DogChampion[15] = {"1-2", "1-3", "1-4", "1-5", "1-6", "2-3", "2-4", "2-5", "2-6", "3-4", "3-5", "3-6", "4-5", "4-6", "5-6"};

DogBeteLayer* DogBeteLayer::_instance = NULL;

DogBeteLayer::DogBeteLayer()
{
    
}

DogBeteLayer::~DogBeteLayer()
{
    m_arrChampion -> release();
    m_arrBeteNormal -> release();
    m_arrBeteDown -> release();
    _instance = NULL;
}

DogBeteLayer* DogBeteLayer::getInstance()
{
    if(_instance == NULL)
    {
        _instance=new DogBeteLayer();
        if (_instance && _instance->init())
        {
            _instance->autorelease();
        }
        else
        {
            delete _instance;
        }
    }
    return _instance;
}

CCScene* DogBeteLayer::scene()
{
    CCScene *scene = CCScene::create();
    if (_instance == NULL)
    {
        _instance=new DogBeteLayer();
        if (_instance && _instance->init())
        {
            _instance->autorelease();
        }
        else
        {
            delete _instance;
        }
    }
    scene->addChild(_instance);
    
    return scene;
}

bool DogBeteLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    this -> setScale(1.125f);
    srand((unsigned)time(NULL));
    m_arrChampion = CCArray::create();
    m_arrChampion -> retain();
    m_arrBeteNormal = CCArray::create();
    m_arrBeteNormal -> retain();
    m_arrBeteDown = CCArray::create();
    m_arrBeteDown -> retain();
    resetData();
    
    this -> setTouchEnabled(true);
    
    loadRes();//加载plist资源
    
    CClientKernel * pClientKernel = new CClientKernel();
    pClientKernel->m_pIClientKernelSink = this;
    m_pClientKernel=pClientKernel;
    
    CMainLogic::sharedMainLogic()->BeforeGameStart();
    CMainLogic::sharedMainLogic()->SendGameOption();
    
    return true;
}

void DogBeteLayer::resetData()//初始化或者清零数据
{
    m_nBetTime = 10;
    m_n56Time = 40;
    m_llCaichi = 0;
    m_nCurrentBete = 2000;
    m_llOwnBete = 0;
    m_bBetStart = false;
    for(int i = 0; i< 15; i++)
    {
        m_nOwnOneBete[i] = 0;
    }
}

void DogBeteLayer::loadRes()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("dog/dog_interface.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("dog/dog_animal.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("dog/dog_load.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("dog/dog_effect.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("dog/dog_match.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("dog/dog_result_or_charts.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("dog/dog_emo.plist");
    loadChatBgUI();//初始化聊天界面
    loadDogUI();//初始化dog界面
    loadBeteUI();//初始化押注界面
}

void DogBeteLayer::loadChatBgUI()
{
    m_pDogBg = CCSprite::create("dog/dog_bg.png");
    m_pDogBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    this -> addChild(m_pDogBg);
    CCSprite* pChatBg = CCSprite::createWithSpriteFrameName("dog_chat.png");
    pChatBg -> setPosition(ccp(pChatBg->getContentSize().width/2, pChatBg->getContentSize().height/2));
    m_pDogBg -> addChild(pChatBg);
    CCSprite* pKingBottomBg = CCSprite::createWithSpriteFrameName("dog_king_bottom_bg.png");
    pKingBottomBg -> setPosition(ccp(pChatBg->getContentSize().width/2, pChatBg->getContentSize().height-pKingBottomBg->getContentSize().height/2-15));
    pChatBg -> addChild(pKingBottomBg);
    CCSprite* pKingBg = CCSprite::createWithSpriteFrameName("dog_king_bg.png");
    pKingBg -> setPosition(ccp(pChatBg->getContentSize().width/2, pChatBg->getContentSize().height+12));
    pChatBg -> addChild(pKingBg);
    CCSprite* pLightBg1 = CCSprite::createWithSpriteFrameName("dog_light_bg1.png");
    pLightBg1 -> setPosition(ccp(pChatBg->getContentSize().width/2, pChatBg->getContentSize().height-pLightBg1->getContentSize().height/2-10));
    pChatBg -> addChild(pLightBg1);
    CCSprite* pLightBg2 = CCSprite::createWithSpriteFrameName("dog_light_bg2.png");
    pLightBg2 -> setPosition(ccp(pChatBg->getContentSize().width/2, pChatBg->getContentSize().height-pLightBg2->getContentSize().height/2-10));
    pChatBg -> addChild(pLightBg2);
    float fLightDelay = 0.4f;
    pLightBg1 -> runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(fLightDelay), CCHide::create(), CCDelayTime::create(fLightDelay), CCShow::create(), CCDelayTime::create(fLightDelay), CCHide::create(), CCDelayTime::create(fLightDelay), CCShow::create(),  CCDelayTime::create(fLightDelay), CCHide::create(), CCDelayTime::create(1.0f), CCShow::create(), NULL)));
    pLightBg2 -> runAction(CCRepeatForever::create(CCSequence::create(CCHide::create(), CCDelayTime::create(fLightDelay), CCShow::create(), CCDelayTime::create(fLightDelay), CCHide::create(), CCDelayTime::create(fLightDelay), CCShow::create(), CCDelayTime::create(fLightDelay), CCHide::create(), CCDelayTime::create(fLightDelay), CCShow::create(), CCDelayTime::create(1.0f), NULL)));
    //聊天、礼花、走势
    CCSprite* pChatNormal = CCSprite::createWithSpriteFrameName("dog_chat_btn_normal.png");
    CCSprite* pChatDown = CCSprite::createWithSpriteFrameName("dog_chat_btn_down.png");
    CCMenuItemSprite* chatItem = CCMenuItemSprite::create(pChatNormal, pChatDown, this, menu_selector(DogBeteLayer::clickChatMenu));
    chatItem -> setTag(eChatItemTag);
    chatItem -> setPosition(ccp(pChatBg->getContentSize().width/4-17, chatItem->getContentSize().height/2));
    CCSprite* pLihuaNormal = CCSprite::createWithSpriteFrameName("dog_lihua_btn_normal.png");
    CCSprite* pLihuaDown = CCSprite::createWithSpriteFrameName("dog_lihua_btn_down.png");
    CCMenuItemSprite* lihuaItem = CCMenuItemSprite::create(pLihuaNormal, pLihuaDown, this, menu_selector(DogBeteLayer::clickChatMenu));
    lihuaItem -> setTag(eLihuaItemTag);
    lihuaItem -> setPosition(ccp(pChatBg->getContentSize().width/2, lihuaItem->getContentSize().height/2));
    CCSprite* pZoushiNormal = CCSprite::createWithSpriteFrameName("dog_charts_btn_normal.png");
    CCSprite* pZoushiDown = CCSprite::createWithSpriteFrameName("dog_charts_btn_down.png");
    CCMenuItemSprite* zoushiItem = CCMenuItemSprite::create(pZoushiNormal, pZoushiDown, this, menu_selector(DogBeteLayer::clickChatMenu));
    zoushiItem -> setTag(eZoushiItemTag);
    zoushiItem -> setPosition(ccp(pChatBg->getContentSize().width/4*3+17, zoushiItem->getContentSize().height/2));
    CCMenu* pChatMenu = CCMenu::create(chatItem, lihuaItem, zoushiItem, NULL);
    pChatMenu -> setPosition(CCPointZero);
    pChatMenu -> setAnchorPoint(CCPointZero);
    pChatBg -> addChild(pChatMenu);
    
}

void DogBeteLayer::clickChatMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    switch (item->getTag()) {
        case eChatItemTag:
        {
            printf("聊天按钮\n");
            break;
        }
        case eLihuaItemTag:
        {
            printf("礼花按钮\n");
            break;
        }
        case eZoushiItemTag:
        {
            printf("走势按钮\n");
            break;
        }
        default:
            break;
    }
}

void DogBeteLayer::loadDogUI()
{
    int nDistanceX = 144;
    int nDistanceY = 74;
    CCSprite* pBeteInfo = CCSprite::createWithSpriteFrameName("dog_zi_bg.png");
    pBeteInfo -> setPosition(ccp(m_pDogBg->getContentSize().width/2+145, m_pDogBg->getContentSize().height-14));
    m_pDogBg -> addChild(pBeteInfo);
    CCSprite* gaojiasuo = CCSprite::createWithSpriteFrameName("animal1/gaojiasuo_daiji1.png");
    gaojiasuo -> setPosition(ccp(m_pDogBg->getContentSize().width/4+72, m_pDogBg->getContentSize().height-nDistanceY));
    gaojiasuo -> setTag(eGaojisuoTag);
    m_pDogBg -> addChild(gaojiasuo);
    CCSprite* gaojisuobg = CCSprite::createWithSpriteFrameName("dog_animal_number.png");
    gaojisuobg -> setPosition(ccp(m_pDogBg->getContentSize().width/4+72, m_pDogBg->getContentSize().height-nDistanceY-55));
    m_pDogBg -> addChild(gaojisuobg);
    CCSprite* gaojiasuoChampion = CCSprite::createWithSpriteFrameName("dog_starshine_bg2.png");
    gaojiasuoChampion -> setPosition(ccp(gaojisuobg->getContentSize().width/2, gaojisuobg->getContentSize().height-15));
    gaojiasuoChampion -> setAnchorPoint(ccp(0.5f, 0));
    gaojisuobg -> addChild(gaojiasuoChampion);
    gaojiasuoChampion -> setVisible(false);
    m_arrChampion -> addObject(gaojiasuoChampion);
    ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE};
    gaojiasuoChampion -> setBlendFunc(cbl);
    CCSprite* gaojisuoName = CCSprite::createWithSpriteFrameName("dog_name_1.png");
    gaojisuoName -> setPosition(ccp(gaojisuobg->getContentSize().width-gaojisuoName->getContentSize().width/2-5, gaojisuobg->getContentSize().height/2));
    gaojisuobg -> addChild(gaojisuoName);
    CCSprite* pIdBg1 = CCSprite::createWithSpriteFrameName("dog_id_bg.png");
    pIdBg1 -> setPosition(ccp(-10, gaojisuoName->getContentSize().height/2));
    gaojisuoName -> addChild(pIdBg1);
    CCLabelBMFont* gaojisuoID = CCLabelBMFont::create("1", "dog/bairen_qianliangminxuhao.fnt");
    gaojisuoID -> setPosition(ccp(12, gaojisuobg->getContentSize().height/2-4));
    gaojisuobg -> addChild(gaojisuoID);
    CCSprite* jinmao = CCSprite::createWithSpriteFrameName("animal3/jinmao_daiji1.png");
    jinmao -> setPosition(ccp(m_pDogBg->getContentSize().width/4+nDistanceX+71, m_pDogBg->getContentSize().height-nDistanceY));
    m_pDogBg -> addChild(jinmao);
    jinmao -> setTag(eJinmaoTag);
    CCSprite* jinmaobg = CCSprite::createWithSpriteFrameName("dog_animal_number.png");
    jinmaobg -> setPosition(ccp(m_pDogBg->getContentSize().width/4+nDistanceX+71, m_pDogBg->getContentSize().height-nDistanceY-55));
    m_pDogBg -> addChild(jinmaobg);
    CCSprite* jinmaoChampion = CCSprite::createWithSpriteFrameName("dog_starshine_bg2.png");
    jinmaoChampion -> setPosition(ccp(jinmaobg->getContentSize().width/2, jinmaobg->getContentSize().height-15));
    jinmaoChampion -> setAnchorPoint(ccp(0.5f, 0));
    jinmaobg -> addChild(jinmaoChampion);
    jinmaoChampion -> setBlendFunc(cbl);
    jinmaoChampion -> setVisible(false);
    m_arrChampion -> addObject(jinmaoChampion);
    CCSprite* jinmaoName = CCSprite::createWithSpriteFrameName("dog_name_3.png");
    jinmaoName -> setPosition(ccp(jinmaobg->getContentSize().width-jinmaoName->getContentSize().width/2-10, jinmaobg->getContentSize().height/2));
    jinmaobg -> addChild(jinmaoName);
    CCSprite* pIdBg3 = CCSprite::createWithSpriteFrameName("dog_id_bg.png");
    pIdBg3 -> setPosition(ccp(-12, jinmaoName->getContentSize().height/2));
    jinmaoName -> addChild(pIdBg3);
    CCLabelBMFont* jinmaoID = CCLabelBMFont::create("2", "dog/bairen_qianliangminxuhao.fnt");
    jinmaoID -> setPosition(ccp(17, jinmaobg->getContentSize().height/2-4));
    jinmaobg -> addChild(jinmaoID);
    CCSprite* samoye = CCSprite::createWithSpriteFrameName("animal4/samoye_daiji1.png");
    samoye -> setPosition(ccp(m_pDogBg->getContentSize().width/4+2*nDistanceX+71, m_pDogBg->getContentSize().height-nDistanceY));
    m_pDogBg -> addChild(samoye);
    samoye -> setTag(eSamoyeTag);
    CCSprite* samoyebg = CCSprite::createWithSpriteFrameName("dog_animal_number.png");
    samoyebg -> setPosition(ccp(m_pDogBg->getContentSize().width/4+2*nDistanceX+71, m_pDogBg->getContentSize().height-nDistanceY-55));
    m_pDogBg -> addChild(samoyebg);
    CCSprite* samoyeChampion = CCSprite::createWithSpriteFrameName("dog_starshine_bg2.png");
    samoyeChampion -> setPosition(ccp(samoyebg->getContentSize().width/2, samoyebg->getContentSize().height-15));
    samoyeChampion -> setAnchorPoint(ccp(0.5f, 0));
    samoyebg -> addChild(samoyeChampion);
    samoyeChampion -> setBlendFunc(cbl);
    samoyeChampion -> setVisible(false);
    m_arrChampion -> addObject(samoyeChampion);
    CCSprite* samoyeName = CCSprite::createWithSpriteFrameName("dog_name_4.png");
    samoyeName -> setPosition(ccp(samoyebg->getContentSize().width-samoyeName->getContentSize().width/2-5, samoyebg->getContentSize().height/2));
    samoyebg -> addChild(samoyeName);
    CCSprite* pIdBg4 = CCSprite::createWithSpriteFrameName("dog_id_bg.png");
    pIdBg4 -> setPosition(ccp(-10, samoyeName->getContentSize().height/2));
    samoyeName -> addChild(pIdBg4);
    CCLabelBMFont* samoyeID = CCLabelBMFont::create("3", "dog/bairen_qianliangminxuhao.fnt");
    samoyeID -> setPosition(ccp(12, samoyebg->getContentSize().height/2-4));
    samoyebg -> addChild(samoyeID);
    CCSprite* shapi = CCSprite::createWithSpriteFrameName("animal6/shapi_daiji1.png");
    shapi -> setPosition(ccp(m_pDogBg->getContentSize().width/4+3*nDistanceX+70, m_pDogBg->getContentSize().height-nDistanceY));
    m_pDogBg -> addChild(shapi);
    shapi -> setTag(eShapiTag);
    CCSprite* shapibg = CCSprite::createWithSpriteFrameName("dog_animal_number.png");
    shapibg -> setPosition(ccp(m_pDogBg->getContentSize().width/4+3*nDistanceX+70, m_pDogBg->getContentSize().height-nDistanceY-55));
    m_pDogBg -> addChild(shapibg);
    CCSprite* shapiChampion = CCSprite::createWithSpriteFrameName("dog_starshine_bg2.png");
    shapiChampion -> setPosition(ccp(shapibg->getContentSize().width/2, shapibg->getContentSize().height-15));
    shapiChampion -> setAnchorPoint(ccp(0.5f, 0));
    shapibg -> addChild(shapiChampion);
    shapiChampion -> setBlendFunc(cbl);
    shapiChampion -> setVisible(false);
    m_arrChampion -> addObject(shapiChampion);
    CCSprite* shapiName = CCSprite::createWithSpriteFrameName("dog_name_6.png");
    shapiName -> setPosition(ccp(shapibg->getContentSize().width-shapiName->getContentSize().width/2-10, shapibg->getContentSize().height/2));
    shapibg -> addChild(shapiName);
    CCSprite* pIdBg6 = CCSprite::createWithSpriteFrameName("dog_id_bg.png");
    pIdBg6 -> setPosition(ccp(-12, shapiName->getContentSize().height/2));
    shapiName -> addChild(pIdBg6);
    CCLabelBMFont* shapiID = CCLabelBMFont::create("4", "dog/bairen_qianliangminxuhao.fnt");
    shapiID -> setPosition(ccp(17, shapibg->getContentSize().height/2-4));
    shapibg -> addChild(shapiID);
    CCSprite* taidi = CCSprite::createWithSpriteFrameName("animal7/taidi_daiji1.png");
    taidi -> setPosition(ccp(m_pDogBg->getContentSize().width/4+4*nDistanceX+67, m_pDogBg->getContentSize().height-nDistanceY));
    m_pDogBg -> addChild(taidi);
    taidi -> setTag(eTaidiTag);
    CCSprite* taidibg = CCSprite::createWithSpriteFrameName("dog_animal_number.png");
    taidibg -> setPosition(ccp(m_pDogBg->getContentSize().width/4+4*nDistanceX+67, m_pDogBg->getContentSize().height-nDistanceY-55));
    m_pDogBg -> addChild(taidibg);
    CCSprite* taidiChampion = CCSprite::createWithSpriteFrameName("dog_starshine_bg2.png");
    taidiChampion -> setPosition(ccp(taidibg->getContentSize().width/2, taidibg->getContentSize().height-15));
    taidiChampion -> setAnchorPoint(ccp(0.5f, 0));
    taidibg -> addChild(taidiChampion);
    taidiChampion -> setBlendFunc(cbl);
    taidiChampion -> setVisible(false);
    m_arrChampion -> addObject(taidiChampion);
    CCSprite* taidiName = CCSprite::createWithSpriteFrameName("dog_name_7.png");
    taidiName -> setPosition(ccp(taidibg->getContentSize().width-taidiName->getContentSize().width/2-10, taidibg->getContentSize().height/2));
    taidibg -> addChild(taidiName);
    CCSprite* pIdBg7 = CCSprite::createWithSpriteFrameName("dog_id_bg.png");
    pIdBg7 -> setPosition(ccp(-12, taidiName->getContentSize().height/2));
    taidiName -> addChild(pIdBg7);
    CCLabelBMFont* taidiID = CCLabelBMFont::create("5", "dog/bairen_qianliangminxuhao.fnt");
    taidiID -> setPosition(ccp(17, taidibg->getContentSize().height/2-4));
    taidibg -> addChild(taidiID);
    CCSprite* lachang = CCSprite::createWithSpriteFrameName("animal8/lachang_daiji1.png");
    lachang -> setPosition(ccp(m_pDogBg->getContentSize().width/4+5*nDistanceX+65, m_pDogBg->getContentSize().height-nDistanceY));
    m_pDogBg -> addChild(lachang);
    lachang -> setTag(eLachangTag);
    CCSprite* lachangbg = CCSprite::createWithSpriteFrameName("dog_animal_number.png");
    lachangbg -> setPosition(ccp(m_pDogBg->getContentSize().width/4+5*nDistanceX+65, m_pDogBg->getContentSize().height-nDistanceY-55));
    m_pDogBg -> addChild(lachangbg);
    CCSprite* lachangChampion = CCSprite::createWithSpriteFrameName("dog_starshine_bg2.png");
    lachangChampion -> setPosition(ccp(lachangbg->getContentSize().width/2, lachangbg->getContentSize().height-15));
    lachangChampion -> setAnchorPoint(ccp(0.5f, 0));
    lachangbg -> addChild(lachangChampion);
    lachangChampion -> setBlendFunc(cbl);
    lachangChampion -> setVisible(false);
    m_arrChampion -> addObject(lachangChampion);
    CCSprite* lachangName = CCSprite::createWithSpriteFrameName("dog_name_8.png");
    lachangName -> setPosition(ccp(lachangbg->getContentSize().width-lachangName->getContentSize().width/2-10, lachangbg->getContentSize().height/2));
    lachangbg -> addChild(lachangName);
    CCSprite* pIdBg8 = CCSprite::createWithSpriteFrameName("dog_id_bg.png");
    pIdBg8 -> setPosition(ccp(-12, lachangName->getContentSize().height/2));
    lachangName -> addChild(pIdBg8);
    CCLabelBMFont* lachangID = CCLabelBMFont::create("6", "dog/bairen_qianliangminxuhao.fnt");
    lachangID -> setPosition(ccp(17, lachangbg->getContentSize().height/2-4));
    lachangbg -> addChild(lachangID);
    
    playDogAnimate();
    this -> schedule(schedule_selector(DogBeteLayer::playDogAnimate), 2.1f);
}

void DogBeteLayer::playDogAnimate()
{
    //随机播放狗待机动画
    int nIndex = rand()%6;
    switch (nIndex) {
        case 0:
        {
            CCSprite* sprite = (CCSprite*)m_pDogBg -> getChildByTag(eGaojisuoTag);
            if(sprite != NULL)
            {
                sprite->runAction(CCRepeat::create(getDogAnimate("animal1/gaojiasuo_daiji", 1, 3, 0.1), 7));
            }
            break;
        }
        case 1:
        {
            CCSprite* sprite = (CCSprite*)m_pDogBg -> getChildByTag(eJinmaoTag);
            if(sprite != NULL)
            {
                sprite->runAction(CCRepeat::create(getDogAnimate("animal3/jinmao_daiji", 1, 3, 0.1), 7));
            }
            break;
        }
        case 2:
        {
            CCSprite* sprite = (CCSprite*)m_pDogBg -> getChildByTag(eSamoyeTag);
            if(sprite != NULL)
            {
                sprite->runAction(CCRepeat::create(getDogAnimate("animal4/samoye_daiji", 1, 3, 0.1), 7));
            }
            break;
        }
        case 3:
        {
            CCSprite* sprite = (CCSprite*)m_pDogBg -> getChildByTag(eShapiTag);
            if(sprite != NULL)
            {
                sprite->runAction(CCRepeat::create(getDogAnimate("animal6/shapi_daiji", 1, 3, 0.1), 7));
            }
            break;
        }
        case 4:
        {
            CCSprite* sprite = (CCSprite*)m_pDogBg -> getChildByTag(eTaidiTag);
            if(sprite != NULL)
            {
                sprite->runAction(CCRepeat::create(getDogAnimate("animal7/taidi_daiji", 1, 3, 0.1), 7));
            }
            break;
        }
        case 5:
        {
            CCSprite* sprite = (CCSprite*)m_pDogBg -> getChildByTag(eLachangTag);
            if(sprite != NULL)
            {
                sprite->runAction(CCRepeat::create(getDogAnimate("animal8/lachang_daiji", 1, 3, 0.1), 7));
            }
            break;
        }
        default:
            break;
    }
}

void DogBeteLayer::loadBeteUI()
{
    //bottom
    CCSprite* pBottomBg = CCSprite::createWithSpriteFrameName("dog_bg_bottom.png");
    pBottomBg -> setPosition(ccp((m_pDogBg->getContentSize().width-272)/2+272, pBottomBg->getContentSize().height/2));
    m_pDogBg -> addChild(pBottomBg);
    CCSprite* pRebet = CCSprite::createWithSpriteFrameName("dog_rebet_btn_bottom.png");
    CCMenuItemSprite* rebetItem = CCMenuItemSprite::create(pRebet, pRebet, this, menu_selector(DogBeteLayer::clickRebet));//押注按钮
    rebetItem -> setTag(199);
    rebetItem -> setPosition(ccp(pBottomBg->getContentSize().width-rebetItem->getContentSize().width/2-5, pBottomBg->getContentSize().height/2-5));
    CCMenu* pRebetMenu = CCMenu::create(rebetItem, NULL);
    pRebetMenu -> setPosition(CCPointZero);
    pRebetMenu -> setPosition(CCPointZero);
    pBottomBg -> addChild(pRebetMenu, 2);
    m_pBetGuang = CCSprite::createWithSpriteFrameName("dog_bet_money_guang.png");
    m_pBetGuang -> setPosition(ccp(360, 33));
    pBottomBg -> addChild(m_pBetGuang, 1);
    for(int i = 3; i >= 0; i--)
    {
        CCSprite* pBetBottom = CCSprite::createWithSpriteFrameName("dog_bet_bottom.png");
        pBetBottom -> setPosition(ccp(pBottomBg->getContentSize().width-rebetItem->getContentSize().width-i*(pBetBottom->getContentSize().width+5)-pBetBottom->getContentSize().width/2-5, pBetBottom->getContentSize().height/2));
        pBottomBg -> addChild(pBetBottom);
        CCSprite* betMoney = CCSprite::createWithSpriteFrameName("dog_bet_money.png");
        CCMenuItemSprite* betItem = CCMenuItemSprite::create(betMoney, betMoney, this, menu_selector(DogBeteLayer::clickRebet));
        betItem -> setTag(200+i);
        betItem -> setPosition(ccp(pBottomBg->getContentSize().width-rebetItem->getContentSize().width-i*(pBetBottom->getContentSize().width+5)-pBetBottom->getContentSize().width/2-7, betItem->getContentSize().height/2-4));
        printf("pox is %f, poy is %f\n", pBottomBg->getContentSize().width-rebetItem->getContentSize().width-i*(pBetBottom->getContentSize().width+5)-pBetBottom->getContentSize().width/2-7, betItem->getContentSize().height/2-4);
        pRebetMenu -> addChild(betItem);
        CCLabelBMFont* betMoneyLabel = CCLabelBMFont::create(BetMoney[i], "dog/dog_bet_fnt2.fnt");
        betMoneyLabel -> setPosition(ccp(betItem->getContentSize().width/2, betItem->getContentSize().height/2));
        betItem -> addChild(betMoneyLabel);
    }
    CCSprite* pScoreBg = CCSprite::createWithSpriteFrameName("dog_money.png");
    pScoreBg -> setPosition(ccp(pBottomBg->getContentSize().width-400-rebetItem->getContentSize().width-pScoreBg->getContentSize().width/2, pScoreBg->getContentSize().height/2));
    pBottomBg -> addChild(pScoreBg);
    CCSprite* pScoreAdd = CCSprite::createWithSpriteFrameName("dog_add_btn.png");
    CCMenuItemSprite* scoreAddItem = CCMenuItemSprite::create(pScoreAdd, pScoreAdd, this, menu_selector(DogBeteLayer::clickChargeMenu));
    scoreAddItem -> setPosition(ccp(pScoreBg->getContentSize().width-scoreAddItem->getContentSize().width/2-6, pScoreBg->getContentSize().height/2));
    CCMenu* pChargeMenu = CCMenu::create(scoreAddItem, NULL);
    pChargeMenu -> setPosition(CCPointZero);
    pChargeMenu -> setAnchorPoint(CCPointZero);
    pScoreBg -> addChild(pChargeMenu);
    CCSprite* pHead = CCSprite::create("h2all/touxiangnv.png");
    pHead -> setPosition(ccp(pHead->getContentSize().width/2, pBottomBg->getContentSize().height/2));
    pBottomBg -> addChild(pHead);
    char temp[16];
    sprintf(temp, "%lld", CMainLogic::sharedMainLogic()->m_lUserScore);
    m_pLabelUserScore = CCLabelBMFont::create(temp, "dog/dog_bet_fnt2.fnt");
    m_pLabelUserScore -> setAnchorPoint(ccp(0, 0.5f));
    m_pLabelUserScore -> setPosition(ccp(50, pScoreBg->getContentSize().height/2-10));
    pScoreBg -> addChild(m_pLabelUserScore);
    CCLabelTTF* pLabelName = CCLabelTTF::create(CMainLogic::sharedMainLogic()->m_sNickName.c_str(), "Arial-BoldMT", 24);
    pLabelName -> setAnchorPoint(ccp(0, 0.5f));
    pLabelName -> setColor(ccc3(73, 46, 32));
    pLabelName -> setPosition(ccp(50, 60));
    pScoreBg -> addChild(pLabelName);
    
    //奖池
    CCSprite* pCaichiBg = CCSprite::createWithSpriteFrameName("dog_caici.png");
    pCaichiBg -> setPosition(ccp(pCaichiBg->getContentSize().width/2+275, pBottomBg->getContentSize().height+pCaichiBg->getContentSize().height/2+5));
    m_pDogBg -> addChild(pCaichiBg);
    CCSprite* pDollar = CCSprite::createWithSpriteFrameName("dog_dollar_bg.png");
    pDollar -> setPosition(ccp(30, 67));
    pCaichiBg -> addChild(pDollar);
    CCScrollView * pBigPrizeScroll = CCScrollView::create(CCSizeMake(305, 44));
    CCLayer* containLayer = CCLayer::create();
    pBigPrizeScroll->setContainer(containLayer);
    pBigPrizeScroll->setTouchEnabled(false);
    pBigPrizeScroll->setPosition(ccp(42, 44));
    pCaichiBg->addChild(pBigPrizeScroll, 1);
    for(int i = 0; i < 10; i++)
    {
        m_pDogBigPrize[i] = CCSprite::createWithSpriteFrameName("dog_spr_goldNum_0.png");
        for(int j = 1; j < 10; j++)
        {
            char sSp[32];
            sprintf(sSp, "dog_spr_goldNum_%d.png", j);
            CCSprite* pSp = CCSprite::createWithSpriteFrameName(sSp);
            pSp -> setPosition(ccp(m_pDogBigPrize[i]->getContentSize().width/2, m_pDogBigPrize[i]->getContentSize().height/2-m_pDogBigPrize[i]->getContentSize().height*j));
            m_pDogBigPrize[i] -> addChild(pSp);
        }
        containLayer -> addChild(m_pDogBigPrize[i], 2);
        m_pDogBigPrize[i]->setScale(0.8f);
        if(i == 0)
        {
            m_pDogBigPrize[i]->setPosition(ccp(20, m_pDogBigPrize[i]->getContentSize().height/2-5));
        }
        else if(i > 0 && i <= 3)
        {
            m_pDogBigPrize[i]->setPosition(ccp(30+27*i, m_pDogBigPrize[i]->getContentSize().height/2-5));
        }
        else if(i > 3 && i <= 6)
        {
            m_pDogBigPrize[i]->setPosition(ccp(40+27*i, m_pDogBigPrize[i]->getContentSize().height/2-5));
        }
        else
        {
            m_pDogBigPrize[i]->setPosition(ccp(50+27*i, m_pDogBigPrize[i]->getContentSize().height/2-5));
        }
    }
    for (int i = 0 ; i < 3; i++)
    {
        CCSprite* pBigPrizeD = CCSprite::createWithSpriteFrameName("dog_spr_goldNum_comma.png");
        pBigPrizeD->setPosition(ccp(80+90*i, 70));
        pCaichiBg->addChild(pBigPrizeD);
    }
//    CCSpriteBatchNode* batch = CCSpriteBatchNode::create("dog/dog_starshine_bg1.png");
//    pCaichiBg -> addChild(batch, 4, 2);
//    for(int i = 0; i < 40; i++)
//    {
//        int nPox = pCaichiBg->getContentSize().width/2+rand()%350-170;
//        int nPoy = 60+rand()%20-10;
//        CCSprite* pEffect1 = CCSprite::create("dog/dog_starshine_bg1.png");
//        pEffect1 -> setPosition(ccp(nPox, nPoy));
//        batch -> addChild(pEffect1);
//        pEffect1 -> setColor(ccc3(255, 97, 234));
//        int nMoveX = 20+rand()%20-10;
//        int nMoveY = 40+rand()%30-10;
//        int nMoveTime = 1 + rand()%1;
//        pEffect1 -> runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0, 255), CCScaleTo::create(0, 1.0f), CCSpawn::create(CCMoveBy::create(nMoveTime, ccp(nMoveX, nMoveY)), CCScaleTo::create(nMoveTime, 0.2f), CCFadeOut::create(nMoveTime), NULL), CCMoveTo::create(0, ccp(nPox, nPoy)), NULL)));
//    }
    
    
    CCSprite* pTransparent = CCSprite::create("dog/dog_bnt_transparent.png");
    pTransparent -> setPosition(ccp(pCaichiBg->getContentSize().width/2, 70));
    pTransparent -> setScale(4);
    pCaichiBg -> addChild(pTransparent, 3);
    CCSprite* pCaichiEffect = CCSprite::createWithSpriteFrameName("dog_lightning_bg11.png");
    ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE};
    pCaichiEffect -> setBlendFunc(cbl);
    pCaichiEffect -> setRotation(90);
    pCaichiEffect -> setScale(1.7f);
    pCaichiEffect -> setPosition(ccp(pCaichiBg->getContentSize().width/2, 70));
    pCaichiBg -> addChild(pCaichiEffect, 3);
    CCAnimate* pCaichiAniamte1 = getDogAnimate("dog_lightning_bg", 11, 7, 0.1f);
    pCaichiEffect -> runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0, 255), CCShow::create(), pCaichiAniamte1, CCHide::create(), CCDelayTime::create(0.5f), CCShow::create(), CCSpawn::create(pCaichiAniamte1, CCFadeTo::create(0.7f, 100), NULL), CCHide::create(), CCDelayTime::create(2.0f), NULL)));
    
    //下注倒计时
    CCSprite* pTimeBg = CCSprite::createWithSpriteFrameName("dog_time_bg.png");
    pTimeBg -> setPosition(ccp(350, pBottomBg->getContentSize().height+pCaichiBg->getContentSize().height+pTimeBg->getContentSize().height/2+15));
    m_pDogBg -> addChild(pTimeBg);
    CCSprite* pSpBetTime = CCSprite::createWithSpriteFrameName("dog_bet_time.png");
    pSpBetTime -> setPosition(ccp(pTimeBg->getContentSize().width/2, pTimeBg->getContentSize().height-4));
    pTimeBg -> addChild(pSpBetTime);
    sprintf(temp, "%d", m_nBetTime);
    m_pLabelBetTime = CCLabelBMFont::create(temp, "dog/dog_time_fnt.fnt");
    m_pLabelBetTime -> setPosition(ccp(pTimeBg->getContentSize().width/2, pTimeBg->getContentSize().height/2-4));
    pTimeBg -> addChild(m_pLabelBetTime);
    
    //56大奖打开时间
    CCSprite* pOpen56Bg = CCSprite::createWithSpriteFrameName("dog_open56_bg.png");
    pOpen56Bg -> setPosition(ccp(275+pCaichiBg->getContentSize().width+pOpen56Bg->getContentSize().width/2+5, pBottomBg->getContentSize().height+120));
    m_pDogBg -> addChild(pOpen56Bg);
    m_pLabel56Time = CCLabelBMFont::create("", "dog/dog_open56_fnt.fnt");
    m_pLabel56Time -> setPosition(ccp(pOpen56Bg->getContentSize().width/2, pOpen56Bg->getContentSize().height/4+7));
    pOpen56Bg -> addChild(m_pLabel56Time);
    
    //我的押注、玩家总注
    CCSprite* quizBg1 = CCSprite::createWithSpriteFrameName("dog_quiz_bottom.png");
    quizBg1 -> setPosition(ccp(275+pCaichiBg->getContentSize().width+quizBg1->getContentSize().width/2, pBottomBg->getContentSize().height+40));
    m_pDogBg -> addChild(quizBg1);
    CCSprite* myQuiz = CCSprite::createWithSpriteFrameName("dog_quiz_money.png");
    myQuiz -> setPosition(ccp(myQuiz->getContentSize().width-10, quizBg1->getContentSize().height/4*3));
    quizBg1 -> addChild(myQuiz);
    m_pLabelOwnBete = CCLabelBMFont::create("", "dog/dog_bet_fnt1.fnt");
    m_pLabelOwnBete -> setAnchorPoint(ccp(0, 0.5f));
    m_pLabelOwnBete -> setPosition(ccp(quizBg1->getContentSize().width/2-40, quizBg1->getContentSize().height/4*3-4));
    quizBg1 -> addChild(m_pLabelOwnBete);
    CCSprite* quizBg2 = CCSprite::createWithSpriteFrameName("dog_quiz_bottom.png");
    quizBg2 -> setPosition(ccp(275+pCaichiBg->getContentSize().width+quizBg2->getContentSize().width/2, pBottomBg->getContentSize().height+5));
    m_pDogBg -> addChild(quizBg2);
    CCSprite* allQuiz = CCSprite::createWithSpriteFrameName("dog_all_quiz_money.png");
    allQuiz -> setPosition(ccp(allQuiz->getContentSize().width-10, quizBg2->getContentSize().height/4*3));
    quizBg2 -> addChild(allQuiz);
    m_pLabelAllBete = CCLabelBMFont::create("", "dog/dog_bet_fnt1.fnt");
    m_pLabelAllBete -> setAnchorPoint(ccp(0, 0.5f));
    m_pLabelAllBete -> setPosition(ccp(quizBg2->getContentSize().width/2-40, quizBg2->getContentSize().height/4*3-4));
    quizBg2 -> addChild(m_pLabelAllBete);
    
    //bete
    int index = 0;
    for(int i = 0; i < 5; i++)
    {
        for(int j = i; j < 5; j++)
        {
            CCSprite* pBeteNormal = CCSprite::createWithSpriteFrameName("dog_bet_btn_normal.png");
            pBeteNormal -> setPosition(ccp(m_pDogBg->getContentSize().width/4+72+(175*j), m_pDogBg->getContentSize().height-198-(78*i)));
            m_pDogBg -> addChild(pBeteNormal);
            m_arrBeteNormal -> addObject(pBeteNormal);
            CCSprite* pBeteDown = CCSprite::createWithSpriteFrameName("dog_bet_btn_down.png");
            pBeteDown -> setPosition(ccp(m_pDogBg->getContentSize().width/4+72+(175*j), m_pDogBg->getContentSize().height-198-(78*i)));
            m_pDogBg -> addChild(pBeteDown);
            pBeteDown -> setOpacity(0);
            m_arrBeteDown -> addObject(pBeteDown);
            pBeteNormal -> setTag(401+index);
            if(index < 15)
            {
                CCLabelBMFont* pMultiple = CCLabelBMFont::create(DogMultiple[index], "dog/bairen_gongyong.fnt");
                pMultiple -> setPosition(ccp(80, 38));
                pMultiple -> setAnchorPoint(ccp(0, 0));
                pBeteDown -> addChild(pMultiple);
                CCLabelBMFont* pChampion = CCLabelBMFont::create(DogChampion[index], "dog/bairen_qianliangminxuhao.fnt");
                pChampion -> setPosition(ccp(40, 48));
                pBeteDown -> addChild(pChampion);
                index++;
                CCLabelBMFont* pBeteLabel = CCLabelBMFont::create("", "dog/dog_bet_fnt2.fnt");
                pBeteLabel -> setPosition(ccp(pBeteDown->getContentSize().width/2, 20));
                pBeteDown -> addChild(pBeteLabel);
                pBeteLabel -> setTag(BETE_MONEY_TAG);
            }
            
        }
    }
    
    this -> schedule(schedule_selector(DogBeteLayer::scheduleBetTime), 1.0f);
    this -> schedule(schedule_selector(DogBeteLayer::schedule56PrizeTime), 1.0f);
}

void DogBeteLayer::scheduleBetTime()
{
    m_nBetTime--;
    if(m_nBetTime < 1)
    {
        this -> unschedule(schedule_selector(DogBeteLayer::scheduleBetTime));
        //进入小狗跑的界面
//        DogRunLayer* pDogRunLayer = DogRunLayer::create();
//        this -> addChild(pDogRunLayer, 200);
//        pDogRunLayer -> setMeBetMoney();
        return ;
    }
    char temp[16];
    sprintf(temp, "%d", m_nBetTime);
    m_pLabelBetTime -> setString(temp);
}

void DogBeteLayer::clickRebet(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    int nTag = item->getTag();
    if(nTag != 199)
    {
        item -> stopAllActions();
        item -> runAction(CCSequence::create(CCScaleTo::create(0.1f, 1.1f), CCScaleTo::create(0.1f, 1.0f), NULL));
    }
    switch (nTag) {
        case 199://押注按钮
        {
            break;
        }
        case 200://10万押注按钮
        {
            m_nCurrentBete = 100000;
            m_pBetGuang -> setPosition(ccp(651, 33));
            break;
        }
        case 201://5万押注按钮
        {
            m_nCurrentBete = 50000;
            m_pBetGuang -> setPosition(ccp(554, 33));
            break;
        }
        case 202://1万押注按钮
        {
            m_nCurrentBete = 10000;
            m_pBetGuang -> setPosition(ccp(457, 33));
            break;
        }
        case 203://2000押注按钮
        {
            m_nCurrentBete = 2000;
            m_pBetGuang -> setPosition(ccp(360, 33));
            break;
        }
        default:
            break;
    }
}

//充值按钮
void DogBeteLayer::clickChargeMenu(CCObject* object)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_EFFECT);
    CCMenuItem* item = (CCMenuItem*)object;
    item->stopAllActions();
    item->runAction(CCSequence::create(CCScaleTo::create(0.1f, 1.1f), CCScaleTo::create(0.1f, 1.0f), NULL));
}

void DogBeteLayer::schedule56PrizeTime()
{
    char temp[32];
    m_n56Time++;
    if(m_n56Time <= 60)
    {
        sprintf(temp, "%d秒前", m_n56Time);
    }
    else if(m_n56Time <= 3600)
    {
        int nMinute = m_n56Time/60;
        //int nSecond = m_n56Time%60;
        sprintf(temp, "%d分钟前", nMinute);
    }
    else
    {
        int nHour = m_n56Time/3600;
        int nMinute = (m_n56Time%3600)/60;
        sprintf(temp, "%d小时%d分钟前", nHour, nMinute);
    }
    m_pLabel56Time -> setString(temp);
}

void DogBeteLayer::changeDogBigPrize(LONGLONG lBigPrize)
{
    int a[10];
    memset(a, 0, sizeof(a));
    for (int i=0; i<10; i++)
    {
        int p = (int)pow(10, 9-i);
        a[i] = (int)((lBigPrize/p)%10);
    }
    for (int i=0; i<10; i++)
    {
        if (m_pDogBigPrize[i] == NULL)
        {
            return;
        }
        m_pDogBigPrize[i]->setVisible(true);
        m_pDogBigPrize[i]->stopAllActions();
        int yPosDest = 22+44*a[i];
        CCMoveTo* moveTo = CCMoveTo::create(1, ccp(m_pDogBigPrize[i]->getPositionX(), yPosDest));
        m_pDogBigPrize[i]->runAction(moveTo);
    }
}

CCAnimate* DogBeteLayer::getDogAnimate(std::string sFirstName, int nFirstCount, int nTotalCount, float fDelayTime)
{
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCArray* array = CCArray::create();
    char temp[32];
    for(int i = nFirstCount; i < nFirstCount+nTotalCount; i++)
    {
        sprintf(temp, "%s%d.png", sFirstName.c_str(), i);
        CCSpriteFrame* frame = cache->spriteFrameByName(temp);
        array -> addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(array, fDelayTime);
    CCAnimate* animate = CCAnimate::create(animation);
    return animate;
}

void DogBeteLayer::setDogChampion(int nFirst, int nSecond)//设置冠军亚军，显示特效
{
    if(nFirst >= 1 && nFirst <= 6 && nSecond >= 1 && nSecond <= 6)
    {
        for(int i = 0; i < m_arrChampion->count(); i++)
        {
            CCSprite* sprite = (CCSprite*)m_arrChampion->objectAtIndex(i);
            if(sprite == NULL) continue;
            if(i == nFirst || i == nSecond)
            {
                sprite -> setVisible(true);
                //播放星星特效
            }
            else
            {
                sprite -> setVisible(false);
            }
        }
    }
}

void DogBeteLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
}

bool DogBeteLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    m_pStartPoint = m_pDogBg->convertToNodeSpace(pTouch->getLocation());
    return true;
}

void DogBeteLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
//    if(m_nBetTime < 3)//游戏开始前3秒内不能押注
//        return ;
    if(!m_bBetStart)
        return ;
    m_pEndPoint = m_pDogBg->convertToNodeSpace(pTouch->getLocation());
    for(int i = 0; i < m_arrBeteNormal->count(); i++)
    {
        CCSprite* pNormal = (CCSprite*)m_arrBeteNormal->objectAtIndex(i);
        if(pNormal == NULL) continue;
        CCRect rect = pNormal->boundingBox();
        if(rect.containsPoint(m_pEndPoint) && rect.containsPoint(m_pStartPoint))
        {
            CCSprite* pDown = (CCSprite*)m_arrBeteDown->objectAtIndex(i);
            if(pDown == NULL) continue;
            pDown -> setOpacity(255);
            CCLabelBMFont* pBeteMoney = (CCLabelBMFont*)pDown->getChildByTag(BETE_MONEY_TAG);
            if(pBeteMoney == NULL) continue;
            int nBeteMoney = atoi(pBeteMoney->getString());
            int nUserScore = atoi(m_pLabelUserScore->getString());
            if(nUserScore < nBeteMoney)
            {
                CMainLogic::sharedMainLogic()->ShowMessage("亲，您当前金币不足，不能压此注！", eMBOK);
                return ;
            }
            m_nOwnOneBete[i] += m_nCurrentBete;
            char temp[16];
            sprintf(temp, "%d", m_nOwnOneBete[i]);
            pBeteMoney -> setString(temp);
            
            //我的总押注
            m_llOwnBete += m_nCurrentBete;
            std::string sOwnBete = getStringByScore(m_llOwnBete);
            m_pLabelOwnBete -> setString(sOwnBete.c_str());
            //发送押注消息
            if(i < AREA_ALL)
            {
                CMD_C_PlayerBet PlayerBet = {0};
                PlayerBet.lBetScore[i] = m_nCurrentBete;
                m_pClientKernel->SendSocketData(MDM_GF_GAME,SUB_C_PLAYER_BET,&PlayerBet,sizeof(PlayerBet));
            }
            
            break;
        }
    }
}

std::string DogBeteLayer::getStringByScore(SCORE llBeteMoney)
{
//    char temp[32] = {'\0'};
//    char temp2[32] = {'\0'};
//    int i = 0;
//    SCORE llLast1 = 0;
//    SCORE llLast2 = 0;
//    for(i = 0; ; i++)
//    {
//        llLast2 = llBeteMoney/1000;
//        
//        if(llLast2 <= 0)
//        {
//            sprintf(temp, "%lld%s", llBeteMoney, temp2);
//            sprintf(temp2, "%s", temp);
//            break;
//        }
//        else
//        {
//            llLast1 = llBeteMoney%1000;
//            sprintf(temp, ",%03lld%s", llLast1, temp2);
//            sprintf(temp2, "%s", temp);
//        }
//        llBeteMoney = llBeteMoney/1000;
//    }
//
    int i = 0;
    std::string sRet = "";
    do
    {
        std::string s1;
        s1 += (char)('0' + llBeteMoney % 10);
        s1 += sRet;
        i++;
        llBeteMoney /= 10;
        if (i == 3 && llBeteMoney > 0)
        {
            s1 = "," + s1;
            i = 0;
        }
        sRet = s1;
    } while (llBeteMoney > 0);
    return sRet;
}

//ClientKernel
bool DogBeteLayer::OnEventSocket(const TCP_Command & Command, const void * pBuffer, WORD wDataSize)
{
    printf("%d, %s, %hu, %hu\n", __LINE__, __FUNCTION__, Command.wMainCmdID, Command.wSubCmdID);
    if (MDM_GF_GAME!=Command.wMainCmdID&&Command.wSubCmdID!=SUB_GF_SYSTEM_MESSAGE)
    {
        return false;
    }
    switch (Command.wSubCmdID)
    {
        case SUB_GF_SYSTEM_MESSAGE://公告
        {
            printf("跑狗王公告\n");
            return true;
        }
        case SUB_S_PLAYER_BET://下注成功
        {
            if(wDataSize < sizeof(CMD_S_PlayerBet))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_PlayerBet* pPlayerBet = (CMD_S_PlayerBet*)pBuffer;
            if(pPlayerBet != NULL)
            {
                printf("下注成功\n");
                return true;
            }
            
        }
        case SUB_S_PLAYER_BET_FAIL://下注失败
        {
            if(wDataSize < sizeof(CMD_S_PlayerBetFail))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_PlayerBetFail* pPlayerBetFail = (CMD_S_PlayerBetFail*)pBuffer;
            if(pPlayerBetFail != NULL)//0不在下注时间
            {
                printf("下注失败  %d, %d\n", pPlayerBetFail->wChairID, pPlayerBetFail->cbFailType);
                return true;
            }
            return true;
        }
        case SUB_S_BET_START://开始下注
        {
            printf("开始下注\n");
            m_bBetStart = true;
            break;
        }
        case SUB_S_BET_END://下注结束
        {
            printf("下注结束\n");
            m_bBetStart = false;
            break;
        }
        case SUB_S_HORSES_START://跑马开始
        {
            printf("跑马开始\n");
            if(wDataSize < sizeof(CMD_S_HorsesStart))
            {
                ERROR_LOG;
                return true;
            }
            CMD_S_HorsesStart* pHorsesStart = (CMD_S_HorsesStart*)pBuffer;
            if(pHorsesStart != NULL)
            {
                printf("time is %d\n", pHorsesStart->nTimeLeave);
                //进入小狗跑的界面
                DogRunLayer* pDogRunLayer = DogRunLayer::create();
                this -> addChild(pDogRunLayer, 200, 999);
                pDogRunLayer -> setMeBetMoney();
                pDogRunLayer -> setChampion(pHorsesStart->cbHorsesRanking[0]+1, pHorsesStart->cbHorsesRanking[1]+1);
                pDogRunLayer -> setCalcData(pHorsesStart->nPlayerMultiple, pHorsesStart->lPlayerWinning, pHorsesStart->lPlayerCaiChi, pHorsesStart->lPlayerReturnBet);
                pDogRunLayer -> setCalcRank(pHorsesStart->szNickName, pHorsesStart->lScore, pHorsesStart->lCaiChi);
                //420
                for(int i = 0; i < 6; i++)
                {
                    for(int j = 0; j < 10; j++)
                    {
                        pDogRunLayer->createRoadBlock(420*(j+1), 490-i*80, pHorsesStart->nWayItem[i][j]);
                    }
                }
                
            }
            break;
        }
        case SUB_S_CONTROL_SYSTEM://系统控制
        {
            printf("系统控制1\n");
            break;
        }
        case SUB_S_NAMED_HORSES://马屁冠名
        {
            printf("马屁冠名\n");
            break;
        }
        case SUB_S_HORSES_END://跑马结束
        {
            printf("跑马结束\n");
            if(wDataSize < sizeof(CMD_S_HorsesEnd))
            {
                ERROR_LOG;
                return true;
            }
            /*
             struct CMD_S_HorsesEnd
             {
             INT								nTimeLeave;							//剩余时间
             tagHistoryRecord				RecordRecord;						//历史记录
             INT								nWinCount[HORSES_ALL];				//全天赢的场次
             
             //倍数
             INT								nMultiple[AREA_ALL];				//区域倍数
             
             //玩家成绩
             LONGLONG						lPlayerWinning[AREA_ALL];			//玩家输赢
             LONGLONG						lPlayerBet[AREA_ALL];				//下注
             };
             */
            CMD_S_HorsesEnd* pHorsesEnd = (CMD_S_HorsesEnd*)pBuffer;
            if(pHorsesEnd != NULL)
            {
                printf("nTimeLeave is %d\n", pHorsesEnd->nTimeLeave);
            }
            DogRunLayer* pDogRunLayer = (DogRunLayer*)this->getChildByTag(999);
            if(pDogRunLayer != NULL)
            {
                DogCalcLayer* pCalcLayer = DogCalcLayer::create(pDogRunLayer->m_nChampion1, pDogRunLayer->m_nChampion2);
                pDogRunLayer -> addChild(pCalcLayer, 100);
                pCalcLayer -> setCalcRank(pDogRunLayer->m_sNameRank, pDogRunLayer->m_llScoreRank, pDogRunLayer->m_llCaiChiRank);
                pCalcLayer -> setLabel(pDogRunLayer->m_nBeilv, pDogRunLayer->m_llGetCoin, pDogRunLayer->m_llCaiChi, pDogRunLayer->m_llReturnBet);
            }
            
            break;
        }
        case SUB_S_MANDATOY_END://强制结束
        {
            printf("强制结束\n");
            break;
        }
        case SUB_S_ADMIN_COMMDN://系统控制
        {
            printf("系统控制2\n");
            break;
        }
    }
    return true;
}

bool DogBeteLayer::OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
    printf("%d, %s\n", __LINE__, __FUNCTION__);
    return true;
}

bool DogBeteLayer::OnEventUserLeave(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)
{
    printf("%d, %s\n", __LINE__, __FUNCTION__);
    return true;
}

bool DogBeteLayer::OnEventUserEnter(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser)
{
    printf("%d, %s\n", __LINE__, __FUNCTION__);
    return true;
}
