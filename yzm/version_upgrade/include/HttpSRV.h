#ifndef __HTTP_SRV_H__
#define __HTTP_SRV_H__



#ifdef __cplusplus //兼容纯C的调用
	extern "C"{
#endif

/*
接收数据结构
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
response数据的函数类型
*/
typedef int (*ResponseCB)(const void* const _cbThis, const char* data);

/*
request数据返回类型
*/
typedef int (*RequestCB)(const RecvInfo* pRecvInfo, ResponseCB respCB, const void* const _cbThis, void* arg);

/*
初始化函数接口
*/
HttpSRV* HttpSRV_init();


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
int HttpSRV_addRequestCB(HttpSRV* http, const char* uri, RequestCB requestCB,void* arg);

/*
开始服务
*/
int HttpSRV_start(HttpSRV* http, int port);

/*
关闭服务，并清理数据
*/
int HttpSRV_destory(HttpSRV* http); 

#ifdef __cplusplus
	};
#endif

#endif