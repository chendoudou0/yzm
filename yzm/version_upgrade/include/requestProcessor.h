#ifndef _REQUEST_PROCESSOR_H
#define _REQUEST_PROCESSOR_H

#include"DBOperator.h"

#include <iostream>
#include <string>
#include <memory>


class CrequestProcessor
{
public:
	CrequestProcessor(const char* request_buffer);
	~CrequestProcessor();
	void dealwith_android_verison_query();
	void dealwith_livershow_verison_query();
	void dealwith_liveshow_version_update();
	void dealwith_fitting_online_version_update();
	void dealwith_android_version_add();
	void dealwith_liveshow_version_add();
	bool check_version_update_paramter();
	bool check_android_version_add_paramter(ANDROID_VERSION_INFO&   avInfo);
	bool check_liveshow_version_add_paramter(LIVESHOW_ADD_INFO&     lsInfo);
	void gen_error_response();
	bool gen_update_response(int type ,SqlMapVector& inFileMapVec);
	bool query_android_version();
	bool query_live_show_version();      
	bool query_ffitting_online_version();
	bool get_value_from_redis(std::string& key, std::string& value);
	bool set_value_to_redis(std::string& key, std::string& value);
	bool find_differ_files(int type, SqlMapVector& outFileVec);

	string& get_response()  {return _response; }

private:
	const char* _request_buffer;
	std::string _error_msg;
	std::string _error_code;
	std::string _response;
	std::string _version_current;
	std::string _version_to;

};

#endif
