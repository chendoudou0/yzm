#include "aos_log.h"
#include "aos_util.h"
#include "aos_string.h"
#include "aos_status.h"
#include "oss_auth.h"
#include "oss_util.h"
#include "oss_api.h"
#include "oss_define.h"

#include <iostream>
#include <string>
#include <vector>  
#include <conio.h>
#include <map>
#include "HttpSRV.h"
using namespace std;

#include "configManager.h"
#include "rapidjson/document.h"		 
#include "rapidjson/prettywriter.h"	
#include "rapidjson/stringbuffer.h" 
#include "rapidjson/writer.h"

using namespace rapidjson;
using rapidjson::Document;
typedef map<string, string> MAP_JSON;
typedef MAP_JSON::iterator  MAP_JSON_IT;

//const char OSS_ENDPOINT[] = "oss-cn-shenzhen.aliyuncs.com";    
const char OSS_ENDPOINT[] = "oss-cn-shenzhen-internal.aliyuncs.com";
const char ACCESS_KEY_ID[] = "ItHyfCNDQzdGvmLl";
const char ACCESS_KEY_SECRET[] = "s7SwH31Pah9j67zYOmPceRLjpfwnz1";
const char BUCKET_NAME[] = "cloudreammsapi";
const char OBJECT_NAME[] = "ddtest/MgrGscSystem.rar";


conf::CConfigManager* g_ptrConf;
#define CONF_PATH  "./conf/conf.json"
#define LOG_PATH   "./log/oss"

string g_oss_endpoint;
string g_access_key_id;
string g_access_key_secret;

void init_sample_request_options(oss_request_options_t *options, int is_cname);
void put_object_from_buffer();
void put_object_from_file();
void get_object_to_buffer();
void get_object_to_file();
int get_object_to_file_my(MAP_JSON&  mapJson);
int put_object_from_file_my(MAP_JSON&  mapJson);
void delete_object();

inline string getValueName(const Value& pValue, std::string& strValue)
{
	char szTmp[256] = { 0 };
	if (pValue.IsInt())	//ÕûÐÎ
		sprintf_s(szTmp, "%d", pValue.GetInt());
	else if (pValue.IsUint())
		sprintf_s(szTmp, "%u", pValue.GetUint());
	else if (pValue.IsUint64())
		sprintf_s(szTmp, "%lu", pValue.GetUint64());
	else if (pValue.IsString())	//×Ö·û´®
		strcpy_s(szTmp, pValue.GetString());

	strValue = szTmp;
	return strValue;
}

int GenFailResponse(string& response)
{
	response += "{";

	response += "\"ret\":";
	response += "\"1\"";

	response += "}";
	return 0;
}

int GenSuccessResponse(string& response)
{
	response += "{";

	response += "\"ret\":";
	response += "\"0\"";

	response += "}";
	return 0;
}

void GenJson(MAP_JSON& objMap, string &JsonStr)
{
	MAP_JSON_IT iter;
	JsonStr += "{";
	for (iter = objMap.begin(); iter != objMap.end(); iter++)
	{
		if (iter != objMap.begin())
		{
			JsonStr.append(",");
		}
		JsonStr += "\"" + iter->first + "\":";
		JsonStr += "\"" + iter->second + "\"";
	}
	JsonStr += "}";
}

int  analyzeJson(char* strJson, MAP_JSON& mapJson)
{
	Document doc;
	doc.Parse<0>(strJson);
	if (doc.HasParseError())
	{
		rapidjson::ParseErrorCode code = doc.GetParseError();
		return -1;
	}

	Document::ConstMemberIterator it = doc.MemberBegin();
	for (; it != doc.MemberEnd(); ++it)
	{
		string name = (it->name.GetString());
		string strValue;
		const Value& pValue = it->value;
		getValueName(pValue, strValue);
		mapJson[name] = strValue;
	}

	return 0;
}

