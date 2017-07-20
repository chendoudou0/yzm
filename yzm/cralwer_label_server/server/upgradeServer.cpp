#include"upgradeServer.h"
#include"config.h"   
#include"CLogger.h"  
#include"requestProcessor.h"
#include"redisOperator.h"
#include"dbPool.h"
#include"FreeImage.h" 

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <fstream>
#include <sstream>
#include <chrono>
#include<thread>
#include<glog/logging.h> 

#include "CrawlerService.h"
#include "LabelService.h"
#include"Pose3DAnnPreprocess.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

///////////client
#include <thrift/transport/TSocket.h>  


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::server::pose_label;
using namespace  ::ThriftPose3DAnn;

       
    
using namespace std;
using namespace std::chrono;

CLogger* 		                       gPtrAppLog = nullptr;
shared_ptr<CRedisOperator>             gPtrRedisOperator;
shared_ptr<CDbPool<CDBOperator> >      gPtrDbpool;
shared_ptr<CrawlerServiceClient>       gPtrAgClient;

cv::Mat ResizeImg(cv::Mat src, float dstWidth, float dstHeight)
{
	cv::Mat dst = cv::Mat::zeros(dstHeight, dstWidth, src.type());
	int fx = 0;
	int fy = 0;
	int newWidth = dstWidth;
	int newHeight = dstHeight;
	if (dstHeight / dstWidth < src.rows/(float)src.cols )
	{
		newWidth = src.cols * dstHeight/src.rows;
		fx = (dstWidth - newWidth) / 2;
	}
	else
	{
		newHeight = src.rows * dstWidth / src.cols;
		fy = (dstHeight - newHeight)/2;
	}
	
	cv::Mat tmp;
	cv::resize(src, tmp, cv::Size(newWidth, newHeight));
	tmp.copyTo(dst(cv::Rect(fx,fy,newWidth,newHeight)));
	return dst;
}

class CrawlerServiceHandler : virtual public CrawlerServiceIf {
 public:
  CrawlerServiceHandler() {
    // Your initialization goes here

  }


  void UploadRequest(ReturnVals& _return, const std::string& md5) {
    // Your implementation goes here
	
	_return.code = gPtrDbpool->GetDbOper()->QueryPicByMd5(md5);
	                          

	LOG(INFO) << "UploadRequest end, md5: " << md5 << " , ret code : " << _return.code;
   
  }

