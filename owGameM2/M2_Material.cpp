#include "stdafx.h"

// General
#include "M2_Material.h"

M2_Material::M2_Material() :
	Material(_RenderDevice)
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();
	m_pConstantBuffer = m_RenderDevice->CreateConstantBuffer(*m_pProperties);
	memset(m_pProperties, 0x00, sizeof(MaterialProperties));


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
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

void M2_Material::SetAnimated(uint32 value)
{
	m_pProperties->gIsAnimated = value;
	m_Dirty = true;
}

void M2_Material::SetBones(const std::vector<mat4>& bones)
{
	for (uint8 i = 0; i < bones.size(); i++)
	{
		m_pProperties->Bones[i] = bones[i];
	}
	m_Dirty = true;
}

void M2_Material::SetMaxInfluences(uint32 value)
{
	m_pProperties->gBonesMaxInfluences = value;
	m_Dirty = true;
}

void M2_Material::SetBlendMode(uint32 value)
{
	m_pProperties->gBlendMode = value;
	m_Dirty = true;
}

//--

void M2_Material::UpdateConstantBuffer() const
{
	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->Set(*m_pProperties);
	}
}