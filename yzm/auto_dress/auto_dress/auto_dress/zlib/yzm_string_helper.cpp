
/**************************************************************************
Copyright	: Cloudream All Rights Reserved
Author		: chenys@cloudream.net (߹��)
Date		: 2015/08/24

Description	: �ṩһЩstd::string�ĸ���������ʹ��std::string����������˳��һЩ
**************************************************************************/

#include "yzm_string_helper.h"
#include "yzm_log.h"

#include <windows.h>



NAMESPACE_BEGIN










/************************************************************************
  **********************       StringHelper        **********************
************************************************************************/
//һ��int32ת���ַ��������󳤶ȣ��п��ܱ�ת��Ϊ�����ƣ����������Ҫ��Ϊ40
const int	MAX_INT32_CHAR_LEN	= 40;
const int	MAX_INT64_CHAR_LEN	= 80;

//StrT::Format����֧�ֵ�������ַ������ȣ�������/0��
const int		MAX_FORMAT_STR_LEN	= 10240;

size_t			SIZE_HAS_CONTENT	= 1;
const unsigned char		HEX_SIZE	= 16;
char	NUM_TO_HEXCHAR[HEX_SIZE]	=
{
	'0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'A', 'B',
	'C', 'D', 'E', 'F'
};

char	RAND_CHAR_ARRAY[]	= "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const size_t	RAND_SIZE	= ARRAY_LEN(RAND_CHAR_ARRAY);


/*
����strOld��strNew�Ĵ����Ȳ�һ���������滻�Ĺ�����Ҫ�ǵ��ʵ�ƫ�Ƶ�����������������ѭ�����滻���������
�磬����123abc123def����old=��123����new=��123#123������Ϊ�滻������ʵʱ�ģ������滻��һ��123�󣬴�Ϊ
��123#123abc123def����ʱ�������ƫ�Ƶ���������ô��ͷ��Զ��123���������İѵ�����+1Ҳ�����⣬��Ϊ#���滹�и�
123��������ô�滻������ѭ������������Ĭ��ƫ��new�ĳ��ȡ���ֹ��Щ��������ķ���
*/
//˽��ͨ���滻������nCount���滻������-1����ȫ���滻������ʵ���滻����
size_t		__replaceMany(std::string &strText, const std::string &strOld, const std::string &strNew, size_t nMaxCount = 0)
{
	if ( isEmpty(strText) || isEmpty(strOld) || strOld == strNew)
	{
		return	0;
	}

	size_t		nReplaceCount	= 0;
	std::string::size_type	nPos	= 0;

	//�������������࣬��Ҫ����Ϊ��������ͨ�õĺ������迼��Ч�����⣬��Ҫʱ����if�ж�
	if (nMaxCount > 0)
	{
		//�д�������
		while ( (nPos = strText.find(strOld, nPos)) != std::string::npos )
		{
			strText.replace(nPos, strOld.size(), strNew.c_str(), strNew.size());

			++nReplaceCount;
			if ( nReplaceCount >= nMaxCount )
			{
				//���滻��������ʱ���ֻ�滻nMaxCount��
				break;
			}

			nPos	+= strNew.size();
		}
	}
	else
	{
		//ȫ���滻
		while ( (nPos = strText.find(strOld, nPos)) != std::string::npos )
		{
			strText.replace(nPos, strOld.size(), strNew.c_str(), strNew.size());
			++nReplaceCount;
			nPos	+= strNew.size();
		}
	}
		
	return	nReplaceCount;
}


std::string StrT::replaceAll_C(const std::string &strText, const char chOld, const char chNew)
{
	WarningIf_r( strText.empty() || chOld == chNew, strText );
	std::string	strReturn	= strText;
	
	__replaceMany(strReturn, std::string(1, chOld), std::string(1, chNew));

	return	strReturn;
}

