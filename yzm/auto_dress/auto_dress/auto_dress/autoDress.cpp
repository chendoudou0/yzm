#include"autoDress.h"
#include"config.h"
#include "zlib/yzm_zip_helper.h"
#include "md5/yzm_md5.h"
#include "jsonOperator.h"
#include "db/DBOperator.h"    
#include"db/dbPool.h"


#include <glog/logging.h> 
#include <chrono>
#include <list>
#include <map>
#include <iostream>
#include <thread>
#include <fstream>
#include <conio.h>
#include <filesystem>
#include "windows.h"

using namespace std;
using namespace std::chrono;

const std::string  download_dir = "d:/zip_download/";
const std::string  unzip_dir    = "d:/unzip/";
const int MAX_TASK_POOL_SIZE = 30;

shared_ptr<CDbPool<CDBOperator>>      gPtrDbpool;

bool  unzip(const std::string&  zipPath, const std::string& dir) {	
	bool ret = false;
	do 
	{
		yzm::ZipReader zipReader;
		if (!zipReader.open(zipPath))
			break;
		if (!zipReader.setBaseDir(dir))
			break;
		if (!zipReader.uncompress())
			break;
		zipReader.close();
		ret = true;
	} while (0);
	LOG(INFO) << "unzip " << zipPath << (ret ? " success" : " failed");
	
	return ret;
}

/*
*@ 计算文件MD5值
*/
std::string calcFileMD5(const std::string& filename)
{
	std::filebuf *pbuf;
	std::ifstream file;
	long size;
	char * buffer;
	// 要读入整个文件，必须采用二进制打开   
	file.open(filename.c_str(), std::ios::binary);
	if (!file)
	{
		return "";
	}
	// 获取filestr对应buffer对象的指针   
	pbuf = file.rdbuf();
	// 调用buffer对象方法获取文件大小  
	size = pbuf->pubseekoff(0, std::ios::end, std::ios::in);
	pbuf->pubseekpos(0, std::ios::in);
	// 分配内存空间  
	buffer = new char[size];
	// 获取文件内容	
	pbuf->sgetn(buffer, size);
	//std::string data = buffer;	
	//md5校验
	yzm::MD5 objMD5;
	objMD5.generate((unsigned char*)buffer, (int)size);
	char output[33];
	for (int j = 0; j < 16; j++)
	{
		sprintf(output + j * 2, "%02x", ((unsigned char*)objMD5.m_data)[j]);
	}
#ifdef _DEBUG
	std::cout << "MD5(\"" << filename << "\") = " << output << std::endl;
#endif
	// 关闭文件
	file.close();
	//回收堆栈
	delete[]buffer;
	return output;
}

/*
*@ 计算文件MD5值
*/
std::string calcBufferMD5(const char* dataBuffer, int dataLen)
{
	//md5校验
	yzm::MD5 objMD5;
	objMD5.generate((unsigned char*)dataBuffer, (int)dataLen);
	char output[33];
	for (int j = 0; j < 16; j++)
	{
		sprintf(output + j * 2, "%02x", ((unsigned char*)objMD5.m_data)[j]);
	}
	return output;
}




void find(const char * lpPath)
{
	char szFind[2014] = { 0 };
	WIN32_FIND_DATA  FindFileData;
	strcpy(szFind, lpPath);
	strcat(szFind, "/*.*");
 	HANDLE hFind = FindFirstFile(szFind, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)  return;
	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0)
			{
				char szFile[2014] = { 0 };
				strcpy(szFile, lpPath);
				strcat(szFile, "/");
				strcat(szFile, FindFileData.cFileName);
				find(szFile);
			}
		}
		else
		{
			cout << FindFileData.cFileName;
		}
		if (!FindNextFile(hFind, &FindFileData)) break;
	}
	FindClose(hFind);
}

void findDir(const char * lpPath, vector<pair<string, int>>& dirVec)
{
	char szFind[2014] = { 0 };
	WIN32_FIND_DATA  FindFileData;
	strcpy(szFind, lpPath);
	strcat(szFind, "/*.*");
	HANDLE hFind = FindFirstFile(szFind, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)  return;
	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0)
			{
				char szFile[2014] = { 0 };
				strcpy(szFile, lpPath);
				strcat(szFile, "/");
				strcat(szFile, FindFileData.cFileName);
				int sex = 0;
				if (FindFileData.cFileName[0] == '0')
				{
					sex = 0;
				}else
				{
					sex = 1;
				}
				
				dirVec.push_back(make_pair(szFile, sex));
			}
		}
		if (!FindNextFile(hFind, &FindFileData)) break;
	}
	FindClose(hFind);
}


