#include "DataService.h"   
#include "LabelService.h"  
#include "CrawlerClientService.h"  
#include <thrift/transport/TSocket.h>  
#include <thrift/transport/TBufferTransports.h>  
#include <thrift/protocol/TBinaryProtocol.h>  
#include<fstream>
#include<sstream>
#include<glog/logging.h> 
#include <uuid/uuid.h> 
  
using namespace apache::thrift;  
using namespace apache::thrift::protocol;  
using namespace apache::thrift::transport;  
  
using boost::shared_ptr;  
using namespace  ::server::pose_label;
  
int main(int argc, char **argv) { 

    boost::shared_ptr<TSocket> socket__(new TSocket("localhost", 10077));  
    boost::shared_ptr<TTransport> transport__(new TFramedTransport(socket__));  
    boost::shared_ptr<TProtocol> protocol__(new TBinaryProtocol(transport__));  
  
    transport__->open();  
    
	server::pose_label::CrawlerClientServiceClient client__(protocol__);
/*
    ReturnVals sRet;
    client__.start(sRet, "街拍帅哥", "Baidu", "街拍帅哥");
    if(sRet.code == 0){
         LOG(INFO) << "start success ";
    }else{
        LOG(INFO) << "start failed ";
    }

    ReturnVals stopRet;
    client__.stop(stopRet);
    if(stopRet.code == 0){
         LOG(INFO) << "stop success ";
    }else{
        LOG(INFO) << "stop failed ";
    }
*/
    CrawlerStatusRet  cRet;
    client__.InquireCrawlerStatus(cRet);
    if(cRet.code == 0){
         LOG(INFO) << "InquireCrawlerStatus success ";
         LOG(INFO) << "totalPicSum : " << cRet.crawlerStatus.totalPicSum;
         LOG(INFO) << "dbPicSum : " <<   cRet.crawlerStatus.dbPicSum;
    }else{
        LOG(INFO) << "InquireCrawlerStatus failed ";
    }

    CrawlerHistoryRet cHisRet;
    HistoryQueryCondition hisQc;
    hisQc.tag = "舞";
    client__.QueryCrawlingHistory(cHisRet , hisQc );
    if(cHisRet.code == 0){
        LOG(INFO) << "QueryCrawlingHistory success ";
        LOG(INFO) << "vec[0].key "<<cHisRet.hisVec[0].key;

    }else{
        LOG(INFO) << "QueryCrawlingHistory failed ";

    }

    CrawleredPicRet  qcraedRet;
    QueryConditions  cpqCondi;
    cpqCondi.key = "太极";
    client__.QueryCrawleredPic(qcraedRet, cpqCondi, 0);
     if(qcraedRet.code == 0){
        LOG(INFO) << "QueryCrawleredPic success ";
        LOG(INFO) << "picVec[0].pic_url  "<<qcraedRet.picVec[0].pic_url;

    }else{
        LOG(INFO) << "QueryCrawleredPic failed ";

    }

    ReturnVals upRet;
    client__.UpdatePicInfo(upRet, 64, "改标签", "蹲下");
    if(upRet.code == 0){
        LOG(INFO) << "UpdatePicInfo success ";
    }else{
        LOG(INFO) << "UpdatePicInfo  failed ";
    }

    LOG(INFO) << "client end ";
    return 0;
    

////////////////////////////////////////////////////////////////////////////////
    boost::shared_ptr<TSocket> socket_(new TSocket("192.168.1.110", 10099));  
    boost::shared_ptr<TTransport> transport_(new TFramedTransport(socket_));  
    boost::shared_ptr<TProtocol> protocol_(new TBinaryProtocol(transport_));  
  
    transport_->open();  
    
	server::pose_label::LabelServiceClient client_(protocol_);

    QueryUnlabeledRet qRet;
    QueryCondition qc;
    qc.pose_type = "";
    qc.tag = "";
    qc.tBegin = "2017-7-19 10:10:10";
    qc.tEnd = "2017-7-29 10:10:10";   
    client_.QueryUnlabeledPic(qRet, "shiyl", 0, qc);  

    if(qRet.code != 0){
        LOG(ERROR) << "QueryUnlabeledPic  failed ";

    }
    else{
        LOG(INFO) << "QueryUnlabeledPic success ";
        LOG(INFO) << "vector size :  " <<qRet.picVec.size();
        if(qRet.picVec.size() > 0){
            for(auto&pic : qRet.picVec ){
                LOG(INFO) << "pic_url :" << pic.pic_url  << "bin length :  " << pic.screenshot_bin.length();
                LOG(INFO) << "create_time  :" << pic.create_time;
                LOG(INFO) << "update_user  :" << pic.lastLabeledUser;           
            }
        }
    }   

    QueryLabeledRet qlRet;
    client_.QueryLabeledPic(qlRet, "shiyl", 0, qc);  

    if(qRet.code != 0){
        LOG(ERROR) << "QueryLabeledPic  failed ";
      
    }
    else{
        LOG(INFO) << "QueryLabeledPic success ";
        LOG(INFO) << "vector size :  " <<qlRet.picVec.size();
        if(qlRet.picVec.size() > 0){
            for(auto&pic : qlRet.picVec ){
                 LOG(INFO) <<"pic_id : "  <<  pic.pic_id;
                 //LOG(INFO) << "pic_url : " << pic.pic_url  << "   bin length :  " << pic.screenshot_bin.length();
            }
        }
    }

    QueryLabeledRet qoRet;
    client_.QueryPicLabeledByOthers(qoRet, "chenzx", 30, qc);  

    if(qoRet.code != 0){
        LOG(ERROR) << "QueryLabeledPicByOthers  failed ";
       
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




    QueryByIdRet qbRet;
    client_.QueryPicById(qbRet, "chenzx", 120);  

    if(qbRet.code != 0){
        LOG(ERROR) << "QueryPicById  failed ";
    
    }else{
        LOG(INFO) << "QueryPicById success ";
      
        LOG(INFO) <<" pic_url : "  << qbRet.pic.pic_url;
    }

    ReturnVals  rRet;
    client_.Register(rRet, "cccccc", "ddddddddddd");
    if(rRet.code == 0){
         LOG(INFO) << "Register success ";
    }else{
        LOG(INFO) << "Register failed ";
    }

    LoginRet  loginRet;
    client_.Login(loginRet, "cccccc", "ddddddddddd");
    if(rRet.code == 0){
         LOG(INFO) << "login success ";
         LOG(INFO) << "token  "<<loginRet.token;
         LOG(INFO) << "role_id  "<<loginRet.role_id;
    }else{
        LOG(INFO) << "login failed ";
    }
   
    client_.QueryUnlabeledPic(qRet, loginRet.token, 0, qc);  

    if(qRet.code != 0){
        LOG(ERROR) << "QueryUnlabeledPic  failed ";

    }
    else{
        LOG(INFO) << "QueryUnlabeledPic success ";
        LOG(INFO) << "vector size :  " <<qRet.picVec.size();
        if(qRet.picVec.size() > 0){
            for(auto&pic : qRet.picVec ){
            
                LOG(INFO) << "pic_url :" << pic.pic_url  << "bin length :  " << pic.screenshot_bin.length();
                LOG(INFO) << "create_time  :" << pic.create_time;
                 LOG(INFO) << "update_user  :" << pic.lastLabeledUser;
            }
        }
    } 

    LabeledPoseDataRet  lpRet;
    client_.QueryLabeledPoseData(lpRet, 40, "../pic/e2356db743b6a5de82bc1aba50fd0d60.jpeg", "chenzx");
    if(lpRet.code == 0){
        LOG(INFO) <<"QueryLabeledPoseData  success ";
        LOG(INFO) <<"QueryLabeledPoseData  : " << lpRet.poseData;
    }else{
          LOG(INFO) <<"QueryLabeledPoseData  failed ";
    } 
    
    QueryLabeledRet  qbuRet;
    client_.QueryPicByUserName(qbuRet, "123456", "aoj", 0, qc);  

    if(qbuRet.code != 0){
        LOG(ERROR) << "QueryPicByUserName  failed ";
       
    }
    else{
        LOG(INFO) << "QueryPicByUserName success ";
        LOG(INFO) << "vector size :  " <<  qbuRet.picVec.size();
        if(qbuRet.picVec.size() > 1){
            for(auto&pic : qbuRet.picVec ){
                 LOG(INFO) <<"pic_id : "  << pic.pic_id;
            }
        }
    } 

/*
    // ReturnVals ivRet;
    // client_.InvalidatePicture(ivRet, 69, "chenzx", true); 
    // if(ivRet.code == 0){
    //     LOG(INFO) <<"InvalidatePicture  success ";
    // }else{
    //     LOG(INFO) <<"InvalidatePicture  failed ";
    // }

    // DownloadRet dRet;
    // client_.DownloadPic(dRet, "../pic/e2356db743b6a5de82bc1aba50fd0d60.jpeg");
    // LOG(INFO) << "HumanPose2DInfo length :  " << dRet.HumanPose2DInfo.length() ;
    // LOG(INFO) << "\n\n\n" << "HumanPose3DInfo length :  " << dRet.HumanPose3DInfo.length();
    // LOG(INFO) << "file length :  "   << dRet.bin.length();
    // LOG(INFO) << "pic id :  "   << dRet.pic_id;

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