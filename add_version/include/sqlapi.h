/**
  * FileName: sqlapi.h 
  * Author: Created by caihf 2007-03-05
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


#include "Error.h"
#include "ErrorNo.h"

#include   "exception.h"
#include   <string>
#include   <map>
#include   <vector>
#include   <sstream>
#include<iostream>

using namespace std;
 
 typedef	map<string,string>						   KeyValueMap;
 typedef	map<string, string> 					   SqlResultSet;
 typedef	map<string, pair<string, string> >			KeyPairMap;
 typedef	vector<KeyValueMap> 					   SqlMapVector;
 typedef	vector<SqlMapVector>					   SqlVectorVectorMap;

static const int MAX_FILED_LENGTH = 32000;
#define	NSTR	""

/**
  * 本类对MySQL的接口进行了简单封装，主要包括如下功能:
  *     1、内部保存了连接信息
  *     2、数据库自动重连功能
  *     3、非SELECT语句后获取结果集抛出异常
  *     4、获取SQL影响行数出错时抛出异常
  */
 
class CMySQL
{
public:
    CMySQL(const char* host, const char* user, const char* pswd, const char* port, int iOverTime);
    ~CMySQL();
  
    void Connect() throw(CException);
    void Close();
    bool Ping(); 

    /**
     * 执行Query
     */
    void  Query(const char *query, unsigned int length) throw(CException);
    unsigned long AffectedRows() throw(CException);
    MYSQL_RES* FetchResult() throw(CException);
    int FetchResultRow(MYSQL_ROW &row) throw(CException);

    /**
     * 取结果集返回map格式
     * 
     */
    int  FetchMd5Map(KeyValueMap& outMap) throw(CException);
    int  FetchResultMap(SqlResultSet & objMap) throw(CException);
    int  FetchResultMVector(SqlMapVector & objMapVector) throw(CException);
    int  FetchResultMapVector(SqlMapVector & objMapVector) throw(CException);
    int  FetchResultVectorVectorMap(SqlVectorVectorMap & objVectorVectorMap) throw(CException);

    // 返回查询总行数
    int  FetchRows();
	
    /**
     * 事务处理语句
     */
    void Begin() throw(CException);
    void Commit() throw(CException);
    void Rollback() throw(CException);

    // 静态处理语句
    static string GetReplaceSQL(SqlResultSet & objMap, const string & strTableName) throw(CException);
    static string GetSelectSQL(SqlResultSet & objMap, const string & strTableName, const string & strOther = "");
    static string GetInsertSQL(SqlResultSet & objMap, const string & strTableName) throw(CException);
    static string GetUpdateSQL(SqlResultSet & objValueMap, SqlResultSet & objWhereMap, const string & strTableName) throw(CException);

    static string GetWhere(SqlResultSet & objWhereMap);

    // 转义
    static string EscapeStr(const string & buff);

    static char *ValiStr(char *str);
	
private:
    // 禁止拷贝和赋值操作
    CMySQL(const CMySQL& ref);
    CMySQL& operator=(const CMySQL& ref);
        
protected:    
    std::string m_strHost;          // 登录主机IP
    std::string m_strUser;          // 登录ID
    std::string m_strPasswd;      // 登录密码
    std::string m_strPort;    	 	 // DB端口
    MYSQL*      m_SqlHandle;       // SQL操作句柄
    unsigned int m_iOverTime;    // 连接超时时长

    bool    m_connFlag;         // 数据库连接状态标志
};

#endif

