#include "stdafx.h"

// General
#include "ADT_WMO_Instance.h"

ADT_WMO_Instance::ADT_WMO_Instance(WMO* _wmoObject, ADT_MODF _placementInfo) : 
    m_Object(_wmoObject),
    m_PlacementInfo(_placementInfo)
{
    assert1(m_Object);
	CalculateMatrix();
}

ADT_WMO_Instance::~ADT_WMO_Instance()
{
}

//

void ADT_WMO_Instance::Render()
{
	if (m_AlreadyDraw.find(m_PlacementInfo.uniqueId) != m_AlreadyDraw.end())
	{
		return;
	}
	m_AlreadyDraw.insert(m_PlacementInfo.uniqueId);

	//
	
	_Pipeline->Clear();
	{
		_Pipeline->SetWorld(m_RelTransform);
		m_Object->Render(m_PlacementInfo.doodadSetIndex);
		PERF_INC(PERF_MAP_MODELS_WMOs);
	}
}

void ADT_WMO_Instance::CalculateMatrix()
{
	// Convert rotation
	m_PlacementInfo.rotation = degToRad(m_PlacementInfo.rotation);
	m_PlacementInfo.rotation.x = -m_PlacementInfo.rotation.x;
	m_PlacementInfo.rotation.y = m_PlacementInfo.rotation.y - Math::PiHalf;

	// Build relative matrix
	m_RelTransform.translate(m_PlacementInfo.position);
	m_RelTransform.rotate(m_PlacementInfo.rotation.z, m_PlacementInfo.rotation.y, m_PlacementInfo.rotation.x);

	if (m_Parent != nullptr)
	{
		m_AbsTransform = (m_Parent->getAbsTrans()) * m_RelTransform;
	}
	else
	{
		m_AbsTransform = m_RelTransform;
	}
}

//

void ADT_WMO_Instance::reset()
{
	m_AlreadyDraw.clear();
}

set<uint32> ADT_WMO_Instance::m_AlreadyDraw;
