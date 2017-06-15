#include"redisOperator.h"


CRedisOperator::CRedisOperator(string ip, int port, int timeout )
:_strIp(ip),_iPort(port),_iTimeout(timeout),
_beginInvalidTime(0)
{
	_lock.Init();
}
CRedisOperator::~CRedisOperator()
{
	_lock.Lock();
    while(!_clients.empty())
    {
        redisContext *ctx = _clients.front();
        redisFree(ctx);
        _clients.pop();
    }

	_lock.unLock();

}

bool CRedisOperator::setValue(string & key,string & value)
{
	char command[1024*1024*4] = {0};
	sprintf(command, "set %s %s", key.c_str(), value.c_str());
	string response;
	if( !(ExecuteCmd(command, response) && strcmp(response.c_str(), "OK")==0) )
	{
		return false;
	}
		
	return true;

}
bool CRedisOperator::getValue(string & key,string & value)
{
	char command[1024] = {0};
	sprintf(command, "get %s", key.c_str());
	if(!(ExecuteCmd(command, value) && value!= ""))
	{
		return false;
	}
		
	return true;
   
}  

bool CRedisOperator::setExpireTime(string & key,string secTime)
{
	char command[1024] = {0};
	sprintf(command, "expire %s %s", key.c_str(), secTime.c_str());
	string response;
	if(!(ExecuteCmd(command, response) && strcmp(response.c_str(), "OK")==0))
	{
		return false;
	}
	
	return true;
   
}



redisReply* CRedisOperator::ExecuteCmd(const char *cmd)
{
	redisContext *ctx = CreateContext();
    if(ctx == NULL) return NULL;

    redisReply *reply = (redisReply*)redisCommand(ctx, cmd);

    ReleaseContext(ctx, reply != NULL);

    return reply;
}   
bool CRedisOperator::ExecuteCmd(const char *cmd, string &response)
{
	redisReply *reply = ExecuteCmd(cmd);
    if(reply == NULL) return false;

    if(reply->type == REDIS_REPLY_INTEGER)
    {
		stringstream ss;
		ss << reply->integer;
		ss >> response;
		freeReplyObject(reply);
        return true;
    }
    else if(reply->type == REDIS_REPLY_STRING)
    {
        response.assign(reply->str, reply->len);
		freeReplyObject(reply);
        return true;
    }
    else if(reply->type == REDIS_REPLY_STATUS)
    {
        response.assign(reply->str, reply->len);
		freeReplyObject(reply);
        return true;
    }
    else if(reply->type == REDIS_REPLY_NIL)
    {
        response = "";
		freeReplyObject(reply);
        return true;
    }
    else if(reply->type == REDIS_REPLY_ERROR)
    {
        response.assign(reply->str, reply->len);
		freeReplyObject(reply);
        return false;
    }
    else if(reply->type == REDIS_REPLY_ARRAY)
    {
        response = "Not Support Array Result!!!";
		freeReplyObject(reply);
        return false;
    }
    else
    {     
        response = "Undefine Reply Type";
		freeReplyObject(reply);
        return false;
    }

}

redisContext* CRedisOperator::CreateContext()
{
	{
        if(!_clients.empty())
        {
        	_lock.Lock();
            redisContext *ctx = _clients.front();
            _clients.pop();
			_lock.unLock();

            return ctx;
        }
    }

	time_t now = time(NULL);
    if(now < _beginInvalidTime + _maxReconnectInterval) return NULL;

    struct timeval tv;
    tv.tv_sec = _iTimeout;
    tv.tv_usec = 0;
    redisContext *ctx = redisConnectWithTimeout(_strIp.c_str(), _iPort, tv);
    if(ctx == NULL || ctx->err != 0)
    {
        if(ctx != NULL) redisFree(ctx);

        _beginInvalidTime = time(NULL);
        
        return NULL;
    }

    return ctx;

}

void CRedisOperator::ReleaseContext(redisContext *ctx, bool active)
{
	if(ctx == NULL) return;
    if(!active) {redisFree(ctx); return;}

    _lock.Lock();
    _clients.push(ctx);
	_lock.unLock();

}

bool CRedisOperator::CheckStatus(redisContext *ctx)
{
	redisReply *reply = (redisReply*)redisCommand(ctx, "ping");
    if(reply == NULL) return false;


    if(reply->type != REDIS_REPLY_STATUS)  goto _false;
    if(strcmp(reply->str,"PONG") != 0)     goto _false;

	freeReplyObject(reply);
    return true;

	_false:
		freeReplyObject(reply);
		return false;

}
