#include "stdafx.h"

// General
#include "GameState_CreateCharacter.h"

struct
{
	uint32 index;
	Race::List race;
} Races[10] = 
{
	{ 0, Race::Human },
	{ 1, Race::Orc },
	{ 2, Race::Dwarf },
	{ 3, Race::Nightelf },
	{ 4, Race::Undead },
	{ 5, Race::Tauren },
	{ 6, Race::Gnome },
	{ 7, Race::Troll },
	{ 8, Race::Bloodelf },
	{ 9, Race::Draenei }
};

GameState_CreateCharacter::GameState_CreateCharacter() :
	m_Char(nullptr)
{}

void GameState_CreateCharacter::CreateBackgroud(Race::List _charRace)
{
	if (_charRace == Race::Troll) _charRace = Race::Orc;
	if (_charRace == Race::Gnome) _charRace = Race::Dwarf;

	DBC_ChrRacesRecord* raceRecord = DBC_ChrRaces[_charRace];
	assert1(raceRecord != nullptr);

	char buff[256];
	sprintf_s(buff, "Interface\\Glues\\Models\\UI_%s\\UI_%s.m2", raceRecord->Get_ClientFileString(), raceRecord->Get_ClientFileString());

	M2* model = GetManager<IM2Manager>()->Add(buff);
	model->getMiscellaneous()->getCameraDirect(0)->setup(vec3(0), 0);

	m_BackgroudModel = new CM2_Base_Instance(nullptr, model);
}

void GameState_CreateCharacter::OnBtnRace(int _val)
{
	m_CurrentRace += 1;
	if (m_CurrentRace < 0)
	{
		m_CurrentRace = 9;
	}
	if (m_CurrentRace > 9)
	{
		m_CurrentRace = 0;
	}
	UpdateState();
}

void GameState_CreateCharacter::UpdateState()
{
	Race::List race = Races[m_CurrentRace].race;

	DBC_ChrRacesRecord* raceRecord = DBC_ChrRaces[race];
	assert1(raceRecord != nullptr);

	CharacterTemplate tempPala;
	tempPala.TemplateFillDefaultPaladin();

	m_Char = new Character();
	m_Char->InitFromTemplate(tempPala);
	m_Char->setScale(vec3(10.0f));


	/*SafeDelete(m_Char);
	m_Char = new Character();
	m_Char->InitFromDisplayInfoCreating(raceRecord->Get_MaleModelID(), race, Gender::Male);*/

	CreateBackgroud(race);
}


bool GameState_CreateCharacter::Init()
{
	CGameState::Init();

	new CM2_Manager();


	// UI

	UIWowButon* btnPrev = new UIWowButon(GetManager<IUIMgr>());
	btnPrev->Init(vec2(20, 20));
	btnPrev->AttachTo(m_Window);
	btnPrev->ShowText();
	btnPrev->SetText("<");
	SETBUTTONACTION_ARG(btnPrev, GameState_CreateCharacter, this, OnBtnRace, int32, -1);

	UIWowButon* btnNext = new UIWowButon(GetManager<IUIMgr>());
	btnNext->Init(vec2(220, 20));
	btnNext->AttachTo(m_Window);
	btnNext->ShowText();
	btnNext->SetText(">");
	SETBUTTONACTION_ARG(btnNext, GameState_CreateCharacter, this, OnBtnRace, int32, 1);

	_Render->getCamera()->Position = vec3(50, 50, 50);
	_Render->getCamera()->setViewMatrix(glm::lookAt(vec3(25, 25, 25), vec3(), vec3(0, 1, 0)));
	_Render->getCamera()->SetNeedUpdate();

	m_CurrentRace = 0;
	UpdateState();

	return true;
}

void GameState_CreateCharacter::Destroy()
{
	CGameState::Destroy();
}

bool GameState_CreateCharacter::Set()
{
	CGameState::Set();

	_Bindings->RegisterRenderable3DObject(this);

	return true;
}

void GameState_CreateCharacter::Unset()
{
	CGameState::Unset();

	_Bindings->UnregisterRenderable3DObject(this);
}

//

void GameState_CreateCharacter::Update(double _time, double _dTime)
{
	_Render->getCamera()->Update(_time, _dTime);

	if (m_BackgroudModel)
	{
		m_BackgroudModel->Update(_time, _dTime);
		m_BackgroudModel->getM2()->getMiscellaneous()->getCameraDirect(0)->setup(vec3(0), 0);
	}

	CM2_Part_Attachment* attach = m_BackgroudModel->getM2()->getMiscellaneous()->getAttachmentDirect(0);

	mat4 relMatrix;
	relMatrix = glm::translate(relMatrix, attach->getBone()->getPivot());

	if (m_Char) m_Char->setAbsTrans(attach->getBone()->getTransformMatrix() * relMatrix);
}

bool GameState_CreateCharacter::PreRender3D()
{
	_Render->BindRBs();
	_Render->getTechniquesMgr()->PreRender3D(_Render->getCamera(), _Render->m_RenderBuffer);

	return true;
}

void GameState_CreateCharacter::Render3D()
{
	if (m_BackgroudModel)
	{
		m_BackgroudModel->PreRender3D();
		m_BackgroudModel->Render3D();
	}

	if (m_Char)
	{
		m_Char->PreRender3D();
		m_Char->Render3D();
	}
}

void GameState_CreateCharacter::PostRender3D()
{
	_Render->UnbindRBs();
	_Render->PostprocessSimple();
}

void GameState_CreateCharacter::RenderUI()
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

bool GameState_CreateCharacter::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
	if (_key == OW_KEY_ESCAPE)
	{
		m_Engine->SetNeedExit();
	}

	return false;
}