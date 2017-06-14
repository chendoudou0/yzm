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
�������ݽṹ
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
request���ݷ�������
*/
using  RequestCB_fun = std::function <int(const RecvInfo&, std::string&)> ;
/*
��ʼ�������ӿ�                                                  
*/
HTTPTOOL_API HttpSRV* HttpSRV_init();

/*
���ӻص������ӿ�
˵��:ÿ��uri��Ӧһ���ص�������uri�����ظ����������Ӳ��ɹ�
����˵��:
1��http���̶�����
2��uri��uri��http://127.0.0.1:1998/test  (/test����uri)
3��requestCB���������ݽӿڣ�һ��ʹ��
   requestCB(cbConstArg, data)
4��arg���ص���ʱ��Ĳ���ָ��

����ֵ:
 <0 : ʧ�ܡ�uri�ظ�   
>=0 ���ɹ�     
*/

HTTPTOOL_API int HttpSRV_addRequestCB(HttpSRV* http, const char* uri, RequestCB_fun requestCB);

/*
��ʼ����
*/
HTTPTOOL_API int HttpSRV_start(HttpSRV* http, int port);

/*
�رշ��񣬲���������
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