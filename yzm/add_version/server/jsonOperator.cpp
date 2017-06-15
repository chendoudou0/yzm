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

	return true;
}

bool CJsonOperator::addArray(string key, SqlMapVector& mapVec)
{
	SqlMapVector::iterator iter;
	Value vArray(kArrayType);
    for (iter = mapVec.begin(); iter != mapVec.end(); ++iter)
    {
		 Value item(kObjectType);
		 KeyValueMap::iterator iter2;
		 for(iter2=(*iter).begin(); iter2!=(*iter).end(); iter2++)
         {
            Value vKey(kStringType);
			vKey.SetString(iter2->first.c_str(), _allocator);
			Value vValue(kStringType);
			vValue.SetString(iter2->second.c_str(), _allocator);
			item.AddMember(vKey, vValue, _allocator);
         }
		vArray.PushBack(item, _allocator); 
    }
	Value arrayKey(kStringType);
	arrayKey.SetString(key.c_str(), _allocator);
	_root->AddMember(arrayKey, vArray, _allocator);
	
	return true;  
}

bool CJsonOperator::genOutput()
{
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	_root->Accept(writer);
	_strOutput= buffer.GetString();
	return true; 
}