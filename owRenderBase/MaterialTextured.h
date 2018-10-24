#pragma once

#include "Material.h"
class Texture;
class ConstantBuffer;

class MaterialTextured : public Material
{
public:
	MaterialTextured(RenderDevice* renderDevice);
	virtual ~MaterialTextured();

	std::shared_ptr<Texture> GetTexture() const;
	void SetTexture(std::shared_ptr<Texture> texture);

private:
	virtual TextureMap GetTextureMap() const;

	// Textures
	TextureMap m_Textures;
};