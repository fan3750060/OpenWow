#pragma once

// FORWARD BEGIN
class MaterialWrapper;
// FORWARD END

class MaterialTextured : public MaterialWrapper
{
public:
	MaterialTextured(std::shared_ptr<Material> _material);
	virtual ~MaterialTextured();

	std::shared_ptr<Texture> GetTexture() const;
	void SetTexture(std::shared_ptr<Texture> texture);
};