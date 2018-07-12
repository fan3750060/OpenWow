#include "stdafx.h"

// General
#include "GameState_InWorld.h"

GameState_InWorld::~GameState_InWorld()
{
	OW_SAFEDELETE(sceneManager);
}

bool GameState_InWorld::Init()
{
	CGameState::Init();

	minimapActive = false;

	m_WorldRender = new WorldRender(_World);

	m_TestRenderBuffer = _Render->r.createRenderBuffer(m_VideoSettings.GetWindowSize(), R_TextureFormats::RGBA8, true, 4, 8);
	m_TestRenderBufferFinal = _Render->r.createRenderBuffer(m_VideoSettings.GetWindowSize(), R_TextureFormats::RGBA8, false, 1, 0);
	m_TestCamera = new Camera;
	m_TestCamera->setupViewParams(Math::Pi / 2.0f, m_VideoSettings.aspectRatio, 5.0f, 10000.0f);


	for (auto& i : DBC_CinematicSequences)
	{
		const DBC_CinematicSequencesRecord& record = i;
		Log::Info("[%d] = [%d][%s]", record.Get_ID(), record.Get_CameraRec()->Get_ID(), record.Get_CameraRec()->Get_Filename());
	}

	sceneManager = new CSceneManager(_World->Map());
	sceneManager->setCamera(_Render->getCamera());
	sceneManager->setFrustrumCamera(_Render->getCamera());

	/*
	~[2] = [2][Cameras\FlybyUndead.mdx]
	~[21] = [235][Cameras\FlybyOrc.mdx]
	~[41] = [234][Cameras\FlyByDwarf.mdx]
	~[61] = [122][Cameras\FlybyNightElf.mdx]
	~[81] = [142][Cameras\FlyByHuman.mdx]
	~[101] = [162][Cameras\FlyByGnome.mdx]
	~[121] = [182][Cameras\FlyByTroll.mdx]
	~[141] = [202][Cameras\FlyByTauren.mdx]
	*/


	m2_Camera = nullptr;
	/*DBC_CinematicCameraRecord* camRec = DBC_CinematicSequences[81]->Get_CameraRec();
	M2* cinematicCamera = GetManager<IM2Manager>()->Add(camRec->Get_Filename());
	//m2_Camera = new CM2_Base_Instance(nullptr, cinematicCamera);
	//m2_Camera->getAnimator()->setOnEndFunction(FUNCTION_CLASS_Builder(GameState_InWorld, this, SwitchOnEnd));
	//m2_Camera->getAnimator()->PlayAnimation(0, false);
	//_Render->getCamera()->DisableUpdate();

	vec3 humans = vec3(-8949.95f, -132.493f, 83.5312f);
	vec3 orcs = vec3(-618.518f, -4251.67f, 38.718);
	vec3 dwarvs = vec3(-6240.32, 331.033, 382.758); // 6.17716
	vec3 nights = vec3(10311.3, 832.463, 1326.41); // 5.69632
	vec3 undeads = vec3(1676.71f, 1678.31f, 121.67); // 2.70526
	vec3 taurens = vec3(-2917.58f, -257.98f, 52.9968f); // 2.70526

	vec3 gPos = vec3(camRec->Get_EndY(), camRec->Get_EndX(), camRec->Get_EndZ());
	m_Rot = camRec->Get_Rotation();
	m_DefPos = fromGameToReal(gPos);
	m_Ended = false;*/

	_Bindings->RegisterRenderable3DObject(this, 25);

	return true;
}

void GameState_InWorld::Destroy()
{
	CGameState::Destroy();
}

void GameState_InWorld::Update(double _time, double _dTime)
{
	if (!m_Ended && m2_Camera != nullptr)
	{
		m2_Camera->Update(_time, _dTime);

		vec3 pos, tar;
		float fov, nearP, farP;
		m2_Camera->getObject()->m_Cameras[0]->getParams(&pos, &tar, &fov, &nearP, &farP);

		pos = m_DefPos + rotateV(pos, 4.52f);
		tar = m_DefPos + rotateV(tar, 4.52f);

		_Render->getCamera()->Position = pos;
		_Render->getCamera()->setupViewParams(fov, m_VideoSettings.aspectRatio, nearP, farP * 10000.0f);
		_Render->getCamera()->setViewMatrix(mat4::lookAtRH(pos, tar, vec3(0.0f, 1.0f, 0.0f)));
	}
}

void GameState_InWorld::Render3D()
{
	SceneNode* intNode = GetManager<ISceneManager>()->getIntersectedNode();
	if (intNode != nullptr)
	{
		_Render->DrawBoundingBox(intNode->getBounds(), vec4(1.0f, 0.2f, 0.2f, 0.8f));
	}

	// tauren 1.58
	// nights 0.95
	// human 4.52

	/*for (uint32 i = m2_Camera->getAnimator()->getStart(); i < m2_Camera->getAnimator()->getEnd(); i += 50)
	{
		m2_Camera->getObject()->m_Cameras[0].calc(i, 0);

		vec3 pos, tar;
		float fov, nearP, farP;
		m2_Camera->getObject()->m_Cameras[0].getParams(&pos, &tar, &fov, &nearP, &farP);

		//pos = m_DefPos + pos;

		pos = m_DefPos + rotateV(pos, 4.52);

		_Render->DrawSphere(mat4(), pos, 1);
	}*/
}

void GameState_InWorld::PostRender3D()
{
	//--

	/*m_TestCamera->Position = _Render->getCamera()->Position + vec3(0.0f, 1, 0.0f) * 300.0f;
	m_TestCamera->Roll = _Render->getCamera()->Roll;
	m_TestCamera->Pitch = -90.0f;
	m_TestCamera->m_NeedUpdate = true;
	m_TestCamera->Update(0, 0);

	m_TestRenderBuffer->setRenderBuffer();
	{
		_Render->r.clear();
		_Render->getTechniquesMgr()->PreRender3D(m_TestCamera, m_TestRenderBuffer);

		ADT_WMO_Instance::reset();
		ADT_MDX_Instance::reset();

		// Render here
		sceneManager->SetRenderQueueDebug(true);
		sceneManager->Render3D();
		sceneManager->SetRenderQueueDebug(false);
	}
	m_TestRenderBuffer->resetRenderBuffer();

	m_TestRenderBufferFinal->setRenderBuffer();
	{
		_Render->r.clear();
		for (uint32 i = 0; i < 4; i++)
		{
			_Render->r.setTexture(i, m_TestRenderBuffer->getRenderBufferTex(i), SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_CLAMP, 0);
		}

		_Render->PostprocessSimple();
	}
	m_TestRenderBufferFinal->resetRenderBuffer();*/
}



bool GameState_InWorld::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
	if (_key == OW_KEY_ESCAPE)
	{
		GetManager<IGameStateManager>()->SetGameState(GameStatesNames::GAME_STATE_MENU);
		_World->Map()->Unload();
		return true;
	}

	if (_key == OW_KEY_M)
	{
		SwitchBool(minimapActive);
		return true;
	}


	return false;
}