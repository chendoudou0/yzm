#include "HttpTool.h"

/*
设计考虑
1、线程安全：存在两个全局变量 gCBMap,gHttpManager,一般只是在初始化的时候用
	不涉及到读写问题
2、uri对应不同的函数
3、所有的内存，必须在main函数结束全部释放，不借助系统结束进程。

*/
#include <map>
#include <string>
#include <mutex>
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

//////////////////////////////////////client/
class HttpClient::Imp
{
public:
	Poco::Net::HTTPClientSession session_;
	Poco::URI uri_;
	std::string  path_;
	Poco::Net::HTMLForm form_;
	std::mutex  mtx_;

};

HttpClient::HttpClient() 
{
	ImpUPtr_.reset(new Imp);
}

HttpClient::~HttpClient()
{
}

void HttpClient::set_uri(const std::string& uri){
	ImpUPtr_->uri_ = uri;
	ImpUPtr_->path_ = ImpUPtr_->uri_.getPathAndQuery();
	if (ImpUPtr_->path_.empty()){
		ImpUPtr_->path_ = "/";
	}
	
	ImpUPtr_->session_.setHost(ImpUPtr_->uri_.getHost());
	ImpUPtr_->session_.setPort(ImpUPtr_->uri_.getPort());
}

bool  HttpClient::send(std::string& uri, std::string& strSend, std::string& strResponse, std::string&  error){
	try
	{
		std::lock_guard<std::mutex>  lk(ImpUPtr_->mtx_);

		ImpUPtr_->session_.reset();

		set_uri(uri);

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
//////////////////////////////////////client


/*
uri - > callback 映射部分      
*/

using  CBMap = map<string, RequestCB_fun>;

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

		if (it->second != nullptr) {  
			RecvInfo recvInfo;
			StreamCopier::copyToString(istr, recvInfo.data_);

			recvInfo.client_ip_ = request.clientAddress().toString();
			recvInfo.uri_ = request.getURI();
			
			string response_data;

			int ret = it->second(recvInfo, response_data);

			ostr << response_data;
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
	HttpSRV* http = new HttpSRV;

	http->srv = NULL;    
	http->mapData = new CBMap;

	return http;
}

int HttpSRV_addRequestCB(HttpSRV* http, const char* uri, RequestCB_fun requestCB)
{
	CBMap& cbMap = *(CBMap*)(http->mapData);
	if (cbMap.find(uri) != cbMap.end()) {
		return -1;
	}
	cbMap[uri] = requestCB;       
	return 0;
}

int HttpSRV_start(HttpSRV* http, int port)
{
	ServerSocket svs(port);                  
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setMaxQueued(100);  
	pParams->setMaxThreads(16);
	http->srv = new HTTPServer(new HTTPRequestHandlerFactory_((CBMap*)(http->mapData)), svs, pParams);
	((HTTPServer*)(http->srv))->start();

	return 0;      
}

int HttpSRV_destory(HttpSRV* http)
{
	if (http->srv)
	{
		((HTTPServer*)(http->srv))->stop();
	}
	
	if (http != NULL) {
		if (http->mapData != NULL) {
			delete http->mapData;
			http->mapData = NULL;
		}
		if (http->srv!= NULL) {
			delete http->srv;  
			http->srv = NULL;
		}

		delete http;
		http = NULL;
	}
	
	return 0;
}
