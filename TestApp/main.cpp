#include "stdafx.h"

// Additional
#include <ctime>


#include "CamContrTEMP.h"


extern Camera g_Camera;

std::shared_ptr<Scene> g_pScene = nullptr;

RenderWindow* g_pRenderWindow;
std::shared_ptr<Query> g_pFrameQuery;
double g_FrameTime = 0.0;

std::shared_ptr<Shader> g_pPixelShader;


//--

std::shared_ptr<PipelineState> g_pTransparentPipeline;

//--

std::shared_ptr<SamplerState> g_LinearClampSampler;
std::shared_ptr<SamplerState> g_LinearRepeatSampler;

//--

void OnPreRender(RenderEventArgs& e);
void OnRender(RenderEventArgs& e);
void OnPostRender(RenderEventArgs& e);


// Techique
RenderTechnique g_ForwardTechnique;

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

		CBindingController bindingController;
		_Bindings = &bindingController;

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

		g_pRenderWindow = app.CreateRenderWindow("Name", 1024, 768);

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
		g_pRenderWindow->KeyPressed += &OnKeyPressed;
		g_pRenderWindow->KeyReleased += &OnKeyReleased;

		RenderDevice* renderDevice = app.GetRenderDevice();


		//std::shared_ptr<Texture> t0 = renderDevice->CreateTexture2D("Textures\\ShaneCube.blp"); // DXT1
		//std::shared_ptr<Texture> t1 = renderDevice->CreateTexture2D("Textures\\Minimap\\00b445de1413eeca80cc683deb9af58b.blp"); // DXT1A
		//std::shared_ptr<Texture> t2 = renderDevice->CreateTexture2D("Textures\\MinimapMask.blp"); // DXT3
		std::shared_ptr<Texture> t3 = renderDevice->CreateTexture2D("Textures\\SpellChainEffects\\VR_Chain.blp"); // DXT5

		std::shared_ptr<Texture> t0 = renderDevice->CreateTexture2D("Textures\\Moon02Glare.blp"); // RAW0
		std::shared_ptr<Texture> t1 = renderDevice->CreateTexture2D("Textures\\ShadowBlob.blp"); // RAW1
		std::shared_ptr<Texture> t2 = renderDevice->CreateTexture2D("Textures\\moon.blp"); // RAW8

		std::shared_ptr<Texture> t30 = renderDevice->CreateTexture2D("Textures\\SunGlare.blp"); // PURE


		renderDevice->CreateTexture2D("TILESET\\Barrens\\BarrensBaseBush.blp");

		// Material
		std::shared_ptr<MaterialBase> mat = std::make_shared<MaterialBase>(renderDevice);
		mat->SetTexture(MaterialBase::TextureType::Diffuse, t30); // DXT1

		std::shared_ptr<Mesh> mesh1 = renderDevice->CreateSphere();
		mesh1->SetMaterial(mat);
		std::shared_ptr<Mesh> mesh2 = renderDevice->CreateCube();
		mesh2->SetMaterial(mat);
		std::shared_ptr<Mesh> mesh3 = renderDevice->CreatePlane();
		mesh3->SetMaterial(mat);
		std::shared_ptr<Mesh> mesh4 = renderDevice->CreatePlane();
		mesh4->SetMaterial(mat);
		std::shared_ptr<Mesh> mesh5 = renderDevice->CreatePlane();
		mesh5->SetMaterial(mat);

		g_pScene = std::make_shared<SceneBase>();

		/*std::shared_ptr<SceneNode> n1 = g_pScene->CreateSceneNode();
		n1->SetLocalTransform(glm::translate(vec3(-100.0f, 0.0f, 0.0f)) * glm::scale(vec3(15.0f)));
		n1->AddMesh(mesh1);

		std::shared_ptr<SceneNode> n2 = g_pScene->CreateSceneNode();
		n2->SetLocalTransform(glm::translate(vec3(-50.0f, 0.0f, 0.0f)) * glm::scale(vec3(15.0f)));
		n2->AddMesh(mesh2);

		std::shared_ptr<SceneNode> n3 = g_pScene->CreateSceneNode();
		n3->SetLocalTransform(glm::translate(vec3(0.0f, 0.0f, 0.0f)) * glm::scale(vec3(15.0f)));
		n3->AddMesh(mesh3);

		std::shared_ptr<SceneNode> n4 = g_pScene->CreateSceneNode();
		n4->SetLocalTransform(glm::translate(vec3(50.0f, 0.0f, 0.0f)) * glm::scale(vec3(15.0f)));
		n4->AddMesh(mesh4);

		std::shared_ptr<SceneNode> n5 = g_pScene->CreateSceneNode();
		n5->SetLocalTransform(glm::translate(vec3(100.0f, 0.0f, 0.0f)) * glm::scale(vec3(15.0f)));
		n5->AddMesh(mesh5);*/

		OpenDBs();

		std::shared_ptr<MapController> contr = std::make_shared<MapController>();
		contr->SetParent(g_pScene->GetRootNode());

		contr->MapPreLoad(*DBC_Map[1]);
		contr->MapLoad();
		contr->MapPostLoad();

		contr->EnterMap(30, 30);


		Viewport viewPort(0, 0, 1024.0f, 768.0f);

		g_pFrameQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 1);

		// CAMERA
		g_Camera.SetTranslate(vec3(0, 0, 0));
		g_Camera.SetTranslate(vec3(30 * C_TileSize, 200, 30 * C_TileSize));
		g_Camera.SetRotate(vec3(0, 0, 0));
		g_Camera.SetViewport(viewPort);
		g_Camera.SetProjectionRH(45.0f, 1024.0f / 768.0f, 0.1f, 10000.0f);

		g_pForwardOpaqueQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 2);
		g_pForwardTransparentQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 2);

		// SHADERS
