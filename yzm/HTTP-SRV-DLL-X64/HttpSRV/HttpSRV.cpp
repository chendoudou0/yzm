#include "HttpSRV.h"

/*
设计考虑
1、线程安全：存在两个全局变量 gCBMap,gHttpManager,一般只是在初始化的时候用
	不涉及到读写问题
2、uri对应不同的函数
3、所有的内存，必须在main函数结束全部释放，不借助系统结束进程。

*/

#include <map>
#include <string>
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/StreamCopier.h"

//////////////////////////////////////client
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include <Poco/Net/HTTPCredentials.h>
#include <Poco/Net/HTMLForm.h>
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
//////////////////////////////////////client

using namespace std;
using namespace Poco;
using namespace Net;

class HttpClient::Imp
{
public:
	Poco::Net::HTTPClientSession session_;
	Poco::URI uri_;
	std::string  path_;
	Poco::Net::HTMLForm form_;

public:
	Imp(){};
	
};

HttpClient::HttpClient()
:ImpUPtr_(make_unique<Imp>())
{

}

HttpClient::~HttpClient()
{

}

void HttpClient::set_uri(const std::string& uri){
//	ImpUPtr_->session_.reset();

	ImpUPtr_->uri_ = uri;
	ImpUPtr_->path_ = ImpUPtr_->uri_.getPathAndQuery();
	if (ImpUPtr_->path_.empty()){
		ImpUPtr_->path_ = "/";
	}

	ImpUPtr_->session_.setHost(ImpUPtr_->uri_.getHost());
	ImpUPtr_->session_.setPort(ImpUPtr_->uri_.getPort());
}

bool  HttpClient::send(std::string& strSend, std::string& strResponse, std::string&  error){
	try
	{
		ImpUPtr_->session_.reset();

		HTTPRequest request(HTTPRequest::HTTP_POST, ImpUPtr_->path_, HTTPMessage::HTTP_1_1);

		request.setContentLength((int)strSend.length());
		std::ostream& osSend = ImpUPtr_->session_.sendRequest(request);
		osSend << strSend << std::flush;

		Poco::Net::HTTPResponse response;
		std::istream& responseBody = ImpUPtr_->session_.receiveResponse(response);

		std::string responseText;
		StreamCopier::copyToString(responseBody, responseText);
		strResponse = responseText;
	}
	catch (Exception& exc)
	{
		error = exc.displayText();
		return  false;
	}


	return  true;

}



/*
uri - > callback 映射部分
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
response数据部分
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
poco http request 部分
*/
class HTTPRequestHandler_: public HTTPRequestHandler
{
public:
	HTTPRequestHandler_(CBMap* cbMap)
		: _cbMap(cbMap) {}

	//处理一个请求
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
		std::istream& istr = request.stream();
		std::ostream& ostr = response.send();

		const std::string strURI = request.getURI();
		CBMap::iterator it = _cbMap->find(strURI);
		if (it == _cbMap->end()) {
			//ostr << "uri is not exist!!";
			return;
		}  

		if (it->second->callback != NULL) {
			string recvStr;
			StreamCopier::copyToString(istr, recvStr);

			//接收
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
			
			ResponseData respData;
			respData.data = NULL;       

			int ret = it->second->callback(&recvInfo, responseCB, &respData, it->second->user);

			//response 数据的内存释放
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
HTTPSRV_API HttpSRV* HttpSRV_init()               
{
	HttpSRV* http = (HttpSRV*)malloc(sizeof(HttpSRV));
	if (http != NULL) {
		http->srv= NULL;
		http->mapData = new CBMap;
		return http;
	}
	return NULL;
}

HTTPSRV_API int HttpSRV_addRequestCB(HttpSRV* http, const char* uri, RequestCB requestCB, void* arg)
{
	CBMap& cbMap = *(CBMap*)(http->mapData);
	if (cbMap.find(uri) != cbMap.end()) {
		return -1;
	}
	cbMap[uri] = new CallBackSrc(arg, requestCB);
	return 0;
}

HTTPSRV_API int HttpSRV_start(HttpSRV* http, int port)
{
	ServerSocket svs(port);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setMaxQueued(100);
	pParams->setMaxThreads(16);
	http->srv = new HTTPServer(new HTTPRequestHandlerFactory_((CBMap*)(http->mapData)), svs, pParams);
	((HTTPServer*)(http->srv))->start();
	return 0;      
}

HTTPSRV_API int HttpSRV_destory(HttpSRV* http)                   
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
