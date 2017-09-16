
/**************************************************************************
Copyright	: Cloudream All Rights Reserved
Author		: chenys@cloudream.net (߹��)
Date		: 2015/08/24

Description	: �ṩһ���ļ�ϵͳ�����࣬�ṩ���õ��ļ����������ļ���Ŀ¼�ĸ��ơ��ƶ���ɾ��
	���ж��ļ�·���ĸ�ʽ����ת��������Ŀǰȫ������Win32 APIʵ�֣���δ��ƽ̨��
**************************************************************************/

#include "yzm_file_system.h"
#include "yzm_log.h"
#include "yzm_string_helper.h"

#include <windows.h>


NAMESPACE_BEGIN

	





/************************************************************************
  ***********************         FSys          ***********************
************************************************************************/
std::string	FSys::getParentPath(const std::string &strFullPath, const char chPathMark)
{
	WarningIf_r ( isEmpty(strFullPath), EMPTY_STRING );

	std::string	strPath	= StrT::trimRight_C(strFullPath, std::string(1, chPathMark));
	size_t		nPos	= strPath.rfind(chPathMark);
	if (nPos == std::string::npos ||
		nPos == strPath.size()-1)
	{
		//���û��·�����ţ�������ӣ���֤���ص�·���ԡ�\����β
		return	EMPTY_STRING;
	}

	return	strPath.substr(0, nPos+1);
}

bool FSys::isFileExist(const std::string &strFilePath)
{
	ErrorIf_r ( isEmpty(strFilePath), false );

	DWORD	dwAttr	= ::GetFileAttributesA( makeFullPath_C(strFilePath).c_str() );
	if (dwAttr != INVALID_FILE_ATTRIBUTES && !(dwAttr & FILE_ATTRIBUTE_DIRECTORY) )
	{
		return	true;
	}

	return	false;
}

bool FSys::toDeleteFile(const std::string &strFilePath)
{
	ErrorIf_r ( isEmpty(strFilePath), false );

	std::string		strFullPath	= makeFullPath_C(strFilePath);
	if ( !isFileExist(strFullPath) )
	{
		return	false;
	}

	if ( ::DeleteFileA(strFullPath.c_str()) )
	{
		return	true;
	}

	return	false;
}

bool FSys::toCopyFile(const std::string &strFileSrc, const std::string &strFileDst, bool bIsReplace)
{
	ErrorIf_r ( isEmpty(strFileSrc) || isEmpty(strFileDst), false );

	std::string		strFullSrcPath	= makeFullPath_C(strFileSrc);
	std::string		strFullDstPath	= makeFullPath_C(strFileDst);
	if (strFullSrcPath == strFullDstPath)
	{
		return	true;
	}

	//ԭ�ļ������ڻ���Ŀ��·���Ѿ�����ͬ��Ŀ¼����ʧ��
	if ( !isFileExist(strFullSrcPath) || isDirExist(strFullDstPath) )
	{
		return	false;
	}

	//���Ǵ���
	if (isFileExist(strFullDstPath))
	{
		if (bIsReplace)
		{
			toDeleteFile(strFullDstPath);
		}
		else
		{
			return	false;
		}
	}
	else
	{
		//Ŀ�겻����ʱ��������Ҫ������Ŀ¼
		std::string		strParentDir	= getParentPath(strFullDstPath);
		if (!isDirExist(strParentDir) && !toCreateDirLoop(strParentDir))
		{
			return	false;
		}
	}

	if ( ::CopyFileA(strFullSrcPath.c_str(), strFullDstPath.c_str(), false) )
	{
		return	true;
	}

	return	false;
}

