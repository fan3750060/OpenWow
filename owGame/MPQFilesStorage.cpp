#include "stdafx.h"

// General
#include "MPQFilesStorage.h"

//#if (VERSION == VERSION_Vanila)
//const char* archives = "D:/_games/World of Warcraft 1.12.1/Data/";
//#elif (VERSION == VERSION_WotLK)
//const char* archives = "D:/_games/World of Warcraft 3.3.5a/Data/";
//#endif

CMPQFilesStorage::CMPQFilesStorage(std::string _path, Priority _priority)
	: m_Path(_path)
	, m_Priority(_priority)
{
	// Files 1.12
#if (VERSION == VERSION_Vanila)
	AddArchive(std::string("backup.MPQ"));
	AddArchive(std::string("base.MPQ"));
	AddArchive(std::string("dbc.MPQ"));
	AddArchive(std::string("fonts.MPQ"));
	AddArchive(std::string("interface.MPQ"));
	AddArchive(std::string("misc.MPQ"));
	AddArchive(std::string("model.MPQ"));
	AddArchive(std::string("patch.MPQ"));
	AddArchive(std::string("patch-2.MPQ"));
	AddArchive(std::string("patch-3.MPQ"));
	AddArchive(std::string("sound.MPQ"));
	AddArchive(std::string("speech.MPQ"));
	AddArchive(std::string("terrain.MPQ"));
	AddArchive(std::string("texture.MPQ"));
	AddArchive(std::string("wmo.MPQ"));
	//AddArchive(std::string("ruRU/patch-1.MPQ"));
	//AddArchive(std::string("ruRU/patch-2.MPQ"));
	//AddArchive(std::string("ruRU/patch-3.MPQ"));

#elif (VERSION == VERSION_WotLK)
	AddArchive(std::string("common.MPQ"));
	AddArchive(std::string("common-2.MPQ"));
	AddArchive(std::string("expansion.MPQ"));
	AddArchive(std::string("lichking.MPQ"));
	AddArchive(std::string("patch.MPQ"));
	AddArchive(std::string("patch-2.MPQ"));
	AddArchive(std::string("patch-3.MPQ"));

	AddArchive(std::string("ruRU/locale-ruRU.MPQ"));
	AddArchive(std::string("ruRU/expansion-locale-ruRU.MPQ"));
	AddArchive(std::string("ruRU/lichking-locale-ruRU.MPQ"));
	AddArchive(std::string("ruRU/patch-ruRU.MPQ"));
	AddArchive(std::string("ruRU/patch-ruRU-2.MPQ"));
	AddArchive(std::string("ruRU/patch-ruRU-3.MPQ"));
#endif
}

CMPQFilesStorage::~CMPQFilesStorage()
{
	for (auto it : m_OpenArchives)
	{
		libmpq__archive_close(it);
	}
}



//
// // IFilesStorage
//
std::shared_ptr<IFile> CMPQFilesStorage::CreateFile(const std::string& _name)
{
	std::lock_guard<std::mutex> lock(m_Lock);

	std::shared_ptr<CFile> file = std::make_shared<CFile>("<null>", _name);
	CByteBuffer& byteBuffer = file->GetByteBuffer();
	
	SMPQFileLocation location = GetFileLocation(file->Path_Name());
	if (!location.exists)
		return nullptr;

	libmpq__off_t size;
	libmpq__file_size_unpacked(location.archive, location.fileNumber, &size);

	// Allocate space and set data
	byteBuffer.Allocate(size);
	libmpq__file_read(location.archive, location.fileNumber, &(byteBuffer.getDataEx())[0], byteBuffer.getSize(), &size);
	assert1(byteBuffer.getSize() == size);
	byteBuffer.SetFilled();

	//Log::Info("File[%s] opened. [mpq]", Path_Name().c_str());

	return file;
}

size_t CMPQFilesStorage::GetFileSize(const std::string& _name)
{
	std::lock_guard<std::mutex> lock(m_Lock);

	SMPQFileLocation location = GetFileLocation(_name);

	if (location.exists)
	{
		libmpq__off_t size;
		libmpq__file_size_unpacked(location.archive, location.fileNumber, &size);
		return size;
	}

	return 0;
}

bool CMPQFilesStorage::IsFileExists(const std::string& _name)
{
	std::lock_guard<std::mutex> lock(m_Lock);

	return GetFileLocation(_name).exists;
}



//
// IFilesStorageEx
//
IFilesStorageEx::Priority CMPQFilesStorage::GetPriority() const
{
	return m_Priority;
}



//
// CMPQFilesStorage
//
void CMPQFilesStorage::AddArchive(std::string filename)
{
	mpq_archive_s* mpq_a;
	int result = libmpq__archive_open(&mpq_a, (m_Path + filename).c_str(), -1);
	//Log::Info("Opening %s", filename.c_str());
	if (result)
	{
		switch (result)
		{
		case LIBMPQ_ERROR_OPEN:
			Log::Error("Error opening archive [%s]: Does file really exist?", filename.c_str());
			break;

		case LIBMPQ_ERROR_FORMAT:            /* bad file format */
			Log::Error("Error opening archive [%s]: Bad file format", filename.c_str());
			break;

		case LIBMPQ_ERROR_SEEK:         /* seeking in file failed */
			Log::Error("Error opening archive [%s]: Seeking in file failed", filename.c_str());
			break;

		case LIBMPQ_ERROR_READ:              /* Read error in archive */
			Log::Error("Error opening archive [%s]: Read error in archive", filename.c_str());
			break;

		case LIBMPQ_ERROR_MALLOC:               /* maybe not enough memory? :) */
			Log::Error("Error opening archive [%s]: Maybe not enough memory", filename.c_str());
			break;

		default:
			Log::Error("Error opening archive [%s]: Unknown error\n", filename.c_str());
			break;
		}

		return;
	}

	m_OpenArchives.push_back(mpq_a);
	Log::Green("CMPQFile[%s]: Added!", filename.c_str());
}

SMPQFileLocation CMPQFilesStorage::GetFileLocation(const std::string& filename)
{
	for (auto& i = m_OpenArchives.rbegin(); i != m_OpenArchives.rend(); ++i)
	{
		mpq_archive_s* mpq_a = *i;

		uint32 filenum;
		if (libmpq__file_number(mpq_a, filename.c_str(), &filenum) == LIBMPQ_ERROR_EXIST)
			continue;

		return SMPQFileLocation(mpq_a, filenum);
	}

	return SMPQFileLocation();
}
