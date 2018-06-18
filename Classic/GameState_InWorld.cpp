#include "stdafx.h"

// General
#include "GameState_InWorld.h"


vec3 fromRealToGame(vec3 p)
{
	return vec3(
		-p.x + C_ZeroPoint,
		-p.z + C_ZeroPoint,
		(p.y)
	);
}

// Y            X        Z			// DBC
//-618.518, -4251.67, 38.718, 0
// X			Y        Z
//-4251.67, -618.518, 38.718, 0

vec3 fromGameToReal(vec3 p)
{
	return vec3(
		-p.x + C_ZeroPoint,
		p.z,
		-p.y + C_ZeroPoint
	);
}


vec3 rotateV(cvec3 v, float theta)
{
	float cs = cosf(theta);
	float sn = sinf(theta);

	float X = v.x * cs - v.z * sn;
	float Z = v.x * sn + v.z * cs;

	vec3 neW = vec3(X, v.y, Z);

	return neW;
}

GameState_InWorld::GameState_InWorld() :
	groupVideo(GetSettingsGroup<CGroupVideo>()),
	groupQuality(GetSettingsGroup<CGroupQuality>())
{
}




bool GameState_InWorld::Init()
{
	CGameState::Init();

	enableFreeCamera = false;
	cameraSprint = false;

	minimapActive = false;

	m_WorldRender = new WorldRender(_World);


	m_TestRenderBuffer = _Render->r.createRenderBuffer(groupVideo.GetWindowSize(), R_TextureFormats::RGBA8, true, 4, 8);
	m_TestRenderBufferFinal = _Render->r.createRenderBuffer(groupVideo.GetWindowSize(), R_TextureFormats::RGBA8, false, 1, 0);
	m_TestCamera = new Camera;
	m_TestCamera->setupViewParams(Math::Pi / 2.0f, groupVideo.aspectRatio, 5.0f, 10000.0f);


	for (auto i = DBC_CinematicSequences.Records().begin(); i != DBC_CinematicSequences.Records().end(); ++i)
	{
		auto record = (i->second);
		Log::Info("[%d] = [%d][%s]", record->Get_ID(), record->Get_CameraRec()->Get_ID(), record->Get_CameraRec()->Get_Filename());
	}

	sceneManager = new CSceneManager(_World->Map());
	sceneManager->setCamera(_Render->getCamera());
	sceneManager->setFrustrumCamera(_Render->getCamera());


	DBC_CinematicCameraRecord* camRec = DBC_CinematicSequences[81]->Get_CameraRec();
	M2* cinematicCamera = GetManager<IM2Manager>()->Add(camRec->Get_Filename());
	m2_Camera = new Single_M2_Instance(nullptr, cinematicCamera);

		/*
		~[2] = [2][Cameras\FlybyUndead.mdx]
		~[21] = [235][Cameras\FlybyOrc.mdx]
		~[41] = [234][Cameras\FlyByDwarf.mdx]
		~[61] = [122][Cameras\FlybyNightElf.mdx]
		~[81] = [142][Cameras\FlyByHuman.mdx]
		~[101] = [162][Cameras\FlyByGnome.mdx]
		~[121] = [182][Cameras\FlyByTroll.mdx]
		~[141] = [202][Cameras\FlyByTauren.mdx]
		*/

	vec3 humans = vec3(-8949.95f, -132.493f, 83.5312f);
	vec3 orcs = vec3(-618.518f, -4251.67f, 38.718);
	vec3 dwarvs = vec3(-6240.32, 331.033, 382.758); // 6.17716
	vec3 nights = vec3(10311.3, 832.463, 1326.41); // 5.69632
	vec3 undeads = vec3(1676.71f, 1678.31f, 121.67); // 2.70526
	vec3 taurens = vec3(-2917.58f, -257.98f, 52.9968f); // 2.70526

	vec3 gPos = vec3(camRec->Get_EndY(), camRec->Get_EndX(), camRec->Get_EndZ());
	m_Rot = camRec->Get_Rotation();
	m_DefPos = fromGameToReal(gPos);
	m_Ended = false;
	m2_Camera->getAnimator()->setOnEndFunction(FUNCTION_CLASS_Builder(GameState_InWorld, this, SwitchOnEnd));
	m2_Camera->getAnimator()->PlayAnimation(0, false);

	_Bindings->RegisterRenderable3DObject(this, 25);
	setVisible(true);


	return true;
}

void GameState_InWorld::Destroy()
{
	CGameState::Destroy();
}

void GameState_InWorld::Input(double _time, double _dTime)
{
	float speed = 4.5f;

	if (cameraSlow)
		speed *= 0.2f;

	if (cameraSprint)
		speed *= 3.0f;

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_W))
		_Render->getCamera()->ProcessKeyboard(FORWARD, speed);

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_S))
		_Render->getCamera()->ProcessKeyboard(BACKWARD, speed);

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_A))
		_Render->getCamera()->ProcessKeyboard(LEFT, speed);

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_D))
		_Render->getCamera()->ProcessKeyboard(RIGHT, speed);
}

