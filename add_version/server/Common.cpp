#include "Common.h"
#include <curl/curl.h>


int string2int(string &paraStr, int &paraInt)
{
	stringstream ss;
	ss << paraStr;
	ss >> paraInt; 

	return 0;
}

int int2string(int paraInt, string &paraStr)
{
	stringstream ss;
	ss << paraInt;
	ss >> paraStr; 

	return 0;
}



int IsAnotherCopyRunning(const char * filename)
{
	struct flock lock;
	int fd;
	char line[11];
	char filepath[512];
	
	lock.l_type=F_WRLCK;
	lock.l_start=0;
	lock.l_whence=SEEK_SET;
	lock.l_len=0;
	
	memset(filepath,0,sizeof(filepath));	
	sprintf(filepath,"%s/%s",getenv("UPGW_REL_TMP"),filename);
	fd=open(filepath,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IXOTH);
	
	if(fd<0)
	{
		return -1;
	}
	
	if(fcntl(fd,F_SETLK,&lock)<0)
	{
		return 1;
	}
	
	sprintf(line,"%10d",getpid());
	
	write(fd,line,strlen(line));
	
	return 0;
}
/*
Function :	Make an daemon process
*/
void DaemonInit()
{
	int i;
	pid_t pid;

	if((pid = fork())!=0)
		exit(0);

	setsid();
	
	signal(SIGHUP, SIG_IGN);

	if((pid = fork())!=0)
		exit(0);

	//chdir("/");

	umask(0);
	
	for (i=0; i<MAXFD; i++)
		close(i);
		
}


void SetNonBlocking( int nSock )
{
	int opts;
	opts = fcntl ( nSock, F_GETFL);
	if(opts<0)
	{
		printf("fcntl(sock,GETFL)");
		exit(1);
	}
		
	opts = opts | O_NONBLOCK;
	if(fcntl(nSock, F_SETFL, opts)<0)
	{
		printf("fcntl(sock,SETFL,opts)");
		exit(1);
	}
}


int myCpy( char *desc , char *src, int nLen )
{
	if( desc==NULL || src==NULL || nLen==0 )
	{
		printf("myCpy param error !\n");
		return -1;
	}

	for( ; nLen>0 ; nLen-- )
	{
		*desc++ = *src++;
	}
	
	return 0;
}


void PrintHex(char *pBuf, int nBufLen)
{ 
     char pMsgData[nBufLen*3+1];
   
    memset(pMsgData,0, nBufLen*3+1 );
    for (int i = 0; i < nBufLen; i++)
    {
        sprintf(pMsgData + i * 3, "%02x ", pBuf[i] & 0xFF);
	  // printf("\0x%02x ", pBuf[i] & 0xFF);
    }

   // printf("Output< %d > bytes data : %s \n", nBufLen, pMsgData);
   printf("Output< %d > bytes data : %s \n", nBufLen, pMsgData);
    
}


void PrintDecimal(char *pBuf, int nBufLen)
{
    char pMsgData[nBufLen*3+1];
    memset(pMsgData,0, nBufLen*3+1 );

    for (int i = 0; i < nBufLen; i++)
    {
        sprintf(pMsgData + i * 3, "%02d ", pBuf[i] & 0xFF);
	  // printf("\0x%02x ", pBuf[i] & 0xFF);
    }

    printf("Output< %d > bytes data : %s \n", nBufLen, pMsgData);
}

void PrintAlpha(char *pBuf, int nBufLen)
{
    char *pMsgData = new char[nBufLen*2+1];
    memset(pMsgData,0, nBufLen*2+1 );

    for (int i = 0; i < nBufLen; i++)
    {
        sprintf(pMsgData + i * 2, "%c ", pBuf[i] );
	  // printf("\0x%02x ", pBuf[i] & 0xFF);
    }

    printf("Output< %d > bytes data : %s \n", nBufLen, pMsgData);
    delete [] pMsgData;
}

