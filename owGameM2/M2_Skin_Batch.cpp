#include "stdafx.h"

// Include
#include "M2.h"
#include "M2_Base_Instance.h"
#include "M2_Skin_Builder.h"

// General
#include "M2_Skin_Batch.h"

CM2_Skin_Batch::CM2_Skin_Batch(const std::weak_ptr<const M2> _parentM2, std::shared_ptr<IMesh> _mesh) :
	MeshWrapper(SN_TYPE_M2, _mesh),
	m_ParentM2(_parentM2),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	m_TestMaterial = std::make_shared<M2_Material>();

	SetMaterial(m_TestMaterial);
}

bool CM2_Skin_Batch::Render(RenderEventArgs& renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation, UINT indexCnt)
{
	// TODO: Shit code. Delete me later
	SceneNode* sceneNode = dynamic_cast<SceneNode*>(renderEventArgs.Node);
	CM2_Base_Instance* sceneNodeAsM2Instance = dynamic_cast<CM2_Base_Instance*>(sceneNode);

	const SM2_SkinSection& proto = m_SkinSection->getProto();

	uint32 meshPartID = proto.meshPartID;
	if (!sceneNodeAsM2Instance->isMeshEnabled(meshPartID))
	{
		return false;
	}

	std::shared_ptr<CM2_Comp_Skeleton> skeleton = m_ParentM2.lock()->getSkeleton();

	bool isAnimated = skeleton->hasBones() && m_ParentM2.lock()->m_IsAnimated;
	m_TestMaterial->SetAnimated(isAnimated ? 1 : 0);
	if (isAnimated)
	{
		m_TestMaterial->SetMaxInfluences(proto.boneInfluences);

		for (uint16 i = proto.bonesStartIndex; i < proto.bonesStartIndex + proto.boneCount; i++)
			skeleton->getBoneLookup(i)->SetNeedCalculate();

		for (uint16 i = proto.bonesStartIndex; i < proto.bonesStartIndex + proto.boneCount; i++)
			skeleton->getBoneLookup(i)->calcMatrix(sceneNodeAsM2Instance->getAnimator()->getSequenceIndex(), sceneNodeAsM2Instance->getAnimator()->getCurrentTime(), 0);

		//for (uint16 i = proto.bonesStartIndex; i < proto.bonesStartIndex + proto.boneCount; i++)
		//	skeleton->getBoneLookup(i)->calcBillboard(sceneNodeAsM2Instance->GetWorldTransfom());

		std::vector<mat4> bones;
		for (uint16 i = proto.bonesStartIndex; i < proto.bonesStartIndex + proto.boneCount; i++)
		{
			assert1(skeleton->isLookupBoneCorrect(i));
			bones.push_back(skeleton->getBoneLookup(i)->getTransformMatrix());
		}

		m_TestMaterial->SetBones(bones);
	}

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

	
	return MeshWrapper::Render(renderEventArgs, perObject, 0, proto.indexCount);
}
