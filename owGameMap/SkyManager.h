#pragma once

#include "Sky.h"

// FORWARD BEGIN
class MapController;
// FORWARD END

class SkyManager : public SceneNodeModel3D, public ISkyManager
{
public:
	SkyManager(std::weak_ptr<MapController> _mapController, DBC_MapRecord _mapRecord);
	virtual ~SkyManager();

public:
	// SceneNodeModel3D
	void UpdateCamera(const Camera* camera) override;

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

private:
	void InitBuffer();
	void CalculateSkiesWeights(cvec3 pos);

private:
	SkyParams m_Interpolated;

	std::shared_ptr<IBuffer> colorsBuffer;

	std::vector<std::shared_ptr<Sky>> skies;

private: // PARENT
	std::weak_ptr<MapController> m_MapController;
};

