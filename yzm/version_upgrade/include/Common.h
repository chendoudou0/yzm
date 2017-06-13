#ifndef  COMMON_H_
#define  COMMON_H_
#include <sys/stat.h>

#include <netinet/in.h>    // for sockaddr_in 
#include <sys/types.h>    // for socket 
#include <sys/socket.h>    // for socket 
#include <stdio.h>        // for printf 
#include <stdlib.h>        // for exit 
#include <string.h>        // for bzero 
#include <time.h>                //for time_t and time 
#include <sys/timeb.h>
#include <arpa/inet.h> 
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h> 
#include <dirent.h>
#include<vector>

//htons, htonlͷ�ļ���
#include<netinet/in.h>
#include "stdio.h"
#include <string>
#include <iostream>
#include <signal.h>
#include <map>
#include <sstream>

#define MAXFD 64

using namespace std;

void SetNonBlocking( int nSock );
int string2int(string &paraStr, int &paraInt);
int int2string(int paraInt, string &paraStr);

void creat_daemon(void);
bool callSystemCmd(string cmd);
vector<string> listDir(const char *path);  
bool generateMD5(string& path, string& strMD5);
bool getFileSize(string& inPath, int& inSize);

struct RemoteSvr
{
    void * m_pBaseWork;
    int PortId;
    int nPort;
    string strIP;
    int nMaxSocketId;
};

typedef struct DatabaseInfo
{
    int datatype;
    int    index;
    char dbmaster[16];
    char dbslave[16];
    int    dbport;
    char dbname[12];
    char dbuser[12];
    char dbpasswd[20];
    char dbtype[20];
}DATABASEINFO;

//����
typedef struct SecurityInfo
{
    char type[10]; 
    int black_check;
    int area_check;
    int amt_limit_check;
    int auto_recovery;
}SECURITYINFO;


//����
typedef struct LogInfo
{
    char type[8];
    char log_file[20];
    int log_level; 
    int max_size;
}LOGINFO;

//�ϴ�
typedef struct ThreadInfo
{
    int thread_num;
    int dbconn_num;
}THREADINFO;




enum portType
{
        TcpPort,
        ClientPort
};


#pragma pack(1)
typedef struct sPacketInfo
{
        sPacketInfo()
        {
            memset(this, 0, sizeof(sPacketInfo));
        }
        int nSocketId;
        int nBufferId;
        int cPort;
        char cCmd;
}sPacketInfo;

typedef struct sMsgInfo
{
        sPacketInfo oPacketInfo;
        string strMsg;
}sMsgInfo;

#pragma pack()


int myCpy( char *desc , char *src, int nLen );

void PrintHex(char *pBuf, int nBufLen);
void PrintDecimal(char *pBuf, int nBufLen);
void PrintAlpha(char *pBuf, int nBufLen);
int ConnectRemoteServer(string strIp, int nPort );
char * GetSystemDate();
char *  GetSystemTime();
//void GetTimeForFile(char time[24]);



////////////////////////////////////////////////////////////////////////////////////////////////////
class  HttpParamHelper
{
public:
	HttpParamHelper();
	virtual ~HttpParamHelper();

	
	//**********************************************************
	// Method   : setHostPath
	// Remark   : ����Host��Pathֵ
	// DateTime : 2015/08/24
	// Parameter: const std::string & host			URL�е�Host�磺 192.168.1.214
	// Parameter: const std::string & path			URL�е�Path�磺 /index.php
	// Returns  : void
	//**********************************************************
	void	setHostPath(const std::string &host, const std::string &path);
	const std::string&	getHost() const;
	const std::string&	getPath() const;
	
public:
	
	//**********************************************************
	// Method   : reset
	// Remark   : ���裨��������С���ֵ��������Host��Path���ᱻ���
	// DateTime : 2015/08/24
	// Returns  : void
	//**********************************************************
	virtual void	reset();

	
	//**********************************************************
	// Method   : setParam
	// Remark   : д���ֵ���ṩ3�г������ͣ������ֵ������Ḳ�Ǿɼ�ֵ��
	// DateTime : 2015/08/24
	// Parameter: const std::string & key			��Key
	// Parameter: int val							ֵValue
	// Returns  : bool
	//**********************************************************
	bool	setParam(const std::string &key, int val);
	bool	setParam(const std::string &key, bool val);
	bool	setParam(const std::string &key, const std::string &val);
	bool	setParam(const std::string &key, const char *val);

	
	//**********************************************************
	// Method   : getParam
	// Remark   : ��ȡ��ֵ��ֻ�ṩ�ַ�����ʽ���û����Խ���StrT��������ת��
	// DateTime : 2015/08/24
	// Parameter: const std::string & key			��Key
	// Returns  : const std::string&				ֵValue
	//**********************************************************
	const std::string&	getParam(const std::string &key) const;

	
	//**********************************************************
	// Method   : hasParam
	// Remark   : �ж��Ƿ����ָ����ֵ����ȫƥ�䣩
	// DateTime : 2015/08/24
	// Parameter: const std::string & key			��Key
	// Returns  : bool
	//**********************************************************
	bool	hasParam(const std::string &key) const;
	
