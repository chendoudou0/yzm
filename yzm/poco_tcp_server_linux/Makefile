include  ./Inc.mk

INCLUDE = -I../pretretment/include  -I/home/doudou/myACE/include
		  
TARGET = test
OBJS  := ClientHandler.o TcpSvrMgr.o test.o

LIBS =  -lpthread  -L/home/doudou/myACE/lib -lPocoFoundation -lPocoNet  -lPocoUtil -lPocoXML


CFLAGS += -DTIXML_USE_STL -fPIC -g -Wall
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -g  -lc -o $@ $(CFLAGS) $^ $(LIBS) $(INCLUDE)
	
install:

clean:
	rm -f $(OBJS) .*.d $(TARGET)
