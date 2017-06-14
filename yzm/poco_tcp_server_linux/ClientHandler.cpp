#include "TcpSvrMgr.h"
#include "ClientHandler.h"

#include<iostream>
#include<arpa/inet.h>
#include<chrono>
#include<thread>


using namespace std;
using namespace chrono;

#define SAFE_STRCOPY(dst, src)	strncpy((dst), (src), sizeof(dst)-1);


////////////////////////////////////////////////////////////////////////////////////////////

/* ���� */
enum emTrpcConst
{
    MAXTIDENT = 32,
    MAX_SERVICE_NAME = 32,
    MAX_SERVER_NAME = 32,
    MAX_LOG_KEY_LEN = 64,           // ��־�е�key����󳤶�
    MAX_TRPC_ERRMSG_LEN = 128,
    MAX_TRPC_DATA_LEN = 32000,
    MAX_TRPC_PACKAGE_SIZE = 655360
};

bool _bInDataIsNew = true;
char _sInData[MAX_TRPC_PACKAGE_SIZE] = {0};
static const char PACK_STX = 0x02;
long _iInDataLen = 0;
int _iInLeftLen = 0;


inline void TRPC_GET_INT32(int& data, const void * src_buf)
{
    memcpy(&data, src_buf, sizeof(int));
	/////data = ntohl(data);
}
///////////////////////////////////////////////////////////////////////////////////



ClientHandler::ClientHandler(StreamSocket& socket, SocketReactor& reactor)
:_socket(socket),
_reactor(reactor),
_pBuffer(new char[BUFFER_SIZE])  
{
	//�յ�������
	_fd = CTcpSvrMgr::instance()->getNextFd();
	printf("ClientHandler::Connection[%u] from:%s\n", _fd, _socket.peerAddress().toString().c_str());
	ConnInfo info;
	info.fd = _fd;
	SAFE_STRCOPY(info.ip, _socket.peerAddress().host().toString().c_str());
	info.port = _socket.peerAddress().port();
	info.clMgr = this;
	CTcpSvrMgr::instance()->onConnect(info);

	_reactor.addEventHandler(_socket, NObserver<ClientHandler, ReadableNotification>(*this, &ClientHandler::onReadable));
	_reactor.addEventHandler(_socket, NObserver<ClientHandler, ShutdownNotification>(*this, &ClientHandler::onShutdown));
}

ClientHandler::~ClientHandler()
{
	//�Ͽ�����
	printf("ClientHandler::Disconnecting[%u] from:%s\n", _fd, _socket.peerAddress().toString().c_str());
	ConnInfo info;
	info.fd = _fd;
	SAFE_STRCOPY(info.ip, _socket.peerAddress().host().toString().c_str());
	info.port = _socket.peerAddress().port();
	CTcpSvrMgr::instance()->onClose(info);

	_reactor.removeEventHandler(_socket, NObserver<ClientHandler, ReadableNotification>(*this, &ClientHandler::onReadable));
	_reactor.removeEventHandler(_socket, NObserver<ClientHandler, ShutdownNotification>(*this, &ClientHandler::onShutdown));
	delete [] _pBuffer, _pBuffer = 0;
}

static int g_count = 0;
static steady_clock::time_point g_begin;
static steady_clock::time_point g_end;

void ClientHandler::onReadable(const AutoPtr<ReadableNotification>& pNf)
{
	int iBytes;
	int iLen = 0;
    if (_bInDataIsNew) 
	{
		iBytes = _socket.receiveBytes(_sInData, 5);
        if (iBytes == 0) 
		{
			cout<<"aaaaaaaaaaaaa"<<endl;
            return ;
        }
        else if (iBytes != 5) 
		{
            cout<<"bbbbbbbbbbb"<<iBytes<<endl;
            return ;
        }
        if (_sInData[0] != PACK_STX) 
		{
           cout<<"cccccccccc"<<endl;
            return ;
        }
        TRPC_GET_INT32(iLen, _sInData + 1);
//		cout<<"total len : "<<iLen<<endl;
        if (iLen < 10) 
		{
            return ;
        }
        else if (iLen > MAX_TRPC_PACKAGE_SIZE) 
		{
            return ;
        }
        _iInDataLen = iBytes;
        _iInLeftLen = iLen - iBytes;
        _bInDataIsNew = false;
    }

	iBytes = _socket.receiveBytes(_sInData + _iInDataLen, _iInLeftLen);

    if (iBytes == 0)
	{
        return ;
    }
    _iInDataLen += iBytes;
    _iInLeftLen -= iBytes;
    if (_iInLeftLen > 0) 
	{
		cout<<"have more!"<<endl;
        return ;
    }
	
    _bInDataIsNew = true;
//	cout << "one packet over!" << endl;
	memset(_sInData, 0, MAX_TRPC_PACKAGE_SIZE);

	if(g_count == 0){
		g_begin = steady_clock::now();
	}
	g_count++;
	if(g_count == 1024*100){                    //200M
		g_end  = steady_clock::now();
		double time = duration_cast<milliseconds>(g_end - g_begin).count();
		cout << "net speed : " << 200 *1000/time << "M/s" << endl << endl;
		g_count = 0;
	}
	
    return ;

	_error:
		delete this;                        
		return ;         


	int len = _socket.receiveBytes(_pBuffer, BUFFER_SIZE);
	printf("ClientHandler::onReadable[%u] len:%d\n", _fd, len);

	if ( 0 < len )
	{
		_pBuffer[len] = '\0';
		ConnInfo info;
		info.fd = _fd;
	    SAFE_STRCOPY(info.ip, _socket.peerAddress().host().toString().c_str());
		info.port = _socket.peerAddress().port();
		CTcpSvrMgr::instance()->onData(info, _pBuffer, len);
	}
	else
	{
		delete this;
	}
	
}

// When ShutdownNotification is detected, this method will be invoked.
void ClientHandler::onShutdown(const AutoPtr<ShutdownNotification>& pNf)
{
	delete this;
}

int ClientHandler::sendData( const char* data, int len )
{
	if ( NULL != data && 0 != data[0] && 0 < len )
	{
		return _socket.sendBytes(data, len);
	}
	
	return 0;
}
