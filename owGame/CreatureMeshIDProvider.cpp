#include "stdafx.h"

// General
#include "CreatureMeshIDProvider.h"

CCreatureMeshIDProvider::CCreatureMeshIDProvider()
{
	for (uint8 i = 0; i < SM2_Texture::SM2_Texture_Type::TEX_COMPONENT_COUNT; i++)
	{
		m_SpecialTextures[i] = _Render->TexturesMgr()->DefaultTexture();
	}
}

void CCreatureMeshIDProvider::setSpecialTexture(SM2_Texture::SM2_Texture_Type _type, cstring _name)
{
	m_SpecialTextures[_type] = GetManager<ITexturesManager>()->Add(_name);
}

R_Texture* CCreatureMeshIDProvider::getSpecialTexture(SM2_Texture::SM2_Texture_Type _type) const
{
	return m_SpecialTextures[_type];
}

bool CCreatureMeshIDProvider::isMeshEnabled(uint32 _index) const
{
	uint32 div100 = _index / 100;
	uint32 mod100 = _index % 100;

	//assert1(div100 == 0 && mod100 == 0);

	return true;
}
