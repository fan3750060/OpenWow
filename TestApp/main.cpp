#include "stdafx.h"

// Additional
#include <ctime>

Camera g_Camera;
glm::vec2 g_PreviousMousePosition;

struct CameraMovement
{
	// Translation movement
	float Forward, Back, Left, Right, Up, Down;
	// Rotation movement
	float RollCW, RollCCW;
	float Pitch, Yaw;
	// Move in/out from pivot point.
	float PivotTranslate;
	// Do you want to go faster?
	bool TranslateFaster;
	bool RotateFaster;

	CameraMovement()
		: Forward(0.0f)
		, Back(0.0f)
		, Left(0.0f)
		, Right(0.0f)
		, Up(0.0f)
		, Down(0.0f)
		, RollCW(0.0f)
		, RollCCW(0.0f)
		, Pitch(0.0f)
		, Yaw(0.0f)
		, PivotTranslate(0.0f)
		, TranslateFaster(false)
		, RotateFaster(false)
	{}
};
CameraMovement g_CameraMovement;


std::shared_ptr<Scene> g_pScene = nullptr;


std::shared_ptr<Query> g_pFrameQuery;
double g_FrameTime = 0.0;

std::shared_ptr<Shader> g_pPixelShader;


//--

std::shared_ptr<PipelineState> g_pOpaquePipeline;
std::shared_ptr<PipelineState> g_pTransparentPipeline;

//--

std::shared_ptr<SamplerState> g_LinearClampSampler;
std::shared_ptr<SamplerState> g_LinearRepeatSampler;

//--

void OnUpdate(UpdateEventArgs& e);

void OnMouseButtonPressed(MouseButtonEventArgs& e);
void OnMouseButtonReleased(MouseButtonEventArgs& e);
void OnMouseMoved(MouseMotionEventArgs& e);

void OnPreRender(RenderEventArgs& e);
void OnRender(RenderEventArgs& e);
void OnPostRender(RenderEventArgs& e);

void OnKeyPressed(KeyEventArgs& e);
void OnKeyReleased(KeyEventArgs& e);

