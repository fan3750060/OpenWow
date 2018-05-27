#include "stdafx.h"

// General
#include "WMO_MODD.h"

WMO_MODD::WMO_MODD(IFile* f)
{
	DoodadPlacementInfo placementInfo;
	f->ReadBytes(&placementInfo, DoodadPlacementInfo::__size);

	// Convert
	placementInfo.position.toXZmY();
	m_Rotate; // FIXME
	m_Scale = vec3(placementInfo.scale, -placementInfo.scale, -placementInfo.scale);

	// Build relative matrix
	m_RelTransform.translate(m_Translate);
	m_RelTransform *= Quaternion(-placementInfo.orientation.z, placementInfo.orientation.x, placementInfo.orientation.y, placementInfo.orientation.w);
	m_RelTransform.scale(m_Scale);

	if (m_Parent != nullptr)
	{
		m_AbsTransform = (m_Parent->getAbsTrans()) * m_RelTransform;
	}
	else
	{
		m_AbsTransform = m_RelTransform;
	}
}

WMO_MODD::~WMO_MODD()
{

}

void WMO_MODD::SetModel(MDX* _model)
{
	assert1(_model != nullptr);

	m_Object = _model;
	m_Bounds = m_Object->m_Bounds;
	m_Bounds.transform(m_AbsTransform);
}

void WMO_MODD::Render()
{
	BoundingBox aabb = m_Bounds;
	aabb.transform(_Pipeline->GetWorld());

	_Pipeline->Push(); // Save world matrix
	{
		_Pipeline->Mult(m_RelTransform);

		// Get actual position
		//vec3 pos = _Pipeline->GetWorld() * vec3(1.0f, 1.0f, 1.0f);
		//float radius = modelObject->m_Bounds.Radius * placementInfo->scale;

		// Distance test
		//if ((pos - _Camera->Position).length2() > (_Config.doodaddrawdistance2 + radius))
		//{
		//	_Pipeline->Pop(); // restore matrix
		//	return;
		//}

		// Frustrum test
		if (_CameraFrustum->_frustum.cullBox(aabb))
		{
			_Pipeline->Pop(); // restore matrix
			return;
		}

		m_Object->Render();
		PERF_INC(PERF_MAP_MODELS_WMOs_DOODADS);
	}
	_Pipeline->Pop();  // restore matrix
}

void WMO_MODD::CalculateMatrix()
{
	

	if (m_Parent != nullptr)
	{
		m_AbsTransform = (m_Parent->getAbsTrans()) * m_RelTransform;
	}
	else
	{
		m_AbsTransform = m_RelTransform;
	}
}


