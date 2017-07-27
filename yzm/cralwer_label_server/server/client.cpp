#include "DataService.h"   
#include "LabelService.h"  
#include "CrawlerClientService.h"  
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

    
/*
    boost::shared_ptr<TSocket> socket__(new TSocket("localhost", 10077));  
    boost::shared_ptr<TTransport> transport__(new TFramedTransport(socket__));  
    boost::shared_ptr<TProtocol> protocol__(new TBinaryProtocol(transport__));  
  
    transport__->open();  
    
	server::pose_label::CrawlerClientServiceClient client__(protocol__);

    ReturnVals sRet;
    client__.start(sRet, "街拍帅哥", "Baidu", "街拍帅哥");
    if(sRet.code == 0){
         LOG(INFO) << "start success ";
    }else{
        LOG(INFO) << "start failed ";
    }

    CrawlerStatusRet  cRet;
    client__.InquireCrawlerStatus(cRet);
    if(cRet.code == 0){
         LOG(INFO) << "InquireCrawlerStatus success ";
         LOG(INFO) << "totalPicSum : " << cRet.crawlerStatus.totalPicSum;
          LOG(INFO) << "dbPicSum : " <<   cRet.crawlerStatus.dbPicSum;
    }else{
        LOG(INFO) << "InquireCrawlerStatus failed ";
    }


    ReturnVals stopRet;
    client__.stop(stopRet);
    if(stopRet.code == 0){
         LOG(INFO) << "stop success ";
    }else{
        LOG(INFO) << "stop failed ";
    }


    LOG(INFO) << "client end ";
    */
   
////////////////////////////////////////////////////////////////////////////////
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
            for(auto&pic : qRet.picVec ){
                //  LOG(INFO) <<"pic_id :"  <<pic.pic_id;
                //  LOG(INFO) <<"create_time  :"  <<pic.create_time;
                //  LOG(INFO) <<"labeled_count :"  <<pic.labeledCount;
                  LOG(INFO) << "pic_url :" << pic.pic_url  << "   bin length :  " << pic.screenshot_bin.length();
            }
        }
    }   


    QueryLabeledRet qlRet;
    client_.QueryLabeledPic(qlRet, "chenzx", 0);  

    if(qRet.code != 0){
        LOG(ERROR) << "QueryLabeledPic  failed ";
        return -1;
    }
    else{
        LOG(INFO) << "QueryLabeledPic success ";
        LOG(INFO) << "vector size :  " <<qlRet.picVec.size();
        if(qlRet.picVec.size() > 0){
            for(auto&pic : qlRet.picVec ){
                 LOG(INFO) <<"pic_id : "  <<  pic.pic_id;
               //  LOG(INFO) << "pic_url : " << pic.pic_url  << "   bin length :  " << pic.screenshot_bin.length();
            }
        }
    }

    QueryLabeledRet qoRet;
    client_.QueryPicLabeledByOthers(qoRet, "chenzx", 30);  

    if(qoRet.code != 0){
        LOG(ERROR) << "QueryLabeledPicByOthers  failed ";
        return -1;
    }
    else{
        LOG(INFO) << "QueryLabeledPicByOthers success ";
        LOG(INFO) << "vector size :  " <<qoRet.picVec.size();
        if(qoRet.picVec.size() > 1){
            for(auto&pic : qoRet.picVec ){
                 LOG(INFO) <<"other labeled pic_id : "  << pic.pic_id;
               //  LOG(INFO) << "pic_url : " << pic.pic_url  << "   bin length :  " << pic.screenshot_bin.length();
            }
        }
    }
    ReturnVals ivRet;
    client_.InvalidatePicture(ivRet, 69, "chenzx"); 
    if(ivRet.code == 0){
        LOG(INFO) <<"InvalidatePicture  success ";
    }else{
        LOG(INFO) <<"InvalidatePicture  failed ";
    }
    LabeledPoseDataRet  lpRet;
    client_.QueryLabeledPoseData(lpRet, 40, "../pic/e2356db743b6a5de82bc1aba50fd0d60.jpeg", "chenzx");
    if(lpRet.code == 0){
        LOG(INFO) <<"QueryLabeledPoseData  success ";
        LOG(INFO) <<"QueryLabeledPoseData  : " << lpRet.poseData;
    }else{
          LOG(INFO) <<"QueryLabeledPoseData  failed ";
    }

    DownloadRet dRet;
    client_.DownloadPic(dRet, "../pic/e2356db743b6a5de82bc1aba50fd0d60.jpeg");
    LOG(INFO) << "HumanPose2DInfo length :  " << dRet.HumanPose2DInfo.length() ;
    LOG(INFO) << "\n\n\n" << "HumanPose3DInfo length :  " << dRet.HumanPose3DInfo.length();
    LOG(INFO) << "file length :  "   << dRet.bin.length();
    LOG(INFO) << "pic id :  "   << dRet.pic_id;

    ReturnVals  iRet;
    client_.InsertToDb(iRet, "poseInfo", 42, "chenzixun");
    if(iRet.code == 0){
         LOG(INFO) << "InsertToDb  success  "  ;
    }
    else{
        LOG(INFO) << "InsertToDb  failed  "  ;
    }

    LOG(INFO) << "client end ";
   /*
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
	client.PicUpload(ret, pic, 0.0001, 1000);
    
    if(ret.code == 0)
    {
        LOG(INFO) << "PicUpload success ";
    }
    else{
         LOG(INFO) << "PicUpload failed ";
    }
  
    transport->close(); 
//////////////////////////////////////////////////////
  */

    LOG(INFO) << "client end ";
    return 0;  

} 