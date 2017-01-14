//-----------------------------------------------------------------------------
// File: platform_cocos.h
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#ifndef MAGIC_PARTICLES_PLATFORM_COCOS
#define MAGIC_PARTICLES_PLATFORM_COCOS

#include "platform/CCPlatformConfig.h"

#include "platform_win_posix.h"

class MP_Platform_COCOS : public MP_Platform_WIN_POSIX
{
public:
	MP_Platform_COCOS();
	virtual ~MP_Platform_COCOS();

	// eng: Returns path to folder with emitters
	// rus: Возвращает путь к папке с ptc-файлами
	virtual const char* GetPathToPTC();

	#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	// eng: Returns path to folder with textures
	// rus: Возвращает путь к папке с текстурами
	virtual const char* GetPathToTexture();
	#endif
};

#endif