std::string StrT::replaceAll_C(const std::string &strText, const std::string &strOld, const std::string &strNew)
{
	WarningIf_r( strText.empty() || strOld.empty() || strOld == strNew, strText );
	std::string	strReturn	= strText;

	__replaceMany(strReturn, strOld, strNew);

	return	strReturn;
}
std::string StrT::replaceMany_C(const std::string &strText, const char chOld, const char chNew, size_t nMaxCount)
{
	WarningIf_r( strText.empty() || chOld == chNew || nMaxCount <= 0, strText );
	std::string	strReturn	= strText;

	__replaceMany(strReturn, std::string(1, chOld), std::string(1, chNew), nMaxCount);

	return	strReturn;
}

std::string StrT::replaceMany_C(const std::string &strText, const std::string &strOld, const std::string &strNew, size_t nMaxCount)
{
	WarningIf_r( strText.empty() || strOld.empty() || strOld == strNew || nMaxCount <= 0, strText );
	std::string	strReturn	= strText;

	__replaceMany(strReturn, strOld, strNew, nMaxCount);

	return	strReturn;
}

std::string StrT::trimLeft_C(const std::string &strText, const std::string &strTrim)
{
	//WarningIf_r( strText.empty() || strTrim.empty(), strText );

	std::string	strReturn	= strText;
	trimLeft(strReturn, strTrim);
	return	strReturn;
}

std::string StrT::trimRight_C(const std::string &strText, const std::string &strTrim)
{
	//WarningIf_r( strText.empty() || strTrim.empty(), strText );

	std::string	strReturn	= strText;
	trimRight(strReturn, strTrim);
	return	strReturn;
}

std::string StrT::trim_C(const std::string &strText, const std::string &strTrim)
{
	//WarningIf_r( strText.empty() || strTrim.empty(), strText );
	
	std::string	strReturn	= strText;
	trim(strReturn, strTrim);
	return	strReturn;
}

std::string StrT::toLower_C(const std::string &strText)
{
	WarningIf_r( strText.empty(), strText );

	std::string	strReturn	= strText;
	toLower(strReturn);
	return	strReturn;
}

std::string StrT::toUpper_C(const std::string &strText)
{
	WarningIf_r( strText.empty(), strText );

	std::string	strReturn	= strText;
	toUpper(strReturn);
	return	strReturn;
}

std::string StrT::left(const std::string &strText, size_t nLen)
{
	WarningIf_r( strText.empty() || nLen == 0, EMPTY_STRING );
	WarningIf_r( strText.size() < nLen, strText );

	if (strText.empty() || nLen == 0)
	{
		return	EMPTY_STRING;
	}

	if (strText.size() <= nLen)//size() == nLen�ǺϷ���
	{
		return	strText;
	}

	return	strText.substr(0, nLen);
}

std::string StrT::mid(const std::string &strText, size_t nStart, size_t nLen)
{
	WarningIf_r( strText.empty() || nLen == 0 || nStart > strText.size(), EMPTY_STRING );

	if (strText.empty() || nLen == 0 || nStart > strText.size())
	{
		return	EMPTY_STRING;
	}

	if (nStart == 0)
	{
		nStart = 1;
	}

	return	strText.substr(nStart-1, nLen);
}

std::string StrT::right(const std::string &strText, size_t nLen)
{
	WarningIf_r( strText.empty() || nLen == 0, EMPTY_STRING );
	WarningIf_r( strText.size() < nLen, strText );

	if (strText.empty() || nLen == 0)
	{
		return	EMPTY_STRING;
	}

	if (strText.size() <= nLen)//size() == nLen�ǺϷ���
	{
		return	strText;
	}

	return	strText.substr(strText.size()-nLen, nLen);
}





size_t StrT::replaceAll(std::string &strText, const char chOld, const char chNew)
{
	WarningIf_r( strText.empty() || chOld == chNew, 0 );
	
	return	__replaceMany(strText, std::string(1, chOld), std::string(1, chNew));
}

size_t StrT::replaceAll(std::string &strText, const std::string &strOld, const std::string &strNew)
{
	WarningIf_r( strText.empty() || strOld.empty() || strOld == strNew, 0 );
	
	return	__replaceMany(strText, strOld, strNew);
}

