#ifndef _TCPSVR_MGR_H
#define _TCPSVR_MGR_H

#include <pthread.h>

#include "TcpSvr_define.h"
#include "singleton.h"
#include "ClientHandler.h"

typedef std::map<uint32, ClientHandler*> ConnMap;

class CTcpSvrMgr : public Singleton<CTcpSvrMgr>
{
	friend class Singleton<CTcpSvrMgr>;
private:
	CTcpSvrMgr(void);
	~CTcpSvrMgr();

public:
	int startListen(int port);
	void stopServer();
	int sendData(uint32 fd, const char* data, int len);
	void closeConn(uint32 fd);
	uint32 getNextFd(void);

public:
	virtual int onConnect(ConnInfo& info);
	virtual int onClose(ConnInfo& info);
	virtual int onData(ConnInfo& info, const char* buf, const uint32 len);

public:
	fConnectCallBack _cbConnect;
	fCloseCallBack _cbClose;
	fDataCallBack _cbData;
	void* _user;

private:
	SocketReactor _reactor;
	
	pthread_mutex_t _mtxId = PTHREAD_MUTEX_INITIALIZER;
	uint32 _id;

	pthread_mutex_t _mtxConn = PTHREAD_MUTEX_INITIALIZER;
	ConnMap _connMap;

};

#endif