#pragma once

#include "WMO_Headers.h"

#include "WMO_Group.h"

class CWMO_Liquid_Instance : public Liquid_Instance
{
public:
	CWMO_Liquid_Instance(std::weak_ptr<SceneNode> _parent, Liquid* _liquidObject, vec3 _position, const WMO_Group* _group);
	~CWMO_Liquid_Instance();

	// IRenderable3D
	bool PreRender3D();

private:
	const WMO_Group*	m_Group;					// used for visibility
};