#include "exception.h"

/**
 * Description: �쳣��Ĺ��캯��
 * Input:  errNum       ������
 *            szErrInfo     ����������Ϣ
 */
CException::CException(int errNum, const char* szErrInfo) throw() 
                       : _errno(errNum), errinfo(szErrInfo)
{
}

/**
 * Description: �쳣��Ĺ��캯��
 * Input:  errNum       ������
 *            szErrInfo     ����������Ϣ
 */
CException::CException(int errNo, const string& strErrInfo) throw()
                        : _errno(errNo), errinfo(strErrInfo)
{
}

/**
 * Description: �쳣�����������
 */
CException::~CException() throw()
{
}

/**
 * Description: ��ȡ�쳣�Ĵ�����
 * Return: ������
 */
int CException::error() const throw()
{
    return _errno;
}

/**
 * Description: �쳣�����������
 * Return: ����������Ϣ
 */
const char* CException::what() const throw()
{
    return errinfo.c_str();
}
