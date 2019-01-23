#pragma once

#include "Scene.h"

class IMesh;
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

	virtual void Accept(IVisitor& visitor);

	void OnUpdate(UpdateEventArgs& e);

private:
	std::shared_ptr<SceneNode> m_pRootNode;
};
