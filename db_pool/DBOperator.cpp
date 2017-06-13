#include "DBOperator.h"

CDBOperator::CDBOperator(DB_PARAM  db_pa)
:db_pa_(db_pa)
{
    initMYSQL();
}

void CDBOperator::initMYSQL()  
{
    _ptrMysql.reset(new CMySQL(db_pa_));
	_ptrMysql->Connect(); 
}

void CDBOperator::QueryAndroidExistingVersions(SqlMapVector &objOutMapVector)  
{

    string strSql = "SET NAMES UTF8";
	_ptrMysql->Query(strSql.c_str(),  strSql.length());
	strSql = "select  Fversion_description,Fversion_name,Foss_path,Ffile_size, \
    Ffile_md5 from yzm_version_db.t_version_android   \
    where Fversion_status='0' order by Fversion_name desc";
	 
     _ptrMysql->Query(strSql.c_str(),  strSql.length()); 
    _ptrMysql->FetchResultMVector(objOutMapVector); 
   
}

/*
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
            throw CException(ERR_DB_FETCH, "閿熸枻鎷峰彇閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹澶遍敓鏉帮綇鎷烽敓鏂ゆ嫹纭敓鏂ゆ嫹閿熻鍑ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�!");
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
            throw CException(ERR_DB_FETCH, "閿熸枻鎷峰彇閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹澶遍敓鏉帮綇鎷烽敓鏂ゆ嫹纭敓鏂ゆ嫹閿熻鍑ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�!");
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
            throw CException(ERR_DB_FETCH, "閿熸枻鎷峰彇閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹澶遍敓鏉帮綇鎷烽敓鏂ゆ嫹纭敓鏂ゆ嫹閿熻鍑ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�!");
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
            throw CException(ERR_DB_FETCH, "閿熸枻鎷峰彇閿熸枻鎷烽敓鏂ゆ嫹澶遍敓鏉帮綇鎷烽敓鏂ゆ嫹纭敓鏂ゆ嫹閿熻鍑ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�!");
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
	_ptrMysql->Begin();
	SqlMapVector::iterator iter;  
    for (iter = inMapVec.begin(); iter != inMapVec.end(); ++iter)
    {
	    char strSql[1024] = {0};
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

*/






	
