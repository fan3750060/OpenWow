#include "stdafx.h"

// Include
#include "RenderTechnique.h"

// General
#include "CreatePasses.h"

// Additional
#include "Debug_Pass.h"

void AddDebugPasses(std::shared_ptr<IRenderDevice> device, std::shared_ptr<IRenderTarget> _renderTarget, RenderTechnique * technique, Viewport * viewport, std::shared_ptr<Scene> scene)
{
	// STATES
	BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendOperation::Add, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
	BlendState::BlendMode disableBlending;
	DepthStencilState::DepthMode enableDepthWrites(true, DepthStencilState::DepthWrite::Enable);
	DepthStencilState::DepthMode disableDepthWrites(false, DepthStencilState::DepthWrite::Disable);

	// PIPELINES
	std::shared_ptr<PipelineState> DebugPipeline = device->CreatePipelineState();
	DebugPipeline->GetBlendState().SetBlendMode(disableBlending);
	DebugPipeline->GetDepthStencilState().SetDepthMode(enableDepthWrites);
	DebugPipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::None);
	DebugPipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Wireframe);
	DebugPipeline->SetRenderTarget(_renderTarget);
	DebugPipeline->GetRasterizerState().SetViewport(*viewport);

	technique->AddPass(std::make_shared<Debug_Pass>(scene, DebugPipeline));
}
