#pragma once

#include "Liquid.h"

class Liquid_Instance : public SceneNode
{
public:
	Liquid_Instance(std::weak_ptr<SceneNode> _parent, std::shared_ptr<Liquid> _liquidObject, vec3 _position);
	virtual ~Liquid_Instance() {};

private:
	const CGroupQuality& m_QualitySettings;
};