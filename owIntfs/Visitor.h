#pragma once

// FORWARD BEGIN
class Scene;
class SceneNode3D;
class SceneNodeUI;
class IMesh;
// FORWARD END

class IVisitor
{
public:
	virtual bool Visit(SceneNode3D& node) = 0;
	virtual bool Visit(IMesh& mesh) = 0;
};