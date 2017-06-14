include  ./Inc.mk

INCLUDE = -I../pretretment/include  -I/home/doudou/myACE/include
		  
TARGET = test
OBJS  := HttpSRV.o  HttpSRVTest.o

LIBS =  -lpthread  -L/home/doudou/myACE/lib  -lACE  -lPocoFoundation -lPocoNet  -lPocoUtil -lPocoXML  -lcurl


CFLAGS += -DTIXML_USE_STL -fPIC -g -Wall
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -g  -lc -o $@ $(CFLAGS) $^ $(LIBS) $(INCLUDE)
	


install:

clean:
	rm -f $(OBJS) .*.d $(TARGET)
