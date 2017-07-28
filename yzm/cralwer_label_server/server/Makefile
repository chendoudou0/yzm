include  ./Inc.mk

vpath %.h   ../thrift/gen-cpp   ../thrift/arithmetic/gen-cpp
vpath %.cpp ../thrift/gen-cpp   ../thrift/arithmetic/gen-cpp

INCLUDE =  -I. -I/usr/include/mysql  -I/usr/local/mysql/include -I../include  \
-I../3party  -I../3party/rapidjson    -I../thrift/gen-cpp   -I../thrift/arithmetic/gen-cpp  \
-I/usr/local/include/opencv  -I/usr/local/include/opencv2     \
-I../3party/opencv2.4.9/opencv2  -I../3party/opencv2.4.9/opencv -I../3party/opencv2.4.9  \
-I../3party/poco/include 

TARGET = label_server		  

OBJS  := DataService.o LabelService.o CrawlerClientService.o  CrawlerService.o \
pose_label_types.o pose_label_constants.o \
Pose3DAnnPreprocess.o Pose3DAnnNew_types.o \
ThreadBase.o  IApp.o config.o upgradeServer.o   CLogger.o  main.o \
DBOperator.o sqlapi.o    Common.o   \
jsonOperator.o

LIBS =  -L../3party/poco/lib  -L/usr/local/mysql/lib  -lpthread  -lPocoFoundation -lPocoNet   \
-lcurl -lmysqlclient   \
-lglog   \
-lthrift       \
-L../3party/dev_opencv        \
-L../3party/opencv2.4.9/lib   \
-lopencv_core  -lopencv_highgui  -lopencv_imgproc   \

CFLAGS += -DTIXML_USE_STL -w -std=c++11 -g  
all: $(TARGET)  

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(CFLAGS) $^ $(LIBS) $(INCLUDE) 

crawler_client_obj := DataService.o  LabelService.o  CrawlerClientService.o  pose_label_types.o  client.o
crawler_client: $(crawler_client_obj)
	$(CXX) -o $@ $(CFLAGS) $^ $(LIBS) $(INCLUDE) 

syl: arithmetic_client  arithmetic_server
arithmetic_client_obj := Pose3DAnnPreprocess.o Pose3DAnnNew_types.o  arithmetic_client.o
arithmetic_client: $(arithmetic_client_obj)
	$(CXX) -o $@ $(CFLAGS) $^ $(LIBS) $(INCLUDE) 
arithmetic_serve_obj := Pose3DAnnPreprocess.o Pose3DAnnNew_types.o  Pose3DAnnPreprocess_server.skeleton.o
arithmetic_server: $(arithmetic_serve_obj)
	$(CXX) -o $@ $(CFLAGS) $^ $(LIBS) $(INCLUDE) 

install:
	@mkdir -p ../build/
	@mkdir -p ../build/bin/
	@mkdir -p ../build/log/
	@mkdir -p ../build/glog/
	@mkdir -p ../build/pic_ori/
	@mkdir -p ../build/pic_preproccess/
	@mkdir -p ../build/pic_error/
	
	cp -r ../libs/ ../build/
	cp -rf ../conf/ ../build/
	cp -f $(TARGET) ../build/bin/    

start:
	sudo  supervisorctl stop   label_server   
	@cp ./label_server    ../build/bin/
	sudo  supervisorctl start   label_server   

genThrift:
	@-rm   ../thrift/gen-cpp/*
	thrift  -r --gen cpp   -o  ../thrift  ../thrift/pose_label.thrift
	@-rm  -r ../thrift/arithmetic/*
	thrift  -r --gen cpp   -o  ../thrift/arithmetic  ../thrift/Pose3DAnnNew.thrift

clean:
	rm -f $(OBJS) .*.d $(TARGET)  $(crawler_client_obj)  $(arithmetic_client_obj)  $(arithmetic_serve_obj)
