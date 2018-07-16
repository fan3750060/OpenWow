#include "stdafx.h"

// General
#include "GameState_InWorld.h"

void GameState_InWorld::RenderUI()
{
	vec2 size = m_TestRenderBufferFinal->getRenderBufferTex(0)->GetSize() / 2.5f;
	_Render->RenderTexture
	(
		m_VideoSettings.GetWindowSize() - size,
		m_TestRenderBufferFinal->getRenderBufferTex(0),
		size,
		true
	);



	if (minimapActive)
	{
		int basex = 200;
		int basey = 0;

		if (_World->Map()->getMinimap() != 0)
		{
			const int len = 768;

			_Render->RenderTexture(vec2(basex, basey), _World->Map()->getMinimap(), vec2(len, len));

			// Player position
			/*glBegin(GL_LINES);
			float fx, fz;
			fx = basex + _World->mainCamera->Position.x / C_TileSize * 12.0f;
			fz = basey + _World->mainCamera->Position.z / C_TileSize * 12.0f;
			glVertex2f(fx, fz);
			glColor4f(1, 1, 1, 0);
			glVertex2f(fx + 10.0f * cosf(degToRad(_World->mainCamera->Roll)), fz + 10.0f * sinf(degToRad(_World->mainCamera->Roll)));
			glEnd();*/

			return;
		}
	}

	// HUD
	// Skyname
	//char* sn = _World->m_SkyManager->getSkyName();
	//if(sn)
	//	_Render->RenderText(vec2(200, 0), string(sn));

	// Area and region




	// Area
	DBC_AreaTableRecord* areaRecord = nullptr;
	string areaName = "<unknown>";

	areaRecord = DBC_AreaTable[_World->Map()->GetAreaID()];
	if (areaRecord != nullptr)
	{
		areaName = areaRecord->Get_AreaName();
	}

	// Region
	DBC_AreaTableRecord* regionRecord = nullptr;
	string regionName = "<unknown>";

	if (areaRecord != nullptr)
	{
		regionRecord = areaRecord->Get_ParentAreaNum();
		if (regionRecord != nullptr)
		{
			regionName = regionRecord->Get_AreaName();
		}
	}

	//


	_Render->RenderText(vec2(5, 20), "Area: [" + areaName + "] [Area id = " + std::to_string(_World->Map()->GetAreaID()) + "]");
	_Render->RenderText(vec2(5, 40), "Region: [" + regionName + "]");
	_Render->RenderText(vec2(5, 60), "CURRX: " + to_string(_World->Map()->GetCurrentX()) + ", CURRZ " + to_string(_World->Map()->GetCurrentZ()));


	///



	if (sceneManager->getIntersectedNode() != nullptr)
	{
		_Render->RenderText(vec2(5, m_VideoSettings.windowSizeY - 110), sceneManager->getIntersectedNodeInfo());
	}

	vec3 g = fromRealToGame(_Render->getCamera()->Position);

	_Render->RenderText(vec2(5, m_VideoSettings.windowSizeY - 88), "REAL CamPos: [" + to_string(_Render->getCamera()->Position.x) + "], [" + to_string(_Render->getCamera()->Position.y) + "], [" + to_string(_Render->getCamera()->Position.z) + "]");
	_Render->RenderText(vec2(5, m_VideoSettings.windowSizeY - 66), "CamPos: [" + to_string(g.x) + "], [" + to_string(g.y) + "], [" + to_string(g.z) + "]");
	_Render->RenderText(vec2(5, m_VideoSettings.windowSizeY - 44), "CamDir: [" + to_string(_Render->getCamera()->Direction.x) + "], [" + to_string(_Render->getCamera()->Direction.y) + "], [" + to_string(_Render->getCamera()->Direction.z) + "]");
	_Render->RenderText(vec2(5, m_VideoSettings.windowSizeY - 22), "CamRot: [" + to_string(_Render->getCamera()->Roll) + "], [" + to_string(_Render->getCamera()->Pitch) + "]");

	// Time
	_Render->RenderText(vec2(m_VideoSettings.windowSizeX - 150, 0), "TIME [" + to_string(_World->Map()->getTime()->GetHour()) + "." + to_string(_World->Map()->getTime()->GetMinute()) + "]");
	char buff[256];

	// Ambient

	/*sprintf_s(buff, "Amb[c=[%0.2f %0.2f %0.2f] i=[%f]]",
		_World->EnvM()->dayNightPhase.ambientColor.x, _World->EnvM()->dayNightPhase.ambientColor.y, _World->EnvM()->dayNightPhase.ambientColor.z,
		_World->EnvM()->dayNightPhase.ambientIntensity
	);
	_Render->RenderText(vec2(m_VideoSettings.windowSizeX - 400, 20), buff);

	// Day
	sprintf_s(buff, "Day[c=[%0.2f %0.2f %0.2f] i=[%f] d=[%0.2f %0.2f %0.2f]]",
		_World->EnvM()->dayNightPhase.dayColor.x, _World->EnvM()->dayNightPhase.dayColor.y, _World->EnvM()->dayNightPhase.dayColor.z,
		_World->EnvM()->dayNightPhase.dayIntensity,
		_World->EnvM()->dayNightPhase.dayDir.x, _World->EnvM()->dayNightPhase.dayDir.y, _World->EnvM()->dayNightPhase.dayDir.z
	);
	_Render->RenderText(vec2(m_VideoSettings.windowSizeX - 400, 40), buff);

	// Night
	sprintf_s(buff, "Nig[c=[%0.2f %0.2f %0.2f] i=[%f] d=[%0.2f %0.2f %0.2f]]\0",
		_World->EnvM()->dayNightPhase.nightColor.x, _World->EnvM()->dayNightPhase.nightColor.y, _World->EnvM()->dayNightPhase.nightColor.z,
		_World->EnvM()->dayNightPhase.nightIntensity,
		_World->EnvM()->dayNightPhase.nightDir.x, _World->EnvM()->dayNightPhase.nightDir.y, _World->EnvM()->dayNightPhase.nightDir.z
	);
	_Render->RenderText(vec2(m_VideoSettings.windowSizeX - 400, 60), buff);*/

	// Fog
	sprintf_s(buff, "Fog[end=[%f] koeff=[%f]]\0",
		_World->Map()->getSkyManager()->GetFog(LightFogs::LIGHT_FOG_DISTANCE),
		_World->Map()->getSkyManager()->GetFog(LightFogs::LIGHT_FOG_MULTIPLIER)
	);
	_Render->RenderText(vec2(m_VideoSettings.windowSizeX - 400, 80), buff);

	// Colors
	float xPos = m_VideoSettings.windowSizeX - 400;
	float yPos = 100;
	const char* names[18] =
	{
		"LIGHT_COLOR_GLOBAL_DIFFUSE" ,
		"LIGHT_COLOR_GLOBAL_AMBIENT",

		"LIGHT_COLOR_SKY_0",
		"LIGHT_COLOR_SKY_1",
		"LIGHT_COLOR_SKY_2",
		"LIGHT_COLOR_SKY_3",
		"LIGHT_COLOR_SKY_4",

		"LIGHT_COLOR_FOG",

		"LIGHT_COLOR_UNK0",

		"LIGHT_COLOR_SUN",
		"LIGHT_COLOR_SUN_HALO",

		"LIGHT_COLOR_UNK1",

		"LIGHT_COLOR_CLOUD",

		"LIGHT_COLOR_UNK2",

		"LIGHT_COLOR_OCEAN_LIGHT",
		"LIGHT_COLOR_OCEAN_DARK",

		"LIGHT_COLOR_RIVER_LIGHT",
		"LIGHT_COLOR_RIVER_DARK"
	};
	for (uint8 i = 0; i < 18; i++)
	{
		_Render->RenderRectangle(vec2(xPos, yPos + i * 16), vec2(16.0f, 16.0f), Color(_World->Map()->getSkyManager()->GetColor((LightColors::List)i)));
		_Render->RenderText(vec2(xPos + 20, yPos + i * 16), names[i]);
	}

	sprintf_s(buff, "Buffer memory [%s]", _Render->r.getBufferMemStr().c_str());
	_Render->RenderText(vec2(m_VideoSettings.windowSizeX - 400, m_VideoSettings.windowSizeY - 40), buff);

	sprintf_s(buff, "R_Texture memory [%s]", _Render->r.getTextureMemStr().c_str());
	_Render->RenderText(vec2(m_VideoSettings.windowSizeX - 400, m_VideoSettings.windowSizeY - 20), buff);

	/*if (_World->Map()->dir != nullptr)
	{
		_World->Map()->dir->Render(_Render->getCamera()->Position);
	}*/
}