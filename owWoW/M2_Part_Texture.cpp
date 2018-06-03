#include "stdafx.h"

// General
#include "M2_Part_Texture.h"

CM2_Part_Texture::CM2_Part_Texture() :
	m_Texture(nullptr),
	m_SpecialTextures(-1),
	m_TextureReplaced(nullptr),
	m_TexturesUseSpecialTexture(false)
{
}

void CM2_Part_Texture::init(IFile* f, SM2_Texture& data)
{
	if (data.type == 0) // Common texture
	{
		char buff[256];
		strncpy_s(buff, (const char*)(f->GetData() + data.filename.offset), data.filename.size);
		buff[data.filename.size] = '\0';
		m_Texture = _Render->TexturesMgr()->Add(buff);
	}
	else // special texture - only on characters and such...
	{
		/*m_Textures[i] = nullptr;
		m_SpecialTextures[i] = m_M2Textures[i].type;

		if (m_M2Textures[i].type < TEXTURE_MAX)
		{
		m_TexturesUseSpecialTexture[m_M2Textures[i].type] = true;
		}

		// a fix for weapons with type-3 m_DiffuseTextures.
		if (m_M2Textures[i].type == 3)
		{
		m_TextureReplaced[m_M2Textures[i].type] = _Render->TexturesMgr()->Add("Item\\ObjectComponents\\Weapon\\ArmorReflect4.BLP");
		}*/
	}
}
