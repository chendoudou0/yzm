#include "DBOperator.h"
#include "ErrorNo.h"
#include"WriteLog.h"
#include"Common.h"
#include"xml-config.h"
#include"def.h"
#include"glog/logging.h"

extern CWriteLog     Log;

CDBOperator::CDBOperator(string path)
:_path(path),
_ptrMysql(NULL)
{

    
}

CDBOperator::~CDBOperator()
{
   
}
void CDBOperator::GenRepJson(SqlMapVector objVecMap, string &JsonStr)
{
    SqlMapVector::iterator iter;
    //JsonStr += "{";
    for (iter = objVecMap.begin(); iter != objVecMap.end(); ++iter)
    {
    	if(iter != objVecMap.begin())
		{
            JsonStr.append(",");
		}  
		
        KeyValueMap::iterator iter1;
		JsonStr += "{";
        for(iter1 = iter->begin();iter1 != iter->end(); iter1++)
        {
        	if(iter1 != iter->begin())
			{
                JsonStr.append(",");
			}
            JsonStr += "\"" + iter1->first + "\":";
            JsonStr += "\"" + iter1->second + "\"";
        }
		JsonStr += "}";

    }
    //JsonStr += "}";
}

bool CDBOperator::readConf()
{
   XmlConfig config(_path.c_str());
   if( !config ) 
   {
	   goto _error;   
   }

   
   _strDBHost= config.getValue(DB_HOST_PATH, DEFAULT_STRING_NULL);
   if( _strDBHost== DEFAULT_STRING_NULL ) 
   {
	   goto _error;
   }
	
    _strDBUser= config.getValue(DB_USER_PATH, DEFAULT_STRING_NULL);
   if( _strDBUser== DEFAULT_STRING_NULL ) 
   {
	   goto _error;
   }

   _strDBPass= config.getValue(DB_PASSWD_PATH, DEFAULT_STRING_NULL);
   if( _strDBPass== DEFAULT_STRING_NULL ) 
   {
	   goto _error;
   }
  

    _strDBPort= config.getValue(DB_PORT_PATH, DEFAULT_STRING_NULL);
   if( _strDBHost == DEFAULT_STRING_NULL ) 
   {
	   goto _error;
   }
  

    _iDBOverTime= config.getValue(DB_OVERTIME_PATH, 0);
   if( _iDBOverTime == 0 ) 
   {
	   goto _error;
   }

   _error:
		Log.WriteLog(4, "[%s:%d]  read XmlConfig error \n", __FILE__, __LINE__);
		return false;

}

bool CDBOperator::initMYSQL()  
{
	
	_ptrMysql = new CMySQL(_strDBHost.c_str(),
			   _strDBUser.c_str(), _strDBPass.c_str(),
			   _strDBPort.c_str(), _iDBOverTime);
	if(NULL == _ptrMysql)
	{
		return false;
	}
	 _ptrMysql->Connect(); 

	 return true;

}



void CDBOperator::QueryExistingVersions(int type, SqlMapVector &objOutMapVector)  throw (CException)
{
	string jsonStr;
    char strSql[1024]; 
    string table;
    if (type == LIVE_SHOW_VERSION_QUERY)
    {
        table = "t_version_live_show";
    }
    else if (type == FITTING_ONLINE_VERSION_QUERY)
    {
        table = "t_version_fitting_online";
    }

	sprintf(strSql,  "SET NAMES UTF8");
	_ptrMysql->Query(strSql,  strlen(strSql));
	memset(strSql, 0, 1024);
	sprintf(strSql,  "select  Fversion_description,Fversion_name  from yzm_version_db.%s where Fversion_status='0' order by Fversion_name desc",\
    table.c_str());
	 
    try
    {
        _ptrMysql->Query(strSql,  strlen(strSql));
        if(0!= _ptrMysql->FetchResultMVector(objOutMapVector))
        {
            throw CException(ERR_DB_FETCH, "获取配置数据失败，请确认是否存在数据!");
        }
        
    }
    catch(...)
    {
        throw;
    }
    
    return;

} 

void CDBOperator::QueryAndroidExistingVersions(SqlMapVector &objOutMapVector)  throw (CException)
{
	string jsonStr;

    char strSql[1024]; 

	sprintf(strSql,  "SET NAMES UTF8");
	_ptrMysql->Query(strSql,  strlen(strSql));
	memset(strSql, 0, 1024);
	sprintf(strSql,  "select  Fversion_description,Fversion_name,Foss_path,Ffile_size, \
		Ffile_md5 from yzm_version_db.t_version_android \
		where Fversion_status='0' order by Fversion_name desc");
	 
    try
    {
        _ptrMysql->Query(strSql,  strlen(strSql));  
        if(0!= _ptrMysql->FetchResultMVector(objOutMapVector))
        {
            throw CException(ERR_DB_FETCH, "获取配置数据失败，请确认是否存在数据!");
        }
   
    }
    catch(...)   
    {
        throw;
    }
    
	
    return;


}