// Techique
enum class RenderingTechnique
{
	Forward,
	Deferred,
	ForwardPlus
};
RenderingTechnique g_RenderingTechnique = RenderingTechnique::Forward;
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
		{
			Log::Info("Process priority class set to HIGH");
		}
		else
		{
			Log::Error("Can't set process priority class.");
		}

		CMPQArchiveManager mpqArchiveManager;
		CFilesManager filesManager;

		Application app;

		RenderWindow* renderWindow = app.CreateRenderWindow("Name", 1024, 768);

		// Register initialize/terminate events.
		app.Initialize += boost::bind(&RenderWindow::OnInitialize, renderWindow, _1);
		app.Terminate += boost::bind(&RenderWindow::OnTerminate, renderWindow, _1);
		app.Update += boost::bind(&RenderWindow::OnUpdate, renderWindow, _1);


		renderWindow->ShowWindow();

		renderWindow->Update += &OnUpdate;
		renderWindow->MouseButtonPressed += &OnMouseButtonPressed;
		renderWindow->MouseButtonReleased += &OnMouseButtonReleased;
		renderWindow->MouseMoved += &OnMouseMoved;
		renderWindow->PreRender += &OnPreRender;
		renderWindow->Render += &OnRender;
		renderWindow->PostRender += &OnPostRender;
		renderWindow->KeyPressed += &OnKeyPressed;
		renderWindow->KeyReleased += &OnKeyReleased;

		RenderDevice* renderDevice = app.GetRenderDevice();

		std::shared_ptr<Texture> tt = renderDevice->CreateTexture("Textures\\Minimap\\00e1cf872b32bc88dd2a0e2f47b72890.blp");
		
		g_pScene = std::make_shared<SceneBase>();

		std::shared_ptr<Mesh> mesh = renderDevice->CreateSphere(5.0f);
		std::shared_ptr<Mesh> mesh2 = renderDevice->CreatePlane(5.0f);
		std::shared_ptr<Mesh> mesh3 = renderDevice->CreateCube(5.0f);
		mesh2->GetMaterial()->SetTexture(Material::TextureType::Diffuse, tt);


		std::shared_ptr<SceneNode> n0 = g_pScene->CreateSceneNode();
		n0->AddMesh(mesh);

		std::shared_ptr<SceneNode> n1 = g_pScene->CreateSceneNode();
		mat4 world;
		world = glm::scale(vec3(5.0f));

		n1->SetLocalTransform(world);
		n1->AddMesh(mesh2);

		std::shared_ptr<SceneNode> n2 = g_pScene->CreateSceneNode();
		n2->AddMesh(mesh3);

		//g_pFrameQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 2);

		// CAMERA
		g_Camera.SetTranslate(vec3(0, 0, 0));
		g_Camera.SetRotate(vec3(0, 0, 0));
		g_Camera.SetViewport(Viewport(0, 0, 1024.0f, 768.0f));
		g_Camera.SetProjectionRH(45.0f, 1024.0f / 768.0f, 0.1f, 1000.0f);

		//g_pForwardOpaqueQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 2);
		//g_pForwardTransparentQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 2);

		// SHADERS
		std::shared_ptr<Shader> g_pVertexShader = renderDevice->CreateShader();
		g_pVertexShader->LoadShaderFromFile(Shader::VertexShader, "shaders/Debug/Debug.vs", Shader::ShaderMacros(), "VS_main", "latest");
		g_pPixelShader = renderDevice->CreateShader();
		g_pPixelShader->LoadShaderFromFile(Shader::PixelShader, "shaders/Debug/Debug.fs", Shader::ShaderMacros(), "PS_main", "latest");

		// SAMPLER

		// Create samplers
		g_LinearRepeatSampler = renderDevice->CreateSamplerState();
		g_LinearRepeatSampler->SetFilter(SamplerState::MinFilter::MinLinear, SamplerState::MagFilter::MagLinear, SamplerState::MipFilter::MipLinear);

		g_LinearClampSampler = renderDevice->CreateSamplerState();
		g_LinearClampSampler->SetFilter(SamplerState::MinFilter::MinLinear, SamplerState::MagFilter::MagLinear, SamplerState::MipFilter::MipLinear);
		g_LinearClampSampler->SetWrapMode(SamplerState::WrapMode::Clamp, SamplerState::WrapMode::Clamp, SamplerState::WrapMode::Clamp);

		// STATES
		BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
		DepthStencilState::DepthMode disableDepthWrites(true, DepthStencilState::DepthWrite::Disable);

		// PIPELINES
		g_pOpaquePipeline = renderDevice->CreatePipelineState();
		g_pOpaquePipeline->SetShader(Shader::VertexShader, g_pVertexShader);
		g_pOpaquePipeline->SetShader(Shader::PixelShader, g_pPixelShader);
		g_pOpaquePipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::None);
		//g_pOpaquePipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Wireframe);
		g_pOpaquePipeline->SetRenderTarget(renderWindow->GetRenderTarget());

		/*g_pTransparentPipeline = renderDevice->CreatePipelineState();
		g_pTransparentPipeline->SetShader(Shader::VertexShader, g_pVertexShader);
		g_pTransparentPipeline->SetShader(Shader::PixelShader, g_pPixelShader);
		g_pTransparentPipeline->GetBlendState().SetBlendMode(alphaBlending);
		g_pTransparentPipeline->GetDepthStencilState().SetDepthMode(disableDepthWrites);
		g_pTransparentPipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::None);
		g_pTransparentPipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Wireframe);
		g_pTransparentPipeline->SetRenderTarget(renderWindow->GetRenderTarget());*/



		// Add a pass to render opaque geometry.
		g_ForwardTechnique.AddPass(std::make_shared<ClearRenderTargetPass>(renderWindow->GetRenderTarget(), ClearFlags::All, g_ClearColor, 1.0f, 0));
		g_ForwardTechnique.AddPass(std::make_shared<BeginQueryPass>(g_pForwardOpaqueQuery));
		//g_ForwardTechnique.AddPass(std::make_shared<OpaquePass>(g_pScene, g_pOpaquePipeline));
		g_ForwardTechnique.AddPass(std::make_shared<EndQueryPass>(g_pForwardOpaqueQuery));
		
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

void OnUpdate(UpdateEventArgs& e)
{
	float moveMultiplier = (g_CameraMovement.TranslateFaster) ? 3 : 2;
	float rotateMultiplier = (g_CameraMovement.RotateFaster) ? 3 : 2;

	g_Camera.TranslateX((g_CameraMovement.Right - g_CameraMovement.Left) * e.ElapsedTime * moveMultiplier);
	g_Camera.TranslateY((g_CameraMovement.Up - g_CameraMovement.Down) * e.ElapsedTime * moveMultiplier);
	g_Camera.TranslateZ((g_CameraMovement.Back - g_CameraMovement.Forward) * e.ElapsedTime * moveMultiplier);
	g_Camera.AddPitch(g_CameraMovement.Pitch * 60.0f * e.ElapsedTime * rotateMultiplier, Camera::Space::Local);
	g_Camera.AddYaw(g_CameraMovement.Yaw * 60.0f * e.ElapsedTime * rotateMultiplier, Camera::Space::World);
}

