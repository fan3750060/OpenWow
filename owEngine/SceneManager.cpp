#include "stdafx.h"

// General
#include "SceneManager.h"

CSceneManager::CSceneManager(SceneNode* _rootNode) :
	m_RootNode(_rootNode),
	m_IntersectedNode(nullptr),
	m_MainCamera(nullptr),
	m_FrustrumCamera(nullptr),
	m_RenderQueueDebug(false),
	m_Engine(GetManager<IEngine>()),
	m_Video(GetSettingsGroup<CGroupVideo>())
{
	AddManager<ISceneManager>(this);

	_Bindings->RegisterRenderable3DObject(this, 15);
}

CSceneManager::~CSceneManager()
{
	DelManager<ISceneManager>();

	_Bindings->UnregisterRenderable3DObject(this);
}

void CSceneManager::RenderRecursy(SceneNode* _node)
{
	if (_node == nullptr)
	{
		return;
	}

	// CurrentNode
	bool isNodeVisible = _node->PreRender3D();
	_node->setVisible(isNodeVisible);
	if (!isNodeVisible)
	{
		return;
	}
	m_RenderQueue.push_back(_node);

	//Intersection(_node);

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

void CSceneManager::Intersection(SceneNode * _node)
{
	if (!(_node->isSelectable()))
	{
		return;
	}

	vec3 dir = screenToWord
	(
		_Render->getAdapter()->GetInput()->GetMouse(),
		m_Video.GetWindowSize(),
		_Render->getCamera()->getProjMat(),
		_Render->getCamera()->getViewMat()
	);

	bool result = rayAABBIntersection(getCamera()->Position, dir * 100000.0f, _node->getBounds().getMin(), _node->getBounds().getMax());
	if (result)
	{
		// First node
		if (m_IntersectedNode == nullptr)
		{
			m_IntersectedNode = _node;
		}

		if (m_IntersectedNode == _node->getParent())
		{
			m_IntersectedNode = _node;
		}
		else if (m_IntersectedNode->getDrawOrder() == _node->getDrawOrder())
		{
			float distToOld = glm::length(getCamera()->Position - m_IntersectedNode->getBounds().getCenter()) - m_IntersectedNode->getBounds().getRadius();
			float distToNew = glm::length(getCamera()->Position - _node->getBounds().getCenter()) - _node->getBounds().getRadius();
			//float old = nearestDistToAABB(getCamera()->Position, m_IntersectedNode->getBounds().Min, m_IntersectedNode->getBounds().Max);
			//float neww = nearestDistToAABB(getCamera()->Position, _node->getBounds().Min, _node->getBounds().Max);
			if (m_IntersectedNode->getBounds().isPointInside(_node->getBounds().getCenter()))
			{
				m_IntersectedNode = _node;
			}
			else if (distToOld > distToNew)
			{
				m_IntersectedNode = _node;
			}
		}
		else if (m_IntersectedNode->getDrawOrder() < _node->getDrawOrder())
		{
			m_IntersectedNode = _node;
		}
	}
}

bool CSceneManager::PreRender3D()
{
	return true;
}

void CSceneManager::Render3D()
{
	m_RenderQueue.clear();

	if (m_RootNode != nullptr)
	{
		m_IntersectedNode = nullptr;
		RenderRecursy(m_RootNode);
	}

	if (m_RenderQueue.empty())
	{
		return;
	}

	if (!m_RenderQueueDebug)
	{
		std::sort(m_RenderQueue.begin(), m_RenderQueue.end(), SceneNodeCompare(getCamera()));
	}

	for (auto& it : m_RenderQueue)
	{
		if (m_RenderQueueDebug)
		{
			//_Render->DrawBoundingBox(it->getBounds(), it->getDebugColor());
			it->RenderDebug3D();
			continue;
		}

		it->Render3D();
	}
}

void CSceneManager::PostRender3D()
{
}
