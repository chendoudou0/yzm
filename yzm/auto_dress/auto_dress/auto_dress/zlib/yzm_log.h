
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

#pragma once

#include "yzm_assist.h"
#include "yzm_string_helper.h"


NAMESPACE_BEGIN
	





/************************************************************************
  *************************       Log        *************************
************************************************************************/
class  Log
{
public:
	static const int	Error	= 0x0001;
	static const int	Warn	= 0x0002;
	static const int	Info	= 0x0004;
	static const int	Debug	= 0x0008;

	enum { BUFFER_SIZE = 10240, CACHE_SIZE = 512, };

public:
	~Log();

	
	//**********************************************************
	// Method   : getInstancePtr
	// Remark   : ��ȡ����ָ�루��־��һ��ϵͳӦ��Ψһ��
	// DateTime : 2015/08/24
	// Returns  : Log*					����ָ��
	//**********************************************************
	static Log* getInstancePtr();
	static Log&	getInstance();


	
	//**********************************************************
	// Method   : write
	// Remark   : д��һ����־
	// DateTime : 2015/08/24
	// Parameter: int level							��־���ͣ����󡢾��桢��Ϣ��
	// Parameter: const char * filename				Դ�ļ� �ļ���
	// Parameter: const char * function				Դ�ļ� ������
	// Parameter: int line							Դ�ļ� ��������
	// Parameter: const char * format				��־��ʽ����
	// Parameter: ...								��־����XXX
	// Returns  : void
	//**********************************************************
	void	write(int level, 
				const char *filename, 
				const char *function, 
				int line,
				const char *format, ...);
	
	bool	init(const std::string &filePath);
	void	cleanup();
	void	write(const std::string &text);

private:
	// �����ǵ���ģʽ������ȫ��˽�л�
	Log();
	Log(const Log&);
	Log& operator=(const Log&);


private:
	std::string		filePath_;		// ��־ȫ·����Ĭ��log.html
	Mutex			mutex_;			// ������ֹ���̵߳�����־��ʱ������
	FILE			*handle_;		// ��־�ļ����
};

NAMESPACE_END


	





/************************************************************************
  ******************       ����־����õĸ�����        ********************
************************************************************************/
#define		YZM_ERROR(format, ...)	yzm::Log::getInstance().write(yzm::Log::Error, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)	
#define		YZM_WARN(format, ...)	yzm::Log::getInstance().write(yzm::Log::Warn, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)	
#define		YZM_INFO(format, ...)	yzm::Log::getInstance().write(yzm::Log::Info, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)	
#define		YZM_DEBUG(format, ...)	yzm::Log::getInstance().write(yzm::Log::Debug, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define		YZM_VALUE(val)			yzm::Log::getInstance().write(yzm::Log::Info, __FILE__, __FUNCTION__, __LINE__, "  [%s = %s]  ", #val, yzm::StrT::toString(val).c_str())

#define		YZM_LOG_NAME(name)		yzm::Log::getInstance().init(name)


	





/************************************************************************
    ******************       �򻯴���ĸ�����        ********************
************************************************************************/
#define		WarningIf(exp)				do{ if(exp){ YZM_WARN(#exp);	return; } }while(0)
#define		ErrorIf(exp)				do{ if(exp){ YZM_ERROR(#exp);	return; } }while(0)

#define		WarningIf2(exp,msg)			do{ if(exp){ YZM_WARN(msg);		return; } }while(0)
#define		ErrorIf2(exp,msg)			do{ if(exp){ YZM_ERROR(msg);	return; } }while(0)

#define		WarningIf_r(exp,ret)		do{ if(exp){ YZM_WARN(#exp);	return ret; } }while(0)
#define		ErrorIf_r(exp,ret)			do{ if(exp){ YZM_ERROR(#exp);	return ret; } }while(0)

#define		WarningIf_r2(exp,msg,ret)	do{ if(exp){ YZM_WARN(msg);		return ret; } }while(0)
#define		ErrorIf_r2(exp,msg,ret)		do{ if(exp){ YZM_ERROR(msg);	return ret; } }while(0)


