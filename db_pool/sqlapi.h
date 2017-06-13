/**
  * FileName: sqlapi.h 
  * Author: Created by 
  * History: 本编译单元基于原有的dbconn.c改造而来，以适应C++
  *             的开发方式，可以参考原有的dbconn.c
  *          vermin 增加生成sql的接口和取结果集的接口
  */
#ifndef _SQL_API_H_
#define _SQL_API_H_

#include <mysql.h>
#include <errmsg.h>
#include <mysqld_error.h>
#include <mysqld_error.h>

#include<string>
#include<map>
#include<vector>

using namespace std;
 
using   KeyValueMap   = map<string,string>;
using	SqlResultSet  = map<string, string>;
using   SqlMapVector = vector<KeyValueMap>;

struct  DB_PARAM{
	string  host_;
  	string  user_;      
  	string  pass_;
  	int     port_{3306};
  	int     overTime_{2};
};

class CMySQL
{
public:
    CMySQL(DB_PARAM db_pa);
    ~CMySQL();
    CMySQL(const CMySQL& ref) = delete;
    CMySQL& operator=(const CMySQL& ref) = delete;
  
    bool Connect();
    void Close();
    bool Ping(); 
    bool       Query(const char *query, unsigned int length);
    int64_t    AffectedRows();
    MYSQL_RES* FetchResult();
    int  FetchResultMap(SqlResultSet & objMap) ;
    int  FetchResultMVector(SqlMapVector & objMapVector) ;
    // 返回查询总行数
    int  FetchRows();

    int Begin() ;
    int Commit() ;
    int Rollback() ;

    static string EscapeStr(const string & buff);
	
protected:    
    DB_PARAM    db_pa_;
    MYSQL*      m_SqlHandle{nullptr};  // SQL操作句柄
    bool        m_connFlag{false};     // 数据库连接状态标志
};

#endif

