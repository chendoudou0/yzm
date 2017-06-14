#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "..\HttpSRV\HttpSRV.h"
#include <conio.h>
#include<windows.h>
#include<map>
#include <memory>

using namespace std;


int callbackForPHP(const RecvInfo* pRecvInfo, ResponseCB respCB, const void* const _cbThis, void* arg)
{
	string response = "hello";
	

	if (pRecvInfo->data) 
	{
		cout << "receive  :" << pRecvInfo->dLen << endl;
	} 
	else 
	{
		cout << "no data" << endl;
		return -1;
	}

	vector<char>  vecSend;
	vecSend.resize(1024 * 1024, 'r');
	response.resize(vecSend.size());
	std::copy(vecSend.begin(), vecSend.end(), &response[0]);
                                          
	respCB(_cbThis, response.c_str());

	return 0;       
}


int main()
{ 
	
	HttpSRV *http = HttpSRV_init();                  
	HttpSRV_addRequestCB(http, "/test",  (RequestCB)callbackForPHP, NULL);
	HttpSRV_start(http,  2019);  

	string  send;
	vector<char>  cVec;
	cVec.resize(1024 * 1024, 't');
	send.resize(1024 * 1024);
	std::copy(cVec.begin(), cVec.end(), &send[0]);
	for (int i = 0; i < 5; i++)
	{
		HttpClient  httpCli;
		string  resp;
		string  error;
		httpCli.set_uri("http://127.0.0.1:2020/test");
		httpCli.send(send, resp, error);

		cout << "resp  : " << resp.length() << endl << "error : " << error << endl;
	}

	while (_getch() != 'q');
	HttpSRV_destory(http);        

	return 0;  
                                                                                                         
}