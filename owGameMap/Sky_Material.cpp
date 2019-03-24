#include "stdafx.h"

// General
#include "Sky_Material.h"

Sky_Material::Sky_Material() :
	MaterialWrapper(_RenderDevice->CreateMaterial())
{
	// CreateShaders
#ifdef  IS_DX11
	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_D3D/Sky/Sky.hlsl", Shader::ShaderMacros(), "VS_main", "latest"
	);
    g_pVertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<Shader> g_pPixelShader = _RenderDevice->CreateShader(
		Shader::PixelShader, "shaders_D3D/Sky/Sky.hlsl", Shader::ShaderMacros(), "PS_main", "latest"
	);
#else
	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_OGL/Sky/Sky.vs", Shader::ShaderMacros(), "", ""
	);
	std::shared_ptr<Shader> g_pPixelShader = _RenderDevice->CreateShader(
		Shader::PixelShader, "shaders_OGL/Sky/Sky.fs", Shader::ShaderMacros(), "", ""
	);
#endif

	// Material
	SetShader(Shader::VertexShader, g_pVertexShader);
	SetShader(Shader::PixelShader, g_pPixelShader);
}

Sky_Material::~Sky_Material()
{
}
