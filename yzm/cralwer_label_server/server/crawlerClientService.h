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

#include "DataService.h"
#include "CrawlerClientService.h"
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

shared_ptr<CDbPool<CDBOperator> >      gPtrDbpool;

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
    // Your implementation goes here 
	do{
		if( pic.oriBin.length() == 0){
			_return.code = -1;
			LOG(INFO) << "ori pic is empty ";
			break;
		}
		string path = "../pic/";
		int pos;
		pos = pic.filename.find_last_of('.');
		string tail = pic.filename.substr(pos );
		string oriUrl = path + pic.md5 + tail;
		std::ofstream oriPic(oriUrl.c_str() , std::ofstream::binary);
		oriPic.write(pic.oriBin.c_str(), pic.oriBin.length());
		oriPic.close();

		string inflateUrl = path + pic.md5 +  "600*800" + tail;
		cv::Mat src = cv::imread(oriUrl); 
    	cv::Mat dst = ResizeImg(src, 600, 800);
		cv::imwrite(inflateUrl, dst); 
	    LOG(INFO) << "ori pic url : " << oriUrl;
		string str2dPose;
		string str3dPose;
		if(!preProcess(inflateUrl, str2dPose, str3dPose))
		{
			string errorPath = "../pic_error/";
			string errorUrl = errorPath + pic.md5 + tail;
			std::ofstream errorPic(errorUrl.c_str() , std::ofstream::binary);
			errorPic.write(pic.oriBin.c_str(), pic.oriBin.length());
			errorPic.close();

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

		Image  img;
		cv::Mat mat = cv::imread(url); 
		img.width = mat.cols;
		img.height = mat.rows;
		img.channel = mat.channels();
		std::string buf(mat.datastart, mat.dataend);
		img.imgBin = buf;
		remove(url.c_str());

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
	string suanfa_ip_{config::CConfigManager::instance()->suanfa_ip_};
	int suanfa_port_{config::CConfigManager::instance()->suanfa_port_};
	bool connected_{false}; 

};