#pragma once

#include "WDL_LowResTile.h"
#include "ADT_WMO_Instance.h"

// FORWARD BEGIN
class MapController;
// FORWARD END

class WDL
{
public:
	WDL(std::weak_ptr<MapController> _mapController);
	virtual ~WDL();

	void CreateInsances(std::weak_ptr<SceneNode> _parent);
	void Load();

	std::shared_ptr<Texture> getMinimap() { return m_Minimap; }

private:
	std::shared_ptr<Texture>					    m_Minimap;
	uint32											m_MAREOffsets[C_TilesInMap][C_TilesInMap];
	std::vector<std::shared_ptr<CWDL_LowResTile>>	m_LowResilutionTiles;
	std::vector<std::string>						m_LowResolutionWMOsNames;
	std::vector<ADT_MODF>					        m_LowResolutionWMOsPlacementInfo;
	std::vector<std::shared_ptr<ADT_WMO_Instance>>	m_LowResolutionWMOs;

private: // PARENT
	const std::weak_ptr<MapController>          m_MapController;
};