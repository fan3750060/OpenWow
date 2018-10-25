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

ADT::ADT(std::weak_ptr<SceneNode> _mapController, uint32 _intexX, uint32 _intexZ) :
	m_MapController(std::dynamic_pointer_cast<MapController>(_mapController.lock())),
	m_IndexX(_intexX), 
	m_IndexZ(_intexZ),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	// Scene node params
	{
		// Set translate
		//setTranslate(vec3(_intexX * C_TileSize, 0.0f, _intexZ * C_TileSize), false);
		// Bounds
		//BoundingBox bbox
		//(
		//	vec3(getTranslate().x,              Math::MaxFloat, getTranslate().z),
		//	vec3(getTranslate().x + C_TileSize, Math::MinFloat, getTranslate().z + C_TileSize)
		//);
		//setBounds(bbox);
	}

	/*setOpaque(true);
	setDrawOrder(20);
	setDebugColor(vec4(0.0f, 0.3f, 0.3f, 0.3f));
	setSelectable();*/
}

ADT::~ADT()
{
	//Log::Info("ADT Deleted");
}

bool ADT::Load()
{
	std::shared_ptr<MapController> mapController = m_MapController.lock();
	_ASSERT(mapController != NULL);

	char filename[256];
	sprintf_s(filename, "%s_%d_%d.adt", mapController->getFilenameT().c_str(), m_IndexX, m_IndexZ);

	std::shared_ptr<IFile> f = GetManager<IFilesManager>()->Open(filename);
	uint32_t startPos = f->getPos() + 20;
	
	// MVER + size (8)
	f->seekRelative(8);
	{
		uint32 version;
		f->readBytes(&version, 4);
		_ASSERT(version == 18);
	}

	// MHDR + size (8)
	ADT_MHDR header;
	f->seekRelative(8);
	{
		f->readBytes(&header, sizeof(ADT_MHDR));
	}

	// Chunks info
	ADT_MCIN chunks[256];
	f->seek(startPos + header.MCIN);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		uint32 count = size / sizeof(ADT_MCIN);
		_ASSERT(count == C_ChunksInTileGlobal);
		memcpy(chunks, f->getDataFromCurrent(), sizeof(ADT_MCIN) * count);
	}

	// TextureInfo
	f->seek(startPos + header.MTEX);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		WOWCHUNK_READ_STRINGS_BEGIN

		std::shared_ptr<ADT_TextureInfo> textureInfo = std::make_shared<ADT_TextureInfo>();
		textureInfo->textureName = _string;

		m_Textures.push_back(textureInfo);

		WOWCHUNK_READ_STRINGS_END
	}

	// M2 names
	std::vector<std::string> m_MDXsNames;
	f->seek(startPos + header.MMDX);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		WOWCHUNK_READ_STRINGS_BEGIN
			m_MDXsNames.push_back(_string);
		WOWCHUNK_READ_STRINGS_END
		
	}

	// M2 Offsets
	std::vector<uint32> m_MDXsOffsets;
	f->seek(startPos + header.MMID);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		uint32 count = size / sizeof(uint32);
		_ASSERT(count == m_MDXsNames.size());
		for (uint32_t i = 0; i < count; i++)
		{
			uint32 offset;
			f->readBytes(&offset, sizeof(uint32));
			m_MDXsOffsets.push_back(offset);
		}
	}

	// WMO Names
	std::vector<std::string> m_WMOsNames;
	f->seek(startPos + header.MWMO);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		WOWCHUNK_READ_STRINGS_BEGIN
			m_WMOsNames.push_back(_string);
		WOWCHUNK_READ_STRINGS_END
	}

	// WMO Offsets
	std::vector<uint32> m_WMOsOffsets;
	f->seek(startPos + header.MWID);
	{
		f->seekRelative(4);
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));

		uint32 count = size / sizeof(uint32);
		_ASSERT(count == m_WMOsNames.size());
		for (uint32_t i = 0; i < count; i++)
		{
			uint32 offset;
			f->readBytes(&offset, sizeof(uint32));
			m_WMOsOffsets.push_back(offset);
		}
	}

	// M2 PlacementInfo
	std::vector<ADT_MDXDef> m_MDXsPlacementInfo;
	f->seek(startPos + header.MDDF);
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
	std::vector<ADT_MODF> m_WMOsPlacementInfo;
	f->seek(startPos + header.MODF);
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
	if (header.MH20 != 0)
	{
		f->seek(startPos + header.MH20);
		{
			f->seekRelative(4);
			uint32_t size;
			f->readBytes(&size, sizeof(uint32_t));
			_ASSERT(size > 0);

			const uint8* abuf = f->getDataFromCurrent();
			for (uint32 i = 0; i < C_ChunksInTile; i++)
			{
				for (uint32 j = 0; j < C_ChunksInTile; j++)
				{
					MH2O_Header* mh2o_Header = (MH2O_Header*) abuf;
					if (mh2o_Header->layersCount > 0)
					{
						CADT_Liquid* liquid = new CADT_Liquid(8, 8);
						liquid->CreateFromTerrainMH2O(f.operator->(), mh2o_Header);

						// Create instance
						std::shared_ptr<Liquid_Instance> instance = std::make_shared<Liquid_Instance>(weak_from_this(), liquid, vec3(getTranslate().x + j * C_ChunkSize, 0.0f, getTranslate().z + i * C_ChunkSize));
						instance->SetParent(weak_from_this());
						m_LiquidsInstances.push_back(instance);
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
		it->diffuseTexture = Application::Get().GetRenderDevice()->CreateTexture2D(it->textureName);

		// PreLoad specular texture
		std::string specularTextureName = it->textureName;
		specularTextureName = specularTextureName.insert(specularTextureName.length() - 4, "_s");
		it->specularTexture = Application::Get().GetRenderDevice()->CreateTexture2D(specularTextureName);
	}

	//-- Load Chunks ---------------------------------------------------------------------

	for (uint32_t i = 0; i < C_ChunksInTileGlobal; i++)
	{
		f->seek(chunks[i].offset);

		// Chunk + size (8)
		f->seekRelative(4); // MCNK
		uint32_t size;
		f->readBytes(&size, sizeof(uint32_t));
		_ASSERT(size + 8 == chunks[i].size);

		std::shared_ptr<ADT_MCNK> chunk = std::make_shared<ADT_MCNK>(m_MapController, std::static_pointer_cast<ADT, SceneNode>(shared_from_this()), f.operator->());
		chunk->Load();
		chunk->SetParent(m_MapController);
		m_Chunks.push_back(chunk);

		BoundingBox bbox = getBounds();
		bbox.makeUnion(chunk->getBounds());
		setBounds(bbox);
	}

	//-- WMOs --------------------------------------------------------------------------

	for (auto& it : m_WMOsPlacementInfo)
	{
		SmartWMOPtr wmo = GetManager<IWMOManager>()->Add(m_WMOsNames[it.nameIndex]);
		if (wmo)
		{
			std::shared_ptr<ADT_WMO_Instance> inst = std::make_shared<ADT_WMO_Instance>(std::static_pointer_cast<ADT, SceneNode>(shared_from_this()), wmo, it);
			inst->Load();
			inst->SetParent(shared_from_this());
			m_WMOsInstances.push_back(inst);

			BoundingBox bbox = getBounds();
			bbox.makeUnion(inst->getBounds());
			setBounds(bbox);
		}
	}

#ifdef GAME_MAP_INCLUDE_WMO_AND_M2
	//-- MDXs -------------------------------------------------------------------------
	for (auto& it : m_MDXsPlacementInfo)
	{
		SmartM2Ptr mdx = GetManager<IM2Manager>()->Add(m_MDXsNames[it.nameIndex]);
		if (mdx)
		{
			std::shared_ptr<ADT_MDX_Instance> inst = std::make_shared<ADT_MDX_Instance>(std::static_pointer_cast<ADT, SceneNode>(shared_from_this()), mdx, it);
			m_MDXsInstances.push_back(inst);

			BoundingBox bbox = getBounds();
			bbox.makeUnion(inst->getBounds());
			setBounds(bbox);
		}
	}
#endif
	//---------------------------------------------------------------------------------

	Log::Green("ADT[%d, %d, %s]: Loaded!", m_IndexX, m_IndexZ, filename);

	return true;
}

bool ADT::Delete()
{
	return true;
}

void ADT::Render(RenderEventArgs& renderEventArgs)
{
	std::shared_ptr<MapController> mapController = m_MapController.lock();
	_ASSERT(mapController != NULL);

	PipelineState* state = renderEventArgs.PipelineState;
	if (!mapController->getTileIsCurrent(m_IndexX, m_IndexZ))
	{
		return;
	}

	// Check frustrum
	//if (!checkFrustum())
	//{
	//	return false;
	//}
}
