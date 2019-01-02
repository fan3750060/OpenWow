#include "stdafx.h"

// General
#include "M2_Material.h"

M2_Material::M2_Material() :
	Material(_RenderDevice)
{
	// CreateShaders
	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_D3D/M2/M2.hlsl", Shader::ShaderMacros(), "VS_main", "latest"
	);
	std::shared_ptr<Shader> g_pPixelShader = _RenderDevice->CreateShader(
		Shader::PixelShader, "shaders_D3D/M2/M2.hlsl", Shader::ShaderMacros(), "PS_main", "latest"
	);

	// Create samplers
	std::shared_ptr<SamplerState> g_Sampler = _RenderDevice->CreateSamplerState();
	g_Sampler->SetFilter(SamplerState::MinFilter::MinLinear, SamplerState::MagFilter::MagLinear, SamplerState::MipFilter::MipLinear);
	g_Sampler->SetWrapMode(SamplerState::WrapMode::Clamp, SamplerState::WrapMode::Clamp);

	// Assign samplers
	g_pPixelShader->GetShaderParameterByName("DiffuseTextureSampler").Set(g_Sampler);

	// Material
	SetShader(Shader::VertexShader, g_pVertexShader);
	SetShader(Shader::PixelShader, g_pPixelShader);
}

M2_Material::~M2_Material()
{
}