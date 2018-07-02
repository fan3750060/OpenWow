#include "stdafx.h"

// General
#include "GameState_CharacterViewer.h"

GameState_CharacterViewer::GameState_CharacterViewer()
{}

void GameState_CharacterViewer::CreateDebugGeom()
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

void GameState_CharacterViewer::PlayAnim(uint16 _anim)
{
	m_Char->m_Model->getAnimator()->PlayAnimation(_anim);
}

void GameState_CharacterViewer::InfoAnim()
{
	m_Char->m_Model->getAnimator()->PrintList();
}

bool GameState_CharacterViewer::Init()
{
	CGameState::Init();

	new CM2_Manager();

	CreateDebugGeom();

	m_Char = new Character();
	//m_Char->InitDefault();
	m_Char->InitFromDisplayInfo(19873);

	_Render->getCamera()->Position = vec3(50, 50, 50);
	_Render->getCamera()->setViewMatrix(mat4::lookAtRH(vec3(25, 25, 25), vec3(), vec3(0, 1, 0)));
	_Render->getCamera()->SetNeedUpdate();

	ADDCONSOLECOMMAND_CLASS_WITHARGS("a_play", GameState_CharacterViewer, PlayAnim, uint16);
	ADDCONSOLECOMMAND_CLASS("a_info", GameState_CharacterViewer, InfoAnim);

	return true;
}

void GameState_CharacterViewer::Destroy()
{
	CGameState::Destroy();
}

bool GameState_CharacterViewer::Set()
{
	CGameState::Set();

	_Bindings->RegisterRenderable3DObject(this);

	return true;
}

void GameState_CharacterViewer::Unset()
{
	CGameState::Unset();

	_Bindings->UnregisterRenderable3DObject(this);
}

//

void GameState_CharacterViewer::Update(double _time, double _dTime)
{
	if (m_Char)
	{
		m_Char->m_Model->Update(_time, _dTime);
	}

	_Render->getCamera()->Update(_time, _dTime);
}

bool GameState_CharacterViewer::PreRender3D()
{
	_Render->BindRBs();
	_Render->getTechniquesMgr()->PreRender3D(_Render->getCamera(), _Render->m_RenderBuffer);

	return true;
}

void GameState_CharacterViewer::Render3D()
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
	m_Char->m_Model->PreRender3D();
	m_Char->m_Model->Render3D();
}

void GameState_CharacterViewer::PostRender3D()
{
	_Render->UnbindRBs();
	_Render->PostprocessSimple();
}

void GameState_CharacterViewer::RenderUI()
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

bool GameState_CharacterViewer::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
	if (_key == OW_KEY_ESCAPE)
	{
		m_Engine->SetNeedExit();
	}

	return false;
}