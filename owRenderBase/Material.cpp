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
{
	if (m_pConstantBuffer)
	{
		m_RenderDevice->DestroyConstantBuffer(m_pConstantBuffer);
		m_pConstantBuffer.reset();
	}
}


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


std::shared_ptr<Texture> Material::GetTexture(uint8 type) const
{
	TextureMap::const_iterator itr = m_Textures.find(type);
	if (itr != m_Textures.end())
	{
		return itr->second;
	}

	return nullptr;
}

void Material::SetTexture(uint8 type, std::shared_ptr<Texture> texture)
{
	m_Textures[type] = texture;
	m_Dirty = true;
}


void Material::Bind() const
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

			for (auto texture : m_Textures)
			{
				std::shared_ptr<Texture> pTexture = texture.second;
				pTexture->Bind((uint32_t)texture.first, pShader, ShaderParameter::Type::Texture);
			}

			ShaderParameter& materialParameter = pShader->GetShaderParameterByName("Material");
			if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
			{
				materialParameter.Set<ConstantBuffer>(m_pConstantBuffer);
				materialParameter.Bind();
			}
		}
	}
}

void Material::Unbind() const
{
	for (auto shader : m_Shaders)
	{
		std::shared_ptr<Shader> pShader = shader.second;
		if (pShader)
		{
			for (auto texture : m_Textures)
			{
				std::shared_ptr<Texture> pTexture = texture.second;
				pTexture->UnBind((uint32_t)texture.first, pShader, ShaderParameter::Type::Texture);
			}

			ShaderParameter& materialParameter = pShader->GetShaderParameterByName("Material");
			if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
			{
				materialParameter.UnBind();
			}

			pShader->UnBind();
		}
	}
}

//--

void Material::CreateConstantBuffer()
{
}

void Material::UpdateConstantBuffer() const
{
}
