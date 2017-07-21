#include "DataService.h"  // 替换成你的.h  
#include "LabelService.h"  // 替换成你的.h 
#include <thrift/transport/TSocket.h>  
#include <thrift/transport/TBufferTransports.h>  
#include <thrift/protocol/TBinaryProtocol.h>  
#include<fstream>
#include<sstream>
#include<glog/logging.h> 
  
using namespace apache::thrift;  
using namespace apache::thrift::protocol;  
using namespace apache::thrift::transport;  
  
using boost::shared_ptr;  
using namespace  ::server::pose_label;
  
int main(int argc, char **argv) {  

    boost::shared_ptr<TSocket> socket_(new TSocket("localhost", 10099));  
    boost::shared_ptr<TTransport> transport_(new TFramedTransport(socket_));  
    boost::shared_ptr<TProtocol> protocol_(new TBinaryProtocol(transport_));  
  
    transport_->open();  
    
	server::pose_label::LabelServiceClient client_(protocol_);

    QueryUnlabeledRet qRet;
    client_.QueryUnlabeledPic(qRet, "chenzx", 0);  

    if(qRet.code != 0){
        LOG(ERROR) << "QueryUnlabeledPic  failed ";
        return -1;
    }
    else{
        LOG(INFO) << "QueryUnlabeledPic success ";
        LOG(INFO) << "vector size :  " <<qRet.picVec.size();
        if(qRet.picVec.size() > 1){
            LOG(INFO) << "pic_url :" << qRet.picVec[0].pic_url  << "   bin length :  " << qRet.picVec[0].screenshot_bin.length();
        }
    }
    DownloadRet dRet;
    client_.DownloadPic(dRet, "../pic/e2356db743b6a5de82bc1aba50fd0d60.jpeg");
    LOG(INFO) << "HumanPose2DInfo:  " << dRet.HumanPose2DInfo ;
    LOG(INFO) << "\n\n\n" << "HumanPose3DInfo:  " << dRet.HumanPose3DInfo;
    LOG(INFO) << "file length :  "   << dRet.bin.length();

    LOG(INFO) << "client end ";
    return 0; 
    //////////////////////////////
    boost::shared_ptr<TSocket> socket(new TSocket("localhost", 10088));  
    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));  
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));  
  
    transport->open();  
    
	server::pose_label::DataServiceClient client(protocol);

    server::pose_label::ReturnVals  ret;
    std::string md5 = "38a92787baf71dd1ac46147c5f515854";
    client.UploadRequest(ret, md5);

    if(ret.code != 0){
        LOG(ERROR) << "pic already exist ";
        return -1;
    }
    else{
        LOG(INFO) << "UploadRequest success ";
    }

    PicInfo pic;
    std::string url = "../test.jpg";
    std::ifstream fin;
	std::stringstream ss;
	fin.open(url, std::ifstream::binary);
	if (!fin.is_open()){
		return false;
	}

	ss << fin.rdbuf();
	pic.oriBin = ss.str();
	fin.close();
    pic.md5 = "38a92787baf71dd1ac46147c5f515854";
    pic.key = "key";
    pic.tag = "tag";
    pic.filename = "test.jpg";
	client.PicUpload(ret, pic, 0.001);
    
    if(ret.code == 0)
    {
        LOG(INFO) << "PicUpload success ";
    }
    else{
         LOG(INFO) << "PicUpload failed ";
    }
  
    transport->close(); 
//////////////////////////////////////////////////////
  

    LOG(INFO) << "client end ";
    return 0;  
} 