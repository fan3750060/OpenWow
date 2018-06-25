#pragma once

#include "M2.h"

class CM2_Base_Instance : public SceneNode
{
public:
	CM2_Base_Instance(SceneNode* _parent, M2* _m2Object);
	~CM2_Base_Instance();

	// CM2_Base_Instance
	void InitLocal();


	M2* getObject() { return m_Object; }
	CM2_Animator* getAnimator() { return m_Animator; }

	// IUpdatable
	void Update(double _time, double _dTime) override;

	// IRenderable3D
	bool PreRender3D() override;
	void Render3D() override;

protected:
	SmartM2Ptr			m_Object;

	CM2_Animator*		m_Animator;
	bool				m_NeedRecalcAnimation;

	CGroupQuality&		m_QualitySettings;
};