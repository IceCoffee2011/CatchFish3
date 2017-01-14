#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "MagicScene.h"
#include "platform_cocos.h"
#include "platform/CCPlatformConfig.h"
//#include "Utils.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "header.h"
#endif


using namespace cocos2d;


// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
bool CCMagicEmitter::init()
{
	emitter=NULL;
	bool bRet = false;
    do 
    {
		CC_BREAK_IF(! CCLayer::init());
		bRet = true;
    } while (0);

	return bRet;
}

CCMagicEmitter* CCMagicEmitter::create(HM_EMITTER hmEmitter)
{
	CCMagicEmitter* magic_emitter=new CCMagicEmitter;
	magic_emitter->change(hmEmitter);
	magic_emitter->scheduleUpdate();
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	magic_emitter->autorelease();
#endif
	return magic_emitter;
}

void CCMagicEmitter::change(HM_EMITTER hmEmitter)
{
	MP_Manager& MP=MP_Manager::GetInstance();
	emitter=MP.GetEmitter(hmEmitter);
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	emitter->SetState(MAGIC_STATE_UPDATE);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	emitter->SetState(MAGIC_STATE_STOP);
#endif
	
}


void CCMagicEmitter::change(HM_EMITTER hmEmitter, float x, float y)
{
	MP_Manager& MP=MP_Manager::GetInstance();
	emitter=MP.GetEmitter(hmEmitter);

	MP_POSITION pos;
	emitter->GetPosition(pos);
	pos.x=x;
	pos.y=y;
	emitter->Move(pos);

	emitter->SetState(MAGIC_STATE_UPDATE);
}

void CCMagicEmitter::update(float delta)
{
	cocos2d::CCLayer::update(delta);

	double rate=0.01;
	rate=1000*delta;

	if (rate>500)
		rate=0.01;

	emitter->Update(rate);
}

void CCMagicEmitter::draw(void)
{
	GLboolean attr_GL_BLEND;
	glGetBooleanv(GL_BLEND, &attr_GL_BLEND);

	GLint attr_GL_BLEND_SRC_ALPHA, attr_GL_BLEND_DST_ALPHA;
	glGetIntegerv(GL_BLEND_SRC_ALPHA, &attr_GL_BLEND_SRC_ALPHA);
	glGetIntegerv(GL_BLEND_DST_ALPHA, &attr_GL_BLEND_DST_ALPHA);
	
	// Set the shader program for OpenGL
	CCGLProgram* program=CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor);
	setShaderProgram(program);
	program->use();
	program->setUniformsForBuiltins();
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords | kCCVertexAttribFlag_Color); 
	
	emitter->Render();
	
	if (attr_GL_BLEND)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	glBlendFunc(attr_GL_BLEND_SRC_ALPHA, attr_GL_BLEND_DST_ALPHA);
}

void CCMagicEmitter::setPosition(const cocos2d::CCPoint &position)
{
	MP_POSITION pos;
	pos.x=position.x;
	pos.y=position.y;
	pos.z=0.f;
	emitter->SetPosition(pos);
}

const cocos2d::CCPoint& CCMagicEmitter::getPosition()
{
	MP_POSITION pos;
	emitter->GetPosition(pos);

	temp_position.x=pos.x;
	temp_position.y=pos.y;
	return temp_position;
}