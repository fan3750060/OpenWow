#pragma once

#include "M2_Types.h"

// FORWARD BEGIN
class CM2_MeshPartID_Provider;
// FORWARD END

class CM2_Part_Texture
{
public:
	CM2_Part_Texture(IFile* f, const SM2_Texture& _proto);

	void UpdateReplacedTexture(CM2_MeshPartID_Provider* _provider);

	bool isWrapX() const { return m_WrapX; }
	bool isWrapY() const { return m_WrapY; }

	void set(uint32 _slot) const;

	// Common texture
	R_Texture* getTexture() const { return m_Texture; }

	// Special texture
	SM2_Texture::SM2_Texture_Type getSpecialTextureType() const { return m_SpecialType; }
	R_Texture* getReplacedTexture() const { return m_TextureReplaced; }

private:
	bool								m_WrapX;
	bool								m_WrapY;

	// Common texture
	SmartTexturePtr						m_Texture;

	// Replaced texture
	bool								m_TexturesUseSpecialTexture;
	SM2_Texture::SM2_Texture_Type		m_SpecialType;
	SmartTexturePtr						m_TextureReplaced;
	
	//--
	CGroupQuality&						m_QualitySettings;
};