#pragma once

#include "M2_Types.h"

// FORWARD BEGIN
class CM2_Skin_Builder;
// FORWARD END

class CM2_Part_Texture
{
	friend class MDX;
	friend class CM2_Skin;
	friend class CM2_Skin_Builder;
public:
	CM2_Part_Texture();

	void init(IFile* f, SM2_Texture& data);

	R_Texture* getTexture() { return m_Texture; }

private:
	SmartTexturePtr	m_Texture;
	int				m_SpecialTextures;
	SmartTexturePtr	m_TextureReplaced;
	bool			m_TexturesUseSpecialTexture;
};