bool FSys::toMoveFile(const std::string &strFileSrc, const std::string &strFileDst, bool bIsReplace)
{
	ErrorIf_r( isEmpty(strFileSrc) || isEmpty(strFileDst), false);

	std::string		strFullSrcPath	= makeFullPath_C(strFileSrc);
	std::string		strFullDstPath	= makeFullPath_C(strFileDst);
	if (strFullSrcPath == strFullDstPath)
	{
		return	true;
	}

	//ԭ�ļ������ڻ���Ŀ��·���Ѿ�����ͬ��Ŀ¼����ʧ��
	if ( !isFileExist(strFullSrcPath) || isDirExist(strFullDstPath) )
	{
		return	false;
	}

	//���Ǵ���
	if (isFileExist(strFullDstPath))
	{
		if (bIsReplace)
		{
			toDeleteFile(strFullDstPath);
		}
		else
		{
			return	false;
		}
	}
	else
	{
		//Ŀ�겻����ʱ��������Ҫ������Ŀ¼
		std::string		strParentDir	= getParentPath(strFullDstPath);
		if ( !isDirExist(strParentDir) && !toCreateDirLoop(strParentDir) )
		{
			return	false;
		}
	}

	if ( ::MoveFileA(strFullSrcPath.c_str(), strFullDstPath.c_str()) )
	{
		return	true;
	}

	return	false;
}


bool FSys::isDirExist(const std::string &strDirPath)
{
	ErrorIf_r( isEmpty(strDirPath), false);

	DWORD	dwAttr	= ::GetFileAttributesA( makeFullPath_C(strDirPath).c_str() );
	if (dwAttr != INVALID_FILE_ATTRIBUTES && (dwAttr & FILE_ATTRIBUTE_DIRECTORY) )
	{
		return	true;
	}

	return	false;
}

bool FSys::toCreateDir(const std::string &strDirPath)
{
	ErrorIf_r( isEmpty(strDirPath), false);

	std::string		strFullPath	= makeFullPath_C(strDirPath);
	if (isDirExist(strFullPath.c_str()))
	{
		return	true;
	}

	//���·������ͬ���ļ���ʧ��
	if (isFileExist(strFullPath.c_str()))
	{
		return	false;
	}

	if ( ::CreateDirectoryA(strFullPath.c_str(), NULL) )
	{
		return	true;
	}

	return	false;
}


/// TODO: Bug ��������Data\\�����Ļ���ѭ���ݹ鵼�±���
bool FSys::toCreateDirLoop(const std::string &strDirPath)
{
	ErrorIf_r( isEmpty(strDirPath), false);
	
	std::string		strFullPath	= makeFullPath_C(strDirPath);
	if (isDirExist(strFullPath.c_str()))
	{
		return	true;
	}

	std::string		strParentDir	= getParentPath(StrT::trimRight_C(strFullPath, "\\"));
	//create parent dir First
	if ( !isDirExist(strParentDir) && !toCreateDirLoop(strParentDir) )
	{
		return	false;	
	}

	return	toCreateDir(strFullPath);
}

bool FSys::toDeleteDir(const std::string &strDirPath)
{
	ErrorIf_r ( isEmpty(strDirPath), false );
		
	std::string		strFullPath	= makeFullPath_C(strDirPath);
	if ( !isDirExist(strFullPath) )
	{
		return	false;
	}

	std::string			strToDel	= strFullPath;
	StrT::trimRight(strToDel, "\\");
	strToDel.resize(strToDel.size()+1, 0);

	//################# �뿴�ļ���ʼ����ע��SHFileOperationʹ����� #######################//
	SHFILEOPSTRUCTA		stFileOpr	= {0};
	stFileOpr.fFlags	= FOF_ALLOWUNDO | FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION;
	stFileOpr.pFrom		= strToDel.c_str();
	stFileOpr.pTo		= NULL;
	stFileOpr.wFunc		= FO_DELETE;

	return	::SHFileOperationA(&stFileOpr) == 0;
}

