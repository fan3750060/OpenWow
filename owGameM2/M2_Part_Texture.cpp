#include "stdafx.h"

// Include 
#include "M2_MeshPartID_Provider.h"

// General
#include "M2_Part_Texture.h"

CM2_Part_Texture::CM2_Part_Texture(IFile* f, const SM2_Texture& _proto) :
	m_Texture(nullptr),

	m_TexturesUseSpecialTexture(false),
	m_SpecialType(SM2_Texture::SM2_Texture_Type::NONE),
	m_TextureReplaced(nullptr),
	
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
	m_TexturesUseSpecialTexture = true;
	m_SpecialType = _proto.type;
	m_TextureReplaced = _Render->TexturesMgr()->DefaultTexture();
	Log::Info("Special texture is [%d]", m_SpecialType);
}

void CM2_Part_Texture::UpdateReplacedTexture(CM2_MeshPartID_Provider* _provider)
{
	if (m_TexturesUseSpecialTexture)
	{
		m_TextureReplaced = _provider->getSpecialTexture(m_SpecialType);
	}
}

void CM2_Part_Texture::set(uint32 _slot) const
{
	uint16 sampler = m_QualitySettings.Texture_Sampler;

	if (isWrapX())
	{
		sampler |= SS_ADDRU_WRAP;
	}

	if (isWrapY())
	{
		sampler |= SS_ADDRV_WRAP;
	}

	if (m_TexturesUseSpecialTexture)
	{
		_Render->r.setTexture(_slot, getReplacedTexture(), sampler, 0);
		return;
	}

	_Render->r.setTexture(_slot, getTexture(), sampler, 0);
}
