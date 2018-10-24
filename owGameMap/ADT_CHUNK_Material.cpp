#include "stdafx.h"

// General
#include "ADT_CHUNK_Material.h"


ADT_CHUNK_Material::ADT_CHUNK_Material(RenderDevice * _device) :
	Material(_device)
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();
	m_pConstantBuffer = m_RenderDevice->CreateConstantBuffer(*m_pProperties);
}

ADT_CHUNK_Material::~ADT_CHUNK_Material()
{
	if (m_pConstantBuffer)
	{
		m_RenderDevice->DestroyConstantBuffer(m_pConstantBuffer);
		m_pConstantBuffer.reset();
	}

	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

//--

std::shared_ptr<Texture> ADT_CHUNK_Material::GetTexture(uint8 type) const
{
	TextureMap::const_iterator itr = m_Textures.find(type);
	if (itr != m_Textures.end())
	{
		return itr->second;
	}

	return nullptr;
}

void ADT_CHUNK_Material::SetTexture(uint8 type, std::shared_ptr<Texture> texture)
{
	m_Textures[type] = texture;
	m_Dirty = true;
}

uint32 ADT_CHUNK_Material::GetLayersCnt() const
{
	return m_pProperties->m_LayersCnt;
}

void ADT_CHUNK_Material::SetLayersCnt(uint32 value)
{
	m_pProperties->m_LayersCnt = value;
	m_Dirty = true;
}

//--

Material::TextureMap ADT_CHUNK_Material::GetTextureMap() const
{
	return m_Textures;
}

std::shared_ptr<ConstantBuffer> ADT_CHUNK_Material::GetConstantBuffer() const
{
	return m_pConstantBuffer;
}

void ADT_CHUNK_Material::UpdateConstantBuffer() const
{
	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->Set(*m_pProperties);
	}
}
