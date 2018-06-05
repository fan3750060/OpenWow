#include "stdafx.h"

// General
#include "WMO_MODD.h"

// Additional
#include "WorldController.h"

WMO_MODD::WMO_MODD(IFile* f)
{
	DoodadPlacementInfo placementInfo;
	f->ReadBytes(&placementInfo, sizeof(DoodadPlacementInfo));

	m_NameIndex = placementInfo.flags.nameIndex;

	{
		// Convert
		m_Translate = placementInfo.position.toXZmY();
		m_RotateQuat = Quaternion(-placementInfo.orientation.z, placementInfo.orientation.x, placementInfo.orientation.y, placementInfo.orientation.w);;
		m_Scale = vec3(placementInfo.scale, -placementInfo.scale, -placementInfo.scale);

		// Build relative matrix
		CalculateMatrix(true);
	}
}

void WMO_MODD::SetModel(M2* _model)
{
	assert1(_model != nullptr);

	m_Object = _model;
	m_Bounds = m_Object->GetBounds();
	m_Bounds.transform(m_AbsTransform);
}

void WMO_MODD::Render()
{
	BoundingBox aabb = m_Bounds;
	aabb.transform(_Pipeline->GetWorld());

	_Pipeline->Push(); // Save world matrix
	{
		_Pipeline->Mult(m_AbsTransform);

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

		m_Object->Render(_World->EnvM()->globalTime);
		PERF_INC(PERF_MAP_MODELS_WMOs_DOODADS);
	}
	_Pipeline->Pop();  // restore matrix
}


