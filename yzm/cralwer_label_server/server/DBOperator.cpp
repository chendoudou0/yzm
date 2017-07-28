#include "DBOperator.h"
#include "CLogger.h"
#include "config.h"
#include"glog/logging.h"

extern CLogger* gPtrAppLog;

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

int CDBOperator::QueryPicByMd5(const std::string& md5)
{
    char strSql[1024] = {0};
    sprintf(strSql, "select * from yzm_pose_label_db.t_picture where Fpic_md5='%s' ", \
    md5.c_str());
    if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
        return 1;                //查询失败 
    }

    int iRows = _ptrMysql->FetchRows();
    if(iRows != 0)     
    {
        return 2;      //图片已存在
    }

    return 0;     //允许上传
}

int  CDBOperator::AddPicture(const server::pose_label::PicInfo& pic, string url)
{
    char strSql[1024] = {0};
    int  count = 5;
    sprintf(strSql,  "insert into yzm_pose_label_db.t_picture(Fpic_url, Fpic_md5, Fkey_word, Ftag_word,  \
    Flabel_count, Fcreate_time, Fupdate_time)  \
    values('%s', '%s','%s','%s', '%d', '%s', '%s')", url.c_str(), 
    pic.md5.c_str(), pic.key.c_str(),  pic.tag.c_str(),\
    count , GetSystemTime(), GetSystemTime());
    LOG(INFO) << "SQL : " << strSql;
    if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
        return -1;                
    }  

    return 0;
}

int  CDBOperator::QueryPicByURL(const std::string& url ,SqlResultSet &objOutMap)
{
    char strSql[1024] = {0};
    sprintf(strSql, "select * from yzm_pose_label_db.t_picture where Fpic_url='%s' ", \
    url.c_str());
    if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
        return -1;                //查询失败 
    }

    if(!_ptrMysql->FetchResultMap(objOutMap)){
        return -1;
    }

    return 0;     //允许上传

}

bool  CDBOperator::UpdatePicPoseDate(const std::string& url, string& str2dPose, string& str3dPose)
{
    bool ret = false;
	char strSql[10240];
    memset(strSql, 0, sizeof(strSql));
    do{
        sprintf(strSql, "select * from yzm_pose_label_db.t_picture where Fpic_url='%s' ", \
        url.c_str());
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  
        int iRows = _ptrMysql->FetchRows();
        
        if(iRows != 1)     
        {
            LOG(ERROR) << "CDBOperator::UpdatePicPoseDate failed, _ptrMysql->FetchRows() != 1 ";
            break;
        }
       
        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "update yzm_pose_label_db.t_picture set Fpre2DPoseInfo='%s', Fpre3DPoseInfo='%s',  \
        Fupdate_time='%s' where Fpic_url='%s'",  \
        str2dPose.c_str(),
        str3dPose.c_str(),
        GetSystemTime(),
        url.c_str()); 
    
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  

        if(_ptrMysql->AffectedRows() != 1 ){
            LOG(ERROR) << "CDBOperator::UpdatePicPoseDate failed, _ptrMysql->AffectedRows() != 1 ";
            _ptrMysql->Rollback(); 
            break;
        }

        ret = true;
        
    }while(0); 

    return ret;

}

bool CDBOperator::AddLabeledDate(int picId,const std::string user,const std::string poseDate)
{
    bool ret = false;
	char strSql[102400];

    memset(strSql, 0, sizeof(strSql));
    do{
        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "select * from yzm_pose_label_db.t_user where Fuser_name='%s' ", \
        user.c_str());
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  
        int iRows = _ptrMysql->FetchRows();
        if(iRows != 1)     
        {
            LOG(ERROR) << "CDBOperator::AddLabeledDate failed, user " <<user << " not exit ";
            break;
        }

        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "delete from yzm_pose_label_db.t_label where Fuser_name='%s' and Fpic_id='%d' ", \
        user.c_str(),
        picId);
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  
        
        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "insert into yzm_pose_label_db.t_label  \
        values(NULL, '%d','%s','%s', '%s', '%s')",  \
        picId,
        user.c_str(),
        poseDate.c_str(),
        GetSystemTime(),
        GetSystemTime() ); 
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  

        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "update yzm_pose_label_db.t_picture set Flabel_count=Flabel_count -1, Fupdate_time='%s' \
        where Fid='%d' ",  \
        GetSystemTime(),  \
        picId ); 
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  
        if(_ptrMysql->AffectedRows() != 1 ){
            LOG(ERROR) << "update Flabel_count failed, AffectedRows() != 1 ";
            _ptrMysql->Rollback(); 
            break;      
        }

        ret = true;
        
    }while(0); 

    return ret;

}

