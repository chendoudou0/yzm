#include <boost/asio.hpp>  
#include <boost/bind.hpp>  
#include <iostream>  
#include <string>  
#include <thread>
#include <chrono>
#include "AsyncThreadPool.h"

using namespace boost::asio;
using namespace std;

void PrintSomething( std::string& msg) {
	std::cout << "PrintSomething : "<<msg << '\n';
	msg = "22222222222222";
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
	
	cout << "response : " << response << endl;

	response = "22222222 \n\n";

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
	string name = "chenzixun";
	for (int i = 0; i < 64;i++)
	{
		CT ct;
		string name = "chenzixun" + to_string(i);
		auto   callBack_fun = std::bind(&CT::httpCallBack, &ct, name);
		ProcessPool.Post(callBack_fun);

		auto   print_fun = std::bind(&PrintSomething, name);
		ProcessPool.Post(print_fun);
		this_thread::sleep_for(chrono::seconds(6));
		cout << name;
	}

	cout << "post end" << endl;

	return 0;
}