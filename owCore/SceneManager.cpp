#include "stdafx.h"

// General
#include "SceneManager.h"

CSceneManager::CSceneManager()
{
	_Bindings->RegisterRenderable3DObject(this, 15);
}

CSceneManager::~CSceneManager()
{
	_Bindings->UnregisterRenderable3DObject(this);
}

void CSceneManager::RenderRecursy(SceneNode* _node)
{
	if (_node == nullptr)
	{
		return;
	}

	// CurrentNode
	_node->PreRender3D();
	if (!(_node->IsVisible()))
	{
		return;
	}
	_node->Render3D();

	// Some checks
	if (_node->getChilds().empty())
	{
		return;
	}
	std::sort(_node->getChilds().begin(), _node->getChilds().end(), Renderable3DObjectCompare());

	// Childs
	for (auto& it : _node->getChilds())
	{
		RenderRecursy(it);
	}
}

void CSceneManager::PreRender3D()
{
	SetVisible(true);
}

void CSceneManager::Render3D()
{
	if (m_RootNode != nullptr)
	{
		RenderRecursy(m_RootNode);
	}
}

void CSceneManager::PostRender3D()
{
}
