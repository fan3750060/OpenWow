#include "stdafx.h"

// Include
#include "MDX.h"

// General
#include "MDX_Skin.h"

Model_Skin::Model_Skin(MDX* _parent, File& f, M2SkinProfile& view) 
	: m_Parent(_parent)
{
	// Skin data
	uint16*        vertexesIndex = (uint16*)        (f.GetData() + view.vertices.offset);
	uint16*        indexesIndex  = (uint16*)        (f.GetData() + view.indices.offset);
	M2SkinBones*   bonesIndex    = (M2SkinBones*)        (f.GetData() + view.bones.offset);
	M2SkinSection* skins         = (M2SkinSection*) (f.GetData() + view.submeshes.offset);
	M2SkinBatch*   skinBatch     = (M2SkinBatch*)   (f.GetData() + view.batches.offset);
    uint32         bonesMax      = view.boneCountMax;

	//------------

	M2Material*    m2Materials =   (M2Material*)    (f.GetData() + _parent->header.materials.offset);
	uint16*        texlookup =     (uint16*)        (f.GetData() + _parent->header.texture_lookup_table.offset);
	uint16*        texanimlookup = (uint16*)        (f.GetData() + _parent->header.texture_transforms_lookup_table.offset);
	uint16*        texweightlookup=(uint16*)        (f.GetData() + _parent->header.transparency_lookup_table.offset);

	for (uint32 j = 0; j < view.batches.size; j++)
	{
		MDX_Skin_Batch* pass = new MDX_Skin_Batch();

		uint16 m2SkinIndex = skinBatch[j].m2SkinIndex;

		pass->m2SkinIndex = (int)m2SkinIndex;

		// Geometry data
		pass->indexStart = skins[m2SkinIndex].indexStart;
		pass->indexCount = skins[m2SkinIndex].indexCount;
		pass->vertexStart = skins[m2SkinIndex].vertexStart;
		pass->vertexCount = skins[m2SkinIndex].vertexCount;
        pass->boneInfluences = skins[m2SkinIndex].boneInfluences;

        // Bones
        pass->bonesStartIndex = skins[m2SkinIndex].bonesStartIndex;
        pass->bonesCount = skins[m2SkinIndex].boneCount;

		// Get classes
		M2Material& rf = m2Materials[skinBatch[j].materialIndex];

		// Diffuse texture
		if (_parent->m_SpecialTextures[texlookup[skinBatch[j].texture_Index]] == -1)
		{
			uint32 index = texlookup[skinBatch[j].texture_Index];
			if (index < _parent->m_Textures.size())
			{
				pass->__material.SetDiffuseTexture(_parent->m_Textures[index]);
			}
		}
		else
		{
			pass->__material.SetDiffuseTexture(_TexturesMgr->DefaultTexture());
			/*R_Texture* diffuseSpecialTexture = _parent->m_TextureReplaced[_parent->m_SpecialTextures[texlookup[skinBatch[j].texture_Index]]];

			if (diffuseSpecialTexture != nullptr)
			{
				pass->__material.SetDiffuseTexture(diffuseSpecialTexture);
			}*/
		}
		
		// Render state
		pass->__material.SetRenderState(rf.flags.M2MATERIAL_FLAGS_TWOSIDED, rf.flags.M2MATERIAL_FLAGS_DEPTHWRITE == 0, rf.flags.M2MATERIAL_FLAGS_DEPTHTEST == 0);

		// Blend state
		pass->__blendMode = rf.blending_mode;
		switch (rf.blending_mode)
		{
			// 1 table
			case M2BLEND_OPAQUE:
			pass->__material.SetBlendEGxBlendIndex(0);
			break;

			case M2BLEND_ALPHA_KEY:
			pass->__material.SetBlendEGxBlendIndex(1);
			break;

			case M2BLEND_ALPHA:
			pass->__material.SetBlendEGxBlendIndex(2);
			break;

			case M2BLEND_NO_ALPHA_ADD:
			pass->__material.SetBlendEGxBlendIndex(10);
			break;

			case M2BLEND_ADD:
			pass->__material.SetBlendEGxBlendIndex(3);
			break;

			case M2BLEND_MOD:
			pass->__material.SetBlendEGxBlendIndex(4);
			break;

			case M2BLEND_MOD2X:
			pass->__material.SetBlendEGxBlendIndex(5);
			break;

			default:
			assert1(rf.blending_mode);
		}
		
		pass->__colorIndex = skinBatch[j].colorIndex;

		// R_Texture weight
		assert1(skinBatch[j].texture_WeightIndex != -1);
		assert1(m_Parent->header.texture_weights.size > 0);
		pass->__textureWeight = texweightlookup[skinBatch[j].texture_WeightIndex];

		// Anim texture
		if (skinBatch[j].flags.Flag_TextureStatic)
		{
			pass->__textureAnims = -1;
		}
		else
		{
			pass->__textureAnims = texanimlookup[skinBatch[j].texture_TransformIndex];
		}

		/*pass->order = skinBatch[j].shader_id;

		//R_Texture* texid = m_DiffuseTextures[texlookup[tex[j].textureid]];
		//pass->texture = texid;
		pass->tex = texlookup[skinBatch[j].texture_Index];

		// TODO: figure out these flags properly -_-
		


		pass->blendmode = rf.blending_mode;
		pass->color = skinBatch[j].colorIndex;
		pass->opacity = _parent->transLookup[skinBatch[j].texture_WeightIndex];

		pass->unlit = (rf.flags.M2MATERIAL_FLAGS_UNLIT) != 0;
		pass->cull = (rf.flags.M2MATERIAL_FLAGS_TWOSIDED) == 0 && rf.blending_mode == 0;

		pass->billboard = (rf.flags.M2MATERIAL_FLAGS_DEPTHTEST) != 0;

		pass->useEnvMap = (texunitlookup[skinBatch[j].materialLayer] == -1) && pass->billboard && rf.blending_mode > 2;
		pass->noZWrite = (rf.flags.M2MATERIAL_FLAGS_DEPTHWRITE) != 0;

		// ToDo: Work out the correct way to get the true/false of transparency
		pass->trans = (pass->blendmode > 0) && (pass->opacity > 0);	// Transparency - not the correct way to get transparency

		pass->p = skins[m2SkinIndex].centerPosition.x;

		// R_Texture flags
		pass->swrap = (_parent->texdef[pass->tex].flags.M2TEXTURE_FLAGS_WRAPX) != 0; // R_Texture wrap X
		pass->twrap = (_parent->texdef[pass->tex].flags.M2TEXTURE_FLAGS_WRAPY) != 0; // R_Texture wrap Y

		if (_parent->m_TexturesAnims)
		{
			if (skinBatch[j].flags & TEXTUREUNIT_STATIC)
			{
				pass->texanim = -1; // no texture animation
			}
			else
			{
				pass->texanim = texanimlookup[skinBatch[j].texture_TransformIndex];
			}
		}
		else
		{
			pass->texanim = -1; // no texture animation
		}*/

		m_Batches.push_back(pass);
	}



	// transparent parts come later
	//std::sort(m_Batches.begin(), m_Batches.end());


	showGeosets = new bool[view.submeshes.size];
	for (uint32 i = 0; i < view.submeshes.size; i++)
	{
		showGeosets[i] = true;
	}

	uint16* indices = new uint16[view.indices.size];
	for (uint32 i = 0; i < view.indices.size; i++)
	{
		indices[i] = vertexesIndex[indexesIndex[i]];
	}


	// Begin geometry

	__geom = _Render->r->beginCreatingGeometry(_RenderStorage->__layout_GxVBF_PBNT2);
    
	// Vertex params
	_Render->r->setGeomVertexParams(__geom, m_Parent->__vb, R_DataType::T_FLOAT, 0 * sizeof(float), sizeof(M2Vertex)); // pos 0-2
	_Render->r->setGeomVertexParams(__geom, m_Parent->__vb, R_DataType::T_FLOAT, 3 * sizeof(float), sizeof(M2Vertex)); // blend 3
	_Render->r->setGeomVertexParams(__geom, m_Parent->__vb, R_DataType::T_FLOAT, 4 * sizeof(float), sizeof(M2Vertex)); // index 4
    _Render->r->setGeomVertexParams(__geom, m_Parent->__vb, R_DataType::T_FLOAT, 5 * sizeof(float), sizeof(M2Vertex)); // normal 5-7
    _Render->r->setGeomVertexParams(__geom, m_Parent->__vb, R_DataType::T_FLOAT, 8 * sizeof(float), sizeof(M2Vertex)); // tc0 8-9
    _Render->r->setGeomVertexParams(__geom, m_Parent->__vb, R_DataType::T_FLOAT, 10 * sizeof(float), sizeof(M2Vertex)); // tc1 10-11

	// Index bufer
    R_Buffer* __ib = _Render->r->createIndexBuffer(view.indices.size * sizeof(uint16), indices);
	_Render->r->setGeomIndexParams(__geom, __ib, R_IndexFormat::IDXFMT_16);

	// Finish
	_Render->r->finishCreatingGeometry(__geom);

	delete[] indices;
}

