#include "stdafx.h"

// Include
#include "WMO_Base_Instance.h"

// General
#include "WMO.h"

struct WMO_GroupInfoDef
{
	SWMOGroup_Flags	flags;
	CAaBox			bounding_box;
	int32			nameoffset;		// name in MOGN chunk (-1 for no name)
};

WMO::WMO(cstring name) :
	m_FileName(name),
	m_TexturesNames(nullptr),
	m_PortalController(nullptr),
	m_DoodadsFilenames(nullptr)

{}

WMO::~WMO()
{
	Log::Info("WMO[%s]: Unloading...", m_FileName.c_str());

	//

	if (m_TexturesNames) delete[] m_TexturesNames;
	ERASE_VECTOR(m_Materials);
	ERASE_VECTOR(m_Groups);
	ERASE_VECTOR(m_Portals);
	if (m_PortalController) delete m_PortalController;
	ERASE_VECTOR(m_Lights);
	if (m_DoodadsFilenames) delete[] m_DoodadsFilenames;
	ERASE_VECTOR(m_Fogs);
}

void WMO::CreateInsances(CWMO_Base_Instance* _parent)
{
	for (auto& it : m_Groups)
	{
		CWMO_Group_Instance* groupInstance = new CWMO_Group_Instance(_parent, it);
		_parent->AddGroupInstance(groupInstance);
		if (it->m_Header.flags.IS_OUTDOOR)
		{
			_parent->AddOutdoorGroupInstance(groupInstance);
		}

		it->CreateInsances(groupInstance);
	}
}

