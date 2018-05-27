#include "stdafx.h"

// General
#include "GameState_Menu.h"

// Additional
#include "GameState_InWorld.h"

bool GameState_Menu::Init()
{
	GameState::Init();

	//

	OpenDBs();

	m_MinimapTexture = new R_Texture(&_Render->r);

	m_MinimapUI = new UIElement(100);
	m_MinimapUI->Init(vec2(200, 0), vec2(768, 768), m_MinimapTexture, COLOR_WHITE);
	m_MinimapUI->Hide();

	_World->EnvM()->globalTime = 0;
	_World->EnvM()->animtime = 0;

	cmd = CMD_NONE;
	backgroundModel = 0;
	randBackground();

	//_Map->PreLoad(DBC_Map[1]);
	//LoadWorld(vec3(17644, 68, 17823));
	//return true;

	unsigned mapsXStart = 10;
	unsigned mapsYStart = 10;
	unsigned mapsYdelta = 20;

	for (auto i = DBC_Map.Records().begin(); i != DBC_Map.Records().end(); ++i)
	{
		auto record = (i->second);
		auto image = new Image(_Render->TexturesMgr()->Add("Interface\\Buttons\\UI-DialogBox-Button-Up.blp"), vec2(), vec2(128, 22));

		// Add btn
		auto btn = new UIButton();


		btn->Init(vec2(100 + 200, mapsYStart), image);


		btn->AttachTo(m_Window);
		btn->ShowText();
		btn->SetText(record->Get_Name());
		SETBUTTONACTION_ARG(btn, GameState_Menu, this, OnBtn, DBC_MapRecord*, record);

		mapsYStart += mapsYdelta;
	}

	//
	enableFreeCamera = false;
	cameraSprint = false;
	//

	return true;
}

void GameState_Menu::Destroy()
{
	GameState::Destroy();
}

//

bool GameState_Menu::Set()
{
	GameState::Set();

	cmd = CMD_NONE;

	return true;
}

void GameState_Menu::Unset()
{
	GameState::Unset();
}

//

void GameState_Menu::Input(double t, double dt)
{
	float speed = 4.5f;

	if (cameraSlow)
		speed *= 0.2f;

	if (cameraSprint)
		speed *= 3.0f;

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_W))
		_Render->mainCamera->ProcessKeyboard(FORWARD, speed);

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_S))
		_Render->mainCamera->ProcessKeyboard(BACKWARD, speed);

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_A))
		_Render->mainCamera->ProcessKeyboard(LEFT, speed);

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_D))
		_Render->mainCamera->ProcessKeyboard(RIGHT, speed);
}

void GameState_Menu::Update(double t, double dt)
{
	_World->EnvM()->animtime += (dt * 1000.0f);
	_World->EnvM()->globalTime = (int)_World->EnvM()->animtime;

	if (backgroundModel)
	{
		backgroundModel->updateEmitters(dt);
	}
}

void GameState_Menu::Render(double t, double dt)
{
	if (backgroundModel != nullptr)
	{
		//_ModelsMgr->resetAnim();

		_Render->rb->setRenderBuffer();
		_Render->r.clear();

		// Camera
		_Pipeline->Clear();
		/*backgroundModel->m_Cameras[0].setup(_EnvironmentManager->globalTime);
		_PipelineGlobal->SetCamera(backgroundModel->m_Cameras[0].GetCamera());
		_PipelineGlobal->SetCameraFrustum(backgroundModel->m_Cameras[0].GetCamera());*/

		_PipelineGlobal->SetCamera(_Render->mainCamera);
		_PipelineGlobal->SetCameraFrustum(_Render->mainCamera);


		// Geom
		backgroundModel->Render();

		// Postprocess pass
		_Render->rb->resetRenderBuffer();
		for (uint32 i = 0; i < 4; i++) _Render->r.setTexture(i, _Render->rb->getRenderBufferTex(i), 0, 0);
		_Render->r.clear(CLR_COLOR_RT0 | CLR_DEPTH);

		// Simple pass
		_Render->TechniquesMgr()->m_POST_Simple->Bind();
		_Render->TechniquesMgr()->m_POST_Simple->SetScreenSize(_Config.windowSizeX, _Config.windowSizeY);

		_Render->r.setDepthTest(false);
		_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_ONE, R_BlendFunc::BS_BLEND_ONE);

		_Render->RenderQuad();

		_Render->r.setBlendMode(false);
		_Render->r.setDepthTest(true);

		_Render->TechniquesMgr()->m_POST_Simple->Unbind();
	}
}

