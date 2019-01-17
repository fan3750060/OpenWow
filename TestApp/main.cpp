#include "stdafx.h"

// Additional
#include <ctime>


#include "CamContrTEMP.h"


#include "UI_Color_Material.h"

extern Camera g_Camera;
Viewport g_Viewport;

std::shared_ptr<Scene> g_pScene = nullptr;
std::shared_ptr<UIScene> g_pUIScene = nullptr;

RenderWindow* g_pRenderWindow;
std::shared_ptr<Query> g_pFrameQuery;
double g_FrameTime = 0.0;
std::shared_ptr<MapController> contr;
//--

void OnPreRender(RenderEventArgs& e);
void OnRender(RenderEventArgs& e);
void OnPostRender(RenderEventArgs& e);
void OnRenderUI(RenderUIEventArgs& e);

// Techique
RenderTechnique g_ForwardTechnique;
RenderUITechnique g_UITechnique;

std::shared_ptr<Query> g_pForwardOpaqueQuery;
std::shared_ptr<Query> g_pForwardTransparentQuery;

glm::vec4 g_ClearColor(0.39f, 0.58f, 0.93f, 1.0f);



int main(int argumentCount, char* arguments[])
{
#ifdef _DEBUG 
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif
	{
		Random::SetSeed(static_cast<unsigned long>(time(0)));

		CBaseManager baseManager;
		_BaseManager = &baseManager;

		CSettings settings;
		settings.AddDefaults();

		CLog log;

		CConsole console;
		console.AddCommonCommands();

		HANDLE hProcess = GetCurrentProcess();
		if (SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS))
			Log::Info("Process priority class set to HIGH");
		else
			Log::Error("Can't set process priority class.");

		CMPQArchiveManager mpqArchiveManager;
		CFilesManager filesManager;





		Application app;

		g_pRenderWindow = app.CreateRenderWindow("Name", 1280, 1024);

		// Register initialize/terminate events.
		app.Initialize += boost::bind(&RenderWindow::OnInitialize, g_pRenderWindow, _1);
		app.Terminate += boost::bind(&RenderWindow::OnTerminate, g_pRenderWindow, _1);
		app.Update += boost::bind(&RenderWindow::OnUpdate, g_pRenderWindow, _1);


		g_pRenderWindow->ShowWindow();

		g_pRenderWindow->Update += &OnUpdate;
		g_pRenderWindow->MouseButtonPressed += &OnMouseButtonPressed;
		g_pRenderWindow->MouseButtonReleased += &OnMouseButtonReleased;
		g_pRenderWindow->MouseMoved += &OnMouseMoved;
		g_pRenderWindow->MouseWheel += &OnMouseWheel;
		g_pRenderWindow->PreRender += &OnPreRender;
		g_pRenderWindow->Render += &OnRender;
		g_pRenderWindow->PostRender += &OnPostRender;
		g_pRenderWindow->RenderUI += &OnRenderUI;
		g_pRenderWindow->KeyPressed += &OnKeyPressed;
		g_pRenderWindow->KeyReleased += &OnKeyReleased;

		IRenderDevice* renderDevice = app.GetRenderDevice();


		/*renderDevice->CreateTexture2D("Textures\\ShaneCube.blp"); // DXT1
		renderDevice->CreateTexture2D("Textures\\Minimap\\00b445de1413eeca80cc683deb9af58b.blp"); // DXT1A
		renderDevice->CreateTexture2D("Textures\\MinimapMask.blp"); // DXT3
		renderDevice->CreateTexture2D("Textures\\SpellChainEffects\\VR_Chain.blp"); // DXT5
		renderDevice->CreateTexture2D("Textures\\Moon02Glare.blp"); // RAW0
		renderDevice->CreateTexture2D("Textures\\ShadowBlob.blp"); // RAW1
		renderDevice->CreateTexture2D("Textures\\moon.blp"); // RAW8
		renderDevice->CreateTexture2D("Textures\\SunGlare.blp"); // PURE*/

		g_pScene = std::make_shared<SceneBase>();
		g_pUIScene = std::make_shared<UISceneBase>();

		OpenDBs();

		const float x = 36;
		const float y = 32;

		new FontsManager(_RenderDevice);
		new WMOsManager();
		new CM2_Manager();

		//contr = std::make_shared<MapController>();
		//contr->SetParent(g_pScene->GetRootNode());
		//contr->MapPreLoad(*DBC_Map[1/*571*/]);
		//contr->MapLoad();
		//contr->MapPostLoad();
		//contr->EnterMap(x, y);

		//std::shared_ptr<M2> model = GetManager<IM2Manager>()->Add("creature\\PHOENIX\\Phoenix.m2");
		//std::shared_ptr<CM2_Base_Instance> inst = std::make_shared<CM2_Base_Instance>(model);
		//inst->CreateInstances();
		//inst->SetParent(g_pScene->GetRootNode());


		Viewport viewPort(0, 0, 1280.0f, 1024.0f);
		g_Viewport = viewPort;

		g_pFrameQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 1);

		// CAMERA
		g_Camera.SetTranslate(vec3(0, 0, 0));
		g_Camera.SetTranslate(vec3(x * C_TileSize, 200, y * C_TileSize));
		g_Camera.SetRotate(vec3(0, 0, 0));
		g_Camera.SetViewport(viewPort);
		g_Camera.SetProjectionRH(45.0f, 1280.0f / 1024.0f, 0.5f, 2000.0f);

		g_pForwardOpaqueQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 2);
		g_pForwardTransparentQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 2);

		g_ForwardTechnique.AddPass(std::make_shared<ClearRenderTargetPass>(g_pRenderWindow->GetRenderTarget(), ClearFlags::All, g_ClearColor, 1.0f, 0));
		AddSkyPasses(renderDevice, g_pRenderWindow, &g_ForwardTechnique, &viewPort, g_pScene);
		AddWDLPasses(renderDevice, g_pRenderWindow, &g_ForwardTechnique, &viewPort, g_pScene);
		AddMCNKPasses(renderDevice, g_pRenderWindow, &g_ForwardTechnique, &viewPort, g_pScene);
		AddWMOPasses(renderDevice, g_pRenderWindow, &g_ForwardTechnique, &viewPort, g_pScene);
		AddLiquidPasses(renderDevice, g_pRenderWindow, &g_ForwardTechnique, &viewPort, g_pScene);
		AddM2Passes(renderDevice, g_pRenderWindow, &g_ForwardTechnique, &viewPort, g_pScene);

		CharacterTemplate tempPala;
		tempPala.TemplateFillDefaultPaladin();

		CharacterTemplate tempShaman;
		tempShaman.TemplateFillDefaultShaman();

		std::shared_ptr<Character> character  = std::make_shared<Character>();
		character->InitFromTemplate(tempPala);
		character->CreateInstances();
		character->SetParent(g_pScene->GetRootNode());
		character->SetTranslate(vec3(x * C_TileSize, 100, y * C_TileSize));
		character->SetScale(vec3(10.0f));
		character->GetLocalTransform();

		//
		// UI
		//

		std::vector<vec2> vecrtices;
		vecrtices.push_back(vec2(-1.0f, -1.0f));
		vecrtices.push_back(vec2(1.0f, -1.0f));
		vecrtices.push_back(vec2(-1.0f, 1.0f));
		vecrtices.push_back(vec2(1.0f, 1.0f));
		std::shared_ptr<Buffer> __vb = _RenderDevice->CreateVertexBuffer(vecrtices);

		uint16 indexes[6] = { 0, 1, 2, 2, 1, 3 };
		std::shared_ptr<Buffer> __ib = _RenderDevice->CreateIndexBuffer(indexes, 6);

		std::shared_ptr<UI_Color_Material> uiMaterial = std::make_shared<UI_Color_Material>();
		uiMaterial->SetColor(vec4(0, 1, 0, 0.3f));

		std::shared_ptr<IMesh> __geom = _RenderDevice->CreateMesh();
		__geom->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);
		__geom->SetIndexBuffer(__ib);
		__geom->SetMaterial(uiMaterial);

		// FONT BEGIN
		std::shared_ptr<Font> defFont = GetManager<IFontsManager>()->GetMainFont();
		// FONT END


		std::shared_ptr<UINode> node = std::make_shared<UINode>();
		node->SetMesh(defFont);
		node->SetTranslate(vec2(80.0f, 80.0f));
		//node->SetScale(vec2(10.0f, 10.0f));
		node->GetLocalTransform();
		node->SetParent(g_pUIScene->GetRootNode());

		BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendOperation::Add, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
		BlendState::BlendMode disableBlending;
		DepthStencilState::DepthMode enableDepthWrites(true, DepthStencilState::DepthWrite::Enable);
		DepthStencilState::DepthMode disableDepthWrites(false, DepthStencilState::DepthWrite::Disable);

		std::shared_ptr<PipelineState> UIPipeline = renderDevice->CreatePipelineState();
		UIPipeline->GetBlendState().SetBlendMode(alphaBlending);
		UIPipeline->GetDepthStencilState().SetDepthMode(enableDepthWrites);
		UIPipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::None);
		UIPipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
		UIPipeline->SetRenderTarget(g_pRenderWindow->GetRenderTarget());
		UIPipeline->GetRasterizerState().SetViewport(g_Viewport);

		g_UITechnique.SetPass(std::make_shared<BaseUIPass>(g_pUIScene, UIPipeline));

		app.Run();
	}

	return 0;
}

