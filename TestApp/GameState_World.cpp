#include "stdafx.h"

// General
#include "GameState_World.h"

CGameState_World::CGameState_World()
{
	g_Viewport = Viewport(0, 0, 1280.0f, 1024.0f);

	g_pScene = std::make_shared<SceneBase>();
	g_pUIScene = std::make_shared<UISceneBase>();
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

	Application& app = Application::Get();
	std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();

	const float x = 40;
	const float y = 29;
	//const float x = 29;
	//const float y = 21;

	//
	// Camera controller
	//
	g_CameraController = std::make_shared<CCameraController>();
	g_CameraController->Init(app.GetRenderWindow());
	g_CameraController->GetCamera()->SetTranslate(vec3(0, 0, 0));
	g_CameraController->GetCamera()->SetTranslate(vec3(x * C_TileSize, 200, y * C_TileSize));
	g_CameraController->GetCamera()->SetRotate(vec3(0, 0, 0));
	g_CameraController->GetCamera()->SetViewport(g_Viewport);
	g_CameraController->GetCamera()->SetProjectionRH(45.0f, 1280.0f / 1024.0f, 1.0f, 4000.0f);


	//
	// World
	//

	// Cube

	std::shared_ptr<IMesh> cube = renderDevice->CreateCube();
	cube->SetType(SceneNodeTypes::SN_TYPE_DEBUG);
	std::shared_ptr<MaterialDebug> mat = std::make_shared<MaterialDebug>(renderDevice->CreateMaterial());
	mat->SetWrapper(mat);
	mat->SetDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	cube->SetMaterial(mat);

	std::shared_ptr<SceneNode> cubeNode = std::make_shared<SceneNode>();
	cubeNode->AddMesh(cube);
	cubeNode->SetTranslate(vec3(x * C_TileSize, 200, y * C_TileSize));
	cubeNode->SetScale(vec3(15, 15, 15));
	cubeNode->SetParent(g_pScene->GetRootNode());

	// M2 Model
	/*std::shared_ptr<M2> model = GetManager<IM2Manager>()->Add("Creature\\ARTHASLICHKING\\ARTHASLICHKING.m2");
	std::shared_ptr<CM2_Base_Instance> inst = std::make_shared<CM2_Base_Instance>(model);
	inst->CreateInstances();
	inst->SetParent(g_pScene->GetRootNode());
	inst->SetScale(vec3(15.0f));
	inst->GetLocalTransform();*/

	m_MapController = std::make_shared<MapController>();
	m_MapController->SetParent(g_pScene->GetRootNode());
	m_MapController->MapPreLoad(*DBC_Map[1]);
	m_MapController->MapLoad();
	m_MapController->MapPostLoad();
	m_MapController->EnterMap(x, y);


	g_pFrameQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 1);

	//
	// Passes
	//
	g_ForwardTechnique.AddPass(std::make_shared<ClearRenderTargetPass>(app.GetRenderWindow()->GetRenderTarget(), ClearFlags::All, g_ClearColor, 1.0f, 0));
	AddSkyPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &g_ForwardTechnique, &g_Viewport, g_pScene);
	AddWDLPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &g_ForwardTechnique, &g_Viewport, g_pScene);
	AddDebugPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &g_ForwardTechnique, &g_Viewport, g_pScene);
	AddMCNKPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &g_ForwardTechnique, &g_Viewport, g_pScene);
	AddWMOPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &g_ForwardTechnique, &g_Viewport, g_pScene);
	AddLiquidPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &g_ForwardTechnique, &g_Viewport, g_pScene);
	AddM2Passes(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &g_ForwardTechnique, &g_Viewport, g_pScene);

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
}


//
//
//
void CGameState_World::OnPreRender(RenderEventArgs& e)
{
	g_pFrameQuery->Begin(e.FrameCounter);

	ADT_WMO_Instance::reset();
	ADT_MDX_Instance::reset();
}

void CGameState_World::OnRender(RenderEventArgs& e)
{
	e.Camera = g_CameraController->GetCameraConst().operator->(); // TODO: Shit code. Refactor me.

	g_ForwardTechnique.Render(e);
}

void CGameState_World::OnPostRender(RenderEventArgs& e)
{
	g_pFrameQuery->End(e.FrameCounter);


	// Retrieve GPU timer results.
	// Don't retrieve the immediate query result, but from the previous frame.
	// Checking previous frame counters will alleviate GPU stalls.
	Query::QueryResult frameResult = g_pFrameQuery->GetQueryResult(e.FrameCounter - (g_pFrameQuery->GetBufferCount() - 1));
	if (frameResult.IsValid)
	{
		// Frame time in milliseconds
#ifdef  IS_DX11
		g_FrameTime = frameResult.ElapsedTime * 1000.0;
#else
		g_FrameTime = frameResult.ElapsedTime / 1000000.0;
#endif

		std::string title = std::to_string(g_FrameTime);
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
	e.Viewport = &g_Viewport;

	g_UITechnique.Render(e);
}