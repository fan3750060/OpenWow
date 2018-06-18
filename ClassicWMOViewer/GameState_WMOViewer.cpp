#include "stdafx.h"

// General
#include "GameState_WMOViewer.h"

GameState_WMOViewer::GameState_WMOViewer() :
	animtime(0),
	globalTime(0),
	groupVideo(GetSettingsGroup<CGroupVideo>()),
	groupQuality(GetSettingsGroup<CGroupQuality>())
{

}

void GameState_WMOViewer::CreateDebugGeom()
{
	vector<vec3> vecrtices;
	vecrtices.push_back(vec3(-100, 0, 100));
	vecrtices.push_back(vec3(-100, 0, -100));
	vecrtices.push_back(vec3(100, 0, -100));

	vecrtices.push_back(vec3(-100, 0, 100));
	vecrtices.push_back(vec3(100, 0, -100));
	vecrtices.push_back(vec3(100, 0, 100));

	// Vertex buffer
	R_Buffer* __vb = _Render->r.createVertexBuffer(vecrtices.size() * sizeof(vec3), vecrtices.data());
	m_DebugGeom = _Render->r.beginCreatingGeometry(_Render->getRenderStorage()->__layout_GxVBF_P);
	m_DebugGeom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 0, 0);
	m_DebugGeom->finishCreatingGeometry();
}

bool GameState_WMOViewer::Init()
{
	CGameState::Init();

	//

	OpenDBs();

	new CM2_Manager();
	new WMOsManager(m_Engine->GetAdapter());

	CreateDebugGeom();

	_Render->getCamera()->Position = vec3(0, 0, 0);
	_Render->getCamera()->SetNeedUpdate();

	m_TestRenderBuffer = _Render->r.createRenderBuffer(groupVideo.GetWindowSize(), R_TextureFormats::RGBA8, true, 4, 0);
	m_TestRenderBufferFinal = _Render->r.createRenderBuffer(groupVideo.GetWindowSize(), R_TextureFormats::RGBA8, false, 1, 0);
	m_TestCamera = new Camera;
	m_TestCamera->setupViewParams(Math::Pi / 4.0f, groupVideo.aspectRatio, 2.0f, 2000.0f);

	SceneNode* root = new SceneNode();
	root->setVisible(true);


	if (m_Engine->GetArgumentsCount() > 1)
	{
		string path = m_Engine->GetArgument(1);
		int pos = path.find("ExData");
		assert1(pos != -1);
		path = path.substr(pos + 7);

		WMO* wmo = GetManager<IWMOManager>()->Add(path);
		backgroundModel = new Single_WMO_Instance(root, wmo);

	}
	else
	{
		//WMO* wmo = GetManager<IWMOManager>()->Add("World\\wmo\\Dungeon\\AZ_StormwindPrisons\\StormwindJail.wmo");
		WMO* wmo = GetManager<IWMOManager>()->Add("World\\wmo\\Azeroth\\Buildings\\Stormwind\\Stormwind.wmo");
		//WMO* wmo = GetManager<IWMOManager>()->Add("World\\wmo\\Kalimdor\\Ogrimmar\\Ogrimmar.wmo");
		//WMO* wmo = GetManager<IWMOManager>()->Add("World\\wmo\\Kalimdor\\Winterspring\\MD_WinterspringCave01.wmo");
		new Single_WMO_Instance(root, wmo);
	}


	sceneManager = new CSceneManager(root);
	sceneManager->setCamera(_Render->getCamera());
	sceneManager->setFrustrumCamera(_Render->getCamera());

	enableFreeCamera = false;
	cameraSprint = false;

	return true;
}

void GameState_WMOViewer::Destroy()
{
	CGameState::Destroy();
}

bool GameState_WMOViewer::Set()
{
	CGameState::Set();

	_Bindings->RegisterRenderable3DObject(this, 15);

	return true;
}

void GameState_WMOViewer::Unset()
{
	CGameState::Unset();

	_Bindings->UnregisterRenderable3DObject(this);
}

//

void GameState_WMOViewer::Input(double _time, double _dTime)
{
	float speed = 3.0f;

	if (cameraSlow)
		speed *= 0.1f;

	if (cameraSprint)
		speed *= 2.0f;

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_W))
		_Render->getCamera()->ProcessKeyboard(FORWARD, speed);

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_S))
		_Render->getCamera()->ProcessKeyboard(BACKWARD, speed);

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_A))
		_Render->getCamera()->ProcessKeyboard(LEFT, speed);

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_D))
		_Render->getCamera()->ProcessKeyboard(RIGHT, speed);
}

