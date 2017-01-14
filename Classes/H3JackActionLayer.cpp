#include "header.h"

extern char* txName[10];

JackActionLayer::JackActionLayer()
{
    
}

JackActionLayer::~JackActionLayer()
{
    m_arrFuhao->release();
}

bool JackActionLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    m_nJackPos = 2*8+rand()%8+1;
    printf("m_nJackPos is %d\n", m_nJackPos);
    m_arrFuhao = CCArray::create();
    m_arrFuhao -> retain();
    m_node1 = CCNode::create();
    this -> addChild(m_node1);
    
    m_node2 = CCNode::create();
    this -> addChild(m_node2);
    m_node2 -> setVisible(false);
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), CLIENT_WIDTH, CLIENT_HEIGHT);
    m_node1->addChild(pLayerColor);
    
    loadUI();
    loadJackUI();
    
    this -> setTouchEnabled(true);
    
    return true;
}

void JackActionLayer::loadUI()
{
    CCSprite* spJiangchi = CCSprite::createWithSpriteFrameName("h3card_jiangchi.png");
    spJiangchi -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT-170));
    m_node1 -> addChild(spJiangchi);
    CCSprite* pJiangchi1 = CCSprite::createWithSpriteFrameName("h3card_jiangchi04.png");
    pJiangchi1 -> setPosition(ccp(spJiangchi->getContentSize().width/4*3, 21));
    spJiangchi -> addChild(pJiangchi1);
    CCSprite* pJiangchi2 = CCSprite::createWithSpriteFrameName("h3card_jiangchi04.png");
    pJiangchi2 -> setPosition(ccp(spJiangchi->getContentSize().width/4, 73));
    spJiangchi -> addChild(pJiangchi2);
    pJiangchi1 -> runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(-180, 0)), CCHide::create(), NULL));
    pJiangchi2 -> runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(180, 0)), CCHide::create(), NULL));
    ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE };
    pJiangchi1->setBlendFunc(cbl);
    pJiangchi2->setBlendFunc(cbl);
    
    char temp[32];
    sprintf(temp, "%lld", H3CardBeteLayer::_instance->m_llJackpot);
    CCLabelBMFont* pLabelJackpot = CCLabelBMFont::create(temp, "h3card/h3card_lotterypool_btn_pot.fnt");
    pLabelJackpot -> setPosition(ccp(spJiangchi->getContentSize().width/2, spJiangchi->getContentSize().height/3));
    spJiangchi -> addChild(pLabelJackpot);
    
    m_spRedDi = CCSprite::createWithSpriteFrameName("h3card_kaijiang_huangquan.png");
    m_spRedDi -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    m_node1 -> addChild(m_spRedDi);
    m_spRedDi -> setScale(0);
    m_spRedDi -> setVisible(false);
    CCSprite* spReddi = CCSprite::createWithSpriteFrameName("h3card_kaijiang_hongguang.png");
    spReddi -> setPosition(ccp(m_spRedDi->getContentSize().width/2, m_spRedDi->getContentSize().height/2));
    m_spRedDi -> addChild(spReddi);
    
    CCSprite* pJackLogo = CCSprite::createWithSpriteFrameName("h3card_kaijiang_title.png");
    pJackLogo -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2+60));
    m_node1 -> addChild(pJackLogo, 2);
    pJackLogo -> setScale(0.2f);
    pJackLogo -> runAction(CCScaleTo::create(0.2f, 1.0f));
    
    
    
    CCSprite* pXingDi = CCSprite::createWithSpriteFrameName("h3card_player_luckkuang.png");
    pXingDi -> setPosition(ccp(CLIENT_WIDTH/5*4+28.5, CLIENT_HEIGHT-212));
    m_node1 -> addChild(pXingDi);
    tagUserInfo* pUserInfoXing = CMainLogic::sharedMainLogic()->SearchUserByUserID(H3CardBeteLayer::_instance->m_dwPlayerUserID[0]);
    CCSprite* pTxXing = NULL;
    if(pUserInfoXing != NULL && pUserInfoXing->wFaceID-1>=0 && pUserInfoXing->wFaceID-1<10)
    {
        pTxXing = CCSprite::create(txName[pUserInfoXing->wFaceID-1]);
    }
    else
    {
        pTxXing = CCSprite::create(txName[0]);
    }
    pTxXing -> setPosition(ccp(pXingDi->getContentSize().width/2, pXingDi->getContentSize().height/2));
    pXingDi -> addChild(pTxXing);
    pTxXing -> setScale(0.56f);
    
    CCSprite* pFuhao1 = CCSprite::createWithSpriteFrameName("h3card_player_fuhaokuang.png");
    pFuhao1 -> setPosition(ccp(CLIENT_WIDTH/5-18.5, CLIENT_HEIGHT-212));
    m_node1 -> addChild(pFuhao1, 20);
    tagUserInfo* pUserInfo1 = CMainLogic::sharedMainLogic()->SearchUserByUserID(H3CardBeteLayer::_instance->m_dwPlayerUserID[1]);
    CCSprite* pTx = NULL;
    if(pUserInfo1 != NULL && pUserInfo1->wFaceID-1>=0 && pUserInfo1->wFaceID-1<10)
    {
        pTx = CCSprite::create(txName[pUserInfo1->wFaceID-1]);
    }
    else
    {
        pTx = CCSprite::create(txName[0]);
    }
    pTx -> setPosition(ccp(pFuhao1->getContentSize().width/2, pFuhao1->getContentSize().height/2));
    pFuhao1 -> addChild(pTx);
    pTx -> setScale(0.56f);
    
    CCSprite* pFuhao2 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    pFuhao2 -> setPosition(ccp(CLIENT_WIDTH/5-34, CLIENT_HEIGHT-333));
    m_node1 -> addChild(pFuhao2, 20);
    CCSprite* pFuhao3 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    pFuhao3 -> setPosition(ccp(CLIENT_WIDTH/5-34, CLIENT_HEIGHT-433));
    m_node1 -> addChild(pFuhao3, 20);
    CCSprite* pFuhao4 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    pFuhao4 -> setPosition(ccp(CLIENT_WIDTH/5-34, CLIENT_HEIGHT-533));
    m_node1 -> addChild(pFuhao4, 20);
    CCSprite* pFuhao5 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    pFuhao5 -> setPosition(ccp(CLIENT_WIDTH/5*4+35, CLIENT_HEIGHT-333));
    m_node1 -> addChild(pFuhao5, 20);
    CCSprite* pFuhao6 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    pFuhao6 -> setPosition(ccp(CLIENT_WIDTH/5*4+35, CLIENT_HEIGHT-433));
    m_node1 -> addChild(pFuhao6, 20);
    CCSprite* pFuhao7 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    pFuhao7 -> setPosition(ccp(CLIENT_WIDTH/5*4+35, CLIENT_HEIGHT-533));
    m_node1 -> addChild(pFuhao7, 20);
    
    m_arrFuhao -> addObject(pXingDi);
    m_arrFuhao -> addObject(pFuhao5);
    m_arrFuhao -> addObject(pFuhao6);
    m_arrFuhao -> addObject(pFuhao7);
    m_arrFuhao -> addObject(pFuhao4);
    m_arrFuhao -> addObject(pFuhao3);
    m_arrFuhao -> addObject(pFuhao2);
    m_arrFuhao -> addObject(pFuhao1);
    pXingDi -> setVisible(false);
    pFuhao1 -> setVisible(false);
    pFuhao2 -> setVisible(false);
    pFuhao3 -> setVisible(false);
    pFuhao4 -> setVisible(false);
    pFuhao5 -> setVisible(false);
    pFuhao6 -> setVisible(false);
    pFuhao7 -> setVisible(false);
    
    int nIndex[8] = {0, 5, 6, 7, 4, 3, 2, 1};
    for(int i = 0; i < m_arrFuhao->count(); i++)
    {
        CCSprite* spdi = (CCSprite*)m_arrFuhao->objectAtIndex(i);
        if(spdi != NULL)
        {
            tagUserInfo* pUserInfo = CMainLogic::sharedMainLogic()->SearchUserByUserID(H3CardBeteLayer::_instance->m_dwPlayerUserID[nIndex[i]]);
            if(pUserInfo != NULL && pUserInfo->wFaceID-1>=0 && pUserInfo->wFaceID-1<10)
            {
                if(i != 0 && i != 7)
                {
                    CCSprite* pTx = CCSprite::create(txName[pUserInfo->wFaceID-1]);
                    pTx -> setPosition(ccp(spdi->getContentSize().width/2, spdi->getContentSize().height/2-8));
                    spdi -> addChild(pTx);
                    pTx -> setScale(0.56f);
                    CCLabelTTF* pFuhaoName = CCLabelTTF::create(getNickName(CCharCode::GB2312ToUTF8(pUserInfo->szNickName)).c_str(), "Arial", 18);
                    pFuhaoName -> setPosition(ccp(spdi->getContentSize().width/2, spdi->getContentSize().height-10));
                    spdi -> addChild(pFuhaoName);
                }
            }
            else
            {
                CCSprite* pTx = CCSprite::create(txName[0]);
                pTx -> setPosition(ccp(spdi->getContentSize().width/2, spdi->getContentSize().height/2-8));
                spdi -> addChild(pTx);
                pTx -> setScale(0.56f);
                if(i != 0 && i != 7)
                {
                    CCLabelTTF* pFuhaoName = CCLabelTTF::create("w222222", "Arial", 18);
                    pFuhaoName -> setPosition(ccp(spdi->getContentSize().width/2, spdi->getContentSize().height-10));
                    spdi -> addChild(pFuhaoName);
                }
            }
        }
    }
    showZhuanAction();
    this -> schedule(schedule_selector(JackActionLayer::showZhuanAction), 0.4f);
}