  void PicUpload(ReturnVals& _return, const PicInfo& pic) {
    // Your implementation goes here 
	do{
		string path = "../pic/";
		string oriUrl = path + pic.md5 + ".jpg";
		std::ofstream oriPic(oriUrl.c_str() , std::ofstream::binary);
		oriPic.write(pic.oriBin.c_str(), pic.oriBin.length());
		oriPic.close();

		string inflateUrl = path + pic.md5 +  "600*800.jpg";
		cv::Mat src = cv::imread(oriUrl); 
    	cv::Mat dst = ResizeImg(src, 600, 800);
		cv::imwrite(inflateUrl, dst); 
	    
		string str2dPose;
		string str3dPose;
		if(!preProcess(inflateUrl, str2dPose, str3dPose))
		{
			_return.code = -1;
			break;
		}
		if(-1 == gPtrDbpool->GetDbOper()->AddPicture(pic, oriUrl))
		{
			_return.code = -1;
			break;
		}

		if(!gPtrDbpool->GetDbOper()->UpdatePicPoseDate(oriUrl, str2dPose, str3dPose)){
			_return.code = -1;
			break;
		}
		
		_return.code = 0;

	}while(0);

	LOG(INFO) << "PicUpload end , ret code : " << _return.code;
 
  }

bool  connectPrehandle()
{
	
	LOG(INFO) << "connect to " << ip_ << " port "<< port_;
	socket_.reset(new TSocket(ip_, port_));
	transport_.reset(new TFramedTransport(socket_));
	protocol_.reset(new TBinaryProtocol(transport_));
	try
	{
		transport_->open();
	}
	catch (TException& tx)
	{
		LOG(ERROR) << "connect error, msg : " << tx.what();
		return false;
	}
	ppClient_.reset(new Pose3DAnnPreprocessClient(protocol_));

	connected_ = true;
	LOG(INFO) << "connect  shiyurong  success";
	return true;
	
	
}

bool  closeThrift()
{
	try
	{
		transport_->close();
	}
	catch (TException& tx)
	{
		LOG(ERROR) << "close error, msg : " << tx.what();
		return false;
	}
	connected_ = false;
	return true;
}

bool preProcess(string& url, string& str2dPose, string& str3dPose)
{
	bool ret = false;

	do{
		if(!connected_){
			if(!connectPrehandle()){
				closeThrift();
				break;
			}
		}
		/*
		FREE_IMAGE_FORMAT fmt = FreeImage_GetFileType(url.c_str(), 0);
		FIBITMAP* dib = FreeImage_Load(fmt, url.c_str(), 0);
		unsigned int w = FreeImage_GetWidth(dib);
		unsigned int h = FreeImage_GetHeight(dib);
		BITMAPINFOHEADER* bitInfo =  FreeImage_GetInfoHeader(dib);
		int channel = bitInfo->biBitCount/8;
		FreeImage_Unload(dib);
		img.width = w;   
		img.height = h;
		img.channel = channel;
		*/
		Image  img;
		cv::Mat mat = cv::imread(url); 
		img.width = mat.cols;
		img.height = mat.rows;
		img.channel = mat.channels();
		std::string buf(mat.datastart, mat.dataend);
		img.imgBin = buf;

		GuessInfo gsInfo;
		vector<HumanPose3DInfo>  tdInfo;
		try
		{
			if(!ppClient_->CheckIsValid(img)){
				break;
			}
		
			ppClient_->DoInitGuess(gsInfo, img);
			if(!gsInfo.isSuccess){
				break;
			}

			ppClient_->AutoCalPose3D(tdInfo, img, gsInfo);
			if(tdInfo.size() != 2){
				break;
			}
			
		}
		catch (TException& tx)
		{
			LOG(ERROR) << "catch exception,  msg : " << tx.what();
			closeThrift();
			break;
		}

		gen2dPoseJson(gsInfo, str2dPose);
		gen3dPoseJson(tdInfo, str3dPose);

		ret = true;

	}while(0);
	LOG(INFO) << "str2dPose " << str2dPose;
	LOG(INFO) << "str3dPose " << str3dPose;
	LOG(INFO) << "preProcess end";

	return ret;
}

bool gen2dPoseJson(GuessInfo& gsInfo, string& str2dPose)
{
	Document doc;
	Value    root(kObjectType); 

	Value    goArray(kArrayType);
    for (auto& it : gsInfo.guessOrien)
    {
		Value item(kObjectType);
		Value xAxis(kObjectType);
		xAxis.AddMember("x", it.xAxis.x , doc.GetAllocator());
		xAxis.AddMember("y", it.xAxis.x , doc.GetAllocator());
		xAxis.AddMember("z", it.xAxis.x , doc.GetAllocator());
		item.AddMember("xAxis", xAxis, doc.GetAllocator());

		Value yAxis(kObjectType);         
		yAxis.AddMember("x", it.yAxis.x , doc.GetAllocator());
		yAxis.AddMember("y", it.yAxis.x , doc.GetAllocator());
		yAxis.AddMember("z", it.yAxis.x , doc.GetAllocator());

		item.AddMember("yAxis", yAxis, doc.GetAllocator());

		Value zAxis(kObjectType);   
		zAxis.AddMember("x", it.zAxis.x , doc.GetAllocator());
		zAxis.AddMember("y", it.zAxis.x , doc.GetAllocator());
		zAxis.AddMember("z", it.zAxis.x , doc.GetAllocator());

		item.AddMember("zAxis", zAxis, doc.GetAllocator());

		goArray.PushBack(item, doc.GetAllocator());  
         
    }

	root.AddMember("guessOrien", goArray, doc.GetAllocator());
	///////////////////////////////////////////////////////////

	Value    gjArray(kArrayType);
    for (auto& it : gsInfo.guessJoints)
    {
		Value item(kObjectType);
		
		item.AddMember("x", it.x , doc.GetAllocator());
		item.AddMember("y", it.y , doc.GetAllocator());
		item.AddMember("z", it.z , doc.GetAllocator());
         
		gjArray.PushBack(item, doc.GetAllocator()); 
    }

	root.AddMember("guessJoints", gjArray, doc.GetAllocator());	

	Value    ciArray(kArrayType);
	for (auto& it : gsInfo.camInfo.camMatrix)
    {          
		Value item(kObjectType);
	
		item.AddMember("camera_paramer", it,  doc.GetAllocator());

		ciArray.PushBack(it, doc.GetAllocator()); 
    }

	root.AddMember("camInfo", ciArray, doc.GetAllocator());
	////////////////////////////////////////////////////////////
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	root.Accept(writer);
	str2dPose = buffer.GetString();

	return true;

}

bool gen3dPoseJson(vector<HumanPose3DInfo>&  hp3DInfoVec, string& str3dPose)
{
	Document doc;
	Value    root1(kObjectType);
	
	Value    hpArray(kArrayType);
	for (auto& hp3DInfo : hp3DInfoVec)
	{
		Value hpItem(kObjectType);
		
		Value    hjArray(kArrayType);
		for (auto& it : hp3DInfo.joints)
		{
			Value item(kObjectType);
			
			item.AddMember("x", it.x , doc.GetAllocator());
			item.AddMember("y", it.y , doc.GetAllocator());
			item.AddMember("z", it.z , doc.GetAllocator());
			
			hjArray.PushBack(item, doc.GetAllocator()); 
		}

		hpItem.AddMember("joints", hjArray, doc.GetAllocator());	

	
		Value vOrien(kObjectType);
		Value xAxis(kObjectType);
		xAxis.AddMember("x", hp3DInfo.orien.xAxis.x , doc.GetAllocator());
		xAxis.AddMember("y", hp3DInfo.orien.xAxis.x , doc.GetAllocator());
		xAxis.AddMember("z", hp3DInfo.orien.xAxis.x , doc.GetAllocator());
		vOrien.AddMember("xAxis", xAxis, doc.GetAllocator());

		Value yAxis(kObjectType);
		yAxis.AddMember("x", hp3DInfo.orien.yAxis.x , doc.GetAllocator());
		yAxis.AddMember("y", hp3DInfo.orien.yAxis.x , doc.GetAllocator());
		yAxis.AddMember("z", hp3DInfo.orien.yAxis.x , doc.GetAllocator());
		vOrien.AddMember("yAxis", yAxis, doc.GetAllocator());

		Value zAxis(kObjectType);
		zAxis.AddMember("x", hp3DInfo.orien.zAxis.x , doc.GetAllocator());
		zAxis.AddMember("y", hp3DInfo.orien.zAxis.x , doc.GetAllocator());
		zAxis.AddMember("z", hp3DInfo.orien.zAxis.x , doc.GetAllocator());
		vOrien.AddMember("zAxis", zAxis, doc.GetAllocator());
	
		hpItem.AddMember("orien", vOrien, doc.GetAllocator());

		Value    camArray(kArrayType);
		for (auto& it : hp3DInfo.camInfo.camMatrix)
		{
			Value item(kObjectType);
		
			item.AddMember("camera_paramer", it,  doc.GetAllocator());

			camArray.PushBack(it, doc.GetAllocator()); 
		}

		hpItem.AddMember("camInfo", camArray, doc.GetAllocator());
		hpArray.PushBack(hpItem, doc.GetAllocator());

	}
	root1.AddMember("hp3DInfoArray" , hpArray, doc.GetAllocator());
	////////////////////////////////////////////////////////////
	StringBuffer buffer1;
	Writer<StringBuffer> writer1(buffer1);
	root1.Accept(writer1);
	str3dPose = buffer1.GetString();

	return true;

}

private:
	boost::shared_ptr<TSocket>    socket_;
    boost::shared_ptr<TTransport> transport_; 
    boost::shared_ptr<TProtocol>  protocol_;
	boost::shared_ptr<Pose3DAnnPreprocessClient>  ppClient_;
	string ip_{"192.168.1.191"};
	int port_{10000};
	bool connected_{false}; 

};

