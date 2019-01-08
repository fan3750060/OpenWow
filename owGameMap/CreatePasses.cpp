#include "stdafx.h"

// General
#include "CreatePasses.h"

// Additional
#include "Sky_Pass.h"
#include "ADT_MCNK_Pass.h"
#include "WDL_Node_Pass.h"



void AddSkyPasses(IRenderDevice * device, RenderWindow * window, RenderTechnique * technique, Viewport * viewport, std::shared_ptr<Scene> scene)
{
	BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendOperation::Add, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
	BlendState::BlendMode disableBlending;
	DepthStencilState::DepthMode enableDepthWrites(true, DepthStencilState::DepthWrite::Enable);
	DepthStencilState::DepthMode disableDepthWrites(false, DepthStencilState::DepthWrite::Disable);


	// PIPELINES
	std::shared_ptr<PipelineState> WDLPipeline = device->CreatePipelineState();
	WDLPipeline->GetBlendState().SetBlendMode(disableBlending);
	WDLPipeline->GetDepthStencilState().SetDepthMode(disableDepthWrites);
	WDLPipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::None);
	WDLPipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
	WDLPipeline->SetRenderTarget(window->GetRenderTarget());
	WDLPipeline->GetRasterizerState().SetViewport(*viewport);
	technique->AddPass(std::make_shared<Sky_Pass>(scene, WDLPipeline));
}

void AddWDLPasses(IRenderDevice* device, RenderWindow* window, RenderTechnique * technique, Viewport * viewport, std::shared_ptr<Scene> scene)
{
	BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendOperation::Add, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
	BlendState::BlendMode disableBlending;
	DepthStencilState::DepthMode enableDepthWrites(true, DepthStencilState::DepthWrite::Enable);
	DepthStencilState::DepthMode disableDepthWrites(false, DepthStencilState::DepthWrite::Disable);


	// PIPELINES
	std::shared_ptr<PipelineState> WDLPipeline = device->CreatePipelineState();
	WDLPipeline->GetBlendState().SetBlendMode(disableBlending);
	WDLPipeline->GetDepthStencilState().SetDepthMode(disableDepthWrites);
	WDLPipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::Back);
	WDLPipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Wireframe);
	WDLPipeline->SetRenderTarget(window->GetRenderTarget());
	WDLPipeline->GetRasterizerState().SetViewport(*viewport);
	technique->AddPass(std::make_shared<WDL_Node_Pass>(scene, WDLPipeline));

}

void AddMCNKPasses(IRenderDevice* device, RenderWindow* window, RenderTechnique * technique, Viewport * viewport, std::shared_ptr<Scene> scene)
{
	BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendOperation::Add, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
	BlendState::BlendMode disableBlending;
	DepthStencilState::DepthMode enableDepthWrites(true, DepthStencilState::DepthWrite::Enable);
	DepthStencilState::DepthMode disableDepthWrites(false, DepthStencilState::DepthWrite::Disable);


	// PIPELINES
	std::shared_ptr<PipelineState> ADTPipeline = device->CreatePipelineState();
	ADTPipeline->GetBlendState().SetBlendMode(disableBlending);
	ADTPipeline->GetDepthStencilState().SetDepthMode(enableDepthWrites);
	ADTPipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::Back);
	ADTPipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
	ADTPipeline->SetRenderTarget(window->GetRenderTarget());
	ADTPipeline->GetRasterizerState().SetViewport(*viewport);
	technique->AddPass(std::make_shared<ADT_MCNK_Pass>(scene, ADTPipeline));
}
