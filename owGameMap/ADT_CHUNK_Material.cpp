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
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

//--

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

void ADT_CHUNK_Material::UpdateConstantBuffer() const
{
	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->Set(*m_pProperties);
	}
}
