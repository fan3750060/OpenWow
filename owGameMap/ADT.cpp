#include "stdafx.h"

// Include
#include "MapController.h"

// General
#include "ADT.h"

// Additional
#include "ADT_Liquid.h"

ADT::ADT(std::weak_ptr<SceneNode3D> _mapController, uint32 _intexX, uint32 _intexZ) :
	m_MapController(std::dynamic_pointer_cast<MapController>(_mapController.lock())),
	m_IndexX(_intexX), 
	m_IndexZ(_intexZ),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	// Scene node params
	{
		SetTranslate(vec3(_intexX * C_TileSize, 0.0f, _intexZ * C_TileSize));

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
// SceneNode3D
//
void ADT::UpdateLocalTransform()
{
	// do nothing
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

	return SceneNode3D::Accept(visitor);
}

bool ADT::Load()
{
#if (VERSION == VERSION_WotLK)
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
		f->readBytes(&m_Header, sizeof(ADT_MHDR));
	}

	// Chunks info
#if (VERSION == VERSION_WotLK)
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
#endif

	// TextureInfo
	f->seek(startPos + m_Header.MTEX);
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
	std::vector<uint32> m_MDXsOffsets;
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
	std::vector<std::string> m_WMOsNames;
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
	std::vector<uint32> m_WMOsOffsets;
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
	std::vector<ADT_MDXDef> m_MDXsPlacementInfo;
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
	std::vector<ADT_MODF> m_WMOsPlacementInfo;
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
		std::shared_ptr<ADT_MCNK> chunk = std::make_shared<ADT_MCNK>(m_MapController, std::static_pointer_cast<ADT, SceneNode3D>(shared_from_this()), f->Path_Name(), chunks[i]);
		chunk->SetParent(m_MapController);
		Application::Get().GetLoader()->AddToLoadQueue(chunk);
		m_Chunks.push_back(chunk);

		// Update THIS bounds
		//BoundingBox bbox = GetBounds();
		//bbox.makeUnion(chunk->GetBounds());
		//SetBounds(bbox);
	}

	//-- WMOs --------------------------------------------------------------------------

	/*for (auto& it : m_WMOsPlacementInfo)
	{
		std::shared_ptr<ADT_WMO_Instance> inst = std::make_shared<ADT_WMO_Instance>(m_WMOsNames[it.nameIndex], it);
		inst->SetParent(shared_from_this());
		Application::Get().GetLoader()->AddToLoadQueue(inst);
		m_WMOsInstances.push_back(inst);

		// Update THIS bounds
		//BoundingBox bbox = GetBounds();
		//bbox.makeUnion(inst->GetBounds());
		//SetBounds(bbox);
	}

	//-- MDXs -------------------------------------------------------------------------
	for (auto& it : m_MDXsPlacementInfo)
	{
		std::shared_ptr<ADT_MDX_Instance> inst = std::make_shared<ADT_MDX_Instance>(m_MDXsNames[it.nameIndex], it);
		inst->SetParent(shared_from_this());
		Application::Get().GetLoader()->AddToLoadQueue(inst);
		m_MDXsInstances.push_back(inst);

		// Update THIS bounds
		//BoundingBox bbox = GetBounds();
		//bbox.makeUnion(inst->GetBounds());
		//SetBounds(bbox);
	}*/
	//---------------------------------------------------------------------------------

	Log::Green("ADT[%d, %d, %s]: Loaded!", m_IndexX, m_IndexZ, filename);
#endif

	Load_ADT();
	Load_OBJ();
	Load_TEX();

	return true;
}

bool ADT::Delete()
{
	return true;
}





