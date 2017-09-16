
/**************************************************************************
Copyright	: Cloudream All Rights Reserved
Author		: chenys@cloudream.net (吖树)
Date		: 2015/08/24

Description	: 提供一些公用的辅助的小函数、宏、常量等
**************************************************************************/

#pragma once




/************************************************************************
  *********************       Config        *********************
************************************************************************/
// _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
// 正在链接对象，如同没有调试信息一样
#pragma warning(disable:4204)
#pragma warning(disable:4099)






/************************************************************************
  *******************       System Header Files        ******************
************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <ctime>
#include <cmath>
#include <limits>

#include <cstring>
#include <string>
#include <sstream>
#include <strstream>

#include <functional>
#include <utility>
#include <algorithm>
#include <numeric>

#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <stack>

#include <iostream>
#include <fstream>
#include <cassert>

// C++ 11 thread
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>




/************************************************************************
  *******************       Common Assist Macro        ******************
************************************************************************/
#define		NAMESPACE_BEGIN		namespace yzm { 
#define		NAMESPACE_END		} // namespace yzm


#define		YZM_API_EXPORT		__declspec(dllexport)
#define		YZM_API_IMPORT		__declspec(dllimport)

#ifdef YZM_DLL_EXPORTS
#	define	YZM_API		YZM_API_EXPORT
#else
#	define	YZM_API		YZM_API_IMPORT
#endif



template class YZM_API_EXPORT std::basic_string<char, std::char_traits<char>, std::allocator<char> >;
template class YZM_API_EXPORT std::map<std::string, std::string>;
template class YZM_API_EXPORT std::vector<std::string>;
template class YZM_API_EXPORT std::list<int>;



#ifdef _DEBUG
#	ifdef	_M_IX86
#	define		AssertIf(exp)		do{ if ( !(exp) ){ __asm{int 3}; } }while(0)
#	else
#	define		AssertIf(exp)		do{ if ( !(exp) ){ throw std::string("Error"); } }while(0)
#	endif
#else
#	define		AssertIf(exp)		
#endif
#define		ARRAY_LEN(arr)			( sizeof( (arr) ) / sizeof( (arr[0]) ) )
#define		SafeRelease(ptr)		do{ if ( ptr != NULL ){ ptr->Release(); ptr = NULL; } }while(0)
#define		SafeDelete(ptr)			do{ if ( ptr != NULL ){ delete ptr; ptr = NULL; } }while(0)




NAMESPACE_BEGIN
/************************************************************************
  *************************       Mutex        **************************
************************************************************************/
class  Mutex
{
public:
	Mutex();
	~Mutex();

	void	Lock() const;
	void	Unlock() const;

private:
	void	*handle_;
};

class  AutoLock
{
public:
	AutoLock(const Mutex &mutex);
	~AutoLock();

private:
	const Mutex &mutex_;
};


class  IDCreater
{
public:
	IDCreater();
	~IDCreater();

	int		getID();
	void	delID(int id);

private:
	typedef		std::list<int>		ID_LIST;
	int			curID_;
	ID_LIST		unusedID_;
};







/************************************************************************
  *****************       Common Assist Functions        ****************
************************************************************************/
 std::string		generateGUID();
 void			waitSecond(float sec);

NAMESPACE_END


	
//namespace CLOUDREAM = yzm;
namespace cloudream = yzm;



