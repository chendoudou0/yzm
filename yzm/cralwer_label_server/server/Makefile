include  ./Inc.mk

vpath %.h   ../thrift/gen-cpp   ../thrift/arithmetic/gen-cpp
vpath %.cpp ../thrift/gen-cpp   ../thrift/arithmetic/gen-cpp

INCLUDE =  -I. -I/usr/include/mysql  -I/usr/local/mysql/include -I../include  \
-I../3party  -I../3party/rapidjson    -I../thrift/gen-cpp   -I../thrift/arithmetic/gen-cpp  \
-I/usr/local/include/opencv  -I/usr/local/include/opencv2  

TARGET = upgrade_server		  

OBJS  := CrawlerService.o LabelService.o \
pose_label_types.o pose_label_constants.o \
Pose3DAnnPreprocess.o Pose3DAnnNew_types.o \
ThreadBase.o  IApp.o config.o upgradeServer.o requestProcessor.o  CLogger.o  main.o \
redisOperator.o DBOperator.o sqlapi.o    Common.o  HttpTool.o \
jsonOperator.o


LIBS =  -L/usr/local/mysql/lib  -lpthread  -lPocoFoundation -lPocoNet   \
-lcurl -lmysqlclient   \
-lhiredis   -lglog   \
-lthrift    -lfreeimage   \
-lopencv_calib3d  -lopencv_core  -lopencv_highgui \
-lopencv_imgproc  -lopencv_objdetect \

CFLAGS += -DTIXML_USE_STL -w -std=c++11 -g  
all: $(TARGET)  

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(CFLAGS) $^ $(LIBS) $(INCLUDE) 

crawler_client: CrawlerService.o pose_label_types.o  client.o
	$(CXX) -o $@ $(CFLAGS) $^ $(LIBS) $(INCLUDE) 

syl: arithmetic_client  arithmetic_server
arithmetic_client: Pose3DAnnPreprocess.o Pose3DAnnNew_types.o  arithmetic_client.o
	$(CXX) -o $@ $(CFLAGS) $^ $(LIBS) $(INCLUDE) 

arithmetic_server: Pose3DAnnPreprocess.o Pose3DAnnNew_types.o  Pose3DAnnPreprocess_server.skeleton.o
	$(CXX) -o $@ $(CFLAGS) $^ $(LIBS) $(INCLUDE) 

install:
	@mkdir -p ../build/
	@mkdir -p ../build/bin/
	@mkdir -p ../build/log/
	@mkdir -p ../build/glog/
	
	cp -r ../libs/ ../build/
	cp -rf ../conf/ ../build/
	cp -f $(TARGET) ../build/bin/    

install2:
	supervisorctl stop   upgrade_server   
	supervisorctl stop   upgrade_server_dev
	sleep 2
	cp ./upgrade_server /opt/yzm_upgrade/build/bin/
	cp ./upgrade_server /opt/yzm_upgrade_dev/build/bin/
	supervisorctl start   upgrade_server   
	supervisorctl start   upgrade_server_dev

genThrift:
	rm   ../thrift/gen-cpp/*
	thrift  -r --gen cpp   -o  ../thrift  ../thrift/pose_label.thrift
	rm  -r ../thrift/arithmetic/*
	thrift  -r --gen cpp   -o  ../thrift/arithmetic  ../thrift/Pose3DAnnNew.thrift

clean:
	rm -f $(OBJS) .*.d $(TARGET)
