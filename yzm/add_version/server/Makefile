include  ./Inc.mk

INCLUDE =  -I. -I/usr/include/mysql  -I/usr/local/mysql/include -I../include  -I../rapidjson  -I../tinyxml
		  
TARGET = test
OBJS  := configManager.o CLogger.o httpHandler.o main.o jsonOperator.o redisOperator.o DBOperator.o sqlapi.o exception.o  Mutex.o  WriteLog.o  Common.o  HttpSRV.o

LIBS =  -L/usr/local/mysql/lib  -lpthread  -lPocoFoundation -lPocoNet  -lPocoUtil -lPocoXML -lcurl -lmysqlclient   \
../tinyxml/libtinyxml.a  -lhiredis   -lglog


CFLAGS += -DTIXML_USE_STL -fPIC -g -w -std=c++11
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -g  -lc -o $@ $(CFLAGS) $^ $(LIBS) $(INCLUDE)
	

install:


clean:
	rm -f $(OBJS) .*.d $(TARGET)
