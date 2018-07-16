#include "stdafx.h"

// Include
#include "WMO_Base_Instance.h"
#include "WMO_Doodad_Instance.h"

// General
#include "WMO_Group_Instance.h"

CWMO_Group_Instance::CWMO_Group_Instance(CWMO_Base_Instance* _parent, const WMO_Group* _object) :
	SceneNode(_parent),
	m_Object(_object),
	m_PortalsVis(true),
	m_Calculated(false)
{
	{
		CalculateMatrix();

		BoundingBox bbox = _object->m_Bounds;
		bbox.calculateCenter();
		bbox.transform(_parent->getAbsTrans());
		setBounds(bbox);
	}

	setDebugColor(vec4(0.0f, 0.0f, 1.0f, 0.9f));
	setSelectable();
}

CWMO_Group_Instance::~CWMO_Group_Instance()
{
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
	m_Object->Render(getAbsTrans());

	//m_Object->RenderCollision(getAbsTrans());
}
