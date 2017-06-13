#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <dirent.h>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <chrono>

#include"WriteLog.h"
#include"def.h"
#include"Common.h"
#include"HttpSRV.h"
#include"httpHandler.h"    
#include"CLogger.h"  
#include"monitor.h" 
#include"configManager.h"
#include"requestProcessor.h"     
      
using namespace std;
using namespace std::chrono;
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
CConfigManager* gPtrConf = NULL;

bool initDB()
{
	gPtrAppLog->debug("init db begin");
	 
	gPtrAppLog->debug("db host : %s", gPtrConf->_db_param._DBHost.c_str());

	gPtrDBOperator = new CDBOperator(gPtrConf->_db_param);  
	
	if (!gPtrDBOperator->initMYSQL())
	{
		gPtrAppLog->error("connect mysql failed !");
		return false;
	}
	
	gPtrAppLog->info("connect mysql success! ");
	
	return true;
}

bool initRedis()
{
	gPtrRedisOperator = new CRedisOperator(gPtrConf->_redis_param._ip \
	 , gPtrConf->_redis_param._port,            \
	 gPtrConf->_redis_param._time_out);

	gPtrAppLog->info("redis ip: %s, redis port : %d, redis time out : %d" , \
	gPtrConf->_redis_param._ip.c_str(),  \
	gPtrConf->_redis_param._port,            \
	gPtrConf->_redis_param._time_out);

	return true;
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

	gPtrAppLog->info("my log begin to work ......");
}


void signalCallback(int num) 
{ 
    gPtrAppLog->debug("[%s:%d]  Catch a signal SIGTERM", __FILE__, __LINE__);

	gPtrMoniotr->Notify();
}

bool  readConf()     
{
	gPtrAppLog->debug("read conf paramer begin");

	gPtrConf =  new CConfigManager;
	if (!gPtrConf->GetDBParam(CONF_PATH))
	{
		return false;
	}
	gPtrAppLog->debug("read conf over");

	if (!gPtrConf->GetRedisParam(CONF_PATH))
	{
		return false;
	}

	gPtrAppLog->debug("read conf paramer end");

	return true;

}

bool init()
{
	initLog(LOG_PATH);

	if (!readConf())
	{
		gPtrAppLog->error(" get conf paramer failed!");
		return false;
	} 

	if (!initDB())
	{
		gPtrAppLog->error(" init  db  failed!");
		return false;
	} 

	if (!initRedis())
	{
		gPtrAppLog->error(" init redis failed!");
		return false;
	} 

	return true;
}

int callback(const RecvInfo* pRecvInfo, ResponseCB respCB, const void* const _cbThis, void* arg)
{
	gPtrAppLog->info("callback begin ...");

	auto begin = steady_clock::now();
	int type = 0;
	string response;
	string uri;
	uri = pRecvInfo->uri;
	gPtrAppLog->info("uri : %s ", pRecvInfo->uri);
	if (pRecvInfo->data != NULL)
	{
		gPtrAppLog->info("receive json: %s ", pRecvInfo->data);
	}

	if (uri == LIVE_SHOW_QUERY_URI)
	{
		type = LIVE_SHOW_VERSION_QUERY;
	}
	else if (uri == LIVE_SHOW_UPDATE_URI)
	{
		type = LIVE_SHOW_VERSION_UPDATE;
	}
	else if (uri == ANDROID_QUERY_URI)
	{
		type = VERSION_QUERY_ANDROID;
	}

	shared_ptr<CrequestProcessor> request_pro(new CrequestProcessor(pRecvInfo->data));
   
	switch (type)
	{
	case LIVE_SHOW_VERSION_QUERY :    
		 request_pro->dealwith_livershow_verison_query();
	     response = request_pro->get_response();

		 break;
	case LIVE_SHOW_VERSION_UPDATE:
		 request_pro->dealwith_liveshow_version_update();
	     response = request_pro->get_response();

		 break;
	case VERSION_QUERY_ANDROID:
	     request_pro->dealwith_android_verison_query();
	     response = request_pro->get_response();
	    
		 break;
	default:
		 break;
	}

	respCB(_cbThis, response.c_str());

	auto end = steady_clock::now();
    auto cost = duration_cast<milliseconds>(end - begin).count();
	gPtrAppLog->info("callback end, cost %d ms",  cost);
	
	return 0;       
}

int main()
{
	creat_daemon();
	if (!init())
	{
		return -1;
	}

	gPtrHttp = HttpSRV_init();    
	HttpSRV_addRequestCB(gPtrHttp, LIVE_SHOW_QUERY_URI,   (RequestCB)callback,   NULL);
	HttpSRV_addRequestCB(gPtrHttp, LIVE_SHOW_UPDATE_URI,  (RequestCB)callback,   NULL);
	HttpSRV_addRequestCB(gPtrHttp, ANDROID_QUERY_URI,     (RequestCB)callback,   NULL);
	HttpSRV_start(gPtrHttp, 2018); 

	signal(SIGTERM,  signalCallback);            
	gPtrMoniotr = new Monitor();
	gPtrMoniotr->Wait(); 
  
	HttpSRV_destory(gPtrHttp); 

	return 0;                               

}
