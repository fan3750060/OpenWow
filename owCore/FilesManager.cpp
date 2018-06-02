#include "stdafx.h"

// General
#include "FilesManager.h"

// Additional
#include "BaseManager.h"
#include "LocalFile.h"
#include "MPQFile.h"

CFilesManager::CFilesManager()
{
	AddManager<IFilesManager>(this);
}

CFilesManager::~CFilesManager()
{
	DelManager<IFilesManager>();
}

IFile* CFilesManager::Open(cstring _fileName)
{
	IFile* file = nullptr;

	if (CLocalFile::IsFileExists(_fileName))
	{
		file = new CLocalFile(_fileName);
		file->Open();
	}
	else if (CMPQFile::IsFileExists(_fileName))
	{
		file = new CMPQFile(_fileName);
		file->Open();
	}

	return file;
}