#include"requestProcessor.h"
#include"jsonOperator.h"  
#include"CLogger.h" 
#include"redisOperator.h" 
#include"dbPool.h"
#include"DBOperator.h"
#include"glog/logging.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

extern shared_ptr<CDbPool<CDBOperator> >   gPtrDbpool;
extern shared_ptr<CRedisOperator>          gPtrRedisOperator;      

CrequestProcessor::CrequestProcessor(const char* request_buffer)
:_request_buffer(request_buffer) 
{ 
}

CrequestProcessor::~CrequestProcessor()
{
}

bool CrequestProcessor::get_value_from_redis(string& key, string& value)
{
	if(!gPtrRedisOperator->getValue(key, value))    
	{
		return false;
	}

	return true;
}

bool CrequestProcessor::set_value_to_redis(string& key, string& value)
{
	if(!gPtrRedisOperator->setValue(key, value))    
	{
		return false;
	}

	return true;
}

bool CrequestProcessor::gen_update_response(int type , SqlMapVector& inFileMapVec)
{
	string bucket; 
	string domain;
	string head;

	if(!gPtrDbpool->GetDbOper()->QueryBucketAndDomain(type ,_version_to, bucket, domain)){
		LOG(ERROR) << "QueryBucketAndDomain failed, version name : " << _version_to;
		_error_msg = "query bucket or domain failed";
		return false;  
	}

	head = "http://" + bucket + "." + domain +"/" + _version_to  +  "/";

	CJsonOperator* pJson = new CJsonOperator();
	pJson->addStringMember("ret", "0");
	pJson->addStringMember("url_head", head);  
	pJson->addStringMember("version_current", _version_current);
	pJson->addStringMember("version_to", _version_to);
	pJson->addStringArray("file_array", inFileMapVec);   
	pJson->genOutput();          
	_response = pJson->getOutput();

	delete pJson;
	return true;

}


bool CrequestProcessor::find_differ_files(int type, SqlMapVector& outFileVec)
{
	LOG(INFO) << "find_differ_files begin...";
	KeyValueMap currentMD5Map;
	KeyValueMap toMD5Map;            
	
	if(!gPtrDbpool->GetDbOper()->QueryMd5s(type ,_version_current, currentMD5Map)){
		return false;
	}

	if(!gPtrDbpool->GetDbOper()->QueryMd5s(type ,_version_to, toMD5Map)){
		return false;
	}
	
	for(auto it=currentMD5Map.begin(); it!=currentMD5Map.end();it++)
	{
  		 auto it2 = toMD5Map.find(it->first);
   		 if(it2 == toMD5Map.end())
   		 {
			KeyValueMap deleteMap;
			deleteMap["file_path"] = it->first;
			deleteMap["file_status"] = "0";
			deleteMap["md5"] = "NULL";
			outFileVec.push_back(deleteMap);
		 }   
	}

	for(auto it3=toMD5Map.begin(); it3!=toMD5Map.end();it3++)
	{ 
  		 auto it4 = currentMD5Map.find(it3->first);
   		 if(it4 == currentMD5Map.end())
   		 {
			KeyValueMap addMap;
			addMap["file_path"] = it3->first;
			addMap["file_status"] = "1";
			addMap["md5"] = it3->second;
			outFileVec.push_back(addMap);
		 }     
	}

    
	for(auto it5=currentMD5Map.begin(); it5!=currentMD5Map.end();it5++)
	{ 
  		 auto it6 = toMD5Map.find(it5->first);
   		 if(it6 != toMD5Map.end())           
   		 {
			if(it5->second != it6->second)
			{
				KeyValueMap changeMap;
				changeMap["file_path"] = it5->first;
				changeMap["file_status"] = "2";
				changeMap["md5"] = it6->second;
				outFileVec.push_back(changeMap);  
			}
		 }       
	}

	LOG(INFO) << "find_differ_files end";
	return true;  

}

void CrequestProcessor::dealwith_liveshow_version_update()
{
	LOG(INFO) << "dealwith_liveShow_version_update begin";

	SqlMapVector fileVec;  
	string redis_key;
	bool error = true;

	do
	{
		if (!check_version_update_paramter())
    	{
    		break;
    	}                  

    	redis_key = _version_current + _version_to + LIVE_SHOW_KEY_FLAG;

		if(get_value_from_redis(redis_key, _response))    
		{
			LOG(INFO) << " redis exit ";  
			error = false;      
			break;
		}

		LOG(INFO) << " get_value_from_redis fialed "; 

		if(!find_differ_files(LIVE_SHOW_VERSION_UPDATE , fileVec))
		{
			_error_msg = "query md5 failed";
			LOG(ERROR) << "find_differ_files  failed";
			break;
		} 

		if(!gen_update_response(LIVE_SHOW_VERSION_UPDATE , fileVec))   
		{  
			break;
		}

		if(!set_value_to_redis(redis_key, _response))
		{
			LOG(ERROR) << " set value to redis failed ";  
		}
		
		gPtrRedisOperator->setExpireTime(redis_key, REDIS_EXPIRE_TIME); 

		error = false;

	}while(0);

	if (error)
	{
		gen_error_response();
	}

	LOG(INFO) << "dealwith_liveShow_version_update end";

}

