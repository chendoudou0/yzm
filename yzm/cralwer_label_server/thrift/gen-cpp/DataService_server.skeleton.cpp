// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "DataService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::server::pose_label;

class DataServiceHandler : virtual public DataServiceIf {
 public:
  DataServiceHandler() {
    // Your initialization goes here
  }

  void UploadRequest(ReturnVals& _return, const std::string& md5) {
    // Your implementation goes here
    printf("UploadRequest\n");
  }

  void PicUpload(ReturnVals& _return, const PicInfo& pic, const double percent, const int32_t sumTotal) {
    // Your implementation goes here
    printf("PicUpload\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<DataServiceHandler> handler(new DataServiceHandler());
  shared_ptr<TProcessor> processor(new DataServiceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

