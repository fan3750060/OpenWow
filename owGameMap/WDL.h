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

	SharedTexturePtr getMinimap() { return m_Minimap; }

private:
	SharedTexturePtr					        m_Minimap;
	uint32								        m_MAREOffsets[C_TilesInMap][C_TilesInMap];
	vector<std::shared_ptr<CWDL_LowResTile>>	m_LowResilutionTiles;
	vector<string>						        m_LowResolutionWMOsNames;
	vector<ADT_MODF>					        m_LowResolutionWMOsPlacementInfo;
#ifdef GAME_MAP_INCLUDE_WMO_AND_M2
	vector<std::shared_ptr<ADT_WMO_Instance>>	m_LowResolutionWMOs;
#endif

private: // PARENT
	const std::weak_ptr<MapController>          m_MapController;
};