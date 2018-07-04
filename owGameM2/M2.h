#pragma once

#include "M2_Headers.h"

// Animator
#include "M2_Animator.h"
#include "M2_MeshPartID_Provider.h"

// Bones
#include "M2_Part_Bone.h"
// Colors and textures
#include "M2_Part_Color.h"
#include "M2_Part_Material.h"
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

// FORWARD BEGIN
class CM2_Builder;
class CM2_Skin_Builder;
// FORWARD END

class M2 : public CRefItem
{
	friend class CM2_Builder;
	friend class CM2_Skin_Builder;
public:
	M2(cstring name);
	~M2();

	void updateEmitters(float dt);

	void Render(cmat4 _worldMatrix, CM2_MeshPartID_Provider* _provider);
	void RenderCollision(cmat4 _worldMatrix);
	void drawModel(cmat4 _worldMatrix, CM2_MeshPartID_Provider* _provider);

	void animate(uint16 _animationIndex, cmat4 _worldMatrix, uint32 _time, uint32 globalTime);

#pragma region Getters
public:
	string getFilename() const { return m_FileName; }
	string getFilenameWithoutExt() const { return m_FileNameWithoutExt; }
	string getFilePath() const { return m_FilePath; }
	string getUniqueName() const { return m_UniqueName; }
	cbbox getBounds() const { return m_Bounds; }
	
	const SM2_Sequence& getSequence(uint32 _index) const
	{
		assert1(_index < m_Header.sequencesLookup.size);
		int16 newIndex = m_SequencesLookup[_index];
		assert1(newIndex < static_cast<int16>(m_Header.sequences.size));
		return m_Sequences[newIndex];
	}
	const bool isAnimated() const { return m_IsAnimated; }
	const CM2_Part_Bone& getBone(uint32 _index) const
	{
		assert1(_index < m_Header.bone_lookup_table.size);
		int16 newIndex = m_BonesLookup[_index];
		assert1(newIndex < static_cast<int16>(m_Header.bones.size));
		return *(m_Bones[newIndex]);
	}
	const bool hasBones() const { return m_HasBones; }
	const bool isAnimBones() const { return m_IsAnimBones; }
	const bool isBillboard() const { return m_IsBillboard; }

	// Colors and Textures

	const CM2_Part_Color& GetColor(uint32 _index) const
	{
		assert1(_index < m_Header.colors.size);
		return *(m_Colors[_index]);
	}
	const CM2_Part_Material& GetMaterial(uint32 _index) const
	{
		assert1(_index < m_Header.materials.size);
		return *(m_Materials[_index]);
	}
	const CM2_Part_Texture& GetTexture(uint32 _index) const
	{
		assert1(_index < m_Header.textureLookup.size);
		int16 newIndex = m_TexturesLookup[_index];
		assert1(newIndex < static_cast<int16>(m_Header.textures.size));
		return *(m_Textures[newIndex]);
	}
	const CM2_Part_TextureWeight& GetTextureWeight(uint32 _index) const
	{
		assert1(_index < m_Header.textureWeightsLookup.size);
		int16 newIndex = m_TextureWeightsLookup[_index];
		assert1(newIndex < static_cast<int16>(m_Header.textureWeights.size));
		return *(m_TextureWeights[newIndex]);
	}
	const CM2_Part_TextureTransform& GetTextureTransform(uint32 _index) const
	{
		assert1(_index < m_Header.textureTransformsLookup.size);
		int16 newIndex = m_TexturesTransformLookup[_index];
		assert1(newIndex < static_cast<int16>(m_Header.textureTransforms.size));
		return *(m_TexturesTransform[newIndex]);
	}

	// Attachments

#pragma endregion


#pragma region Header
public:
	string								m_FileName;
	string								m_FileNameWithoutExt;
	string								m_FilePath;

	SM2_Header							m_Header;
	string								m_UniqueName;
	BoundingBox							m_Bounds;

	// Loops & Sequences
	GlobalLoopSeq						m_GlobalLoops;
	vector<SM2_Sequence>				m_Sequences;
	vector<int16>						m_SequencesLookup;
	bool								m_IsAnimated;

	// Bones
	vector<CM2_Part_Bone*>				m_Bones;
	vector<int16>						m_BonesLookup;
	bool								m_HasBones;
	bool								m_IsAnimBones;
	bool								m_IsBillboard;

	// Vertices
	bool								m_IsContainGeom;

	// Skins
	vector<CM2_Skin*>					m_Skins;

	// Colors and textures
	vector<CM2_Part_Color*>				m_Colors;
	vector<CM2_Part_Material*>			m_Materials;
	vector<CM2_Part_Texture*>			m_Textures;
	vector<int16_t>						m_TexturesLookup;
	vector<int16_t>						m_TexturesUnitLookup;
	vector<int16_t>						m_ReplacebleLookup;    // index is TextureType, value is texture number
	vector<CM2_Part_TextureWeight*>		m_TextureWeights;
	vector<int16_t>						m_TextureWeightsLookup;
	vector<CM2_Part_TextureTransform*>	m_TexturesTransform;
	vector<int16_t>						m_TexturesTransformLookup;
	bool								m_IsAnimTextures;

	// Attachments, events, lights and cameras
	vector<CM2_Part_Light*>				m_Lights;
	vector<CM2_Part_Camera*>			m_Cameras;
	vector<int16_t>						m_CamerasLookup;

	// Particles
	vector<CM2_RibbonEmitters*>			m_RibbonEmitters;
#ifdef MDX_PARTICLES_ENABLE
	ParticleSystem*						particleSystems;
#endif
	bool								m_HasMisc;
#pragma endregion


private:
	// Buffers and geom
	SmartBufferPtr						m_VBuffer;
	SmartGeomPtr						m_CollisionGeom;

private: // Static and Consts
	const uint8							C_TexturesMaxCount = 128;
	const uint8							C_BonesInfluences = 4;
};


struct M2Deleter
{
	void operator()(M2* p)
	{
		GetManager<IM2Manager>()->Delete(p);
	}
};
typedef SmartPtr<M2, M2Deleter> SmartM2Ptr;
