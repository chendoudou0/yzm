#include"Pose3DAnnPreprocess.h"
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
using namespace  ThriftPose3DAnn;
  
int main(int argc, char **argv) {  
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
        return -1;
	} 
    
	Pose3DAnnPreprocessClient client(protocol);
	LOG(INFO) << "connect  shiyurong  success";
  
    transport->close();  
  
    return 0;  
} 