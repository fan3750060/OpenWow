#include "stdafx.h"

// General
#include "EnvironmentManager.h"

// Additiobal
#include <ctime>

EnvironmentManager::EnvironmentManager() :
	m_DistancesSettings(GetSettingsGroup<CGroupDistances>()),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
    /*time_t t = time(0);   // get time now
    tm* now = localtime(&t);
	m_GameTime.Set(now->tm_hour, now->tm_min);*/
    m_GameTime.Set(11, 0);

	m_SkyManager = 0;
	dayNightCycle = new DayNightCycle();

	// Colors
    m_OutdoorAmbientColor = vec4();
	m_OutdoorDayDiffuseColor = vec4();
	m_OutdoorNightDiffuseColor = vec4();
	m_OutdoorSpecularColor = vec4();

	_Bindings->RegisterUpdatableObject(this);
	_Bindings->RegisterRenderable3DObject(this, 5);
}

EnvironmentManager::~EnvironmentManager()
{
	_Bindings->UnregisterRenderable3DObject(this);
	_Bindings->UnregisterUpdatableObject(this);
}

void EnvironmentManager::Update(double _Time, double _deltaTime)
{
}

void EnvironmentManager::PreRender3D()
{
	if (m_QualitySettings.timeEnable)
	{
		m_GameTime.Tick();
	}

	m_HasSky = false;

	dayNightPhase = dayNightCycle->getPhase(m_GameTime.GetTime());

	if (m_SkyManager != nullptr)
	{
		m_SkyManager->Calculate(m_GameTime.GetTime());
	}
}

void EnvironmentManager::InitSkies(DBC_MapRecord* _mapRecord)
{
	m_SkyManager = new SkyManager(_mapRecord);
}

void EnvironmentManager::outdoorLighting()
{
	m_OutdoorAmbientColor = vec4(m_SkyManager->GetColor(LIGHT_COLOR_GLOBAL_AMBIENT), 1.0f); // BLACK?

	m_OutdoorDayDiffuseColor = vec4(m_SkyManager->GetColor(LIGHT_COLOR_GLOBAL_DIFFUSE) * dayNightPhase.dayIntensity, 1.0f);
	m_OutdoorNightDiffuseColor = vec4(m_SkyManager->GetColor(LIGHT_COLOR_GLOBAL_DIFFUSE) * dayNightPhase.nightIntensity, 1.0f);

	m_OutdoorSpecularColor = vec4(1.4f, 1.4f, 1.4f, 1.0f);
}

void EnvironmentManager::SetAmbientLights(bool on)
{
	if (on)
	{
		vec4 ambient(m_SkyManager->GetColor(LIGHT_COLOR_GLOBAL_AMBIENT), 1);
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

		if (dayNightPhase.dayIntensity > 0)
		{
			//glEnable(GL_LIGHT0);
		}
		else
		{
			//glDisable(GL_LIGHT0);
		}

		if (dayNightPhase.nightIntensity > 0)
		{
			//glEnable(GL_LIGHT1);
		}
		else
		{
			//glDisable(GL_LIGHT1);
		}
	}
	else
	{
		vec4 ambient(0, 0, 0, 1);
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
		//glDisable(GL_LIGHT0);
		//glDisable(GL_LIGHT1);
	}
}

void EnvironmentManager::SetFog()
{
	if (m_QualitySettings.drawfog)
	{
		float fogdist = m_DistancesSettings.fogdistance;
		float fogstart = 0.5f;

		m_DistancesSettings.culldistance = fogdist;

		vec4 fogcolor(m_SkyManager->GetColor(LIGHT_COLOR_FOG), 1);
		//glFogfv(GL_FOG_COLOR, fogcolor); // TODO: retreive fogstart and fogend from lights.lit somehow
		//glFogf(GL_FOG_START, fogdist * fogstart);
		//glFogf(GL_FOG_END, fogdist);
		//glEnable(GL_FOG);
	}
	else
	{
		//glDisable(GL_FOG);
		m_DistancesSettings.culldistance = m_DistancesSettings.mapdrawdistance;
	}
}

