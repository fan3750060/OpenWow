#include "stdafx.h"

// General
#include "LiquidInstance.h"

Liquid_Instance::Liquid_Instance(SceneNode* _parent, Liquid* _liquidObject, vec3 _position) :
	SceneNode(_parent),
	m_Object(_liquidObject)
{
	// Scene node params
	{
		// Translate
		m_Translate = _position;
		//
		CalculateMatrix();
		//
		m_Bounds.Min = -vec3(25, 25, 25); // Don't use from WMO model!!!
		m_Bounds.Max = vec3(25, 25, 25);
		m_Bounds.calculateCenter();
		m_Bounds.transform(getAbsTrans());
	}

	setDrawOrder(29);
}

void Liquid_Instance::PreRender3D()
{
	setVisible(true);
}

void Liquid_Instance::Render3D()
{
	//_Render->DrawBoundingBox(m_Bounds);

	_Pipeline->Clear();
	{
		_Pipeline->SetWorld(getAbsTrans());
		m_Object->Render();
	}
	_Pipeline->Clear();
}