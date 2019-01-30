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

	// SceneNode
	void UpdateLocalTransform() override;
	bool Accept(IVisitor& visitor) override;

	std::shared_ptr<ADT_MCNK> getChunk(int32 x, int32 z)
	{
		if (x < 0 || x >= C_ChunksInTile || z < 0 || z >= C_ChunksInTile)
		{
			return nullptr;
		}

		return m_Chunks[x * C_ChunksInTile + z];
	}

	// ILoadableObject
	bool Load() override;
	bool Delete() override;
	void setLoaded() {}
	bool isLoaded() const { return true; }

public:
	const int										m_IndexX, m_IndexZ;
	
	ADT_MHDR                                        header;
	std::vector<std::shared_ptr<ADT_TextureInfo>>	m_Textures;

	// Instances
	std::vector<std::shared_ptr<ADT_WMO_Instance>>	m_WMOsInstances;
	std::vector<std::shared_ptr<ADT_MDX_Instance>>	m_MDXsInstances;

	std::vector<std::shared_ptr<Liquid_Instance>>	m_LiquidsInstances;
	std::vector<std::shared_ptr<ADT_MCNK>>			m_Chunks;

private: // PARENT
	const std::weak_ptr<MapController>				m_MapController;
	CGroupQuality&									m_QualitySettings;
};
