#pragma once


class CFilesManager : public IFilesManager
{
public:
	CFilesManager();
	~CFilesManager();

	// IFilesManager
	std::shared_ptr<IFile> Open(cstring _fileName) override;
};