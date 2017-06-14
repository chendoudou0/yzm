/**
  * FileName: exception.h
  * Author: jackcai
  * Version :1.0
  * Date: 2007-03-05
  * Description: 业务操作异常类的基类，从STL标准异常继承
  *                   而来，如果有新的需要，可以从该类继承。
  */
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <string>
#include <exception>
#include "sstring.h"

using std::string;

#ifdef __GNUC__
    /* Version 2.4 and later of GCC define a magical variable `__PRETTY_FUNCTION__'
       which contains the name of the function currently being defined.
       This is broken in G++ before version 2.6.
       C9x has a similar variable called __func__, but prefer the GCC one since
       it demangles C++ function names.  */
    # if defined __cplusplus ? __GNUC_PREREQ (2, 6) : __GNUC_PREREQ (2, 4)
    #   define __FUNCTION	__func__ //__PRETTY_FUNCTION__
    # else
    #  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
    #   define __FUNCTION	__func__
    #  else
    #   define __FUNCTION	((__const char *) 0)
    #  endif
    # endif
#else
    #define __FUNCTION "NULL"
#endif

inline const char* baseName(const char* szFilePath)
{
    if (!szFilePath)
	return NULL;

    const char* p = szFilePath + strlen(szFilePath) - 1;
    for (;p > szFilePath; --p)
    {
	if (*p == '/' || *p == '\\')
    {
        ++p;
        break;
    }
    }
    
    return p;
}

struct ExceptionSource
{
    std::string _func;
    std::string _file;
    int _line;

    ExceptionSource(const char* func, const char* file, int line) :
	_func((func)?(func):"unknown"), _file((file)?(file):"unknown"), _line(line)
    {
       file = baseName(file);
       if (file != NULL)
           _file = file;
    }

    ExceptionSource() : _func("unknown"), _file("unknown"), _line(0)
    {}
};

#define CURRENT_SOURCE ExceptionSource(__FUNCTION, __FILE__, __LINE__)

class Exception
{
public:
    Exception() : _source(CURRENT_SOURCE)
    {}
    
    Exception(const char* szMessage, const ExceptionSource& src = ExceptionSource()) :
	_message(szMessage), _source(src)
    {}

    Exception(const std::string& msg, const ExceptionSource& src = ExceptionSource()) :
	_message(msg), _source(src)
    {}

    Exception(const ExceptionSource& src) : _source(src)
    {}
    
    virtual ~Exception()
    {}

    virtual std::string message()const {
	return _message;
    }
    
    virtual std::string tostr()const {
	return SuperString::format(
        "%s, source=[%s:%d:%s]",
        this->message().c_str(),
        _source._file.c_str(),
        _source._line,
        _source._func.c_str()
        );
    }

    ExceptionSource source()const {
	return _source;
    }

protected:
    std::string _message;

private:
    ExceptionSource _source;
};


#define EXCEPTION(m) Exception((m), CURRENT_SOURCE)


class CException : public std::exception
{
public:
    CException(int errNo, const char* szErrInfo) throw();
    CException(int errNo, const string& strErrInfo) throw();
    virtual ~CException() throw();

    int error() const throw();                          // 获取错误码
    virtual const char* what() const throw();   // 获取错误描述信息

//protected:
public:
    int  _errno;
    string errinfo;
};



#endif

