#pragma once

#include "M2_Types.h"

// FORWARD BEGIN
class CM2_Skin_Builder;
// FORWARD END

class CM2_Part_Texture
{
public:
	CM2_Part_Texture(IFile* f, const SM2_Texture& _proto);

	R_Texture* getTexture() const { return m_Texture; }
	int	getSpecialTexture() const { return m_SpecialTextures; }

private:
	SmartTexturePtr	m_Texture;
	int				m_SpecialTextures;
	SmartTexturePtr	m_TextureReplaced;
	bool			m_TexturesUseSpecialTexture;
};