#include"poseLabelServer.h"
#include"config.h"
#include "DBOperator.h"   
#include"CLogger.h"  
#include"dbPool.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <fstream>
#include <sstream>
#include <chrono>
#include<thread>
#include<glog/logging.h> 

#include "DataService.h"
#include "LabelService.h"
#include "CrawlerClientService.h"
#include "CrawlerService.h"
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
shared_ptr<CDbPool<CDBOperator> >      gPtrDbpool;
int  gTotalPic;
int  gDbPic;
 
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
	if(src.data == nullptr){
		return tmp;
	}
	cv::resize(src, tmp, cv::Size(newWidth, newHeight));
	tmp.copyTo(dst(cv::Rect(fx,fy,newWidth,newHeight)));
	return dst;
}

bool MoveFile(std::string oriUrl, std::string dstUrl)
{
	std::ifstream fin;
	std::stringstream ss;
	fin.open(oriUrl,  ios::binary);
	if (!fin.is_open()){
		return false;
	}

	ss << fin.rdbuf();
	string bin = ss.str();
	fin.close();

	std::ofstream fout(dstUrl.c_str() , std::ofstream::binary);
	fout.write(bin.c_str(), bin.length());
	fout.close();

	remove(oriUrl.c_str());

	return true;
}

bool CopyFile(std::string oriUrl, std::string dstUrl)
{
	std::ifstream fin;
	std::stringstream ss;
	fin.open(oriUrl,  ios::binary);
	if (!fin.is_open()){
		return false;
	}

	ss << fin.rdbuf();
	string bin = ss.str();
	fin.close();

	std::ofstream fout(dstUrl.c_str() , std::ofstream::binary);
	fout.write(bin.c_str(), bin.length());
	fout.close();

	return true;
}

