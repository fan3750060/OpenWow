#include "stdafx.h"

// General
#include "GameState_WMOViewer.h"

GameState_WMOViewer::GameState_WMOViewer() :
	animtime(0),
	globalTime(0)
{}

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
	new WMOsManager();

	CreateDebugGeom();

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
		WMO* wmo = GetManager<IWMOManager>()->Add("World\\wmo\\Lorderon\\Undercity\\Undercity.wmo");
		new Single_WMO_Instance(root, wmo);
		new Single_WMO_Instance(root, wmo); 
		//new Single_WMO_Instance(root, wmo);
	}

	CSceneManager* sceneManager = new CSceneManager(root);

	_Render->getCamera()->Position = vec3(50, 50, 50);
	_Render->getCamera()->SetNeedUpdate();

	//
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

void GameState_WMOViewer::Update(double _time, double _dTime)
{
	animtime += (_dTime * 1000.0f);
	globalTime = static_cast<int>(animtime);
}

void GameState_WMOViewer::PreRender3D()
{
	_Render->BindRBs();

	_Render->getTechniquesMgr()->PreRender3D();

	setVisible(true);
}

void GameState_WMOViewer::Render3D()
{
}

void GameState_WMOViewer::PostRender3D()
{
	_Render->UnbindRBs();

	_Render->PostprocessSimple();
}

void GameState_WMOViewer::RenderUI()
{
	_Render->RenderText
	(
		vec2(5, m_VideoSettings.windowSizeY - 44),
		"CamPos: [" + 
		to_string(-(_Render->mainCamera->Position.x - C_ZeroPoint)) + "], [" +
		to_string(-(_Render->mainCamera->Position.z - C_ZeroPoint)) + "], [" + 
		to_string(_Render->mainCamera->Position.y) + "]"
	);
	
	_Render->RenderText
	(
		vec2(5, m_VideoSettings.windowSizeY - 22), 
		"CamRot: [" + 
		to_string(_Render->mainCamera->Direction.x) + "], [" + 
		to_string(_Render->mainCamera->Direction.y) + "], [" + 
		to_string(_Render->mainCamera->Direction.z) + "]"
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