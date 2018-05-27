#pragma once

#include "File.h"

class LocalFile : public File
{
public:
	LocalFile(cstring _fullFileName);
	LocalFile(cstring _name, cstring _path);
	~LocalFile();

	// IFile
	bool Open() override;

public:
	static uint64_t GetFileSize(cstring _name);
	static bool IsFileExists(cstring _name);
};