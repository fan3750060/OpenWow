#pragma once

#include "WMO_Headers.h"

#include "WMO_Group.h"

class WMO_Doodad_Instance : public SceneNode
{
public:
	WMO_Doodad_Instance(SceneNode* _parent, M2* _mdxObject, uint32 _index, const WMO_Doodad_PlacementInfo& _placement, const WMO_Group* _group);
	~WMO_Doodad_Instance();

	// ISceneNode
	string getObjectInfo() override { return m_Object->getFilename(); }

	// IUpdatable
	void Update(double _time, double _dTime) override;

	// IRenderable
	void PreRender3D() override;
	void Render3D() override;
	void RenderDebug3D() override;

private:
	SmartM2Ptr			m_Object;
	const uint32		m_Index;
	const WMO_Group*	m_Group;					// used for visibility

	CM2_Animator*		m_Animator;
	bool				m_NeedRecalcAnimation;

	CGroupQuality&		m_QualitySettings;
	CGroupDistances&	m_DistancesSettings;
};