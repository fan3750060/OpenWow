#include "stdafx.h"

// General
#include "LocalFile.h"

// Additional
const char* m_LocalFilesFolder = "D:\\_programming\\OpenWow\\_gamedata\\";

CLocalFile::CLocalFile(cstring _fullFileName)	: 
	CFile(_fullFileName)
{}

CLocalFile::CLocalFile(cstring _name, cstring _path) :
	CFile(_name, _path)
{}

bool CLocalFile::Open()
{
	// Open stream
	std::ifstream stream;
	stream.open(std::string(m_LocalFilesFolder + Path_Name()), std::ios::binary);

	// Check stream
	if (!stream.is_open())
	{
		stream.clear();
		stream.close();

		stream.open(std::string(Path_Name()), std::ios::binary);

		if (!stream.is_open())
		{
			//Log::Error("File[%s]: Can not open file!", Path_Name().c_str());
			return false;
		}
	}

	// Filesize
	stream.seekg(0, stream.end);
	uint32_t fileSize = uint32_t(stream.tellg());
	stream.seekg(0, stream.beg);

	// Check filesize
	if (fileSize == 0)
	{
		char buff[256];
		sprintf_s(buff, "File[%s]: Is empty!", Path_Name().c_str());
		fail2(buff);

		return false;
	}

	// Read data
	m_ByteBuffer.Allocate(fileSize + 1);
	stream.read((char*)&m_ByteBuffer.getDataEx()[0], fileSize);
	m_ByteBuffer.SetFilled();
	m_ByteBuffer.getDataEx()[fileSize] = '\0';

	std::streamsize readedBytes = stream.gcount();
	if (readedBytes < fileSize)
	{
		//memset(&data[0] + readedBytes, 0, fileSize - static_cast<uint32_t>(readedBytes));

		char buff[256];
		sprintf_s(buff, "File[%s]: Stream reading error. Readed [%d], filesize [%d]", Path_Name().c_str(), static_cast<int64>(readedBytes), fileSize);
		fail2(buff);
	}

	// Close stream
	stream.close();
	stream.clear();

	return true;
}

uint64_t CLocalFile::GetFileSize(cstring _name)
{
	// Open stream
	std::ifstream stream;
	stream.open(std::string(m_LocalFilesFolder + _name), std::ios::binary);

	// Check stream
	if (!stream.is_open())
	{
		return 0;
	}

	// Filesize
	stream.seekg(0, stream.end);
	uint64_t fileSize = static_cast<uint64_t>(stream.tellg());
	stream.seekg(0, stream.beg);

	stream.clear();
	stream.close();

	return fileSize;
}

bool CLocalFile::IsFileExists(cstring _name)
{
	// Open stream
	std::ifstream stream;
	stream.open(std::string(m_LocalFilesFolder + _name), std::ios::binary);

	// Check stream
	if (!stream.is_open())
	{
		stream.clear();
		stream.close();

		stream.open(_name, std::ios::binary);

		if (!stream.is_open())
		{
			return false;
		}
	}

	stream.clear();
	stream.close();

	return true;
}
