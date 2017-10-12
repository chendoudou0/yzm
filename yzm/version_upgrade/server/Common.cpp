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


bool callSystemCmd(string cmd)
{
	pid_t status;  
            
    status = system(cmd.c_str());  
  
    if (-1 == status)  
    {  
        return false;  
    }  
    else  
    {  
   
        if (WIFEXITED(status))  
        {  
            if (WEXITSTATUS(status) != 0)  
            {  
                return false; 
            }    
        }  
        else  
        {  
            return false; 
        }  
    }  
  
    return true;

}


vector<string> listDir(const char *path)  
{  
	vector<string>  filenameVec;  
	
    DIR              *pDir  = NULL;
    struct dirent    *ent  ;  
    int               i=0  ;  
    char              childpath[512];  
	char              filePath[512];

    pDir = opendir(path);  
	if(NULL == pDir)
	{
		return filenameVec;
	}
    memset(childpath, 0, sizeof(childpath));  
	memset(filePath, 0, sizeof(filePath));

    while((ent = readdir(pDir)) != NULL)  
    {  
        if(ent->d_type & DT_DIR)  
        {  
            if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0) 
            {
               continue; 
            }
			
			sprintf(childpath, "%s/%s", path, ent->d_name); 
		    vector<string>  tempVec = listDir(childpath); 
			for (vector<string>::iterator it = tempVec.begin(); it != tempVec.end(); it++)  
			{  
				filenameVec.push_back(*it);  
			}  

        }  
		else
		{
			sprintf(filePath, "%s/%s", path, ent->d_name); 
			filenameVec.push_back(filePath);   
		}
		
    }  

	return filenameVec;
  
}  

bool generateMD5(string& path, string& strMD5)
{
   char cmd[300] = {0};
   sprintf(cmd, "md5sum %s > outputMD5.txt ", path.c_str());
 
    if(!callSystemCmd(cmd))
    {
	   return false;
	}

   char* md5FileName = "outputMD5.txt";  
   int fileDes = open(md5FileName, O_RDWR|O_APPEND, 0666);  
   if(-1 == fileDes)  
   {  
   	  close(fileDes); 
      return false;
   }  
  
   lseek(fileDes, 0, SEEK_SET);  
   char md5Buf[33] = {0};  
  
   if(read(fileDes, md5Buf, 32) <= 0)  
   {  
   	  close(fileDes); 
      return false;
   } 
   strMD5 = md5Buf;
   
   close(fileDes);
   return  true;

}

bool getFileSize(string& inPath, int& inSize)
{
 
   int fileDes = open(inPath.c_str(), O_RDWR|O_APPEND, 0666);  
   if(-1 == fileDes)  
   {  
   	  close(fileDes); 
      return false;
   }  
  
   inSize = lseek(fileDes, 0, SEEK_END);  
   
   close(fileDes);  
   return  true;

}

void creat_daemon(void)
{
    pid_t pid;              
	int fd;
    pid = fork();
    if( pid == -1)		exit(EXIT_FAILURE);     
    if(pid > 0 )		exit(EXIT_SUCCESS);
    if(setsid() == -1)	exit(EXIT_FAILURE);                               
    int i;
    for( i = 0; i < 3; ++i)
    {
        close(i);  
    }
	
    if (umask(0) == -1)   exit(1);

	if ((fd = open("/dev/null", O_RDWR)) == -1)  exit(1); 
	dup2(fd, STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	close(fd);
	
    return;
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




