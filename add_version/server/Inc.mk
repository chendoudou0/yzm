# mms server Makefile����������
#

#CC = gcc
CC = g++
CXX = g++

CFLAGS = -O3 -DNDEBUG -w   


# �Զ������ļ���������ϵ
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

