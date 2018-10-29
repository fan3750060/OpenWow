#include "stdafx.h"

// Include
#include "RenderDevice.h"
#include "ConstantBuffer.h"

// General
#include "MaterialDebug.h"

MaterialDebug::MaterialDebug(RenderDevice* renderDevice)
	: Material(renderDevice)
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	m_pConstantBuffer = m_RenderDevice->CreateConstantBuffer(*m_pProperties);
}

MaterialDebug::~MaterialDebug()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

cvec4 MaterialDebug::GetDiffuseColor() const
{
	return m_pProperties->m_DiffuseColor;
}

//-----

void MaterialDebug::SetDiffuseColor(cvec4 diffuse)
{
	m_pProperties->m_DiffuseColor = diffuse;
	m_Dirty = true;
}

void MaterialDebug::UpdateConstantBuffer() const
{
	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->Set(*m_pProperties);
	}
}