bool CDBOperator::QueryUnlabeledPicture(SqlMapVector &objOutMapVector, const std::string user, int index, int& page, const QueryCondition& qc)
{
    bool ret = false;
	string jsonStr;
    char strSql[1024] = {0}; 
    do{
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        ///////////////////check user
        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "select * from yzm_pose_label_db.t_user where Fuser_name='%s' ", \
        user.c_str());
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            LOG(ERROR) << "QueryUnlabeledPicture failed, Query failed ";
            break;
        }  
        int iRows = _ptrMysql->FetchRows();
        if(iRows != 1)     
        {
            LOG(ERROR) << "QueryUnlabeledPicture failed, user_name not exist ";  
            break;
        }
        ///////////////////////page num
        stringstream ss;
        string pose_type;
        string tag;
        string tBegin;
        string tEnd;
        ss << "%" <<qc.pose_type<< "%";
        ss >> pose_type;
        ss.clear();
        ss.str("");
        ss << "%" <<qc.tag<< "%";
        ss >> tag;
        if(qc.tBegin.empty()){
            tBegin = "2016-1-1 10:10:10";
        }else{
            tBegin = qc.tBegin;
        }
        if(qc.tEnd.empty()){
            tEnd = "2030-1-1 10:10:10";
        }else{
                tEnd = qc.tEnd;
        }

        memset(strSql, 0, 1024);
        sprintf(strSql,  " select COUNT(*) count from yzm_pose_label_db.t_picture  \
        where  Flabel_count = '5' and Fpic_status='0' and Fpose_type like '%s'   \
        and  Ftag_word like '%s' and  Fcreate_time BETWEEN '%s' and '%s' ",
        pose_type.c_str(),
        tag.c_str(),
        tBegin.c_str(),
        tEnd.c_str());
        LOG(INFO) <<"sql : "<<strSql;
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        SqlResultSet  objMap;
        if(!_ptrMysql->FetchResultMap(objMap)){
            break;
        }
        ss.clear();
        ss.str("");
        int count;
        ss << objMap["count"];
        ss >> count;
        if(count % 10 == 0){
            page = count/10;
        }else{
            page = count/10;
            page++;
        }
        LOG(INFO) << "page num  : "<< page;
        ///////////////////////select
        memset(strSql, 0, 1024);
        sprintf(strSql,  " select Fid, Fpic_url, Ftag_word, Fpose_type, Fcreate_time, Flabel_count from yzm_pose_label_db.t_picture  \
        where  Flabel_count = '5' and Fpic_status='0' and Fpose_type like '%s'  \
        and  Ftag_word like '%s' and  Fcreate_time BETWEEN '%s' and '%s' limit %d ,10 ",  \
        pose_type.c_str(),
        tag.c_str(),
        tBegin.c_str(),
        tEnd.c_str(),
        index);
        LOG(INFO) << "SQL : "<<strSql;
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

