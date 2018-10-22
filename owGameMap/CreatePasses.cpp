#include "stdafx.h"

// General
#include "CreatePasses.h"

// Additional
#include "ADT_CHUNK_Pass.h"
#include "WDL_Node_Pass.h"

void AddMapPasses(RenderDevice* device, RenderWindow* window, RenderTechnique * technique, Viewport * viewport, std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Shader> g_pVertexShader = device->CreateShader();
	g_pVertexShader->LoadShaderFromFile(Shader::VertexShader, "shaders_D3D/Map/MapWDL.hlsl", Shader::ShaderMacros(), "VS_main", "latest");
	std::shared_ptr<Shader> g_pPixelShader = device->CreateShader();
	g_pPixelShader->LoadShaderFromFile(Shader::PixelShader, "shaders_D3D/Map/MapWDL.hlsl", Shader::ShaderMacros(), "PS_main", "latest");

	// Create samplers
	std::shared_ptr<SamplerState> g_LinearRepeatSampler = device->CreateSamplerState();
	g_LinearRepeatSampler->SetFilter(SamplerState::MinFilter::MinLinear, SamplerState::MagFilter::MagLinear, SamplerState::MipFilter::MipLinear);
	g_LinearRepeatSampler->SetWrapMode(SamplerState::WrapMode::Repeat, SamplerState::WrapMode::Repeat, SamplerState::WrapMode::Repeat);

	std::shared_ptr<SamplerState> g_LinearClampSampler = device->CreateSamplerState();
	g_LinearClampSampler->SetFilter(SamplerState::MinFilter::MinLinear, SamplerState::MagFilter::MagLinear, SamplerState::MipFilter::MipLinear);
	g_LinearClampSampler->SetWrapMode(SamplerState::WrapMode::Clamp, SamplerState::WrapMode::Clamp, SamplerState::WrapMode::Clamp);

	g_pPixelShader->GetShaderParameterByName("DiffuseSampler").Set(g_LinearRepeatSampler);

	// STATES
	BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendOperation::Add, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
	DepthStencilState::DepthMode enableDepthWrites(true, DepthStencilState::DepthWrite::Enable);
	DepthStencilState::DepthMode disableDepthWrites(true, DepthStencilState::DepthWrite::Disable);

	// PIPELINES
	std::shared_ptr<PipelineState> g_pOpaquePipeline = device->CreatePipelineState();
	g_pOpaquePipeline->SetShader(Shader::VertexShader, g_pVertexShader);
	g_pOpaquePipeline->SetShader(Shader::PixelShader, g_pPixelShader);
	g_pOpaquePipeline->GetBlendState().SetBlendMode(alphaBlending);
	g_pOpaquePipeline->GetDepthStencilState().SetDepthMode(enableDepthWrites);
	g_pOpaquePipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::None);
	g_pOpaquePipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Wireframe);
	g_pOpaquePipeline->SetRenderTarget(window->GetRenderTarget());
	g_pOpaquePipeline->GetRasterizerState().SetViewport(*viewport);

	technique->AddPass(std::make_shared<WDL_Node_Pass>(scene, g_pOpaquePipeline));
	//technique->AddPass(std::make_shared<ADT_CHUNK_Pass>(scene, g_pOpaquePipeline));
}
