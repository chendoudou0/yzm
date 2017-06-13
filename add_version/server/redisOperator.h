#ifndef _REDIS_OPERATOR_H
#define _REDIS_OPERATOR_H
 
#include <string>  
#include <string.h> 
#include <iostream>
#include <queue> 
#include <hiredis/hiredis.h>
#include <sstream>
#include "Mutex.h"

using namespace std;

class CRedisOperator
{
public:
	CRedisOperator(string ip, int port, int timeout = 2);
	~CRedisOperator();
	bool setValue(string & key,string & value);
	bool getValue(string & key,string & value);
	bool setExpireTime(string & key,string secTime);
	redisReply* ExecuteCmd(const char *cmd);
	redisContext* CreateContext();
	bool ExecuteCmd(const char *cmd, string &response);
	void ReleaseContext(redisContext *ctx, bool active);
	bool CheckStatus(redisContext *ctx);  

private:

	string _strIp;
	int _iPort;
	int _iTimeout;
	cMutex _lock;
	queue<redisContext *> _clients;
	time_t _beginInvalidTime;
    static const int _maxReconnectInterval = 3;  
	

};

#endif

