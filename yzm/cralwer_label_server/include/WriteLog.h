#ifndef _WRITELOG_H_
#define _WRITELOG_H_


#include <stdarg.h>
#include <string.h>
#include <map>

#include "Mutex.h"
#include "Common.h"

using namespace std;

enum LogLevel
{
    levelDebug                                = 1,                //调试级别(在调用程序时使用,输出较多的调试信息)
    levelNormal                                = 2,                //普通信息级别(一般的信息)
    levelWarning                               = 3,                //警告信息级别(告警,系统可能有错误发生)
    levelError                                       = 4,                //错误信息级别(告警,系统有确定的一般错误发生)
    levelFatalError                        = 5                //致命错误信息级别(告警,系统存在致命错误,会对系统的稳定运行产生影响)
};

class CWriteLog
{
  public:

    CWriteLog();
    virtual ~CWriteLog();
    int Init(const string  strFileName, int nWorkFlag, int nlevel, long filesize );
        
    //功能:日志输入函数(参数及功能同CWriteLog类的同名函数)
    int WriteLog(int nlevel, const char *pszFormat, ...);
    int OutputLog(int nlevel, const char * pszFormat, va_list argList);

  private:
          FILE * m_fp;
          char * pstrLogFile;
          cMutex logLock;

        long nFileSize;

        int nLogWorkFlag;
        int nLogLevel ;
        long nLogMaxSize;
        string sLogFilename;
         char * sLogDate;
         map <int ,string > mlevelinfo;

};

#endif
