#include "exception.h"

/**
 * Description: 异常类的构造函数
 * Input:  errNum       错误码
 *            szErrInfo     错误描述信息
 */
CException::CException(int errNum, const char* szErrInfo) throw() 
                       : _errno(errNum), errinfo(szErrInfo)
{
}

/**
 * Description: 异常类的构造函数
 * Input:  errNum       错误码
 *            szErrInfo     错误描述信息
 */
CException::CException(int errNo, const string& strErrInfo) throw()
                        : _errno(errNo), errinfo(strErrInfo)
{
}

/**
 * Description: 异常类的析构函数
 */
CException::~CException() throw()
{
}

/**
 * Description: 获取异常的错误码
 * Return: 错误码
 */
int CException::error() const throw()
{
    return _errno;
}

/**
 * Description: 异常类的析构函数
 * Return: 错误描述信息
 */
const char* CException::what() const throw()
{
    return errinfo.c_str();
}