void GameState_Menu::RenderUI()
{
	if (cmd == CMD_SELECT)
	{

		if (_World->Map()->m_WDL.GetMinimap() != 0)
		{
			m_MinimapUI->SetTexture(_World->Map()->m_WDL.GetMinimap());
			m_MinimapUI->Show();
		}
		else
		{
			m_MinimapUI->Hide();
		}

		if (_World->Map()->m_WDT.MapHasTiles())
		{
			_Render->RenderText(vec2(400, 0), "Select your starting point");
		}
		else
		{
			_Render->RenderText(vec2(400, 360), "Click to enter");
		}
	}
}

//

void GameState_Menu::OnBtn(DBC_MapRecord* _e)
{
	Log::Green("Load level %s [%d]", _e->Get_Directory(), _e->Get_ID());

	_World->Map()->PreLoad(_e);
	cmd = CMD_SELECT;

	m_MinimapUI->AttachTo(m_Window);
}

bool GameState_Menu::LoadWorld(vec3 _pos)
{
	_World->Map()->Load();
	_World->Map()->EnterMap(_pos.x / C_TileSize, _pos.z / C_TileSize);
	_World->Map()->PostLoad();

	if (_World->Map()->m_WDT.MapHasGlobalWMO())
	{
		_pos = _World->Map()->m_WDT.GetGlobalWMOPlacementInfo()->position;
	}

	_Render->mainCamera->Position = _pos;
	_Render->mainCamera->Update();

	// Change GameState
	GameStateManager::SetGameState(GameStatesNames::GAME_STATE_WORLD);

	return true;
}

//

#pragma region Input functional

On_Mouse_Moved(GameState_Menu)
{
	if (enableFreeCamera)
	{
		vec2 mouseDelta = (_mousePos - lastMousePos) / _Config.GetWindowSize();

		_Render->mainCamera->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);

		m_Engine->GetAdapter()->SetMousePosition(lastMousePos);
	}
}

On_Mouse_Pressed(GameState_Menu)
{
	// Select point
	if (cmd == CMD_SELECT && _mousePos.x >= 200 && _mousePos.x < 200 + 12 * 64 && _mousePos.y < 12 * 64)
	{
		int selectedPointX = _mousePos.x - 200;
		int selectedPointZ = _mousePos.y;

		vec3 pointInWorld = vec3(selectedPointX / 12.0f, 0.1f, selectedPointZ / 12.0f) * C_TileSize;

		if (backgroundModel != nullptr)
		{
			delete backgroundModel;
			backgroundModel = nullptr;
		}

		LoadWorld(pointInWorld);

		return true;
	}

	return false;
}

On_Mouse_Released(GameState_Menu)
{
	enableFreeCamera = false;
	lastMousePos = vec2();
	m_Engine->GetAdapter()->ShowCursor();
	return true;
}

On_Keyboard_Pressed(GameState_Menu)
{
	if (_key == OW_KEY_ESCAPE)
	{
		if (cmd == CMD_SELECT)
		{
			cmd = CMD_NONE;

			m_MinimapUI->Detach();
			//_UIMgr->Attach(window);
		}
		else
		{
			delete m_Engine;
		}
	}

	if (_key == OW_KEY_KP_1)
	{
		_Config.draw_map_chunk = !_Config.draw_map_chunk;
		return true;
	}
	if (_key == OW_KEY_KP_2)
	{
		_Config.draw_map_wmo = !_Config.draw_map_wmo;
		return true;
	}
	if (_key == OW_KEY_KP_3)
	{
		_Config.draw_map_wmo_doodads = !_Config.draw_map_wmo_doodads;
		return true;
	}
	if (_key == OW_KEY_KP_4)
	{
		_Config.draw_map_mdx = !_Config.draw_map_mdx;
		return true;
	}

	if (_key == OW_KEY_C)
	{
		_Config.Switch(_Config.Quality.Terrain_MCCV);
		return true;
	}

	if (_key == OW_KEY_V)
	{
		_Config.Switch(_Config.Quality.Terrain_MCLV);
		return true;
	}

	if (_key == OW_KEY_F)
	{
		_Config.drawfog = !_Config.drawfog;
		return true;
	}

	return false;
}

#pragma endregion

void GameState_Menu::randBackground()
{
	if (backgroundModel != nullptr)
		delete backgroundModel;

	char* ui[] = { "MainMenu", "NightElf", "Human", "Dwarf", "Orc", "Tauren", "Scourge" };

	char* randui = ui[Random::GenerateMax(7)];
	char path[256];
	sprintf_s(path, "Interface\\Glues\\Models\\UI_%s\\UI_%s.m2", randui, randui);

	/*backgroundModel = new MDX(path);
	backgroundModel->Init();
	backgroundModel->m_IsBillboard = true;*/
}