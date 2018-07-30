#pragma once

#include "Scene.h"

class Mesh;
class Texture;
class SceneNode;

// A model base class.
// Implements a basic model loader using Assimp.
class SceneBase : public Scene
{
public:
	SceneBase();
	virtual ~SceneBase();

	virtual std::shared_ptr<SceneNode> GetRootNode() const;

	virtual std::shared_ptr<SceneNode> CreateSceneNode(std::shared_ptr<SceneNode> parent = nullptr);

	virtual void ImportSceneNode(std::shared_ptr<SceneNode> parent, std::shared_ptr<Mesh> _mesh);

	virtual void Render(RenderEventArgs& renderArgs);

	virtual void Accept(IVisitor& visitor);

private:
	std::shared_ptr<SceneNode> m_pRootNode;
};
