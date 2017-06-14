#ifndef __HTTP_SRV_H__
#define __HTTP_SRV_H__

#ifdef HTTPSRV_EXPORTS
	#define HTTPSRV_API	 __declspec(dllexport)
#else
	#define HTTPSRV_API  __declspec(dllimport)
#endif

#include <memory>
#include <string>

//////////////////////////////////////////////////////////////////////client
class HTTPSRV_API HttpClient
{
	class Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:
	HttpClient();
	~HttpClient();
	void  set_uri(const std::string& uri);

	bool  send(std::string& strSend, std::string&  strResponse, std::string&  error);

private:

};
//////////////////////////////////////////////////////////////////////client

#ifdef __cplusplus //���ݴ�C�ĵ���
	extern "C"{
#endif

/*
�������ݽṹ
*/
struct RecvInfo {
	char* ip;
	char* uri;
	char* data;
	int dLen;
};

struct HttpSRV {
	void* srv;
	void* mapData;
};

/*
response���ݵĺ�������
*/
typedef int (*ResponseCB)(const void* const _cbThis, const char* data);

/*
request���ݷ�������
*/
typedef int (*RequestCB)(const RecvInfo* pRecvInfo, ResponseCB respCB, const void* const _cbThis, void* arg);

/*
��ʼ�������ӿ�                                                  
*/
HTTPSRV_API HttpSRV* HttpSRV_init();


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
HTTPSRV_API int HttpSRV_addRequestCB(HttpSRV* http, const char* uri, RequestCB requestCB, void* arg);

/*
��ʼ����
*/
HTTPSRV_API int HttpSRV_start(HttpSRV* http, int port);

/*
�رշ��񣬲���������
*/
HTTPSRV_API int HttpSRV_destory(HttpSRV* http);           

#ifdef __cplusplus
	};
#endif



#endif