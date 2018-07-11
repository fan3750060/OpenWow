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
	m_DebugGeom = _Render->r.beginCreatingGeometry(PRIM_TRILIST, _Render->getRenderStorage()->__layout_GxVBF_P);
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

void GameState_M2Viewer::OpenFile(const char* _fname)
{
	string path = _fname;
	int pos = path.find("ExData");
	assert1(pos != -1);
	path = path.substr(pos + 7);

	M2* m2 = GetManager<IM2Manager>()->Add(path);
	m2_Model = new CM2_Viewer_Instance(m2);
}

bool GameState_M2Viewer::Init()
{
	CGameState::Init();

	//

	new CM2_Manager();
	_Render->getAdapter()->GetInput()->setOnFileDropperCallback(FUNCTION_CLASS_WA_Builder(GameState_M2Viewer, this, OpenFile, const char*));

	CreateDebugGeom();

	if (m_Engine->GetArgumentsCount() > 1)
	{
		OpenFile(m_Engine->GetArgument(1).c_str());
	}
	else
	{
		//M2* model = GetManager<IM2Manager>()->Add("CREATURE\\ArthasLichKing\\ArthasLichKing.M2");
		M2* model = GetManager<IM2Manager>()->Add("creature\\PHOENIX\\Phoenix.m2");
		m2_Model = new CM2_Viewer_Instance(model);
	}

	//m_Char = new Character();
	//m_Char->InitDefault();


	_Render->getCamera()->Position = vec3(50, 50, 50);
	_Render->getCamera()->setViewMatrix(mat4::lookAtRH(vec3(25, 25, 25), vec3(), vec3(0, 1, 0)));
	_Render->getCamera()->SetNeedUpdate();

	ADDCONSOLECOMMAND_CLASS_WITHARGS("a_play", GameState_M2Viewer, PlayAnim, uint16);
	ADDCONSOLECOMMAND_CLASS("a_info", GameState_M2Viewer, InfoAnim);

	DBC_CinematicCameraRecord* camRec = DBC_CinematicSequences[21]->Get_CameraRec();
	M2* cinematicCamera = GetManager<IM2Manager>()->Add(camRec->Get_Filename());
	m2_Camera = new CM2_Base_Instance(nullptr, cinematicCamera);

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

void GameState_M2Viewer::Update(double _time, double _dTime)
{
	

	//m2_Camera->getObject()->m_Cameras[0]->calc(_time * 3.0f, _dTime);
	//m2_Model->setTranslate(m2_Camera->getObject()->m_Cameras[0]->getTranslate());
	//m2_Model->setRotate(vec3(0, -Math::Pi / 2.0f, 0));

	//m2_Model->setTranslate(_Render->getCamera()->Position + _Render->getCamera()->Direction * 250.0f);
	//m2_Model->setRotate(_Render->getCamera()->Direction);

	//_Render->getCamera()->Position = 
	_Render->getCamera()->Update(_time, _dTime);
}

bool GameState_M2Viewer::PreRender3D()
{
	_Render->BindRBs();
	_Render->getTechniquesMgr()->PreRender3D(_Render->getCamera(), _Render->m_RenderBuffer);

	return true;
}

void GameState_M2Viewer::Render3D()
{
	// Debug
	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->getTechniquesMgr()->Debug_Pass->Bind();
	_Render->getTechniquesMgr()->Debug_Pass->setWorld(mat4());
	{
		_Render->getTechniquesMgr()->Debug_Pass->SetColor4(vec4(0.7f, 0.7f, 0.7f, 0.5f));

		_Render->r.setGeometry(m_DebugGeom);
		_Render->r.draw(0, 6);
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

bool GameState_M2Viewer::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
	if (_key == OW_KEY_ESCAPE)
	{
		m_Engine->SetNeedExit();
	}

	return false;
}