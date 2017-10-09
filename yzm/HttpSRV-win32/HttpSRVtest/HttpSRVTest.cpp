#include <iostream>
#include <string>
#include <vector>  
#include "..\HttpSRV\HttpSRV.h"
# include <conio.h>

using namespace std;

int callback(const RecvInfo* pRecvInfo, ResponseCB respCB, const void* const _cbThis, void* arg)
{
	cout << "callback receive data: " << endl;
	if (pRecvInfo->data) 
	{
		cout << pRecvInfo->data << endl;
	} 
	else 
	{
		cout << "no data" << endl;                                                                                       
	}

	string respon("response data from callback: ");                                                          

	if (pRecvInfo->data)      
	{
		respon += pRecvInfo->data;                  
	}

	respCB(_cbThis, respon.c_str());

	return 0;       
}

int callback1(const RecvInfo* pRecvInfo, ResponseCB respCB, const void* const _cbThis, void* arg)
{
	cout << "callback1 receive data: " << endl;
	if(pRecvInfo->data)
	{
	
		cout << pRecvInfo->data << endl;
	}
	         

	string respon("response data from callback1: ");
	respon += pRecvInfo->data;
	respCB(_cbThis, respon.c_str());                                                                                                   
	return 0;         

}

int main()
{
	HttpSRV *http = HttpSRV_init();                  
	HttpSRV_addRequestCB(http, "/test",  (RequestCB)callback,    "usr");
	HttpSRV_addRequestCB(http, "/test1", (RequestCB)callback1,  NULL);
	HttpSRV_start(http, 1998);                       
	
	while (_getch() != 'q');
	HttpSRV_destory(http);                            
	return 0;  
                        
	
	                                                                                    
}