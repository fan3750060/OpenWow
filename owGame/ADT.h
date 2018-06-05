#pragma once

#include "ADT_MCNK.h"
#include "ADT_MDX_Instance.h"
#include "ADT_WMO_Instance.h"

struct ADT_TextureInfo : public CRefItem
{
	ADT_TextureInfo() :
		diffuseTexture(nullptr),
		specularTexture(nullptr)
	{}

	//

	string textureName;
	SmartTexturePtr diffuseTexture;
	SmartTexturePtr specularTexture;
};

class ADT : public SceneNode
{
public:
	ADT(uint32 _intexX, uint32 _intexZ, string _name, IFile* _file);

	inline ADT_MCNK* getChunk(int32 x, int32 z)
	{
		assert1(x < C_ChunksInTile && z < C_ChunksInTile);
		return m_Chunks[x * C_ChunksInTile + z];
	}

	// ILoadableObject
	bool Load() override;

public:
	string								m_Name;
	SmartPtr<IFile>						m_File;
	int									m_IndexX, m_IndexZ;

	vector<SmartPtr<ADT_TextureInfo>>	m_Textures;

	vector<SmartPtr<ADT_WMO_Instance>>	m_WMOsInstances;
	vector<SmartPtr<ADT_MDX_Instance>>	m_MDXsInstances;

	vector<SmartPtr<ADT_MCNK>>			m_Chunks;
};
