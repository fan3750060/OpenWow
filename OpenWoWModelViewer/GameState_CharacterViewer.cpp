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
	m_DebugGeom = _Render->r.beginCreatingGeometry(PRIM_TRILIST, _Render->getRenderStorage()->__layout_GxVBF_P);
	m_DebugGeom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 0, 0);
	m_DebugGeom->finishCreatingGeometry();
}

void GameState_CharacterViewer::PlayAnim(uint16 _anim)
{
	m_Char[0]->getAnimator()->PlayAnimation(_anim);
}

void GameState_CharacterViewer::InfoAnim()
{
	m_Char[0]->getAnimator()->PrintList();
}

bool GameState_CharacterViewer::Init()
{
	CGameState::Init();

	new CM2_Manager();

	CreateDebugGeom();

	/*vector<uint32> exists;
	for (int i = 0; i < cnt; i++)
	{
		for (int j = 0; j < cnt; j++)
		{
			int index = i + j * cnt;
			m_CharExtra[index] = new Character();
			//m_Char->InitDefault();

			while (true)
			{
				int random = Random::GenerateMax(32000);

				DBC_CreatureDisplayInfoRecord* rec = DBC_CreatureDisplayInfo[random];
				if (rec == nullptr)	continue;

				DBC_CreatureDisplayInfoExtraRecord* exRec = rec->Get_HumanoidData();
				if (exRec == nullptr) continue;

				if (exRec->Get_Race()->Get_ID() > 10) continue;


				if (std::find(exists.begin(), exists.end(), random) != exists.end()) continue;

				m_CharExtra[index]->InitFromDisplayInfo(random);
				m_CharExtra[index]->SetScale(5.0f);

				exists.push_back(random);
				break;
			}

			m_CharExtra[index]->SetTranslate(vec3(i * 10.0f, 0.0f, j * 10.0f));
		}
	}*/

	CharacterTemplate tempPala;
	tempPala.TemplateFillDefaultPaladin();

	CharacterTemplate tempShaman;
	tempShaman.TemplateFillDefaultShaman();

	m_Char[0] = new Character();
	m_Char[0]->InitFromTemplate(tempPala);
	m_Char[0]->SetScale(vec3(10.0f));

	/*m_Char[1] = new Character();
	m_Char[1]->SetTranslate(vec3(0, 0, 25), false);
	m_Char[1]->InitFromTemplate(tempShaman);
	m_Char[1]->SetScale(10.0f);

	m_Char[2] = new Character();
	m_Char[2]->SetTranslate(vec3(0, 0, 50), false);
	m_Char[2]->InitFromDisplayInfo(2638);
	m_Char[2]->SetScale(10.0f);*/

	_Render->getCamera()->Position = vec3(50, 50, 50);
	_Render->getCamera()->setViewMatrix(glm::lookAt(vec3(25, 25, 25), vec3(), vec3(0, 1, 0)));
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
	m = glm::translate(m, vec3(0.0f, -5.0f, 0.0f));
	m = glm::scale(m, vec3(1000.0f));

	_Render->getTechniquesMgr()->Debug_Pass->Bind();
	_Render->getTechniquesMgr()->Debug_Pass->setWorld(m);
	{
		_Render->getTechniquesMgr()->Debug_Pass->SetColor4(vec4(0.7f, 0.7f, 0.7f, 0.5f));

		_Render->r.setGeometry(m_DebugGeom);
		_Render->r.draw(0, 6);
	}
	_Render->getTechniquesMgr()->Debug_Pass->Unbind();
	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);

	for (uint32 i = 0; i < 10; i++)
	{
		if (m_Char[i])
		{
			m_Char[i]->PreRender3D();
			m_Char[i]->Render3D();
		}
	}


	// Geom
	for (int i = 0; i < cnt; i++)
	{
		for (int j = 0; j < cnt; j++)
		{
			int index = i + j * cnt;
			if (m_CharExtra[index])
			{
				m_CharExtra[index]->PreRender3D();
				m_CharExtra[index]->Render3D();
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
	m_UIMgr->RenderText
	(
		vec2(5, m_VideoSettings.windowSizeY - 44),
		"CamPos: [" + 
		to_string(_Render->getCamera()->Position.x) + "], [" +
		to_string(_Render->getCamera()->Position.y) + "], [" +
		to_string(_Render->getCamera()->Position.z) + "]",
		COLOR_WHITE
	);
	
	m_UIMgr->RenderText
	(
		vec2(5, m_VideoSettings.windowSizeY - 22), 
		"CamRot: [" + 
		to_string(_Render->getCamera()->Direction.x) + "], [" +
		to_string(_Render->getCamera()->Direction.y) + "], [" +
		to_string(_Render->getCamera()->Direction.z) + "]",
		COLOR_WHITE
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