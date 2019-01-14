#pragma once

// FORWARD BEGIN
class Scene;
class SceneNode;
class UINode;
class IMesh;
// FORWARD END

class IVisitor
{
public:
	virtual bool Visit(SceneNode& node) = 0;
	virtual bool Visit(UINode& node) = 0;
	virtual bool Visit(IMesh& mesh) = 0;
};