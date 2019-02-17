#pragma once

// MPQ Files
#include <libmpq/mpq.h>

struct SMPQFileLocation
{
	SMPQFileLocation() :
		archive(nullptr),
		fileNumber(0),
		exists(false)
	{}

	SMPQFileLocation(mpq_archive* _archive, uint32 _fileNumber) :
		archive(_archive),
		fileNumber(_fileNumber),
		exists(true)
	{}

	bool exists;
	mpq_archive* archive;
	uint32 fileNumber;
};

class CMPQFilesStorage : public IFilesStorage, public IFilesStorageEx
{
public:
	CMPQFilesStorage(std::string _path, Priority _priority = PRIOR_NORMAL);
	virtual ~CMPQFilesStorage();

	// IFilesStorage
	std::shared_ptr<IFile>  CreateFile(cstring _name) override;
	size_t                  GetFileSize(cstring _name) override;
	bool                    IsFileExists(cstring _name) override;

	// IFilesStorageEx
	Priority GetPriority() const;

	// CMPQFilesStorage
	void AddArchive(std::string _filename);
	SMPQFileLocation GetFileLocation(cstring _filename);

private:
	const std::string           m_Path;
	const Priority              m_Priority;

	std::vector<mpq_archive_s*> m_OpenArchives;
	std::mutex                  m_Lock;
};