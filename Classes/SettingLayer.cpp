//
//  SettingLayer.cpp
//  CatchFish
//
//  Created by 河北腾游 on 15/3/12.
//
//

#include "header.h"

enum eSettingControlID
{
    eEffectTag=300,
    eSoundTag,
    eParticleTag,
    eShadowTag,
};

SettingLayer::SettingLayer()
{
    
}
SettingLayer::~SettingLayer()
{
    
}

bool SettingLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
    m_bParticle = CCUserDefault::sharedUserDefault()->getBoolForKey("particle", true);
    m_bShadow = CCUserDefault::sharedUserDefault()->getBoolForKey("shadow", true);
    m_fSound = CCUserDefault::sharedUserDefault()->getFloatForKey("sound", 1);
    m_fEffect = CCUserDefault::sharedUserDefault()->getFloatForKey("effect", 1);
    m_fOldEffect = m_fEffect;
    vSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(0, 0, 0, 190), vSize.width, vSize.height);
    this->addChild(pLayerColor);

    m_nodeRegister = CCSprite::create();
    m_nodeRegister->setPosition(ccp(vSize.width/2, vSize.height/2));
    this -> addChild(m_nodeRegister);
    
    CCSprite* pSpBg = CCSprite::create("setting/settingbg.png");
    pSpBg -> setPosition(ccp(0, 0));
    m_nodeRegister -> addChild(pSpBg);
    
    CCSprite* pSpTitle = CCSprite::create("setting/settingtitle.png");
    pSpTitle -> setPosition(ccp(pSpBg->getContentSize().width/2, pSpBg->getContentSize().height-pSpTitle->getContentSize().height/2-27));
    pSpBg -> addChild(pSpTitle);
    
    yinyueSlider = CCControlSlider::create("setting/effort2.png", "setting/effort1.png", "setting/effortBtn.png");
    effortSlider = CCControlSlider::create("setting/effort2.png", "setting/effort1.png", "setting/effortBtn.png");
    yinyueSlider->setPosition(ccp(pSpBg->getContentSize().width/2+60, pSpBg->getContentSize().height/5*4-72));
    effortSlider->setPosition(ccp(pSpBg->getContentSize().width/2+60, pSpBg->getContentSize().height/5*3-53));
    yinyueSlider->setTag(eSoundTag);
    effortSlider->setTag(eEffectTag);
    yinyueSlider->setValue(0.5f);
    effortSlider->setValue(0.7f);
    yinyueSlider->setMaximumValue(1);
    effortSlider->setMaximumValue(1);
    yinyueSlider->setMinimumValue(0);
    effortSlider->setMinimumValue(0);
    yinyueSlider->setTouchPriority(-200);
    effortSlider->setTouchPriority(-200);
    yinyueSlider->setValue(m_fSound);
    effortSlider->setValue(m_fEffect);
    yinyueSlider->addTargetWithActionForControlEvents(this,cccontrol_selector(SettingLayer::sliderValueChanged),CCControlEventValueChanged);
    effortSlider->addTargetWithActionForControlEvents(this,cccontrol_selector(SettingLayer::sliderValueChanged),CCControlEventValueChanged);
    pSpBg->addChild(yinyueSlider);
    pSpBg->addChild(effortSlider);
    
    CCSprite * pOnZT = CCSprite::create("setting/onZT.png");
    CCSprite * pOffZT = CCSprite::create("setting/offZT.png");
    CCSprite * pON = CCSprite::create("setting/yinyueOn.png");
    CCSprite * pOFF = CCSprite::create("setting/yinyueOff.png");
    pOnZT->setPosition(ccp(pON->getContentSize().width/2 - 10, pON->getContentSize().height/2));
    pOffZT->setPosition(ccp(pOFF->getContentSize().width/2+8, pOFF->getContentSize().height/2));
    pON->addChild(pOnZT);
    pOFF->addChild(pOffZT);
    CCSprite * pYinYueBtn = CCSprite::create("setting/yinyueBtn1.png");
    CCSprite * pSwitchMark = CCSprite::create("setting/switch_mark.png");
    CCControlSwitch * m_switchShadow = CCControlSwitch::create(pSwitchMark, pON, pOFF, pYinYueBtn);
    m_switchShadow -> setTag(eShadowTag);
    m_switchShadow->setPosition(ccp(pSpBg->getContentSize().width/3+15, pSpBg->getContentSize().height/4+28));
    m_switchShadow -> addTargetWithActionForControlEvents(this, cccontrol_selector(SettingLayer::switchValueChanged), CCControlEventValueChanged);
    m_switchShadow -> setOn(m_bShadow);
    m_switchShadow->setTouchPriority(-200);
    pSpBg->addChild(m_switchShadow);
    
    CCSprite * onRight = CCSprite::create("setting/onZT.png");
    CCSprite * offRight = CCSprite::create("setting/offZT.png");
    CCSprite * pONRight = CCSprite::create("setting/yinyueOn.png");
    CCSprite * pOFFRight = CCSprite::create("setting/yinyueOff.png");
    CCSprite* pBgDi = CCSprite::create("setting/switch_mark.png");
    onRight->setPosition(ccp(pONRight->getContentSize().width/2 - 10, pONRight->getContentSize().height/2));
    offRight->setPosition(ccp(pOFFRight->getContentSize().width/2+8, pOFFRight->getContentSize().height/2));
    pONRight->addChild(onRight);
    pOFFRight->addChild(offRight);
    CCSprite * effortBtn = CCSprite::create("setting/yinyueBtn.png");
    
    CCControlSwitch * m_switchParticle = CCControlSwitch::create(pBgDi, pONRight, pOFFRight, effortBtn);
    m_switchParticle -> setTag(eParticleTag);
    m_switchParticle->setPosition(ccp(pSpBg->getContentSize().width/4*3, pSpBg->getContentSize().height/4+28));
    m_switchParticle -> addTargetWithActionForControlEvents(this, cccontrol_selector(SettingLayer::switchValueChanged), CCControlEventValueChanged);
    m_switchParticle -> setOn(m_bParticle);
    m_switchParticle->setTouchPriority(-200);
    pSpBg->addChild(m_switchParticle);
    
    CCMenuItemImage * closeItem = CCMenuItemImage::create("Dragon/dragoncloseitem.png","Dragon/dragoncloseitem.png",this,menu_selector(SettingLayer::closeWnd));
    CCMenu * closeMenu = CCMenu::create(closeItem,NULL);
    closeMenu->setTouchPriority(-200);
    closeMenu->setPosition(ccp(pSpBg->getContentSize().width-20, pSpBg->getContentSize().height-20));
    pSpBg->addChild(closeMenu,2);
    closeItem->setScale(1.2f);
    
    //pSpBg -> setScale(1.2f);
    
    this->setTouchEnabled(true);
    
    showSettingWnd();
    
    openTime();
    
    return true;
}

