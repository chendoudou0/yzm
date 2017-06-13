#ifndef CDBOperator_H_
#define CDBOperator_H_

#include "Common.h"
#include "exception.h"
#include "mysql.h" 
#include "Error.h"
#include <string>
#include "sqlapi.h"
#include "def.h"

using namespace std;

class CDBOperator
{
public:

    CDBOperator(string path);
    virtual ~CDBOperator();
	bool readConf();
	bool initMYSQL();  
	void GenRepJson(SqlMapVector objVecMap, string &JsonStr);
	void QueryExistingVersions(int type, SqlMapVector &objOutMapVector)  throw (CException);
	void QueryAndroidExistingVersions(SqlMapVector &objOutMapVector)  throw (CException);
	void QueryNewaddedVersions(SqlMapVector &objOutMapVector, string type)  throw (CException);
	void QueryAndroidNewaddedVersions(SqlMapVector &objOutMapVector)  throw (CException);
	void QueryMd5s(int type, string version, KeyValueMap& outMap)  throw (CException);
	void insertMD5toDB(SqlMapVector& inMapVec, string type)  throw (CException);
	void QueryBucketAndDomain(int type ,string& inVersion, string& outBucket, string& outDomain)  throw (CException);
	void QueryAndroidBucketAndDomain(string& inVersion, string& outBucket, string& outDomain)  throw (CException);
	void UpdateVersionStatus(string& inVersionName, string type)  throw (CException);  
	void UpdateAndroidVersionStatus(string& inVersionName, string& inMd5, int& inSize)  throw (CException);
	

 
private:
	string _path;
	string _strDBHost;
  	string _strDBUser;
  	string _strDBPass;
  	string _strDBPort;
  	int    _iDBOverTime;
	CMySQL* _ptrMysql;
 
};

#endif 