bool WMO::Load()
{
	UniquePtr<IFile> f = GetManager<IFilesManager>()->Open(m_FileName);
	if (f == nullptr)
	{
		return false;
	}

	char* groupsNames;
	char* skyboxFilename;

	char fourcc[5];
	uint32 size;
	while (!f->isEof())
	{
		memset(fourcc, 0, 4);
		f->readBytes(fourcc, 4);
		flipcc(fourcc);
		fourcc[4] = 0;
		size = 0;
		f->readBytes(&size, 4);
		if (size == 0) continue;
		uint32_t nextpos = f->getPos() + size;

		if (strcmp(fourcc, "MVER") == 0)                    // Version
		{
			uint32 version;
			f->readBytes(&version, 4);
			assert1(version == 17);
		}
		else if (strcmp(fourcc, "MOHD") == 0)               // Header
		{
			f->readBytes(&m_Header, sizeof(SWMO_HeaderDef));
			m_Bounds.set(m_Header.bounding_box.min, m_Header.bounding_box.max, true);
		}
		else if (strcmp(fourcc, "MOTX") == 0)               // List of m_DiffuseTextures (BLP Files) used in this map object.
		{
			m_TexturesNames = new char[size + 1];
			f->readBytes(m_TexturesNames, size);
			m_TexturesNames[size] = 0x00;
		}
		else if (strcmp(fourcc, "MOMT") == 0)               // Materials used in this map object, 64 bytes per texture (BLP file), nMaterials entries.
		{
			uint32 materialsCount = size / sizeof(SWMO_MaterialDef);
			SWMO_MaterialDef* materials = (SWMO_MaterialDef*)f->getDataFromCurrent();
			assert1(materialsCount == m_Header.nTextures);
			for (uint32 i = 0; i < materialsCount; i++)
			{
				WMO_Part_Material* _mat = new WMO_Part_Material(this, materials[i]);
				m_Materials.push_back(_mat);
			}
		}
		else if (strcmp(fourcc, "MOGN") == 0)              // List of group names for the m_Groups in this map object.
		{
			groupsNames = new char[size + 1];
			f->readBytes(groupsNames, size);
			groupsNames[size] = 0x00;
		}
		else if (strcmp(fourcc, "MOGI") == 0)
		{
			uint32 groupInfosCount = size / sizeof(WMO_GroupInfoDef);
			WMO_GroupInfoDef* groupInfos = (WMO_GroupInfoDef*)f->getDataFromCurrent();
			assert1(groupInfosCount == m_Header.nGroups);
			for (uint32 i = 0; i < groupInfosCount; i++)
			{
				char temp[256];
				strcpy_s(temp, m_FileName.c_str());
				temp[m_FileName.length() - 4] = 0;

				char fname[256];
				sprintf_s(fname, "%s_%03d.wmo", temp, i);
				IFile* groupFile = GetManager<IFilesManager>()->Open(fname);

				string groupName = groupFile->Name();
				if (groupInfos[i].nameoffset > 0)
				{
					groupName = string(groupsNames + groupInfos[i].nameoffset);
				}

				WMO_Group* group = new WMO_Group(this, i, groupName, groupFile);
				m_Groups.push_back(group);
			}
		}
		else if (strcmp(fourcc, "MOSB") == 0) // Skybox. 
		{
			if (size > 4)
			{
				skyboxFilename = new char[size + 1];
				f->readBytes(skyboxFilename, size);
				skyboxFilename[size] = 0x00;
				Log::Error("WMO[%s]: Skybox [%s]", m_FileName.c_str(), skyboxFilename);
				//m_Skybox = _World->LoadMDX(skyboxFilename);
			}
		}
		else if (strcmp(fourcc, "MOPV") == 0)
		{
			uint32 portalVertexesCount = size / sizeof(vec3);
			vec3* portalVertexes = (vec3*)f->getDataFromCurrent();
			for (uint32 i = 0; i < portalVertexesCount; i++)
			{
				m_PortalVertices.push_back(portalVertexes[i].toXZmY());
			}
			m_PortalVB = _Render->r.createVertexBuffer(m_PortalVertices.size() * sizeof(vec3), m_PortalVertices.data(), false);
		}
		else if (strcmp(fourcc, "MOPT") == 0)
		{
			uint32 portalDefsCount = size / sizeof(SWMO_PortalDef);
			SWMO_PortalDef* portalDefs = (SWMO_PortalDef*)f->getDataFromCurrent();
			assert1(portalDefsCount == m_Header.nPortals);
			for (uint32 i = 0; i < portalDefsCount; i++)
			{
				CWMO_Part_Portal* portal = new CWMO_Part_Portal(this, portalDefs[i]);
				m_Portals.push_back(portal);
			}
		}
		else if (strcmp(fourcc, "MOPR") == 0)
		{
			uint32 portalReferencesCount = size / sizeof(SWMO_PortalReferencesDef);
			SWMO_PortalReferencesDef* portalReferences = (SWMO_PortalReferencesDef*)f->getDataFromCurrent();
			for (uint32 i = 0; i < portalReferencesCount; i++)
			{
				m_PortalReferences.push_back(portalReferences[i]);
			}
		}
		else if (strcmp(fourcc, "MOVV") == 0)
		{
			uint32 visibleVertexesCount = size / sizeof(vec3);
			vec3* visibleVertexes = (vec3*)f->getDataFromCurrent();
			for (uint32 i = 0; i < visibleVertexesCount; i++)
			{
				m_VisibleBlockVertices.push_back(visibleVertexes[i].toXZmY());
			}
		}
		else if (strcmp(fourcc, "MOVB") == 0)
		{
			uint32 visibleBlockListsCount = size / sizeof(SWMO_VisibleBlockListDef);
			SWMO_VisibleBlockListDef* visibleBlockLists = (SWMO_VisibleBlockListDef*)f->getDataFromCurrent();
			for (uint32 i = 0; i < visibleBlockListsCount; i++)
			{
				m_VisibleBlockList.push_back(visibleBlockLists[i]);
			}
		}
		else if (strcmp(fourcc, "MOLT") == 0)
		{
			uint32 lightsCount = size / sizeof(SWMO_LightDef);
			SWMO_LightDef* lights = (SWMO_LightDef*)f->getDataFromCurrent();
			assert1(lightsCount == m_Header.nLights);
			for (uint32 i = 0; i < lightsCount; i++)
			{
				WMO_Part_Light* _wmoLight = new WMO_Part_Light(lights[i]);
				m_Lights.push_back(_wmoLight);
			}
		}
		else if (strcmp(fourcc, "MODS") == 0)
		{
			uint32 doodadsSetsCount = size / sizeof(SWMO_Doodad_SetInfo);
			SWMO_Doodad_SetInfo* doodadsSets = (SWMO_Doodad_SetInfo*)f->getDataFromCurrent();
			assert1(doodadsSetsCount == m_Header.nDoodadSets);
			for (uint32 i = 0; i < doodadsSetsCount; i++)
			{
				m_DoodadsSetInfos.push_back(doodadsSets[i]);
			}
		}
		else if (strcmp(fourcc, "MODN") == 0) // List of filenames for M2 (mdx) models that appear in this WMO.
		{
			m_DoodadsFilenames = new char[size + 1];
			f->readBytes(m_DoodadsFilenames, size);
			m_DoodadsFilenames[size] = 0x00;
		}
		else if (strcmp(fourcc, "MODD") == 0) // Information for doodad instances. 40 bytes per doodad instance, nDoodads entries.
		{
			uint32 doodadsPlacementsCount = size / sizeof(SWMO_Doodad_PlacementInfo);
			SWMO_Doodad_PlacementInfo* doodadsPlacements = (SWMO_Doodad_PlacementInfo*)f->getDataFromCurrent();
			for (uint32 i = 0; i < doodadsPlacementsCount; i++)
			{
				m_DoodadsPlacementInfos.push_back(doodadsPlacements[i]);
			}

			m_Header.nDoodadDefs = doodadsPlacementsCount; // HACK! INCORRECT SIZE
		}
		else if (strcmp(fourcc, "MFOG") == 0)
		{
			uint32 fogsCount = size / sizeof(SWMO_FogDef);
			SWMO_FogDef* fogs = (SWMO_FogDef*)f->getDataFromCurrent();
			for (uint32 i = 0; i < fogsCount; i++)
			{
				WMO_Part_Fog* fog = new WMO_Part_Fog(fogs[i]);
				m_Fogs.push_back(fog);
			}
		}
		else if (strcmp(fourcc, "MCVP") == 0)
		{
			//Convex Volume Planes.Contains blocks of floating - point numbers.
			// C4Plane convex_volume_planes[];   // normal points out
		}
		else
		{
			Log::Fatal("WMO[%s]: No implement wmo chunk [%s] [%d].", m_FileName.c_str(), fourcc, size);
		}

		f->seek(nextpos);
	}

	// Create portal controller
	if (m_Portals.size() > 0)
	{
		m_PortalController = new CWMO_PortalsController(this);

		for (auto& it : m_PortalReferences)
		{
			assert1(it.portalIndex < m_Portals.size());
			assert1(it.groupIndex < m_Groups.size());
		}
	}

	// Init m_Groups
	for (auto& it : m_Groups)
	{
		it->Load();

		assert1(it->m_Header.flags.IS_OUTDOOR != it->m_Header.flags.IS_INDOOR);

		// Add outdoor group
		if (it->m_Header.flags.IS_OUTDOOR)
		{
			m_OutdoorGroups.push_back(it);
		}
	}

	return true;
}

//#define WMO_DISABLE_PORTALS

void WMO::Render(CWMO_Base_Instance* _localContr) const
{
	/*for (auto& it : m_Portals)
	{
		it->Render(_localContr->getAbsTrans());
	}*/
}

bool WMO::drawSkybox()
{
	if (m_Skybox == nullptr)
	{
		return false;
	}

	mat4 worldMatrix;
	worldMatrix.translate(_Render->getCamera()->Position);
	worldMatrix.scale(2.0f);

	m_Skybox->Render(worldMatrix, nullptr, vec4(1.0f));

	//_World->EnvM()->m_HasSky = true;

	return true;
}