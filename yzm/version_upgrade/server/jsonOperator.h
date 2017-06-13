#ifndef _JSON_OPERATOR_H
#define _JSON_OPERATOR_H

#include <string>  
#include <string.h> 
#include <iostream>
#include <map>
#include <vector>


#include "document.h"		       // rapidjson's DOM-style API
#include "prettywriter.h"	       // for stringify JSON
#include "stringbuffer.h" 
#include "writer.h"

using namespace rapidjson;
using namespace std;


typedef    map<string,string>		KeyValueMap;
typedef    vector<KeyValueMap>		SqlMapVector;  


class CJsonOperator
{
public:
	CJsonOperator(Document::AllocatorType& allocator);
	~CJsonOperator();
	bool addMember(string key, string value);
	bool addArray(string key, SqlMapVector& mapVec);
	bool genOutput();
	string getOutput() { return _strOutput; }


private:
	Document::AllocatorType& _allocator;
	Value* _root;
	string _strOutput;

};


#endif
