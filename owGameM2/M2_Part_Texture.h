#pragma once

#include "M2_Types.h"

// FORWARD BEGIN
class CM2_Skin_Builder;
// FORWARD END

class CM2_Part_Texture
{
public:
	CM2_Part_Texture(IFile* f, const SM2_Texture& _proto);

	bool isWrapX() const { return m_WrapX; }
	bool isWrapY() const { return m_WrapY; }

	void set(uint32 _slot) const;

	R_Texture* getTexture() const { return m_Texture; }
	int	getSpecialTexture() const { return m_SpecialTextures; }

private:
	bool			m_WrapX;
	bool			m_WrapY;

	SmartTexturePtr	m_Texture;
	int				m_SpecialTextures;
	SmartTexturePtr	m_TextureReplaced;
	bool			m_TexturesUseSpecialTexture;

	CGroupQuality&	m_QualitySettings;
};