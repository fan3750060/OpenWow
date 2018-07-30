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

std::shared_ptr<IFile> CFilesManager::Open(cstring _fileName)
{
	std::shared_ptr<IFile> file = nullptr;

	if (CLocalFile::IsFileExists(_fileName))
	{
		file = make_shared<CLocalFile>(_fileName);
	}
	else if (CMPQFile::IsFileExists(_fileName))
	{
		file = make_shared<CMPQFile>(_fileName);
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
