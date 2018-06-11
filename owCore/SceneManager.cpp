#include "stdafx.h"

// General
#include "SceneManager.h"

CSceneManager::CSceneManager() :
	m_RootNode(nullptr),
	m_IntersectedNode(nullptr)
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
	m_RenderQueue.push_back(_node);

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
	m_RenderQueue.clear();

	if (m_RootNode != nullptr)
	{
		RenderRecursy(m_RootNode);
	}

	std::sort(m_RenderQueue.begin(), m_RenderQueue.end(), Renderable3DObjectCompare());
	for (auto& it : m_RenderQueue)
	{
		it->Render3D();
	}
}

void CSceneManager::PostRender3D()
{
}
