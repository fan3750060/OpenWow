#include "stdafx.h"

// Include
#include "M2.h"
#include "M2_Skin.h"

// General
#include "M2_Skin_Batch.h"


CM2_Skin_Batch::CM2_Skin_Batch(const M2* _parentM2, const CM2_Skin* _parentSkin) :
	m_ParentM2(_parentM2),
	m_ParentSkin(_parentSkin),

	m_Color(nullptr),
	m_Material(nullptr),
	m_TextureWeight(nullptr),
	m_TextureTransform(nullptr),

	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{}

void CM2_Skin_Batch::Init()
{
	m_Material->fillRenderState(&m_State);
	m_State.setGeometry(m_ParentSkin->__geom);
}

void CM2_Skin_Batch::Render(CM2_MeshPartID_Provider* _provider)
{
	if (_provider != nullptr)
	{
		if (!_provider->isMeshEnabled(m_SkinProtoSection.meshPartID))
		{
			return;
		}
	}

	//--

	CM2_Pass* pass = _Render->getTechniquesMgr()->M2_Pass;
	{
		pass->SetShader(newShader);

		// Model color
		bool isColorEnable = (m_Color != nullptr);
		pass->SetColorEnable(isColorEnable);
		if (isColorEnable)
		{
			pass->SetColor(m_Color->getValue());
		}

		// Material
		pass->SetBlendMode(m_Material->getBlendMode());

		// Bind textures
		for (uint32 i = 0; i < m_Textures.size(); i++)
		{
			m_Textures[i]->set(&m_State, Material::C_DiffuseTextureIndex + i, _provider);
		}

		// Texture alpha
		bool isTextureWeightEnable = (m_TextureWeight != nullptr);
		pass->SetTextureWeightEnable(isTextureWeightEnable);
		if (isTextureWeightEnable)
		{
			pass->SetTextureWeight(m_TextureWeight->getValue());
		}

		// Texture transform
		bool isTextureTransformEnable = (m_TextureTransform != nullptr);
		pass->SetTextureAnimEnable(isTextureTransformEnable);
		if (isTextureTransformEnable)
		{
			pass->SetTextureAnimMatrix(m_TextureTransform->getValue());
		}
	}

	_Render->r.drawIndexed
	(
		PRIM_TRILIST,
		m_SkinProtoSection.indexStart,
		m_SkinProtoSection.indexCount,
		m_SkinProtoSection.vertexStart,
		m_SkinProtoSection.vertexCount,
		&m_State,
		false
	);
}
