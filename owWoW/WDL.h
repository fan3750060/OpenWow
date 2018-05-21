#pragma once

#include "ADT_WMO_Instance.h"

class WDL : public Renderable3DObject
{
public:
	WDL();
	~WDL();

	void Load(cstring _name);

	void InitLowResolutionWMOs();

	void PreRender3D(double t, double dt) override;
	void Render3D() override;
	void PostRender3D() override;

	R_Texture* GetMinimap() { return m_Minimap; }

private:
	R_Texture*				   m_Minimap;

	R_GeometryInfo*            m_LowResilutionTiles[C_TilesInMap][C_TilesInMap];

	vector<string>             m_LowResolutionWMOsNames;
	vector<ADT_MODF>		   m_LowResolutionWMOsPlacementInfo;
	vector<ADT_WMO_Instance>   m_LowResolutionWMOs;
};