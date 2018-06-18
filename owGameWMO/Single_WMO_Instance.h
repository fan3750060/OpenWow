#pragma once

#include "WMO.h"
#include "WMO_InstanceController.h"

class Single_WMO_Instance : public SceneNode
{
public:
	Single_WMO_Instance(SceneNode* _parent, WMO* _wmoObject);
	~Single_WMO_Instance();

	// IUpdatable
	void Update(double _time, double _dTime) override;

	// IRenderable3D
	void PreRender3D() override;
	void Render3D() override;

private:
	const SmartWMOPtr			m_Object;
	CWMO_InstanceController*	m_LocalPortalController;
};