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
	//backgroundModel->m_Animator->PlayAnimation(_anim);
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
		backgroundModel = GetManager<IM2Manager>()->Add(path);
	}
	else
	{
		backgroundModel = GetManager<IM2Manager>()->Add("World\\Generic\\PassiveDoodads\\Lights\\Torch.m2"/*"Creature\\Ragnaros\\Ragnaros.m2"*/);
	}

	_Render->getCamera()->Position = vec3(50, 50, 50);
	_Render->getCamera()->SetNeedUpdate();

	//
	enableFreeCamera = false;
	cameraSprint = false;


	ADDCONSOLECOMMAND_CLASS_WITHARGS("play", GameState_M2Viewer, PlayAnim, uint16);

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
	if (backgroundModel)
	{
		backgroundModel->updateEmitters(_dTime);
		//backgroundModel->Update(_time, _dTime);
	}
}

void GameState_M2Viewer::PreRender3D()
{
	_Render->getTechniquesMgr()->PreRender3D();

	setVisible(backgroundModel != nullptr);
}

void GameState_M2Viewer::Render3D()
{
	_Render->BindRBs();

	// Debug
	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
	_Render->getTechniquesMgr()->Debug_Pass->Bind();
	_Render->getTechniquesMgr()->Debug_Pass->SetColor4(vec4(0.7, 0.7, 0.7, 1.0));
	_Render->r.setGeometry(m_DebugGeom);
	_Render->r.draw(PRIM_TRILIST, 0, 6);
	_Render->getTechniquesMgr()->Debug_Pass->Unbind();
	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);

	Camera* tt = nullptr;

	/*if (backgroundModel->m_Cameras.size() > 0)
	{
		backgroundModel->m_Cameras[0].setup(animtime, globalTime);
		tt = backgroundModel->m_Cameras[0].GetCamera();
		_PipelineGlobal->SetCamera(backgroundModel->m_Cameras[0].GetCamera());
		_PipelineGlobal->SetCameraFrustum(backgroundModel->m_Cameras[0].GetCamera());
	}
	else*/
	{
		tt = _Render->mainCamera;
	}


	// Geom
	mat4 worldMatrix;
	worldMatrix.scale(5);
	backgroundModel->Render(worldMatrix);
	


	/*
	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
	_Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	_Render->getTechniquesMgr()->Debug_Pass->Bind();
	_Render->getTechniquesMgr()->Debug_Pass->SetPVW();
	_Render->getTechniquesMgr()->Debug_Pass->SetColor4(vec4(1.0, 1.0, 1.0, 1.0));
	_Render->r.setGeometry(tt->__geom);
	_Render->r.draw(PRIM_TRILIST, 0, 24);
	_Render->getTechniquesMgr()->Debug_Pass->Unbind();

	_Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);*/
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