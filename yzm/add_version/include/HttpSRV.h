#ifndef __HTTP_SRV_H__
#define __HTTP_SRV_H__



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
HttpSRV* HttpSRV_init();


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
int HttpSRV_addRequestCB(HttpSRV* http, const char* uri, RequestCB requestCB,void* arg);

/*
��ʼ����
*/
int HttpSRV_start(HttpSRV* http, int port);

/*
�رշ��񣬲���������
*/
int HttpSRV_destory(HttpSRV* http); 

#ifdef __cplusplus
	};
#endif

#endif