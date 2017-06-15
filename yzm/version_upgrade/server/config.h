#ifndef   _CONFIG__H
#define   _CONFIG__H

#include <glog/logging.h>
#include "rapidjson/document.h"		 
#include "rapidjson/prettywriter.h"	
#include "rapidjson/stringbuffer.h" 
#include "rapidjson/writer.h"
#include "singleton.h"
#include <string>

using namespace rapidjson;
using rapidjson::Document;

using namespace std;

namespace config
{
	struct GLOG_PARA{
		string _log_lever;    
		string _log_dir;
		int _file_size{ 0 };
	};

	struct  DB_PARA{
		string  host_;
		string  user_;
		string  pass_;
		int     port_{ 0 };
		int     overTime_{ 0 };
	};

	void init_glog(string log_name);        

	class CConfigManager : public Singleton<CConfigManager>
	{
	public:
		CConfigManager() = default; 
		~CConfigManager() = default;
		bool ParseConf(string& path, Document& doc);       
		bool get_glog_paramer(string path);
		bool get_db_paramer(string path);

	public:
		GLOG_PARA _glog_para; 
		DB_PARA   db_para_;

	private:

	};

}


#endif