#pragma once

#include "Liquid.h"

class Liquid_Instance : public SceneNode
{
public:
	Liquid_Instance(SceneNode* _parent, Liquid* _liquidObject, vec3 _position);

	// IRenderable3D
	void PreRender3D() override;
	void Render3D() override;

private:
	SmartPtr<Liquid> m_Object;
};