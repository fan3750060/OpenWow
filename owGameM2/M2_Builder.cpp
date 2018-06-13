#include "stdafx.h"

// General
#include "M2_Builder.h"

// Additional
#include "M2_Skin_Builder.h"

CM2_Builder::CM2_Builder(M2* _model) :
	m_MDX(_model),
	m_F(nullptr),
	m_Header(nullptr),
	m_GlobalLoops(nullptr),
	m_M2Bones(nullptr),
	m_Skins(nullptr),
	m_Textures(nullptr),
	m_TexturesWeight(nullptr),
	m_TexturesTransform(nullptr)
{
	m_F = GetManager<IFilesManager>()->Open(m_MDX->getFilename());
	if (m_F == nullptr)
	{
		Log::Info("CM2_Builder[%s]: Unable to open file.", m_MDX->getFilename().c_str());
	}
}

CM2_Builder::~CM2_Builder()
{
	if (m_F != nullptr)	delete m_F;
}

void CM2_Builder::Load()
{
	if (m_F == nullptr)
	{
		return;
	}

	Step1Header();
	Step2GlobalLoops();
	Step3Bones();
	Step4Vertices();
	Step5ColorAndTextures();
	Step6Misc();
	Step7Particles();

	Step8Skins();
	Step9Collision();

	SetAnimated();
}

#pragma region Loader

void CM2_Builder::Step1Header()
{
	// Prepare
	memcpy(&m_MDX->m_Header, m_F->GetData(), sizeof(SM2_Header));
	m_Header = &m_MDX->m_Header;

	// 1 Magic is 'MD20'
	//assert1(m_Header->magic == "MD20");

	// 2 Version is ???
	assert1(m_Header->version == 256);

	// 3 Unique name
	if (m_Header->name.size > 0)
	{
		m_MDX->m_UniqueName = "";
		for (uint32 i = 0; i < m_Header->name.size; i++)
		{
			m_MDX->m_UniqueName += ((char*)(m_F->GetData() + m_Header->name.offset))[i];
		}
	}

	// Bounds
	m_MDX->m_Bounds.set(m_Header->bounding_box.min, m_Header->bounding_box.max, true);
}

void CM2_Builder::Step2GlobalLoops()
{
	if (m_Header->global_loops.size > 0)
	{
		SM2_Loop* GlobalLoops = (SM2_Loop*)(m_F->GetData() + m_Header->global_loops.offset);
		for (uint32 i = 0; i < m_Header->global_loops.size; i++)
		{
			m_MDX->m_GlobalLoops.push_back(GlobalLoops[i]);
		}

		m_GlobalLoops = &(m_MDX->m_GlobalLoops);
	}

	// Sequences
	if (m_Header->sequences.size > 0)
	{
		SM2_Sequence* Sequences = (SM2_Sequence*)(m_F->GetData() + m_Header->sequences.offset);
		for (uint32 i = 0; i < m_Header->sequences.size; i++)
		{
			m_MDX->m_Sequences.push_back(Sequences[i]);
		}
	}

	// Sequences Lookup
	if (m_Header->sequencesLookup.size > 0)
	{
		int16* SequencesLookup = (int16*)(m_F->GetData() + m_Header->sequencesLookup.offset);
		for (uint32 i = 0; i < m_Header->sequencesLookup.size; i++)
		{
			m_MDX->m_SequencesLookup.push_back(SequencesLookup[i]);
		}
	}
}

void CM2_Builder::Step3Bones()
{
	// Bones
	if (m_Header->bones.size > 0)
	{
		m_M2Bones = (SM2_Bone*)(m_F->GetData() + m_Header->bones.offset);
		for (uint32 i = 0; i < m_Header->bones.size; i++)
		{
			CM2_Part_Bone bone(m_F, m_M2Bones[i], m_GlobalLoops);
			m_MDX->m_Bones.push_back(bone);
		}

		m_MDX->m_HasBones = true;
	}

	// Bones Lookup
	if (m_Header->bone_lookup_table.size > 0)
	{
		int16* BonesLookup = (int16*)(m_F->GetData() + m_Header->bone_lookup_table.offset);
		for (uint32 i = 0; i < m_Header->bone_lookup_table.size; i++)
		{
			m_MDX->m_BonesLookup.push_back(BonesLookup[i]);
		}
	}

	//assert1(m_Header->bones.size == m_Header->bone_lookup_table.size);
}

