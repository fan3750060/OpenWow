#include "stdafx.h"

// General
#include "M2_Builder.h"

// Additional
#include "M2_Skin_Builder.h"

CM2_Builder::CM2_Builder(M2* _model) :
	m_M2(_model),
	m_F(nullptr),
	m_GlobalLoops(nullptr),
	m_M2Bones(nullptr),
	m_Skins(nullptr),
	m_Textures(nullptr),
	m_TexturesWeight(nullptr),
	m_TexturesTransform(nullptr)
{
	// Fix filename
	if (m_M2->m_FileName.back() != '2')
	{
		m_M2->m_FileName[m_M2->m_FileName.length() - 2] = '2';
		m_M2->m_FileName[m_M2->m_FileName.length() - 1] = '\0';
		m_M2->m_FileName.resize(m_M2->m_FileName.length() - 1);
	}

	// Openfile
	m_F = GetManager<IFilesManager>()->Open(m_M2->getFilename());
	if (m_F == nullptr)
	{
		Log::Error("CM2_Builder[%s]: Unable to open file.", m_M2->getFilename().c_str());
		return;
	}

	m_M2->m_FilePath = m_F->Path();
	m_M2->m_FileNameWithoutExt = m_M2->m_FileName.substr(0, m_M2->m_FileName.length() - 3);
}

CM2_Builder::~CM2_Builder()
{
	if (m_F != nullptr)	delete m_F;
}

