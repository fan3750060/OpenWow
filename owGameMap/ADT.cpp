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
		SetTranslate(vec3(_intexX * C_TileSize, 0.0f, _intexZ * C_TileSize));

		UpdateLocalTransform();
		UpdateWorldTransform();

		BoundingBox bbox
		(
			vec3(GetTranslation().x,              Math::MaxFloat, GetTranslation().z),
			vec3(GetTranslation().x + C_TileSize, Math::MinFloat, GetTranslation().z + C_TileSize)
		);
		SetBounds(bbox);
	}
}

ADT::~ADT()
{
	//Log::Info("ADT Deleted");
}

//
// SceneNode
//
void ADT::UpdateLocalTransform(bool _forced)
{
	// Don't calculate local transform
	SetLocalUnderty();
}

bool ADT::Accept(IVisitor& visitor)
{
	const BasePass& visitorAsBasePass = reinterpret_cast<BasePass&>(visitor);
	const Camera* camera = visitorAsBasePass.GetRenderEventArgs().Camera;

	std::shared_ptr<MapController> mapController = m_MapController.lock();
	assert1(mapController != NULL);
	//if (!mapController->getTileIsCurrent(m_IndexX, m_IndexZ))
	//{
	//	return false;
	//}

	// Check frustrum
	//if (!checkFrustum(camera))
	//{
	//	return false;
	//}

	return SceneNode::Accept(visitor);
}

bool ADT::Load()
{
	std::shared_ptr<MapController> mapController = m_MapController.lock();
	assert1(mapController != NULL);

	char filename[256];
	sprintf_s(filename, "%s_%d_%d.adt", mapController->getFilenameT().c_str(), m_IndexX, m_IndexZ);

	std::shared_ptr<IFile> f = GetManager<IFilesManager>()->Open(filename);
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
		assert1(count == C_ChunksInTileGlobal);
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
		assert1(count == m_MDXsNames.size());
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
		assert1(count == m_WMOsNames.size());
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
			assert1(size > 0);

			const uint8* abuf = f->getDataFromCurrent();
			for (uint32 i = 0; i < C_ChunksInTile; i++)
			{
				for (uint32 j = 0; j < C_ChunksInTile; j++)
				{
					MH2O_Header* mh2o_Header = (MH2O_Header*) abuf;
					if (mh2o_Header->layersCount > 0)
					{
						std::shared_ptr<CADT_Liquid> liquid = std::make_shared<CADT_Liquid>(8, 8);
						liquid->CreateFromTerrainMH2O(f, mh2o_Header);

						// Create instance
						std::shared_ptr<Liquid_Instance> instance = std::make_shared<Liquid_Instance>(liquid, vec3(GetTranslation().x + j * C_ChunkSize, 0.0f, GetTranslation().z + i * C_ChunkSize));
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
		it->diffuseTexture = _RenderDevice->CreateTexture2D(it->textureName);

		// PreLoad specular texture
		std::string specularTextureName = it->textureName;
		specularTextureName = specularTextureName.insert(specularTextureName.length() - 4, "_s");
		it->specularTexture = _RenderDevice->CreateTexture2D(specularTextureName);
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

		std::shared_ptr<ADT_MCNK> chunk = std::make_shared<ADT_MCNK>(m_MapController, std::static_pointer_cast<ADT, SceneNode>(shared_from_this()), f);
		chunk->SetParent(m_MapController);
		chunk->Load();
		m_Chunks.push_back(chunk);

		BoundingBox bbox = GetBounds();
		bbox.makeUnion(chunk->GetBounds());
		SetBounds(bbox);
	}

	//-- WMOs --------------------------------------------------------------------------

	for (auto& it : m_WMOsPlacementInfo)
	{
		std::shared_ptr<WMO> wmo = GetManager<IWMOManager>()->Add(m_WMOsNames[it.nameIndex]);
		if (wmo)
		{
			std::shared_ptr<ADT_WMO_Instance> inst = std::make_shared<ADT_WMO_Instance>(wmo, it);
			inst->SetParent(shared_from_this());
			inst->Load();
			m_WMOsInstances.push_back(inst);

			BoundingBox bbox = GetBounds();
			bbox.makeUnion(inst->GetBounds());
			SetBounds(bbox);
		}
	}

	//-- MDXs -------------------------------------------------------------------------
	for (auto& it : m_MDXsPlacementInfo)
	{
		std::shared_ptr<M2> mdx = GetManager<IM2Manager>()->Add(m_MDXsNames[it.nameIndex]);
		if (mdx)
		{
			std::shared_ptr<ADT_MDX_Instance> inst = std::make_shared<ADT_MDX_Instance>(mdx, it);
			inst->SetParent(shared_from_this());
			inst->CreateInstances();
			m_MDXsInstances.push_back(inst);

			BoundingBox bbox = GetBounds();
			bbox.makeUnion(inst->GetBounds());
			SetBounds(bbox);
		}
	}
	//---------------------------------------------------------------------------------

	Log::Green("ADT[%d, %d, %s]: Loaded!", m_IndexX, m_IndexZ, filename);

	return true;
}

bool ADT::Delete()
{
	return true;
}