//--------------------------------------------



void OnPreRender(RenderEventArgs& e)
{
	g_pFrameQuery->Begin(e.FrameCounter);

	ADT_WMO_Instance::reset();
	ADT_MDX_Instance::reset();
}

void OnRender(RenderEventArgs& e)
{
	// Render the scene from the perspective of this camera.
	e.Camera = &g_Camera;

	g_ForwardTechnique.Render(e);
}

void OnPostRender(RenderEventArgs& e)
{
	g_pFrameQuery->End(e.FrameCounter);

	//RenderWindow& renderWindow = dynamic_cast<RenderWindow&>(const_cast<Object&>(e.Caller));
	//renderWindow.Present();

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
		if (contr != nullptr)
			title += "_" + std::to_string(contr->GetCurrentX()) + "_" + std::to_string(contr->GetCurrentZ());

		g_pRenderWindow->SetWindowName(title);
	}

	// Query results for forward rendering technique.
	//Query::QueryResult forwardOpaqueResult = g_pForwardOpaqueQuery->GetQueryResult(e.FrameCounter - (g_pForwardOpaqueQuery->GetBufferCount() - 1));
	//Query::QueryResult forwardTransparentResult = g_pForwardTransparentQuery->GetQueryResult(e.FrameCounter - (g_pForwardTransparentQuery->GetBufferCount() - 1));

	/*if (forwardOpaqueResult.IsValid)
	{
		g_ForwardOpaqueStatistic.Sample(forwardOpaqueResult.ElapsedTime * 1000.0);
	}
	if (forwardTransparentResult.IsValid)
	{
		g_ForwardTransparentStatistic.Sample(forwardTransparentResult.ElapsedTime * 1000.0);
	}*/
}

void OnRenderUI(RenderUIEventArgs& e)
{
	e.Viewport = &g_Viewport;

	g_UITechnique.Render(e);
}
