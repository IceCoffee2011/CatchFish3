//
//  CharCode.cpp
//  FishHallC
//
//  Created by ChrisHappiness on 14-10-28.
//
//

#include "stdlib.h"
#include "CharCode.h"

std::string CCharCode::GB2312ToUTF8(const char * gb2312)
{
    std::string retStr;
//    retStr = charCode(gb2312, "gb2312", "utf8");
    retStr = charCode(gb2312, "gb2312", "utf-8");
    return retStr;
}

std::string CCharCode::UTF8ToGB2312(const char * utf8)
{
    std::string retStr;
//    retStr = charCode(utf8, "utf8", "gb2312");
    retStr = charCode(utf8, "utf-8", "gb2312");
    return retStr;
}

std::string CCharCode::charCode(std::string fromStr, const char * fromCode, const char * toCode)
{
    std::string retStr = "";
    iconv_t cd = iconv_open(toCode, fromCode);
    if (cd == (iconv_t)0)
    {
        return retStr;
    }
    const char * inStr = fromStr.c_str();
    const char ** inBuf = &inStr;
    size_t inLen = fromStr.length();
    size_t outLen = inLen * 4;
    char * outBuf = (char *)malloc(outLen);
    char * pBuf = outBuf;
    memset(outBuf, 0, outLen);
    if (-1 == iconv(cd, (char **)inBuf, &inLen, &outBuf, &outLen))
//    if (-1 == iconv(cd, inBuf, &inLen, &outBuf, &outLen))
    {
        iconv_close(cd);
        return retStr;
    }
   
    retStr = pBuf;
    free(pBuf);
    iconv_close(cd);
    
    return retStr;
}
