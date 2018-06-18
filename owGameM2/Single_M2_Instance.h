#pragma once

#include "M2.h"

class Single_M2_Instance : public SceneNode
{
public:
	Single_M2_Instance(SceneNode* _parent, M2* _m2Object);
	~Single_M2_Instance();

	M2* getObject() { return m_Object; }
	CM2_Animator* getAnimator() { return m_Animator; }

	// IUpdatable
	void Update(double _time, double _dTime) override;

	// IRenderable3D
	void PreRender3D() override;
	void Render3D() override;

private:
	SmartM2Ptr			m_Object;
	CM2_Animator*		m_Animator;
	uint32				m_CurrentAnimation;
	bool				m_NeedRecalcAnimation;
};