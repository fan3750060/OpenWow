#include "stdafx.h"

// Include
#include "M2.h"
#include "M2_Skin_Builder.h"

// General
#include "M2_SkinSection.h"

CM2_SkinSection::CM2_SkinSection(M2* _model, const SM2_SkinSection& _proto) :
	m_ParentM2(_model),
	m_Proto(_proto),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{

}

void CM2_SkinSection::Final()
{
	R_Buffer* __vb = _Render->r.createVertexBuffer(static_cast<uint32>(m_Verts.size()) * sizeof(SM2_Vertex), m_Verts.data(), false);
	R_Buffer* __ib = _Render->r.createIndexBuffer(static_cast<uint32>(m_Index.size()) * sizeof(uint16), m_Index.data(), false);

	// Begin geometry
	__geom = _Render->r.beginCreatingGeometry(PRIM_TRILIST, _Render->getRenderStorage()->__layout_GxVBF_PBNT2);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 0 * sizeof(float), sizeof(SM2_Vertex)); // pos 0-2
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 3 * sizeof(float), sizeof(SM2_Vertex)); // blend 3
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 4 * sizeof(float), sizeof(SM2_Vertex)); // index 4
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 5 * sizeof(float), sizeof(SM2_Vertex)); // normal 5-7
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 8 * sizeof(float), sizeof(SM2_Vertex)); // tc0 8-9
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 10 * sizeof(float), sizeof(SM2_Vertex)); // tc1 10-11
	__geom->setGeomIndexParams(__ib, R_IndexFormat::IDXFMT_16);
	__geom->finishCreatingGeometry();

	m_Verts.clear();
	m_Index.clear();
}

void CM2_SkinSection::Draw(RenderState* _state, uint16 _animationIndex, cmat4 _worldMatrix, uint32 _time, uint32 globalTime)
{
	CM2_Pass* pass = _Render->getTechniquesMgr()->M2_Pass;

	bool isAnimated =  m_ParentM2->m_HasBones /*&& m_ParentM2->m_IsAnimated*/;
	pass->SetAnimated(isAnimated);
	if (isAnimated)
	{
		pass->SetBonesMaxInfluences(m_Proto.boneInfluences);

		/*for (uint32 i = m_Proto.bonesStartIndex; i < m_Proto.bonesStartIndex + m_Proto.boneCount; i++)
		{
			m_ParentM2->m_Bones[m_ParentM2->m_BonesLookup[i]]->calcMatrix(_animationIndex, _time, globalTime);
		}

		for (uint32 i = m_Proto.bonesStartIndex; i < m_Proto.bonesStartIndex + m_Proto.boneCount; i++)
		{
			m_ParentM2->m_Bones[m_ParentM2->m_BonesLookup[i]]->calcBillboard(_worldMatrix);
		}*/

		vector<mat4> bones;
		for (uint32 i = m_Proto.bonesStartIndex; i < m_Proto.bonesStartIndex + m_Proto.boneCount; i++)
		{
			assert1(m_ParentM2->m_BonesLookup[i] != -1);
			bones.push_back(m_ParentM2->m_Bones[m_ParentM2->m_BonesLookup[i]]->getTransformMatrix());
		}

		pass->SetBones(bones);
	}

	_state->setGeometry(__geom);

	_Render->r.drawIndexed
	(
		0,
		getProto().indexCount,
		0,
		getProto().vertexCount,
		_state,
		false
	);
}
