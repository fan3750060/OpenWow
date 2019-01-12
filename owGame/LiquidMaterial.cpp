#include "stdafx.h"

// General
#include "LiquidMaterial.h"

LiquidMaterial::LiquidMaterial() :
	Material(_RenderDevice)
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();
	m_pConstantBuffer = m_RenderDevice->CreateConstantBuffer(*m_pProperties);

	// CreateShaders
	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_D3D/Liquid/Liquid.hlsl", Shader::ShaderMacros(), "VS_main", "latest"
	);
	std::shared_ptr<Shader> g_pPixelShader = _RenderDevice->CreateShader(
		Shader::PixelShader, "shaders_D3D/Liquid/Liquid.hlsl", Shader::ShaderMacros(), "PS_main", "latest"
	);

	// Create samplers
	std::shared_ptr<SamplerState> g_Sampler = _RenderDevice->CreateSamplerState();
	g_Sampler->SetFilter(SamplerState::MinFilter::MinLinear, SamplerState::MagFilter::MagLinear, SamplerState::MipFilter::MipLinear);
	g_Sampler->SetWrapMode(SamplerState::WrapMode::Clamp, SamplerState::WrapMode::Clamp, SamplerState::WrapMode::Clamp);

	// Assign samplers
	g_pPixelShader->GetShaderParameterByName("DiffuseTextureSampler").Set(g_Sampler);

	// This
	SetShader(Shader::VertexShader, g_pVertexShader);
	SetShader(Shader::PixelShader, g_pPixelShader);
}

LiquidMaterial::~LiquidMaterial()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

void LiquidMaterial::SetShallowAlpha(float value)
{
	m_pProperties->gShallowAlpha = value;
	m_Dirty = true;
}

void LiquidMaterial::SetDeepAlpha(float value)
{
	m_pProperties->gDeepAlpha = value;
	m_Dirty = true;
}

void LiquidMaterial::SetColorLight(vec3 value)
{
	m_pProperties->gColorLight = value;
	m_Dirty = true;
}

void LiquidMaterial::SetColorDark(vec3 value)
{
	m_pProperties->gColorDark = value;
	m_Dirty = true;
}

//--

void LiquidMaterial::UpdateConstantBuffer() const
{
	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->Set(*m_pProperties);
	}
}