class CPreProcessor
{
public:
	bool  connectPrehandle()
	{
	
		LOG(INFO) << "connect to suanfa : " << suanfa_ip_ << " port "<< suanfa_port_;
		socket_.reset(new TSocket(suanfa_ip_, suanfa_port_));
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

void genBodyPosJson(ImgAABB& aabb, string& bodyPosJson){
	Document doc;
	Value    root(kObjectType);
	
	root.AddMember("top_x" , aabb.minX, doc.GetAllocator());
	root.AddMember("top_y" , aabb.minY, doc.GetAllocator());
	root.AddMember("bottom_x" , aabb.maxX, doc.GetAllocator());
	root.AddMember("bottom_y" , aabb.maxY, doc.GetAllocator());
	////////////////////////////////////////////////////////////
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	root.Accept(writer);
	bodyPosJson = buffer.GetString();

}

bool preProcess(cv::Mat mat, string& str2dPose, string& str3dPose, string& pose_type, string& body_pos, string& errorType)
{
	LOG(INFO) << "preProcess  begin ";
	bool ret = false;

	do{
		if(mat.data == nullptr){
			LOG(ERROR) << "preProcess failed, mat is null";
			break;
		}
		if(!connected_){
			if(!connectPrehandle()){
				closeThrift();
				errorType = "thrift_error";
				break;
			}
		}

		Image  img; 
		img.width = mat.cols;
		img.height = mat.rows;
		img.channel = mat.channels();
		std::string buf(mat.datastart, mat.dataend);
		img.imgBin = buf;
	
		GuessInfo gsInfo;
		vector<HumanPose3DInfo>  tdInfo;
		try
		{
			ImageInitData checkRet;
			ppClient_->CheckIsValid(checkRet, img);
			if(!checkRet.isValid){
				LOG(INFO) << "CheckIsValid  failed ";
				errorType = "0-";
				break;
			}
			pose_type = checkRet.poseJsonStr;
			genBodyPosJson(checkRet.imgAABB, body_pos);
			LOG(INFO) << "pose_type : " <<pose_type;
			LOG(INFO) << "body_pos : "<<body_pos;
		
			ppClient_->DoInitGuess(gsInfo, img);
			if(!gsInfo.isSuccess){
				errorType = "1-";
				LOG(INFO) << "DoInitGuess  failed ";
				break;
			}

			ppClient_->AutoCalPose3D(tdInfo, img, gsInfo);
			if(tdInfo.size() != 2){
				errorType = "2-";
				LOG(INFO) << "AutoCalPose3D  failed ";
				break;
			}
			
		}
		catch (TException& tx)
		{
			LOG(ERROR) << "  preProcess catch exception,  msg : " << tx.what();
			closeThrift();
			connected_ = false;
			errorType = "thrift_error";
			break;
		}

		gen2dPoseJson(gsInfo, str2dPose);
		gen3dPoseJson(tdInfo, str3dPose);

		ret = true;

	}while(0);
	LOG(INFO) << "str2dPose  length : " <<  str2dPose.length();
	LOG(INFO) << "str3dPose  length : " <<  str3dPose.length();
	LOG(INFO) << "preProcess end, ret "<< ret;

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
	string suanfa_ip_{config::CConfigManager::instance()->suanfa_ip_};
	int   suanfa_port_{config::CConfigManager::instance()->suanfa_port_};
	bool  connected_{false}; 

};

shared_ptr<CPreProcessor>              gPtrPreprocessor;

class DataServiceHandler : virtual public DataServiceIf {
 public:
  DataServiceHandler() {
    // Your initialization goes here
  }

  void UploadRequest(ReturnVals& _return, const std::string& md5) {
    // Your implementation goes here
	
	_return.code = gPtrDbpool->GetDbOper()->QueryPicByMd5(md5);
	                          
	LOG(INFO) << "UploadRequest end, md5: " << md5 << " , ret code : " << _return.code;
   
  }

  void PicUpload(ReturnVals& _return, const PicInfo& pic, double percent, int sumTotal) {
	_return.code = -1;
	do{
		LOG(INFO) << "PicUpload BEGIN";
		if( pic.oriBin.length() == 0){
			LOG(INFO) << "ori pic is empty ";
			break;
		}
		string path = config::CConfigManager::instance()->ori_path_;
		string ppPath = config::CConfigManager::instance()->success_path_;
		string ppBakPath = config::CConfigManager::instance()->bak_path_;
		string errorPath = config::CConfigManager::instance()->error_path_;

		int pos;
		pos = pic.filename.find_last_of('.');
		string tail = pic.filename.substr(pos);
		string oriUrl = path + pic.md5 + tail;
		string ppUrl  = ppPath + pic.md5 + tail;     
		string ppBakUrl = ppBakPath + pic.md5 + tail;
		std::ofstream oriPic(oriUrl.c_str() , std::ofstream::binary);
		oriPic.write(pic.oriBin.c_str(), pic.oriBin.length());      
		oriPic.close();
		if(percent == 0){
			gTotalPic = 0;
			gDbPic    = 0;
		}else{
			gTotalPic = sumTotal;
		}    

		string inflateUrl = path + pic.md5 +  "600*800" + tail;
		cv::Mat src = cv::imread(oriUrl); 
    	cv::Mat dst = ResizeImg(src, 600, 800);
		cv::imwrite(inflateUrl, dst); 
		string str2dPose;
		string str3dPose;
		string pose_type;
		string body_pos;
		string errorType;
		if(!gPtrPreprocessor->preProcess(dst, str2dPose, str3dPose, pose_type, body_pos, errorType))
		{
			string errorUrl = errorPath + errorType + pic.md5 + tail;
			if(errorType != "thrift_error"){   
				MoveFile(oriUrl, errorUrl);
			}else{
				sleep(5);                 //连接不上预处理算法
			}
			_return.code = 1;
			break;
		}
		MoveFile(oriUrl, ppUrl);
		CopyFile(ppUrl,  ppBakUrl);
		if(-1 == gPtrDbpool->GetDbOper()->AddPicture(pic, ppUrl, str2dPose, str3dPose, pose_type, body_pos))
		{
			break;
		}
		
		gDbPic++;
		_return.code = 0;                                      
	}while(0);

	LOG(INFO) << "PicUpload end , ret code : " << _return.code;
  }

   void UploadCrawlingResult(ReturnVals& _return, const CrawlingResult& cs) {
	LOG(INFO) << "UploadCrawlingResult  begin";
	_return.code = -1;
	do{
		if(!gPtrDbpool->GetDbOper()->AddCrawlingResult(cs,  gDbPic)){
			break;
		}

		_return.code = 0;   
		gDbPic = 0;
		gTotalPic = 0;

	}while(0);                     
	
	LOG(INFO) << "UploadCrawlingResult end , ret code : " << _return.code;
    return;
  }

};

class CrawlerClientServiceHandler : virtual public CrawlerClientServiceIf {
 public:
  CrawlerClientServiceHandler() {
  }
  void UploadRequest(ReturnVals& _return, const std::string& md5) {
	_return.code = gPtrDbpool->GetDbOper()->QueryPicByMd5(md5);                     
	LOG(INFO) << "UploadRequest end, md5: " << md5 << " , ret code : " << _return.code;
  }

  void PicUpload(ReturnVals& _return, const PicInfo& pic) { 
	_return.code = -1;
	do{
		LOG(INFO) << "PicUpload BEGIN";
		if( pic.oriBin.length() == 0){
			LOG(INFO) << "ori pic is empty ";  
			break;   
		}
		string path = config::CConfigManager::instance()->ori_path_;
		string ppPath = config::CConfigManager::instance()->success_path_;
		string ppBakPath = config::CConfigManager::instance()->bak_path_;
		string errorPath = config::CConfigManager::instance()->error_path_;
		/////////////////////////////////
		int pos;
		pos = pic.filename.find_last_of('.');
		string tail = pic.filename.substr(pos );
		string oriUrl = path + pic.md5 + tail;
		string ppUrl = ppPath + pic.md5 + tail;
		string ppBakUrl = ppBakPath + pic.md5 + tail;
		std::ofstream oriPic(oriUrl.c_str() , std::ofstream::binary);
		oriPic.write(pic.oriBin.c_str(), pic.oriBin.length());
		oriPic.close();
		/////////////////////////////////
		string inflateUrl = path + pic.md5 +  "600*800" + tail;
		cv::Mat src = cv::imread(oriUrl); 
    	cv::Mat dst = ResizeImg(src, 600, 800);
		cv::imwrite(inflateUrl, dst); 
		/////////////////////////////////
		string str2dPose;
		string str3dPose;
		string pose_type;
		string body_pos;
		string errorType;
		if(!gPtrPreprocessor->preProcess(dst, str2dPose, str3dPose, pose_type, body_pos, errorType))
		{
			string errorUrl = errorPath + errorType + pic.md5 + tail;
			if(errorType != "thrift_error"){   
				MoveFile(oriUrl, errorUrl);
			}else{
				sleep(5);                 //连接不上预处理算法
			}
			_return.code = 1;
			break;
		}
		MoveFile(oriUrl, ppUrl);
		CopyFile(ppUrl,  ppBakUrl);
		if(-1 == gPtrDbpool->GetDbOper()->AddPicture(pic, ppUrl, str2dPose, str3dPose, pose_type, body_pos))
		{
			break;
		}
		_return.code = 0;

	}while(0);
	LOG(INFO) << "PicUpload end , ret code : " << _return.code;
  
  }

  void PacketPicVec(std::vector<QueryedPicInfo>&  retVec, SqlMapVector&  sqlVec ){
		 for(auto& pic : sqlVec){
			int pos;
			pos = pic["pic_url"].find_last_of('.');
			string tail = pic["pic_url"].substr(pos);
			string screenshotUrl =  pic["pic_url"].substr(0, pos)  + "screenshot"  + tail;
			cv::Mat src = cv::imread(pic["pic_url"]); 
			if(src.data == nullptr){
				LOG(INFO) << "ori pic " << pic["pic_url"] << " not exist";
				continue;
			}
    		cv::Mat dst = ResizeImg(src, 69, 92);
			cv::imwrite(screenshotUrl, dst); 
			
			std::ifstream fin;         
			std::stringstream ss;
			fin.open(screenshotUrl, std::ifstream::binary);
			if (!fin.is_open()){
				continue;
			}
			ss << fin.rdbuf();
			string screenshot_bin = ss.str();
		
			fin.close();
			remove(screenshotUrl.c_str());
			int id;
			ss.clear();
			ss.str("");
			ss << pic["id"];
			ss >> id;
		
			QueryedPicInfo picInfo;
			picInfo.pic_id = id;
			picInfo.pic_url = pic["pic_url"];
			picInfo.tag = pic["tag_word"];
			picInfo.pose_type = pic["pose_type"];
			picInfo.create_time = pic["create_time"];
			picInfo.website = pic["website"];
			picInfo.key = pic["key_word"];
			int count;
			ss.clear();
			ss.str("");
			ss << pic["label_count"];
			ss >> count;
			picInfo.labeledCount = 5 - count;
			picInfo.lastLabeledUser = pic["update_user"];
			picInfo.screenshot_bin = screenshot_bin;
			retVec.push_back(picInfo);
		 }

	}

  void QueryCrawlingHistory(CrawlerHistoryRet& _return , const HistoryQueryCondition&  qc ) {
      _return.code = -1;   
	  do{
		 LOG(INFO) << "QueryCrawlingHistory begin ";
		 SqlMapVector dbVec;
	
		 if(!gPtrDbpool->GetDbOper()->QueryCrawlingHistory(dbVec,  qc)){
			 _return.msg = "db query failed !"; 
			 LOG(ERROR) << " db  QueryCrawlingHistory failed ";
			 break;    
		 }
		 if(dbVec.size() == 0){
			 _return.code = 1;
			 _return.msg = "no labeled pic !"; 
			 LOG(ERROR) <<" has no crawling istory";
			 break;
		 }
		LOG(INFO) << " picVec.size()  "<<dbVec.size();
		for(auto& his : dbVec){
			CrawlerHistory  cHis;
			cHis.tag = his["tag"];
			cHis.key = his["key"];
			cHis.time_create = his["create_time"];
			stringstream ss;
			ss << his["total_sum"];
			ss >> cHis.total_sum;
			ss.clear();
			ss.str("");
			ss << his["db_sum"];
			ss >> cHis.db_sum;

			_return.hisVec.push_back(cHis);

		}
		 _return.code = 0;

	}while(0);

	LOG(INFO) << "QueryCrawlingHistory end , ret code : " << _return.code;  
  }
  void QueryCrawleredPic(CrawleredPicRet&  _return , const QueryConditions&  qc , const int32_t  index ) {
	_return.code = -1;   
	  do{
		 LOG(INFO) << "QueryCrawleredPic begin ";
		 SqlMapVector picVec;
	
		 if(!gPtrDbpool->GetDbOper()->QueryCrawleredPic(picVec, qc, index, _return.pageNum)){
			 _return.msg = "db query failed !"; 
			 LOG(ERROR) << " db  QueryCrawleredPic failed ";
			 break;    
		 }
		 if(picVec.size() == 0){
			 _return.code = 1;
			 _return.msg = "no crawlered pic !"; 
			 LOG(ERROR) <<" has no crawled pic";     
			 break;
		 }
		LOG(INFO) << " picVec.size()  "<<picVec.size();

		PacketPicVec(_return.picVec, picVec);
		_return.code = 0;  

	}while(0);

	LOG(INFO) << "QueryCrawleredPic end , ret code : " << _return.code; 
    return;
  }

   void UpdatePicInfo(ReturnVals&  _return , const int32_t  pic_id , const std::string&  tag , const std::string&  pose_type ) {
	   LOG(INFO) << "UpdatePicInfo begin "; 
	   _return.code = -1;
	   do{
		
		 if(!gPtrDbpool->GetDbOper()->UpdatePicInfoById(pic_id, tag, pose_type)){
			 break;    
		 }
		 
		_return.code = 0;  

	}while(0);

	LOG(INFO) << "UpdatePicInfo end , ret code : " << _return.code; 

    return;
  }

  void start(ReturnVals& _return, const std::string& keyword, const std::string& website, const std::string& tag){
	  LOG(INFO) << "start begin....";
	  LOG(INFO) <<"key : " <<keyword;
	  LOG(INFO) <<"website : " <<website;
	  LOG(INFO) <<"tag : " <<tag;
	  do{
			if(!connected_)
			{
				if(!connectCrawler()){
					closeThrift();
					_return.msg = "connect crawler failed";
					_return.code = -1;
					break;
				}
			}
			try
			{
				csClient_->start(_return, keyword, website, tag);
			}
			catch (TException& tx)
			{
				LOG(ERROR) << "csClient_->start throw exception , msg : " << tx.what();
				connected_ = false;
				_return.code = -1;
				break;
			}
			gDbPic = 0;  
			gTotalPic = 0;

	  }while(0);

	  LOG(INFO) << "start END : " << "return code : " <<_return.code;

  }
  void InquireCrawlerStatus(CrawlerStatusRet& _return){
		_return.crawlerStatus.totalPicSum = gTotalPic;
		_return.crawlerStatus.dbPicSum = gDbPic;
  }

   void stop(ReturnVals& _return){
	  do{
		   LOG(INFO) << "stop crawler begin...";
			if(!connected_)
			{
				if(!connectCrawler()){
					closeThrift();
					_return.msg = "connect crawler failed";
					_return.code = -1;
					break;
				}
			}
			try
			{
				csClient_->stop(_return);
			}
			catch (TException& tx)
			{
				LOG(ERROR) << "csClient_->stop() throw exception , msg : " << tx.what();
				connected_ = false;
				_return.code = -1;
				break;
			}

	  }while(0);
	
	  LOG(INFO) << "stop crawler END, ret.code : " << _return.code;

  }
  bool  connectCrawler()
	{
		LOG(INFO) << "connect to Crawler : " << crawler_ip_ << " port "<< crawler_port_;
		socket_.reset(new TSocket(crawler_ip_, crawler_port_));
		transport_.reset(new TFramedTransport(socket_));
		protocol_.reset(new TBinaryProtocol(transport_));
		try
		{
			transport_->open();
		}
		catch (TException& tx)
		{
			LOG(ERROR) << "connect crawler failed, msg : " << tx.what();
			return false;
		}
		csClient_.reset(new CrawlerServiceClient(protocol_));

		connected_ = true;
		LOG(INFO) << "connect  Crawler  success";
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

private:
	boost::shared_ptr<TSocket>    socket_;
    boost::shared_ptr<TTransport> transport_; 
    boost::shared_ptr<TProtocol>  protocol_;
	boost::shared_ptr<CrawlerServiceClient>  csClient_;
	string crawler_ip_{config::CConfigManager::instance()->crawler_ip_};
	int    crawler_port_{config::CConfigManager::instance()->crawler_port_};
	bool   connected_{false}; 
  
};

class LabelServiceHandler : virtual public LabelServiceIf {
 public:
  LabelServiceHandler() {  
    // Your initialization goes here
  }
	void PacketPicVec(std::vector<QueryedPicInfo>&  retVec, SqlMapVector&  sqlVec ){
		 for(auto& pic : sqlVec){
			int pos;
			pos = pic["pic_url"].find_last_of('.');
			string tail = pic["pic_url"].substr(pos);
			string screenshotUrl =  pic["pic_url"].substr(0, pos)  + "screenshot"  + tail;
			cv::Mat src = cv::imread(pic["pic_url"]); 
			if(src.data == nullptr){
				LOG(INFO) << "ori pic " << pic["pic_url"] << " not exist";
				continue;
			}
    		cv::Mat dst = ResizeImg(src, 60, 80);
			cv::imwrite(screenshotUrl, dst); 
			
			std::ifstream fin;         
			std::stringstream ss;
			fin.open(screenshotUrl, std::ifstream::binary);
			if (!fin.is_open()){
				continue;
			}
			ss << fin.rdbuf();
			string screenshot_bin = ss.str();
		
			fin.close();
			remove(screenshotUrl.c_str());
			int id;
			ss.clear();
			ss.str("");
			ss << pic["id"];
			ss >> id;
		
			QueryedPicInfo picInfo;
			picInfo.pic_id = id;
			picInfo.pic_url = pic["pic_url"];
			picInfo.tag = pic["tag_word"];
			picInfo.pose_type = pic["pose_type"];
			picInfo.create_time = pic["create_time"];
			int count;
			ss.clear();
			ss.str("");
			ss << pic["label_count"];
			ss >> count;
			picInfo.labeledCount = 5 - count;
			picInfo.lastLabeledUser = pic["update_user"];
			picInfo.screenshot_bin = screenshot_bin;
			retVec.push_back(picInfo);
		 }

	}
  

  void QueryUnlabeledPic(QueryUnlabeledRet& _return, const std::string& user, const int32_t index, const QueryCondition& qc) {
	do{
		 LOG(INFO) << "QueryUnlabeledPic begin ";
		 SqlMapVector picVec;
	
		 if(!gPtrDbpool->GetDbOper()->QueryUnlabeledPicture(picVec, user, index, _return.pageNum, qc)){
			 _return.code = -1;
			 _return.msg = "db query failed !"; 
			 break;   
		 }
		 if(picVec.size() == 0){
			 _return.code = 1;
			 _return.msg = "no unlabeled picture !"; 
			 break;
		 }

		 PacketPicVec(_return.picVec, picVec);
		_return.code = 0;

	}while(0);         

	LOG(INFO) << "QueryUnlabeledPic end , ret code : " << _return.code;

  }

  void QueryLabeledPic(QueryLabeledRet& _return, const std::string& user, const int32_t index, const QueryCondition& qc) {
	  _return.code = -1;   
	  do{
		 LOG(INFO) << "QueryLabeledPic begin ";
		 SqlMapVector picVec;
	
		 if(!gPtrDbpool->GetDbOper()->QueryLabeledPicture(picVec, user, index,  _return.pageNum, qc)){
			 _return.msg = "db query failed !"; 
			 LOG(ERROR) << " db  QueryLabeledPicture failed ";
			 break;    
		 }
		 if(picVec.size() == 0){
			 _return.code = 1;
			 _return.msg = "no labeled pic !"; 
			 LOG(ERROR) << user <<" has no labeled pic";
			 break;
		 }
		LOG(INFO) << " picVec.size()  "<<picVec.size();
		PacketPicVec(_return.picVec, picVec);
		 _return.code = 0;

	}while(0);

	LOG(INFO) << "QueryUnlabeledPic end , ret code : " << _return.code;    

  }

  void QueryPicLabeledByOthers(QueryLabeledRet& _return, const std::string& user, const int32_t index, const QueryCondition& qc) {
	  _return.code = -1;
	  do{
		 LOG(INFO) << "QueryPicLabeledByOthers begin ";
		 SqlMapVector picVec;
		 if(!gPtrDbpool->GetDbOper()->QueryLabeledPicByOthers(picVec, user, index, _return.pageNum, qc)){
			 _return.msg = "db query failed !"; 
			LOG(ERROR) << " db  query failed ";
			 break;   
		 }
		 if(picVec.size() == 0){
			 _return.code = 1;
			 _return.msg = "no labeled pic !"; 
			 LOG(ERROR) << " has no labeled pic by others";
			 break;
		 }

		 LOG(INFO) << " picVec.size()  "<< picVec.size();
		 PacketPicVec(_return.picVec, picVec);
		 _return.code = 0;

	}while(0);

	LOG(INFO) << "QueryPicLabeledByOthers end , ret code : " << _return.code;

  }

  void QueryPicByUserName(QueryLabeledRet& _return, const std::string& token, const std::string& user, const int32_t index, const QueryCondition& qc){
	   _return.code = -1;
	  do{
		 LOG(INFO) << "QueryPicByUserName begin ";
		 SqlMapVector picVec;
		 int ret = gPtrDbpool->GetDbOper()->QueryPicByUser(picVec, token, user, index, _return.pageNum, qc);
		 if(-1 == ret){
			 _return.msg = "db query failed !"; 
			 LOG(ERROR) << " db  query failed ";
			 break;   
		 }else if(2 == ret){
			  _return.msg = "user is not admin!"; 
			  _return.code = 2;
			 break;  
		 }
		 if(picVec.size() == 0){
			 _return.code = 1;
			 _return.msg = "no labeled pic !"; 
			 LOG(ERROR) << " no labeled pic";
			 break;
		 }

		 LOG(INFO) << " picVec.size()  "<<picVec.size();
		 PacketPicVec(_return.picVec, picVec);
		 _return.code = 0;

	}while(0);

	LOG(INFO) << "QueryPicByUserName end , ret code : " << _return.code;
  }

  void QueryPicById(QueryByIdRet& _return, const std::string& user, const int32_t pic_id) {
	  _return.code = -1;
	  do{
		 LOG(INFO) << "QueryPicById begin ";
		 SqlMapVector sqlVec;
		 if(!gPtrDbpool->GetDbOper()->QueryPicById(sqlVec, pic_id, user)){
			_return.msg = "db query failed !"; 
			LOG(ERROR) << " db  query failed ";
			break;   
		 }
		 if(sqlVec.size() == 0){
			 _return.code = 1;
			 _return.msg = "no such pic !"; 
			 _return.pageNum = 0;
			 LOG(ERROR) << " no such pic ： "<< pic_id;  
			 break;
		 }
		 std::vector<QueryedPicInfo>  picVec;
		 PacketPicVec(picVec, sqlVec);

		 _return.pic = picVec[0];
		 _return.pageNum = 1;
		 _return.code = 0;

	}while(0);

	LOG(INFO) << "QueryPicById end , ret code : " << _return.code;

  }

  void DownloadPic(DownloadRet& _return, const std::string& pic_url) {
	LOG(INFO) << "DownloadPic begin ";
	_return.code = -1;
	do{
		 SqlResultSet  sqlMap;
		 if(gPtrDbpool->GetDbOper()->QueryPicByURL(pic_url, sqlMap) != 0){
			 _return.msg = "db query failed !";     
		 }
		 _return.HumanPose2DInfo = sqlMap["Fpre2DPoseInfo"];
		 _return.HumanPose3DInfo = sqlMap["Fpre3DPoseInfo"];
		 stringstream ss;
		 ss << sqlMap["Fid"];
		 ss >>_return.pic_id;            
		
		 if(!GetInflatedPicBin(pic_url, 600, 800, sqlMap["Fbody_pos"], _return.bin)){
			   LOG(ERROR) << " get pic 600*800 bin  failed  ";
			   break;
		  }

		_return.code = 0;

	}while(0);

	LOG(INFO) << "DownloadPic end , ret code " << _return.code;
  }


   void QueryLabeledPoseData(LabeledPoseDataRet& _return, const int32_t pic_id, const std::string& pic_url, const std::string& token) {
	LOG(INFO) << "QueryLabeledPoseData begin ";
	_return.code = -1;
	do{
		 SqlResultSet  sqlMap;
		 if(gPtrDbpool->GetDbOper()->QueryLabeledPoseData(sqlMap, pic_id, token) != 0){
			 _return.msg = "db query failed !";     
		 }
		 _return.poseData = sqlMap["Fpose_data"] ;

		 SqlResultSet  picInfoMap;
		 if(gPtrDbpool->GetDbOper()->QueryPicByURL(pic_url, picInfoMap) != 0){
			 _return.msg = "QueryPicByURL failed !";     
		 }
		 if(!GetInflatedPicBin(pic_url, 600, 800, picInfoMap["Fbody_pos"], _return.bin)){
			   LOG(ERROR) << " get pic 600*800 bin  failed  ";
			   break;
		  }

		_return.code = 0;
	}while(0);

	LOG(INFO) << "QueryLabeledPoseData end , ret code " << _return.code;
  }
  struct BODY_POS{
	  int top_x_{0};
	  int top_y_{0};
	  int bottom_x_{0};
	  int bottom_y_{0};
  };
  bool ParseBodyPosition(string strBodyPos, BODY_POS bodyPos){
		rapidjson::Document doc;
		doc.Parse<0>(strBodyPos.c_str());
		if (doc.HasParseError())
		{
			rapidjson::ParseErrorCode code = doc.GetParseError();
			LOG(ERROR) << "parse strBodyPos failed "; 
			return  false;
		}

		Value &top_x = doc["top_x"];
		if (top_x.IsNull() || !top_x.IsInt())
		{
			LOG(ERROR) << "top_x is null";  
			return false;
		}
		bodyPos.top_x_ = top_x.GetInt();
		LOG(INFO) << "bodyPos.top_x_ : " << bodyPos.top_x_;

		Value &top_y = doc["top_y"];
		if (top_y.IsNull() || !top_y.IsInt())
		{
			LOG(ERROR) << "top_x is null";  
			return false;
		}
		bodyPos.top_y_ = top_y.GetInt();
		LOG(INFO) << "bodyPos.top_y_ : " << bodyPos.top_y_;

		Value &bottom_x = doc["bottom_x"];
		if (bottom_x.IsNull() || !bottom_x.IsInt())
		{
			LOG(ERROR) << "bottom_x is null";  
			return false;
		}
		bodyPos.bottom_x_ = bottom_x.GetInt();
		LOG(INFO) << "bodyPos.bottom_x_ : " << bodyPos.bottom_x_;

		Value &bottom_y = doc["bottom_y"];
		if (bottom_y.IsNull() || !bottom_y.IsInt())
		{
			LOG(ERROR) << "bottom_y is null";  
			return false;
		}
		bodyPos.bottom_y_ = bottom_y.GetInt();
		LOG(INFO) << "bodyPos.bottom_y_ : " << bodyPos.bottom_y_;

		return true;
  }

  bool GetInflatedPicBin(const string url, float width, float height , string body_pos, string& bin){
	bool ret = false;
	do{
		int pos;
		pos = url.find_last_of('.');
		string tail = url.substr(pos);
		stringstream ss;
		string tag;
		ss << width << "*" << height;
		ss >> tag;
		string inflateUrl =  url.substr(0, pos)  + tag  + tail;
		cv::Mat src = cv::imread(url);
		if(src.data == nullptr){
				LOG(ERROR) << " pic  "<< url << " not exist";
				break;
		} 
		/////////////////////////////////////////////
		BODY_POS bodyPos;
		cv::Mat  roi_img;
		if(!ParseBodyPosition(body_pos, bodyPos)){
			roi_img = src;
		}else{
			roi_img = src(cv::Range(bodyPos.bottom_y_ , bodyPos.top_y_), 
			cv::Range(bodyPos.top_x_,bodyPos.bottom_x_));
		}
		/////////////////////////////////////////////
		cv::Mat dst = ResizeImg(roi_img, width, height);
		cv::imwrite(inflateUrl, dst); 
		std::ifstream fin;
		fin.open(inflateUrl, ifstream::binary);
		if (!fin.is_open()){        
			break;
		}
		ss.clear();
		ss.str("");
		ss << fin.rdbuf();
		bin = ss.str();
		fin.close();
		remove(inflateUrl.c_str());
		ret = true;

	} while(0);

	return ret;

  }

  void QueryLastLabeledPoseData(LabeledPoseDataRet& _return, const int32_t pic_id, const std::string& pic_url, const std::string& user) {
	LOG(INFO) << "QueryLastLabeledPoseData begin ";
	_return.code = -1;
	do{
		 SqlResultSet  sqlMap;
		 if(gPtrDbpool->GetDbOper()->QueryLastLabeledPoseData(sqlMap, pic_id,  user) != 0){
			 _return.msg = "db query failed !";     
		 }
		 _return.poseData = sqlMap["Fpose_data"] ;
		//////////////////////
		int pos;
		pos = pic_url.find_last_of('.');
		string tail = pic_url.substr(pos);
		string inflateUrl =  pic_url.substr(0, pos)  + "600*800"  + tail;
		cv::Mat src = cv::imread(pic_url);
		if(src.data == nullptr){
			 LOG(ERROR) << " pic  "<<pic_url << " not exist";
			 break;
		} 
    	cv::Mat dst = ResizeImg(src, 600, 800);
		cv::imwrite(inflateUrl, dst); 
		std::ifstream fin;
		fin.open(inflateUrl, ifstream::binary);
		if (!fin.is_open()){        
			break;
		}
		stringstream ss;
		ss << fin.rdbuf();
		_return.bin = ss.str();
		fin.close();
		remove(inflateUrl.c_str());

		_return.code = 0;

	}while(0);

	LOG(INFO) << "QueryLastLabeledPoseData end , ret code " << _return.code;
  }

  void InvalidatePicture(ReturnVals& _return, const int32_t pic_id, const std::string& userName, const bool  type) {
	LOG(INFO) << "InvalidatePicture begin ";
	_return.code = -1;
	do{
		 if(_return.code = gPtrDbpool->GetDbOper()->InvalidatePicture(pic_id, userName, type) != 0){
			      LOG(ERROR) << userName <<  " InvalidatePicture  "<< pic_id << " failed";
		 }
		 if(_return.code == 1){
			 _return.msg = "such user is not admin user";
		 }
		
	}while(0);

	LOG(INFO) << "InvalidatePicture end , ret code " << _return.code;

  }

  void InsertToDb(ReturnVals& _return, const std::string& poseInfo, const int32_t pic_id, const std::string& userName) {
	if(!gPtrDbpool->GetDbOper()->AddLabeledDate(pic_id, userName, poseInfo )){
		_return.code = -1;
		_return.msg = "insert into db failed";
	}
  }

void Register(ReturnVals&  _return , const std::string&  user , const std::string&  passwd ) {
	_return.code = gPtrDbpool->GetDbOper()->Register(user, passwd);
	
	
    return;
  }
  void Login(LoginRet&  _return , const std::string&  user , const std::string&  passwd ) {
	  LOG(INFO) << "Login begin";

	_return.code = gPtrDbpool->GetDbOper()->Login(_return.token, _return.role_id, user, passwd);
	
	LOG(INFO) << "login end , ret code " << _return.code;
	
    return;
  }
  void QueryPicPoseData(PoseDatasRet&  _return , const int32_t  pic_id , const std::string& pic_url, const std::string&  token ) {
	LOG(INFO) << " QueryPicPoseData begin ";
	_return.code = -1;
	do{
		 SqlMapVector sqlVec;
		 if(!gPtrDbpool->GetDbOper()->QueryPicPoseData(sqlVec, pic_id, token)){
			      LOG(ERROR) << " QueryPicPoseData  failed  ";
				  break;
		 }

		  for(auto& row :sqlVec ){
			    PoseData pd;
				pd.pose_data  =  row["pose_data"];
				pd.user_name  =  row["user_name"];
				pd.label_time =  row["update_time"];
				_return.vecPoseData.push_back(pd);
		  }
		 SqlResultSet  picInfoMap;
		 if(gPtrDbpool->GetDbOper()->QueryPicByURL(pic_url, picInfoMap) != 0){
			 _return.msg = "QueryPicByURL failed !";     
		 }
		 if(!GetInflatedPicBin(pic_url, 600, 800, picInfoMap["Fbody_pos"], _return.pic_bin)){
			   LOG(ERROR) << " get pic 600*800 bin  failed  ";
			   break;
		}
		 
		_return.code = 0;
	}while(0);

	LOG(INFO) << " QueryPicPoseData end , ret code " << _return.code;
    return;
  }
  void QueryPoseDataScore(ScoreQueryRet&  _return , const int32_t  pic_id , const std::string&  label_user , const std::string&  token ) {
	LOG(INFO) << "QueryPoseDataScore begin ";   
	_return.code = -1;
	do{
		 if(!gPtrDbpool->GetDbOper()->QueryPoseDataScore(_return, pic_id, label_user, token)){
			      LOG(ERROR) << " QueryPoseDataScore  failed  ";
				  break;           
		 }
		
		_return.code = 0;
	}while(0);

	LOG(INFO) << "QueryPoseDataScore end , ret code " << _return.code;
    return;
  }
  void ScorePoseData(ReturnVals&  _return , const int32_t  pic_id , const std::string&  label_user , const std::string&  token , const double  score ) {
	LOG(INFO) << "ScorePoseData begin ";   
	_return.code = -1;
	do{
		 if(!gPtrDbpool->GetDbOper()->ScorePoseData(pic_id, label_user, token, score)){
			      LOG(ERROR) << " ScorePoseData  failed  ";
				  break;                            
		 }
		
		_return.code = 0;
	}while(0);

	LOG(INFO) << "ScorePoseData end , ret code " << _return.code;
	
    return;
  }
  void RepreProcessPic(ReturnVals&  _return , const std::string&  pic_url , const std::string&  token ) {
    LOG(INFO) << "RepreProcessPic begin ";   
	_return.code = -1;
	do{
		cv::Mat src = cv::imread(pic_url); 
    	cv::Mat dst = ResizeImg(src, 600, 800);
		string str2dPose;
		string str3dPose; 
		string pose_type;
		string body_pos;  
		string errorType;
		if(!gPtrPreprocessor->preProcess(dst, str2dPose, str3dPose, pose_type, body_pos, errorType))
		{
			break;
		}
		
		if(!gPtrDbpool->GetDbOper()->UpdatePicPoseDate(pic_url, str2dPose, str3dPose, pose_type, body_pos)){
			break;
		}
		 
		_return.code = 0;
	}while(0);

	LOG(INFO) << "RepreProcessPic end , ret code " << _return.code;
    return;
  }

};

class CPoseLabelServer::Imp
{
public:
	HttpSRV * httpPtr_{NULL};
	shared_ptr<thread>  crawlerServerThread_;
	shared_ptr<thread>  crawlerClientServerThread_;
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

		return true;
	}

	bool initDB()
	{
		gPtrDbpool.reset(new CDbPool<CDBOperator>);
		gPtrDbpool->SetPoolSize(1);
		gPtrDbpool->InitPool();
		
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
		config::init_glog("label_server");
		LOG(INFO) << "GLOG BEGIN TO WORK"; 

		if (! config::CConfigManager::instance()->get_db_paramer(CONF_PATH))
		{
			return false;
		}

		if (! config::CConfigManager::instance()->get_main_param(CONF_PATH))  
		{
			return false;
		}

		return true;

	}
};

CPoseLabelServer::CPoseLabelServer()
{
	impPtr_.reset(new Imp);
}

CPoseLabelServer::~CPoseLabelServer()
{

}

bool  CPoseLabelServer::Start()
{
	if (!impPtr_->init())
	{
		return  false;
	}
	gPtrPreprocessor.reset(new CPreProcessor);

	impPtr_->crawlerServerThread_.reset(new thread([](){
	int port = config::CConfigManager::instance()->crawler_server_listen_port_;
	boost::shared_ptr<DataServiceHandler> handler(new DataServiceHandler());
	boost::shared_ptr<TProcessor> processor(new DataServiceProcessor(handler));
	boost::shared_ptr<TServerTransport>  serverTransport(new TServerSocket(port));
	boost::shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
	boost::shared_ptr<TProtocolFactory>  protocolFactory(new TBinaryProtocolFactory());

	TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);

	server.serve();

	}));

	impPtr_->labelServerThread_.reset(new thread([](){
		int port = config::CConfigManager::instance()->label_server_port_;
  		boost::shared_ptr<LabelServiceHandler> handler(new LabelServiceHandler());
  		boost::shared_ptr<TProcessor> processor(new LabelServiceProcessor(handler));
 		boost::shared_ptr<TServerTransport>  serverTransport(new TServerSocket(port));
  		boost::shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
  		boost::shared_ptr<TProtocolFactory>  protocolFactory(new TBinaryProtocolFactory());

  		TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
 		server.serve();

	}));

	impPtr_->crawlerClientServerThread_.reset(new thread([](){
		
		int port = config::CConfigManager::instance()->crawler_client_server_listen_port_;
  		boost::shared_ptr<CrawlerClientServiceHandler> handler(new CrawlerClientServiceHandler());
  		boost::shared_ptr<TProcessor> processor(new CrawlerClientServiceProcessor(handler));
 		boost::shared_ptr<TServerTransport>  serverTransport(new TServerSocket(port));
  		boost::shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
  		boost::shared_ptr<TProtocolFactory>  protocolFactory(new TBinaryProtocolFactory());

  		TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
 		server.serve();      
		 
	}));

	return  true;
}

void  CPoseLabelServer::Stop()
{
	LOG(INFO) << "Server stop";
}
	 
int CPoseLabelServer::httpCallBack(const RecvInfo& pRecvInfo, string&  response)
{
	return 0;  
}