size_t StrT::replaceMany(std::string &strText, const char chOld, const char chNew, size_t nMaxCount)
{
	WarningIf_r( strText.empty() || chOld == chNew || nMaxCount <= 0, 0 );

	return	__replaceMany(strText, std::string(1, chOld), std::string(1, chNew), nMaxCount);
}

size_t StrT::replaceMany(std::string &strText, const std::string &strOld, const std::string &strNew, size_t nMaxCount)
{
	WarningIf_r( strText.empty() || strOld.empty() || strOld == strNew || nMaxCount <= 0, 0 );

	return	__replaceMany(strText, strOld, strNew, nMaxCount);
}

size_t StrT::trimLeft(std::string &strText, const std::string &strTrim)
{
	if (strText.empty() || strTrim.empty())
	{
		return	0;
	}

	std::string::size_type	nPos	= strText.find_first_not_of(strTrim);
	std::string::size_type	nOldSize= strText.size();

	if (nPos == std::string::npos)
	{
		//û���ҵ�strTrim������ַ�������strTextȫ���ַ�����Ҫ���
		strText.erase(strText.begin(), strText.end());
	}
	else
	{
		strText.erase(0, nPos);
	}

	return	(nOldSize - strText.size());
}

size_t StrT::trimRight(std::string &strText, const std::string &strTrim)
{
	if (strText.empty() || strTrim.empty())
	{
		return	0;
	}

	std::string::size_type	nPos	= strText.find_last_not_of(strTrim);
	std::string::size_type	nOldSize= strText.size();

	if (nPos == std::string::npos)
	{
		//û���ҵ�strTrim������ַ�������strTextȫ���ַ�����Ҫ���
		strText.erase(strText.begin(), strText.end());
	}
	else
	{
		strText.erase(nPos+1);
	}

	return	(nOldSize - strText.size());
}

size_t StrT::trim(std::string &strText, const std::string &strTrim)
{
	if (strText.empty() || strTrim.empty())
	{
		return	0;
	}

	std::string::size_type	nPos1	= strText.find_first_not_of(strTrim);
	std::string::size_type	nPos2	= strText.find_last_not_of(strTrim);
	std::string::size_type	nOldSize= strText.size();

	if (nPos1 == std::string::npos || nPos2 == std::string::npos)
	{
		//û���ҵ�strTrim������ַ�������strTextȫ���ַ�����Ҫ���
		strText.erase(strText.begin(), strText.end());
	}
	else
	{
		strText.erase(nPos2+1);
		strText.erase(0, nPos1);
	}

	return	(nOldSize - strText.size());
}

void StrT::toLower(std::string &strText)
{
	WarningIf( strText.empty() );

	if (!strText.empty())
	{
		std::transform(strText.begin(), strText.end(), strText.begin(), tolower);
	}
}

void StrT::toUpper(std::string &strText)
{
	WarningIf( strText.empty() );

	if (!strText.empty())
	{
		std::transform(strText.begin(), strText.end(), strText.begin(), toupper);
	}
}


bool StrT::split(const std::string &strText, std::vector<std::string> &vecResult, const std::string &strSpan)
{
	vecResult.clear();
	WarningIf_r(strText.empty() || strSpan.empty(), false);

	if (strText.empty() || strSpan.empty() )
	{
		return	false;
	}

	size_t		nOldPos	= 0;
	size_t		nPos	= 0;

	nPos	= strText.find_first_of(strSpan, nOldPos);
	if (std::string::npos == nPos)
	{
		vecResult.push_back(strText);
		return	true;
	}

	while (nPos != std::string::npos)
	{
		if ( (nPos-nOldPos) >= SIZE_HAS_CONTENT )
		{
			vecResult.push_back(strText.substr(nOldPos, nPos-nOldPos));
		}

		nOldPos	= nPos + 1;
		nPos	= strText.find_first_of(strSpan, nOldPos);
	}

	//��ֹ©�����һ��
	if ( nOldPos < strText.size() )
	{
		vecResult.push_back(strText.substr(nOldPos, strText.size()-nOldPos));
	}

	return	(!vecResult.empty());
}

