#include "stdafx.h"

// Include
#include "MapController.h"

// General
#include "MapWDT.h"

CMapWDT::CMapWDT(std::weak_ptr<const CMapController> _mapController) :
	m_IsTileBased(false),
	m_GlobalWMO(nullptr),
	m_MapController(_mapController)
{}

CMapWDT::~CMapWDT()
{
}

void CMapWDT::CreateInsances(std::weak_ptr<SceneNodeModel3D> _parent)
{
	Log::Green("Map_GlobalWMOs[]: Global WMO exists [%s].", !m_GlobalWMOName.empty() ? "true" : "false");
	if (!m_GlobalWMOName.empty())
	{
		m_GlobalWMO = std::make_shared<ADT_WMO_Instance>(m_GlobalWMOName, m_GlobalWMOPlacementInfo);
		m_GlobalWMO->SetParent(_parent);
		Application::Get().GetLoader()->AddToLoadQueue(m_GlobalWMO);
	}
}

void CMapWDT::Load()
{
    std::shared_ptr<const CMapController> mapController = m_MapController.lock();
    assert1(mapController != NULL);

    std::string fileName = mapController->getFilenameT() + ".wdt";

    WoWChunkReader reader(fileName);
    std::shared_ptr<IByteBuffer> buffer = nullptr;

    {
        buffer = reader.OpenChunk("MVER");
        uint32 version;
        buffer->readBytes(&version, 4);
        assert1(version == 18);
    }


    {
        buffer = reader.OpenChunk("MPHD");
        buffer->readBytes(&m_MPHD, sizeof(WDT_MPHD));
    }


    {
        buffer = reader.OpenChunk("MAIN");

        for (int i = 0; i < 64; i++)
        {
            for (int j = 0; j < 64; j++)
            {
                buffer->readBytes(&m_TileFlag[j][i], sizeof(WDT_MAIN));

                if (m_TileFlag[j][i].flags.Flag_HasADT)
                    m_IsTileBased = true;
            }
        }
    }

    {
        buffer = reader.OpenChunk("MWMO");

        if (buffer && buffer->getSize() > 0)
        {
            char* buf = new char[buffer->getSize()];
            buffer->readBytes(buf, buffer->getSize());
            m_GlobalWMOName = std::string(buf);
            delete[] buf;
        }
    }

    {
        buffer = reader.OpenChunk("MODF");

        if (buffer != nullptr)
        {
            assert1(m_MPHD.flags.Flag_GlobalWMO);
            assert1((buffer->getSize() / sizeof(ADT_MODF)) == 1);
            buffer->readBytes(&m_GlobalWMOPlacementInfo, sizeof(ADT_MODF));
        }
    }


	assert1(m_IsTileBased || m_GlobalWMOName.size() > 0);
}