bool check_parameter(char* strJson, MAP_JSON& map)
{
	Document doc;
	doc.Parse<0>(strJson);
	if (doc.HasParseError())
	{
		rapidjson::ParseErrorCode code = doc.GetParseError();
		return false;
	}

	if (doc["bucket"].IsNull() || !doc["bucket"].IsString())
	{
		return false;
	}

	if (doc["object"].IsNull() || !doc["object"].IsString())
	{
		return false;
	}

	if (doc["file_path"].IsNull() || !doc["file_path"].IsString())
	{
		return false;
	}

	map["bucket"]	 = doc["bucket"].GetString();
	map["object"]	 = doc["object"].GetString();
	map["file_path"] = doc["file_path"].GetString();


	return true;
	
}

int uploadCallback(const RecvInfo* pRecvInfo, ResponseCB respCB, const void* const _cbThis, void* arg)
{
	MAP_JSON  mapJson;
	string response;
	do 
	{
		if (!pRecvInfo->data)
		{
			break;
		}

		LOG(INFO) << "receive json : " << pRecvInfo->data;
		if (!check_parameter(pRecvInfo->data, mapJson))
		{
			break;
		}

		if (0 == put_object_from_file_my(mapJson))
		{
			GenSuccessResponse(response);
		}
		else
		{
			GenFailResponse(response);
		}

	} while (0);
	

	respCB(_cbThis, response.c_str());

	return 0;
}


int downloadCallback(const RecvInfo* pRecvInfo, ResponseCB respCB, const void* const _cbThis, void* arg)
{
	MAP_JSON mapJson;
	string response;

	do
	{
		if (!pRecvInfo->data)
		{
			break;
		}
		LOG(INFO) << "receive json : " << pRecvInfo->data;
		if (!check_parameter(pRecvInfo->data, mapJson))
		{
			break;    
		}

		if (0 == get_object_to_file_my(mapJson))
		{
			GenSuccessResponse(response);
		}
		else
		{
			GenFailResponse(response);
		}

	} while (0);
	
	respCB(_cbThis, response.c_str());    

	return 0;
}

void init_sample_request_options(oss_request_options_t *options, int is_cname)
{
    options->config = oss_config_create(options->pool);
//	aos_str_set(&options->config->endpoint, OSS_ENDPOINT);
//  aos_str_set(&options->config->access_key_id, ACCESS_KEY_ID);
//  aos_str_set(&options->config->access_key_secret, ACCESS_KEY_SECRET);
	aos_str_set(&options->config->endpoint, g_oss_endpoint.c_str());
	aos_str_set(&options->config->access_key_id, g_access_key_id.c_str());
	aos_str_set(&options->config->access_key_secret, g_access_key_secret.c_str());
    options->config->is_cname = is_cname;

    options->ctl = aos_http_controller_create(options->pool, 0);
}

void put_object_from_buffer()
{
    aos_pool_t *p = NULL;
    aos_string_t bucket;
    aos_string_t object;
    int is_cname = 0;
    aos_table_t *headers = NULL;
    aos_table_t *resp_headers = NULL;
    oss_request_options_t *options = NULL;
    aos_list_t buffer;
    aos_buf_t *content = NULL;
    char *str = "test oss c sdk";
    aos_status_t *s = NULL;

    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_cname);
    headers = aos_table_make(p, 1);
    apr_table_set(headers, "x-oss-meta-author", "oss");
    aos_str_set(&bucket, BUCKET_NAME);
    aos_str_set(&object, OBJECT_NAME);

    aos_list_init(&buffer);
    content = aos_buf_pack(options->pool, str, strlen(str));
    aos_list_add_tail(&content->node, &buffer);

    s = oss_put_object_from_buffer(options, &bucket, &object, 
                   &buffer, headers, &resp_headers);

    if (aos_status_is_ok(s)) {
        printf("put object from buffer succeeded\n");
    } else {
        printf("put object from buffer failed\n");      
    }    

    aos_pool_destroy(p);
}

