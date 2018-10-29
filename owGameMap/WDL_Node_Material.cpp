#include "stdafx.h"

// General
#include "WDL_Node_Material.h"

WDL_Node_Material::WDL_Node_Material() :
	MaterialDebug(_RenderDevice)
{
	// CreateShaders
	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_D3D/Map/MapWDL.hlsl", Shader::ShaderMacros(), "VS_main", "latest"
	);
	std::shared_ptr<Shader> g_pPixelShader = _RenderDevice->CreateShader(
		Shader::PixelShader, "shaders_D3D/Map/MapWDL.hlsl", Shader::ShaderMacros(), "PS_main", "latest"
	);

	// Material
	SetDiffuseColor(vec4(0, 0.2, 0.8, 1.0));
	SetShader(Shader::VertexShader, g_pVertexShader);
	SetShader(Shader::PixelShader, g_pPixelShader);
}

WDL_Node_Material::~WDL_Node_Material()
{
}
