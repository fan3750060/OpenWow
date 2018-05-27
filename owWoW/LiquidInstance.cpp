#include "stdafx.h"

// General
#include "LiquidInstance.h"

Liquid_Instance::Liquid_Instance(Liquid* _liquidObject, cvec3 _position) :
	m_Object(_liquidObject)
{
	assert1(m_Object);

	m_Translate = _position;

	CalculateMatrix();
	_Bindings->RegisterRenderable3DObject(this, 20);
}

Liquid_Instance::~Liquid_Instance()
{
	_Bindings->UnregisterRenderable3DObject(this);
}

void Liquid_Instance::PreRender3D(double t, double dt)
{
	SetVisible(true);
}

void Liquid_Instance::Render3D()
{
	_Pipeline->Clear();
	{
		_Pipeline->SetWorld(m_RelTransform);
		m_Object->Render();
	}
}