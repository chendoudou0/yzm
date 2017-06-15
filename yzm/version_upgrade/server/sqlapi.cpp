#include "sqlapi.h"


/**
 * Description: 构造函数
 * Input:         host         数据库主机IP地址
 *                   user         数据库用户ID
 *                   passwd     数据库用户密码
 *                   iOverTime 连接超时时长
 */
CMySQL::CMySQL(const char* host, const char* user, const char* passwd, const char* port, int iOverTime)
                               : m_strHost(host), m_strUser(user), m_strPasswd(passwd), m_strPort(port), m_iOverTime(iOverTime)
{
    m_SqlHandle = NULL;      
    m_connFlag = false;         // 数据库未连接
    
   // gPtrAppLog->debug("host: %s ,user %s, pwd: %s, port: %s, timeout: %d", 
	////	m_strHost.c_str(), m_strUser.c_str(), m_strPasswd.c_str(), m_strPort.c_str(), m_iOverTime);
}

/**
 * Description: 构造函数
 * Input:         host         数据库主机IP地址
 *                   user         数据库用户ID
 *                   passwd     数据库用户密码
 *                   port     	数据库端口
 *                   iOverTime 连接超时时长
 */
CMySQL::~CMySQL()
{
    Close();
}

/**
 * Description: 连接数据库
 * Return:       出错抛出异常
 */
void  CMySQL::Connect() throw(CException)
{
	//gPtrAppLog->debug("[Connect]host: %s ,user %s, pwd: %s, port: %s", m_strHost.c_str(), m_strUser.c_str(), m_strPasswd.c_str(), m_strPort.c_str());
    // 初始化连接句柄
    if(m_SqlHandle == NULL)
    {
        m_SqlHandle =(MYSQL *)malloc(sizeof(MYSQL));
    }
    
    mysql_init(m_SqlHandle);

    // 初始化失败
    if (m_SqlHandle == NULL)
    {
        throw CException(ERR_DB_INITIAL, "Failed to allocate mysql handle in mysql_init");
    }
    
    // 设置连接选项
    mysql_options(m_SqlHandle, MYSQL_OPT_CONNECT_TIMEOUT,  (char *)&m_iOverTime);   // 超时时长
    //my_bool reconnect = 0;
    //mysql_options(m_SqlHandle, MYSQL_OPT_RECONNECT, &reconnect);	// 禁止自动重连
    if (!mysql_real_connect(m_SqlHandle, m_strHost.c_str(), m_strUser.c_str(), m_strPasswd.c_str(), NULL, atoi(m_strPort.c_str()), NULL, 0))
    {
        throw CException(ERR_DB_CONNECT, mysql_error(m_SqlHandle));
    }

    // 设置连接成功标志
    m_connFlag = true;

	cout<<"connect  mysql success!!!!"<<endl;

//	gPtrAppLog->debug("Connect  success!!!!");
}


/**
 * Description: 关闭数据库
 */
void  CMySQL::Close()
{
    if (m_SqlHandle) 
    { 
        mysql_close(m_SqlHandle);
        free(m_SqlHandle); 
        m_SqlHandle = NULL;
    }        

    m_connFlag = false;
}

/**
 * Description: 检测数据库状态
 * Input:         mysql     SQL操作句柄
 */


bool CMySQL::Ping()
{
    return mysql_ping(m_SqlHandle) == 0;
}  


/**
 * Description: 执行SELECT类型的SQL语句
 * Input:         query     SQL语句
 *                   length     SQL语句的长度
 * Return:       失败抛出异常
 */
void CMySQL::Query(const char *query, unsigned int length) throw(CException)
{
    if (!m_connFlag)
    {
        Close();
        Connect();
    }

    if (mysql_real_query(m_SqlHandle, query, length)  !=  0 )
    {
        // 判断是否为数据库断连
        if (mysql_errno(m_SqlHandle) == CR_SERVER_LOST || mysql_errno(m_SqlHandle) == CR_SERVER_GONE_ERROR)
        {
            m_connFlag = false;
            throw CException(ERR_DB_LOST, string(query) + string(":") +string(mysql_error(m_SqlHandle)));
        }
        else
        {
            m_connFlag = false;
            throw CException(ERR_DB_UNKNOW_FAULT, string(query) + string(":") + string(mysql_error(m_SqlHandle)));
        }
    }
}