	//**********************************************************
	// Method   : delParam
	// Remark   : ɾ��ָ����ֵ
	// DateTime : 2015/08/24
	// Parameter: const std::string & key			��Key
	// Returns  : bool
	//**********************************************************
	bool	delParam(const std::string &key);
	
	
	//**********************************************************
	// Method   : getFullUrl
	// Remark   : ��������URL��URL��Ҫ��&key=value�ķ�ʽ��ɣ������������£�
	//		http://192.168.1.214/api/index.php?ver=1.0&time=1440417938&key=test&action=app_update&platform=ios
	// DateTime : 2015/08/24
	// Returns  : std::string
	//**********************************************************
	std::string		getFullUrl() const;

	
	//**********************************************************
	// Method   : generateGetParamString
	// Remark   : ������ֵ�������ض���ʽ��ƴ�Ӵ�
	// DateTime : 2015/08/24
	// Parameter: const std::string & firstPrefix		ƴ�Ӵ�ǰ׺
	// Parameter: const std::string & otherPrefix		��ֵ�����
	// Parameter: const std::string & operPrefix		��ֵ��ֵ��
	// Returns  : std::string
	//**********************************************************
	std::string		generateGetParamString(const std::string &firstPrefix = "?",
											const std::string &otherPrefix = "&",
											const std::string &operPrefix = "=") const;
	//**********************************************************
	// Method   : setFuctionName
	// Remark   : ���ú�������
	// DateTime : 2015/08/24
	// Parameter: std::string name		��������
	//**********************************************************
	void            setFunctionName(std::string name);
protected:
	typedef		std::map<std::string, std::string>		PARAM_MAP;

	PARAM_MAP		paramMap_;		// ��ֵӳ�伯��
	std::string		apiHost_;		// URL�е�Host�磺 192.168.1.214
	std::string		apiPath_;		// URL�е�Path�磺 /index.php
	std::string		name_;
};

/************************************************************************
  *********************       PostParamHelper        *********************
************************************************************************/
class  PostParamHelper : public HttpParamHelper
{
public:
	PostParamHelper();
	virtual ~PostParamHelper();
	
	
	//**********************************************************
	// Method   : reset
	// Remark   : ���裨����մ��ϴ��ļ��б�
	// DateTime : 2015/08/24
	// Returns  : void
	//**********************************************************
	virtual void	reset();
	
	
	//**********************************************************
	// Method   : setPostDataString
	// Remark   : �����Զ���Post����������
	// DateTime : 2015/08/24
	// Parameter: const std::string & data			�Զ�������
	// Returns  : void
	//**********************************************************
	void	setPostDataString(const std::string &data);
	bool	getPostDataString(std::string *data) const;

	
	//**********************************************************
	// Method   : addFile
	// Remark   : ���ָ���ļ������ϴ��ļ��б�
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			�ļ�·��
	// Parameter: const std::string & name			�Զ����ļ���
	// Returns  : bool
	//**********************************************************
	bool	addFile(const std::string &path, const std::string &name = "");
	
	//**********************************************************
	// Method   : delFile
	// Remark   : �Ӵ��ϴ��ļ��б��Ƴ�ָ���ļ�
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			�ļ�·��
	// Returns  : bool
	//**********************************************************
	bool	delFile(const std::string &path);
	
	//**********************************************************
	// Method   : hasFile
	// Remark   : �жϴ��ϴ��ļ��б��Ƿ����ָ���ļ�����ȫƥ�䣩
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			�ļ�·��
	// Returns  : bool
	//**********************************************************
	bool	hasFile(const std::string &path) const;

private:
	const char*		getContentTypeByFilename(const std::string &fileName) const;

private:
	typedef		std::map<std::string/*Path*/, std::string/*Name*/>	FILE_LIST;
	FILE_LIST	fileList_;			// Ҫ�ϴ����ļ��б�
	std::string	customData_;		// �Զ���Post���ݣ�Post���������壩
	bool		isCustomData_;		// �Ƿ���Post�Զ�������
};




