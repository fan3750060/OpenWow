#include <stdafx.h>

#include "PipelineStateDX11.h"

PipelineStateDX11::PipelineStateDX11(ID3D11Device2* pDevice)
	: m_pDevice(pDevice)
	, m_BlendState(pDevice)
	, m_RasterizerState(pDevice)
	, m_DepthStencilState(pDevice)
{
	m_pDevice->GetImmediateContext2(&m_pDeviceContext);
}

PipelineStateDX11::~PipelineStateDX11()
{

}


void PipelineStateDX11::SetBlendState(const BlendState& blendState)
{
	m_BlendState = dynamic_cast<const BlendStateDX11&>(blendState);
}

BlendState& PipelineStateDX11::GetBlendState()
{
	return m_BlendState;
}

void PipelineStateDX11::SetRasterizerState(const RasterizerState& rasterizerState)
{
	m_RasterizerState = dynamic_cast<const RasterizerStateDX11&>(rasterizerState);
}

RasterizerState& PipelineStateDX11::GetRasterizerState()
{
	return m_RasterizerState;
}

void PipelineStateDX11::SetDepthStencilState(const DepthStencilState& depthStencilState)
{
	m_DepthStencilState = dynamic_cast<const DepthStencilStateDX11&>(depthStencilState);
}

DepthStencilState& PipelineStateDX11::GetDepthStencilState()
{
	return m_DepthStencilState;
}

void PipelineStateDX11::SetRenderTarget(std::shared_ptr<RenderTarget> renderTarget)
{
	m_RenderTarget = renderTarget;
}

std::shared_ptr<RenderTarget> PipelineStateDX11::GetRenderTarget() const
{
	return m_RenderTarget;
}

void PipelineStateDX11::Bind()
{
	if (m_RenderTarget)
	{
		m_RenderTarget->Bind();
	}

	m_BlendState.Bind();
	m_RasterizerState.Bind();
	m_DepthStencilState.Bind();

	/*sh*/
}

void PipelineStateDX11::UnBind()
{
	if (m_RenderTarget)
	{
		m_RenderTarget->UnBind();
	}

	/*sh*/
}