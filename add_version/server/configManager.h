#ifndef   _CONFIG_MANAGER_H
#define   _CONFIG_MANAGER_H

#include <glog/logging.h>
#include <string>
#include "document.h"		       // rapidjson's DOM-style API
#include "prettywriter.h"	       // for stringify JSON
#include "stringbuffer.h" 
#include "writer.h"
#include "WriteLog.h"
#include "def.h"
using namespace rapidjson;
using rapidjson::Document;
using namespace std;

extern CWriteLog Log;

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
		string _cpp_callback_url;
		string _uri_for_cpp;
		int _listen_port;
		vector<string> _uri_vec;
		GLOG_PARA _glog_para;

	private:


	};

}


#endif