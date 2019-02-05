#include "stdafx.h"

// General
#include "CreatePasses.h"

// Additional
#include "WMO_Pass.h"

void AddWMOPasses(std::shared_ptr<IRenderDevice> device, std::shared_ptr<IRenderTarget> _renderTarget, RenderTechnique * technique, Viewport * viewport, std::shared_ptr<Scene3D> scene)
{
	BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendOperation::Add, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
	BlendState::BlendMode disableBlending;
	DepthStencilState::DepthMode enableDepthWrites(true, DepthStencilState::DepthWrite::Enable);
	DepthStencilState::DepthMode disableDepthWrites(false, DepthStencilState::DepthWrite::Disable);

	// PIPELINES
	std::shared_ptr<PipelineState> WMOPipeline = device->CreatePipelineState();
	WMOPipeline->GetBlendState().SetBlendMode(disableBlending);
	WMOPipeline->GetDepthStencilState().SetDepthMode(enableDepthWrites);
	WMOPipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::Back);
	WMOPipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
	WMOPipeline->SetRenderTarget(_renderTarget);
	WMOPipeline->GetRasterizerState().SetViewport(*viewport);

	technique->AddPass(std::make_shared<WMO_Pass>(scene, WMOPipeline));
}
