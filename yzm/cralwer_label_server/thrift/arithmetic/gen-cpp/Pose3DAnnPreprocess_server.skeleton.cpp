// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "Pose3DAnnPreprocess.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::ThriftPose3DAnn;

class Pose3DAnnPreprocessHandler : virtual public Pose3DAnnPreprocessIf {
 public:
  Pose3DAnnPreprocessHandler() {
    // Your initialization goes here
  }

  bool CheckIsValid(const Image& img) {
    // Your implementation goes here
    printf("CheckIsValid\n");
  }

  void DoInitGuess(GuessInfo& _return, const Image& img) {
    // Your implementation goes here
    printf("DoInitGuess\n");
  }

  void AutoCalPose3D(std::vector<HumanPose3DInfo> & _return, const Image& img, const GuessInfo& guessInfo) {
    // Your implementation goes here
    printf("AutoCalPose3D\n");
  }

};

int main(int argc, char **argv) {
  int port = 10000;
  shared_ptr<Pose3DAnnPreprocessHandler> handler(new Pose3DAnnPreprocessHandler());
  shared_ptr<TProcessor> processor(new Pose3DAnnPreprocessProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

