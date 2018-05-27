#pragma once

class FilesManager : public IFilesManager
{
	CLASS_INSTANCE(FilesManager);
public:
	FilesManager();
	~FilesManager();

	// IFilesManager
	IFile* Open(cstring _fileName) override;
};

#define _Files FilesManager::instance()