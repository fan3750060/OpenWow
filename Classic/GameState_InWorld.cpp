#include "stdafx.h"

// General
#include "GameState_InWorld.h"


bool GameState_InWorld::Init()
{
    CGameState::Init();

    enableFreeCamera = false;
    cameraSprint = false;

    minimapActive = false;

	m_WorldRender = new WorldRender(_World);

	sceneManager = new CSceneManager(_World->Map());
	sceneManager->setCamera(_Camera);
	sceneManager->setFrustrumCamera(_CameraFrustum);

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
        _Render->mainCamera->ProcessKeyboard(FORWARD, speed);

    if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_S))
        _Render->mainCamera->ProcessKeyboard(BACKWARD, speed);

    if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_A))
        _Render->mainCamera->ProcessKeyboard(LEFT, speed);

    if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_D))
        _Render->mainCamera->ProcessKeyboard(RIGHT, speed);
}

void GameState_InWorld::Update(double _time, double _dTime)
{

}

void GameState_InWorld::Render3D()
{
	SceneNode* intNode = GetManager<ISceneManager>()->getIntersectedNode();
	if (intNode != nullptr)
	{
		_Render->DrawBoundingBox(intNode->getBounds(), vec4(1.0f, 0.2f, 0.2f, 0.8f));
	}
}

void GameState_InWorld::RenderUI()
{
    //if (_World->loading)
    //{
    //	_Render->RenderText(vec2(_Render->GetWindowSize().x / 2, 200), _World->GetMap()->IsOutOfBounds() ? "Out of bounds" : "Loading...");
    //}

    if (minimapActive)
    {
        int basex = 200;
        int basey = 0;

        if (_World->Map()->m_WDL->GetMinimap() != 0)
        {
            const int len = 768;

            _Render->RenderTexture(vec2(basex, basey), _World->Map()->m_WDL->GetMinimap(), vec2(len, len));

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
        areaName = areaRecord->Get_Name();
    }

    // Region
    DBÑ_AreaTableRecord* regionRecord = nullptr;
    string regionName = "<unknown>";

    if (areaRecord != nullptr)
    {
        regionRecord = areaRecord->Get_ParentAreaNum();
        if (regionRecord != nullptr)
        {
            regionName = regionRecord->Get_Name();
        }
    }


    //
    // DEBUG
    //
    _Render->RenderTexture(vec2(m_VideoSettings.windowSizeX * 2.0 / 3.0, m_VideoSettings.windowSizeY * 2.0 / 3.0), m_WorldRender->GetTestRB()->getRenderBufferTex(2), vec2(m_VideoSettings.windowSizeX / 3, m_VideoSettings.windowSizeY / 3));


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
    _Render->RenderText(vec2(5, m_VideoSettings.windowSizeY - 66), "REAL CamPos: [" + to_string(_Render->mainCamera->Position.x) + "], [" + to_string(_Render->mainCamera->Position.y) + "], [" + to_string(_Render->mainCamera->Position.z) + "]");
    _Render->RenderText(vec2(5, m_VideoSettings.windowSizeY - 44), "CamPos: [" + to_string(-(_Render->mainCamera->Position.x - C_ZeroPoint)) + "], [" + to_string(-(_Render->mainCamera->Position.z - C_ZeroPoint)) + "], [" + to_string(_Render->mainCamera->Position.y) + "]");
    _Render->RenderText(vec2(5, m_VideoSettings.windowSizeY - 22), "CamRot: [" + to_string(_Render->mainCamera->Direction.x) + "], [" + to_string(_Render->mainCamera->Direction.y) + "], [" + to_string(_Render->mainCamera->Direction.z) + "]");

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
        _Render->RenderRectangle(vec2(xPos,      yPos + i * 16), vec2(16.0f, 16.0f), Color(_World->EnvM()->m_SkyManager->GetColor((LightColors)i)));
        _Render->RenderText(     vec2(xPos + 20, yPos + i * 16), names[i]);
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
        _Render->mainCamera->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);
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