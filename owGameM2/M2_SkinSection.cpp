#include "stdafx.h"

// Include
#include "M2.h"
#include "M2_Base_Instance.h"
#include "M2_Skin_Builder.h"

// General
#include "M2_SkinSection.h"

CM2_SkinSection::CM2_SkinSection(const std::weak_ptr<const M2> _model, const uint16 _index, const SM2_SkinSection& _proto) :
	m_Index(_index),
	m_Proto(_proto),
	m_ParentM2(_model),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{

}

void CM2_SkinSection::CreateGeometry(const std::vector<SM2_Vertex>& _vertexes, const std::vector<uint16>& _indexes)
{
	//std::shared_ptr<Buffer> __vb = _Render->r.createVertexBuffer(static_cast<uint32>(_vertexes.size()) * sizeof(SM2_Vertex), _vertexes.data(), false);
	std::shared_ptr<Buffer> __ib = _RenderDevice->CreateIndexBuffer(_indexes);

	//std::shared_ptr<Buffer> __vbPos = _RenderDevice->CreateFloatBuffer(

	// Begin geometry
	/*__geom = _Render->r.beginCreatingGeometry(PRIM_TRILIST, _Render->getRenderStorage()->__layout_GxVBF_PBNT2);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 0 * sizeof(float), sizeof(SM2_Vertex)); // pos 0-2
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 3 * sizeof(float), sizeof(SM2_Vertex)); // blend 3
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 4 * sizeof(float), sizeof(SM2_Vertex)); // index 4
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 5 * sizeof(float), sizeof(SM2_Vertex)); // normal 5-7
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 8 * sizeof(float), sizeof(SM2_Vertex)); // tc0 8-9
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 10 * sizeof(float), sizeof(SM2_Vertex)); // tc1 10-11
	__geom->setGeomIndexParams(__ib, R_IndexFormat::IDXFMT_16);
	__geom->finishCreatingGeometry();*/
}

void CM2_SkinSection::Draw(CM2_Base_Instance* _instance)
{
	/*CM2_Pass* pass = _Render->getTechniquesMgr()->M2_Pass.operator->();

	CM2_Comp_Skeleton* skeleton = m_ParentM2->getSkeleton();

	bool isAnimated = skeleton->hasBones() && m_ParentM2->m_IsAnimated;
	pass->SetAnimated(isAnimated);
	if (isAnimated)
	{
		pass->SetBonesMaxInfluences(m_Proto.boneInfluences);

		//for (uint16 i = m_Proto.bonesStartIndex; i < m_Proto.bonesStartIndex + m_Proto.boneCount; i++)
		//	skeleton->getBoneLookup(i)->SetNeedCalculate();

		//for (uint16 i = m_Proto.bonesStartIndex; i < m_Proto.bonesStartIndex + m_Proto.boneCount; i++)
		//	skeleton->getBoneLookup(i)->calcMatrix(_instance->getAnimator()->getSequenceIndex(), _instance->getAnimator()->getCurrentTime(), _instance->m_Time);

		//for (uint16 i = m_Proto.bonesStartIndex; i < m_Proto.bonesStartIndex + m_Proto.boneCount; i++)
		//	skeleton->getBoneLookup(i)->calcBillboard(_instance->GetWorldTransfom());

		std::vector<mat4> bones;
		for (uint16 i = m_Proto.bonesStartIndex; i < m_Proto.bonesStartIndex + m_Proto.boneCount; i++)
		{
			assert1(skeleton->isLookupBoneCorrect(i));
			bones.push_back(skeleton->getBoneLookup(i)->getTransformMatrix());
		}

		pass->SetBones(bones);
	}

	_state->setGeometry(__geom);

	_Render->r.drawIndexed
	(
		0, getProto().indexCount,
		0, getProto().vertexCount,
		_state,	true
	);*/
}
