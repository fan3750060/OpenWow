#pragma once

#include "M2_SkinTypes.h"
#include "M2_Skin_Batch.h"

// FORWARD BEGIN
class CM2_Skin_Builder;
// FORWARD END

class CM2_Skin
{
	friend CM2_Skin_Builder;
public:
	CM2_Skin(MDX* _model);
	~CM2_Skin();

	void Draw();

private:
	MDX*						m_MDX;
	vector<MDX_Skin_Batch*>		m_Batches;

	SmartGeomPtr				__geom;
	vector<bool>				m_VisibleSubmeshes;
};