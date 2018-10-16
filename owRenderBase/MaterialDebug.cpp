#include "stdafx.h"

// Include
#include "RenderDevice.h"
#include "Shader.h"
#include "ShaderParameter.h"
#include "Texture.h"
#include "ConstantBuffer.h"

// General
#include "MaterialDebug.h"

MaterialDebug::MaterialDebug(RenderDevice* renderDevice)
	: Material(renderDevice)
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	// Construct default material properties.
	*m_pProperties = MaterialProperties();

	// Initialize the constant buffer.
	m_pConstantBuffer = m_RenderDevice->CreateConstantBuffer(*m_pProperties);
}

MaterialDebug::~MaterialDebug()
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

void MaterialDebug::Bind(std::weak_ptr<Shader> wpShader) const
{
	std::shared_ptr<Shader> pShader = wpShader.lock();
	_ASSERT(pShader != NULL);

	if (m_Dirty)
	{
		// Make sure the constant buffer associated to this material is updated.
		MaterialDebug* _this = const_cast<MaterialDebug*>(this);
		_this->UpdateConstantBuffer();
		_this->m_Dirty = false;
	}

	// If the shader has a parameter called "MaterialDebug".
	ShaderParameter& materialParameter = pShader->GetShaderParameterByName("MaterialDebug");
	if (materialParameter.IsValid())
	{
		// Assign this material's constant buffer to it.
		materialParameter.Set<ConstantBuffer>(m_pConstantBuffer);
		// If the shader parameter is modified, they have to be rebound to update the rendering pipeline.
		materialParameter.Bind();
	}
}

cvec4 MaterialDebug::GetDiffuseColor() const
{
	return m_pProperties->m_DiffuseColor;
}

void MaterialDebug::SetDiffuseColor(cvec4 diffuse)
{
	m_pProperties->m_DiffuseColor = diffuse;
	m_Dirty = true;
}

void MaterialDebug::UpdateConstantBuffer()
{
	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->Set(*m_pProperties);
	}
}
