#pragma once

#include "WMO_Headers.h"

#include "WMO_Group.h"

class CWMO_Liquid_Instance : public Liquid_Instance
{
public:
	CWMO_Liquid_Instance(std::shared_ptr<Liquid> _liquidObject, vec3 _position, const std::weak_ptr<const WMO_Group> _group);

	// IRenderable3D
	bool PreRender3D();

private:
	const std::weak_ptr<const WMO_Group>	m_Group;					// used for visibility
};