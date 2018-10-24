#include "stdafx.h"

// Include
#include "RenderDevice.h"
#include "Shader.h"
#include "ShaderParameter.h"
#include "Texture.h"
#include "ConstantBuffer.h"

// General
#include "MaterialTextured.h"

MaterialTextured::MaterialTextured(RenderDevice* renderDevice)
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

//-----

Material::TextureMap MaterialTextured::GetTextureMap() const
{
	return m_Textures;
}