int StrT::caseCompare(const std::string &strLeft, const std::string &strRight)
{
#if defined(_MSC_VER) && _MSC_VER >= 1400
	return	_stricmp(strLeft.c_str(), strRight.c_str());
#else
	return	stricmp(strLeft.c_str(), strRight.c_str());
#endif
}

bool StrT::startWith(const std::string &strLeft, const std::string &strRight)
{
	WarningIf_r(strLeft.empty() || strRight.empty(), false);
	if (strLeft.empty() || strRight.empty() || strLeft.size() < strRight.size())
	{
		return	false;
	}

	if (&strLeft == &strRight)
	{
		return	true;
	}

	size_t		nPos	= 0;
	for (; nPos < strRight.size(); ++nPos)
	{
		if (strLeft[nPos] != strRight[nPos])
		{
			return	false;
		}
	}

	return	true;
}

bool StrT::endWith(const std::string &strLeft, const std::string &strRight)
{
	WarningIf_r(strLeft.empty() || strRight.empty(), false);
	if (strLeft.empty() || strRight.empty() || strLeft.size() < strRight.size())
	{
		return	false;
	}

	if (&strLeft == &strRight)
	{
		return	true;
	}

	size_t		nPos;
	size_t		nDataEnd	= strLeft.size() - 1;
	size_t		nTailEnd	= strRight.size() - 1;

	for ( nPos=0; nPos<strRight.size(); ++nPos )
	{
		if ( strLeft[nDataEnd-nPos] != strRight[nTailEnd-nPos] )
		{
			return	false;
		}
	}

	return	true;
}

bool StrT::caseStartWith(const std::string &strLeft, const std::string &strRight)
{
	WarningIf_r(strLeft.empty() || strRight.empty(), false);
	if (strLeft.empty() || strRight.empty() || strLeft.size() < strRight.size())
	{
		return	false;
	}

	if (&strLeft == &strRight)
	{
		return	true;
	}

	size_t		nPos	= 0;
	for (; nPos < strRight.size(); ++nPos)
	{
		if (tolower(strLeft[nPos]) != tolower(strRight[nPos]))
		{
			return	false;
		}
	}

	return	true;
}

bool StrT::caseEndWith(const std::string &strLeft, const std::string &strRight)
{
	WarningIf_r(strLeft.empty() || strRight.empty(), false);
	if (strLeft.empty() || strRight.empty() || strLeft.size() < strRight.size())
	{
		return	false;
	}

	if (&strLeft == &strRight)
	{
		return	true;
	}

	size_t		nPos;
	size_t		nDataEnd	= strLeft.size() - 1;
	size_t		nTailEnd	= strRight.size() - 1;

	for ( nPos=0; nPos<strRight.size(); ++nPos )
	{
		if ( tolower(strLeft[nDataEnd-nPos]) != tolower(strRight[nTailEnd-nPos]) )
		{
			return	false;
		}
	}

	return	true;
}

bool	StrT::contains(const std::string &strLeft, const std::string &strRight)
{
	return ( strLeft.find(strRight) != std::string::npos );
}

bool	StrT::caseContains(const std::string &strLeft, const std::string &strRight)
{
	std::string	lowerLeft	= toLower_C(strLeft);
	std::string lowerRight	= toLower_C(strRight);

	return contains(lowerLeft, lowerRight);
}

