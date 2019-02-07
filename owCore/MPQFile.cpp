#include "stdafx.h"

// General
#include "MPQFile.h"

// Additional
#include "BaseManager.h"
#include "MPQArchiveManager.h"

CMPQFile::CMPQFile(cstring _fullFileName) :
	CFile(_fullFileName)
{}

CMPQFile::CMPQFile(cstring _name, cstring _path) :
	CFile(_name, _path)
{}

bool CMPQFile::Open()
{
	std::shared_ptr<IMPQArchiveManager> MPQArchiveManager = GetManager<IMPQArchiveManager>();
	std::lock_guard<std::mutex> lock(MPQArchiveManager->Guard());

	SMPQFileLocation location = MPQArchiveManager->GetFileLocation(Path_Name());
	if (!location.exists)
	{
		return false;
	}

	libmpq__off_t size;
	libmpq__file_size_unpacked(location.archive, location.fileNumber, &size);

	// Allocate space and set data
	m_ByteBuffer.Allocate(size);
	libmpq__file_read(location.archive, location.fileNumber, &(m_ByteBuffer.getDataEx())[0], m_ByteBuffer.getSize(), &size);
	assert1(m_ByteBuffer.getSize() == size);
	m_ByteBuffer.SetFilled();

	//Log::Info("File[%s] opened. [mpq]", Path_Name().c_str());

	return true;
}

uint64_t CMPQFile::GetFileSize(cstring _name)
{
	std::shared_ptr<IMPQArchiveManager> MPQArchiveManager = GetManager<IMPQArchiveManager>();
	std::lock_guard<std::mutex> lock(MPQArchiveManager->Guard());

	SMPQFileLocation location = MPQArchiveManager->GetFileLocation(_name);

	if (location.exists)
	{
		libmpq__off_t size;
		libmpq__file_size_unpacked(location.archive, location.fileNumber, &size);
		return size;
	}

	return 0;
}

bool CMPQFile::IsFileExists(cstring _name)
{
	std::shared_ptr<IMPQArchiveManager> MPQArchiveManager = GetManager<IMPQArchiveManager>();
	std::lock_guard<std::mutex> lock(MPQArchiveManager->Guard());

	return MPQArchiveManager->GetFileLocation(_name).exists;
}


