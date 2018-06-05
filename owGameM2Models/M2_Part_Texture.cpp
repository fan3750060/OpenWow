#include "stdafx.h"

// General
#include "M2_Part_Texture.h"

CM2_Part_Texture::CM2_Part_Texture(IFile* f, const SM2_Texture& _proto) :
	m_Texture(nullptr),
	m_SpecialTextures(-1),
	m_TextureReplaced(nullptr),
	m_TexturesUseSpecialTexture(false)
{
	string textureFileName = "";
	const char* FileName = (const char*)(f->GetData() + _proto.filename.offset);
	for (uint32 i = 0; i < _proto.filename.size; i++)
	{
		textureFileName += FileName[i];
	}

	if (_proto.type == 0) // Common texture
	{
		m_Texture = _Render->TexturesMgr()->Add(textureFileName);
	}
	else // special texture - only on characters and such...
	{
		Log::Green("Normal texture is [%s]", textureFileName);
		m_Texture = _Render->TexturesMgr()->Add(f->Path() + "RagnarosSkin.blp");
		/*m_SpecialTexture = _proto.type;

		if (_proto.type < TEXTURE_MAX)
		{
			m_TexturesUseSpecialTexture[_proto.type] = true;
		}

		// a fix for weapons with type-3 m_DiffuseTextures.
		if (_proto.type == 3)
		{
			m_TextureReplaced[_proto.type] = _Render->TexturesMgr()->Add("Item\\ObjectComponents\\Weapon\\ArmorReflect4.BLP");
		}*/
	}
}
