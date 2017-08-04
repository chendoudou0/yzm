#include "DBOperator.h"
#include "CLogger.h"
#include "config.h"
#include"glog/logging.h"
#include <uuid/uuid.h> 

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

bool CDBOperator::AddLabeledDate(int picId,const std::string token,const std::string poseDate)
{
    bool ret = false;
	char strSql[102400];
    memset(strSql, 0, sizeof(strSql));
     _ptrMysql->Begin();
    do{
        string user;
        int role_id;
       if(!GetUserByToken(token, user, role_id)){
           LOG(ERROR) << "token not exsist ";
           break;
       }
      
       ///////////////////////////////
        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql, "select * from yzm_pose_label_db.t_label where Fuser_name='%s' and Fpic_id='%d' ", \
        user.c_str(),
        picId);
        LOG(INFO) << "sql : "<<strSql;
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        } 
        int iRows = _ptrMysql->FetchRows();
        if(iRows == 1)       //已经标记过，标记次数就不用减一 
        {
            LOG(INFO) << "user " <<user<<" has labeled pic : "<< picId;
            memset(strSql, 0, sizeof(strSql));
            sprintf(strSql, "update yzm_pose_label_db.t_label set Fpose_data='%s' where Fuser_name='%s' and Fpic_id='%d' ", \
            poseDate.c_str(),
            user.c_str(),
            picId);
            LOG(INFO) << "sql : "<<strSql;
            if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
                break;
            }    
        }else{
            memset(strSql, 0, sizeof(strSql));
            sprintf(strSql, "insert into yzm_pose_label_db.t_label  \
            values(NULL, '%d','%s','%s', '%s', '%s')",  \
            picId,
            user.c_str(),
            poseDate.c_str(),
            GetSystemTime(),
            GetSystemTime() ); 
            LOG(INFO) << "sql : "<<strSql;
            if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
                break;
            }  

            memset(strSql, 0, sizeof(strSql));
            sprintf(strSql, "update yzm_pose_label_db.t_picture set Flabel_count=Flabel_count -1, Fupdate_user='%s', Fupdate_time='%s'  \
            where Fid='%d' ",  \
            user.c_str(),    
            GetSystemTime(), 
            picId ); 
            LOG(INFO) << "sql : "<<strSql;
            if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
                break;
            }  
            if(_ptrMysql->AffectedRows() != 1 ){
                LOG(ERROR) << "update Flabel_count failed, AffectedRows() != 1 ";
                break;      
            }
        }

        ret = true;
    }while(0); 

    if(!ret){
         _ptrMysql->Rollback();
         LOG(ERROR) << "Rollback()";
    }else{
        _ptrMysql->Commit();
         LOG(INFO) << "AddLabeledDate  success";
    }

    return ret;

}

bool CDBOperator::QueryUnlabeledPicture(SqlMapVector &objOutMapVector, const std::string token, int index, int& page, const QueryCondition& qc)
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
       string user;
        int role_id;
       if(!GetUserByToken(token, user, role_id)){
           LOG(ERROR) << "GetUserByToken  failed ";
           break;
       }

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
        ///////////////////////page num
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

