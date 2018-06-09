#pragma once

#include "SceneNode.h"

class CSceneManager : CRenderable3DObject
{
public:
	CSceneManager();
	~CSceneManager();

	void SetRootNode(SceneNode* _rootNode) { m_RootNode = _rootNode; }

	// Recurcy
	void RenderRecursy(SceneNode* _node);

	// IRenderable3D
	void PreRender3D() override;
	void Render3D() override;
	void PostRender3D() override;

private:
	SceneNode* m_RootNode;

};