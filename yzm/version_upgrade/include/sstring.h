//
// sstring.h
// super string class
// view: tab=8 indent=4
// 2006-12-07
//
//

#ifndef __SSTRING_H__
#define __SSTRING_H__

#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef WIN32
#   pragma warning(disable:4267)
#endif

//////////////////////////////////////////////////////////////////////////

template <typename CharType>
    class super_string : public std::basic_string<CharType>
{
public:
    typedef std::basic_string<CharType> base;
    typedef CharType value_type;
    typedef typename base::size_type size_type;
    typedef typename base::traits_type traits_type;
    typedef typename base::allocator_type allocator_type;
		
    super_string() 
      {}		
	
    super_string(const super_string& value) : base(value)
      {}

    super_string(const base& str) : base(str)
      {}

    super_string(const value_type* szStr) : base(szStr)
      {}

    super_string(const value_type* szStr, size_type _Count, const allocator_type& al = allocator_type())
      : base(szStr, _Count, al)
      {}

    super_string(
		 const base& _Right,
		 size_type _Roff,
		 size_type _Count = base::npos
		 ) : base(_Right, _Roff, _Count)
      {}
	
    super_string(
		 size_type _Count,
		 value_type _Ch
		 ) : base(_Count, _Ch)
      {}

    super_string& upcase() 
      {
	return (*this);
      }
	
    super_string& downcase()
      {
	return (*this);
      }

    super_string& operator = (const value_type* pElem) {
	this->assign(pElem);
	return (*this);
    }

    super_string& operator = (value_type _Ch) {
	this->assign(_Ch);
	return (*this);
    }

    super_string& operator = (const super_string& _Right) {
	this->assign(_Right);
	return (*this);
    }
	
    // implement in special template member implementaion
    // if szTrimChar == NULL, space, tab, newline, return
    // will be removed
    super_string& rtrim(const CharType* szTrimChar = NULL)
	{		
	    return (*this);		
	}
	
    super_string& ltrim(const CharType* szTrimChar = NULL)
	{
	    return (*this);
	}
	
    super_string& trim(const CharType* szTrimChar = NULL)
	{
	    return rtrim(szTrimChar).ltrim(szTrimChar);
	}

    bool isDigit()const 
	{
	    return true;
	}	
	
    super_string& truncate(int maxLength) {
	if (maxLength < base::length())
	    base::erase(maxLength);
	return (*this);
    }

    super_string right(size_t size)const {
    	size_t p = this->length();
    	if (p < size)
	    p = 0;
	else
	    p -= size;
	
    	return this->substr(p);
    }

    super_string left(size_t size)const {
    	if (size >= this->length())
	    size = base::npos;
	
    	return this->substr(0, size);
    }

    // 根据分割字符串分割字符串，如果字符串中匹配分割符N次，
    // 生成的字符串向量中有 N+1 个元素
    std::vector<super_string> split(const value_type* splitter)const {
	std::vector<super_string> strv;
	size_type pos = 0, endpos = 0;
	size_type len = traits_type::length(splitter);		

	while (pos <= this->length()) {					
	    for (endpos = pos;endpos < this->length() && traits_type::find(splitter, len, this->at(endpos)) == NULL; 
		 ++endpos
		 );

	    strv.push_back(this->substr(pos, endpos - pos));
	    pos = endpos + len;		
	}

	return strv;
    }

    // 根据分割符分割字符串，如果字符串中匹配分割符N次，
    // 生成的字符串向量中有 N+1 个元素
    std::vector<super_string> split(const value_type splitter)const {
	std::vector<super_string> strv;
	size_type pos = 0, endpos = 0;
	
	while (true) {	    
	    endpos = this->find(splitter, pos);
	    if (endpos == base::npos)
	    {
		strv.push_back(this->substr(pos));
		break;
	    }
	    else
	    {
		strv.push_back(this->substr(pos, endpos - pos));
		pos = endpos + 1;

		if (pos == this->length())
		{
		    strv.push_back("");
		    break;
		}
	    }	    
	}

	return strv;
    }

    static super_string format(const value_type* fmt, ...)
	{
	    return super_string();
	}

    static super_string vformat(const value_type* fmt, va_list ap)
	{
	    return super_string();
	}
};


//
// template partialize for string
//
template <> inline
    super_string<char>& 
    super_string<char>::downcase()
{	
    size_type dis = 'a' - 'A';
    for (size_type i = 0; i < this->length(); ++i) {
	char& ch = (*this)[i];
	if (ch >= 'A' && ch <= 'Z')
	    ch += dis;
    }
		
    return (*this);
}

