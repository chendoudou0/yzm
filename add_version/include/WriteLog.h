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
    levelDebug                                = 1,                //���Լ���(�ڵ��ó���ʱʹ��,����϶�ĵ�����Ϣ)
    levelNormal                                = 2,                //��ͨ��Ϣ����(һ�����Ϣ)
    levelWarning                               = 3,                //������Ϣ����(�澯,ϵͳ�����д�����)
    levelError                                       = 4,                //������Ϣ����(�澯,ϵͳ��ȷ����һ�������)
    levelFatalError                        = 5                //����������Ϣ����(�澯,ϵͳ������������,���ϵͳ���ȶ����в���Ӱ��)
};

class CWriteLog
{
  public:

    CWriteLog();
    virtual ~CWriteLog();
    int Init(const string  strFileName, int nWorkFlag, int nlevel, long filesize );
        
    //����:��־���뺯��(����������ͬCWriteLog���ͬ������)
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
