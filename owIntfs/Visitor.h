#pragma once

class Scene;
class SceneNode;
class Mesh;

struct IVisitor
{
public:
	virtual void Visit(Scene& scene) = 0;
	virtual void Visit(SceneNode& node) = 0;
	virtual void Visit(Mesh& mesh) = 0;
};