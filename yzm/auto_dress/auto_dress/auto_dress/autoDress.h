#ifndef _AUTO_DRESS_SERVER_H
#define _AUTO_DRESS_SERVER_H

#include"HttpTool.h"

#include <string>
#include <memory>

const std::string  TASK_ADD_URI   = "/auto_dress/task_begin";
const std::string  TASK_QUERY_URI = "/auto_dress/task_query";

class CAutoDressServer
{
	class Imp;
	std::unique_ptr<Imp>   impPtr_;

public:
	CAutoDressServer();
	virtual  ~CAutoDressServer();

public:
	bool Start();

	void Stop();

	int  httpCallBack(const RecvInfo& pRecvInfo, std::string&  response);



};

#endif


