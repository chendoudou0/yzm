#include"WriteLog.h"
#include"HttpTool.h"
#include"httpHandler.h"    
#include"CLogger.h"  
#include"configManager.h"
#include"requestProcessor.h"
#include"redisOperator.h"

#include <string>
#include <memory>
#include <chrono>
    
using namespace std;
using namespace std::chrono;

CLogger* gPtrAppLog = nullptr;
CWriteLog       Log;
CDBOperator*    gPtrDBOperator = nullptr;
CRedisOperator* gPtrRedisOperator = nullptr;

class CUpgradeServer 
{
	class Imp;
	unique_ptr<Imp>   impPtr_;
	
public:
  CUpgradeServer();
  virtual  ~CUpgradeServer();

public:
	bool Start();

	void Stop();

	int  httpCallBack(const RecvInfo& pRecvInfo, string&  response);

};

class CUpgradeServer::Imp
{
public:
	HttpSRV * httpPtr_{NULL};
	CConfigManager* confPtr_{NULL};

public:
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

	
	bool initDB()
	{
		gPtrAppLog->debug("init db begin");
		
		gPtrAppLog->debug("db host : %s", confPtr_->_db_param._DBHost.c_str());

		gPtrDBOperator = new CDBOperator(confPtr_->_db_param);  
		
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
		gPtrRedisOperator = new CRedisOperator(confPtr_->_redis_param._ip \
		, confPtr_->_redis_param._port,            \
		confPtr_->_redis_param._time_out);

		gPtrAppLog->info("redis ip: %s, redis port : %d, redis time out : %d" , \
		confPtr_->_redis_param._ip.c_str(),  \
		confPtr_->_redis_param._port,            \
		confPtr_->_redis_param._time_out);

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

	bool  readConf()     
	{
		gPtrAppLog->debug("read conf paramer begin");

		confPtr_ =  new CConfigManager;
		if (!confPtr_->GetDBParam(CONF_PATH))
		{
			return false;
		}
		gPtrAppLog->debug("read conf over");

		if (!confPtr_->GetRedisParam(CONF_PATH))
		{
			return false;
		}

		gPtrAppLog->debug("read conf paramer end");

		return true;

	}


};

CUpgradeServer::CUpgradeServer()
{
	impPtr_.reset(new Imp);
}

CUpgradeServer::~CUpgradeServer()
{

}

bool  CUpgradeServer::Start()
{
	if (!impPtr_->init())
	{
		return  false;
	}

	impPtr_->httpPtr_ = HttpSRV_init();   
	auto  callback = bind(&CUpgradeServer::httpCallBack, this, placeholders::_1, placeholders::_2);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, LIVE_SHOW_QUERY_URI,   callback);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, LIVE_SHOW_UPDATE_URI,  callback);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, ANDROID_QUERY_URI,     callback);

	HttpSRV_start(impPtr_->httpPtr_, 2018); 

	return  true;

}

void  CUpgradeServer::Stop()
{
     HttpSRV_destory(impPtr_->httpPtr_);
	 gPtrAppLog->info("CUpgradeServer::Stop()");
}

int CUpgradeServer::httpCallBack(const RecvInfo& pRecvInfo, string&  response)
{
	gPtrAppLog->info("callback begin ...");
	auto begin = steady_clock::now();
	int type = 0;
	gPtrAppLog->info("uri : %s ", pRecvInfo.uri_.c_str());          
	gPtrAppLog->info("receive json: %s ", pRecvInfo.data_.c_str());

	if (pRecvInfo.uri_ == LIVE_SHOW_QUERY_URI)
	{
		type = LIVE_SHOW_VERSION_QUERY;
	}
	else if (pRecvInfo.uri_ == LIVE_SHOW_UPDATE_URI)
	{
		type = LIVE_SHOW_VERSION_UPDATE;
	}
	else if (pRecvInfo.uri_ == ANDROID_QUERY_URI)
	{
		type = VERSION_QUERY_ANDROID;
	}
	shared_ptr<CrequestProcessor> request_pro(new CrequestProcessor(pRecvInfo.data_.c_str()));
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
	auto end = steady_clock::now();
    auto cost = duration_cast<milliseconds>(end - begin).count();
	gPtrAppLog->info("callback end, cost %d ms",  cost);
	
	return 0; 
}

