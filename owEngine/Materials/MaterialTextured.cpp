#include "stdafx.h"

// General
#include "MaterialTextured.h"

MaterialTextured::MaterialTextured(std::shared_ptr<Material> _material)
	: MaterialWrapper(_material)
{
}

MaterialTextured::~MaterialTextured()
{
}

std::shared_ptr<Texture> MaterialTextured::GetTexture() const
{
	return MaterialWrapper::GetTexture(0);
}

void MaterialTextured::SetTexture(std::shared_ptr<Texture> texture)
{
	MaterialWrapper::SetTexture(0, texture);
}
