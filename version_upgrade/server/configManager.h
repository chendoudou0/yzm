#ifndef   _CONFIG_MANAGER_H
#define   _CONFIG_MANAGER_H

#include <string>
#include "../rapidjson/document.h"		 
#include "../rapidjson/prettywriter.h"	
#include "../rapidjson/stringbuffer.h" 
#include "../rapidjson/writer.h"
#include "WriteLog.h"
#include "def.h"
#include "CLogger.h"
#include"xml-config.h"
using namespace rapidjson;
using rapidjson::Document;
using namespace std;

extern CWriteLog Log;
extern CLogger* gPtrAppLog;

class CConfigManager
{
public:
	CConfigManager() = default;
	~CConfigManager() = default;
	bool ParseConf(string& path, Document& doc);
	bool  GetServerIpList(string path);
	bool GetMainParam(string path); 
	bool GetDBParam(string path);
	bool GetRedisParam(string path);

public:
	string _cpp_callback_url;
	string _uri_for_cpp;
	int _listen_port;
	URI_IPS_MAP _uri_ip_map;
	vector<string> _uri_vec;
	DB_PARAM _db_param;
	REDIS_PARAM _redis_param;

private:
	

};


#endif