void GameState_WMOViewer::Update(double _time, double _dTime)
{
	animtime += (_dTime * 1000.0f);
	globalTime = static_cast<int>(animtime);
}

void GameState_WMOViewer::PreRender3D()
{
	_Render->BindRBs();
	_Render->getTechniquesMgr()->PreRender3D(_Render->getCamera(), _Render->m_RenderBuffer);

	setVisible(true);
}

void GameState_WMOViewer::Render3D()
{
	// Debug
	/*	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);
	_Render->getTechniquesMgr()->Debug_Pass->Bind();
	_Render->getTechniquesMgr()->Debug_Pass->SetColor4(vec4(0.7, 0.7, 0.7, 1.0));
	_Render->r.setGeometry(m_DebugGeom);
	_Render->r.draw(PRIM_TRILIST, 0, 6);
	_Render->getTechniquesMgr()->Debug_Pass->Unbind();
	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);*/
}

void GameState_WMOViewer::PostRender3D()
{
	_Render->UnbindRBs();
	_Render->PostprocessSimple();

	//--

	m_TestCamera->Position = _Render->getCamera()->Position + vec3(0, 1, 0) * 750.0f;
	m_TestCamera->Roll = _Render->getCamera()->Roll + 180.0f;
	m_TestCamera->Pitch = -89.0f;
	m_TestCamera->SetNeedUpdate();
	m_TestCamera->Update(0, 0);

	m_TestRenderBuffer->setRenderBuffer();
	{
		_Render->r.clear();

		_Render->getTechniquesMgr()->PreRender3D(m_TestCamera, m_TestRenderBuffer);

		// Render here
		sceneManager->Render3D();
		_Render->getCamera()->Render();
	}
	m_TestRenderBuffer->resetRenderBuffer();

	m_TestRenderBufferFinal->setRenderBuffer();
	{
		_Render->r.clear();
		for (uint32 i = 0; i < 4; i++)
		{
			_Render->r.setTexture(i, m_TestRenderBuffer->getRenderBufferTex(i), 0, 0);
		}
		
		_Render->PostprocessSimple();
	}
	m_TestRenderBufferFinal->resetRenderBuffer();
}

void GameState_WMOViewer::RenderUI()
{
	vec2 size = m_TestRenderBufferFinal->getRenderBufferTex(0)->GetSize() / 2.5f;
	_Render->RenderTexture
	(
		m_VideoSettings.GetWindowSize() - size,
		m_TestRenderBufferFinal->getRenderBufferTex(0),
		size,
		true
	);

	_Render->RenderText
	(
		vec2(5, m_VideoSettings.windowSizeY - 44),
		"CamPos: [" +
		to_string(-(_Render->getCamera()->Position.x)) + "], [" +
		to_string(-(_Render->getCamera()->Position.z)) + "], [" +
		to_string(_Render->getCamera()->Position.y) + "]"
	);

	_Render->RenderText
	(
		vec2(5, m_VideoSettings.windowSizeY - 22),
		"CamRot: [" +
		to_string(_Render->getCamera()->Direction.x) + "], [" +
		to_string(_Render->getCamera()->Direction.y) + "], [" +
		to_string(_Render->getCamera()->Direction.z) + "]"
	);
}

//

void GameState_WMOViewer::OnMouseMoved(cvec2 _mousePos)
{
	if (enableFreeCamera)
	{
		vec2 mouseDelta = (_mousePos - lastMousePos) / m_VideoSettings.GetWindowSize();

		_Render->getCamera()->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);

		m_Engine->GetAdapter()->SetMousePosition(lastMousePos);
	}
}

bool GameState_WMOViewer::OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos)
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

bool GameState_WMOViewer::OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos)
{
	enableFreeCamera = false;
	lastMousePos = vec2();
	m_Engine->GetAdapter()->ShowCursor();
	return true;
}

bool GameState_WMOViewer::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
	if (_key == OW_KEY_ESCAPE)
	{
		m_Engine->SetNeedExit();
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

	return false;
}

bool GameState_WMOViewer::OnKeyboardReleased(int _key, int _scancode, int _mods)
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
