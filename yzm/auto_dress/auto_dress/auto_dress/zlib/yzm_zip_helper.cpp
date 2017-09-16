
/**************************************************************************
Copyright	: Cloudream All Rights Reserved
Author		: chenys@cloudream.net (吖树)
Date		: 2015/08/24

Description	: 提供一个ZIP包的读取类（解压缩），和一个ZIP包的写入类（打包）
**************************************************************************/

#pragma once

#include "yzm_zip_helper.h"
#include "yzm_file_system.h"
#include "yzm_log.h"
#include "yzm_string_helper.h"

#include "unzip.h"
#include "zip.h"



NAMESPACE_BEGIN

	


/************************************************************************
  ***********************       ZipReader        ***********************
************************************************************************/
struct ZipReader_inner_data
{
	HZIP		hz;
	ZIPENTRY	ze;
};

#define		ZIP_PTR		reinterpret_cast<ZipReader_inner_data*>(handle_)


ZipReader::ZipReader(const std::string &filePath)
{
	handle_		= new ZipReader_inner_data;
	baseDir_	= "";
	isOpen_		= false;

	if ( !isEmpty(filePath) )
	{
		open(filePath);
	}
}

ZipReader::~ZipReader()
{
	close();
	delete	ZIP_PTR;
	handle_		= NULL;
}

bool	ZipReader::open(const std::string &filePath)
{
	std::string		fullPath	= FSys::makeFullPath_C(filePath);
	ErrorIf_r( isEmpty(filePath), false );
	ErrorIf_r( !FSys::isFileExist(fullPath), false );

	if ( isOpen() )
	{
		close();
	}

	ZIP_PTR->hz		= OpenZip(fullPath.c_str(), NULL);
	ErrorIf_r( NULL == ZIP_PTR->hz, false );

	isOpen_		= true;
	filePath_	= fullPath;
	setBaseDir( FSys::getParentPath(filePath_) );
	return true;
}

void	ZipReader::close()
{
	if ( isOpen() )
	{
		CloseZipU(ZIP_PTR->hz);
		ZIP_PTR->hz		= NULL;

		isOpen_		= false;
	}
}

bool	ZipReader::isOpen() const
{
	return isOpen_;
}

const std::string&	ZipReader::getFilePath() const
{
	return filePath_;
}

const std::string&	ZipReader::getBaseDir() const
{
	return baseDir_;
}

bool	ZipReader::setBaseDir(const std::string &dirPath)
{
	ErrorIf_r( !isOpen(), false );

	ZRESULT	retCode	= ZR_FAILED;
	retCode	= SetUnzipBaseDir(ZIP_PTR->hz, dirPath.c_str());

	bool	isSucc	= (ZR_OK == retCode);
	if ( isSucc )
	{
		baseDir_	= dirPath;
	}

	return isSucc;
}

bool	ZipReader::containFile(const std::string &name) const
{
	ErrorIf_r( !isOpen(), false );

	ZRESULT	retCode	= ZR_FAILED;
	retCode	= GetZipItem(ZIP_PTR->hz, -1, &ZIP_PTR->ze);
	ErrorIf_r( retCode != ZR_OK, false );

	int	num	= ZIP_PTR->ze.index;
	for (int i = 0; i < num; ++i)
	{
		retCode	= GetZipItem(ZIP_PTR->hz, i, &ZIP_PTR->ze);
		if ( retCode != ZR_OK ) continue;

		if ( StrT::caseContains(ZIP_PTR->ze.name, name) )
		{
			return true;
		}
	}

	return false;
}

ZipReader::FILE_LIST	ZipReader::getFilelist(const std::string &keyword) const
{
	FILE_LIST	lst;
	ErrorIf_r( !isOpen(), lst );

	ZRESULT		retCode	= ZR_FAILED;
	retCode	= GetZipItem(ZIP_PTR->hz, -1, &ZIP_PTR->ze);
	ErrorIf_r( retCode != ZR_OK, lst );

	int	num	= ZIP_PTR->ze.index;
	for (int i = 0; i < num; ++i)
	{
		retCode	= GetZipItem(ZIP_PTR->hz, i, &ZIP_PTR->ze);
		if ( retCode != ZR_OK ) continue;

		if ( StrT::caseContains(ZIP_PTR->ze.name, keyword) )
		{
			lst.push_back(ZIP_PTR->ze.name);
		}
	}

	return lst;
}

