#include "requestProcessor.h"
#include"jsonOperator.h"  
#include"CLogger.h" 
#include"redisOperator.h" 

#include <chrono>
          
using namespace std;
using namespace std::chrono;

extern CDBOperator*    gPtrDBOperator;
extern CRedisOperator* gPtrRedisOperator;
extern CLogger* gPtrAppLog;


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
	try
	{
		gPtrDBOperator->QueryBucketAndDomain(type ,_version_to, bucket, domain);
	}
	catch(...)
	{
		gPtrAppLog->error("QueryBucketAndDomain failed, version name : %s", _version_to.c_str());
		_error_msg = "query bucket or domain failed"; 
		return false;      
	} 

	head = "http://" + bucket + "." + domain +"/" + _version_to  +  "/";

	Document document;
	Document::AllocatorType& allocator = document.GetAllocator();
	CJsonOperator* pJson = new CJsonOperator(allocator);
	pJson->addMember("ret", "0");
	pJson->addMember("url_head", head);  
	pJson->addMember("version_current", _version_current);
	pJson->addMember("version_to", _version_to);
	pJson->addArray("file_array", inFileMapVec);   
	pJson->genOutput();          
	_response = pJson->getOutput();

	delete pJson;
	return true;

}


bool CrequestProcessor::find_differ_files(int type, SqlMapVector& outFileVec)
{
	gPtrAppLog->info("find_differ_files begin");
	KeyValueMap currentMD5Map;
	KeyValueMap toMD5Map;            
	
	try               
    {
        gPtrDBOperator->QueryMd5s(type ,_version_current, currentMD5Map);
		gPtrDBOperator->QueryMd5s(type ,_version_to, toMD5Map);   
    }
    catch(...)          
    {
		return false;                       
    }
	
	KeyValueMap::iterator it; 
	for(it=currentMD5Map.begin(); it!=currentMD5Map.end();it++)
	{
		 KeyValueMap::iterator it2; 
  		 it2 = toMD5Map.find(it->first);
   		 if(it2 == toMD5Map.end())
   		 {
			KeyValueMap deleteMap;
			deleteMap["file_path"] = it->first;
			deleteMap["file_status"] = "0";
			deleteMap["md5"] = "NULL";
			outFileVec.push_back(deleteMap);
		 }   
	}

	KeyValueMap::iterator it3;
	for(it3=toMD5Map.begin(); it3!=toMD5Map.end();it3++)
	{
		 KeyValueMap::iterator it4; 
  		 it4 = currentMD5Map.find(it3->first);
   		 if(it4 == currentMD5Map.end())
   		 {
			KeyValueMap addMap;
			addMap["file_path"] = it3->first;
			addMap["file_status"] = "1";
			addMap["md5"] = it3->second;
			outFileVec.push_back(addMap);
		 }     
	}

	KeyValueMap::iterator it5;     
	for(it5=currentMD5Map.begin(); it5!=currentMD5Map.end();it5++)
	{
		 KeyValueMap::iterator it6; 
  		 it6 = toMD5Map.find(it5->first);
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

	gPtrAppLog->info("find_differ_files end");
	return true;  


}


void CrequestProcessor::dealwith_liveshow_version_update()
{
	gPtrAppLog->info("dealwith_liveShow_version_update begin");

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
			gPtrAppLog->info(" redis exit");  
			error = false;      
			break;
		}

		gPtrAppLog->info(" get_value_from_redis fialed"); 

		if(!find_differ_files(LIVE_SHOW_VERSION_UPDATE , fileVec))
		{
			_error_msg = "query md5 failed";
			gPtrAppLog->error("query md5 failed");
			break;
		} 

		if(!gen_update_response(LIVE_SHOW_VERSION_UPDATE , fileVec))   
		{  
			break;
		}

		if(!set_value_to_redis(redis_key, _response))
		{
			gPtrAppLog->error("set value to redis failed");
		}
		
		gPtrRedisOperator->setExpireTime(redis_key, REDIS_EXPIRE_TIME); 

		error = false;

	}while(0);

	if (error)
	{
		gen_error_response();
	}

	gPtrAppLog->info("dealwith_liveShow_version_update end");


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
			gPtrAppLog->error("parse json failed");
			_error_msg = "invalid json";
			break;
		}

		if (doc["version_current"].IsNull() || !doc["version_current"].IsString() )
		{
			gPtrAppLog->error("version_current is null");
			_error_msg = "version_current is null";
			break;
		}
		_version_current = doc["version_current"].GetString();
		if (doc["version_to"].IsNull() || !doc["version_to"].IsString())
		{
			gPtrAppLog->error("version_to is null");
			_error_msg = "version_to is null";
			break;
		}
		_version_to = doc["version_to"].GetString();
		ret = true;

	}while(0);
	
	return  ret;
}

