#pragma once

#include "DayNightCycle.h"
#include "SkyManager.h"

//

class EnvironmentManager : public CRenderable3DObject
{
public:
	EnvironmentManager();
	virtual ~EnvironmentManager();

	void outdoorLighting();
	void SetAmbientLights(bool on);
	void SetFog();

	// IRenderable3D
	bool PreRender3D() override;
	void Render3D() override {}
	void PostRender3D() override {}

public:
	vec4 m_OutdoorAmbientColor;
	vec4 m_OutdoorDayDiffuseColor;
	vec4 m_OutdoorNightDiffuseColor;
	vec4 m_OutdoorSpecularColor;
	
public:
	bool m_HasSky;


	std::shared_ptr<DayNightCycle> dayNightCycle;
	DayNightPhase dayNightPhase;
};