#include "stdafx.h"

// General
#include "M2_MeshPartID_Provider.h"

CM2_MeshPartID_Provider::CM2_MeshPartID_Provider()
{
	for (uint8 i = 0; i < SM2_Texture::SM2_Texture_Type::TEX_COMPONENT_COUNT; i++)
	{
		m_SpecialTextures[i] = _Render->TexturesMgr()->DefaultTexture();
	}

	for (uint32 i = 0; i < MeshIDType::Count; i++)
	{
		m_MeshID[i] = 1;
	}

	m_MeshID[Ears] = 2;
}

void CM2_MeshPartID_Provider::setSpecialTexture(SM2_Texture::SM2_Texture_Type _type, cstring _name)
{
	m_SpecialTextures[_type] = GetManager<ITexturesManager>()->Add(_name);
}

R_Texture* CM2_MeshPartID_Provider::getSpecialTexture(SM2_Texture::SM2_Texture_Type _type) const
{
	//if (_type < SM2_Texture::SM2_Texture_Type::TEX_COMPONENT_COUNT)
	{
		return m_SpecialTextures[_type];
	}

	//return _Render->TexturesMgr()->DefaultTexture();
}



bool CM2_MeshPartID_Provider::isMeshEnabled(uint32 _index)
{
	uint32 div100 = _index / 100;
	uint32 mod100 = _index % 100;

	for (uint32 i = 0; i < MeshIDType::Count; i++)
	{
		// Special case for skin
		if (div100 == 0 && mod100 == 0)
		{
			return true;
		}

		// Others
		if (div100 == i)
		{
			if (m_MeshID[i] == mod100)
			{
				return true;
			}
		}
	}

	return false;
}
