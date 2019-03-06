#include "stdafx.h"

// General
#include "UIWindow.h"


CUIWindowNode::CUIWindowNode()
{
}

CUIWindowNode::~CUIWindowNode()
{
	m_Children.clear();
}



//
// Parent & childs functional
//
void CUIWindowNode::AddChild(std::shared_ptr<CUIBaseNode> pNode)
{
	if (pNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), pNode);
		if (iter == m_Children.end())
		{
			pNode->SetParentInternal(weak_from_this());

			m_Children.push_back(pNode);
			if (!pNode->GetName().empty())
			{
				m_ChildrenByName.insert(NodeNameMap::value_type(pNode->GetName(), pNode));
			}
		}
	}
}

void CUIWindowNode::RemoveChild(std::shared_ptr<CUIBaseNode> pNode)
{
	if (pNode == nullptr)
	{
		return;
	}

	NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), pNode);
	if (iter != m_Children.end())
	{
		pNode->SetParent(std::weak_ptr<CUIWindowNode>());

		m_Children.erase(iter);

		// Also remove it from the name map.
		NodeNameMap::iterator iter2 = m_ChildrenByName.find(pNode->GetName());
		if (iter2 != m_ChildrenByName.end())
		{
			m_ChildrenByName.erase(iter2);
		}
	}
}



//
// CUIBaseNode
//
bool CUIWindowNode::Accept(IVisitor& visitor)
{
	bool visitResult = base::Accept(visitor);
	if (!visitResult)
		return false;

	for (auto child : m_Children)
	{
		child->Accept(visitor);
	}

	return true;
}



//
// Input events
//
bool CUIWindowNode::OnKeyPressed(KeyEventArgs & e)
{
	// Childs
	bool result = false;
	for (auto it : m_Children)
	{
		if (it->OnKeyPressed(e))
			result = true;
	}
	return result;
}

void CUIWindowNode::OnKeyReleased(KeyEventArgs & e)
{
	for (auto it : m_Children)
	{
		it->OnKeyPressed(e);
	}
}

void CUIWindowNode::OnMouseMoved(MouseMotionEventArgs& e)
{
	for (auto it : m_Children)
	{
		MouseMotionEventArgs eInner(e);
		e.X -= static_cast<int>(GetTranslation().x);
		e.Y -= static_cast<int>(GetTranslation().y);

		it->OnMouseMoved(eInner);

		// Synteric events impl
		if (it->IsPointInBounds(eInner.GetPoint()))
		{
			if (!it->IsMouseOnNode())
			{
				it->OnMouseEntered();
				it->DoMouseEntered();
			}
		}
		else
		{
			if (it->IsMouseOnNode())
			{
				it->OnMouseLeaved();
				it->DoMouseLeaved();
			}
		}
	}
}

bool CUIWindowNode::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	bool result = false;
	for (auto it : m_Children)
	{
		MouseButtonEventArgs eInner(e);
		e.X -= static_cast<int>(GetTranslation().x);
		e.Y -= static_cast<int>(GetTranslation().y);

		if (it->IsPointInBounds(eInner.GetPoint()))
			if (it->OnMouseButtonPressed(eInner))
				result = true;
	}
	return result;
}

void CUIWindowNode::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	for (auto it : m_Children)
	{
		MouseButtonEventArgs eInner(e);
		e.X -= static_cast<int>(GetTranslation().x);
		e.Y -= static_cast<int>(GetTranslation().y);

		it->OnMouseButtonReleased(eInner);
	}
}

bool CUIWindowNode::OnMouseWheel(MouseWheelEventArgs & e)
{
	bool result = false;
	for (auto it : m_Children)
	{
		MouseWheelEventArgs eInner(e);
		e.X -= static_cast<int>(GetTranslation().x);
		e.Y -= static_cast<int>(GetTranslation().y);

		if (it->IsPointInBounds(eInner.GetPoint()))
			if (it->OnMouseWheel(eInner))
				result = true;
	}
	return result;
}
