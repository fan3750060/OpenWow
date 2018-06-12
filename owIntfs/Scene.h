#pragma once

#include "Refs.h"

/*__interface ISceneNode : public IRefItem
{
	void setParent(ISceneNode* _parent);
	ISceneNode* getParent();
	void addChild(ISceneNode* _child);
	void removeChild(ISceneNode* _child);


	cvec3 getTranslate() const;
	cvec3 getRotate() const;
	cquat getRotateQuat() const;
	cvec3 getScale() const;

	cmat4 getRelTrans() const;
	cmat4 getAbsTrans() const;
};*/

// FORWARD BEGIN
class Camera;
class SceneNode;
// FORWARD END

__interface 
	__declspec(uuid("71DC4B4D-45AB-4030-95FD-6C7C3B061526"))
	ISceneManager : public IManager
{
	SceneNode* getIntersectedNode();

	void setCamera(Camera* _camera);
	Camera* getCamera();
	void setFrustrumCamera(Camera* _camera);
	Camera* getFrustrumCamera();
};