#ifndef _REDIS_OPERATOR_H
#define _REDIS_OPERATOR_H

#include <hiredis/hiredis.h>

#include <string>  
#include <queue> 
#include <mutex>

class CRedisOperator
{
public:
	CRedisOperator(std::string ip, int port, int timeout = 2);
	~CRedisOperator();
	bool setValue(std::string & key,std::string & value);
	bool getValue(std::string & key,std::string & value);
	bool setExpireTime(std::string & key,std::string secTime);
	redisReply* ExecuteCmd(const char *cmd);
	redisContext* CreateContext();
	bool ExecuteCmd(const char *cmd, std::string &response);
	void ReleaseContext(redisContext *ctx, bool active);
	bool CheckStatus(redisContext *ctx); 
	void setPoolSize(int size);

private:
	int _poolSize{16};
	std::string _strIp;
	int _iPort;
	int _iTimeout;
	std::mutex _lock;
	std::queue<redisContext *> _clients;
	time_t _beginInvalidTime;
    static const int _maxReconnectInterval = 3;  
	
};

#endif

