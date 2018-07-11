#pragma once

#include "WDL_LowResTile.h"
#include "ADT_WMO_Instance.h"

// FORWARD BEGIN
class MapController;
// FORWARD END

class WDL
{
public:
	WDL(MapController* _mapController);
	~WDL();

	void CreateInsances(SceneNode* _parent);

	void Load();

	R_Texture* getMinimap() { return m_Minimap; }

private:
	const MapController*				m_MapController;

	SharedTexturePtr						m_Minimap;
	uint32								m_MAREOffsets[C_TilesInMap][C_TilesInMap];
	vector<SharedPtr<CWDL_LowResTile>>	m_LowResilutionTiles;
	vector<string>						m_LowResolutionWMOsNames;
	vector<ADT_MODF>					m_LowResolutionWMOsPlacementInfo;
	vector<SharedPtr<ADT_WMO_Instance>>	m_LowResolutionWMOs;
};