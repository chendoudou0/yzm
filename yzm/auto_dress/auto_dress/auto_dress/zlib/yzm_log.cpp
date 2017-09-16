
/**************************************************************************
Copyright	: Cloudream All Rights Reserved
Author		: chenys@cloudream.net (߹��)
Date		: 2015/08/24

Description	: �ṩһ����־�⣬�ṩ YZM_*** һϵ�к�������־�������Щ���ʹ�ú�
		printf��������һ�£���־��߱���������
		1. �����ʽΪhtml����
		2. ��ͬ���͵���־�ò�ͬ��ɫ���������󡢾������ɫ������ʾ��
		3. ��־���Զ���֣�ÿ�����־д��һ����־�ļ���
		4. ��־���Զ���������һ����������־���Զ�ɾ��
**************************************************************************/

#include "yzm_log.h"
#include "yzm_string_helper.h"
#include "yzm_file_system.h"


NAMESPACE_BEGIN

	





/************************************************************************
  *************************       Log        *************************
************************************************************************/
Log*	Log::getInstancePtr()
{
	return &(getInstance());
}

Log&	Log::getInstance()
{
	static Log s_instance;
	return s_instance;
}

Log::Log()
{
	handle_		= NULL;
	filePath_	= "";

	/*if ( !init("log.html") )
	{
		printf("Init Log FAIL.\n");
	}*/
}

Log::~Log()
{
	cleanup();
}

void	Log::write(int level, 
		const char *filename, 
		const char *function, 
		int line,
		const char *format, ...)
{
	static char	textString[BUFFER_SIZE];
	static char	fullBuffer[BUFFER_SIZE];
	static char	timeString[64];

	AutoLock	autoLock(mutex_);

	va_list args;
    va_start(args, format);
	
	//�����_TRUNCATE����Ҫ�����MAX_FORMAT_STR_LEN�ᵼ�¹���ʱ����
#if defined(_MSC_VER) && _MSC_VER >= 1400
	int	nReturn	= _vsnprintf_s(textString, BUFFER_SIZE, _TRUNCATE, format, args);
#else
	int	nReturn	= _vsnprintf(textString, BUFFER_SIZE, format, args);
#endif

	if (nReturn == -1 || nReturn == BUFFER_SIZE)
	{
		textString[BUFFER_SIZE-1]	= '\0';
	}

    va_end(args);

	time_t	utcTime;
	tm		*localTime;
	utcTime		= time(NULL);
	localTime	= localtime(&utcTime);
	strftime(timeString, sizeof(timeString), "%H:%M:%S", localTime);

	std::string		begPrefix	= "<div><pre><font face='Courier New' color='#0000BB'>";
	std::string		endPrefix	= "</font></pre></div>";
	std::string		typeString	= "=";
	if ( Log::Error == level )
	{
		typeString	= "[E";
		begPrefix	= "<div style='font-weight:bold'><pre><font face='Courier New' color='#FF0000'>";
	}
	else if ( Log::Warn == level )
	{
		typeString	= "[W";
		begPrefix	= "<div style='font-weight:bold'><pre><font face='Courier New' color='#FF00FF'>";
	}
	else if ( Log::Debug == level )
	{
		typeString	= "[D";
		begPrefix	= "<div><pre><font face='Courier New' color='#000000'>";
	}

	std::string		funcString	= function;
	if ( StrT::caseStartWith(funcString, "yzm::") )
	{
		funcString.erase(0, strlen("yzm::"));
	}
	

	if ( Log::Error == level || Log::Warn == level )
	{
		_snprintf(fullBuffer, sizeof(fullBuffer),
					"%s[%s]:%s %s %s>  %s  %s\n", 
					begPrefix.c_str(),

					timeString, 
                    filename, // :
					funcString.c_str(),
					typeString.c_str(),
					textString,
					
					endPrefix.c_str() );
	}
	else
	{
		_snprintf(fullBuffer, sizeof(fullBuffer),
					"%s[%s]:%s %s %s>  %s  %s\n", 
					begPrefix.c_str(),

					timeString, 
                    filename, // :
                    funcString.c_str(),
					typeString.c_str(),
					textString,
					
					endPrefix.c_str() );
	}

    write( fullBuffer );
}

bool	Log::init(const std::string &filePath)
{
	std::string		fullPath	= FSys::makeFullPath_C(filePath);
	FILE	*newHandle	= fopen(fullPath.c_str(), "w+");
	if ( NULL == newHandle )
	{
		return false;
	}

	cleanup();
	filePath_	= fullPath;
	handle_		= newHandle;

	return true;
}

void	Log::cleanup()
{
	if ( handle_ != NULL )
	{
		fflush(handle_);
		fclose(handle_);
		handle_		= NULL;
		filePath_	= "";
	}
}

void	Log::write(const std::string &text)
{
	if ( NULL == handle_ )
	{
		printf("Please Init Log. [%s]", text.c_str());
		return;
	}


	size_t	byteWrite	= fwrite(text.data(), text.size(), 1, handle_);
	if ( byteWrite != 1 )
	{
		printf("Write Log FAIL. [%s]", text.c_str());
		return;
	}
	
	fflush(handle_);
}


NAMESPACE_END
