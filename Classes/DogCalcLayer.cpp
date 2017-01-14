#include "header.h"

const char* AnimalName1[6] = {"animal1/gaojiasuo_daiji1.png", "animal3/jinmao_daiji1.png", "animal4/samoye_daiji1.png", "animal6/shapi_daiji1.png", "animal7/taidi_daiji1.png", "animal8/lachang_daiji1.png"};
const char* AnimalName2[6] = {"animal1/gaojiasuo_daiji", "animal3/jinmao_daiji", "animal4/samoye_daiji", "animal6/shapi_daiji", "animal7/taidi_daiji", "animal8/lachang_daiji"};

DogCalcLayer::DogCalcLayer()
{
    
}

DogCalcLayer::~DogCalcLayer()
{
    
}

DogCalcLayer* DogCalcLayer::create(int nFirst, int nSecond)
{
    DogCalcLayer* pRet = new DogCalcLayer();
    if(pRet && pRet->init(nFirst, nSecond))
    {
        pRet -> autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool DogCalcLayer::init(int nFirst, int nSecond)
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_nFirst = nFirst;
    m_nSecond = nSecond;
    
    loadUI();
    
    this -> scheduleOnce(schedule_selector(DogCalcLayer::scheduleRemove), 5.0f);
    
    this -> setTouchEnabled(true);
    
    return true;
}

void DogCalcLayer::loadUI()
{
    float width = 900;
    float height = 460;
    m_pBg = CCScale9Sprite::createWithSpriteFrameName("dog_res_floorbox.png");
    m_pBg -> setContentSize(CCSizeMake(width, height));
    m_pBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-50));
    this -> addChild(m_pBg);
    CCScale9Sprite* pFloor = CCScale9Sprite::createWithSpriteFrameName("dog_floor.png");
    pFloor -> setContentSize(CCSizeMake(width-20, height-20));
    pFloor -> setPosition(ccp(m_pBg->getContentSize().width/2, m_pBg->getContentSize().height/2));
    m_pBg -> addChild(pFloor);
    CCSprite* pTitle1 = CCSprite::createWithSpriteFrameName("dog_biaotu.png");
    pTitle1 -> setPosition(ccp(m_pBg->getContentSize().width/2, m_pBg->getContentSize().height+10));
    pTitle1 -> setAnchorPoint(ccp(0, 0.5f));
    m_pBg -> addChild(pTitle1);
    CCSprite* pTitle2 = CCSprite::createWithSpriteFrameName("dog_biaotu.png");
    pTitle2 -> setPosition(ccp(m_pBg->getContentSize().width/2, m_pBg->getContentSize().height+10));
    pTitle2 -> setAnchorPoint(ccp(1, 0.5f));
    m_pBg -> addChild(pTitle2);
    pTitle2 -> setFlipX(true);
    CCSprite* pTitle3 = CCSprite::createWithSpriteFrameName("dog_res_star.png");
    pTitle3 -> setPosition(ccp(m_pBg->getContentSize().width/2-46, m_pBg->getContentSize().height+20));
    m_pBg -> addChild(pTitle3);
    CCScale9Sprite* pAnimalBox = CCScale9Sprite::createWithSpriteFrameName("dog_res_animalbox.png");
    pAnimalBox -> setContentSize(CCSizeMake(380, 384));
    pAnimalBox -> setPosition(ccp(pAnimalBox->getContentSize().width/2+26, m_pBg->getContentSize().height/2-14));
    m_pBg -> addChild(pAnimalBox);
    m_pAnimalBg1 = CCSprite::createWithSpriteFrameName("dog_res_animal.png");
    m_pAnimalBg1 -> setPosition(ccp(pAnimalBox->getContentSize().width/4+5, pAnimalBox->getContentSize().height-m_pAnimalBg1->getContentSize().height/2-15));
    pAnimalBox -> addChild(m_pAnimalBg1);
    m_pAnimalBg2 = CCSprite::createWithSpriteFrameName("dog_res_animal.png");
    m_pAnimalBg2 -> setPosition(ccp(pAnimalBox->getContentSize().width/4*3-5, pAnimalBox->getContentSize().height-m_pAnimalBg2->getContentSize().height/2-15));
    pAnimalBox -> addChild(m_pAnimalBg2);
    CCSprite* pGuang1 = CCSprite::createWithSpriteFrameName("dog_res_guang.png");
    pGuang1 -> setPosition(ccp(m_pAnimalBg1->getContentSize().width/2, m_pAnimalBg1->getContentSize().height/2));
    m_pAnimalBg1 -> addChild(pGuang1);
    CCSprite* pGuang2 = CCSprite::createWithSpriteFrameName("dog_res_guang.png");
    pGuang2 -> setPosition(ccp(m_pAnimalBg2->getContentSize().width/2, m_pAnimalBg2->getContentSize().height/2));
    m_pAnimalBg2 -> addChild(pGuang2);
    CCSprite* pAnimalDi1 = CCSprite::createWithSpriteFrameName("dog_res_animal_d.png");
    pAnimalDi1 -> setPosition(ccp(m_pAnimalBg1->getContentSize().width/2, pAnimalDi1->getContentSize().height/2));
    m_pAnimalBg1 -> addChild(pAnimalDi1);
    CCSprite* pAnimalDi2 = CCSprite::createWithSpriteFrameName("dog_res_animal_d.png");
    pAnimalDi2 -> setPosition(ccp(m_pAnimalBg2->getContentSize().width/2, pAnimalDi2->getContentSize().height/2));
    m_pAnimalBg2 -> addChild(pAnimalDi2);
    CCSprite* pGuanjun = CCSprite::createWithSpriteFrameName("dog_res_guanjun.png");
    pGuanjun -> setPosition(ccp(pAnimalDi1->getContentSize().width/2, pAnimalDi1->getContentSize().height/2));
    pAnimalDi1 -> addChild(pGuanjun);
    CCSprite* pYajun = CCSprite::createWithSpriteFrameName("dog_res_yajun.png");
    pYajun -> setPosition(ccp(pAnimalDi2->getContentSize().width/2, pAnimalDi2->getContentSize().height/2));
    pAnimalDi2 -> addChild(pYajun);
    CCSprite* pZi3 = CCSprite::createWithSpriteFrameName("dog_zi3.png");
    pZi3 -> setPosition(ccp(38, 170-5));
    pAnimalBox -> addChild(pZi3);
    pZi3 -> setAnchorPoint(ccp(0, 0.5f));
    CCSprite* pZi2 = CCSprite::createWithSpriteFrameName("dog_zi2.png");
    pZi2 -> setAnchorPoint(ccp(0, 0.5f));
    pZi2 -> setPosition(ccp(38, 85-5));
    pAnimalBox -> addChild(pZi2);
    CCSprite* pZi1 = CCSprite::createWithSpriteFrameName("dog_zi1.png");
    pZi1 -> setAnchorPoint(ccp(0, 0.5f));
    pZi1 -> setPosition(ccp(38, 126-5));
    pAnimalBox -> addChild(pZi1);
    m_pLabelTotal = CCLabelBMFont::create("总计：0", "dog/paogou_zongji.fnt");
    m_pLabelTotal -> setPosition(ccp(pAnimalBox->getContentSize().width/2, 30));
    pAnimalBox -> addChild(m_pLabelTotal);
    m_pLabelCaichi = CCLabelTTF::create("0", "Arial", 32);
    m_pLabelCaichi -> setAnchorPoint(ccp(0, 0.5f));
    m_pLabelCaichi -> setPosition(ccp(150, 80));
    pAnimalBox -> addChild(m_pLabelCaichi);
    m_pLabelGetCoin = CCLabelTTF::create("0", "Arial", 32);
    m_pLabelGetCoin -> setAnchorPoint(ccp(0, 0.5f));
    m_pLabelGetCoin -> setPosition(ccp(150, 121));
    pAnimalBox -> addChild(m_pLabelGetCoin);
    m_pLabelGetCoin -> setColor(ccc3(222, 90, 30));
    m_pLabelCaichi -> setColor(ccc3(222, 90, 30));
    char temp[8];
    sprintf(temp, "%d", m_nFirst);
    m_pLabelGuanjun = CCLabelTTF::create(temp, "Arial", 48);
    m_pLabelGuanjun -> setPosition(ccp(195, 165));
    pAnimalBox -> addChild(m_pLabelGuanjun);
    CCLabelTTF* pLabelXian = CCLabelTTF::create("-", "Arial", 48);
    pLabelXian -> setPosition(ccp(218, 165));
    pAnimalBox -> addChild(pLabelXian);
    pLabelXian -> setColor(ccc3(169, 106, 13));
    sprintf(temp, "%d", m_nSecond);
    m_pLabelYajun = CCLabelTTF::create(temp, "Arial", 48);
    m_pLabelYajun -> setPosition(ccp(240, 165));
    pAnimalBox -> addChild(m_pLabelYajun);
    CCSprite* pBeilvDi = CCSprite::createWithSpriteFrameName("dog_res_beishu.png");
    pBeilvDi -> setPosition(ccp(pAnimalBox->getContentSize().width-pBeilvDi->getContentSize().width/2-10, 165));
    pAnimalBox -> addChild(pBeilvDi);
    m_pLabelBeilv = CCLabelTTF::create("", "Arial", 32);
    m_pLabelBeilv -> setPosition(ccp(pBeilvDi->getContentSize().width/2, pBeilvDi->getContentSize().height/2));
    pBeilvDi -> addChild(m_pLabelBeilv);
    m_pLabelGuanjun -> setColor(ccc3(255, 0, 0));
    m_pLabelYajun -> setColor(ccc3(243, 38, 255));
    m_pLabelBeilv -> setColor(ccc3(248, 254, 179));
    
    //load animal
    if(m_nFirst>=1 && m_nFirst<=6 && m_nSecond>=1 && m_nSecond<=6)
    {
        CCSprite* pAnimalGuanjun = CCSprite::createWithSpriteFrameName(AnimalName1[m_nFirst-1]);
        pAnimalGuanjun -> setPosition(ccp(m_pAnimalBg1->getContentSize().width/2, m_pAnimalBg1->getContentSize().height/2));
        m_pAnimalBg1 -> addChild(pAnimalGuanjun);
        CCSprite* pAnimalYajun = CCSprite::createWithSpriteFrameName(AnimalName1[m_nSecond-1]);
        pAnimalYajun -> setPosition(ccp(m_pAnimalBg2->getContentSize().width/2, m_pAnimalBg2->getContentSize().height/2));
        m_pAnimalBg2 -> addChild(pAnimalYajun);
        pAnimalGuanjun -> runAction(CCRepeatForever::create(DogBeteLayer::_instance->getDogAnimate(AnimalName2[m_nFirst-1], 1, 3, 0.2)));
        pAnimalYajun -> runAction(CCRepeatForever::create(DogBeteLayer::_instance->getDogAnimate(AnimalName2[m_nSecond-1], 1, 3, 0.2)));
    }
    
    
}

