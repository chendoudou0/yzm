#ifndef   _CONFIG_MANAGER_H
#define   _CONFIG_MANAGER_H

#define  GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog/logging.h"
#include <string>
#include "rapidjson/document.h"		 
#include "rapidjson/prettywriter.h"	
#include "rapidjson/stringbuffer.h" 
#include "rapidjson/writer.h"
using namespace rapidjson;
using rapidjson::Document;
using namespace std;

namespace conf
{
	typedef struct _GLOG_PARA
	{
		string _log_lever;
		string _log_dir;
		int _file_size;
	}GLOG_PARA;

	void init_glog(string log_name);

	class CConfigManager
	{
	public:
		CConfigManager() = default;
		~CConfigManager() = default;
		bool ParseConf(string& path, Document& doc);
		bool GetMainParam(string path);
		bool get_glog_paramer(string path);

	public:
		int _listen_port;
		string _oss_endpoint;
		string _access_key_id;
		string _access_key_secret;
		GLOG_PARA _glog_para;   
		
	private:


	};

}


#endif