void OnPreRender(RenderEventArgs& e)
{
	//g_pFrameQuery->Begin(e.FrameCounter);

	g_pPixelShader->GetShaderParameterByName("DiffuseSampler").Set(g_LinearRepeatSampler);
}

void OnRender(RenderEventArgs& e)
{
	// Render the scene from the perspective of this camera.
	e.Camera = &g_Camera;

	g_pOpaquePipeline->GetRasterizerState().SetViewport(Viewport(0, 0, 1024.0f, 768.0f));
	//g_pTransparentPipeline->GetRasterizerState().SetViewport(Viewport(0, 0, 1024.0f, 768.0f));

	switch (g_RenderingTechnique)
	{
	case RenderingTechnique::Forward:
		g_ForwardTechnique.Render(e);
		break;
	}


	// Generate light picking texture
	// This is only done when the mouse is clicked.
	// @see: OnMouseButtonReleased
	//g_LightPickingTechnique.Render( e );
}

void OnPostRender(RenderEventArgs& e)
{
	//g_pFrameQuery->End(e.FrameCounter);

	RenderWindow& renderWindow = dynamic_cast<RenderWindow&>(const_cast<Object&>(e.Caller));
	renderWindow.Present();

	// Retrieve GPU timer results.
	// Don't retrieve the immediate query result, but from the previous frame.
	// Checking previous frame counters will alleviate GPU stalls.
	/*Query::QueryResult frameResult = g_pFrameQuery->GetQueryResult(e.FrameCounter - (g_pFrameQuery->GetBufferCount() - 1));
	if (frameResult.IsValid)
	{
		// Frame time in milliseconds
		g_FrameTime = frameResult.ElapsedTime * 1000.0;
	}*/

	switch (g_RenderingTechnique)
	{
	case RenderingTechnique::Forward:
	{
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
	break;
	}

}

//--------------------------------------------

void OnKeyPressed(KeyEventArgs& e)
{
	switch (e.Key)
	{
	case KeyCode::W:
	{
		g_CameraMovement.Forward = 1.0f;
	}
	break;
	case KeyCode::A:
	{
		g_CameraMovement.Left = 1.0f;
	}
	break;
	case KeyCode::S:
	{
		g_CameraMovement.Back = 1.0f;
	}
	break;
	case KeyCode::D:
	{
		g_CameraMovement.Right = 1.0f;
	}
	break;
	case KeyCode::Q:
	{
		g_CameraMovement.Down = 1.0f;
	}
	break;
	case KeyCode::E:
	{
		g_CameraMovement.Up = 1.0f;
	}
	break;
	}

}

void OnKeyReleased(KeyEventArgs& e)
{
	switch (e.Key)
	{
	case KeyCode::W:
	{
		g_CameraMovement.Forward = 0.0f;
	}
	break;
	case KeyCode::A:
	{
		g_CameraMovement.Left = 0.0f;
	}
	break;
	case KeyCode::S:
	{
		g_CameraMovement.Back = 0.0f;
	}
	break;
	case KeyCode::D:
	{
		g_CameraMovement.Right = 0.0f;
	}
	break;
	case KeyCode::Q:
	{
		g_CameraMovement.Down = 0.0f;
	}
	break;
	case KeyCode::E:
	{
		g_CameraMovement.Up = 0.0f;
	}
	break;
	case KeyCode::R:
	{
		g_CameraMovement.RollCW = 0.0f;
	}
	break;
	case KeyCode::F:
	{
		g_CameraMovement.RollCCW = 0.0f;
	}
	break;
	case KeyCode::ShiftKey:
	{
		g_CameraMovement.TranslateFaster = false;
		g_CameraMovement.RotateFaster = false;
	}
	break;
	}
}

void OnMouseButtonPressed(MouseButtonEventArgs& e)
{
	g_Camera.OnMousePressed(e);

	g_PreviousMousePosition = glm::vec2(e.X, e.Y);
}

void OnMouseButtonReleased(MouseButtonEventArgs& e)
{
	glm::vec2 currentMousePosition = glm::vec2(e.X, e.Y);
	float offset = glm::distance(g_PreviousMousePosition, currentMousePosition);
}

void OnMouseMoved(MouseMotionEventArgs& e)
{
	if (e.LeftButton)
	{
		g_Camera.OnMouseMoved(e);
	}
}