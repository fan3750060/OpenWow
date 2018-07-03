#include "stdafx.h"

// Include
#include "M2.h"

// General
#include "M2_Skin_Batch.h"


M2_Skin_Batch::M2_Skin_Batch(const M2* _parentM2) :
	m_ParentM2(_parentM2),

	color(nullptr),
	material(nullptr),
	texture_WeightIndex(nullptr),
	texture_Transform(nullptr),

	m_IsBilldoard(false),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{}

void M2_Skin_Batch::Render(CM2_MeshPartID_Provider* _provider)
{
	if (_provider != nullptr)
	{
		if (!_provider->isMeshEnabled(m_SkinProtoSection.meshPartID))
		{
			return;
		}
	}

	//--

	material->Set();

	CM2_Pass* pass = _Render->getTechniquesMgr()->M2_Pass;
	{
		pass->SetShader(newShader);
		pass->SetBillboard(m_IsBilldoard);

		// Model color
		bool isColorEnable = (color != nullptr);
		pass->SetColorEnable(isColorEnable);
		if (isColorEnable)
		{
			pass->SetColor(color->getValue());
		}

		// Material
		material->Set();

		// Bind textures
		for (uint32 i = 0; i < m_Textures.size(); i++)
		{
			m_Textures[i]->set(Material::C_DiffuseTextureIndex + i, _provider);
		}

		// Texture alpha
		bool isTextureWeightEnable = (texture_WeightIndex != nullptr);
		pass->SetTextureWeightEnable(isTextureWeightEnable);
		if (isTextureWeightEnable)
		{
			pass->SetTextureWeight(texture_WeightIndex->getValue());
		}

		// Texture transform
		bool isTextureTransformEnable = (texture_Transform != nullptr);
		pass->SetTextureAnimEnable(isTextureTransformEnable);
		if (isTextureTransformEnable)
		{
			pass->SetTextureAnimMatrix(texture_Transform->getValue());
		}
	}

	_Render->r.drawIndexed
	(
		PRIM_TRILIST,
		m_SkinProtoSection.indexStart,
		m_SkinProtoSection.indexCount,
		m_SkinProtoSection.vertexStart,
		m_SkinProtoSection.vertexCount,
		false
	);
}
