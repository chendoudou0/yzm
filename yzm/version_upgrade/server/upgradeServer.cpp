#include"upgradeServer.h"
#include"config.h"   
#include"requestProcessor.h"
#include"redisOperator.h"
#include"dbPool.h"

#include <chrono>
#include<glog/logging.h>         
    
using namespace std;
using namespace std::chrono;

shared_ptr<CRedisOperator>          gPtrRedisOperator;
shared_ptr<CDbPool<CDBOperator> >   gPtrDbpool;

class CUpgradeServer::Imp
{
public:
	HttpSRV * httpPtr_{NULL};

public:
	bool init() 
	{
		if (!readConf())
		{
			LOG(ERROR) << "read conf paramer failed!";
			return false;      
		} 

		if (!initDB())
		{
			LOG(ERROR) << "init  db  failed!";
			return false;
		} 

		if (!initRedis())
		{
			LOG(ERROR) << "init redis failed!";
			return false;
		} 

		return true;
	}

	bool initDB()
	{
		 
		gPtrDbpool.reset(new CDbPool<CDBOperator>);

		gPtrDbpool->SetPoolSize(4);

		gPtrDbpool->InitPool();
		
		return true;              
	}

	bool initRedis()
	{
		gPtrRedisOperator.reset(new CRedisOperator(config::CConfigManager::instance()->redis_para_.ip_ \
		, config::CConfigManager::instance()->redis_para_.port_,            \
		config::CConfigManager::instance()->redis_para_.overTime_) );

		gPtrRedisOperator->setPoolSize(16);

		return true;
	}

	bool  readConf()     
	{    
		if (! config::CConfigManager::instance()->get_glog_paramer(CONF_PATH))
		{
			return false;
		}
		config::init_glog("upgrade_server");
		LOG(INFO) << "GLOG BEGIN TO WORK"; 

		if (! config::CConfigManager::instance()->get_db_paramer(CONF_PATH))
		{
			return false;
		}

		if (! config::CConfigManager::instance()->get_redis_param(CONF_PATH))  
		{
			return false;
		}

		if (! config::CConfigManager::instance()->get_main_param(CONF_PATH))  
		{
			return false;
		}

		return true;

	}

	void getServiceType(const string& uri, int& type)
	{
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
		else if (uri == ANDROID_VERSION_ADD_URI)
		{
			type = ANDROID_VERSION_ADD;
		}
		else if(uri == LIVESHOW_VERSION_ADD_URI)
		{
			type = LIVESHOW_VERSION_ADD;
		}

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
	HttpSRV_addRequestCB(impPtr_->httpPtr_, LIVE_SHOW_QUERY_URI,      callback);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, LIVE_SHOW_UPDATE_URI,     callback);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, ANDROID_QUERY_URI,        callback);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, ANDROID_VERSION_ADD_URI,  callback);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, LIVESHOW_VERSION_ADD_URI, callback);  

	HttpSRV_start(impPtr_->httpPtr_,  config::CConfigManager::instance()->listen_port_);

	return  true;                     

}

void  CUpgradeServer::Stop()
{
     HttpSRV_destory(impPtr_->httpPtr_);
	 
	 LOG(INFO) << "UpgradeServer stop";
}
	 
int CUpgradeServer::httpCallBack(const RecvInfo& pRecvInfo, string&  response)
{
	LOG(INFO) << "httpCallBack begin ...";
	LOG(INFO) << "uri "<< pRecvInfo.uri_ << ", receive data  "<< pRecvInfo.data_;
	auto begin = steady_clock::now();
	int type = 0;
	impPtr_->getServiceType(pRecvInfo.uri_, type);

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
	case ANDROID_VERSION_ADD:
	     request_pro->dealwith_android_version_add();
	     response = request_pro->get_response();
	    
		 break;
	case LIVESHOW_VERSION_ADD:
	     request_pro->dealwith_liveshow_version_add();
	     response = request_pro->get_response();
	    
		 break;
	default:
		 break;
	}
	auto end = steady_clock::now();
    auto cost = duration_cast<milliseconds>(end - begin).count();
	LOG(INFO) << "response buffer length : " << response.length() << "   data :" << response;
	LOG(INFO) << "callback end, cost " << cost << "ms"; 
	
	return 0;  
}

