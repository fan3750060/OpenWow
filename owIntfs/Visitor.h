#pragma once

// FORWARD BEGIN
class Scene;
class SceneNode;
class IMesh;
// FORWARD END

class IVisitor
{
public:
	virtual void Visit(SceneNode& node) = 0;
	virtual void Visit(IMesh& mesh) = 0;
};