/**
 * Description: 执行事务开始语句
 * Return:       失败抛出异常
 */
void CMySQL::Begin() throw (CException)
{
    // 若处于断连状态，则进行重连
    if (!m_connFlag)
    {
        Close();
        Connect();
    }

    // 开始事务
    if(mysql_real_query(m_SqlHandle, "BEGIN", sizeof("BEGIN")) != 0)
    {
        throw CException(ERR_DB_BEGIN, mysql_error(m_SqlHandle));
    };
}


/**
 * Description: 执行事务提交
 * Return:       失败抛出异常
 */
void CMySQL::Commit() throw (CException)
{
//	gPtrAppLog->debug("Commit");
    // 提交事务
    if(mysql_real_query(m_SqlHandle, "COMMIT", sizeof("COMMIT")) != 0)
    {
 //   	gPtrAppLog->debug("Err Commit: errno: %d info: %s", mysql_errno(m_SqlHandle), mysql_error(m_SqlHandle));
		
        throw CException(ERR_DB_COMMIT, string(mysql_error(m_SqlHandle)));
    };

//	gPtrAppLog->debug("Commit end");
}

/**
 * Description: 执行事务回滚
 * Return:      失败抛出异常
 */
void CMySQL::Rollback() throw (CException)
{
//	gPtrAppLog->debug("Rollback");
    // 回滚事务
    if(mysql_real_query(m_SqlHandle, "ROLLBACK", sizeof("ROLLBACK")) != 0)
    {
        throw CException(ERR_DB_ROLLBACK, mysql_error(m_SqlHandle));
    };
}

/**
 * Description: 获取SQL影响的行数
 * Return:      失败抛出异常
 */
unsigned long CMySQL::AffectedRows() throw(CException)
{
    my_ulonglong rownum = mysql_affected_rows(m_SqlHandle);
    if (rownum == (my_ulonglong) -1)
    {
        throw CException(ERR_DB_AFFECT_ROW, mysql_error(m_SqlHandle));
    }

    return rownum;
}

/**
 * Description: 取结果集
 * Return:      失败抛出异常
 */
MYSQL_RES* CMySQL::FetchResult() throw(CException)
{
    MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
    if (pRes == NULL)
    {
        throw CException(ERR_DB_NULL_RESULT, "Fetch result for non-select statement");
    }
    
    return pRes;
}

int 
CMySQL::FetchRows()
{
	MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
    if (pRes == NULL)
    {
        throw CException(ERR_DB_NULL_RESULT, "Fetch result for non-select statement");
    }

	int iRet = mysql_num_rows(pRes);
	
	mysql_free_result(pRes);
	
	return iRet;
}



int  
CMySQL::FetchResultRow(MYSQL_ROW &row) throw(CException)
{
	row = NULL;
	
	MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
   	if (pRes == NULL)
    	{
       		throw CException(ERR_DB_NULL_RESULT, "Fetch result for non-select statement");
    	}
	try
	{
		int iRetNum = mysql_num_rows(pRes);

		if(iRetNum == 0)
		{
			mysql_free_result(pRes);
			return ERR_DB_SQL_NOVALUE;
		}
		if(iRetNum != 1)
		{
			stringstream ssErrMsg;
			ssErrMsg << "mysql_num_rows is: " << iRetNum;
			throw CException(ERR_DB_MULTI_ROW, ssErrMsg.str());
		}

		row = mysql_fetch_row(pRes);

	}
	catch(...)
	{
		mysql_free_result(pRes);
		throw;
	}

	mysql_free_result(pRes);
	
	return 0;
}


