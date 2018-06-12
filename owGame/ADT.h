#pragma once

#include "ADT_MCNK.h"
#include "ADT_MDX_Instance.h"
#include "ADT_WMO_Instance.h"

#include "Map_Headers.h"

class ADT : public SceneNode
{
public:
	ADT(SceneNode* _parent, uint32 _intexX, uint32 _intexZ, string _name, IFile* _file);

	ADT_MCNK* getChunk(int32 x, int32 z)
	{
		assert1(x < C_ChunksInTile && z < C_ChunksInTile);
		return m_Chunks[x * C_ChunksInTile + z];
	}

	// ISceneNode
	string getObjectInfo() override { return m_Name; }

	// ILoadableObject
	bool Load() override;
	bool Delete() override;

	// IRenderable3D
	void PreRender3D() override;
	void Render3D() override;

public:
	const string						m_Name;
	const SmartPtr<IFile>				m_File;
	const int							m_IndexX, m_IndexZ;

	vector<SmartPtr<ADT_TextureInfo>>	m_Textures;

	vector<SmartPtr<ADT_WMO_Instance>>	m_WMOsInstances;
	vector<SmartPtr<ADT_MDX_Instance>>	m_MDXsInstances;

	vector<SmartPtr<ADT_MCNK>>			m_Chunks;

	//
	CGroupQuality& m_QualitySettings;
};