#ifdef  IS_DX11
		std::shared_ptr<Shader> g_pVertexShader = renderDevice->CreateShader();
		g_pVertexShader->LoadShaderFromFile(Shader::VertexShader, "shaders_D3D/DefaultShader.hlsl", Shader::ShaderMacros(), "VS_main", "latest");
		g_pPixelShader = renderDevice->CreateShader();
		g_pPixelShader->LoadShaderFromFile(Shader::PixelShader, "shaders_D3D/DefaultShader.hlsl", Shader::ShaderMacros(), "PS_main", "latest");
#else
		std::shared_ptr<Shader> g_pVertexShader = renderDevice->CreateShader();
		g_pVertexShader->LoadShaderFromFile(Shader::VertexShader, "shaders_OGL/Debug/Debug.vs", Shader::ShaderMacros(), "VS_main", "latest");
		g_pPixelShader = renderDevice->CreateShader();
		g_pPixelShader->LoadShaderFromFile(Shader::PixelShader, "shaders_OGL/Debug/Debug.fs", Shader::ShaderMacros(), "PS_main", "latest");
#endif

		// SAMPLER

		// Add a pass to render opaque geometry.
		g_ForwardTechnique.AddPass(std::make_shared<ClearRenderTargetPass>(g_pRenderWindow->GetRenderTarget(), ClearFlags::All, g_ClearColor, 1.0f, 0));
		AddMapPasses(renderDevice, g_pRenderWindow, &g_ForwardTechnique, &viewPort, g_pScene);

		// Add a pass for rendering transparent geometry
		//g_ForwardTechnique.AddPass(std::make_shared<BeginQueryPass>(g_pForwardTransparentQuery));
		//g_ForwardTechnique.AddPass(std::make_shared<TransparentPass>(g_pScene, g_pTransparentPipeline));
		//g_ForwardTechnique.AddPass(std::make_shared<EndQueryPass>(g_pForwardTransparentQuery));

		// Add a pass to render the lights in the scene as opaque geometry. Can be toggled with 'l' key.
		//std::shared_ptr<LightsPass> g_LightsPassFront = std::make_shared<LightsPass>(g_Config.Lights, g_Sphere, g_Cone, g_Arrow, g_pLightsPipelineFront);
		//std::shared_ptr<LightsPass> g_LightsPassBack = std::make_shared<LightsPass>(g_Config.Lights, g_Sphere, g_Cone, g_Arrow, g_pLightsPipelineBack);
		//g_ForwardTechnique.AddPass(g_LightsPassBack);
		//g_ForwardTechnique.AddPass(g_LightsPassFront);

		app.Run();
	}

	return 0;
}

//--------------------------------------------



void OnPreRender(RenderEventArgs& e)
{
	g_pFrameQuery->Begin(e.FrameCounter);

#ifdef  IS_DX11
	//g_pPixelShader->GetShaderParameterByName("DiffuseSampler").Set(g_LinearRepeatSampler);
#else
	//g_pPixelShader->GetShaderParameterByName("DiffuseTexture").Set(g_LinearRepeatSampler);
#endif
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

	RenderWindow& renderWindow = dynamic_cast<RenderWindow&>(const_cast<Object&>(e.Caller));
	renderWindow.Present();

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
		g_pRenderWindow->SetWindowName(std::to_string(g_FrameTime));
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
