#pragma once

#include "Liquid.h"

class Liquid_Instance : protected SceneNode, public Renderable3DObject
{
public:
	Liquid_Instance(Liquid* _liquidObject, cvec3 _position);
	~Liquid_Instance();

	// Renderable3DObject
	void PreRender3D(double t, double dt);
	void Render3D();

private:
	Liquid* m_Object;
};