void findPic(const char * lpPath, string& pngPath, string& jpgPath)
{
	char szFind[2014] = { 0 };
	WIN32_FIND_DATA  FindFileData;
	strcpy(szFind, lpPath);
	strcat(szFind, "/*.*");
	HANDLE hFind = FindFirstFile(szFind, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)  return;
	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			
		}
		else
		{
			string path = lpPath;
			path += "/";
			path += FindFileData.cFileName;
			int pos = 0;
			pos = path.find_last_of('.');
			string tail = path.substr(pos + 1);
			if (tail == "png")
			{
				pngPath = path;
			}else if (tail == "jpg")
			{
				jpgPath = path;
			}
		}
		if (!FindNextFile(hFind, &FindFileData)) break;
	}
	FindClose(hFind);
}

bool downloadFromOSS(const string& file_path, const string& bucket, const string& object) {
	LOG(INFO) << "download " << file_path << " begin";
	bool ret = false;
	do
	{
		CJsonOperator* pJson = new CJsonOperator();
		pJson->addStringMember("file_path", file_path);
		pJson->addStringMember("bucket", bucket);
		pJson->addStringMember("object", object);
		pJson->genOutput();
		string send = pJson->getOutput();
		delete pJson;

		HttpClient cli;
		string resp, error;
		string uri = "http://127.0.0.1:18888/download";
		if (!cli.send(uri, send, resp, error))
		{
			LOG(ERROR) << "download " << file_path << " failed";
			break;
		}
		//////////////////////////////////////////
		Document doc;
		doc.Parse<0>(resp.c_str());
		if (doc.HasParseError())
		{
			rapidjson::ParseErrorCode code = doc.GetParseError();
			LOG(ERROR) << "parse json failed";
			break;
		}
		if (doc["ret"].IsNull() || !doc["ret"].IsString())
		{
			break;
		}
		string dRet = doc["ret"].GetString();
		if (dRet == "1")
		{
			break;
		}

		ret = true;
	} while (0);
	LOG(INFO) << "download " << file_path << (ret ? " success" : " failed");
	return ret;
}

bool uploadToOSS(const string& file_path, const string& bucket, const string& object) {
	LOG(INFO) << "upload " << file_path << " begin";
	bool ret = false;
	do
	{
		CJsonOperator* pJson = new CJsonOperator();
		pJson->addStringMember("file_path", file_path);
		pJson->addStringMember("bucket", bucket);
		pJson->addStringMember("object", object);
		pJson->genOutput();
		string send = pJson->getOutput();
		delete pJson;

		HttpClient   client;
		string resp, error;
		string uri = "http://127.0.0.1:18888/upload";
		if (!client.send(uri, send, resp, error))
		{
			LOG(ERROR) << "upload " << file_path << " failed";
			break;
		}
		///////////////////////////////////////////
		Document doc;
		doc.Parse<0>(resp.c_str());
		if (doc.HasParseError())
		{
			rapidjson::ParseErrorCode code = doc.GetParseError();
			LOG(ERROR) << "parse json failed";
			break;
		}
		if (doc["ret"].IsNull() || !doc["ret"].IsString()){
			break;
		}
		string dRet = doc["ret"].GetString();
		if (dRet == "1"){
			break;
		}

		ret = true;

	} while (0);
	string  flag;
	if (ret)
	{
		flag = " successed";
	}
	else
	{
		flag = " failed";
	}
	LOG(INFO) << "upload " << file_path << flag;

	return ret;
}

struct TASK_INFO
{
	int task_id_{-1};
	string zip_url_;
	vector<ClothInfo>    clothesVec_;
};