void JackActionLayer::showZhuanAction()
{
    static int nZhuanTime = 0;
    nZhuanTime++;
    CCSprite* spdi = (CCSprite*)m_arrFuhao->objectAtIndex((nZhuanTime-1)%8);
    if(spdi != NULL)
    {
        float fTime = 0;
        if(nZhuanTime+3<m_nJackPos)
        {
            fTime = (m_nJackPos-nZhuanTime-3)/10.0f;
        }
        else
        {
            fTime = abs(m_nJackPos/2-nZhuanTime)/20.0f * abs(m_nJackPos/2-nZhuanTime)/20.0f;

        }
        if(nZhuanTime == m_nJackPos)
        {
            nZhuanTime = 0;
            spdi -> runAction(CCSequence::create(CCDelayTime::create(fTime), CCShow::create(), CCDelayTime::create(0.2f), CCCallFuncN::create(this, callfuncN_selector(JackActionLayer::showStopAction)), NULL));
            this -> unschedule(schedule_selector(JackActionLayer::showZhuanAction));
            return ;
        }
        spdi -> stopAllActions();
        spdi -> runAction(CCSequence::create(CCDelayTime::create(fTime), CCShow::create(), CCDelayTime::create(0.7f), CCHide::create(), NULL));
    }
}

void JackActionLayer::showStopAction(CCNode* node)
{
    CCSprite* spdi = (CCSprite*)node;
    if(spdi == NULL)
        return ;
//    CCSprite* spguang1 = CCSprite::createWithSpriteFrameName("h3card_wanjia_xuanzhuan.png");
//    spguang1 -> setPosition(ccp(spdi->getContentSize().width/2, spdi->getContentSize().height/2));
//    spdi -> addChild(spguang1, 4);
//    spguang1 -> setColor(ccYELLOW);
//    ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE };
//    spguang1->setBlendFunc(cbl);
    CCSprite* spguang2 = CCSprite::createWithSpriteFrameName("h3card_wanjia_xuanzhuan2.png");
    spguang2 -> setPosition(ccp(spdi->getContentSize().width/2, spdi->getContentSize().height/2));
    spdi -> addChild(spguang2);
    //spguang2 -> runAction(CCRepeat::create(CCSequence::create(CCScaleTo::create(0, 1.0f),CCSpawn::create(CCScaleTo::create(0.25f, 1.3f), CCFadeOut::create(0.25f), NULL), NULL), 5));
    spguang2 -> runAction(CCSequence::create(CCBlink::create(1.4f, 5), CCCallFuncN::create(this, callfuncN_selector(JackActionLayer::showJackAnimate)), NULL));
}

