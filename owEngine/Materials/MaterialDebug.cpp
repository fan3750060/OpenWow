#include "stdafx.h"

// General
#include "MaterialDebug.h"

MaterialDebug::MaterialDebug(std::shared_ptr<Material> _material)
	: MaterialWrapper(_material)
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	MaterialWrapper::CreateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
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
	MarkConstantBufferDirty();
}

void MaterialDebug::UpdateConstantBuffer() const
{
	MaterialWrapper::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
