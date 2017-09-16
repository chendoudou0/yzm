#include"glog/logging.h"
#include "DBOperator.h"
#include "../config.h"


const std::string GetSystemTime()
{
	auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct tm* ptm = localtime(&tt);
	char date[60] = { 0 };
	sprintf(date, "%d-%02d-%02d %02d:%02d:%02d",
		(int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
		(int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
	return std::string(date);
}

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

int  CDBOperator::AddClothes(ClothInfo clothInfo) {
	char strSql[2048] = { 0 };
	sprintf(strSql, "insert into yzm_auto_dress_db.t_auto_dress_clothes(Fid, Ftask_id, Fclothes_name, Fstatus,  \
    Fpng_local_path, Fjpg_local_path, Fmatt_local_path, Fdress_local_path, \
	Fpng_url, Fjpg_url, Fmatt_url, Fdress_url, Fcreate_time, Fupdate_time) \
    values(NULL, '%d','%s','%d', '%s', '%s', '%s','%s','%s','%s', '%s', '%s', '%s', '%s')" , \
	clothInfo.task_id_,
	clothInfo.cloth_name_.c_str(),
	clothInfo.status_,
	clothInfo.png_path_.c_str(),
	clothInfo.jpg_path_.c_str(),
	clothInfo.matt_path_.c_str(),
	clothInfo.dress_path_.c_str(),
	clothInfo.png_url_.c_str(),
	clothInfo.jpg_url_.c_str(),
	clothInfo.matt_url_.c_str(),
	clothInfo.dress_url_.c_str(),
	GetSystemTime().c_str(),
	GetSystemTime().c_str());
	if (!_ptrMysql->Query(strSql, strlen(strSql))) {
		return -1;
	}
	LOG(INFO) << "add clothes to db success";
	return 0;
}







	
