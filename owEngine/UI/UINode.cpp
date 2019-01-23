#include "stdafx.h"

// General
#include "UINode.h"

UINode::UINode()
	: m_Name("UINode")
	, m_Scale(1.0f, 1.0f)
	, m_IsLocalDirty(true)
	, m_IsWorldDirty(true)
{
}

UINode::~UINode()
{
	m_Children.clear();
}

cstring UINode::GetName() const
{
	return m_Name;
}

void UINode::SetName(cstring name)
{
	m_Name = name;
}

// Translate
void UINode::SetTranslate(cvec2 _translate) 
{ 
	m_Translate = _translate;
	m_IsLocalDirty = true;
	m_IsWorldDirty = true;
}
cvec2 UINode::GetTranslation() const 
{ 
	return m_Translate; 
}

// Rotate
void UINode::SetRotation(cvec3 _rotate) 
{ 
	m_Rotate = _rotate;
	m_IsLocalDirty = true;
	m_IsWorldDirty = true;
}
cvec3 UINode::GetRotation() const 
{ 
	return m_Rotate; 
}

// Scale
void UINode::SetScale(cvec2 _scale) 
{ 
	m_Scale = _scale;
	m_IsLocalDirty = true;
	m_IsWorldDirty = true;
}
cvec2 UINode::GetScale() const 
{ 
	return m_Scale; 
}

bool UINode::IsDirty() const
{
	return m_IsLocalDirty || m_IsWorldDirty;
}

// Local transform

mat4 UINode::GetLocalTransform()
{
	UpdateLocalTransform();
	return m_LocalTransform;
}

void UINode::SetLocalTransform(cmat4 localTransform)
{
	m_LocalTransform = localTransform;
	m_IsLocalDirty = true;
	m_IsWorldDirty = true;
}

// World transform

mat4 UINode::GetWorldTransfom()
{
	UpdateWorldTransform();
	return m_WorldTransform;
}

void UINode::SetWorldTransform(cmat4 worldTransform)
{
	mat4 inverseParentTransform = glm::inverse(GetParentWorldTransform());
	SetLocalTransform(inverseParentTransform * worldTransform);
}


mat4 UINode::GetParentWorldTransform() const
{
	mat4 parentTransform(1.0f);
	if (std::shared_ptr<UINode> parent = m_pParentNode.lock())
	{
		parentTransform = parent->GetWorldTransfom();
	}

	return parentTransform;
}

//

void UINode::UpdateLocalTransform()
{
	if (m_IsLocalDirty)
	{
		m_LocalTransform = mat4();

		m_LocalTransform = glm::translate(m_LocalTransform, vec3(m_Translate, 0.0f));
		m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.x, vec3(1, 0, 0));
		m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.y, vec3(0, 1, 0));
		m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.z, vec3(0, 0, 1));
		m_LocalTransform = glm::scale(m_LocalTransform, vec3(m_Scale, 1.0f));
		m_IsLocalDirty = false;
		m_IsWorldDirty = true;
	}
}

void UINode::UpdateWorldTransform()
{
	if (m_IsWorldDirty)
	{
		m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
		m_IsWorldDirty = false;
	}
}

void UINode::SetLocalUnderty()
{
	m_IsLocalDirty = false;
}

void UINode::AddChild(std::shared_ptr<UINode> pNode)
{
	if (pNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), pNode);
		if (iter == m_Children.end())
		{
			pNode->m_pParentNode = shared_from_this();
			pNode->m_IsWorldDirty = true;

			m_Children.push_back(pNode);
			if (!pNode->GetName().empty())
			{
				m_ChildrenByName.insert(NodeNameMap::value_type(pNode->GetName(), pNode));
			}
		}
	}
}

void UINode::RemoveChild(std::shared_ptr<UINode> pNode)
{
	if (pNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), pNode);
		if (iter != m_Children.end())
		{
			pNode->SetParent(std::weak_ptr<UINode>());

			m_Children.erase(iter);

			// Also remove it from the name map.
			NodeNameMap::iterator iter2 = m_ChildrenByName.find(pNode->GetName());
			if (iter2 != m_ChildrenByName.end())
			{
				m_ChildrenByName.erase(iter2);
			}
		}
		else
		{
			// Maybe this node appears lower in the hierarchy...
			for (auto child : m_Children)
			{
				child->RemoveChild(pNode);
			}
		}
	}
}

void UINode::SetParent(std::weak_ptr<UINode> wpNode)
{
	std::shared_ptr<UINode> me = shared_from_this();

	if (std::shared_ptr<UINode> parent = wpNode.lock())
	{
		parent->AddChild(me);
	}
	else if (parent = m_pParentNode.lock())
	{
		parent->RemoveChild(me);
		m_pParentNode.reset();
	}
}

void UINode::SetMesh(std::shared_ptr<IMesh> mesh)
{
	assert1(mesh != nullptr);
	m_Mesh = mesh;
}

std::shared_ptr<IMesh> UINode::GetMesh() const
{
	return m_Mesh;
}

void UINode::UpdateViewport(const Viewport* viewport)
{
	// Do nothing...
}

bool UINode::Accept(IVisitor& visitor)
{
	bool visitResult = visitor.Visit(*this);
	if (!visitResult)
		return false;

	if (m_Mesh != nullptr)
	{
		m_Mesh->Accept(visitor);
	}

	for (auto child : m_Children)
	{
		child->Accept(visitor);
	}

	return visitResult;
}

bool UINode::OnKeyPressed(KeyEventArgs & e)
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

bool UINode::OnKeyReleased(KeyEventArgs & e)
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

void UINode::OnMouseMoved(MouseMotionEventArgs & e)
{
	// Childs
	for (auto it : m_Children)
	{
		it->OnMouseMoved(e);
	}
}

bool UINode::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	// Childs
	bool result = false;
	for (auto it : m_Children)
	{
		if (it->OnMouseButtonPressed(e))
			result = true;
	}
	return result;
}

bool UINode::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	// Childs
	bool result = false;
	for (auto it : m_Children)
	{
		if (it->OnMouseButtonReleased(e))
			result = true;
	}
	return result;
}

bool UINode::OnMouseWheel(MouseWheelEventArgs & e)
{
	// Childs
	bool result = false;
	for (auto it : m_Children)
	{
		if (it->OnMouseWheel(e))
			result = true;
	}
	return result;
}
