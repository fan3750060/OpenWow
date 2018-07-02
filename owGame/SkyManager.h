#pragma once

#include "Sky.h"

class SkyManager : public ISkyManager, public CRenderable3DObject
{
public:
	SkyManager(DBC_MapRecord _mapRecord);
	~SkyManager();

public:
	// ISkyManager
	void Calculate(uint32 _time);
	bool HasSkies() { return !skies.empty(); }

	vec3 GetColor(LightColors _color) override { return m_Interpolated.m_InterpolatedColors[_color]; }
	float GetFog(LightFogs _fog) override { return m_Interpolated.m_InterpolatedFogs[_fog]; }
	float GetGlow() override { return m_Interpolated.m_glow; }
	float GetWaterShallowAlpha() override { return m_Interpolated.m_waterShallowAlpha; }
	float GetWaterDarkAlpha() override { return m_Interpolated.m_waterDeepAlpha; }
	float GetOceanShallowAlpha() override { return m_Interpolated.m_oceanShallowAlpha; }
	float GetOceanDarkAlpha() override { return m_Interpolated.m_oceanDeepAlpha; }

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
	SmartBufferPtr __vb;
	SmartGeomPtr __geom;
	uint32 __vertsSize;

	vector<SmartPtr<Sky>> skies;
	//M2* stars;  // BOUZI FIXME ENABLE ME
};

