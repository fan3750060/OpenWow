#include "stdafx.h"

// General
#include "GameState_CharacterViewer.h"

GameState_CharacterViewer::GameState_CharacterViewer()
{}

void GameState_CharacterViewer::CreateDebugGeom()
{
	vector<vec3> vecrtices;
			vecrtices.push_back(vec3(-1.0f, 0, 1.0f));
			vecrtices.push_back(vec3(-1.0f, 0, -1.0f));
			vecrtices.push_back(vec3(1.0f, 0, -1.0f));

			vecrtices.push_back(vec3(-1.0f, 0, 1.0f));
			vecrtices.push_back(vec3(1.0f, 0, -1.0f));
			vecrtices.push_back(vec3(1.0f, 0, 1.0f));

	// Vertex buffer
	R_Buffer* __vb = _Render->r.createVertexBuffer(vecrtices.size() * sizeof(vec3), vecrtices.data());
	m_DebugGeom = _Render->r.beginCreatingGeometry(_Render->getRenderStorage()->__layout_GxVBF_P);
	m_DebugGeom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 0, 0);
	m_DebugGeom->finishCreatingGeometry();
}

void GameState_CharacterViewer::PlayAnim(uint16 _anim)
{
	//m_Char->getModel()->getAnimator()->PlayAnimation(_anim);
}

void GameState_CharacterViewer::InfoAnim()
{
	//m_Char->getModel()->getAnimator()->PrintList();
}

void GameState_CharacterViewer::DeleteAll()
{
	for (int i = 0; i < cnt; i++)
	{
		for (int j = 0; j < cnt; j++)
		{
			int index = i + j * cnt;

			delete m_Char[index];
			m_Char[index] = nullptr;
		}
	}
}

bool GameState_CharacterViewer::Init()
{
	CGameState::Init();

	new CM2_Manager();

	CreateDebugGeom();

	vector<uint32> exists;

	for (int i = 0; i < cnt; i++)
	{
		for (int j = 0; j < cnt; j++)
		{
			int index = i + j * cnt;
			m_Char[index] = new Creature(vec3(i * 10.0f, 0.0f, j * 10.0f));
			//m_Char->InitDefault();

			while (true)
			{
				int random = Random::GenerateMax(32000);
				DBC_CreatureDisplayInfoRecord* rec = DBC_CreatureDisplayInfo[random];
				if (rec == nullptr)	continue;
				if (rec->Get_HumanoidData() != nullptr) continue;
				if (std::find(exists.begin(), exists.end(), random) != exists.end()) continue;

				m_Char[index]->InitFromDisplayInfo(random);
				exists.push_back(random);
				break;
			}
			
		}
	}
	_Render->getCamera()->Position = vec3(50, 50, 50);
	_Render->getCamera()->setViewMatrix(mat4::lookAtRH(vec3(25, 25, 25), vec3(), vec3(0, 1, 0)));
	_Render->getCamera()->SetNeedUpdate();

	ADDCONSOLECOMMAND_CLASS_WITHARGS("a_play", GameState_CharacterViewer, PlayAnim, uint16);
	ADDCONSOLECOMMAND_CLASS("a_info", GameState_CharacterViewer, InfoAnim);
	ADDCONSOLECOMMAND_CLASS("del_all", GameState_CharacterViewer, DeleteAll);

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

	mat4 m;
	m.translate(0.0f, -5.0f, 0.0f);
	m.scale(1000.0f);

	_Render->getTechniquesMgr()->Debug_Pass->Bind();
	_Render->getTechniquesMgr()->Debug_Pass->SetWorldMatrix(m);
	{
		_Render->getTechniquesMgr()->Debug_Pass->SetColor4(vec4(0.7f, 0.7f, 0.7f, 0.5f));

		_Render->r.setGeometry(m_DebugGeom);
		_Render->r.draw(PRIM_TRILIST, 0, 6);
	}
	_Render->getTechniquesMgr()->Debug_Pass->Unbind();
	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);

	// Geom
	for (int i = 0; i < cnt; i++)
	{
		for (int j = 0; j < cnt; j++)
		{
			int index = i + j * cnt;
			if (m_Char[index])
			{
				m_Char[index]->PreRender3D();
				m_Char[index]->Render3D();
			}
		}
	}
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