void put_object_from_file()
{
    aos_pool_t *p = NULL;
    aos_string_t bucket;
    aos_string_t object;
    int is_cname = 0;
    aos_table_t *headers = NULL;
    aos_table_t *resp_headers = NULL;
    oss_request_options_t *options = NULL;

    char *filename = "D:\\NetSarangXmanagerEnterprise5.rar";
	const char OBJECT_NAME[] = "MgrGscSystem.rar";

    aos_status_t *s = NULL;
    aos_string_t file;

    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_cname);
    headers = aos_table_make(options->pool, 1);
    apr_table_set(headers, OSS_CONTENT_TYPE, "image/jpeg");
    aos_str_set(&bucket, BUCKET_NAME);
    aos_str_set(&object, OBJECT_NAME);
    aos_str_set(&file, filename);

    s = oss_put_object_from_file(options, &bucket, &object, &file, 
                                 headers, &resp_headers);

    if (aos_status_is_ok(s)) {
        printf("put object from file succeeded\n");
    } else {
        printf("put object from file failed, code:%d, error_code:%s, error_msg:%s, request_id:%s\n",
            s->code, s->error_code, s->error_msg, s->req_id);
    }

    aos_pool_destroy(p);
}

int put_object_from_file_my(MAP_JSON&  mapJson)
{
	aos_pool_t *p = NULL;
	aos_string_t bucket;
	aos_string_t object;
	int is_cname = 0;
	aos_table_t *headers = NULL;
	aos_table_t *resp_headers = NULL;
	oss_request_options_t *options = NULL;

	aos_status_t *s = NULL;
	aos_string_t file;     

	aos_pool_create(&p, NULL);
	options = oss_request_options_create(p);
	init_sample_request_options(options, is_cname);
	headers = aos_table_make(options->pool, 1);
//	apr_table_set(headers, OSS_CONTENT_TYPE, "image/jpeg");
	apr_table_set(headers, OSS_CONTENT_TYPE, "application/octet-stream");
	aos_str_set(&bucket, mapJson["bucket"].c_str());
	aos_str_set(&object, mapJson["object"].c_str());
	aos_str_set(&file,   mapJson["file_path"].c_str());

	s = oss_put_object_from_file(options, &bucket, &object, &file,
		headers, &resp_headers);

	if (aos_status_is_ok(s)) {
		printf("put object from file succeeded\n");
		return 0;
	}
	else {
		printf("put object from file failed, code:%d, error_code:%s, error_msg:%s, request_id:%s\n",
			s->code, s->error_code, s->error_msg, s->req_id);
		return -1;
	}

	aos_pool_destroy(p);
}

void get_object_to_buffer()
{
    aos_pool_t *p = NULL;
    aos_string_t bucket;
    aos_string_t object;
    int is_cname = 0;
    oss_request_options_t *options = NULL;
    aos_table_t *headers = NULL;
    aos_table_t *params = NULL;
    aos_table_t *resp_headers = NULL;
    aos_status_t *s = NULL;
    aos_list_t buffer;
    aos_buf_t *content = NULL;
    char *buf = NULL;
    int64_t len = 0;
    int64_t size = 0;
    int64_t pos = 0;

    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_cname);
    aos_str_set(&bucket, BUCKET_NAME);
    aos_str_set(&object, OBJECT_NAME);
    aos_list_init(&buffer);

    s = oss_get_object_to_buffer(options, &bucket, &object, 
                                 headers, params, &buffer, &resp_headers);

    if (aos_status_is_ok(s)) 
	{
        printf("get object to buffer succeeded\n");
    }
    else 
	{
        printf("get object to buffer failed\n");  
    }

    //get buffer len
    aos_list_for_each_entry(aos_buf_t, content, &buffer, node) {
        len += aos_buf_size(content);
    }

    buf = (char*)aos_pcalloc(p, (apr_size_t)(len + 1));
    buf[len] = '\0';

    //copy buffer content to memory
    aos_list_for_each_entry(aos_buf_t, content, &buffer, node) {
        size = aos_buf_size(content);
        memcpy(buf + pos, content->pos, (size_t)size);
        pos += size;
    }

    aos_pool_destroy(p);
}

void get_object_to_file()
{
    aos_pool_t *p = NULL;
    aos_string_t bucket;

	char *download_filename = NULL;   

	static int i = 0;
	if (i == 0)
	{
		download_filename = "D:\\SouthIsland_NewZealand_New.jpg";
	} 

	else
	{
		download_filename = "D:\\SouthIsland_NewZealand_New1111111111.jpg";
	}
	i++;

    aos_string_t object;
    int is_cname = 0;
    oss_request_options_t *options = NULL;
    aos_table_t *headers = NULL;
    aos_table_t *params = NULL;
    aos_table_t *resp_headers = NULL;
    aos_status_t *s = NULL;
    aos_string_t file;

    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_cname);
    aos_str_set(&bucket, BUCKET_NAME);
    aos_str_set(&object, OBJECT_NAME);
    headers = aos_table_make(p, 0);
    aos_str_set(&file, download_filename);

    s = oss_get_object_to_file(options, &bucket, &object, headers, 
                               params, &file, &resp_headers);

    if (aos_status_is_ok(s)) 
	{
        printf("get object to local file succeeded\n");
    } 
	else 
	{
        printf("get object to local file failed\n");
    }

    aos_pool_destroy(p);
}

