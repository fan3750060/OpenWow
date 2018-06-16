#include "stdafx.h"

// General
#include "ADT_MDX_Instance.h"

// Additional
#include "WorldController.h"

ADT_MDX_Instance::ADT_MDX_Instance(SceneNode* _parent, M2* _mdxObject, const ADT_MDDF& _placementInfo) :
	SceneNode(_parent),
    m_Object(_mdxObject),
	m_NeedRecalcAnimation(true),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>()),
	m_DistancesSettings(GetSettingsGroup<CGroupDistances>())
{
    assert1(_mdxObject);
	m_UniqueId = _placementInfo.uniqueId;

	// Create animator
	if (m_Object->isAnimated())
	{
		m_Animator = new CM2_Animator(m_Object);
	}

	// Scene node params
	{
		// Translate
		m_Translate = _placementInfo.position;
		// Rotate
		vec3 rotate = _placementInfo.rotation.toRad();
		rotate.x = -rotate.x;
		rotate.y = rotate.y - Math::PiHalf;
		m_Rotate = vec3(rotate.z, rotate.y, rotate.x);
		// Scale
		m_Scale = vec3(static_cast<float>(_placementInfo.scale) / 1024.0f);
		//
		CalculateMatrix();
		//
		m_Bounds = m_Object->getBounds();
		m_Bounds.transform(getAbsTrans());
	}
	
	setDrawOrder(21);
	setSelectable();

	if (m_Object->isAnimated())
	{
		_Bindings->RegisterUpdatableObject(this);
	}
}

ADT_MDX_Instance::~ADT_MDX_Instance()
{
	if (m_Object->isAnimated())
	{
		delete m_Animator;
		_Bindings->UnregisterUpdatableObject(this);
	}
}

void ADT_MDX_Instance::Update(double _time, double _dTime)
{
	if (m_Object->isAnimated())
	{
		if (m_Object->isBillboard())
		{
			m_Object->animate(m_Animator->getSId(), m_Animator->getCurrentTime(_time), _time);
		}
		else
		{
			//if (!m_NeedRecalcAnimation)
			{
				m_Object->animate(m_Animator->getSId(), m_Animator->getCurrentTime(_time), _time);
			//	m_NeedRecalcAnimation = true;
			}
		}
	}

	m_Object->updateEmitters(_dTime);
}

void ADT_MDX_Instance::PreRender3D()
{
	setVisible(false);

	if (m_AlreadyDraw.find(m_UniqueId) != m_AlreadyDraw.end())
	{
		return;
	}

	// Check distance to camera
	float distToCamera = (_Render->getCamera()->Position.toX0Z() - getBounds().getCenter().toX0Z()).length();
	if (distToCamera > m_DistancesSettings.ADT_MDX_Distance)
	{
		return;
	}

	// Frustrum culling
	if (_Render->getCamera()->_frustum.cullBox(m_Bounds))
	{
		return;
	}

	m_AlreadyDraw.insert(m_UniqueId);
	setVisible(true);
}

void ADT_MDX_Instance::Render3D()
{
	if (!m_QualitySettings.draw_map_m2)
	{
		return;
	}

	//_Render->DrawBoundingBox(m_Bounds);

	m_Object->Render(getAbsTrans());
	PERF_INC(PERF_MAP_MODELS_MDXs);
}

//
set<uint32> ADT_MDX_Instance::m_AlreadyDraw;
void ADT_MDX_Instance::reset()
{
	m_AlreadyDraw.clear();
}
