#include "stdafx.h"

// General
#include "GameState_M2Viewer.h"

GameState_M2Viewer::GameState_M2Viewer()
{}

void GameState_M2Viewer::CreateDebugGeom()
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

void GameState_M2Viewer::PlayAnim(uint16 _anim)
{
	m2_Model->getAnimator()->PlayAnimation(_anim);
}

void GameState_M2Viewer::InfoAnim()
{
	m2_Model->getAnimator()->PrintList();
}

bool GameState_M2Viewer::Init()
{
	CGameState::Init();

	//

	OpenDBs();

	new CM2_Manager();

	CreateDebugGeom();

	if (m_Engine->GetArgumentsCount() > 1)
	{
		string path = m_Engine->GetArgument(1);
		int pos = path.find("ExData");
		assert1(pos != -1);
		path = path.substr(pos + 7);
		M2* model = GetManager<IM2Manager>()->Add(path);
		m2_Model = new Single_M2_Instance(nullptr, model);
	}
	else
	{
		M2* model = GetManager<IM2Manager>()->Add("Creature\\Ragnaros\\Ragnaros.m2");
		m2_Model = new Single_M2_Instance(nullptr, model);
	}

	_Render->getCamera()->Position = vec3(50, 50, 50);
	_Render->getCamera()->setViewMatrix(mat4::lookAtRH(vec3(25, 25, 25), vec3(), vec3(0, 1, 0)));
	_Render->getCamera()->SetNeedUpdate();

	//
	enableFreeCamera = false;
	cameraSprint = false;


	ADDCONSOLECOMMAND_CLASS_WITHARGS("a_play", GameState_M2Viewer, PlayAnim, uint16);
	ADDCONSOLECOMMAND_CLASS("a_info", GameState_M2Viewer, InfoAnim);

	return true;
}

void GameState_M2Viewer::Destroy()
{
	CGameState::Destroy();
}

bool GameState_M2Viewer::Set()
{
	CGameState::Set();

	_Bindings->RegisterRenderable3DObject(this);

	return true;
}

void GameState_M2Viewer::Unset()
{
	CGameState::Unset();

	_Bindings->UnregisterRenderable3DObject(this);
}

//

void GameState_M2Viewer::Input(double _time, double _dTime)
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

void GameState_M2Viewer::Update(double _time, double _dTime)
{
	if (m2_Model)
	{
		m2_Model->Update(_time, _dTime);
	}

	_Render->getCamera()->Update(_time, _dTime);
}

void GameState_M2Viewer::PreRender3D()
{
	_Render->BindRBs();
	_Render->getTechniquesMgr()->PreRender3D(_Render->getCamera(), _Render->m_RenderBuffer);

	setVisible(true);
}

void GameState_M2Viewer::Render3D()
{
	// Debug
	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->getTechniquesMgr()->Debug_Pass->Bind();
	_Render->getTechniquesMgr()->Debug_Pass->SetWorldMatrix(mat4());
	{
		_Render->getTechniquesMgr()->Debug_Pass->SetColor4(vec4(0.7, 0.7, 0.7, 0.5));

		_Render->r.setGeometry(m_DebugGeom);
		_Render->r.draw(PRIM_TRILIST, 0, 6);
	}
	_Render->getTechniquesMgr()->Debug_Pass->Unbind();
	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);

	// Geom
	m2_Model->PreRender3D();
	m2_Model->Render3D();
}

void GameState_M2Viewer::PostRender3D()
{
	_Render->UnbindRBs();
	_Render->PostprocessSimple();
}

void GameState_M2Viewer::RenderUI()
{
	_Render->RenderText
	(
		vec2(5, m_VideoSettings.windowSizeY - 44),
		"CamPos: [" + 
		to_string(_Render->getCamera()->Position.x) + "], [" +
		to_string(_Render->getCamera()->Position.y) + "], [" +
		to_string(_Render->getCamera()->Position.z) + "]"
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

void GameState_M2Viewer::OnMouseMoved(cvec2 _mousePos)
{
	if (enableFreeCamera)
	{
		vec2 mouseDelta = (_mousePos - lastMousePos) / m_VideoSettings.GetWindowSize();

		_Render->getCamera()->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);

		m_Engine->GetAdapter()->SetMousePosition(lastMousePos);
	}
}

bool GameState_M2Viewer::OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos)
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

bool GameState_M2Viewer::OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos)
{
	enableFreeCamera = false;
	lastMousePos = vec2();
	m_Engine->GetAdapter()->ShowCursor();
	return true;
}

bool GameState_M2Viewer::OnKeyboardPressed(int _key, int _scancode, int _mods)
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