#include "stdafx.h"

// General
#include "M2_Material.h"

M2_Material::M2_Material() :
	MaterialDebug(_RenderDevice)
{
	// CreateShaders
	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_D3D/M2/M2Debug.hlsl", Shader::ShaderMacros(), "VS_main", "latest"
	);
	std::shared_ptr<Shader> g_pPixelShader = _RenderDevice->CreateShader(
		Shader::PixelShader, "shaders_D3D/M2/M2Debug.hlsl", Shader::ShaderMacros(), "PS_main", "latest"
	);

	// Material
	SetDiffuseColor(vec4(1.0, 0.2, 0.4, 1.0));
	SetShader(Shader::VertexShader, g_pVertexShader);
	SetShader(Shader::PixelShader, g_pPixelShader);
}

M2_Material::~M2_Material()
{
}