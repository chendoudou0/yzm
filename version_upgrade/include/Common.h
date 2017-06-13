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

//htons, htonl头文件：
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

//下载
typedef struct SecurityInfo
{
    char type[10]; 
    int black_check;
    int area_check;
    int amt_limit_check;
    int auto_recovery;
}SECURITYINFO;


//汇总
typedef struct LogInfo
{
    char type[8];
    char log_file[20];
    int log_level; 
    int max_size;
}LOGINFO;

//上传
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
	// Remark   : 设置Host和Path值
	// DateTime : 2015/08/24
	// Parameter: const std::string & host			URL中的Host如： 192.168.1.214
	// Parameter: const std::string & path			URL中的Path如： /index.php
	// Returns  : void
	//**********************************************************
	void	setHostPath(const std::string &host, const std::string &path);
	const std::string&	getHost() const;
	const std::string&	getPath() const;
	
public:
	
	//**********************************************************
	// Method   : reset
	// Remark   : 重设（会清空所有‘键值’，但是Host和Path不会被清空
	// DateTime : 2015/08/24
	// Returns  : void
	//**********************************************************
	virtual void	reset();

	
	//**********************************************************
	// Method   : setParam
	// Remark   : 写入键值，提供3中常用类型（如果键值存在则会覆盖旧键值）
	// DateTime : 2015/08/24
	// Parameter: const std::string & key			键Key
	// Parameter: int val							值Value
	// Returns  : bool
	//**********************************************************
	bool	setParam(const std::string &key, int val);
	bool	setParam(const std::string &key, bool val);
	bool	setParam(const std::string &key, const std::string &val);
	bool	setParam(const std::string &key, const char *val);

	
	//**********************************************************
	// Method   : getParam
	// Remark   : 获取键值，只提供字符串方式，用户可以借助StrT进行数据转换
	// DateTime : 2015/08/24
	// Parameter: const std::string & key			键Key
	// Returns  : const std::string&				值Value
	//**********************************************************
	const std::string&	getParam(const std::string &key) const;

	
	//**********************************************************
	// Method   : hasParam
	// Remark   : 判断是否包含指定键值（完全匹配）
	// DateTime : 2015/08/24
	// Parameter: const std::string & key			键Key
	// Returns  : bool
	//**********************************************************
	bool	hasParam(const std::string &key) const;
	
	//**********************************************************
	// Method   : delParam
	// Remark   : 删除指定键值
	// DateTime : 2015/08/24
	// Parameter: const std::string & key			键Key
	// Returns  : bool
	//**********************************************************
	bool	delParam(const std::string &key);
	
	
	//**********************************************************
	// Method   : getFullUrl
	// Remark   : 生成完整URL（URL主要有&key=value的方式组成，返回例子如下）
	//		http://192.168.1.214/api/index.php?ver=1.0&time=1440417938&key=test&action=app_update&platform=ios
	// DateTime : 2015/08/24
	// Returns  : std::string
	//**********************************************************
	std::string		getFullUrl() const;

	
	//**********************************************************
	// Method   : generateGetParamString
	// Remark   : 遍历键值，生成特定格式的拼接串
	// DateTime : 2015/08/24
	// Parameter: const std::string & firstPrefix		拼接串前缀
	// Parameter: const std::string & otherPrefix		键值间隔符
	// Parameter: const std::string & operPrefix		键值赋值符
	// Returns  : std::string
	//**********************************************************
	std::string		generateGetParamString(const std::string &firstPrefix = "?",
											const std::string &otherPrefix = "&",
											const std::string &operPrefix = "=") const;
	//**********************************************************
	// Method   : setFuctionName
	// Remark   : 设置函数名称
	// DateTime : 2015/08/24
	// Parameter: std::string name		函数名称
	//**********************************************************
	void            setFunctionName(std::string name);