int get_object_to_file_my(MAP_JSON&  mapJson)
{
	aos_pool_t *p = NULL;
	aos_string_t bucket;

	aos_string_t object;
	int is_cname = 0;
	oss_request_options_t *options = NULL;
	aos_table_t *headers = NULL;
	aos_table_t *params = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *s = NULL;
	aos_string_t file;

	aos_pool_create(&p, NULL);
	options = oss_request_options_create(p);
	init_sample_request_options(options, is_cname);
	aos_str_set(&bucket, mapJson["bucket"].c_str());
	aos_str_set(&object, mapJson["object"].c_str());
	headers = aos_table_make(p, 0);
	aos_str_set(&file, mapJson["file_path"].c_str());

	s = oss_get_object_to_file(options, &bucket, &object, headers,
		params, &file, &resp_headers);

	if (aos_status_is_ok(s))
	{
		printf("get object to local file succeeded\n");
		return 0;
	}
	else
	{
		printf("get object to local file failed\n");
		return -1;
	}

	aos_pool_destroy(p);
}

void delete_object()
{
    aos_pool_t *p = NULL;
    aos_string_t bucket;
    aos_string_t object;
    int is_cname = 0;
    oss_request_options_t *options = NULL;
    aos_table_t *resp_headers = NULL;
    aos_status_t *s = NULL;

    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_cname);
    aos_str_set(&bucket, BUCKET_NAME);
    aos_str_set(&object, OBJECT_NAME);

    s = oss_delete_object(options, &bucket, &object, &resp_headers);

    if (aos_status_is_ok(s)) {
        printf("delete object succeed\n");
    } else {
        printf("delete object failed\n");
    }

    aos_pool_destroy(p);
}

void set_oss_parameter()
{
	g_oss_endpoint = g_ptrConf->_oss_endpoint;
	g_access_key_id = g_ptrConf->_access_key_id;
	g_access_key_secret = g_ptrConf->_access_key_secret;
}

int main(int argc, char *argv[])
{
	apr_file_t *output = NULL;
	aos_pool_t *pool = NULL;
	apr_status_t ret;

	// initialize http io system, call it olny once
	if (aos_http_io_initialize(NULL, 0) != AOSE_OK) {
		exit(1);
	}
	aos_pool_create(&pool, NULL);
	// open log file
	ret = apr_file_open(&output, "OSS-server.log", APR_FOPEN_CREATE | APR_FOPEN_WRITE | APR_FOPEN_APPEND, APR_OS_DEFAULT, pool);
	if (APR_SUCCESS != ret) {
		exit(1);
	}
	// set log file
	aos_log_set_output(output);
	// set log level
	aos_log_set_level(AOS_LOG_INFO);
	aos_info_log("start");

	//////////////////////////////////////////////////////
	g_ptrConf = new conf::CConfigManager;
	if (!g_ptrConf->get_glog_paramer(CONF_PATH))
	{
		return false;
	}
	conf::init_glog("oss");

	if (!g_ptrConf->GetMainParam(CONF_PATH))
	{
		return false;
	}

	set_oss_parameter();

	HttpSRV *http = HttpSRV_init();
	HttpSRV_addRequestCB(http, "/upload",   (RequestCB)uploadCallback, NULL);
	HttpSRV_addRequestCB(http, "/download", (RequestCB)downloadCallback, NULL);
	HttpSRV_start(http, g_ptrConf->_listen_port);

	while (_getch() != 'q');
	HttpSRV_destory(http);

	// close log file
	apr_file_close(output);
	aos_pool_destroy(pool);

	//deinitialize http io system, call it olny once
	aos_http_io_deinitialize(); 

	return 0;
	
}