
/**************************************************************************
Copyright	: Cloudream All Rights Reserved
Author		: chenys@cloudream.net (߹��)
Date		: 2015/08/24

Description	: �ṩһЩstd::string�ĸ���������ʹ��std::string����������˳��һЩ
**************************************************************************/

#pragma once

#include "yzm_assist.h"


NAMESPACE_BEGIN


class StrT;
typedef		StrT	StringHelper;
	





/************************************************************************
  **********************       StringHelper        **********************
************************************************************************/
class  StrT
{
public:
	//��_C��β�ĺ������ǿ����������޸�ԭ����strText�������ڲ��漰����ο������Ʋ�������������
	static std::string	replaceAll_C(const std::string &strText, const char chOld, const char chNew);

	static std::string	replaceAll_C(const std::string &strText, const std::string &strOld, const std::string &strNew);

	static std::string	replaceMany_C(const std::string &strText, const char chOld, const char chNew, size_t nMaxCount = 1);

	static std::string	replaceMany_C(const std::string &strText, const std::string &strOld, const std::string &strNew, size_t nMaxCount = 1);

	static std::string	trimLeft_C(const std::string &strText, const std::string &strTrim = " ");

	static std::string	trimRight_C(const std::string &strText, const std::string &strTrim = " ");

	static std::string	trim_C(const std::string &strText, const std::string &strTrim = " ");

	static std::string	toLower_C(const std::string &strText);

	static std::string	toUpper_C(const std::string &strText);


	static std::string	left(const std::string &strText, size_t nLen);

	//nStart�±��1��ʼ������nStart�ַ�
	static std::string	mid(const std::string &strText, size_t nStart, size_t nLen);

	static std::string	right(const std::string &strText, size_t nLen);


	//���������������ô���ԭ����pstrText�ᱻ�޸ģ����سɹ��������
	static size_t	replaceAll(std::string &strText, const char chOld, const char chNew);

	static size_t	replaceAll(std::string &strText, const std::string &strOld, const std::string &strNew);

	static size_t	replaceMany(std::string &strText, const char chOld, const char chNew, size_t nMaxCount = 1);

	static size_t	replaceMany(std::string &strText, const std::string &strOld, const std::string &strNew, size_t nMaxCount = 1);

	static size_t	trimLeft(std::string &strText, const std::string &strTrim = " ");

	static size_t	trimRight(std::string &strText, const std::string &strTrim = " ");

	static size_t	trim(std::string &strText, const std::string &strTrim = " ");

	static void		toLower(std::string &strText);

	static void		toUpper(std::string &strText);


	static bool		split(const std::string &strText, std::vector<std::string> &vecResult, const std::string &strSpan="|,;");

	static int		caseCompare(const std::string &strLeft, const std::string &strRight);
	
	static bool		startWith(const std::string &strLeft, const std::string &strRight);

	static bool		endWith(const std::string &strLeft, const std::string &strRight);

	static bool		caseStartWith(const std::string &strLeft, const std::string &strRight);

	static bool		caseEndWith(const std::string &strLeft, const std::string &strRight);

	static bool		contains(const std::string &strLeft, const std::string &strRight);

	static bool		caseContains(const std::string &strLeft, const std::string &strRight);

	static size_t	format(std::string &strText, const char* pszFormat, ...);


	static int		toInt(const std::string &strText);

	static long		toLong(const std::string &strText);

	static __int64	toInt64(const std::string &strText);

	static float	toFloat(const std::string &strText);

	static double	toDouble(const std::string &strText);


	static std::string	fromInt(int	nVal, int nRadix = 10);

	static std::string	fromLong(long lVal, int nRadix = 10);

	static std::string	fromInt64(__int64 nVal, int nRadix = 10);

	static std::string	fromFloat(float fVal);


	static std::string	toHexString(const void *pData, size_t nLen);

	template<typename _T>
	static std::string	toString(const _T &val);

	static size_t		generateRandString(std::string &strText, const std::string &rndSeed, size_t ulLen = 0);
	static size_t		generateRandString(std::string &strText, const std::string *rndSeeds, size_t seedLen, size_t ulLen = 0);
	static size_t		getRandStringCN(std::string &strText, size_t ulLen = 0);
	static size_t		getRandString(std::string &strText, size_t ulLen = 0);
	
	static std::string	generateRandString_C(const std::string &rndSeed, size_t ulLen = 0);
	static std::string	generateRandString_C(const std::string *rndSeeds, size_t seedLen, size_t ulLen = 0);
	static std::string	getRandStringCN_C(size_t ulLen = 0);
	static std::string	getRandString_C(size_t ulLen = 0);

};







/************************************************************************
  *********       ANSI��UTF8��UNICODE �����ñ�����໥ת��        **********
************************************************************************/
YZM_API std::wstring	Ansi2Unicode(const std::string &str);
YZM_API std::string		Ansi2Utf8(const std::string &str);
YZM_API std::string		Unicode2Ansi(const std::wstring &wstr);
YZM_API std::string		Unicode2Utf8(const std::wstring &wstr);
YZM_API std::string		Utf82Ansi(const std::string &str);
YZM_API std::wstring	Utf82Unicode(const std::string &str);

// ���ֽ�ת���ֽ�
YZM_API std::wstring str2wstr( const std::string &ms, unsigned uCode = 0 /*CP_ACP*/ );
// ���ֽ�ת���ֽ�
YZM_API std::string wstr2str( const std::wstring &ws, unsigned uCode = 0 /*CP_ACP*/ );





/************************************************************************
  *******************       ���õĿ�ֵ��غ���        ********************
************************************************************************/
YZM_API bool	isEmpty(const std::string &str);
YZM_API bool	isEmpty(const char *str);

YZM_API extern const std::string	EMPTY_STRING;

template<typename _T>
std::string	StrT::toString(const _T &val)
{
	std::stringstream	ss;
	ss << val;
	return ss.str();
}

NAMESPACE_END
