#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "uploadFile.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "header.h"
#endif

uploadFile* uploadFile::m_inst = NULL;   
bool uploadFile::bComplete = false;  

uploadFile::uploadFile()
{
}

uploadFile* uploadFile::GetInst()    
{    
	if(!m_inst)    
	{    
		m_inst = new uploadFile();    
		return m_inst;    
	}    
	return NULL;    
}    

void uploadFile::UpLoadFile(string photoPath, UserInfo* userInfo)    
{    
	CCLOG("IN uploadFile::UpLoadFile(");    
	CURLcode cURLcode;    
	CURL *cURL;    

	//Init    
	cURLcode = curl_global_init(CURL_GLOBAL_SSL);    
	if(CURLE_OK != cURLcode)    
	{    
		curl_global_cleanup();    
		return ;    
	}    

	cURL = curl_easy_init();    
	if(!cURL)    
	{    
		curl_easy_cleanup(cURL);    
		curl_global_cleanup();    
		return ;    
	}    
	curl_httppost *post = NULL;    
	curl_httppost *last = NULL;    

	curl_formadd(&post, &last,CURLFORM_COPYNAME,"photo",    
		CURLFORM_FILE,photoPath.c_str(),    
		CURLFORM_CONTENTTYPE,"Image/png",CURLFORM_END);    

	char temp[256];
	sprintf(temp, "http://lelewap.66y.com/WriteTxt.aspx?uid=%d&vsion=%lld", userInfo->nUserID, userInfo->nVersion);
	
	curl_easy_setopt(cURL, CURLOPT_URL,temp);    
	curl_easy_setopt(cURL, CURLOPT_TIMEOUT,10);
	curl_easy_setopt(cURL, CURLOPT_HTTPPOST,post);
	curl_easy_setopt(cURL, CURLOPT_WRITEFUNCTION,write_data);
	curl_easy_setopt(cURL, CURLOPT_VERBOSE,1);
	curl_easy_perform(cURL);    

	curl_easy_cleanup(cURL);    

	curl_global_cleanup();    

}    


size_t uploadFile::write_data(uint8_t *dataBack, size_t size, size_t nmemb, void *user_p)    
{    
	bComplete = true;
	string szData = string((char*)dataBack); 
	return 0;    
}