/************************************************************************
  ***********************       HttpHelper        ***********************
************************************************************************/
class HttpHelper
{
public:

    static bool     checkNetworking(const std::string& url);
	
	
	static bool		submitPage(const PostParamHelper &helper, std::string *html);
	
	//**********************************************************
	// Method   : uploadFile
	// Remark   : �ļ��ϴ���multipart/form-data��ʽ��
	// DateTime : 2015/08/24
	// Parameter: const std::string & url			�ϴ��ӿ�URL
	// Parameter: std::string * html				�ϴ��ӿڷ��ص�html
	// Parameter: const std::string & filePath		��Ҫ�ϴ����ļ�·��
	// Parameter: const std::string & nickName		�Զ����ļ�������ѡ��
	// Returns  : bool
	//**********************************************************
	static bool		uploadFile(const std::string &url, 
							std::string *html, 
							const std::string &filePath,
							const std::string &nickName = "",
                            int timeout = 6);

	
	
	//**********************************************************
	// Method   : formatURL
	// Remark   : ��ʽ��URL��ȥ�����������/б�ܣ�
	// DateTime : 2015/08/24
	// Parameter: std::string * url					url��ַ
	// Returns  : void
	//**********************************************************
	static void			formatURL(std::string *url);
	
	//**********************************************************
	// Method   : formatURL_C
	// Remark   : ��ʽ��URL��ȥ�����������/б�ܣ�
	// DateTime : 2015/08/24
	// Parameter: const std::string & url			url��ַ
	// Returns  : std::string				��ʽ�����URL
	//**********************************************************
	static std::string	formatURL_C(const std::string &url);

	
	//**********************************************************
	// Method   : setDebug
	// Remark   : ���õ���״̬������ʱ������־���������http�������ݣ�
	// DateTime : 2015/08/24
	// Parameter: bool isDebug						�Ƿ�������
	// Returns  : void
	//**********************************************************
	static void			setDebug(bool isDebug);


private:
	typedef		size_t (*WRITE_FUNC)(void*, size_t, size_t, void*);

	// Get��Post�Ļص��������ֱ��ǰ���վ���ص�����д�뵽�ڴ����д�뵽�����ļ���
	static size_t	onWriteMemory(void *buffer, 
								size_t size, 
								size_t nmemb, 
								void *ptr);

	static size_t	onWriteFile(void *buffer, 
								size_t size, 
								size_t nmemb, 
								void *ptr);
	

	// Get����ʵ�֣���Ϊhttp��https����
	static bool		getMethod(const std::string &url, 
							void *param, 
							WRITE_FUNC func);
	
	static bool		getMethod_ssl(const std::string &url, 
								void *param, 
								WRITE_FUNC func,
								const std::string &caPath = "");


	// Post����ʵ�֣���Ϊhttp��https����
	static bool		postMethod(const std::string &url, 
							const std::string &postData, 
							void *param, 
							WRITE_FUNC func,
                            int timeout = 6);

	static bool		postMethod_ssl(const std::string &url, 
								const std::string &postData, 
								void *param, WRITE_FUNC func,
								const std::string &caPath = "");


private:
	HttpHelper();
	static bool	s_isDebug;		// �Ƿ�������ģʽ
};


/************************************************************************
  *************************       MD5        *************************
************************************************************************/
class  MD5
{
private:
    #define uint8  unsigned char
    #define uint32 unsigned long int

    struct md5_context
    {
        uint32 total[2];
        uint32 state[4];
        uint8 buffer[64];
    };

    void starts( struct md5_context *ctx );
    void process( struct md5_context *ctx, uint8 data[64] );
    void update( struct md5_context *ctx, uint8 *input, uint32 length );
    void finish( struct md5_context *ctx, uint8 digest[16] );

public:
    void generate(unsigned char* buffer,int bufferlen);
    void generate(const std::string &buffer);

    MD5();
    MD5(const char *md5src);
    MD5(unsigned long *md5src);

    MD5 operator+(const MD5 &adder);
    bool operator==(const MD5 &cmper) const;

    std::string toString();

    unsigned long m_data[4];
};





#endif

