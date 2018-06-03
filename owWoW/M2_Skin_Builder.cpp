#include "stdafx.h"

// General
#include "M2_Skin_Builder.h"

CM2_Skin_Builder::CM2_Skin_Builder(MDX* _model, const SM2_SkinProfile& _skinProto, CM2_Skin* _skin, IFile* _file) :
	m_MDX(_model),
	m_Skin(_skin),
	m_SkinProto(_skinProto),
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
	// Skin data
	uint16*	t_verticesIndexes = (uint16*)(m_F->GetData() + m_SkinProto.vertices.offset);
	for (uint32 i = 0; i < m_SkinProto.vertices.size; i++)
	{
		verticesIndexes.push_back(t_verticesIndexes[i]);
	}

	uint16*	t_indexesIndexes = (uint16*)(m_F->GetData() + m_SkinProto.indices.offset);
	for (uint32 i = 0; i < m_SkinProto.indices.size; i++)
	{
		indexesIndexes.push_back(t_indexesIndexes[i]);
	}

	SM2_SkinBones* t_bonesIndexes = (SM2_SkinBones*)(m_F->GetData() + m_SkinProto.bones.offset);
	for (uint32 i = 0; i < m_SkinProto.bones.size; i++)
	{
		bonesIndexes.push_back(t_bonesIndexes[i]);
	}

	SM2_SkinSection* t_skins = (SM2_SkinSection*)(m_F->GetData() + m_SkinProto.submeshes.offset);
	for (uint32 i = 0; i < m_SkinProto.submeshes.size; i++)
	{
		skins.push_back(t_skins[i]);
	}

	SM2_SkinBatch* t_Batches = (SM2_SkinBatch*)(m_F->GetData() + m_SkinProto.batches.offset);
	for (uint32 i = 0; i < m_SkinProto.batches.size; i++)
	{
		batches.push_back(t_Batches[i]);
	}

	uint32	t_bonesMax = m_SkinProto.boneCountMax;
	assert1(t_bonesMax == 256);

	for (uint32 i = 0; i < m_SkinProto.submeshes.size; i++)
	{
		m_Skin->m_VisibleSubmeshes.push_back(true);
	}
}

void CM2_Skin_Builder::Step2InitBatches()
{
	for (auto it : batches)
	{
		MDX_Skin_Batch* batch = new MDX_Skin_Batch();

		uint16 m2SkinIndex = it.m2SkinIndex;

		batch->m2SkinIndex = m2SkinIndex;

		// Geometry data
		batch->m_SkinSection = skins[m2SkinIndex];

		// Get classes
		SM2_Material& rf = m_MDX->m_Materials[it.materialIndex];

		// Diffuse texture
		CM2_Part_Texture& texture = m_MDX->GetTexture(it.texture_Index);
		if (texture.m_SpecialTextures == -1)
		{
			batch->__material.SetDiffuseTexture(texture.m_Texture);
		}
		else
		{
			batch->__material.SetDiffuseTexture(_Render->TexturesMgr()->DefaultTexture());
			/*R_Texture* diffuseSpecialTexture = _parent->m_TextureReplaced[_parent->m_SpecialTextures[texlookup[it.texture_Index]]];

			if (diffuseSpecialTexture != nullptr)
			{
			batch->__material.SetDiffuseTexture(diffuseSpecialTexture);
			}*/
		}

		// Render state
		batch->__material.SetRenderState(rf.flags.TWOSIDED, rf.flags.DEPTHWRITE == 0, rf.flags.DEPTHTEST == 0);

		// Blend state
		batch->__blendMode = rf.blending_mode;
		switch (rf.blending_mode)
		{
			// 1 table
		case M2BLEND_OPAQUE:
			batch->__material.SetBlendEGxBlendIndex(0);
			break;

		case M2BLEND_ALPHA_KEY:
			batch->__material.SetBlendEGxBlendIndex(1);
			break;

		case M2BLEND_ALPHA:
			batch->__material.SetBlendEGxBlendIndex(2);
			break;

		case M2BLEND_NO_ALPHA_ADD:
			batch->__material.SetBlendEGxBlendIndex(10);
			break;

		case M2BLEND_ADD:
			batch->__material.SetBlendEGxBlendIndex(3);
			break;

		case M2BLEND_MOD:
			batch->__material.SetBlendEGxBlendIndex(4);
			break;

		case M2BLEND_MOD2X:
			batch->__material.SetBlendEGxBlendIndex(5);
			break;

		default:
			assert1(rf.blending_mode);
		}

		batch->__colorIndex = it.colorIndex;

		// R_Texture weight
		assert1(it.texture_WeightIndex != -1);
		assert1(m_MDX->m_Header.textureWeights.size > 0);
		batch->__textureWeight = m_MDX->m_TextureWeightsLookup[it.texture_WeightIndex];

		// Anim texture
		if (it.flags.Flag_TextureStatic)
		{
			batch->__textureAnims = -1;
		}
		else
		{
			batch->__textureAnims = m_MDX->m_TexturesTransformLookup[it.texture_TransformIndex];
		}

		m_Skin->m_Batches.push_back(batch);
	}
}

void CM2_Skin_Builder::StepBuildGeometry()
{
	vector<uint16> indices;
	for (uint32 i = 0; i < indexesIndexes.size(); i++)
	{
		indices.push_back(verticesIndexes[indexesIndexes[i]]);
	}


	// Begin geometry
	m_Skin->__geom = _Render->r.beginCreatingGeometry(_Render->Storage()->__layout_GxVBF_PBNT2);

	// Vertex params
	m_Skin->__geom->setGeomVertexParams(m_MDX->m_VBuffer, R_DataType::T_FLOAT, 0 * sizeof(float), sizeof(SM2_Vertex)); // pos 0-2
	m_Skin->__geom->setGeomVertexParams(m_MDX->m_VBuffer, R_DataType::T_FLOAT, 3 * sizeof(float), sizeof(SM2_Vertex)); // blend 3
	m_Skin->__geom->setGeomVertexParams(m_MDX->m_VBuffer, R_DataType::T_FLOAT, 4 * sizeof(float), sizeof(SM2_Vertex)); // index 4
	m_Skin->__geom->setGeomVertexParams(m_MDX->m_VBuffer, R_DataType::T_FLOAT, 5 * sizeof(float), sizeof(SM2_Vertex)); // normal 5-7
	m_Skin->__geom->setGeomVertexParams(m_MDX->m_VBuffer, R_DataType::T_FLOAT, 8 * sizeof(float), sizeof(SM2_Vertex)); // tc0 8-9
	m_Skin->__geom->setGeomVertexParams(m_MDX->m_VBuffer, R_DataType::T_FLOAT, 10 * sizeof(float), sizeof(SM2_Vertex)); // tc1 10-11

	// Index bufer
	R_Buffer* __ib = _Render->r.createIndexBuffer(indexesIndexes.size() * sizeof(uint16), indices.data());
	m_Skin->__geom->setGeomIndexParams(__ib, R_IndexFormat::IDXFMT_16);

	// Finish
	m_Skin->__geom->finishCreatingGeometry();
}
