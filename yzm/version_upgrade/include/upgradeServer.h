#ifndef _UPGRADE_SERVER_H
#define _UPGRADE_SERVER_H

#include"HttpTool.h"

#include <string>
#include <memory>

class CUpgradeServer 
{
	class Imp;
	std::unique_ptr<Imp>   impPtr_;
	
public:
  CUpgradeServer();
  virtual  ~CUpgradeServer();

public:
	bool Start();

	void Stop();

	int  httpCallBack(const RecvInfo& pRecvInfo, std::string&  response);

};

#endif


