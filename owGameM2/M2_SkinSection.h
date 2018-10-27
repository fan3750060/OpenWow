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
	CM2_SkinSection(const std::weak_ptr<M2> _model, const uint16 _index, const SM2_SkinSection& _proto);
	void CreateGeometry(const std::vector<SM2_Vertex>& _vertexes, const std::vector<uint16>& _indexes);

	void Draw(/*RenderState* _state, */CM2_Base_Instance* _instance);

	const SM2_SkinSection& getProto() const { return m_Proto; }


private:
	const uint16			m_Index;
	const SM2_SkinSection	m_Proto;

	std::shared_ptr<IMesh> __geom;

private: // PARENT
	const std::weak_ptr<M2> m_ParentM2;
	CGroupQuality& m_QualitySettings;
};