class LabelServiceHandler : virtual public LabelServiceIf {
 public:
  LabelServiceHandler() {  
    // Your initialization goes here
  }

  void QueryUnlabeledPic(QueryUnlabeledRet& _return) {
	do{
		 SqlMapVector picVec;
		 if(!gPtrDbpool->GetDbOper()->QueryUnlabeledPicture(picVec)){
			 _return.code = -1;
			 _return.msg = "db query failed !";    
		 }



	}while(0);

  }

  void DownloadPic(DownloadRet& _return, const std::string& pic_url) {
	do{
		 SqlResultSet  sqlMap;
		 if(gPtrDbpool->GetDbOper()->QueryPicByURL(pic_url, sqlMap) != 0){
			 _return.code = -1;
			 _return.msg = "db query failed !";    
		 }
		 _return.HumanPose2DInfo = sqlMap["Fpre2DPoseInfo"];
		 _return.HumanPose3DInfo = sqlMap["Fpre3DPoseInfo"];

		 std::ifstream fin;
		 std::stringstream ss;
		 fin.open(pic_url, ifstream::binary);
		 if (!fin.is_open()){
			_return.code = -1;
			_return.msg = "read picture failed !";         
			break;
		}

		ss << fin.rdbuf();
		_return.bin = ss.str();
		fin.close();

	}while(0);

  }

