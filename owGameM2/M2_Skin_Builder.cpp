#include "stdafx.h"

// General
#include "M2_Skin_Builder.h"

// Additional
#include "ShaderResolver.h"

CM2_Skin_Builder::CM2_Skin_Builder(M2* _model, CM2_Skin* _skin, IFile* _file) :
	m_ParentM2(_model),
	m_Skin(_skin),
	m_F(_file)
{
}

CM2_Skin_Builder::~CM2_Skin_Builder()
{
}

void CM2_Skin_Builder::Load()
{
	Step1LoadProfile();
	Step2InitBatches();
	StepBuildGeometry();
}

void CM2_Skin_Builder::Step1LoadProfile()
{
	m_F->readBytes(&m_SkinProto, sizeof(SM2_SkinProfile));

	// Skin data
	uint16*	t_verticesIndexes = (uint16*)(m_F->getData() + m_SkinProto.vertices.offset);
	for (uint32 i = 0; i < m_SkinProto.vertices.size; i++)
	{
		m_VerticesIndexes.push_back(t_verticesIndexes[i]);
	}

	uint16*	t_indexesIndexes = (uint16*)(m_F->getData() + m_SkinProto.indices.offset);
	for (uint32 i = 0; i < m_SkinProto.indices.size; i++)
	{
		m_IndexesIndexes.push_back(t_indexesIndexes[i]);
	}

	SM2_SkinBones* t_bonesIndexes = (SM2_SkinBones*)(m_F->getData() + m_SkinProto.bones.offset);
	for (uint32 i = 0; i < m_SkinProto.bones.size; i++)
	{
		m_SkinBones.push_back(t_bonesIndexes[i]);
	}

	SM2_SkinSection* t_skins = (SM2_SkinSection*)(m_F->getData() + m_SkinProto.submeshes.offset);
	for (uint32 i = 0; i < m_SkinProto.submeshes.size; i++)
	{
		m_SkinSections.push_back(t_skins[i]);
	}

	SM2_SkinBatch* t_Batches = (SM2_SkinBatch*)(m_F->getData() + m_SkinProto.batches.offset);
	for (uint32 i = 0; i < m_SkinProto.batches.size; i++)
	{
		m_SkinBatches.push_back(t_Batches[i]);
	}

	uint32	t_bonesMax = m_SkinProto.boneCountMax;
	//assert1(t_bonesMax == 256);
}

void CM2_Skin_Builder::Step2InitBatches()
{
	for (auto& it : m_SkinBatches)
	{
		M2_Skin_Batch* batch = new M2_Skin_Batch(m_ParentM2);

		if (it.shader_id < 0)
		{
			Log::Error("Shader is [%d]", it.shader_id);
			fail1();
		}

		// Geometry data
		batch->m_SkinIndex = it.skinSectionIndex;
		batch->m_SkinSection = m_SkinSections[it.skinSectionIndex];

		batch->newShader = GetPixel(&it);

		//const CM2_Part_Bone& bone = m_ParentM2->m_Bones[batch->m_SkinSection.bonesStartIndex + batch->m_SkinSection.centerBoneIndex];
		//batch->m_IsBilldoard = bone.IsBillboard();

		/*for (uint32 i = 0; i < batch->m_SkinSection.boneCount; i++)
		{
			const CM2_Part_Bone& bone = m_ParentM2->getBone(batch->m_SkinSection.bonesStartIndex + i);
			if (bone.IsBillboard())
			{
				Log::Warn("Bone [%s] [%d] is billbord!!!", m_ParentM2->getFilename().c_str(), batch->m_SkinSection.bonesStartIndex + i, bone.IsBillboard());
				batch->m_IsBilldoard = true;
				break;
			}
		}*/


		// Get classes
		batch->material = &(m_ParentM2->GetMaterial(it.materialIndex));

		// Textures
		for (uint32 i = 0; i < it.textureCount; i++)
		{
			batch->m_Textures.push_back(&(m_ParentM2->GetTexture(it.texture_Index + i)));
		}

		// Color
		if (it.colorIndex != UINT16_MAX)
		{
			batch->color = &(m_ParentM2->GetColor(it.colorIndex));
		}

		// Texture weight
		if (it.texture_WeightIndex != UINT16_MAX)
		{
			batch->texture_WeightIndex = &(m_ParentM2->m_TextureWeights[it.texture_WeightIndex]);
		}

		// Texture transfowm
		if (!(it.flags.TextureStatic))
		{
			if (it.texture_TransformIndex != UINT16_MAX)
			{
				uint16 index = m_ParentM2->m_TexturesTransformLookup[it.texture_TransformIndex];
				if (index != UINT16_MAX)
					batch->texture_Transform = &(m_ParentM2->GetTextureTransform(it.texture_TransformIndex));
			}
		}

		m_Skin->m_Batches.push_back(batch);
	}
}

