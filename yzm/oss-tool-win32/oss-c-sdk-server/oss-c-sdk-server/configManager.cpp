#include "configManager.h"
#include <fstream>

extern conf::CConfigManager* g_ptrConf;

namespace conf
{
	void FailureHandler(const char* data, int size)
	{
		LOG(ERROR) << data;
	}

	void init_glog(string log_name)
	{
		google::InitGoogleLogging(log_name.c_str());

		FLAGS_logbufsecs = 0;
		FLAGS_minloglevel = google::GLOG_INFO;
		FLAGS_v = 99;
		FLAGS_stop_logging_if_full_disk = true;
		FLAGS_alsologtostderr = true;
		FLAGS_max_log_size = g_ptrConf->_glog_para._file_size;

		std::string prefix =
			g_ptrConf->_glog_para._log_dir + "/" + log_name;
		LOG(INFO) << prefix;
		google::SetLogDestination(google::GLOG_INFO,
			std::string(prefix + ".info_").c_str());
		google::SetLogDestination(google::GLOG_WARNING, std::string(prefix + ".warning_").c_str());
		google::SetLogDestination(google::GLOG_ERROR, std::string(prefix + ".error_").c_str());
		google::SetLogDestination(google::GLOG_FATAL, std::string(prefix + ".fatal_").c_str());

//		google::InstallFailureSignalHandler();
//		google::InstallFailureWriter(FailureHandler);  

	}
	bool CConfigManager::get_glog_paramer(string path)
	{
		Document doc;
		if (!ParseConf(path, doc))
		{
			LOG(ERROR) << "parse conf file failed,  file path : " << path;
			return false;
		}

		using rapidjson::Value;

		Value & log_dir = doc["log"]["log_dir"];

		if (log_dir.IsNull())
		{
			LOG(ERROR) << "log dir is null";
			return false;
		}

		_glog_para._log_dir = log_dir.GetString();

		LOG(INFO) << "glog_para._log_dir : " << _glog_para._log_dir;

		Value & log_level = doc["log"]["log_level"];

		if (log_level.IsNull())
		{
			LOG(ERROR) << "log level is null";
			return false;
		}

		_glog_para._log_lever = log_level.GetString();
		LOG(INFO) << "glog_para._log_level : " << _glog_para._log_lever;

		Value & file_size = doc["log"]["file_size"];

		if (file_size.IsNull())
		{
			LOG(ERROR) << "file size is null";
			return false;
		}

		_glog_para._file_size = file_size.GetInt();
		LOG(INFO) << "_glog_para._file_size : " << _glog_para._file_size;

		return true;

	}
	bool CConfigManager::ParseConf(string& path, Document& doc)
	{
		string      stringFromStream;
		ifstream    in;
		in.open(path, ifstream::in);
		if (!in.is_open())
		{
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
			return false;
		}

		using rapidjson::Value;
		
		Value & v2 = doc["listen_port"];
		if (v2.IsNull() && !v2.IsInt())
		{
			return false;
		}
		_listen_port = v2.GetInt();

		LOG(INFO) << "listen_port " << _listen_port;

		Value & v3 = doc["oss_endpoint"];
		if (v3.IsNull() || !v3.IsString())
		{
			return false;
		}

		_oss_endpoint = v3.GetString();
		LOG(INFO) << "oss_endpoint " << _oss_endpoint;

		Value & v4 = doc["access_key_id"];
		if (v4.IsNull() || !v4.IsString())
		{
			return false;
		}

		_access_key_id = v4.GetString();

		LOG(INFO) << "access_key_id " << _access_key_id;
		Value & v5 = doc["access_key_secret"];
		if (v5.IsNull() || !v5.IsString())
		{
			return false;
		}

		_access_key_secret = v5.GetString();
		LOG(INFO) << "access_key_secret " << _access_key_secret;
		return true;
	}

}




