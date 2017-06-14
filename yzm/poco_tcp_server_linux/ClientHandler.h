#ifndef _CLIENTHANDLER_H
#define _CLIENTHANDLER_H

#include "Poco.h"
class ClientHandler
{
public:
	ClientHandler(StreamSocket& socket, SocketReactor& reactor);
	~ClientHandler();

	int sendData(const char* data, int len);

public:
	void onReadable(const AutoPtr<ReadableNotification>& pNf);
	void onShutdown(const AutoPtr<ShutdownNotification>& pNf);

private:
	enum
	{
		BUFFER_SIZE = 1024
	};
	StreamSocket _socket;
	SocketReactor& _reactor;
	char *_pBuffer;

	//ÊôÐÔ×Ö¶Î
	unsigned int _fd;
	std::string _ip;
	int _port;
};

#endif