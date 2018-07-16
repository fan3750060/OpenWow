#include "stdafx.h"

// General
#include "CM2_Viewer_Instance.h"

CM2_Viewer_Instance::CM2_Viewer_Instance(M2 * _object) :
	CM2_Base_Instance(nullptr, _object)
{
	// Scene node params
	{
		setScale(10.0f);

		CalculateMatrix();
	}

	InitLocal();

	setDebugColor(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	setSelectable();
}
