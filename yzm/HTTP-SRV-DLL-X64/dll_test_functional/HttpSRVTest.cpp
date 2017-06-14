#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <conio.h>
#include<map>
#include <memory>
#include <thread>
#include <chrono>
#include "singleton.h"

#include "..\HttpServer\HttpTool.h"

using namespace std;

#pragma   comment(lib, "HttpTool.lib")

class CT
{
public:           
	CT() = default;
	~CT() = default;
	int httpCallBack(const RecvInfo& pRecvInfo, string&  response);

	void  start();

private:


};

int CT::httpCallBack(const RecvInfo& pRecvInfo, string&  response){
	
	cout << "CT::httpCallBack  receive length :  " << pRecvInfo.data_.length() << endl;

	vector<char>  cVec;
	cVec.resize(1024 * 10, 'r');
	response.resize(1024 * 10);
	std::copy(cVec.begin(), cVec.end(), &response[0]);

	return 0;
}

void  CT::start(){
	HttpSRV *http = HttpSRV_init();

	auto   callBack_fun = bind(&CT::httpCallBack, this, std::placeholders::_1, std::placeholders::_2);
	HttpSRV_addRequestCB(http, "/test", callBack_fun);

	HttpSRV_start(http, 2020);   

}

int httpCallBack2(const RecvInfo& pRecvInfo, string&  response){
	
	cout << "httpCallBack2 receive : " << pRecvInfo.data_ << endl;
	cout << "from  : " << pRecvInfo.client_ip_ << endl;

	response = "hello";

	return 0;
}

void  test_exception(){
	throw exception("test");
}

int main()
{ 
	CT  ct;
	ct.start();
	////////////////////////////////////////////
	std::vector<std::thread>  tvec;
	HttpClient  httpCli;
	for (int i = 0; i < 10; i++)
	{
		tvec.emplace_back([&](){   
			std::string  send;
			std::vector<char>  vecSend;
			vecSend.resize(1024*1024* 10, 'c');
			send.resize(1024 * 1024* 10);
			std::copy(vecSend.begin(), vecSend.end(), &send[0]);
			std::string  resp;
			std::string  error;    
			std::string  uri = "http://127.0.0.1:2019/test";
			httpCli.send(uri, send, resp, error);          
			std::cout << "resp length : " << resp.length() << std::endl << "error : " << error << std::endl;
		});
	}
	for (auto& th : tvec)      
	{
		th.join();
	}

// 	try   
// 	{
// 		test_exception();
// 	}
// 	catch (exception& e)   
// 	{
// 		cout << "catch exception" << endl;
// 	}

	while (_getch() != 'q');
	return 0;
                                                                                                    
}