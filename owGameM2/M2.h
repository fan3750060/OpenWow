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
#include "M2_Part_Attachment.h"
#include "M2_Part_Event.h"
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

	void Render(cmat4 _worldMatrix, CM2_MeshPartID_Provider* _provider, cvec4 _doodadColor, uint16 _animationIndex, uint32 _time, uint32 globalTime);
	void RenderCollision(cmat4 _worldMatrix);
	void drawModel(cmat4 _worldMatrix, CM2_MeshPartID_Provider* _provider, cvec4 _doodadColor, uint16 _animationIndex, uint32 _time, uint32 globalTime);

	void animate(uint16 _animationIndex, cmat4 _worldMatrix, uint32 _time, uint32 globalTime);

public:
	string getFilename() const { return m_FileName; }
	string getFilenameWithoutExt() const { return m_FileNameWithoutExt; }
	string getFilePath() const { return m_FilePath; }
	string getUniqueName() const { return m_UniqueName; }
	cbbox getBounds() const { return m_Bounds; }
	
#pragma region Loops & Sequences
public:
	const SM2_Sequence& getSequence(uint32 _index) const
	{
		assert1(_index < m_SequencesLookup.size());
		int16 newIndex = m_SequencesLookup[_index];
		assert1(newIndex < static_cast<int16>(m_Sequences.size()));
		return m_Sequences[newIndex];
	}
	const bool isAnimated() const { return m_IsAnimated; }
public:
	GlobalLoopSeq						m_GlobalLoops;
	vector<SM2_Sequence>				m_Sequences;
	vector<int16>						m_SequencesLookup;
	bool								m_IsAnimated;
#pragma endregion


#pragma region Bones
public:
	const CM2_Part_Bone* getBone(uint32 _index) const
	{
		if (_index >= m_BonesLookup.size())
		{
			//Log::Warn("M2[%s]: getBone [%d] not found in Lookup[%d]", m_FileName.c_str(), _index, m_Header.bonesLookup.size);
			return nullptr;
		}
		int16 newIndex = m_BonesLookup[_index];
		assert1(newIndex < static_cast<int16>(m_Bones.size()));
		return (m_Bones[newIndex]);
	}
	const CM2_Part_Bone* getGameBone(GameBoneType::List _type) const
	{
		if (_type >= static_cast<int16>(m_GameBonesLookup.size()))
		{
			Log::Warn("M2[%s]: getGameBone [%d] not found in Lookup[%d]", m_FileName.c_str(), _type, m_GameBonesLookup.size());
			return nullptr;
		}
		int16 newIndex = m_GameBonesLookup[_type];
		assert1(newIndex < static_cast<int16>(m_Bones.size()));
		return (m_Bones[newIndex]);
	}
	const bool hasBones() const { return m_HasBones; }
	const bool isAnimBones() const { return m_IsAnimBones; }
	const bool isBillboard() const { return m_IsBillboard; }
public: //TODO
	vector<CM2_Part_Bone*>				m_Bones;
	vector<int16>						m_BonesLookup;
	vector<int16>						m_GameBonesLookup;
	bool								m_HasBones;
	bool								m_IsAnimBones;
	bool								m_IsBillboard;
#pragma endregion


#pragma region Colors and Textures
public:
	const CM2_Part_Color* GetColor(uint32 _index) const
	{
		assert1(_index < m_Colors.size());
		return (m_Colors[_index]);
	}
	const CM2_Part_Material* GetMaterial(uint32 _index) const
	{
		assert1(_index < m_Materials.size());
		return (m_Materials[_index]);
	}
public:
	vector<CM2_Part_Color*>				m_Colors;
	vector<CM2_Part_Material*>			m_Materials;
#pragma endregion


