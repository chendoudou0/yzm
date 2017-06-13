#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <dirent.h>
#include <mutex>
#include <condition_variable>

#include"WriteLog.h"
#include"def.h"
#include"DBOperator.h"
#include"Common.h"
#include"HttpSRV.h"
#include"redisOperator.h"
#include"jsonOperator.h"
#include"httpHandler.h"    
#include"CLogger.h"  
#include"monitor.h"   
#include "configManager.h"   

#include "document.h"		
#include "prettywriter.h"	
#include "stringbuffer.h" 
#include "writer.h"
using namespace rapidjson;       
using namespace std;
using namespace comm;  

CWriteLog       Log;
CDBOperator*    gPtrDBOperator = NULL;
CRedisOperator* gPtrRedisOperator = NULL;
mutex g_mtx; 
condition_variable g_cond; 
bool g_bStop        = false;
HttpSRV *gPtrHttp   = NULL;
CLogger* gPtrAppLog = NULL;
comm::Monitor*  gPtrMoniotr = NULL;
conf::CConfigManager* gPtrConf = NULL;

bool initDB()
{
	gPtrDBOperator = new CDBOperator(CONF_PATH);
	if(!gPtrDBOperator)
	{
		return false;
	}
	gPtrDBOperator->readConf();
	gPtrDBOperator->initMYSQL(); 
	return true;
	
}

bool initRedis()
{
	gPtrRedisOperator = new CRedisOperator("127.0.0.1", 6379, 2);
	if(!gPtrRedisOperator)
	{
		return false;
	}
	return true;
}


bool callSystemCmd(string cmd)
{
	pid_t status;    
            
    status = system(cmd.c_str());  
  
    if (-1 == status)  
    {  
        return false;  
    }  
    else  
    {  
   
        if (WIFEXITED(status))  
        {  
            if (WEXITSTATUS(status) != 0)  
            {  
                return false; 
            }    
        }  
        else  
        {  
            return false; 
        }  
    }  
  
    return true;

}


int OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{  
    string* str = dynamic_cast<string*>((string *)lpVoid);  
    if( NULL == str || NULL == buffer )  
    {  
        return -1;  
    }  
    char* pData = (char*)buffer;  
    str->append(pData, size * nmemb);  
    return nmemb;  
} 

bool post(string& url,const char* strSend, string&  response)
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();  
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strSend);               
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());                        
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	return true;  
}

inline string getValueName(const Value& pValue, std::string& strValue)
{
	char szTmp[256] = { 0 };
	if (pValue.IsInt())	//??D?
		sprintf(szTmp, "%d", pValue.GetInt());
	else if (pValue.IsUint())
		sprintf(szTmp, "%u", pValue.GetUint());
	else if (pValue.IsUint64())
		sprintf(szTmp, "%lu", pValue.GetUint64());
	else if (pValue.IsString())	//×?·?′?
		strcpy(szTmp, pValue.GetString());

	strValue = szTmp;
	return strValue;
}

bool initLog(string path) 
{
	Log.Init(path.c_str(),  1,  0,  1024*10); 								
	Log.WriteLog(1, "[%s:%d] server start ! \n", __FILE__, __LINE__);  

	string tail = "-index";
	gPtrAppLog = new CLogger(path.c_str(), 
                        1024*1024, 5,                  //1M
                        CLogger::LOG_LEVEL(5),
                        CLogger::SHIFT_MODE(2)); 
	gPtrAppLog->setSuffix(tail);

	gPtrAppLog->info("[%s:%d] log begin to work ...", __FILE__, __LINE__);
}


vector<string> listDir(const char *path)  
{  
	vector<string>  filenameVec;  
	
    DIR              *pDir  = NULL;
    struct dirent    *ent  ;  
    int               i=0  ;  
    char              childpath[512];  
	char              filePath[512];

    pDir = opendir(path);  
	if(NULL == pDir)
	{
		return filenameVec;
	}
    memset(childpath, 0, sizeof(childpath));  
	memset(filePath, 0, sizeof(filePath));

    while((ent = readdir(pDir)) != NULL)  
    {  
        if(ent->d_type & DT_DIR)  
        {  
            if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0) 
            {
               continue; 
            }
			
			sprintf(childpath, "%s/%s", path, ent->d_name); 
		    vector<string>  tempVec = listDir(childpath); 
			for (vector<string>::iterator it = tempVec.begin(); it != tempVec.end(); it++)  
			{  
				filenameVec.push_back(*it);  
			}  

        }  
		else
		{
			sprintf(filePath, "%s/%s", path, ent->d_name); 
			filenameVec.push_back(filePath);   
		}
		
    }  

	return filenameVec;
  
}  

bool generateMD5(string& path, string& strMD5)
{
   char cmd[300] = {0};
   sprintf(cmd, "md5sum %s > outputMD5.txt ", path.c_str());
 
    if(!callSystemCmd(cmd))
    {
	   return false;
	}

   char* md5FileName = "outputMD5.txt";  
   int fileDes = open(md5FileName, O_RDWR|O_APPEND, 0666);  
   if(-1 == fileDes)  
   {  
   	  close(fileDes); 
      return false;
   }  
  
   lseek(fileDes, 0, SEEK_SET);  
   char md5Buf[33] = {0};  
  
   if(read(fileDes, md5Buf, 32) <= 0)  
   {  
   	  close(fileDes); 
      return false;
   } 
   strMD5 = md5Buf;
   
   close(fileDes);
   return  true;

}

bool getFileSize(string& inPath, int& inSize)
{
 
   int fileDes = open(inPath.c_str(), O_RDWR|O_APPEND, 0666);  
   if(-1 == fileDes)  
   {  
   	  close(fileDes); 
      return false;
   }  
  
   inSize = lseek(fileDes, 0, SEEK_END);  
   
   close(fileDes);  
   return  true;

}


