
/**************************************************************************
Copyright	: Cloudream All Rights Reserved
Author		: chenys@cloudream.net (߹��)
Date		: 2015/08/24

Description	: �ṩһ��ZIP���Ķ�ȡ�ࣨ��ѹ��������һ��ZIP����д���ࣨ�����
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
	// Remark   : ��ZIP��
	// DateTime : 2015/08/24
	// Parameter: const std::string & filePath		�ļ�·��
	// Returns  : bool
	//**********************************************************
	bool	open(const std::string &filePath);
	
	//**********************************************************
	// Method   : close
	// Remark   : �ر�ZIP��
	// DateTime : 2015/08/24
	// Returns  : void
	//**********************************************************
	void	close();
	
	//**********************************************************
	// Method   : isOpen
	// Remark   : �ж�ZIP���Ƿ��
	// DateTime : 2015/08/24
	// Returns  : bool
	//**********************************************************
	bool	isOpen() const;

	
	//**********************************************************
	// Method   : getFilePath
	// Remark   : ��ȡZIP���ļ�ȫ·��
	// DateTime : 2015/08/24
	// Returns  : const std::string&	ZIP������·��
	//**********************************************************
	const std::string&	getFilePath() const;
	
	//**********************************************************
	// Method   : getBaseDir
	// Remark   : ��ȡ��Ŀ¼��ZIP�����ݻ�ȫ����ѹ����Ŀ¼�£�
	// DateTime : 2015/08/24
	// Returns  : const std::string&	��Ŀ¼·��
	//**********************************************************
	const std::string&	getBaseDir() const;

	
	//**********************************************************
	// Method   : setBaseDir
	// Remark   : ���û�Ŀ¼��ZIP�����ݻ�ȫ����ѹ����Ŀ¼�£�
	// DateTime : 2015/08/24
	// Parameter: const std::string & dirPath		Ŀ¼·��
	// Returns  : bool
	//**********************************************************
	bool	setBaseDir(const std::string &dirPath);
	
	//**********************************************************
	// Method   : containFile
	// Remark   : �ж�ZIP���Ƿ��а���ָ���ַ������ļ����ļ�·����
	// DateTime : 2015/08/24
	// Parameter: const std::string & name			�ַ���
	// Returns  : bool
	//**********************************************************
	bool	containFile(const std::string &name) const;
	FILE_LIST	getFilelist(const std::string &keyword = "") const;
	std::string	getFirstFile(const std::string &keyword = "") const;
	
	//**********************************************************
	// Method   : uncompress
	// Remark   : ִ�н�ѹ���������Զ�����������Ŀ¼��
	// DateTime : 2015/08/24
	// Returns  : int
	//**********************************************************
	int		uncompress();

private:
	std::string		filePath_;	// ZIP�������ļ�·��
	std::string		baseDir_;	// ��ѹʱ�Ļ�·��
	void			*handle_;	// �ڲ������ļ����
	bool			isOpen_;	// ZIP���Ƿ��
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
	// Remark   : ����һ��ZIP�����Ḳ�Ǿɵ�ZIP����
	// DateTime : 2015/08/24
	// Parameter: const std::string & filePath		�ļ�·��
	// Returns  : bool
	//**********************************************************
	bool	open(const std::string &filePath);
	
	//**********************************************************
	// Method   : close
	// Remark   : �ر�ZIP��
	// DateTime : 2015/08/24
	// Returns  : void
	//**********************************************************
	void	close();
	
	//**********************************************************
	// Method   : isOpen
	// Remark   : ZIP���Ƿ��
	// DateTime : 2015/08/24
	// Returns  : bool
	//**********************************************************
	bool	isOpen() const;

	
	//**********************************************************
	// Method   : getFilePath
	// Remark   : ��ȡZIP�����ļ�ȫ·��
	// DateTime : 2015/08/24
	// Returns  : const std::string&	�ļ�ȫ·��
	//**********************************************************
	const std::string&	getFilePath() const;

	
	//**********************************************************
	// Method   : addFile
	// Remark   : ����ļ���ZIP����ֻ��һ���ļ��б�ά������δ����������ZIP�����
	// DateTime : 2015/08/24
	// Parameter: const std::string & filePath		��Ҫ��ӵ��ļ�ȫ·��
	// Parameter: const std::string & nickName		�ļ���ZIP���е����ƣ�Ĭ�����ļ�����
	// Returns  : bool
	//**********************************************************
	bool	addFile(const std::string &filePath, const std::string &nickName = "");
	
	//**********************************************************
	// Method   : compress
	// Remark   : ִ��ѹ�����������ǰ���addFile��������ӵ��ļ��б�
	// DateTime : 2015/08/24
	// Returns  : int					�ɹ�������ļ�����
	//**********************************************************
	int		compress();

private:
	FILE_LIST		fileList_;	// �ȴ�������ļ��б�
	std::string		filePath_;	// ZIP�������ļ�·��
	void			*handle_;	// �ڲ������ļ����
	bool			isOpen_;	// ZIP���Ƿ��
};


NAMESPACE_END