std::string	ZipReader::getFirstFile(const std::string &keyword) const
{	
	FILE_LIST	lst	= getFilelist(keyword);
	if ( lst.empty() )
	{
		return EMPTY_STRING;
	}

	return lst.front();
}

int		ZipReader::uncompress()
{
	ErrorIf_r( !isOpen(), false );

	int		succCount	= 0;
	ZRESULT	retCode		= ZR_FAILED;
	retCode	= GetZipItem(ZIP_PTR->hz, -1, &ZIP_PTR->ze);
	ErrorIf_r( retCode != ZR_OK, 0 );

	int	num	= ZIP_PTR->ze.index;
	for (int i = 0; i < num; ++i)
	{
		retCode	= GetZipItem(ZIP_PTR->hz, i, &ZIP_PTR->ze);
		if ( retCode != ZR_OK ) continue;

		retCode	= UnzipItem(ZIP_PTR->hz, i, ZIP_PTR->ze.name);
		if ( retCode != ZR_OK ) continue;

		++succCount;
	}

	return succCount;
}






	


/************************************************************************
  ***********************       ZipWriter        ***********************
************************************************************************/
struct ZipWriter_inner_data
{
	HZIP		hz;
};

#undef		ZIP_PTR
#define		ZIP_PTR		reinterpret_cast<ZipWriter_inner_data*>(handle_)

ZipWriter::ZipWriter(const std::string &filePath)
{
	handle_		= new ZipWriter_inner_data;
	isOpen_		= false;

	if ( !isEmpty(filePath) )
	{
		open(filePath);
	}
}

ZipWriter::~ZipWriter()
{
	close();
	delete	ZIP_PTR;
	handle_		= NULL;
}

bool	ZipWriter::open(const std::string &filePath)
{
	std::string		fullPath	= FSys::makeFullPath_C(filePath);
	ErrorIf_r( isEmpty(fullPath), false );

	if ( isOpen() )
	{
		close();
	}

	ZIP_PTR->hz		= CreateZip(fullPath.c_str(), NULL);
	ErrorIf_r( NULL == ZIP_PTR->hz, false );

	isOpen_		= true;
	filePath_	= fullPath;

	return true;
}

void	ZipWriter::close()
{
	if ( isOpen() )
	{
		CloseZipZ(ZIP_PTR->hz);
		ZIP_PTR->hz		= NULL;

		isOpen_		= false;
	}
}

bool	ZipWriter::isOpen() const
{
	return isOpen_;
}

const std::string&	ZipWriter::getFilePath() const
{
	return filePath_;
}

bool	ZipWriter::addFile(const std::string &filePath, const std::string &nickName)
{
	ErrorIf_r( !isOpen(), false );

	std::string		fmtPath		= FSys::makeFullPath_C(filePath);
	ErrorIf_r( !FSys::isFileExist(fmtPath), false );

	std::string		fileName;
	if ( isEmpty(nickName) )
	{
		fileName	= FSys::getFileName(fmtPath, true);
	} 
	else
	{
		fileName	= FSys::formatPath_C(nickName);
	}
	fileList_[fileName]			= fmtPath;
	return true;
}

int		ZipWriter::compress()
{
	ErrorIf_r( !isOpen(), false );

	int		succCount	= 0;
	ZRESULT	retCode		= ZR_FAILED;

	FILE_LIST::iterator		itr	= fileList_.begin();
	for (; itr != fileList_.end(); ++itr)
	{
		retCode	= ZipAdd(ZIP_PTR->hz, itr->first.c_str(), itr->second.c_str());
		if ( retCode != ZR_OK )
        {
            YZM_ERROR("压缩文件错误:%d,%s,%s,",retCode,itr->first.c_str(),itr->second.c_str());
            continue;
        }
		
		++succCount;
	}

	return succCount;
}


NAMESPACE_END
