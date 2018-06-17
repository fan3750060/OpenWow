#include "stdafx.h"

// Include
#include "MapController.h"

// General
#include "ADT.h"

// Additional
#include "WorldController.h"

#include __PACK_BEGIN

struct ADT_MHDR
{
	uint32 unk0;

	uint32 MCIN;
	uint32 MTEX;
	uint32 MMDX;
	uint32 MMID;
	uint32 MWMO;
	uint32 MWID;
	uint32 MDDF;
	uint32 MODF;

	uint8 unk1[28];
};

struct ADT_MCIN
{
	uint32_t offset;               // absolute offset.
	uint32_t size;                 // the size of the MCNK chunk, this is refering to.
	uint32_t flags;                // always 0. only set in the client., FLAG_LOADED = 1
	uint32_t asyncId;
};

#include __PACK_END

ADT::ADT(MapController* _mapController, uint32 _intexX, uint32 _intexZ, string _name, IFile* _file) :
	SceneNode(_mapController),
	m_MapController(_mapController),
	m_IndexX(_intexX), 
	m_IndexZ(_intexZ),
	m_Name(_name),
	m_File(_file),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	// Scene node params
	{
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

bool ADT::Load()
{
	SmartPtr<IFile> f = m_File;

	uint32_t startPos = f->GetPos() + 20;
	ADT_MCIN chunks[256];

	// MVER + size (8)
	f->SeekRelative(8);
	{
		uint32 version;
		f->ReadBytes(&version, 4);
		assert1(version == 18);
	}

	// MHDR + size (8)
	ADT_MHDR m_Header;
	f->SeekRelative(8);
	{
		f->ReadBytes(&m_Header, sizeof(ADT_MHDR));
	}

	// Chunks info
	f->Seek(startPos + m_Header.MCIN);
	{
		f->SeekRelative(4);
		uint32_t size;
		f->ReadBytes(&size, sizeof(uint32_t));

		uint32 count = size / sizeof(ADT_MCIN);
		assert1(count == C_ChunksInTileGlobal);
		for (uint32_t i = 0; i < count; i++)
		{
			f->ReadBytes(&chunks[i], sizeof(ADT_MCIN));
		}
	}

	// TextureInfo
	f->Seek(startPos + m_Header.MTEX);
	{
		f->SeekRelative(4);
		uint32_t size;
		f->ReadBytes(&size, sizeof(uint32_t));

		WOWCHUNK_READ_STRINGS_BEGIN

		SmartPtr<ADT_TextureInfo> textureInfo = new ADT_TextureInfo();
		textureInfo->textureName = _string;

		m_Textures.push_back(textureInfo);

		//Log::Info("ADT[]: Texture [%s] added", textureInfo->textureName.c_str());

		WOWCHUNK_READ_STRINGS_END
	}

	// M2 names
	vector<string> m_MDXsNames;
	f->Seek(startPos + m_Header.MMDX);
	{
		f->SeekRelative(4);
		uint32_t size;
		f->ReadBytes(&size, sizeof(uint32_t));

		WOWCHUNK_READ_STRINGS_BEGIN
			m_MDXsNames.push_back(_string);
		WOWCHUNK_READ_STRINGS_END
		
	}

	// M2 Offsets
	vector<uint32> m_MDXsOffsets;
	f->Seek(startPos + m_Header.MMID);
	{
		f->SeekRelative(4);
		uint32_t size;
		f->ReadBytes(&size, sizeof(uint32_t));

		uint32 count = size / sizeof(uint32);
		assert1(count == m_MDXsNames.size());
		for (uint32_t i = 0; i < count; i++)
		{
			uint32 offset;
			f->ReadBytes(&offset, sizeof(uint32));
			m_MDXsOffsets.push_back(offset);
		}
	}

	// WMO Names
	vector<string> m_WMOsNames;
	f->Seek(startPos + m_Header.MWMO);
	{
		f->SeekRelative(4);
		uint32_t size;
		f->ReadBytes(&size, sizeof(uint32_t));

		WOWCHUNK_READ_STRINGS_BEGIN
			m_WMOsNames.push_back(_string);
		WOWCHUNK_READ_STRINGS_END
	}

	// WMO Offsets
	vector<uint32> m_WMOsOffsets;
	f->Seek(startPos + m_Header.MWID);
	{
		f->SeekRelative(4);
		uint32_t size;
		f->ReadBytes(&size, sizeof(uint32_t));

		uint32 count = size / sizeof(uint32);
		assert1(count == m_WMOsNames.size());
		for (uint32_t i = 0; i < count; i++)
		{
			uint32 offset;
			f->ReadBytes(&offset, sizeof(uint32));
			m_WMOsOffsets.push_back(offset);
		}
	}

	// M2 PlacementInfo
	vector<ADT_MDDF> m_MDXsPlacementInfo;
	f->Seek(startPos + m_Header.MDDF);
	{
		f->SeekRelative(4);
		uint32_t size;
		f->ReadBytes(&size, sizeof(uint32_t));

		for (uint32 i = 0; i < size / sizeof(ADT_MDDF); i++)
		{
			ADT_MDDF placementInfo;
			f->ReadBytes(&placementInfo, sizeof(ADT_MDDF));
			m_MDXsPlacementInfo.push_back(placementInfo);
		}
	}

	// WMO PlacementInfo
	vector<ADT_MODF> m_WMOsPlacementInfo;
	f->Seek(startPos + m_Header.MODF);
	{
		f->SeekRelative(4);
		uint32_t size;
		f->ReadBytes(&size, sizeof(uint32_t));

		for (uint32 i = 0; i < size / sizeof(ADT_MODF); i++)
		{
			ADT_MODF placementInfo;
			f->ReadBytes(&placementInfo, sizeof(ADT_MODF));
			m_WMOsPlacementInfo.push_back(placementInfo);
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
		f->Seek(chunks[i].offset);

		// Chunk + size (8)
		f->SeekRelative(4);
		uint32_t size;
		f->ReadBytes(&size, sizeof(uint32_t));
		assert1(size + 8 == chunks[i].size);

		SmartPtr<ADT_MCNK> chunk = new ADT_MCNK(this, f);
		chunk->Load();
		m_Chunks.push_back(chunk);

		m_Bounds.makeUnion(chunk->getBounds());
	}

	//-- WMOs --------------------------------------------------------------------------

	for (auto& it : m_WMOsPlacementInfo)
	{
		WMO* wmo = (WMO*)GetManager<IWMOManager>()->Add(m_WMOsNames[it.nameIndex]);
		if (wmo)
		{
			ADT_WMO_Instance* inst = new ADT_WMO_Instance(this, wmo, it);
			m_WMOsInstances.push_back(inst);
			m_Bounds.makeUnion(inst->getBounds());
		}
	}
	
	//-- MDXs -------------------------------------------------------------------------

	/*for (auto& it : m_MDXsPlacementInfo)
	{
		M2* mdx = (M2*)GetManager<IM2Manager>()->Add(m_MDXsNames[it.nameIndex]);
		if (mdx)
		{
			ADT_MDX_Instance* inst = new ADT_MDX_Instance(this, mdx, it);
			m_MDXsInstances.push_back(inst);
			m_Bounds.makeUnion(inst->getBounds());
		}
	}*/

	//---------------------------------------------------------------------------------

	Log::Green("ADT[%d, %d, %s]: Loaded!", m_IndexX, m_IndexZ, m_Name.c_str());

	return SceneNode::Load();
}

bool ADT::Delete()
{
	return SceneNode::Delete();
}

void ADT::PreRender3D()
{
	setVisible(false);

	if (!m_MapController->getTileIsCurrent(m_IndexX, m_IndexZ))
	{
		return;
	}

	// Check frustrum
	if (_Render->getCamera()->_frustum.cullBox(m_Bounds))
	{
		return;
	}

	setVisible(true);
}

void ADT::Render3D()
{
	//_Render->DrawBoundingBox(m_Bounds);
}
