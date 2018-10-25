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

private:
	std::vector<mpq_archive_s*> m_OpenArchives;
};