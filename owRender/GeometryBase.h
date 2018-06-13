#pragma once

#include "Technique.h"

// FORWARD BEGIN
class TechniquesManager;
// FORWARD END

class GeometryBase : public Technique
{
	friend TechniquesManager;
public:
	GeometryBase(RenderDevice* _RenderDevice, cstring _name) :
		Technique(_RenderDevice, _name)
	{
		gProjection = getLocation("gProjection");
		gView = getLocation("gView");
		gWorld = getLocation("gWorld");
	}

	GeometryBase(RenderDevice* _RenderDevice, cstring _vsName, cstring _fsName, cstring _gsName = "") : 
		Technique(_RenderDevice, _vsName, _fsName, _gsName)
	{
		gProjection = getLocation("gProjection");
		gView = getLocation("gView");
		gWorld = getLocation("gWorld");
	}

	void SetWorldMatrix(cmat4 WorldInverse)
	{
		m_Shader->setShaderConst(gWorld, CONST_FLOAT44, &WorldInverse.x[0]);
	}

	void SetViewMatrix(cmat4 WorldInverse)
	{
		m_Shader->setShaderConst(gView, CONST_FLOAT44, &WorldInverse.x[0]);
	}

	void SetProjectionMatrix(cmat4 WorldInverse)
	{
		m_Shader->setShaderConst(gProjection, CONST_FLOAT44, &WorldInverse.x[0]);
	}

private:
	int32 gProjection;
	int32 gView;
	int32 gWorld;
};