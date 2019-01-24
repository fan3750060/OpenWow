#include <stdafx.h>

#include "PipelineStateOGL.h"

// Additional
#include "ShaderOGL.h"

PipelineStateOGL::PipelineStateOGL()
	: m_BlendState()
	, m_RasterizerState()
	, m_DepthStencilState()
{}

PipelineStateOGL::~PipelineStateOGL()
{}

void PipelineStateOGL::SetBlendState(const BlendState& blendState)
{
	m_BlendState = dynamic_cast<const BlendStateOGL&>(blendState);
}

BlendState& PipelineStateOGL::GetBlendState()
{
	return m_BlendState;
}

void PipelineStateOGL::SetRasterizerState(const RasterizerState& rasterizerState)
{
	m_RasterizerState = dynamic_cast<const RasterizerStateOGL&>(rasterizerState);
}

RasterizerState& PipelineStateOGL::GetRasterizerState()
{
	return m_RasterizerState;
}

void PipelineStateOGL::SetDepthStencilState(const DepthStencilState& depthStencilState)
{
	m_DepthStencilState = dynamic_cast<const DepthStencilStateOGL&>(depthStencilState);
}

DepthStencilState& PipelineStateOGL::GetDepthStencilState()
{
	return m_DepthStencilState;
}

void PipelineStateOGL::SetRenderTarget(std::shared_ptr<RenderTarget> renderTarget)
{
	m_RenderTarget = renderTarget;
}

std::shared_ptr<RenderTarget> PipelineStateOGL::GetRenderTarget() const
{
	return m_RenderTarget;
}

void PipelineStateOGL::Bind()
{
	if (m_RenderTarget)
	{
		m_RenderTarget->Bind();
	}

	m_BlendState.Bind();
	m_RasterizerState.Bind();
	m_DepthStencilState.Bind();


}

void PipelineStateOGL::UnBind()
{
	glBindProgramPipeline(0);

	if (m_RenderTarget)
	{
		m_RenderTarget->UnBind();
	}
}