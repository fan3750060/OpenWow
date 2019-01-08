#include "stdafx.h"

// General
#include "ADT_MCNK_Material.h"


ADT_MCNK_Material::ADT_MCNK_Material() :
	Material(_RenderDevice)
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();
	m_pConstantBuffer = m_RenderDevice->CreateConstantBuffer(*m_pProperties);

	// CreateShaders
	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_D3D/Map/MapChunk.hlsl", Shader::ShaderMacros(), "VS_main", "latest"
	);
	std::shared_ptr<Shader> g_pPixelShader = _RenderDevice->CreateShader(
		Shader::PixelShader, "shaders_D3D/Map/MapChunk.hlsl", Shader::ShaderMacros(), "PS_main", "latest"
	);

	// Create samplers
	std::shared_ptr<SamplerState> g_LinearClampSampler = _RenderDevice->CreateSamplerState();
	g_LinearClampSampler->SetFilter(SamplerState::MinFilter::MinLinear, SamplerState::MagFilter::MagLinear, SamplerState::MipFilter::MipLinear);
	g_LinearClampSampler->SetWrapMode(SamplerState::WrapMode::Clamp, SamplerState::WrapMode::Clamp, SamplerState::WrapMode::Clamp);

	std::shared_ptr<SamplerState> g_LinearRepeatSampler = _RenderDevice->CreateSamplerState();
	g_LinearRepeatSampler->SetFilter(SamplerState::MinFilter::MinLinear, SamplerState::MagFilter::MagLinear, SamplerState::MipFilter::MipLinear);
	g_LinearRepeatSampler->SetWrapMode(SamplerState::WrapMode::Repeat, SamplerState::WrapMode::Repeat, SamplerState::WrapMode::Repeat);

	g_pPixelShader->GetShaderParameterByName("ColorMapSampler").Set(g_LinearRepeatSampler);
	g_pPixelShader->GetShaderParameterByName("AlphaMapSampler").Set(g_LinearClampSampler);

	SetShader(Shader::VertexShader, g_pVertexShader);
	SetShader(Shader::PixelShader, g_pPixelShader);
}

ADT_MCNK_Material::~ADT_MCNK_Material()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

//--

uint32 ADT_MCNK_Material::GetLayersCnt() const
{
	return m_pProperties->m_LayersCnt;
}

void ADT_MCNK_Material::SetLayersCnt(uint32 value)
{
	m_pProperties->m_LayersCnt = value;
	m_Dirty = true;
}

//--

void ADT_MCNK_Material::UpdateConstantBuffer() const
{
	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->Set(*m_pProperties);
	}
}
