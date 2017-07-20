#include "CrawlerService.h"  // 替换成你的.h  
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
    boost::shared_ptr<TSocket> socket(new TSocket("localhost", 10088));  
    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));  
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));  
  
    transport->open();  
    
	server::pose_label::CrawlerServiceClient client(protocol);

    server::pose_label::ReturnVals  ret;
    std::string md5 = "38a92787baf71dd1ac46147c5f515854";
    client.UploadRequest(ret, md5);

    if(ret.code != 0){
        LOG(ERROR) << "pic already exist ";
        return -1;

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
	client.PicUpload(ret, pic);
  
    transport->close();  
  
    return 0;  
} 