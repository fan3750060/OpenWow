#include "stdafx.h"

// General
#include "ADT_MDX_Instance.h"

ADT_MDX_Instance::ADT_MDX_Instance(MDX* _mdxObject, ADT_MDDF _placementInfo) : 
    m_Object(_mdxObject),
    m_PlacementInfo(_placementInfo)
{
    assert1(_mdxObject);
	CalculateMatrix();
}

ADT_MDX_Instance::~ADT_MDX_Instance()
{
}

void ADT_MDX_Instance::Render()
{
	//float dist = (placementInfo->position - _Camera->Position).length();
	//if (dist > _Config.modeldrawdistance + modelObject->m_Radius * sc)
	//{
	//	return;
	//}

	if (m_AlreadyDraw.find(m_PlacementInfo.uniqueId) != m_AlreadyDraw.end())
	{
		return;
	}
	m_AlreadyDraw.insert(m_PlacementInfo.uniqueId);

	_Pipeline->Clear();
	{
		_Pipeline->SetWorld(m_RelTransform);
		m_Object->Render();
		PERF_INC(PERF_MAP_MODELS_MDXs);
	}
}

void ADT_MDX_Instance::CalculateMatrix()
{
	// Convert rotation
	m_PlacementInfo.rotation = degToRad(m_PlacementInfo.rotation);
	m_PlacementInfo.rotation.x = -m_PlacementInfo.rotation.x;
	m_PlacementInfo.rotation.y = m_PlacementInfo.rotation.y - Math::PiHalf;

	// Build relative matrix
	m_RelTransform.translate(m_PlacementInfo.position);
	m_RelTransform.rotate(m_PlacementInfo.rotation.z, m_PlacementInfo.rotation.y, m_PlacementInfo.rotation.x);
	m_RelTransform.scale(static_cast<float>(m_PlacementInfo.scale) / 1024.0f);

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

void ADT_MDX_Instance::reset()
{
	m_AlreadyDraw.clear();
}

set<uint32> ADT_MDX_Instance::m_AlreadyDraw;