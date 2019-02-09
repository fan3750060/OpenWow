#include "stdafx.h"

// General
#include "M2_Material.h"

M2_Material::M2_Material(std::vector<std::weak_ptr<const CM2_Part_Texture>> m2Textures) :
	MaterialWrapper(_RenderDevice->CreateMaterial())
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();
	CreateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
	memset(m_pProperties, 0x00, sizeof(MaterialProperties));


	// CreateShaders
	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_D3D/M2/M2.hlsl", Shader::ShaderMacros(), "VS_main", "latest"
	);
	std::shared_ptr<Shader> g_pPixelShader = _RenderDevice->CreateShader(
		Shader::PixelShader, "shaders_D3D/M2/M2.hlsl", Shader::ShaderMacros(), "PS_main", "latest"
	);

	// Create samplers
	for (uint8 i = 0; i < ((m2Textures.size() > 2) ? 2 : m2Textures.size()); i++)
	{

		std::shared_ptr<SamplerState> g_Sampler = _RenderDevice->CreateSamplerState();
		g_Sampler->SetFilter(SamplerState::MinFilter::MinLinear, SamplerState::MagFilter::MagLinear, SamplerState::MipFilter::MipLinear);
		g_Sampler->SetWrapMode(m2Textures[i].lock()->GetTextureWrapX(), m2Textures[i].lock()->GetTextureWrapY());

		// Assign samplers
		g_pPixelShader->GetShaderParameterByName("DiffuseTexture" + std::to_string(i) + "Sampler").Set(g_Sampler);
	}

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

void M2_Material::SetAnimated(bool value)
{
	m_pProperties->gIsAnimated = value;
	MarkConstantBufferDirty();
}

void M2_Material::SetColorEnable(bool value)
{
	m_pProperties->gColorEnable = value;
	MarkConstantBufferDirty();
}

void M2_Material::SetColor(vec4 value)
{
	m_pProperties->gColor = value;
	MarkConstantBufferDirty();
}

void M2_Material::SetBones(const std::vector<mat4>& bones)
{
	for (uint8 i = 0; i < bones.size(); i++)
	{
		m_pProperties->Bones[i] = bones[i];
	}
	MarkConstantBufferDirty();
}

void M2_Material::SetMaxInfluences(uint32 value)
{
	m_pProperties->gBonesMaxInfluences = value;
	MarkConstantBufferDirty();
}

void M2_Material::SetBlendMode(uint32 value)
{
	m_pProperties->gBlendMode = value;
	MarkConstantBufferDirty();
}

void M2_Material::SetNewShader(uint32 value)
{
	m_pProperties->gShader = value;
	MarkConstantBufferDirty();
}

void M2_Material::SetTextureWeightEnable(bool value)
{
	m_pProperties->gTextureWeightEnable = value;
	MarkConstantBufferDirty();
}

void M2_Material::SetTextureWeight(float value)
{
	m_pProperties->gTextureWeight = value;
	MarkConstantBufferDirty();
}

void M2_Material::SetTextureAnimEnable(bool value)
{
	m_pProperties->gTextureAnimEnable = value;
	MarkConstantBufferDirty();
}

void M2_Material::SetTextureAnimMatrix(cmat4 value)
{
	m_pProperties->gTextureAnimMatrix = value;
	MarkConstantBufferDirty();
}

//--

void M2_Material::UpdateConstantBuffer() const
{
	MaterialWrapper::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}