#include "DBOperator.h"
#include "config.h"
#include"glog/logging.h"


CDBOperator::CDBOperator()
{
    initMYSQL();
}

CDBOperator::~CDBOperator()
{
   
}
bool CDBOperator::initMYSQL()  
{
    _ptrMysql.reset(new CMySQL(config::CConfigManager::instance()->db_para_.host_,  \
    config::CConfigManager::instance()->db_para_.user_,  \
    config::CConfigManager::instance()->db_para_.pass_,  \
    config::CConfigManager::instance()->db_para_.port_,  \
    config::CConfigManager::instance()->db_para_.overTime_) );

    if(!_ptrMysql->Connect()){
        return false;
    }
  
	 return true;
}

bool CDBOperator::QueryExistingVersions(int type, SqlMapVector &objOutMapVector)
{
    bool ret = false;
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
    do{
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        memset(strSql, 0, 1024);
        sprintf(strSql,  "select  Fversion_description,Fversion_name  from cloudream_upgrade.%s  order by Fversion_name desc",\
        table.c_str());
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }

        if(-1 == _ptrMysql->FetchResultMVector(objOutMapVector)){
            break;
        }

        ret = true;

    }while(0);
	
    return ret;
} 

bool CDBOperator::QueryAndroidExistingVersions(SqlMapVector &objOutMapVector)  
{
    bool ret  =   false;
    
    do{
         char strSql[1024]; 
         
         sprintf(strSql,  "SET NAMES UTF8");
         if(!_ptrMysql->Query(strSql,  strlen(strSql))){
             break;
         }
         
         memset(strSql, 0, 1024);
         sprintf(strSql,  "select  Fversion_description,Fversion_name, Fversion_branch,Ffile_url, Ffile_size, \
            Ffile_md5 from cloudream_upgrade.t_version_android \
            order by Fversion_name desc");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        } 
        if(-1 ==_ptrMysql->FetchResultMVector(objOutMapVector)){
            break;  
         }
   
        ret = true;
    }while(0);
   
    return ret;
}

bool CDBOperator::QueryNewaddedVersions(SqlMapVector &objOutMapVector, string type)  
{
    bool ret = false;
    do{
        char strSql[1024];
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
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        memset(strSql, 0, 1024);

        sprintf(strSql,  "select Fversion_name,Fversion_path,Fversion_bucket from cloudream_upgrade.%s where Fversion_status='1'", table.c_str());
        
       if(!_ptrMysql->Query(strSql,  strlen(strSql))){
           break;
       }

       if(-1 == _ptrMysql->FetchResultMVector(objOutMapVector)){
           break;
       }

       ret = true;

    }while(0);

    
    return ret;
}  

bool CDBOperator::QueryAndroidNewaddedVersions(SqlMapVector &objOutMapVector) 
{
    bool ret = false;
    do{
        char strSql[1024] = {0};
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }

        memset(strSql, 0, 1024);
        sprintf(strSql,  "select Fversion_name,Fversion_path from cloudream_upgrade.t_version_android where Fversion_status='1'");
        
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }

        if(-1 == _ptrMysql->FetchResultMVector(objOutMapVector)){
            break;
        }

        ret = true;

    }while(0);
    

    return ret;

}  


bool CDBOperator::QueryBucketAndDomain(int type ,string& inVersion, string& outBucket, string& outDomain) 
{
	bool ret = false;
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
	sprintf(strSql,  "select Fversion_bucket,Fversion_domain from cloudream_upgrade.%s where Fversion_name='%s'", table.c_str(), inVersion.c_str());
	 
    do{
         if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
         }
         if( !_ptrMysql->FetchResultMap(objOutMap) ){
            break;
         }
       
	    outBucket = objOutMap["Fversion_bucket"];
	    outDomain = objOutMap["Fversion_domain"];

        ret = true;

    }while(0);
    
    return ret;

} 

bool CDBOperator::QueryAndroidBucketAndDomain(string& inVersion, string& outBucket, string& outDomain) 
{
	bool ret = false;
    do{
        char strSql[1024] = {0};
        KeyValueMap objOutMap;
        sprintf(strSql,  "select Foss_bucket,Foss_domain from cloudream_upgrade.t_version_android where Fversion_name='%s'", inVersion.c_str());
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
         }
         if( !_ptrMysql->FetchResultMap(objOutMap) ){
            break;
         }

        outBucket = objOutMap["Foss_bucket"];
        outDomain = objOutMap["Foss_domain"];

        ret = true;

    }while(0);

    return ret;
}
	

bool CDBOperator::insertMD5toDB(SqlMapVector& inMapVec, string type) 
{
    bool ret = false;
    string table;
    if (type == "live_show")
    {
        table = "t_md5_live_show";
    }
    else if (type == "fitting_online")
    {
        table = "t_md5_fitting_online";
    }
    do{
        _ptrMysql->Begin();

        SqlMapVector::iterator iter;  
        for (iter = inMapVec.begin(); iter != inMapVec.end(); ++iter)
        {
            char strSql[1024] = {0};
            KeyValueMap sqlMap = *iter; 
            sprintf(strSql,  "insert into cloudream_upgrade.%s	values('%s','%s','%s','%s')", table.c_str(), sqlMap["Fversion_name"].c_str(),sqlMap["Ffile_path"].c_str(),sqlMap["Fmd5"].c_str(), GetSystemTime());
             if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
                 _ptrMysql->Rollback();
                break;
            } 
        }

        _ptrMysql->Commit();

        ret = true; 

    }while(0);

    return ret;
}

