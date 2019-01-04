#include "stdafx.h"

// General
#include "MPQArchiveManager.h"

// Additional
#include "BaseManager.h"

#if (VERSION == VERSION_Vanila)
const char* archives = "D:/_games/World of Warcraft 1.12.1/Data/";
#elif (VERSION == VERSION_WotLK)
//const char* archives = "D:/_games/World of Warcraft 3.3.5a/Data/";
const char* archives = "C:/Data_3.3.5/";
#endif

//


CMPQArchiveManager::CMPQArchiveManager()
{
	AddManager<IMPQArchiveManager>(this);

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
	//AddArchive(std::string("patch-w.MPQ"));
	//AddArchive(std::string("patch-x.MPQ"));

	AddArchive(std::string("ruRU/locale-ruRU.MPQ"));
	AddArchive(std::string("ruRU/expansion-locale-ruRU.MPQ"));
	AddArchive(std::string("ruRU/lichking-locale-ruRU.MPQ"));
	AddArchive(std::string("ruRU/patch-ruRU.MPQ"));
	AddArchive(std::string("ruRU/patch-ruRU-2.MPQ"));
	AddArchive(std::string("ruRU/patch-ruRU-3.MPQ"));
	//AddArchive(std::string("ruRU/patch-ruRU-w.MPQ"));
	//AddArchive(std::string("ruRU/patch-ruRU-x.MPQ"));
#endif



}

CMPQArchiveManager::~CMPQArchiveManager()
{
	for (auto it : m_OpenArchives)
	{
		libmpq__archive_close(it);
	}
}

void CMPQArchiveManager::AddArchive(std::string filename)
{
	mpq_archive_s* mpq_a;
	int result = libmpq__archive_open(&mpq_a, (archives + filename).c_str(), -1);
	Log::Info("Opening %s", filename.c_str());
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

SMPQFileLocation CMPQArchiveManager::GetFileLocation(cstring filename)
{
	for (auto& i = m_OpenArchives.rbegin(); i != m_OpenArchives.rend(); ++i)
	{
		mpq_archive_s* mpq_a = *i;

		uint32 filenum;
		if (libmpq__file_number(mpq_a, filename.c_str(), &filenum) == LIBMPQ_ERROR_EXIST)
		{
			continue;
		}

		return SMPQFileLocation(mpq_a, filenum);
	}

	return SMPQFileLocation();
}