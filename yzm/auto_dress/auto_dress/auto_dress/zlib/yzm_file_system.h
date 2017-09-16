
/**************************************************************************
Copyright	: Cloudream All Rights Reserved
Author		: chenys@cloudream.net (吖树)
Date		: 2015/08/24

Description	: 提供一个文件系统辅助类，提供常用的文件操作，如文件、目录的复制、移动、删除
	还有对文件路径的格式化、转换函数，目前全部调用Win32 API实现，暂未跨平台。
**************************************************************************/

#pragma once

#include "yzm_assist.h"


NAMESPACE_BEGIN


/// TODO: 需要重新过一遍
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
	// Remark   : 判断指定文件是否存在（和isDirExist相区分）
	// DateTime : 2015/08/24
	// Parameter: const std::string & strFilePath	文件路径
	// Returns  : bool
	//**********************************************************
	static bool		isFileExist(const std::string &strFilePath);
	
	
	//**********************************************************
	// Method   : toDeleteFile
	// Remark   : 删除指定文件
	// DateTime : 2015/08/24
	// Parameter: const std::string & strFilePath	文件路径
	// Returns  : bool
	//**********************************************************
	static bool		toDeleteFile(const std::string &strFilePath);

	
	//**********************************************************
	// Method   : toCopyFile
	// Remark   : 复制文件
	// DateTime : 2015/08/24
	// Parameter: const std::string & strFileSrc	源文件路径
	// Parameter: const std::string & strFileDst	目标文件路径
	// Parameter: bool bIsReplace		 目标文件存在时是否覆盖
	// Returns  : bool
	//**********************************************************
	static bool		toCopyFile(const std::string &strFileSrc, const std::string &strFileDst, bool bIsReplace = true);

	
	//**********************************************************
	// Method   : toMoveFile
	// Remark   : 移动文件
	// DateTime : 2015/08/24
	// Parameter: const std::string & strFileSrc	源文件路径
	// Parameter: const std::string & strFileDst	目标文件路径
	// Parameter: bool bIsReplace		目标文件存在时是否覆盖
	// Returns  : bool
	//**********************************************************
	static bool		toMoveFile(const std::string &strFileSrc, const std::string &strFileDst, bool bIsReplace = true);
	
	
	//**********************************************************
	// Method   : isDirExist（和isFileExist相区分）
	// Remark   : 判断指定目录是否存在
	// DateTime : 2015/08/24
	// Parameter: const std::string & strDirPath	目录路径
	// Returns  : bool
	//**********************************************************
	static bool		isDirExist(const std::string &strDirPath);

	
	//**********************************************************
	// Method   : toCreateDir
	// Remark   : 创建目录（必须保证父目录存在）
	// DateTime : 2015/08/24
	// Parameter: const std::string & strDirPath	需要创建的目录路径
	// Returns  : bool
	//**********************************************************
	static bool		toCreateDir(const std::string &strDirPath);

	
	//**********************************************************
	// Method   : toCreateDirLoop
	// Remark   : 创建目录（递归创建）
	//			优点： 无需保证父目录，甚至祖辈目录是否存在
	//			缺点： 增加了更多的校验，比toCreateDir更耗时
	// DateTime : 2015/08/24
	// Parameter: const std::string & strDirPath	需要创建的目录路径
	// Returns  : bool
	//**********************************************************
	static bool		toCreateDirLoop(const std::string &strDirPath);

	
	//**********************************************************
	// Method   : toDeleteDir
	// Remark   : 删除指定目录
	// DateTime : 2015/08/24
	// Parameter: const std::string & strDirPath	目录路径
	// Returns  : bool
	//**********************************************************
	static bool		toDeleteDir(const std::string &strDirPath);

	
	//**********************************************************
	// Method   : toCopyDir
	// Remark   : 复制文件夹（递归操作，包括其子文件夹）
	// DateTime : 2015/08/24
	// Parameter: const std::string & strDirSrc		源目录路径
	// Parameter: const std::string & strDirDst		目标目录路径
	// Parameter: bool bIsReplace		目标目录存在时是否覆盖
	// Returns  : bool
	//**********************************************************
	static bool		toCopyDir(const std::string &strDirSrc, const std::string &strDirDst, bool bIsReplace = true);

	
	//**********************************************************
	// Method   : toMoveDir
	// Remark   : 移动文件夹（递归操作，包括其子文件夹）
	// DateTime : 2015/08/24
	// Parameter: const std::string & strDirSrc		源目录路径
	// Parameter: const std::string & strDirDst		目标目录路径
	// Parameter: bool bIsReplace		目标目录存在时是否覆盖
	// Returns  : bool
	//**********************************************************
	static bool		toMoveDir(const std::string &strDirSrc, const std::string &strDirDst, bool bIsReplace = true);

	
	//**********************************************************
	// Method   : getCurrentDir
	// Remark   : 获取当前exe所在目录路径（包括\反斜杠）
	// DateTime : 2015/08/24
	// Returns  : std::string			全路径
	//**********************************************************
	static std::string		getCurrentDir();
	

	//**********************************************************
	// Method   : getParentPath
	// Remark   : 获取指定文件路径的父路径（纯文本解析，不保证该目录物理存在）
	// DateTime : 2015/08/24
	// Parameter: const std::string & strFullPath	文件全路径
	// Parameter: const char chPathMark				路径分隔符
	// Returns  : std::string			父路径
	//**********************************************************
	static std::string		getParentPath(const std::string &strFullPath, const char chPathMark = '\\');

	
	//**********************************************************
	// Method   : getFileExtension
	// Remark   : 获取指定文件路径的文件扩展名
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			文件全路径
	// Returns  : std::string			文件扩展名
	//**********************************************************
	static std::string		getFileExtension(const std::string &path);

	
	//**********************************************************
	// Method   : getFileName
	// Remark   : 获取指定文件路径的文件名
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			文件全路径
	// Parameter: bool hasExtension					是否包含扩展名部分（如.bmp）
	// Returns  : std::string			文件扩展名
	//**********************************************************
	static std::string		getFileName(const std::string &path, bool hasExtension = false);
	
	
	//**********************************************************
	// Method   : formatPath
	// Remark   : 格式化路径（把所有不正确的、多余的路径分隔符标准化）
	//			以Window下的路径规则为准则，建议在调用所有文件系统相关函数前都进行路径的格式化
	//			注意： 相对路径格式化后还是相对路径， 全路径参考makeFullPath
	// DateTime : 2015/08/24
	// Parameter: std::string & path				文件路径
	// Returns  : void
	//**********************************************************
	static void				formatPath(std::string &path);
	
	//**********************************************************
	// Method   : formatPath_C
	// Remark   : 格式化路径（把所有不正确的、多余的路径分隔符标准化）
	//			以Window下的路径规则为准则，建议在调用所有文件系统相关函数前都进行路径的格式化
	//			注意： 相对路径格式化后还是相对路径， 全路径参考makeFullPath
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			文件路径
	// Returns  : std::string			格式化后的文件路径
	//**********************************************************
	static std::string		formatPath_C(const std::string &path);

	
	//**********************************************************
	// Method   : makeFullPath
	// Remark   : 标准化全路径（确保输出的路径是全路径且经过格式化）
	//			和formatPath的区别就是会把相对路径转为全路径，其他没区别
	// DateTime : 2015/08/24
	// Parameter: std::string & path				文件路径
	// Returns  : void
	//**********************************************************
	static void				makeFullPath(std::string &path);
	
	//**********************************************************
	// Method   : makeFullPath_C
	// Remark   : 标准化全路径（确保输出的路径是全路径且经过格式化）
	//			和formatPath的区别就是会把相对路径转为全路径，其他没区别
	// DateTime : 2015/08/24
	// Parameter: const std::string & path			文件路径
	// Returns  : std::string			格式化后的文件全路径
	//**********************************************************
	static std::string		makeFullPath_C(const std::string &path);
	
	
	//**********************************************************
	// Method   : getFileSize
	// Remark   : 获取指定文件大小
	// DateTime : 2015/08/24
	// Parameter: const std::string & strFilePath	文件全路径
	// Returns  : unsigned long			文件大小（单位：字节）
	//**********************************************************
	static unsigned long	getFileSize(const std::string &strFilePath);

	
	//**********************************************************
	// Method   : readFileToMemory
	// Remark   : 把指定文件内容读取内存块
	// DateTime : 2015/08/24
	// Parameter: const std::string & filePath		文件全路径
	// Parameter: std::string * data				内存块指针
	// Returns  : bool
	//**********************************************************
	static bool		readFileToMemory(const std::string &filePath, std::string *data);
};


NAMESPACE_END