protected:
	typedef		std::map<std::string, std::string>		PARAM_MAP;

	PARAM_MAP		paramMap_;		// 键值映射集合
	std::string		apiHost_;		// URL中的Host如： 192.168.1.214
	std::string		apiPath_;		// URL中的Path如： /index.php
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
	// Remark   : 重设（会清空待上传文件列表）
	// DateTime : 2015/08/24
	// Returns  : void
	//**********************************************************
	virtual void	reset();
	
	
	//**********************************************************
	// Method   : setPostDataString
	// Remark   : 设置自定义Post数据内容体
	// DateTime : 2015/08/24
	// Parameter: const std::string & data			自定义内容
	// Returns  : void
	//**********************************************************
	void	setPostDataString(const std::string &data);
	bool	getPostDataString(std::string *data) const;

	
	//**********************************************************
	// Method   : addFile
	// Remark   : 添加指定文件到待上传文件列表
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			文件路径
	// Parameter: const std::string & name			自定义文件名
	// Returns  : bool
	//**********************************************************
	bool	addFile(const std::string &path, const std::string &name = "");
	
	//**********************************************************
	// Method   : delFile
	// Remark   : 从待上传文件列表移除指定文件
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			文件路径
	// Returns  : bool
	//**********************************************************
	bool	delFile(const std::string &path);
	
	//**********************************************************
	// Method   : hasFile
	// Remark   : 判断待上传文件列表是否包含指定文件（完全匹配）
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			文件路径
	// Returns  : bool
	//**********************************************************
	bool	hasFile(const std::string &path) const;

private:
	const char*		getContentTypeByFilename(const std::string &fileName) const;

private:
	typedef		std::map<std::string/*Path*/, std::string/*Name*/>	FILE_LIST;
	FILE_LIST	fileList_;			// 要上传的文件列表
	std::string	customData_;		// 自定义Post数据（Post包的内容体）
	bool		isCustomData_;		// 是否是Post自定义数据
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
	// Remark   : 文件上传（multipart/form-data方式）
	// DateTime : 2015/08/24
	// Parameter: const std::string & url			上传接口URL
	// Parameter: std::string * html				上传接口返回的html
	// Parameter: const std::string & filePath		需要上传的文件路径
	// Parameter: const std::string & nickName		自定义文件名（可选）
	// Returns  : bool
	//**********************************************************
	static bool		uploadFile(const std::string &url, 
							std::string *html, 
							const std::string &filePath,
							const std::string &nickName = "",
                            int timeout = 6);

	
	
	//**********************************************************
	// Method   : formatURL
	// Remark   : 格式化URL（去除错误、冗余的/斜杠）
	// DateTime : 2015/08/24
	// Parameter: std::string * url					url地址
	// Returns  : void
	//**********************************************************
	static void			formatURL(std::string *url);
	
	//**********************************************************
	// Method   : formatURL_C
	// Remark   : 格式化URL（去除错误、冗余的/斜杠）
	// DateTime : 2015/08/24
	// Parameter: const std::string & url			url地址
	// Returns  : std::string				格式化后的URL
	//**********************************************************
	static std::string	formatURL_C(const std::string &url);

	
	//**********************************************************
	// Method   : setDebug
	// Remark   : 设置调试状态（调试时会在日志中输出所有http包的数据）
	// DateTime : 2015/08/24
	// Parameter: bool isDebug						是否开启调试
	// Returns  : void
	//**********************************************************
	static void			setDebug(bool isDebug);


private:
	typedef		size_t (*WRITE_FUNC)(void*, size_t, size_t, void*);

	// Get和Post的回调函数，分别是把网站返回的数据写入到内存或者写入到本地文件中
	static size_t	onWriteMemory(void *buffer, 
								size_t size, 
								size_t nmemb, 
								void *ptr);

	static size_t	onWriteFile(void *buffer, 
								size_t size, 
								size_t nmemb, 
								void *ptr);
	

	// Get方法实现，分为http和https两种
	static bool		getMethod(const std::string &url, 
							void *param, 
							WRITE_FUNC func);
	
	static bool		getMethod_ssl(const std::string &url, 
								void *param, 
								WRITE_FUNC func,
								const std::string &caPath = "");


	// Post方法实现，分为http和https两种
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
	static bool	s_isDebug;		// 是否开启调试模式
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

