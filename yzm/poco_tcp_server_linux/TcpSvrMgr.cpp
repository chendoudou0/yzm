#include "TcpSvrMgr.h"
#pragma warning(disable:4996)

CTcpSvrMgr::CTcpSvrMgr( void )
{
	_cbConnect = NULL;
	_cbClose = NULL;
	_cbData = NULL;
	_user = NULL;
	_id = 0;
}

CTcpSvrMgr::~CTcpSvrMgr()
{

}

int CTcpSvrMgr::startListen( int port)
{
	printf("CTcpSvrMgr::startListen port:%d  \n", port);

	ServerSocket serverSocket(port);
	//SocketReactor reactor;
	SocketAcceptor<ClientHandler> acceptor(serverSocket, _reactor);
	_reactor.run();         
	
	return 0;
}

void CTcpSvrMgr::stopServer()  
{
	_reactor.stop();
}

int CTcpSvrMgr::onConnect( ConnInfo& info )
{
	printf("CTcpSvrMgr::onConnect [%d-%s:%d]\n", info.fd, info.ip, info.port);
	
	pthread_mutex_lock(&_mtxConn);
	ConnMap::iterator it = _connMap.find(info.fd);
	if ( it == _connMap.end() )
		_connMap.insert(std::make_pair(info.fd, (ClientHandler*)info.clMgr));
	pthread_mutex_unlock(&_mtxConn);

	if ( NULL != _cbConnect )
	{
		_cbConnect(info, _user);      
	}
	
	return 0;
}

int CTcpSvrMgr::onClose( ConnInfo& info )
{
	printf("CTcpSvrMgr::onClose(), fd=%d\n", info.fd);
	
	pthread_mutex_lock(&_mtxConn);
	ConnMap::iterator it = _connMap.find(info.fd);
	if ( it != _connMap.end() )
	{
		_connMap.erase(it);
	}
	pthread_mutex_unlock(&_mtxConn);
	
	if ( NULL != _cbClose)
	{
		_cbClose(info, _user);   
	}
	return 0;
}

int CTcpSvrMgr::onData( ConnInfo& info, const char* buf, const uint32 len)
{
	printf("CTcpSvrMgr::onData(), fd=%d, len=%d\n", info.fd, len);
	if ( NULL != _cbData )
	{
		return _cbData(info.fd, buf, len, _user);
	}
	return 0;
}

int CTcpSvrMgr::sendData( uint32 fd, const char* data, int len )
{
	printf("CTcpSvrMgr::sendData(), fd=%d, len=%d\n", fd, len);
	int ret = -1;
	
	pthread_mutex_lock(&_mtxConn);
	ConnMap::iterator it = _connMap.find(fd);
	if ( it != _connMap.end() )
	{
		ClientHandler* clMgr = it->second;
		if ( NULL != clMgr )
		{
			ret = clMgr->sendData(data, len);
		}
	}
	pthread_mutex_unlock(&_mtxConn);
	
	return ret;
}

void CTcpSvrMgr::closeConn( uint32 fd )
{
	printf("CTcpSvrMgr::closeConn(), fd=%d\n", fd);
	
	pthread_mutex_lock(&_mtxConn);
	ConnMap::iterator it = _connMap.find(fd);
	if ( it != _connMap.end() )
	{
		_connMap.erase(it);  
		delete it->second;
	}   
	pthread_mutex_unlock(&_mtxConn);
	
}

uint32 CTcpSvrMgr::getNextFd( void )
{
	pthread_mutex_lock(&_mtxId);
	uint32 nextId = _id ++;
	if ( -1 == _id )
		_id = 0;
	pthread_mutex_unlock(&_mtxId);
	return nextId;
}

