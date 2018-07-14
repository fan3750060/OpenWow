#pragma once

#include "Sky.h"

// FORWARD BEGIN
class MapController;
// FORWARD END

class SkyManager : public CRenderable3DObject, public ISkyManager
{
public:
	SkyManager(MapController* _mapController, DBC_MapRecord _mapRecord);
	virtual ~SkyManager();

public:
	// ISkyManager
	void Calculate(uint32 _time) override;
	bool HasSkies() const override { return !skies.empty(); }

	vec3  GetColor(LightColors::List _color) const override { return m_Interpolated.m_Colors[_color]; }
	float GetFog(LightFogs::List _fog) const override { return m_Interpolated.m_Fogs[_fog]; }
	float GetGlow() const override { return m_Interpolated.m_glow; }
	float GetWaterShallowAlpha() const override { return m_Interpolated.m_waterShallowAlpha; }
	float GetWaterDarkAlpha() const override { return m_Interpolated.m_waterDeepAlpha; }
	float GetOceanShallowAlpha() const override { return m_Interpolated.m_oceanShallowAlpha; }
	float GetOceanDarkAlpha() const override { return m_Interpolated.m_oceanDeepAlpha; }

	// IRenderable3D
	bool PreRender3D() override;
	void Render3D() override;
	void PostRender3D() override {};
	bool DEBUG_Render();

private:
	void InitBuffer();
	void CalculateSkiesWeights(cvec3 pos);

private:
	SkyParams m_Interpolated;

	SharedBufferPtr colorsBuffer;
	SharedGeomPtr __geom;
	uint32 __vertsSize;

	vector<Sky*> skies;

private: // PARENT
	MapController* m_MapController;
};

