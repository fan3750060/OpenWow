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


	Application& app = Application::Get();
	std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();

	//
	// Camera controller
	//
	m_CameraController = std::make_shared<CCameraController>();
	m_CameraController->Init(app.GetRenderWindow());
	m_CameraController->GetCamera()->SetTranslate(vec3(0, 0, 0));
	m_CameraController->GetCamera()->SetRotate(vec3(0, 0, 0));
	m_CameraController->GetCamera()->SetViewport(m_Viewport);
	m_CameraController->GetCamera()->SetProjectionRH(45.0f, 1280.0f / 1024.0f, 1.0f, 4000.0f);
	
	m_FrameQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 1);

	//m_3DTechnique.AddPass(std::make_shared<ClearRenderTargetPass>(app.GetRenderWindow()->GetRenderTarget(), ClearFlags::All, g_ClearColor, 1.0f, 0));

	LoadGBuffer();
	Load3D();
	LoadUI();

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
	Application::Get().GetLoader()->SetCamera(m_CameraController->GetCamera());

	m_3DDeferredTechnique.Render(e);
}

void CGameState_World::OnPostRender(Render3DEventArgs& e)
{
	m_FrameQuery->End(e.FrameCounter);

	vec3 cameraTrans = m_CameraController->GetCamera()->GetTranslation();
	m_CameraPosText->SetText("Pos: " + std::to_string(cameraTrans.x) + ", " + std::to_string(cameraTrans.y) + ", " + std::to_string(cameraTrans.z));

	vec3 cameraRot = m_CameraController->GetCamera()->GetEulerAngles();
	m_CameraRotText->SetText("Rot: " + std::to_string(cameraRot.x) + ", " + std::to_string(cameraRot.y) + ", " + std::to_string(cameraRot.z));

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
}

void CGameState_World::OnRenderUI(RenderUIEventArgs& e)
{
	e.Viewport = &m_Viewport;

	//m_UITechnique.RenderUI(e);
}

//
//
//

void CGameState_World::LoadGBuffer()
{
	Application& app = Application::Get();
	std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();
	std::shared_ptr<RenderWindow> renderWindow = app.GetRenderWindow();

	uint32 numSamples = 1;

	// Position (Color0) 
	Texture::TextureFormat positionTextureFormat(
		Texture::Components::RGBA,
		Texture::Type::UnsignedNormalized,
		numSamples,
		8, 8, 8, 8, 0, 0);
	std::shared_ptr<Texture> positionTexture = renderDevice->CreateTexture2D(c_WindowsWidth, c_WindowsHeight, 1, positionTextureFormat);

	// Diffuse buffer (Color1)
	Texture::TextureFormat duffuseTextureFormat(
		Texture::Components::RGBA,
		Texture::Type::UnsignedNormalized,
		numSamples,
		8, 8, 8, 8, 0, 0);
	std::shared_ptr<Texture> duffuseTexture = renderDevice->CreateTexture2D(c_WindowsWidth, c_WindowsHeight, 1, duffuseTextureFormat);

	// Specular buffer (Color2)
	Texture::TextureFormat specularTextureFormat(
		Texture::Components::RGBA,
		Texture::Type::UnsignedNormalized,
		numSamples,
		8, 8, 8, 8, 0, 0);
	std::shared_ptr<Texture> specularTexture = renderDevice->CreateTexture2D(c_WindowsWidth, c_WindowsHeight, 1, specularTextureFormat);

	// Normal buffer (Color3)
	Texture::TextureFormat normalTextureFormat(
		Texture::Components::RGBA,
		Texture::Type::Float,
		numSamples,
		32, 32, 32, 32, 0, 0);
	std::shared_ptr<Texture> normalTexture = renderDevice->CreateTexture2D(c_WindowsWidth, c_WindowsHeight, 1, normalTextureFormat);

	// Depth/stencil buffer
	Texture::TextureFormat depthStencilTextureFormat(
		Texture::Components::DepthStencil,
		Texture::Type::UnsignedNormalized,
		numSamples,
		0, 0, 0, 0, 24, 8);
	std::shared_ptr<Texture> depthStencilTexture = renderDevice->CreateTexture2D(c_WindowsWidth, c_WindowsHeight, 1, depthStencilTextureFormat);

	// Create a render target for the geometry pass.
	m_GBufferRenderTarget = renderDevice->CreateRenderTarget();

	// Use the render window's color attachment point for the "light accumulation" texture (no reason to have an additional buffer for this, that I'm aware of..)
	m_GBufferRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color0, positionTexture/*renderWindow->GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)*/);
	m_GBufferRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color1, duffuseTexture);
	m_GBufferRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color2, specularTexture);
	m_GBufferRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color3, normalTexture);
	m_GBufferRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, depthStencilTexture /*renderWindow->GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil)*/);
}

