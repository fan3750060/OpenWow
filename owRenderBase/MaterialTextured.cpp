#include "stdafx.h"

// Include
#include "RenderDevice.h"
#include "Texture.h"

// General
#include "MaterialTextured.h"

MaterialTextured::MaterialTextured(IRenderDevice* renderDevice)
	: Material(renderDevice)
{
}

MaterialTextured::~MaterialTextured()
{
}

std::shared_ptr<Texture> MaterialTextured::GetTexture() const
{
	if (m_Textures.begin() != m_Textures.end())
	{
		return m_Textures.begin()->second;
	}

	return nullptr;
}

void MaterialTextured::SetTexture(std::shared_ptr<Texture> texture)
{
	m_Textures[0] = texture;
	m_Dirty = true;
}
