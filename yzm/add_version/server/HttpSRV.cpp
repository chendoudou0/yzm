#include "HttpSRV.h"
#include<iostream>

/*
Éè¼Æ¿¼ÂÇ
1¡¢Ïß³Ì°²È«£º´æÔÚÁ½¸öÈ«¾Ö±äÁ¿ gCBMap,gHttpManager,Ò»°ãÖ»ÊÇÔÚ³õÊ¼»¯µÄÊ±ºòÓÃ
	²»Éæ¼°µ½¶ÁÐ´ÎÊÌâ
2¡¢uri¶ÔÓ¦²»Í¬µÄº¯Êý
3¡¢ËùÓÐµÄÄÚ´æ£¬±ØÐëÔÚmainº¯Êý½áÊøÈ«²¿ÊÍ·Å£¬²»½èÖúÏµÍ³½áÊø½ø³Ì¡£

*/

#include <map>
#include <string>
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/StreamCopier.h"

using namespace std;
using namespace Poco;
using namespace Net;

/*
uri - > callback Ó³Éä²¿·Ö
*/
class CallBackSrc {
public:
	CallBackSrc(void* iuser, RequestCB icallback)
		: user(iuser)
		, callback(icallback)
	{}
	void* user;
	RequestCB callback;
};
typedef map<string, CallBackSrc*> CBMap;


/*
responseÊý¾Ý²¿·Ö
*/
#ifdef __cplusplus
extern "C"{
#endif

struct ResponseData {
	char* data;
};

int responseCB(const void* const cbConstArg, const char* idata)
{
	struct ResponseData* resp = (struct ResponseData*)cbConstArg;
	if (idata != NULL) {
		int len = strlen(idata);
		if (len > 0) {
			resp->data = (char*)malloc(len + 1);
			strncpy(resp->data, idata, len);
			resp->data[len] = 0;
		}
		return 0;
	}
	return -1;
}

#ifdef __cplusplus
};
#endif


/*
poco http request ²¿·Ö
*/
class HTTPRequestHandler_: public HTTPRequestHandler
{
public:
	HTTPRequestHandler_(CBMap* cbMap)
		: _cbMap(cbMap) {}

	//´¦ÀíÒ»¸öÇëÇó
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
		std::istream& istr = request.stream();
		std::ostream& ostr = response.send();

		const std::string strURI = request.getURI();
		const std::string strMethod = request.getMethod();
		cout<<strURI<<endl<<strMethod<<endl;
		CBMap::iterator it = _cbMap->find(strURI);
		if (it == _cbMap->end()) {
			ostr << "uri is not exist!!";
			return;
		}

		if (it->second->callback != NULL) {
			string recvStr;
			StreamCopier::copyToString(istr, recvStr);

			//½ÓÊÕ
			RecvInfo recvInfo;
			recvInfo.ip = (char*)request.getHost().c_str();
			recvInfo.uri = (char*)request.getURI().c_str();
			if (recvStr.empty()) {
				recvInfo.data = NULL;
				recvInfo.dLen = 0;
			} else {
				recvInfo.data = (char*)recvStr.c_str();
				recvInfo.dLen = recvStr.size();  
			}
			
			ResponseData respData;// = (ResponseData*)malloc(sizeof(ResponseData));
			respData.data = NULL;

			int ret = it->second->callback(&recvInfo, responseCB, &respData, it->second->user);

			//response Êý¾ÝµÄÄÚ´æÊÍ·Å
			if (respData.data != NULL) {
				//response
				ostr << respData.data; 
				free(respData.data);
			}
		}
	}
private:
	CBMap* _cbMap;
};

class HTTPRequestHandlerFactory_ : public HTTPRequestHandlerFactory {
public:
	HTTPRequestHandlerFactory_(CBMap* cbMap) : _cbMap(cbMap) {}

	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) {
		return new HTTPRequestHandler_(_cbMap);
	}
private:
	CBMap* _cbMap;
};


//API
 HttpSRV* HttpSRV_init()
{
	HttpSRV* http = (HttpSRV*)malloc(sizeof(HttpSRV));
	if (http != NULL) {
		http->srv= NULL;
		http->mapData = new CBMap;
		return http;
	}
	return NULL;
}

int HttpSRV_addRequestCB(HttpSRV* http,const char* uri, RequestCB requestCB,void* arg)
{
	CBMap& cbMap = *(CBMap*)(http->mapData);
	if (cbMap.find(uri) != cbMap.end()) {
		return -1;
	}
	cbMap[uri] = new CallBackSrc(arg, requestCB);
	return 0;
}      

int HttpSRV_start(HttpSRV* http, int port)
{
	ServerSocket svs(port);
	http->srv = new HTTPServer(new HTTPRequestHandlerFactory_((CBMap*)(http->mapData)), svs, new HTTPServerParams);
	((HTTPServer*)(http->srv))->start();
	return 0;
}

int HttpSRV_destory(HttpSRV* http) 
{
	if (http != NULL) {
		if (http->mapData != NULL) {
			CBMap& cbMap = *(CBMap*)(http->mapData);
			for (CBMap::iterator it = cbMap.begin(); it != cbMap.end(); ++it) {
				delete it->second;
			}

			delete http->mapData;
			http->mapData = NULL;                       
		}
		if (http->srv!= NULL) {
			delete http->srv;
			http->srv = NULL;
		}
	}
	free(http);           
	http = NULL;
	return 0;
}
