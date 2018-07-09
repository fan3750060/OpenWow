#pragma once

#include "M2_Types.h"

// FORWARD BEGIN
class CM2_MeshPartID_Provider;
// FORWARD END

class CM2_Part_Texture
{
public:
	CM2_Part_Texture(IFile* f, const SM2_Texture& _proto);

	void set(RenderState* _state, uint32 _slot, CM2_MeshPartID_Provider* _provider) const;

	// Common texture
	R_Texture* getTexture() const { return m_Texture; }
	// Special texture
	bool isTextureSpecial() const { return m_SpecialType != SM2_Texture::SM2_Texture_Type::NONE; }
	SM2_Texture::SM2_Texture_Type getSpecialTextureType() const { return m_SpecialType; }

private:
	bool								m_WrapX;
	bool								m_WrapY;

	// Common texture
	SmartTexturePtr						m_Texture;
	// Special texture
	SM2_Texture::SM2_Texture_Type		m_SpecialType;

	//--
	CGroupQuality&						m_QualitySettings;
};