class CTask
{
public:
	CTask()  = default;
	~CTask() = default;
	void begin() {
		pThread_.reset(new thread([this](){
			while (!this->stop_)
			{
				if (this->taskList_.empty())
				{
					this_thread::sleep_for(chrono::seconds(3));
					continue;
				}
				TASK_INFO ti = this->taskList_.front();
				this->taskList_.pop_front();
				///下载解压   
				int front = ti.zip_url_.find_first_of("//");
				int back  = ti.zip_url_.find_first_of('.');
				string  bucket = ti.zip_url_.substr(front + 2, back - front -2);
				front = ti.zip_url_.find("aliyuncs.com");
				string  object = ti.zip_url_.substr(front + 1 +  12);
				front = ti.zip_url_.find_last_of('/');
				string  zip_name = ti.zip_url_.substr(front + 1);
				back = zip_name.find_last_of('.');
				string  dir_name = zip_name.substr(0, back);
				if (!downloadFromOSS(download_dir + zip_name, bucket, object))
				{
					continue;
				}
				if (!unzip(download_dir + zip_name, unzip_dir))
				{
					continue;
				}
				vector<pair<string, int>>  clothVec;
				string unzip_dir_path = unzip_dir + dir_name;
				findDir(unzip_dir_path.c_str(), clothVec);
				for (auto& cloth : clothVec)
				{
					ClothInfo clothInfo;
					clothInfo.sex_ = cloth.second;
					findPic(cloth.first.c_str(), clothInfo.png_path_, clothInfo.jpg_path_);
					clothInfo.cloth_name_ = calcFileMD5(clothInfo.png_path_);
					clothInfo.bucket_ = bucket;
					clothInfo.task_id_ = ti.task_id_;
					ti.clothesVec_.push_back(clothInfo);
				}
	
				for (auto& clothes : ti.clothesVec_)
				{
					////////////////////////////////////算法处理
					////////////////////////////////////test
					clothes.matt_path_  = "d:/pic_dressed/"  +  clothes.cloth_name_  +  "/3333.jpg";
					clothes.dress_path_ = "d:/pic_dressed/"  +  clothes.cloth_name_  +  "/4444.jpg";

					std::tr2::sys::path p1(clothes.matt_path_.c_str());
					std::tr2::sys::create_directories(p1);
					////////////////////////////////////test
					////////////////////////////////////上传图片
					this->uploadPicsToOSS(clothes);
					this->installOssUrl(clothes);
					///////////////////////////////////写入数据库
					if (gPtrDbpool->GetDbOper()->AddClothes(clothes) != 0) {
						LOG(ERROR) << " add clothes to db failed ";
					}
				}
				this->completeTask_[ti.task_id_] = ti;

			}
		}));

	}
	enum ADD_RET_CODE {
		success    = 0,
		over_flow   = 1,
		para_error = 2,
	};
	void genAddRequestResp(int code, string& resp) {
		string msg;
		CJsonOperator* pJson = new CJsonOperator();
		switch (code)
		{
		case 0:
			msg = "success";
			break;
		case 1:
			msg = "can not add task, task is over flow";
			break;
		case 2:
			msg = "sorry, parameter error";
			break;
		default:
			break;
		}
		pJson->addStringMember("msg", msg);
		pJson->addIntegerMember("code", code);
		pJson->genOutput();
		resp = pJson->getOutput();
		delete pJson;

	}