size_t StrT::format(std::string &strText, const char* pszFormat, ...)
{
	ErrorIf_r(NULL == pszFormat, 0 );

	if (NULL == pszFormat)
	{
		return	0;
	}

	if ( strlen(pszFormat) <= 0 )
	{
		strText.clear();
		return	0;
	}

	char	szBuffer[MAX_FORMAT_STR_LEN]	= {0};

	va_list		_args;
	va_start(_args, pszFormat);

	//�����_TRUNCATE����Ҫ�����MAX_FORMAT_STR_LEN�ᵼ�¹���ʱ����
#if defined(_MSC_VER) && _MSC_VER >= 1400
	int	nReturn	= _vsnprintf_s(szBuffer, MAX_FORMAT_STR_LEN, _TRUNCATE, pszFormat, _args);
#else
	int	nReturn	= _vsnprintf(szBuffer, MAX_FORMAT_STR_LEN, pszFormat, _args);
#endif

	if (nReturn == -1 || nReturn == MAX_FORMAT_STR_LEN)
	{
		//WarningLn(" nReturn == -1 || nReturn == MAX_FORMAT_STR_LEN");
		szBuffer[MAX_FORMAT_STR_LEN-1]	= '\0';
	}

	va_end(_args);

	strText.assign(szBuffer);
	return	strText.size();
}

int StrT::toInt(const std::string &strText)
{
	WarningIf_r( strText.empty(), 0 );
	if (strText.empty())
	{
		return	0;
	}
	
	/*	atoi()��ɨ�����nptr�ַ���������ǰ��Ŀո��ַ���ֱ���������ֻ��������Ųſ�ʼ��ת��
	�������������ֻ��ַ�������ʱ('\0')�Ž���ת�� */
	return	atoi(strText.c_str());
}

long StrT::toLong(const std::string &strText)
{
	WarningIf_r( strText.empty(), 0 );
	if (strText.empty())
	{
		return	0;
	}

	/*	atol()��ɨ�����nptr�ַ���������ǰ��Ŀո��ַ���ֱ���������ֻ��������Ųſ�ʼ��ת��
	�������������ֻ��ַ�������ʱ('\0')�Ž���ת�� */
	return	atol(strText.c_str());
}

__int64 StrT::toInt64(const std::string &strText)
{
	WarningIf_r( strText.empty(), 0 );
	if (strText.empty())
	{
		return	0;
	}

	return	_atoi64(strText.c_str());
}

float StrT::toFloat(const std::string &strText)
{
	WarningIf_r( strText.empty(), (float)0.0 );
	if (strText.empty())
	{
		return	(float)0.0;
	}

	/*	atof()��ɨ�����nptr�ַ���������ǰ��Ŀո��ַ���ֱ���������ֻ��������Ųſ�ʼ��ת��
	�������������ֻ��ַ�������ʱ('\0')�Ž���ת��������������ء�����nptr�ַ����ɰ���������
	С�����E(e)����ʾָ�����֣���123.456��123e-2 */
	return	(float)atof(strText.c_str());
}

double StrT::toDouble(const std::string &strText)
{
	WarningIf_r( strText.empty(), (float)0.0 );
	if (strText.empty())
	{
		return	(double)0.0;
	}

	return	atof(strText.c_str());
}

/*
����ת���������ǲ�̫��ȫ�ģ�ʹ���ڲ����壬�д��Ż�
*/
std::string StrT::fromInt(int nVal, int nRadix)
{
	static char		szBuffer[MAX_INT32_CHAR_LEN];

#if defined(_MSC_VER) && _MSC_VER >= 1400
	_itoa_s(nVal, szBuffer, MAX_INT32_CHAR_LEN, nRadix);
#else
	_itoa(nVal, szBuffer, nRadix);
#endif

	return	std::string(szBuffer);
}

std::string StrT::fromLong(long lVal, int nRadix)
{
	static char		szBuffer[MAX_INT32_CHAR_LEN];

#if defined(_MSC_VER) && _MSC_VER >= 1400
	_ltoa_s(lVal, szBuffer, MAX_INT32_CHAR_LEN, nRadix);
#else
	_ltoa(lVal, szBuffer, nRadix);
#endif

	return	std::string(szBuffer);
}

std::string StrT::fromInt64(__int64 nVal, int nRadix)
{
	static char		szBuffer[MAX_INT64_CHAR_LEN];

#if defined(_MSC_VER) && _MSC_VER >= 1400
	_i64toa_s(nVal, szBuffer, MAX_INT64_CHAR_LEN, nRadix);
#else
	_i64toa(nVal, szBuffer, nRadix);
#endif

	return	std::string(szBuffer);
}

