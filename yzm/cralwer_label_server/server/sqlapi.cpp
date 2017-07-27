#include "sqlapi.h"
#include "glog/logging.h"

using namespace std;


/**
 * Description: 构造函数
 * Input:         host         数据库主机IP地址
 *                   user      数据库用户ID
 *                   passwd    数据库用户密码
 *                   iOverTime 连接超时时长
 */
CMySQL::CMySQL(std::string host, std::string user, std::string pswd, int port, int overTime)
:host_(host),
user_(user),
passwd_(pswd),
port_(port),
overTime_(overTime)
{       
}

CMySQL::~CMySQL()
{
    Close();
}

bool  CMySQL::Connect() 
{
    // 初始化连接句柄
    if(m_SqlHandle == nullptr)
    {
        m_SqlHandle = new MYSQL;
    }
    
    mysql_init(m_SqlHandle);
    
    // 设置连接选项
    mysql_options(m_SqlHandle, MYSQL_OPT_CONNECT_TIMEOUT,  (char *)&overTime_);   // 超时时长
    //my_bool reconnect = 0;
    //mysql_options(m_SqlHandle, MYSQL_OPT_RECONNECT, &reconnect);	                // 禁止自动重连
    if (!mysql_real_connect(m_SqlHandle, host_.c_str(), user_.c_str(),
	passwd_.c_str(), NULL, port_, NULL, 0))
    {
		LOG(ERROR) << "connect mysql failed!!!!";
        return  false;
    }

    // 设置连接成功标志
    m_connFlag = true;
   
	LOG(INFO) << "connect mysql  success!!!!";

	return  true;

}

/**
 * Description: 关闭数据库
 */
void  CMySQL::Close()
{
    if (m_SqlHandle) 
    { 
        mysql_close(m_SqlHandle);
        delete m_SqlHandle;
        m_SqlHandle = NULL;
    }        

    m_connFlag = false;
}


bool CMySQL::Ping()
{
    return mysql_ping(m_SqlHandle) == 0;
}  

bool CMySQL::Query(const char *query, unsigned int length) 
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
			LOG(ERROR) << "mysql lost or go away";
        }
        else
        {
            m_connFlag = false;
            LOG(ERROR) << "unknowed mysql error....";
        }
		return  false;
    }

	return true;
}

/**
 * Description: 执行事务开始语句
 * Return:       失败抛出异常
 */
bool CMySQL::Begin() 
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
		LOG(ERROR) << "CMySQL::Begin() failed";
        return  false;
    }

	return true;
}

/**
 * Description: 执行事务提交
 * Return:       失败抛出异常
 */
bool CMySQL::Commit() 
{
    // 提交事务
    if(mysql_real_query(m_SqlHandle, "COMMIT", sizeof("COMMIT")) != 0)
    {
        LOG(ERROR) << "CMySQL::Commit() failed";
        return  false;
    };

	return  true;   
}

/**
 * Description: 执行事务回滚
 * Return:      失败抛出异常
 */
bool CMySQL::Rollback() 
{
    // 回滚事务
    if(mysql_real_query(m_SqlHandle, "ROLLBACK", sizeof("ROLLBACK")) != 0)
    {
        LOG(ERROR) << "CMySQL::Rollback() failed";
        return  false;
    }

	return true;
}

/**
 * Description: 获取SQL影响的行数
 * Return:      失败抛出异常
 */
int64_t CMySQL::AffectedRows() 
{
    int64_t rownum = mysql_affected_rows(m_SqlHandle);
   
    return rownum;
}

/**
 * Description: 取结果集
 * Return:      失败抛出异常
 */
MYSQL_RES* CMySQL::FetchResult() 
{
    MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
    if (pRes == NULL)
    {
         LOG(ERROR) << "CMySQL::FetchResult() failed";
    }
    
    return pRes;
}

int 
CMySQL::FetchRows()
{
	MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
    if (pRes == NULL)
    {
        LOG(ERROR) << "CMySQL::FetchResult() failed";
		return 0;
    }

	int iRet = mysql_num_rows(pRes);
	
	mysql_free_result(pRes);
	
	return iRet;
}

// 结果集为0，返回1，其他情况返回0
bool CMySQL::FetchResultMap(SqlResultSet & objMap) 
{
	bool ret = false;
	// 清空以前map
	objMap.clear();
	
	int iNumberFields;

	unsigned long *lengths;
	MYSQL_ROW row;
	MYSQL_FIELD * pField;
	MYSQL_RES* pRes = nullptr;

	do{
		MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
   		if (pRes == nullptr)
		{
			 break;
		}
	
		int iRetNum = mysql_num_rows(pRes);

		if(iRetNum != 1)
		{
			break;
		}
	
		iNumberFields = mysql_num_fields(pRes);
		row = mysql_fetch_row(pRes);
		pField = mysql_fetch_fields(pRes);

		for(int i = 0; i < iNumberFields; i++)
		{
			objMap[pField[i].name] = row[i];
		}
		ret = true;

	}while(0);
	
	mysql_free_result(pRes);
	
	return ret;
}


// 结果集为0，返回1，其他情况返回0
//VECTOR里面放着MAP
  
int CMySQL::FetchResultMVector(SqlMapVector & objMapVector) 
{
	int ret = -1;              //SQL语句执行失败      
	objMapVector.clear();
	
	int iNumberFields;
	MYSQL_ROW row;
	MYSQL_FIELD * pField;
	MYSQL_RES* pRes = nullptr;
	do{
		pRes = mysql_store_result(m_SqlHandle);
   		if (pRes == nullptr)
    	{
			LOG(ERROR) << "CMySQL::FetchResultMVector mysql_store_result failed";
			break;
   		}

		int iRetNum = mysql_num_rows(pRes);

		if(iRetNum == 0)
		{
			LOG(ERROR) << "mysql_num_rows is  0";
			ret = -2;                             
			break;
		}

		while(NULL != (row = mysql_fetch_row(pRes)))
		{
			KeyValueMap mResultSet;
			iNumberFields = 0;
			pField = mysql_fetch_fields(pRes);
			iNumberFields = mysql_num_fields(pRes);  
		
			mResultSet.clear();
			for(int i = 0; i < iNumberFields; i++)
			{
				string fieldName = pField[i].name;
				fieldName = fieldName.substr(1, fieldName.length()-1);
				mResultSet[fieldName] = row[i];
			}
			objMapVector.push_back(mResultSet);
		}

		ret = 0;   

	}while(0);
	
	mysql_free_result(pRes);
	
	return  ret;
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

bool CMySQL::FetchMd5Map(KeyValueMap& outMap) 
{
	bool ret = false;
	outMap.clear();
	int iNumberFields;
	MYSQL_RES* pRes = nullptr;
	MYSQL_ROW row;
	MYSQL_FIELD * pField;
	do{
		pRes = mysql_store_result(m_SqlHandle);
		if (pRes == NULL)
		{
			break;
		}
		
		int iRetNum = mysql_num_rows(pRes);

		if(iRetNum == 0)
		{
			break;
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
					filePath = row[i];
				}
				if(fieldName == "Fmd5")
				{
					md5 = row[i];
				}
			}
			outMap[filePath] = md5; 
		}

		ret = true;

	}while(0);

	mysql_free_result(pRes);
	
	return ret;
}