void CGameState_World::Load3D()
{
	Application& app = Application::Get();
	std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();
	std::shared_ptr<RenderWindow> renderWindow = app.GetRenderWindow();

	const float x = 40;
	const float y = 29;
	//const float x = 29;
	//const float y = 21;

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
	
	m_CameraController->GetCamera()->SetTranslate(vec3(x * C_TileSize, 200, y * C_TileSize));
	//m_CameraController->GetCamera()->SetTranslate(vec3(20873, 229, 16601));
	m_CameraController->GetCamera()->SetEulerAngles(vec3(-17, -36, 3));

	const uint32 cnt = 10;
	/*std::shared_ptr<Character> m_CharExtra[cnt * cnt];

	std::vector<uint32> exists;
	for (int i = 0; i < cnt; i++)
	{
		for (int j = 0; j < cnt; j++)
		{
			int index = i + j * cnt;
			m_CharExtra[index] = std::make_shared<Character>();

			while (true)
			{
				int random = Random::GenerateMax(32000);

				DBC_CreatureDisplayInfoRecord* rec = DBC_CreatureDisplayInfo[random];
				if (rec == nullptr)	continue;

				const DBC_CreatureDisplayInfoExtraRecord* exRec = rec->Get_HumanoidData();
				if (exRec == nullptr) continue;

				if (exRec->Get_Race()->Get_ID() > 10) continue;


				if (std::find(exists.begin(), exists.end(), random) != exists.end()) continue;


				m_CharExtra[index]->InitFromDisplayInfo(random);
				m_CharExtra[index]->SetParent(m_3DScene->GetRootNode());
				m_CharExtra[index]->CreateInstances();
				m_CharExtra[index]->SetScale(vec3(5.0f));

				exists.push_back(random);
				break;
			}

			m_CharExtra[index]->SetTranslate(vec3(i * 10.0f, 0.0f, j * 10.0f));
		}
	}*/

	/*std::shared_ptr<Creature> m_Char[cnt * cnt];
	std::vector<uint32> exists;
	for (int i = 0; i < cnt; i++)
	{
		for (int j = 0; j < cnt; j++)
		{
			int index = i + j * cnt;
			m_Char[index] = std::make_shared<Creature>();
			//m_Char->InitDefault();

			while (true)
			{
				int random = Random::GenerateMax(32000);
				std::shared_ptr<const DBC_CreatureDisplayInfoRecord> rec = DBC_CreatureDisplayInfo[random];
				if (rec == nullptr)	continue;

				if (rec->Get_HumanoidData() != nullptr) continue;
				if (std::find(exists.begin(), exists.end(), random) != exists.end()) continue;

				m_Char[index]->InitFromDisplayInfo(random);
				m_Char[index]->SetParent(m_3DScene->GetRootNode());
				m_Char[index]->CreateInstances();
				m_Char[index]->SetTranslate(vec3(i * 15.0f, 0.0f, j * 15.0f));
				//m_Char[index]->SetScale(vec3(5.0f));
				exists.push_back(random);
				break;
			}
		}
	}*/


	/*CharacterTemplate tempPala;
	tempPala.TemplateFillDefaultPaladin();

	CharacterTemplate tempShaman;
	tempShaman.TemplateFillDefaultShaman();

	std::shared_ptr<Character> character  = std::make_shared<Character>();
	character->InitFromTemplate(tempPala);
	character->CreateInstances();
	character->SetParent(m_3DScene->GetRootNode());
	character->SetTranslate(vec3(0, 15, 0));
	character->SetTranslate(vec3(x * C_TileSize, 200, y * C_TileSize));
	character->SetScale(vec3(10.0f));
	character->GetLocalTransform();*/

	//
	// PreRender passes
	//

	m_3DDeferredTechnique.AddPass(std::make_shared<ClearRenderTargetPass>(m_GBufferRenderTarget, ClearFlags::All, g_ClearColor, 1.0f, 0));


	//
	// 3D Passes
	//
	AddSkyPasses(renderDevice, m_GBufferRenderTarget, &m_3DDeferredTechnique, &m_Viewport, m_3DScene);
	AddWDLPasses(renderDevice, m_GBufferRenderTarget, &m_3DDeferredTechnique, &m_Viewport, m_3DScene);
	AddDebugPasses(renderDevice, m_GBufferRenderTarget, &m_3DDeferredTechnique, &m_Viewport, m_3DScene);
	AddMCNKPasses(renderDevice, m_GBufferRenderTarget, &m_3DDeferredTechnique, &m_Viewport, m_3DScene);
	AddWMOPasses(renderDevice, m_GBufferRenderTarget, &m_3DDeferredTechnique, &m_Viewport, m_3DScene);
	AddLiquidPasses(renderDevice, m_GBufferRenderTarget, &m_3DDeferredTechnique, &m_Viewport, m_3DScene);
	AddM2Passes(renderDevice, m_GBufferRenderTarget, &m_3DDeferredTechnique, &m_Viewport, m_3DScene);


	std::shared_ptr<Scene3D> quadScene = std::make_shared<Scene3D>();

	std::shared_ptr<SceneNode3D> quadSceneNode = std::make_shared<SceneNode3D>();
	quadSceneNode->SetParent(quadScene->GetRootNode());

	std::shared_ptr<IMesh> quadMesh = renderDevice->CreateScreenQuad(0, c_WindowsWidth, c_WindowsHeight, 0);
	quadSceneNode->AddMesh(quadMesh);
	
	// Shaders that unite 4 textures
	//std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(Shader::VertexShader, "shaders_D3D/GBuffer/GBuffer_Simple.hlsl", Shader::ShaderMacros(), "VS_main", "latest");
	//std::shared_ptr<Shader> g_pDeferredLightingPixelShader = _RenderDevice->CreateShader(Shader::PixelShader, "shaders_D3D/GBuffer/GBuffer_Simple.hlsl", Shader::ShaderMacros(), "PS_main", "latest");

	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(Shader::VertexShader, "shaders_D3D/DeferredRendering.hlsl", Shader::ShaderMacros(), "VS_main", "latest");
	std::shared_ptr<Shader> g_pDeferredLightingPixelShader = _RenderDevice->CreateShader(Shader::PixelShader, "shaders_D3D/DeferredRendering.hlsl", Shader::ShaderMacros(), "PS_DeferredLighting", "latest");


	// Pipeline State for result texture

	BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendOperation::Add, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
	BlendState::BlendMode additiveBlending(true, false, BlendState::BlendFactor::One, BlendState::BlendFactor::One);
	BlendState::BlendMode disableBlending;
	DepthStencilState::DepthMode enableDepthWrites(true, DepthStencilState::DepthWrite::Enable);
	DepthStencilState::DepthMode disableDepthWrites(false, DepthStencilState::DepthWrite::Disable);

	/*std::shared_ptr<PipelineState> resultPipelineState = renderDevice->CreatePipelineState();
	resultPipelineState->GetBlendState().SetBlendMode(disableBlending);
	resultPipelineState->GetDepthStencilState().SetDepthMode(disableDepthWrites);
	resultPipelineState->GetRasterizerState().SetCullMode(RasterizerState::CullMode::None);
	resultPipelineState->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
	resultPipelineState->GetRasterizerState().SetViewport(m_Viewport);
	resultPipelineState->SetRenderTarget(renderWindow->GetRenderTarget());
	resultPipelineState->SetShader(Shader::VertexShader, g_pVertexShader);
	resultPipelineState->SetShader(Shader::PixelShader, g_pDeferredLightingPixelShader);

	std::shared_ptr<IRenderPass> renderToSceneQuadPass = std::make_shared<GBufferPass>(quadScene, resultPipelineState, m_Viewport.OrthoMatrix,
		m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0), // position
		m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color1), // diffuse
		m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color2), // specular
		m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color3), // normal
		m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil)
	);*/

	std::shared_ptr<IRenderTarget> g_pDepthOnlyRenderTarget = renderDevice->CreateRenderTarget();
	g_pDepthOnlyRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, renderWindow->GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));


	// Pipeline for deferred lighting (stage 1 to determine lit pixels)
	std::shared_ptr<PipelineState> g_pDeferredLightingPipeline1;
	{
		g_pDeferredLightingPipeline1 = renderDevice->CreatePipelineState();
		g_pDeferredLightingPipeline1->SetShader(Shader::VertexShader, g_pVertexShader);
		g_pDeferredLightingPipeline1->SetRenderTarget(g_pDepthOnlyRenderTarget);

		// Setup rasterizer state
		g_pDeferredLightingPipeline1->GetRasterizerState().SetViewport(m_Viewport);
		g_pDeferredLightingPipeline1->GetRasterizerState().SetCullMode(RasterizerState::CullMode::Back);
		g_pDeferredLightingPipeline1->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
		g_pDeferredLightingPipeline1->GetRasterizerState().SetDepthClipEnabled(true);

		// Setup depth mode
		// Disable writing to the depth buffer.
		DepthStencilState::DepthMode depthMode(true, DepthStencilState::DepthWrite::Disable); // Disable depth writes.
		// Pass depth test if the light volume is behind scene geometry.
		depthMode.DepthFunction = DepthStencilState::CompareFunction::Greater;
		g_pDeferredLightingPipeline1->GetDepthStencilState().SetDepthMode(depthMode);

		// Setup stencil mode
		DepthStencilState::StencilMode stencilMode(true); // Enable stencil operations
		DepthStencilState::FaceOperation faceOperation;
		faceOperation.StencilDepthPass = DepthStencilState::StencilOperation::DecrementClamp;
		stencilMode.StencilReference = 1;
		stencilMode.FrontFace = faceOperation;

		g_pDeferredLightingPipeline1->GetDepthStencilState().SetStencilMode(stencilMode);
	}

	// Pipeline for deferred lighting (stage 2 to render lit pixels)
	std::shared_ptr<PipelineState> g_pDeferredLightingPipeline2;
	{
		g_pDeferredLightingPipeline2 = renderDevice->CreatePipelineState();
		g_pDeferredLightingPipeline2->SetShader(Shader::VertexShader, g_pVertexShader);
		g_pDeferredLightingPipeline2->SetShader(Shader::PixelShader, g_pDeferredLightingPixelShader);
		g_pDeferredLightingPipeline2->SetRenderTarget(renderWindow->GetRenderTarget());

		// Setup rasterizer state.
		g_pDeferredLightingPipeline2->GetRasterizerState().SetViewport(m_Viewport);
		g_pDeferredLightingPipeline2->GetRasterizerState().SetCullMode(RasterizerState::CullMode::Front);
		g_pDeferredLightingPipeline2->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
		g_pDeferredLightingPipeline2->GetRasterizerState().SetDepthClipEnabled(false);

		// Perform additive blending if a pixel passes the depth/stencil tests.
		g_pDeferredLightingPipeline2->GetBlendState().SetBlendMode(additiveBlending);

		// Setup depth mode
		// Disable depth writes
		DepthStencilState::DepthMode depthMode(true, DepthStencilState::DepthWrite::Disable); // Disable depth writes.
		depthMode.DepthFunction = DepthStencilState::CompareFunction::GreaterOrEqual;
		g_pDeferredLightingPipeline2->GetDepthStencilState().SetDepthMode(depthMode);

		// Setup stencil mode
		DepthStencilState::StencilMode stencilMode(true);
		DepthStencilState::FaceOperation faceOperation;
		// Render pixel if the depth function passes and the stencil was not un-marked in the previous pass.
		faceOperation.StencilFunction = DepthStencilState::CompareFunction::Equal;
		stencilMode.StencilReference = 1;
		stencilMode.BackFace = faceOperation;
		g_pDeferredLightingPipeline2->GetDepthStencilState().SetStencilMode(stencilMode);

	}

	// Pipeline for directional lights in deferred shader (only requires a single pass)
	std::shared_ptr<PipelineState> g_pDirectionalLightsPipeline;
	
		g_pDirectionalLightsPipeline = renderDevice->CreatePipelineState();
		g_pDirectionalLightsPipeline->SetShader(Shader::VertexShader, g_pVertexShader);
		g_pDirectionalLightsPipeline->SetShader(Shader::PixelShader, g_pDeferredLightingPixelShader);
		g_pDirectionalLightsPipeline->SetRenderTarget(renderWindow->GetRenderTarget());

		// Setup rasterizer state.
		g_pDirectionalLightsPipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::None);
		g_pDirectionalLightsPipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
		g_pDirectionalLightsPipeline->GetRasterizerState().SetViewport(m_Viewport);

		// Perform additive blending if a pixel passes the depth/stencil tests.
		g_pDirectionalLightsPipeline->GetBlendState().SetBlendMode(additiveBlending);

		// Setup depth mode
		DepthStencilState::DepthMode depthMode(true, DepthStencilState::DepthWrite::Disable); // Disable depth writes.
		// The full-screen quad that will be used to light pixels will be placed at the far clipping plane.
		// Only light pixels that are "in front" of the full screen quad (exclude sky box pixels)
		depthMode.DepthFunction = DepthStencilState::CompareFunction::Greater;
		g_pDirectionalLightsPipeline->GetDepthStencilState().SetDepthMode(disableDepthWrites);
	

	Light dir;
	dir.m_Enabled = true;
	dir.m_Type = Light::LightType::Directional;
	dir.m_DirectionWS = vec4(0.0, -0.5f, 0.0f, 0.0f);
	dir.m_Color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_Lights.push_back(dir);

	UpdateNumLights();
	UpdateLights();
	g_pDeferredLightingPixelShader->GetShaderParameterByName("Lights").Set(m_LightsStructuredBuffer);

	std::shared_ptr<Texture> depthStencilBuffer = renderWindow->GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil);
	std::shared_ptr<IRenderPass> renderToSceneQuadPass = std::make_shared<DeferredLightingPass>(
		m_Lights, 
		nullptr, nullptr,
		g_pDeferredLightingPipeline1, g_pDeferredLightingPipeline2, g_pDirectionalLightsPipeline,
		m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0), // position
		m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color1), // diffuse
		m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color2), // specular
		m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color3), // normal
		depthStencilBuffer
		);

	m_3DDeferredTechnique.AddPass(std::make_shared<ClearRenderTargetPass>(renderWindow->GetRenderTarget(), ClearFlags::All, g_ClearColor, 1.0f, 0));
	m_3DDeferredTechnique.AddPass(renderToSceneQuadPass);
}

