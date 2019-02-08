#include "stdafx.h"

// General
#include "WMO_Base_Instance.h"

CWMO_Base_Instance::CWMO_Base_Instance(std::string _wmoName) :
	m_WMOName(_wmoName)
{}

CWMO_Base_Instance::~CWMO_Base_Instance()
{}


void CWMO_Base_Instance::CreateInstances()
{
	m_WMO->CreateInsances(std::static_pointer_cast<CWMO_Base_Instance, SceneNode3D>(shared_from_this()));

	if (m_WMO->m_PortalController != nullptr)
	{
		for (auto& v : m_WMO->m_PortalVertices)
		{
			m_ConvertedVerts.push_back(GetWorldTransfom() * vec4(v, 1.0f));
		}
	}
}

void CWMO_Base_Instance::setWMO(std::shared_ptr<WMO> _model)
{
	assert1(m_WMO == nullptr);
	assert1(_model != nullptr);

	m_WMO = _model;
}

std::shared_ptr<WMO> CWMO_Base_Instance::getWMO() const
{
	return m_WMO;
}


//#define WMO_DISABLE_PORTALS

bool CWMO_Base_Instance::Load()
{
	std::shared_ptr<WMO> wmo = GetManager<IWMOManager>()->Add(m_WMOName);
	if (wmo)
	{
		setWMO(wmo);
		CreateInstances();
		return true;
	}

	return false;
}

bool CWMO_Base_Instance::Delete()
{
	return false;
}



void CWMO_Base_Instance::UpdateCamera(const Camera* camera)
{
#ifndef WMO_DISABLE_PORTALS
	if (m_WMO && m_WMO->m_PortalController)
	{
		m_WMO->m_PortalController->Update(std::dynamic_pointer_cast<CWMO_Base_Instance, SceneNode3D>(shared_from_this()), *camera);
	}
#endif
}

bool CWMO_Base_Instance::Accept(IVisitor& visitor)
{
	const BasePass& visitorAsBasePass = reinterpret_cast<BasePass&>(visitor);
	const Camera* camera = visitorAsBasePass.GetRenderEventArgs().Camera;

	//if (!checkDistance2D(m_QualitySettings.ADT_WMO_Distance))
	//{
	//	return;
	//}

	if (!checkFrustum(camera))
	{
		return false;
	}

	return SceneNode3D::Accept(visitor);
}
