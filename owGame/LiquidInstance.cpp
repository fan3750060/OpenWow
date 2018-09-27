#include "stdafx.h"

// General
#include "LiquidInstance.h"

Liquid_Instance::Liquid_Instance(std::weak_ptr<SceneNode> _parent, Liquid* _liquidObject, vec3 _position) :
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	// Scene node params
	{
		// Translate
		setTranslate(_position, false);
		//
		CalculateLocalTransform();
		//
		BoundingBox bbox(vec3(Math::MinFloat, Math::MinFloat, Math::MinFloat), vec3(Math::MaxFloat, Math::MaxFloat, Math::MaxFloat));
		bbox.transform(GetWorldTransfom());
		setBounds(bbox);
	}
}