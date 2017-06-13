#ifndef _HTTP_HANDLE_H
#define _HTTP_HANDLE_H

#include <iostream>
using namespace std;

class ChttpHandler
{
public:
	ChttpHandler();
	~ChttpHandler();
	int dealFittingOnlineVersionQuery(string& response,  bool& error, string& errorInfo);
	
};

#endif
