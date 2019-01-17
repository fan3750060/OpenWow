#pragma once

class IMesh;
class SceneNode;
class RenderEventArgs;
class IVisitor;

class Scene : public Object
{
public:
	virtual std::shared_ptr<SceneNode> GetRootNode() const = 0;

	virtual void Accept(IVisitor& visitor) = 0;

	virtual void OnUpdate(UpdateEventArgs& e) = 0;
};