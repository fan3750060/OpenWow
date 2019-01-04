#include <stdafx.h>

#include <Application.h>

// General
#include "LightPickingPass.h"

LightPickingPass::LightPickingPass(std::vector<Light>& lights, std::shared_ptr<Scene> pointLight, std::shared_ptr<Scene> spotLight, std::shared_ptr<Scene> directionalLight, std::shared_ptr<PipelineState> pipeline)
	: base(lights, pointLight, spotLight, directionalLight, pipeline)
	, m_RenderDevice(_RenderDevice)
{
	m_pLightParams = (LightParams*)_aligned_malloc(sizeof(LightParams), 16);
	m_LightParamsCB = m_RenderDevice->CreateConstantBuffer(LightParams());
}

LightPickingPass::~LightPickingPass()
{
	m_RenderDevice->DestroyConstantBuffer(m_LightParamsCB);
	_aligned_free(m_pLightParams);
}

void LightPickingPass::PreRender(RenderEventArgs& e)
{
	// Make sure the light index is bound to the pixel shader stage.
	//e.PipelineState->GetShader(Shader::PixelShader)->GetShaderParameterByName("LightIndexBuffer").Set(m_LightParamsCB);

	base::PreRender(e);
}

bool LightPickingPass::Visit(IMesh& mesh)
{
	m_pLightParams->m_LightIndex = GetCurrentLightIndex();
	m_LightParamsCB->Set(*m_pLightParams);

	mesh.Render(GetRenderEventArgs(), m_LightParamsCB);

	return true;
}