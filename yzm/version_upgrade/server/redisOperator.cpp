#include"redisOperator.h"
#include"glog/logging.h"

#include <sstream>

using  namespace std;

CRedisOperator::CRedisOperator(string ip, int port, int timeout )
:_strIp(ip),_iPort(port),_iTimeout(timeout),
_beginInvalidTime(0)
{

}
CRedisOperator::~CRedisOperator()
{
	std::lock_guard<mutex>  lk(_lock);
    while(!_clients.empty())
    {
        redisContext *ctx = _clients.front();
        redisFree(ctx);
        _clients.pop();
    }

}

bool CRedisOperator::setValue(string & key,string & value)
{
    std::stringstream  ss;
    ss << "set " <<key << "  "<<value;
    string cmd;
    cmd = ss.str();
	string response;
	if( !(ExecuteCmd(cmd.c_str(), response) && response == "OK") )
	{
		return false;
	}
		
	return true;

}
bool CRedisOperator::getValue(string & key,string & value)
{
	string cmd;
    stringstream ss;
    ss <<"get  "<<key;
    cmd = ss.str();

	if(!(ExecuteCmd(cmd.c_str(), value) && value!= ""))
	{
		return false;
	}
		
	return true;
}  

bool CRedisOperator::setExpireTime(string & key,string secTime)
{
	string cmd; 	
    string response;
    stringstream ss;
    ss <<"expire  "<<key << "   "<<secTime;
    cmd = ss.str();
	if(!(ExecuteCmd(cmd.c_str(), response) && response == "OK"))
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
        	_lock.lock();
            redisContext *ctx = _clients.front();
            _clients.pop();
			_lock.unlock();

            return ctx;
        }
    }

	time_t now = time(NULL);
    if(now < _beginInvalidTime + _maxReconnectInterval) return NULL;
    if(_clients.size() > _poolSize)   return nullptr;

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

    lock_guard<mutex>  lk(_lock);
    _clients.push(ctx);   
}   

bool CRedisOperator::CheckStatus(redisContext *ctx)
{
	redisReply *reply = (redisReply*)redisCommand(ctx, "ping");
    if(reply == NULL) return false;


    if(reply->type != REDIS_REPLY_STATUS)    goto _false;
    if(strcmp(reply->str,"PONG") != 0)       goto _false;

	freeReplyObject(reply);
    return true;

	_false:
		freeReplyObject(reply);
		return false;

}

void CRedisOperator::setPoolSize(int size)
{
    _poolSize = size;
}