void GameState_InWorld::Update(double _time, double _dTime)
{
	if (m_Ended)
	{
		_Render->getCamera()->Update(_time, _dTime);
	}
	else
	{
		m2_Camera->Update(_time, _dTime);

		vec3 pos, tar;
		float fov, nearP, farP;
		m2_Camera->getObject()->m_Cameras[0].getParams(&pos, &tar, &fov, &nearP, &farP);

		pos = m_DefPos + rotateV(pos, 4.52f);
		tar = m_DefPos + rotateV(tar, 4.52f);

		_Render->getCamera()->Position = pos;
		_Render->getCamera()->setupViewParams(fov, m_VideoSettings.aspectRatio, nearP, farP * 10000.0f);
		_Render->getCamera()->setViewMatrix(mat4::lookAtRH(pos, tar, vec3(0.0f, 1.0f, 0.0f)));
	}
}

void GameState_InWorld::Render3D()
{
	SceneNode* intNode = GetManager<ISceneManager>()->getIntersectedNode();
	if (intNode != nullptr)
	{
		_Render->DrawBoundingBox(intNode->getBounds(), vec4(1.0f, 0.2f, 0.2f, 0.8f));
	}

	// tauren 1.58
	// nights 0.95
	// human 4.52

	/*for (uint32 i = m2_Camera->getAnimator()->getStart(); i < m2_Camera->getAnimator()->getEnd(); i += 50)
	{
		m2_Camera->getObject()->m_Cameras[0].calc(i, 0);

		vec3 pos, tar;
		float fov, nearP, farP;
		m2_Camera->getObject()->m_Cameras[0].getParams(&pos, &tar, &fov, &nearP, &farP);

		//pos = m_DefPos + pos;

		pos = m_DefPos + rotateV(pos, 4.52);

		_Render->DrawSphere(mat4(), pos, 1);
	}*/
}

void GameState_InWorld::PostRender3D()
{
	//--

	/*m_TestCamera->Position = _Render->getCamera()->Position + vec3(0.0f, 1, 0.0f) * 300.0f;
	m_TestCamera->Roll = _Render->getCamera()->Roll;
	m_TestCamera->Pitch = -90.0f;
	m_TestCamera->m_NeedUpdate = true;
	m_TestCamera->Update(0, 0);

	m_TestRenderBuffer->setRenderBuffer();
	{
		_Render->r.clear();
		_Render->getTechniquesMgr()->PreRender3D(m_TestCamera, m_TestRenderBuffer);

		ADT_WMO_Instance::reset();
		ADT_MDX_Instance::reset();

		// Render here
		sceneManager->SetRenderQueueDebug(true);
		sceneManager->Render3D();
		sceneManager->SetRenderQueueDebug(false);
	}
	m_TestRenderBuffer->resetRenderBuffer();

	m_TestRenderBufferFinal->setRenderBuffer();
	{
		_Render->r.clear();
		for (uint32 i = 0; i < 4; i++)
		{
			_Render->r.setTexture(i, m_TestRenderBuffer->getRenderBufferTex(i), SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_CLAMP, 0);
		}

		_Render->PostprocessSimple();
	}
	m_TestRenderBufferFinal->resetRenderBuffer();*/
}

