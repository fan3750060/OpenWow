#pragma once

#include "M2_Types.h"

// FORWARD BEGIN
class CM2_Base_Instance;
// FORWARD END

class CM2_Part_Texture
{
public:
	CM2_Part_Texture(IFile* f, const SM2_Texture& _proto);

	//void set(RenderState* _state, uint32 _slot, CM2_Base_Instance* _instance) const;

	// Common texture
	std::shared_ptr<Texture> getTexture() const { return m_Texture; }
	// Special texture
	bool isTextureSpecial() const { return (m_SpecialType != SM2_Texture::Type::NONE); }
	SM2_Texture::Type getSpecialTextureType() const { return m_SpecialType; }

private:
	bool								m_WrapX;
	bool								m_WrapY;

	std::shared_ptr<Texture>					m_Texture;
	SM2_Texture::Type		m_SpecialType;

	//--
	CGroupQuality&						m_QualitySettings;
};