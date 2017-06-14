#include <time.h>
#include <string>
#include <iostream>
#include "TcpSvrMgr.h"



void TR_SetCallBack( fConnectCallBack cbConnect, fCloseCallBack cbClose, fDataCallBack cbData, void* user )
{
	CTcpSvrMgr::instance()->_cbConnect = cbConnect;
	CTcpSvrMgr::instance()->_cbClose = cbClose;
	CTcpSvrMgr::instance()->_cbData = cbData;
	CTcpSvrMgr::instance()->_user = user;
}

int TR_Start( int port )
{
	CTcpSvrMgr::instance()->startListen(port);
	return 0; 
}

void TR_Stop()
{
	CTcpSvrMgr::instance()->stopServer();
}

int TR_SendData( uint32 fd, const char* data, int len )
{
	return CTcpSvrMgr::instance()->sendData(fd, data, len);
}

void TR_CloseConn( uint32 fd )
{
	CTcpSvrMgr::instance()->closeConn(fd);
}

void  onConnect(ConnInfo& info, void* user)
{
	printf("onConnect:%u addr=%s:%d \n", info.fd, info.ip, info.port);
	printf("----------------------------------------------\n");
}

void onClose(ConnInfo& info, void* user)
{
	printf("onClose:%u addr=%s:%d \n", info.fd, info.ip, info.port);
	printf("----------------------------------------------\n");
}

int onRecvData(uint32 fd, const char* data, int len, void* user)
{
	printf("onRecvData:Recv From:%u len=%d\n", fd, len);
	char* rvData = new char[len];
	memset(rvData, 0, len);
	strncpy(rvData, data, len);
	//rvData[len] = '\0';
	printf("-------------        data      ---------------\n");
	printf("%s\n", rvData);
	printf("----------------------------------------------\n");
	TR_SendData(fd, rvData, len);
	delete[] rvData;
	return len;
}

int  main()
{
	TR_SetCallBack(onConnect, onClose, onRecvData, NULL);
	TR_Start(6888);
	printf("StartListen:9999  \n");

	std::string input;
	std::cin>>input;
	while( std::string("q") != input )
	{
		std::cin>>input;
	}

	printf("StopServer\n");
	TR_Stop();
	return 0;
}

