#pragma once

#include "Liquid.h"

class Liquid_Instance : public SceneNode3D
{
public:
	Liquid_Instance(std::shared_ptr<Liquid> _liquidObject, vec3 _position);
	virtual ~Liquid_Instance() {};

	// SceneNode3D
	bool Accept(IVisitor& visitor) override;

private:
	const CGroupQuality& m_QualitySettings;
};