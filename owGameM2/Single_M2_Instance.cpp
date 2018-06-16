#include "stdafx.h"

// General
#include "Single_M2_Instance.h"

Single_M2_Instance::Single_M2_Instance(SceneNode* _parent, M2* _m2Object) :
	SceneNode(_parent),
	m_Object(_m2Object),
	m_Animator(nullptr)
{
	assert1(m_Object);

	// Create animator
	if (m_Object->isAnimated())
	{
		m_Animator = new CM2_Animator(m_Object);
	}

	// Scene node params
	{
		// Translate
		//m_Translate = vec3(100, 0, 100);
		// Rotate
		//m_Rotate = vec3(Math::TwoPi, Math::TwoPi, Math::PiHalf);
		//
		CalculateMatrix();
		//
		m_Bounds = m_Object->m_Bounds;
		m_Bounds.transform(getAbsTrans());
	}

	setDrawOrder(21);

	if (m_Object->isAnimated())
	{
		_Bindings->RegisterUpdatableObject(this);
	}
}

Single_M2_Instance::~Single_M2_Instance()
{
	if (m_Object->isAnimated())
	{
		delete m_Animator;
		_Bindings->UnregisterUpdatableObject(this);
	}
}

void Single_M2_Instance::Update(double _time, double _dTime)
{
	if (m_Object->isAnimated())
	{
		if (m_Object->isBillboard())
		{
			m_Object->animate(m_Animator->getSId(), m_Animator->getCurrentTime(_time), _time);
		}
		else
		{
			if (!m_NeedRecalcAnimation)
			{
				m_Object->animate(m_Animator->getSId(), m_Animator->getCurrentTime(_time), _time);
				m_NeedRecalcAnimation = true;
			}
		}
	}

	m_Object->updateEmitters(_dTime);
}

void Single_M2_Instance::PreRender3D()
{
	setVisible(!_Render->getCamera()->_frustum.cullBox(m_Bounds));
}

void Single_M2_Instance::Render3D()
{
	//_Render->DrawBoundingBox(m_Bounds);

	m_Object->Render(getAbsTrans());
	PERF_INC(PERF_MAP_MODELS_WMOs);
}