// 结果集为0，返回1，其他情况返回0
int  
CMySQL::FetchResultMap(SqlResultSet & objMap) throw(CException)
{
	// 清空以前map
	objMap.clear();
	
	//char szFiledValue[MAX_FILED_LENGTH] = {0};
	int iNumberFields;

	unsigned long *lengths;
	MYSQL_ROW row;
	MYSQL_FIELD * pField;
	
	MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
   	if (pRes == NULL)
    	{
       		throw CException(ERR_DB_NULL_RESULT, "Fetch result for non-select statement");
    	}
	try
	{
		int iRetNum = mysql_num_rows(pRes);

		if(iRetNum == 0)
		{
			mysql_free_result(pRes);
			return ERR_DB_SQL_NOVALUE;
		}
		if(iRetNum != 1)
		{
			stringstream ssErrMsg;
			ssErrMsg << "mysql_num_rows is: " << iRetNum;
			throw CException(ERR_DB_MULTI_ROW, ssErrMsg.str());
		}

		lengths = mysql_fetch_lengths(pRes);
		iNumberFields = mysql_num_fields(pRes);
		row = mysql_fetch_row(pRes);
		pField = mysql_fetch_fields(pRes);

		//DEBUG_LOG("iNumberFields : %d", iNumberFields);
	
		for(int i = 0; i < iNumberFields; i++)
		{
	//		gPtrAppLog->debug("[FetchResultMap]i: %d,field: %s,   value: %s ", i, pField[i].name, ValiStr(row[i]));

			objMap[pField[i].name] = ValiStr(row[i]);
		}
	}
	catch(...)
	{
		mysql_free_result(pRes);
		throw;
	}

	mysql_free_result(pRes);
	
	return 0;
}


// 结果集为0，返回1，其他情况返回0
//VECTOR里面放着MAP
int  
CMySQL::FetchResultMVector(SqlMapVector & objMapVector) throw(CException)
{
	objMapVector.clear();
	
	int iNumberFields;
//	unsigned long *lengths;
	MYSQL_ROW row;
	MYSQL_FIELD * pField;

//	gPtrAppLog->debug("1111111111111111111111111");
	
	MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
   	if (pRes == NULL)
    	{
    //	    gPtrAppLog->debug("mysql_store_result return is NULL");
       		throw CException(ERR_DB_NULL_RESULT, "Fetch result for non-select statement");
    	}
	try
	{
		int iRetNum = mysql_num_rows(pRes);

		if(iRetNum == 0)
		{
	//	    gPtrAppLog->debug("mysql_num_rows return is NULL");
			mysql_free_result(pRes);
			return 1;
		}

	  // lengths = mysql_fetch_lengths(pRes);
		while(NULL != (row = mysql_fetch_row(pRes)))
		{
			KeyValueMap mResultSet;
			iNumberFields = 0;
			pField = mysql_fetch_fields(pRes);
			iNumberFields = mysql_num_fields(pRes);

			//gPtrAppLog->debug("iNumberFields : %d", iNumberFields);
		
			mResultSet.clear();
			for(int i = 0; i < iNumberFields; i++)
			{
		//		gPtrAppLog->debug("i: %d,field: %s,   value: %s ", i, pField[i].name, ValiStr(row[i]));
				
				string fieldName = pField[i].name;
				fieldName = fieldName.substr(1, fieldName.length()-1);
				mResultSet[fieldName] = ValiStr(row[i]);
				//cout<<ValiStr(row[i])<<endl;
				
				//mResultSet[pField[i].name] = ValiStr(row[i]);  
			}
			objMapVector.push_back(mResultSet);
		}
	}
	catch(...)
	{
		mysql_free_result(pRes);
		throw;
	}

	mysql_free_result(pRes);
	
	return 0;
}