void CGameState_World::LoadUI()
{
	Application& app = Application::Get();
	std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();


	// Font
	m_CameraPosText = std::make_shared<UIText>();
	m_CameraPosText->SetParent(m_UIScene->GetRootNode());
	m_CameraPosText->SetText("Camera position");
	m_CameraPosText->SetTranslate(vec2(0.0f, 0.0f));

	m_CameraRotText = std::make_shared<UIText>();
	m_CameraRotText->SetParent(m_UIScene->GetRootNode());
	m_CameraRotText->SetText("Camera rotation");
	m_CameraRotText->SetTranslate(vec2(0.0f, 20.0f));


	// Texture
	if (m_MapController != nullptr)
	{
		/*std::shared_ptr<UITexture> node3 = std::make_shared<UITexture>();
		node3->SetParent(m_UIScene->GetRootNode());
		node3->SetTexture(m_MapController->getMinimap());
		node3->SetTranslate(vec2(100.0f, 100.0f));
		node3->SetScale(vec2(512.0f, 512.0f));*/
	}

	// Texture 2
	//std::shared_ptr<UITexture> node4 = std::make_shared<UITexture>();
	//node4->SetParent(m_UIScene->GetRootNode());
	//node4->SetTexture(renderDevice->CreateTexture2D("Textures\\Moon02Glare.blp"));
	//node4->SetTranslate(vec2(200.0f, 000.0f));
	//node4->SetScale(vec2(100.0f, 100.0f));


	// Color quad
	//std::shared_ptr<SceneNodeUI> node2 = std::make_shared<SceneNodeUI>();
	//node2->SetParent(m_UIScene->GetRootNode());
	//node2->SetMesh(__geom);
	//node2->SetTranslate(vec2(180.0f, 180.0f));
	//node2->SetScale(vec2(10.0f, 10.0f));

	//
	// UI Passes
	//
	//AddUIPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &m_UITechnique, &m_Viewport, m_UIScene);
}

