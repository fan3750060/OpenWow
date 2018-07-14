#include "stdafx.h"

// Include
#include "MapController.h"

// General
#include "ADT.h"

// Additional
#include "ADT_Liquid.h"

#include __PACK_BEGIN

struct ADT_MCIN
{
	uint32_t offset;               // absolute offset.
	uint32_t size;                 // the size of the MCNK chunk, this is refering to.
	uint32_t flags;                // always 0. only set in the client., FLAG_LOADED = 1
	uint32_t asyncId;
};

#include __PACK_END

ADT::ADT(MapController* _mapController, uint32 _intexX, uint32 _intexZ) :
	SceneNode(_mapController),
	m_MapController(_mapController),
	m_IndexX(_intexX), 
	m_IndexZ(_intexZ),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	// Scene node params
	{
		setOpaque(true);

		// DON'T CALCULATE MATRIX
		CalculateMatrix(); 

		// Set translate
		m_Translate = vec3(_intexX * C_TileSize, 0.0f, _intexZ * C_TileSize);

		// Bounds
		m_Bounds.setMin(vec3(m_Translate.x, Math::MaxFloat, m_Translate.z));
		m_Bounds.setMax(vec3(m_Translate.x + C_TileSize, Math::MinFloat, m_Translate.z + C_TileSize));
		m_Bounds.calculateCenter();
		
	}

	setDrawOrder(20);
	setDebugColor(vec4(0.0f, 0.3f, 0.3f, 0.3f));
	setSelectable();
}

ADT::~ADT()
{
	//Log::Info("ADT Deleted");
}

