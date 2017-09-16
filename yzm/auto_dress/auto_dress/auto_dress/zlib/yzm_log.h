
/**************************************************************************
Copyright	: Cloudream All Rights Reserved
Author		: chenys@cloudream.net (吖树)
Date		: 2015/08/24

Description	: 提供一个日志库，提供 YZM_*** 一系列宏用于日志输出，这些宏的使用和
		printf函数参数一致，日志库具备如下特征
		1. 输出格式为html代码
		2. 不同类型的日志用不同颜色输出（如错误、警告需红色高亮显示）
		3. 日志会自动拆分，每天的日志写到一个日志文件中
		4. 日志会自动管理，超过一定天数的日志会自动删除
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
	// Remark   : 获取单例指针（日志在一个系统应该唯一）
	// DateTime : 2015/08/24
	// Returns  : Log*					单例指针
	//**********************************************************
	static Log* getInstancePtr();
	static Log&	getInstance();


	
	//**********************************************************
	// Method   : write
	// Remark   : 写入一条日志
	// DateTime : 2015/08/24
	// Parameter: int level							日志类型（错误、警告、信息）
	// Parameter: const char * filename				源文件 文件名
	// Parameter: const char * function				源文件 函数名
	// Parameter: int line							源文件 代码行数
	// Parameter: const char * format				日志格式内容
	// Parameter: ...								日志内容XXX
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
	// 由于是单例模式，所以全部私有化
	Log();
	Log(const Log&);
	Log& operator=(const Log&);


private:
	std::string		filePath_;		// 日志全路径，默认log.html
	Mutex			mutex_;			// 锁，防止多线程调用日志库时出问题
	FILE			*handle_;		// 日志文件句柄
};

NAMESPACE_END


	





/************************************************************************
  ******************       简化日志库调用的辅助宏        ********************
************************************************************************/
#define		YZM_ERROR(format, ...)	yzm::Log::getInstance().write(yzm::Log::Error, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)	
#define		YZM_WARN(format, ...)	yzm::Log::getInstance().write(yzm::Log::Warn, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)	
#define		YZM_INFO(format, ...)	yzm::Log::getInstance().write(yzm::Log::Info, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)	
#define		YZM_DEBUG(format, ...)	yzm::Log::getInstance().write(yzm::Log::Debug, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define		YZM_VALUE(val)			yzm::Log::getInstance().write(yzm::Log::Info, __FILE__, __FUNCTION__, __LINE__, "  [%s = %s]  ", #val, yzm::StrT::toString(val).c_str())

#define		YZM_LOG_NAME(name)		yzm::Log::getInstance().init(name)


	





/************************************************************************
    ******************       简化代码的辅助宏        ********************
************************************************************************/
#define		WarningIf(exp)				do{ if(exp){ YZM_WARN(#exp);	return; } }while(0)
#define		ErrorIf(exp)				do{ if(exp){ YZM_ERROR(#exp);	return; } }while(0)

#define		WarningIf2(exp,msg)			do{ if(exp){ YZM_WARN(msg);		return; } }while(0)
#define		ErrorIf2(exp,msg)			do{ if(exp){ YZM_ERROR(msg);	return; } }while(0)

#define		WarningIf_r(exp,ret)		do{ if(exp){ YZM_WARN(#exp);	return ret; } }while(0)
#define		ErrorIf_r(exp,ret)			do{ if(exp){ YZM_ERROR(#exp);	return ret; } }while(0)

#define		WarningIf_r2(exp,msg,ret)	do{ if(exp){ YZM_WARN(msg);		return ret; } }while(0)
#define		ErrorIf_r2(exp,msg,ret)		do{ if(exp){ YZM_ERROR(msg);	return ret; } }while(0)