bool FSys::toCopyDir(const std::string &strDirSrc, const std::string &strDirDst, bool bIsReplace)
{
	ErrorIf_r ( isEmpty(strDirSrc) || isEmpty(strDirDst), false );

	std::string		strFullSrcPath	= makeFullPath_C(strDirSrc);
	std::string		strFullDstPath	= makeFullPath_C(strDirDst);
	if (strFullSrcPath == strFullDstPath)
	{
		return	true;
	}

	//Դ�ļ��в����ڣ���Ŀ��·������ͬ���ļ�
	if ( !isDirExist(strFullSrcPath) || isFileExist(strFullDstPath) )
	{
		return	false;
	}

	//���Ǵ���
	if (isDirExist(strFullDstPath))
	{
		if (bIsReplace)
		{
			toDeleteDir(strFullDstPath);
		} 
		else
		{
			return	false;
		}
	} 
	else
	{
		//Ŀ�겻����ʱ��������Ҫ������Ŀ¼
		std::string		strParentDir	= getParentPath(strFullDstPath);
		if (!isDirExist(strParentDir) && !toCreateDirLoop(strParentDir))
		{
			return	false;
		}
	}

	std::string			strSrcToCopy	= strFullSrcPath;
	std::string			strDstToCopy	= strFullDstPath;
	StrT::trimRight(strSrcToCopy, "\\");
	StrT::trimRight(strDstToCopy, "\\");
	strSrcToCopy.resize(strSrcToCopy.size()+1, 0);
	strDstToCopy.resize(strDstToCopy.size()+1, 0);

	//################# �뿴�ļ���ʼ����ע��SHFileOperationʹ����� #######################//
	SHFILEOPSTRUCTA		stFileOpr	= {0};
	stFileOpr.fFlags	= FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
	stFileOpr.pFrom		= strSrcToCopy.c_str();
	stFileOpr.pTo		= strDstToCopy.c_str();
	stFileOpr.wFunc		= FO_COPY;

	return	::SHFileOperationA(&stFileOpr) == 0;
}

bool FSys::toMoveDir(const std::string &strDirSrc, const std::string &strDirDst, bool bIsReplace)
{
	ErrorIf_r( isEmpty(strDirSrc) || isEmpty(strDirDst), false);

	std::string		strFullSrcPath	= makeFullPath_C(strDirSrc);
	std::string		strFullDstPath	= makeFullPath_C(strDirDst);
	if (strFullSrcPath == strFullDstPath)
	{
		return	true;
	}

	//Դ�ļ��в����ڣ���Ŀ��·������ͬ���ļ�
	if (!isDirExist(strFullSrcPath) || isFileExist(strFullDstPath))
	{
		return	false;
	}

	//���Ǵ���
	if (isDirExist(strFullDstPath))
	{
		if (bIsReplace)
		{
			toDeleteDir(strFullDstPath);
		} 
		else
		{
			return	false;
		}
	} 
	else
	{
		//Ŀ�겻����ʱ��������Ҫ������Ŀ¼
		std::string		strParentDir	= getParentPath(strFullDstPath);
		if (!isDirExist(strParentDir) && !toCreateDirLoop(strParentDir))
		{
			return	false;
		}
	}

	std::string			strSrcToMove	= strFullSrcPath;
	std::string			strDstToMove	= strFullDstPath;
	StrT::trimRight(strSrcToMove, "\\");
	StrT::trimRight(strDstToMove, "\\");
	strSrcToMove.resize(strSrcToMove.size()+1, 0);
	strDstToMove.resize(strDstToMove.size()+1, 0);

	//################# �뿴�ļ���ʼ����ע��SHFileOperationʹ����� #######################//
	SHFILEOPSTRUCTA		stFileOpr	= {0};
	stFileOpr.fFlags	= FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
	stFileOpr.pFrom		= strSrcToMove.c_str();
	stFileOpr.pTo		= strDstToMove.c_str();

	if (getParentPath(strFullSrcPath) == getParentPath(strFullDstPath))
	{
		//ͬĿ¼��Rename
		stFileOpr.wFunc		= FO_RENAME;
	} 
	else
	{
		//��ͬĿ¼��Move
		stFileOpr.wFunc		= FO_MOVE;
	}

	return	::SHFileOperationA(&stFileOpr) == 0;
}

