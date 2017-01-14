//
//  CharCode.h
//  FishHallC
//
//  Created by ChrisHappiness on 14-10-28.
//
//

#ifndef _CharCode_h
#define _CharCode_h

#include <string>
#include <iconv.h>
//#include "iconv\iconv.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#pragma comment(lib, "libiconv.lib")
#endif
class CCharCode
{
public:
    static std::string GB2312ToUTF8(const char * gb2312);
    static std::string UTF8ToGB2312(const char * utf8);
    static std::string charCode(std::string fromStr, const char * fromCode, const char * toCode);
};

#endif
