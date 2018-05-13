#include "stdafx.h"

// General
#include "Map_Tile.h"

// Additional
#include "EnvironmentManager.h"
#include "ModelsManager.h"
#include "WMO_Manager.h"



MapTile::MapTile(uint32 _intexX, uint32 _intexZ) : m_IndexX(_intexX), m_IndexZ(_intexZ)
{
	m_GamePositionX = _intexX * C_TileSize;
	m_GamePositionZ = _intexZ * C_TileSize;
}

MapTile::~MapTile()
{
	Log::Info("MapTile[%d, %d]: Unloading tile...", m_IndexX, m_IndexZ);

	//---------------------------------------------------------------------------------

	ERASE_VECTOR(m_Chunks);

	for (auto it : m_Textures)
	{
		_TexturesMgr->Delete(it.diffuseTexture);
		_TexturesMgr->Delete(it.specularTexture);
	}

	for (auto it : m_WMOsNames)
	{
		_WMOsMgr->Delete(it);
	}
	ERASE_VECTOR(m_WMOsInstances);

	for (auto it : m_MDXsNames)
	{
		_ModelsMgr->Delete(it);
	}
	ERASE_VECTOR(m_MDXsInstances);

	//---------------------------------------------------------------------------------

	Log::Green("MapTile[%d, %d]: Unloaded.", m_IndexX, m_IndexZ);
}

