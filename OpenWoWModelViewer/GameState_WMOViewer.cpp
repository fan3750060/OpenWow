#include "stdafx.h"

// General
#include "GameState_WMOViewer.h"

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
	m_DebugGeom = _Render->r.beginCreatingGeometry(PRIM_TRILIST, _Render->getRenderStorage()->__layout_GxVBF_P);
	m_DebugGeom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 0, 0);
	m_DebugGeom->finishCreatingGeometry();
}

void GameState_WMOViewer::OpenFile(const char* _fname)
{
	std::string path = _fname;
	int pos = path.find("ExData");
	assert1(pos != -1);
	path = path.substr(pos + 7);

	WMO* wmo = GetManager<IWMOManager>()->Add(path);
	backgroundModel = new CWMO_Base_Instance(sceneManager->getRootNode(), wmo);
	backgroundModel->InitTransform();
	backgroundModel->EmptyTransformAndBounds();
}

bool GameState_WMOViewer::Init()
{
	CGameState::Init();

	//

	new CM2_Manager();
	new WMOsManager(_Render->getAdapter());
	_Render->getAdapter()->GetInput()->setOnFileDropperCallback(FUNCTION_CLASS_WA_Builder(GameState_WMOViewer, this, OpenFile, const char*));

	CreateDebugGeom();

	_Render->getCamera()->Position = vec3(50, 50, 50);
	_Render->getCamera()->setViewMatrix(glm::lookAt(vec3(25, 25, 25), vec3(), vec3(0, 1, 0)));
	_Render->getCamera()->SetNeedUpdate();

	m_TestRenderBuffer = _Render->r.createRenderBuffer(m_VideoSettings.GetWindowSize(), R_TextureFormats::RGBA8, true, 4, 0);
	m_TestRenderBufferFinal = _Render->r.createRenderBuffer(m_VideoSettings.GetWindowSize(), R_TextureFormats::RGBA8, false, 1, 0);
	m_TestCamera = new Camera;
	m_TestCamera->setupViewParams(Math::Pi / 4.0f, m_VideoSettings.aspectRatio, 2.0f, 2000.0f);

	BoundingBox bb;
	bb.setMin(vec3(Math::MinFloat));
	bb.setMax(vec3(Math::MaxFloat));
	bb.calculateCenter();

	SceneNode3D* root = new SceneNode3D();
	root->setVisible(true);
	root->SetBounds(bb);

	sceneManager = new CSceneManager(root);
	sceneManager->setCamera(_Render->getCamera());
	sceneManager->setFrustrumCamera(_Render->getCamera());

	if (m_Engine->GetArgumentsCount() > 1)
	{
		OpenFile(m_Engine->GetArgument(1).c_str());
	}
	else
	{
		//WMO* wmo = GetManager<IWMOManager>()->Add("World\\wmo\\Dungeon\\AZ_StormwindPrisons\\StormwindJail.wmo");
		WMO* wmo = GetManager<IWMOManager>()->Add("World\\wmo\\Northrend\\Dalaran\\ND_Dalaran.wmo");
		//WMO* wmo = GetManager<IWMOManager>()->Add("World\\wmo\\Kalimdor\\Ogrimmar\\Ogrimmar.wmo");
		//WMO* wmo = GetManager<IWMOManager>()->Add("World\\wmo\\Kalimdor\\Winterspring\\MD_WinterspringCave01.wmo");
		backgroundModel = new CWMO_Base_Instance(root, wmo);
		backgroundModel->InitTransform();
		backgroundModel->EmptyTransformAndBounds();
	}

	

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

void GameState_WMOViewer::Update(double _time, double _dTime)
{
	
}

bool GameState_WMOViewer::PreRender3D()
{
	_Render->BindRBs();
	_Render->getTechniquesMgr()->PreRender3D(_Render->getCamera(), _Render->m_RenderBuffer);

	return true;
}

void GameState_WMOViewer::Render3D()
{
	// Debug
	/*	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);
	_Render->getTechniquesMgr()->Debug_Pass->Bind();
	_Render->getTechniquesMgr()->Debug_Pass->SetColor4(vec4(0.7, 0.7, 0.7, 1.0));
	_Render->r.setGeometry(m_DebugGeom);
	_Render->r.draw(PRIM_TRILIST, 0, 6);
	_Render->getTechniquesMgr()->Debug_Pass->Unbind();
	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);*/
}

void GameState_WMOViewer::PostRender3D()
{
	_Render->UnbindRBs();
	_Render->PostprocessSimple();

	//--

	m_TestCamera->Position = _Render->getCamera()->Position + vec3(0, 1, 0) * 750.0f;
	m_TestCamera->Roll = _Render->getCamera()->Roll + 180.0f;
	m_TestCamera->Pitch = -89.0f;
	m_TestCamera->SetNeedUpdate();
	m_TestCamera->Update(0, 0);

	m_TestRenderBuffer->setRenderBuffer();
	{
		_Render->r.clear();

		_Render->getTechniquesMgr()->PreRender3D(m_TestCamera, m_TestRenderBuffer);

		// Render here
		sceneManager->Render3D();
		_Render->getCamera()->Render();
	}
	m_TestRenderBuffer->resetRenderBuffer();

	m_TestRenderBufferFinal->setRenderBuffer();
	{
		_Render->r.clear();
		for (uint32 i = 0; i < 4; i++)
		{
			_Render->r.setTexture(i, m_TestRenderBuffer->getRenderBufferTex(i), 0, 0);
		}
		
		_Render->PostprocessSimple();
	}
	m_TestRenderBufferFinal->resetRenderBuffer();
}

void GameState_WMOViewer::RenderUI()
{
	vec2 size = m_TestRenderBufferFinal->getRenderBufferTex(0)->GetSize() / 2.5f;
	_Render->RenderTexture
	(
		m_VideoSettings.GetWindowSize() - size,
		m_TestRenderBufferFinal->getRenderBufferTex(0),
		size,
		true
	);

	m_UIMgr->RenderText
	(
		vec2(5, m_VideoSettings.windowSizeY - 44),
		"CamPos: [" +
		to_string(-(_Render->getCamera()->Position.x)) + "], [" +
		to_string(-(_Render->getCamera()->Position.z)) + "], [" +
		to_string(_Render->getCamera()->Position.y) + "]",
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

bool GameState_WMOViewer::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
	if (_key == OW_KEY_ESCAPE)
	{
		m_Engine->SetNeedExit();
	}

	return false;
}