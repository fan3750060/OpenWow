#pragma once

class CFilesManager : public IFilesManager
{
public:
	CFilesManager();
	~CFilesManager();

	// IFilesManager
	IFile* Open(cstring _fileName) override;
};