int  
CMySQL::FetchMd5Map(KeyValueMap& outMap) throw(CException)
{
	outMap.clear();
	
	int iNumberFields;
	MYSQL_ROW row;
	MYSQL_FIELD * pField;

	
	MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
   	if (pRes == NULL)
	{
   		throw CException(ERR_DB_NULL_RESULT, "Fetch result for non-select statement");
	}
	try
	{
		int iRetNum = mysql_num_rows(pRes);

		if(iRetNum == 0)
		{
	//	    gPtrAppLog->debug("mysql_num_rows return is NULL");
			mysql_free_result(pRes);
			return 1;
		}
		while(NULL != (row = mysql_fetch_row(pRes)))
		{
			iNumberFields = 0;
			pField = mysql_fetch_fields(pRes);
			iNumberFields = mysql_num_fields(pRes);
			
			string filePath;
			string md5;
			for(int i = 0; i < iNumberFields; i++)
			{
				string fieldName = pField[i].name;
				if(fieldName == "Ffile_path")
				{
					filePath = ValiStr(row[i]);
				}
				if(fieldName == "Fmd5")
				{
					md5 = ValiStr(row[i]);
				}
			}
			outMap[filePath] = md5; 
			
		}
	}
	catch(...)
	{
		mysql_free_result(pRes);
		throw;
	}

	mysql_free_result(pRes);
	
	return 0;
}


// 结果集为0，返回1，其他情况返回0
int  
CMySQL::FetchResultMapVector(SqlMapVector & objMapVector) throw(CException)
{
	objMapVector.clear();
	
	//char szFiledValue[MAX_FILED_LENGTH] = {0};
	int iNumberFields;

	unsigned long *lengths;
	MYSQL_ROW row;
	MYSQL_FIELD * pField;
	
	MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
   	if (pRes == NULL)
    	{
       		throw CException(ERR_DB_NULL_RESULT, "Fetch result for non-select statement");
    	}
	try
	{
		int iRetNum = mysql_num_rows(pRes);

		if(iRetNum == 0)
		{
			mysql_free_result(pRes);
			return 1;
		}
	
		if(iRetNum != 1)
		{
			stringstream ssErrMsg;
			ssErrMsg << "mysql_num_rows is: " << iRetNum;
			throw CException(ERR_DB_MULTI_ROW, ssErrMsg.str());
		}

		lengths = mysql_fetch_lengths(pRes);
		while(NULL != (row = mysql_fetch_row(pRes)))  
		{
			KeyValueMap mResultSet;
			iNumberFields = 0;
			pField = mysql_fetch_fields(pRes);
			iNumberFields = mysql_num_fields(pRes);

		
			mResultSet.clear();
			for(int i = 0; i < iNumberFields; i++)
			{
				//gPtrAppLog->debug("i: %d,field: %s,   value: %s ", i, pField[i].name, ValiStr(row[i]));
				mResultSet[pField[i].name] = ValiStr(row[i]);
			}
			objMapVector.push_back(mResultSet);
		}
	}
	catch(...)
	{
		mysql_free_result(pRes);
		throw;
	}

	mysql_free_result(pRes);
	
	return 0;
}



// 结果集为0，返回1，其他情况返回0
int  
CMySQL::FetchResultVectorVectorMap(SqlVectorVectorMap & objVectorVectorMap) throw(CException)
{
	SqlMapVector objMapVector;
	objVectorVectorMap.clear();
	//char szFiledValue[MAX_FILED_LENGTH] = {0};
	int iNumberFields;

	unsigned long *lengths;
	MYSQL_ROW row;
	MYSQL_FIELD * pField;
	
	MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
   	if (pRes == NULL)
    	{
       		throw CException(ERR_DB_NULL_RESULT, "Fetch result for non-select statement");
    	}
	try
	{
		int iRetNum = mysql_num_rows(pRes);

		if(iRetNum == 0)
		{
			mysql_free_result(pRes);
			return 1;
		}

		lengths = mysql_fetch_lengths(pRes);
		while(NULL != (row = mysql_fetch_row(pRes)))
		{
			objMapVector.clear();
			KeyValueMap mResultSet;
			iNumberFields = 0;
			pField = mysql_fetch_fields(pRes);
			iNumberFields = mysql_num_fields(pRes);

//			gPtrAppLog->debug("iNumberFields : %d", iNumberFields);

			mResultSet.clear();
		
			for(int i = 0; i < iNumberFields; i++)
			{
//				gPtrAppLog->debug("i: %d,field: %s,   value: %s ", i, pField[i].name, ValiStr(row[i]));
				mResultSet[pField[i].name] = ValiStr(row[i]);
			}

			objMapVector.push_back(mResultSet);
			objVectorVectorMap.push_back(objMapVector);			
				
		}

	}
	catch(...)
	{
		mysql_free_result(pRes);
		throw;
	}

	mysql_free_result(pRes);
	
	return 0;
}