int ConnectRemoteServer(string strIp, int nPort )
{
	//设置一个socket地址结构client_addr,代表客户机internet地址, 端口 
    struct sockaddr_in client_addr; 
    bzero(&client_addr,sizeof(client_addr)); //把一段内存区的内容全部设置为0 
    client_addr.sin_family = AF_INET;    //internet协议族 
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);//INADDR_ANY表示自动获取本机地址 
    client_addr.sin_port = htons(0);    //0表示让系统自动分配一个空闲端口 

    //创建用于internet的流协议(TCP)socket,用client_socket代表客户机socket 
    int client_socket = socket(AF_INET,SOCK_STREAM,0); 
    if( client_socket < 0) 
    { 
        //Error1("Create Socket Failed!msg: %s\n", strerror(errno)); 
        exit(1); 
    } 

 
    //把客户机的socket和客户机的socket地址结构联系起来 
    if( bind(client_socket,(struct sockaddr*)&client_addr,sizeof(client_addr))) 
    { 
        //Error1("Client Bind Port Failed!msg: %s\n", strerror(errno)); 
	 int nRet=close(client_socket);
	 if( nRet == -1 )
	{
		//Error2("CloseFd()  close return error! sockID=%d  msg: %s\n", client_socket, strerror(errno))
	}
        exit(1); 
    } 

   //设置一个socket地址结构server_addr,代表服务器的internet地址, 端口
    struct sockaddr_in server_addr; 
    bzero(&server_addr,sizeof(server_addr)); 
    server_addr.sin_family = AF_INET; 
    if(inet_aton(strIp.c_str(),&server_addr.sin_addr) == 0) //服务器的IP地址来自程序的参数 
    { 
        //Error0("Server IP Address Error!\n"); 
	 int nRet=close(client_socket);
	 if( nRet == -1 )
	{
		//Error2("CloseFd()  close return error! sockID=%d  msg: %s\n", client_socket, strerror(errno))
	}
        exit(1); 
    } 
    server_addr.sin_port = htons(nPort);


    socklen_t server_addr_length = sizeof(server_addr); 
    //向服务器发起连接,连接成功后client_socket代表了客户机和服务器的一个socket连接 
    if(connect(client_socket,(struct sockaddr*)&server_addr, server_addr_length) < 0) 
    { 
       // printf("Can Not Connect To ip=%s port=%d!\n", strIp.c_str(), nPort);
       int nRet=close(client_socket);
	 if( nRet == -1 )
	{
		//Error2("CloseFd()  close return error! sockID=%d  msg: %s\n", client_socket, strerror(errno))
	}
        return -1;
    } 
    else
    {
	  // printf("Success Connect To ip=%s port=%d!\n", strIp.c_str(), nPort);
    }

    return client_socket;

}

char * GetSystemDate()
{
	  struct	timeb	tp;
	  time_t			timer;
	  struct	tm		*ltm;

	  ftime(&tp);
	  char *date =new char[24];
	  memset(date,0,sizeof(date));
	  timer = tp.time;
    
	  ltm = localtime(&timer);
	  sprintf(date,"%04d-%02d-%02d",
	          ltm->tm_year  +  1900,
	  				ltm->tm_mon  +  1,
	  				ltm->tm_mday);

	  return date;				
}

char * GetSystemTime()
{
	  struct	timeb	tp;
	  time_t			timer;
	  struct	tm		*ltm;

	  ftime(&tp);
	  char *time =new char[24];
	  memset(time,0,sizeof(time));
	  timer = tp.time;
    
	  ltm = localtime(&timer);
	  sprintf(time,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
	          ltm->tm_year  +  1900,
	  				ltm->tm_mon  +  1,
	  				ltm->tm_mday,
	  				ltm->tm_hour,
	  				ltm->tm_min,
	  				ltm->tm_sec,
	  				tp.millitm);   

	  return time;				
}
/*
void GetSystemTime(char time[24])   
{
	struct	timeb	tp;
	time_t			timer;
	struct	tm		*ltm;

	ftime(&tp);
	memset(time,0,sizeof(time));
	timer = tp.time;

	ltm = localtime(&timer);
	sprintf(time,"%02d-%02d %02d:%02d:%02d.%03d",
					ltm->tm_mon  +  1,
					ltm->tm_mday,
					ltm->tm_hour,
					ltm->tm_min,
					ltm->tm_sec,
					tp.millitm);
}

void GetTimeForFile(char time[24])
{
	struct	timeb	tp;
	time_t			timer;
	struct	tm		*ltm;

	ftime(&tp);
	timer = tp.time;

	ltm = localtime(&timer);
	sprintf(time,"%02d%02d%02d%02d",
					ltm->tm_mon  +  1,
					ltm->tm_mday,
					ltm->tm_hour,
					ltm->tm_min);
}*/