  void InsertToDb(ReturnVals& _return, const std::string& poseInfo, const int32_t pic_id, const std::string& userName) {
    // Your implementation goes here
	if(!gPtrDbpool->GetDbOper()->AddLabeledDate(pic_id, userName,poseInfo ))
	{
		_return.code = -1;
	}
	else
	{
		_return.code = 0;
	}
  }

};

class CUpgradeServer::Imp
{
public:
	HttpSRV * httpPtr_{NULL};
	shared_ptr<thread>  crawlerServerThread_;
	shared_ptr<thread>  labelServerThread_;


public:
	bool init() 
	{
		initLog(LOG_PATH);

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

		if (!initRedis())
		{
			LOG(ERROR) << "init redis failed!";
			return false;
		} 

		return true;
	}

	bool initDB()
	{
		 
		gPtrDbpool.reset(new CDbPool<CDBOperator>);

		gPtrDbpool->SetPoolSize(4);

		gPtrDbpool->InitPool();
		
		return true;              
	}

	bool initRedis()
	{
		gPtrRedisOperator.reset(new CRedisOperator(config::CConfigManager::instance()->redis_para_.ip_ \
		, config::CConfigManager::instance()->redis_para_.port_,            \
		config::CConfigManager::instance()->redis_para_.overTime_) );

		gPtrRedisOperator->setPoolSize(16);

		return true;
	}

	bool initLog(string path) 
	{
		string tail = "-index";
		gPtrAppLog = new CLogger(path.c_str(), 
							1024*1024, 5,                  //1M
							CLogger::LOG_LEVEL(5),
							CLogger::SHIFT_MODE(2)); 
		gPtrAppLog->setSuffix(tail);

		gPtrAppLog->info("my log begin to work ......");
	}

	bool  readConf()     
	{    
		if (! config::CConfigManager::instance()->get_glog_paramer(CONF_PATH))
		{
			return false;
		}
		config::init_glog("upgrade_server");
		LOG(INFO) << "GLOG BEGIN TO WORK"; 

		if (! config::CConfigManager::instance()->get_db_paramer(CONF_PATH))
		{
			return false;
		}

		if (! config::CConfigManager::instance()->get_redis_param(CONF_PATH))  
		{
			return false;
		}

		if (! config::CConfigManager::instance()->get_main_param(CONF_PATH))  
		{
			return false;
		}

		return true;

	}

