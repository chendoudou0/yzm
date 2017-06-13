#ifndef CDBOperator_H_
#define CDBOperator_H_

#include <string>
#include<memory>

#include "sqlapi.h"

using namespace std;

class CDBOperator
{
public:
    CDBOperator(DB_PARAM  db_pa);
    virtual ~CDBOperator() = default;
	void initMYSQL();  
	void QueryAndroidExistingVersions(SqlMapVector &objOutMapVector);
	/*
	void QueryExistingVersions(int type, SqlMapVector &objOutMapVector)  throw (CException);
	void QueryNewaddedVersions(SqlMapVector &objOutMapVector, string type)  throw (CException);
	void QueryAndroidNewaddedVersions(SqlMapVector &objOutMapVector)  throw (CException);
	void QueryMd5s(int type, string version, KeyValueMap& outMap)  throw (CException);
	void insertMD5toDB(SqlMapVector& inMapVec, string type)  throw (CException);
	void QueryBucketAndDomain(int type ,string& inVersion, string& outBucket, string& outDomain)  throw (CException);
	void QueryAndroidBucketAndDomain(string& inVersion, string& outBucket, string& outDomain)  throw (CException);
	void UpdateVersionStatus(string& inVersionName, string type)  throw (CException);  
	void UpdateAndroidVersionStatus(string& inVersionName, string& inMd5, int& inSize)  throw (CException);
	*/

private:
	DB_PARAM  db_pa_;
	shared_ptr<CMySQL>   _ptrMysql;
 
};

#endif 
