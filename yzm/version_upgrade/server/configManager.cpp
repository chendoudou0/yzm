#include <fstream>
#include "def.h"
#include "configManager.h"


bool CConfigManager::ParseConf(string& path, Document& doc)
{
	string      stringFromStream;
	ifstream    in;
	in.open(path, ifstream::in);
	if (!in.is_open())
	{
		Log.WriteLog(4, "open conf file failed! \n");
		return false;
	}
	string line;
	while (getline(in, line))
	{
		stringFromStream.append(line + "\n");
	}
	in.close();

	using rapidjson::Document;
	doc.Parse<0>(stringFromStream.c_str());
	if (doc.HasParseError())
	{
		rapidjson::ParseErrorCode code = doc.GetParseError();
		return  false;
	}

	return true;

}


bool  CConfigManager::GetMainParam(string path)
{
	Document doc;
	if (!ParseConf(path, doc))
	{
		Log.WriteLog(4, "parse conf file failed,  file path: %s \n\n", path.c_str());
		return false;
	}
	
	using rapidjson::Value;

	Value & v1 = doc["cpp_callback_url"];

	if (v1.IsNull() || !v1.IsString())
	{
		Log.WriteLog(4, "conf read cpp_callback_url failed !");
		return false;
	}
	_cpp_callback_url = v1.GetString();


	Value & v2 = doc["listen_port"];
	if (v2.IsNull() && !v2.IsInt())
	{
		Log.WriteLog(4, "conf  read listen_port failed !");
		return false;
	}
	_listen_port = v2.GetInt();

	Value & v3 = doc["uri_for_cpp"];
	if (v3.IsNull() || !v3.IsString())
	{
		Log.WriteLog(4, "conf read uri_for_cpp failed !");
		return false;
	}

	_uri_for_cpp = v3.GetString();

	Log.WriteLog(1, "cpp_callback_url: %s, port: %d , uri_for_cpp : %s", _cpp_callback_url.c_str(), _listen_port, \
		_uri_for_cpp.c_str());
	
	return true;
}

bool  CConfigManager::GetServerIpList(string path)
{
	Document doc;
	if (!ParseConf(path, doc))
	{
		Log.WriteLog(4, "parse conf file failed,  file path: %s \n\n", path.c_str());
		return false;
	}

	Document::ConstMemberIterator it = doc.MemberBegin();
	for (; it != doc.MemberEnd(); ++it)
	{
		const Value& value = it->value;
		string uri;
		vector<string> vecIp;
		if (value.IsNull())
		{
			Log.WriteLog(4, " ip_array or uri is null \n");
			return false;
		}
		
		if (!(value.HasMember("uri") && value["uri"].IsString()))
		{
			Log.WriteLog(4, "ConfigManager get uri failed \n ");
			return false;
		}
		uri = value["uri"].GetString();

		const Value& ip_array = value["ip_array"];
		for (size_t i = 0; i < ip_array.Size(); ++i)
		{
			const Value& vIp = ip_array[i];
			if (!(vIp.HasMember("ip") && vIp["ip"].IsString()))
			{
				Log.WriteLog(4, "ip_array has no member ip \n ");
				return false;
			}
			string ip = vIp["ip"].GetString();
			vecIp.push_back(ip);
		}

		_uri_vec.push_back(uri);
		_uri_ip_map[uri] = make_pair(vecIp, 0);

	}

	return true;

}

bool CConfigManager::GetDBParam(string path)
{
	
		XmlConfig config(path.c_str());
	    if( !config ) 
	    {
	    	  gPtrAppLog->info("parse conf file failed ");   
		      goto _error; 
	    }

	    _db_param._DBHost = config.getValue("/root/db_param/DBHost", DEFAULT_STRING_NULL);
	    if( _db_param._DBHost.empty() ) 
	    {
	    	gPtrAppLog->error("db host empty");
		    goto _error;   
	    }
	    gPtrAppLog->debug("db host : %s ", _db_param._DBHost.c_str());
	    _db_param._DBUser = config.getValue("/root/db_param/DBUser", DEFAULT_STRING_NULL);
	    if( _db_param._DBUser.empty() ) 
	    {
	    	gPtrAppLog->error("db user empty");
		    goto _error;   
	    }
	    gPtrAppLog->debug("db user : %s ", _db_param._DBUser.c_str());
	    _db_param._DBPass = config.getValue("/root/db_param/DBPass", DEFAULT_STRING_NULL);
	    if( _db_param._DBUser.empty() ) 
	    {
	    	gPtrAppLog->error("db passwd empty");
		    goto _error;   
	    }
	    gPtrAppLog->debug("db pass : %s ", _db_param._DBPass.c_str());
	     _db_param._DBPort = config.getValue("/root/db_param/DBPort", DEFAULT_STRING_NULL);
	    if( _db_param._DBPort.empty() ) 
	    {
	    	gPtrAppLog->error("db port empty");
		    goto _error;   
	    }
	    gPtrAppLog->debug("db port : %s ", _db_param._DBPort.c_str());
	     _db_param._DBOver_time = config.getValue("/root/db_param/DBOverTime", 0);
	    if( _db_param._DBOver_time == 0 ) 
	    {
	    	gPtrAppLog->error("db over_time empty");
		    goto _error;   
	    }
	    gPtrAppLog->debug("db over time : %d ", _db_param._DBOver_time);

	    return true;

   _error:
		return false;

}


bool CConfigManager::GetRedisParam(string path)
{
	XmlConfig config(path.c_str());
    if( !config ) 
    {
    	  gPtrAppLog->info("parse conf file failed ");   
	      goto _error; 
    }


    _redis_param._ip = config.getValue("/root/redis_param/ip", DEFAULT_STRING_NULL);
    if( _redis_param._ip.empty() ) 
    {
    	gPtrAppLog->error("redis ip empty");
	    goto _error;   
    }
    gPtrAppLog->debug("redis ip : %s ", _redis_param._ip.c_str());

    _redis_param._port = config.getValue("/root/redis_param/port", 0);
    if( _redis_param._port == 0 ) 
    {
    	gPtrAppLog->error("redis port empty");
	    goto _error;   
    }
    gPtrAppLog->debug("redis port : %d ", _redis_param._port);

    _redis_param._time_out = config.getValue("/root/redis_param/time_out", 0);
    if( _redis_param._time_out == 0 ) 
    {
    	gPtrAppLog->error("redis time_out empty");
	    goto _error;   
    }
    
    gPtrAppLog->debug("redis time_out : %d ", _redis_param._time_out);

    return true;

   _error:
		return false;

}






