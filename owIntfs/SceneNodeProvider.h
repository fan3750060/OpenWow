#pragma once

// FORWARD BEGIN
class SceneNode;
// FORWARD END

class ISceneNodeProvider
{
public:
	virtual void CreateInsances(std::weak_ptr<SceneNode> _parent) = 0;
};