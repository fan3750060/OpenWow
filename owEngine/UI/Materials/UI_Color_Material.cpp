#include "stdafx.h"

// General
#include "UI_Color_Material.h"

// Additional
#include "Application.h"

UI_Color_Material::UI_Color_Material() :
	MaterialWrapper(_RenderDevice->CreateMaterial())
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();
	CreateConstantBuffer(m_pProperties, sizeof(MaterialProperties));

	// CreateShaders
	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_D3D/UI/UI_VertexBase.hlsl", Shader::ShaderMacros(), "VS_main", "latest"
	);
	std::vector<D3DVERTEXELEMENT9> elements;
	elements.push_back({ 0, 0,  D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_POSITION, 0 });
	elements.push_back({ 0, 12, D3DDECLTYPE_FLOAT2, 0, D3DDECLUSAGE_TEXCOORD, 0 });
	elements.push_back({ 0, 20, D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_NORMAL, 0 });
	g_pVertexShader->LoadInputLayoutFromD3DElement(elements);

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
	MarkConstantBufferDirty();
}

void UI_Color_Material::UpdateConstantBuffer() const
{
	MaterialWrapper::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
