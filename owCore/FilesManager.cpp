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
	}
	else if (CMPQFile::IsFileExists(_fileName))
	{
		file = new CMPQFile(_fileName);
	}

	if (file != nullptr)
	{
		Lock();
		file->Open();
		Unlock();
	}

	return file;
}

void CFilesManager::Lock()
{
	m_Lock.lock();
}

void CFilesManager::Unlock()
{
	m_Lock.unlock();
}

void CFilesManager::Guard()
{
	std::lock_guard<std::mutex> lock(m_Lock);
}
