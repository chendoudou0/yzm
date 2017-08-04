#ifndef   _CONFIG__H
#define   _CONFIG__H

#include <glog/logging.h>
#include "document.h"		 
#include "prettywriter.h"	
#include "stringbuffer.h" 
#include "writer.h"
#include "singleton.h"
#include <string>

using namespace rapidjson;
using rapidjson::Document;

namespace config
{
	struct GLOG_PARA{
		std::string _log_lever;    
		std::string _log_dir;
		int _file_size{ 0 };
	};

	struct  DB_PARA{
		std::string  host_;
		std::string  user_;
		std::string  pass_;
		int     port_{ 0 };
		int     overTime_{ 0 };
	};

	struct REDIS_PARAM
	{
		std::string ip_;
		int port_{ 0 };
		int overTime_{ 0 };
	};

	void init_glog(std::string log_name);        

	class CConfigManager : public Singleton<CConfigManager>
	{
	public:
		CConfigManager() = default; 
		~CConfigManager() = default;
		bool ParseConf(std::string& path, Document& doc);  
		bool get_string(rapidjson::Value& v, std::string& str); 
		bool get_int(rapidjson::Value& v, int& iValue);  
		bool get_glog_paramer(std::string path);
		bool get_db_paramer(std::string  path);      
		bool get_redis_param(std::string path);
		bool get_main_param(std::string  path);
	public:
		GLOG_PARA _glog_para; 
		DB_PARA   db_para_;
		REDIS_PARAM   redis_para_;
		int 	listen_port_{2018};
		int   crawler_server_listen_port_{0};     
		int   crawler_client_server_listen_port_{0};
		int   label_server_port_;
		std::string suanfa_ip_;
		int suanfa_port_;
		std::string crawler_ip_;
		int  crawler_port_;
		std::string  ori_path_;
		std::string  success_path_;
		std::string  error_path_;
		std::string  bak_path_;


	private:

	};

}


#endif