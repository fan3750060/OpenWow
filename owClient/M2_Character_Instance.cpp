#include "stdafx.h"

// General
#include "M2_Character_Instance.h"

CM2_Character_Instance::CM2_Character_Instance(M2* _object, vec3 translate, float scale) :
	CM2_Base_Instance(nullptr, _object)
{
	// Scene node params
	{
		m_Translate = translate;
		m_Scale = vec3(scale);

		CalculateMatrix(true);
		//
		m_Bounds = m_Object->getBounds();
		m_Bounds.transform(getAbsTrans());
	}

	InitLocal();

	setDebugColor(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	setSelectable();
}