void CDBOperator::QueryNewaddedVersions(SqlMapVector &objOutMapVector, string type)  throw (CException)
{
    char strSql[1024];
	string jsonStr;
    string table;
    if (type == "live_show")
    {
        table = "t_version_live_show";
    }
    else if (type == "fitting_online")
    {
        table = "t_version_fitting_online";
    }
	sprintf(strSql,  "SET NAMES UTF8");
	_ptrMysql->Query(strSql,  strlen(strSql));
	memset(strSql, 0, 1024);

	sprintf(strSql,  "select Fversion_name,Fversion_path,Fversion_bucket from yzm_version_db.%s where Fversion_status='1'", table.c_str());
	 
    try
    {
        _ptrMysql->Query(strSql,  strlen(strSql));

        if(0!= _ptrMysql->FetchResultMVector(objOutMapVector))
        {
            throw CException(ERR_DB_FETCH, "获取配置数据失败，请确认是否存在数据!");
        }
        
    }
    catch(...)
    {
        throw;
    }
    
	
    return;

}  

void CDBOperator::QueryAndroidNewaddedVersions(SqlMapVector &objOutMapVector)  throw (CException)
{
    LOG(ERROR) << "QueryAndroidNewaddedVersions  begin"; 
	string jsonStr;

    char strSql[1024];

	sprintf(strSql,  "SET NAMES UTF8");
	_ptrMysql->Query(strSql,  strlen(strSql));
	memset(strSql, 0, 1024);

	sprintf(strSql,  "select Fversion_name,Fversion_path from yzm_version_db.t_version_android where Fversion_status='1'");
	 
    try
    {
        _ptrMysql->Query(strSql,  strlen(strSql));

        if(0!= _ptrMysql->FetchResultMVector(objOutMapVector))
        {
            throw CException(ERR_DB_FETCH, "获取数据失败，请确认是否存在数据!");
        }
    
    }
    catch(...)
    {
        throw;
    }
    
	
    return;

}  


void CDBOperator::QueryBucketAndDomain(int type ,string& inVersion, string& outBucket, string& outDomain)  throw (CException)
{
	string jsonStr;
    char strSql[1024];
	KeyValueMap objOutMap;
    string table;
    
    if (type == LIVE_SHOW_VERSION_UPDATE)
    {
        table = "t_version_live_show";   
    }
    else if (type == FITTING_ONLINE_VERSION_UPDATE)
    {
        table = "t_version_fitting_online";
    }
	sprintf(strSql,  "select Fversion_bucket,Fversion_domain from yzm_version_db.%s where Fversion_name='%s'", table.c_str(), inVersion.c_str());
	 
    try
    {
        _ptrMysql->Query(strSql,  strlen(strSql));
        if(0!= _ptrMysql->FetchResultMap(objOutMap))
        {
            throw CException(ERR_DB_FETCH, "0000");
        }
        
    }
    catch(...)
    {
        throw;
    }

	outBucket = objOutMap["Fversion_bucket"];
	outDomain = objOutMap["Fversion_domain"];
    
    return;

} 

void CDBOperator::QueryAndroidBucketAndDomain(string& inVersion, string& outBucket, string& outDomain)  throw (CException)
{
	string jsonStr;
    char strSql[1024] = {0};
	KeyValueMap objOutMap;

	sprintf(strSql,  "select Foss_bucket,Foss_domain from yzm_version_db.t_version_android where Fversion_name='%s'", inVersion.c_str());
	 
    try
    {
        _ptrMysql->Query(strSql,  strlen(strSql));
        if(0!= _ptrMysql->FetchResultMap(objOutMap))
        {
            throw CException(ERR_DB_FETCH, "0000");
        }
        
    }
    catch(...)
    {
        throw;
    }

	outBucket = objOutMap["Foss_bucket"];
	outDomain = objOutMap["Foss_domain"];
    
    return;

}
	