string EMPTY_STRING;

/************************************************************************
  *********************       HttpParamHelper        *********************
************************************************************************/
HttpParamHelper::HttpParamHelper()
{
	apiHost_		= "";
	apiPath_		= "";
}

HttpParamHelper::~HttpParamHelper()
{
}

void HttpParamHelper::setHostPath(const std::string &host, const std::string &path)
{
	apiHost_		= host;
	apiPath_		= path; 
}

const std::string& HttpParamHelper::getHost() const
{
	return apiHost_;
}

const std::string& HttpParamHelper::getPath() const
{
	return apiPath_;
}

void	HttpParamHelper::reset()
{
	paramMap_.clear();
}

bool	HttpParamHelper::setParam(const std::string &key, int val)
{
	char temp[300] = {0};
	sprintf(temp, "%d", val);
	string strVal = temp;
	return setParam(key, strVal);
}

bool	HttpParamHelper::setParam(const std::string &key, bool val)
{
	static const std::string	TRUE_STR	= "true";
	static const std::string	FALSE_STR	= "false";

	return setParam(key, val ? TRUE_STR : FALSE_STR);
}

bool	HttpParamHelper::setParam(const std::string &key, const char *val)
{
	return setParam(key, std::string(val));
}

bool	HttpParamHelper::setParam(const std::string &key, const std::string &val)
{
	bool	isUpdate	= hasParam(key);
	paramMap_[key]		= val;

	return	isUpdate;
}

const std::string&	HttpParamHelper::getParam(const std::string &key) const
{
	if ( hasParam(key) )
	{
		return paramMap_.at(key);
	}
	else
	{
		//YZM_WARN("Key [%s] no Exist.", key.c_str());
		string EMPTY_STRING;
		return EMPTY_STRING;
	}
}

bool	HttpParamHelper::hasParam(const std::string &key) const
{
	if ( paramMap_.find(key) != paramMap_.end() )
	{
		return true;
	} 
	else
	{
		return false;
	}
}

bool	HttpParamHelper::delParam(const std::string &key)
{
	PARAM_MAP::iterator		itr	= paramMap_.find(key);
	if ( itr == paramMap_.end() )
	{
		//YZM_WARN("Key [%s] no Exist.", key.c_str());
		return false;
	} 
	else
	{
		paramMap_.erase(itr);
		return true;
	}
}

void   HttpParamHelper::setFunctionName(std::string name)
{
	name_ = name;
}

std::string		HttpParamHelper::getFullUrl() const
{
	std::string		fullUrl;
	fullUrl		+= "http://";
	fullUrl		+= apiHost_;
	fullUrl		+= apiPath_;
	fullUrl		+= name_;
	fullUrl		+= generateGetParamString();

	return fullUrl;
}

std::string		HttpParamHelper::generateGetParamString(const std::string &firstPrefix,
											const std::string &otherPrefix,
											const std::string &operPrefix) const
{
	std::string		fullParamString;

	PARAM_MAP::const_iterator	itr	= paramMap_.begin();
	for (; itr != paramMap_.end(); ++itr)
	{
		if ( itr == paramMap_.begin() )
		{
			fullParamString		+= firstPrefix;
		} 
		else
		{
			fullParamString		+= otherPrefix;
		}

		fullParamString			+= itr->first;    
		fullParamString			+= operPrefix;
		fullParamString			+= itr->second;
	}

	return fullParamString;
}



