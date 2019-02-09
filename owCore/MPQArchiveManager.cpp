#include "stdafx.h"

// General
#include "MPQArchiveManager.h"

// Additional
#include "BaseManager.h"

#if (VERSION == VERSION_Vanila)
const char* archives = "D:/_games/World of Warcraft 1.12.1/Data/";
#elif (VERSION == VERSION_WotLK)
const char* archives = "D:/_games/World of Warcraft 3.3.5a/Data/";
#elif (VERSION == VERSION_Cata)
const char* archives = "D:/_games/World of Warcraft 4.3.4/Data/";
#endif

//


CMPQArchiveManager::CMPQArchiveManager()
{
	// Files 1.12
#if (VERSION == VERSION_Vanila)
	AddArchive("backup.MPQ");
	AddArchive("base.MPQ");
	AddArchive("dbc.MPQ");
	AddArchive("fonts.MPQ");
	AddArchive("interface.MPQ");
	AddArchive("misc.MPQ");
	AddArchive("model.MPQ");
	AddArchive("patch.MPQ");
	AddArchive("patch-2.MPQ");
	AddArchive("patch-3.MPQ");
	AddArchive("sound.MPQ");
	AddArchive("speech.MPQ");
	AddArchive("terrain.MPQ");
	AddArchive("texture.MPQ");
	AddArchive("wmo.MPQ");

	//AddArchive("ruRU/patch-1.MPQ");
	//AddArchive("ruRU/patch-2.MPQ");
	//AddArchive("ruRU/patch-3.MPQ");

#elif (VERSION == VERSION_WotLK)
	AddArchive("common.MPQ");
	AddArchive("common-2.MPQ");
	AddArchive("expansion.MPQ");
	AddArchive("lichking.MPQ");
	AddArchive("patch.MPQ");
	AddArchive("patch-2.MPQ");
	AddArchive("patch-3.MPQ");

	AddArchive("ruRU/locale-ruRU.MPQ");
	AddArchive("ruRU/expansion-locale-ruRU.MPQ");
	AddArchive("ruRU/lichking-locale-ruRU.MPQ");
	AddArchive("ruRU/patch-ruRU.MPQ");
	AddArchive("ruRU/patch-ruRU-2.MPQ");
	AddArchive("ruRU/patch-ruRU-3.MPQ");

#elif (VERSION == VERSION_Cata)
	AddArchive("art.MPQ");
	AddArchive("expansion1.MPQ");
	AddArchive("expansion2.MPQ");
	AddArchive("expansion3.MPQ");
	AddArchive("world.MPQ");
	AddArchive("world2.MPQ");
	//AddArchive("wow-update-base-15211.MPQ");
	//AddArchive("wow-update-base-15354.MPQ");
	//AddArchive("wow-update-base-15595.MPQ");

	AddArchive("ruRU/expansion1-locale-ruRU.MPQ");
	AddArchive("ruRU/expansion2-locale-ruRU.MPQ");
	AddArchive("ruRU/expansion3-locale-ruRU.MPQ");
	AddArchive("ruRU/locale-ruRU.MPQ");
	//AddArchive("ruRU/wow-update-ruRU-15211.MPQ");
	//AddArchive("ruRU/wow-update-ruRU-15354.MPQ");
	//AddArchive("ruRU/wow-update-ruRU-15595.MPQ");
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

std::mutex& CMPQArchiveManager::Guard()
{
	return m_Lock;
}