template <> inline
    super_string<char>& 
    super_string<char>::upcase()
{
    unsigned int dis = 'A' - 'a';
    for (unsigned int i = 0; i < this->length(); ++i) {
	char& ch = (*this)[i];
	if (ch >= 'a' && ch <= 'z')
	    ch += dis;
    }

    return (*this);
}

template <> inline
    super_string<char>& 
    super_string<char>::ltrim(const char* szTrimChar)
{
    if (szTrimChar == NULL)
	szTrimChar = "\x20\t\r\n";

    for (;!empty() && strchr(szTrimChar, (*this)[0]) != NULL;) {
	erase(begin());
    }	

    return (*this);
}

template <> inline
    super_string<char>&
    super_string<char>::rtrim(const char* szTrimChar)
{
    if (szTrimChar == NULL)
	szTrimChar = "\x20\t\r\n";

    int pos = (int)this->length() - 1;
    for (;pos >= 0 && strchr(szTrimChar, (*this)[pos]) != NULL; --pos) {
	this->erase(pos);
    }

    return (*this);
}

template<> inline
    bool super_string<char>::isDigit()const
{
    if (this->empty())
	return false;
	
    for (unsigned int i = 0; i < this->length(); ++i) {
	char ch = at(i);
	if (ch < '0' || ch > '9')
	    return false;
    }
	
    return true;
}

template<> inline
    super_string<char>
    super_string<char>::vformat(const char* fmt, va_list ap)
{
    super_string str;
	
#ifdef _GNU_SOURCE
    char* szBuff = NULL;
    vasprintf(&szBuff, fmt, ap);
    if (szBuff != NULL)
	{
	    str = szBuff;
	    free(szBuff);
	}
#else
    int nBytes = 1023; //_vscprintf(fmt, ap);
    char* buff = new char[nBytes + 1];
    if (buff)
	{
	    vsprintf(buff, fmt, ap);
	    str = buff;
	    delete[] buff;
	}	
#endif
	
    return str;
}

template <> inline
    super_string<char> 
    super_string<char>::format(const char* fmt, ...)
{
    super_string str;
    va_list ap;
    va_start(ap, fmt);
    // Must be after vformat
    str = vformat(fmt, ap);
    va_end(ap);
    return str;
}


//
// template partialize for wide string
//
template<> inline	
    super_string<wchar_t>& 
    super_string<wchar_t>::downcase()
{
    size_type dis = L'a' - L'A';
    for (size_type i = 0; i < this->length(); ++i) {
	wchar_t& ch = (*this)[i];
	if (ch >= L'A' && ch <= L'Z')
	    ch += dis;
    }
	
    return (*this);	
}

template <> inline
    super_string<wchar_t>& 
    super_string<wchar_t>::upcase()
{
    size_type dis = L'A' - L'a';
    for (size_type i = 0; i < this->length(); ++i) {
	wchar_t& ch = (*this)[i];
	if (ch >= L'a' && ch <= L'z')
	    ch += dis;
    }
	
    return (*this);
}

template <> inline
    super_string<wchar_t>&
    super_string<wchar_t>::ltrim(const wchar_t* szTrimChar)
{
    if (szTrimChar == NULL)
	szTrimChar = L"\x20\t\r\n";
	
    for (;!empty() && wcschr(szTrimChar, (*this)[0]) != NULL;) {
	erase(begin());
    }	
	
    return (*this);		
}

template <> inline
    super_string<wchar_t>&
    super_string<wchar_t>::rtrim(const wchar_t* szTrimChar)
{
    if (szTrimChar == NULL)
	szTrimChar = L"\x20\t\r\n";
	
    int pos = (int)length() - 1;
    for (;pos >= 0 && wcschr(szTrimChar, (*this)[pos]) != NULL; --pos) {
	erase(pos);
    }
	
    return (*this);		
}

template<> inline
    bool super_string<wchar_t>::isDigit()const
{
    if (this->empty())
	return false;
	
    for (size_type i = 0; i < this->length(); ++i) {
	wchar_t ch = this->at(i);
	if (ch < L'0' || ch > L'9')
	    return false;
    }
	
    return true;
}

template<> inline
    super_string<wchar_t>
    super_string<wchar_t>::vformat(const wchar_t* fmt, va_list ap)
{
    // not implementation
    return super_string();
}

template <> inline
    super_string<wchar_t>
    super_string<wchar_t>::format(const wchar_t* fmt, ...)
{
    // not implementation
    return super_string();
}

//////////////////////////////////////////////////////////////////////////

typedef super_string<char> SuperString;
typedef super_string<wchar_t> WSuperString;

typedef std::vector<SuperString> sstr_vector_t;

//////////////////////////////////////////////////////////////////////////

#endif // __SSTRING_H__

