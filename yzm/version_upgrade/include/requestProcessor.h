#ifndef _REQUEST_PROCESSOR_H
#define _REQUEST_PROCESSOR_H

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

#include"def.h"
#include"DBOperator.h"
#include"Common.h"
#include"redisOperator.h"
#include"jsonOperator.h"  
#include"CLogger.h"  
#include"monitor.h" 
          
using namespace std;
using namespace std::chrono;
using namespace comm;
   

class CrequestProcessor
{
public:
	CrequestProcessor(char* request_buffer);
	~CrequestProcessor();
	void dealwith_android_verison_query();
	void dealwith_livershow_verison_query();
	void dealwith_liveshow_version_update();
	void dealwith_fitting_online_version_update();
	bool check_version_update_paramter();
	void gen_error_response();
	bool gen_update_response(int type ,SqlMapVector& inFileMapVec);
	bool query_android_version();
	bool query_live_show_version();
	bool query_ffitting_online_version();
	bool get_value_from_redis(string& key, string& value);
	bool set_value_to_redis(string& key, string& value);
	bool find_differ_files(int type, SqlMapVector& outFileVec);

	string& get_response()  {return _response; }

private:
	char* _request_buffer;
	string _error_msg;
	string _response;
	string _version_current;
	string _version_to;

};

#endif