bool ADT::Load()
{
	char filename[256];
	sprintf_s(filename, "%s_%d_%d.adt", m_MapController->getFilenameT().c_str(), m_IndexX, m_IndexZ);

	SharedPtr<IFile> f = GetManager<IFilesManager>()->Open(filename);
	uint32_t startPos = f->getPos() + 20;
	
	// MVER + size (8)
	f->seekRelative(8);
	{
		uint32 version;
		f->readBytes(&version, 4);
		assert1(version == 18);
	}

	// MHDR + size (8)
	f->seekRelative(8);
	{
		f->readBytes(&m_Header, sizeof(ADT_MHDR));
	}

	// Chunks info
	ADT_MCIN chunks[256];
	f->seek(startPos + m_Header.MCIN);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		uint32 count = size / sizeof(ADT_MCIN);
		assert1(count == C_ChunksInTileGlobal);
		memcpy(chunks, f->getDataFromCurrent(), sizeof(ADT_MCIN) * count);
	}

	// TextureInfo
	f->seek(startPos + m_Header.MTEX);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		WOWCHUNK_READ_STRINGS_BEGIN

		SharedPtr<ADT_TextureInfo> textureInfo = new ADT_TextureInfo();
		textureInfo->textureName = _string;

		m_Textures.push_back(textureInfo);

		//Log::Info("ADT[]: Texture [%s] added", textureInfo->textureName.c_str());

		WOWCHUNK_READ_STRINGS_END
	}

	// M2 names
	vector<string> m_MDXsNames;
	f->seek(startPos + m_Header.MMDX);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		WOWCHUNK_READ_STRINGS_BEGIN
			m_MDXsNames.push_back(_string);
		WOWCHUNK_READ_STRINGS_END
		
	}

	// M2 Offsets
	vector<uint32> m_MDXsOffsets;
	f->seek(startPos + m_Header.MMID);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		uint32 count = size / sizeof(uint32);
		assert1(count == m_MDXsNames.size());
		for (uint32_t i = 0; i < count; i++)
		{
			uint32 offset;
			f->readBytes(&offset, sizeof(uint32));
			m_MDXsOffsets.push_back(offset);
		}
	}

	// WMO Names
	vector<string> m_WMOsNames;
	f->seek(startPos + m_Header.MWMO);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		WOWCHUNK_READ_STRINGS_BEGIN
			m_WMOsNames.push_back(_string);
		WOWCHUNK_READ_STRINGS_END
	}

	// WMO Offsets
	vector<uint32> m_WMOsOffsets;
	f->seek(startPos + m_Header.MWID);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		uint32 count = size / sizeof(uint32);
		assert1(count == m_WMOsNames.size());
		for (uint32_t i = 0; i < count; i++)
		{
			uint32 offset;
			f->readBytes(&offset, sizeof(uint32));
			m_WMOsOffsets.push_back(offset);
		}
	}

	// M2 PlacementInfo
	vector<ADT_MDXDef> m_MDXsPlacementInfo;
	f->seek(startPos + m_Header.MDDF);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		for (uint32 i = 0; i < size / sizeof(ADT_MDXDef); i++)
		{
			ADT_MDXDef placementInfo;
			f->readBytes(&placementInfo, sizeof(ADT_MDXDef));
			m_MDXsPlacementInfo.push_back(placementInfo);
		}
	}

	// WMO PlacementInfo
	vector<ADT_MODF> m_WMOsPlacementInfo;
	f->seek(startPos + m_Header.MODF);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		for (uint32 i = 0; i < size / sizeof(ADT_MODF); i++)
		{
			ADT_MODF placementInfo;
			f->readBytes(&placementInfo, sizeof(ADT_MODF));
			m_WMOsPlacementInfo.push_back(placementInfo);
		}
	}

	// Liquids
	if (m_Header.MH20 != 0)
	{
		f->seek(startPos + m_Header.MH20);
		{
			f->seekRelative(4);
			uint32_t size;
			f->readBytes(&size, sizeof(uint32_t));
			assert1(size > 0);

			const uint8* abuf = f->getDataFromCurrent();
			for (uint32 i = 0; i < C_ChunksInTile; i++)
			{
				for (uint32 j = 0; j < C_ChunksInTile; j++)
				{
					MH2O_Header* mh2o_Header = (MH2O_Header*) abuf;
					if (mh2o_Header->layersCount > 0)
					{
						CADT_Liquid* liquid = new CADT_Liquid(8, 8);
						liquid->CreateFromTerrainMH2O(f, mh2o_Header);

						// Create instance
						new Liquid_Instance(this, liquid, vec3(m_Translate.x + j * C_ChunkSize, 0.0f, m_Translate.z + i * C_ChunkSize));
					}
					abuf += sizeof(MH2O_Header);
				}
			}
		}
	}

	//-- Load Textures -------------------------------------------------------------------

	for (auto& it : m_Textures)
	{
		// PreLoad diffuse texture
		it->diffuseTexture = _Render->TexturesMgr()->Add(it->textureName);

		// PreLoad specular texture
		string specularTextureName = it->textureName;
		specularTextureName = specularTextureName.insert(specularTextureName.length() - 4, "_s");
		it->specularTexture = _Render->TexturesMgr()->Add(specularTextureName);
	}

	//-- Load Chunks ---------------------------------------------------------------------

	for (uint32_t i = 0; i < C_ChunksInTileGlobal; i++)
	{
		f->seek(chunks[i].offset);

		// Chunk + size (8)
		f->seekRelative(4); // MCNK
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));
		assert1(size + 8 == chunks[i].size);

		SharedPtr<ADT_MCNK> chunk = new ADT_MCNK(m_MapController, this, f);
		chunk->Load();
		m_Chunks.push_back(chunk);

		m_Bounds.makeUnion(chunk->getBounds());
	}

	//-- WMOs --------------------------------------------------------------------------

	/*for (auto& it : m_WMOsPlacementInfo)
	{
		WMO* wmo = (WMO*)GetManager<IWMOManager>()->Add(m_WMOsNames[it.nameIndex]);
		if (wmo)
		{
			ADT_WMO_Instance* inst = new ADT_WMO_Instance(this, wmo, it);
			m_WMOsInstances.push_back(inst);
			m_Bounds.makeUnion(inst->getBounds());
		}
	}*/

	//-- MDXs -------------------------------------------------------------------------

	for (auto& it : m_MDXsPlacementInfo)
	{
		M2* mdx = (M2*)GetManager<IM2Manager>()->Add(m_MDXsNames[it.nameIndex]);
		if (mdx)
		{
			ADT_MDX_Instance* inst = new ADT_MDX_Instance(this, mdx, it);
			m_MDXsInstances.push_back(inst);
			m_Bounds.makeUnion(inst->getBounds());
		}
	}

	//---------------------------------------------------------------------------------

	Log::Green("ADT[%d, %d, %s]: Loaded!", m_IndexX, m_IndexZ, filename);

	return SceneNode::Load();
}

bool ADT::Delete()
{
	return true;
}

bool ADT::PreRender3D()
{
	if (!m_MapController->getTileIsCurrent(m_IndexX, m_IndexZ))
	{
		return false;
	}

	// Check frustrum
	if (_Render->getCamera()->_frustum.cullBox(m_Bounds))
	{
		return false;
	}

	return true;
}

void ADT::Render3D()
{
	//_Render->DrawBoundingBox(m_Bounds);
}