	void genQueryRequestResp(int code, TASK_INFO& ti, string& resp) {
		string msg;
		switch (code)
		{
		case 0:
			msg = "success";
			break;
		case 1:
			msg = "no such task";
			break;
		case 2:
			msg = "task uncompleted";
			break;
		case 3:
			msg = "sorry, parameter error";
			break;
		default:
			break;
		}
	
		Document doc;
		Value    root(kObjectType);  

		root.AddMember("code", code, doc.GetAllocator());
		Value msgValue;
		msgValue.SetString(msg.c_str(), doc.GetAllocator());
		root.AddMember("msg", msgValue, doc.GetAllocator());
		
		Value    clothArray(kArrayType);
		for (auto& cloth : ti.clothesVec_)
		{
			Value item(kObjectType);

			Value nameValue;
			nameValue.SetString(cloth.cloth_name_.c_str(), doc.GetAllocator());
			item.AddMember("name", nameValue, doc.GetAllocator());
			Value dressValue;
			dressValue.SetString(cloth.dress_url_.c_str(), doc.GetAllocator());
			item.AddMember("dress_url", dressValue, doc.GetAllocator());
			Value mattValue;
			mattValue.SetString(cloth.matt_url_.c_str(), doc.GetAllocator());
			item.AddMember("matt_url", mattValue, doc.GetAllocator());
			item.AddMember("status", cloth.status_, doc.GetAllocator());
			Value infoValue;
			infoValue.SetString(cloth.info_.c_str(), doc.GetAllocator());
			item.AddMember("info", infoValue, doc.GetAllocator());
			
			clothArray.PushBack(item, doc.GetAllocator());
		}
		root.AddMember("list", clothArray, doc.GetAllocator());
		////////////////////////////////////////////////////////////
		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		root.Accept(writer);
		resp = buffer.GetString();

	}
	void add(const char* data, string& resp) {
		ADD_RET_CODE code;
		do 
		{
			int     task_id;
			string  zip_url;
			if (!checkAddParam(data, task_id, zip_url))
			{
				code = para_error;
				break;
			}
			TASK_INFO  taskInfo;
			taskInfo.task_id_ = task_id;
			taskInfo.zip_url_ = zip_url;
			if (taskList_.size() < MAX_TASK_POOL_SIZE)
			{
				taskList_.push_back(taskInfo);
				code = success;
			}
			else
			{
				code = over_flow;
				LOG(INFO) << "task is over flow";
			}

		} while (0);
		genAddRequestResp(code, resp);
	}
	enum QUERY_RET_CODE {
		query_success        = 0,
		no_such_task		 = 1,
		task_uncomplete      = 2,
		query_para_error     = 3,
	};
	void  getTaskInfo(int task_id, QUERY_RET_CODE& code, TASK_INFO& ti){
		do 
		{
			if (completeTask_.find(task_id) != completeTask_.end() )
			{
				ti = completeTask_[task_id];
				code = query_success;
				break;
			}
			for (auto& task: taskList_)
			{
				if (task.task_id_ == task_id)
				{
					code = task_uncomplete;
					break;
				}
			}
			
			code = no_such_task;

		} while (0);
	
	}
	void query(const char* data, string& resp) {
		QUERY_RET_CODE code;
		TASK_INFO ti;
		do
		{
			int     task_id;
			if (!checkQueryParam(data, task_id))
			{
				code = query_para_error;
				break;
			}
			getTaskInfo(task_id, code, ti);
		
		} while (0);
		genQueryRequestResp(code, ti, resp);
	}
	void uploadPicsToOSS(ClothInfo pic) {
		bool  ret = false;
		string pic_base_dir = "clothes/";
		do 
		{
			int pos = pic.png_path_.find_last_of('/');
			string png_name = pic.png_path_.substr(pos + 1);
			string png_object = pic_base_dir + pic.cloth_name_ + "/" +  png_name;
			uploadToOSS(pic.png_path_, pic.bucket_, png_object);

			pos = pic.jpg_path_.find_last_of('/');
			string jpg_name = pic.jpg_path_.substr(pos + 1);
			string jpg_object = pic_base_dir + pic.cloth_name_ + "/" + jpg_name;
			uploadToOSS(pic.jpg_path_, pic.bucket_, jpg_object);

			pos = pic.matt_path_.find_last_of('/');
			string matt_name = pic.matt_path_.substr(pos + 1);
			string matt_object = pic_base_dir + pic.cloth_name_ + "/" + matt_name;
			uploadToOSS(pic.matt_path_, pic.bucket_, matt_object);

			pos = pic.dress_path_.find_last_of('/');
			string dress_name = pic.dress_path_.substr(pos + 1);
			string dress_object = pic_base_dir + pic.cloth_name_ + "/" + dress_name;
			uploadToOSS(pic.dress_path_, pic.bucket_, dress_object);

		} while (0);
		
	}
	void installOssUrl(ClothInfo& clothes) {
		//D:/pic_dressed/0_chenzixun/333.jpg
		//http://auto-dress.oss-cn-shenzhen.aliyuncs.com/clothes/0_chenzixun/1111.jpg
		int pos = clothes.png_path_.find_last_of('/');
		string name = clothes.png_path_.substr(pos + 1);
		clothes.png_url_ = "http://" + clothes.bucket_ + ".oss-cn-shenzhen.aliyuncs.com/clothes/" + clothes.cloth_name_ + "/" + name;
		  
		pos = clothes.jpg_path_.find_last_of('/');     
		name = clothes.jpg_path_.substr(pos + 1);
		clothes.jpg_url_ = "http://" + clothes.bucket_ + ".oss-cn-shenzhen.aliyuncs.com/clothes/" + clothes.cloth_name_ + "/" + name;

		pos = clothes.matt_path_.find_last_of('/');
		name = clothes.matt_path_.substr(pos + 1);
		clothes.matt_url_ = "http://" + clothes.bucket_ + ".oss-cn-shenzhen.aliyuncs.com/clothes/" + clothes.cloth_name_ + "/" + name;

		pos = clothes.dress_path_.find_last_of('/');
		name = clothes.dress_path_.substr(pos + 1);
		clothes.dress_url_ = "http://" + clothes.bucket_ + ".oss-cn-shenzhen.aliyuncs.com/clothes/" + clothes.cloth_name_ + "/" + name;

	}
	bool checkAddParam(const char* data, int& task_id, string&  zip_url ) {
		bool  ret = false;  
		do    
		{
			Document doc;
			doc.Parse<0>(data);
			if (doc.HasParseError())
			{
				rapidjson::ParseErrorCode code = doc.GetParseError();
				LOG(ERROR) << "parse json failed";
				break;
			}

			if (doc["task_id"].IsNull() || !doc["task_id"].IsInt())
			{
				break;
			}
			task_id = doc["task_id"].GetInt();

			if (doc["zip_url"].IsNull() || !doc["zip_url"].IsString())
			{
				break;
			}
			zip_url = doc["zip_url"].GetString();
			
			ret = true;
		} while (0);

		return  ret;
	}
	bool checkQueryParam(const char* data, int& task_id) {
		bool  ret = false;
		do
		{
			Document doc;
			doc.Parse<0>(data);
			if (doc.HasParseError())
			{
				rapidjson::ParseErrorCode code = doc.GetParseError();
				LOG(ERROR) << "parse json failed";
				break;
			}

			if (doc["task_id"].IsNull() || !doc["task_id"].IsInt())
			{
				break;
			}
			task_id = doc["task_id"].GetInt();
			ret = true;

		} while (0);

		return  ret;
	}
	

private:
	list<TASK_INFO>      taskList_;
	using COMPLETE_TASK = map<int, TASK_INFO>;
	COMPLETE_TASK        completeTask_;
	bool                 stop_{ false };
	shared_ptr<thread>   pThread_;
	
};

