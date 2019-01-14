#include "stdafx.h"

// General
#include "UI_Color_Material.h"

UI_Color_Material::UI_Color_Material() :
	Material(_RenderDevice)
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();
	m_pConstantBuffer = m_RenderDevice->CreateConstantBuffer(*m_pProperties);

	// CreateShaders
	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_D3D/UI/UI_Color.hlsl", Shader::ShaderMacros(), "VS_main", "latest"
	);
	std::shared_ptr<Shader> g_pPixelShader = _RenderDevice->CreateShader(
		Shader::PixelShader, "shaders_D3D/UI/UI_Color.hlsl", Shader::ShaderMacros(), "PS_main", "latest"
	);

	// Material
	SetShader(Shader::VertexShader, g_pVertexShader);
	SetShader(Shader::PixelShader, g_pPixelShader);
}

UI_Color_Material::~UI_Color_Material()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

void UI_Color_Material::SetColor(vec4 color)
{
	m_pProperties->Color = color;
	m_Dirty = true;
}

void UI_Color_Material::UpdateConstantBuffer() const
{
	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->Set(*m_pProperties);
	}
}