void CDBOperator::insertMD5toDB(SqlMapVector& inMapVec, string type)  throw (CException)
{
    string table;
    if (type == "live_show")
    {
        table = "t_md5_live_show";
    }
    else if (type == "fitting_online")
    {
        table = "t_md5_fitting_online";
    }
    //delete old md5s
    char strSql[1024] = {0};
	sprintf(strSql,  "delete from  yzm_version_db.%s where Fversion_name='%s'	", \
    table.c_str(), (*inMapVec.begin())["Fversion_name"].c_str());
	
	try
    {
        _ptrMysql->Query(strSql,  strlen(strSql));  
    }
    catch(...)
    {
        Log.WriteLog(4, "delete md5s  failed \n");
        throw CException(ERR_DB_DELETE, "delete md5s  failed");
    }

	_ptrMysql->Begin();
	SqlMapVector::iterator iter;  
    for (iter = inMapVec.begin(); iter != inMapVec.end(); ++iter)
    {
	    memset(strSql, 0, 1024);
		KeyValueMap sqlMap = *iter; 
		sprintf(strSql,  "insert into yzm_version_db.%s	values('%s','%s','%s','%s')", table.c_str(), sqlMap["Fversion_name"].c_str(),sqlMap["Ffile_path"].c_str(),sqlMap["Fmd5"].c_str(), GetSystemTime());
		   
	    try
	    {
	       _ptrMysql->Query(strSql,  strlen(strSql));  
	    }
	    catch(...)
	    {
	        _ptrMysql->Rollback();
			Log.WriteLog(4, "rollback !!!\n");
			throw CException(ERR_DB_INSERT, "insertMD5toDB error");
	    }

    }

	_ptrMysql->Commit(); 
}

void CDBOperator::QueryMd5s(int type, string version, KeyValueMap& outMap)  throw (CException)
{
	char strSql[1024] = {0};
    string table;
    if (type == LIVE_SHOW_VERSION_UPDATE)
    {
        table = "t_md5_live_show";   
    }
    else if (type == FITTING_ONLINE_VERSION_UPDATE)
    {
        table = "t_md5_fitting_online";
    }
	sprintf(strSql,  "select Ffile_path, Fmd5 from yzm_version_db.%s where Fversion_name ='%s'", table.c_str(), version.c_str());
	   
    try
    {
        _ptrMysql->Query(strSql,  strlen(strSql));      
    }
    catch(...)
    {
        throw;
    }


	if(0 != _ptrMysql->FetchMd5Map(outMap))
    {
        throw CException(ERR_DB_FETCH, "query md5 error");   
    }


}

void CDBOperator::UpdateVersionStatus(string& inVersionName, string type)  throw (CException)
{
	char strSql[1024];
    memset(strSql, 0x00, sizeof(strSql));
    string table;
    if (type == "live_show")
    {
        table = "t_version_live_show";
    }
    else if (type == "fitting_online")
    {
        table = "t_version_fitting_online";
    }

	sprintf(strSql, "select * from yzm_version_db.%s where Fversion_name='%s' ", table.c_str(), inVersionName.c_str());
    _ptrMysql->Query( strSql, strlen(strSql) );
    int iRows = _ptrMysql->FetchRows();
    if(0== iRows)     
    {
        throw CException(ERR_DB_FETCH, "version not exits");

    }
    else if(1==iRows)
    {
        memset(strSql, 0, sizeof(strSql));
		sprintf(strSql, "update yzm_version_db.%s set Fversion_status='0', Fupdate_time='%s' where Fversion_name='%s'",  \
		table.c_str(), GetSystemTime(), inVersionName.c_str());
		_ptrMysql->Query( strSql, strlen(strSql) );  
		int iRows = _ptrMysql->AffectedRows();
		if(1 != iRows)
		{
			_ptrMysql->Rollback();   
			throw CException(ERR_DB_UPDATE, "update  error");
		}
    }
    else
    {
        throw CException(ERR_DB_UPDATE,  "update  error!");
    }
	
}

void CDBOperator::UpdateAndroidVersionStatus(string& inVersionName, string& inMd5, int& inSize)	throw (CException)
{
	string strSize;
	int2string(inSize, strSize);  
	char strSql[1024];
    memset(strSql, 0x00, sizeof(strSql));

	sprintf(strSql, "select * from yzm_version_db.t_version_android where Fversion_name='%s' ",  inVersionName.c_str());
    _ptrMysql->Query( strSql, strlen(strSql) );
    int iRows = _ptrMysql->FetchRows();
    if(0== iRows)     
    {
        throw CException(ERR_DB_FETCH, "version not exits");

    }
    else if(1==iRows)
    {
        memset(strSql, 0, sizeof(strSql));
		sprintf(strSql, "update yzm_version_db.t_version_android set Fversion_status='0', Fupdate_time='%s', Ffile_md5='%s', Ffile_size='%s' where Fversion_name='%s'",  \
		GetSystemTime(), 
		inMd5.c_str(),
		strSize.c_str(),
		inVersionName.c_str()); 
		_ptrMysql->Query( strSql, strlen(strSql) );  
		int iRows = _ptrMysql->AffectedRows();
		if(1 != iRows)
		{
			_ptrMysql->Rollback();   
			throw CException(ERR_DB_UPDATE, "update  error");
		}
    }
    else
    {
        throw CException(ERR_DB_UPDATE,  "update  error!");
    }

}






	