void JackActionLayer::showJackAnimate(CCNode* node)
{
    CCSprite* sprite = (CCSprite*)node;
    if(sprite == NULL)
        return ;
    sprite -> removeFromParent();
    
    char temp[32];
    sprintf(temp, "%d", 3067325);
    CCLabelBMFont* pLabelJackpot = CCLabelBMFont::create(temp, "h3card/h3card_lotterypool_btn_pot.fnt");
    pLabelJackpot -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-20));
    m_node1 -> addChild(pLabelJackpot);
    pLabelJackpot -> setScale(5.0f);
    pLabelJackpot -> runAction(CCSequence::create(CCDelayTime::create(0.2f), CCScaleTo::create(0.3f, 1.5f), CCScaleTo::create(0.1f, 2.0f), NULL));
    
    m_spRedDi -> setVisible(true);
    m_spRedDi -> runAction(CCScaleTo::create(0.3f, 1.0f));
    CCSprite* spCai = CCSprite::createWithSpriteFrameName("h3card_kaijiang_caidai.png");
    spCai -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2));
    spCai -> setScale(0);
    m_node1 -> addChild(spCai);
    CCCallFuncN* funcN = CCCallFuncN::create(this, callfuncN_selector(JackActionLayer::showJackUI));
    spCai -> runAction(CCSequence::create(CCDelayTime::create(1.0f), CCEaseSineOut::create(CCScaleTo::create(1.0f, 1.0f)), CCHide::create(), CCDelayTime::create(0.5f), funcN, NULL));
}

