#pragma once

#include "ADT_WMO_Instance.h"

class WDL : public Renderable3DObject
{
public:
	WDL();
	~WDL();

	void Load(cstring _name);

	void InitLowResolutionWMOs();

	R_Texture* GetMinimap() { return m_Minimap; }

	// IRenderable3D
	void PreRender3D(double t, double dt) override;
	void Render3D() override;
	void PostRender3D() override {};

private:
	SmartTexturePtr						m_Minimap;
	SmartGeomPtr						m_LowResilutionTiles[C_TilesInMap][C_TilesInMap];
	vector<string>						m_LowResolutionWMOsNames;
	vector<ADT_MODF>					m_LowResolutionWMOsPlacementInfo;
	vector<SmartPtr<ADT_WMO_Instance>>	m_LowResolutionWMOs;
};