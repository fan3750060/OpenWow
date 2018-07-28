#pragma once

#include "Technique.h"

// FORWARD BEGIN
class TechniquesManager;
// FORWARD END

class UIBase : public Technique
{
	friend TechniquesManager;
public:
	UIBase(RenderDevice* _RenderDevice, cstring _name) :
		Technique(_RenderDevice, _name)
	{
		gProjection = getLocation("gProjection");
	}

	void setProj(cmat4 WorldInverse)
	{
		m_Shader->setShaderConst(gProjection, CONST_FLOAT44, glm::value_ptr(WorldInverse));
	}

private:
	int32 gProjection;
};