int CDBOperator::QueryPicByUser(SqlMapVector &objOutMapVector, const std::string token, const std::string user, int index, int& page, const QueryCondition& qc){
    int ret = -1;
	string jsonStr;
    char strSql[1024] = {0}; 
    do{
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        ///////////////////check user
        string userName;             
        int role_id;
       if(!GetUserByToken(token, userName, role_id)){
           LOG(ERROR) << "GetUserByToken  failed ";
           break;
       }
       if(role_id != 0){
            LOG(ERROR) << "user "<< userName << " is not admin";
            ret = 2;
            break;
       }
      ///////////////////////////////////////
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

        ///////////////////////page num          
        memset(strSql, 0, 1024);
        if(!user.empty()){
            sprintf(strSql,  " select COUNT(*) count from yzm_pose_label_db.t_picture  \
            where Fpose_type like '%s' and  Ftag_word like '%s' and  Fcreate_time BETWEEN '%s' and '%s' \
            and Fid  in (select Fpic_id from yzm_pose_label_db.t_label WHERE Fuser_name = '%s' GROUP BY Fpic_id )  ",
            pose_type.c_str(),
            tag.c_str(),
            tBegin.c_str(),
            tEnd.c_str(),
            user.c_str());

        }else{                 //用户名为空，即返回所有标注过的图片
            sprintf(strSql,  " select COUNT(*) count from yzm_pose_label_db.t_picture  \
            where Fpose_type like '%s' and  Ftag_word like '%s' and  Fcreate_time BETWEEN '%s' and '%s' \
            and Fid  in (select Fpic_id from yzm_pose_label_db.t_label  GROUP BY Fpic_id )  ",
            pose_type.c_str(),
            tag.c_str(),
            tBegin.c_str(),
            tEnd.c_str());
        }
      
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
        if(!user.empty()){
            sprintf(strSql,  " select Fid, Fpic_url, Ftag_word, Fpose_type, Fupdate_user, Fcreate_time, Flabel_count \
            from yzm_pose_label_db.t_picture  \
            where Fpose_type like '%s' and  Ftag_word like '%s' and  Fcreate_time BETWEEN '%s' and '%s' \
            and Fid  in (select Fpic_id from yzm_pose_label_db.t_label WHERE Fuser_name = '%s' GROUP BY Fpic_id ) limit %d ,10 ",
            pose_type.c_str(),
            tag.c_str(),
            tBegin.c_str(),
            tEnd.c_str(),
            user.c_str(),
            index);

        }else{
            sprintf(strSql,  " select Fid, Fpic_url, Ftag_word, Fpose_type, Fupdate_user, Fcreate_time, Flabel_count \
            from yzm_pose_label_db.t_picture  \
            where Fpose_type like '%s' and  Ftag_word like '%s' and  Fcreate_time BETWEEN '%s' and '%s' \
            and Fid  in (select Fpic_id from yzm_pose_label_db.t_label  GROUP BY Fpic_id ) limit %d ,10 ",
            pose_type.c_str(),
            tag.c_str(),
            tBegin.c_str(),
            tEnd.c_str(),
            index);
        }
        LOG(INFO) << "SQL : "<<strSql;
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        if(-1 == _ptrMysql->FetchResultMVector(objOutMapVector)){
            break;
        }
        ret = 0;

    }while(0);
	
    return ret;
}

