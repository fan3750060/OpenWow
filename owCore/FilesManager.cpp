#include "stdafx.h"

// General
#include "FilesManager.h"

// Additional
#include "BaseManager.h"
#include "LocalFile.h"
#include "MPQFile.h"

CFilesManager::CFilesManager()
{
}

CFilesManager::~CFilesManager()
{
	DelManager<IFilesManager>();
}

std::shared_ptr<IFile> CFilesManager::Open(cstring _fileName)
{
	std::shared_ptr<IFile> file = nullptr;

	if (CLocalFile::IsFileExists(_fileName))
	{
		file = std::make_shared<CLocalFile>(_fileName);
	}
	else if (CMPQFile::IsFileExists(_fileName))
	{
		file = std::make_shared<CMPQFile>(_fileName);
	}

	if (file != nullptr)
	{
		file->Open();
	}

	return file;
}