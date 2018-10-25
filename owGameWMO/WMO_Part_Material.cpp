#include "stdafx.h"

// Include
#include "Wmo.h"

// General
#include "Wmo_Part_Material.h"

WMO_Part_Material::WMO_Part_Material(const WMO* _parentWMO, const SWMO_MaterialDef& _proto) :
	m_ParentWMO(_parentWMO),
	m_Proto(_proto),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	//m_DiffuseTexture[0] = Application::Get().GetRenderDevice()->CreateTexture2D(_parentWMO->m_TexturesNames + m_Proto.diffuseNameIndex);

	if (m_Proto.envNameIndex)
	{
		//m_DiffuseTexture[1] = Application::Get().GetRenderDevice()->CreateTexture2D(_parentWMO->m_TexturesNames + m_Proto.envNameIndex);
	}

	//Log::Warn("Shader = [%d], Blend mode [%d]", m_Proto.shader, m_Proto.blendMode);
	if (m_Proto.blendMode > 1)
	{
		int x = 0;
	}

	vec4 color = fromARGB(m_Proto.diffColor);
}

#ifdef GAME_WMO_INCLUDE_WM2
void WMO_Part_Material::fillRenderState(RenderState* _state) const
{
	uint16 sampler = m_QualitySettings.Texture_Sampler;
	sampler |= (m_Proto.flags.TextureClampS) ? SS_ADDRU_CLAMP : SS_ADDRU_WRAP;
	sampler |= (m_Proto.flags.TextureClampT) ? SS_ADDRV_CLAMP : SS_ADDRV_WRAP;

	_state->setTexture(Material::C_DiffuseTextureIndex + 0, m_DiffuseTexture[0], sampler, 0);
	_state->setTexture(Material::C_DiffuseTextureIndex + 1, m_DiffuseTexture[1], sampler, 0);
	_state->setCullMode(m_Proto.flags.IsTwoSided ? RS_CULL_NONE : RS_CULL_BACK);
	_Render->getRenderStorage()->SetEGxBlend(_state, m_Proto.blendMode);
}
#endif

void WMO_Part_Material::set() const
{
	/*uint16 sampler = m_QualitySettings.Texture_Sampler;
	sampler |= (m_Proto.flags.TextureClampS) ? SS_ADDRU_CLAMP : SS_ADDRU_WRAP;
	sampler |= (m_Proto.flags.TextureClampT) ? SS_ADDRV_CLAMP : SS_ADDRV_WRAP;

	_Render->r.setTexture(Material::C_DiffuseTextureIndex + 0, m_DiffuseTexture[0], sampler, 0);
	_Render->r.setTexture(Material::C_DiffuseTextureIndex + 1, m_DiffuseTexture[1], sampler, 0);

	_Render->r.setCullMode(m_Proto.flags.IsTwoSided ? RS_CULL_NONE : RS_CULL_BACK);

	//_Render->r.setAlphaToCoverage(true);

	_Render->getRenderStorage()->SetEGxBlend(_Render->r.getState(), m_Proto.blendMode);*/
}
