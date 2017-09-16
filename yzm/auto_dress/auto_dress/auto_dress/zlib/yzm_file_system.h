
/**************************************************************************
Copyright	: Cloudream All Rights Reserved
Author		: chenys@cloudream.net (߹��)
Date		: 2015/08/24

Description	: �ṩһ���ļ�ϵͳ�����࣬�ṩ���õ��ļ����������ļ���Ŀ¼�ĸ��ơ��ƶ���ɾ��
	���ж��ļ�·���ĸ�ʽ����ת��������Ŀǰȫ������Win32 APIʵ�֣���δ��ƽ̨��
**************************************************************************/

#pragma once

#include "yzm_assist.h"


NAMESPACE_BEGIN


/// TODO: ��Ҫ���¹�һ��
class FSys;
typedef		FSys	FileSystem;



/************************************************************************
  ***********************         FSys          ***********************
************************************************************************/
class  FSys
{
public:
	
	//**********************************************************
	// Method   : isFileExist
	// Remark   : �ж�ָ���ļ��Ƿ���ڣ���isDirExist�����֣�
	// DateTime : 2015/08/24
	// Parameter: const std::string & strFilePath	�ļ�·��
	// Returns  : bool
	//**********************************************************
	static bool		isFileExist(const std::string &strFilePath);
	
	
	//**********************************************************
	// Method   : toDeleteFile
	// Remark   : ɾ��ָ���ļ�
	// DateTime : 2015/08/24
	// Parameter: const std::string & strFilePath	�ļ�·��
	// Returns  : bool
	//**********************************************************
	static bool		toDeleteFile(const std::string &strFilePath);

	
	//**********************************************************
	// Method   : toCopyFile
	// Remark   : �����ļ�
	// DateTime : 2015/08/24
	// Parameter: const std::string & strFileSrc	Դ�ļ�·��
	// Parameter: const std::string & strFileDst	Ŀ���ļ�·��
	// Parameter: bool bIsReplace		 Ŀ���ļ�����ʱ�Ƿ񸲸�
	// Returns  : bool
	//**********************************************************
	static bool		toCopyFile(const std::string &strFileSrc, const std::string &strFileDst, bool bIsReplace = true);

	
	//**********************************************************
	// Method   : toMoveFile
	// Remark   : �ƶ��ļ�
	// DateTime : 2015/08/24
	// Parameter: const std::string & strFileSrc	Դ�ļ�·��
	// Parameter: const std::string & strFileDst	Ŀ���ļ�·��
	// Parameter: bool bIsReplace		Ŀ���ļ�����ʱ�Ƿ񸲸�
	// Returns  : bool
	//**********************************************************
	static bool		toMoveFile(const std::string &strFileSrc, const std::string &strFileDst, bool bIsReplace = true);
	
	
	//**********************************************************
	// Method   : isDirExist����isFileExist�����֣�
	// Remark   : �ж�ָ��Ŀ¼�Ƿ����
	// DateTime : 2015/08/24
	// Parameter: const std::string & strDirPath	Ŀ¼·��
	// Returns  : bool
	//**********************************************************
	static bool		isDirExist(const std::string &strDirPath);

	
	//**********************************************************
	// Method   : toCreateDir
	// Remark   : ����Ŀ¼�����뱣֤��Ŀ¼���ڣ�
	// DateTime : 2015/08/24
	// Parameter: const std::string & strDirPath	��Ҫ������Ŀ¼·��
	// Returns  : bool
	//**********************************************************
	static bool		toCreateDir(const std::string &strDirPath);

	
	//**********************************************************
	// Method   : toCreateDirLoop
	// Remark   : ����Ŀ¼���ݹ鴴����
	//			�ŵ㣺 ���豣֤��Ŀ¼�������汲Ŀ¼�Ƿ����
	//			ȱ�㣺 �����˸����У�飬��toCreateDir����ʱ
	// DateTime : 2015/08/24
	// Parameter: const std::string & strDirPath	��Ҫ������Ŀ¼·��
	// Returns  : bool
	//**********************************************************
	static bool		toCreateDirLoop(const std::string &strDirPath);

	
	//**********************************************************
	// Method   : toDeleteDir
	// Remark   : ɾ��ָ��Ŀ¼
	// DateTime : 2015/08/24
	// Parameter: const std::string & strDirPath	Ŀ¼·��
	// Returns  : bool
	//**********************************************************
	static bool		toDeleteDir(const std::string &strDirPath);

	
	//**********************************************************
	// Method   : toCopyDir
	// Remark   : �����ļ��У��ݹ���������������ļ��У�
	// DateTime : 2015/08/24
	// Parameter: const std::string & strDirSrc		ԴĿ¼·��
	// Parameter: const std::string & strDirDst		Ŀ��Ŀ¼·��
	// Parameter: bool bIsReplace		Ŀ��Ŀ¼����ʱ�Ƿ񸲸�
	// Returns  : bool
	//**********************************************************
	static bool		toCopyDir(const std::string &strDirSrc, const std::string &strDirDst, bool bIsReplace = true);

	
	//**********************************************************
	// Method   : toMoveDir
	// Remark   : �ƶ��ļ��У��ݹ���������������ļ��У�
	// DateTime : 2015/08/24
	// Parameter: const std::string & strDirSrc		ԴĿ¼·��
	// Parameter: const std::string & strDirDst		Ŀ��Ŀ¼·��
	// Parameter: bool bIsReplace		Ŀ��Ŀ¼����ʱ�Ƿ񸲸�
	// Returns  : bool
	//**********************************************************
	static bool		toMoveDir(const std::string &strDirSrc, const std::string &strDirDst, bool bIsReplace = true);

	
	//**********************************************************
	// Method   : getCurrentDir
	// Remark   : ��ȡ��ǰexe����Ŀ¼·��������\��б�ܣ�
	// DateTime : 2015/08/24
	// Returns  : std::string			ȫ·��
	//**********************************************************
	static std::string		getCurrentDir();
	

