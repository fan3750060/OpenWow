#include "stdafx.h"

// Include
#include "MaterialWrapper.h"

// General
#include "MaterialTextured.h"

// Additional
#include <Application.h>

MaterialTextured::MaterialTextured(std::shared_ptr<Material> _material)
	: MaterialWrapper(_material)
{
	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_D3D/Debug/Textured.hlsl", Shader::ShaderMacros(), "VS_main", "latest"
	);
	std::shared_ptr<Shader> g_pPixelShader = _RenderDevice->CreateShader(
		Shader::PixelShader, "shaders_D3D/Debug/Textured.hlsl", Shader::ShaderMacros(), "PS_main", "latest"
	);

	std::shared_ptr<SamplerState> g_Sampler = _RenderDevice->CreateSamplerState();
	g_Sampler->SetFilter(SamplerState::MinFilter::MinLinear, SamplerState::MagFilter::MagLinear, SamplerState::MipFilter::MipLinear);
	g_Sampler->SetWrapMode(SamplerState::WrapMode::Repeat, SamplerState::WrapMode::Repeat);

	// Assign samplers
	g_pPixelShader->GetShaderParameterByName("DiffuseTextureSampler").Set(g_Sampler);

	SetShader(Shader::ShaderType::VertexShader, g_pVertexShader);
	SetShader(Shader::ShaderType::PixelShader, g_pPixelShader);
}

MaterialTextured::~MaterialTextured()
{
}

std::shared_ptr<Texture> MaterialTextured::GetTexture() const
{
	return MaterialWrapper::GetTexture(0);
}

void MaterialTextured::SetTexture(std::shared_ptr<Texture> texture)
{
	MaterialWrapper::SetTexture(0, texture);
}