void ADT::Load_ADT()
{
	std::shared_ptr<MapController> mapController = m_MapController.lock();
	assert1(mapController != NULL);

	char filename[256];
	sprintf_s(filename, "%s_%d_%d.adt", mapController->getFilenameT().c_str(), m_IndexX, m_IndexZ);

	std::shared_ptr<IFile> f = GetManager<IFilesManager>()->Open(filename);

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

		if (strcmp(fourcc, "MVER") == 0)
		{
			uint32 version;
			f->readBytes(&version, 4);
			assert1(version == 18);
		}
		else if (strcmp(fourcc, "MHDR") == 0)
		{
			f->readBytes(&header, sizeof(ADT_MHDR));
		}
		else if (strcmp(fourcc, "MH2O") == 0)
		{
			const uint8* abuf = f->getDataFromCurrent();
			for (uint32 i = 0; i < C_ChunksInTile; i++)
			{
				for (uint32 j = 0; j < C_ChunksInTile; j++)
				{
					MH2O_Header* mh2o_Header = (MH2O_Header*)abuf;
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
		else if (strcmp(fourcc, "MCNK") == 0)
		{
			std::shared_ptr<ADT_MCNK> chunk = std::make_shared<ADT_MCNK>(m_MapController, std::static_pointer_cast<ADT, SceneNode3D>(shared_from_this()));
			chunk->SetParent(m_MapController);
			chunk->Load_MCNK_ADT(f, size);
			m_Chunks.push_back(chunk);
		}
		else if (strcmp(fourcc, "MFBO") == 0)
		{
		}
		else
		{
			Log::Fatal("ADT[%s]: No implement '*.adt' chunk [%s] [%d].", filename, fourcc, size);
		}

		f->seek(nextpos);
	}
}

void ADT::Load_OBJ()
{
	std::shared_ptr<MapController> mapController = m_MapController.lock();
	assert1(mapController != NULL);

	char filename[256];
	sprintf_s(filename, "%s_%d_%d_obj%d.adt", mapController->getFilenameT().c_str(), m_IndexX, m_IndexZ, 0);

	std::shared_ptr<IFile> f = GetManager<IFilesManager>()->Open(filename);

	std::vector<std::string> m_MDXsNames;
	std::vector<uint32> m_MDXsOffsets;
	std::vector<std::string> m_WMOsNames;
	std::vector<uint32> m_WMOsOffsets;
	std::vector<ADT_MDXDef> m_MDXsPlacementInfo;
	std::vector<ADT_MODF> m_WMOsPlacementInfo;

	uint32 chunksCntr = 0;

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

		if (strcmp(fourcc, "MVER") == 0)
		{
			uint32 version;
			f->readBytes(&version, 4);
			assert1(version == 18);
		}
		else if (strcmp(fourcc, "MMDX") == 0)
		{
			WOWCHUNK_READ_STRINGS_BEGIN
				m_MDXsNames.push_back(_string);
			WOWCHUNK_READ_STRINGS_END
		}
		else if (strcmp(fourcc, "MMID") == 0)
		{
			uint32 count = size / sizeof(uint32);
			assert1(count == m_MDXsNames.size());
			for (uint32_t i = 0; i < count; i++)
			{
				uint32 offset;
				f->readBytes(&offset, sizeof(uint32));
				m_MDXsOffsets.push_back(offset);
			}
		}
		else if (strcmp(fourcc, "MWMO") == 0)
		{
			WOWCHUNK_READ_STRINGS_BEGIN
				m_WMOsNames.push_back(_string);
			WOWCHUNK_READ_STRINGS_END
		}
		else if (strcmp(fourcc, "MWID") == 0)
		{
			uint32 count = size / sizeof(uint32);
			assert1(count == m_WMOsNames.size());
			for (uint32_t i = 0; i < count; i++)
			{
				uint32 offset;
				f->readBytes(&offset, sizeof(uint32));
				m_WMOsOffsets.push_back(offset);
			}
		}
		else if (strcmp(fourcc, "MDDF") == 0)
		{
			for (uint32 i = 0; i < size / sizeof(ADT_MDXDef); i++)
			{
				ADT_MDXDef placementInfo;
				f->readBytes(&placementInfo, sizeof(ADT_MDXDef));
				m_MDXsPlacementInfo.push_back(placementInfo);
			}
		}
		else if (strcmp(fourcc, "MODF") == 0)
		{
			for (uint32 i = 0; i < size / sizeof(ADT_MODF); i++)
			{
				ADT_MODF placementInfo;
				f->readBytes(&placementInfo, sizeof(ADT_MODF));
				m_WMOsPlacementInfo.push_back(placementInfo);
			}
		}
		else if (strncmp(fourcc, "MCNK", 4) == 0)
		{
			m_Chunks[chunksCntr++]->Load_MCNK_OBJ(f, size);
		}
		else
		{
			Log::Fatal("ADT[%s]: No implement '*_objN.adt' chunk [%s] [%d].", filename, fourcc, size);
		}

		f->seek(nextpos);
	}


	for (auto& it : m_WMOsPlacementInfo)
	{
		std::shared_ptr<ADT_WMO_Instance> inst = std::make_shared<ADT_WMO_Instance>(m_WMOsNames[it.nameIndex], it);
		inst->SetParent(shared_from_this());
		Application::Get().GetLoader()->AddToLoadQueue(inst);
		m_WMOsInstances.push_back(inst);

		// Update THIS bounds
		//BoundingBox bbox = GetBounds();
		//bbox.makeUnion(inst->GetBounds());
		//SetBounds(bbox);
	}

	//-- MDXs -------------------------------------------------------------------------
	for (auto& it : m_MDXsPlacementInfo)
	{
		std::shared_ptr<ADT_MDX_Instance> inst = std::make_shared<ADT_MDX_Instance>(m_MDXsNames[it.nameIndex], it);
		inst->SetParent(shared_from_this());
		Application::Get().GetLoader()->AddToLoadQueue(inst);
		m_MDXsInstances.push_back(inst);

		// Update THIS bounds
		//BoundingBox bbox = GetBounds();
		//bbox.makeUnion(inst->GetBounds());
		//SetBounds(bbox);
	}
}

void ADT::Load_TEX()
{
	std::shared_ptr<MapController> mapController = m_MapController.lock();
	assert1(mapController != NULL);

	char filename[256];
	sprintf_s(filename, "%s_%d_%d_tex%d.adt", mapController->getFilenameT().c_str(), m_IndexX, m_IndexZ, 0);

	std::shared_ptr<IFile> f = GetManager<IFilesManager>()->Open(filename);

	uint32 chunksCntr = 0;

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

		if (strcmp(fourcc, "MVER") == 0)
		{
			uint32 version;
			f->readBytes(&version, 4);
			assert1(version == 18);
		}
		else if (strcmp(fourcc, "MAMP") == 0)
		{
			/*
			struct
			{
			  char fred; // texture_size = 64 / (2^mamp_value). either defined here or in MHDR.mamp_value.
			} mamp;
			*/
		}
		else if (strcmp(fourcc, "MTEX") == 0)
		{
			WOWCHUNK_READ_STRINGS_BEGIN

				std::shared_ptr<ADT_TextureInfo> textureInfo = std::make_shared<ADT_TextureInfo>();
			textureInfo->textureName = _string;

			m_Textures.push_back(textureInfo);

			// PreLoad diffuse texture
			textureInfo->diffuseTexture = _RenderDevice->CreateTexture2D(textureInfo->textureName);

			// PreLoad specular texture
			std::string specularTextureName = textureInfo->textureName;
			specularTextureName = specularTextureName.insert(specularTextureName.length() - 4, "_s");
			textureInfo->specularTexture = _RenderDevice->CreateTexture2D(specularTextureName);

			WOWCHUNK_READ_STRINGS_END
		}
		else if (strncmp(fourcc, "MCNK", 4) == 0)
		{
			/*if (_phase == load_phases::main_file)
			{
				MapChunk* chunk = new MapChunk(this);
				m_Chunks.push_back(chunk);
			}*/

			m_Chunks[chunksCntr++]->Load_MCNK_TEX(f, size);
			m_Chunks[chunksCntr - 1]->setLoaded();
		}
		else if (strncmp(fourcc, "MTXF", 4) == 0)
		{

		}
		else
		{
			Log::Fatal("ADT[%s]: No implement '*_texN.adt' chunk [%s] [%d].", filename, fourcc, size);
		}

		f->seek(nextpos);
	}

	/*for (auto& it : m_Textures)
	{
		// PreLoad diffuse texture
		it->diffuseTexture = _RenderDevice->CreateTexture2D(it->textureName);

		// PreLoad specular texture
		std::string specularTextureName = it->textureName;
		specularTextureName = specularTextureName.insert(specularTextureName.length() - 4, "_s");
		it->specularTexture = _RenderDevice->CreateTexture2D(specularTextureName);
	}*/
}
