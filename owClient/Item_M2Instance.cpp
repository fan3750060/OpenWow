#include "stdafx.h"

// General
#include "Item_M2Instance.h"

CItem_M2Instance::CItem_M2Instance(CM2_Base_Instance* _parent, SmartM2Ptr _model) :
	CM2_Base_Instance(_parent, _model)
{ 

}

void CItem_M2Instance::AddVisualEffect(std::shared_ptr<CM2_Base_Instance> _visualEffect)
{
	m_VisualEffects.push_back(_visualEffect);
}

void CItem_M2Instance::Render3D()
{
	CM2_Base_Instance::Render3D();

	// Visual effects
	for (auto& vis : m_VisualEffects)
	{
		vis->Render3D();
	}
}