std::string FSys::getCurrentDir()
{
	char	szPath[MAX_PATH];
	::GetModuleFileNameA(NULL, szPath, MAX_PATH);
	return	getParentPath(szPath);
}

DWORD FSys::getFileSize(const std::string &strFilePath)
{
	ErrorIf_r( isEmpty(strFilePath), 0 );

	std::string		strFullPath	= makeFullPath_C(strFilePath);
	if ( !isFileExist(strFullPath) )
	{
		return	0;
	}

	WIN32_FIND_DATAA	dataFile;
	HANDLE				hFile;

	hFile		= ::FindFirstFileA(strFullPath.c_str(), &dataFile);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		return	dataFile.nFileSizeLow;
	}
	::FindClose(hFile);

	return	0;
}

void	FSys::formatPath(std::string &path)
{
	StrT::replaceAll(path, "/", "\\");
	bool	isNetPath	= StrT::startWith(path, "\\\\");

	size_t		maxReplaceCount		= 50;
	size_t		lastReplaceCount	= 1;
	while ( lastReplaceCount > 0 && --maxReplaceCount > 0 )
	{
		lastReplaceCount	= StrT::replaceAll(path, "\\\\", "\\");
	}
	
	if ( isNetPath )
	{
		path.insert(path.begin(), '\\');
	}
}

std::string		FSys::formatPath_C(const std::string &path)
{
	std::string	fmtPath = path;
	formatPath(fmtPath);
	return fmtPath;
}

void			FSys::makeFullPath(std::string &path)
{
	ErrorIf( isEmpty(path) );
	
	bool	isRelativePath	= (path.size() >= 2 && path[1] != ':') || 
								(path.size() < 2);
	if ( isRelativePath )
	{
		path.insert(0, getCurrentDir());
	}
	
	formatPath(path);

	StrT::replaceAll(path, "\\.\\", "\\");
	//  ..\ ��ʱ������

	formatPath(path);
}

std::string		FSys::makeFullPath_C(const std::string &path)
{
	std::string	fmtPath	= path;
	makeFullPath(fmtPath);
	return fmtPath;
}

std::string		FSys::getFileExtension(const std::string &path)
{
	std::string	fmtPath	= formatPath_C(path);
	std::string::size_type		pos1	= fmtPath.rfind('\\');
	std::string::size_type		pos2	= fmtPath.rfind('.');

	if ( pos1 == std::string::npos ||
		pos2 == std::string::npos ||
		pos1 >= pos2 )
	{
		return EMPTY_STRING;
	}

	return fmtPath.substr(pos2);
}

std::string		FSys::getFileName(const std::string &path, bool hasExtension)
{
	std::string	fmtPath	= formatPath_C(path);
	std::string::size_type		pos1	= fmtPath.rfind('\\');
	std::string::size_type		pos2	= fmtPath.rfind('.');
	
	if ( pos1 == std::string::npos ||
		pos2 == std::string::npos ||
		pos1 >= pos2 )
	{
		return fmtPath;
	}

	if (hasExtension)
	{
		return fmtPath.substr(pos1+1);
	} 
	else
	{
		return fmtPath.substr(pos1+1, pos2-pos1-1);
	}
}

bool	FSys::readFileToMemory(const std::string &filePath, std::string *data)
{
	AssertIf(data != NULL);
	ErrorIf_r(NULL == data, false);

	std::string		fullPath	= makeFullPath_C(filePath);
	if ( !isFileExist(fullPath) )
	{
		return	false;
	}

	FILE	*fp	= fopen(fullPath.c_str(), "rb");
	if ( NULL == fp )
	{
		return false;
	}

	data->assign("");

	char		buffer[10240];
	size_t		byteRead = 0;
	while ( !feof(fp) )
	{
		byteRead	= fread(buffer, 1, sizeof(buffer), fp);
		data->append(buffer, byteRead);
	}

	fclose(fp);
	return true;
}


NAMESPACE_END
