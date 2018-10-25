#pragma once

#include "M2_Headers.h"

// COMPONENTS BEGIN
#include "M2_Animator.h"
#include "M2_Comp_Materials.h"
#include "M2_Comp_Miscellaneous.h"
#include "M2_Comp_Skeleton.h"
// COMPONETS END

// Skins
#include "M2_Skin.h"

// FORWARD BEGIN
class CM2_Base_Instance;
class CM2_Builder;
class CM2_Skin_Builder;
// FORWARD END

class M2
{
	friend class CM2_Builder;
	friend class CM2_Skin_Builder;
public:
	M2(cstring name);
	~M2();

	void Render(CM2_Base_Instance* _instance);
	void RenderCollision(cmat4 _worldMatrix);

	void update(double _time, double _dTime);
	void calc(uint16 _animationIndex, cmat4 _worldMatrix, uint32 _time, uint32 globalTime);

public:
	std::string getFilename() const { return m_FileName; }
	std::string getFilenameWithoutExt() const { return m_FileNameWithoutExt; }
	std::string getFilePath() const { return m_FilePath; }
	std::string getUniqueName() const { return m_UniqueName; }
	cbbox getBounds() const { return m_Bounds; }
	
#pragma region Loops & Sequences
public:
	const SM2_Sequence& getSequence(uint32 _index) const
	{
		_ASSERT(_index < m_SequencesLookup.size());
		int16 newIndex = m_SequencesLookup[_index];
		_ASSERT(newIndex < static_cast<int16>(m_Sequences.size()));
		return m_Sequences[newIndex];
	}
	const bool isAnimated() const { return m_IsAnimated; }
public:
	GlobalLoopSeq						m_GlobalLoops;
	vector<SM2_Sequence>				m_Sequences;
	vector<int16>						m_SequencesLookup;
	bool								m_IsAnimated;
#pragma endregion

public:
	CM2_Comp_Materials* getMaterials() const { return m_Materials; }
	CM2_Comp_Materials* m_Materials;

	CM2_Comp_Miscellaneous* getMiscellaneous() const { return m_Miscellaneous; }
	CM2_Comp_Miscellaneous* m_Miscellaneous;

	CM2_Comp_Skeleton* getSkeleton() const { return m_Skeleton; }
	CM2_Comp_Skeleton* m_Skeleton;

public:
	std::string								m_FileName;
	std::string								m_FileNameWithoutExt;
	std::string								m_FilePath;

	std::string								m_UniqueName;
	BoundingBox							m_Bounds;

	// Vertices
	bool								m_IsContainGeom;

	// Skins
	vector<CM2_Skin*>					m_Skins;

private:
	// Buffers and geom
	std::shared_ptr<Mesh>						m_CollisionGeom;
	uint32								m_CollisionIndCnt;
	uint32								m_CollisionVetCnt;

private: // Static and Consts
	const uint8							C_TexturesMaxCount = 128;
	const uint8							C_BonesInfluences = 4;
};


/*struct M2Deleter
{
	void operator()(M2* p)
	{
		GetManager<IM2Manager>()->Delete(p);
	}
};*/
typedef std::shared_ptr<M2> SmartM2Ptr;
