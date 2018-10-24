#include "stdafx.h"

// Include
#include "RenderDevice.h"
#include "Shader.h"

// General
#include "Material.h"

Material::Material(RenderDevice* renderDevice)
	: m_RenderDevice(renderDevice)
	, m_Dirty(false)
{}

Material::~Material()
{}


void Material::SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader)
{
	m_Shaders[type] = pShader;
}

std::shared_ptr<Shader> Material::GetShader(Shader::ShaderType type) const
{
	ShaderMap::const_iterator iter = m_Shaders.find(type);
	if (iter != m_Shaders.end())
	{
		return iter->second;
	}

	return nullptr;
}

const Material::ShaderMap& Material::GetShaders() const
{
	return m_Shaders;
}

void Material::Bind()
{
	if (m_Dirty)
	{
		UpdateConstantBuffer();
		m_Dirty = false;
	}

	for (auto shader : m_Shaders)
	{
		std::shared_ptr<Shader> pShader = shader.second;
		if (pShader)
		{
			pShader->Bind();

			for (auto texture : GetTextureMap())
			{
				std::shared_ptr<Texture> pTexture = texture.second;
				pTexture->Bind((uint32_t)texture.first, pShader, ShaderParameter::Type::Texture);
			}

			ShaderParameter& materialParameter = pShader->GetShaderParameterByName("Material");
			if (materialParameter.IsValid() && GetConstantBuffer() != nullptr)
			{
				materialParameter.Set<ConstantBuffer>(GetConstantBuffer());
				materialParameter.Bind();
			}
		}
	}
}

void Material::Unbind()
{
	for (auto shader : m_Shaders)
	{
		std::shared_ptr<Shader> pShader = shader.second;
		if (pShader)
		{
			pShader->UnBind();
		}
	}
}

//--

Material::TextureMap Material::GetTextureMap() const
{
	return TextureMap();
}

std::shared_ptr<ConstantBuffer> Material::GetConstantBuffer() const
{
	return nullptr;
}

void Material::UpdateConstantBuffer() const
{
}
