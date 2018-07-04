#include "stdafx.h"

// General
#include "M2_Base_Instance.h"

CM2_Base_Instance::CM2_Base_Instance(SceneNode* _parent) :
	SceneNode(_parent),
	m_Animator(nullptr),
	m_NeedRecalcAnimation(true),
	m_MeshProvider(nullptr),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>()),
	m_Time(0.0f),
	m_DTime(0.0f)
{
	setDrawOrder(21);
}

CM2_Base_Instance::CM2_Base_Instance(SceneNode* _parent, M2* _m2Object) :
	SceneNode(_parent),
	m_Object(_m2Object),
	m_Animator(nullptr),
	m_NeedRecalcAnimation(true),
	m_MeshProvider(nullptr),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	assert1(m_Object);

	// Scene node params
	{
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

	if (m_MeshProvider != nullptr)
	{
		delete m_MeshProvider;
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
	m_Time = _time;
	m_DTime = _dTime;
	

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
	if (m_Object->isAnimated())
	{
		m_Animator->Update(m_Time, m_DTime);

		/*if (m_Object->isBillboard())
		{
		m_Object->animate(m_Animator->getAnimID(), m_Animator->getCurrentTime(_time), _time);
		}
		else
		{*/
		//if (!m_NeedRecalcAnimation)
		//{
		m_Object->animate(m_Animator->getSequenceIndex(), getAbsTrans(), m_Animator->getCurrentTime(), static_cast<uint32>(m_Time));
		//	m_NeedRecalcAnimation = true;
		//}
		//}
	}

	m_Object->Render(getAbsTrans(), m_MeshProvider);
}