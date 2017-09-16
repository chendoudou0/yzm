
/**************************************************************************
Copyright	: Cloudream All Rights Reserved
Author		: chenys@cloudream.net (吖树)
Date		: 2015/08/24

Description	: 提供一个ZIP包的读取类（解压缩），和一个ZIP包的写入类（打包）
**************************************************************************/

#pragma once

#include "yzm_assist.h"


NAMESPACE_BEGIN
	


/************************************************************************
  ***********************       ZipReader        ***********************
************************************************************************/
class  ZipReader
{
public:
	typedef		std::vector<std::string>	FILE_LIST;

	ZipReader(const std::string &filePath = "");
	~ZipReader();

	
	//**********************************************************
	// Method   : open
	// Remark   : 打开ZIP包
	// DateTime : 2015/08/24
	// Parameter: const std::string & filePath		文件路径
	// Returns  : bool
	//**********************************************************
	bool	open(const std::string &filePath);
	
	//**********************************************************
	// Method   : close
	// Remark   : 关闭ZIP包
	// DateTime : 2015/08/24
	// Returns  : void
	//**********************************************************
	void	close();
	
	//**********************************************************
	// Method   : isOpen
	// Remark   : 判定ZIP包是否打开
	// DateTime : 2015/08/24
	// Returns  : bool
	//**********************************************************
	bool	isOpen() const;

	
	//**********************************************************
	// Method   : getFilePath
	// Remark   : 获取ZIP包文件全路径
	// DateTime : 2015/08/24
	// Returns  : const std::string&	ZIP包所在路径
	//**********************************************************
	const std::string&	getFilePath() const;
	
	//**********************************************************
	// Method   : getBaseDir
	// Remark   : 获取基目录（ZIP包内容会全部解压到该目录下）
	// DateTime : 2015/08/24
	// Returns  : const std::string&	基目录路径
	//**********************************************************
	const std::string&	getBaseDir() const;

	
	//**********************************************************
	// Method   : setBaseDir
	// Remark   : 设置基目录（ZIP包内容会全部解压到该目录下）
	// DateTime : 2015/08/24
	// Parameter: const std::string & dirPath		目录路径
	// Returns  : bool
	//**********************************************************
	bool	setBaseDir(const std::string &dirPath);
	
	//**********************************************************
	// Method   : containFile
	// Remark   : 判断ZIP包是否有包含指定字符串的文件（文件路径）
	// DateTime : 2015/08/24
	// Parameter: const std::string & name			字符串
	// Returns  : bool
	//**********************************************************
	bool	containFile(const std::string &name) const;
	FILE_LIST	getFilelist(const std::string &keyword = "") const;
	std::string	getFirstFile(const std::string &keyword = "") const;
	
	//**********************************************************
	// Method   : uncompress
	// Remark   : 执行解压缩操作（自动创建所需子目录）
	// DateTime : 2015/08/24
	// Returns  : int
	//**********************************************************
	int		uncompress();

private:
	std::string		filePath_;	// ZIP包所在文件路径
	std::string		baseDir_;	// 解压时的基路径
	void			*handle_;	// 内部定义文件句柄
	bool			isOpen_;	// ZIP包是否打开
};




/************************************************************************
  ***********************       ZipWriter        ***********************
************************************************************************/
class YZM_API ZipWriter
{
public:
	typedef		std::map<std::string, std::string>	FILE_LIST;

	ZipWriter(const std::string &filePath = "");
	~ZipWriter();

	
	//**********************************************************
	// Method   : open
	// Remark   : 创建一个ZIP包（会覆盖旧的ZIP包）
	// DateTime : 2015/08/24
	// Parameter: const std::string & filePath		文件路径
	// Returns  : bool
	//**********************************************************
	bool	open(const std::string &filePath);
	
	//**********************************************************
	// Method   : close
	// Remark   : 关闭ZIP包
	// DateTime : 2015/08/24
	// Returns  : void
	//**********************************************************
	void	close();
	
	//**********************************************************
	// Method   : isOpen
	// Remark   : ZIP包是否打开
	// DateTime : 2015/08/24
	// Returns  : bool
	//**********************************************************
	bool	isOpen() const;

	
	//**********************************************************
	// Method   : getFilePath
	// Remark   : 获取ZIP包的文件全路径
	// DateTime : 2015/08/24
	// Returns  : const std::string&	文件全路径
	//**********************************************************
	const std::string&	getFilePath() const;

	
	//**********************************************************
	// Method   : addFile
	// Remark   : 添加文件到ZIP包（只做一个文件列表维护，还未进行真正的ZIP打包）
	// DateTime : 2015/08/24
	// Parameter: const std::string & filePath		需要添加的文件全路径
	// Parameter: const std::string & nickName		文件在ZIP包中的名称（默认是文件名）
	// Returns  : bool
	//**********************************************************
	bool	addFile(const std::string &filePath, const std::string &nickName = "");
	
	//**********************************************************
	// Method   : compress
	// Remark   : 执行压缩打包操作（前面的addFile仅仅是添加到文件列表）
	// DateTime : 2015/08/24
	// Returns  : int					成功打包的文件个数
	//**********************************************************
	int		compress();

private:
	FILE_LIST		fileList_;	// 等待打包的文件列表
	std::string		filePath_;	// ZIP包所在文件路径
	void			*handle_;	// 内部定义文件句柄
	bool			isOpen_;	// ZIP包是否打开
};


NAMESPACE_END