void CrequestProcessor::dealwith_android_version_add()
{
	LOG(INFO) << "add android version begin";

	bool error = true;

	do
	{
		ANDROID_VERSION_INFO  avInfo;
		if (!check_android_version_add_paramter(avInfo))
    	{
			_error_msg = "invalid  paramter";
			_error_code = "1";
			LOG(ERROR) << _error_msg;
    		break;
    	}

		if(2 == gPtrDbpool->GetDbOper()->AddAndroidVersion(avInfo) ){
			_error_msg =  " version already exist";
			_error_code = "2";
			 LOG(ERROR) << "android version  " << avInfo.name << " already exists";
	       	break;
		}
		else if(1 == gPtrDbpool->GetDbOper()->AddAndroidVersion(avInfo) ){
			_error_msg =  "insert into db failed";
			_error_code = "3";
			LOG(ERROR) << "insert into db failed ";
	       	break;
		}                 

		CJsonOperator* pJson = new CJsonOperator();
		pJson->addStringMember("ret", "0");
		pJson->addStringMember("errmsg", "success");
		pJson->genOutput();
		_response = pJson->getOutput(); 
		delete pJson;
		
		error = false;

	}while(0);

	if (error)
	{
		gen_error_response();
	}

	LOG(INFO) << "add android version end";

}


bool CrequestProcessor::check_version_update_paramter()
{
	bool  ret = false;
	do
	{
		Document doc;
		doc.Parse<0>(_request_buffer);
		if (doc.HasParseError())
		{
			rapidjson::ParseErrorCode code = doc.GetParseError();
			LOG(ERROR) << "parse json failed";
			_error_msg = "invalid json";
			break;
		}

		if (doc["version_current"].IsNull() || !doc["version_current"].IsString() )
		{
			_error_msg = "parameter error, version_current is empty";
			LOG(ERROR) << "parameter error, version_current is empty";
			
			break;
		}
		_version_current = doc["version_current"].GetString();
		if (doc["version_to"].IsNull() || !doc["version_to"].IsString())
		{
			_error_msg = "parameter error, version_to is empty";
			LOG(ERROR) << "parameter error, version_to is empty";

			break;
		}
		_version_to = doc["version_to"].GetString();
		ret = true;

	}while(0);
	
	return  ret;
}

bool CrequestProcessor::check_android_version_add_paramter(ANDROID_VERSION_INFO&   avInfo)
{
	bool  ret = false;
	do
	{
		Document doc;
		doc.Parse<0>(_request_buffer);
		if (doc.HasParseError())
		{
			rapidjson::ParseErrorCode code = doc.GetParseError();
			LOG(ERROR) << "parse json failed";
			break;
		}

		if (doc["version_name"].IsNull() || !doc["version_name"].IsString() )
		{
			break;
		}
		avInfo.name = doc["version_name"].GetString();
		if (doc["oss_domain"].IsNull() || !doc["oss_domain"].IsString())
		{
			break;
		}
		avInfo.domain = doc["oss_domain"].GetString();

		if (doc["oss_bucket"].IsNull() || !doc["oss_bucket"].IsString())
		{
			break;
		}
		avInfo.bucket = doc["oss_bucket"].GetString();

		if (doc["oss_path"].IsNull() || !doc["oss_path"].IsString())
		{
			break;
		}
		avInfo.path = doc["oss_path"].GetString();

		if (doc["md5"].IsNull() || !doc["md5"].IsString())
		{
			break;
		}
		avInfo.md5 = doc["md5"].GetString();

		if (doc["description"].IsNull() || !doc["description"].IsString())
		{
			break;
		}
		avInfo.description = doc["description"].GetString();

		if (doc["file_size"].IsNull() || !doc["file_size"].IsString())
		{
			break;
		}
		avInfo.size = doc["file_size"].GetString();

		if (doc["version_branch"].IsNull() || !doc["version_branch"].IsInt())
		{
			break;
		}
		avInfo.version_branch = doc["version_branch"].GetInt();


		ret = true;

	}while(0);
	
	return  ret;
}