char *
CMySQL::ValiStr(char *str)
{
        if (str == NULL)
                return NSTR;
        else
                return str;
}

string 
CMySQL::GetSelectSQL(SqlResultSet & objMap, const string & strTableName, const string & strOther /* = "" */)
{
	stringstream ssSQL;

	ssSQL << " select * from "<< strTableName << " ";
	
	ssSQL << GetWhere(objMap);

	ssSQL << strOther;

	return ssSQL.str();
}

string
CMySQL::GetInsertSQL(SqlResultSet & objMap, const string & strTableName) throw(CException)
{
	if(objMap.empty())
		throw CException(ERR_DB_SQL_NOVALUE, "GetInsertSQL ValueMap is Empty");

	stringstream ssSQL;

	ssSQL << " insert into " << strTableName << " ";

	stringstream strKey;
	stringstream strValue;

	SqlResultSet::iterator it = objMap.begin();

	strKey << it->first ;
	strValue << " '" << EscapeStr(it->second) << "' ";

	++it;

	for(;it != objMap.end(); ++it)
	{
		strKey << "," << it->first << " ";
		strValue << ", '" << EscapeStr(it->second) << "' ";
	}

	ssSQL << "( " << strKey.str() << ") values (" << strValue.str() << ") ";

	return ssSQL.str();
}

string
CMySQL::GetUpdateSQL(SqlResultSet & objValueMap, SqlResultSet & objWhereMap, const string & strTableName) throw(CException)
{
	stringstream ssSQL;

	ssSQL << " update " << strTableName << " set ";

	SqlResultSet::iterator it = objValueMap.begin();

	ssSQL << it->first;
	ssSQL << "= '" << EscapeStr(it->second) << "' ";

	++it;

	for(;it != objValueMap.end(); ++it)
	{
		ssSQL << " , " << it->first;
		ssSQL << "= '" << EscapeStr(it->second) << "' ";
	}

	// 拼接条件
	ssSQL << GetWhere(objWhereMap);

	return ssSQL.str();
}

string
CMySQL::GetReplaceSQL(SqlResultSet & objMap, const string & strTableName) throw(CException)
{
	if(objMap.empty())
		throw CException(ERR_DB_SQL_NOVALUE, "GetReplaceSQL ValueMap is Empty");

	stringstream ssSQL;

	ssSQL << " replace into " << strTableName << " ";

	stringstream strKey;
	stringstream strValue;

	SqlResultSet::iterator it = objMap.begin();

	strKey << it->first ;
	strValue << " '" << EscapeStr(it->second) << "' ";

	++it;

	for(;it != objMap.end(); ++it)
	{
		strKey << "," << it->first << " ";
		strValue << ", '" << EscapeStr(it->second) << "' ";
	}

	ssSQL << "( " << strKey.str() << ") values (" << strValue.str() << ") ";

	return ssSQL.str();
}

// 拼接条件
string 
CMySQL::GetWhere(SqlResultSet & objWhereMap)
{
	// 拼接条件
	if(objWhereMap.empty())
		return "";

	stringstream ssSQL;
	
	ssSQL << " where ";

	SqlResultSet::iterator it = objWhereMap.begin();

	ssSQL << it->first << "='" << EscapeStr(it->second) << "' ";

	++it;

	for(;it != objWhereMap.end(); ++it)
	{
		ssSQL << " and " << it->first << "='" << EscapeStr(it->second) << "' ";
	}
	
	return ssSQL.str();
}

// 转义
string
CMySQL::EscapeStr(const string & buff)
{
    int len = buff.length();
    char szTmp[len*2 + 1];
    memset(szTmp, 0, sizeof(szTmp));
    mysql_escape_string(szTmp, buff.c_str(), buff.length());
    
    return string(szTmp);
}

