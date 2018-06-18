#include "stdafx.h"

// General
#include "WMO_Doodad_Instance.h"

WMO_Doodad_Instance::WMO_Doodad_Instance(SceneNode* _parent, M2* _mdxObject, uint32 _index, const WMO_Doodad_PlacementInfo& _placement, const WMO_Group* _group) :
	SceneNode(_parent),
	m_Object(_mdxObject),
	m_Index(_index),
	m_Group(_group),
	m_NeedRecalcAnimation(true),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>()),
	m_DistancesSettings(GetSettingsGroup<CGroupDistances>())
{
	assert1(m_Object != nullptr);

	// Create animator
	if (m_Object->isAnimated())
	{
		m_Animator = new CM2_Animator(m_Object);
	}

	// Scene node params
	{
		// Convert
		m_Translate = _placement.position.toXZmY();
		m_RotateQuat = Quaternion(-_placement.orientation.z, _placement.orientation.x, _placement.orientation.y, _placement.orientation.w);;
		m_Scale = vec3(_placement.scale, -_placement.scale, -_placement.scale);
		//
		CalculateMatrix(true);
		//
		m_Bounds = m_Object->getBounds();
		m_Bounds.transform(getAbsTrans());
	}

	setDrawOrder(21);
	setDebugColor(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	setSelectable();

	if (m_Object->isAnimated())
	{
		_Bindings->RegisterUpdatableObject(this);
	}
}

WMO_Doodad_Instance::~WMO_Doodad_Instance()
{
	if (m_Object->isAnimated())
	{
		delete m_Animator;
		_Bindings->UnregisterUpdatableObject(this);
	}
}

void WMO_Doodad_Instance::Update(double _time, double _dTime)
{
	if (m_Object->isAnimated())
	{
		m_Animator->Update(_time, _dTime);

		/*if (m_Object->isBillboard())
		{
		m_Object->animate(m_Animator->getSId(), m_Animator->getCurrentTime(_time), _time);
		}
		else
		{*/
		//if (!m_NeedRecalcAnimation)
		//{
		m_Object->animate(m_Animator->getSId(), m_Animator->getCurrentTime(), static_cast<uint32>(_time));
		//	m_NeedRecalcAnimation = true;
		//}
		//}
	}

	m_Object->updateEmitters(_dTime);
}

void WMO_Doodad_Instance::PreRender3D()
{
	setVisible(false);

	if (!m_Group->m_PortalsVis)
	{
		return;
	}

	// Check distance to camera
	float distToCamera = (_Render->getCamera()->Position - getBounds().getCenter()).length();
	if (distToCamera > m_DistancesSettings.WMO_MODD_Distance)
	{
		return;
	}

	// Frustrum culling
	if (_Render->getCamera()->_frustum.cullBox(m_Bounds))
	{
		return;
	}

	setVisible(true);
}

void WMO_Doodad_Instance::Render3D()
{
	if (!m_QualitySettings.draw_wmo_doodads)
	{
		return;
	}

	//_Render->DrawBoundingBox(m_Bounds);

	m_Object->Render(getAbsTrans());
	_Render->r.checkError();
	PERF_INC(PERF_MAP_MODELS_WMOs_DOODADS);
}

void WMO_Doodad_Instance::RenderDebug3D()
{
	return;
}


