#pragma once

#include "M2_Headers.h"

// Bones
#include "M2_Part_Bone.h"

// Colors and textures
#include "M2_Part_Color.h"
#include "M2_Part_Texture.h"
#include "M2_Part_TextureTransform.h"
#include "M2_Part_TextureWeight.h"

// Misc
#include "M2_Part_Light.h"
#include "M2_Part_Camera.h"

// Particles
#include "Particle.h"
#include "ParticleSystem.h"
#include "RibbonEmitter.h"

#include "MDX_Skin.h"

enum BlendModes
{
	M2BLEND_OPAQUE,
	M2BLEND_ALPHA_KEY,
	M2BLEND_ALPHA,
	M2BLEND_NO_ALPHA_ADD,
	M2BLEND_ADD,
	M2BLEND_MOD,
	M2BLEND_MOD2X
};

// FORWARD BEGIN
class CM2_Builder;
class CM2_Skin_Builder;
// FORWARD END

class MDX : public CRefItem
{
	friend class CM2_Builder;
	friend class CM2_Skin;
	friend class MDX_Skin_Batch;
	friend class CM2_Skin_Builder;
public:
	MDX(cstring name);
	~MDX();

	string GetFileName() const { return m_FileName; }

	bool IsLoaded() { return m_Loaded; }

	void Render();
	void updateEmitters(float dt);

	void drawModel();

	void animate(uint32 _animationIndex, uint32 globalTime);
	void calcBones(uint32 _animationIndex, uint32 time, uint32 globalTime);

	void lightsOn(uint32 lbase);
	void lightsOff(uint32 lbase);

#pragma region Getters
public:
	BoundingBox& GetBounds() { return m_Bounds; }


	SM2_Sequence& GetSequence(uint32 _index) 
	{ 
		assert1(_index < m_Header.sequencesLookup.size);
		uint32 newIndex = m_SequencesLookup[_index];
		assert1(newIndex < m_Header.sequences.size);
		return m_Sequences[newIndex];
	}
	CM2_Part_Bone& GetBone(uint32 _index) 
	{ 
		assert1(_index < m_Header.bonesLookup.size);
		uint32 newIndex = m_BonesLookup[_index];
		assert1(newIndex < m_Header.bones.size);
		return m_Bones[newIndex];
	}
	CM2_Skin* GetSkin(uint32 _index) 
	{ 
		assert1(_index < m_Header.skin_profiles.size);
		return m_Skins[_index]; 
	}
	CM2_Part_Color& GetColor(uint32 _index)
	{
		assert1(_index < m_Header.colors.size);
		return m_Colors[_index];
	}
	CM2_Part_Texture& GetTexture(uint32 _index)
	{
		assert1(_index < m_Header.texturesLookup.size);
		uint32 newIndex = m_TexturesLookup[_index];
		assert1(newIndex < m_Header.textures.size);
		return m_Textures[newIndex];
	}
	CM2_Part_TextureWeight& GetTextureWeight(uint32 _index)
	{
		assert1(_index < m_Header.textureWeightsLookup.size);
		uint32 newIndex = m_TextureWeightsLookup[_index];
		assert1(newIndex < m_Header.textureWeights.size);
		return m_TextureWeights[newIndex];
	}
	CM2_Part_TextureTransform& GetTextureTransform(uint32 _index)
	{
		assert1(_index < m_Header.textureTransformsLookup.size);
		uint32 newIndex = m_TexturesTransformLookup[_index];
		assert1(newIndex < m_Header.textureTransforms.size);
		return m_TexturesTransform[newIndex];
	}
#pragma endregion


#pragma region Header
public:
	bool								m_Loaded;
	string								m_FileName;
	SM2_Header							m_Header;
	string								m_UniqueName;

	// Loops & Sequences
	vector<SM2_Loop>					m_GlobalLoops;
	vector<SM2_Sequence>				m_Sequences;
	vector<uint16>						m_SequencesLookup;
	bool								m_IsAnimated;

	// Bones
	vector<CM2_Part_Bone>				m_Bones;
	vector<int16>						m_BonesLookup;
	bool								m_HasBones;
	bool								m_AnimBones;
	bool								m_IsBillboard;

	// Vertices
	bool								m_ContainGeom;
	// Skins
	vector<CM2_Skin*>					m_Skins;

	// Colors and textures
	vector<CM2_Part_Color>				m_Colors;
	vector<SM2_Material>				m_Materials;
	vector<CM2_Part_Texture>			m_Textures;
	vector<uint16>						m_TexturesLookup;
	vector<CM2_Part_TextureWeight>		m_TextureWeights;
	vector<uint16>						m_TextureWeightsLookup;
	vector<CM2_Part_TextureTransform>	m_TexturesTransform;
	vector<uint16>						m_TexturesTransformLookup;
	bool								m_AnimTextures;

	// Attachments, events, lights and cameras
	vector<CM2_Part_Light>				m_Lights;
	vector<CM2_Part_Camera>				m_Cameras;

	// Particles
#ifdef MDX_PARTICLES_ENABLE
	RibbonEmitter*				ribbons;
	ParticleSystem*				particleSystems;
#endif
	bool						m_HasMisc;
#pragma endregion


#pragma region Header
private:
	// Buffers and geom
	SmartBufferPtr				m_VBuffer;
	BoundingBox					m_Bounds;
#pragma endregion


public:
	bool animcalc;
	int m_AnimationIndex;
	int	m_AnimationTime;

private: // Static and Consts
	const uint8 C_TexturesMaxCount = 128;
	const uint8 C_BonesInfluences = 4;
};


struct MDXDeleter
{
	void operator()(MDX* p)
	{
		GetManager<IMDXManager>()->Delete(p);
	}
};
typedef SmartPtr<MDX, MDXDeleter> SmartMDXPtr;