void DogCalcLayer::setCalcRank(std::string sName[5], SCORE lScore[5], SCORE lCaiChi[5])
{
    CCLabelBMFont* pTitle = CCLabelBMFont::create("本局大赢家", "dog/dog_reward_fnt.fnt");
    pTitle -> setPosition(ccp(m_pBg->getContentSize().width/4*3-30, m_pBg->getContentSize().height-90));
    m_pBg -> addChild(pTitle);
    CCLabelBMFont* pXuhao = CCLabelBMFont::create("序号", "dog/dog_title_fnt.fnt");
    pXuhao -> setPosition(ccp(m_pBg->getContentSize().width/2-10, m_pBg->getContentSize().height-150));
    m_pBg -> addChild(pXuhao);
    CCLabelBMFont* pName = CCLabelBMFont::create("名字", "dog/dog_title_fnt.fnt");
    pName -> setPosition(ccp(m_pBg->getContentSize().width/2+110, m_pBg->getContentSize().height-150));
    m_pBg->addChild(pName);
    CCLabelBMFont* pGetCoin = CCLabelBMFont::create("获得金币", "dog/dog_title_fnt.fnt");
    pGetCoin -> setPosition(ccp(m_pBg->getContentSize().width/4*3+30, m_pBg->getContentSize().height-150));
    m_pBg -> addChild(pGetCoin);
    CCLabelBMFont* pCaichi = CCLabelBMFont::create("彩池奖励", "dog/dog_title_fnt.fnt");
    pCaichi -> setPosition(ccp(m_pBg->getContentSize().width-70, m_pBg->getContentSize().height-150));
    m_pBg -> addChild(pCaichi);
    char temp[16];
    for(int i = 0; i < 5; i++)
    {
        sprintf(temp, "%d", i+1);
        CCLabelTTF* pLabelRank = CCLabelTTF::create(temp, "Arial-BoldMT", 26);
        pLabelRank -> setPosition(ccp(m_pBg->getContentSize().width/2-10, m_pBg->getContentSize().height-200-50*i));
        m_pBg -> addChild(pLabelRank);
        pLabelRank -> setColor(ccc3(188, 127, 34));
        CCLabelTTF* pLabelName = CCLabelTTF::create(sName[i].c_str(), "Arial-BoldMT", 26);
        pLabelName -> setPosition(ccp(m_pBg->getContentSize().width/2+110, m_pBg->getContentSize().height-200-50*i));
        m_pBg -> addChild(pLabelName);
        pLabelName -> setColor(ccc3(188, 127, 34));
        sprintf(temp, "%lld", lScore[i]);
        CCLabelTTF* pLabelScore = CCLabelTTF::create(temp, "Arial-BoldMT", 26);
        pLabelScore -> setPosition(ccp(m_pBg->getContentSize().width/4*3+30, m_pBg->getContentSize().height-200-50*i));
        m_pBg -> addChild(pLabelScore);
        pLabelScore -> setColor(ccc3(188, 127, 34));
        sprintf(temp, "%lld", lCaiChi[i]);
        CCLabelTTF* pLabelCaichi = CCLabelTTF::create(temp, "Arial-BoldMT", 26);
        pLabelCaichi -> setPosition(ccp(m_pBg->getContentSize().width/-70, m_pBg->getContentSize().height-200-50*i));
        m_pBg -> addChild(pLabelCaichi);
        pLabelCaichi -> setColor(ccc3(188, 127, 34));
    }
    
}