#pragma region Textures
public:
	const CM2_Part_Texture* GetTexture(uint32 _index) const
	{
		if (_index >= m_TexturesLookup.size())
		{
			Log::Warn("M2[%s]: GetTexture [%d] not found in Lookup[%d]", m_FileName.c_str(), _index, m_TexturesLookup.size());
			return nullptr;
		}
		int16 newIndex = m_TexturesLookup[_index];
		assert1(newIndex < static_cast<int16>(m_Textures.size()));
		return (m_Textures[newIndex]);
	}
	const CM2_Part_TextureWeight* GetTextureWeight(uint32 _index) const
	{
		if (_index >= m_TextureWeightsLookup.size())
		{
			Log::Warn("M2[%s]: GetTextureWeight [%d] not found in Lookup[%d]", m_FileName.c_str(), _index, m_TextureWeightsLookup.size());
			return nullptr;
		}
		int16 newIndex = m_TextureWeightsLookup[_index];
		assert1(newIndex < static_cast<int16>(m_TextureWeights.size()));
		return (m_TextureWeights[newIndex]);
	}
	const CM2_Part_TextureTransform* GetTextureTransform(uint32 _index) const
	{
		if (_index >= m_TexturesTransformLookup.size())
		{
			Log::Warn("M2[%s]: GetTextureTransform [%d] not found in Lookup[%d]", m_FileName.c_str(), _index, m_TexturesTransformLookup.size());
			return nullptr;
		}
		int16 newIndex = m_TexturesTransformLookup[_index];
		assert1(newIndex < static_cast<int16>(m_TexturesTransform.size()));
		return (m_TexturesTransform[newIndex]);
	}
public:
	vector<CM2_Part_Texture*>			m_Textures;
	vector<int16_t>						m_TexturesLookup;
	//--
	vector<int16_t>						m_TexturesUnitLookup;
	vector<int16_t>						m_ReplacebleLookup;    // index is TextureType, value is texture number
	vector<int16_t>						m_TexturesCombos;
	//--
	vector<CM2_Part_TextureWeight*>		m_TextureWeights;
	vector<int16_t>						m_TextureWeightsLookup;
	//--
	vector<CM2_Part_TextureTransform*>	m_TexturesTransform;
	vector<int16_t>						m_TexturesTransformLookup;
	bool								m_IsAnimTextures;
#pragma endregion


#pragma region Attachments
	const CM2_Part_Attachment* getAttachment(uint32 _index) const
	{
		if (_index >= m_AttachmentsLookup.size())
		{
			Log::Warn("M2[%s]: getAttachment [%d] not found in Lookup[%d]", m_FileName.c_str(), _index, m_AttachmentsLookup.size());
			return nullptr;
		}
		int16 newIndex = m_AttachmentsLookup[_index];
		assert1(newIndex < static_cast<int16>(m_Attachments.size()));
		return (m_Attachments[newIndex]);
	}

	const CM2_Part_Light* getLight(uint32 _index) const
	{
		assert1(_index < m_Lights.size());
		return (m_Lights[_index]);
	}

	const CM2_Part_Camera* getCamera(uint32 _index) const
	{
		if (_index >= m_CamerasLookup.size())
		{
			Log::Warn("M2[%s]: getCamera [%d] not found in Lookup[%d]", m_FileName.c_str(), _index, m_CamerasLookup.size());
			return nullptr;
		}
		int16 newIndex = m_CamerasLookup[_index];
		assert1(newIndex < static_cast<int16>(m_Cameras.size()));
		return (m_Cameras[newIndex]);
	}

#pragma endregion

	


#pragma region Header
public:
	string								m_FileName;
	string								m_FileNameWithoutExt;
	string								m_FilePath;

	string								m_UniqueName;
	BoundingBox							m_Bounds;

	// Vertices
	bool								m_IsContainGeom;

	// Skins
	vector<CM2_Skin*>					m_Skins;


	//--


	// Attachments, events, lights and cameras
	vector<CM2_Part_Attachment*>		m_Attachments;
	vector<int16_t>						m_AttachmentsLookup;
	vector<CM2_Part_Event*>				m_Events;
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
	SharedGeomPtr						m_CollisionGeom;
	uint32								m_CollisionIndCnt;
	uint32								m_CollisionVetCnt;

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
typedef SharedPtr<M2, M2Deleter> SmartM2Ptr;
