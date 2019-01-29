#include "stdafx.h"

// General
#include "CreatePasses.h"

// Additional
#include "M2_Pass.h"

void AddM2Passes(std::shared_ptr<IRenderDevice> device, std::shared_ptr<IRenderTarget> _renderTarget, RenderTechnique * technique, Viewport * viewport, std::shared_ptr<Scene> scene)
{
	// STATES
	BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendOperation::Add, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
	DepthStencilState::DepthMode enableDepthWrites(true, DepthStencilState::DepthWrite::Enable);
	DepthStencilState::DepthMode disableDepthWrites(false, DepthStencilState::DepthWrite::Disable);

	// PIPELINES
	std::shared_ptr<PipelineState> WMOPipeline = device->CreatePipelineState();
	WMOPipeline->GetBlendState().SetBlendMode(alphaBlending);
	WMOPipeline->GetDepthStencilState().SetDepthMode(enableDepthWrites);
	WMOPipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::None);
	WMOPipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
	WMOPipeline->SetRenderTarget(_renderTarget);
	WMOPipeline->GetRasterizerState().SetViewport(*viewport);

	technique->AddPass(std::make_shared<M2_Pass>(scene, WMOPipeline));
}
