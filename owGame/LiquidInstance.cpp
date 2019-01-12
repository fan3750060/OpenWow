#include "stdafx.h"

// General
#include "LiquidInstance.h"

Liquid_Instance::Liquid_Instance(std::shared_ptr<Liquid> _liquidObject, vec3 _position) :
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	// Scene node params
	{
		setTranslate(_position);
		//
		UpdateLocalTransform();
		//
		BoundingBox bbox(vec3(Math::MinFloat, Math::MinFloat, Math::MinFloat), vec3(Math::MaxFloat, Math::MaxFloat, Math::MaxFloat));
		bbox.transform(GetWorldTransfom());
		setBounds(bbox);
	}

	for (const auto& it : _liquidObject->m_WaterLayers)
	{
		AddMesh(it);
	}
}

bool Liquid_Instance::Accept(IVisitor & visitor)
{
	int x = 0;
	assert1(TRUE);
	return SceneNode::Accept(visitor);
}
