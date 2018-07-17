#include "stdafx.h"

// Include
#include "M2_Builder.h"

// General
#include "M2_Skin_Builder.h"

// Additional
#include "ShaderResolver.h"

CM2_Skin_Builder::CM2_Skin_Builder(CM2_Builder* _m2Builder, M2* _model, CM2_Skin* _skin, IFile* _file) :
	m_M2Builder(_m2Builder),
	m_ParentM2(_model),
	m_Skin(_skin),
	m_F(_file)
{}

CM2_Skin_Builder::~CM2_Skin_Builder()
{}

void CM2_Skin_Builder::Load()
{
	Step1LoadProfile();
	Step2InitBatches();
}

void CM2_Skin_Builder::Step1LoadProfile()
{
	m_F->readBytes(&m_SkinProfile, sizeof(SM2_SkinProfile));

	// Skin data
	uint16*				t_verticesIndexes	= (uint16*)				(m_F->getData() + m_SkinProfile.vertices.offset);
	uint16*				t_indexesIndexes	= (uint16*)				(m_F->getData() + m_SkinProfile.indices.offset);
	SM2_SkinBones*		t_bonesIndexes		= (SM2_SkinBones*)		(m_F->getData() + m_SkinProfile.bones.offset);
	SM2_SkinSection*	t_sections			= (SM2_SkinSection*)	(m_F->getData() + m_SkinProfile.submeshes.offset);
	SM2_SkinBatch*		t_Batches			= (SM2_SkinBatch*)		(m_F->getData() + m_SkinProfile.batches.offset);

	assert1(m_SkinProfile.vertices.size == m_SkinProfile.bones.size);

	for (uint32 sectionIndex = 0; sectionIndex < m_SkinProfile.submeshes.size; sectionIndex++)
	{
		const SM2_SkinSection& section = t_sections[sectionIndex];
		CM2_SkinSection* skinSection = new CM2_SkinSection(m_ParentM2, sectionIndex, section);

		vector<SM2_Vertex> vertexes;
		for (uint32 vert = section.vertexStart; vert < section.vertexStart + section.vertexCount; vert++)
		{
			SM2_Vertex newVertex = m_M2Builder->m_Vertexes[t_verticesIndexes[vert]];
			for (uint32 bone = 0; bone < section.boneInfluences; ++bone)
			{
				uint8 boneIndex = t_bonesIndexes[vert].index[bone];
				newVertex.bone_indices[bone] = boneIndex;
				assert1(boneIndex < section.boneCount);
			}
			vertexes.push_back(newVertex);
		}

		vector<uint16> indexes;
		for (uint16 ind = section.indexStart; ind < section.indexStart + section.indexCount; ind++)
		{
			uint16 index = t_indexesIndexes[ind];
			assert1(index >= section.vertexStart);
			assert1(index < section.vertexStart + section.vertexCount);
			indexes.push_back(index - section.vertexStart);
		}

		skinSection->CreateGeometry(vertexes, indexes);

		m_Skin->m_Sections.push_back(skinSection);
	}

	//--

	for (uint32 i = 0; i < m_SkinProfile.batches.size; i++)
	{
		m_SkinBatches.push_back(t_Batches[i]);
	}

	uint32	t_bonesMax = m_SkinProfile.boneCountMax;
	//assert1(t_bonesMax == 256);
	//Log::Warn("t_bonesMax = %d", t_bonesMax);
}

void CM2_Skin_Builder::Step2InitBatches()
{
	CM2_Comp_Materials* materials = m_ParentM2->getMaterials();

	for (auto& it : m_SkinBatches)
	{
		CM2_Skin_Batch* batch = new CM2_Skin_Batch(m_ParentM2);

		batch->newShader = GetPixel(&it);

		// Geometry data
		batch->m_PriorityPlan = it.priorityPlane;
		batch->m_SkinSection = m_Skin->m_Sections[it.skinSectionIndex];

		// Get classes
		batch->m_Material = (materials->GetMaterial(it.materialIndex));

		// Color
		if (it.colorIndex != -1)
		{
			batch->m_Color = (materials->GetColor(it.colorIndex));
		}

		// Textures
		for (uint32 i = 0; i < it.textureCount; i++)
		{
			batch->m_Textures.push_back((materials->GetTexture(it.texture_Index + i)));
		}

		// Texture unit
		if (it.texture_CoordIndex != -1)
		{
			batch->m_TextureUnit = materials->m_TexturesUnitLookup[it.texture_CoordIndex];
		}

		// Texture weight
		if (it.texture_WeightIndex != -1)
		{
			batch->m_TextureWeight = (materials->m_TextureWeights[it.texture_WeightIndex]);
		}

		// Texture transfowm
		if (it.flags.TextureStatic == false)
		{
			if (it.texture_TransformIndex != -1)
			{
				int16 index = materials->m_TexturesTransformLookup[it.texture_TransformIndex];
				if (index != -1)
				{
					batch->m_TextureTransform = (materials->GetTextureTransform(it.texture_TransformIndex));
				}
			}
		}

		batch->Init();

		m_Skin->m_Batches.push_back(batch);
	}

	std::sort(m_Skin->m_Batches.begin(), m_Skin->m_Batches.end(), [](const CM2_Skin_Batch* left, const CM2_Skin_Batch* right)
	{
		return left->getPriorityPlan() < right->getPriorityPlan();
	});
}