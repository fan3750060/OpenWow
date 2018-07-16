#pragma once

#include "M2_CommonTypes.h"
#include "M2_SkinTypes.h"

// FORWARD BEGIN
class M2;
class CM2_Base_Instance;
class CM2_Skin_Builder;
// FORWARD END

class CM2_SkinSection
{
	friend class CM2_Skin_Builder;
public:
	CM2_SkinSection(M2* _model, const uint16 _index, const SM2_SkinSection& _proto);
	void CreateGeometry(vector<SM2_Vertex>& _vertexes, vector<uint16>& _indexes);

	void Draw(RenderState* _state, CM2_Base_Instance* _instance);

	const SM2_SkinSection& getProto() const { return m_Proto; }


private:
	const uint16			m_Index;
	const SM2_SkinSection	m_Proto;

	SharedGeomPtr __geom;

private: // PARENT
	M2* m_ParentM2;
	CGroupQuality& m_QualitySettings;
};