	//**********************************************************
	// Method   : getParentPath
	// Remark   : ��ȡָ���ļ�·���ĸ�·�������ı�����������֤��Ŀ¼������ڣ�
	// DateTime : 2015/08/24
	// Parameter: const std::string & strFullPath	�ļ�ȫ·��
	// Parameter: const char chPathMark				·���ָ���
	// Returns  : std::string			��·��
	//**********************************************************
	static std::string		getParentPath(const std::string &strFullPath, const char chPathMark = '\\');

	
	//**********************************************************
	// Method   : getFileExtension
	// Remark   : ��ȡָ���ļ�·�����ļ���չ��
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			�ļ�ȫ·��
	// Returns  : std::string			�ļ���չ��
	//**********************************************************
	static std::string		getFileExtension(const std::string &path);

	
	//**********************************************************
	// Method   : getFileName
	// Remark   : ��ȡָ���ļ�·�����ļ���
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			�ļ�ȫ·��
	// Parameter: bool hasExtension					�Ƿ������չ�����֣���.bmp��
	// Returns  : std::string			�ļ���չ��
	//**********************************************************
	static std::string		getFileName(const std::string &path, bool hasExtension = false);
	
	
	//**********************************************************
	// Method   : formatPath
	// Remark   : ��ʽ��·���������в���ȷ�ġ������·���ָ�����׼����
	//			��Window�µ�·������Ϊ׼�򣬽����ڵ��������ļ�ϵͳ��غ���ǰ������·���ĸ�ʽ��
	//			ע�⣺ ���·����ʽ���������·���� ȫ·���ο�makeFullPath
	// DateTime : 2015/08/24
	// Parameter: std::string & path				�ļ�·��
	// Returns  : void
	//**********************************************************
	static void				formatPath(std::string &path);
	
	//**********************************************************
	// Method   : formatPath_C
	// Remark   : ��ʽ��·���������в���ȷ�ġ������·���ָ�����׼����
	//			��Window�µ�·������Ϊ׼�򣬽����ڵ��������ļ�ϵͳ��غ���ǰ������·���ĸ�ʽ��
	//			ע�⣺ ���·����ʽ���������·���� ȫ·���ο�makeFullPath
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			�ļ�·��
	// Returns  : std::string			��ʽ������ļ�·��
	//**********************************************************
	static std::string		formatPath_C(const std::string &path);

	
	//**********************************************************
	// Method   : makeFullPath
	// Remark   : ��׼��ȫ·����ȷ�������·����ȫ·���Ҿ�����ʽ����
	//			��formatPath��������ǻ�����·��תΪȫ·��������û����
	// DateTime : 2015/08/24
	// Parameter: std::string & path				�ļ�·��
	// Returns  : void
	//**********************************************************
	static void				makeFullPath(std::string &path);
	
	//**********************************************************
	// Method   : makeFullPath_C
	// Remark   : ��׼��ȫ·����ȷ�������·����ȫ·���Ҿ�����ʽ����
	//			��formatPath��������ǻ�����·��תΪȫ·��������û����
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			�ļ�·��
	// Returns  : std::string			��ʽ������ļ�ȫ·��
	//**********************************************************
	static std::string		makeFullPath_C(const std::string &path);
	
	
	//**********************************************************
	// Method   : getFileSize
	// Remark   : ��ȡָ���ļ���С
	// DateTime : 2015/08/24
	// Parameter: const std::string & strFilePath	�ļ�ȫ·��
	// Returns  : unsigned long			�ļ���С����λ���ֽڣ�
	//**********************************************************
	static unsigned long	getFileSize(const std::string &strFilePath);

	
	//**********************************************************
	// Method   : readFileToMemory
	// Remark   : ��ָ���ļ����ݶ�ȡ�ڴ��
	// DateTime : 2015/08/24
	// Parameter: const std::string & filePath		�ļ�ȫ·��
	// Parameter: std::string * data				�ڴ��ָ��
	// Returns  : bool
	//**********************************************************
	static bool		readFileToMemory(const std::string &filePath, std::string *data);
};


NAMESPACE_END
