#pragma once

#include "ADT_MCNK.h"
#include "ADT_MDX_Instance.h"
#include "ADT_WMO_Instance.h"

#include "Map_Headers.h"

// FORWARD BEGIN
class MapController;
// FORWARD END

class ADT : public SceneNode, public ILoadable
{
public:
	ADT(std::weak_ptr<SceneNode> _mapController, uint32 _intexX, uint32 _intexZ);
	virtual ~ADT();

	std::shared_ptr<ADT_MCNK> getChunk(int32 x, int32 z)
	{
		if (x < 0 || x >= C_ChunksInTile || z < 0 || z >= C_ChunksInTile)
		{
			return nullptr;
		}

		return m_Chunks[x * C_ChunksInTile + z];
	}

	// ISceneNode
	//string getObjectInfo() const override { return "@ADT@" + to_string(m_IndexX) + ":" + to_string(m_IndexZ); }
	//void CalculateMatrix(bool _isRotationQuat = false) override { fail1(); };

	// ILoadableObject
	bool Load() override;
	bool Delete() override;
	void setLoaded() {}
	bool isLoaded() const { return true; }

	// IRenderable3D
	bool PreRender3D();
	void Render3D();

public:
	const int							m_IndexX, m_IndexZ;
	ADT_MHDR							m_Header;

	vector<std::shared_ptr<ADT_TextureInfo>>	m_Textures;

	// Instances
#ifdef GAME_MAP_INCLUDE_WMO_AND_M2
	vector<std::shared_ptr<ADT_WMO_Instance>>	m_WMOsInstances;
	vector<std::shared_ptr<ADT_MDX_Instance>>	m_MDXsInstances;
#endif
	vector<std::shared_ptr<Liquid_Instance>>	m_LiquidsInstances;
	vector<std::shared_ptr<ADT_MCNK>>			m_Chunks;

private: // PARENT
	const std::weak_ptr<MapController>			m_MapController;
	CGroupQuality&								m_QualitySettings;
};
