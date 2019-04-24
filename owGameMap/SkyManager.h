#pragma once

#include "Sky.h"

// FORWARD BEGIN
class CMap;
// FORWARD END

class SkyManager : public SceneNode3D, public ISkyManager
{
public:
	SkyManager();
	virtual ~SkyManager();

	// SceneNode3D
	void UpdateCamera(const Camera* camera) override;

    // ILoadableObject
    bool Load() override;

	// ISkyManager
	void Calculate(const Camera* camera, uint32 _time) override;
	bool HasSkies() const override { return !skies.empty(); }
	vec3  GetColor(LightColors::List _color) const override { return m_Interpolated.m_Colors[_color]; }
	float GetFog(LightFogs::List _fog) const override { return m_Interpolated.m_Fogs[_fog]; }
	float GetGlow() const override { return m_Interpolated.m_glow; }
	float GetWaterShallowAlpha() const override { return m_Interpolated.m_waterShallowAlpha; }
	float GetWaterDarkAlpha() const override { return m_Interpolated.m_waterDeepAlpha; }
	float GetOceanShallowAlpha() const override { return m_Interpolated.m_oceanShallowAlpha; }
	float GetOceanDarkAlpha() const override { return m_Interpolated.m_oceanDeepAlpha; }

protected:
    std::shared_ptr<CMap>                 GetMapController() const;

private:
	void InitBuffer();
	void CalculateSkiesWeights(cvec3 pos);

private:
	SkyParams m_Interpolated;

	std::shared_ptr<IBuffer> colorsBuffer;

	std::vector<std::shared_ptr<Sky>> skies;
};

