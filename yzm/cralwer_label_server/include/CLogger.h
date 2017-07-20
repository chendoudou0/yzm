#ifndef _CFT_LOG_NEW_H_
#define _CFT_LOG_NEW_H_

#include <stdio.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdarg.h>
#include <mutex>

using std::string;

const int ERR_SIZE = 1024;  // ������Ϣ����
const int MAX_BUF = 10240; // ������־��󳤶�
const int SHIFT_FREQ = 128;  // ��־�л�Ƶ��

/**
 * ��־��
 */
class CLogger
{
public:
    
    /**
     * ��־����
     */
    enum LOG_LEVEL
    {
        _NONE = 1, 
        _ERROR = 2,  
        _WARNING = 3,   
        _INFO = 4, 
        _DEBUG = 5 
    };

    /**
     * ��־�л�ģʽ
     */
    enum SHIFT_MODE
    {
        _SIZE_MODE = 1,  // ����С�л�
        _DATE_MODE = 2  // �������л�
    };
    
public:
    
    /**
     * ���캯��
     */
    CLogger(const char* path, int max_size, int max_file = 10,LOG_LEVEL level = _DEBUG, SHIFT_MODE=_SIZE_MODE);

    /**
     * ��������
     */
    virtual ~CLogger();

    /**
     * ������־����
     */
    inline void setLogLevel(LOG_LEVEL level)    { _level = level; }

    /**
     * ������Ϣ���к�
     */
    inline void setMsgId(const string& strMsg)      { _msg_id = strMsg; }

    /**
     * �����ļ�����׺
     */
    inline void setSuffix(const string& suffix) { _suffix = suffix; }

    /**
     * ��ȡ��־ϵͳ������Ϣ
     */
    inline const char* getErrorInfo()    { return _szErrInfo; }

    /**
     * ��ӡ����log
     */
    int error(const char *fmt, ...);
    
    /**
     * ��ӡ�澯log
     */
    int warning(const char *fmt, ...);
    
    /**
     * ��ӡ����log
     */
    //int normal(const char *fmt, ...);
    int info(const char *fmt, ...);

    /**
     * ��ӡ����log
     */
    int debug(const char *fmt, ...);

    /**
     * ֱ��д��־
     */
    int raw(const char *fmt, ...);

protected:
    
    /**
     * ���ļ�
     */
    int _open();

    /**
     * �ر�
     */
    void _close();

    /**
     * �л���־�ļ�
     */
    int _shift();

    /**
     * ��¼��־
     */
    int _write(const char *szLog, int len);

    /**
     * ȷ����־�ļ���
     */
    string _file_name(int index=0);

protected:

    /**
     * �л�ģʽ
     */
    SHIFT_MODE _shift_mode;
    
    /**
     * �ļ�ָ��
     */
    int  _fd;

    /**
     * �ļ�·��
     */
    string _path;

    /**
     * ����ļ���С
     */
    int _max_size;

    /**
     * ����ID
     */
    int _proc_id;

    /**
     * ��ǰϵͳʱ��
     */
    struct tm _tm_now;

    /**
     * ��Ϣ���к�
     */
    string _msg_id;

    /**
     * �ļ�����׺
     */
    string _suffix;
    
    /**
     * ����ļ�����
     */
    int _max_file;

    /**
     * ��־����
     */
    LOG_LEVEL _level;

    /**
     * ��ȡ������Ϣ
     */
    char _szErrInfo[ERR_SIZE + 1];
	std::mutex  _lock;
};

#endif