void CM2_Builder::Step4Vertices()
{
	if (m_Header->vertices.size > 0)
	{
		SM2_Vertex* Vertexes = (SM2_Vertex*)(m_F->GetData() + m_Header->vertices.offset);
		for (uint32 i = 0; i < m_Header->vertices.size; i++)
		{
			m_Vertexes.push_back(Vertexes[i]);
		}

		// Convert
		for (uint32 i = 0; i < m_Header->vertices.size; i++)
		{
			m_Vertexes[i].pos = m_Vertexes[i].pos.toXZmY();
			m_Vertexes[i].normal = m_Vertexes[i].normal.toXZmY();
		}

		m_MDX->m_VBuffer = _Render->r.createVertexBuffer(m_Header->vertices.size * 12 * sizeof(float), m_Vertexes.data(), false);
		m_MDX->m_IsContainGeom = true;
	}
}

void CM2_Builder::Step5ColorAndTextures()
{
	// 1 Colors
	if (m_Header->colors.size > 0)
	{
		SM2_Color* m_Colors = (SM2_Color*)(m_F->GetData() + m_Header->colors.offset);
		for (uint32 i = 0; i < m_Header->colors.size; i++)
		{
			CM2_Part_Color color(m_F, m_Colors[i], m_GlobalLoops);
			m_MDX->m_Colors.push_back(color);

			// Animated
			if (m_Colors[i].color.interpolation_type || m_Colors[i].alpha.interpolation_type)
			{
				m_MDX->m_HasMisc = true;
			}
		}
	}

	// 2 Materials
	if (m_Header->materials.size > 0)
	{
		SM2_Material* m_Materials = (SM2_Material*)(m_F->GetData() + m_Header->materials.offset);
		for (uint32 i = 0; i < m_Header->materials.size; i++)
		{
			m_MDX->m_Materials.push_back(m_Materials[i]);
		}
	}

	// 3.1 Textures
	if (m_Header->textures.size > 0)
	{
		assert1(m_Header->textures.size <= m_MDX->C_TexturesMaxCount);

		m_Textures = (SM2_Texture*)(m_F->GetData() + m_Header->textures.offset);	
		for (uint32 i = 0; i < m_Header->textures.size; i++)
		{
			CM2_Part_Texture texture(m_F, m_Textures[i]);
			m_MDX->m_Textures.push_back(texture);
		}
	}

	// 3.2 Textures lookup
	if (m_Header->texturesLookup.size > 0)
	{
		uint16* TexturesLookup = (uint16*)(m_F->GetData() + m_Header->texturesLookup.offset);
		for (uint32 i = 0; i < m_Header->texturesLookup.size; i++)
		{
			m_MDX->m_TexturesLookup.push_back(TexturesLookup[i]);
		}
	}

	//assert1(m_Header->textures.size == m_Header->texturesLookup.size);

	// 4.1 Textures weights
	if (m_Header->textureWeights.size > 0)
	{
		m_TexturesWeight = (SM2_TextureWeight*)(m_F->GetData() + m_Header->textureWeights.offset);
		for (uint32 i = 0; i < m_Header->textureWeights.size; i++)
		{
			CM2_Part_TextureWeight textureWeight(m_F, m_TexturesWeight[i], m_GlobalLoops);
			m_MDX->m_TextureWeights.push_back(textureWeight);

			// Animated
			if (m_TexturesWeight[i].weight.interpolation_type)
			{
				m_MDX->m_HasMisc = true;
			}
		}
	}

	// 4.2 Textures weights lookup
	if (m_Header->textureWeightsLookup.size > 0)
	{
		uint16* TextureWeightsLookup = (uint16*)(m_F->GetData() + m_Header->textureWeightsLookup.offset);
		for (uint32 i = 0; i < m_Header->textureWeightsLookup.size; i++)
		{
			m_MDX->m_TextureWeightsLookup.push_back(TextureWeightsLookup[i]);
		}
	}

	//assert1(m_Header->textureWeights.size == m_Header->textureWeightsLookup.size);

	// 5.1 Textures transform lookup
	if (m_Header->textureTransforms.size > 0)
	{
		m_TexturesTransform = (SM2_TextureTransform*)(m_F->GetData() + m_Header->textureTransforms.offset);
		for (uint32 i = 0; i < m_Header->textureTransforms.size; i++)
		{
			CM2_Part_TextureTransform textureTransform(m_F, m_TexturesTransform[i], m_GlobalLoops);
			m_MDX->m_TexturesTransform.push_back(textureTransform);
			
			// AnimTextures
			if (m_TexturesTransform[i].translation.interpolation_type || 
				m_TexturesTransform[i].rotation.interpolation_type || 
				m_TexturesTransform[i].scaling.interpolation_type)
			{
				m_MDX->m_AnimTextures = true;
			}
		}	
	}

	// 5.2 Textures transform lookup
	if (m_Header->textureTransformsLookup.size > 0)
	{
		uint16* TextureTransformsLookup = (uint16*)(m_F->GetData() + m_Header->textureTransformsLookup.offset);
		for (uint32 i = 0; i < m_Header->textureTransformsLookup.size; i++)
		{
			m_MDX->m_TexturesTransformLookup.push_back(TextureTransformsLookup[i]);
		}
	}

	//assert1(m_Header->textureTransforms.size == m_Header->textureTransformsLookup.size);
}

