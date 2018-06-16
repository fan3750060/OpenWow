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
	CM2_Skin(M2* _model);
	~CM2_Skin();

	void Draw();
	void RenderNormals(cmat4 _worldMatrix);

private:
	M2*						m_MDX;
	vector<MDX_Skin_Batch*>		m_Batches;

	SmartGeomPtr				__geom;
	SmartGeomPtr				__geomDebugNormals;
	vector<bool>				m_VisibleSubmeshes;
};