std::string StrT::fromFloat(float fVal)
{
	static char		szBuffer[MAX_INT64_CHAR_LEN];

#if defined(_MSC_VER) && _MSC_VER >= 1400
	sprintf_s(szBuffer, MAX_INT64_CHAR_LEN, "%.2f", fVal);
#else
	sprintf(szBuffer, "%f", fVal);
#endif

	return	std::string(szBuffer);
}

std::string StrT::toHexString(const void *pData, size_t nLen)
{
	ErrorIf_r(NULL == pData || nLen == 0, EMPTY_STRING);
	if (NULL == pData || nLen == 0)
	{
		return	EMPTY_STRING;
	}

	//Ԥ�ȷ���ռ䣬���Ч��
	std::string		strHex	= "";
	strHex.reserve(nLen << 1);

	const unsigned char*	pszData	= static_cast<const unsigned char*>(pData);
	for (size_t i = 0; i< nLen; ++i)
	{
		strHex.append(1, NUM_TO_HEXCHAR[ pszData[i]/HEX_SIZE]);
		strHex.append(1, NUM_TO_HEXCHAR[ pszData[i]%HEX_SIZE]);
	}

	return	strHex;
}

size_t	StrT::generateRandString(std::string &strText, const std::string &rndSeed, size_t ulLen)
{
	static unsigned short		_bLoop	= 0;
	//���ڸ�����������ӣ������ظ�������
	if ( _bLoop++ % 1000 == 0 )
	{
		std::srand( (unsigned int)std::clock() );
	}

	if (ulLen == 0)
	{
		ulLen	= std::rand() % 32 + 1;
	}

	char	chRand;
	strText.clear();
	for (size_t i = 0; i < ulLen; ++i)
	{
		chRand	= rndSeed[ std::rand() % rndSeed.size() ];
		strText.append(1, chRand);
	}

	return	ulLen;
}

size_t	StrT::generateRandString(std::string &strText, const std::string *rndSeeds, size_t seedLen, size_t ulLen)
{
	static unsigned short		_bLoop	= 0;
	//���ڸ�����������ӣ������ظ�������
	if ( _bLoop++ % 1000 == 0 )
	{
		std::srand( (unsigned int)std::clock() );
	}

	if (ulLen == 0)
	{
		ulLen	= std::rand() % 32 + 1;
	}

	strText.clear();
	for (size_t i = 0; i < ulLen; ++i)
	{
		const std::string &chRand	= rndSeeds[ std::rand() % seedLen ];
		strText.append( chRand );
	}

	return	ulLen;
}

size_t StrT::getRandString(std::string &strText, size_t ulLen/* = 0 */)
{
	static const std::string	notCnChar(RAND_CHAR_ARRAY, RAND_SIZE);
	return	generateRandString(strText, notCnChar, ulLen);
}

size_t StrT::getRandStringCN(std::string &strText, size_t ulLen/* = 0 */)
{
	/*
		01 - 09 ��Ϊ������š�
		16 - 55 ��Ϊһ�����֣���ƴ������
		56 - 87 ��Ϊ�������֣�������/�ʻ�����

		 ��λ�ֽ� ʹ���� 0xA1 - 0xF7���� 01 - 87 �������ż��� 0xA0��
		 ��λ�ֽ� ʹ���� 0xA1 - 0xFE���� 01 - 94 λ��λ�ż��� 0xA0��
	*/

	static unsigned short		_bLoop	= 0;
	//���ڸ�����������ӣ������ظ�������
	if ( _bLoop++ % 1000 == 0 )
	{
		std::srand( (unsigned long)std::clock() );
	}
	
	static const int	CHAR_BASE	= 0xA0;

	if (ulLen == 0)
	{
		ulLen	= std::rand() % 32 + 1;
	}
	
	// ����ֻ����һ��������
	int		quCode, weiCode;

	strText.clear();
	for (size_t i = 0; i < ulLen; ++i)  
	{  
		// ���� 16 ~ 55
		quCode	= (std::rand() % 40) + 16;
		// λ�� 01 ~ 94������55�����5λ�ǿյ�
		weiCode	= (std::rand() % (quCode == 55 ? 89 : 94)) + 1;

		strText.append( 1, char(quCode + CHAR_BASE) );
		strText.append( 1, char(weiCode + CHAR_BASE) );
	}

	return	ulLen;
}

