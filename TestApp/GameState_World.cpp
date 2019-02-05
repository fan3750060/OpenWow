#include "stdafx.h"

// General
#include "GameState_World.h"

// Additional

CGameState_World::CGameState_World()
{
	m_Viewport = Viewport(0, 0, 1280.0f, 1024.0f);

	m_3DScene = std::make_shared<Scene3D>();
	m_UIScene = std::make_shared<SceneUI>();
}

CGameState_World::~CGameState_World()
{
}


//
// IGameState
//
bool CGameState_World::Init()
{
	std::shared_ptr<IFontsManager> fontsManager = std::make_shared<FontsManager>();
	AddManager<IFontsManager>(fontsManager);

	std::shared_ptr<IWMOManager> wmoManager = std::make_shared<WMOsManager>();
	AddManager<IWMOManager>(wmoManager);

	std::shared_ptr<IM2Manager> m2Manager = std::make_shared<CM2_Manager>();
	AddManager<IM2Manager>(m2Manager);

	//

	Application& app = Application::Get();
	std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();

	const float x = 40;
	const float y = 29;
	//const float x = 29;
	//const float y = 21;

	//
	// Camera controller
	//
	m_CameraController = std::make_shared<CCameraController>();
	m_CameraController->Init(app.GetRenderWindow());
	m_CameraController->GetCamera()->SetTranslate(vec3(0, 0, 0));
	m_CameraController->GetCamera()->SetTranslate(vec3(x * C_TileSize, 200, y * C_TileSize));
	m_CameraController->GetCamera()->SetRotate(vec3(0, 0, 0));
	m_CameraController->GetCamera()->SetViewport(m_Viewport);
	m_CameraController->GetCamera()->SetProjectionRH(45.0f, 1280.0f / 1024.0f, 1.0f, 4000.0f);


	//
	// World
	//

	// Cube Mesh
	std::shared_ptr<IMesh> cube = renderDevice->CreateCube();
	cube->SetType(SceneNodeTypes::SN_TYPE_DEBUG);
	std::shared_ptr<MaterialDebug> mat = std::make_shared<MaterialDebug>(renderDevice->CreateMaterial());
	mat->SetWrapper(mat);
	mat->SetDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	cube->SetMaterial(mat);

	// Cube SN
	std::shared_ptr<SceneNode3D> cubeNode = std::make_shared<SceneNode3D>();
	cubeNode->AddMesh(cube);
	cubeNode->SetTranslate(vec3(x * C_TileSize, 200, y * C_TileSize));
	cubeNode->SetScale(vec3(15, 15, 15));
	//cubeNode->SetParent(m_3DScene->GetRootNode());

	// M2 Model
	/*std::shared_ptr<M2> model = GetManager<IM2Manager>()->Add("Creature\\ARTHASLICHKING\\ARTHASLICHKING.m2");
	std::shared_ptr<CM2_Base_Instance> inst = std::make_shared<CM2_Base_Instance>(model);
	inst->CreateInstances();
	inst->SetParent(m_3DScene->GetRootNode());
	inst->SetScale(vec3(15.0f));
	inst->GetLocalTransform();*/

	// Map
	m_MapController = std::make_shared<MapController>();
	m_MapController->SetParent(m_3DScene->GetRootNode());
	m_MapController->MapPreLoad(*DBC_Map[1]);
	m_MapController->MapLoad();
	m_MapController->MapPostLoad();
	m_MapController->EnterMap(x, y);

	//
	// UI
	//

	std::vector<vec2> vecrtices;
	vecrtices.push_back(vec2(-1.0f, -1.0f));
	vecrtices.push_back(vec2(1.0f, -1.0f));
	vecrtices.push_back(vec2(-1.0f, 1.0f));
	vecrtices.push_back(vec2(1.0f, 1.0f));
	std::shared_ptr<IBuffer> __vb = _RenderDevice->CreateVertexBuffer(vecrtices);
	uint16 indexes[6] = { 0, 1, 2, 2, 1, 3 };
	std::shared_ptr<IBuffer> __ib = _RenderDevice->CreateIndexBuffer(indexes, 6);


	std::shared_ptr<IMesh> __geom = _RenderDevice->CreateMesh();
	__geom->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);
	__geom->SetIndexBuffer(__ib);

	std::shared_ptr<UI_Color_Material> uiMaterial = std::make_shared<UI_Color_Material>();
	uiMaterial->SetWrapper(uiMaterial);
	uiMaterial->SetColor(vec4(0, 1, 0, 0.3f));
	__geom->SetMaterial(uiMaterial);

	// Font
	std::shared_ptr<UIText> node = std::make_shared<UIText>();
	node->SetParent(m_UIScene->GetRootNode());
	node->SetText("Privet, mir!!!");
	node->SetTranslate(vec2(00.0f, 00.0f));
	

	// Texture
	std::shared_ptr<UITexture> node3 = std::make_shared<UITexture>();
	node3->SetParent(m_UIScene->GetRootNode());
	node3->SetTexture(m_MapController->getMinimap());
	node3->SetTranslate(vec2(100.0f, 100.0f));
	node3->SetScale(vec2(512.0f, 512.0f));
	
	// Texture 2
	//std::shared_ptr<UITexture> node4 = std::make_shared<UITexture>();
	//node4->SetParent(m_UIScene->GetRootNode());
	//node4->SetTexture(renderDevice->CreateTexture2D("Textures\\Moon02Glare.blp"));
	//node4->SetTranslate(vec2(200.0f, 000.0f));
	//node4->SetScale(vec2(100.0f, 100.0f));
	

	// Texture 3
	//std::shared_ptr<UITexture> node5 = std::make_shared<UITexture>();
	//node5->SetParent(m_UIScene->GetRootNode());
	//node5->SetTexture(renderDevice->CreateTexture2D("Textures\\ShaneCube.blp"));
	//node5->SetTranslate(vec2(000.0f, 200.0f));
	//node5->SetScale(vec2(100.0f, 100.0f));


	// Color quad
	//std::shared_ptr<SceneNodeUI> node2 = std::make_shared<SceneNodeUI>();
	//node2->SetParent(m_UIScene->GetRootNode());
	//node2->SetMesh(__geom);
	//node2->SetTranslate(vec2(180.0f, 180.0f));
	//node2->SetScale(vec2(10.0f, 10.0f));
	

	m_FrameQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 1);


	//
	// 3D Passes
	//
	m_ForwardTechnique.AddPass(std::make_shared<ClearRenderTargetPass>(app.GetRenderWindow()->GetRenderTarget(), ClearFlags::All, g_ClearColor, 1.0f, 0));
	AddSkyPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &m_ForwardTechnique, &m_Viewport, m_3DScene);
	AddWDLPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &m_ForwardTechnique, &m_Viewport, m_3DScene);
	AddDebugPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &m_ForwardTechnique, &m_Viewport, m_3DScene);
	AddMCNKPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &m_ForwardTechnique, &m_Viewport, m_3DScene);
	AddWMOPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &m_ForwardTechnique, &m_Viewport, m_3DScene);
	AddLiquidPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &m_ForwardTechnique, &m_Viewport, m_3DScene);
	AddM2Passes(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &m_ForwardTechnique, &m_Viewport, m_3DScene);
	
	
	//
	// UI Passes
	//
	AddUIPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &m_UITechnique, &m_Viewport, m_UIScene);


	return base::Init();
}