bool CDBOperator::QueryLabeledPicture(SqlMapVector &objOutMapVector, const  std::string token, int index, int& page, const QueryCondition& qc){
    bool ret = false;
	string jsonStr;
    char strSql[1024] = {0}; 

    do{
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }

        string user;
        int role_id;
       if(!GetUserByToken(token, user, role_id)){
           LOG(ERROR) << "token not exsist ";
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
        sprintf(strSql,  " select Fid, Fpic_url, Ftag_word, Fpose_type, Fupdate_user, Fcreate_time, Flabel_count from yzm_pose_label_db.t_picture where Fid  in \
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

bool CDBOperator::QueryLabeledPicByOthers(SqlMapVector &objOutMapVector, const  std::string token, int index, int& page, const QueryCondition& qc)
{
    bool ret = false;
	string jsonStr;
    char strSql[5000] = {0}; 
    do{
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        string user;
        int role_id;
       if(!GetUserByToken(token, user, role_id)){
           LOG(ERROR) << "token not exsist ";
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
        sprintf(strSql,  " select Fid, Fpic_url, Ftag_word, Fpose_type, Fcreate_time, Flabel_count , Fupdate_user \
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


bool CDBOperator::QueryPicById(SqlMapVector &objOutMapVector, int pic_id, const std::string token)
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
        string user;
        int role_id;
       if(!GetUserByToken(token, user, role_id)){
           LOG(ERROR) << "token not exsist ";
           break;
       }
      
        ///////////////////////select
        memset(strSql, 0, 1024);
        sprintf(strSql,  " select Fid, Fpic_url, Ftag_word, Fpose_type, Fcreate_time, Fupdate_user, Flabel_count from yzm_pose_label_db.t_picture  \
        where  Fid='%d' and Fpic_status = '0' ",  \
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

int  CDBOperator::InvalidatePicture(int  id, const std::string token, const bool type){
   
    int ret = -1;                   
	string jsonStr;
    char strSql[1024] = {0}; 
    do{
       string user;
       int role_id;
       if(!GetUserByToken(token, user, role_id)){
           LOG(ERROR) << "token not exsist ";
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

        int iRows = _ptrMysql->AffectedRows();
    
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

bool CDBOperator::QueryLabeledPoseData(SqlResultSet& oMap, int pic_id, const std::string token)
{
    bool ret = false;
	string jsonStr;
    char strSql[1024] = {0};
    do{
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
       string user;
       int role_id;       
       if(!GetUserByToken(token, user, role_id)){
           LOG(ERROR) << "token not exsist ";
           break;
       }
        /////////////////////////////////////
        /* 
        SqlResultSet tempMap;   
        memset(strSql, 0, sizeof(strSql));
        sprintf(strSql,  " select Fpre2DPoseInfo from yzm_pose_label_db.t_picture where Fid='%d'  ",  \
        pic_id);
        LOG(INFO) << "strSql :"<<strSql;   
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        if(!_ptrMysql->FetchResultMap(tempMap)){
            break;
        }
        pre2dPoseInfo = tempMap["Fpre2DPoseInfo"];  */
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

bool CDBOperator::QueryLastLabeledPoseData(SqlResultSet& oMap, int pic_id, const std::string user)
{
    bool ret = false;
	string jsonStr;
    char strSql[1024] = {0};
    do{
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
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

bool CDBOperator::GetUserByToken(const std::string& token, std::string& user, int& roleId)
{
    bool ret = false;
    char strSql[1024] = {0};
    SqlResultSet  objMap; 
    do{
        sprintf(strSql, "select Fuser_name, Frole_id from yzm_pose_label_db.t_user where Ftoken='%s' ", \
        token.c_str());
        LOG(INFO) << "strSql :"<<strSql;
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  
       
        if(!_ptrMysql->FetchResultMap(objMap)){
             LOG(ERROR) << "FetchResultMap  failed";
            break;
        }
        user =    objMap["Fuser_name"];
        stringstream ss;
        ss << objMap["Frole_id"];
        ss >> roleId;
        ret = true;

    }while(0);
	
    return ret;

}

int  CDBOperator::Register(const std::string& user, const std::string& passWd)
{
    int ret = -1;
    char strSql[1024] = {0};
    SqlResultSet  objMap; 
    do{
        sprintf(strSql, "select * from yzm_pose_label_db.t_user where Fuser_name='%s' ", \
        user.c_str());
        LOG(INFO) << "strSql :"<<strSql;
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  
        int iRows = _ptrMysql->FetchRows();
        if(iRows == 1)     
        {
            LOG(ERROR) << "Register failed, user already exist ";
            ret = 1;
            break;
        }
        memset(strSql, 0, sizeof(strSql));
        int roleId = 1;
        sprintf(strSql, "insert into yzm_pose_label_db.t_user(Fuser_name, Frole_id, Fpass_wd, Fcreate_time, Fupdate_time)  \
        values('%s','%d','%s','%s','%s') ", \
        user.c_str(),
        roleId,
        passWd.c_str(),
        GetSystemTime(),
        GetSystemTime()
        );
        LOG(INFO) << "strSql :"<<strSql;
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  

        ret = 0;

    }while(0);
	
    return ret;

}
int CDBOperator::Login(std::string& token, int& role_id, const std::string& user, const std::string& passWd){
    int ret = -1;
    char strSql[1024] = {0};
    SqlResultSet  objMap; 
    do{
        sprintf(strSql, "select * from yzm_pose_label_db.t_user where Fuser_name='%s' and Fpass_wd='%s'", \
        user.c_str(),
        passWd.c_str());
        LOG(INFO) << "strSql :"<<strSql;
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;                    
        }  
      
        if(!_ptrMysql->FetchResultMap(objMap)){
            ret = 1;
            break;   
        }
        stringstream ss;
        ss << objMap["Frole_id"];
        ss >> role_id;
       
        uuid_t uu; 
        char  buf[1024] = {0};
        uuid_generate( uu ); 
        uuid_unparse(uu, buf);
        token = buf;

        memset(strSql, 0, sizeof(strSql));
        int roleId = 1;
        sprintf(strSql, "update yzm_pose_label_db.t_user set Ftoken='%s', Fupdate_time='%s' where Fuser_name='%s' ", \
        token.c_str(),
        GetSystemTime(),
        user.c_str()
        );
        LOG(INFO) << "strSql :"<<strSql;
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  

        ret = 0;

    }while(0);
	
    return ret;

}

bool CDBOperator::QueryCrawleredPic(SqlMapVector &objOutMapVector,  const QueryConditions& qc, int index, int& page){
  bool ret = false;
	string jsonStr;
    char strSql[1024] = {0}; 
    do{
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        /////////////////////////////////////
        stringstream ss;
        string pose_type, tag, tBegin, tEnd, key, website;            
        ss << "%" <<qc.pose_type<< "%";
        ss >> pose_type;
        ss.clear();  ss.str("");
        ss << "%" <<qc.tag<< "%";
        ss >> tag;
        ss.clear();  ss.str("");
        ss << "%" <<qc.key<< "%";
        ss >> key;
        ss.clear();  ss.str("");
        ss << "%" <<qc.website<< "%";
        ss >> website;
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
        ///////////////////////////////////////////////
        memset(strSql, 0, 1024);
        sprintf(strSql,  " select COUNT(*) count from yzm_pose_label_db.t_picture where  \
        Fpose_type like '%s' and  Ftag_word like '%s' and  Fkey_word like '%s' \
        and Fwebsite like '%s' and Fcreate_time BETWEEN '%s' and '%s'",  \
        pose_type.c_str(),             
        tag.c_str(),    
        key.c_str(),
        website.c_str(),
        tBegin.c_str(),   
        tEnd.c_str()
        );
        LOG(INFO) << "sql : "<< strSql;
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
        SqlResultSet  objMap;
        if(!_ptrMysql->FetchResultMap(objMap)){
            break;
        }
        ss.clear(); ss.str("");
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
        sprintf(strSql,  " select Fid, Fpic_url, Ftag_word, Fkey_word, Fpose_type, Fupdate_user, Fcreate_time, Flabel_count from yzm_pose_label_db.t_picture  \
        where   Fpose_type like '%s' and  Ftag_word like '%s' and  Fkey_word like '%s' \
        and Fwebsite like '%s' and Fcreate_time BETWEEN '%s' and '%s' limit %d,10 ",   \
        pose_type.c_str(),
        tag.c_str(),
        key.c_str(),
        website.c_str(),
        tBegin.c_str(),   
        tEnd.c_str(),
        index); 
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

bool CDBOperator::AddCrawlingResult(const CrawlingResult&  cs, int dbSum){
    bool ret = false;
    char strSql[1024] = {0};
    do{
        sprintf(strSql, "insert into yzm_pose_label_db.t_crawling_history(Fkey, Ftag, Ftotal_sum, Fdb_sum,  \
        Ftime_begin, Ftime_end, Fcreate_time, Fupdate_time) values('%s','%s','%d','%d','%s','%s','%s','%s') ", \
        cs.key.c_str(),
        cs.tag.c_str(),
        cs.sumTotal,
        dbSum,
        cs.tBegin.c_str(),
        cs.tEnd.c_str(),
        GetSystemTime(),
        GetSystemTime()
        );
        LOG(INFO) << "strSql :"<<strSql;
        if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
            break;
        }  
        ret = true;

    }while(0);
	
    return ret;
}

bool CDBOperator::QueryCrawlingHistory(SqlMapVector &objOutMapVector,  const HistoryQueryCondition& qc){
     bool ret = false;
    char strSql[1024] = {0}; 
    do{
        sprintf(strSql,  "SET NAMES UTF8");
        if(!_ptrMysql->Query(strSql,  strlen(strSql))){
            break;
        }
       
        ///////////////////////select
        stringstream ss;
        string tag, key;            
        ss << "%" <<qc.tag<< "%";
        ss >> tag;
        ss.clear();
        ss.str("");
        ss << "%" <<qc.key<< "%";
        ss >> key;
        memset(strSql, 0, 1024);
        sprintf(strSql,  " select Fkey, Ftag, Ftotal_sum, Fdb_sum, Fcreate_time  from yzm_pose_label_db.t_crawling_history \
        where Ftag like '%s' and Fkey like '%s' order by Fcreate_time desc", 
        tag.c_str(),
        key.c_str()
        );
        
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

bool CDBOperator::UpdatePicInfoById(int id, const std::string tag, const std::string pose_type){
    bool ret = false;
	char strSql[1024] = {0};
    do{
        if(!tag.empty()){
            sprintf(strSql, "update yzm_pose_label_db.t_picture set Ftag_word='%s' where Fid='%d' ",  \
            tag.c_str(),
            id); 
            LOG(INFO) << "SQL : "<<strSql;
            if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
                break;
            }  
        }
        if(!pose_type.empty()){
            memset(strSql, 0, sizeof(strSql));
            sprintf(strSql, "update yzm_pose_label_db.t_picture set  Fpose_type='%s' where Fid='%d' ",  \
            pose_type.c_str(),
            id); 
            LOG(INFO) << "SQL : "<<strSql;
            if(!_ptrMysql->Query(strSql,  strlen(strSql)) ){
                break;
            } 
        } 
        
        ret = true;
        
    }while(0); 

    return ret;

}







	
