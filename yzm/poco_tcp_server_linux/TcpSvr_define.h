#ifndef _TCPSVR_DEFINE_H
#define _TCPSVR_DEFINE_H


#include <map>
#include<string.h>

#if !defined(uint32)
typedef unsigned int uint32;
#endif

//���Ӷ���
typedef struct _connInfo
{
	uint32 fd;	//�ͻ���Ψһ��ʶ
	char ip[32];
	int port;
	void* clMgr;
	_connInfo(){memset(this, 0, sizeof(*this));  clMgr=NULL;}
}ConnInfo;


//��������Ϣ�ص�����
typedef void (*fConnectCallBack)(ConnInfo& info, void* user);
typedef void (*fCloseCallBack)(ConnInfo& info, void* user);
typedef int (*fDataCallBack)(uint32 fd, const char* data, int len, void* user);

#endif