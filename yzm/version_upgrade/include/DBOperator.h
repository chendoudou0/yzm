#ifndef CDBOperator_H_
#define CDBOperator_H_

#include "Common.h"
#include "sqlapi.h"
#include "def.h"

#include<memory>
#include <string>
#include<vector>
#include<utility>

struct ANDROID_VERSION_INFO{
	std::string name;
	std::string domain;
	std::string bucket;
	std::string path;
	std::string md5;
	std::string size;
	std::string description;
	int version_branch{0};
};

struct LIVESHOW_ADD_INFO{
	std::string name;
	std::string domain;
	std::string bucket;
	std::string description;
	std::vector<std::pair<std::string, std::string> >   fileVec_;
};

class CDBOperator
{
public:
    CDBOperator();
    virtual ~CDBOperator();
	
public:
	bool initMYSQL();  
	bool QueryExistingVersions(int type, SqlMapVector &objOutMapVector);
	bool QueryAndroidExistingVersions(SqlMapVector &objOutMapVector);
	bool QueryNewaddedVersions(SqlMapVector &objOutMapVector, std::string type);  
	bool QueryAndroidNewaddedVersions(SqlMapVector &objOutMapVector);
	bool QueryMd5s(int type, std::string version, KeyValueMap& outMap);  
	bool insertMD5toDB(SqlMapVector& inMapVec, std::string type);
	bool QueryBucketAndDomain(int type ,std::string& inVersion, std::string& outBucket, std::string& outDomain); 
	bool QueryAndroidBucketAndDomain(std::string& inVersion, std::string& outBucket, std::string& outDomain);
	bool UpdateVersionStatus(std::string& inVersionName, std::string type);
	bool UpdateAndroidVersionStatus(std::string& inVersionName, std::string& inMd5, int& inSize);
	int  AddAndroidVersion(ANDROID_VERSION_INFO& avInfo);
	int  AddLiveshowVersion(LIVESHOW_ADD_INFO&  lsInfo);
	
private:
	std::shared_ptr<CMySQL>  _ptrMysql;
 
};

#endif 