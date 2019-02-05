#pragma once

// FORWARD BEGIN
class SceneNode3D;
// FORWARD END

class ISceneNodeProvider
{
public:
	virtual void CreateInsances(std::weak_ptr<SceneNode3D> _parent) = 0;
};