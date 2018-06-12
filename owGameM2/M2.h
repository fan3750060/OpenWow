#pragma once

#include "M2_Headers.h"

// Animator
#include "M2_Animator.h"

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
#include "M2_RibbonEmitters.h"
// Skins
#include "M2_Skin.h"

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

class M2 : public CRefItem, public IUpdatable
{
	friend class CM2_Builder;
	friend class CM2_Skin_Builder;
public:
	M2(cstring name);
	~M2();

	void Input(double _time, double _dTime) override {};
	void Update(double _time, double _dTime) override;

	void Render();
	void RenderCollision();
	void updateEmitters(float dt);

	void drawModel();

	void animate(uint16 _animationIndex, uint32 _time, uint32 globalTime);

	void lightsOn(uint32 lbase);
	void lightsOff(uint32 lbase);

#pragma region Getters
public:
	bool isLoaded() const 
	{ 
		return m_Loaded; 
	}
	string getFilename() const 
	{ 
		return m_FileName; 
	}
	cbbox getBounds() const 
	{ 
		return m_Bounds; 
	}


	const SM2_Sequence& getSequence(uint32 _index) const
	{ 
		assert1(_index < m_Header.sequencesLookup.size);
		int16 newIndex = m_SequencesLookup[_index];
		assert1(newIndex < m_Header.sequences.size);
		return m_Sequences[newIndex];
	}
	const CM2_Part_Bone& getBone(uint32 _index) const
	{ 
		assert1(_index < m_Header.bone_lookup_table.size);
		int16 newIndex = m_BonesLookup[_index];
		assert1(newIndex < m_Header.bones.size);
		return m_Bones[newIndex];
	}
	const CM2_Skin* GetSkin(uint32 _index) const
	{ 
		assert1(_index < m_Header.skin_profiles.size);
		return m_Skins[_index]; 
	}

	// Colors and Textures

	const CM2_Part_Color& GetColor(uint32 _index) const
	{
		assert1(_index < m_Header.colors.size);
		return m_Colors[_index];
	}
	const SM2_Material& GetMaterial(uint32 _index) const
	{
		assert1(_index < m_Header.materials.size);
		return m_Materials[_index];
	}
	const CM2_Part_Texture& GetTexture(uint32 _index) const
	{
		assert1(_index < m_Header.texturesLookup.size);
		int16 newIndex = m_TexturesLookup[_index];
		assert1(newIndex < m_Header.textures.size);
		return m_Textures[newIndex];
	}
	const CM2_Part_TextureWeight& GetTextureWeight(uint32 _index) const
	{
		assert1(_index < m_Header.textureWeightsLookup.size);
		int16 newIndex = m_TextureWeightsLookup[_index];
		assert1(newIndex < m_Header.textureWeights.size);
		return m_TextureWeights[newIndex];
	}
	const CM2_Part_TextureTransform& GetTextureTransform(uint32 _index) const
	{
		assert1(_index < m_Header.textureTransformsLookup.size);
		int16 newIndex = m_TexturesTransformLookup[_index];
		assert1(newIndex < m_Header.textureTransforms.size);
		return m_TexturesTransform[newIndex];
	}

	// Attachments

#pragma endregion


#pragma region Header
public:
	bool								m_Loaded;
	string								m_FileName;
	SM2_Header							m_Header;
	string								m_UniqueName;
	BoundingBox							m_Bounds;

	// Loops & Sequences
	GlobalLoopSeq						m_GlobalLoops;
	vector<SM2_Sequence>				m_Sequences;
	vector<int16>						m_SequencesLookup;
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
	vector<uint16>						m_CamerasLookup;

	// Particles
	vector<CM2_RibbonEmitters>			m_RibbonEmitters;
#ifdef MDX_PARTICLES_ENABLE
	ParticleSystem*						particleSystems;
#endif
	bool								m_HasMisc;
#pragma endregion


private:
	// Buffers and geom
	SmartBufferPtr						m_VBuffer;
	SmartGeomPtr						m_CollisionGeom;
public:
	bool animcalc;
	CM2_Animator* m_Animator;

private: // Static and Consts
	const uint8 C_TexturesMaxCount = 128;
	const uint8 C_BonesInfluences = 4;
};


struct M2Deleter
{
	void operator()(M2* p)
	{
		GetManager<IM2Manager>()->Delete(p);
	}
};
typedef SmartPtr<M2, M2Deleter> SmartM2Ptr;
