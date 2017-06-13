#include"jsonOperator.h"

CJsonOperator::CJsonOperator(Document::AllocatorType& allocator)
:_allocator(allocator)
{
	_root = new Value(kObjectType);
}

CJsonOperator::~CJsonOperator()
{
	if(_root)
	{
		delete _root;
		_root= NULL;
	}

}

bool CJsonOperator::addMember(string key, string value)
{
	Value vKey(kStringType);
	vKey.SetString(key.c_str(), _allocator);
	Value vValue(kStringType);
	vValue.SetString(value.c_str(), _allocator);

	_root->AddMember(vKey, vValue, _allocator);  
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