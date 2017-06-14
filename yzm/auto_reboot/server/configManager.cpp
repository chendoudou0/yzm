#include "configManager.h"
#include <fstream>

extern conf::CConfigManager* gPtrConf;

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
		FLAGS_minloglevel = google::INFO;
		FLAGS_v = 99;
		FLAGS_stop_logging_if_full_disk = true;
		FLAGS_alsologtostderr = true;
		FLAGS_max_log_size = gPtrConf->_glog_para._file_size;

		std::string prefix =
			gPtrConf->_glog_para._log_dir + "/" + log_name;
		LOG(INFO) << prefix;
		google::SetLogDestination(google::INFO,
			std::string(prefix + ".info_").c_str());
		google::SetLogDestination(google::WARNING,std::string(prefix + ".warning_").c_str());
		google::SetLogDestination(google::GLOG_ERROR, std::string(prefix + ".error_").c_str());
		google::SetLogDestination(google::FATAL, std::string(prefix + ".fatal_").c_str());

		google::InstallFailureSignalHandler();
		google::InstallFailureWriter(FailureHandler);

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
			LOG(ERROR) << "open conf file failed! ";
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
			LOG(ERROR) << "parse conf file failed,  file path : " << path;
			return false;
		}

		using rapidjson::Value;

		Value & v1 = doc["cpp_callback_url"];

		if (v1.IsNull() || !v1.IsString())  
		{
			LOG(ERROR) << "cpp_callback_url is null ";
			return false;
		}
		_cpp_callback_url = v1.GetString();

		Value & v2 = doc["listen_port"];
		if (v2.IsNull() && !v2.IsInt())
		{
			LOG(ERROR) << "listen_port is null ";
			return false;
		}
		_listen_port = v2.GetInt();

		Value & v3 = doc["uri_for_cpp"];
		if (v3.IsNull() || !v3.IsString())
		{
			LOG(ERROR) << "uri_for_cpp is null ";
			return false;
		}

		_uri_for_cpp = v3.GetString();
	
		LOG(INFO) << "cpp_callback_url  " << _cpp_callback_url << "\n" << "_listen_port " << _listen_port  \
			<< "\n" << "uri_for_cpp " << _uri_for_cpp;

		return true;   
	}

	bool CConfigManager::get_monitor_program(string path)
	{
		Document doc;
		if (!ParseConf(path, doc))
		{
			LOG(ERROR) << "parse conf file failed,  file path : " << path;
			return false;
		}

		using rapidjson::Value;

		Value & v1 = doc["program_array"];

		if (v1.IsNull() || !v1.IsArray())
		{
			LOG(ERROR) << "program_array is null ";
			return false;
		}


		for (Value::ConstValueIterator it = v1.Begin(); it != v1.End(); ++it){
			string  url = (*it)["url"].GetString();
			_monitor_program_vec.push_back(url);

			LOG(INFO) << "monitor_program url : " << url;
		}
			

		return true;

	}

}





