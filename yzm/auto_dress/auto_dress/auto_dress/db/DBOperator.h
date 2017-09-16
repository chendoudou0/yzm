#ifndef CDBOperator_H_
#define CDBOperator_H_

#include "sqlapi.h"

#include<memory>
#include <string>
#include<vector>
#include<utility>

struct ClothInfo
{
	std::string png_path_;
	std::string jpg_path_;
	std::string matt_path_;
	std::string dress_path_;
	std::string cloth_name_;
	std::string png_url_;
	std::string jpg_url_;
	std::string matt_url_;
	std::string dress_url_;
	std::string bucket_;
	int task_id_{ 0 };
	int status_{ 0 };
	std::string info_;
	int    sex_{ 0 };
};

class CDBOperator
{
public:
    CDBOperator();
    virtual ~CDBOperator();
	
public:
	bool initMYSQL();  
	/////////////////////////////////////////////////////////////////////////////////////////////////
	int  QueryPicByMd5(const std::string& md5);
	int  QueryPicByURL(const std::string& url, SqlResultSet &objOutMap);
	int  AddClothes(ClothInfo clothInfo);
	
private:   
	std::shared_ptr<CMySQL>  _ptrMysql;   
 
};

#endif 

