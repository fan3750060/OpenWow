#include "stdafx.h"

// Include
#include "M2.h"
#include "M2_Base_Instance.h"
#include "M2_Skin_Builder.h"

// General
#include "M2_Skin_Batch.h"

CM2_Skin_Batch::CM2_Skin_Batch(const std::weak_ptr<const M2> _parentM2, std::shared_ptr<IMesh> _mesh) :
	MeshWrapper(_mesh),
	m_ParentM2(_parentM2),

	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	m_TestMaterial = std::make_shared<M2_Material>();
	SetType(SN_TYPE_M2);
}

void CM2_Skin_Batch::Render(RenderEventArgs & renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation, UINT indexCnt)
{
	/*uint32 meshPartID = m_SkinSection->getProto().meshPartID;
	if (!_instance->isMeshEnabled(meshPartID))
	{
		return;
	}*/

	// Model color
	/*bool isColorEnable = (m_Color != nullptr);
	pass->SetColorEnable(isColorEnable);
	if (isColorEnable)
	{
		pass->SetColor(m_Color->getValue());
	}*/

	// Textures
	for (uint32 i = 0; i < m_Textures.size(); i++)
	{
		m_TestMaterial->SetTexture(i, m_Textures[i].lock()->getTexture());
	}

	// Texture alpha
	/*bool isTextureWeightEnable = (m_TextureWeight != nullptr);
	pass->SetTextureWeightEnable(isTextureWeightEnable);
	if (isTextureWeightEnable)
	{
		pass->SetTextureWeight(m_TextureWeight->getValue());
	}*/

	// Texture transform
	/*bool isTextureTransformEnable = (m_TextureTransform != nullptr);
	pass->SetTextureAnimEnable(isTextureTransformEnable);
	if (isTextureTransformEnable)
	{
		pass->SetTextureAnimMatrix(m_TextureTransform->getValue());
	}*/

	SetMaterial(m_TestMaterial);
	MeshWrapper::Render(renderEventArgs, perObject, 0, m_SkinSection->getProto().indexCount);
	SetMaterial(nullptr);
}

/*void CM2_Skin_Batch::Render(CM2_Base_Instance* _instance)
{
	uint32 meshPartID = m_SkinSection->getProto().meshPartID;

	if (!_instance->isMeshEnabled(meshPartID))
	{
		return;
	}

	//--

	CM2_Pass* pass = _Render->getTechniquesMgr()->M2_Pass.operator->();
	{
		pass->SetShader(newShader);
		pass->SetBlendMode(m_Material->getBlendMode());

		// Model color
		bool isColorEnable = (m_Color != nullptr);
		pass->SetColorEnable(isColorEnable);
		if (isColorEnable)
		{
			pass->SetColor(m_Color->getValue());
		}

		// Bind textures
		for (uint32 i = 0; i < m_Textures.size(); i++)
		{
			m_Textures[i]->set(&m_State, Material::C_DiffuseTextureIndex + i, _instance);
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

	//m_SkinSection->Draw(&m_State, _instance);
}*/