void DogCalcLayer::scheduleRemove()
{
    this -> removeFromParent();//同时也删除跑道类
    DogRunLayer* pDogRunLayer = (DogRunLayer*)DogBeteLayer::getInstance()->getChildByTag(999);
    if(pDogRunLayer != NULL)
    {
        pDogRunLayer -> removeFromParent();
    }
}

void DogCalcLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool DogCalcLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint point = pTouch->getLocation();
    CCRect rect = m_pBg->boundingBox();
    if(!rect.containsPoint(point))
    {
        this -> unschedule(schedule_selector(DogCalcLayer::scheduleRemove));
        this -> removeFromParent();//同时也删除跑道类
        DogRunLayer* pDogRunLayer = (DogRunLayer*)DogBeteLayer::getInstance()->getChildByTag(999);
        if(pDogRunLayer != NULL)
        {
            pDogRunLayer -> removeFromParent();
        }
    }
    return true;
}

void DogCalcLayer::setLabel(int nBeilv, SCORE lScore, SCORE lCaiChi, SCORE lBet)
{
    char temp[16];
    sprintf(temp, "x%d", nBeilv);
    m_pLabelBeilv->setString(temp);
    sprintf(temp, "%lld", lScore+lBet);
    m_pLabelGetCoin->setString(temp);
    sprintf(temp, "%lld", lCaiChi);
    m_pLabelCaichi->setString(temp);
    sprintf(temp, "%lld", lScore+lCaiChi+lBet);
    m_pLabelTotal->setString(temp);
}