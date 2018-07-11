#pragma once

#include "DayNightCycle.h"
#include "SkyManager.h"

//

class EnvironmentManager : public CRenderable3DObject, public IUpdatable
{
public:
	EnvironmentManager();
	virtual ~EnvironmentManager();

	void InitSkies(DBC_MapRecord _mapRecord);
	void outdoorLighting();
	void SetAmbientLights(bool on);
	void SetFog();

	// IUpdatable
	void Input(CInput* _input, double _time, double _dTime) override {}
	void Update(double _Time, double _deltaTime) override;

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
	WowTime m_GameTime;


	SkyManager* m_SkyManager;

	SharedPtr<DayNightCycle> dayNightCycle;
	DayNightPhase dayNightPhase;

	CGroupQuality& m_QualitySettings;
};