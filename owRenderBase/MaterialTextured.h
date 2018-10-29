#pragma once

#include "Material.h"
class Texture;

class MaterialTextured : public Material
{
public:
	MaterialTextured(RenderDevice* renderDevice);
	virtual ~MaterialTextured();

	std::shared_ptr<Texture> GetTexture() const;
	void SetTexture(std::shared_ptr<Texture> texture);
};