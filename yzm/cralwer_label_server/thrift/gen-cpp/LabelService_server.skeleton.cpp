// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "LabelService.h"
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

class LabelServiceHandler : virtual public LabelServiceIf {
 public:
  LabelServiceHandler() {
    // Your initialization goes here
  }

  void QueryUnlabeledPic(QueryUnlabeledRet& _return, const std::string& token, const int32_t index, const QueryCondition& qc) {
    // Your implementation goes here
    printf("QueryUnlabeledPic\n");
  }

  void QueryPicLabeledByOthers(QueryLabeledRet& _return, const std::string& token, const int32_t index, const QueryCondition& qc) {
    // Your implementation goes here
    printf("QueryPicLabeledByOthers\n");
  }

  void QueryLabeledPic(QueryLabeledRet& _return, const std::string& token, const int32_t index, const QueryCondition& qc) {
    // Your implementation goes here
    printf("QueryLabeledPic\n");
  }

  void QueryPicById(QueryByIdRet& _return, const std::string& token, const int32_t pic_id) {
    // Your implementation goes here
    printf("QueryPicById\n");
  }

  void QueryPicByUserName(QueryLabeledRet& _return, const std::string& token, const std::string& user, const int32_t index, const QueryCondition& qc) {
    // Your implementation goes here
    printf("QueryPicByUserName\n");
  }

  void DownloadPic(DownloadRet& _return, const std::string& pic_url) {
    // Your implementation goes here
    printf("DownloadPic\n");
  }

  void InsertToDb(ReturnVals& _return, const std::string& poseInfo, const int32_t pic_id, const std::string& token) {
    // Your implementation goes here
    printf("InsertToDb\n");
  }

  void InvalidatePicture(ReturnVals& _return, const int32_t pic_id, const std::string& token, const bool type) {
    // Your implementation goes here
    printf("InvalidatePicture\n");
  }

  void QueryLabeledPoseData(LabeledPoseDataRet& _return, const int32_t pic_id, const std::string& pic_url, const std::string& token) {
    // Your implementation goes here
    printf("QueryLabeledPoseData\n");
  }

  void QueryLastLabeledPoseData(LabeledPoseDataRet& _return, const int32_t pic_id, const std::string& pic_url, const std::string& user) {
    // Your implementation goes here
    printf("QueryLastLabeledPoseData\n");
  }

  void Register(ReturnVals& _return, const std::string& user, const std::string& passwd) {
    // Your implementation goes here
    printf("Register\n");
  }

  void Login(LoginRet& _return, const std::string& user, const std::string& passwd) {
    // Your implementation goes here
    printf("Login\n");
  }

  void QueryPicPoseData(PoseDatasRet& _return, const int32_t pic_id, const std::string& pic_url, const std::string& token) {
    // Your implementation goes here
    printf("QueryPicPoseData\n");
  }

  void QueryPoseDataScore(ScoreQueryRet& _return, const int32_t pic_id, const std::string& label_user, const std::string& token) {
    // Your implementation goes here
    printf("QueryPoseDataScore\n");
  }

  void ScorePoseData(ReturnVals& _return, const int32_t pic_id, const std::string& label_user, const std::string& token, const double score) {
    // Your implementation goes here
    printf("ScorePoseData\n");
  }

  void RepreProcessPic(ReturnVals& _return, const std::string& pic_url, const std::string& token) {
    // Your implementation goes here
    printf("RepreProcessPic\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<LabelServiceHandler> handler(new LabelServiceHandler());
  shared_ptr<TProcessor> processor(new LabelServiceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

