#pragma once

#include "M2_CommonTypes.h"
#include "M2_SkinTypes.h"

// FORWARD BEGIN
class M2;
class CM2_Skin_Builder;
// FORWARD END

class CM2_SkinSection
{
	friend class CM2_Skin_Builder;
public:
	CM2_SkinSection(M2* _model, const SM2_SkinSection& _proto);
	void Final();

	void Draw(RenderState* _state, uint16 _animationIndex, cmat4 _worldMatrix, uint32 _time, uint32 globalTime);

	const SM2_SkinSection& getProto() const { return m_Proto; }

private:
	const SM2_SkinSection m_Proto;

	vector<SM2_Vertex> m_Verts;
	vector<uint16> m_Index;

	SharedGeomPtr __geom;

	//--
	M2* m_ParentM2;
	CGroupQuality& m_QualitySettings;
};