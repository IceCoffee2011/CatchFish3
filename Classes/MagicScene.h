#ifndef __MAGIC_PARTICLES_SCENE_H__
#define __MAGIC_PARTICLES_SCENE_H__

#include "cocos2d.h"
#include "mp_wrap.h"

USING_NS_CC;

class CCMagicEmitter : public cocos2d::CCLayer
{
protected:
	MP_Emitter* emitter;
	cocos2d::CCPoint temp_position;

public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    static CCMagicEmitter* create(HM_EMITTER hmEmitter);

	void change(HM_EMITTER hmEmitter);

	void change(HM_EMITTER hmEmitter, float x, float y);

	MP_Emitter* getEmitter(){return emitter;}
    
    // implement the "static node()" method manually
    CREATE_FUNC(CCMagicEmitter);

	virtual void update(float delta);

	virtual void draw(void);

	virtual void setPosition(const cocos2d::CCPoint &position);
	virtual const cocos2d::CCPoint& getPosition();
};

#endif  // __MAGIC_PARTICLES_SCENE_H__