class CAutoDressServer::Imp
{
public:
	HttpSRV * httpPtr_{ NULL };
	std::unique_ptr<CTask>  pTask_;

public:
	bool init()
	{
		if (!readConf())
		{
			LOG(ERROR) << "read conf paramer failed!";
			return false;
		}
		if (!initDB())
		{
			LOG(ERROR) << "init  db  failed!";
			return false;
		}
		beginThread();
		return true;
	}

	bool initDB()
	{
		gPtrDbpool.reset(new CDbPool<CDBOperator>);
		gPtrDbpool->SetPoolSize(4);
		gPtrDbpool->InitPool();
		return true;
	}

	bool  readConf()
	{
		if (!config::CConfigManager::instance()->get_glog_paramer(CONF_PATH))
		{
			return false;
		}
		config::init_glog("auto_dress_server");
		LOG(INFO) << "GLOG BEGIN TO WORK.....";

		if (!config::CConfigManager::instance()->get_db_paramer(CONF_PATH))
		{
			return false;
		}
		if (!config::CConfigManager::instance()->get_main_param(CONF_PATH))
		{
			return false;
		}
		return true;
	}
	void beginThread() {
		pTask_.reset(new CTask());
		pTask_->begin();
	}

};

CAutoDressServer::CAutoDressServer()
{
	impPtr_.reset(new Imp);
}

CAutoDressServer::~CAutoDressServer()
{
}

bool  CAutoDressServer::Start()
{
	if (!impPtr_->init())   
	{
		return  false;
	}
	impPtr_->httpPtr_ = HttpSRV_init();
	auto  callback = bind(&CAutoDressServer::httpCallBack, this, placeholders::_1, placeholders::_2);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, TASK_ADD_URI.c_str(),   callback);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, TASK_QUERY_URI.c_str(), callback);
	
	HttpSRV_start(impPtr_->httpPtr_, config::CConfigManager::instance()->listen_port_);

	while (_getch() != 'q');
	HttpSRV_destory(impPtr_->httpPtr_);

	return  true;
}

void  CAutoDressServer::Stop()
{
	HttpSRV_destory(impPtr_->httpPtr_);

	LOG(INFO) << "AutoDressServer stop";
}

int CAutoDressServer::httpCallBack(const RecvInfo& pRecvInfo, string&  response)
{
	LOG(INFO) << "httpCallBack begin ...";
	LOG(INFO) << "receive data  " << pRecvInfo.data_;
	auto begin = steady_clock::now();

	if (pRecvInfo.uri_ == TASK_ADD_URI)
	{
		LOG(INFO) << "receive add_task request ";
		impPtr_->pTask_->add(pRecvInfo.data_.c_str(), response);
	}
	else if (pRecvInfo.uri_ == TASK_QUERY_URI)
	{
		LOG(INFO) << "receive query_task request ";
		impPtr_->pTask_->query(pRecvInfo.data_.c_str(), response);
	}

	auto end = steady_clock::now();
	auto cost = duration_cast<milliseconds>(end - begin).count();
	LOG(INFO) << "response buffer length : " << to_string(response.length())  << "   data :" << response;
	LOG(INFO) << "callback end, cost " << to_string(cost) << "ms";

	return 0;
}

