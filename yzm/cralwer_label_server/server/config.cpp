#include "config.h"
#include <fstream>

using namespace rapidjson;
using rapidjson::Document;
using namespace std;

namespace  config
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
		FLAGS_max_log_size = CConfigManager::instance()->_glog_para._file_size;

		std::string prefix = CConfigManager::instance()->_glog_para._log_dir + "/" + log_name;
		LOG(INFO) << prefix;
		google::SetLogDestination(google::INFO,
			std::string(prefix + ".info_").c_str());
		google::SetLogDestination(google::WARNING, std::string(prefix + ".warning_").c_str());
		google::SetLogDestination(google::GLOG_ERROR, std::string(prefix + ".error_").c_str());
		google::SetLogDestination(google::FATAL, std::string(prefix + ".fatal_").c_str());

		google::InstallFailureSignalHandler();
		google::InstallFailureWriter(FailureHandler);

	}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
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

	bool CConfigManager::get_string(rapidjson::Value& v, string& str)
	{
		
		if (v.IsNull() || !v.IsString())
		{
			return false;
		}
	
		str = v.GetString();

		return true;
	}

	bool CConfigManager::get_int(rapidjson::Value& v, int&  iValue)
	{
		if (v.IsNull() || !v.IsInt())
		{
			return false;
		}

		iValue = v.GetInt();

		return true;

	}

	bool CConfigManager::get_db_paramer(string path)
	{
		Document doc;
		if (!ParseConf(path, doc))
		{
			LOG(ERROR) << "parse conf file failed,  file path : " << path;
			return false;
		}

		using rapidjson::Value;

		Value & host = doc["db"]["host"];

		if (host.IsNull() || !host.IsString())
		{
			LOG(ERROR) << "db host is null";
			return false;
		}

		db_para_.host_= host.GetString();

		LOG(INFO) << "db_para_.host_ : " << db_para_.host_;

		Value & user = doc["db"]["user"];

		if (user.IsNull() || !user.IsString())
		{
			LOG(ERROR) << "db user is null";
			return false;
		}

		db_para_.user_ = user.GetString();
		LOG(INFO) << "db_para_.user_ : " << db_para_.user_;

		Value & pass = doc["db"]["pass"];

		if (pass.IsNull() || !pass.IsString())
		{
			LOG(ERROR) << "db pass is null";
			return false;
		}

		db_para_.pass_ = pass.GetString();
		LOG(INFO) << "db_para_.pass_ : " << db_para_.pass_;

		Value &port = doc["db"]["port"];    

		if (port.IsNull() || !port.IsInt())
		{
			LOG(ERROR) << "db port is null";
			return false;
		}

		db_para_.port_ = port.GetInt();
		LOG(INFO) << "db_para_.port_ : " << db_para_.port_;

		Value &over_time = doc["db"]["over_time"];

		if (over_time.IsNull() || !over_time.IsInt())
		{
			LOG(ERROR) << "db over_time is null";
			return false;
		}

		db_para_.overTime_ = over_time.GetInt();
		LOG(INFO) << "db_para_.overTime_ : " << db_para_.overTime_;

		return true;
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

	bool CConfigManager::get_redis_param(std::string path)
	{
		Document doc;
		if (!ParseConf(path, doc))
		{
			LOG(ERROR) << "parse conf file failed,  file path : " << path;
			return false;
		}

		using rapidjson::Value;

		Value & ip = doc["redis"]["ip"];

		if (ip.IsNull() || !ip.IsString())
		{
			LOG(ERROR) << "redis ip is null";
			return false;
		}

		redis_para_.ip_ = ip.GetString();

		LOG(INFO) << "redis_para.ip_ : " << redis_para_.ip_;

		Value & port = doc["redis"]["port"];

		if (port.IsNull() || !port.IsInt())
		{
			LOG(ERROR) << "redis_para_.port_ is null";
			return false;
		}

		redis_para_.port_ = port.GetInt();
		LOG(INFO) << "redis_para_.port_ : " << redis_para_.port_;

		Value &over_time = doc["redis"]["over_time"];

		if (over_time.IsNull() || !over_time.IsInt())
		{
			LOG(ERROR) << "redis over_time is null";
			return false;
		}

		redis_para_.overTime_ = over_time.GetInt();
		LOG(INFO) << "redis_para_.overTime_ : " << redis_para_.overTime_;

		return true;

	}

	bool CConfigManager::get_main_param(std::string  path)
	{
		Document doc;
		if (!ParseConf(path, doc))
		{
			LOG(ERROR) << "parse conf file failed,  file path : " << path;
			return false;
		}

		using rapidjson::Value;

		if(!get_string(doc["ori_path"], ori_path_))
		{
			LOG(ERROR) << "ori_path empty";
			return false;
		}
		LOG(INFO) << "ori_path : " << ori_path_;

		if(!get_string(doc["success_path"], success_path_))
		{
			LOG(ERROR) << "success_path empty";
			return false;
		}
		LOG(INFO) << "success_path : " << success_path_;

		if(!get_string(doc["error_path"], error_path_))
		{
			LOG(ERROR) << "error_path empty";
			return false;
		}
		LOG(INFO) << "error_path : " << error_path_;

		if(!get_string(doc["bak_path"], bak_path_))
		{
			LOG(ERROR) << "bak_path empty";
			return false;
		}
		LOG(INFO) << "bak_path : " << bak_path_;
		
		if(!get_int(doc["crawler_server_listen_port"], crawler_server_listen_port_))
		{
			LOG(ERROR) << "crawler_server_listen_port empty";
			return false;
		}
		LOG(INFO) << "crawler_server_listen_port : " << crawler_server_listen_port_;

		if(!get_int(doc["crawler_client_server_listen_port"], crawler_client_server_listen_port_))
		{
			LOG(ERROR) << "crawler_client_server_listen_port empty";
			return false;
		}
		LOG(INFO) << "crawler_client_server_listen_port : " << crawler_client_server_listen_port_;

		if(!get_int(doc["label_server_port"], label_server_port_))
		{
			LOG(ERROR) << "laebl_server_port empty";
			return false;
		}
		LOG(INFO) << "label_server_port : " << label_server_port_;

		if(!get_string(doc["suanfa_ip"], suanfa_ip_))
		{
			LOG(ERROR) << "suanfa_ip empty";
			return false;
		}
		LOG(INFO) << "suanfa_ip : " << suanfa_ip_;

		if(!get_int(doc["suanfa_port"], suanfa_port_))
		{
			LOG(ERROR) << "suanfa_port empty";
			return false;
		}
		LOG(INFO) << "suanfa_port : " << suanfa_port_;

		if(!get_string(doc["crawler_ip"], crawler_ip_))
		{
			LOG(ERROR) << "crawler_ip empty";
			return false;
		}
		LOG(INFO) << "crawler_ip : " << crawler_ip_;

		if(!get_int(doc["crawler_port"], crawler_port_))
		{
			LOG(ERROR) << "crawler_port empty";
			return false;
		}
		LOG(INFO) << "crawler_port : " << crawler_port_;

		
		return true;

	}

}




