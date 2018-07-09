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

		gProjView = getLocation("gProjView");
	}

	GeometryBase(RenderDevice* _RenderDevice, cstring _vsName, cstring _fsName, cstring _gsName = "") : 
		Technique(_RenderDevice, _vsName, _fsName, _gsName)
	{
		gProjection = getLocation("gProjection");
		gView = getLocation("gView");
		gWorld = getLocation("gWorld");

		gProjView = getLocation("gProjView");
	}

	void setWorld(cmat4 _world)
	{
		m_Shader->setShaderConst(gWorld, CONST_FLOAT44, &_world.x[0]);
	}
	void setView(cmat4 _view)
	{
		m_Shader->setShaderConst(gView, CONST_FLOAT44, &_view.x[0]);
	}
	void setProj(cmat4 _proj)
	{
		m_Shader->setShaderConst(gProjection, CONST_FLOAT44, &_proj.x[0]);
	}
	void setProjView(cmat4 _projView)
	{
		m_Shader->setShaderConst(gProjView, CONST_FLOAT44, &_projView.x[0]);
	}

private:
	int32 gProjection;
	int32 gView;
	int32 gWorld;

	int32 gProjView;
};