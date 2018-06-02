#pragma once

#include "File.h"

class CMPQFile : public CFile
{
public:
	CMPQFile(cstring _fullFileName);
	CMPQFile(cstring _name, cstring _path);
	~CMPQFile();

	// IFile
	bool Open() override;

public:
	static uint64_t GetFileSize(cstring _name);
	static bool IsFileExists(cstring _name);
};