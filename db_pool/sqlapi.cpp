#include "sqlapi.h"
#include "glog/logging.h"


/**
 * Description: ���캯��
 * Input:         host         ���ݿ�����IP��ַ
 *                   user      ���ݿ��û�ID
 *                   passwd    ���ݿ��û�����
 *                   iOverTime ���ӳ�ʱʱ��
 */
CMySQL::CMySQL(DB_PARAM  db_pa)
:db_pa_(db_pa)
{       
}

CMySQL::~CMySQL()
{
    Close();
}

bool  CMySQL::Connect() 
{
    // ��ʼ�����Ӿ��
    if(m_SqlHandle == nullptr)
    {
        m_SqlHandle = new MYSQL;
    }
    
    mysql_init(m_SqlHandle);
    
    // ��������ѡ��
    mysql_options(m_SqlHandle, MYSQL_OPT_CONNECT_TIMEOUT,  (char *)&db_pa_.overTime_);   // ��ʱʱ��
    //my_bool reconnect = 0;
    //mysql_options(m_SqlHandle, MYSQL_OPT_RECONNECT, &reconnect);	                // ��ֹ�Զ�����
    if (!mysql_real_connect(m_SqlHandle, db_pa_.host_.c_str(), db_pa_.user_.c_str(),
	db_pa_.pass_.c_str(), NULL, db_pa_.port_, NULL, 0))
    {
		LOG(ERROR) << "connect mysql failed!!!!";
        return  false;
    }

    // �������ӳɹ���־
    m_connFlag = true;
   
	LOG(INFO) << "mysql connect success!!!!";

	return  true;

}

/**
 * Description: �ر����ݿ�
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
        // �ж��Ƿ�Ϊ���ݿ����
        if (mysql_errno(m_SqlHandle) == CR_SERVER_LOST || mysql_errno(m_SqlHandle) == CR_SERVER_GONE_ERROR)
        {
            m_connFlag = false;
			LOG(ERROR) << "mysql lost";
        }
        else
        {
            m_connFlag = false;
            LOG(ERROR) << "unknowed mysql error";
        }
		return  false;
    }

	return true;
}

/**
 * Description: ִ������ʼ���
 * Return:       ʧ���׳��쳣
 */
int CMySQL::Begin() 
{
    // �����ڶ���״̬�����������
    if (!m_connFlag)
    {
        Close();
        Connect();
    }

    // ��ʼ����
    if(mysql_real_query(m_SqlHandle, "BEGIN", sizeof("BEGIN")) != 0)
    {
		LOG(ERROR) << "CMySQL::Begin() failed";
        return -1;
    };

	return 0;
}

/**
 * Description: ִ�������ύ
 * Return:       ʧ���׳��쳣
 */
int CMySQL::Commit() 
{
    // �ύ����
    if(mysql_real_query(m_SqlHandle, "COMMIT", sizeof("COMMIT")) != 0)
    {
        LOG(ERROR) << "CMySQL::Commit() failed";
        return -1;
    };

	return 0;
}

/**
 * Description: ִ������ع�
 * Return:      ʧ���׳��쳣
 */
int CMySQL::Rollback() 
{
    // �ع�����
    if(mysql_real_query(m_SqlHandle, "ROLLBACK", sizeof("ROLLBACK")) != 0)
    {
        LOG(ERROR) << "CMySQL::Rollback() failed";
        return -1;
    };

	return 0;
}

/**
 * Description: ��ȡSQLӰ�������
 * Return:      ʧ���׳��쳣
 */
int64_t CMySQL::AffectedRows() 
{
    int64_t rownum = mysql_affected_rows(m_SqlHandle);
   
    return rownum;
}

/**
 * Description: ȡ�����
 * Return:      ʧ���׳��쳣
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
    }

	int iRet = mysql_num_rows(pRes);
	
	mysql_free_result(pRes);
	
	return iRet;
}

/*
int  
CMySQL::FetchResultRow(MYSQL_ROW &row)
{
	row = NULL;
	
	MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
   	if (pRes == NULL)
    {
       	 LOG(ERROR) << "CMySQL::FetchResultRow() mysql_store_result failed";
		 return -1;
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

*/

// �����Ϊ0������1�������������0
int  
CMySQL::FetchResultMap(SqlResultSet & objMap) 
{
	// �����ǰmap
	objMap.clear();
	
	int iNumberFields;

	unsigned long *lengths;
	MYSQL_ROW row;
	MYSQL_FIELD * pField;
	MYSQL_RES* pRes = nullptr;
	int ret = -1;

	do{
		MYSQL_RES* pRes = mysql_store_result(m_SqlHandle);
   		if (pRes == nullptr)
		{
			 break;
		}
	
		int iRetNum = mysql_num_rows(pRes);

		if(iRetNum == 0 || iRetNum != 1)
		{
			break;
		}
	
		iNumberFields = mysql_num_fields(pRes);
		row = mysql_fetch_row(pRes);
		pField = mysql_fetch_fields(pRes);

		for(int i = 0; i < iNumberFields; i++)
		{
			LOG(INFO) << " [FetchResultMap]i: " <<i
			<<" field: "<<pField[i].name<<" value:  "<<row[i];
			objMap[pField[i].name] = row[i];
		}
		ret = 0;

	}while(0);
	
	mysql_free_result(pRes);
	
	return ret;
}


// �����Ϊ0������1�������������0
//VECTOR�������MAP
int  
CMySQL::FetchResultMVector(SqlMapVector & objMapVector) 
{
	objMapVector.clear();
	
	int iNumberFields;
	MYSQL_ROW row;
	MYSQL_FIELD * pField;
	MYSQL_RES* pRes = nullptr;
	int ret = -1;
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
			LOG(ERROR) << "mysql_num_rows return is NULL";
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
				LOG(INFO) << "i : "<<i
				<<" field: "<< pField[i].name
				<<" value: "<< row[i];
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

// ת��
string
CMySQL::EscapeStr(const string & buff)
{
    int len = buff.length();
    char szTmp[len*2 + 1];
    memset(szTmp, 0, sizeof(szTmp));
    mysql_escape_string(szTmp, buff.c_str(), buff.length());
    
    return string(szTmp);
}