bool MapTile::Load(cstring _filename)
{
	char name[256];
	sprintf_s(name, "World\\Maps\\%s\\%s_%d_%d.adt", _filename.c_str(), _filename.c_str(), m_IndexX, m_IndexZ);

	File f = name;
	if (!f.Open())
	{
		Log::Error("MapTile[%d, %d, %s]: Error open file!", m_IndexX, m_IndexZ, name);
		return false;
	}

	SMChunkInfo chunks[256];

	// MVER + size
	f.SeekRelative(8);
	{
		uint32 version;
		f.ReadBytes(&version, 4);
		assert1(version == 18);
	}

	// MHDR + size
	f.SeekRelative(8);
	uint32_t startPos = f.GetPos();
	{
		f.ReadBytes(&m_Header, sizeof(ADT_MHDR));
	}

	// Chunks info
	f.Seek(startPos + m_Header.MCIN);
	{
		f.SeekRelative(8);
		for (uint32_t i = 0; i < 256; i++)
		{
			f.ReadBytes(&chunks[i], 16);
		}
	}

	// TextureInfo
	f.Seek(startPos + m_Header.MTEX);
	{
		f.SeekRelative(4);
		uint32_t size;
		f.ReadBytes(&size, sizeof(uint32_t));

		WOWCHUNK_READ_STRINGS_BEGIN

			Map_Tile_TextureInfo textureInfo;
		textureInfo.textureName = _string;

		m_Textures.push_back(textureInfo);

		WOWCHUNK_READ_STRINGS_END
	}

	// MDX names
	f.Seek(startPos + m_Header.MMDX);
	{
		/*
		f.SeekRelative(4);
		uint32_t size;
		f.ReadBytes(&size, sizeof(uint32_t));

		WOWCHUNK_READ_STRINGS_BEGIN
			m_MDXsNames.push_back(_string);
		WOWCHUNK_READ_STRINGS_END
		*/
	}

	// MDX Offsets
	f.Seek(startPos + m_Header.MMID);
	{
		/*
		f.SeekRelative(4);
		uint32_t size;
		f.ReadBytes(&size, sizeof(uint32_t));

		assert1(m_MDXsNames.size() == size / sizeof(uint32));
		*/
	}

	// WMO Names
	f.Seek(startPos + m_Header.MWMO);
	{
		f.SeekRelative(4);
		uint32_t size;
		f.ReadBytes(&size, sizeof(uint32_t));

		WOWCHUNK_READ_STRINGS_BEGIN
			m_WMOsNames.push_back(_string);
		WOWCHUNK_READ_STRINGS_END
	}

	// WMO Offsets
	f.Seek(startPos + m_Header.MWID);
	{
		f.SeekRelative(4);
		uint32_t size;
		f.ReadBytes(&size, sizeof(uint32_t));

		assert1(m_WMOsNames.size() == size / sizeof(uint32));
	}

	// MDX PlacementInfo
	f.Seek(startPos + m_Header.MDDF);
	{
		/*
		f.SeekRelative(4);
		uint32_t size;
		f.ReadBytes(&size, sizeof(uint32_t));

		for (uint32 i = 0; i < size / ModelPlacementInfo::__size; i++)
		{
			ModelPlacementInfo placementInfo;
			f.ReadBytes(&placementInfo, ModelPlacementInfo::__size);
			m_MDXsPlacementInfo.push_back(placementInfo);
		}
		*/
	}

	// WMO PlacementInfo
	f.Seek(startPos + m_Header.MODF);
	{
		f.SeekRelative(4);
		uint32_t size;
		f.ReadBytes(&size, sizeof(uint32_t));

		for (uint32 i = 0; i < size / WMOPlacementInfo::__size; i++)
		{
			WMOPlacementInfo placementInfo;
			f.ReadBytes(&placementInfo, WMOPlacementInfo::__size);
			m_WMOsPlacementInfo.push_back(placementInfo);
		}
	}

	//---------------------------------------------------------------------------------

	// Load Chunks
	for (uint32_t i = 0; i < 256; i++)
	{
		f.Seek(chunks[i].offset);

		MapChunk* chunk = new MapChunk(this);
		chunk->Load(f);

		m_Chunks.push_back(chunk);
	}


	//---------------------------------------------------------------------------------

	// Load Textures
	for (auto& it : m_Textures)
	{
		if (it.mtxf.do_not_load_specular_or_height_texture_but_use_cubemap)
		{
			it.diffuseTexture = _TexturesMgr->DefaultTexture();
			it.specularTexture = _TexturesMgr->DefaultTexture();
			continue;
		}

		// Preload diffuse texture
		it.diffuseTexture = _TexturesMgr->Add(it.textureName);

		// Preload specular texture
		string specularTextureName = it.textureName;
		specularTextureName = specularTextureName.insert(specularTextureName.length() - 4, "_s");
		it.specularTexture = _TexturesMgr->Add(specularTextureName);
	}

	//---------------------------------------------------------------------------------

	// WMOs
	for (auto it : m_WMOsPlacementInfo)
	{
		_WMOsMgr->Add(m_WMOsNames[it.nameIndex]);

		WMO* wmo = (WMO*)_WMOsMgr->objects[m_WMOsNames[it.nameIndex]];
		WMOInstance* inst = new WMOInstance(wmo, it);
		m_WMOsInstances.push_back(inst);
	}

	//---------------------------------------------------------------------------------

	// MDXs
	for (auto it : m_MDXsPlacementInfo)
	{
		_ModelsMgr->Add(m_MDXsNames[it.nameId]);

		MDX* mdx = (MDX*)_ModelsMgr->GetItemByName(m_MDXsNames[it.nameId]);
		ModelInstance* inst = new ModelInstance(mdx, it);
		m_MDXsInstances.push_back(inst);
	}

	//---------------------------------------------------------------------------------

	// Chunks
	assert1(m_Chunks.size() == C_ChunksInTileGlobal);
	for (auto it : m_Chunks)
	{
		it->Post_Load();
	}

	//---------------------------------------------------------------------------------

	Log::Green("MapTile[%d, %d, %s]: Loaded!", m_IndexX, m_IndexZ, _filename.c_str());

	return true;
}

//

void MapTile::draw()
{
	for (auto it : m_Chunks)
	{
		it->Render();
	}
}

void MapTile::Render_DEBUG()
{
	for (auto it : m_Chunks)
	{
		it->Render_DEBUG();
	}
}

void MapTile::drawWater()
{
	for (auto it : m_MH2O)
	{
		it->Render();
	}
}

void MapTile::drawObjects()
{
	for (auto it : m_WMOsInstances)
	{
		it->Render();
	}
}

void MapTile::drawSky()
{
	for (auto it : m_WMOsInstances)
	{
		it->GetWMO()->drawSkybox();

		if (_EnvironmentManager->m_HasSky)
		{
			break;
		}
	}
}

void MapTile::drawModels()
{
	for (auto it : m_MDXsInstances)
	{
		it->Render();
	}
}
