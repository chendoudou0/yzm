#ifndef _UPGRADE_SERVER_H
#define _UPGRADE_SERVER_H

#include"HttpTool.h"

#include <string>
#include <memory>

class CPoseLabelServer 
{
	class Imp;
	std::unique_ptr<Imp>   impPtr_;
	
public:
  CPoseLabelServer();
  virtual  ~CPoseLabelServer();

public:
	bool Start();

	void Stop();

	int  httpCallBack(const RecvInfo& pRecvInfo, std::string&  response);

};

#endif