std::string	StrT::generateRandString_C(const std::string &rndSeed, size_t ulLen)
{
	std::string		ret;
	generateRandString(ret, rndSeed, ulLen);
	return ret;
}

std::string	StrT::generateRandString_C(const std::string *rndSeeds, size_t seedLen, size_t ulLen)
{
	std::string		ret;
	generateRandString(ret, rndSeeds, seedLen, ulLen);
	return ret;
}

std::string	StrT::getRandStringCN_C(size_t ulLen)
{
	std::string		ret;
	getRandStringCN(ret, ulLen);
	return ret;
}

std::string	StrT::getRandString_C(size_t ulLen)
{
	std::string		ret;
	getRandString(ret, ulLen);
	return ret;
}












/************************************************************************
  *********       ANSI��UTF8��UNICODE �����ñ�����໥ת��        **********
************************************************************************/
std::wstring	Ansi2Unicode(const std::string &str)
{
	std::wstring	unicode;
	int	len	= ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	unicode.resize(len-1);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &unicode[0], len);

	return unicode;
}

std::string		Ansi2Utf8(const std::string &str)
{
	std::wstring	unicode;
	unicode	= Ansi2Unicode(str);
	return Unicode2Utf8(unicode);
}

std::string		Unicode2Ansi(const std::wstring &wstr)
{
	std::string		ansi;
	int	len = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	ansi.resize(len-1);
	::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &ansi[0], len, NULL, NULL);

	return ansi;
}

std::string		Unicode2Utf8(const std::wstring &wstr)
{
	std::string		utf;
	int	len = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	utf.resize(len-1);
	::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf[0], len, NULL, NULL);

	return utf;
}

std::string		Utf82Ansi(const std::string &str)
{
	std::wstring	unicode;
	unicode	= Utf82Unicode(str);
	return Unicode2Ansi(unicode);
}

std::wstring	Utf82Unicode(const std::string &str)
{
	std::wstring	unicode;
	int	len = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	unicode.resize(len-1);
	::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &unicode[0], len);

	return unicode;
}

std::wstring str2wstr(const std::string &ms, unsigned uCode /*= CP_ACP */)
{
    int nLen = MultiByteToWideChar( uCode, 0, ms.c_str(), -1, NULL, 0 );
    if( nLen <= 0 )
        return std::wstring();
    else
    {
        wchar_t *wc = new wchar_t[ nLen ];
        _ASSERT( wc != NULL );
        MultiByteToWideChar( uCode, 0, ms.c_str(), -1, wc, nLen );
        std::wstring ws( wc );
        delete [] wc;
        return ws;
    }
}

std::string wstr2str(const std::wstring &ws, unsigned uCode /*= CP_ACP */)
{
    int nLen = WideCharToMultiByte( uCode, 0, ws.c_str(), -1, NULL, 0, NULL, NULL );
    if( nLen <= 0 )
        return std::string();
    else
    {
        char *mc = new char[ nLen ];
        _ASSERT( mc != NULL );
        WideCharToMultiByte( uCode, 0, ws.c_str(), -1, mc, nLen, NULL, NULL );
        std::string ms( mc );
        delete [] mc;
        return ms;
    }
}









/************************************************************************
  *******************       ���õĿ�ֵ��غ���        ********************
************************************************************************/
const std::string	EMPTY_STRING	= "";

bool	isEmpty(const std::string &str)
{
	return isEmpty(str.c_str());
}

bool	isEmpty(const char *str)
{
	std::string		s	= StrT::trim_C(str);
	return ("" == s);
}


NAMESPACE_END