void CM2_Skin_Builder::StepBuildGeometry()
{
	vector<uint16> indices;
	for (uint32 i = 0; i < m_IndexesIndexes.size(); i++)
	{
		indices.push_back(m_VerticesIndexes[m_IndexesIndexes[i]]);
	}

	// Index bufer
	R_Buffer* __ib = _Render->r.createIndexBuffer(m_IndexesIndexes.size() * sizeof(uint16), indices.data(), false);

	// Begin geometry
	m_Skin->__geom = _Render->r.beginCreatingGeometry(_Render->getRenderStorage()->__layout_GxVBF_PBNT2);
	m_Skin->__geom->setGeomVertexParams(m_ParentM2->m_VBuffer, R_DataType::T_FLOAT, 0 * sizeof(float), sizeof(SM2_Vertex)); // pos 0-2
	m_Skin->__geom->setGeomVertexParams(m_ParentM2->m_VBuffer, R_DataType::T_FLOAT, 3 * sizeof(float), sizeof(SM2_Vertex)); // blend 3
	m_Skin->__geom->setGeomVertexParams(m_ParentM2->m_VBuffer, R_DataType::T_FLOAT, 4 * sizeof(float), sizeof(SM2_Vertex)); // index 4
	m_Skin->__geom->setGeomVertexParams(m_ParentM2->m_VBuffer, R_DataType::T_FLOAT, 5 * sizeof(float), sizeof(SM2_Vertex)); // normal 5-7
	m_Skin->__geom->setGeomVertexParams(m_ParentM2->m_VBuffer, R_DataType::T_FLOAT, 8 * sizeof(float), sizeof(SM2_Vertex)); // tc0 8-9
	m_Skin->__geom->setGeomVertexParams(m_ParentM2->m_VBuffer, R_DataType::T_FLOAT, 10 * sizeof(float), sizeof(SM2_Vertex)); // tc1 10-11
	m_Skin->__geom->setGeomIndexParams(__ib, R_IndexFormat::IDXFMT_16);
	m_Skin->__geom->finishCreatingGeometry();

	////////////////////////////
	// Debug geom
	m_Skin->__geomDebugNormals = _Render->r.beginCreatingGeometry(_Render->getRenderStorage()->__layout_GxVBF_PN);
	m_Skin->__geomDebugNormals->setGeomVertexParams(m_ParentM2->m_VBuffer, R_DataType::T_FLOAT, 0 * sizeof(float), sizeof(SM2_Vertex));
	m_Skin->__geomDebugNormals->setGeomVertexParams(m_ParentM2->m_VBuffer, R_DataType::T_FLOAT, 5 * sizeof(float), sizeof(SM2_Vertex));
	m_Skin->__geomDebugNormals->setGeomIndexParams(__ib, R_IndexFormat::IDXFMT_16);
	m_Skin->__geomDebugNormals->finishCreatingGeometry();
}
