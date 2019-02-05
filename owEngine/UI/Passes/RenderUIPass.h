#pragma once

class RenderUIEventArgs;
class Scene3D;
class SceneNode3D;
class SceneNodeUI;
class IMesh;

class IRenderUIPass : public Object
{
public:
	virtual void SetEnabled(bool enabled) = 0;
	virtual bool IsEnabled() const = 0;

	virtual void RenderUI(RenderUIEventArgs& e) = 0;
};