	void getServiceType(const string& uri, int& type)
	{
		if (uri == LIVE_SHOW_QUERY_URI)
		{
			type = LIVE_SHOW_VERSION_QUERY;
		}
		else if (uri == LIVE_SHOW_UPDATE_URI)
		{
			type = LIVE_SHOW_VERSION_UPDATE;
		}
		else if (uri == ANDROID_QUERY_URI)
		{
			type = VERSION_QUERY_ANDROID;
		}
		else if (uri == ANDROID_VERSION_ADD_URI)
		{
			type = ANDROID_VERSION_ADD;
		}
		else if(uri == LIVESHOW_VERSION_ADD_URI)
		{
			type = LIVESHOW_VERSION_ADD;
		}

	}


};

CUpgradeServer::CUpgradeServer()
{
	impPtr_.reset(new Imp);
}

CUpgradeServer::~CUpgradeServer()
{

}

bool  CUpgradeServer::Start()
{
	if (!impPtr_->init())
	{
		return  false;
	}
	impPtr_->httpPtr_ = HttpSRV_init();   
	auto  callback = bind(&CUpgradeServer::httpCallBack, this, placeholders::_1, placeholders::_2);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, LIVE_SHOW_QUERY_URI,     callback);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, LIVE_SHOW_UPDATE_URI,    callback);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, ANDROID_QUERY_URI,       callback);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, ANDROID_VERSION_ADD_URI, callback);
	HttpSRV_addRequestCB(impPtr_->httpPtr_, LIVESHOW_VERSION_ADD_URI, callback);  

	HttpSRV_start(impPtr_->httpPtr_,  config::CConfigManager::instance()->listen_port_);

	impPtr_->crawlerServerThread_.reset(new thread([](){
		int port = 10088;
  		boost::shared_ptr<CrawlerServiceHandler> handler(new CrawlerServiceHandler());
  		boost::shared_ptr<TProcessor> processor(new CrawlerServiceProcessor(handler));
 		boost::shared_ptr<TServerTransport>  serverTransport(new TServerSocket(port));
  		boost::shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
  		boost::shared_ptr<TProtocolFactory>  protocolFactory(new TBinaryProtocolFactory());

  		TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
	
 		server.serve();

	}));

	impPtr_->labelServerThread_.reset(new thread([](){
		int port = 10099;   
  		boost::shared_ptr<LabelServiceHandler> handler(new LabelServiceHandler());
  		boost::shared_ptr<TProcessor> processor(new LabelServiceProcessor(handler));
 		boost::shared_ptr<TServerTransport>  serverTransport(new TServerSocket(port));
  		boost::shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
  		boost::shared_ptr<TProtocolFactory>  protocolFactory(new TBinaryProtocolFactory());

  		TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
 		server.serve();

	}));

	boost::shared_ptr<TSocket> socket(new TSocket("localhost", 10000));  
    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));  
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));  
  
    try
	{
		transport->open();
	}
	catch (TException& tx)
	{
		LOG(ERROR) << "connect error, msg : " << tx.what();
	
	} 
    
	Pose3DAnnPreprocessClient client(protocol);
	LOG(INFO) << "connect  shiyurong  success";
	


	GuessInfo gsInfo;
	vector<Orientation> guessOrien; 
	Orientation ori;
	ori.xAxis.x = 1.11;
	ori.xAxis.y = 1.11;
	ori.xAxis.z = 1.11;

	ori.yAxis.x = 2.22;
	ori.yAxis.y = 2.22;
	ori.yAxis.z = 2.22;

	ori.zAxis.x = 3.33;
	ori.zAxis.y = 3.33;
	ori.zAxis.z = 3.33;

	guessOrien.push_back(ori);
	guessOrien.push_back(ori);
	gsInfo.guessOrien = guessOrien;

	vector<Vector3DFloat> guessJoints;
	Vector3DFloat tdf;
	tdf.x = 4.44;
	tdf.y = 4.44;
	tdf.z = 4.44;
	guessJoints.push_back(tdf);
	guessJoints.push_back(tdf);
	gsInfo.guessJoints = guessJoints;

	CameraInfo camInfo;
	camInfo.camMatrix.push_back(5.55);
	camInfo.camMatrix.push_back(5.55);
	gsInfo.camInfo = camInfo;
	//////////////////////////////////
	Document doc;
	Value    root(kObjectType); 

	Value    goArray(kArrayType);
    for (auto& it : gsInfo.guessOrien)
    {
		Value item(kObjectType);
		Value xAxis(kObjectType);
		xAxis.AddMember("x", it.xAxis.x , doc.GetAllocator());
		xAxis.AddMember("y", it.xAxis.x , doc.GetAllocator());
		xAxis.AddMember("z", it.xAxis.x , doc.GetAllocator());
		item.AddMember("xAxis", xAxis, doc.GetAllocator());

		Value yAxis(kObjectType);         
		yAxis.AddMember("x", it.yAxis.x , doc.GetAllocator());
		yAxis.AddMember("y", it.yAxis.x , doc.GetAllocator());
		yAxis.AddMember("z", it.yAxis.x , doc.GetAllocator());

		item.AddMember("yAxis", yAxis, doc.GetAllocator());

		Value zAxis(kObjectType);   
		zAxis.AddMember("x", it.zAxis.x , doc.GetAllocator());
		zAxis.AddMember("y", it.zAxis.x , doc.GetAllocator());
		zAxis.AddMember("z", it.zAxis.x , doc.GetAllocator());

		item.AddMember("zAxis", zAxis, doc.GetAllocator());

		goArray.PushBack(item, doc.GetAllocator());  
         
    }

	root.AddMember("guessOrien", goArray, doc.GetAllocator());
	///////////////////////////////////////////////////////////

	Value    gjArray(kArrayType);
    for (auto& it : gsInfo.guessJoints)
    {
		Value item(kObjectType);
		
		item.AddMember("x", it.x , doc.GetAllocator());
		item.AddMember("y", it.y , doc.GetAllocator());
		item.AddMember("z", it.z , doc.GetAllocator());
         
		gjArray.PushBack(item, doc.GetAllocator()); 
    }

	root.AddMember("guessJoints", gjArray, doc.GetAllocator());	

	Value    ciArray(kArrayType);
	for (auto& it : gsInfo.camInfo.camMatrix)
    {          
		Value item(kObjectType);
	
		item.AddMember("camera_paramer", it,  doc.GetAllocator());

		ciArray.PushBack(item, doc.GetAllocator()); 
    }

	root.AddMember("camInfo", ciArray, doc.GetAllocator());
	////////////////////////////////////////////////////////////
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	root.Accept(writer);
	string strOutput= buffer.GetString();
	LOG(ERROR) << "GuessInfo  : " << strOutput;
