#include"jsonOperator.h"

using namespace std;

CJsonOperator::CJsonOperator()
{
	_root.reset(new Value);
	_root->SetObject();
}

CJsonOperator::~CJsonOperator()
{

}

bool CJsonOperator::addStringMember(string key, string value)
{
	Value vValue;
	Value vKey;
	vKey.SetString(key.c_str(), key.length(), doc_.GetAllocator());
	vValue.SetString(value.c_str(), value.length(), doc_.GetAllocator());
	_root->AddMember(vKey, vValue,  doc_.GetAllocator());  

	return true;
}

bool CJsonOperator::addIntegerMember(std::string key, int value)
{
	_root->AddMember(StringRef(key.c_str()), value, doc_.GetAllocator());  

	return true;   
}

bool CJsonOperator::addStringArray(string key, ssMapVec& mapVec)
{
	Value vArray(kArrayType);
    for (auto& it : mapVec)
    {
		 Value item(kObjectType);
		 for(auto&  it2  : it) 
         {
			Value vKey,vValue;
			vKey.SetString(it2.first.c_str(), it2.first.length(), doc_.GetAllocator());
			vValue.SetString(it2.second.c_str(), it2.second.length(), doc_.GetAllocator());
			item.AddMember(vKey, vValue, doc_.GetAllocator());
         }

		 vArray.PushBack(item, doc_.GetAllocator()); 
    }
	 Value Key;
	 Key.SetString(key.c_str(), key.length(), doc_.GetAllocator());
	_root->AddMember(Key, vArray, doc_.GetAllocator());
	
	return true;  
}

void CJsonOperator::genOutput()
{
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	_root->Accept(writer);
	_strOutput= buffer.GetString();

}