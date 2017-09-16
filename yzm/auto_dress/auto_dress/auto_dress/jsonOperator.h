#ifndef _JSON_OPERATOR_H
#define _JSON_OPERATOR_H

#include <string>   
#include <map>
#include <vector>
#include<memory>

#include "document.h"		       // rapidjson's DOM-style API
#include "prettywriter.h"	       // for stringify JSON
#include "stringbuffer.h" 
#include "writer.h"

using namespace rapidjson;

using    ssMap    =   std::map<std::string,std::string>;		
using    ssMapVec =   std::vector<ssMap>;  

class CJsonOperator
{
public:
	CJsonOperator();
	~CJsonOperator();
	bool addStringMember(std::string key, std::string value);
	bool addIntegerMember(std::string key, int value);
	bool addStringArray(std::string key,  ssMapVec& mapVec);
	void genOutput();
	std::string getOutput() { return _strOutput; }

private:
	std::shared_ptr<Value>  _root;
	std::string _strOutput;
	Document doc_;    

};


#endif