Model_Skin::~Model_Skin()
{
	delete[] showGeosets;
}

//

void Model_Skin::Draw()
{
	if (m_Batches.size() <= 0)
	{
		return;
	}

	bool one = false;
	for (uint32_t i = 0; i < m_Batches.size(); i++)
	{
		if (showGeosets[m_Batches[i]->m2SkinIndex])
		{
			one = true;
			break;
		}
	}

	assert1(one);

	_TechniquesMgr->m_Model->BindS();
	_TechniquesMgr->m_Model->SetPVW();

    _TechniquesMgr->m_Model->SetAnimated(m_Parent->animBones && m_Parent->animated);
    if (m_Parent->animBones && m_Parent->animated)
    {
        //_TechniquesMgr->m_Model->SetBoneStartIndex(p->bonesStartIndex); FIXME
        //_TechniquesMgr->m_Model->SetBoneMaxCount(p->boneInfluences);

        vector<mat4> bones;
        for (uint32 i = 0; i < m_Parent->header.bones.size; i++)
        {
            bones.push_back(m_Parent->m_Part_Bones[i].m_TransformMatrix);
        }
        _TechniquesMgr->m_Model->SetBones(bones);
    }

	_Render->r->setGeometry(__geom);

	for (uint32_t i = 0; i < m_Batches.size(); i++)
	{
		MDX_Skin_Batch* p = m_Batches[i];

		if (showGeosets[p->m2SkinIndex])
		{
			p->__material.Set();

			// Color
			if (p->__colorIndex != 65535)
			{
				_TechniquesMgr->m_Model->SetColor(m_Parent->m_Colors[p->__colorIndex].getValue());
			}
		    else
			{
				_TechniquesMgr->m_Model->SetColor(vec4(0.5f, 0.5f, 0.5f, 1.0f));
			}

			// Blend & Alpha
			_TechniquesMgr->m_Model->SetBlendMode(p->__blendMode);

			// R_Texture weight
			_TechniquesMgr->m_Model->SetTextureWeight(m_Parent->m_TextureWeights[p->__textureWeight].getValue());

			// Billboard
			_TechniquesMgr->m_Model->SetBillboard(m_Parent->m_IsBillboard);

			// R_Texture anim
			_TechniquesMgr->m_Model->SetTextureAnimEnable(p->__textureAnims != -1);
			if (p->__textureAnims != -1)
			{
				_TechniquesMgr->m_Model->SetTextureAnimMatrix(m_Parent->m_TexturesAnims[p->__textureAnims].getValue());
			}
			else
			{
				//continue;
			}

			_Render->r->drawIndexed(PRIM_TRILIST, p->indexStart, p->indexCount, p->vertexStart, p->vertexCount, false);
		}
	}

	_TechniquesMgr->m_Model->Unbind();
}
