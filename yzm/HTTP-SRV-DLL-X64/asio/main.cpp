#include <boost/asio.hpp>  
#include <boost/bind.hpp>  
#include <iostream>  
#include <string>  
#include <thread>
#include <chrono>
#include "AsyncThreadPool.h"

using namespace boost::asio;
using namespace std;

void PrintSomething(const std::string& msg) {
	std::cout << msg << '\n';
}

void PrintHello() {
	std::cout << "Hello\n";
}


class CT
{
public:
	CT() = default;
	~CT() = default;
	void httpCallBack(string&  response);

	void  start();

private:


};

void CT::httpCallBack(string&  response){

	std::this_thread::sleep_for(chrono::seconds(10));
	cout << "response : " << response << endl;

}


int main() {
// 	io_service service;
// 
// 	service.post(PrintHello);
// 
// 	service.post(boost::bind(PrintSomething, "whoami"));
// 
// 	CT ct;
// 	string name = "chenzixun";
// 	auto   callBack_fun = std::bind(&CT::httpCallBack, &ct, name);
// 
// 	service.post(callBack_fun);
// 
// 	service.run();

	AsyncThreadPool					ProcessPool;
	ProcessPool.StartAsync(16);

	for (int i = 0; i < 64;i++)
	{
		CT ct;
		string name = "chenzixun" + to_string(i);
		auto   callBack_fun = std::bind(&CT::httpCallBack, &ct, name);
		ProcessPool.Post(callBack_fun);
	}

	cout << "post end" << endl;

	return 0;
}