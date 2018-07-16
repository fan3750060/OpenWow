#include "stdafx.h"

// General
#include "LiquidInstance.h"

Liquid_Instance::Liquid_Instance(SceneNode* _parent, Liquid* _liquidObject, vec3 _position) :
	SceneNode(_parent),
	m_Object(_liquidObject),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	// Scene node params
	{
		// Translate
		setTranslate(_position, false);
		//
		CalculateMatrix();
		//
		BoundingBox bbox(-vec3(25, 25, 25), vec3(25, 25, 25));
		bbox.transform(getAbsTrans());
		setBounds(bbox);
	}

	setDrawOrder(29);
}

bool Liquid_Instance::PreRender3D()
{
	return true;
}

void Liquid_Instance::Render3D()
{
	if (!m_QualitySettings.draw_water)
	{
		return;
	}

	_Render->DrawBoundingBox(getBounds());

	m_Object->Render(getAbsTrans());
}