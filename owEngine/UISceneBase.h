#pragma once

#include "UIScene.h"

class IMesh;
class Texture;
class UINode;

// A model base class.
// Implements a basic model loader using Assimp.
class UISceneBase : public UIScene
{
public:
	UISceneBase();
	virtual ~UISceneBase();

	virtual std::shared_ptr<UINode> GetRootNode() const;

	virtual std::shared_ptr<UINode> CreateSceneNode(std::shared_ptr<UINode> parent = nullptr);

	virtual void ImportSceneNode(std::shared_ptr<UINode> parent, std::shared_ptr<IMesh> _mesh);

	virtual void Accept(IVisitor& visitor);

private:
	std::shared_ptr<UINode> m_pRootNode;
};
