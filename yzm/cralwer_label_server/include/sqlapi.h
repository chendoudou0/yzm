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
 
using   KeyValueMap   = std::map<std::string,std::string>;
using	SqlResultSet  = std::map<std::string, std::string>;
using   SqlMapVector  = std::vector<KeyValueMap>;


class CMySQL
{
public:
    CMySQL(std::string host, std::string user, std::string pswd, int port, int overTime);
    ~CMySQL();
    CMySQL(const CMySQL& ref) = delete;
    CMySQL& operator=(const CMySQL& ref) = delete;
  
    bool Connect();
    void Close();
    bool Ping(); 
    bool       Query(const char *query, unsigned int length);
    int64_t    AffectedRows();
    MYSQL_RES* FetchResult();
    bool  FetchMd5Map(KeyValueMap& outMap) ;
    bool  FetchResultMap(SqlResultSet & objMap) ;
    int  FetchResultMVector(SqlMapVector & objMapVector) ;
    // 返回查询总行数
    int  FetchRows();

    bool Begin() ;
    bool Commit() ;
    bool Rollback() ;

    static std::string EscapeStr(const std::string & buff);
	
protected:    
    std::string    host_;          // 登录主机IP
    std::string    user_;          // 登录ID
    std::string    passwd_;      // 登录密码
    int            port_;    	 	 // DB端口
    int            overTime_;    // 连接超时时长

    MYSQL*      m_SqlHandle{nullptr};  // SQL操作句柄
    bool        m_connFlag{false};     // 数据库连接状态标志
};

#endif

