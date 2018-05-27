#include "stdafx.h"

// General
#include "MPQFile.h"

// Additional
#include "MPQArchiveStorage.h"

MPQFile::MPQFile(cstring _fullFileName) :
	File(_fullFileName)
{}

MPQFile::MPQFile(cstring _name, cstring _path) :
	File(_name, _path)
{}

MPQFile::~MPQFile()
{}

bool MPQFile::Open()
{
	MPQFileLocation location = MPQArchiveStorage::GetFileLocation(Path_Name());

	if (location.exists)
	{
		libmpq__off_t size;
		libmpq__file_size_unpacked(location.archive, location.fileNumber, &size);

		// Allocate space and set data
		m_ByteBuffer.Allocate(size);
		libmpq__file_read(location.archive, location.fileNumber, m_ByteBuffer.GetAccessToData(), m_ByteBuffer.GetSize(), &size);
		assert1(m_ByteBuffer.GetSize() == size);
		m_ByteBuffer.SetFilled();

		return true;
	}

	return false;
}

uint64_t MPQFile::GetFileSize(cstring _name)
{
	MPQFileLocation location = MPQArchiveStorage::GetFileLocation(_name);

	if (location.exists)
	{
		libmpq__off_t size;
		libmpq__file_size_unpacked(location.archive, location.fileNumber, &size);
		return size;
	}
	
	return 0;
}

bool MPQFile::IsFileExists(cstring _name)
{
	return MPQArchiveStorage::GetFileLocation(_name).exists;
}


