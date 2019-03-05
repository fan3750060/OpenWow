#include "stdafx.h"

// General
#include "UIBaseNode.h"

// Additional
#include "Application.h"

CUIBaseNode::CUIBaseNode()
	: m_Name("CUIBaseNode")
	, m_Translate(vec2())
	, m_Rotate(vec3())
	, m_Scale(1.0f, 1.0f)
{

	m_PerObjectData = (PerObject*)_aligned_malloc(sizeof(PerObject), 16);
	m_PerObjectConstantBuffer = _RenderDevice->CreateConstantBuffer(PerObject());
}

CUIBaseNode::~CUIBaseNode()
{
	m_Children.clear();

	if (m_PerObjectData)
	{
		_aligned_free(m_PerObjectData);
		_RenderDevice->DestroyConstantBuffer(m_PerObjectConstantBuffer);
	}
}

cstring CUIBaseNode::GetName() const
{
	return m_Name;
}

void CUIBaseNode::SetName(cstring name)
{
	m_Name = name;
}

// Translate
void CUIBaseNode::SetTranslate(cvec2 _translate) 
{ 
	m_Translate = _translate;

	UpdateLocalTransform();
}
cvec2 CUIBaseNode::GetTranslation() const 
{ 
	return m_Translate; 
}

// Rotate
void CUIBaseNode::SetRotation(cvec3 _rotate) 
{ 
	m_Rotate = _rotate;

	UpdateLocalTransform();
}
cvec3 CUIBaseNode::GetRotation() const 
{ 
	return m_Rotate; 
}

// Scale
void CUIBaseNode::SetScale(cvec2 _scale) 
{ 
	m_Scale = _scale;

	UpdateLocalTransform();
}
cvec2 CUIBaseNode::GetScale() const 
{ 
	return m_Scale; 
}

// Local transform

mat4 CUIBaseNode::GetLocalTransform() const
{
	return m_LocalTransform;
}

void CUIBaseNode::SetLocalTransform(cmat4 localTransform)
{
	m_LocalTransform = localTransform;

	UpdateWorldTransform();
}

// World transform

mat4 CUIBaseNode::GetWorldTransfom() const
{
	return m_WorldTransform;
}

void CUIBaseNode::SetWorldTransform(cmat4 worldTransform)
{
	mat4 inverseParentTransform = glm::inverse(GetParentWorldTransform());
	SetLocalTransform(inverseParentTransform * worldTransform);
}


mat4 CUIBaseNode::GetParentWorldTransform() const
{
	mat4 parentTransform(1.0f);
	if (std::shared_ptr<CUIBaseNode> parent = m_pParentNode.lock())
	{
		parentTransform = parent->GetWorldTransfom();
	}

	return parentTransform;
}

//

void CUIBaseNode::UpdateLocalTransform()
{
	m_LocalTransform = mat4();

	m_LocalTransform = glm::translate(m_LocalTransform, vec3(m_Translate, 0.0f));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.x, vec3(1, 0, 0));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.y, vec3(0, 1, 0));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.z, vec3(0, 0, 1));
	m_LocalTransform = glm::scale(m_LocalTransform, vec3(m_Scale, 1.0f));

	UpdateWorldTransform();
}

void CUIBaseNode::UpdateWorldTransform()
{
	m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
}

void CUIBaseNode::AddChild(std::shared_ptr<CUIBaseNode> pNode)
{
	if (pNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), pNode);
		if (iter == m_Children.end())
		{
			pNode->m_pParentNode = shared_from_this();
			pNode->UpdateWorldTransform();

			m_Children.push_back(pNode);
			if (!pNode->GetName().empty())
			{
				m_ChildrenByName.insert(NodeNameMap::value_type(pNode->GetName(), pNode));
			}
		}
	}
}

void CUIBaseNode::RemoveChild(std::shared_ptr<CUIBaseNode> pNode)
{
	if (pNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), pNode);
		if (iter != m_Children.end())
		{
			pNode->SetParent(std::weak_ptr<CUIBaseNode>());
			pNode->UpdateWorldTransform();

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

void CUIBaseNode::SetParent(std::weak_ptr<CUIBaseNode> wpNode)
{
	std::shared_ptr<CUIBaseNode> me = shared_from_this();

	if (std::shared_ptr<CUIBaseNode> parent = wpNode.lock())
	{
		parent->AddChild(me);
	}
	else if (parent = m_pParentNode.lock())
	{
		parent->RemoveChild(me);
		m_pParentNode.reset();
	}
}

void CUIBaseNode::SetMesh(std::shared_ptr<IMesh> mesh)
{
	assert1(mesh != nullptr);
	m_Mesh = mesh;
}

std::shared_ptr<IMesh> CUIBaseNode::GetMesh() const
{
	return m_Mesh;
}

void CUIBaseNode::UpdateViewport(const Viewport* viewport)
{
	// Do nothing...
}

bool CUIBaseNode::Render(RenderUIEventArgs& renderEventArgs)
{
	const Viewport* viewport = renderEventArgs.Viewport;
	assert1(viewport != nullptr);

	PerObject perObjectData;
	perObjectData.Model               = GetWorldTransfom();
	perObjectData.ModelView           = mat4(1.0f);
	perObjectData.ModelViewProjection = viewport->OrthoMatrix * perObjectData.Model;
	m_PerObjectConstantBuffer->Set(perObjectData);

	UpdateViewport(viewport);

	RenderMesh(renderEventArgs);

	for (auto child : m_Children)
	{
		child->Render(renderEventArgs);
	}

	return true;
}

bool CUIBaseNode::RenderMesh(RenderUIEventArgs& renderEventArgs)
{
	if (GetMesh() != nullptr)
	{
		return GetMesh()->Render(renderEventArgs, m_PerObjectConstantBuffer);
	}

	return false;
}



/**
 * Input events
 */
bool CUIBaseNode::OnKeyPressed(KeyEventArgs & e)
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

bool CUIBaseNode::OnKeyReleased(KeyEventArgs & e)
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

void CUIBaseNode::OnMouseMoved(MouseMotionEventArgs & e)
{
	// Childs
	for (auto it : m_Children)
	{
		it->OnMouseMoved(e);
	}
}

bool CUIBaseNode::OnMouseButtonPressed(MouseButtonEventArgs & e)
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

bool CUIBaseNode::OnMouseButtonReleased(MouseButtonEventArgs & e)
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

bool CUIBaseNode::OnMouseWheel(MouseWheelEventArgs & e)
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
