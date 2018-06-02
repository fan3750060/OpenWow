#pragma once

#include "Liquid.h"

class Liquid_Instance : public SceneNode, public CRenderable3DObject
{
public:
	Liquid_Instance(Liquid* _liquidObject, cvec3 _position);
	~Liquid_Instance();

	// IRenderable3D
	void PreRender3D(double t, double dt) override;
	void Render3D() override;
	void PostRender3D() override {};

private:
	SmartPtr<Liquid> m_Object;
};