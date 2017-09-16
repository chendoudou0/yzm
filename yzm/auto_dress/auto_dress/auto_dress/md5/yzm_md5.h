
/**************************************************************************
Copyright	: Cloudream All Rights Reserved
Author		: chenys@cloudream.net (߹��)
Date		: 2015/08/24

Description	: �ṩһ��MD5������࣬�Լ�caculateMD5���ýӿ�
**************************************************************************/

#pragma once

#include "../zlib/yzm_assist.h"


NAMESPACE_BEGIN

	

/************************************************************************
  *************************       MD5        *************************
************************************************************************/
class  MD5
{
private:
    #define uint8  unsigned char
    #define uint32 unsigned long int

    struct md5_context
    {
        uint32 total[2];
        uint32 state[4];
        uint8 buffer[64];
    };

    void starts( struct md5_context *ctx );
    void process( struct md5_context *ctx, uint8 data[64] );
    void update( struct md5_context *ctx, uint8 *input, uint32 length );
    void finish( struct md5_context *ctx, uint8 digest[16] );

public:
    void generate(unsigned char* buffer,int bufferlen);
    void generate(const std::string &buffer);

    MD5();
    MD5(const char *md5src);
    MD5(unsigned long *md5src);

    MD5 operator+(const MD5 &adder);
    bool operator==(const MD5 &cmper) const;

    std::string toString();

    unsigned long m_data[4];
};


//**********************************************************
// Method   : caculateMD5
// Remark   : ���ɸ����ַ�����MD5ֵ
// DateTime : 2015/08/24
// Parameter: const std::string & str			�ַ���
// Parameter: bool isUpper						��Сд����ĸa~f��
// Returns  : std::string				MD5ֵ
//**********************************************************
YZM_API std::string		caculateMD5(const std::string &str, bool isUpper = true);


NAMESPACE_END