void GameState_InWorld::RenderUI()
{
	//if (_World->loading)
	//{
	//	_Render->RenderText(vec2(_Render->GetWindowSize().x / 2, 200), _World->GetMap()->IsOutOfBounds() ? "Out of bounds" : "Loading...");
	//}

	vec2 size = m_TestRenderBufferFinal->getRenderBufferTex(0)->GetSize() / 2.1f;
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

		if (_World->Map()->m_WDL->getMinimap() != 0)
		{
			const int len = 768;

			_Render->RenderTexture(vec2(basex, basey), _World->Map()->m_WDL->getMinimap(), vec2(len, len));

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
	DBÑ_AreaTableRecord* areaRecord = nullptr;
	string areaName = "<unknown>";

	areaRecord = DBÑ_AreaTable[_World->Map()->GetAreaID()];
	if (areaRecord != nullptr)
	{
		areaName = areaRecord->Get_AreaName();
	}

	// Region
	DBÑ_AreaTableRecord* regionRecord = nullptr;
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

	_Render->DrawPerfomance(vec2(5, 100));

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
	_Render->RenderText(vec2(m_VideoSettings.windowSizeX - 150, 0), "TIME [" + to_string(_World->EnvM()->m_GameTime.GetHour()) + "." + to_string(_World->EnvM()->m_GameTime.GetMinute()) + "]");
	char buff[256];

	// Ambient

	sprintf(buff, "Amb[c=[%0.2f %0.2f %0.2f] i=[%f]]",
		_World->EnvM()->dayNightPhase.ambientColor.x, _World->EnvM()->dayNightPhase.ambientColor.y, _World->EnvM()->dayNightPhase.ambientColor.z,
		_World->EnvM()->dayNightPhase.ambientIntensity
	);
	_Render->RenderText(vec2(m_VideoSettings.windowSizeX - 400, 20), buff);

	// Day
	sprintf(buff, "Day[c=[%0.2f %0.2f %0.2f] i=[%f] d=[%0.2f %0.2f %0.2f]]",
		_World->EnvM()->dayNightPhase.dayColor.x, _World->EnvM()->dayNightPhase.dayColor.y, _World->EnvM()->dayNightPhase.dayColor.z,
		_World->EnvM()->dayNightPhase.dayIntensity,
		_World->EnvM()->dayNightPhase.dayDir.x, _World->EnvM()->dayNightPhase.dayDir.y, _World->EnvM()->dayNightPhase.dayDir.z
	);
	_Render->RenderText(vec2(m_VideoSettings.windowSizeX - 400, 40), buff);

	// Night
	sprintf(buff, "Nig[c=[%0.2f %0.2f %0.2f] i=[%f] d=[%0.2f %0.2f %0.2f]]\0",
		_World->EnvM()->dayNightPhase.nightColor.x, _World->EnvM()->dayNightPhase.nightColor.y, _World->EnvM()->dayNightPhase.nightColor.z,
		_World->EnvM()->dayNightPhase.nightIntensity,
		_World->EnvM()->dayNightPhase.nightDir.x, _World->EnvM()->dayNightPhase.nightDir.y, _World->EnvM()->dayNightPhase.nightDir.z
	);
	_Render->RenderText(vec2(m_VideoSettings.windowSizeX - 400, 60), buff);

	// Fog
	sprintf(buff, "Fog[end=[%f] koeff=[%f]]\0",
		_World->EnvM()->m_SkyManager->GetFog(LIGHT_FOG_DISTANCE),
		_World->EnvM()->m_SkyManager->GetFog(LIGHT_FOG_MULTIPLIER)
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
		_Render->RenderRectangle(vec2(xPos, yPos + i * 16), vec2(16.0f, 16.0f), Color(_World->EnvM()->m_SkyManager->GetColor((LightColors)i)));
		_Render->RenderText(vec2(xPos + 20, yPos + i * 16), names[i]);
	}

	sprintf(buff, "Buffer memory [%d] bytes", _Render->r.getBufferMem());
	_Render->RenderText(vec2(m_VideoSettings.windowSizeX - 400, m_VideoSettings.windowSizeY - 40), buff);

	sprintf(buff, "R_Texture memory [%d] bytes", _Render->r.getTextureMem());
	_Render->RenderText(vec2(m_VideoSettings.windowSizeX - 400, m_VideoSettings.windowSizeY - 20), buff);
}

void GameState_InWorld::OnMouseMoved(cvec2 _mousePos)
{
	if (enableFreeCamera)
	{
		vec2 mouseDelta = (_mousePos - lastMousePos) / m_VideoSettings.GetWindowSize();
		_Render->getCamera()->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);
		m_Engine->GetAdapter()->SetMousePosition(lastMousePos);
	}
}

bool GameState_InWorld::OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos)
{
	if (_button == OW_MOUSE_BUTTON_LEFT)
	{
		enableFreeCamera = true;
		lastMousePos = _mousePos;
		m_Engine->GetAdapter()->HideCursor();
		return true;
	}

	return false;
}

bool GameState_InWorld::OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos)
{
	if (_button == OW_MOUSE_BUTTON_LEFT)
	{
		enableFreeCamera = false;
		lastMousePos = vec2();
		m_Engine->GetAdapter()->ShowCursor();
		return true;
	}

	return false;
}

bool GameState_InWorld::OnMouseWheel(int _yoffset)
{
	return false;
}

bool GameState_InWorld::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
	if (_key == OW_KEY_ESCAPE)
	{
		GetManager<IGameStateManager>()->SetGameState(GameStatesNames::GAME_STATE_MENU);
		return true;
	}

	if (_key == OW_KEY_X)
	{
		cameraSprint = true;
		return true;
	}

	if (_key == OW_KEY_Z)
	{
		cameraSlow = true;
		return true;
	}

	if (_key == OW_KEY_M)
	{
		SwitchBool(minimapActive);
		return true;
	}


	return false;
}

bool GameState_InWorld::OnKeyboardReleased(int _key, int _scancode, int _mods)
{
	if (_key == OW_KEY_X)
	{
		cameraSprint = false;
		return true;
	}

	if (_key == OW_KEY_Z)
	{
		cameraSlow = false;
		return true;
	}

	return false;
}