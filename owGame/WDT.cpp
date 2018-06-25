#include "stdafx.h"

// General
#include "WDT.h"

// Additional
#include "WorldController.h"

WDT::WDT() :
	m_IsTileBased(false)
{}

void WDT::CreateInsances(SceneNode* _parent)
{
	Log::Green("Map_GlobalWMOs[]: Global WMO exists [%s].", !m_GlobalWMOName.empty() ? "true" : "false");
	if (!m_GlobalWMOName.empty())
	{
		WMO* wmo = GetManager<IWMOManager>()->Add(m_GlobalWMOName);
		m_GlobalWMO = new ADT_WMO_Instance(_parent, wmo, m_GlobalWMOPlacementInfo);
	}
}

void WDT::Load(cstring _name)
{
	UniquePtr<IFile> f = GetManager<IFilesManager>()->Open(_name);
	if (f == nullptr)
	{
		Log::Info("Map[%s]: WDT: Error opening.", _name.c_str());
		return;
	}

	char fourcc[5];
	uint32 size;

	while (!f->isEof())
	{
		memset(fourcc, 0, 4);
		size = 0;
		f->readBytes(fourcc, 4);
		f->readBytes(&size, 4);
		flipcc(fourcc);
		fourcc[4] = 0;
		if (size == 0) continue;
		uint32_t nextpos = f->getPos() + size;

		if (strcmp(fourcc, "MVER") == 0)
		{
			uint32 version;
			f->readBytes(&version, 4);
			assert1(version == 18);
		}
		else if (strcmp(fourcc, "MPHD") == 0)
		{
			f->readBytes(&m_Flag, 4);
		}
		else if (strcmp(fourcc, "MAIN") == 0) // Map tile table. Contains 64x64 = 4096 records of 8 bytes each.
		{
			for (int i = 0; i < 64; i++)
			{
				for (int j = 0; j < 64; j++)
				{
					// Flag
					f->readBytes(&m_TileFlag[j][i], sizeof(WDT_MAIN));

					if (m_TileFlag[j][i].flags.Flag_HasADT)
					{
						m_IsTileBased = true;
					}
				}
			}
		}
		else if (strcmp(fourcc, "MWMO") == 0) // Global WMO filename
		{
			if (size > 0)
			{
				char* buf = new char[size];
				f->readBytes(buf, size);
				m_GlobalWMOName = string(buf);
				delete[] buf;
			}
		}
		else if (strcmp(fourcc, "MODF") == 0)
		{
			assert1((size / sizeof(ADT_MODF)) == 1);
			f->readBytes(&m_GlobalWMOPlacementInfo, sizeof(ADT_MODF));
		}
		else
		{
			Log::Info("Map[%s]: WDT: Chunks [%s], Size [%d] not implemented.", _name.c_str(), fourcc, size);
		}

		f->seek(nextpos);
	}
}
