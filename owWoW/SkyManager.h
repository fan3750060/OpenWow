#pragma once

#include "Sky.h"

class SkyManager : public Renderable3DObject
{
public:
	SkyManager(DBC_MapRecord* _mapRecord);
	~SkyManager();

public:
	void Calculate(uint32 _time);
	bool HasSkies() { return !skies.empty(); }

	vec3 GetColor(LightColors _color) { return m_Interpolated.m_InterpolatedColors[_color]; }
	float GetFog(LightFogs _fog) { return m_Interpolated.m_InterpolatedFogs[_fog]; }
	float GetGlow() { return m_Interpolated.m_glow; }
	float GetWaterShallowAlpha() { return m_Interpolated.m_waterShallowAlpha; }
	float GetWaterDarkAlpha() { return m_Interpolated.m_waterDeepAlpha; }
	float GetOceanShallowAlpha() { return m_Interpolated.m_oceanShallowAlpha; }
	float GetOceanDarkAlpha() { return m_Interpolated.m_oceanDeepAlpha; }

	void PreRender3D(double t, double dt) override;
	void Render3D() override;

private:
	void InitBuffer();
	void CalculateSkiesWeights(cvec3 pos);

private:
	SkyParams m_Interpolated;
	R_Buffer* __vb;
	R_GeometryInfo* __geom;
	uint32 __vertsSize;

	vector<Sky*> skies;
	//MDX* stars;  // BOUZI FIXME ENABLE ME
};