void SettingLayer::sliderValueChanged(CCObject* sender, CCControlEvent controlEvent)
{
    CCControlSlider* slider = (CCControlSlider*)sender;
    switch (slider->getTag()) {
        case eSoundTag:
        {
            m_fSound = slider->getValue();
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_fSound);
            break;
        }
        case eEffectTag:
        {
            m_fEffect = slider->getValue();
            break;
        }
        default:
            break;
    }
}

void SettingLayer::switchValueChanged(CCObject* sender, CCControlEvent controlEvent)
{
    CCControlSwitch* pSwitch = (CCControlSwitch*)sender;
    switch (pSwitch->getTag()) {
        case eShadowTag:
        {
            if(pSwitch->isOn())
            {
                m_bShadow = true;
                if(GameMainScene::_instance != NULL)
                {
                    GameMainScene::GetInstance()->setFishShadow(true);
                    GameMainScene::GetInstance()->boolAppearShadow(true);
                }
            }
            else
            {
                m_bShadow = false;
                if(GameMainScene::_instance != NULL)
                {
                    GameMainScene::GetInstance()->setFishShadow(false);
                    GameMainScene::GetInstance()->boolAppearShadow(false);
                }
            }
            break;
        }
        case eParticleTag:
        {
            if(pSwitch->isOn())
            {
                m_bParticle = true;
                if(GameMainScene::_instance != NULL)
                {
                    GameMainScene::GetInstance()->setParticle(true);
                }
            }
            else
            {
                m_bParticle = false;
                if(GameMainScene::_instance != NULL)
                {
                    GameMainScene::GetInstance()->setParticle(false);
                }
            }
            break;
        }
        default:
            break;
    }
}

void SettingLayer::closeWnd(CCObject * obj)
{
    SimpleAudioEngine::sharedEngine()->playEffect(BUTTON_CLOSE_EFFECT);
    CCUserDefault::sharedUserDefault()->setFloatForKey("sound", m_fSound);
    CCUserDefault::sharedUserDefault()->setFloatForKey("effect", m_fEffect);
    CCUserDefault::sharedUserDefault()->setBoolForKey("shadow", m_bShadow);
    CCUserDefault::sharedUserDefault()->setBoolForKey("particle", m_bParticle);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(SettingLayer::removeWnd));
    m_nodeRegister->runAction(CCSequence::create(CCScaleTo::create(0.15f, 1.2f), CCScaleTo::create(0.06f, 0.3f), func, NULL));
}

void SettingLayer::removeWnd()
{
    this->removeFromParent();
}

void SettingLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
}

bool SettingLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

void SettingLayer::showSettingWnd()
{
    m_nodeRegister->runAction(CCSequence::create(CCScaleTo::create(0, 0.8f),CCScaleTo::create(0.1f, 1.08f), CCScaleTo::create(0.07f, 1.0f), NULL));
}

void SettingLayer::setEffectValume()
{
    if(m_fOldEffect != m_fEffect)
    {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_fEffect);
        m_fOldEffect = m_fEffect;
    }
}

void SettingLayer::openTime()
{
    this -> schedule(schedule_selector(SettingLayer::setEffectValume), 1.0f);
}