void CM2_Builder::Step6Misc()
{
	// Lights
	if (m_Header->lights.size > 0)
	{
		SM2_Light* m_Lights = (SM2_Light*)(m_F->GetData() + m_Header->lights.offset);
		for (uint32 i = 0; i < m_Header->lights.size; i++)
		{
			CM2_Part_Light light(m_F, m_Lights[i], m_GlobalLoops);
			m_MDX->m_Lights.push_back(light);
		}

		// Animated
		m_MDX->m_HasMisc = true;
	}

	// Cameras
	if (m_Header->cameras.size > 0)
	{
		SM2_Camera* m_Cameras = (SM2_Camera*)(m_F->GetData() + m_Header->cameras.offset);
		for (uint32 i = 0; i < m_Header->cameras.size; i++)
		{
			CM2_Part_Camera* camera = new CM2_Part_Camera(m_F, m_Cameras[i], m_GlobalLoops);
			m_MDX->m_Cameras.push_back(*camera);
		}

		// Animated
		m_MDX->m_HasMisc = true;
	}

	// Cameras Lookup
	if (m_Header->camerasLookup.size > 0)
	{
		uint16* CamerasLookup = (uint16*)(m_F->GetData() + m_Header->camerasLookup.offset);
		for (uint32 i = 0; i < m_Header->camerasLookup.size; i++)
		{
			m_MDX->m_CamerasLookup.push_back(CamerasLookup[i]);
		}
	}
}

void CM2_Builder::Step7Particles()
{
	// Ribbons
	if (m_Header->ribbon_emitters.size > 0)
	{
		SM2_RibbonEmitter* Ribbons = (SM2_RibbonEmitter*)(m_F->GetData() + m_Header->ribbon_emitters.offset);
		for (uint32 i = 0; i < m_Header->ribbon_emitters.size; i++)
		{
			CM2_RibbonEmitters ribbon(m_MDX, m_F, Ribbons[i], m_GlobalLoops);
			m_MDX->m_RibbonEmitters.push_back(ribbon);
		}

		// Animated
		m_MDX->m_HasMisc = true;
	}

#ifdef MDX_PARTICLES_ENABLE
	// Particle systems
	if (m_Header->particle_emitters.size > 0)
	{
		M2Particle* pdefs = (M2Particle*)(m_F->GetData() + m_Header->particle_emitters.offset);
		m_MDX->particleSystems = new ParticleSystem[m_Header->particle_emitters.size];
		for (uint32 i = 0; i < m_Header->particle_emitters.size; i++)
		{
			m_MDX->particleSystems[i].m_MDX = m_MDX;
			m_MDX->particleSystems[i].init(m_F, pdefs[i], m_GlobalLoops->data());
		}

		// Animated
		m_MDX->m_HasMisc = true;
	}
#endif
}

