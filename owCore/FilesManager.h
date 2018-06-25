#pragma once

#include <mutex>

class CFilesManager : public IFilesManager
{
public:
	CFilesManager();
	~CFilesManager();

	// IFilesManager
	IFile* Open(cstring _fileName) override;
	void Lock() override;
	void Unlock() override;
	void Guard() override;

private:
	std::mutex m_Lock;
};