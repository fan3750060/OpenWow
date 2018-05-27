#pragma once

#include "DayNightCycle.h"
#include "SkyManager.h"

//

class EnvironmentManager : public Renderable3DObject, public UpdatableObject
{
public:
	EnvironmentManager();
	~EnvironmentManager();

	//-- UpdatableObject
	void Update(double _Time, double _deltaTime) override;

	//-- RenderableObject
	void PreRender3D(double t, double dt) override;

	//
	void InitSkies(DBC_MapRecord* _mapRecord);
	void outdoorLighting();
	void SetAmbientLights(bool on);
	void SetFog();

public:
	vec4 m_OutdoorAmbientColor;
	vec4 m_OutdoorDayDiffuseColor;
	vec4 m_OutdoorNightDiffuseColor;
	vec4 m_OutdoorSpecularColor;
	

public:
	float animtime;
	int globalTime;


	bool m_HasSky;
	WowTime m_GameTime;


	SkyManager* skies;

	DayNightCycle* dayNightCycle;
	DayNightPhase dayNightPhase;
};