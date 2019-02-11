#pragma once


class CFilesManager : public IFilesManager
{
public:
	CFilesManager();
	virtual ~CFilesManager();

	// IFilesManager
	std::shared_ptr<IFile> Open(cstring _fileName) override final;
};