#include "stdafx.h"

// Include
#include "RenderTechnique.h"
#include "RenderUITechnique.h"

// General
#include "CreatePasses.h"

// Additional
#include "Debug_Pass.h"
#include "BaseUIPass.h"

void AddDebugPasses(std::shared_ptr<IRenderDevice> device, std::shared_ptr<IRenderTarget> _renderTarget, RenderTechnique * technique, Viewport * viewport, std::shared_ptr<Scene3D> scene)
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


void AddUIPasses(std::shared_ptr<IRenderDevice> device, std::shared_ptr<IRenderTarget> _renderTarget, RenderUITechnique * technique, Viewport * viewport, std::shared_ptr<SceneUI> scene)
{
	BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendOperation::Add, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
	BlendState::BlendMode disableBlending;
	DepthStencilState::DepthMode enableDepthWrites(true, DepthStencilState::DepthWrite::Enable);
	DepthStencilState::DepthMode disableDepthWrites(false, DepthStencilState::DepthWrite::Disable);

	std::shared_ptr<PipelineState> UIPipeline = device->CreatePipelineState();
	UIPipeline->GetBlendState().SetBlendMode(alphaBlending);
	UIPipeline->GetDepthStencilState().SetDepthMode(enableDepthWrites);
	UIPipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::None);
	UIPipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
	UIPipeline->SetRenderTarget(_renderTarget);
	UIPipeline->GetRasterizerState().SetViewport(*viewport);

	technique->SetPass(std::make_shared<BaseUIPass>(scene, UIPipeline));
}