bool	PostParamHelper::getPostDataString(std::string *data) const
{
	//AssertIf(data != NULL);
	//ErrorIf_r(data == NULL, false);
	
	if(data == NULL)
	{
		return false;
	}
	
	if ( isCustomData_ )
	{
		data->assign(customData_);
		return true;
	}

	char			*aboundary		= "-----------------------------7df14cc3e0f92";
	std::string		first_boundary	= "-------------------------------7df14cc3e0f92\r\n";
    std::string		delimiter		= "\r\n-------------------------------7df14cc3e0f92\r\n";
    std::string		end_boundary	= "\r\n-------------------------------7df14cc3e0f92--\r\n";

	std::string		fileData;
	bool			hasSucc	= true;;
	char			buffer[256];
	std::string		token;

	data->assign( EMPTY_STRING );

	/// begin
	data->append(first_boundary);

	/// content
	PARAM_MAP::const_iterator iter = paramMap_.begin();
	for (; iter != paramMap_.end(); ++iter)
	{
		if ( iter != paramMap_.begin() )
		{
			data->append(delimiter);
		} 

		snprintf(buffer, sizeof(buffer), "Content-Disposition: form-data; name=\"%s\"\r\n\r\n", iter->first.c_str());
		data->append(buffer);
		data->append(iter->second);
	}

/*
	/// files
	FILE_LIST::const_iterator	itr	= fileList_.begin();
	for (; itr != fileList_.end(); ++itr)
	{
		data->append(delimiter);

		const std::string &filepath	= itr->first;
		const std::string &filename	= itr->second;

		snprintf(buffer, sizeof(buffer), "Content-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\n", filename.c_str());
		data->append(buffer);

		snprintf(buffer, sizeof(buffer), "Content-Type: %s\r\n\r\n", getContentTypeByFilename(filename));
		data->append(buffer);

		if ( FSys::readFileToMemory(filepath, &fileData) )
		{
			data->append(fileData.data(), fileData.size());
		}
		else
		{
			hasSucc	= false;
			data->assign(filepath);
			break;
		}
	}

	*/

	/// end
	if ( hasSucc )
	{
		data->append(end_boundary);
	}

	return hasSucc;
}




/************************************************************************
  *********************       PostParamHelper        *********************
************************************************************************/
PostParamHelper::PostParamHelper()
{
	isCustomData_	= false;
}

PostParamHelper::~PostParamHelper()
{
}

void	PostParamHelper::reset()
{
	HttpParamHelper::reset();
	fileList_.clear();
}


/************************************************************************
  ***********************       HttpHelper        ***********************
************************************************************************/

bool	HttpHelper::s_isDebug	= false;
static CURLSH* share_handle = NULL;

bool	HttpHelper::postMethod(const std::string &url, 
                               const std::string &postData, 
                               void *param, 
                               WRITE_FUNC func,
                               int timeout)
{
	if(param == NULL || func == NULL)
	{
		return false;
	}
	

	CURLcode	res;
	CURL		*curl	= curl_easy_init();
	curl_slist	*headers= NULL;

	// CURLE_FAILED_INIT
	if(curl == NULL)
	{
		return false;
	}
	
	if( s_isDebug )
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, NULL);
	}

    if (!share_handle)
    {
        share_handle = curl_share_init();
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    }
    curl_easy_setopt(curl, CURLOPT_SHARE, share_handle);
    curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 5 * 60);

	headers	= curl_slist_append(headers, "Content-Type:text/plain;charset=UTF-8");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, func);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, param);
	
	/**
	* 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
	* 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
	*/
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

    //auto startTime = time(NULL);
    res = curl_easy_perform(curl);
    //YZM_INFO("curl_easy_perform(postMethod) time : %d", time(NULL) - startTime);
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
	
	if ( res != CURLE_OK )
	{
		//YZM_ERROR("[%s] FAIL.\n\t ErrorMsg = %s", url.c_str(), curl_easy_strerror(res));
	}

	return (CURLE_OK == res);
}



size_t	HttpHelper::onWriteMemory(void *buffer, size_t size, size_t nmemb, void *ptr)
{
	//WarningIf_r(NULL == ptr || NULL == buffer, -1);

	if(ptr == NULL || buffer == NULL)
	{
	    return -1;                             
	}

	std::string		*str	= reinterpret_cast<std::string*>(ptr);
    char			*data	= reinterpret_cast<char*>(buffer);

	static	void	*lastPtr		= NULL;
	static	const size_t	MAX_SIZE = 1024 * 1024 * 100;

	try
	{
		if ( str->size()+size*nmemb <= MAX_SIZE )
		{
			str->append(data, size * nmemb);
		}
		else if (lastPtr != ptr)
		{
			///YZM_WARN("【BaseLib】下载到内存的网页超过100MB，Html = %s", str->substr(0, 20).c_str());
			lastPtr	= ptr;
		}
	}
	catch(...)
	{
		if (lastPtr != ptr)
		{
			//////YZM_ERROR("【BaseLib】基础组件HttpHelper遇到未知错误，Html = %s", str->substr(0, 100).c_str());
			lastPtr	= ptr;
		}
	}

	return nmemb;
}


