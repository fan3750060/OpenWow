#include "stdafx.h"

// Include
#include "WMO_Base_Instance.h"
#include "WMO_Doodad_Instance.h"
#include "WMO_Liquid_Instance.h"

// General
#include "WMO_Group_Instance.h"

CWMO_Group_Instance::CWMO_Group_Instance(const std::shared_ptr<WMO_Group> _object) :
	m_Object(_object),
	m_PortalsVis(true),
	m_Calculated(false)
{
}

CWMO_Group_Instance::~CWMO_Group_Instance()
{
}

void CWMO_Group_Instance::SetParent(std::weak_ptr<SceneNode> pNode)
{
	SceneNode::SetParent(pNode);

	BoundingBox bbox = m_Object->m_Bounds;
	bbox.calculateCenter();
	bbox.transform(GetParentWorldTransform());
	SetBounds(bbox);
}

bool CWMO_Group_Instance::Accept(IVisitor& visitor)
{
	if (!m_PortalsVis)
	{
		return false;
	}

	return SceneNode::Accept(visitor);
}
