# mms server Makefile公共规则定义
#

#CC = gcc
CC = g++
CXX = g++
#CFLAGS = -Wall -g
#CFLAGS = -Wall -g -DDEBUG_LOG_ARGS
CFLAGS = -O3 -DNDEBUG -w  -std=c++11 -D EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET  \


TRPC_HOME = ${HOME}/cmsc/middle
INC_MYSQL = -I/usr/local/mysql/include/mysql
COMM =$(TRPC_HOME)/comm

LIB_TRPC_SYS = $(TRPC_HOME)/lib/libtrpc_sys.a
LIB_TRPC_SERVER = $(TRPC_HOME)/lib/libtrpc_server.a
LIB_TRPC = $(TRPC_HOME)/lib/libtrpc.a
LIB_TRPC_CLIENT = $(TRPC_HOME)/lib/libtrpc_client.a


# 自动计算文件的依赖关系
.%.d: %.cpp
	$(CC) $(INCLUDE) -MM $< > $@
	@$(CC) $(INCLUDE) -MM $< | sed s/"^"/"\."/  |  sed s/"^\. "/" "/  | \
                sed s/"\.o"/"\.d"/  >> $@
%.o: %.cpp 
	$(CXX) $(CFLAGS) $(INCLUDE) -c $<

.%.d: %.c
	$(CC) $(INCLUDE) -MM $< > $@
	@$(CC) $(INCLUDE) -MM $< | sed s/"^"/"\."/  |  sed s/"^\. "/" "/  | \
                sed s/"\.o"/"\.d"/  >> $@
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