bool	HttpHelper::submitPage(const PostParamHelper &helper, string *html)
{
	//AssertIf(html != NULL);
	//ErrorIf_r(NULL == html, false);
	if(html == NULL)
	{
		return false;
	}

	std::string		url	= helper.getFullUrl();
	std::string		data;
	bool	isSucc	= helper.getPostDataString(&data);
	if ( !isSucc )
	{
		//YZM_ERROR("[%s] FAIL.\n\t Data = %s", url.c_str(), data.c_str());
		return false;
	}

	isSucc	= postMethod(url, data, html, onWriteMemory);
	if ( isSucc )
	{
		//YZM_INFO("[%s] SUCC.", url.c_str());
	} 
	else
	{
		//YZM_ERROR("[%s] FAIL.\n\t Data = %s\n\t Ret = %s", url.c_str(), data.c_str(), html->c_str());
	}
	return isSucc;
}


	

/************************************************************************
  *************************       MD5        *************************
************************************************************************/

/*
#define GET_UINT32(n,b,i)                                     \
{                                                             \
    (n) = (uint32) ((uint8 *) b)[(i)]                         \
      | (((uint32) ((uint8 *) b)[(i)+1]) <<  8)               \
      | (((uint32) ((uint8 *) b)[(i)+2]) << 16)               \
      | (((uint32) ((uint8 *) b)[(i)+3]) << 24);              \
}

#define PUT_UINT32(n,b,i)                                     \
{                                                             \
    (((uint8 *) b)[(i)]  ) = (uint8) (((n)      ) & 0xFF);    \
    (((uint8 *) b)[(i)+1]) = (uint8) (((n) >>  8) & 0xFF);    \
    (((uint8 *) b)[(i)+2]) = (uint8) (((n) >> 16) & 0xFF);    \
    (((uint8 *) b)[(i)+3]) = (uint8) (((n) >> 24) & 0xFF);    \
}

void MD5::starts( struct md5_context *ctx )
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
}

void MD5::process( struct md5_context *ctx, uint8 data[64] )
{
    uint32 A, B, C, D, X[16];

    GET_UINT32( X[0],  data,  0 );
    GET_UINT32( X[1],  data,  4 );
    GET_UINT32( X[2],  data,  8 );
    GET_UINT32( X[3],  data, 12 );
    GET_UINT32( X[4],  data, 16 );
    GET_UINT32( X[5],  data, 20 );
    GET_UINT32( X[6],  data, 24 );
    GET_UINT32( X[7],  data, 28 );
    GET_UINT32( X[8],  data, 32 );
    GET_UINT32( X[9],  data, 36 );
    GET_UINT32( X[10], data, 40 );
    GET_UINT32( X[11], data, 44 );
    GET_UINT32( X[12], data, 48 );
    GET_UINT32( X[13], data, 52 );
    GET_UINT32( X[14], data, 56 );
    GET_UINT32( X[15], data, 60 );

#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))

#define P(a,b,c,d,k,s,t)                                    \
{                                                                           \
    a += F(b,c,d) + X[k] + t; a = S(a,s) + b;      \
}

    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];

#define F(x,y,z) (z ^ (x & (y ^ z)))

    P( A, B, C, D,  0,  7, 0xD76AA478 );
    P( D, A, B, C,  1, 12, 0xE8C7B756 );
    P( C, D, A, B,  2, 17, 0x242070DB );
    P( B, C, D, A,  3, 22, 0xC1BDCEEE );
    P( A, B, C, D,  4,  7, 0xF57C0FAF );
    P( D, A, B, C,  5, 12, 0x4787C62A );
    P( C, D, A, B,  6, 17, 0xA8304613 );
    P( B, C, D, A,  7, 22, 0xFD469501 );
    P( A, B, C, D,  8,  7, 0x698098D8 );
    P( D, A, B, C,  9, 12, 0x8B44F7AF );
    P( C, D, A, B, 10, 17, 0xFFFF5BB1 );
    P( B, C, D, A, 11, 22, 0x895CD7BE );
    P( A, B, C, D, 12,  7, 0x6B901122 );
    P( D, A, B, C, 13, 12, 0xFD987193 );
    P( C, D, A, B, 14, 17, 0xA679438E );
    P( B, C, D, A, 15, 22, 0x49B40821 );

#undef F

#define F(x,y,z) (y ^ (z & (x ^ y)))

    P( A, B, C, D,  1,  5, 0xF61E2562 );
    P( D, A, B, C,  6,  9, 0xC040B340 );
    P( C, D, A, B, 11, 14, 0x265E5A51 );
    P( B, C, D, A,  0, 20, 0xE9B6C7AA );
    P( A, B, C, D,  5,  5, 0xD62F105D );
    P( D, A, B, C, 10,  9, 0x02441453 );
    P( C, D, A, B, 15, 14, 0xD8A1E681 );
    P( B, C, D, A,  4, 20, 0xE7D3FBC8 );
    P( A, B, C, D,  9,  5, 0x21E1CDE6 );
    P( D, A, B, C, 14,  9, 0xC33707D6 );
    P( C, D, A, B,  3, 14, 0xF4D50D87 );
    P( B, C, D, A,  8, 20, 0x455A14ED );
    P( A, B, C, D, 13,  5, 0xA9E3E905 );
    P( D, A, B, C,  2,  9, 0xFCEFA3F8 );
    P( C, D, A, B,  7, 14, 0x676F02D9 );
    P( B, C, D, A, 12, 20, 0x8D2A4C8A );

#undef F
    
#define F(x,y,z) (x ^ y ^ z)

    P( A, B, C, D,  5,  4, 0xFFFA3942 );
    P( D, A, B, C,  8, 11, 0x8771F681 );
    P( C, D, A, B, 11, 16, 0x6D9D6122 );
    P( B, C, D, A, 14, 23, 0xFDE5380C );
    P( A, B, C, D,  1,  4, 0xA4BEEA44 );
    P( D, A, B, C,  4, 11, 0x4BDECFA9 );
    P( C, D, A, B,  7, 16, 0xF6BB4B60 );
    P( B, C, D, A, 10, 23, 0xBEBFBC70 );
    P( A, B, C, D, 13,  4, 0x289B7EC6 );
    P( D, A, B, C,  0, 11, 0xEAA127FA );
    P( C, D, A, B,  3, 16, 0xD4EF3085 );
    P( B, C, D, A,  6, 23, 0x04881D05 );
    P( A, B, C, D,  9,  4, 0xD9D4D039 );
    P( D, A, B, C, 12, 11, 0xE6DB99E5 );
    P( C, D, A, B, 15, 16, 0x1FA27CF8 );
    P( B, C, D, A,  2, 23, 0xC4AC5665 );

#undef F

#define F(x,y,z) (y ^ (x | ~z))

    P( A, B, C, D,  0,  6, 0xF4292244 );
    P( D, A, B, C,  7, 10, 0x432AFF97 );
    P( C, D, A, B, 14, 15, 0xAB9423A7 );
    P( B, C, D, A,  5, 21, 0xFC93A039 );
    P( A, B, C, D, 12,  6, 0x655B59C3 );
    P( D, A, B, C,  3, 10, 0x8F0CCC92 );
    P( C, D, A, B, 10, 15, 0xFFEFF47D );
    P( B, C, D, A,  1, 21, 0x85845DD1 );
    P( A, B, C, D,  8,  6, 0x6FA87E4F );
    P( D, A, B, C, 15, 10, 0xFE2CE6E0 );
    P( C, D, A, B,  6, 15, 0xA3014314 );
    P( B, C, D, A, 13, 21, 0x4E0811A1 );
    P( A, B, C, D,  4,  6, 0xF7537E82 );
    P( D, A, B, C, 11, 10, 0xBD3AF235 );
    P( C, D, A, B,  2, 15, 0x2AD7D2BB );
    P( B, C, D, A,  9, 21, 0xEB86D391 );

#undef F

    ctx->state[0] += A;
    ctx->state[1] += B;
    ctx->state[2] += C;
    ctx->state[3] += D;
}

void MD5::update( struct md5_context *ctx, uint8 *input, uint32 length )
{
    uint32 left, fill;

    if( ! length ) return;

    left = ( ctx->total[0] >> 3 ) & 0x3F;
    fill = 64 - left;

    ctx->total[0] += length <<  3;
    ctx->total[1] += length >> 29;

    ctx->total[0] &= 0xFFFFFFFF;
    ctx->total[1] += ctx->total[0] < length << 3;

    if( left && length >= fill )
    {
        memcpy( (void *) (ctx->buffer + left), (void *) input, fill );
        process( ctx, ctx->buffer );
        length -= fill;
        input  += fill;
        left = 0;
    }

    while( length >= 64 )
    {
        process( ctx, input );
        length -= 64;
        input  += 64;
    }

    if( length )
    {
        memcpy( (void *) (ctx->buffer + left), (void *) input, length );
    }
}

static uint8 md5_padding[64] =
{
 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void MD5::finish( struct md5_context *ctx, uint8 digest[16] )
{
    uint32 last, padn;
    uint8 msglen[8];

    PUT_UINT32( ctx->total[0], msglen, 0 );
    PUT_UINT32( ctx->total[1], msglen, 4 );

    last = ( ctx->total[0] >> 3 ) & 0x3F;
    padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );

    update( ctx, md5_padding, padn );
    update( ctx, msglen, 8 );

    PUT_UINT32( ctx->state[0], digest,  0 );
    PUT_UINT32( ctx->state[1], digest,  4 );
    PUT_UINT32( ctx->state[2], digest,  8 );
    PUT_UINT32( ctx->state[3], digest, 12 );
}

void MD5::generate(unsigned char* buffer,int bufferlen)
{
    struct md5_context context;
    starts (&context);
    update (&context, buffer, bufferlen);
    finish (&context,(unsigned char*)m_data);
}

void MD5::generate(const std::string &buffer)
{
	generate( (unsigned char*)const_cast<char*>(buffer.c_str()),
		static_cast<int>(buffer.size()) );
}

MD5::MD5()
{
    for(int i=0;i<4;i++)
        m_data[i]=0;
}

MD5::MD5(unsigned long* md5src)
{
    memcpy(m_data,md5src,16);
}
int _httoi(const char *value)
{
  struct CHexMap
  {
    char chr;
    int value;
  };
  const int HexMapL = 16;
  CHexMap HexMap[HexMapL] =
  {
    {'0', 0}, {'1', 1},
    {'2', 2}, {'3', 3},
    {'4', 4}, {'5', 5},
    {'6', 6}, {'7', 7},
    {'8', 8}, {'9', 9},
    {'a', 10}, {'b', 11},
    {'c', 12}, {'d', 13},
    {'e', 14}, {'f', 15}
  };

  char *s = const_cast<char*>(value);

  int result = 0;
  if (*s == '0' && *(s + 1) == 'X') s += 2;
  bool firsttime = true;
  while (*s != '/0')
  {
    bool found = false;
    for (int i = 0; i < HexMapL; i++)
    {
      if (*s == HexMap[i].chr)
      {
        if (!firsttime) result <<= 4;
        result |= HexMap[i].value;
        found = true;
        break;
      }
    }
    if (!found) break;
    s++;
    firsttime = false;
  }

  return result;
}


MD5::MD5(const char* md5src)
{
    if (strcmp(md5src,"")==0)
    {
        for(int i=0;i<4;i++)
            m_data[i]=0;
        return;
    }
    for(int j = 0; j < 16; j++ )
    {
        char buf[10];
        strncpy(buf,md5src,2);
        md5src+=2;
        ((unsigned char*)m_data)[j] = _httoi(buf);
    }
}

MD5 MD5::operator+(const MD5 &adder)
{
    unsigned long m_newdata[4];
    for(int i=0;i<4;i++)
        m_newdata[i]=m_data[i]^(adder.m_data[i]);
    return MD5(m_newdata);
}

bool MD5::operator ==(const MD5 &cmper) const
{
    return (memcmp(cmper.m_data ,m_data,16)==0);
}

string MD5::toString()
{
    char output[33];
    for(int j = 0; j < 16; j++ )
    {
        sprintf( output + j * 2, "%02x", ((unsigned char*)m_data)[j]);
    }
    return string(output);
}

*/