bool  CDBOperator::QueryMd5s(int type, string version, KeyValueMap& outMap)  
{
    bool ret = false;
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
	sprintf(strSql,  "select Ffile_path, Fmd5 from cloudream_upgrade.%s where Fversion_name ='%s'", table.c_str(), version.c_str());
	
    do{
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
         }
         if( !_ptrMysql->FetchMd5Map(outMap) ){
            break;
         }

        ret = true;

    }while(0);

    return ret;
    
}

bool CDBOperator::UpdateVersionStatus(string& inVersionName, string type) 
{
    bool ret = false;
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
    do{
        sprintf(strSql, "select * from cloudream_upgrade.%s where Fversion_name='%s' ", table.c_str(), inVersionName.c_str());
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }
        int iRows = _ptrMysql->FetchRows();
        if(iRows != 1)     
        {
            LOG(ERROR) << "CDBOperator::UpdateVersionStatus failed, row_num != 1 ";
            break;
        }
        
        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "update cloudream_upgrade.%s set Fversion_status='0', Fupdate_time='%s' where Fversion_name='%s'",  \
        table.c_str(), GetSystemTime(), inVersionName.c_str());
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  

        if(_ptrMysql->AffectedRows() != 1 ){
            LOG(ERROR) << "CDBOperator::UpdateVersionStatus failed, _ptrMysql->AffectedRows() != 1 ";
            _ptrMysql->Rollback(); 
            break;
        }
        
        ret = true;

    }while(0);

    return ret;

}

bool CDBOperator::UpdateAndroidVersionStatus(string& inVersionName, string& inMd5, int& inSize)	
{
    bool ret = false;
	string strSize;
	int2string(inSize, strSize);  
	char strSql[1024];
    memset(strSql, 0x00, sizeof(strSql));
    do{
        sprintf(strSql, "select * from cloudream_upgrade.t_version_android where Fversion_name='%s' ",  inVersionName.c_str());
        
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  
        int iRows = _ptrMysql->FetchRows();
        if(iRows != 1)     
        {
            LOG(ERROR) << "CDBOperator::UpdateAndroidVersionStatus failed, _ptrMysql->FetchRows() != 1 ";
            break;
        }
       
        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "update cloudream_upgrade.t_version_android set Fversion_status='0', Fupdate_time='%s', Ffile_md5='%s', Ffile_size='%s' where Fversion_name='%s'",  \
        GetSystemTime(), 
        inMd5.c_str(),
        strSize.c_str(),
        inVersionName.c_str()); 
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  

        if(_ptrMysql->AffectedRows() != 1 ){
            LOG(ERROR) << "CDBOperator::UpdateAndroidVersionStatus failed, _ptrMysql->AffectedRows() != 1 ";
            _ptrMysql->Rollback(); 
            break;
        }

        ret = true;
        
    }while(0); 

    return ret;

}

int  CDBOperator::AddAndroidVersion(ANDROID_VERSION_INFO& avInfo)
{
	char strSql[1024] = {0};
    sprintf(strSql, "select * from cloudream_upgrade.t_version_android where Fversion_name='%s' ", \
    avInfo.name.c_str());
    if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
        return 1;       
    }

    int iRows = _ptrMysql->FetchRows();
    if(iRows != 0)     
    {
        return 2;   //版本已经存在
    }
    
    memset(strSql, 0, sizeof(strSql));
    sprintf(strSql,  "insert into cloudream_upgrade.t_version_android \
    values(NULL, '%s', '%d', '%s','%s','%s', '%s', '%s')", avInfo.name.c_str(),  avInfo.version_branch,
    avInfo.description.c_str(), avInfo.path.c_str(),\
    avInfo.md5.c_str(), avInfo.size.c_str(), GetSystemTime());
    LOG(INFO) << "SQL : " << strSql;
    if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
        return 1;                
    }  

    return 0;

}

int  CDBOperator::AddLiveshowVersion(LIVESHOW_ADD_INFO&  lsInfo)
{
    char strSql[1024] = {0};
    sprintf(strSql, "select Fversion_name from cloudream_upgrade.t_version_live_show where Fversion_name='%s' ", \
    lsInfo.name.c_str());
    LOG(INFO) << "SQL : " << strSql;
    if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
        return 1;       
    }

    int iRows = _ptrMysql->FetchRows();
    if(iRows != 0)     
    {
        return 2;   //版本已经存在
    }

    _ptrMysql->Begin(); 

    memset(strSql, 0, sizeof(strSql));

    sprintf(strSql,  "insert into cloudream_upgrade.t_version_live_show \
    values(NULL, '%s', %s','%s','%s', '%s')", lsInfo.name.c_str(), lsInfo.bucket.c_str(),   \
    lsInfo.domain.c_str(),  lsInfo.description.c_str(), GetSystemTime() );

    LOG(INFO) << "SQL : " << strSql;
    if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
        return 1;                
    }  
    /////////////////////////// insert md5
    for (auto&  file : lsInfo.fileVec_)
    {
	    memset(strSql, 0, 1024);
		sprintf(strSql,  "insert into cloudream_upgrade.t_md5_live_show values(NULL, '%s','%s','%s','%s')", lsInfo.name.c_str(),  \
        file.first.c_str(), file.second.c_str(), GetSystemTime());
        LOG(INFO) << "SQL : " << strSql;
         if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            _ptrMysql->Rollback();   
            LOG(ERROR) << "rollback";
            return 1;    
         }
    }

	_ptrMysql->Commit(); 
    
    return 0;

}






	