bool CDBOperator::QueryLabeledPicture(SqlMapVector &objOutMapVector, const  std::string user, int index, int& page, const QueryCondition& qc){
    bool ret = false;
	string jsonStr;
    char strSql[1024] = {0}; 
    do{
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }

        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "select * from yzm_pose_label_db.t_user where Fuser_name='%s' ", \
        user.c_str());
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  
        int iRows = _ptrMysql->FetchRows();
        if(iRows != 1)     
        {
            LOG(ERROR) << "QueryLabeledPicture failed, user_name not exit ";
            break;
        }
        /////////////////////////////////////
        stringstream ss;
        string pose_type;
        string tag;
        string tBegin;
        string tEnd;
        ss << "%" <<qc.pose_type<< "%";
        ss >> pose_type;
        ss.clear();
        ss.str("");
        ss << "%" <<qc.tag<< "%";
        ss >> tag;
        if(qc.tBegin.empty()){
            tBegin = "2016-1-1 10:10:10";
        }else{
            tBegin = qc.tBegin;
        }
        if(qc.tEnd.empty()){
            tEnd = "2030-1-1 10:10:10";
        }else{
                tEnd = qc.tEnd;
        }

        memset(strSql, 0, 1024);
        sprintf(strSql,  " select COUNT(*) count from yzm_pose_label_db.t_picture where Fid  in \
        (select Fpic_id from yzm_pose_label_db.t_label WHERE Fuser_name = '%s' GROUP BY Fpic_id )  \
        and Fpose_type like '%s' and  Ftag_word like '%s' and  Fcreate_time BETWEEN '%s' and '%s'",  \
        user.c_str(),
        pose_type.c_str(),
        tag.c_str(),
        tBegin.c_str(),
        tEnd.c_str()
        );
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        SqlResultSet  objMap;
        if(!_ptrMysql->FetchResultMap(objMap)){
            break;
        }
        ss.clear();
        ss.str("");
        int count;
        ss << objMap["count"];
        ss >> count;
        if(count % 10 == 0){
            page = count/10;
        }else{
            page = count/10;
            page++;
        }

        LOG(INFO) << "page num  : "<< page;
        ///////////////////////////////////// 
        memset(strSql, 0, 1024);
        sprintf(strSql,  " select Fid, Fpic_url, Ftag_word, Fpose_type, Fcreate_time, Flabel_count from yzm_pose_label_db.t_picture where Fid  in \
        (select Fpic_id from yzm_pose_label_db.t_label WHERE Fuser_name = '%s' GROUP BY Fpic_id )        \
        and Fpose_type like '%s' and  Ftag_word like '%s' and  Fcreate_time BETWEEN '%s' and '%s' limit %d,10 ",   \
        user.c_str(),
        pose_type.c_str(),
        tag.c_str(),
        tBegin.c_str(),
        tEnd.c_str(),
        index
        ); 
        LOG(INFO) << "sql : "<< strSql;
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

bool CDBOperator::QueryLabeledPicByOthers(SqlMapVector &objOutMapVector, const  std::string user, int index, int& page, const QueryCondition& qc)
{
    bool ret = false;
	string jsonStr;
    char strSql[5000] = {0}; 
    do{
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "select * from yzm_pose_label_db.t_user where Fuser_name='%s' and Frole_id = '0' ", \
        user.c_str());
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            LOG(ERROR) << "_ptrMysql->Query failed" ;
            break;
        }  
        
        int iRows = _ptrMysql->FetchRows();
   
        if(iRows != 1)     
        {
            LOG(ERROR) << "QueryUnlabeledPicture failed, "<<user << " is not admin user"; 
            break;
        }
        //////////////////////////////////////////////page num
        stringstream ss;
        string pose_type;
        string tag;     
        string tBegin;
        string tEnd;
        ss << "%" <<qc.pose_type<< "%";
        ss >> pose_type;
        ss.clear();
        ss.str("");
        ss << "%" <<qc.tag<< "%";
        ss >> tag;
        if(qc.tBegin.empty()){
            tBegin = "2016-1-1 10:10:10";
        }else{
            tBegin = qc.tBegin;
        }
        if(qc.tEnd.empty()){
            tEnd = "2030-1-1 10:10:10";
        }else{
                tEnd = qc.tEnd;
        }
         memset(strSql, 0, sizeof(strSql));

         sprintf(strSql,  " select COUNT(*) count  \
         from yzm_pose_label_db.t_picture  \
         where Fid  in (select Fpic_id from yzm_pose_label_db.t_label WHERE Fuser_name != '%s' GROUP BY Fpic_id )   \
         and not EXISTS (select Fpic_id from yzm_pose_label_db.t_label \
         WHERE yzm_pose_label_db.t_label.Fpic_id = yzm_pose_label_db.t_picture.Fid and Fuser_name = '%s')  \
         and Fpose_type like '%s' and  Ftag_word like '%s' and  Fcreate_time BETWEEN '%s' and '%s' ",
         user.c_str(),
         user.c_str(),
         pose_type.c_str(),
         tag.c_str(),
         tBegin.c_str(),
         tEnd.c_str()
         );
         LOG(INFO) << "sql : "<< strSql;
         if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            LOG(ERROR) << "_ptrMysql->Query failed" ;
            break;
         }
        SqlResultSet  objMap;
        if(!_ptrMysql->FetchResultMap(objMap)){
            break;
        }
        ss.clear();
        ss.str("");
        int count;
        ss << objMap["count"];
        ss >> count;
        if(count % 10 == 0){
            page = count/10;
        }else{
            page = count/10;
            page++;
        }
        LOG(INFO) << "page num  : "<< page;

        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql,  " select Fid, Fpic_url, Ftag_word, Fpose_type, Fcreate_time, Flabel_count  \
        from yzm_pose_label_db.t_picture  \
        where Fid  in (select Fpic_id from yzm_pose_label_db.t_label WHERE Fuser_name != '%s' GROUP BY Fpic_id )   \
        and not EXISTS (select Fpic_id from yzm_pose_label_db.t_label WHERE  \
        yzm_pose_label_db.t_label.Fpic_id = yzm_pose_label_db.t_picture.Fid and Fuser_name = '%s')   \
        and Fpose_type like '%s' and  Ftag_word like '%s' and  Fcreate_time BETWEEN '%s' and '%s' limit %d, 10 ",
        user.c_str(),
        user.c_str(),
        pose_type.c_str(),
        tag.c_str(),
        tBegin.c_str(),
        tEnd.c_str(),   
        index
        );
        LOG(INFO) << "sql  : "<< strSql;
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

