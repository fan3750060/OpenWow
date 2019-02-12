#pragma once

// FORWARD BEGIN
class SceneNode3D;
class SceneNodeUI;
class IMesh;
class CLight3D;
// FORWARD END

class IVisitor
{
public:
	virtual bool Visit(SceneNode3D& node) = 0;
	virtual bool Visit(IMesh& mesh) = 0;
	virtual bool Visit(CLight3D& light) = 0;
};