void CGameState_World::UpdateLights()
{
	glm::mat4 viewMatrix = m_CameraController->GetCamera()->GetViewMatrix();

	// Update the viewspace vectors of the light.
	for (unsigned int i = 0; i < m_Lights.size(); i++)
	{
		// Update the lights so that their position and direction are in view space.
		Light& light = m_Lights[i];
		light.m_PositionVS = viewMatrix * glm::vec4(light.m_PositionWS.xyz(), 1);
		light.m_DirectionVS = glm::normalize(viewMatrix * glm::vec4(light.m_DirectionWS.xyz(), 0));
	}

	// Update constant buffer data with lights array.
	m_LightsStructuredBuffer->Set(m_Lights);
}

void CGameState_World::UpdateNumLights()
{
	size_t numLights = m_Lights.size();

	std::shared_ptr<IRenderDevice> renderDevice = Application::Get().GetRenderDevice();

	// Destroy the old constant buffer
	renderDevice->DestroyStructuredBuffer(m_LightsStructuredBuffer);

	// Create a new one of the right size.
	m_LightsStructuredBuffer = renderDevice->CreateStructuredBuffer(m_Lights, CPUAccess::Write);

	// Recompile the shaders with the new size of lights array.
	//Shader::ShaderMacros shaderMacros;
	//{
	//	std::stringstream ss;
	//	ss << numLights;
	//	shaderMacros["NUM_LIGHTS"] = ss.str();
	//}

	// Recompile pixel shaders with updated number of lights.
	//g_pPixelShader->LoadShaderFromFile(Shader::PixelShader, L"../Assets/shaders/ForwardRendering.hlsl", shaderMacros, "PS_main", "latest");
	//g_pDeferredLightingPixelShader->LoadShaderFromFile(Shader::PixelShader, L"../Assets/shaders/DeferredRendering.hlsl", shaderMacros, "PS_DeferredLighting", "latest");
}

