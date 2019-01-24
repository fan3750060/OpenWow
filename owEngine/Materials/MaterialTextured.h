#pragma once

#include "MaterialWrapper.h"

class MaterialTextured : public MaterialWrapper
{
public:
	MaterialTextured(std::shared_ptr<Material> _material);
	virtual ~MaterialTextured();

	std::shared_ptr<Texture> GetTexture() const;
	void SetTexture(std::shared_ptr<Texture> texture);
};