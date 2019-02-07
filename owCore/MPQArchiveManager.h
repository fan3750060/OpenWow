#pragma once

// MPQ Files
#include <libmpq/mpq.h>

class CMPQArchiveManager : public IMPQArchiveManager
{
public:
	CMPQArchiveManager();
	~CMPQArchiveManager();

	void AddArchive(std::string _filename);
	SMPQFileLocation GetFileLocation(cstring _filename);

	std::mutex& Guard();

private:
	std::vector<mpq_archive_s*> m_OpenArchives;
	std::mutex                  m_Lock;
};