#pragma once

#include "WMO_Headers.h"

#include "WMO_Group.h"

class CWMO_Liquid_Instance : public Liquid_Instance
{
public:
	CWMO_Liquid_Instance(SceneNode* _parent, Liquid* _liquidObject, vec3 _position, const WMO_Group* _group);

	// IRenderable3D
	void PreRender3D() override;

private:
	const WMO_Group*	m_Group;					// used for visibility
};