int getMD5list(KeyValueMap& strMap, SqlMapVector& outVecMap)
{
    vector<string> pathVec = listDir(strMap["version_path"].c_str());
    if (pathVec.size() == 0)
    {
    	return -1;
    }
	for (vector<string>::iterator it = pathVec.begin(); it != pathVec.end(); it++)  
	{  
		KeyValueMap objMap;
		string md5;
		if(!generateMD5(*it, md5))
		{
			Log.WriteLog(1, "generate md5 failed, file path : %s ", (*it).c_str());
			gPtrAppLog->error("generate md5 failed, file path : %s", (*it).c_str());
			return -1;
		}
			
		int pos = (*it).rfind(strMap["version_name"]);
	    objMap["Ffile_path"] = (*it).substr(pos+1+strMap["version_name"].length());
		objMap["Fversion_name"] = strMap["version_name"];
		objMap["Fmd5"] = md5;
		outVecMap.push_back(objMap);
	}  
	
	return 0;       
}

bool addOneVersion(KeyValueMap& strMap, string type)
{
	bool ret = false;
	do
	{
		SqlMapVector objVecMap;
		if(getMD5list(strMap, objVecMap) != 0)
		{
			Log.WriteLog(1, "%s  is empty", strMap["version_path"].c_str());
			break;
		}

		try
		{   
			gPtrDBOperator->insertMD5toDB(objVecMap, type);
			gPtrDBOperator->UpdateVersionStatus(strMap["version_name"], type);
		}
		catch(...)
		{
			Log.WriteLog(1, " insert md5 to db failed or update version status failed \n");
			break;
		}

		ret = true;

	}while(0);

	return ret;  
}

bool addOneAndroidVersion(KeyValueMap& strMap)
{
	SqlMapVector objVecMap;
	string md5;
	int fileSize = 0;
	bool ret = false;
	do
	{
		if(!generateMD5(strMap["version_path"], md5))
		{
			Log.WriteLog(1, "generateMD5 failed, file path %s", strMap["version_path"].c_str());
			break;
		}

		if(!getFileSize(strMap["version_path"], fileSize))
		{
			Log.WriteLog(1, "getFileSize failed, file path %s", strMap["version_path"].c_str());
			break;
		}

		try
		{
			gPtrDBOperator->UpdateAndroidVersionStatus(strMap["version_name"], md5, fileSize);
		}
		catch(...)
		{
			Log.WriteLog(1, "UpdateAndroidVersionStatus failed");
			break;
		}

		ret = true;

	}while(0);
	

	return ret;      
}

void addNewVersions(string type)
{
	SqlMapVector objVecMap; 

	do
	{             
		try
		{
			gPtrDBOperator->QueryNewaddedVersions(objVecMap, type);
		}
		catch(...)
		{
			Log.WriteLog(1, "no %s version to be added", type.c_str());
			break;
		}
		
                   
		SqlMapVector::iterator iter;
	    for (iter = objVecMap.begin(); iter != objVecMap.end(); ++iter)
	    {
	    	if (!addOneVersion(*iter, type))
	    	{
	    		Log.WriteLog(1, " add version failed, version name %s\n", (*iter)["version_name"].c_str());
	    	} 
	    	else
	    	{
	    		Log.WriteLog(1, " add version successfully, version name  %s\n", (*iter)["version_name"].c_str());
	    	}

	    }

	}while(0);

}

void addAndroidVersions()
{
	SqlMapVector objVecMap;
	do
	{
		try  
	    {
	        gPtrDBOperator->QueryAndroidNewaddedVersions(objVecMap);  
	    }
	    catch(...)
	    {
	    	Log.WriteLog(1, " no android version to be added\n");
			break;                              
	    }

		SqlMapVector::iterator iter;
	    for (iter = objVecMap.begin(); iter != objVecMap.end(); ++iter)
	    {
	    	if (!addOneAndroidVersion(*iter))
	    	{
	    		Log.WriteLog(1, "add version failed, version name %s", (*iter)["version_name"].c_str());
	    	}
	    	else
	    	{
	    		Log.WriteLog(1, "add version successfully, version name %s", (*iter)["version_name"].c_str());
	    	}	                    
	    }

	}while(0);
               
}

void signalCallback(int num) 
{ 
    gPtrAppLog->debug("[%s:%d]  Catch a signal SIGTERM", __FILE__, __LINE__);

	gPtrMoniotr->Notify();
}


void creat_daemon(void)
{
    pid_t pid;              
	int fd;
    pid = fork();
    if( pid == -1)		exit(EXIT_FAILURE);     
    if(pid > 0 )		exit(EXIT_SUCCESS);
    if(setsid() == -1)	exit(EXIT_FAILURE);                               
    int i;
    for( i = 0; i < 3; ++i)
    {
        close(i);  
    }
	
    if (umask(0) == -1)   exit(1);

	if ((fd = open("/dev/null", O_RDWR)) == -1)  exit(1); 
	dup2(fd, STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	close(fd);
	
	
    return;
}

int main()
{
	gPtrConf = new conf::CConfigManager();
	gPtrConf->get_glog_paramer("../conf/app.conf");
	conf::init_glog("add_version");
	LOG(INFO) << "glog begin to work";
	LOG(ERROR) << "glog begin to work";  

	initLog(LOG_PATH);           
	if(!initDB())
	{   
		gPtrAppLog->debug("init db failed");
		return -1;
	}   

	addAndroidVersions();
	addNewVersions(ADD_LIVE_SHOW_VERSION);    
	addNewVersions(ADD_FITTING_ONLINE);   

	
	

	return 0;                        

}