///////////////////////////////////////////////////////////////// HumanPose3DInfo
	
	vector<HumanPose3DInfo>  hp3DInfoVec;
	HumanPose3DInfo  hp3DInfo;
	vector<Vector3DFloat>  joints;
	
	joints.push_back(tdf);
	joints.push_back(tdf);
	hp3DInfo.joints = joints;
	hp3DInfo.orien = ori;
	hp3DInfo.camInfo = camInfo;

	hp3DInfoVec.push_back(hp3DInfo);   
	hp3DInfoVec.push_back(hp3DInfo);

	Document doc1;
	Value    root1(kObjectType);
	
	Value    hpArray(kArrayType);
	for (auto& hp3DInfo : hp3DInfoVec)
	{
		Value hpItem(kObjectType);
		
		Value    hjArray(kArrayType);
		for (auto& it : hp3DInfo.joints)
		{
			Value item(kObjectType);
			
			item.AddMember("x", it.x , doc.GetAllocator());
			item.AddMember("y", it.y , doc.GetAllocator());
			item.AddMember("z", it.z , doc.GetAllocator());
			
			hjArray.PushBack(item, doc.GetAllocator()); 
		}

		hpItem.AddMember("joints", hjArray, doc.GetAllocator());	

	
		Value vOrien(kObjectType);
		Value xAxis(kObjectType);
		xAxis.AddMember("x", hp3DInfo.orien.xAxis.x , doc.GetAllocator());
		xAxis.AddMember("y", hp3DInfo.orien.xAxis.x , doc.GetAllocator());
		xAxis.AddMember("z", hp3DInfo.orien.xAxis.x , doc.GetAllocator());
		vOrien.AddMember("xAxis", xAxis, doc.GetAllocator());

		Value yAxis(kObjectType);
		yAxis.AddMember("x", hp3DInfo.orien.yAxis.x , doc.GetAllocator());
		yAxis.AddMember("y", hp3DInfo.orien.yAxis.x , doc.GetAllocator());
		yAxis.AddMember("z", hp3DInfo.orien.yAxis.x , doc.GetAllocator());
		vOrien.AddMember("yAxis", yAxis, doc.GetAllocator());

		Value zAxis(kObjectType);
		zAxis.AddMember("x", hp3DInfo.orien.zAxis.x , doc.GetAllocator());
		zAxis.AddMember("y", hp3DInfo.orien.zAxis.x , doc.GetAllocator());
		zAxis.AddMember("z", hp3DInfo.orien.zAxis.x , doc.GetAllocator());
		vOrien.AddMember("zAxis", zAxis, doc.GetAllocator());
	
		hpItem.AddMember("orien", vOrien, doc.GetAllocator());

		Value    camArray(kArrayType);
		for (auto& it : hp3DInfo.camInfo.camMatrix)
		{
			Value item(kObjectType);
		
			item.AddMember("camera_paramer", it,  doc.GetAllocator());

			camArray.PushBack(item, doc.GetAllocator()); 
		}

		hpItem.AddMember("camInfo", camArray, doc.GetAllocator());
		hpArray.PushBack(hpItem, doc.GetAllocator());

	}
	root1.AddMember("hp3DInfoArray" , hpArray, doc.GetAllocator());
	////////////////////////////////////////////////////////////
	StringBuffer buffer1;
	Writer<StringBuffer> writer1(buffer1);
	root1.Accept(writer1);
	string strOutput1= buffer1.GetString();
	LOG(ERROR) << "HumanPose3DInfo  : " << strOutput1;
	

