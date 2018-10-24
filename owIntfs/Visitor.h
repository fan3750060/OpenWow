#pragma once

// FORWARD BEGIN
class Scene;
class SceneNode;
class Mesh;
// FORWARD END

struct IVisitor
{
public:
	virtual void Visit(SceneNode& node) = 0;
	virtual void Visit(Mesh& mesh) = 0;
};