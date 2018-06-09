#include "stdafx.h"

// General
#include "WMO_MODD.h"

WMO_MODD::WMO_MODD(SceneNode* _parent, const WMO_MODD_PlacementInfo& _placement) :
	SceneNode(_parent)
{
	m_NameIndex = _placement.flags.nameIndex;

	{
		// Convert
		m_Translate = _placement.position.toXZmY();
		m_RotateQuat = Quaternion(-_placement.orientation.z, _placement.orientation.x, _placement.orientation.y, _placement.orientation.w);;
		m_Scale = vec3(_placement.scale, -_placement.scale, -_placement.scale);

		// Build relative matrix
		CalculateMatrix(true);
	}

	SetDrawOrder(20);
}

void WMO_MODD::SetModel(M2* _model)
{
	assert1(_model != nullptr);

	m_Object = _model;
	m_Bounds = m_Object->GetBounds();
	m_Bounds.transform(m_AbsTransform);
}

void WMO_MODD::Update(double _time, double _dTime)
{
	m_Object->Update(_time, _dTime);
}

void WMO_MODD::PreRender3D()
{
	SetVisible(true);
}

void WMO_MODD::Render3D()
{
	/*BoundingBox aabb = m_Bounds;
	aabb.transform(_Pipeline->GetWorld());

	_Pipeline->Push(); // Save world matrix
	{
		_Pipeline->Mult(m_AbsTransform);

		// Get actual position
		//vec3 pos = _Pipeline->GetWorld() * vec3(1.0f, 1.0f, 1.0f);
		//float radius = modelObject->m_Bounds.Radius * _placement->scale;

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
	_Pipeline->Pop();  // restore matrix*/

	_Pipeline->Clear();
	{
		_Pipeline->Mult(m_AbsTransform);
		m_Object->Render();
		PERF_INC(PERF_MAP_MODELS_WMOs_DOODADS);
	}
}


