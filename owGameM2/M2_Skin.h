#pragma once

#include "M2_SkinTypes.h"

#include "M2_Skin_Batch.h"
#include "M2_SkinSection.h"

// FORWARD BEGIN
class CM2_Skin_Builder;
// FORWARD END

class CM2_Skin
{
	friend CM2_Skin_Batch;
	friend CM2_Skin_Builder;
public:
	CM2_Skin(M2* _model);
	~CM2_Skin();

	void Draw(CM2_MeshPartID_Provider* _provider, uint16 _animationIndex, cmat4 _worldMatrix, uint32 _time, uint32 globalTime);

private:
	vector<CM2_SkinSection*>	m_Sections;
	vector<CM2_Skin_Batch*>		m_Batches;

	//--
	M2* m_ParentM2;
	CGroupQuality& m_QualitySettings;
};