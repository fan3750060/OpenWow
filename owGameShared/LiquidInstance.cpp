#include "stdafx.h"

// General
#include "LiquidInstance.h"

Liquid_Instance::Liquid_Instance(SceneNode* _parent, Liquid* _liquidObject, cvec3 _position) :
	SceneNode(_parent),
	m_Object(_liquidObject)
{
	assert1(m_Object);

	m_Translate = _position;

	CalculateMatrix();
	SetDrawOrder(20);
}

void Liquid_Instance::PreRender3D()
{
	SetVisible(m_Object->getVerticesCnt() > 0);
}

void Liquid_Instance::Render3D()
{
	_Pipeline->Clear();
	{
		_Pipeline->SetWorld(m_RelTransform);
		m_Object->Render();
	}
}