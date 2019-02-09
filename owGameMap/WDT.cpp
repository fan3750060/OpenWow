#include "stdafx.h"

// Include
#include "MapController.h"

// General
#include "WDT.h"

WDT::WDT(std::weak_ptr<const MapController> _mapController) :
	m_IsTileBased(false),
	m_GlobalWMO(nullptr),
	m_MapController(_mapController)
{}

WDT::~WDT()
{
}

void WDT::CreateInsances(std::weak_ptr<SceneNode3D> _parent)
{
	return;

	Log::Green("Map_GlobalWMOs[]: Global WMO exists [%s].", !m_GlobalWMOName.empty() ? "true" : "false");
	if (!m_GlobalWMOName.empty())
	{
		m_GlobalWMO = std::make_shared<ADT_WMO_Instance>(m_GlobalWMOName, m_GlobalWMOPlacementInfo);
		m_GlobalWMO->SetParent(_parent);
		Application::Get().GetLoader()->AddToLoadQueue(m_GlobalWMO);
	}
}

void WDT::Load()
{
	std::shared_ptr<const MapController> mapController = m_MapController.lock();
	assert1(mapController != NULL);

	std::string fileName = mapController->getFilenameT() + ".wdt";

	std::shared_ptr<IFile> f = GetManager<IFilesManager>()->Open(fileName);
	if (f == nullptr)
	{
		Log::Info("Map[%s]: WDT: Error opening.", fileName.c_str());
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
			f->readBytes(&m_MPHD, sizeof(WDT_MPHD));
		}
		else if (strcmp(fourcc, "MAIN") == 0) // Map tile table. Contains 64x64 = 4096 records of 8 bytes each.
		{
			for (int i = 0; i < 64; i++)
			{
				for (int j = 0; j < 64; j++)
				{
					f->readBytes(&m_TileFlag[j][i], sizeof(WDT_MAIN));

					if (m_TileFlag[j][i].flags.Flag_HasADT)
						m_IsTileBased = true;
				}
			}
		}
		else if (strcmp(fourcc, "MWMO") == 0) // Global WMO filename
		{
			if (size > 0)
			{
				char* buf = new char[size];
				f->readBytes(buf, size);
				m_GlobalWMOName = std::string(buf);
				delete[] buf;
			}
		}
		else if (strcmp(fourcc, "MODF") == 0)
		{
			assert1(m_MPHD.flags.Flag_GlobalWMO);
			assert1((size / sizeof(ADT_MODF)) == 1);
			f->readBytes(&m_GlobalWMOPlacementInfo, sizeof(ADT_MODF));
		}
		else
		{
			Log::Fatal("Map[%s]: WDT: Chunks [%s], Size [%d] not implemented.", fileName.c_str(), fourcc, size);
		}

		f->seek(nextpos);
	}

	assert1(m_IsTileBased || m_GlobalWMOName.size() > 0);
}
