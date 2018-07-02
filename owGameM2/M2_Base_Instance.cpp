#include "stdafx.h"

// General
#include "M2_Base_Instance.h"

CM2_Base_Instance::CM2_Base_Instance(SceneNode* _parent, M2* _m2Object) :
	SceneNode(_parent),
	m_Object(_m2Object),
	m_Animator(nullptr),
	m_NeedRecalcAnimation(true),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	assert1(m_Object);

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
}

CM2_Base_Instance::~CM2_Base_Instance()
{
	if (m_Object->isAnimated())
	{
		delete m_Animator;
		_Bindings->UnregisterUpdatableObject(this);
	}

}

void CM2_Base_Instance::InitLocal()
{
	// Create animator
	if (m_Object->isAnimated())
	{
		m_Animator = new CM2_Animator(m_Object);
		_Bindings->RegisterUpdatableObject(this);
	}
}

void CM2_Base_Instance::Update(double _time, double _dTime)
{
	if (m_Object->isAnimated())
	{
		m_Animator->Update(_time, _dTime);

		/*if (m_Object->isBillboard())
		{
			m_Object->animate(m_Animator->getAnimID(), m_Animator->getCurrentTime(_time), _time);
		}
		else
		{*/
			//if (!m_NeedRecalcAnimation)
			//{
				m_Object->animate(m_Animator->getSequenceIndex(), m_Animator->getCurrentTime(), static_cast<uint32>(_time));
			//	m_NeedRecalcAnimation = true;
			//}
		//}
	}

	m_Object->updateEmitters(_dTime);
}

bool CM2_Base_Instance::PreRender3D()
{
	if (_Render->getCamera()->_frustum.cullBox(m_Bounds))
	{
		return false;
	}

	return true;
}

void CM2_Base_Instance::Render3D()
{
	//_Render->DrawBoundingBox(m_Bounds);
	m_Object->Render(getAbsTrans());
}