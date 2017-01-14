//-----------------------------------------------------------------------------
// File: platform_cocos.cpp
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#include "cocos2d.h"
#include "platform_cocos.h"

MP_Platform_COCOS::MP_Platform_COCOS() : MP_Platform_WIN_POSIX()
{
}

MP_Platform_COCOS::~MP_Platform_COCOS()
{
}

// eng: Returns path to folder with emitters
// rus: Возвращает путь к папке с ptc-файлами
const char* MP_Platform_COCOS::GetPathToPTC()
{
	if (ptc_path.empty())
	{
		#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
		wchar_t path[MAX_PATH];
		GetCurrentDirectoryW(MAX_PATH, path);
		ptc_path=wchar_to_utf8(path);
		ptc_path+='\\';
		#else
			#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
			NSString* filename=[[NSBundle mainBundle] resourcePath];
			if (filename)
			{
				ptc_path=[filename UTF8String];
				ptc_path+="/";
			}
			#else
				cocos2d::CCFileUtils* fileUtils=cocos2d::CCFileUtils::sharedFileUtils();
				ptc_path=fileUtils->getWritablePath();
				ptc_path+='/';
			#endif
		#endif
	}

	return ptc_path.c_str();
}

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
// eng: Returns path to folder with textures
// rus: Возвращает путь к папке с текстурами
const char* MP_Platform_COCOS::GetPathToTexture()
{
	if (texture_path.empty())
	{
		texture_path="textures/";
	}

	return texture_path.c_str();
}
#endif