void CM2_Builder::Step8Skins()
{
	if (!(m_MDX->m_IsContainGeom))
	{
		Log::Warn("M2[%s] don't contain geometry", m_MDX->getFilename().c_str());
		return;
	}

	assert1(m_Header->skin_profiles.size > 0);

	/*if (m_Header->skin_profiles.size > 0)
	{
		m_Skins = (SM2_SkinProfile*)(m_F->GetData() + m_Header->skin_profiles.offset);
		for (uint32 i = 0; i < m_Header->skin_profiles.size; i++)
		{
			CM2_Skin* skin = new CM2_Skin(m_Skins[i]);
			CM2_Skin_Builder builder(m_MDX, skin, m_F);
			builder.Load();
			m_MDX->m_Skins.push_back(skin);
		}
	}*/

	CM2_Skin* skin = new CM2_Skin(m_MDX);
	CM2_Skin_Builder builder
	(
		m_MDX, 
		((SM2_SkinProfile*)(m_F->GetData() + m_Header->skin_profiles.offset))[0], 
		skin,
		m_F
	);
	builder.Load();
	m_MDX->m_Skins.push_back(skin);
}

void CM2_Builder::Step9Collision()
{
	R_Buffer* collisonVB = nullptr;
	R_Buffer* collisonIB = nullptr;

	if (m_Header->collisionVertices.size > 0)
	{
		vector<vec3> collisionVertices;
		vec3* CollisionVertices = (vec3*)(m_F->GetData() + m_Header->collisionVertices.offset);
		for (uint32 i = 0; i < m_Header->collisionVertices.size; i++)
		{
			collisionVertices.push_back(CollisionVertices[i]);
		}

		for (uint32 i = 0; i < m_Header->collisionVertices.size; i++)
		{
			collisionVertices[i] = collisionVertices[i].toXZmY();
		}

		collisonVB = _Render->r.createVertexBuffer(collisionVertices.size() * sizeof(vec3), collisionVertices.data(), false);
	}

	if (m_Header->collisionTriangles.size > 0)
	{
		vector<uint16> collisionTriangles;
		uint16* CollisionTriangles = (uint16*)(m_F->GetData() + m_Header->collisionTriangles.offset);
		for (uint32 i = 0; i < m_Header->collisionTriangles.size; i++)
		{
			collisionTriangles.push_back(CollisionTriangles[i]);
		}

		collisonIB = _Render->r.createIndexBuffer(collisionTriangles.size() * sizeof(uint16), collisionTriangles.data(), false);
	}

	if (collisonVB != nullptr && collisonIB != nullptr)
	{
		m_MDX->m_CollisionGeom = _Render->r.beginCreatingGeometry(_Render->Storage()->__layout_GxVBF_P);
		m_MDX->m_CollisionGeom->setGeomVertexParams(collisonVB, R_DataType::T_FLOAT, 0, sizeof(vec3)); // pos 0-2
		m_MDX->m_CollisionGeom->setGeomIndexParams(collisonIB, R_IndexFormat::IDXFMT_16);
		m_MDX->m_CollisionGeom->finishCreatingGeometry();
	}
	else
	{
		m_MDX->m_CollisionGeom = nullptr;
	}
}

#pragma endregion

void CM2_Builder::SetAnimated()
{
	for (uint32 i = 0; i < m_Header->vertices.size && !(m_MDX->m_IsAnimBones); i++)
	{
		for (uint32 b = 0; b < m_MDX->C_BonesInfluences; b++)
		{
			if (m_Vertexes[i].bone_weights[b] > 0)
			{
				SM2_Bone& bone = m_M2Bones[m_Vertexes[i].bone_indices[b]]; // ?? Lookup

				if (bone.translation.interpolation_type || 
					bone.rotation.interpolation_type || 
					bone.scale.interpolation_type || 
					bone.flags.spherical_billboard || 
					bone.flags.cylindrical_billboard_lock_x ||
					bone.flags.cylindrical_billboard_lock_y || 
					bone.flags.cylindrical_billboard_lock_z
					)
				{
					if (bone.flags.spherical_billboard || 
						bone.flags.cylindrical_billboard_lock_x || 
						bone.flags.cylindrical_billboard_lock_y || 
						bone.flags.cylindrical_billboard_lock_z
						)
					{
						m_MDX->m_IsBillboard = true;
					}

					m_MDX->m_IsAnimBones = true;

					break;
				}
			}
		}
	}

	m_MDX->m_IsAnimated = m_MDX->m_IsAnimBones || m_MDX->m_IsBillboard || m_MDX->m_AnimTextures || m_MDX->m_HasMisc;
}
