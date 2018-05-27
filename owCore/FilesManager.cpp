#include "stdafx.h"

// General
#include "FilesManager.h"

// Additional
#include "LocalFile.h"
#include "MPQFile.h"

IFile* FilesManager::Open(cstring _fileName)
{
	IFile* file = nullptr;

	if (LocalFile::IsFileExists(_fileName))
	{
		file = new LocalFile(_fileName);
		file->Open();
	}
	else if (MPQFile::IsFileExists(_fileName))
	{
		file = new MPQFile(_fileName);
		file->Open();
	}

	return file;
}