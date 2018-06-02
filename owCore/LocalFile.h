#pragma once

#include "File.h"

class CLocalFile : public CFile
{
public:
	CLocalFile(cstring _fullFileName);
	CLocalFile(cstring _name, cstring _path);
	~CLocalFile();

	// IFile
	bool Open() override;

public:
	static uint64_t GetFileSize(cstring _name);
	static bool IsFileExists(cstring _name);
};