/*
	boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));  
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));  
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport)); 
    transport->open();  
	gPtrAgClient.reset(new CrawlerServiceClient(protocol));
*/
	sleep(1000);
	return  true;

}

void  CUpgradeServer::Stop()
{
     HttpSRV_destory(impPtr_->httpPtr_);
	 
	 LOG(INFO) << "UpgradeServer stop";
}
	 
int CUpgradeServer::httpCallBack(const RecvInfo& pRecvInfo, string&  response)
{
	LOG(INFO) << "httpCallBack begin ...";
	LOG(INFO) << "uri "<< pRecvInfo.uri_ << ", receive data  "<< pRecvInfo.data_;

	auto begin = steady_clock::now();
	int type = 0;
	impPtr_->getServiceType(pRecvInfo.uri_, type);

	shared_ptr<CrequestProcessor> request_pro(new CrequestProcessor(pRecvInfo.data_.c_str()));
	switch (type)
	{
	case LIVE_SHOW_VERSION_QUERY :    
		 request_pro->dealwith_livershow_verison_query();
	     response = request_pro->get_response();

		 break;
	case LIVE_SHOW_VERSION_UPDATE:
		 request_pro->dealwith_liveshow_version_update();
	     response = request_pro->get_response();

		 break;
	case VERSION_QUERY_ANDROID:
	     request_pro->dealwith_android_verison_query();
	     response = request_pro->get_response();
	    
		 break;
	case ANDROID_VERSION_ADD:
	     request_pro->dealwith_android_version_add();
	     response = request_pro->get_response();
	    
		 break;
	case LIVESHOW_VERSION_ADD:
	     request_pro->dealwith_liveshow_version_add();
	     response = request_pro->get_response();
	    
		 break;
	default:
		 break;
	}
	auto end = steady_clock::now();
    auto cost = duration_cast<milliseconds>(end - begin).count();
	LOG(INFO) << "response buffer length : " << response.length() << "   data :" << response;
	LOG(INFO) << "callback end, cost " << cost << "ms";
	
	return 0;  
}