void JackActionLayer::showJackUI()
{
    m_node1 -> setVisible(false);
    m_node2 -> setVisible(true);
    
    CCCallFuncN* funcN = CCCallFuncN::create(this, callfuncN_selector(JackActionLayer::removeWnd));
    m_spBg -> runAction(CCSequence::create(CCScaleTo::create(0.2f, 1.0f), CCDelayTime::create(2.8f), funcN, NULL));
}

void JackActionLayer::loadJackUI()
{
    m_spBg = CCSprite::create("h3card/nopack_window_frame.png");
    m_spBg -> setPosition(ccp(CLIENT_WIDTH/2, CLIENT_HEIGHT/2-40));
    m_node2 -> addChild(m_spBg);
    CCSprite* pGuang = CCSprite::createWithSpriteFrameName("h3card_kaijiang_huangguang.png");
    pGuang -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/2));
    m_spBg -> addChild(pGuang, -1);
    //m_spBg->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
    CCSprite* pTitle = CCSprite::createWithSpriteFrameName("h3card_kaijiang_biaoti.png");
    pTitle -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height));
    m_spBg -> addChild(pTitle);
    pTitle -> setScale(0.68f);
    m_spBg -> setScale(0);
    
    CCScale9Sprite* pDi1 = CCScale9Sprite::create("h3card/nopack_caichi_msgbg.png");
    pDi1 -> setContentSize(CCSize(550, 162));
    pDi1 -> ignoreAnchorPointForPosition(false);
    pDi1 -> setAnchorPoint(ccp(0.5f, 0.5f));
    pDi1 -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/4*3-22));
    m_spBg -> addChild(pDi1);
    
    CCScale9Sprite* pDi2 = CCScale9Sprite::create("h3card/nopack_caichi_msgbg.png");
    pDi2 -> setContentSize(CCSize(550, 162));
    pDi2 -> ignoreAnchorPointForPosition(false);
    pDi2 -> setAnchorPoint(ccp(0.5f, 0.5f));
    pDi2 -> setPosition(ccp(m_spBg->getContentSize().width/2, m_spBg->getContentSize().height/4+5));
    m_spBg -> addChild(pDi2);
    
    CCSprite* pFirst = CCSprite::createWithSpriteFrameName("h3card_kaijiang_rank1.png");
    pFirst -> setPosition(ccp(40, pDi1->getContentSize().height/2));
    pDi1 -> addChild(pFirst);
    CCSprite* pSecond = CCSprite::createWithSpriteFrameName("h3card_kaijiang_rank2.png");
    pSecond -> setPosition(ccp(210, pDi1->getContentSize().height/2));
    pDi1 -> addChild(pSecond);
    CCSprite* pThird = CCSprite::createWithSpriteFrameName("h3card_kaijiang_rank3.png");
    pThird -> setPosition(ccp(390, pDi1->getContentSize().height/2));
    pDi1 -> addChild(pThird);
    CCSprite* pTx1 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    pTx1 -> setPosition(ccp(120, pDi1->getContentSize().height/2+10));
    pDi1 -> addChild(pTx1);
    CCSprite* pTx2 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    pTx2 -> setPosition(ccp(290, pDi1->getContentSize().height/2+10));
    pDi1 -> addChild(pTx2);
    CCSprite* pTx3 = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    pTx3 -> setPosition(ccp(470, pDi1->getContentSize().height/2+10));
    pDi1 -> addChild(pTx3);
    //玩家信息
    CCLabelTTF* pName1 = CCLabelTTF::create(getNickName(CMainLogic::sharedMainLogic()->m_sNickName).c_str(), "Arial", 18);
    pName1 -> setPosition(ccp(pTx1->getContentSize().width/2, pTx1->getContentSize().height-10));
    pTx1 -> addChild(pName1);
    if(CMainLogic::sharedMainLogic()->m_nTouxiangID >= 1 && CMainLogic::sharedMainLogic()->m_nTouxiangID <= 10)
    {
        CCSprite* spTx1 = CCSprite::create(txName[CMainLogic::sharedMainLogic()->m_nTouxiangID-1]);
        spTx1 -> setPosition(ccp(pTx1->getContentSize().width/2, pTx1->getContentSize().height/2-8));
        pTx1 -> addChild(spTx1);
        spTx1 -> setScale(0.56f);
    }
    CCLabelTTF* pName2 = CCLabelTTF::create(getNickName(CMainLogic::sharedMainLogic()->m_sNickName).c_str(), "Arial", 18);
    pName2 -> setPosition(ccp(pTx2->getContentSize().width/2, pTx2->getContentSize().height-10));
    pTx2 -> addChild(pName2);
    if(CMainLogic::sharedMainLogic()->m_nTouxiangID >= 1 && CMainLogic::sharedMainLogic()->m_nTouxiangID <= 10)
    {
        CCSprite* spTx2 = CCSprite::create(txName[CMainLogic::sharedMainLogic()->m_nTouxiangID-1]);
        spTx2 -> setPosition(ccp(pTx2->getContentSize().width/2, pTx2->getContentSize().height/2-8));
        pTx2 -> addChild(spTx2);
        spTx2 -> setScale(0.56f);
    }
    CCLabelTTF* pName3 = CCLabelTTF::create(getNickName(CMainLogic::sharedMainLogic()->m_sNickName).c_str(), "Arial", 18);
    pName3 -> setPosition(ccp(pTx3->getContentSize().width/2, pTx3->getContentSize().height-10));
    pTx3 -> addChild(pName3);
    if(CMainLogic::sharedMainLogic()->m_nTouxiangID >= 1 && CMainLogic::sharedMainLogic()->m_nTouxiangID <= 10)
    {
        CCSprite* spTx3 = CCSprite::create(txName[CMainLogic::sharedMainLogic()->m_nTouxiangID-1]);
        spTx3 -> setPosition(ccp(pTx3->getContentSize().width/2, pTx3->getContentSize().height/2-8));
        pTx3 -> addChild(spTx3);
        spTx3 -> setScale(0.56f);
    }
    
    CCLabelBMFont* pFont1 = CCLabelBMFont::create("1533662", "h3card/h3card_golden.fnt");
    pFont1 -> setPosition(ccp(120, 30));
    pDi1 -> addChild(pFont1);
    CCLabelBMFont* pFont2 = CCLabelBMFont::create("1533662", "h3card/h3card_golden.fnt");
    pFont2 -> setPosition(ccp(290, 30));
    pDi1 -> addChild(pFont2);
    CCLabelBMFont* pFont3 = CCLabelBMFont::create("1533662", "h3card/h3card_golden.fnt");
    pFont3 -> setPosition(ccp(470, 30));
    pDi1 -> addChild(pFont3);
    pFont1 -> setScale(0.75f);
    pFont2 -> setScale(0.75f);
    pFont3 -> setScale(0.75f);
    
    //自己信息
    CCLabelTTF* pLabel2 = CCLabelTTF::create("真遗憾，下次加油！", "Arial", 20);
    pLabel2 -> setPosition(ccp(pDi2->getContentSize().width/2, pDi2->getContentSize().height/2));
    pDi2 -> addChild(pLabel2);
    pLabel2 -> setVisible(false);
    
    CCLabelTTF* pLabel = CCLabelTTF::create("本场名次：第37名", "Arial", 20);
    pLabel -> setPosition(ccp(pDi2->getContentSize().width/4, pDi2->getContentSize().height/2));
    pDi2 -> addChild(pLabel);
    CCSprite* pTxMy = CCSprite::createWithSpriteFrameName("h3card_player_touxiangkuang.png");
    pTxMy -> setPosition(ccp(pDi2->getContentSize().width/2+15, pDi2->getContentSize().height/2));
    pDi2 -> addChild(pTxMy);
    CCSprite* sp4 = CCSprite::createWithSpriteFrameName("h3card_common_chouma.png");
    sp4 -> setPosition(ccp(360, pDi2->getContentSize().height/2));
    pDi2 -> addChild(sp4);
    CCLabelBMFont* pFontMy = CCLabelBMFont::create("1533662", "h3card/h3card_golden.fnt");
    pFontMy -> setPosition(ccp(380, pDi2->getContentSize().height/2));
    pFontMy -> setAnchorPoint(ccp(0, 0.5f));
    pDi2 -> addChild(pFontMy);
    pFontMy -> setScale(0.7f);
    
    CCLabelTTF* pName = CCLabelTTF::create(getNickName(CMainLogic::sharedMainLogic()->m_sNickName).c_str(), "Arial", 18);
    pName -> setPosition(ccp(pTxMy->getContentSize().width/2, pTxMy->getContentSize().height-10));
    pTxMy -> addChild(pName);
    if(CMainLogic::sharedMainLogic()->m_nTouxiangID >= 1 && CMainLogic::sharedMainLogic()->m_nTouxiangID <= 10)
    {
        CCSprite* pMyTx = CCSprite::create(txName[CMainLogic::sharedMainLogic()->m_nTouxiangID-1]);
        pMyTx -> setPosition(ccp(pTxMy->getContentSize().width/2, pTxMy->getContentSize().height/2-8));
        pTxMy -> addChild(pMyTx);
        pMyTx -> setScale(0.56f);
    }
}

std::string JackActionLayer::getNickName(std::string strName)
{
    if(strlen(strName.c_str()) > 9)
    {
        std::string str = strName.substr(0, 9) + "..";
        return str;
    }
    return strName;
}

void JackActionLayer::removeWnd()
{
    this -> removeFromParent();
}

void JackActionLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool JackActionLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    this -> removeFromParent();
    return true;
}
