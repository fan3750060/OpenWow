#include "stdafx.h"

// General
#include "CreatePasses.h"

// Additional
#include "ADT_CHUNK_Pass.h"
#include "WDL_Node_Pass.h"



void AddWDLPasses(RenderDevice* device, RenderWindow* window, RenderTechnique * technique, Viewport * viewport, std::shared_ptr<Scene> scene)
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

void AddMCNKPasses(RenderDevice* device, RenderWindow* window, RenderTechnique * technique, Viewport * viewport, std::shared_ptr<Scene> scene)
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
	technique->AddPass(std::make_shared<ADT_CHUNK_Pass>(scene, ADTPipeline));
}
