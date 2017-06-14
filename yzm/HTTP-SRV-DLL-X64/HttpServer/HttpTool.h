#ifndef __HTTP_TOOL_H__
#define __HTTP_TOOL_H__

#ifdef HTTPSERVER_EXPORTS
#define HTTPTOOL_API  __declspec(dllexport)
#else
#define HTTPTOOL_API  __declspec(dllimport)
#endif

#include <functional>
#include <string>
#include <memory>

/*
接收数据结构
*/
struct RecvInfo {
	std::string	  client_ip_;
	std::string   uri_;
	std::string   data_;
};

struct HttpSRV {
	void* srv{NULL};
	void* mapData{NULL};
};

/*
request数据返回类型
*/
using  RequestCB_fun = std::function <int(const RecvInfo&, std::string&)> ;
/*
初始化函数接口                                                  
*/
HTTPTOOL_API HttpSRV* HttpSRV_init();

/*
增加回调函数接口
说明:每个uri对应一个回调函数，uri不能重复，否则增加不成功
参数说明:
1、http，固定参数
2、uri，uri，http://127.0.0.1:1998/test  (/test就是uri)
3、requestCB，返回数据接口，一般使用
   requestCB(cbConstArg, data)
4、arg，回调的时候的参数指针

返回值:
 <0 : 失败。uri重复   
>=0 ：成功     
*/

HTTPTOOL_API int HttpSRV_addRequestCB(HttpSRV* http, const char* uri, RequestCB_fun requestCB);

/*
开始服务
*/
HTTPTOOL_API int HttpSRV_start(HttpSRV* http, int port);

/*
关闭服务，并清理数据
*/
HTTPTOOL_API int HttpSRV_destory(HttpSRV* http);

/////////////////////////////////////////////////////////////////CLIENT
class HTTPTOOL_API HttpClient
{
	class Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:
	HttpClient();
	~HttpClient();
	void  set_uri(const std::string& uri);

	bool  send(std::string& uri, std::string& strSend, std::string&  strResponse, std::string&  error);

private:

};


#endif