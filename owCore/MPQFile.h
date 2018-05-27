#pragma once

#include "File.h"

class MPQFile : public File
{
public:
	MPQFile(cstring _fullFileName);
	MPQFile(cstring _name, cstring _path);
	~MPQFile();

	// IFile
	bool Open() override;

public:
	static uint64_t GetFileSize(cstring _name);
	static bool IsFileExists(cstring _name);
};