#pragma once

#include "M2.h"

class CM2_Base_Instance : public SceneNode
{
public:
	CM2_Base_Instance(SceneNode* _parent);
	CM2_Base_Instance(SceneNode* _parent, M2* _m2Object);
	~CM2_Base_Instance();

	// CM2_Base_Instance
	void InitLocal();

	void setObject(M2* _model) { m_Object = _model; }
	M2* getObject() { return m_Object; }

	CM2_Animator* getAnimator() { return m_Animator; }

	void setMeshProvider(CM2_MeshPartID_Provider* _provider) { m_MeshProvider = _provider; }
	CM2_MeshPartID_Provider* getMeshProvider() const { return m_MeshProvider; }

	// IUpdatable
	void Update(double _time, double _dTime) override;

	// IRenderable3D
	bool PreRender3D() override;
	void Render3D() override;

protected:
	SmartM2Ptr			m_Object;

	CM2_Animator*		m_Animator;
	bool				m_NeedRecalcAnimation;

	CM2_MeshPartID_Provider* m_MeshProvider;

	CGroupQuality&		m_QualitySettings;

	double m_Time;
	double m_DTime;

	vec4 m_DoodadColor;
};