bool CrequestProcessor::check_liveshow_version_add_paramter(LIVESHOW_ADD_INFO&     lsInfo)
{
	bool  ret = false;
	do
	{
		Document doc;
		doc.Parse<0>(_request_buffer);
		if (doc.HasParseError())
		{
			rapidjson::ParseErrorCode code = doc.GetParseError();
			LOG(ERROR) << "parse json failed";
			break;
		}

		if (doc["version_name"].IsNull() || !doc["version_name"].IsString() )
		{
			break;
		}
		lsInfo.name = doc["version_name"].GetString();
		if (doc["oss_domain"].IsNull() || !doc["oss_domain"].IsString())
		{
			break;
		}
		lsInfo.domain = doc["oss_domain"].GetString();

		if (doc["oss_bucket"].IsNull() || !doc["oss_bucket"].IsString())
		{
			break;
		}
		lsInfo.bucket = doc["oss_bucket"].GetString();

		if (doc["description"].IsNull() || !doc["description"].IsString())
		{
			break;
		}
		lsInfo.description = doc["description"].GetString();

		if (doc["path_array"].IsNull() || !doc["path_array"].IsArray() )
		{
			break;
		}
		lsInfo.fileVec_.reserve(doc["path_array"].Size());
		for (auto it = doc["path_array"].Begin(); it != doc["path_array"].End(); ++it)
		{
			if ((*it)["file_path"].IsNull() || !(*it)["file_path"].IsString())
			{
				break;
			}
			string path = (*it)["file_path"].GetString();
			int pos = path.find('/');
			path = path.substr(pos+1);
			LOG(INFO) << "PATH : "<< path << "  pos : "<< pos;
			if ((*it)["md5"].IsNull() || !(*it)["md5"].IsString())
			{
				break;
			}
			string md5 = (*it)["md5"].GetString();

			lsInfo.fileVec_.emplace_back(path, md5);

		}
    
		ret = true;

	}while(0);
	
	return  ret;

}

void CrequestProcessor::dealwith_livershow_verison_query()  
{
	LOG(INFO) << "dealwith_livershow_verison_query begin";

    bool error = true;
    do
    {
    	SqlMapVector objVecMap;
		
		if(!gPtrDbpool->GetDbOper()->QueryExistingVersions(LIVE_SHOW_VERSION_QUERY ,objVecMap)){
			_error_msg =  "mysql  query failed";
			LOG(ERROR) << "mysql  query failed";
	       	break;
		}

		CJsonOperator* pJson = new CJsonOperator();
		pJson->addStringMember("ret", "0");
		pJson->addStringArray("version_array", objVecMap);
		pJson->genOutput();
		_response = pJson->getOutput(); 

		delete pJson;
		error = false;

    }while(0);


    if (error)
    {
    	gen_error_response();
    }
	
	LOG(INFO) << "dealwith_livershow_verison_query end";
}

void CrequestProcessor::dealwith_android_verison_query()
{
	LOG(INFO) << "dealwith_android_verison_query begin";
	do
	{
		if (!query_android_version())
		{
			_error_msg = "mysql query failed";
			gen_error_response();

			LOG(ERROR) << "mysql query failed";
		}

	}while(0);

	LOG(INFO) << "dealwith_android_verison_query end";
}

bool CrequestProcessor::query_android_version()
{
	SqlMapVector objVecMap;

	if(!gPtrDbpool->GetDbOper()->QueryAndroidExistingVersions(objVecMap)){
		return false;
	}

	CJsonOperator* pJson = new CJsonOperator();
	pJson->addStringMember("ret", "0");
	pJson->addStringArray("version_array", objVecMap);
	pJson->genOutput();
	_response = pJson->getOutput();

	delete pJson;
	return true;   
}

void CrequestProcessor::gen_error_response()
{

	CJsonOperator* pJson = new CJsonOperator();

	pJson->addStringMember("ret", _error_code);

	pJson->addStringMember("errmsg", _error_msg);
	pJson->genOutput();
	_response = pJson->getOutput();

	delete pJson;
}

void CrequestProcessor::dealwith_liveshow_version_add()
{
	LOG(INFO) << "add liveshow version begin";

	bool error = true;

	do
	{
		LIVESHOW_ADD_INFO  lsInfo;
		if (!check_liveshow_version_add_paramter(lsInfo))
    	{
			_error_msg = "invalid  paramter";
			_error_code = "1";
			LOG(ERROR) << _error_msg;
    		break;
    	}
		int ret = gPtrDbpool->GetDbOper()->AddLiveshowVersion(lsInfo) ;
		if(2 == ret ){
			_error_msg =  " version already exist";
			_error_code = "2";
			LOG(ERROR) << "liveshow version  " << lsInfo.name << " already exists";
	       	break;
		}
		else if(1 == ret ){
			_error_msg =  "insert into db failed";
			_error_code = "3";
			LOG(ERROR) << "insert into db failed";
	       	break;
		}                 

		CJsonOperator* pJson = new CJsonOperator();
		pJson->addStringMember("ret", "0");
		pJson->addStringMember("errmsg", "success");
		pJson->genOutput();
		_response = pJson->getOutput(); 
		delete pJson;
		
		error = false;

	}while(0);

	if (error)
	{
		gen_error_response();
	}

	LOG(INFO) << "add android version end";

}



