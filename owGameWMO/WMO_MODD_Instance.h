#pragma once

#include "WMO_Headers.h"

class WMO_MODD_Instance : public SceneNode
{
public:
	WMO_MODD_Instance(SceneNode* _parent, M2* _mdxObject, const WMO_MODD_PlacementInfo& _placement);
	~WMO_MODD_Instance();

	// ISceneNode
	string getObjectInfo() override { return m_Object->getFilename(); }

	// IUpdatable
	virtual void Update(double _time, double _dTime) override;

	// IRenderable
	virtual void PreRender3D() override;
	virtual void Render3D() override;

private:
	SmartM2Ptr			m_Object;
	CM2_Animator*		m_Animator;
	bool				m_NeedRecalcAnimation;

	CGroupQuality&		m_QualitySettings;
	CGroupDistances&	m_DistancesSettings;
};