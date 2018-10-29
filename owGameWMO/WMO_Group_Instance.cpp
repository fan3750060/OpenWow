#include "stdafx.h"

// Include
#include "WMO_Base_Instance.h"
#include "WMO_Doodad_Instance.h"
#include "WMO_Liquid_Instance.h"

// General
#include "WMO_Group_Instance.h"

CWMO_Group_Instance::CWMO_Group_Instance(const std::weak_ptr<const CWMO_Base_Instance> _parent, const std::shared_ptr<WMO_Group> _object) :
	m_Object(_object),
	m_PortalsVis(true),
	m_Calculated(false)
{
	{
		// Matrix
		CalculateLocalTransform();

		//BoundingBox bbox = _object->m_Bounds;
		//bbox.calculateCenter();
		//bbox.transform(_parent.lock()->GetWorldTransfom());
		//setBounds(bbox);
	}

	//setDebugColor(vec4(0.0f, 0.0f, 1.0f, 0.9f));
	//setSelectable();
}

void CWMO_Group_Instance::Accept(IVisitor& visitor)
{
	SceneNode::Accept(visitor);
}

bool CWMO_Group_Instance::PreRender3D()
{
	if (!m_PortalsVis)
	{
		return false;
	}

	return true;
}

void CWMO_Group_Instance::Render3D()
{
	//m_Object->Render(GetWorldTransfom());

	//m_Object->RenderCollision(GetWorldTransfom());
}
