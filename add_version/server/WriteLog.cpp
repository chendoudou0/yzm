#include "WriteLog.h"
#include<iostream>

using  namespace std;

CWriteLog sysLog;
CWriteLog watLog;
CWriteLog statLog;


//功能:构造函数
CWriteLog::CWriteLog()
{
    nFileSize = 0;
    nLogWorkFlag = 0;
    nLogLevel = 0;
    nLogMaxSize = 0;

    mlevelinfo.clear();
	
    pstrLogFile = new char[300];
    sLogDate= new char[11];
}


//功能:析构函数
CWriteLog::~CWriteLog()
{
    delete []pstrLogFile;
}

		
int CWriteLog::Init(const string  strFileName, int nWorkFlag, int nlevel, long filesize )
{

    nLogWorkFlag = nWorkFlag;
    nLogLevel = nlevel;
    nLogMaxSize = filesize;
    sLogFilename = strFileName ;
    sLogDate = GetSystemDate();

    mlevelinfo[1] = "Debug";
    mlevelinfo[2] = "Normal";
    mlevelinfo[3] = "Warning";
    mlevelinfo[4] = "Error";
    mlevelinfo[5] = "Fatal";

    memset(pstrLogFile, 0x00, 300);
    sprintf(pstrLogFile, "%s.%s",sLogFilename.c_str(), 	 sLogDate);

    logLock.Init();                                              
    return 0;
}


//功能:输出一条日志信息
//  nLogLevel - [in]该日志信息的级别
//  logMsg - [in]日志信息的格式串
int CWriteLog::WriteLog(int nlevel, const char *pszFormat, ...)
{
    int nResult;        //函数的返回结果
    
    if ((nLogLevel > nlevel))
    {
        return 0;
    }
    	
    if( nLogWorkFlag==0 )
    {
        return 0;
    }
	
    va_list argList;
    
    va_start(argList, pszFormat);
    
    nResult = OutputLog(nlevel, pszFormat, argList);
    
    va_end(argList);
    
    return nResult;
}


//功能:输出一条日志信息 
int CWriteLog::OutputLog( int nlevel, const char * pszFormat, va_list argList)
{
    
    char Content[1024*10];
    char LogContent[1024*16];

    memset(Content, 0x00, sizeof(Content));
    memset(LogContent, 0x00, sizeof(LogContent));

    vsprintf(Content, pszFormat, argList);

    if ((nLogLevel <= nlevel))
    {
        if(nlevel >=1 && nlevel<=5) 
        {
             sprintf(LogContent, "%s [%s] %s",GetSystemTime(), mlevelinfo[nlevel].c_str(), Content);
        }
        else
	 {
             sprintf(LogContent, "%s  %s",GetSystemTime(), Content);
	 }
    }
    else
    {
        return 0;
    }

	if(1 == nlevel)
	{
       cout<<LogContent<<endl;
	}

   // int nContentLen = strlen(Content);
   // nFileSize = nFileSize + nContentLen;

    char *  sCurDate = GetSystemDate();
    // if(nFileSize > nLogMaxSize)

    if(strcmp(sLogDate , sCurDate)!=0)
    {
        //fclose(m_fp);
        memset(pstrLogFile, 0x00, sizeof(pstrLogFile));
        sprintf(pstrLogFile, "%s.%s",sLogFilename.c_str(),sCurDate);
    }

    logLock.Lock();
    m_fp = fopen(pstrLogFile, "a+");
    if( m_fp==NULL )
    {
         printf("fopen file fail: %s\n", sLogFilename.c_str());
         exit(1); //中止程序 
    }


    //fwrite( cInfo, LogContent,1,m_fp);
    fprintf(m_fp,"%s", LogContent);
    fclose(m_fp);

    logLock.unLock();
    return 0;
}