void CGameState_World::Destroy()
{
	/*renderDevice->CreateTexture2D("Textures\\ShaneCube.blp"); // DXT1
	renderDevice->CreateTexture2D("Textures\\Minimap\\00b445de1413eeca80cc683deb9af58b.blp"); // DXT1A
	renderDevice->CreateTexture2D("Textures\\MinimapMask.blp"); // DXT3
	renderDevice->CreateTexture2D("Textures\\SpellChainEffects\\VR_Chain.blp"); // DXT5
	renderDevice->CreateTexture2D("Textures\\Moon02Glare.blp"); // RAW0
	renderDevice->CreateTexture2D("Textures\\ShadowBlob.blp"); // RAW1
	renderDevice->CreateTexture2D("Textures\\moon.blp"); // RAW8
	renderDevice->CreateTexture2D("Textures\\SunGlare.blp"); // PURE*/

	base::Destroy();
}


//
//
//
void CGameState_World::OnPreRender(Render3DEventArgs& e)
{
	m_FrameQuery->Begin(e.FrameCounter);

	ADT_WMO_Instance::reset();
	ADT_MDX_Instance::reset();
}

void CGameState_World::OnRender(Render3DEventArgs& e)
{
	e.Camera = m_CameraController->GetCameraConst().operator->(); // TODO: Shit code. Refactor me.

	m_ForwardTechnique.Render(e);
}

void CGameState_World::OnPostRender(Render3DEventArgs& e)
{
	m_FrameQuery->End(e.FrameCounter);


	Query::QueryResult frameResult = m_FrameQuery->GetQueryResult(e.FrameCounter - (m_FrameQuery->GetBufferCount() - 1));
	if (frameResult.IsValid)
	{
		// Frame time in milliseconds
#ifdef  IS_DX11
		m_FrameTime = frameResult.ElapsedTime * 1000.0;
#else
		m_FrameTime = frameResult.ElapsedTime / 1000000.0;
#endif

		std::string title = std::to_string(m_FrameTime);
		if (m_MapController != nullptr)
			title += "_" + std::to_string(m_MapController->GetCurrentX()) + "_" + std::to_string(m_MapController->GetCurrentZ());

		Application::Get().GetRenderWindow()->SetWindowName(title);
	}

	// Query results for forward rendering technique.
	/*Query::QueryResult forwardOpaqueResult = g_pForwardOpaqueQuery->GetQueryResult(e.FrameCounter - (g_pForwardOpaqueQuery->GetBufferCount() - 1));
	Query::QueryResult forwardTransparentResult = g_pForwardTransparentQuery->GetQueryResult(e.FrameCounter - (g_pForwardTransparentQuery->GetBufferCount() - 1));
	if (forwardOpaqueResult.IsValid)
	{
		g_ForwardOpaqueStatistic.Sample(forwardOpaqueResult.ElapsedTime * 1000.0);
	}
	if (forwardTransparentResult.IsValid)
	{
		g_ForwardTransparentStatistic.Sample(forwardTransparentResult.ElapsedTime * 1000.0);
	}*/
}

void CGameState_World::OnRenderUI(RenderUIEventArgs& e)
{
	e.Viewport = &m_Viewport;

	m_UITechnique.RenderUI(e);
}