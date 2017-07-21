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