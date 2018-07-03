#include "stdafx.h"

// Include 
#include "M2_MeshPartID_Provider.h"
#include "M2.h"

// General
#include "M2_Part_Texture.h"

CM2_Part_Texture::CM2_Part_Texture(IFile* f, const SM2_Texture& _proto) :
	m_Texture(nullptr),
	m_SpecialType(SM2_Texture::SM2_Texture_Type::NONE),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	m_WrapX = _proto.flags.WRAPX;
	m_WrapY = _proto.flags.WRAPY;

	
	// Common texture
	if (_proto.type == SM2_Texture::SM2_Texture_Type::NONE)
	{
		string textureFileName = (const char*)(f->getData() + _proto.filename.offset);
		m_Texture = _Render->TexturesMgr()->Add(textureFileName);
		return;
	}

	// Special
	m_SpecialType = _proto.type;
}

void CM2_Part_Texture::set(uint32 _slot, CM2_MeshPartID_Provider* _provider) const
{
	uint16 sampler = m_QualitySettings.Texture_Sampler;

	if (m_WrapX)
	{
		sampler |= SS_ADDRU_WRAP;
	}

	if (m_WrapY)
	{
		sampler |= SS_ADDRV_WRAP;
	}

	if (isTextureSpecial())
	{
		if (_provider != nullptr)
		{
			R_Texture* specialTexture = _provider->getSpecialTexture(m_SpecialType);
			assert1(specialTexture != nullptr);
			_Render->r.setTexture(_slot, specialTexture, sampler, 0);
		}
		return;
	}

	_Render->r.setTexture(_slot, getTexture(), sampler, 0);
}
