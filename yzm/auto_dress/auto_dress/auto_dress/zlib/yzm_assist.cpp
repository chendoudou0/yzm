
/**************************************************************************
Copyright	: Cloudream All Rights Reserved
Author		: chenys@cloudream.net (吖树)
Date		: 2015/08/24

Description	: 提供一些公用的辅助的小函数、宏、常量等
**************************************************************************/

#include "yzm_assist.h"
#include "yzm_log.h"
#include "yzm_string_helper.h"
#include <windows.h>


NAMESPACE_BEGIN
	




/************************************************************************
  *************************       Mutex        **************************
************************************************************************/
Mutex::Mutex()
{
	handle_		= ::CreateMutexA(NULL, FALSE, NULL);
}

Mutex::~Mutex()
{
	::CloseHandle(handle_);
	handle_		= NULL;
}

void	Mutex::Lock() const
{
	::WaitForSingleObject(handle_, INFINITE);
}

void	Mutex::Unlock() const
{
	::ReleaseMutex(handle_);
}

	
AutoLock::AutoLock(const Mutex &m)
	: mutex_(m)
{
	mutex_.Lock();
}

AutoLock::~AutoLock()
{
	mutex_.Unlock();
}



IDCreater::IDCreater()
{
	curID_		= 1;
}

IDCreater::~IDCreater()
{
}

int		IDCreater::getID()
{
	int		id	= 0;
	if ( !unusedID_.empty() )
	{
		id		= unusedID_.front();
		unusedID_.pop_front();
	}
	else
	{
		id		= curID_++;
	}

	return id;
}

void	IDCreater::delID(int id)
{
	// 回收还没有分配的ID，属于错误调用
	if ( id >= curID_ )
	{
		return;
	}

	ID_LIST::iterator	itr = std::find(unusedID_.begin(), unusedID_.end(), id);
	if ( itr != unusedID_.end() )
	{
		// 重复回收，属于错误调用
		return;
	}

	unusedID_.push_back(id);
}








/************************************************************************
  *****************       Common Assist Functions        ****************
************************************************************************/
class _InitCOMHelper
{
public:
	_InitCOMHelper()
	{
		::CoInitialize(NULL); 
	}

	~_InitCOMHelper()
	{
		::CoUninitialize();
	}
};

static _InitCOMHelper	g_ComIniter;
/*
std::string		generateGUID()
{
#define NEW_GENERATE_GUID   1 // 10位的时间戳+6位随机数

#if NEW_GENERATE_GUID
    time_t timestamp;
    std::string strRandom6;
    char buf[1024] = {0};

    time(&timestamp);
    StrT::getRandString(strRandom6, 6);

    sprintf_s(buf, "%10ld%s", timestamp, strRandom6);

    return std::string(buf);   
#else
	static const int	GUID_LEN = 36;

	std::string		guidString;
	GUID		guid;
	HRESULT		ret;

	ret		= ::CoCreateGuid(&guid);
	if ( ret != S_OK )
	{
		StrT::getRandString(guidString, GUID_LEN);
		YZM_ERROR("FAIL. ret = %d", (int)ret);
		return guidString;
	}

	StrT::format(guidString,
		"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3, 
		guid.Data4[0], guid.Data4[1], 
		guid.Data4[2], guid.Data4[3], 
		guid.Data4[4], guid.Data4[5], 
		guid.Data4[6], guid.Data4[7] );
	return guidString;
#endif
}
*/
void		waitSecond(float sec)
{
	int		msec	= (int)(sec * 1000.0f);
	::Sleep(msec);
}

NAMESPACE_END

	