bool CM2_Builder::Load()
{
	if (m_F == nullptr)
	{
		return false;
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
	//m_ParentM2->m_IsAnimated = false;

	return true;
}

#pragma region Loader

void CM2_Builder::Step1Header()
{
	// Prepare
	memcpy(&m_Header, m_F->getData(), sizeof(SM2_Header));

	// 1 Magic is 'MD20'
	//assert1(m_Header.magic == "MD20");

	// 2 Version is ???
#if (VERSION == VERSION_Vanila)
	assert1(m_Header.version == 256);
#elif (VERSION == VERSION_WotLK)
	assert1(m_Header.version == 264)
#endif


		// 3 Unique name
		if (m_Header.name.size > 0)
		{
			m_M2->m_UniqueName = "";
			for (uint32 i = 0; i < m_Header.name.size; i++)
			{
				m_M2->m_UniqueName += ((char*)(m_F->getData() + m_Header.name.offset))[i];
			}
		}

	// Bounds
	m_M2->m_Bounds.set(m_Header.bounding_box.min, m_Header.bounding_box.max, true);
}

void CM2_Builder::Step2GlobalLoops()
{
	if (m_Header.global_loops.size > 0)
	{
		SM2_Loop* GlobalLoops = (SM2_Loop*)(m_F->getData() + m_Header.global_loops.offset);
		for (uint32 i = 0; i < m_Header.global_loops.size; i++)
		{
			m_M2->m_GlobalLoops.push_back(GlobalLoops[i]);
		}

		m_GlobalLoops = &(m_M2->m_GlobalLoops);
	}

	// Sequences
	if (m_Header.sequences.size > 0)
	{
		SM2_Sequence* Sequences = (SM2_Sequence*)(m_F->getData() + m_Header.sequences.offset);
		for (uint32 i = 0; i < m_Header.sequences.size; i++)
		{
			m_M2->m_Sequences.push_back(Sequences[i]);

			char buf[256];
			sprintf_s(buf, "%s%04d-%02d.anim", m_M2->m_FileNameWithoutExt.c_str(), Sequences[i].__animID, Sequences[i].variationIndex);

			if (Sequences[i].flags.DataInM2)
			{
				animfiles.push_back(nullptr);
			}
			else
			{
				animfiles.push_back(GetManager<IFilesManager>()->Open(buf));
			}
		}
	}

	// Sequences Lookup
	if (m_Header.sequencesLookup.size > 0)
	{
		int16* SequencesLookup = (int16*)(m_F->getData() + m_Header.sequencesLookup.offset);
		for (uint32 i = 0; i < m_Header.sequencesLookup.size; i++)
		{
			m_M2->m_SequencesLookup.push_back(SequencesLookup[i]);
		}
	}
}

void CM2_Builder::Step3Bones()
{
#ifdef M2BUILDER_LOADBONES
	// Bones
	if (m_Header.bones.size > 0)
	{
		m_M2Bones = (SM2_Bone*)(m_F->getData() + m_Header.bones.offset);
		for (uint32 i = 0; i < m_Header.bones.size; i++)
		{
			CM2_Part_Bone* bone = new CM2_Part_Bone(m_F, m_M2Bones[i], m_GlobalLoops, &animfiles);
			m_M2->m_Bones.push_back(bone);
		}

		for (uint32 i = 0; i < m_Header.bones.size; i++)
		{
			m_M2->m_Bones[i]->setParentBone(m_M2);
		}

		m_M2->m_HasBones = true;
	}

	// Bones Lookup
	if (m_Header.bonesLookup.size > 0)
	{
		int16* BonesLookup = (int16*)(m_F->getData() + m_Header.bonesLookup.offset);
		for (uint32 i = 0; i < m_Header.bonesLookup.size; i++)
		{
			m_M2->m_BonesLookup.push_back(BonesLookup[i]);
		}
	}

	// Game Bones Lookup
	if (m_Header.gameBonesLookup.size > 0)
	{
		int16* GameBonesLookup = (int16*)(m_F->getData() + m_Header.gameBonesLookup.offset);
		for (uint32 i = 0; i < m_Header.gameBonesLookup.size; i++)
		{
			m_M2->m_GameBonesLookup.push_back(GameBonesLookup[i]);
		}

		assert1(m_Header.gameBonesLookup.size <= GameBoneType::Count);
	}

	ERASE_VECTOR(animfiles);

#else
	m_ParentM2->m_HasBones = false;
#endif
}

void CM2_Builder::Step4Vertices()
{
	if (m_Header.vertices.size > 0)
	{
		SM2_Vertex* Vertexes = (SM2_Vertex*)(m_F->getData() + m_Header.vertices.offset);
		for (uint32 i = 0; i < m_Header.vertices.size; i++)
		{
			m_Vertexes.push_back(Vertexes[i]);
		}

		// Convert
		for (uint32 i = 0; i < m_Header.vertices.size; i++)
		{
			m_Vertexes[i].pos = m_Vertexes[i].pos.toXZmY();
			m_Vertexes[i].normal = m_Vertexes[i].normal.toXZmY();
		}

		m_M2->m_IsContainGeom = true;
	}
}

void CM2_Builder::Step5ColorAndTextures()
{
	// 1 Colors
	if (m_Header.colors.size > 0)
	{
		SM2_Color* m_Colors = (SM2_Color*)(m_F->getData() + m_Header.colors.offset);
		for (uint32 i = 0; i < m_Header.colors.size; i++)
		{
			CM2_Part_Color* color = new CM2_Part_Color(m_F, m_Colors[i], m_GlobalLoops);
			m_M2->m_Colors.push_back(color);

			// Animated
			if (m_Colors[i].color.interpolation_type || m_Colors[i].alpha.interpolation_type)
			{
				m_M2->m_HasMisc = true;
			}
		}
	}

	// 2 Materials
	if (m_Header.materials.size > 0)
	{
		SM2_Material* m_Materials = (SM2_Material*)(m_F->getData() + m_Header.materials.offset);
		for (uint32 i = 0; i < m_Header.materials.size; i++)
		{
			CM2_Part_Material* material = new CM2_Part_Material(m_Materials[i]);
			m_M2->m_Materials.push_back(material);
		}
	}

	// 3.1 Textures
	if (m_Header.textures.size > 0)
	{
		assert1(m_Header.textures.size <= m_M2->C_TexturesMaxCount);

		m_Textures = (SM2_Texture*)(m_F->getData() + m_Header.textures.offset);
		for (uint32 i = 0; i < m_Header.textures.size; i++)
		{
			CM2_Part_Texture* texture = new CM2_Part_Texture(m_F, m_Textures[i]);
			m_M2->m_Textures.push_back(texture);
		}
	}

	// 3.2 Textures lookup
	if (m_Header.textureLookup.size > 0)
	{
		int16* TexturesLookup = (int16*)(m_F->getData() + m_Header.textureLookup.offset);
		for (uint32 i = 0; i < m_Header.textureLookup.size; i++)
		{
			m_M2->m_TexturesLookup.push_back(TexturesLookup[i]);
		}
	}

	// 3.3 Textures unit lookup
	if (m_Header.textureUnitLookup.size > 0)
	{
		int16* TexturesUnitLookup = (int16*)(m_F->getData() + m_Header.textureUnitLookup.offset);
		for (uint32 i = 0; i < m_Header.textureUnitLookup.size; i++)
		{
			m_M2->m_TexturesUnitLookup.push_back(TexturesUnitLookup[i]);
		}
	}

	// 3.4 Replaceble textures lookup
	if (m_Header.replacable_texture_lookup.size > 0)
	{
		int16* ReplacebleLookup = (int16*)(m_F->getData() + m_Header.replacable_texture_lookup.offset);
		for (uint32 i = 0; i < m_Header.replacable_texture_lookup.size; i++)
		{
			m_M2->m_ReplacebleLookup.push_back(ReplacebleLookup[i]);
		}
	}

	// 3.4 Replaceble textures lookup
	if (m_Header.global_flags.flag_use_texture_combiner_combos && m_Header.textureCombinerCombos.size > 0)
	{
		int16* TexturesCombos = (int16*)(m_F->getData() + m_Header.textureCombinerCombos.offset);
		for (uint32 i = 0; i < m_Header.textureCombinerCombos.size; i++)
		{
			m_M2->m_TexturesCombos.push_back(TexturesCombos[i]);
		}
	}

	//assert1(m_Header.textures.size == m_Header.texturesLookup.size);

	// 4.1 Textures weights
	if (m_Header.textureWeights.size > 0)
	{
		m_TexturesWeight = (SM2_TextureWeight*)(m_F->getData() + m_Header.textureWeights.offset);
		for (uint32 i = 0; i < m_Header.textureWeights.size; i++)
		{
			CM2_Part_TextureWeight* textureWeight = new CM2_Part_TextureWeight(m_F, m_TexturesWeight[i], m_GlobalLoops);
			m_M2->m_TextureWeights.push_back(textureWeight);

			// Animated
			if (m_TexturesWeight[i].weight.interpolation_type)
			{
				m_M2->m_HasMisc = true;
			}
		}
	}

	// 4.2 Textures weights lookup
	if (m_Header.textureWeightsLookup.size > 0)
	{
		int16* TextureWeightsLookup = (int16*)(m_F->getData() + m_Header.textureWeightsLookup.offset);
		for (uint32 i = 0; i < m_Header.textureWeightsLookup.size; i++)
		{
			m_M2->m_TextureWeightsLookup.push_back(TextureWeightsLookup[i]);
		}
	}

	//assert1(m_Header.textureWeights.size == m_Header.textureWeightsLookup.size);

	// 5.1 Textures transform
	if (m_Header.textureTransforms.size > 0)
	{
		m_TexturesTransform = (SM2_TextureTransform*)(m_F->getData() + m_Header.textureTransforms.offset);
		for (uint32 i = 0; i < m_Header.textureTransforms.size; i++)
		{
			CM2_Part_TextureTransform* textureTransform = new CM2_Part_TextureTransform(m_F, m_TexturesTransform[i], m_GlobalLoops);
			m_M2->m_TexturesTransform.push_back(textureTransform);

			// AnimTextures
			if (m_TexturesTransform[i].translation.interpolation_type ||
				m_TexturesTransform[i].rotation.interpolation_type ||
				m_TexturesTransform[i].scaling.interpolation_type)
			{
				m_M2->m_IsAnimTextures = true;
			}
		}
	}

	// 5.2 Textures transform lookup
	if (m_Header.textureTransformsLookup.size > 0)
	{
		int16* TextureTransformsLookup = (int16*)(m_F->getData() + m_Header.textureTransformsLookup.offset);
		for (uint32 i = 0; i < m_Header.textureTransformsLookup.size; i++)
		{
			m_M2->m_TexturesTransformLookup.push_back(TextureTransformsLookup[i]);
		}
	}

	//assert1(m_Header.textureTransforms.size == m_Header.textureTransformsLookup.size);
}

void CM2_Builder::Step6Misc()
{
	// Attachments
	if (m_Header.attachments.size > 0)
	{
		SM2_Attachment* Attachments = (SM2_Attachment*)(m_F->getData() + m_Header.attachments.offset);
		for (uint32 i = 0; i < m_Header.attachments.size; i++)
		{
			CM2_Part_Attachment* attachment = new CM2_Part_Attachment(m_M2, m_F, Attachments[i], m_GlobalLoops);
			m_M2->m_Attachments.push_back(attachment);
		}

		// Animated
		m_M2->m_HasMisc = true;
	}

	// Attachments Lookup
	if (m_Header.attachmentLookup.size > 0)
	{
		int16* AttachmentsLookup = (int16*)(m_F->getData() + m_Header.attachmentLookup.offset);
		for (uint32 i = 0; i < m_Header.attachmentLookup.size; i++)
		{
			m_M2->m_AttachmentsLookup.push_back(AttachmentsLookup[i]);
		}
	}

	// Events
	if (m_Header.events.size > 0)
	{
		SM2_Event* Events = (SM2_Event*)(m_F->getData() + m_Header.events.offset);
		for (uint32 i = 0; i < m_Header.events.size; i++)
		{
			CM2_Part_Event* event = new CM2_Part_Event(m_M2, m_F, Events[i], m_GlobalLoops);
			m_M2->m_Events.push_back(event);
		}

		// Animated
		m_M2->m_HasMisc = true;
	}

	// Lights
	if (m_Header.lights.size > 0)
	{
		SM2_Light* Lights = (SM2_Light*)(m_F->getData() + m_Header.lights.offset);
		for (uint32 i = 0; i < m_Header.lights.size; i++)
		{
			CM2_Part_Light* light = new CM2_Part_Light(m_M2, m_F, Lights[i], m_GlobalLoops);
			m_M2->m_Lights.push_back(light);
		}

		// Animated
		m_M2->m_HasMisc = true;
	}

	// Cameras
	if (m_Header.cameras.size > 0)
	{
		SM2_Camera* Cameras = (SM2_Camera*)(m_F->getData() + m_Header.cameras.offset);
		for (uint32 i = 0; i < m_Header.cameras.size; i++)
		{
			CM2_Part_Camera* camera = new CM2_Part_Camera(m_F, Cameras[i], m_GlobalLoops);
			m_M2->m_Cameras.push_back(camera);
		}

		// Animated
		m_M2->m_HasMisc = true;
	}

	// Cameras Lookup
	if (m_Header.camerasLookup.size > 0)
	{
		int16* CamerasLookup = (int16*)(m_F->getData() + m_Header.camerasLookup.offset);
		for (uint32 i = 0; i < m_Header.camerasLookup.size; i++)
		{
			m_M2->m_CamerasLookup.push_back(CamerasLookup[i]);
		}
	}
}

void CM2_Builder::Step7Particles()
{
	// Ribbons
	if (m_Header.ribbon_emitters.size > 0)
	{
		SM2_RibbonEmitter* Ribbons = (SM2_RibbonEmitter*)(m_F->getData() + m_Header.ribbon_emitters.offset);
		for (uint32 i = 0; i < m_Header.ribbon_emitters.size; i++)
		{
			CM2_RibbonEmitters* ribbon = new CM2_RibbonEmitters(m_M2, m_F, Ribbons[i], m_GlobalLoops);
			m_M2->m_RibbonEmitters.push_back(ribbon);
		}

		// Animated
		m_M2->m_HasMisc = true;
	}

#ifdef MDX_PARTICLES_ENABLE
	// Particle systems
	if (m_Header.particle_emitters.size > 0)
	{
		M2Particle* pdefs = (M2Particle*)(m_F->getData() + m_Header.particle_emitters.offset);
		m_ParentM2->particleSystems = new ParticleSystem[m_Header.particle_emitters.size];
		for (uint32 i = 0; i < m_Header.particle_emitters.size; i++)
		{
			m_ParentM2->particleSystems[i].m_ParentM2 = m_ParentM2;
			m_ParentM2->particleSystems[i].init(m_F, pdefs[i], m_GlobalLoops->data());
		}

		// Animated
		m_ParentM2->m_HasMisc = true;
	}
#endif
}

void CM2_Builder::Step8Skins()
{
	if (!(m_M2->m_IsContainGeom))
	{
		Log::Warn("M2[%s] don't contain geometry. Skins [%d]", m_M2->getFilename().c_str(), m_Header.num_skin_profiles);
		return;
	}

#if (VERSION == VERSION_Vanila)
	assert1(m_Header.skin_profiles.size > 0);
	if (m_Header.skin_profiles.size > 0)
	{
		m_Skins = (SM2_SkinProfile*)(m_F->getData() + m_Header.skin_profiles.offset);
		for (uint32 i = 0; i < m_Header.skin_profiles.size; i++)
		{
			CM2_Skin* skin = new CM2_Skin(m_Skins[i]);
			CM2_Skin_Builder builder(m_ParentM2, skin, m_F);
			builder.Load();
			m_ParentM2->m_Skins.push_back(skin);
		}
	}
#elif (VERSION == VERSION_WotLK)
	assert1(m_Header.num_skin_profiles > 0);
	//for (uint32 i = 0; i < m_Header.num_skin_profiles; i++)
	{
		char buf[256];
		sprintf_s(buf, "%s%02d.skin", m_M2->m_FileNameWithoutExt.c_str(), m_Header.num_skin_profiles - 1);
		SharedPtr<IFile> skinFile = GetManager<IFilesManager>()->Open(buf);
		assert1(skinFile != nullptr);

		CM2_Skin* skin = new CM2_Skin(m_M2);

		CM2_Skin_Builder builder(this, m_M2, skin, skinFile);
		builder.Load();

		m_M2->m_Skins.push_back(skin);
	}
#endif
}

void CM2_Builder::Step9Collision()
{
	R_Buffer* collisonVB = nullptr;
	R_Buffer* collisonIB = nullptr;

	if (m_Header.collisionVertices.size > 0)
	{
		vector<vec3> collisionVertices;
		vec3* CollisionVertices = (vec3*)(m_F->getData() + m_Header.collisionVertices.offset);
		for (uint32 i = 0; i < m_Header.collisionVertices.size; i++)
		{
			collisionVertices.push_back(CollisionVertices[i]);
		}

		for (uint32 i = 0; i < m_Header.collisionVertices.size; i++)
		{
			collisionVertices[i] = collisionVertices[i].toXZmY();
		}

		collisonVB = _Render->r.createVertexBuffer(static_cast<uint32>(collisionVertices.size()) * sizeof(vec3), collisionVertices.data(), false);
	}

	if (m_Header.collisionTriangles.size > 0)
	{
		vector<uint16> collisionTriangles;
		uint16* CollisionTriangles = (uint16*)(m_F->getData() + m_Header.collisionTriangles.offset);
		for (uint32 i = 0; i < m_Header.collisionTriangles.size; i++)
		{
			collisionTriangles.push_back(CollisionTriangles[i]);
		}

		collisonIB = _Render->r.createIndexBuffer(collisionTriangles.size() * sizeof(uint16), collisionTriangles.data(), false);
	}

	if (collisonVB != nullptr && collisonIB != nullptr)
	{
		m_M2->m_CollisionVetCnt = m_Header.collisionVertices.size;
		m_M2->m_CollisionIndCnt = m_Header.collisionTriangles.size;
		m_M2->m_CollisionGeom = _Render->r.beginCreatingGeometry(PRIM_TRILIST, _Render->getRenderStorage()->__layout_GxVBF_P);
		m_M2->m_CollisionGeom->setGeomVertexParams(collisonVB, R_DataType::T_FLOAT, 0, sizeof(vec3)); // pos 0-2
		m_M2->m_CollisionGeom->setGeomIndexParams(collisonIB, R_IndexFormat::IDXFMT_16);
		m_M2->m_CollisionGeom->finishCreatingGeometry();
	}
	else
	{
		m_M2->m_CollisionGeom = nullptr;
	}
}

#pragma endregion

void CM2_Builder::SetAnimated()
{
#ifdef M2BUILDER_LOADBONES
	for (uint32 i = 0; i < m_Header.vertices.size && !(m_M2->m_IsAnimBones); i++)
	{
		for (uint32 b = 0; b < m_M2->C_BonesInfluences; b++)
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
						m_M2->m_IsBillboard = true;
					}

					m_M2->m_IsAnimBones = true;

					break;
				}
			}
		}
	}
#endif

	m_M2->m_IsAnimated = m_M2->m_IsAnimBones || m_M2->m_IsBillboard || m_M2->m_IsAnimTextures || m_M2->m_HasMisc;
}
