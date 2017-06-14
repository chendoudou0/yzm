#include <iostream>
#include <string>
#include "HttpSRV.h"
#include <stdio.h>
#include <string.h>
#include<unistd.h>


#include <sys/types.h>  // 基本数据类型
#include <curl/curl.h>


using namespace std;





int OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{  
    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);  
    if( NULL == str || NULL == buffer )  
    {  
        return -1;  
    }  
  
    char* pData = (char*)buffer;  
    str->append(pData, size * nmemb);  

	cout<<strlen(pData)<<endl;


    return nmemb;  
} 


bool postUrl(char *filename)
{
	string strResponse;
    CURL *curl;
    CURLcode res;
    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL)
        return false;
    curl = curl_easy_init();
    if (curl)
    {
    	
       //curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt"); // 指定cookie文件

	   cout<<"00000000000000000"<<endl;

	   char buf1[1024*1024*1] = {0};
	   cout<<"1111111111111"<<endl;
	   int j = 0;
	   for(j= 0; j<1024*1024*20 -1 ;j++)
	   {
			buf1[j] = 'c';
	   }

	   
       char buf[1024*1024*6] = {0};
	   cout<<"1111111111111"<<endl;
	   int i = 0;
	   for(i= 0; i<10 -1 ;i++)
	   {
			buf[i] = 'c';
	   }

	    cout<<"22222222222222"<<endl;  
		
		//curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); 

		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);    // 指定post内容
        cout<<"3333333333333333"<<endl;
        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:1998/test");   // 指定url
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse); 
       // curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		 cout<<"444444444444444444444"<<endl;
        res = curl_easy_perform(curl);
		cout<<"55555555555555555"<<endl;  

		cout<<strResponse<<endl;
        curl_easy_cleanup(curl);
    }
    fclose(fp);
    return true;
}

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

	postUrl("./html");

	respCB(_cbThis, respon.c_str());


	return 0;       
}

int callback1(const RecvInfo* pRecvInfo, ResponseCB respCB,  void* const _cbThis, void* arg)
{
	cout << "callback1 receive data: " << endl;

	if(pRecvInfo->data)
	{
		//cout << pRecvInfo->data << endl;

		int  size = strlen(pRecvInfo->data);

		cout << "size:" <<size<<endl;
	}

	//string respon("response data from callback1: ");

	//sleep(300);

	char buff[10] = {0};
	int i = 0;
	for(i = 0; i <10 -1;i++)
	{
		buff[i] = 'x';
	}
	
	string respon = buff;
	respCB(_cbThis, respon.c_str());  



	return 0;
}

int main()
{
	HttpSRV *http = HttpSRV_init();                            
	HttpSRV_addRequestCB(http, "/test",  (RequestCB)callback,   NULL);
	HttpSRV_addRequestCB(http, "/test1", (RequestCB)callback1,   NULL);
	HttpSRV_start(http, 2017);                                   
                                            
	while (getchar() != 'q');
	HttpSRV_destory(http);                            
	return 0;
	
}
