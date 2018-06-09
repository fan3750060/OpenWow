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
	m_DebugGeom = _Render->r.beginCreatingGeometry(_Render->Storage()->__layout_GxVBF_P);
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

	if (m_Engine->GetArgumentsCount() > 1)
	{
		string path = m_Engine->GetArgument(1);
		int pos = path.find("ExData");
		assert1(pos != -1);
		path = path.substr(pos + 7);
		backgroundModel = GetManager<IWMOManager>()->Add(path);
	}
	else
	{
		backgroundModel = GetManager<IWMOManager>()->Add("World\\wmo\\cameron.wmo");
	}

	_PipelineGlobal->GetCamera()->Position = vec3(50, 50, 50);
	_PipelineGlobal->GetCamera()->SetNeedUpdate();

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

	_Bindings->RegisterRenderable3DObject(this);

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
		_PipelineGlobal->GetCamera()->ProcessKeyboard(FORWARD, speed);

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_S))
		_PipelineGlobal->GetCamera()->ProcessKeyboard(BACKWARD, speed);

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_A))
		_PipelineGlobal->GetCamera()->ProcessKeyboard(LEFT, speed);

	if (m_Engine->GetAdapter()->GetInput()->IsKeyPressed(OW_KEY_D))
		_PipelineGlobal->GetCamera()->ProcessKeyboard(RIGHT, speed);
}

void GameState_WMOViewer::Update(double _time, double _dTime)
{
	animtime += (_dTime * 1000.0f);
	globalTime = static_cast<int>(animtime);
}

void GameState_WMOViewer::PreRender3D()
{
	SetVisible(backgroundModel != nullptr);
}

void GameState_WMOViewer::Render3D()
{
	_Render->rb->setRenderBuffer();
	_Render->r.clear();

	// Camera
	_Pipeline->Clear();

	// Debug
	/*_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
	_Render->TechniquesMgr()->m_Debug_GeometryPass->Bind();
	_Render->TechniquesMgr()->m_Debug_GeometryPass->SetPVW();
	_Render->TechniquesMgr()->m_Debug_GeometryPass->SetColor4(vec4(0.7, 0.7, 0.7, 1.0));
	_Render->r.setGeometry(m_DebugGeom);
	_Render->r.draw(PRIM_TRILIST, 0, 6);
	_Render->TechniquesMgr()->m_Debug_GeometryPass->Unbind();
	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);*/

	Camera* tt = nullptr;

	tt = _Render->mainCamera;
	_PipelineGlobal->SetCamera(_Render->mainCamera);
	_PipelineGlobal->SetCameraFrustum(_Render->mainCamera);


	// Geom
	_Pipeline->Clear();
	backgroundModel->Render(0);
	


	/*_Pipeline->Clear();

	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
	_Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	_Render->TechniquesMgr()->m_Debug_GeometryPass->Bind();
	_Render->TechniquesMgr()->m_Debug_GeometryPass->SetPVW();
	_Render->TechniquesMgr()->m_Debug_GeometryPass->SetColor4(vec4(1.0, 1.0, 1.0, 1.0));
	_Render->r.setGeometry(tt->__geom);
	_Render->r.draw(PRIM_TRILIST, 0, 24);
	_Render->TechniquesMgr()->m_Debug_GeometryPass->Unbind();

	_Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);*/
}

void GameState_WMOViewer::PostRender3D()
{
	if (backgroundModel == nullptr) return;

	// Postprocess pass
	_Render->rb->resetRenderBuffer();
	for (uint32 i = 0; i < 4; i++)
	{
		_Render->r.setTexture(i, _Render->rb->getRenderBufferTex(i), 0, 0);
	}
	_Render->r.clear(CLR_COLOR_RT0 | CLR_DEPTH);

	_Render->TechniquesMgr()->m_POST_Simple->Bind();
	_Render->TechniquesMgr()->m_POST_Simple->SetCameraPos(_Camera->Position);

	_Render->r.setDepthTest(false);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->RenderQuad();

	_Render->r.setBlendMode(false);
	_Render->r.setDepthTest(true);

	_Render->TechniquesMgr()->m_POST_Simple->Unbind();
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

		_PipelineGlobal->GetCamera()->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);

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