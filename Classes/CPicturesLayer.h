//
//  CPicturesLayer.h
//  CatchFish
//
//  Created by 河北腾游 on 15/6/24.
//
//

#ifndef __CatchFish__CPicturesLayer__
#define __CatchFish__CPicturesLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "uploadFile.h"
USING_NS_CC_EXT;
USING_NS_CC;



class PicturesLayer : public CCLayer{
public:
    
    PicturesLayer();
    
    ~PicturesLayer();
    
    virtual bool init();
    
    CREATE_FUNC(PicturesLayer);
    
    void selectedMenu(CCObject* object);
    void uploadMenu(CCObject* object);
    void closeMenu(CCObject* object);
    
    void uploadImage();
    
    void update2(float f);
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    
    
    CCMenu* m_Menu;
    
    uploadFile* pUploadFile;
    
    CCClippingNode* m_pClippingNode;
    CCSprite* m_pSpBg;
};


#endif /* defined(__CatchFish__CPicturesLayer__) */
