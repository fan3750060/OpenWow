#pragma once

#include "SceneNode.h"

class CSceneManager : public ISceneManager, public CRenderable3DObject
{
public:
	CSceneManager(SceneNode* _rootNode);
	~CSceneManager();

	void RenderRecursy(SceneNode* _node);
	void Intersection(SceneNode* _node);

	// ISceneManager
	SceneNode* getIntersectedNode() override { return m_IntersectedNode; }
	string getIntersectedNodeInfo() { return m_IntersectedNode->getObjectInfo(); }

	void setCamera(Camera* _camera) override { m_MainCamera = _camera; }
	Camera* getCamera() override { return m_MainCamera; }
	void setFrustrumCamera(Camera* _camera) override { m_FrustrumCamera = _camera; }
	Camera* getFrustrumCamera() override { return m_FrustrumCamera; }

	// IRenderable3D
	void PreRender3D() override;
	void Render3D() override;
	void PostRender3D() override;

private:
	SceneNode*				m_RootNode;
	SceneNode*				m_IntersectedNode;

	// Cameras and frustrum
	Camera*					m_MainCamera;
	Camera*					m_FrustrumCamera;
	
	vector<SceneNode*>		m_RenderQueue;

private: // Settings
	IEngine*				m_Engine;
	const CGroupVideo&		m_Video;

};