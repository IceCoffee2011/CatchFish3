#ifndef __CATCHFISH_UPLOADFILE_H__
#define __CATCHFISH_UPLOADFILE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "cocos2d.h"
#include "curl/curl.h"
using namespace cocos2d;



struct UserInfo
{
    long long nVersion;
    int nUserID;
};

#endif
using namespace std;
class uploadFile : public CCObject
{
public:
	uploadFile();
	static uploadFile *m_inst;
	static uploadFile *GetInst();

	void UpLoadFile(string photoPath, UserInfo* userInfo);

	static size_t write_data(uint8_t *dataBack, size_t size, size_t nmemb, void *userp);
	static bool bComplete;
};

#endif