bool CDBOperator::QueryPicById(SqlMapVector &objOutMapVector, int pic_id, const std::string user)
{
    bool ret = false;
	string jsonStr;
    char strSql[1024] = {0}; 
    do{
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        ///////////////////check user    
        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "select * from yzm_pose_label_db.t_user where Fuser_name='%s' ", \
        user.c_str());
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            LOG(ERROR) << "QueryUnlabeledPicture failed, Query failed ";
            break;
        }  
        int iRows = _ptrMysql->FetchRows();
        if(iRows != 1)     
        {
            LOG(ERROR) << "QueryUnlabeledPicture failed, user_name not exist ";  
            break;
        }
      
        ///////////////////////select
        memset(strSql, 0, 1024);
        sprintf(strSql,  " select Fid, Fpic_url, Ftag_word, Fpose_type, Fcreate_time, Flabel_count from yzm_pose_label_db.t_picture  \
        where  Fid='%d' ",  \
        pic_id);
        
        LOG(INFO) << "SQL : "<<strSql;
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

int  CDBOperator::InvalidatePicture(int  id, const std::string user, const bool type){
   
    int ret = -1;                   
	string jsonStr;
    char strSql[1024] = {0}; 
    do{
        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "select * from yzm_pose_label_db.t_user where Fuser_name='%s' and Frole_id=0", \
        user.c_str());
        LOG(INFO) << "SQL : "<< strSql;
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  
      
        int iRows = _ptrMysql->FetchRows();
     
        if(iRows != 1)     
        {
            LOG(ERROR) << "InvalidatePicture failed, no such admin user ";
            ret = 1;       //没有该管理员用户
            break;
        }
       ////////////////////////////////////////////////////////////////////////
        memset(strSql, 0, sizeof(strSql));
        if(type){
             sprintf(strSql,  " UPDATE yzm_pose_label_db.t_picture set Fpic_status = '1' WHERE Fid = '%d'",  id);
        }else{
            sprintf(strSql,  " UPDATE yzm_pose_label_db.t_picture set Fpic_status = '0' WHERE Fid = '%d'",  id);
        } 
        LOG(INFO) << "sql : "<<strSql;
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }

        iRows = _ptrMysql->AffectedRows();
    
        if(iRows != 1)     
        {
            LOG(ERROR) << "UPDATE failed,  FetchRows() != 1 ";
            ret = -1;       
            break;
        }

        ret = 0;

    }while(0);
	
    return ret;

}

bool CDBOperator::QueryLabeledPoseData(SqlResultSet& oMap, int pic_id, const std::string user)
{
    bool ret = false;
	string jsonStr;
    char strSql[1024] = {0}; 
    do{
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "select * from yzm_pose_label_db.t_user where Fuser_name='%s' ", \
        user.c_str());
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  
        int iRows = _ptrMysql->FetchRows();
        if(iRows != 1)     
        {
            LOG(ERROR) << "QueryLabeledPicture failed, user_name not exit ";
            break;
        }
        /////////////////////////////////////
        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql,  " select Fpose_data from yzm_pose_label_db.t_label  where Fpic_id='%d' and Fuser_name='%s' ",  \
        pic_id,
        user.c_str());
        LOG(INFO) << "strSql :"<<strSql;
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        if(!_ptrMysql->FetchResultMap(oMap)){
            break;
        }
        ret = true;

    }while(0);
	
    return ret;
}








	
