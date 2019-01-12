#include "stdafx.h"

// General
#include "WMO_Base_Instance.h"

CWMO_Base_Instance::CWMO_Base_Instance(std::shared_ptr<WMO> _wmoObject) :
	m_Object(_wmoObject)
{
	assert1(m_Object != nullptr);
}

CWMO_Base_Instance::~CWMO_Base_Instance()
{}

void CWMO_Base_Instance::InitTransform()
{
	m_Object->CreateInsances(std::static_pointer_cast<CWMO_Base_Instance, SceneNode>(shared_from_this()));

	m_InvWorld = glm::inverse(GetWorldTransfom());
	if (m_Object->m_PortalController != nullptr)
	{
		for (auto& v : m_Object->m_PortalVertices)
		{
			m_ConvertedVerts.push_back(GetWorldTransfom() * vec4(v, 0));
		}
	}
}

void CWMO_Base_Instance::EmptyTransformAndBounds()
{
	// Matrix
	UpdateLocalTransform();

	// Bounds
	BoundingBox bbox;
	bbox.setMin(vec3(-100000.0f, -100000.0f, -100000.0f));
	bbox.setMax(vec3(100000.0f, 100000.0f, 100000.0f));
	bbox.calculateCenter();
	setBounds(bbox);
}

#define WMO_DISABLE_PORTALS

bool CWMO_Base_Instance::Accept(IVisitor & visitor)
{
	const BasePass& visitorAsBasePass = reinterpret_cast<BasePass&>(visitor);
	const Camera& camera = *(visitorAsBasePass.GetRenderEventArgs().Camera);

	//if (!checkDistance2D(m_QualitySettings.ADT_WMO_Distance))
	//{
	//	return;
	//}

	if (!checkFrustum(camera))
	{
		return false;
	}

#ifndef WMO_DISABLE_PORTALS
	if (m_Object->m_PortalController != nullptr)
	{
		m_Object->m_PortalController->Update(this, getInvWorld() * (_Render->getCamera()->Position));
	}
#endif

	return SceneNode::Accept(visitor);
}
