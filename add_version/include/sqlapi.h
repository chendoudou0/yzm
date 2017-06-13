/**
  * FileName: sqlapi.h 
  * Author: Created by caihf 2007-03-05
  * History: �����뵥Ԫ����ԭ�е�dbconn.c�������������ӦC++
  *             �Ŀ�����ʽ�����Բο�ԭ�е�dbconn.c
  *          vermin ��������sql�Ľӿں�ȡ������Ľӿ�
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
  * �����MySQL�Ľӿڽ����˼򵥷�װ����Ҫ�������¹���:
  *     1���ڲ�������������Ϣ
  *     2�����ݿ��Զ���������
  *     3����SELECT�����ȡ������׳��쳣
  *     4����ȡSQLӰ����������ʱ�׳��쳣
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
     * ִ��Query
     */
    void  Query(const char *query, unsigned int length) throw(CException);
    unsigned long AffectedRows() throw(CException);
    MYSQL_RES* FetchResult() throw(CException);
    int FetchResultRow(MYSQL_ROW &row) throw(CException);

    /**
     * ȡ���������map��ʽ
     * 
     */
    int  FetchMd5Map(KeyValueMap& outMap) throw(CException);
    int  FetchResultMap(SqlResultSet & objMap) throw(CException);
    int  FetchResultMVector(SqlMapVector & objMapVector) throw(CException);
    int  FetchResultMapVector(SqlMapVector & objMapVector) throw(CException);
    int  FetchResultVectorVectorMap(SqlVectorVectorMap & objVectorVectorMap) throw(CException);

    // ���ز�ѯ������
    int  FetchRows();
	
    /**
     * ���������
     */
    void Begin() throw(CException);
    void Commit() throw(CException);
    void Rollback() throw(CException);

    // ��̬�������
    static string GetReplaceSQL(SqlResultSet & objMap, const string & strTableName) throw(CException);
    static string GetSelectSQL(SqlResultSet & objMap, const string & strTableName, const string & strOther = "");
    static string GetInsertSQL(SqlResultSet & objMap, const string & strTableName) throw(CException);
    static string GetUpdateSQL(SqlResultSet & objValueMap, SqlResultSet & objWhereMap, const string & strTableName) throw(CException);

    static string GetWhere(SqlResultSet & objWhereMap);

    // ת��
    static string EscapeStr(const string & buff);

    static char *ValiStr(char *str);
	
private:
    // ��ֹ�����͸�ֵ����
    CMySQL(const CMySQL& ref);
    CMySQL& operator=(const CMySQL& ref);
        
protected:    
    std::string m_strHost;          // ��¼����IP
    std::string m_strUser;          // ��¼ID
    std::string m_strPasswd;      // ��¼����
    std::string m_strPort;    	 	 // DB�˿�
    MYSQL*      m_SqlHandle;       // SQL�������
    unsigned int m_iOverTime;    // ���ӳ�ʱʱ��

    bool    m_connFlag;         // ���ݿ�����״̬��־
};

#endif