void CrequestProcessor::dealwith_livershow_verison_query()  
{
    gPtrAppLog->info("LIVE_SHOW_VERSION_QUERY, date : %s", _request_buffer);
    gPtrAppLog->info("dealwith_livershow_verison_query() begin");

    bool error = true;
    do
    {
    	
    	SqlMapVector objVecMap;
		try
	    {
	        gPtrDBOperator->QueryExistingVersions(LIVE_SHOW_VERSION_QUERY ,objVecMap); 
	    }
	    catch(...)
	    {
	        _error_msg =  "query live_show version failed";
	       	break;
	    }

		Document document;
		Document::AllocatorType& allocator = document.GetAllocator();
		CJsonOperator* pJson = new CJsonOperator(allocator);
		pJson->addMember("ret", "0");
		pJson->addArray("version_array", objVecMap);
		pJson->genOutput();
		_response = pJson->getOutput(); 

		delete pJson;
		error = false;

    }while(0);


    if (error)
    {
    	gen_error_response();
    }
	
	gPtrAppLog->info("dealwith_livershow_verison_query() end");
}

void CrequestProcessor::dealwith_android_verison_query()
{
	gPtrAppLog->info("dealwith_android_verison_query begin");
	do
	{
		if (!query_android_version())
		{
			_error_msg = "no android version exits";
			gen_error_response();
			gPtrAppLog->error("no android version exit");
		}

	}while(0);

	gPtrAppLog->info("dealwith_android_verison_query end");
}

bool CrequestProcessor::query_android_version()
{
	SqlMapVector objVecMap;
	try
    {
        gPtrDBOperator->QueryAndroidExistingVersions(objVecMap); 
    }
    catch(...)
    {
    	gPtrAppLog->error("QueryAndroidExistingVersions failed");
        return false; 
    }
	SqlMapVector::iterator it;
	for(it = objVecMap.begin(); it != objVecMap.end(); it++)
	{
		string bucket;
		string domain;
		string head;
		string url;
		try
	    {
	        gPtrDBOperator->QueryAndroidBucketAndDomain((*it)["version_name"], bucket, domain);
	    }
	    catch(...)
	    {
	    	gPtrAppLog->error("QueryAndroidBucketAndDomain failed");
	        return false; 
	    }

		head = "http://" + bucket + "." + domain +"/" + (*it)["version_name"]  +  "/";
		url = head + (*it)["oss_path"];

		KeyValueMap::iterator pos = (*it).find("oss_path");

		if (pos != (*it).end())
		{
		    (*it).erase(pos);
		}

		(*it)["file_url"] = url;
		
	}
	Document document;
	Document::AllocatorType& allocator = document.GetAllocator();
	CJsonOperator* pJson = new CJsonOperator(allocator);
	pJson->addMember("ret", "0");
	pJson->addArray("version_array", objVecMap);
	pJson->genOutput();
	_response = pJson->getOutput();

	delete pJson;
	return true;   
}

void CrequestProcessor::gen_error_response()
{
	Document document;
	Document::AllocatorType& allocator = document.GetAllocator();
	CJsonOperator* pJson = new CJsonOperator(allocator);
	pJson->addMember("ret", "1");
	pJson->addMember("error", _error_msg);
	pJson->genOutput();
	_response = pJson->getOutput();

	delete pJson;
}



