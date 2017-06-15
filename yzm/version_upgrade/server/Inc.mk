# mms server Makefile公共规则定义
#

#CC = gcc
CC = g++
CXX = g++

CFLAGS = -O3 -DNDEBUG -w   


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

