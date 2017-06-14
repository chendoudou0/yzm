include  ./Inc.mk

INCLUDE =  -I. -I/usr/include/mysql  -I/usr/local/mysql/include -I../include  -I../rapidjson  \
-I../tinyxml  -I../include/rapidjson
		  
TARGET = monitor
OBJS  := configManager.o   monitor.o

LIBS =  -L/usr/local/mysql/lib  -lpthread  -lPocoFoundation -lPocoNet  -lPocoUtil -lPocoXML -lcurl -lmysqlclient   \
  -lhiredis  -lglog


CFLAGS += -DTIXML_USE_STL  -g -w -std=c++11
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -g  -lc -o $@ $(CFLAGS) $^ $(LIBS) $(INCLUDE)
	

install:

clean:
	rm -f $(OBJS) .*.d $(TARGET)
