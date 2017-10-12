include  ./Inc.mk

INCLUDE =  -I. -I/usr/include/mysql  -I/usr/local/mysql/include -I../include  \
-I../3party  -I../3party/rapidjson  

TARGET = upgrade_server		  

OBJS  :=   IApp.o config.o upgradeServer.o requestProcessor.o   main.o \
 redisOperator.o DBOperator.o sqlapi.o    Common.o  HttpTool.o \
 jsonOperator.o

LIBS =  -L/usr/local/mysql/lib  -lpthread  -lPocoFoundation -lPocoNet   \
-lcurl -lmysqlclient   \
-lhiredis   -lglog

CFLAGS += -DTIXML_USE_STL -w -std=c++11 -g  
all: $(TARGET)  

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(CFLAGS) $^ $(LIBS) $(INCLUDE) 

install:
	@mkdir -p ../build/
	@mkdir -p ../build/bin/
	@mkdir -p ../build/log/
	@mkdir -p ../build/glog/     
	
	cp -r ../libs/ ../build/
	cp -rf ../conf/ ../build/
	cp -f $(TARGET) ../build/bin/    

supervisor:
	supervisorctl stop   upgrade_server   
	supervisorctl stop   upgrade_server_dev
	cp ./upgrade_server /opt/yzm_upgrade/build/bin/    
	cp ./upgrade_server /opt/yzm_upgrade_dev/build/bin/
	supervisorctl start   upgrade_server     
	supervisorctl start   upgrade_server_dev

clean:
	rm -f $(OBJS) .*.d $(TARGET)
