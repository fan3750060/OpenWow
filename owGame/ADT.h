#pragma once

#include "ADT_MCNK.h"
#include "ADT_MDX_Instance.h"
#include "ADT_WMO_Instance.h"

#include "Map_Headers.h"

// FORWARD BEGIN
class MapController;
// FORWARD END

class ADT : public SceneNode
{
public:
	ADT(MapController* _mapController, uint32 _intexX, uint32 _intexZ, string _name, IFile* _file);
	~ADT();

	ADT_MCNK* getChunk(int32 x, int32 z)
	{
		if (x < 0 || x >= C_ChunksInTile || z < 0 || z >= C_ChunksInTile)
		{
			return nullptr;
		}

		return m_Chunks[x * C_ChunksInTile + z];
	}

	// ISceneNode
	string getObjectInfo() override { return "@ADT@" + m_Name; }

	// ILoadableObject
	bool Load() override;
	bool Delete() override;

	// IRenderable3D
	bool PreRender3D() override;
	void Render3D() override;

public:
	const MapController*				m_MapController;
	const string						m_Name;
	const SmartPtr<IFile>				m_File;
	const int							m_IndexX, m_IndexZ;
	ADT_MHDR							m_Header;

	vector<SmartPtr<ADT_TextureInfo>>	m_Textures;

	// Instances
	vector<SmartPtr<ADT_WMO_Instance>>	m_WMOsInstances;
	vector<SmartPtr<ADT_MDX_Instance>>	m_MDXsInstances;
	vector<SmartPtr<ADT_MCNK>>			m_Chunks;

	//
	CGroupQuality&						m_QualitySettings;
};
