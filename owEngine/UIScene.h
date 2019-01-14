#pragma once

class IMesh;
class UINode;
class RenderEventArgs;
class IVisitor;

class UIScene : public Object
{
public:
	virtual std::shared_ptr<UINode> GetRootNode() const = 0;

	virtual std::shared_ptr<UINode> CreateSceneNode(std::shared_ptr<UINode> parent = nullptr) = 0;

	virtual void ImportSceneNode(std::shared_ptr<UINode> parent, std::shared_ptr<IMesh> _mesh) = 0;

	virtual void Accept(IVisitor& visitor) = 0;
};