#pragma once

#include "WDL_LowResTile.h"
#include "ADT_WMO_Instance.h"

class WDL
{
public:
	WDL(cstring _fileName);
	~WDL();

	void CreateInsances(SceneNode* _parent);

	void Load();

	R_Texture* GetMinimap() { return m_Minimap; }

private:
	const string 						m_FileName;

	SmartTexturePtr						m_Minimap;
	uint32								m_MAREOffsets[C_TilesInMap][C_TilesInMap];
	vector<SmartPtr<CWDL_LowResTile>>	m_LowResilutionTiles;
	vector<string>						m_LowResolutionWMOsNames;
	vector<ADT_MODF>					m_LowResolutionWMOsPlacementInfo;
	vector<SmartPtr<ADT_WMO_Instance>>	m_LowResolutionWMOs;
};