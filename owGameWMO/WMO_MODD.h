#pragma once

#include "WMO_Headers.h"

class WMO_MODD : public SceneNode
{
public:
	WMO_MODD(SceneNode* _parent, const WMO_MODD_PlacementInfo& _placement);

	void SetModel(M2* m);
	M2* GetMDX() { return m_Object; }

	// IUpdatable
	virtual void Update(double _time, double _dTime) override;

	// IRenderable
	virtual void PreRender3D() override;
	virtual void Render3D() override;

	uint32 GetNameIndex() { return m_NameIndex; }

private:
	SmartM2Ptr	m_Object;
	uint32		m_NameIndex;
};